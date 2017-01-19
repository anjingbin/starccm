/**********************************************************************
 * pl_comp.c		- Compiler part of the PL/pgSQL
 *			  procedural language
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/pl/plpgsql/src/pl_comp.c,v 1.2 2004/07/05 08:56:18 SuLiang Exp $
 *
 *	  This software is copyrighted by Jan Wieck - Hamburg.
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
 **********************************************************************/

#include "plpgsql.h"

#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <setjmp.h>

#include "pl.tab.h"

#include "access/heapam.h"
#include "catalog/catname.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"
#include "catalog/pg_class.h"
#include "catalog/pg_attribute.h"
#include "catalog/pg_attrdef.h"
#include "commands/trigger.h"
#include "executor/spi.h"
#include "fmgr.h"
#include "parser/gramparse.h"
#include "parser/parse_type.h"
#include "tcop/tcopprot.h"
#include "utils/builtins.h"
#include "utils/syscache.h"


/* ----------
 * Variables in the parser that shouldn't go into plpgsql.h
 * ----------
 */
extern PLPGSQL_YYSTYPE plpgsql_yylval;

/* ----------
 * Our own local and global variables
 * ----------
 */
static int	datums_alloc;
int			plpgsql_nDatums;
PLpgSQL_datum **plpgsql_Datums;
static int	datums_last = 0;

int			plpgsql_error_lineno;
char	   *plpgsql_error_funcname;
int			plpgsql_DumpExecTree = 0;

PLpgSQL_function *plpgsql_curr_compile;


/*
 * This routine is a crock, and so is everyplace that calls it.  The problem
 * is that the compiled form of a plpgsql function is allocated permanently
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


/* ----------
 * plpgsql_compile		Given a pg_proc's oid, make
 *						an execution tree for it.
 * ----------
 */
