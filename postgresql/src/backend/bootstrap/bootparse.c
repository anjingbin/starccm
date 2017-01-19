/* A Bison parser, made from bootparse.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	CONST	257
# define	ID	258
# define	OPEN	259
# define	XCLOSE	260
# define	XCREATE	261
# define	INSERT_TUPLE	262
# define	STRING	263
# define	XDEFINE	264
# define	XDECLARE	265
# define	INDEX	266
# define	ON	267
# define	USING	268
# define	XBUILD	269
# define	INDICES	270
# define	UNIQUE	271
# define	COMMA	272
# define	EQUALS	273
# define	LPAREN	274
# define	RPAREN	275
# define	OBJ_ID	276
# define	XBOOTSTRAP	277
# define	XWITHOUT_OIDS	278
# define	NULLVAL	279
# define	low	280
# define	high	281

#line 1 "bootparse.y"

/*-------------------------------------------------------------------------
 *
 * backendparse.y
 *	  yacc parser grammer for the "backend" initialization program.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/bootstrap/bootparse.c,v 1.3 2004/07/06 07:19:07 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <time.h>
#include <unistd.h>

#include "access/attnum.h"
#include "access/htup.h"
#include "access/itup.h"
#include "access/skey.h"
#include "access/strat.h"
#include "access/tupdesc.h"
#include "access/xact.h"
#include "bootstrap/bootstrap.h"
#include "catalog/catalog.h"
#include "catalog/heap.h"
#include "catalog/pg_am.h"
#include "catalog/pg_attribute.h"
#include "catalog/pg_class.h"
#include "commands/defrem.h"
#include "miscadmin.h"
#include "nodes/nodes.h"
#include "nodes/parsenodes.h"
#include "nodes/pg_list.h"
#include "nodes/primnodes.h"
#include "rewrite/prs2lock.h"
#include "storage/block.h"
#include "storage/fd.h"
#include "storage/ipc.h"
#include "storage/itemptr.h"
#include "storage/off.h"
#include "storage/smgr.h"
#include "tcop/dest.h"
#include "utils/nabstime.h"
#include "utils/rel.h"


static void
do_start()
{
	StartTransactionCommand();
	if (DebugMode)
		elog(DEBUG, "start transaction");
}


static void
do_end()
{
	CommitTransactionCommand();
	if (DebugMode)
		elog(DEBUG, "commit transaction");
	if (isatty(0))
	{
		printf("bootstrap> ");
		fflush(stdout);
	}
}


int num_columns_read = 0;


#line 80 "bootparse.y"
#ifndef YYSTYPE
typedef union
{
	List		*list;
	IndexElem	*ielem;
	char		*str;
	int			ival;
	Oid			oidval;
} Int_yystype;
# define YYSTYPE Int_yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		79
#define	YYFLAG		-32768
#define	YYNTBASE	28

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 281 ? Int_yytranslate[x] : 52)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char Int_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27
};

#if YYDEBUG
static const short Int_yyprhs[] =
{
       0,     0,     2,     3,     5,     8,    10,    12,    14,    16,
      18,    20,    22,    25,    28,    30,    31,    32,    42,    43,
      50,    61,    73,    76,    80,    82,    85,    87,    88,    90,
      91,    93,    97,   101,   105,   106,   108,   111,   115,   117,
     119,   121,   123
};
static const short Int_yyrhs[] =
{
      29,     0,     0,    30,     0,    29,    30,     0,    31,     0,
      32,     0,    33,     0,    36,     0,    38,     0,    39,     0,
      40,     0,     5,    51,     0,     6,    51,     0,     6,     0,
       0,     0,     7,    43,    44,    51,    20,    34,    45,    35,
      21,     0,     0,     8,    47,    37,    20,    48,    21,     0,
      11,    12,    51,    13,    51,    14,    51,    20,    41,    21,
       0,    11,    17,    12,    51,    13,    51,    14,    51,    20,
      41,    21,     0,    15,    16,     0,    41,    18,    42,     0,
      42,     0,    51,    51,     0,    23,     0,     0,    24,     0,
       0,    46,     0,    45,    18,    46,     0,    51,    19,    51,
       0,    22,    19,    51,     0,     0,    49,     0,    48,    49,
       0,    48,    18,    49,     0,    51,     0,    50,     0,    25,
       0,     3,     0,     4,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short Int_yyrline[] =
{
       0,   108,   110,   113,   115,   118,   120,   121,   122,   123,
     124,   125,   128,   137,   144,   152,   152,   152,   212,   212,
     240,   253,   266,   270,   272,   275,   284,   286,   289,   291,
     294,   296,   299,   308,   310,   313,   315,   316,   319,   322,
     324,   328,   332
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const Int_yytname[] =
{
  "$", "error", "$undefined.", "CONST", "ID", "OPEN", "XCLOSE", "XCREATE", 
  "INSERT_TUPLE", "STRING", "XDEFINE", "XDECLARE", "INDEX", "ON", "USING", 
  "XBUILD", "INDICES", "UNIQUE", "COMMA", "EQUALS", "LPAREN", "RPAREN", 
  "OBJ_ID", "XBOOTSTRAP", "XWITHOUT_OIDS", "NULLVAL", "low", "high", 
  "TopLevel", "Boot_Queries", "Boot_Query", "Boot_OpenStmt", 
  "Boot_CloseStmt", "Boot_CreateStmt", "@1", "@2", "Boot_InsertStmt", 
  "@3", "Boot_DeclareIndexStmt", "Boot_DeclareUniqueIndexStmt", 
  "Boot_BuildIndsStmt", "boot_index_params", "boot_index_param", 
  "optbootstrap", "optwithoutoids", "boot_typelist", "boot_type_thing", 
  "optoideq", "boot_tuplelist", "boot_tuple", "boot_const", "boot_ident", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short Int_yyr1[] =
{
       0,    28,    28,    29,    29,    30,    30,    30,    30,    30,
      30,    30,    31,    32,    32,    34,    35,    33,    37,    36,
      38,    39,    40,    41,    41,    42,    43,    43,    44,    44,
      45,    45,    46,    47,    47,    48,    48,    48,    49,    49,
      49,    50,    51
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short Int_yyr2[] =
{
       0,     1,     0,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     0,     0,     9,     0,     6,
      10,    11,     2,     3,     1,     2,     1,     0,     1,     0,
       1,     3,     3,     3,     0,     1,     2,     3,     1,     1,
       1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short Int_yydefact[] =
{
       2,     0,    14,    27,    34,     0,     0,     1,     3,     5,
       6,     7,     8,     9,    10,    11,    42,    12,    13,    26,
      29,     0,    18,     0,     0,    22,     4,    28,     0,     0,
       0,     0,     0,     0,    33,     0,     0,     0,    15,    41,
      40,     0,    35,    39,    38,     0,     0,     0,     0,    19,
      36,     0,     0,    16,    30,     0,    37,     0,     0,     0,
       0,     0,     0,     0,    31,    17,    32,     0,    24,     0,
       0,     0,    20,    25,     0,    23,    21,     0,     0,     0
};

static const short Int_yydefgoto[] =
{
      77,     7,     8,     9,    10,    11,    47,    60,    12,    30,
      13,    14,    15,    67,    68,    20,    28,    53,    54,    22,
      41,    42,    43,    44
};

static const short Int_yypact[] =
{
       5,     1,     1,   -17,    -7,     2,     7,     5,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
       8,    -2,-32768,     1,    14,-32768,-32768,-32768,     1,     1,
      18,    11,     1,    19,-32768,     4,     1,    23,-32768,-32768,
  -32768,     0,-32768,-32768,-32768,    26,     1,     1,     4,-32768,
  -32768,     1,    27,    24,-32768,    25,-32768,    28,     1,     1,
      22,     1,     1,    29,-32768,-32768,-32768,    12,-32768,     1,
       1,     1,-32768,-32768,    16,-32768,-32768,    47,    51,-32768
};

static const short Int_yypgoto[] =
{
  -32768,-32768,    45,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   -16,   -18,-32768,-32768,-32768,    -4,-32768,
  -32768,   -39,-32768,    -1
};


#define	YYLAST		70


static const short Int_yytable[] =
{
      17,    18,    50,    39,    16,    16,    19,    39,    16,    56,
       1,     2,     3,     4,    23,    21,     5,    29,    48,    24,
       6,    49,    31,    25,    36,    40,    32,    33,    34,    40,
      71,    37,    27,    72,    71,    45,    46,    76,    35,    38,
      51,    58,    59,    65,    61,    52,    55,    78,    62,    70,
      57,    79,    26,    75,    74,    64,     0,    63,    55,     0,
      66,    69,     0,     0,     0,     0,     0,     0,    73,    69,
      69
};

static const short Int_yycheck[] =
{
       1,     2,    41,     3,     4,     4,    23,     3,     4,    48,
       5,     6,     7,     8,    12,    22,    11,    19,    18,    17,
      15,    21,    23,    16,    13,    25,    12,    28,    29,    25,
      18,    32,    24,    21,    18,    36,    13,    21,    20,    20,
      14,    14,    18,    21,    19,    46,    47,     0,    20,    20,
      51,     0,     7,    71,    70,    59,    -1,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71
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

/* All symbols defined below should begin with Int_yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (Int_yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (Int_yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (Int_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union Int_yyalloc
{
  short Int_yyss;
  YYSTYPE Int_yyvs;
# if YYLSP_NEEDED
  YYLTYPE Int_yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union Int_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T Int_yyi;		\
	  for (Int_yyi = 0; Int_yyi < (Count); Int_yyi++)	\
	    (To)[Int_yyi] = (From)[Int_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T Int_yynewbytes;						\
	YYCOPY (&Int_yyptr->Stack, Stack, Int_yysize);				\
	Stack = &Int_yyptr->Stack;						\
	Int_yynewbytes = Int_yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	Int_yyptr += Int_yynewbytes / sizeof (*Int_yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define Int_yyerrok		(Int_yyerrstatus = 0)
#define Int_yyclearin	(Int_yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto Int_yyacceptlab
#define YYABORT 	goto Int_yyabortlab
#define YYERROR		goto Int_yyerrlab1
/* Like YYERROR except do call Int_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto Int_yyerrlab
#define YYRECOVERING()  (!!Int_yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (Int_yychar == YYEMPTY && Int_yylen == 1)				\
    {								\
      Int_yychar = (Token);						\
      Int_yylval = (Value);						\
      Int_yychar1 = YYTRANSLATE (Int_yychar);				\
      YYPOPSTACK;						\
      goto Int_yybackup;						\
    }								\
  else								\
    { 								\
      Int_yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `Int_yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		Int_yylex (&Int_yylval, &Int_yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		Int_yylex (&Int_yylval, &Int_yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		Int_yylex (&Int_yylval, YYLEX_PARAM)
#  else
#   define YYLEX		Int_yylex (&Int_yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			Int_yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (Int_yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int Int_yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef Int_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define Int_yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
Int_yystrlen (const char *Int_yystr)
#   else
Int_yystrlen (Int_yystr)
     const char *Int_yystr;
#   endif
{
  register const char *Int_yys = Int_yystr;

  while (*Int_yys++ != '\0')
    continue;

  return Int_yys - Int_yystr - 1;
}
#  endif
# endif

# ifndef Int_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define Int_yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
Int_yystpcpy (char *Int_yydest, const char *Int_yysrc)
#   else
Int_yystpcpy (Int_yydest, Int_yysrc)
     char *Int_yydest;
     const char *Int_yysrc;
#   endif
{
  register char *Int_yyd = Int_yydest;
  register const char *Int_yys = Int_yysrc;

  while ((*Int_yyd++ = *Int_yys++) != '\0')
    continue;

  return Int_yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into Int_yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int Int_yyparse (void *);
# else
int Int_yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int Int_yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE Int_yylval;						\
							\
/* Number of parse errors so far.  */			\
int Int_yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE Int_yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
Int_yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int Int_yystate;
  register int Int_yyn;
  int Int_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int Int_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int Int_yychar1 = 0;

  /* Three stacks and their tools:
     `Int_yyss': related to states,
     `Int_yyvs': related to semantic values,
     `Int_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow Int_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	Int_yyssa[YYINITDEPTH];
  short *Int_yyss = Int_yyssa;
  register short *Int_yyssp;

  /* The semantic value stack.  */
  YYSTYPE Int_yyvsa[YYINITDEPTH];
  YYSTYPE *Int_yyvs = Int_yyvsa;
  register YYSTYPE *Int_yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE Int_yylsa[YYINITDEPTH];
  YYLTYPE *Int_yyls = Int_yylsa;
  YYLTYPE *Int_yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (Int_yyvsp--, Int_yyssp--, Int_yylsp--)
