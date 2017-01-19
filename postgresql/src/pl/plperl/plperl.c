/**********************************************************************
 * plperl.c - perl as a procedural language for PostgreSQL
 *
 * IDENTIFICATION
 *
 *	  This software is copyrighted by Mark Hollomon
 *	 but is shameless cribbed from pltcl.c by Jan Weick.
 *
 *	  The author hereby grants permission  to  use,  copy,	modify,
 *	  distribute,  and	license this software and its documentation
 *	  for any purpose, provided that existing copyright notices are
 *	  retained	in	all  copies  and  that	this notice is included
 *	  verbatim in any distributions. No written agreement, license,
 *	  or  royalty  fee	is required for any of the authorized uses.
 *	  Modifications to this software may be  copyrighted  by  their
 *	  author  and  need  not  follow  the licensing terms described
 *	  here, provided that the new terms are  clearly  indicated  on
 *	  the first page of each file where they apply.
 *
 *	  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY
 *	  PARTY  FOR  DIRECT,	INDIRECT,	SPECIAL,   INCIDENTAL,	 OR
 *	  CONSEQUENTIAL   DAMAGES  ARISING	OUT  OF  THE  USE  OF  THIS
 *	  SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF, EVEN
 *	  IF  THE  AUTHOR  HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
 *	  DAMAGE.
 *
 *	  THE  AUTHOR  AND	DISTRIBUTORS  SPECIFICALLY	 DISCLAIM	ANY
 *	  WARRANTIES,  INCLUDING,  BUT	NOT  LIMITED  TO,  THE	IMPLIED
 *	  WARRANTIES  OF  MERCHANTABILITY,	FITNESS  FOR  A  PARTICULAR
 *	  PURPOSE,	AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON
 *	  AN "AS IS" BASIS, AND THE AUTHOR	AND  DISTRIBUTORS  HAVE  NO
 *	  OBLIGATION   TO	PROVIDE   MAINTENANCE,	 SUPPORT,  UPDATES,
 *	  ENHANCEMENTS, OR MODIFICATIONS.
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/pl/plperl/plperl.c,v 1.2 2004/07/05 08:56:18 SuLiang Exp $
 *
 **********************************************************************/

#include "postgres.h"

/* system stuff */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <setjmp.h>

/* postgreSQL stuff */
#include "executor/spi.h"
#include "commands/trigger.h"
#include "utils/elog.h"
#include "fmgr.h"
#include "access/heapam.h"

#include "tcop/tcopprot.h"
#include "utils/syscache.h"
#include "catalog/pg_language.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"

/* perl stuff */
/*
 * Evil Code Alert
 *
 * both posgreSQL and perl try to do 'the right thing'
 * and provide union semun if the platform doesn't define
 * it in a system header.
 * psql uses HAVE_UNION_SEMUN
 * perl uses HAS_UNION_SEMUN
 * together, they cause compile errors.
 * If we need it, the psql headers above will provide it.
 * So we tell perl that we have it.
 */
#ifndef HAS_UNION_SEMUN
#define HAS_UNION_SEMUN
#endif

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

/* just in case these symbols aren't provided */
#ifndef pTHX_
#define pTHX_
#define pTHX void
#endif


/**********************************************************************
 * The information we cache about loaded procedures
 **********************************************************************/
typedef struct plperl_proc_desc
{
	char	   *proname;
	TransactionId fn_xmin;
	CommandId	fn_cmin;
	bool		lanpltrusted;
	FmgrInfo	result_in_func;
	Oid			result_in_elem;
	int			nargs;
	FmgrInfo	arg_out_func[FUNC_MAX_ARGS];
	Oid			arg_out_elem[FUNC_MAX_ARGS];
	int			arg_is_rel[FUNC_MAX_ARGS];
	SV		   *reference;
}	plperl_proc_desc;


/**********************************************************************
 * Global data
 **********************************************************************/
static int	plperl_firstcall = 1;
static int	plperl_call_level = 0;
static int	plperl_restart_in_progress = 0;
static PerlInterpreter *plperl_interp = NULL;
static HV  *plperl_proc_hash = NULL;