PLpgSQL_function *
plpgsql_compile(Oid fn_oid, int functype)
{
	int			parse_rc;
	HeapTuple	procTup;
	Form_pg_proc procStruct;
	HeapTuple	typeTup;
	Form_pg_type typeStruct;
	char	   *proc_source;
	PLpgSQL_function *function;
	PLpgSQL_var *var;
	PLpgSQL_row *row;
	PLpgSQL_rec *rec;
	int			i;
	int			arg_varnos[FUNC_MAX_ARGS];
	sigjmp_buf	save_restart;

	/*
	 * Lookup the pg_proc tuple by Oid
	 */
	procTup = SearchSysCache(PROCOID,
							 ObjectIdGetDatum(fn_oid),
							 0, 0, 0);
	if (!HeapTupleIsValid(procTup))
		elog(ERROR, "plpgsql: cache lookup for proc %u failed", fn_oid);

	/*
	 * Setup the scanner input and error info
	 */
	procStruct = (Form_pg_proc) GETSTRUCT(procTup);
	proc_source = DatumGetCString(DirectFunctionCall1(textout,
								  PointerGetDatum(&procStruct->prosrc)));
	plpgsql_setinput(proc_source, functype);
	plpgsql_error_funcname = pstrdup(NameStr(procStruct->proname));
	plpgsql_error_lineno = 0;

	/*
	 * Catch elog() so we can provide notice about where the error is
	 */
	memcpy(&save_restart, &Warn_restart, sizeof(save_restart));
	if (sigsetjmp(Warn_restart, 1) != 0)
	{
		memcpy(&Warn_restart, &save_restart, sizeof(Warn_restart));

		/*
		 * If we are the first of cascaded error catchings, print where
		 * this happened
		 */
		if (plpgsql_error_funcname != NULL)
		{
			elog(NOTICE, "plpgsql: ERROR during compile of %s near line %d",
				 plpgsql_error_funcname, plpgsql_error_lineno);

			plpgsql_error_funcname = NULL;
		}

		siglongjmp(Warn_restart, 1);
	}

	/*
	 * Initialize the compiler
	 */
	plpgsql_ns_init();
	plpgsql_ns_push(NULL);
	plpgsql_DumpExecTree = 0;

	datums_alloc = 128;
	plpgsql_nDatums = 0;
	plpgsql_Datums = palloc(sizeof(PLpgSQL_datum *) * datums_alloc);
	datums_last = 0;

	/*
	 * Create the new function node
	 */
	function = malloc(sizeof(PLpgSQL_function));
	memset(function, 0, sizeof(PLpgSQL_function));
	plpgsql_curr_compile = function;

	function->fn_name = strdup(NameStr(procStruct->proname));
	function->fn_oid = fn_oid;
	function->fn_xmin = procTup->t_data->t_xmin;
	function->fn_cmin = procTup->t_data->t_cmin;
	function->fn_functype = functype;

	switch (functype)
	{
		case T_FUNCTION:

			/*
			 * Normal function has a defined returntype
			 */
			function->fn_rettype = procStruct->prorettype;
			function->fn_retset = procStruct->proretset;

			/*
			 * Lookup the functions return type
			 */
			typeTup = SearchSysCache(TYPEOID,
								ObjectIdGetDatum(procStruct->prorettype),
									 0, 0, 0);
			if (!HeapTupleIsValid(typeTup))
			{
				if (!OidIsValid(procStruct->prorettype))
					elog(ERROR, "plpgsql functions cannot return type \"opaque\""
						 "\n\texcept when used as triggers");
				else
					elog(ERROR, "cache lookup for return type %u failed",
						 procStruct->prorettype);
			}
			typeStruct = (Form_pg_type) GETSTRUCT(typeTup);
			if (typeStruct->typrelid != InvalidOid)
				function->fn_retistuple = true;
			else
			{
				function->fn_retbyval = typeStruct->typbyval;
				function->fn_rettyplen = typeStruct->typlen;
				function->fn_rettypelem = typeStruct->typelem;
				perm_fmgr_info(typeStruct->typinput, &(function->fn_retinput));
			}
			ReleaseSysCache(typeTup);

			/*
			 * Create the variables for the procedures parameters
			 */
			for (i = 0; i < procStruct->pronargs; i++)
			{
				char		buf[256];

				/*
				 * Get the parameters type
				 */
				typeTup = SearchSysCache(TYPEOID,
							ObjectIdGetDatum(procStruct->proargtypes[i]),
										 0, 0, 0);
				if (!HeapTupleIsValid(typeTup))
				{
					if (!OidIsValid(procStruct->proargtypes[i]))
						elog(ERROR, "plpgsql functions cannot take type \"opaque\"");
					else
						elog(ERROR, "cache lookup for argument type %u failed",
							 procStruct->proargtypes[i]);
				}
				typeStruct = (Form_pg_type) GETSTRUCT(typeTup);

				if (typeStruct->typrelid != InvalidOid)
				{
					/*
					 * For tuple type parameters, we set up a record of
					 * that type
					 */
					sprintf(buf, "%s%%rowtype", NameStr(typeStruct->typname));
					if (plpgsql_parse_wordrowtype(buf) != T_ROW)
						elog(ERROR, "cannot get tuple struct of argument %d",
							 i + 1);

					row = plpgsql_yylval.row;
					sprintf(buf, "$%d", i + 1);

					row->refname = strdup(buf);

					plpgsql_adddatum((PLpgSQL_datum *) row);
					plpgsql_ns_additem(PLPGSQL_NSTYPE_ROW, row->rowno, buf);

					arg_varnos[i] = row->rowno;
				}
				else
				{
					/*
					 * Normal parameters get a var node
					 */
					var = malloc(sizeof(PLpgSQL_var));
					memset(var, 0, sizeof(PLpgSQL_var));
					var->datatype = malloc(sizeof(PLpgSQL_type));
					memset(var->datatype, 0, sizeof(PLpgSQL_type));

					sprintf(buf, "$%d", i + 1);
					var->dtype = PLPGSQL_DTYPE_VAR;
					var->refname = strdup(buf);
					var->lineno = 0;
					var->datatype->typname = strdup(NameStr(typeStruct->typname));
					var->datatype->typoid = procStruct->proargtypes[i];
					perm_fmgr_info(typeStruct->typinput, &(var->datatype->typinput));
					var->datatype->typelem = typeStruct->typelem;
					var->datatype->typbyval = typeStruct->typbyval;
					var->datatype->typlen = typeStruct->typlen;
					var->datatype->atttypmod = -1;
					var->isconst = true;
					var->notnull = false;
					var->default_val = NULL;

					plpgsql_adddatum((PLpgSQL_datum *) var);
					plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, buf);

					arg_varnos[i] = var->varno;
				}
				ReleaseSysCache(typeTup);
			}
			break;

		case T_TRIGGER:

			/*
			 * Trigger procedures return type is unknown yet
			 */
			function->fn_rettype = InvalidOid;
			function->fn_retbyval = false;
			function->fn_retistuple = true;
			function->fn_retset = false;

			/*
			 * Add the record for referencing NEW
			 */
			rec = malloc(sizeof(PLpgSQL_rec));
			memset(rec, 0, sizeof(PLpgSQL_rec));
			rec->dtype = PLPGSQL_DTYPE_REC;
			rec->refname = strdup("new");
			rec->tup = NULL;
			rec->tupdesc = NULL;
			rec->freetup = false;
			plpgsql_adddatum((PLpgSQL_datum *) rec);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_REC, rec->recno, rec->refname);
			function->new_varno = rec->recno;

			/*
			 * Add the record for referencing OLD
			 */
			rec = malloc(sizeof(PLpgSQL_rec));
			memset(rec, 0, sizeof(PLpgSQL_rec));
			rec->dtype = PLPGSQL_DTYPE_REC;
			rec->refname = strdup("old");
			rec->tup = NULL;
			rec->tupdesc = NULL;
			rec->freetup = false;
			plpgsql_adddatum((PLpgSQL_datum *) rec);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_REC, rec->recno, rec->refname);
			function->old_varno = rec->recno;

			/*
			 * Add the variable tg_name
			 */
			var = malloc(sizeof(PLpgSQL_var));
			memset(var, 0, sizeof(PLpgSQL_var));

			var->dtype = PLPGSQL_DTYPE_VAR;
			var->refname = strdup("tg_name");
			var->lineno = 0;
			var->datatype = plpgsql_parse_datatype("name");
			var->isconst = false;
			var->notnull = false;
			var->default_val = NULL;

			plpgsql_adddatum((PLpgSQL_datum *) var);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, var->refname);
			function->tg_name_varno = var->varno;

			/*
			 * Add the variable tg_when
			 */
			var = malloc(sizeof(PLpgSQL_var));
			memset(var, 0, sizeof(PLpgSQL_var));

			var->dtype = PLPGSQL_DTYPE_VAR;
			var->refname = strdup("tg_when");
			var->lineno = 0;
			var->datatype = plpgsql_parse_datatype("text");
			var->isconst = false;
			var->notnull = false;
			var->default_val = NULL;

			plpgsql_adddatum((PLpgSQL_datum *) var);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, var->refname);
			function->tg_when_varno = var->varno;

			/*
			 * Add the variable tg_level
			 */
			var = malloc(sizeof(PLpgSQL_var));
			memset(var, 0, sizeof(PLpgSQL_var));

			var->dtype = PLPGSQL_DTYPE_VAR;
			var->refname = strdup("tg_level");
			var->lineno = 0;
			var->datatype = plpgsql_parse_datatype("text");
			var->isconst = false;
			var->notnull = false;
			var->default_val = NULL;

			plpgsql_adddatum((PLpgSQL_datum *) var);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, var->refname);
			function->tg_level_varno = var->varno;

			/*
			 * Add the variable tg_op
			 */
			var = malloc(sizeof(PLpgSQL_var));
			memset(var, 0, sizeof(PLpgSQL_var));

			var->dtype = PLPGSQL_DTYPE_VAR;
			var->refname = strdup("tg_op");
			var->lineno = 0;
			var->datatype = plpgsql_parse_datatype("text");
			var->isconst = false;
			var->notnull = false;
			var->default_val = NULL;

			plpgsql_adddatum((PLpgSQL_datum *) var);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, var->refname);
			function->tg_op_varno = var->varno;

			/*
			 * Add the variable tg_relid
			 */
			var = malloc(sizeof(PLpgSQL_var));
			memset(var, 0, sizeof(PLpgSQL_var));

			var->dtype = PLPGSQL_DTYPE_VAR;
			var->refname = strdup("tg_relid");
			var->lineno = 0;
			var->datatype = plpgsql_parse_datatype("oid");
			var->isconst = false;
			var->notnull = false;
			var->default_val = NULL;

			plpgsql_adddatum((PLpgSQL_datum *) var);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, var->refname);
			function->tg_relid_varno = var->varno;

			/*
			 * Add the variable tg_relname
			 */
			var = malloc(sizeof(PLpgSQL_var));
			memset(var, 0, sizeof(PLpgSQL_var));

			var->dtype = PLPGSQL_DTYPE_VAR;
			var->refname = strdup("tg_relname");
			var->lineno = 0;
			var->datatype = plpgsql_parse_datatype("name");
			var->isconst = false;
			var->notnull = false;
			var->default_val = NULL;

			plpgsql_adddatum((PLpgSQL_datum *) var);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, var->refname);
			function->tg_relname_varno = var->varno;

			/*
			 * Add the variable tg_nargs
			 */
			var = malloc(sizeof(PLpgSQL_var));
			memset(var, 0, sizeof(PLpgSQL_var));

			var->dtype = PLPGSQL_DTYPE_VAR;
			var->refname = strdup("tg_nargs");
			var->lineno = 0;
			var->datatype = plpgsql_parse_datatype("int4");
			var->isconst = false;
			var->notnull = false;
			var->default_val = NULL;

			plpgsql_adddatum((PLpgSQL_datum *) var);
			plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, var->refname);
			function->tg_nargs_varno = var->varno;

			break;

		default:
			elog(ERROR, "unknown function type %u in plpgsql_compile()",
				 functype);
			break;
	}

	/*
	 * Create the magic found variable indicating if the last FOR or
	 * SELECT statement returned data
	 */
	var = malloc(sizeof(PLpgSQL_var));
	memset(var, 0, sizeof(PLpgSQL_var));

	var->dtype = PLPGSQL_DTYPE_VAR;
	var->refname = strdup("found");
	var->lineno = 0;
	var->datatype = plpgsql_parse_datatype("bool");
	var->isconst = false;
	var->notnull = false;
	var->default_val = NULL;

	plpgsql_adddatum((PLpgSQL_datum *) var);
	plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, var->varno, strdup("found"));
	function->found_varno = var->varno;

	/*
	 * Forget about the above created variables
	 */
	plpgsql_add_initdatums(NULL);

	/*
	 * Now parse the functions text
	 */
	parse_rc = plpgsql_yyparse();
	if (parse_rc != 0)
		elog(ERROR, "plpgsql: parser returned %d ???", parse_rc);

	/*
	 * If that was successful, complete the functions info.
	 */
	function->fn_nargs = procStruct->pronargs;
	for (i = 0; i < function->fn_nargs; i++)
		function->fn_argvarnos[i] = arg_varnos[i];
	function->ndatums = plpgsql_nDatums;
	function->datums = malloc(sizeof(PLpgSQL_datum *) * plpgsql_nDatums);
	for (i = 0; i < plpgsql_nDatums; i++)
		function->datums[i] = plpgsql_Datums[i];
	function->action = plpgsql_yylval.program;

	ReleaseSysCache(procTup);

	/*
	 * Restore the previous elog() jump target
	 */
	plpgsql_error_funcname = NULL;
	plpgsql_error_lineno = 0;
	memcpy(&Warn_restart, &save_restart, sizeof(Warn_restart));

	/*
	 * Finally return the compiled function
	 */
	if (plpgsql_DumpExecTree)
		plpgsql_dumptree(function);
	return function;
}


