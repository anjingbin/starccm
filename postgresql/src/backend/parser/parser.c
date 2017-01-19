/*-------------------------------------------------------------------------
 *
 * parser.c
 *		Main entry point/driver for PostgreSQL grammar
 *
 * Note that the grammar is not allowed to perform any table access
 * (since we need to be able to do basic parsing even while inside an
 * aborted transaction).  Therefore, the data structures returned by
 * the grammar are "raw" parsetrees that still need to be analyzed by
 * parse_analyze.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/parser/parser.c,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "nodes/parsenodes.h"
#include "parser/gramparse.h"
#include "parser/parse.h"
#include "parser/parser.h"
#include "parser/parse_expr.h"


#if defined(FLEX_SCANNER)
extern void DeleteBuffer(void);
#endif   /* FLEX_SCANNER */

char	   *parseString;		/* the char* which holds the string to be
								 * parsed */
List	   *parsetree;			/* result of parsing is left here */

static int	lookahead_token;	/* one-token lookahead */
static bool have_lookahead;		/* lookahead_token set? */


/*
 * parser
 *		Given a query in string form, and optionally info about
 *		parameter types, do lexical and syntactic analysis.
 *
 * Returns a list of raw (un-analyzed) parse trees.
 */
List *
parser(char *str, Oid *typev, int nargs)
{
	int			yyresult;

	parseString = str;
	parsetree = NIL;			/* in case parser forgets to set it */
	have_lookahead = false;

	scanner_init();
	parser_init(typev, nargs);
	parse_expr_init();

	yyresult = yyparse();

#if defined(FLEX_SCANNER)
	DeleteBuffer();
#endif   /* FLEX_SCANNER */

	clearerr(stdin);

	if (yyresult)				/* error */
		return NIL;

	return parsetree;
}


/*
 * Intermediate filter between parser and base lexer (base_yylex in scan.l).
 *
 * The filter is needed because in some cases SQL92 requires more than one
 * token lookahead.  We reduce these cases to one-token lookahead by combining
 * tokens here, in order to keep the grammar LR(1).
 *
 * Using a filter is simpler than trying to recognize multiword tokens
 * directly in scan.l, because we'd have to allow for comments between the
 * words ...
 */
int
yylex(void)
{
	int			cur_token;

	/* Get next token --- we might already have it */
	if (have_lookahead)
	{
		cur_token = lookahead_token;
		have_lookahead = false;
	}
	else
		cur_token = base_yylex();

	/* Do we need to look ahead for a possible multiword token? */
	switch (cur_token)
	{
		case UNION:
			/* UNION JOIN must be reduced to a single UNIONJOIN token */
			lookahead_token = base_yylex();
			if (lookahead_token == JOIN)
				cur_token = UNIONJOIN;
			else
				have_lookahead = true;
			break;

		default:
			break;
	}

	return cur_token;
}