/**********************************************************************
 * Forward declarations
 **********************************************************************/
static void plperl_init_all(void);
static void plperl_init_interp(void);

Datum		plperl_call_handler(PG_FUNCTION_ARGS);

static Datum plperl_func_handler(PG_FUNCTION_ARGS);

static plperl_proc_desc *compile_plperl_function(Oid fn_oid, bool is_trigger);

static SV  *plperl_build_tuple_argument(HeapTuple tuple, TupleDesc tupdesc);
static void plperl_init_shared_libs(pTHX);


/*
 * This routine is a crock, and so is everyplace that calls it.  The problem
 * is that the cached form of plperl functions/queries is allocated permanently
 * (mostly via malloc()) and never released until backend exit.  Subsidiary
 * data structures such as fmgr info records therefore must live forever
 * as well.  A better implementation would store all this stuff in a per-
 * function memory context that could be reclaimed at need.  In the meantime,
 * fmgr_info_cxt must be called specifying TopMemoryContext so that whatever
 * it might allocate, and whatever the eventual function might allocate using
 * fn_mcxt, will live forever too.
 */
static void
perm_fmgr_info(Oid functionId, FmgrInfo *finfo)
{
	fmgr_info_cxt(functionId, finfo, TopMemoryContext);
}

/**********************************************************************
 * plperl_init_all()		- Initialize all
 **********************************************************************/
static void
plperl_init_all(void)
{

	/************************************************************
	 * Do initialization only once
	 ************************************************************/
	if (!plperl_firstcall)
		return;


	/************************************************************
	 * Destroy the existing Perl interpreter
	 ************************************************************/
	if (plperl_interp != NULL)
	{
		perl_destruct(plperl_interp);
		perl_free(plperl_interp);
		plperl_interp = NULL;
	}

	/************************************************************
	 * Free the proc hash table
	 ************************************************************/
	if (plperl_proc_hash != NULL)
	{
		hv_undef(plperl_proc_hash);
		SvREFCNT_dec((SV *) plperl_proc_hash);
		plperl_proc_hash = NULL;
	}

	/************************************************************
	 * Now recreate a new Perl interpreter
	 ************************************************************/
	plperl_init_interp();

	plperl_firstcall = 0;
}


/**********************************************************************
 * plperl_init_interp() - Create the Perl interpreter
 **********************************************************************/
static void
plperl_init_interp(void)
{

	char	   *embedding[3] = {
		"", "-e",

		/*
		 * no commas between the next 5 please. They are supposed to be
		 * one string
		 */
		"require Safe; SPI::bootstrap();"
		"sub ::mksafefunc { my $x = new Safe; $x->permit_only(':default');$x->permit(':base_math');"
		"$x->share(qw[&elog &DEBUG &NOTICE &ERROR]);"
		" return $x->reval(qq[sub { $_[0] }]); }"
		"sub ::mkunsafefunc {return eval(qq[ sub { $_[0] } ]); }"
	};

	plperl_interp = perl_alloc();
	if (!plperl_interp)
		elog(ERROR, "plperl_init_interp(): could not allocate perl interpreter");

	perl_construct(plperl_interp);
	perl_parse(plperl_interp, plperl_init_shared_libs, 3, embedding, NULL);
	perl_run(plperl_interp);



	/************************************************************
	 * Initialize the proc and query hash tables
	 ************************************************************/
	plperl_proc_hash = newHV();

}



/**********************************************************************
 * plperl_call_handler		- This is the only visible function
 *				  of the PL interpreter. The PostgreSQL
 *				  function manager and trigger manager
 *				  call this function for execution of
 *				  perl procedures.
 **********************************************************************/
PG_FUNCTION_INFO_V1(plperl_call_handler);