/* ----------
 * plpgsql_parse_word		The scanner calls this to postparse
 *				any single word not found by a
 *				keyword rule.
 * ----------
 */
int
plpgsql_parse_word(char *word)
{
	PLpgSQL_nsitem *nse;
	char	   *cp;

	/*
	 * We do our lookups case insensitive
	 */
	cp = plpgsql_tolower(word);

	/*
	 * Special handling when compiling triggers
	 */
	if (plpgsql_curr_compile->fn_functype == T_TRIGGER)
	{
		if (strcmp(cp, "tg_argv") == 0)
		{
			int			save_spacescanned = plpgsql_SpaceScanned;
			PLpgSQL_trigarg *trigarg;

			trigarg = malloc(sizeof(PLpgSQL_trigarg));
			memset(trigarg, 0, sizeof(PLpgSQL_trigarg));
			trigarg->dtype = PLPGSQL_DTYPE_TRIGARG;

			if (plpgsql_yylex() != '[')
				plpgsql_yyerror("expected [");

			trigarg->argnum = plpgsql_read_expression(']', "]");

			plpgsql_adddatum((PLpgSQL_datum *) trigarg);
			plpgsql_yylval.trigarg = trigarg;

			plpgsql_SpaceScanned = save_spacescanned;
			return T_TGARGV;
		}
	}

	/*
	 * Do a lookup on the compilers namestack
	 */
	nse = plpgsql_ns_lookup(cp, NULL);
	if (nse != NULL)
	{
		pfree(cp);
		switch (nse->itemtype)
		{
			case PLPGSQL_NSTYPE_LABEL:
				return T_LABEL;

			case PLPGSQL_NSTYPE_VAR:
				plpgsql_yylval.var = (PLpgSQL_var *) (plpgsql_Datums[nse->itemno]);
				return T_VARIABLE;

			case PLPGSQL_NSTYPE_REC:
				plpgsql_yylval.rec = (PLpgSQL_rec *) (plpgsql_Datums[nse->itemno]);
				return T_RECORD;

			case PLPGSQL_NSTYPE_ROW:
				plpgsql_yylval.row = (PLpgSQL_row *) (plpgsql_Datums[nse->itemno]);
				return T_ROW;

			default:
				return T_ERROR;
		}
	}

	/*
	 * Nothing found - up to now it's a word without any special meaning
	 * for us.
	 */
	pfree(cp);
	return T_WORD;
}


