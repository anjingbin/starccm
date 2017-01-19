/* A Bison parser, made from gram.y
   by GNU bison 1.35.  */

#define PLPGSQL_YYBISON 1  /* Identify Bison output.  */

# define	K_ALIAS	257
# define	K_ASSIGN	258
# define	K_BEGIN	259
# define	K_CLOSE	260
# define	K_CONSTANT	261
# define	K_CURSOR	262
# define	K_DEBUG	263
# define	K_DECLARE	264
# define	K_DEFAULT	265
# define	K_DIAGNOSTICS	266
# define	K_DOTDOT	267
# define	K_ELSE	268
# define	K_ELSIF	269
# define	K_END	270
# define	K_EXCEPTION	271
# define	K_EXECUTE	272
# define	K_EXIT	273
# define	K_FOR	274
# define	K_FETCH	275
# define	K_FROM	276
# define	K_GET	277
# define	K_IF	278
# define	K_IN	279
# define	K_INTO	280
# define	K_IS	281
# define	K_LOOP	282
# define	K_NOT	283
# define	K_NOTICE	284
# define	K_NULL	285
# define	K_OPEN	286
# define	K_PERFORM	287
# define	K_ROW_COUNT	288
# define	K_RAISE	289
# define	K_RECORD	290
# define	K_RENAME	291
# define	K_RESULT_OID	292
# define	K_RETURN	293
# define	K_REVERSE	294
# define	K_SELECT	295
# define	K_THEN	296
# define	K_TO	297
# define	K_TYPE	298
# define	K_WHEN	299
# define	K_WHILE	300
# define	T_FUNCTION	301
# define	T_TRIGGER	302
# define	T_LABEL	303
# define	T_STRING	304
# define	T_VARIABLE	305
# define	T_ROW	306
# define	T_ROWTYPE	307
# define	T_RECORD	308
# define	T_RECFIELD	309
# define	T_TGARGV	310
# define	T_DTYPE	311
# define	T_WORD	312
# define	T_NUMBER	313
# define	T_ERROR	314
# define	O_OPTION	315
# define	O_DUMP	316

#line 1 "gram.y"

/**********************************************************************
 * gram.y				- Parser for the PL/pgSQL
 *						  procedural language
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/pl/plpgsql/src/pl_gram.c,v 1.3 2004/07/06 07:19:07 AnJingBin Exp $
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


static	PLpgSQL_expr	*read_sql_construct(int until,
											const char *expected,
											bool isexpression,
											const char *sqlstart);
static	PLpgSQL_expr	*read_sql_stmt(const char *sqlstart);
static	PLpgSQL_type	*read_datatype(int tok);
static	PLpgSQL_stmt	*make_select_stmt(void);
static	PLpgSQL_stmt	*make_fetch_stmt(void);
static	PLpgSQL_expr	*make_tupret_expr(PLpgSQL_row *row);


#line 54 "gram.y"
#ifndef PLPGSQL_YYSTYPE
typedef union {
		int32					ival;
		char					*str;
		struct
		{
			char *name;
			int  lineno;
		}						varname;
		struct
		{
			int  nalloc;
			int  nused;
			int	 *nums;
		}						intlist;
		struct
		{
			int  nalloc;
			int  nused;
			PLpgSQL_diag_item *dtitems;
		}						dtlist;
		struct
		{
			int  reverse;
			PLpgSQL_expr *expr;
		}						forilow;
		struct
		{
			char *label;
			int  n_initvars;
			int  *initvarnos;
		}						declhdr;
		PLpgSQL_type			*dtype;
		PLpgSQL_var				*var;
		PLpgSQL_row				*row;
		PLpgSQL_rec				*rec;
		PLpgSQL_recfield		*recfield;
		PLpgSQL_trigarg			*trigarg;
		PLpgSQL_expr			*expr;
		PLpgSQL_stmt			*stmt;
		PLpgSQL_stmts			*stmts;
		PLpgSQL_stmt_block		*program;
		PLpgSQL_nsitem			*nsitem;
} plpgsql_yystype;
# define PLPGSQL_YYSTYPE plpgsql_yystype
# define PLPGSQL_YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef PLPGSQL_YYDEBUG
# define PLPGSQL_YYDEBUG 0
#endif



#define	PLPGSQL_YYFINAL		230
#define	PLPGSQL_YYFLAG		-32768
#define	PLPGSQL_YYNTBASE	69

/* PLPGSQL_YYTRANSLATE(PLPGSQL_YYLEX) -- Bison token number corresponding to PLPGSQL_YYLEX. */
#define PLPGSQL_YYTRANSLATE(x) ((unsigned)(x) <= 316 ? plpgsql_yytranslate[x] : 141)

/* PLPGSQL_YYTRANSLATE[PLPGSQL_YYLEX] -- Bison token number corresponding to PLPGSQL_YYLEX. */
static const char plpgsql_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      66,    67,     2,     2,    68,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    63,
      64,     2,    65,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62
};

#if PLPGSQL_YYDEBUG
static const short plpgsql_yyprhs[] =
{
       0,     0,     5,    10,    11,    13,    16,    18,    21,    22,
      24,    30,    32,    35,    39,    41,    44,    46,    52,    54,
      56,    62,    66,    70,    76,    82,    83,    91,    92,    93,
      97,    99,   103,   106,   108,   110,   112,   114,   116,   118,
     119,   121,   122,   123,   126,   128,   130,   132,   134,   135,
     137,   140,   142,   145,   147,   149,   151,   153,   155,   157,
     159,   161,   163,   165,   167,   169,   171,   173,   175,   177,
     179,   181,   185,   190,   196,   202,   206,   208,   210,   212,
     214,   216,   218,   227,   228,   234,   237,   242,   248,   257,
     259,   261,   263,   264,   273,   282,   284,   286,   289,   294,
     297,   304,   310,   312,   314,   316,   318,   321,   323,   326,
     329,   332,   337,   340,   344,   348,   353,   358,   360,   362,
     364,   366,   367,   368,   369,   370,   376,   377,   379,   381,
     384,   386
};
static const short plpgsql_yyrhs[] =
{
      47,    70,    74,    73,     0,    48,    70,    74,    73,     0,
       0,    71,     0,    71,    72,     0,    72,     0,    61,    62,
       0,     0,    63,     0,    75,     5,   140,    95,    16,     0,
     136,     0,   136,    76,     0,   136,    76,    77,     0,    10,
       0,    77,    78,     0,    78,     0,    64,    64,   139,    65,
      65,     0,    10,     0,    79,     0,    88,    90,    91,    92,
      93,     0,    88,    36,    63,     0,    88,    87,    63,     0,
      88,     3,    20,    86,    63,     0,    37,    89,    43,    89,
      63,     0,     0,    88,     8,    80,    82,    85,    41,    81,
       0,     0,     0,    66,    83,    67,     0,    84,     0,    83,
      68,    84,     0,    88,    91,     0,    27,     0,    20,     0,
      58,     0,    52,     0,    58,     0,    58,     0,     0,     7,
       0,     0,     0,    29,    31,     0,    63,     0,    94,     0,
       4,     0,    11,     0,     0,    96,     0,    96,    97,     0,
      97,     0,    74,    63,     0,    99,     0,   105,     0,   107,
       0,   108,     0,   109,     0,   113,     0,   116,     0,   117,
       0,   118,     0,   119,     0,   125,     0,   126,     0,   114,
       0,    98,     0,   100,     0,   127,     0,   128,     0,   129,
       0,    33,   140,   133,     0,   104,   140,     4,   133,     0,
      23,    12,   140,   101,    63,     0,   101,    68,   103,     4,
     102,     0,   103,     4,   102,     0,    34,     0,    38,     0,
      51,     0,    55,     0,    51,     0,    55,     0,    24,   140,
     134,    95,   106,    16,    24,    63,     0,     0,    15,   140,
     134,    95,   106,     0,    14,    95,     0,   136,    28,   140,
     124,     0,   136,    46,   140,   135,   124,     0,   136,    20,
     140,   110,    25,   112,   135,   124,     0,   111,     0,    51,
       0,    58,     0,     0,   136,    20,   140,   115,    25,    41,
     135,   124,     0,   136,    20,   140,   115,    25,    18,   135,
     124,     0,    54,     0,    52,     0,    41,   140,     0,    19,
     140,   137,   138,     0,    39,   140,     0,    35,   140,   121,
     120,   122,    63,     0,    35,   140,   121,   120,    63,     0,
      50,     0,    17,     0,    30,     0,     9,     0,   122,   123,
       0,   123,     0,    68,    51,     0,    68,    55,     0,    68,
      56,     0,    95,    16,    28,    63,     0,   132,   140,     0,
      18,   140,   133,     0,    32,   140,   130,     0,    21,   140,
     131,    26,     0,     6,   140,   131,    63,     0,    51,     0,
      51,     0,    58,     0,    60,     0,     0,     0,     0,     0,
      64,    64,   139,    65,    65,     0,     0,    49,     0,    63,
       0,    45,   133,     0,    58,     0,     0
};

#endif

#if PLPGSQL_YYDEBUG
/* PLPGSQL_YYRLINE[PLPGSQL_YYN] -- source line where rule number PLPGSQL_YYN was defined. */
static const short plpgsql_yyrline[] =
{
       0,   209,   213,   219,   220,   223,   224,   227,   233,   234,
     237,   258,   266,   274,   285,   291,   293,   297,   299,   301,
     305,   325,   339,   349,   354,   358,   358,   409,   421,   425,
     449,   475,   486,   509,   509,   512,   537,   543,   551,   558,
     560,   564,   575,   577,   581,   583,   639,   640,   642,   650,
     654,   664,   681,   683,   685,   687,   689,   691,   693,   695,
     697,   699,   701,   703,   705,   707,   709,   711,   713,   715,
     717,   721,   737,   753,   770,   785,   795,   799,   805,   814,
     821,   830,   836,   853,   861,   904,   910,   928,   947,   969,
     995,  1000,  1007,  1025,  1054,  1083,  1085,  1091,  1098,  1114,
    1161,  1177,  1194,  1200,  1204,  1208,  1214,  1227,  1236,  1240,
    1244,  1250,  1254,  1267,  1280,  1370,  1382,  1395,  1406,  1417,
    1419,  1423,  1427,  1431,  1435,  1440,  1447,  1449,  1453,  1455,
    1459,  1463
};
#endif


#if (PLPGSQL_YYDEBUG) || defined PLPGSQL_YYERROR_VERBOSE

/* PLPGSQL_YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const plpgsql_yytname[] =
{
  "$", "error", "$undefined.", "K_ALIAS", "K_ASSIGN", "K_BEGIN", "K_CLOSE", 
  "K_CONSTANT", "K_CURSOR", "K_DEBUG", "K_DECLARE", "K_DEFAULT", 
  "K_DIAGNOSTICS", "K_DOTDOT", "K_ELSE", "K_ELSIF", "K_END", 
  "K_EXCEPTION", "K_EXECUTE", "K_EXIT", "K_FOR", "K_FETCH", "K_FROM", 
  "K_GET", "K_IF", "K_IN", "K_INTO", "K_IS", "K_LOOP", "K_NOT", 
  "K_NOTICE", "K_NULL", "K_OPEN", "K_PERFORM", "K_ROW_COUNT", "K_RAISE", 
  "K_RECORD", "K_RENAME", "K_RESULT_OID", "K_RETURN", "K_REVERSE", 
  "K_SELECT", "K_THEN", "K_TO", "K_TYPE", "K_WHEN", "K_WHILE", 
  "T_FUNCTION", "T_TRIGGER", "T_LABEL", "T_STRING", "T_VARIABLE", "T_ROW", 
  "T_ROWTYPE", "T_RECORD", "T_RECFIELD", "T_TGARGV", "T_DTYPE", "T_WORD", 
  "T_NUMBER", "T_ERROR", "O_OPTION", "O_DUMP", "';'", "'<'", "'>'", "'('", 
  "')'", "','", "pl_function", "comp_optsect", "comp_options", 
  "comp_option", "opt_semi", "pl_block", "decl_sect", "decl_start", 
  "decl_stmts", "decl_stmt", "decl_statement", "@1", "decl_cursor_query", 
  "decl_cursor_args", "decl_cursor_arglist", "decl_cursor_arg", 
  "decl_is_from", "decl_aliasitem", "decl_rowtype", "decl_varname", 
  "decl_renname", "decl_const", "decl_datatype", "decl_notnull", 
  "decl_defval", "decl_defkey", "proc_sect", "proc_stmts", "proc_stmt", 
  "stmt_perform", "stmt_assign", "stmt_getdiag", "getdiag_list", 
  "getdiag_item", "getdiag_target", "assign_var", "stmt_if", "stmt_else", 
  "stmt_loop", "stmt_while", "stmt_fori", "fori_var", "fori_varname", 
  "fori_lower", "stmt_fors", "stmt_dynfors", "fors_target", "stmt_select", 
  "stmt_exit", "stmt_return", "stmt_raise", "raise_msg", "raise_level", 
  "raise_params", "raise_param", "loop_body", "stmt_execsql", 
  "stmt_dynexecute", "stmt_open", "stmt_fetch", "stmt_close", 
  "cursor_varptr", "cursor_variable", "execsql_start", "expr_until_semi", 
  "expr_until_then", "expr_until_loop", "opt_label", "opt_exitlabel", 
  "opt_exitcond", "opt_lblname", "lno", 0
};
#endif

/* PLPGSQL_YYR1[PLPGSQL_YYN] -- Symbol number of symbol that rule PLPGSQL_YYN derives. */
static const short plpgsql_yyr1[] =
{
       0,    69,    69,    70,    70,    71,    71,    72,    73,    73,
      74,    75,    75,    75,    76,    77,    77,    78,    78,    78,
      79,    79,    79,    79,    79,    80,    79,    81,    82,    82,
      83,    83,    84,    85,    85,    86,    87,    88,    89,    90,
      90,    91,    92,    92,    93,    93,    94,    94,    95,    95,
      96,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    98,    99,   100,   101,   101,   102,   102,   103,   103,
     104,   104,   105,   106,   106,   106,   107,   108,   109,   110,
     111,   111,   112,   113,   114,   115,   115,   116,   117,   118,
     119,   119,   120,   121,   121,   121,   122,   122,   123,   123,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     132,   133,   134,   135,   136,   136,   137,   137,   138,   138,
     139,   140
};