/* keep non-static */
Datum
plperl_call_handler(PG_FUNCTION_ARGS)
{
	Datum		retval;

	/************************************************************
	 * Initialize interpreters on first call
	 ************************************************************/
	if (plperl_firstcall)
		plperl_init_all();

	/************************************************************
	 * Connect to SPI manager
	 ************************************************************/
	if (SPI_connect() != SPI_OK_CONNECT)
		elog(ERROR, "plperl: cannot connect to SPI manager");
	/************************************************************
	 * Keep track about the nesting of Perl-SPI-Perl-... calls
	 ************************************************************/
	plperl_call_level++;

	/************************************************************
	 * Determine if called as function or trigger and
	 * call appropriate subhandler
	 ************************************************************/
	if (CALLED_AS_TRIGGER(fcinfo))
	{
		elog(ERROR, "plperl: can't use perl in triggers yet.");

		/*
		 * retval = PointerGetDatum(plperl_trigger_handler(fcinfo));
		 */
		/* make the compiler happy */
		retval = (Datum) 0;
	}
	else
		retval = plperl_func_handler(fcinfo);

	plperl_call_level--;

	return retval;
}


/**********************************************************************
 * plperl_create_sub()		- calls the perl interpreter to
 *		create the anonymous subroutine whose text is in the SV.
 *		Returns the SV containing the RV to the closure.
 **********************************************************************/
static
SV *
plperl_create_sub(char *s, bool trusted)
{
	dSP;

	SV		   *subref = NULL;
	int			count;

	ENTER;
	SAVETMPS;
	PUSHMARK(SP);
	XPUSHs(sv_2mortal(newSVpv(s, 0)));
	PUTBACK;
	count = perl_call_pv((trusted ? "mksafefunc" : "mkunsafefunc"),
						 G_SCALAR | G_EVAL | G_KEEPERR);
	SPAGAIN;

	if (SvTRUE(ERRSV))
	{
		POPs;
		PUTBACK;
		FREETMPS;
		LEAVE;
		elog(ERROR, "creation of function failed: %s", SvPV(ERRSV, PL_na));
	}

	if (count != 1)
		elog(ERROR, "creation of function failed - no return from mksafefunc");

	/*
	 * need to make a deep copy of the return. it comes off the stack as a
	 * temporary.
	 */
	subref = newSVsv(POPs);

	if (!SvROK(subref))
	{
		PUTBACK;
		FREETMPS;
		LEAVE;

		/*
		 * subref is our responsibility because it is not mortal
		 */
		SvREFCNT_dec(subref);
		elog(ERROR, "plperl_create_sub: didn't get a code ref");
	}

	PUTBACK;
	FREETMPS;
	LEAVE;
	return subref;
}

/**********************************************************************
 * plperl_init_shared_libs()		-
 *
 * We cannot use the DynaLoader directly to get at the Opcode
 * module (used by Safe.pm). So, we link Opcode into ourselves
 * and do the initialization behind perl's back.
 *
 **********************************************************************/

EXTERN_C void boot_DynaLoader(pTHX_ CV* cv);
EXTERN_C void boot_SPI(pTHX_ CV* cv);

static void
plperl_init_shared_libs(pTHX)
{
	char	   *file = __FILE__;

	newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
	newXS("SPI::bootstrap", boot_SPI, file);
}

/**********************************************************************
 * plperl_call_perl_func()		- calls a perl function through the RV
 *			stored in the prodesc structure. massages the input parms properly
 **********************************************************************/
static
SV *
plperl_call_perl_func(plperl_proc_desc * desc, FunctionCallInfo fcinfo)
{
	dSP;

	SV		   *retval;
	int			i;
	int			count;


	ENTER;
	SAVETMPS;

	PUSHMARK(sp);
	for (i = 0; i < desc->nargs; i++)
	{
		if (desc->arg_is_rel[i])
		{
			TupleTableSlot *slot = (TupleTableSlot *) fcinfo->arg[i];
			SV		   *hashref;

			Assert(slot != NULL && !fcinfo->argnull[i]);

			/*
			 * plperl_build_tuple_argument better return a mortal SV.
			 */
			hashref = plperl_build_tuple_argument(slot->val,
											  slot->ttc_tupleDescriptor);
			XPUSHs(hashref);
		}
		else
		{
			if (fcinfo->argnull[i])
				XPUSHs(&PL_sv_undef);
			else
			{
				char	   *tmp;

				tmp = DatumGetCString(FunctionCall3(&(desc->arg_out_func[i]),
													fcinfo->arg[i],
								 ObjectIdGetDatum(desc->arg_out_elem[i]),
													Int32GetDatum(-1)));
				XPUSHs(sv_2mortal(newSVpv(tmp, 0)));
				pfree(tmp);
			}
		}
	}
	PUTBACK;
	count = perl_call_sv(desc->reference, G_SCALAR | G_EVAL | G_KEEPERR);

	SPAGAIN;

	if (count != 1)
	{
		PUTBACK;
		FREETMPS;
		LEAVE;
		elog(ERROR, "plperl: didn't get a return item from function");
	}

	if (SvTRUE(ERRSV))
	{
		POPs;
		PUTBACK;
		FREETMPS;
		LEAVE;
		elog(ERROR, "plperl: error from function: %s", SvPV(ERRSV, PL_na));
	}

	retval = newSVsv(POPs);


	PUTBACK;
	FREETMPS;
	LEAVE;

	return retval;


}