/* ----------
 * plpgsql_parse_dblword		Same lookup for two words
 *					separated by a dot.
 * ----------
 */
int
plpgsql_parse_dblword(char *string)
{
	char	   *word1;
	char	   *word2;
	PLpgSQL_nsitem *ns;

	/*
	 * Convert to lower case and separate the words
	 */
	word1 = plpgsql_tolower(string);
	word2 = strchr(word1, '.');
	*word2++ = '\0';

	/*
	 * Lookup the first word
	 */
	ns = plpgsql_ns_lookup(word1, NULL);
	if (ns == NULL)
	{
		pfree(word1);
		return T_ERROR;
	}

	switch (ns->itemtype)
	{
		case PLPGSQL_NSTYPE_LABEL:

			/*
			 * First word is a label, so second word could be a variable,
			 * record or row in that bodies namestack. Anything else could
			 * only be something in a query given to the SPI manager and
			 * T_ERROR will get eaten up by the collector routines.
			 */
			ns = plpgsql_ns_lookup(word2, word1);
			if (ns == NULL)
			{
				pfree(word1);
				return T_ERROR;
			}
			switch (ns->itemtype)
			{
				case PLPGSQL_NSTYPE_VAR:
					plpgsql_yylval.var = (PLpgSQL_var *) (plpgsql_Datums[ns->itemno]);
					pfree(word1);
					return T_VARIABLE;

				case PLPGSQL_NSTYPE_REC:
					plpgsql_yylval.rec = (PLpgSQL_rec *) (plpgsql_Datums[ns->itemno]);
					pfree(word1);
					return T_RECORD;

				case PLPGSQL_NSTYPE_ROW:
					plpgsql_yylval.row = (PLpgSQL_row *) (plpgsql_Datums[ns->itemno]);
					pfree(word1);
					return T_ROW;

				default:
					pfree(word1);
					return T_ERROR;
			}

		case PLPGSQL_NSTYPE_REC:
			{
				/*
				 * First word is a record name, so second word must be a
				 * field in this record.
				 */
				PLpgSQL_recfield *new;

				new = malloc(sizeof(PLpgSQL_recfield));
				new->dtype = PLPGSQL_DTYPE_RECFIELD;
				new->fieldname = strdup(word2);
				new->recno = ns->itemno;

				plpgsql_adddatum((PLpgSQL_datum *) new);

				pfree(word1);
				plpgsql_yylval.recfield = new;
				return T_RECFIELD;
			}

		case PLPGSQL_NSTYPE_ROW:
			{
				/*
				 * First word is a row name, so second word must be a
				 * field in this row.
				 */
				PLpgSQL_row *row;
				int			i;

				row = (PLpgSQL_row *) (plpgsql_Datums[ns->itemno]);
				for (i = 0; i < row->nfields; i++)
				{
					if (strcmp(row->fieldnames[i], word2) == 0)
					{
						plpgsql_yylval.var = (PLpgSQL_var *) (plpgsql_Datums[row->varnos[i]]);
						pfree(word1);
						return T_VARIABLE;
					}
				}
				elog(ERROR, "row %s doesn't have a field %s",
					 word1, word2);
			}

		default:
			break;
	}

	pfree(word1);
	return T_ERROR;
}