/* PLPGSQL_YYR2[PLPGSQL_YYN] -- Number of symbols composing right hand side of rule PLPGSQL_YYN. */
static const short plpgsql_yyr2[] =
{
       0,     4,     4,     0,     1,     2,     1,     2,     0,     1,
       5,     1,     2,     3,     1,     2,     1,     5,     1,     1,
       5,     3,     3,     5,     5,     0,     7,     0,     0,     3,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     0,
       1,     0,     0,     2,     1,     1,     1,     1,     0,     1,
       2,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     5,     5,     3,     1,     1,     1,     1,
       1,     1,     8,     0,     5,     2,     4,     5,     8,     1,
       1,     1,     0,     8,     8,     1,     1,     2,     4,     2,
       6,     5,     1,     1,     1,     1,     2,     1,     2,     2,
       2,     4,     2,     3,     3,     4,     4,     1,     1,     1,
       1,     0,     0,     0,     0,     5,     0,     1,     1,     2,
       1,     0
};

/* PLPGSQL_YYDEFACT[S] -- default rule to reduce with in state S when PLPGSQL_YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short plpgsql_yydefact[] =
{
       0,     3,     3,     0,   124,     4,     6,   124,     7,     0,
       8,     0,    11,     5,     8,     0,     9,     1,   131,    14,
      12,     2,   130,     0,   124,    18,     0,    37,     0,    13,
      16,    19,    39,     0,   131,   131,   131,   131,     0,   131,
     131,   131,   131,   131,   131,    80,    81,   119,   120,     0,
       0,   124,    51,    66,    53,    67,   131,    54,    55,    56,
      57,    58,    65,    59,    60,    61,    62,    63,    64,    68,
      69,    70,   131,    11,    38,     0,     0,    15,     0,    40,
      25,     0,    36,     0,    41,   125,     0,   121,   126,     0,
     131,   122,     0,   121,     0,    99,    97,    52,    10,    50,
       0,   112,   131,   131,   131,     0,     0,     0,    28,    21,
      22,    42,   118,     0,   113,   127,     0,     0,     0,   124,
     117,   114,    71,   105,   103,   104,     0,   121,     0,   124,
     123,     0,     0,    35,     0,     0,     0,     0,     0,   116,
     121,   128,    98,   115,    78,    79,     0,     0,    83,   102,
       0,    72,    90,    96,    95,    91,     0,    89,     0,     0,
      86,   124,    24,    17,    23,     0,    30,    41,    34,    33,
       0,    43,    46,    47,    44,    20,    45,   129,    73,     0,
       0,   124,   131,     0,   101,     0,     0,   107,    92,     0,
       0,    87,    29,     0,    32,    27,     0,    76,    77,    75,
      85,   122,     0,   108,   109,   110,   100,   106,   123,   123,
     123,     0,    31,    26,     0,   124,     0,   124,   124,   124,
     111,    74,    83,    82,    88,    94,    93,    84,     0,     0,
       0
};

static const short plpgsql_yydefgoto[] =
{
     228,     4,     5,     6,    17,    49,    11,    20,    29,    30,
      31,   108,   213,   136,   165,   166,   170,   134,    83,    32,
      75,    84,   111,   138,   175,   176,   159,    51,    52,    53,
      54,    55,   146,   199,   147,    56,    57,   183,    58,    59,
      60,   156,   157,   208,    61,    62,   158,    63,    64,    65,
      66,   150,   126,   186,   187,   160,    67,    68,    69,    70,
      71,   121,   113,    72,   114,   119,   161,    73,   116,   142,
      23,    24
};

static const short plpgsql_yypact[] =
{
      51,   -43,   -43,   -35,   -29,   -43,-32768,   -29,-32768,   -18,
      -9,    57,    68,-32768,    -9,    27,-32768,-32768,-32768,-32768,
      18,-32768,-32768,    28,   111,-32768,    44,-32768,    40,    18,
  -32768,-32768,   112,    48,-32768,-32768,-32768,-32768,   104,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    77,
     105,    26,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,     5,-32768,    93,    27,-32768,   121,-32768,
  -32768,    79,-32768,    82,-32768,-32768,    96,-32768,   102,    96,
  -32768,-32768,   103,-32768,    13,-32768,-32768,-32768,-32768,-32768,
     149,-32768,-32768,-32768,-32768,    44,    90,    98,    92,-32768,
  -32768,   130,-32768,    97,-32768,-32768,   -25,   135,    -3,    73,
  -32768,-32768,-32768,-32768,-32768,-32768,   113,-32768,    49,   111,
  -32768,   107,   100,-32768,   109,   110,    -6,   136,     1,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   -39,   169,   124,-32768,
     -37,-32768,-32768,-32768,-32768,-32768,   151,-32768,   152,   158,
  -32768,   111,-32768,-32768,-32768,    58,-32768,-32768,-32768,-32768,
     137,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    -3,
      22,   111,-32768,   163,-32768,    67,     0,-32768,-32768,    -5,
     153,-32768,-32768,   110,-32768,-32768,   176,-32768,-32768,-32768,
  -32768,-32768,   159,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   119,-32768,-32768,    22,    73,   122,   111,   111,   111,
  -32768,-32768,   124,-32768,-32768,-32768,-32768,-32768,   184,   186,
  -32768
};

static const short plpgsql_yypgoto[] =
{
  -32768,   185,-32768,   183,   175,    30,-32768,-32768,-32768,   161,
  -32768,-32768,-32768,-32768,-32768,    -1,-32768,-32768,-32768,  -118,
      88,-32768,    29,-32768,-32768,-32768,   -24,-32768,   143,-32768,
  -32768,-32768,-32768,   -19,    19,-32768,-32768,   -23,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,    11,  -145,-32768,-32768,-32768,-32768,
  -32768,-32768,   114,-32768,   -74,     3,   -99,    76,-32768,-32768,
     125,   -33
};


#define	PLPGSQL_YYLAST		204


static const short plpgsql_yytable[] =
{
      50,    86,    87,    88,    89,   172,    91,    92,    93,    94,
      95,    96,   173,   209,   168,    19,   191,   167,     3,   122,
     140,   169,   123,   100,   178,   102,   184,     8,    25,   179,
     124,   185,    34,   103,    10,     9,   210,    14,   141,   101,
     -49,   -49,   -49,   125,    35,    36,    15,    37,   144,    38,
      39,   104,   145,   151,    16,    26,   197,   118,    40,    41,
     198,    42,    18,   206,   174,    43,   177,    44,   185,   128,
     129,   130,   224,   225,   226,   167,    27,    45,    19,    34,
      12,    46,    28,    12,    47,    22,    48,   -48,   -48,   -48,
       9,    35,    36,    33,    37,   148,    38,    39,     1,     2,
     152,   153,    74,   154,    76,    40,    41,   155,    42,   217,
     218,   219,    43,    85,    44,    78,    90,    34,   203,    79,
      80,    98,   204,   205,    45,   192,   193,   -48,    46,    35,
      36,    47,    37,    48,    38,    39,   105,     9,   181,   182,
      97,   107,   109,    40,    41,   110,    42,   112,    81,   201,
      43,   115,    44,   127,   120,   132,   133,   200,   135,   137,
     139,   143,    45,   149,    82,   163,    46,   171,    27,    47,
     162,    48,   164,   180,   190,     9,   188,   189,   195,   202,
     214,   211,   220,   216,   229,   223,   230,     7,    13,    21,
      77,   222,   212,   131,    99,   221,   194,   207,   196,   227,
       0,   106,     0,   117,   215
};

static const short plpgsql_yycheck[] =
{
      24,    34,    35,    36,    37,     4,    39,    40,    41,    42,
      43,    44,    11,    18,    20,    10,   161,   135,    61,    93,
      45,    27,     9,    56,    63,    20,    63,    62,    10,    68,
      17,    68,     6,    28,     4,    64,    41,     7,    63,    72,
      14,    15,    16,    30,    18,    19,    64,    21,    51,    23,
      24,    46,    55,   127,    63,    37,    34,    90,    32,    33,
      38,    35,     5,    63,    63,    39,   140,    41,    68,   102,
     103,   104,   217,   218,   219,   193,    58,    51,    10,     6,
       4,    55,    64,     7,    58,    58,    60,    14,    15,    16,
      64,    18,    19,    65,    21,   119,    23,    24,    47,    48,
      51,    52,    58,    54,    64,    32,    33,    58,    35,   208,
     209,   210,    39,    65,    41,     3,    12,     6,    51,     7,
       8,    16,    55,    56,    51,    67,    68,    16,    55,    18,
      19,    58,    21,    60,    23,    24,    43,    64,    14,    15,
      63,    20,    63,    32,    33,    63,    35,    51,    36,   182,
      39,    49,    41,     4,    51,    65,    58,   181,    66,    29,
      63,    26,    51,    50,    52,    65,    55,    31,    58,    58,
      63,    60,    63,     4,    16,    64,    25,    25,    41,    16,
       4,    28,    63,    24,     0,    63,     0,     2,     5,    14,
      29,   215,   193,   105,    51,   214,   167,   186,   179,   222,
      -1,    76,    -1,    89,   201
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with plpgsql_yy or PLPGSQL_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (plpgsql_yyoverflow) || defined (PLPGSQL_YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if PLPGSQL_YYSTACK_USE_ALLOCA
#  define PLPGSQL_YYSTACK_ALLOC alloca
# else
#  ifndef PLPGSQL_YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define PLPGSQL_YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define PLPGSQL_YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef PLPGSQL_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define PLPGSQL_YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define PLPGSQL_YYSIZE_T size_t
#  endif
#  define PLPGSQL_YYSTACK_ALLOC malloc
#  define PLPGSQL_YYSTACK_FREE free
# endif
#endif /* ! defined (plpgsql_yyoverflow) || defined (PLPGSQL_YYERROR_VERBOSE) */


