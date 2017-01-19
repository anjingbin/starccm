/**********************************************************************
 * plpgsql.h		- Definitions for the PL/pgSQL
 *			  procedural language
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/pl/plpgsql/src/plpgsql.h,v 1.2 2004/07/05 08:56:18 SuLiang Exp $
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
#ifndef PLPGSQL_H
#define PLPGSQL_H

#include "postgres.h"

#include "fmgr.h"
#include "executor/spi.h"
#include "commands/trigger.h"

/**********************************************************************
 * Definitions
 **********************************************************************/

/* ----------
 * Compilers namestack item types
 * ----------
 */
enum
{
	PLPGSQL_NSTYPE_LABEL,
	PLPGSQL_NSTYPE_VAR,
	PLPGSQL_NSTYPE_ROW,
	PLPGSQL_NSTYPE_REC,
	PLPGSQL_NSTYPE_RECFIELD
};

/* ----------
 * Datum array node types
 * ----------
 */
enum
{
	PLPGSQL_DTYPE_VAR,
	PLPGSQL_DTYPE_ROW,
	PLPGSQL_DTYPE_REC,
	PLPGSQL_DTYPE_RECFIELD,
	PLPGSQL_DTYPE_EXPR,
	PLPGSQL_DTYPE_TRIGARG
};

/* ----------
 * Execution tree node types
 * ----------
 */
enum
{
	PLPGSQL_STMT_BLOCK,
	PLPGSQL_STMT_ASSIGN,
	PLPGSQL_STMT_IF,
	PLPGSQL_STMT_LOOP,
	PLPGSQL_STMT_WHILE,
	PLPGSQL_STMT_FORI,
	PLPGSQL_STMT_FORS,
	PLPGSQL_STMT_SELECT,
	PLPGSQL_STMT_EXIT,
	PLPGSQL_STMT_RETURN,
	PLPGSQL_STMT_RAISE,
	PLPGSQL_STMT_EXECSQL,
	PLPGSQL_STMT_DYNEXECUTE,
	PLPGSQL_STMT_DYNFORS,
	PLPGSQL_STMT_GETDIAG,
	PLPGSQL_STMT_OPEN,
	PLPGSQL_STMT_FETCH,
	PLPGSQL_STMT_CLOSE
};


/* ----------
 * Execution node return codes
 * ----------
 */
enum
{
	PLPGSQL_RC_OK,
	PLPGSQL_RC_EXIT,
	PLPGSQL_RC_RETURN
};

/* ----------
 * GET DIAGNOSTICS system attrs
 * ----------
 */
enum
{
	PLPGSQL_GETDIAG_ROW_COUNT,
	PLPGSQL_GETDIAG_RESULT_OID
};


/**********************************************************************
 * Node and structure definitions
 **********************************************************************/


typedef struct
{								/* Dynamic string control structure */
	int			alloc;
	int			used;
	char	   *value;
}	PLpgSQL_dstring;


typedef struct
{								/* Postgres base data type		*/
	char	   *typname;
	Oid			typoid;
	FmgrInfo	typinput;
	Oid			typelem;
	int16		typlen;
	bool		typbyval;
	int32		atttypmod;
}	PLpgSQL_type;


typedef struct
{								/* Generic datum array item		*/
	int			dtype;
	int			dno;
}	PLpgSQL_datum;


typedef struct
{								/* SQL Query to plan and execute	*/
	int			dtype;
	int			exprno;
	char	   *query;
	void	   *plan;
	Node	   *plan_simple_expr;
	Oid			plan_simple_type;
	Oid		   *plan_argtypes;
	int			nparams;
	int			params[1];
}	PLpgSQL_expr;


typedef struct
{								/* Local variable			*/
	int			dtype;
	int			varno;
	char	   *refname;
	int			lineno;

	PLpgSQL_type *datatype;
	int			isconst;
	int			notnull;
	PLpgSQL_expr *default_val;
	PLpgSQL_expr *cursor_explicit_expr;
	int			cursor_explicit_argrow;

	Datum		value;
	bool		isnull;
	bool		freeval;
}	PLpgSQL_var;