/* ----------
 * plpgsql_parse_tripword		Same lookup for three words
 *					separated by dots.
 * ----------
 */
int
plpgsql_parse_tripword(char *string)
{
	char	   *word1;
	char	   *word2;
	char	   *word3;
	PLpgSQL_nsitem *ns;

	/*
	 * Convert to lower case and separate the words
	 */
	word1 = plpgsql_tolower(string);
	word2 = strchr(word1, '.');
	*word2++ = '\0';
	word3 = strchr(word2, '.');
	*word3++ = '\0';

	/*
	 * Lookup the first word - it must be a label
	 */
	ns = plpgsql_ns_lookup(word1, NULL);
	if (ns == NULL)
	{
		pfree(word1);
		return T_ERROR;
	}
	if (ns->itemtype != PLPGSQL_NSTYPE_LABEL)
	{
		pfree(word1);
		return T_ERROR;
	}

	/*
	 * First word is a label, so second word could be a record or row
	 */
	ns = plpgsql_ns_lookup(word2, word1);
	if (ns == NULL)
	{
		pfree(word1);
		return T_ERROR;
	}

	switch (ns->itemtype)
	{
		case PLPGSQL_NSTYPE_REC:
			{
				/*
				 * This word is a record name, so third word must be a
				 * field in this record.
				 */
				PLpgSQL_recfield *new;

				new = malloc(sizeof(PLpgSQL_recfield));
				new->dtype = PLPGSQL_DTYPE_RECFIELD;
				new->fieldname = strdup(word3);
				new->recno = ns->itemno;

				plpgsql_adddatum((PLpgSQL_datum *) new);

				pfree(word1);
				plpgsql_yylval.recfield = new;
				return T_RECFIELD;
			}

		case PLPGSQL_NSTYPE_ROW:
			{
				/*
				 * This word is a row name, so third word must be a field
				 * in this row.
				 */
				PLpgSQL_row *row;
				int			i;

				row = (PLpgSQL_row *) (plpgsql_Datums[ns->itemno]);
				for (i = 0; i < row->nfields; i++)
				{
					if (strcmp(row->fieldnames[i], word3) == 0)
					{
						plpgsql_yylval.var = (PLpgSQL_var *) (plpgsql_Datums[row->varnos[i]]);
						pfree(word1);
						return T_VARIABLE;
					}
				}
				elog(ERROR, "row %s.%s doesn't have a field %s",
					 word1, word2, word3);
			}

		default:
			break;
	}

	pfree(word1);
	return T_ERROR;
}


/* ----------
 * plpgsql_parse_wordtype	The scanner found word%TYPE. word can be
 *				a variable name or a basetype.
 * ----------
 */