/**********************************************************************
 * plperl_func_handler()		- Handler for regular function calls
 **********************************************************************/
static Datum
plperl_func_handler(PG_FUNCTION_ARGS)
{
	plperl_proc_desc *prodesc;
	SV		   *perlret;
	Datum		retval;
	sigjmp_buf	save_restart;

	/* Find or compile the function */
	prodesc = compile_plperl_function(fcinfo->flinfo->fn_oid, false);

	/* Set up error handling */
	memcpy(&save_restart, &Warn_restart, sizeof(save_restart));

	if (sigsetjmp(Warn_restart, 1) != 0)
	{
		memcpy(&Warn_restart, &save_restart, sizeof(Warn_restart));
		plperl_restart_in_progress = 1;
		if (--plperl_call_level == 0)
			plperl_restart_in_progress = 0;
		siglongjmp(Warn_restart, 1);
	}

	/************************************************************
	 * Call the Perl function
	 ************************************************************/
	perlret = plperl_call_perl_func(prodesc, fcinfo);

	/************************************************************
	 * Disconnect from SPI manager and then create the return
	 * values datum (if the input function does a palloc for it
	 * this must not be allocated in the SPI memory context
	 * because SPI_finish would free it).
	 ************************************************************/
	if (SPI_finish() != SPI_OK_FINISH)
		elog(ERROR, "plperl: SPI_finish() failed");

	if (!(perlret && SvOK(perlret)))
	{
		/* return NULL if Perl code returned undef */
		retval = (Datum) 0;
		fcinfo->isnull = true;
	}
	else
	{
		retval = FunctionCall3(&prodesc->result_in_func,
							   PointerGetDatum(SvPV(perlret, PL_na)),
							   ObjectIdGetDatum(prodesc->result_in_elem),
							   Int32GetDatum(-1));
	}

	SvREFCNT_dec(perlret);

	memcpy(&Warn_restart, &save_restart, sizeof(Warn_restart));
	if (plperl_restart_in_progress)
	{
		if (--plperl_call_level == 0)
			plperl_restart_in_progress = 0;
		siglongjmp(Warn_restart, 1);
	}

	return retval;
}


/**********************************************************************
 * compile_plperl_function	- compile (or hopefully just look up) function
 **********************************************************************/