#else
# define YYPOPSTACK   (Int_yyvsp--, Int_yyssp--)
#endif

  YYSIZE_T Int_yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE Int_yyval;
#if YYLSP_NEEDED
  YYLTYPE Int_yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int Int_yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  Int_yystate = 0;
  Int_yyerrstatus = 0;
  Int_yynerrs = 0;
  Int_yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  Int_yyssp = Int_yyss;
  Int_yyvsp = Int_yyvs;
#if YYLSP_NEEDED
  Int_yylsp = Int_yyls;
#endif
  goto Int_yysetstate;

/*------------------------------------------------------------.
| Int_yynewstate -- Push a new state, which is found in Int_yystate.  |
`------------------------------------------------------------*/
 Int_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  Int_yyssp++;

 Int_yysetstate:
  *Int_yyssp = Int_yystate;

  if (Int_yyssp >= Int_yyss + Int_yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T Int_yysize = Int_yyssp - Int_yyss + 1;

#ifdef Int_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *Int_yyvs1 = Int_yyvs;
	short *Int_yyss1 = Int_yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *Int_yyls1 = Int_yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if Int_yyoverflow is a macro.  */
	Int_yyoverflow ("parser stack overflow",
		    &Int_yyss1, Int_yysize * sizeof (*Int_yyssp),
		    &Int_yyvs1, Int_yysize * sizeof (*Int_yyvsp),
		    &Int_yyls1, Int_yysize * sizeof (*Int_yylsp),
		    &Int_yystacksize);
	Int_yyls = Int_yyls1;
# else
	Int_yyoverflow ("parser stack overflow",
		    &Int_yyss1, Int_yysize * sizeof (*Int_yyssp),
		    &Int_yyvs1, Int_yysize * sizeof (*Int_yyvsp),
		    &Int_yystacksize);
# endif
	Int_yyss = Int_yyss1;
	Int_yyvs = Int_yyvs1;
      }