#if (! defined (plpgsql_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (PLPGSQL_YYLTYPE_IS_TRIVIAL && PLPGSQL_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union plpgsql_yyalloc
{
  short plpgsql_yyss;
  PLPGSQL_YYSTYPE plpgsql_yyvs;
# if PLPGSQL_YYLSP_NEEDED
  PLPGSQL_YYLTYPE plpgsql_yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define PLPGSQL_YYSTACK_GAP_MAX (sizeof (union plpgsql_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if PLPGSQL_YYLSP_NEEDED
#  define PLPGSQL_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (PLPGSQL_YYSTYPE) + sizeof (PLPGSQL_YYLTYPE))	\
      + 2 * PLPGSQL_YYSTACK_GAP_MAX)
# else
#  define PLPGSQL_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (PLPGSQL_YYSTYPE))				\
      + PLPGSQL_YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef PLPGSQL_YYCOPY
#  if 1 < __GNUC__
#   define PLPGSQL_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define PLPGSQL_YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register PLPGSQL_YYSIZE_T plpgsql_yyi;		\
	  for (plpgsql_yyi = 0; plpgsql_yyi < (Count); plpgsql_yyi++)	\
	    (To)[plpgsql_yyi] = (From)[plpgsql_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables PLPGSQL_YYSIZE and PLPGSQL_YYSTACKSIZE give the old and new number of
   elements in the stack, and PLPGSQL_YYPTR gives the new location of the
   stack.  Advance PLPGSQL_YYPTR to a properly aligned location for the next
   stack.  */
# define PLPGSQL_YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	PLPGSQL_YYSIZE_T plpgsql_yynewbytes;						\
	PLPGSQL_YYCOPY (&plpgsql_yyptr->Stack, Stack, plpgsql_yysize);				\
	Stack = &plpgsql_yyptr->Stack;						\
	plpgsql_yynewbytes = plpgsql_yystacksize * sizeof (*Stack) + PLPGSQL_YYSTACK_GAP_MAX;	\
	plpgsql_yyptr += plpgsql_yynewbytes / sizeof (*plpgsql_yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (PLPGSQL_YYSIZE_T) && defined (__SIZE_TYPE__)
# define PLPGSQL_YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (PLPGSQL_YYSIZE_T) && defined (size_t)
# define PLPGSQL_YYSIZE_T size_t
#endif
#if ! defined (PLPGSQL_YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define PLPGSQL_YYSIZE_T size_t
# endif
#endif
#if ! defined (PLPGSQL_YYSIZE_T)
# define PLPGSQL_YYSIZE_T unsigned int
#endif

#define plpgsql_yyerrok		(plpgsql_yyerrstatus = 0)
#define plpgsql_yyclearin	(plpgsql_yychar = PLPGSQL_YYEMPTY)
#define PLPGSQL_YYEMPTY		-2
#define PLPGSQL_YYEOF		0
#define PLPGSQL_YYACCEPT	goto plpgsql_yyacceptlab
#define PLPGSQL_YYABORT 	goto plpgsql_yyabortlab
#define PLPGSQL_YYERROR		goto plpgsql_yyerrlab1
/* Like PLPGSQL_YYERROR except do call plpgsql_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of PLPGSQL_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define PLPGSQL_YYFAIL		goto plpgsql_yyerrlab
#define PLPGSQL_YYRECOVERING()  (!!plpgsql_yyerrstatus)
#define PLPGSQL_YYBACKUP(Token, Value)					\
do								\
  if (plpgsql_yychar == PLPGSQL_YYEMPTY && plpgsql_yylen == 1)				\
    {								\
      plpgsql_yychar = (Token);						\
      plpgsql_yylval = (Value);						\
      plpgsql_yychar1 = PLPGSQL_YYTRANSLATE (plpgsql_yychar);				\
      PLPGSQL_YYPOPSTACK;						\
      goto plpgsql_yybackup;						\
    }								\
  else								\
    { 								\
      plpgsql_yyerror ("syntax error: cannot back up");			\
      PLPGSQL_YYERROR;							\
    }								\
while (0)

#define PLPGSQL_YYTERROR	1
#define PLPGSQL_YYERRCODE	256


/* PLPGSQL_YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When PLPGSQL_YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef PLPGSQL_YYLLOC_DEFAULT
# define PLPGSQL_YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* PLPGSQL_YYLEX -- calling `plpgsql_yylex' with the right arguments.  */

#if PLPGSQL_YYPURE
# if PLPGSQL_YYLSP_NEEDED
#  ifdef PLPGSQL_YYLEX_PARAM
#   define PLPGSQL_YYLEX		plpgsql_yylex (&plpgsql_yylval, &plpgsql_yylloc, PLPGSQL_YYLEX_PARAM)
#  else
#   define PLPGSQL_YYLEX		plpgsql_yylex (&plpgsql_yylval, &plpgsql_yylloc)
#  endif
# else /* !PLPGSQL_YYLSP_NEEDED */
#  ifdef PLPGSQL_YYLEX_PARAM
#   define PLPGSQL_YYLEX		plpgsql_yylex (&plpgsql_yylval, PLPGSQL_YYLEX_PARAM)
#  else
#   define PLPGSQL_YYLEX		plpgsql_yylex (&plpgsql_yylval)
#  endif
# endif /* !PLPGSQL_YYLSP_NEEDED */
#else /* !PLPGSQL_YYPURE */
# define PLPGSQL_YYLEX			plpgsql_yylex ()
#endif /* !PLPGSQL_YYPURE */


/* Enable debugging if requested.  */
#if PLPGSQL_YYDEBUG

# ifndef PLPGSQL_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define PLPGSQL_YYFPRINTF fprintf
# endif

# define PLPGSQL_YYDPRINTF(Args)			\
do {						\
  if (plpgsql_yydebug)					\
    PLPGSQL_YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int plpgsql_yydebug;
#else /* !PLPGSQL_YYDEBUG */
# define PLPGSQL_YYDPRINTF(Args)
#endif /* !PLPGSQL_YYDEBUG */

/* PLPGSQL_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	PLPGSQL_YYINITDEPTH
# define PLPGSQL_YYINITDEPTH 200
#endif

/* PLPGSQL_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < PLPGSQL_YYSTACK_BYTES (PLPGSQL_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if PLPGSQL_YYMAXDEPTH == 0
# undef PLPGSQL_YYMAXDEPTH
#endif

#ifndef PLPGSQL_YYMAXDEPTH
# define PLPGSQL_YYMAXDEPTH 10000
#endif

#ifdef PLPGSQL_YYERROR_VERBOSE

# ifndef plpgsql_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define plpgsql_yystrlen strlen
#  else
/* Return the length of PLPGSQL_YYSTR.  */
static PLPGSQL_YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
plpgsql_yystrlen (const char *plpgsql_yystr)
#   else
plpgsql_yystrlen (plpgsql_yystr)
     const char *plpgsql_yystr;
#   endif
{
  register const char *plpgsql_yys = plpgsql_yystr;

  while (*plpgsql_yys++ != '\0')
    continue;

  return plpgsql_yys - plpgsql_yystr - 1;
}
#  endif
# endif

# ifndef plpgsql_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define plpgsql_yystpcpy stpcpy
#  else
/* Copy PLPGSQL_YYSRC to PLPGSQL_YYDEST, returning the address of the terminating '\0' in
   PLPGSQL_YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
plpgsql_yystpcpy (char *plpgsql_yydest, const char *plpgsql_yysrc)
#   else
plpgsql_yystpcpy (plpgsql_yydest, plpgsql_yysrc)
     char *plpgsql_yydest;
     const char *plpgsql_yysrc;
#   endif
{
  register char *plpgsql_yyd = plpgsql_yydest;
  register const char *plpgsql_yys = plpgsql_yysrc;

  while ((*plpgsql_yyd++ = *plpgsql_yys++) != '\0')
    continue;

  return plpgsql_yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define PLPGSQL_YYPARSE_PARAM as the name of an argument to be passed
   into plpgsql_yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef PLPGSQL_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define PLPGSQL_YYPARSE_PARAM_ARG void *PLPGSQL_YYPARSE_PARAM
#  define PLPGSQL_YYPARSE_PARAM_DECL
# else
#  define PLPGSQL_YYPARSE_PARAM_ARG PLPGSQL_YYPARSE_PARAM
#  define PLPGSQL_YYPARSE_PARAM_DECL void *PLPGSQL_YYPARSE_PARAM;
# endif
#else /* !PLPGSQL_YYPARSE_PARAM */
# define PLPGSQL_YYPARSE_PARAM_ARG
# define PLPGSQL_YYPARSE_PARAM_DECL
#endif /* !PLPGSQL_YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef PLPGSQL_YYPARSE_PARAM
int plpgsql_yyparse (void *);
# else
int plpgsql_yyparse (void);
# endif
#endif

/* PLPGSQL_YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to PLPGSQL_YYPARSE.  */

#define PLPGSQL_YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int plpgsql_yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
PLPGSQL_YYSTYPE plpgsql_yylval;						\
							\
/* Number of parse errors so far.  */			\
int plpgsql_yynerrs;

#if PLPGSQL_YYLSP_NEEDED
# define PLPGSQL_YY_DECL_VARIABLES			\
PLPGSQL_YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
PLPGSQL_YYLTYPE plpgsql_yylloc;
#else
# define PLPGSQL_YY_DECL_VARIABLES			\
PLPGSQL_YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !PLPGSQL_YYPURE
PLPGSQL_YY_DECL_VARIABLES
#endif  /* !PLPGSQL_YYPURE */

int
plpgsql_yyparse (PLPGSQL_YYPARSE_PARAM_ARG)
     PLPGSQL_YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if PLPGSQL_YYPURE
  PLPGSQL_YY_DECL_VARIABLES
#endif  /* !PLPGSQL_YYPURE */

  register int plpgsql_yystate;
  register int plpgsql_yyn;
  int plpgsql_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int plpgsql_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int plpgsql_yychar1 = 0;

  /* Three stacks and their tools:
     `plpgsql_yyss': related to states,
     `plpgsql_yyvs': related to semantic values,
     `plpgsql_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow plpgsql_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	plpgsql_yyssa[PLPGSQL_YYINITDEPTH];
  short *plpgsql_yyss = plpgsql_yyssa;
  register short *plpgsql_yyssp;

  /* The semantic value stack.  */
  PLPGSQL_YYSTYPE plpgsql_yyvsa[PLPGSQL_YYINITDEPTH];
  PLPGSQL_YYSTYPE *plpgsql_yyvs = plpgsql_yyvsa;
  register PLPGSQL_YYSTYPE *plpgsql_yyvsp;

#if PLPGSQL_YYLSP_NEEDED
  /* The location stack.  */
  PLPGSQL_YYLTYPE plpgsql_yylsa[PLPGSQL_YYINITDEPTH];
  PLPGSQL_YYLTYPE *plpgsql_yyls = plpgsql_yylsa;
  PLPGSQL_YYLTYPE *plpgsql_yylsp;
#endif

#if PLPGSQL_YYLSP_NEEDED
# define PLPGSQL_YYPOPSTACK   (plpgsql_yyvsp--, plpgsql_yyssp--, plpgsql_yylsp--)
#else
# define PLPGSQL_YYPOPSTACK   (plpgsql_yyvsp--, plpgsql_yyssp--)
#endif

  PLPGSQL_YYSIZE_T plpgsql_yystacksize = PLPGSQL_YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  PLPGSQL_YYSTYPE plpgsql_yyval;
#if PLPGSQL_YYLSP_NEEDED
  PLPGSQL_YYLTYPE plpgsql_yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int plpgsql_yylen;

  PLPGSQL_YYDPRINTF ((stderr, "Starting parse\n"));

  plpgsql_yystate = 0;
  plpgsql_yyerrstatus = 0;
  plpgsql_yynerrs = 0;
  plpgsql_yychar = PLPGSQL_YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  plpgsql_yyssp = plpgsql_yyss;
  plpgsql_yyvsp = plpgsql_yyvs;
#if PLPGSQL_YYLSP_NEEDED
  plpgsql_yylsp = plpgsql_yyls;
#endif
  goto plpgsql_yysetstate;

/*------------------------------------------------------------.
| plpgsql_yynewstate -- Push a new state, which is found in plpgsql_yystate.  |
`------------------------------------------------------------*/
 plpgsql_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  plpgsql_yyssp++;

 plpgsql_yysetstate:
  *plpgsql_yyssp = plpgsql_yystate;

  if (plpgsql_yyssp >= plpgsql_yyss + plpgsql_yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      PLPGSQL_YYSIZE_T plpgsql_yysize = plpgsql_yyssp - plpgsql_yyss + 1;

#ifdef plpgsql_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	PLPGSQL_YYSTYPE *plpgsql_yyvs1 = plpgsql_yyvs;
	short *plpgsql_yyss1 = plpgsql_yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if PLPGSQL_YYLSP_NEEDED
	PLPGSQL_YYLTYPE *plpgsql_yyls1 = plpgsql_yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if plpgsql_yyoverflow is a macro.  */
	plpgsql_yyoverflow ("parser stack overflow",
		    &plpgsql_yyss1, plpgsql_yysize * sizeof (*plpgsql_yyssp),
		    &plpgsql_yyvs1, plpgsql_yysize * sizeof (*plpgsql_yyvsp),
		    &plpgsql_yyls1, plpgsql_yysize * sizeof (*plpgsql_yylsp),
		    &plpgsql_yystacksize);
	plpgsql_yyls = plpgsql_yyls1;
# else
	plpgsql_yyoverflow ("parser stack overflow",
		    &plpgsql_yyss1, plpgsql_yysize * sizeof (*plpgsql_yyssp),
		    &plpgsql_yyvs1, plpgsql_yysize * sizeof (*plpgsql_yyvsp),
		    &plpgsql_yystacksize);
# endif
	plpgsql_yyss = plpgsql_yyss1;
	plpgsql_yyvs = plpgsql_yyvs1;
      }
#else /* no plpgsql_yyoverflow */
# ifndef PLPGSQL_YYSTACK_RELOCATE
      goto plpgsql_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (plpgsql_yystacksize >= PLPGSQL_YYMAXDEPTH)
	goto plpgsql_yyoverflowlab;
      plpgsql_yystacksize *= 2;
      if (plpgsql_yystacksize > PLPGSQL_YYMAXDEPTH)
	plpgsql_yystacksize = PLPGSQL_YYMAXDEPTH;

      {
	short *plpgsql_yyss1 = plpgsql_yyss;
	union plpgsql_yyalloc *plpgsql_yyptr =
	  (union plpgsql_yyalloc *) PLPGSQL_YYSTACK_ALLOC (PLPGSQL_YYSTACK_BYTES (plpgsql_yystacksize));
	if (! plpgsql_yyptr)
	  goto plpgsql_yyoverflowlab;
	PLPGSQL_YYSTACK_RELOCATE (plpgsql_yyss);
	PLPGSQL_YYSTACK_RELOCATE (plpgsql_yyvs);
# if PLPGSQL_YYLSP_NEEDED
	PLPGSQL_YYSTACK_RELOCATE (plpgsql_yyls);
# endif
# undef PLPGSQL_YYSTACK_RELOCATE
	if (plpgsql_yyss1 != plpgsql_yyssa)
	  PLPGSQL_YYSTACK_FREE (plpgsql_yyss1);
      }
# endif
#endif /* no plpgsql_yyoverflow */

      plpgsql_yyssp = plpgsql_yyss + plpgsql_yysize - 1;
      plpgsql_yyvsp = plpgsql_yyvs + plpgsql_yysize - 1;
#if PLPGSQL_YYLSP_NEEDED
      plpgsql_yylsp = plpgsql_yyls + plpgsql_yysize - 1;
#endif

      PLPGSQL_YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) plpgsql_yystacksize));

      if (plpgsql_yyssp >= plpgsql_yyss + plpgsql_yystacksize - 1)
	PLPGSQL_YYABORT;
    }

  PLPGSQL_YYDPRINTF ((stderr, "Entering state %d\n", plpgsql_yystate));

  goto plpgsql_yybackup;


/*-----------.
| plpgsql_yybackup.  |
`-----------*/
plpgsql_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* plpgsql_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  plpgsql_yyn = plpgsql_yypact[plpgsql_yystate];
  if (plpgsql_yyn == PLPGSQL_YYFLAG)
    goto plpgsql_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* plpgsql_yychar is either PLPGSQL_YYEMPTY or PLPGSQL_YYEOF
     or a valid token in external form.  */

  if (plpgsql_yychar == PLPGSQL_YYEMPTY)
    {
      PLPGSQL_YYDPRINTF ((stderr, "Reading a token: "));
      plpgsql_yychar = PLPGSQL_YYLEX;
    }

  /* Convert token to internal form (in plpgsql_yychar1) for indexing tables with */

  if (plpgsql_yychar <= 0)		/* This means end of input. */
    {
      plpgsql_yychar1 = 0;
      plpgsql_yychar = PLPGSQL_YYEOF;		/* Don't call PLPGSQL_YYLEX any more */

      PLPGSQL_YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      plpgsql_yychar1 = PLPGSQL_YYTRANSLATE (plpgsql_yychar);

#if PLPGSQL_YYDEBUG
     /* We have to keep this `#if PLPGSQL_YYDEBUG', since we use variables
	which are defined only if `PLPGSQL_YYDEBUG' is set.  */
      if (plpgsql_yydebug)
	{
	  PLPGSQL_YYFPRINTF (stderr, "Next token is %d (%s",
		     plpgsql_yychar, plpgsql_yytname[plpgsql_yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef PLPGSQL_YYPRINT
	  PLPGSQL_YYPRINT (stderr, plpgsql_yychar, plpgsql_yylval);
# endif
	  PLPGSQL_YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  plpgsql_yyn += plpgsql_yychar1;
  if (plpgsql_yyn < 0 || plpgsql_yyn > PLPGSQL_YYLAST || plpgsql_yycheck[plpgsql_yyn] != plpgsql_yychar1)
    goto plpgsql_yydefault;

  plpgsql_yyn = plpgsql_yytable[plpgsql_yyn];

  /* plpgsql_yyn is what to do for this token type in this state.
     Negative => reduce, -plpgsql_yyn is rule number.
     Positive => shift, plpgsql_yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (plpgsql_yyn < 0)
    {
      if (plpgsql_yyn == PLPGSQL_YYFLAG)
	goto plpgsql_yyerrlab;
      plpgsql_yyn = -plpgsql_yyn;
      goto plpgsql_yyreduce;
    }
  else if (plpgsql_yyn == 0)
    goto plpgsql_yyerrlab;

  if (plpgsql_yyn == PLPGSQL_YYFINAL)
    PLPGSQL_YYACCEPT;

  /* Shift the lookahead token.  */
  PLPGSQL_YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      plpgsql_yychar, plpgsql_yytname[plpgsql_yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (plpgsql_yychar != PLPGSQL_YYEOF)
    plpgsql_yychar = PLPGSQL_YYEMPTY;

  *++plpgsql_yyvsp = plpgsql_yylval;
#if PLPGSQL_YYLSP_NEEDED
  *++plpgsql_yylsp = plpgsql_yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (plpgsql_yyerrstatus)
    plpgsql_yyerrstatus--;

  plpgsql_yystate = plpgsql_yyn;
  goto plpgsql_yynewstate;


/*-----------------------------------------------------------.
| plpgsql_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
plpgsql_yydefault:
  plpgsql_yyn = plpgsql_yydefact[plpgsql_yystate];
  if (plpgsql_yyn == 0)
    goto plpgsql_yyerrlab;
  goto plpgsql_yyreduce;


/*-----------------------------.
| plpgsql_yyreduce -- Do a reduction.  |
`-----------------------------*/
plpgsql_yyreduce:
  /* plpgsql_yyn is the number of a rule to reduce with.  */
  plpgsql_yylen = plpgsql_yyr2[plpgsql_yyn];

  /* If PLPGSQL_YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets PLPGSQL_YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to PLPGSQL_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that PLPGSQL_YYVAL may be used uninitialized.  */
  plpgsql_yyval = plpgsql_yyvsp[1-plpgsql_yylen];

#if PLPGSQL_YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  plpgsql_yyloc = plpgsql_yylsp[1-plpgsql_yylen];
  PLPGSQL_YYLLOC_DEFAULT (plpgsql_yyloc, (plpgsql_yylsp - plpgsql_yylen), plpgsql_yylen);
#endif

#if PLPGSQL_YYDEBUG
  /* We have to keep this `#if PLPGSQL_YYDEBUG', since we use variables which
     are defined only if `PLPGSQL_YYDEBUG' is set.  */
  if (plpgsql_yydebug)
    {
      int plpgsql_yyi;

      PLPGSQL_YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 plpgsql_yyn, plpgsql_yyrline[plpgsql_yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (plpgsql_yyi = plpgsql_yyprhs[plpgsql_yyn]; plpgsql_yyrhs[plpgsql_yyi] > 0; plpgsql_yyi++)
	PLPGSQL_YYFPRINTF (stderr, "%s ", plpgsql_yytname[plpgsql_yyrhs[plpgsql_yyi]]);
      PLPGSQL_YYFPRINTF (stderr, " -> %s\n", plpgsql_yytname[plpgsql_yyr1[plpgsql_yyn]]);
    }
#endif

  switch (plpgsql_yyn) {

case 1:
#line 210 "gram.y"
{
						plpgsql_yylval.program = (PLpgSQL_stmt_block *)plpgsql_yyvsp[-1].stmt;
					}
    break;
case 2:
#line 214 "gram.y"
{
						plpgsql_yylval.program = (PLpgSQL_stmt_block *)plpgsql_yyvsp[-1].stmt;
					}
    break;
case 7:
#line 228 "gram.y"
{
						plpgsql_DumpExecTree = 1;
					}
    break;
case 10:
#line 238 "gram.y"
{
						PLpgSQL_stmt_block *new;

						new = malloc(sizeof(PLpgSQL_stmt_block));
						memset(new, 0, sizeof(PLpgSQL_stmt_block));

						new->cmd_type	= PLPGSQL_STMT_BLOCK;
						new->lineno		= plpgsql_yyvsp[-2].ival;
						new->label		= plpgsql_yyvsp[-4].declhdr.label;
						new->n_initvars = plpgsql_yyvsp[-4].declhdr.n_initvars;
						new->initvarnos = plpgsql_yyvsp[-4].declhdr.initvarnos;
						new->body		= plpgsql_yyvsp[-1].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 11:
#line 259 "gram.y"
{
						plpgsql_ns_setlocal(false);
						plpgsql_yyval.declhdr.label	  = plpgsql_yyvsp[0].str;
						plpgsql_yyval.declhdr.n_initvars = 0;
						plpgsql_yyval.declhdr.initvarnos = NULL;
						plpgsql_add_initdatums(NULL);
					}
    break;
case 12:
#line 267 "gram.y"
{
						plpgsql_ns_setlocal(false);
						plpgsql_yyval.declhdr.label	  = plpgsql_yyvsp[-1].str;
						plpgsql_yyval.declhdr.n_initvars = 0;
						plpgsql_yyval.declhdr.initvarnos = NULL;
						plpgsql_add_initdatums(NULL);
					}
    break;
case 13:
#line 275 "gram.y"
{
						plpgsql_ns_setlocal(false);
						if (plpgsql_yyvsp[0].str != NULL)
							plpgsql_yyval.declhdr.label = plpgsql_yyvsp[0].str;
						else
							plpgsql_yyval.declhdr.label = plpgsql_yyvsp[-2].str;
						plpgsql_yyval.declhdr.n_initvars = plpgsql_add_initdatums(&(plpgsql_yyval.declhdr.initvarnos));
					}
    break;
case 14:
#line 286 "gram.y"
{
						plpgsql_ns_setlocal(true);
					}
    break;
case 15:
#line 292 "gram.y"
{	plpgsql_yyval.str = plpgsql_yyvsp[0].str;	}
    break;
case 16:
#line 294 "gram.y"
{	plpgsql_yyval.str = plpgsql_yyvsp[0].str;	}
    break;
case 17:
#line 298 "gram.y"
{	plpgsql_yyval.str = plpgsql_yyvsp[-2].str;	}
    break;
case 18:
#line 300 "gram.y"
{	plpgsql_yyval.str = NULL;	}
    break;
case 19:
#line 302 "gram.y"
{	plpgsql_yyval.str = NULL;	}
    break;
case 20:
#line 306 "gram.y"
{
						PLpgSQL_var		*new;

						new = malloc(sizeof(PLpgSQL_var));
						memset(new, 0, sizeof(PLpgSQL_var));

						new->dtype		= PLPGSQL_DTYPE_VAR;
						new->refname	= plpgsql_yyvsp[-4].varname.name;
						new->lineno		= plpgsql_yyvsp[-4].varname.lineno;

						new->datatype	= plpgsql_yyvsp[-2].dtype;
						new->isconst	= plpgsql_yyvsp[-3].ival;
						new->notnull	= plpgsql_yyvsp[-1].ival;
						new->default_val = plpgsql_yyvsp[0].expr;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, new->varno,
												plpgsql_yyvsp[-4].varname.name);
					}
    break;
case 21:
#line 326 "gram.y"
{
						PLpgSQL_rec		*new;

						new = malloc(sizeof(PLpgSQL_rec));

						new->dtype		= PLPGSQL_DTYPE_REC;
						new->refname	= plpgsql_yyvsp[-2].varname.name;
						new->lineno		= plpgsql_yyvsp[-2].varname.lineno;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_REC, new->recno,
												plpgsql_yyvsp[-2].varname.name);
					}
    break;
case 22:
#line 340 "gram.y"
{
						plpgsql_yyvsp[-1].row->dtype		= PLPGSQL_DTYPE_ROW;
						plpgsql_yyvsp[-1].row->refname		= plpgsql_yyvsp[-2].varname.name;
						plpgsql_yyvsp[-1].row->lineno		= plpgsql_yyvsp[-2].varname.lineno;

						plpgsql_adddatum((PLpgSQL_datum *)plpgsql_yyvsp[-1].row);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_ROW, plpgsql_yyvsp[-1].row->rowno,
												plpgsql_yyvsp[-2].varname.name);
					}
    break;
case 23:
#line 350 "gram.y"
{
						plpgsql_ns_additem(plpgsql_yyvsp[-1].nsitem->itemtype,
										plpgsql_yyvsp[-1].nsitem->itemno, plpgsql_yyvsp[-4].varname.name);
					}
    break;
case 24:
#line 355 "gram.y"
{
						plpgsql_ns_rename(plpgsql_yyvsp[-3].str, plpgsql_yyvsp[-1].str);
					}
    break;
case 25:
#line 359 "gram.y"
{ plpgsql_ns_push(NULL); }
    break;
case 26:
#line 361 "gram.y"
{
						PLpgSQL_var *new;
						PLpgSQL_expr *curname_def;
						char		buf[1024];
						char		*cp1;
						char		*cp2;

						/* pop local namespace for cursor args */
						plpgsql_ns_pop();

						new = malloc(sizeof(PLpgSQL_var));
						memset(new, 0, sizeof(PLpgSQL_var));

						curname_def = malloc(sizeof(PLpgSQL_expr));
						memset(curname_def, 0, sizeof(PLpgSQL_expr));

						new->dtype		= PLPGSQL_DTYPE_VAR;
						new->refname	= plpgsql_yyvsp[-6].varname.name;
						new->lineno		= plpgsql_yyvsp[-6].varname.lineno;

						curname_def->dtype = PLPGSQL_DTYPE_EXPR;
						strcpy(buf, "SELECT '");
						cp1 = new->refname;
						cp2 = buf + strlen(buf);
						while (*cp1 != '\0')
						{
							if (*cp1 == '\\' || *cp1 == '\'')
								*cp2++ = '\\';
							*cp2++ = *cp1++;
						}
						strcpy(cp2, "'::refcursor");
						curname_def->query = strdup(buf);
						new->default_val = curname_def;

						new->datatype = plpgsql_parse_datatype("refcursor");

						new->cursor_explicit_expr = plpgsql_yyvsp[0].expr;
						if (plpgsql_yyvsp[-3].row == NULL)
							new->cursor_explicit_argrow = -1;
						else
							new->cursor_explicit_argrow = plpgsql_yyvsp[-3].row->rowno;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, new->varno,
										   plpgsql_yyvsp[-6].varname.name);
					}
    break;
case 27:
#line 410 "gram.y"
{
						PLpgSQL_expr *query;

						plpgsql_ns_setlocal(false);
						query = read_sql_stmt("SELECT ");
						plpgsql_ns_setlocal(true);
						
						plpgsql_yyval.expr = query;
					}
    break;
case 28:
#line 422 "gram.y"
{
						plpgsql_yyval.row = NULL;
					}
    break;
case 29:
#line 426 "gram.y"
{
						/* Copy the temp arrays to malloc'd storage */
						int nfields = plpgsql_yyvsp[-1].row->nfields;
						char **ftmp;
						int *vtmp;

						ftmp = malloc(nfields * sizeof(char *));
						vtmp = malloc(nfields * sizeof(int));
						memcpy(ftmp, plpgsql_yyvsp[-1].row->fieldnames, nfields * sizeof(char *));
						memcpy(vtmp, plpgsql_yyvsp[-1].row->varnos, nfields * sizeof(int));

						pfree((char *)(plpgsql_yyvsp[-1].row->fieldnames));
						pfree((char *)(plpgsql_yyvsp[-1].row->varnos));

						plpgsql_yyvsp[-1].row->fieldnames = ftmp;
						plpgsql_yyvsp[-1].row->varnos = vtmp;

						plpgsql_adddatum((PLpgSQL_datum *)plpgsql_yyvsp[-1].row);

						plpgsql_yyval.row = plpgsql_yyvsp[-1].row;
					}
    break;
case 30:
#line 450 "gram.y"
{
						PLpgSQL_row *new;

						new = malloc(sizeof(PLpgSQL_row));
						memset(new, 0, sizeof(PLpgSQL_row));

						new->dtype = PLPGSQL_DTYPE_ROW;
						new->refname = strdup("*internal*");
						new->lineno = plpgsql_yylineno;
						new->rowtypeclass = InvalidOid;
						/*
						 * We make temporary fieldnames/varnos arrays that
						 * are much bigger than necessary.  We will resize
						 * them to just the needed size in the
						 * decl_cursor_args production.
						 */
						new->fieldnames = palloc(1024 * sizeof(char *));
						new->varnos = palloc(1024 * sizeof(int));
						new->nfields = 1;

						new->fieldnames[0] = plpgsql_yyvsp[0].var->refname;
						new->varnos[0] = plpgsql_yyvsp[0].var->varno;

						plpgsql_yyval.row = new;
					}
    break;
case 31:
#line 476 "gram.y"
{
						int i = plpgsql_yyvsp[-2].row->nfields++;

						plpgsql_yyvsp[-2].row->fieldnames[i] = plpgsql_yyvsp[0].var->refname;
						plpgsql_yyvsp[-2].row->varnos[i] = plpgsql_yyvsp[0].var->varno;

						plpgsql_yyval.row = plpgsql_yyvsp[-2].row;
					}
    break;
case 32:
#line 487 "gram.y"
{
						PLpgSQL_var *new;

						new = malloc(sizeof(PLpgSQL_var));
						memset(new, 0, sizeof(PLpgSQL_var));

						new->dtype		= PLPGSQL_DTYPE_VAR;
						new->refname	= plpgsql_yyvsp[-1].varname.name;
						new->lineno		= plpgsql_yyvsp[-1].varname.lineno;

						new->datatype	= plpgsql_yyvsp[0].dtype;
						new->isconst	= false;
						new->notnull	= false;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, new->varno,
										   plpgsql_yyvsp[-1].varname.name);
						
						plpgsql_yyval.var = new;
					}
    break;
case 35:
#line 513 "gram.y"
{
						PLpgSQL_nsitem *nsi;
						char	*name;

						plpgsql_ns_setlocal(false);
						name = plpgsql_tolower(plpgsql_yytext);
						if (name[0] != '$')
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "can only alias positional parameters");
						}
						nsi = plpgsql_ns_lookup(name, NULL);
						if (nsi == NULL)
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "function has no parameter %s", name);
						}

						plpgsql_ns_setlocal(true);

						plpgsql_yyval.nsitem = nsi;
					}
    break;