static plperl_proc_desc *
compile_plperl_function(Oid fn_oid, bool is_trigger)
{
	HeapTuple	procTup;
	Form_pg_proc procStruct;
	char		internal_proname[64];
	int			proname_len;
	plperl_proc_desc *prodesc = NULL;
	int			i;

	/* We'll need the pg_proc tuple in any case... */
	procTup = SearchSysCache(PROCOID,
							 ObjectIdGetDatum(fn_oid),
							 0, 0, 0);
	if (!HeapTupleIsValid(procTup))
		elog(ERROR, "plperl: cache lookup for proc %u failed", fn_oid);
	procStruct = (Form_pg_proc) GETSTRUCT(procTup);

	/************************************************************
	 * Build our internal proc name from the functions Oid
	 ************************************************************/
	if (!is_trigger)
		sprintf(internal_proname, "__PLPerl_proc_%u", fn_oid);
	else
		sprintf(internal_proname, "__PLPerl_proc_%u_trigger", fn_oid);
	proname_len = strlen(internal_proname);

	/************************************************************
	 * Lookup the internal proc name in the hashtable
	 ************************************************************/
	if (hv_exists(plperl_proc_hash, internal_proname, proname_len))
	{
		bool		uptodate;

		prodesc = (plperl_proc_desc *) SvIV(*hv_fetch(plperl_proc_hash,
									  internal_proname, proname_len, 0));

		/************************************************************
		 * If it's present, must check whether it's still up to date.
		 * This is needed because CREATE OR REPLACE FUNCTION can modify the
		 * function's pg_proc entry without changing its OID.
		 ************************************************************/
		uptodate = (prodesc->fn_xmin == procTup->t_data->t_xmin &&
					prodesc->fn_cmin == procTup->t_data->t_cmin);

		if (!uptodate)
		{
			/* need we delete old entry? */
			prodesc = NULL;
		}
	}

	/************************************************************
	 * If we haven't found it in the hashtable, we analyze
	 * the functions arguments and returntype and store
	 * the in-/out-functions in the prodesc block and create
	 * a new hashtable entry for it.
	 *
	 * Then we load the procedure into the Perl interpreter.
	 ************************************************************/
	if (prodesc == NULL)
	{
		HeapTuple	langTup;
		HeapTuple	typeTup;
		Form_pg_language langStruct;
		Form_pg_type typeStruct;
		char	   *proc_source;

		/************************************************************
		 * Allocate a new procedure description block
		 ************************************************************/
		prodesc = (plperl_proc_desc *) malloc(sizeof(plperl_proc_desc));
		if (prodesc == NULL)
			elog(ERROR, "plperl: out of memory");
		MemSet(prodesc, 0, sizeof(plperl_proc_desc));
		prodesc->proname = strdup(internal_proname);
		prodesc->fn_xmin = procTup->t_data->t_xmin;
		prodesc->fn_cmin = procTup->t_data->t_cmin;

		/************************************************************
		 * Lookup the pg_language tuple by Oid
		 ************************************************************/
		langTup = SearchSysCache(LANGOID,
								 ObjectIdGetDatum(procStruct->prolang),
								 0, 0, 0);
		if (!HeapTupleIsValid(langTup))
		{
			free(prodesc->proname);
			free(prodesc);
			elog(ERROR, "plperl: cache lookup for language %u failed",
				 procStruct->prolang);
		}
		langStruct = (Form_pg_language) GETSTRUCT(langTup);
		prodesc->lanpltrusted = langStruct->lanpltrusted;
		ReleaseSysCache(langTup);

		/************************************************************
		 * Get the required information for input conversion of the
		 * return value.
		 ************************************************************/
		if (!is_trigger)
		{
			typeTup = SearchSysCache(TYPEOID,
								ObjectIdGetDatum(procStruct->prorettype),
									 0, 0, 0);
			if (!HeapTupleIsValid(typeTup))
			{
				free(prodesc->proname);
				free(prodesc);
				if (!OidIsValid(procStruct->prorettype))
					elog(ERROR, "plperl functions cannot return type \"opaque\""
						 "\n\texcept when used as triggers");
				else
					elog(ERROR, "plperl: cache lookup for return type %u failed",
						 procStruct->prorettype);
			}
			typeStruct = (Form_pg_type) GETSTRUCT(typeTup);

			if (typeStruct->typrelid != InvalidOid)
			{
				free(prodesc->proname);
				free(prodesc);
				elog(ERROR, "plperl: return types of tuples not supported yet");
			}

			perm_fmgr_info(typeStruct->typinput, &(prodesc->result_in_func));
			prodesc->result_in_elem = typeStruct->typelem;

			ReleaseSysCache(typeTup);
		}

		/************************************************************
		 * Get the required information for output conversion
		 * of all procedure arguments
		 ************************************************************/
		if (!is_trigger)
		{
			prodesc->nargs = procStruct->pronargs;
			for (i = 0; i < prodesc->nargs; i++)
			{
				typeTup = SearchSysCache(TYPEOID,
							ObjectIdGetDatum(procStruct->proargtypes[i]),
										 0, 0, 0);
				if (!HeapTupleIsValid(typeTup))
				{
					free(prodesc->proname);
					free(prodesc);
					if (!OidIsValid(procStruct->proargtypes[i]))
						elog(ERROR, "plperl functions cannot take type \"opaque\"");
					else
						elog(ERROR, "plperl: cache lookup for argument type %u failed",
							 procStruct->proargtypes[i]);
				}
				typeStruct = (Form_pg_type) GETSTRUCT(typeTup);

				if (typeStruct->typrelid != InvalidOid)
					prodesc->arg_is_rel[i] = 1;
				else
					prodesc->arg_is_rel[i] = 0;

				perm_fmgr_info(typeStruct->typoutput, &(prodesc->arg_out_func[i]));
				prodesc->arg_out_elem[i] = typeStruct->typelem;
				ReleaseSysCache(typeTup);
			}
		}

		/************************************************************
		 * create the text of the anonymous subroutine.
		 * we do not use a named subroutine so that we can call directly
		 * through the reference.
		 *
		 ************************************************************/
		proc_source = DatumGetCString(DirectFunctionCall1(textout,
								  PointerGetDatum(&procStruct->prosrc)));

		/************************************************************
		 * Create the procedure in the interpreter
		 ************************************************************/
		prodesc->reference = plperl_create_sub(proc_source, prodesc->lanpltrusted);
		pfree(proc_source);
		if (!prodesc->reference)
		{
			free(prodesc->proname);
			free(prodesc);
			elog(ERROR, "plperl: cannot create internal procedure %s",
				 internal_proname);
		}

		/************************************************************
		 * Add the proc description block to the hashtable
		 ************************************************************/
		hv_store(plperl_proc_hash, internal_proname, proname_len,
				 newSViv((IV) prodesc), 0);
	}

	ReleaseSysCache(procTup);

	return prodesc;
}


