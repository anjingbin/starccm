/**********************************************************************
 * pl_handler.c		- Handler for the PL/pgSQL
 *			  procedural language
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/pl/plpgsql/src/pl_handler.c,v 1.2 2004/07/05 08:56:18 SuLiang Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "plpgsql.h"
#include "pl.tab.h"

#include "access/heapam.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"
#include "utils/builtins.h"
#include "utils/syscache.h"


/*
 * Head of list of already-compiled functions
 */
static PLpgSQL_function *compiled_functions = NULL;


static bool func_up_to_date(PLpgSQL_function * func);


/* ----------
 * plpgsql_call_handler
 *
 * This is the only visible function of the PL interpreter.
 * The PostgreSQL function manager and trigger manager
 * call this function for execution of PL/pgSQL procedures.
 * ----------
 */
PG_FUNCTION_INFO_V1(plpgsql_call_handler);

Datum
plpgsql_call_handler(PG_FUNCTION_ARGS)
{
	bool		isTrigger = CALLED_AS_TRIGGER(fcinfo);
	Oid			funcOid = fcinfo->flinfo->fn_oid;
	PLpgSQL_function *func;
	Datum		retval;

	/*
	 * Connect to SPI manager
	 */
	if (SPI_connect() != SPI_OK_CONNECT)
		elog(ERROR, "plpgsql: cannot connect to SPI manager");

	/*
	 * Check if we already compiled this function and saved the pointer
	 * (ie, current FmgrInfo has been used before)
	 */
	func = (PLpgSQL_function *) fcinfo->flinfo->fn_extra;
	if (func != NULL)
	{
		Assert(func->fn_oid == funcOid);

		/*
		 * But is the function still up to date?
		 */
		if (!func_up_to_date(func))
			func = NULL;
	}

	if (func == NULL)
	{
		/*
		 * Check if we already compiled this function for another caller
		 */
		for (func = compiled_functions; func != NULL; func = func->next)
		{
			if (funcOid == func->fn_oid && func_up_to_date(func))
				break;
		}

		/*
		 * If not, do so and add it to the compiled ones
		 */
		if (func == NULL)
		{
			func = plpgsql_compile(funcOid,
								   isTrigger ? T_TRIGGER : T_FUNCTION);
			func->next = compiled_functions;
			compiled_functions = func;
		}

		/*
		 * Save pointer in FmgrInfo to avoid search on subsequent calls
		 */
		fcinfo->flinfo->fn_extra = (void *) func;
	}

	/*
	 * Determine if called as function or trigger and call appropriate
	 * subhandler
	 */
	if (isTrigger)
		retval = PointerGetDatum(plpgsql_exec_trigger(func,
									   (TriggerData *) fcinfo->context));
	else
		retval = plpgsql_exec_function(func, fcinfo);

	/*
	 * Disconnect from SPI manager
	 */
	if (SPI_finish() != SPI_OK_FINISH)
		elog(ERROR, "plpgsql: SPI_finish() failed");

	return retval;
}


/*
 * Check to see if a compiled function is still up-to-date.  This
 * is needed because CREATE OR REPLACE FUNCTION can modify the
 * function's pg_proc entry without changing its OID.
 */
static bool
func_up_to_date(PLpgSQL_function * func)
{
	HeapTuple	procTup;
	bool		result;

	procTup = SearchSysCache(PROCOID,
							 ObjectIdGetDatum(func->fn_oid),
							 0, 0, 0);
	if (!HeapTupleIsValid(procTup))
		elog(ERROR, "plpgsql: cache lookup for proc %u failed",
			 func->fn_oid);

	result = (func->fn_xmin == procTup->t_data->t_xmin &&
			  func->fn_cmin == procTup->t_data->t_cmin);

	ReleaseSysCache(procTup);

	return result;
}