case 36:
#line 538 "gram.y"
{
						plpgsql_yyval.row = plpgsql_yylval.row;
					}
    break;
case 37:
#line 544 "gram.y"
{
						/* name should be malloc'd for use as varname */
						plpgsql_yyval.varname.name = strdup(plpgsql_tolower(plpgsql_yytext));
						plpgsql_yyval.varname.lineno  = plpgsql_yylineno;
					}
    break;
case 38:
#line 552 "gram.y"
{
						/* the result must be palloc'd, see plpgsql_ns_rename */
						plpgsql_yyval.str = plpgsql_tolower(plpgsql_yytext);
					}
    break;
case 39:
#line 559 "gram.y"
{ plpgsql_yyval.ival = 0; }
    break;
case 40:
#line 561 "gram.y"
{ plpgsql_yyval.ival = 1; }
    break;
case 41:
#line 565 "gram.y"
{
						/*
						 * If there's a lookahead token, read_datatype
						 * should consume it.
						 */
						plpgsql_yyval.dtype = read_datatype(plpgsql_yychar);
						plpgsql_yyclearin;
					}
    break;
case 42:
#line 576 "gram.y"
{ plpgsql_yyval.ival = 0; }
    break;
case 43:
#line 578 "gram.y"
{ plpgsql_yyval.ival = 1; }
    break;