int
plpgsql_parse_wordtype(char *word)
{
	PLpgSQL_nsitem *nse;
	char	   *cp;
	HeapTuple	typeTup;
	Form_pg_type typeStruct;
	char	   *typeXlated;
	bool		old_nsstate;

	/*
	 * We do our lookups case insensitive
	 */
	cp = plpgsql_tolower(word);
	*(strchr(cp, '%')) = '\0';

	/*
	 * Do a lookup on the compilers namestack. But ensure it moves up to
	 * the toplevel.
	 */
	old_nsstate = plpgsql_ns_setlocal(false);
	nse = plpgsql_ns_lookup(cp, NULL);
	plpgsql_ns_setlocal(old_nsstate);

	if (nse != NULL)
	{
		pfree(cp);
		switch (nse->itemtype)
		{
			case PLPGSQL_NSTYPE_VAR:
				plpgsql_yylval.dtype = ((PLpgSQL_var *) (plpgsql_Datums[nse->itemno]))->datatype;
				return T_DTYPE;

			default:
				return T_ERROR;
		}
	}

	/*
	 * Word wasn't found on the namestack. Try to find a data type with
	 * that name, but ignore pg_type entries that are in fact class types.
	 */
	typeXlated = xlateSqlType(cp);
	typeTup = SearchSysCache(TYPENAME,
							 PointerGetDatum(typeXlated),
							 0, 0, 0);
	if (HeapTupleIsValid(typeTup))
	{
		PLpgSQL_type *typ;

		typeStruct = (Form_pg_type) GETSTRUCT(typeTup);

		if (typeStruct->typrelid != InvalidOid)
		{
			ReleaseSysCache(typeTup);
			pfree(cp);
			return T_ERROR;
		}

		typ = (PLpgSQL_type *) malloc(sizeof(PLpgSQL_type));

		typ->typname = strdup(NameStr(typeStruct->typname));
		typ->typoid = typeTup->t_data->t_oid;
		perm_fmgr_info(typeStruct->typinput, &(typ->typinput));
		typ->typelem = typeStruct->typelem;
		typ->typbyval = typeStruct->typbyval;
		typ->typlen = typeStruct->typlen;
		typ->atttypmod = -1;

		plpgsql_yylval.dtype = typ;

		ReleaseSysCache(typeTup);
		pfree(cp);
		return T_DTYPE;
	}

	/*
	 * Nothing found - up to now it's a word without any special meaning
	 * for us.
	 */
	pfree(cp);
	return T_ERROR;
}


/* ----------
 * plpgsql_parse_dblwordtype		Same lookup for word.word%TYPE
 * ----------
 */
int
plpgsql_parse_dblwordtype(char *string)
{
	char	   *word1;
	char	   *word2;
	PLpgSQL_nsitem *nse;
	bool		old_nsstate;
	HeapTuple	classtup;
	Form_pg_class classStruct;
	HeapTuple	attrtup;
	Form_pg_attribute attrStruct;
	HeapTuple	typetup;
	Form_pg_type typeStruct;
	PLpgSQL_type *typ;


	/*
	 * Convert to lower case and separate the words
	 */
	word1 = plpgsql_tolower(string);
	word2 = strchr(word1, '.');
	*word2++ = '\0';
	*(strchr(word2, '%')) = '\0';

	/*
	 * Lookup the first word
	 */
	nse = plpgsql_ns_lookup(word1, NULL);

	/*
	 * If this is a label lookup the second word in that labels namestack
	 * level
	 */
	if (nse != NULL)
	{
		if (nse->itemtype == PLPGSQL_NSTYPE_LABEL)
		{
			old_nsstate = plpgsql_ns_setlocal(false);
			nse = plpgsql_ns_lookup(word2, word1);
			plpgsql_ns_setlocal(old_nsstate);

			pfree(word1);

			if (nse != NULL)
			{
				switch (nse->itemtype)
				{
					case PLPGSQL_NSTYPE_VAR:
						plpgsql_yylval.dtype = ((PLpgSQL_var *) (plpgsql_Datums[nse->itemno]))->datatype;
						return T_DTYPE;

					default:
						return T_ERROR;
				}
			}
			return T_ERROR;
		}
		pfree(word1);
		return T_ERROR;
	}

	/*
	 * First word could also be a table name
	 */
	classtup = SearchSysCache(RELNAME,
							  PointerGetDatum(word1),
							  0, 0, 0);
	if (!HeapTupleIsValid(classtup))
	{
		pfree(word1);
		return T_ERROR;
	}

	/*
	 * It must be a relation, sequence or view
	 */
	classStruct = (Form_pg_class) GETSTRUCT(classtup);
	if (classStruct->relkind != RELKIND_RELATION &&
		classStruct->relkind != RELKIND_SEQUENCE &&
		classStruct->relkind != RELKIND_VIEW)
	{
		ReleaseSysCache(classtup);
		pfree(word1);
		return T_ERROR;
	}

	/*
	 * Fetch the named table field and it's type
	 */
	attrtup = SearchSysCache(ATTNAME,
							 ObjectIdGetDatum(classtup->t_data->t_oid),
							 PointerGetDatum(word2),
							 0, 0);
	if (!HeapTupleIsValid(attrtup))
	{
		ReleaseSysCache(classtup);
		pfree(word1);
		return T_ERROR;
	}
	attrStruct = (Form_pg_attribute) GETSTRUCT(attrtup);

	typetup = SearchSysCache(TYPEOID,
							 ObjectIdGetDatum(attrStruct->atttypid),
							 0, 0, 0);
	if (!HeapTupleIsValid(typetup))
		elog(ERROR, "cache lookup for type %u of %s.%s failed",
			 attrStruct->atttypid, word1, word2);
	typeStruct = (Form_pg_type) GETSTRUCT(typetup);

	/*
	 * Found that - build a compiler type struct and return it
	 */
	typ = (PLpgSQL_type *) malloc(sizeof(PLpgSQL_type));

	typ->typname = strdup(NameStr(typeStruct->typname));
	typ->typoid = typetup->t_data->t_oid;
	perm_fmgr_info(typeStruct->typinput, &(typ->typinput));
	typ->typelem = typeStruct->typelem;
	typ->typbyval = typeStruct->typbyval;
	typ->typlen = typeStruct->typlen;
	typ->atttypmod = attrStruct->atttypmod;

	plpgsql_yylval.dtype = typ;

	ReleaseSysCache(classtup);
	ReleaseSysCache(attrtup);
	ReleaseSysCache(typetup);
	pfree(word1);
	return T_DTYPE;
}