#else /* no Int_yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto Int_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (Int_yystacksize >= YYMAXDEPTH)
	goto Int_yyoverflowlab;
      Int_yystacksize *= 2;
      if (Int_yystacksize > YYMAXDEPTH)
	Int_yystacksize = YYMAXDEPTH;

      {
	short *Int_yyss1 = Int_yyss;
	union Int_yyalloc *Int_yyptr =
	  (union Int_yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (Int_yystacksize));
	if (! Int_yyptr)
	  goto Int_yyoverflowlab;
	YYSTACK_RELOCATE (Int_yyss);
	YYSTACK_RELOCATE (Int_yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (Int_yyls);
# endif
# undef YYSTACK_RELOCATE
	if (Int_yyss1 != Int_yyssa)
	  YYSTACK_FREE (Int_yyss1);
      }
# endif
#endif /* no Int_yyoverflow */

      Int_yyssp = Int_yyss + Int_yysize - 1;
      Int_yyvsp = Int_yyvs + Int_yysize - 1;
#if YYLSP_NEEDED
      Int_yylsp = Int_yyls + Int_yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) Int_yystacksize));

      if (Int_yyssp >= Int_yyss + Int_yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", Int_yystate));

  goto Int_yybackup;


/*-----------.
| Int_yybackup.  |
`-----------*/
Int_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* Int_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  Int_yyn = Int_yypact[Int_yystate];
  if (Int_yyn == YYFLAG)
    goto Int_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* Int_yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (Int_yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      Int_yychar = YYLEX;
    }

  /* Convert token to internal form (in Int_yychar1) for indexing tables with */

  if (Int_yychar <= 0)		/* This means end of input. */
    {
      Int_yychar1 = 0;
      Int_yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      Int_yychar1 = YYTRANSLATE (Int_yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (Int_yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     Int_yychar, Int_yytname[Int_yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, Int_yychar, Int_yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  Int_yyn += Int_yychar1;
  if (Int_yyn < 0 || Int_yyn > YYLAST || Int_yycheck[Int_yyn] != Int_yychar1)
    goto Int_yydefault;

  Int_yyn = Int_yytable[Int_yyn];

  /* Int_yyn is what to do for this token type in this state.
     Negative => reduce, -Int_yyn is rule number.
     Positive => shift, Int_yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (Int_yyn < 0)
    {
      if (Int_yyn == YYFLAG)
	goto Int_yyerrlab;
      Int_yyn = -Int_yyn;
      goto Int_yyreduce;
    }
  else if (Int_yyn == 0)
    goto Int_yyerrlab;

  if (Int_yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      Int_yychar, Int_yytname[Int_yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (Int_yychar != YYEOF)
    Int_yychar = YYEMPTY;

  *++Int_yyvsp = Int_yylval;
#if YYLSP_NEEDED
  *++Int_yylsp = Int_yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (Int_yyerrstatus)
    Int_yyerrstatus--;

  Int_yystate = Int_yyn;
  goto Int_yynewstate;


/*-----------------------------------------------------------.
| Int_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
Int_yydefault:
  Int_yyn = Int_yydefact[Int_yystate];
  if (Int_yyn == 0)
    goto Int_yyerrlab;
  goto Int_yyreduce;


/*-----------------------------.
| Int_yyreduce -- Do a reduction.  |
`-----------------------------*/
Int_yyreduce:
  /* Int_yyn is the number of a rule to reduce with.  */
  Int_yylen = Int_yyr2[Int_yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  Int_yyval = Int_yyvsp[1-Int_yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  Int_yyloc = Int_yylsp[1-Int_yylen];
  YYLLOC_DEFAULT (Int_yyloc, (Int_yylsp - Int_yylen), Int_yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (Int_yydebug)
    {
      int Int_yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 Int_yyn, Int_yyrline[Int_yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (Int_yyi = Int_yyprhs[Int_yyn]; Int_yyrhs[Int_yyi] > 0; Int_yyi++)
	YYFPRINTF (stderr, "%s ", Int_yytname[Int_yyrhs[Int_yyi]]);
      YYFPRINTF (stderr, " -> %s\n", Int_yytname[Int_yyr1[Int_yyn]]);
    }
#endif

  switch (Int_yyn) {

case 12:
#line 130 "bootparse.y"
{
					do_start();
					boot_openrel(LexIDStr(Int_yyvsp[0].ival));
					do_end();
				}
    break;
case 13:
#line 139 "bootparse.y"
{
					do_start();
					closerel(LexIDStr(Int_yyvsp[0].ival));
					do_end();
				}
    break;
case 14:
#line 145 "bootparse.y"
{
					do_start();
					closerel(NULL);
					do_end();
				}
    break;
case 15:
#line 154 "bootparse.y"
{
					do_start();
					numattr = 0;
					if (DebugMode)
					{
						if (Int_yyvsp[-3].ival)
							elog(DEBUG, "creating bootstrap relation %s...",
								 LexIDStr(Int_yyvsp[-1].ival));
						else
							elog(DEBUG, "creating relation %s...",
								 LexIDStr(Int_yyvsp[-1].ival));
					}
				}
    break;
case 16:
#line 168 "bootparse.y"
{
					do_end();
				}
    break;
case 17:
#line 172 "bootparse.y"
{
					do_start();

					if (Int_yyvsp[-7].ival)
					{
						extern Relation reldesc;
						TupleDesc tupdesc;

						if (reldesc)
						{
							elog(DEBUG, "create bootstrap: warning, open relation exists, closing first");
							closerel(NULL);
						}

						tupdesc = CreateTupleDesc(numattr, attrtypes);
						reldesc = heap_create(LexIDStr(Int_yyvsp[-5].ival), tupdesc,
											  false, true, true);
						reldesc->rd_rel->relhasoids = ! (Int_yyvsp[-6].ival);
						if (DebugMode)
							elog(DEBUG, "bootstrap relation created");
					}
					else
					{
						Oid id;
						TupleDesc tupdesc;

						tupdesc = CreateTupleDesc(numattr,attrtypes);
						id = heap_create_with_catalog(LexIDStr(Int_yyvsp[-5].ival),
													  tupdesc,
													  RELKIND_RELATION,
													  ! (Int_yyvsp[-6].ival),
													  false,
													  true);
						if (DebugMode)
							elog(DEBUG, "relation created with oid %u", id);
					}
					do_end();
				}
    break;
case 18:
#line 214 "bootparse.y"
{
					do_start();
					if (DebugMode)
					{
						if (Int_yyvsp[0].oidval)
							elog(DEBUG, "inserting row with oid %u...", Int_yyvsp[0].oidval);
						else
							elog(DEBUG, "inserting row...");
					}
					num_columns_read = 0;
				}
    break;
case 19:
#line 226 "bootparse.y"
{
					if (num_columns_read != numattr)
						elog(ERROR, "incorrect number of columns in row (expected %d, got %d)",
							 numattr, num_columns_read);
					if (reldesc == (Relation)NULL)
					{
						elog(ERROR, "relation not open");
						err_out();
					}
					InsertOneTuple(Int_yyvsp[-4].oidval);
					do_end();
				}
    break;
case 20:
#line 242 "bootparse.y"
{
					do_start();

					DefineIndex(LexIDStr(Int_yyvsp[-5].ival),
								LexIDStr(Int_yyvsp[-7].ival),
								LexIDStr(Int_yyvsp[-3].ival),
								Int_yyvsp[-1].list, false, false, NULL, NIL);
					do_end();
				}
    break;
case 21:
#line 255 "bootparse.y"
{
					do_start();

					DefineIndex(LexIDStr(Int_yyvsp[-5].ival),
								LexIDStr(Int_yyvsp[-7].ival),
								LexIDStr(Int_yyvsp[-3].ival),
								Int_yyvsp[-1].list, true, false, NULL, NIL);
					do_end();
				}
    break;
case 22:
#line 267 "bootparse.y"
{ build_indices(); }
    break;
case 23:
#line 271 "bootparse.y"
{ Int_yyval.list = lappend(Int_yyvsp[-2].list, Int_yyvsp[0].ielem); }
    break;
case 24:
#line 272 "bootparse.y"
{ Int_yyval.list = makeList1(Int_yyvsp[0].ielem); }
    break;
case 25:
#line 277 "bootparse.y"
{
					IndexElem *n = makeNode(IndexElem);
					n->name = LexIDStr(Int_yyvsp[-1].ival);
					n->class = LexIDStr(Int_yyvsp[0].ival);
					Int_yyval.ielem = n;
				}
    break;
case 26:
#line 285 "bootparse.y"
{ Int_yyval.ival = 1; }
    break;
case 27:
#line 286 "bootparse.y"
{ Int_yyval.ival = 0; }
    break;
case 28:
#line 290 "bootparse.y"
{ Int_yyval.ival = 1; }
    break;
case 29:
#line 291 "bootparse.y"
{ Int_yyval.ival = 0; }
    break;
case 32:
#line 301 "bootparse.y"
{
				   if(++numattr > MAXATTR)
						elog(FATAL, "too many columns");
				   DefineAttr(LexIDStr(Int_yyvsp[-2].ival),LexIDStr(Int_yyvsp[0].ival),numattr-1);
				}
    break;
case 33:
#line 309 "bootparse.y"
{ Int_yyval.oidval = atol(LexIDStr(Int_yyvsp[0].ival));	}
    break;
case 34:
#line 310 "bootparse.y"
{ Int_yyval.oidval = (Oid) 0;	}
    break;
case 38:
#line 321 "bootparse.y"
{ InsertOneValue(LexIDStr(Int_yyvsp[0].ival), num_columns_read++); }
    break;
case 39:
#line 323 "bootparse.y"
{ InsertOneValue(LexIDStr(Int_yyvsp[0].ival), num_columns_read++); }
    break;
case 40:
#line 325 "bootparse.y"
{ InsertOneNull(num_columns_read++); }
    break;
case 41:
#line 329 "bootparse.y"
{ Int_yyval.ival=Int_yylval.ival; }
    break;
case 42:
#line 333 "bootparse.y"
{ Int_yyval.ival=Int_yylval.ival; }
    break;
}

#line 705 "/usr/share/bison/bison.simple"


  Int_yyvsp -= Int_yylen;
  Int_yyssp -= Int_yylen;
#if YYLSP_NEEDED
  Int_yylsp -= Int_yylen;
#endif

#if YYDEBUG
  if (Int_yydebug)
    {
      short *Int_yyssp1 = Int_yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (Int_yyssp1 != Int_yyssp)
	YYFPRINTF (stderr, " %d", *++Int_yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++Int_yyvsp = Int_yyval;
#if YYLSP_NEEDED
  *++Int_yylsp = Int_yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  Int_yyn = Int_yyr1[Int_yyn];

  Int_yystate = Int_yypgoto[Int_yyn - YYNTBASE] + *Int_yyssp;
  if (Int_yystate >= 0 && Int_yystate <= YYLAST && Int_yycheck[Int_yystate] == *Int_yyssp)
    Int_yystate = Int_yytable[Int_yystate];
  else
    Int_yystate = Int_yydefgoto[Int_yyn - YYNTBASE];

  goto Int_yynewstate;


/*------------------------------------.
| Int_yyerrlab -- here on detecting error |
`------------------------------------*/
Int_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!Int_yyerrstatus)
    {
      ++Int_yynerrs;

#ifdef YYERROR_VERBOSE
      Int_yyn = Int_yypact[Int_yystate];

      if (Int_yyn > YYFLAG && Int_yyn < YYLAST)
	{
	  YYSIZE_T Int_yysize = 0;
	  char *Int_yymsg;
	  int Int_yyx, Int_yycount;

	  Int_yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (Int_yyx = Int_yyn < 0 ? -Int_yyn : 0;
	       Int_yyx < (int) (sizeof (Int_yytname) / sizeof (char *)); Int_yyx++)
	    if (Int_yycheck[Int_yyx + Int_yyn] == Int_yyx)
	      Int_yysize += Int_yystrlen (Int_yytname[Int_yyx]) + 15, Int_yycount++;
	  Int_yysize += Int_yystrlen ("parse error, unexpected ") + 1;
	  Int_yysize += Int_yystrlen (Int_yytname[YYTRANSLATE (Int_yychar)]);
	  Int_yymsg = (char *) YYSTACK_ALLOC (Int_yysize);
	  if (Int_yymsg != 0)
	    {
	      char *Int_yyp = Int_yystpcpy (Int_yymsg, "parse error, unexpected ");
	      Int_yyp = Int_yystpcpy (Int_yyp, Int_yytname[YYTRANSLATE (Int_yychar)]);

	      if (Int_yycount < 5)
		{
		  Int_yycount = 0;
		  for (Int_yyx = Int_yyn < 0 ? -Int_yyn : 0;
		       Int_yyx < (int) (sizeof (Int_yytname) / sizeof (char *));
		       Int_yyx++)
		    if (Int_yycheck[Int_yyx + Int_yyn] == Int_yyx)
		      {
			const char *Int_yyq = ! Int_yycount ? ", expecting " : " or ";
			Int_yyp = Int_yystpcpy (Int_yyp, Int_yyq);
			Int_yyp = Int_yystpcpy (Int_yyp, Int_yytname[Int_yyx]);
			Int_yycount++;
		      }
		}
	      Int_yyerror (Int_yymsg);
	      YYSTACK_FREE (Int_yymsg);
	    }
	  else
	    Int_yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	Int_yyerror ("parse error");
    }
  goto Int_yyerrlab1;


/*--------------------------------------------------.
| Int_yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
Int_yyerrlab1:
  if (Int_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (Int_yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  Int_yychar, Int_yytname[Int_yychar1]));
      Int_yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  Int_yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto Int_yyerrhandle;


/*-------------------------------------------------------------------.
| Int_yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
Int_yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  Int_yyn = Int_yydefact[Int_yystate];
  if (Int_yyn)
    goto Int_yydefault;
#endif


/*---------------------------------------------------------------.
| Int_yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
Int_yyerrpop:
  if (Int_yyssp == Int_yyss)
    YYABORT;
  Int_yyvsp--;
  Int_yystate = *--Int_yyssp;
#if YYLSP_NEEDED
  Int_yylsp--;
#endif

#if YYDEBUG
  if (Int_yydebug)
    {
      short *Int_yyssp1 = Int_yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (Int_yyssp1 != Int_yyssp)
	YYFPRINTF (stderr, " %d", *++Int_yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| Int_yyerrhandle.  |
`--------------*/
Int_yyerrhandle:
  Int_yyn = Int_yypact[Int_yystate];
  if (Int_yyn == YYFLAG)
    goto Int_yyerrdefault;

  Int_yyn += YYTERROR;
  if (Int_yyn < 0 || Int_yyn > YYLAST || Int_yycheck[Int_yyn] != YYTERROR)
    goto Int_yyerrdefault;

  Int_yyn = Int_yytable[Int_yyn];
  if (Int_yyn < 0)
    {
      if (Int_yyn == YYFLAG)
	goto Int_yyerrpop;
      Int_yyn = -Int_yyn;
      goto Int_yyreduce;
    }
  else if (Int_yyn == 0)
    goto Int_yyerrpop;

  if (Int_yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++Int_yyvsp = Int_yylval;
#if YYLSP_NEEDED
  *++Int_yylsp = Int_yylloc;
#endif

  Int_yystate = Int_yyn;
  goto Int_yynewstate;


/*-------------------------------------.
| Int_yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
Int_yyacceptlab:
  Int_yyresult = 0;
  goto Int_yyreturn;

/*-----------------------------------.
| Int_yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
Int_yyabortlab:
  Int_yyresult = 1;
  goto Int_yyreturn;

/*---------------------------------------------.
| Int_yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
Int_yyoverflowlab:
  Int_yyerror ("parser stack overflow");
  Int_yyresult = 2;
  /* Fall through.  */

Int_yyreturn:
#ifndef Int_yyoverflow
  if (Int_yyss != Int_yyssa)
    YYSTACK_FREE (Int_yyss);
#endif
  return Int_yyresult;
}
#line 335 "bootparse.y"