case 44:
#line 582 "gram.y"
{ plpgsql_yyval.expr = NULL; }
    break;
case 45:
#line 584 "gram.y"
{
						int				tok;
						int				lno;
						PLpgSQL_dstring ds;
						PLpgSQL_expr	*expr;

						lno = plpgsql_yylineno;
						expr = malloc(sizeof(PLpgSQL_expr));
						plpgsql_dstring_init(&ds);
						plpgsql_dstring_append(&ds, "SELECT ");

						expr->dtype   = PLPGSQL_DTYPE_EXPR;
						expr->plan	  = NULL;
						expr->nparams = 0;

						tok = plpgsql_yylex();
						switch (tok)
						{
							case 0:
								plpgsql_error_lineno = lno;
								elog(ERROR, "unexpected end of file");
							case K_NULL:
								if (plpgsql_yylex() != ';')
								{
									plpgsql_error_lineno = lno;
									elog(ERROR, "expected ; after NULL");
								}
								free(expr);
								plpgsql_dstring_free(&ds);

								plpgsql_yyval.expr = NULL;
								break;

							default:
								plpgsql_dstring_append(&ds, plpgsql_yytext);
								while ((tok = plpgsql_yylex()) != ';')
								{
									if (tok == 0)
									{
										plpgsql_error_lineno = lno;
										elog(ERROR, "unterminated default value");
									}
									if (plpgsql_SpaceScanned)
										plpgsql_dstring_append(&ds, " ");
									plpgsql_dstring_append(&ds, plpgsql_yytext);
								}
								expr->query = strdup(plpgsql_dstring_get(&ds));
								plpgsql_dstring_free(&ds);

								plpgsql_yyval.expr = expr;
								break;
						}
					}
    break;
case 48:
#line 643 "gram.y"
{
							PLpgSQL_stmts	*new;

							new = malloc(sizeof(PLpgSQL_stmts));
							memset(new, 0, sizeof(PLpgSQL_stmts));
							plpgsql_yyval.stmts = new;
					}
    break;
case 49:
#line 651 "gram.y"
{ plpgsql_yyval.stmts = plpgsql_yyvsp[0].stmts; }
    break;
case 50:
#line 655 "gram.y"
{
								if (plpgsql_yyvsp[-1].stmts->stmts_used == plpgsql_yyvsp[-1].stmts->stmts_alloc) {
									plpgsql_yyvsp[-1].stmts->stmts_alloc *= 2;
									plpgsql_yyvsp[-1].stmts->stmts = realloc(plpgsql_yyvsp[-1].stmts->stmts, sizeof(PLpgSQL_stmt *) * plpgsql_yyvsp[-1].stmts->stmts_alloc);
								}
								plpgsql_yyvsp[-1].stmts->stmts[plpgsql_yyvsp[-1].stmts->stmts_used++] = (struct PLpgSQL_stmt *)plpgsql_yyvsp[0].stmt;

								plpgsql_yyval.stmts = plpgsql_yyvsp[-1].stmts;
						}
    break;
case 51:
#line 665 "gram.y"
{
								PLpgSQL_stmts	*new;

								new = malloc(sizeof(PLpgSQL_stmts));
								memset(new, 0, sizeof(PLpgSQL_stmts));

								new->stmts_alloc = 64;
								new->stmts_used  = 1;
								new->stmts = malloc(sizeof(PLpgSQL_stmt *) * new->stmts_alloc);
								new->stmts[0] = (struct PLpgSQL_stmt *)plpgsql_yyvsp[0].stmt;

								plpgsql_yyval.stmts = new;

						}
    break;
case 52:
#line 682 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[-1].stmt; }
    break;
case 53:
#line 684 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 54:
#line 686 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 55:
#line 688 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 56:
#line 690 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 57:
#line 692 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 58:
#line 694 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 59:
#line 696 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 60:
#line 698 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 61:
#line 700 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 62:
#line 702 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 63:
#line 704 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 64:
#line 706 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 65:
#line 708 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 66:
#line 710 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 67:
#line 712 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 68:
#line 714 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 69:
#line 716 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 70:
#line 718 "gram.y"
{ plpgsql_yyval.stmt = plpgsql_yyvsp[0].stmt; }
    break;
case 71:
#line 722 "gram.y"
{
						PLpgSQL_stmt_assign *new;

						new = malloc(sizeof(PLpgSQL_stmt_assign));
						memset(new, 0, sizeof(PLpgSQL_stmt_assign));

						new->cmd_type = PLPGSQL_STMT_ASSIGN;
						new->lineno   = plpgsql_yyvsp[-1].ival;
						new->varno = -1;
						new->expr  = plpgsql_yyvsp[0].expr;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 72:
#line 738 "gram.y"
{
						PLpgSQL_stmt_assign *new;

						new = malloc(sizeof(PLpgSQL_stmt_assign));
						memset(new, 0, sizeof(PLpgSQL_stmt_assign));

						new->cmd_type = PLPGSQL_STMT_ASSIGN;
						new->lineno   = plpgsql_yyvsp[-2].ival;
						new->varno = plpgsql_yyvsp[-3].ival;
						new->expr  = plpgsql_yyvsp[0].expr;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 73:
#line 754 "gram.y"
{
						PLpgSQL_stmt_getdiag	 *new;

						new = malloc(sizeof(PLpgSQL_stmt_getdiag));
						memset(new, 0, sizeof(PLpgSQL_stmt_getdiag));

						new->cmd_type = PLPGSQL_STMT_GETDIAG;
						new->lineno   = plpgsql_yyvsp[-2].ival;
						new->ndtitems = plpgsql_yyvsp[-1].dtlist.nused;
						new->dtitems  = malloc(sizeof(PLpgSQL_diag_item) * plpgsql_yyvsp[-1].dtlist.nused);
						memcpy(new->dtitems, plpgsql_yyvsp[-1].dtlist.dtitems, sizeof(PLpgSQL_diag_item) * plpgsql_yyvsp[-1].dtlist.nused);

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 74:
#line 771 "gram.y"
{
						if (plpgsql_yyvsp[-4].dtlist.nused == plpgsql_yyvsp[-4].dtlist.nalloc)
						{
							plpgsql_yyvsp[-4].dtlist.nalloc *= 2;
							plpgsql_yyvsp[-4].dtlist.dtitems = repalloc(plpgsql_yyvsp[-4].dtlist.dtitems, sizeof(PLpgSQL_diag_item) * plpgsql_yyvsp[-4].dtlist.nalloc);
						}
						plpgsql_yyvsp[-4].dtlist.dtitems[plpgsql_yyvsp[-4].dtlist.nused].target = plpgsql_yyvsp[-2].ival;
						plpgsql_yyvsp[-4].dtlist.dtitems[plpgsql_yyvsp[-4].dtlist.nused].item   = plpgsql_yyvsp[0].ival;
						plpgsql_yyvsp[-4].dtlist.nused++;

						plpgsql_yyval.dtlist.nalloc = plpgsql_yyvsp[-4].dtlist.nalloc;
						plpgsql_yyval.dtlist.nused  = plpgsql_yyvsp[-4].dtlist.nused;
						plpgsql_yyval.dtlist.dtitems = plpgsql_yyvsp[-4].dtlist.dtitems;
					}
    break;
case 75:
#line 786 "gram.y"
{
						plpgsql_yyval.dtlist.nalloc = 1;
						plpgsql_yyval.dtlist.nused  = 1;
						plpgsql_yyval.dtlist.dtitems = palloc(sizeof(PLpgSQL_diag_item) * plpgsql_yyval.dtlist.nalloc);
						plpgsql_yyval.dtlist.dtitems[0].target = plpgsql_yyvsp[-2].ival;
						plpgsql_yyval.dtlist.dtitems[0].item   = plpgsql_yyvsp[0].ival;
					}
    break;
case 76:
#line 796 "gram.y"
{
						plpgsql_yyval.ival = PLPGSQL_GETDIAG_ROW_COUNT;
					}
    break;
case 77:
#line 800 "gram.y"
{
						plpgsql_yyval.ival = PLPGSQL_GETDIAG_RESULT_OID;
					}
    break;
case 78:
#line 806 "gram.y"
{
						if (plpgsql_yylval.var->isconst)
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "%s is declared CONSTANT; can not receive diagnostics", plpgsql_yylval.var->refname);
						}
						plpgsql_yyval.ival = plpgsql_yylval.var->varno;
					}
    break;
case 79:
#line 815 "gram.y"
{
						plpgsql_yyval.ival = plpgsql_yylval.recfield->rfno;
					}
    break;
case 80:
#line 822 "gram.y"
{
						if (plpgsql_yylval.var->isconst)
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "%s is declared CONSTANT", plpgsql_yylval.var->refname);
						}
						plpgsql_yyval.ival = plpgsql_yylval.var->varno;
					}
    break;
case 81:
#line 831 "gram.y"
{
						plpgsql_yyval.ival = plpgsql_yylval.recfield->rfno;
					}
    break;