/* ----------
 * plpgsql_parse_wordrowtype		Scanner found word%ROWTYPE.
 *					So word must be a table name.
 * ----------
 */
int
plpgsql_parse_wordrowtype(char *string)
{
	HeapTuple	classtup;
	Form_pg_class classStruct;
	HeapTuple	typetup;
	Form_pg_type typeStruct;
	HeapTuple	attrtup;
	Form_pg_attribute attrStruct;
	char	   *word1;
	char	   *cp;
	int			i;
	PLpgSQL_row *row;
	PLpgSQL_var *var;

	/*
	 * Get the word in lower case and fetch the pg_class tuple.
	 */
	word1 = plpgsql_tolower(string);
	cp = strchr(word1, '%');
	*cp = '\0';

	classtup = SearchSysCache(RELNAME,
							  PointerGetDatum(word1),
							  0, 0, 0);
	if (!HeapTupleIsValid(classtup))
		elog(ERROR, "%s: no such class", word1);
	classStruct = (Form_pg_class) GETSTRUCT(classtup);
	/* accept relation, sequence, or view pg_class entries */
	if (classStruct->relkind != RELKIND_RELATION &&
		classStruct->relkind != RELKIND_SEQUENCE &&
		classStruct->relkind != RELKIND_VIEW)
		elog(ERROR, "%s isn't a table", word1);

	/*
	 * Fetch the table's pg_type tuple too
	 */
	typetup = SearchSysCache(TYPENAME,
							 PointerGetDatum(word1),
							 0, 0, 0);
	if (!HeapTupleIsValid(typetup))
		elog(ERROR, "cache lookup for %s in pg_type failed", word1);

	/*
	 * Create a row datum entry and all the required variables that it
	 * will point to.
	 */
	row = malloc(sizeof(PLpgSQL_row));
	memset(row, 0, sizeof(PLpgSQL_row));

	row->dtype = PLPGSQL_DTYPE_ROW;
	row->nfields = classStruct->relnatts;
	row->rowtypeclass = typetup->t_data->t_oid;
	row->fieldnames = malloc(sizeof(char *) * row->nfields);
	row->varnos = malloc(sizeof(int) * row->nfields);

	ReleaseSysCache(typetup);

	for (i = 0; i < row->nfields; i++)
	{
		/*
		 * Get the attribute and it's type
		 */
		attrtup = SearchSysCache(ATTNUM,
							   ObjectIdGetDatum(classtup->t_data->t_oid),
								 Int16GetDatum(i + 1),
								 0, 0);
		if (!HeapTupleIsValid(attrtup))
			elog(ERROR, "cache lookup for attribute %d of class %s failed",
				 i + 1, word1);
		attrStruct = (Form_pg_attribute) GETSTRUCT(attrtup);

		cp = pstrdup(NameStr(attrStruct->attname));

		typetup = SearchSysCache(TYPEOID,
								 ObjectIdGetDatum(attrStruct->atttypid),
								 0, 0, 0);
		if (!HeapTupleIsValid(typetup))
			elog(ERROR, "cache lookup for type %u of %s.%s failed",
				 attrStruct->atttypid, word1, cp);
		typeStruct = (Form_pg_type) GETSTRUCT(typetup);

		/*
		 * Create the internal variable
		 *
		 * We know if the table definitions contain a default value or if the
		 * field is declared in the table as NOT NULL. But it's possible
		 * to create a table field as NOT NULL without a default value and
		 * that would lead to problems later when initializing the
		 * variables due to entering a block at execution time. Thus we
		 * ignore this information for now.
		 */
		var = malloc(sizeof(PLpgSQL_var));
		memset(var, 0, sizeof(PLpgSQL_var));
		var->dtype = PLPGSQL_DTYPE_VAR;
		var->refname = malloc(strlen(word1) + strlen(cp) + 2);
		strcpy(var->refname, word1);
		strcat(var->refname, ".");
		strcat(var->refname, cp);
		var->datatype = malloc(sizeof(PLpgSQL_type));
		var->datatype->typname = strdup(NameStr(typeStruct->typname));
		var->datatype->typoid = typetup->t_data->t_oid;
		perm_fmgr_info(typeStruct->typinput, &(var->datatype->typinput));
		var->datatype->typelem = typeStruct->typelem;
		var->datatype->typbyval = typeStruct->typbyval;
		var->datatype->typlen = typeStruct->typlen;
		var->datatype->atttypmod = attrStruct->atttypmod;
		var->isconst = false;
		var->notnull = false;
		var->default_val = NULL;
		var->value = (Datum) 0;
		var->isnull = true;
		var->freeval = false;

		ReleaseSysCache(typetup);
		ReleaseSysCache(attrtup);

		plpgsql_adddatum((PLpgSQL_datum *) var);

		/*
		 * Add the variable to the row.
		 */
		row->fieldnames[i] = strdup(cp);
		row->varnos[i] = var->varno;
	}

	ReleaseSysCache(classtup);

	/*
	 * Return the complete row definition
	 */
	plpgsql_yylval.row = row;

	return T_ROW;
}