/**********************************************************************
 * plperl_build_tuple_argument() - Build a string for a ref to a hash
 *				  from all attributes of a given tuple
 **********************************************************************/
static SV  *
plperl_build_tuple_argument(HeapTuple tuple, TupleDesc tupdesc)
{
	int			i;
	SV		   *output;
	Datum		attr;
	bool		isnull;
	char	   *attname;
	char	   *outputstr;
	HeapTuple	typeTup;
	Oid			typoutput;
	Oid			typelem;

	output = sv_2mortal(newSVpv("{", 0));

	for (i = 0; i < tupdesc->natts; i++)
	{
		/************************************************************
		 * Get the attribute name
		 ************************************************************/
		attname = tupdesc->attrs[i]->attname.data;

		/************************************************************
		 * Get the attributes value
		 ************************************************************/
		attr = heap_getattr(tuple, i + 1, tupdesc, &isnull);

		/************************************************************
		 *	If it is null it will be set to undef in the hash.
		 ************************************************************/
		if (isnull)
		{
			sv_catpvf(output, "'%s' => undef,", attname);
			continue;
		}

		/************************************************************
		 * Lookup the attribute type in the syscache
		 * for the output function
		 ************************************************************/
		typeTup = SearchSysCache(TYPEOID,
						   ObjectIdGetDatum(tupdesc->attrs[i]->atttypid),
								 0, 0, 0);
		if (!HeapTupleIsValid(typeTup))
			elog(ERROR, "plperl: Cache lookup for attribute '%s' type %u failed",
				 attname, tupdesc->attrs[i]->atttypid);

		typoutput = ((Form_pg_type) GETSTRUCT(typeTup))->typoutput;
		typelem = ((Form_pg_type) GETSTRUCT(typeTup))->typelem;
		ReleaseSysCache(typeTup);

		/************************************************************
		 * Append the attribute name and the value to the list.
		 ************************************************************/
		outputstr = DatumGetCString(OidFunctionCall3(typoutput,
													 attr,
											   ObjectIdGetDatum(typelem),
						   Int32GetDatum(tupdesc->attrs[i]->atttypmod)));
		sv_catpvf(output, "'%s' => '%s',", attname, outputstr);
		pfree(outputstr);
	}

	sv_catpv(output, "}");
	output = perl_eval_pv(SvPV(output, PL_na), TRUE);
	return output;
}