case 82:
#line 837 "gram.y"
{
						PLpgSQL_stmt_if *new;

						new = malloc(sizeof(PLpgSQL_stmt_if));
						memset(new, 0, sizeof(PLpgSQL_stmt_if));

						new->cmd_type	= PLPGSQL_STMT_IF;
						new->lineno		= plpgsql_yyvsp[-6].ival;
						new->cond		= plpgsql_yyvsp[-5].expr;
						new->true_body	= plpgsql_yyvsp[-4].stmts;
						new->false_body = plpgsql_yyvsp[-3].stmts;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 83:
#line 854 "gram.y"
{
							PLpgSQL_stmts	*new;

							new = malloc(sizeof(PLpgSQL_stmts));
							memset(new, 0, sizeof(PLpgSQL_stmts));
							plpgsql_yyval.stmts = new;
					}
    break;
case 84:
#line 862 "gram.y"
{
					  /*
					   * Translate the structure:	   into:
					   *
					   * IF c1 THEN					   IF c1 THEN		 
					   *	 ...						   ...				 
					   * ELSIF c2 THEN				   ELSE 
					   *								   IF c2 THEN	
					   *	 ...							   ...				 
					   * ELSE							   ELSE				 
					   *	 ...							   ...				 
					   * END IF							   END IF			 
					   *							   END IF
					   * 
					   */

						PLpgSQL_stmts	*new;
						PLpgSQL_stmt_if *new_if;

						/* first create a new if-statement */
						new_if = malloc(sizeof(PLpgSQL_stmt_if));
						memset(new_if, 0, sizeof(PLpgSQL_stmt_if));

						new_if->cmd_type	= PLPGSQL_STMT_IF;
						new_if->lineno		= plpgsql_yyvsp[-3].ival;
						new_if->cond		= plpgsql_yyvsp[-2].expr;
						new_if->true_body	= plpgsql_yyvsp[-1].stmts;
						new_if->false_body	= plpgsql_yyvsp[0].stmts;
						
						/* this is a 'container' for the if-statement */
						new = malloc(sizeof(PLpgSQL_stmts));
						memset(new, 0, sizeof(PLpgSQL_stmts));
						
						new->stmts_alloc = 64;
						new->stmts_used	 = 1;
						new->stmts = malloc(sizeof(PLpgSQL_stmt *) * new->stmts_alloc);
						new->stmts[0] = (struct PLpgSQL_stmt *)new_if;

						plpgsql_yyval.stmts = new;
						
					}
    break;
case 85:
#line 905 "gram.y"
{
						plpgsql_yyval.stmts = plpgsql_yyvsp[0].stmts;				
					}
    break;
case 86:
#line 911 "gram.y"
{
						PLpgSQL_stmt_loop *new;

						new = malloc(sizeof(PLpgSQL_stmt_loop));
						memset(new, 0, sizeof(PLpgSQL_stmt_loop));

						new->cmd_type = PLPGSQL_STMT_LOOP;
						new->lineno   = plpgsql_yyvsp[-1].ival;
						new->label	  = plpgsql_yyvsp[-3].str;
						new->body	  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 87:
#line 929 "gram.y"
{
						PLpgSQL_stmt_while *new;

						new = malloc(sizeof(PLpgSQL_stmt_while));
						memset(new, 0, sizeof(PLpgSQL_stmt_while));

						new->cmd_type = PLPGSQL_STMT_WHILE;
						new->lineno   = plpgsql_yyvsp[-2].ival;
						new->label	  = plpgsql_yyvsp[-4].str;
						new->cond	  = plpgsql_yyvsp[-1].expr;
						new->body	  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 88:
#line 948 "gram.y"
{
						PLpgSQL_stmt_fori		*new;

						new = malloc(sizeof(PLpgSQL_stmt_fori));
						memset(new, 0, sizeof(PLpgSQL_stmt_fori));

						new->cmd_type = PLPGSQL_STMT_FORI;
						new->lineno   = plpgsql_yyvsp[-5].ival;
						new->label	  = plpgsql_yyvsp[-7].str;
						new->var	  = plpgsql_yyvsp[-4].var;
						new->reverse  = plpgsql_yyvsp[-2].forilow.reverse;
						new->lower	  = plpgsql_yyvsp[-2].forilow.expr;
						new->upper	  = plpgsql_yyvsp[-1].expr;
						new->body	  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 89:
#line 970 "gram.y"
{
						PLpgSQL_var		*new;

						new = malloc(sizeof(PLpgSQL_var));
						memset(new, 0, sizeof(PLpgSQL_var));

						new->dtype		= PLPGSQL_DTYPE_VAR;
						new->refname	= plpgsql_yyvsp[0].varname.name;
						new->lineno		= plpgsql_yyvsp[0].varname.lineno;

						new->datatype	= plpgsql_parse_datatype("integer");
						new->isconst	= false;
						new->notnull	= false;
						new->default_val = NULL;

						plpgsql_adddatum((PLpgSQL_datum *)new);
						plpgsql_ns_additem(PLPGSQL_NSTYPE_VAR, new->varno,
												plpgsql_yyvsp[0].varname.name);

						plpgsql_add_initdatums(NULL);

						plpgsql_yyval.var = new;
					}
    break;
case 90:
#line 996 "gram.y"
{
						plpgsql_yyval.varname.name = strdup(plpgsql_yytext);
						plpgsql_yyval.varname.lineno = plpgsql_yylineno;
					}
    break;
case 91:
#line 1001 "gram.y"
{
						plpgsql_yyval.varname.name = strdup(plpgsql_yytext);
						plpgsql_yyval.varname.lineno = plpgsql_yylineno;
					}
    break;
case 92:
#line 1008 "gram.y"
{
						int			tok;

						tok = plpgsql_yylex();
						if (tok == K_REVERSE)
						{
							plpgsql_yyval.forilow.reverse = 1;
						}
						else
						{
							plpgsql_yyval.forilow.reverse = 0;
							plpgsql_push_back_token(tok);
						}

						plpgsql_yyval.forilow.expr = plpgsql_read_expression(K_DOTDOT, "..");
					}
    break;
case 93:
#line 1026 "gram.y"
{
						PLpgSQL_stmt_fors		*new;

						new = malloc(sizeof(PLpgSQL_stmt_fors));
						memset(new, 0, sizeof(PLpgSQL_stmt_fors));

						new->cmd_type = PLPGSQL_STMT_FORS;
						new->lineno   = plpgsql_yyvsp[-5].ival;
						new->label	  = plpgsql_yyvsp[-7].str;
						switch (plpgsql_yyvsp[-4].rec->dtype)
						{
							case PLPGSQL_DTYPE_REC:
								new->rec = plpgsql_yyvsp[-4].rec;
								break;
							case PLPGSQL_DTYPE_ROW:
								new->row = (PLpgSQL_row *)plpgsql_yyvsp[-4].rec;
								break;
							default:
								elog(ERROR, "unknown dtype %d in stmt_fors", plpgsql_yyvsp[-4].rec->dtype);
						}
						new->query = plpgsql_yyvsp[-1].expr;
						new->body  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 94:
#line 1055 "gram.y"
{
						PLpgSQL_stmt_dynfors	*new;

						new = malloc(sizeof(PLpgSQL_stmt_dynfors));
						memset(new, 0, sizeof(PLpgSQL_stmt_dynfors));

						new->cmd_type = PLPGSQL_STMT_DYNFORS;
						new->lineno   = plpgsql_yyvsp[-5].ival;
						new->label	  = plpgsql_yyvsp[-7].str;
						switch (plpgsql_yyvsp[-4].rec->dtype)
						{
							case PLPGSQL_DTYPE_REC:
								new->rec = plpgsql_yyvsp[-4].rec;
								break;
							case PLPGSQL_DTYPE_ROW:
								new->row = (PLpgSQL_row *)plpgsql_yyvsp[-4].rec;
								break;
							default:
								elog(ERROR, "unknown dtype %d in stmt_dynfors", plpgsql_yyvsp[-4].rec->dtype);
						}
						new->query = plpgsql_yyvsp[-1].expr;
						new->body  = plpgsql_yyvsp[0].stmts;

						plpgsql_ns_pop();

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 95:
#line 1084 "gram.y"
{ plpgsql_yyval.rec = plpgsql_yylval.rec; }
    break;
case 96:
#line 1086 "gram.y"
{
						plpgsql_yyval.rec = (PLpgSQL_rec *)(plpgsql_yylval.row);
					}
    break;
case 97:
#line 1092 "gram.y"
{
						plpgsql_yyval.stmt = make_select_stmt();
						plpgsql_yyval.stmt->lineno = plpgsql_yyvsp[0].ival;
					}
    break;
case 98:
#line 1099 "gram.y"
{
						PLpgSQL_stmt_exit *new;

						new = malloc(sizeof(PLpgSQL_stmt_exit));
						memset(new, 0, sizeof(PLpgSQL_stmt_exit));

						new->cmd_type = PLPGSQL_STMT_EXIT;
						new->lineno   = plpgsql_yyvsp[-2].ival;
						new->label	  = plpgsql_yyvsp[-1].str;
						new->cond	  = plpgsql_yyvsp[0].expr;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 99:
#line 1115 "gram.y"
{
						PLpgSQL_stmt_return *new;
						PLpgSQL_expr	*expr = NULL;
						int				tok;

						new = malloc(sizeof(PLpgSQL_stmt_return));
						memset(new, 0, sizeof(PLpgSQL_stmt_return));

						if (plpgsql_curr_compile->fn_retistuple)
						{
							new->retistuple = true;
							new->retrecno	= -1;
							switch (tok = plpgsql_yylex())
							{
								case K_NULL:
									expr = NULL;
									break;

								case T_ROW:
									expr = make_tupret_expr(plpgsql_yylval.row);
									break;

								case T_RECORD:
									new->retrecno = plpgsql_yylval.rec->recno;
									expr = NULL;
									break;

								default:
									plpgsql_yyerror("return type mismatch in function returning table row");
									break;
							}
							if (plpgsql_yylex() != ';')
								plpgsql_yyerror("expected ';'");
						} else {
							new->retistuple = false;
							expr = plpgsql_read_expression(';', ";");
						}

						new->cmd_type = PLPGSQL_STMT_RETURN;
						new->lineno   = plpgsql_yyvsp[0].ival;
						new->expr	  = expr;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 100:
#line 1162 "gram.y"
{
						PLpgSQL_stmt_raise		*new;

						new = malloc(sizeof(PLpgSQL_stmt_raise));

						new->cmd_type	= PLPGSQL_STMT_RAISE;
						new->lineno		= plpgsql_yyvsp[-4].ival;
						new->elog_level = plpgsql_yyvsp[-3].ival;
						new->message	= plpgsql_yyvsp[-2].str;
						new->nparams	= plpgsql_yyvsp[-1].intlist.nused;
						new->params		= malloc(sizeof(int) * plpgsql_yyvsp[-1].intlist.nused);
						memcpy(new->params, plpgsql_yyvsp[-1].intlist.nums, sizeof(int) * plpgsql_yyvsp[-1].intlist.nused);

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 101:
#line 1178 "gram.y"
{
						PLpgSQL_stmt_raise		*new;

						new = malloc(sizeof(PLpgSQL_stmt_raise));

						new->cmd_type	= PLPGSQL_STMT_RAISE;
						new->lineno		= plpgsql_yyvsp[-3].ival;
						new->elog_level = plpgsql_yyvsp[-2].ival;
						new->message	= plpgsql_yyvsp[-1].str;
						new->nparams	= 0;
						new->params		= NULL;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 102:
#line 1195 "gram.y"
{
						plpgsql_yyval.str = strdup(plpgsql_yytext);
					}
    break;
case 103:
#line 1201 "gram.y"
{
						plpgsql_yyval.ival = ERROR;
					}
    break;
case 104:
#line 1205 "gram.y"
{
						plpgsql_yyval.ival = NOTICE;
					}
    break;
case 105:
#line 1209 "gram.y"
{
						plpgsql_yyval.ival = DEBUG;
					}
    break;
case 106:
#line 1215 "gram.y"
{
						if (plpgsql_yyvsp[-1].intlist.nused == plpgsql_yyvsp[-1].intlist.nalloc)
						{
							plpgsql_yyvsp[-1].intlist.nalloc *= 2;
							plpgsql_yyvsp[-1].intlist.nums = repalloc(plpgsql_yyvsp[-1].intlist.nums, sizeof(int) * plpgsql_yyvsp[-1].intlist.nalloc);
						}
						plpgsql_yyvsp[-1].intlist.nums[plpgsql_yyvsp[-1].intlist.nused++] = plpgsql_yyvsp[0].ival;

						plpgsql_yyval.intlist.nalloc = plpgsql_yyvsp[-1].intlist.nalloc;
						plpgsql_yyval.intlist.nused  = plpgsql_yyvsp[-1].intlist.nused;
						plpgsql_yyval.intlist.nums   = plpgsql_yyvsp[-1].intlist.nums;
					}
    break;
case 107:
#line 1228 "gram.y"
{
						plpgsql_yyval.intlist.nalloc = 1;
						plpgsql_yyval.intlist.nused  = 1;
						plpgsql_yyval.intlist.nums   = palloc(sizeof(int) * plpgsql_yyval.intlist.nalloc);
						plpgsql_yyval.intlist.nums[0] = plpgsql_yyvsp[0].ival;
					}
    break;
case 108:
#line 1237 "gram.y"
{
						plpgsql_yyval.ival = plpgsql_yylval.var->varno;
					}
    break;
case 109:
#line 1241 "gram.y"
{
						plpgsql_yyval.ival = plpgsql_yylval.recfield->rfno;
					}
    break;
case 110:
#line 1245 "gram.y"
{
						plpgsql_yyval.ival = plpgsql_yylval.trigarg->dno;
					}
    break;
case 111:
#line 1251 "gram.y"
{ plpgsql_yyval.stmts = plpgsql_yyvsp[-3].stmts; }
    break;
case 112:
#line 1255 "gram.y"
{
						PLpgSQL_stmt_execsql	*new;

						new = malloc(sizeof(PLpgSQL_stmt_execsql));
						new->cmd_type = PLPGSQL_STMT_EXECSQL;
						new->lineno   = plpgsql_yyvsp[0].ival;
						new->sqlstmt  = read_sql_stmt(plpgsql_yyvsp[-1].str);

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 113:
#line 1268 "gram.y"
{
							PLpgSQL_stmt_dynexecute *new;

							new = malloc(sizeof(PLpgSQL_stmt_dynexecute));
							new->cmd_type = PLPGSQL_STMT_DYNEXECUTE;
							new->lineno   = plpgsql_yyvsp[-1].ival;
							new->query	  = plpgsql_yyvsp[0].expr;

							plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
						}
    break;
case 114:
#line 1281 "gram.y"
{
						PLpgSQL_stmt_open *new;
						int				  tok;

						new = malloc(sizeof(PLpgSQL_stmt_open));
						memset(new, 0, sizeof(PLpgSQL_stmt_open));

						new->cmd_type = PLPGSQL_STMT_OPEN;
						new->lineno = plpgsql_yyvsp[-1].ival;
						new->curvar = plpgsql_yyvsp[0].var->varno;

						if (plpgsql_yyvsp[0].var->cursor_explicit_expr == NULL)
						{
						    tok = plpgsql_yylex();

							if (tok != K_FOR)
							{
								plpgsql_error_lineno = plpgsql_yyvsp[-1].ival;
								elog(ERROR, "syntax error at \"%s\" - expected FOR to open a reference cursor", plpgsql_yytext);
							}

							tok = plpgsql_yylex();
							switch (tok)
							{
								case K_SELECT:
									new->query = read_sql_stmt("SELECT ");
									break;

								case K_EXECUTE:
									new->dynquery = read_sql_stmt("SELECT ");
									break;

								default:
									plpgsql_error_lineno = plpgsql_yyvsp[-1].ival;
									elog(ERROR, "syntax error at \"%s\"", plpgsql_yytext);
							}

						}
						else
						{
							if (plpgsql_yyvsp[0].var->cursor_explicit_argrow >= 0)
							{
								char   *cp;

								tok = plpgsql_yylex();

								if (tok != '(')
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "cursor %s has arguments", plpgsql_yyvsp[0].var->refname);
								}

								new->argquery = read_sql_stmt("SELECT ");
								/* Remove the trailing right paren,
                                 * because we want "select 1, 2", not
                                 * "select (1, 2)".
								 */
								cp = new->argquery->query;
								cp += strlen(cp);
								--cp;
								if (*cp != ')')
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "missing )");
								}
								*cp = '\0';
							}
							else
							{
								tok = plpgsql_yylex();

								if (tok == '(')
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "cursor %s has no arguments", plpgsql_yyvsp[0].var->refname);
								}
								
								if (tok != ';')
								{
									plpgsql_error_lineno = plpgsql_yylineno;
									elog(ERROR, "syntax error at \"%s\"", plpgsql_yytext);
								}
							}
						}

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 115:
#line 1371 "gram.y"
{
						PLpgSQL_stmt_fetch *new;

						new = (PLpgSQL_stmt_fetch *)make_fetch_stmt();
						new->curvar = plpgsql_yyvsp[-1].ival;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
						plpgsql_yyval.stmt->lineno = plpgsql_yyvsp[-2].ival;
					}
    break;
case 116:
#line 1383 "gram.y"
{
						PLpgSQL_stmt_close *new;

						new = malloc(sizeof(PLpgSQL_stmt_close));
						new->cmd_type = PLPGSQL_STMT_CLOSE;
						new->lineno = plpgsql_yyvsp[-2].ival;
						new->curvar = plpgsql_yyvsp[-1].ival;

						plpgsql_yyval.stmt = (PLpgSQL_stmt *)new;
					}
    break;
case 117:
#line 1396 "gram.y"
{
						if (plpgsql_yylval.var->datatype->typoid != REFCURSOROID)
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "%s must be of type cursor or refcursor", plpgsql_yylval.var->refname);
						}
						plpgsql_yyval.var = plpgsql_yylval.var;
					}
    break;
case 118:
#line 1407 "gram.y"
{
						if (plpgsql_yylval.var->datatype->typoid != REFCURSOROID)
						{
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "%s must be of type refcursor", plpgsql_yylval.var->refname);
						}
						plpgsql_yyval.ival = plpgsql_yylval.var->varno;
					}
    break;
case 119:
#line 1418 "gram.y"
{ plpgsql_yyval.str = strdup(plpgsql_yytext); }
    break;
case 120:
#line 1420 "gram.y"
{ plpgsql_yyval.str = strdup(plpgsql_yytext); }
    break;
case 121:
#line 1424 "gram.y"
{ plpgsql_yyval.expr = plpgsql_read_expression(';', ";"); }
    break;
case 122:
#line 1428 "gram.y"
{ plpgsql_yyval.expr = plpgsql_read_expression(K_THEN, "THEN"); }
    break;
case 123:
#line 1432 "gram.y"
{ plpgsql_yyval.expr = plpgsql_read_expression(K_LOOP, "LOOP"); }
    break;
case 124:
#line 1436 "gram.y"
{
						plpgsql_ns_push(NULL);
						plpgsql_yyval.str = NULL;
					}
    break;
case 125:
#line 1441 "gram.y"
{
						plpgsql_ns_push(plpgsql_yyvsp[-2].str);
						plpgsql_yyval.str = plpgsql_yyvsp[-2].str;
					}
    break;
case 126:
#line 1448 "gram.y"
{ plpgsql_yyval.str = NULL; }
    break;
case 127:
#line 1450 "gram.y"
{ plpgsql_yyval.str = strdup(plpgsql_yytext); }
    break;
case 128:
#line 1454 "gram.y"
{ plpgsql_yyval.expr = NULL; }
    break;
case 129:
#line 1456 "gram.y"
{ plpgsql_yyval.expr = plpgsql_yyvsp[0].expr; }
    break;
case 130:
#line 1460 "gram.y"
{ plpgsql_yyval.str = strdup(plpgsql_yytext); }
    break;
case 131:
#line 1464 "gram.y"
{
						plpgsql_error_lineno = plpgsql_yylineno;
						plpgsql_yyval.ival = plpgsql_yylineno;
					}
    break;
}

#line 705 "/usr/share/bison/bison.simple"


  plpgsql_yyvsp -= plpgsql_yylen;
  plpgsql_yyssp -= plpgsql_yylen;
#if PLPGSQL_YYLSP_NEEDED
  plpgsql_yylsp -= plpgsql_yylen;
#endif

#if PLPGSQL_YYDEBUG
  if (plpgsql_yydebug)
    {
      short *plpgsql_yyssp1 = plpgsql_yyss - 1;
      PLPGSQL_YYFPRINTF (stderr, "state stack now");
      while (plpgsql_yyssp1 != plpgsql_yyssp)
	PLPGSQL_YYFPRINTF (stderr, " %d", *++plpgsql_yyssp1);
      PLPGSQL_YYFPRINTF (stderr, "\n");
    }
#endif

  *++plpgsql_yyvsp = plpgsql_yyval;
#if PLPGSQL_YYLSP_NEEDED
  *++plpgsql_yylsp = plpgsql_yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  plpgsql_yyn = plpgsql_yyr1[plpgsql_yyn];

  plpgsql_yystate = plpgsql_yypgoto[plpgsql_yyn - PLPGSQL_YYNTBASE] + *plpgsql_yyssp;
  if (plpgsql_yystate >= 0 && plpgsql_yystate <= PLPGSQL_YYLAST && plpgsql_yycheck[plpgsql_yystate] == *plpgsql_yyssp)
    plpgsql_yystate = plpgsql_yytable[plpgsql_yystate];
  else
    plpgsql_yystate = plpgsql_yydefgoto[plpgsql_yyn - PLPGSQL_YYNTBASE];

  goto plpgsql_yynewstate;


/*------------------------------------.
| plpgsql_yyerrlab -- here on detecting error |
`------------------------------------*/
plpgsql_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!plpgsql_yyerrstatus)
    {
      ++plpgsql_yynerrs;

#ifdef PLPGSQL_YYERROR_VERBOSE
      plpgsql_yyn = plpgsql_yypact[plpgsql_yystate];

      if (plpgsql_yyn > PLPGSQL_YYFLAG && plpgsql_yyn < PLPGSQL_YYLAST)
	{
	  PLPGSQL_YYSIZE_T plpgsql_yysize = 0;
	  char *plpgsql_yymsg;
	  int plpgsql_yyx, plpgsql_yycount;

	  plpgsql_yycount = 0;
	  /* Start PLPGSQL_YYX at -PLPGSQL_YYN if negative to avoid negative indexes in
	     PLPGSQL_YYCHECK.  */
	  for (plpgsql_yyx = plpgsql_yyn < 0 ? -plpgsql_yyn : 0;
	       plpgsql_yyx < (int) (sizeof (plpgsql_yytname) / sizeof (char *)); plpgsql_yyx++)
	    if (plpgsql_yycheck[plpgsql_yyx + plpgsql_yyn] == plpgsql_yyx)
	      plpgsql_yysize += plpgsql_yystrlen (plpgsql_yytname[plpgsql_yyx]) + 15, plpgsql_yycount++;
	  plpgsql_yysize += plpgsql_yystrlen ("parse error, unexpected ") + 1;
	  plpgsql_yysize += plpgsql_yystrlen (plpgsql_yytname[PLPGSQL_YYTRANSLATE (plpgsql_yychar)]);
	  plpgsql_yymsg = (char *) PLPGSQL_YYSTACK_ALLOC (plpgsql_yysize);
	  if (plpgsql_yymsg != 0)
	    {
	      char *plpgsql_yyp = plpgsql_yystpcpy (plpgsql_yymsg, "parse error, unexpected ");
	      plpgsql_yyp = plpgsql_yystpcpy (plpgsql_yyp, plpgsql_yytname[PLPGSQL_YYTRANSLATE (plpgsql_yychar)]);

	      if (plpgsql_yycount < 5)
		{
		  plpgsql_yycount = 0;
		  for (plpgsql_yyx = plpgsql_yyn < 0 ? -plpgsql_yyn : 0;
		       plpgsql_yyx < (int) (sizeof (plpgsql_yytname) / sizeof (char *));
		       plpgsql_yyx++)
		    if (plpgsql_yycheck[plpgsql_yyx + plpgsql_yyn] == plpgsql_yyx)
		      {
			const char *plpgsql_yyq = ! plpgsql_yycount ? ", expecting " : " or ";
			plpgsql_yyp = plpgsql_yystpcpy (plpgsql_yyp, plpgsql_yyq);
			plpgsql_yyp = plpgsql_yystpcpy (plpgsql_yyp, plpgsql_yytname[plpgsql_yyx]);
			plpgsql_yycount++;
		      }
		}
	      plpgsql_yyerror (plpgsql_yymsg);
	      PLPGSQL_YYSTACK_FREE (plpgsql_yymsg);
	    }
	  else
	    plpgsql_yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (PLPGSQL_YYERROR_VERBOSE) */
	plpgsql_yyerror ("parse error");
    }
  goto plpgsql_yyerrlab1;


/*--------------------------------------------------.
| plpgsql_yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
plpgsql_yyerrlab1:
  if (plpgsql_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (plpgsql_yychar == PLPGSQL_YYEOF)
	PLPGSQL_YYABORT;
      PLPGSQL_YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  plpgsql_yychar, plpgsql_yytname[plpgsql_yychar1]));
      plpgsql_yychar = PLPGSQL_YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  plpgsql_yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto plpgsql_yyerrhandle;


/*-------------------------------------------------------------------.
| plpgsql_yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
plpgsql_yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  plpgsql_yyn = plpgsql_yydefact[plpgsql_yystate];
  if (plpgsql_yyn)
    goto plpgsql_yydefault;
#endif


/*---------------------------------------------------------------.
| plpgsql_yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
plpgsql_yyerrpop:
  if (plpgsql_yyssp == plpgsql_yyss)
    PLPGSQL_YYABORT;
  plpgsql_yyvsp--;
  plpgsql_yystate = *--plpgsql_yyssp;
#if PLPGSQL_YYLSP_NEEDED
  plpgsql_yylsp--;
#endif

#if PLPGSQL_YYDEBUG
  if (plpgsql_yydebug)
    {
      short *plpgsql_yyssp1 = plpgsql_yyss - 1;
      PLPGSQL_YYFPRINTF (stderr, "Error: state stack now");
      while (plpgsql_yyssp1 != plpgsql_yyssp)
	PLPGSQL_YYFPRINTF (stderr, " %d", *++plpgsql_yyssp1);
      PLPGSQL_YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| plpgsql_yyerrhandle.  |
`--------------*/
plpgsql_yyerrhandle:
  plpgsql_yyn = plpgsql_yypact[plpgsql_yystate];
  if (plpgsql_yyn == PLPGSQL_YYFLAG)
    goto plpgsql_yyerrdefault;

  plpgsql_yyn += PLPGSQL_YYTERROR;
  if (plpgsql_yyn < 0 || plpgsql_yyn > PLPGSQL_YYLAST || plpgsql_yycheck[plpgsql_yyn] != PLPGSQL_YYTERROR)
    goto plpgsql_yyerrdefault;

  plpgsql_yyn = plpgsql_yytable[plpgsql_yyn];
  if (plpgsql_yyn < 0)
    {
      if (plpgsql_yyn == PLPGSQL_YYFLAG)
	goto plpgsql_yyerrpop;
      plpgsql_yyn = -plpgsql_yyn;
      goto plpgsql_yyreduce;
    }
  else if (plpgsql_yyn == 0)
    goto plpgsql_yyerrpop;

  if (plpgsql_yyn == PLPGSQL_YYFINAL)
    PLPGSQL_YYACCEPT;

  PLPGSQL_YYDPRINTF ((stderr, "Shifting error token, "));

  *++plpgsql_yyvsp = plpgsql_yylval;
#if PLPGSQL_YYLSP_NEEDED
  *++plpgsql_yylsp = plpgsql_yylloc;
#endif

  plpgsql_yystate = plpgsql_yyn;
  goto plpgsql_yynewstate;


/*-------------------------------------.
| plpgsql_yyacceptlab -- PLPGSQL_YYACCEPT comes here.  |
`-------------------------------------*/
plpgsql_yyacceptlab:
  plpgsql_yyresult = 0;
  goto plpgsql_yyreturn;

/*-----------------------------------.
| plpgsql_yyabortlab -- PLPGSQL_YYABORT comes here.  |
`-----------------------------------*/
plpgsql_yyabortlab:
  plpgsql_yyresult = 1;
  goto plpgsql_yyreturn;

/*---------------------------------------------.
| plpgsql_yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
plpgsql_yyoverflowlab:
  plpgsql_yyerror ("parser stack overflow");
  plpgsql_yyresult = 2;
  /* Fall through.  */

plpgsql_yyreturn:
#ifndef plpgsql_yyoverflow
  if (plpgsql_yyss != plpgsql_yyssa)
    PLPGSQL_YYSTACK_FREE (plpgsql_yyss);
#endif
  return plpgsql_yyresult;
}
#line 1470 "gram.y"



PLpgSQL_expr *
plpgsql_read_expression(int until, const char *expected)
{
	return read_sql_construct(until, expected, true, "SELECT ");
}

static PLpgSQL_expr *
read_sql_stmt(const char *sqlstart)
{
	return read_sql_construct(';', ";", false, sqlstart);
}

static PLpgSQL_expr *
read_sql_construct(int until,
				   const char *expected,
				   bool isexpression,
				   const char *sqlstart)
{
	int					tok;
	int					lno;
	PLpgSQL_dstring		ds;
	int					parenlevel = 0;
	int					nparams = 0;
	int					params[1024];
	char				buf[32];
	PLpgSQL_expr		*expr;

	lno = plpgsql_yylineno;
	plpgsql_dstring_init(&ds);
	plpgsql_dstring_append(&ds, (char *) sqlstart);

	for (;;)
	{
		tok = plpgsql_yylex();
		if (tok == '(')
			parenlevel++;
		else if (tok == ')')
		{
			parenlevel--;
			if (parenlevel < 0)
				elog(ERROR, "mismatched parentheses");
		}
		else if (parenlevel == 0 && tok == until)
			break;
		/*
		 * End of function definition is an error, and we don't expect to
		 * hit a semicolon either (unless it's the until symbol, in which
		 * case we should have fallen out above).
		 */
		if (tok == 0 || tok == ';')
		{
			plpgsql_error_lineno = lno;
			if (parenlevel != 0)
				elog(ERROR, "mismatched parentheses");
			if (isexpression)
				elog(ERROR, "missing %s at end of SQL expression",
					 expected);
			else
				elog(ERROR, "missing %s at end of SQL statement",
					 expected);
			break;
		}
		if (plpgsql_SpaceScanned)
			plpgsql_dstring_append(&ds, " ");
		switch (tok)
		{
			case T_VARIABLE:
				params[nparams] = plpgsql_yylval.var->varno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			case T_RECFIELD:
				params[nparams] = plpgsql_yylval.recfield->rfno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			case T_TGARGV:
				params[nparams] = plpgsql_yylval.trigarg->dno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			default:
				plpgsql_dstring_append(&ds, plpgsql_yytext);
				break;
		}
	}

	expr = malloc(sizeof(PLpgSQL_expr) + sizeof(int) * nparams - sizeof(int));
	expr->dtype			= PLPGSQL_DTYPE_EXPR;
	expr->query			= strdup(plpgsql_dstring_get(&ds));
	expr->plan			= NULL;
	expr->nparams		= nparams;
	while(nparams-- > 0)
		expr->params[nparams] = params[nparams];
	plpgsql_dstring_free(&ds);

	return expr;
}

static PLpgSQL_type *
read_datatype(int tok)
{
	int					lno;
	PLpgSQL_dstring		ds;
	PLpgSQL_type		*result;
	bool				needspace = false;
	int					parenlevel = 0;

	lno = plpgsql_yylineno;

	/* Often there will be a lookahead token, but if not, get one */
	if (tok == PLPGSQL_YYEMPTY)
		tok = plpgsql_yylex();

	if (tok == T_DTYPE)
	{
		/* lexer found word%TYPE and did its thing already */
		return plpgsql_yylval.dtype;
	}

	plpgsql_dstring_init(&ds);

	while (tok != ';')
	{
		if (tok == 0)
		{
			plpgsql_error_lineno = lno;
			if (parenlevel != 0)
				elog(ERROR, "mismatched parentheses");
			elog(ERROR, "incomplete datatype declaration");
		}
		/* Possible followers for datatype in a declaration */
		if (tok == K_NOT || tok == K_ASSIGN || tok == K_DEFAULT)
			break;
		/* Possible followers for datatype in a cursor_arg list */
		if ((tok == ',' || tok == ')') && parenlevel == 0)
			break;
		if (tok == '(')
			parenlevel++;
		else if (tok == ')')
			parenlevel--;
		if (needspace)
			plpgsql_dstring_append(&ds, " ");
		needspace = true;
		plpgsql_dstring_append(&ds, plpgsql_yytext);

		tok = plpgsql_yylex();
	}

	plpgsql_push_back_token(tok);

	plpgsql_error_lineno = lno;	/* in case of error in parse_datatype */

	result = plpgsql_parse_datatype(plpgsql_dstring_get(&ds));

	plpgsql_dstring_free(&ds);

	return result;
}


static PLpgSQL_stmt *
make_select_stmt()
{
	int					tok;
	int					lno;
	PLpgSQL_dstring		ds;
	int					nparams = 0;
	int					params[1024];
	char				buf[32];
	PLpgSQL_expr		*expr;
	PLpgSQL_row			*row = NULL;
	PLpgSQL_rec			*rec = NULL;
	PLpgSQL_stmt_select *select;
	int					have_nexttok = 0;

	lno = plpgsql_yylineno;
	plpgsql_dstring_init(&ds);
	plpgsql_dstring_append(&ds, "SELECT ");

	while((tok = plpgsql_yylex()) != K_INTO)
	{
		if (tok == ';')
		{
			PLpgSQL_stmt_execsql		*execsql;

			expr = malloc(sizeof(PLpgSQL_expr) + sizeof(int) * nparams - sizeof(int));
			expr->dtype			= PLPGSQL_DTYPE_EXPR;
			expr->query			= strdup(plpgsql_dstring_get(&ds));
			expr->plan			= NULL;
			expr->nparams		= nparams;
			while(nparams-- > 0)
				expr->params[nparams] = params[nparams];
			plpgsql_dstring_free(&ds);

			execsql = malloc(sizeof(PLpgSQL_stmt_execsql));
			execsql->cmd_type = PLPGSQL_STMT_EXECSQL;
			execsql->sqlstmt  = expr;

			return (PLpgSQL_stmt *)execsql;
		}

		if (plpgsql_SpaceScanned)
			plpgsql_dstring_append(&ds, " ");
		switch (tok)
		{
			case T_VARIABLE:
				params[nparams] = plpgsql_yylval.var->varno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			case T_RECFIELD:
				params[nparams] = plpgsql_yylval.recfield->rfno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			case T_TGARGV:
				params[nparams] = plpgsql_yylval.trigarg->dno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			default:
				if (tok == 0)
				{
					plpgsql_error_lineno = plpgsql_yylineno;
					elog(ERROR, "unexpected end of file");
				}
				plpgsql_dstring_append(&ds, plpgsql_yytext);
				break;
		}
	}

	tok = plpgsql_yylex();
	switch (tok)
	{
		case T_ROW:
			row = plpgsql_yylval.row;
			break;

		case T_RECORD:
			rec = plpgsql_yylval.rec;
			break;

		case T_VARIABLE:
		case T_RECFIELD:
			{
				PLpgSQL_var		*var;
				PLpgSQL_recfield *recfield;
				int				nfields = 1;
				char			*fieldnames[1024];
				int				varnos[1024];

				switch (tok)
				{	
					case T_VARIABLE:
						var = plpgsql_yylval.var;
						fieldnames[0] = strdup(plpgsql_yytext);
						varnos[0]	  = var->varno;
						break;

					case T_RECFIELD:
						recfield = plpgsql_yylval.recfield;
						fieldnames[0] = strdup(plpgsql_yytext);
						varnos[0]	  = recfield->rfno;
						break;
				}

				while ((tok = plpgsql_yylex()) == ',')
				{
					tok = plpgsql_yylex();
					switch(tok)
					{
						case T_VARIABLE:
							var = plpgsql_yylval.var;
							fieldnames[nfields] = strdup(plpgsql_yytext);
							varnos[nfields++]	= var->varno;
							break;

						case T_RECFIELD:
							recfield = plpgsql_yylval.recfield;
							fieldnames[0] = strdup(plpgsql_yytext);
							varnos[0]	  = recfield->rfno;
							break;

						default:
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "plpgsql: %s is not a variable or record field", plpgsql_yytext);
					}
				}
				row = malloc(sizeof(PLpgSQL_row));
				row->dtype = PLPGSQL_DTYPE_ROW;
				row->refname = strdup("*internal*");
				row->lineno = plpgsql_yylineno;
				row->rowtypeclass = InvalidOid;
				row->nfields = nfields;
				row->fieldnames = malloc(sizeof(char *) * nfields);
				row->varnos = malloc(sizeof(int) * nfields);
				while (--nfields >= 0)
				{
					row->fieldnames[nfields] = fieldnames[nfields];
					row->varnos[nfields] = varnos[nfields];
				}

				plpgsql_adddatum((PLpgSQL_datum *)row);

				have_nexttok = 1;
			}
			break;

		default:
			{
				if (plpgsql_SpaceScanned)
					plpgsql_dstring_append(&ds, " ");
				plpgsql_dstring_append(&ds, plpgsql_yytext);

				while(1)
				{
					tok = plpgsql_yylex();
					if (tok == ';')
					{
						PLpgSQL_stmt_execsql	*execsql;

						expr = malloc(sizeof(PLpgSQL_expr) + sizeof(int) * nparams - sizeof(int));
						expr->dtype				= PLPGSQL_DTYPE_EXPR;
						expr->query				= strdup(plpgsql_dstring_get(&ds));
						expr->plan				= NULL;
						expr->nparams	= nparams;
						while (nparams-- > 0)
							expr->params[nparams] = params[nparams];
						plpgsql_dstring_free(&ds);

						execsql = malloc(sizeof(PLpgSQL_stmt_execsql));
						execsql->cmd_type = PLPGSQL_STMT_EXECSQL;
						execsql->sqlstmt  = expr;

						return (PLpgSQL_stmt *)execsql;
					}

					if (plpgsql_SpaceScanned)
						plpgsql_dstring_append(&ds, " ");
					switch (tok)
					{
						case T_VARIABLE:
							params[nparams] = plpgsql_yylval.var->varno;
							sprintf(buf, " $%d ", ++nparams);
							plpgsql_dstring_append(&ds, buf);
							break;

						case T_RECFIELD:
							params[nparams] = plpgsql_yylval.recfield->rfno;
							sprintf(buf, " $%d ", ++nparams);
							plpgsql_dstring_append(&ds, buf);
							break;

						case T_TGARGV:
							params[nparams] = plpgsql_yylval.trigarg->dno;
							sprintf(buf, " $%d ", ++nparams);
							plpgsql_dstring_append(&ds, buf);
							break;

						default:
							if (tok == 0)
							{
								plpgsql_error_lineno = plpgsql_yylineno;
								elog(ERROR, "unexpected end of file");
							}
							plpgsql_dstring_append(&ds, plpgsql_yytext);
							break;
					}
				}
			}
	}

	/************************************************************
	 * Eat up the rest of the statement after the target fields
	 ************************************************************/
	while(1)
	{
		if (!have_nexttok) {
			tok = plpgsql_yylex();
		}
		have_nexttok = 0;
		if (tok == ';') {
			break;
		}

		if (plpgsql_SpaceScanned)
			plpgsql_dstring_append(&ds, " ");
		switch (tok)
		{
			case T_VARIABLE:
				params[nparams] = plpgsql_yylval.var->varno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			case T_RECFIELD:
				params[nparams] = plpgsql_yylval.recfield->rfno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			case T_TGARGV:
				params[nparams] = plpgsql_yylval.trigarg->dno;
				sprintf(buf, " $%d ", ++nparams);
				plpgsql_dstring_append(&ds, buf);
				break;

			default:
				if (tok == 0)
				{
					plpgsql_error_lineno = plpgsql_yylineno;
					elog(ERROR, "unexpected end of file");
				}
				plpgsql_dstring_append(&ds, plpgsql_yytext);
				break;
		}
	}

	expr = malloc(sizeof(PLpgSQL_expr) + sizeof(int) * nparams - sizeof(int));
	expr->dtype			= PLPGSQL_DTYPE_EXPR;
	expr->query			= strdup(plpgsql_dstring_get(&ds));
	expr->plan			= NULL;
	expr->nparams		= nparams;
	while(nparams-- > 0)
		expr->params[nparams] = params[nparams];
	plpgsql_dstring_free(&ds);

	select = malloc(sizeof(PLpgSQL_stmt_select));
	memset(select, 0, sizeof(PLpgSQL_stmt_select));
	select->cmd_type = PLPGSQL_STMT_SELECT;
	select->rec		 = rec;
	select->row		 = row;
	select->query	 = expr;

	return (PLpgSQL_stmt *)select;
}