typedef struct
{								/* Rowtype				*/
	int			dtype;
	int			rowno;
	char	   *refname;
	int			lineno;
	Oid			rowtypeclass;

	int			nfields;
	char	  **fieldnames;
	int		   *varnos;
}	PLpgSQL_row;


typedef struct
{								/* Record of undefined structure	*/
	int			dtype;
	int			recno;
	char	   *refname;
	int			lineno;

	HeapTuple	tup;
	TupleDesc	tupdesc;
	bool		freetup;
	bool		freetupdesc;
}	PLpgSQL_rec;


typedef struct
{								/* Field in record			*/
	int			dtype;
	int			rfno;
	char	   *fieldname;
	int			recno;
}	PLpgSQL_recfield;


typedef struct
{								/* Positional argument to trigger	*/
	int			dtype;
	int			dno;
	PLpgSQL_expr *argnum;
}	PLpgSQL_trigarg;


typedef struct
{								/* Item in the compilers namestack	*/
	int			itemtype;
	int			itemno;
	char		name[1];
}	PLpgSQL_nsitem;


typedef struct PLpgSQL_ns
{								/* Compiler namestack level		*/
	int			items_alloc;
	int			items_used;
	PLpgSQL_nsitem **items;
	struct PLpgSQL_ns *upper;
}	PLpgSQL_ns;


typedef struct
{								/* List of execution nodes		*/
	int			stmts_alloc;
	int			stmts_used;
	struct PLpgSQL_stmt **stmts;
}	PLpgSQL_stmts;


typedef struct
{								/* Generic execution node		*/
	int			cmd_type;
	int			lineno;
}	PLpgSQL_stmt;


typedef struct
{								/* Block of statements			*/
	int			cmd_type;
	int			lineno;
	char	   *label;
	PLpgSQL_stmts *body;
	int			n_initvars;
	int		   *initvarnos;
}	PLpgSQL_stmt_block;


typedef struct
{								/* Assign statement			*/
	int			cmd_type;
	int			lineno;
	int			varno;
	PLpgSQL_expr *expr;
}	PLpgSQL_stmt_assign;


typedef struct
{								/* Get Diagnostics item		*/
	int			item;			/* id for diagnostic value desired */
	int			target;			/* where to assign it */
}	PLpgSQL_diag_item;

typedef struct
{								/* Get Diagnostics statement		*/
	int			cmd_type;
	int			lineno;
	int			ndtitems;
	PLpgSQL_diag_item *dtitems;
}	PLpgSQL_stmt_getdiag;


typedef struct
{								/* IF statement				*/
	int			cmd_type;
	int			lineno;
	PLpgSQL_expr *cond;
	PLpgSQL_stmts *true_body;
	PLpgSQL_stmts *false_body;
}	PLpgSQL_stmt_if;


typedef struct
{								/* Unconditional LOOP statement		*/
	int			cmd_type;
	int			lineno;
	char	   *label;
	PLpgSQL_stmts *body;
}	PLpgSQL_stmt_loop;


typedef struct
{								/* WHILE cond LOOP statement		*/
	int			cmd_type;
	int			lineno;
	char	   *label;
	PLpgSQL_expr *cond;
	PLpgSQL_stmts *body;
}	PLpgSQL_stmt_while;


typedef struct
{								/* FOR statement with integer loopvar	*/
	int			cmd_type;
	int			lineno;
	char	   *label;
	PLpgSQL_var *var;
	PLpgSQL_expr *lower;
	PLpgSQL_expr *upper;
	int			reverse;
	PLpgSQL_stmts *body;
}	PLpgSQL_stmt_fori;


typedef struct
{								/* FOR statement running over SELECT	*/
	int			cmd_type;
	int			lineno;
	char	   *label;
	PLpgSQL_rec *rec;
	PLpgSQL_row *row;
	PLpgSQL_expr *query;
	PLpgSQL_stmts *body;
}	PLpgSQL_stmt_fors;