/* ----------
 * plpgsql_parse_datatype			Scanner found something that should
 *					be a datatype name.
 * ----------
 */
PLpgSQL_type *
plpgsql_parse_datatype(char *string)
{
	Oid			type_id;
	int32		typmod;
	HeapTuple	typeTup;
	Form_pg_type typeStruct;
	PLpgSQL_type *typ;

	/* Let the main parser try to parse it under standard SQL rules */
	parseTypeString(string, &type_id, &typmod);

	/* Okay, build a PLpgSQL_type data structure for it */
	typeTup = SearchSysCache(TYPEOID,
							 ObjectIdGetDatum(type_id),
							 0, 0, 0);
	if (!HeapTupleIsValid(typeTup))
		elog(ERROR, "cache lookup failed for type %u", type_id);
	typeStruct = (Form_pg_type) GETSTRUCT(typeTup);

	typ = (PLpgSQL_type *) malloc(sizeof(PLpgSQL_type));

	typ->typname = strdup(NameStr(typeStruct->typname));
	typ->typoid = type_id;
	perm_fmgr_info(typeStruct->typinput, &(typ->typinput));
	typ->typelem = typeStruct->typelem;
	typ->typbyval = typeStruct->typbyval;
	typ->typlen = typeStruct->typlen;
	typ->atttypmod = typmod;

	ReleaseSysCache(typeTup);

	return typ;
}


/* ----------
 * plpgsql_adddatum			Add a variable, record or row
 *					to the compilers datum list.
 * ----------
 */
void
plpgsql_adddatum(PLpgSQL_datum * new)
{
	if (plpgsql_nDatums == datums_alloc)
	{
		datums_alloc *= 2;
		plpgsql_Datums = repalloc(plpgsql_Datums, sizeof(PLpgSQL_datum *) * datums_alloc);
	}

	new->dno = plpgsql_nDatums;
	plpgsql_Datums[plpgsql_nDatums++] = new;
}


/* ----------
 * plpgsql_add_initdatums		Put all datum entries created
 *					since the last call into the
 *					finishing code block so the
 *					block knows which variables to
 *					reinitialize when entered.
 * ----------
 */
int
plpgsql_add_initdatums(int **varnos)
{
	int			i;
	int			n = 0;

	for (i = datums_last; i < plpgsql_nDatums; i++)
	{
		switch (plpgsql_Datums[i]->dtype)
		{
			case PLPGSQL_DTYPE_VAR:
				n++;
				break;

			default:
				break;
		}
	}

	if (varnos != NULL)
	{
		*varnos = (int *) malloc(sizeof(int) * n);

		n = 0;
		for (i = datums_last; i < plpgsql_nDatums; i++)
		{
			switch (plpgsql_Datums[i]->dtype)
			{
				case PLPGSQL_DTYPE_VAR:
					(*varnos)[n++] = plpgsql_Datums[i]->dno;

				default:
					break;
			}
		}
	}

	datums_last = plpgsql_nDatums;
	return n;
}


/* ---------
 * plpgsql_yyerror			Handle parser error
 * ---------
 */

void
plpgsql_yyerror(const char *s)
{
	plpgsql_error_lineno = plpgsql_yylineno;
	elog(ERROR, "%s at or near \"%s\"", s, plpgsql_yytext);
}