static PLpgSQL_stmt *
make_fetch_stmt()
{
	int					tok;
	PLpgSQL_row		   *row = NULL;
	PLpgSQL_rec		   *rec = NULL;
	PLpgSQL_stmt_fetch *fetch;
	int					have_nexttok = 0;

	tok = plpgsql_yylex();
	switch (tok)
	{
		case T_ROW:
			row = plpgsql_yylval.row;
			break;

		case T_RECORD:
			rec = plpgsql_yylval.rec;
			break;

		case T_VARIABLE:
		case T_RECFIELD:
			{
				PLpgSQL_var		*var;
				PLpgSQL_recfield *recfield;
				int				nfields = 1;
				char			*fieldnames[1024];
				int				varnos[1024];

				switch (tok)
				{	
					case T_VARIABLE:
						var = plpgsql_yylval.var;
						fieldnames[0] = strdup(plpgsql_yytext);
						varnos[0]	  = var->varno;
						break;

					case T_RECFIELD:
						recfield = plpgsql_yylval.recfield;
						fieldnames[0] = strdup(plpgsql_yytext);
						varnos[0]	  = recfield->rfno;
						break;
				}

				while ((tok = plpgsql_yylex()) == ',')
				{
					tok = plpgsql_yylex();
					switch(tok)
					{
						case T_VARIABLE:
							var = plpgsql_yylval.var;
							fieldnames[nfields] = strdup(plpgsql_yytext);
							varnos[nfields++]	= var->varno;
							break;

						case T_RECFIELD:
							recfield = plpgsql_yylval.recfield;
							fieldnames[0] = strdup(plpgsql_yytext);
							varnos[0]	  = recfield->rfno;
							break;

						default:
							plpgsql_error_lineno = plpgsql_yylineno;
							elog(ERROR, "plpgsql: %s is not a variable or record field", plpgsql_yytext);
					}
				}
				row = malloc(sizeof(PLpgSQL_row));
				row->dtype = PLPGSQL_DTYPE_ROW;
				row->refname = strdup("*internal*");
				row->lineno = plpgsql_yylineno;
				row->rowtypeclass = InvalidOid;
				row->nfields = nfields;
				row->fieldnames = malloc(sizeof(char *) * nfields);
				row->varnos = malloc(sizeof(int) * nfields);
				while (--nfields >= 0)
				{
					row->fieldnames[nfields] = fieldnames[nfields];
					row->varnos[nfields] = varnos[nfields];
				}

				plpgsql_adddatum((PLpgSQL_datum *)row);

				have_nexttok = 1;
			}
			break;

		default:
			plpgsql_error_lineno = plpgsql_yylineno;
			elog(ERROR, "syntax error at '%s'", plpgsql_yytext);
	}

	if (!have_nexttok)
		tok = plpgsql_yylex();

	if (tok != ';')
	{
		plpgsql_error_lineno = plpgsql_yylineno;
		elog(ERROR, "syntax error at '%s'", plpgsql_yytext);
	}

	fetch = malloc(sizeof(PLpgSQL_stmt_select));
	memset(fetch, 0, sizeof(PLpgSQL_stmt_fetch));
	fetch->cmd_type = PLPGSQL_STMT_FETCH;
	fetch->rec		 = rec;
	fetch->row		 = row;

	return (PLpgSQL_stmt *)fetch;
}


static PLpgSQL_expr *
make_tupret_expr(PLpgSQL_row *row)
{
	PLpgSQL_dstring		ds;
	PLpgSQL_expr		*expr;
	int					i;
	char				buf[16];

	expr = malloc(sizeof(PLpgSQL_expr) + sizeof(int) * (row->nfields - 1));
	expr->dtype			= PLPGSQL_DTYPE_EXPR;

	plpgsql_dstring_init(&ds);
	plpgsql_dstring_append(&ds, "SELECT ");

	for (i = 0; i < row->nfields; i++)
	{
		sprintf(buf, "%s$%d", (i > 0) ? "," : "", i + 1);
		plpgsql_dstring_append(&ds, buf);
		expr->params[i] = row->varnos[i];
	}

	expr->query			= strdup(plpgsql_dstring_get(&ds));
	expr->plan			= NULL;
	expr->plan_argtypes = NULL;
	expr->nparams		= row->nfields;

	plpgsql_dstring_free(&ds);
	return expr;
}