typedef struct
{								/* FOR statement running over EXECUTE	*/
	int			cmd_type;
	int			lineno;
	char	   *label;
	PLpgSQL_rec *rec;
	PLpgSQL_row *row;
	PLpgSQL_expr *query;
	PLpgSQL_stmts *body;
}	PLpgSQL_stmt_dynfors;


typedef struct
{								/* SELECT ... INTO statement		*/
	int			cmd_type;
	int			lineno;
	PLpgSQL_rec *rec;
	PLpgSQL_row *row;
	PLpgSQL_expr *query;
}	PLpgSQL_stmt_select;


typedef struct
{								/* OPEN a curvar					*/
	int			cmd_type;
	int			lineno;
	int			curvar;
	PLpgSQL_row *returntype;
	PLpgSQL_expr *argquery;
	PLpgSQL_expr *query;
	PLpgSQL_expr *dynquery;
}	PLpgSQL_stmt_open;


typedef struct
{								/* FETCH curvar INTO statement		*/
	int			cmd_type;
	int			lineno;
	PLpgSQL_rec *rec;
	PLpgSQL_row *row;
	int			curvar;
}	PLpgSQL_stmt_fetch;


typedef struct
{								/* CLOSE curvar						*/
	int			cmd_type;
	int			lineno;
	int			curvar;
}	PLpgSQL_stmt_close;


typedef struct
{								/* EXIT statement			*/
	int			cmd_type;
	int			lineno;
	char	   *label;
	PLpgSQL_expr *cond;
}	PLpgSQL_stmt_exit;


typedef struct
{								/* RETURN statement			*/
	int			cmd_type;
	int			lineno;
	bool		retistuple;
	PLpgSQL_expr *expr;
	int			retrecno;
}	PLpgSQL_stmt_return;


typedef struct
{								/* RAISE statement			*/
	int			cmd_type;
	int			lineno;
	int			elog_level;
	char	   *message;
	int			nparams;
	int		   *params;
}	PLpgSQL_stmt_raise;


typedef struct
{								/* Generic SQL statement to execute */
	int			cmd_type;
	int			lineno;
	PLpgSQL_expr *sqlstmt;
}	PLpgSQL_stmt_execsql;


typedef struct
{								/* Dynamic SQL string to execute */
	int			cmd_type;
	int			lineno;
	PLpgSQL_expr *query;
}	PLpgSQL_stmt_dynexecute;


typedef struct PLpgSQL_function
{								/* Complete compiled function	  */
	char	   *fn_name;
	Oid			fn_oid;
	TransactionId fn_xmin;
	CommandId	fn_cmin;
	int			fn_functype;

	Oid			fn_rettype;
	int			fn_rettyplen;
	bool		fn_retbyval;
	FmgrInfo	fn_retinput;
	Oid			fn_rettypelem;
	bool		fn_retistuple;
	bool		fn_retset;

	int			fn_nargs;
	int			fn_argvarnos[FUNC_MAX_ARGS];
	int			found_varno;
	int			new_varno;
	int			old_varno;
	int			tg_name_varno;
	int			tg_when_varno;
	int			tg_level_varno;
	int			tg_op_varno;
	int			tg_relid_varno;
	int			tg_relname_varno;
	int			tg_nargs_varno;

	int			ndatums;
	PLpgSQL_datum **datums;
	PLpgSQL_stmt_block *action;

	struct PLpgSQL_function *next;		/* for chaining list of functions */
}	PLpgSQL_function;


typedef struct
{								/* Runtime execution data	*/
	Datum		retval;
	bool		retisnull;
	Oid			rettype;
	bool		retistuple;
	TupleDesc	rettupdesc;
	bool		retisset;
	char	   *exitlabel;

	int			trig_nargs;
	Datum	   *trig_argv;

	int			found_varno;
	int			ndatums;
	PLpgSQL_datum **datums;

	/* temporary state for results from evaluation of query or expr */
	SPITupleTable *eval_tuptable;
	uint32		eval_processed;
	Oid			eval_lastoid;
	ExprContext *eval_econtext;
}	PLpgSQL_execstate;


/**********************************************************************
 * Global variable declarations
 **********************************************************************/

extern int	plpgsql_DumpExecTree;
extern int	plpgsql_SpaceScanned;
extern int	plpgsql_nDatums;
extern PLpgSQL_datum **plpgsql_Datums;

extern int	plpgsql_error_lineno;
extern char *plpgsql_error_funcname;

/* linkage to the real yytext and yylineno variables */
extern char *plpgsql_base_yytext;

#define plpgsql_yytext plpgsql_base_yytext
extern int	plpgsql_base_yylineno;

#define plpgsql_yylineno plpgsql_base_yylineno

extern PLpgSQL_function *plpgsql_curr_compile;

/**********************************************************************
 * Function declarations
 **********************************************************************/

/* ----------
 * Functions in pl_comp.c
 * ----------
 */
extern PLpgSQL_function *plpgsql_compile(Oid fn_oid, int functype);
extern int	plpgsql_parse_word(char *word);
extern int	plpgsql_parse_dblword(char *string);
extern int	plpgsql_parse_tripword(char *string);
extern int	plpgsql_parse_wordtype(char *string);
extern int	plpgsql_parse_dblwordtype(char *string);
extern int	plpgsql_parse_wordrowtype(char *string);
extern PLpgSQL_type *plpgsql_parse_datatype(char *string);
extern void plpgsql_adddatum(PLpgSQL_datum * new);
extern int	plpgsql_add_initdatums(int **varnos);
extern void plpgsql_yyerror(const char *s);

/* ----------
 * Functions in pl_handler.c
 * ----------
 */
extern Datum plpgsql_call_handler(PG_FUNCTION_ARGS);

/* ----------
 * Functions in pl_exec.c
 * ----------
 */
extern Datum plpgsql_exec_function(PLpgSQL_function * func,
					  FunctionCallInfo fcinfo);
extern HeapTuple plpgsql_exec_trigger(PLpgSQL_function * func,
					 TriggerData *trigdata);


/* ----------
 * Functions for the dynamic string handling in pl_funcs.c
 * ----------
 */
extern void plpgsql_dstring_init(PLpgSQL_dstring * ds);
extern void plpgsql_dstring_free(PLpgSQL_dstring * ds);
extern void plpgsql_dstring_append(PLpgSQL_dstring * ds, char *str);
extern char *plpgsql_dstring_get(PLpgSQL_dstring * ds);

/* ----------
 * Functions for the namestack handling in pl_funcs.c
 * ----------
 */
extern void plpgsql_ns_init(void);
extern bool plpgsql_ns_setlocal(bool flag);
extern void plpgsql_ns_push(char *label);
extern void plpgsql_ns_pop(void);
extern void plpgsql_ns_additem(int itemtype, int itemno, char *name);
extern PLpgSQL_nsitem *plpgsql_ns_lookup(char *name, char *nsname);
extern void plpgsql_ns_rename(char *oldname, char *newname);

/* ----------
 * Other functions in pl_funcs.c
 * ----------
 */
extern char *plpgsql_tolower(char *s);
extern const char *plpgsql_stmt_typename(PLpgSQL_stmt * stmt);
extern void plpgsql_dumptree(PLpgSQL_function * func);

/* ----------
 * Externs in gram.y and scan.l
 * ----------
 */
extern PLpgSQL_expr *plpgsql_read_expression(int until, const char *expected);
extern int	plpgsql_yyparse(void);
extern int	plpgsql_base_yylex(void);
extern int	plpgsql_yylex(void);
extern void plpgsql_push_back_token(int token);
extern void plpgsql_setinput(char *s, int functype);

#endif   /* PLPGSQL_H */
