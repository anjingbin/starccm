/* -----------------------------------------------------------------------
 * formatting.c
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/adt/formatting.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 *
 *	 Portions Copyright (c) 1999-2000, PostgreSQL Global Development Group
 *
 *
 *	 TO_CHAR(); TO_TIMESTAMP(); TO_DATE(); TO_NUMBER();
 *
 *	 The PostgreSQL routines for a timestamp/int/float/numeric formatting,
 *	 inspire with Oracle TO_CHAR() / TO_DATE() / TO_NUMBER() routines.
 *
 *
 *	 Cache & Memory:
 *	Routines use (itself) internal cache for format pictures.
 *
 *	The cache uses a static buffers and is persistent across transactions.
 *	If format-picture is bigger than cache buffer, parser is called always.
 *
 *	 NOTE for Number version:
 *	All in this version is implemented as keywords ( => not used
 *	suffixes), because a format picture is for *one* item (number)
 *	only. It not is as a timestamp version, where each keyword (can)
 *	has suffix.
 *
 *	 NOTE for Timestamp routines:
 *	In this module the POSIX 'struct tm' type is *not* used, but rather
 *	PgSQL type, which has tm_mon based on one (*non* zero) and
 *	year *not* based on 1900, but is used full year number.
 *	Module supports AD / BC / AM / PM.
 *
 *	Supported types for to_char():
 *
 *		Timestamp, Numeric, int4, int8, float4, float8
 *
 *	Supported types for reverse conversion:
 *
 *		Timestamp	- to_timestamp()
 *		Date		- to_date()
 *		Numeric		- to_number()
 *
 *
 *	Karel Zak
 *
 * TODO
 *	- better number building (formatting) / parsing, now it isn't
 *		  ideal code
 *	- use Assert()
 *	- add support for abstime
 *	- add support for roman number to standard number conversion
 *	- add support for number spelling
 *	- add support for string to string formatting (we must be better
 *	  than Oracle :-),
 *		to_char('Hello', 'X X X X X') -> 'H e l l o'
 *
 * -----------------------------------------------------------------------
 */

/* ----------
 * UnComment me for DEBUG
 * ----------
 */
/***
#define DEBUG_TO_FROM_CHAR
#define DEBUG_elog_output	NOTICE
***/

#include "postgres.h"

#include <ctype.h>
#include <sys/time.h>
#include <unistd.h>
#ifdef USE_LOCALE
#include <locale.h>
#endif
#include <math.h>
#include <float.h>

#include "utils/builtins.h"
#include "utils/date.h"
#include "utils/datetime.h"
#include "utils/formatting.h"
#include "utils/int8.h"
#include "utils/numeric.h"
#include "utils/pg_locale.h"

/* ----------
 * Routines type
 * ----------
 */
#define DCH_TYPE		1		/* DATE-TIME version	*/
#define NUM_TYPE		2		/* NUMBER version	*/

/* ----------
 * KeyWord Index (ascii from position 32 (' ') to 126 (~))
 * ----------
 */
#define KeyWord_INDEX_SIZE		('~' - ' ')
#define KeyWord_INDEX_FILTER(_c)	((_c) <= ' ' || (_c) >= '~' ? 0 : 1)

/* ----------
 * Maximal length of one node
 * ----------
 */
#define DCH_MAX_ITEM_SIZ		9		/* max julian day		*/
#define NUM_MAX_ITEM_SIZ		8		/* roman number (RN has 15 chars)	*/

/* ----------
 * More is in float.c
 * ----------
 */
#define MAXFLOATWIDTH	64
#define MAXDOUBLEWIDTH	128

/* ----------
 * External (defined in PgSQL dt.c (timestamp utils))
 * ----------
 */
extern char *months[],			/* month abbreviation	*/
		   *days[];				/* full days		*/

/* ----------
 * Format parser structs
 * ----------
 */
typedef struct
{
	char	   *name;			/* suffix string		*/
	int			len,			/* suffix length		*/
				id,				/* used in node->suffix */
				type;			/* prefix / postfix			*/
} KeySuffix;

typedef struct
{
	char	   *name;			/* keyword			*/
	/* action for keyword		*/
	int			len,			/* keyword length		*/
				(*action) (),
				id;				/* keyword id			*/
	bool		isitdigit;		/* is expected output/input digit */
} KeyWord;

typedef struct
{
	int			type;			/* node type			*/
	KeyWord    *key;			/* if node type is KEYWORD	*/
	int			character,		/* if node type is CHAR		*/
				suffix;			/* keyword suffix		*/
} FormatNode;

#define NODE_TYPE_END		1
#define NODE_TYPE_ACTION	2
#define NODE_TYPE_CHAR		3

#define SUFFTYPE_PREFIX		1
#define SUFFTYPE_POSTFIX	2


/* ----------
 * Full months
 * ----------
 */
static char *months_full[] = {
	"January", "February", "March", "April", "May", "June", "July",
	"August", "September", "October", "November", "December", NULL
};

/* ----------
 * AC / DC
 * ----------
 */
#define YEAR_ABS(_y)	(_y < 0 ? -(_y -1) : _y)
#define BC_STR_ORIG " BC"

#define A_D_STR		"A.D."
#define a_d_STR		"a.d."
#define AD_STR		"AD"
#define ad_STR		"ad"

#define B_C_STR		"B.C."
#define b_c_STR		"b.c."
#define BC_STR		"BC"
#define bc_STR		"bc"


/* ----------
 * AM / PM
 * ----------
 */
#define A_M_STR		"A.M."
#define a_m_STR		"a.m."
#define AM_STR		"AM"
#define am_STR		"am"

#define P_M_STR		"P.M."
#define p_m_STR		"p.m."
#define PM_STR		"PM"
#define pm_STR		"pm"


/* ----------
 * Months in roman-numeral
 * (Must be conversely for seq_search (in FROM_CHAR), because
 *	'VIII' must be over 'V')
 * ----------
 */
static char *rm_months_upper[] =
{"XII", "XI", "X", "IX", "VIII", "VII", "VI", "V", "IV", "III", "II", "I", NULL};

static char *rm_months_lower[] =
{"xii", "xi", "x", "ix", "viii", "vii", "vi", "v", "iv", "iii", "ii", "i", NULL};

/* ----------
 * Roman numbers
 * ----------
 */
static char *rm1[] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", NULL};
static char *rm10[] = {"X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC", NULL};
static char *rm100[] = {"C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM", NULL};

/* ----------
 * Ordinal postfixes
 * ----------
 */
static char *numTH[] = {"ST", "ND", "RD", "TH", NULL};
static char *numth[] = {"st", "nd", "rd", "th", NULL};

/* ----------
 * Flags & Options:
 * ----------
 */
#define TO_CHAR		1
#define FROM_CHAR	2

#define ONE_UPPER	1			/* Name */
#define ALL_UPPER	2			/* NAME */
#define ALL_LOWER	3			/* name */

#define FULL_SIZ	0

#define MAX_MON_LEN 3
#define MAX_DY_LEN	3

#define TH_UPPER	1
#define TH_LOWER	2

/* ----------
 * Flags for DCH version
 * ----------
 */
static int	DCH_global_flag = 0;

#define DCH_F_FX	0x01

#define IS_FX		(DCH_global_flag & DCH_F_FX)


/* ----------
 * Number description struct
 * ----------
 */
typedef struct
{
	int			pre,			/* (count) numbers before decimal */
				post,			/* (count) numbers after decimal  */
				lsign,			/* want locales sign		  */
				flag,			/* number parametrs		  */
				pre_lsign_num,	/* tmp value for lsign		  */
				multi,			/* multiplier for 'V'		  */
				zero_start,		/* position of first zero	  */
				zero_end,		/* position of last zero	  */
				need_locale;	/* needs it locale		  */
} NUMDesc;

/* ----------
 * Flags for NUMBER version
 * ----------
 */
#define NUM_F_DECIMAL	0x01
#define NUM_F_LDECIMAL	0x02
#define NUM_F_ZERO	0x04
#define NUM_F_BLANK 0x08
#define NUM_F_FILLMODE	0x10
#define NUM_F_LSIGN 0x20
#define NUM_F_BRACKET	0x40
#define NUM_F_MINUS 0x80
#define NUM_F_PLUS	0x100
#define NUM_F_ROMAN 0x200
#define NUM_F_MULTI 0x400

#define NUM_LSIGN_PRE	-1
#define NUM_LSIGN_POST	1
#define NUM_LSIGN_NONE	0

/* ----------
 * Tests
 * ----------
 */
#define IS_DECIMAL(_f)	((_f)->flag & NUM_F_DECIMAL)
#define IS_LDECIMAL(_f) ((_f)->flag & NUM_F_LDECIMAL)
#define IS_ZERO(_f) ((_f)->flag & NUM_F_ZERO)
#define IS_BLANK(_f)	((_f)->flag & NUM_F_BLANK)
#define IS_FILLMODE(_f) ((_f)->flag & NUM_F_FILLMODE)
#define IS_BRACKET(_f)	((_f)->flag & NUM_F_BRACKET)
#define IS_MINUS(_f)	((_f)->flag & NUM_F_MINUS)
#define IS_LSIGN(_f)	((_f)->flag & NUM_F_LSIGN)
#define IS_PLUS(_f) ((_f)->flag & NUM_F_PLUS)
#define IS_ROMAN(_f)	((_f)->flag & NUM_F_ROMAN)
#define IS_MULTI(_f)	((_f)->flag & NUM_F_MULTI)

/* ----------
 * Format picture cache
 *	(cache size:
 *		Number part = NUM_CACHE_SIZE * NUM_CACHE_FIELDS
 *		Date-time part	= DCH_CACHE_SIZE * DCH_CACHE_FIELDS
 *	)
 * ----------
 */
#define NUM_CACHE_SIZE		64
#define NUM_CACHE_FIELDS	16
#define DCH_CACHE_SIZE		128
#define DCH_CACHE_FIELDS	16

typedef struct
{
	FormatNode	format[DCH_CACHE_SIZE + 1];
	char		str[DCH_CACHE_SIZE + 1];
	int			age;
} DCHCacheEntry;

typedef struct
{
	FormatNode	format[NUM_CACHE_SIZE + 1];
	char		str[NUM_CACHE_SIZE + 1];
	int			age;
	NUMDesc		Num;
} NUMCacheEntry;

/* global cache for --- date/time part */
static DCHCacheEntry DCHCache[DCH_CACHE_FIELDS + 1];

static int	n_DCHCache = 0;		/* number of entries */
static int	DCHCounter = 0;

/* global cache for --- number part */
static NUMCacheEntry NUMCache[NUM_CACHE_FIELDS + 1];
static NUMCacheEntry *last_NUMCacheEntry;

static int	n_NUMCache = 0;		/* number of entries */
static int	NUMCounter = 0;

#define MAX_INT32	(2147483600)

/* ----------
 * For char->date/time conversion
 * ----------
 */
typedef struct
{
	int			hh,
				am,
				pm,
				mi,
				ss,
				ssss,
				d,
				dd,
				ddd,
				mm,
				ms,
				year,
				bc,
				iw,
				ww,
				w,
				cc,
				q,
				j,
				us;
} TmFromChar;

#define ZERO_tmfc( _X ) memset(_X, 0, sizeof(TmFromChar))

/* ----------
 * Debug
 * ----------
 */
#ifdef DEBUG_TO_FROM_CHAR
#define DEBUG_TMFC( _X ) \
		elog(DEBUG_elog_output, "TMFC:\nhh %d\nam %d\npm %d\nmi %d\nss %d\nssss %d\nd %d\ndd %d\nddd %d\nmm %d\nms: %d\nyear %d\nbc %d\niw %d\nww %d\nw %d\ncc %d\nq %d\nj %d\nus: %d", \
			(_X)->hh, (_X)->am, (_X)->pm, (_X)->mi, (_X)->ss, \
			(_X)->ssss, (_X)->d, (_X)->dd, (_X)->ddd, (_X)->mm, (_X)->ms, \
			(_X)->year, (_X)->bc, (_X)->iw, (_X)->ww, (_X)->w, \
			(_X)->cc, (_X)->q, (_X)->j, (_X)->us);
#define DEBUG_TM( _X ) \
		elog(DEBUG_elog_output, "TM:\nsec %d\nyear %d\nmin %d\nwday %d\nhour %d\nyday %d\nmday %d\nnisdst %d\nmon %d\n",\
			(_X)->tm_sec, (_X)->tm_year,\
			(_X)->tm_min, (_X)->tm_wday, (_X)->tm_hour, (_X)->tm_yday,\
			(_X)->tm_mday, (_X)->tm_isdst, (_X)->tm_mon)
#else
#define DEBUG_TMFC( _X )
#define DEBUG_TM( _X )
#endif

/* ----------
 * Datetime to char conversion
 * ----------
 */
typedef struct TmToChar
{
	struct tm	tm;				/* classic 'tm' struct */
	double		fsec;			/* milliseconds */
	char	   *tzn;			/* timezone */
} TmToChar;

#define tmtcTm(_X)	(&(_X)->tm)
#define tmtcTzn(_X) ((_X)->tzn)
#define tmtcFsec(_X)	((_X)->fsec)

#define ZERO_tm( _X ) \
	do {	\
		(_X)->tm_sec  = (_X)->tm_year = (_X)->tm_min = (_X)->tm_wday = \
		(_X)->tm_hour = (_X)->tm_yday = (_X)->tm_isdst = 0; \
		(_X)->tm_mday = (_X)->tm_mon  = 1; \
	} while(0)

#define ZERO_tmtc( _X ) \
	do { \
		ZERO_tm( tmtcTm(_X) ); \
		tmtcFsec(_X) = 0; \
		tmtcTzn(_X) = NULL; \
	} while(0)

/* ----------
 * Utils
 * ----------
 */
#ifndef MIN
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif

/*****************************************************************************
 *			KeyWords definition & action
 *****************************************************************************/

static int	dch_global(int arg, char *inout, int suf, int flag, FormatNode *node, void *data);
static int	dch_time(int arg, char *inout, int suf, int flag, FormatNode *node, void *data);
static int	dch_date(int arg, char *inout, int suf, int flag, FormatNode *node, void *data);

/* ----------
 * Suffixes:
 * ----------
 */
#define DCH_S_FM	0x01
#define DCH_S_TH	0x02
#define DCH_S_th	0x04
#define DCH_S_SP	0x08

/* ----------
 * Suffix tests
 * ----------
 */
#define S_THth(_s)	(((_s & DCH_S_TH) || (_s & DCH_S_th)) ? 1 : 0)
#define S_TH(_s)	((_s & DCH_S_TH) ? 1 : 0)
#define S_th(_s)	((_s & DCH_S_th) ? 1 : 0)
#define S_TH_TYPE(_s)	((_s & DCH_S_TH) ? TH_UPPER : TH_LOWER)

#define S_FM(_s)	((_s & DCH_S_FM) ? 1 : 0)
#define S_SP(_s)	((_s & DCH_S_SP) ? 1 : 0)

/* ----------
 * Suffixes definition for DATE-TIME TO/FROM CHAR
 * ----------
 */
static KeySuffix DCH_suff[] = {
	{"FM", 2, DCH_S_FM, SUFFTYPE_PREFIX},
	{"fm", 2, DCH_S_FM, SUFFTYPE_PREFIX},
	{"TH", 2, DCH_S_TH, SUFFTYPE_POSTFIX},
	{"th", 2, DCH_S_th, SUFFTYPE_POSTFIX},
	{"SP", 2, DCH_S_SP, SUFFTYPE_POSTFIX},
	/* last */
	{NULL, 0, 0, 0}
};

/* ----------
 * Format-pictures (KeyWord).
 *
 * The KeyWord field; alphabetic sorted, *BUT* strings alike is sorted
 *		  complicated -to-> easy:
 *
 *	(example: "DDD","DD","Day","D" )
 *
 * (this specific sort needs the algorithm for sequential search for strings,
 * which not has exact end; -> How keyword is in "HH12blabla" ? - "HH"
 * or "HH12"? You must first try "HH12", because "HH" is in string, but
 * it is not good.
 *
 * (!)
 *	 - Position for the keyword is simular as position in the enum DCH/NUM_poz.
 * (!)
 *
 * For fast search is used the 'int index[]', index is ascii table from position
 * 32 (' ') to 126 (~), in this index is DCH_ / NUM_ enums for each ASCII
 * position or -1 if char is not used in the KeyWord. Search example for
 * string "MM":
 *	1)	see in index to index['M' - 32],
 *	2)	take keywords position (enum DCH_MM) from index
 *	3)	run sequential search in keywords[] from this position
 *
 * ----------
 */

typedef enum
{
	DCH_A_D,
	DCH_A_M,
	DCH_AD,
	DCH_AM,
	DCH_B_C,
	DCH_BC,
	DCH_CC,
	DCH_DAY,
	DCH_DDD,
	DCH_DD,
	DCH_DY,
	DCH_Day,
	DCH_Dy,
	DCH_D,
	DCH_FX,						/* global suffix */
	DCH_HH24,
	DCH_HH12,
	DCH_HH,
	DCH_IW,
	DCH_J,
	DCH_MI,
	DCH_MM,
	DCH_MONTH,
	DCH_MON,
	DCH_MS,
	DCH_Month,
	DCH_Mon,
	DCH_P_M,
	DCH_PM,
	DCH_Q,
	DCH_RM,
	DCH_SSSS,
	DCH_SS,
	DCH_TZ,
	DCH_US,
	DCH_WW,
	DCH_W,
	DCH_Y_YYY,
	DCH_YYYY,
	DCH_YYY,
	DCH_YY,
	DCH_Y,
	DCH_a_d,
	DCH_a_m,
	DCH_ad,
	DCH_am,
	DCH_b_c,
	DCH_bc,
	DCH_cc,
	DCH_day,
	DCH_ddd,
	DCH_dd,
	DCH_dy,
	DCH_d,
	DCH_fx,
	DCH_hh24,
	DCH_hh12,
	DCH_hh,
	DCH_iw,
	DCH_j,
	DCH_mi,
	DCH_mm,
	DCH_month,
	DCH_mon,
	DCH_ms,
	DCH_p_m,
	DCH_pm,
	DCH_q,
	DCH_rm,
	DCH_ssss,
	DCH_ss,
	DCH_tz,
	DCH_us,
	DCH_ww,
	DCH_w,
	DCH_y_yyy,
	DCH_yyyy,
	DCH_yyy,
	DCH_yy,
	DCH_y,

	/* last */
	_DCH_last_
} DCH_poz;

typedef enum
{
	NUM_COMMA,
	NUM_DEC,
	NUM_0,
	NUM_9,
	NUM_B,
	NUM_C,
	NUM_D,
	NUM_E,
	NUM_FM,
	NUM_G,
	NUM_L,
	NUM_MI,
	NUM_PL,
	NUM_PR,
	NUM_RN,
	NUM_SG,
	NUM_SP,
	NUM_S,
	NUM_TH,
	NUM_V,
	NUM_b,
	NUM_c,
	NUM_d,
	NUM_e,
	NUM_fm,
	NUM_g,
	NUM_l,
	NUM_mi,
	NUM_pl,
	NUM_pr,
	NUM_rn,
	NUM_sg,
	NUM_sp,
	NUM_s,
	NUM_th,
	NUM_v,

	/* last */
	_NUM_last_
} NUM_poz;

/* ----------
 * KeyWords for DATE-TIME version
 * ----------
 */
static KeyWord DCH_keywords[] = {
/*	keyword, len, func, type, isitdigit			 is in Index */
	{"A.D.", 4, dch_date, DCH_A_D, FALSE},		/* A */
	{"A.M.", 4, dch_time, DCH_A_M, FALSE},
	{"AD", 2, dch_date, DCH_AD, FALSE},
	{"AM", 2, dch_time, DCH_AM, FALSE},
	{"B.C.", 4, dch_date, DCH_B_C, FALSE},		/* B */
	{"BC", 2, dch_date, DCH_BC, FALSE},
	{"CC", 2, dch_date, DCH_CC, TRUE},	/* C */
	{"DAY", 3, dch_date, DCH_DAY, FALSE},		/* D */
	{"DDD", 3, dch_date, DCH_DDD, TRUE},
	{"DD", 2, dch_date, DCH_DD, TRUE},
	{"DY", 2, dch_date, DCH_DY, FALSE},
	{"Day", 3, dch_date, DCH_Day, FALSE},
	{"Dy", 2, dch_date, DCH_Dy, FALSE},
	{"D", 1, dch_date, DCH_D, TRUE},
	{"FX", 2, dch_global, DCH_FX, FALSE},		/* F */
	{"HH24", 4, dch_time, DCH_HH24, TRUE},		/* H */
	{"HH12", 4, dch_time, DCH_HH12, TRUE},
	{"HH", 2, dch_time, DCH_HH, TRUE},
	{"IW", 2, dch_date, DCH_IW, TRUE},	/* I */
	{"J", 1, dch_date, DCH_J, TRUE},	/* J */
	{"MI", 2, dch_time, DCH_MI, TRUE},
	{"MM", 2, dch_date, DCH_MM, TRUE},
	{"MONTH", 5, dch_date, DCH_MONTH, FALSE},
	{"MON", 3, dch_date, DCH_MON, FALSE},
	{"MS", 2, dch_time, DCH_MS, TRUE},
	{"Month", 5, dch_date, DCH_Month, FALSE},
	{"Mon", 3, dch_date, DCH_Mon, FALSE},
	{"P.M.", 4, dch_time, DCH_P_M, FALSE},		/* P */
	{"PM", 2, dch_time, DCH_PM, FALSE},
	{"Q", 1, dch_date, DCH_Q, TRUE},	/* Q */
	{"RM", 2, dch_date, DCH_RM, FALSE}, /* R */
	{"SSSS", 4, dch_time, DCH_SSSS, TRUE},		/* S */
	{"SS", 2, dch_time, DCH_SS, TRUE},
	{"TZ", 2, dch_time, DCH_TZ, FALSE}, /* T */
	{"US", 2, dch_time, DCH_US, TRUE},	/* U */
	{"WW", 2, dch_date, DCH_WW, TRUE},	/* W */
	{"W", 1, dch_date, DCH_W, TRUE},
	{"Y,YYY", 5, dch_date, DCH_Y_YYY, TRUE},	/* Y */
	{"YYYY", 4, dch_date, DCH_YYYY, TRUE},
	{"YYY", 3, dch_date, DCH_YYY, TRUE},
	{"YY", 2, dch_date, DCH_YY, TRUE},
	{"Y", 1, dch_date, DCH_Y, TRUE},
	{"a.d.", 4, dch_date, DCH_a_d, FALSE},		/* a */
	{"a.m.", 4, dch_time, DCH_a_m, FALSE},
	{"ad", 2, dch_date, DCH_ad, FALSE},
	{"am", 2, dch_time, DCH_am, FALSE},
	{"b.c.", 4, dch_date, DCH_b_c, FALSE},		/* b */
	{"bc", 2, dch_date, DCH_bc, FALSE},
	{"cc", 2, dch_date, DCH_CC, TRUE},	/* c */
	{"day", 3, dch_date, DCH_day, FALSE},		/* d */
	{"ddd", 3, dch_date, DCH_DDD, TRUE},
	{"dd", 2, dch_date, DCH_DD, TRUE},
	{"dy", 2, dch_date, DCH_dy, FALSE},
	{"d", 1, dch_date, DCH_D, TRUE},
	{"fx", 2, dch_global, DCH_FX, FALSE},		/* f */
	{"hh24", 4, dch_time, DCH_HH24, TRUE},		/* h */
	{"hh12", 4, dch_time, DCH_HH12, TRUE},
	{"hh", 2, dch_time, DCH_HH, TRUE},
	{"iw", 2, dch_date, DCH_IW, TRUE},	/* i */
	{"j", 1, dch_time, DCH_J, TRUE},	/* j */
	{"mi", 2, dch_time, DCH_MI, TRUE},	/* m */
	{"mm", 2, dch_date, DCH_MM, TRUE},
	{"month", 5, dch_date, DCH_month, FALSE},
	{"mon", 3, dch_date, DCH_mon, FALSE},
	{"ms", 2, dch_time, DCH_MS, TRUE},
	{"p.m.", 4, dch_time, DCH_p_m, FALSE},		/* p */
	{"pm", 2, dch_time, DCH_pm, FALSE},
	{"q", 1, dch_date, DCH_Q, TRUE},	/* q */
	{"rm", 2, dch_date, DCH_rm, FALSE}, /* r */
	{"ssss", 4, dch_time, DCH_SSSS, TRUE},		/* s */
	{"ss", 2, dch_time, DCH_SS, TRUE},
	{"tz", 2, dch_time, DCH_tz, FALSE}, /* t */
	{"us", 2, dch_time, DCH_US, TRUE},	/* u */
	{"ww", 2, dch_date, DCH_WW, TRUE},	/* w */
	{"w", 1, dch_date, DCH_W, TRUE},
	{"y,yyy", 5, dch_date, DCH_Y_YYY, TRUE},	/* y */
	{"yyyy", 4, dch_date, DCH_YYYY, TRUE},
	{"yyy", 3, dch_date, DCH_YYY, TRUE},
	{"yy", 2, dch_date, DCH_YY, TRUE},
	{"y", 1, dch_date, DCH_Y, TRUE},
/* last */
{NULL, 0, NULL, 0}};

/* ----------
 * KeyWords for NUMBER version (now, isitdigit info is not needful here..)
 * ----------
 */
static KeyWord NUM_keywords[] = {
/*	keyword,	len, func.	type			   is in Index */
	{",", 1, NULL, NUM_COMMA},	/* , */
	{".", 1, NULL, NUM_DEC},	/* . */
	{"0", 1, NULL, NUM_0},		/* 0 */
	{"9", 1, NULL, NUM_9},		/* 9 */
	{"B", 1, NULL, NUM_B},		/* B */
	{"C", 1, NULL, NUM_C},		/* C */
	{"D", 1, NULL, NUM_D},		/* D */
	{"E", 1, NULL, NUM_E},		/* E */
	{"FM", 2, NULL, NUM_FM},	/* F */
	{"G", 1, NULL, NUM_G},		/* G */
	{"L", 1, NULL, NUM_L},		/* L */
	{"MI", 2, NULL, NUM_MI},	/* M */
	{"PL", 2, NULL, NUM_PL},	/* P */
	{"PR", 2, NULL, NUM_PR},
	{"RN", 2, NULL, NUM_RN},	/* R */
	{"SG", 2, NULL, NUM_SG},	/* S */
	{"SP", 2, NULL, NUM_SP},
	{"S", 1, NULL, NUM_S},
	{"TH", 2, NULL, NUM_TH},	/* T */
	{"V", 1, NULL, NUM_V},		/* V */
	{"b", 1, NULL, NUM_B},		/* b */
	{"c", 1, NULL, NUM_C},		/* c */
	{"d", 1, NULL, NUM_D},		/* d */
	{"e", 1, NULL, NUM_E},		/* e */
	{"fm", 2, NULL, NUM_FM},	/* f */
	{"g", 1, NULL, NUM_G},		/* g */
	{"l", 1, NULL, NUM_L},		/* l */
	{"mi", 2, NULL, NUM_MI},	/* m */
	{"pl", 2, NULL, NUM_PL},	/* p */
	{"pr", 2, NULL, NUM_PR},
	{"rn", 2, NULL, NUM_rn},	/* r */
	{"sg", 2, NULL, NUM_SG},	/* s */
	{"sp", 2, NULL, NUM_SP},
	{"s", 1, NULL, NUM_S},
	{"th", 2, NULL, NUM_th},	/* t */
	{"v", 1, NULL, NUM_V},		/* v */

/* last */
{NULL, 0, NULL, 0}};


/* ----------
 * KeyWords index for DATE-TIME version
 * ----------
 */
static int	DCH_index[KeyWord_INDEX_SIZE] = {
/*
0	1	2	3	4	5	6	7	8	9
*/
	/*---- first 0..31 chars are skiped ----*/

	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, DCH_A_D, DCH_B_C, DCH_CC, DCH_DAY, -1,
	DCH_FX, -1, DCH_HH24, DCH_IW, DCH_J, -1, -1, DCH_MI, -1, -1,
	DCH_P_M, DCH_Q, DCH_RM, DCH_SSSS, DCH_TZ, DCH_US, -1, DCH_WW, -1, DCH_Y_YYY,
	-1, -1, -1, -1, -1, -1, -1, DCH_a_d, DCH_b_c, DCH_cc,
	DCH_day, -1, DCH_fx, -1, DCH_hh24, DCH_iw, DCH_j, -1, -1, DCH_mi,
	-1, -1, DCH_p_m, DCH_q, DCH_rm, DCH_ssss, DCH_tz, DCH_us, -1, DCH_ww,
	-1, DCH_y_yyy, -1, -1, -1, -1

	/*---- chars over 126 are skiped ----*/
};

/* ----------
 * KeyWords index for NUMBER version
 * ----------
 */
static int	NUM_index[KeyWord_INDEX_SIZE] = {
/*
0	1	2	3	4	5	6	7	8	9
*/
	/*---- first 0..31 chars are skiped ----*/

	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, NUM_COMMA, -1, NUM_DEC, -1, NUM_0, -1,
	-1, -1, -1, -1, -1, -1, -1, NUM_9, -1, -1,
	-1, -1, -1, -1, -1, -1, NUM_B, NUM_C, NUM_D, NUM_E,
	NUM_FM, NUM_G, -1, -1, -1, -1, NUM_L, NUM_MI, -1, -1,
	NUM_PL, -1, NUM_RN, NUM_SG, NUM_TH, -1, NUM_V, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, NUM_b, NUM_c,
	NUM_d, NUM_e, NUM_fm, NUM_g, -1, -1, -1, -1, NUM_l, NUM_mi,
	-1, -1, NUM_pl, -1, NUM_rn, NUM_sg, NUM_th, -1, NUM_v, -1,
	-1, -1, -1, -1, -1, -1

	/*---- chars over 126 are skiped ----*/
};

/* ----------
 * Number processor struct
 * ----------
 */
typedef struct NUMProc
{
	int			type;			/* FROM_CHAR (TO_NUMBER) or TO_CHAR */

	NUMDesc    *Num;			/* number description		*/

	int			sign,			/* '-' or '+'			*/
				sign_wrote,		/* was sign write		*/
				sign_pos,		/* pre number sign position */
				num_count,		/* number of write digits	*/
				num_in,			/* is inside number		*/
				num_curr,		/* current position in number	*/
				num_pre,		/* space before first number	*/

				read_dec,		/* to_number - was read dec. point	*/
				read_post;		/* to_number - number of dec. digit */

	char	   *number,			/* string with number	*/
			   *number_p,		/* pointer to current number pozition */
			   *inout,			/* in / out buffer	*/
			   *inout_p,		/* pointer to current inout pozition */
			   *last_relevant,	/* last relevant number after decimal
								 * point */

			   *L_negative_sign,	/* Locale */
			   *L_positive_sign,
			   *decimal,
			   *L_thousands_sep,
			   *L_currency_symbol;
} NUMProc;


/* ----------
 * Functions
 * ----------
 */
static KeyWord *index_seq_search(char *str, KeyWord *kw, int *index);
static KeySuffix *suff_search(char *str, KeySuffix *suf, int type);
static void NUMDesc_prepare(NUMDesc *num, FormatNode *n);
static void parse_format(FormatNode *node, char *str, KeyWord *kw,
			 KeySuffix *suf, int *index, int ver, NUMDesc *Num);
static char *DCH_processor(FormatNode *node, char *inout, int flag, void *data);

#ifdef DEBUG_TO_FROM_CHAR
static void dump_index(KeyWord *k, int *index);
static void dump_node(FormatNode *node, int max);
#endif

static char *get_th(char *num, int type);
static char *str_numth(char *dest, char *num, int type);
static int	strdigits_len(char *str);
static char *str_toupper(char *buff);
static char *str_tolower(char *buff);

/* static int is_acdc(char *str, int *len); */
static int	seq_search(char *name, char **array, int type, int max, int *len);
static int	dch_global(int arg, char *inout, int suf, int flag, FormatNode *node, void *data);
static int	dch_time(int arg, char *inout, int suf, int flag, FormatNode *node, void *data);
static int	dch_date(int arg, char *inout, int suf, int flag, FormatNode *node, void *data);
static char *fill_str(char *str, int c, int max);
static FormatNode *NUM_cache(int len, NUMDesc *Num, char *pars_str, int *flag);
static char *int_to_roman(int number);
static void NUM_prepare_locale(NUMProc *Np);
static char *get_last_relevant_decnum(char *num);
static void NUM_numpart_from_char(NUMProc *Np, int id, int plen);
static void NUM_numpart_to_char(NUMProc *Np, int id);
static char *NUM_processor(FormatNode *node, NUMDesc *Num, char *inout, char *number,
			  int plen, int sign, int type);
static DCHCacheEntry *DCH_cache_search(char *str);
static DCHCacheEntry *DCH_cache_getnew(char *str);

static NUMCacheEntry *NUM_cache_search(char *str);
static NUMCacheEntry *NUM_cache_getnew(char *str);
static void NUM_cache_remove(NUMCacheEntry *ent);


/* ----------
 * Fast sequential search, use index for data selection which
 * go to seq. cycle (it is very fast for non-wanted strings)
 * (can't be used binary search in format parsing)
 * ----------
 */
static KeyWord *
index_seq_search(char *str, KeyWord *kw, int *index)
{
	int			poz;

	if (!KeyWord_INDEX_FILTER(*str))
		return (KeyWord *) NULL;

	if ((poz = *(index + (*str - ' '))) > -1)
	{

		KeyWord    *k = kw + poz;

		do
		{
			if (!strncmp(str, k->name, k->len))
				return k;
			k++;
			if (!k->name)
				return (KeyWord *) NULL;
		} while (*str == *k->name);
	}
	return (KeyWord *) NULL;
}

static KeySuffix *
suff_search(char *str, KeySuffix *suf, int type)
{
	KeySuffix  *s;

	for (s = suf; s->name != NULL; s++)
	{
		if (s->type != type)
			continue;

		if (!strncmp(str, s->name, s->len))
			return s;
	}
	return (KeySuffix *) NULL;
}

/* ----------
 * Prepare NUMDesc (number description struct) via FormatNode struct
 * ----------
 */
static void
NUMDesc_prepare(NUMDesc *num, FormatNode *n)
{

	if (n->type != NODE_TYPE_ACTION)
		return;

	switch (n->key->id)
	{

		case NUM_9:
			if (IS_BRACKET(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): '9' must be ahead of 'PR'.");
			}
			if (IS_MULTI(num))
			{
				++num->multi;
				break;
			}
			if (IS_DECIMAL(num))
				++num->post;
			else
				++num->pre;
			break;

		case NUM_0:
			if (IS_BRACKET(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): '0' must be ahead of 'PR'.");
			}
			if (!IS_ZERO(num) && !IS_DECIMAL(num))
			{
				num->flag |= NUM_F_ZERO;
				num->zero_start = num->pre + 1;
			}
			if (!IS_DECIMAL(num))
				++num->pre;
			else
				++num->post;

			num->zero_end = num->pre + num->post;
			break;

		case NUM_B:
			if (num->pre == 0 && num->post == 0 && (!IS_ZERO(num)))
				num->flag |= NUM_F_BLANK;
			break;

		case NUM_D:
			num->flag |= NUM_F_LDECIMAL;
			num->need_locale = TRUE;
		case NUM_DEC:
			if (IS_DECIMAL(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): not unique decimal point.");
			}
			if (IS_MULTI(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): can't use 'V' and decimal poin together.");
			}
			num->flag |= NUM_F_DECIMAL;
			break;

		case NUM_FM:
			num->flag |= NUM_F_FILLMODE;
			break;

		case NUM_S:
			if (IS_LSIGN(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): not unique 'S'.");
			}
			if (IS_PLUS(num) || IS_MINUS(num) || IS_BRACKET(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): can't use 'S' and 'PL'/'MI'/'SG'/'PR' together.");
			}
			if (!IS_DECIMAL(num))
			{
				num->lsign = NUM_LSIGN_PRE;
				num->pre_lsign_num = num->pre;
				num->need_locale = TRUE;
				num->flag |= NUM_F_LSIGN;

			}
			else if (num->lsign == NUM_LSIGN_NONE)
			{
				num->lsign = NUM_LSIGN_POST;
				num->need_locale = TRUE;
				num->flag |= NUM_F_LSIGN;
			}
			break;

		case NUM_MI:
			if (IS_LSIGN(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): can't use 'S' and 'MI' together.");
			}
			num->flag |= NUM_F_MINUS;
			break;

		case NUM_PL:
			if (IS_LSIGN(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): can't use 'S' and 'PL' together.");
			}
			num->flag |= NUM_F_PLUS;
			break;

		case NUM_SG:
			if (IS_LSIGN(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): can't use 'S' and 'SG' together.");
			}
			num->flag |= NUM_F_MINUS;
			num->flag |= NUM_F_PLUS;
			break;

		case NUM_PR:
			if (IS_LSIGN(num) || IS_PLUS(num) || IS_MINUS(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): can't use 'PR' and 'S'/'PL'/'MI'/'SG' together.");
			}
			num->flag |= NUM_F_BRACKET;
			break;

		case NUM_rn:
		case NUM_RN:
			num->flag |= NUM_F_ROMAN;
			break;

		case NUM_L:
		case NUM_G:
			num->need_locale = TRUE;
			break;

		case NUM_V:
			if (IS_DECIMAL(num))
			{
				NUM_cache_remove(last_NUMCacheEntry);
				elog(ERROR, "to_char/to_number(): can't use 'V' and decimal poin together.");
			}
			num->flag |= NUM_F_MULTI;
			break;

		case NUM_E:
			NUM_cache_remove(last_NUMCacheEntry);
			elog(ERROR, "to_char/to_number(): 'E' is not supported.");
	}

	return;
}

/* ----------
 * Format parser, search small keywords and keyword's suffixes, and make
 * format-node tree.
 *
 * for DATE-TIME & NUMBER version
 * ----------
 */
static void
parse_format(FormatNode *node, char *str, KeyWord *kw,
			 KeySuffix *suf, int *index, int ver, NUMDesc *Num)
{
	KeySuffix  *s;
	FormatNode *n;
	int			node_set = 0,
				suffix,
				last = 0;

#ifdef DEBUG_TO_FROM_CHAR
	elog(DEBUG_elog_output, "to_char/number(): run parser.");
#endif

	n = node;

	while (*str)
	{
		suffix = 0;

		/*
		 * Prefix
		 */
		if (ver == DCH_TYPE && (s = suff_search(str, suf, SUFFTYPE_PREFIX)) != NULL)
		{
			suffix |= s->id;
			if (s->len)
				str += s->len;
		}

		/*
		 * Keyword
		 */
		if (*str && (n->key = index_seq_search(str, kw, index)) != NULL)
		{

			n->type = NODE_TYPE_ACTION;
			n->suffix = 0;
			node_set = 1;
			if (n->key->len)
				str += n->key->len;

			/*
			 * NUM version: Prepare global NUMDesc struct
			 */
			if (ver == NUM_TYPE)
				NUMDesc_prepare(Num, n);

			/*
			 * Postfix
			 */
			if (ver == DCH_TYPE && *str && (s = suff_search(str, suf, SUFFTYPE_POSTFIX)) != NULL)
			{
				suffix |= s->id;
				if (s->len)
					str += s->len;
			}

		}
		else if (*str)
		{
			/*
			 * Special characters '\' and '"'
			 */
			if (*str == '"' && last != '\\')
			{

				int			x = 0;

				while (*(++str))
				{
					if (*str == '"' && x != '\\')
					{
						str++;
						break;
					}
					else if (*str == '\\' && x != '\\')
					{
						x = '\\';
						continue;
					}
					n->type = NODE_TYPE_CHAR;
					n->character = *str;
					n->key = (KeyWord *) NULL;
					n->suffix = 0;
					++n;
					x = *str;
				}
				node_set = 0;
				suffix = 0;
				last = 0;

			}
			else if (*str && *str == '\\' && last != '\\' && *(str + 1) == '"')
			{
				last = *str;
				str++;

			}
			else if (*str)
			{
				n->type = NODE_TYPE_CHAR;
				n->character = *str;
				n->key = (KeyWord *) NULL;
				node_set = 1;
				last = 0;
				str++;
			}

		}

		/* end */
		if (node_set)
		{
			if (n->type == NODE_TYPE_ACTION)
				n->suffix = suffix;
			++n;

			n->suffix = 0;
			node_set = 0;
		}

	}

	n->type = NODE_TYPE_END;
	n->suffix = 0;
	return;
}

/* ----------
 * Call keyword's function for each of (action) node in format-node tree
 * ----------
 */
static char *
DCH_processor(FormatNode *node, char *inout, int flag, void *data)
{
	FormatNode *n;
	char	   *s;


	/*
	 * Zeroing global flags
	 */
	DCH_global_flag = 0;

	for (n = node, s = inout; n->type != NODE_TYPE_END; n++)
	{
		if (n->type == NODE_TYPE_ACTION)
		{
			int			len;

			/*
			 * Call node action function
			 */
			len = n->key->action(n->key->id, s, n->suffix, flag, n, data);
			if (len > 0)
				s += len;
			else if (len == -1)
				continue;

		}
		else
		{
			/*
			 * Remove to output char from input in TO_CHAR
			 */
			if (flag == TO_CHAR)
				*s = n->character;

			else
			{
				/*
				 * Skip blank space in FROM_CHAR's input
				 */
				if (isspace((unsigned char) n->character) && IS_FX == 0)
				{
					while (*s != '\0' && isspace((unsigned char) *(s + 1)))
						++s;
				}
			}
		}

		++s;					/* ! */

	}

	if (flag == TO_CHAR)
		*s = '\0';
	return inout;
}


/* ----------
 * DEBUG: Dump the FormatNode Tree (debug)
 * ----------
 */
#ifdef DEBUG_TO_FROM_CHAR

#define DUMP_THth(_suf) (S_TH(_suf) ? "TH" : (S_th(_suf) ? "th" : " "))
#define DUMP_FM(_suf)	(S_FM(_suf) ? "FM" : " ")

static void
dump_node(FormatNode *node, int max)
{
	FormatNode *n;
	int			a;

	elog(DEBUG_elog_output, "to_from-char(): DUMP FORMAT");

	for (a = 0, n = node; a <= max; n++, a++)
	{
		if (n->type == NODE_TYPE_ACTION)
			elog(DEBUG_elog_output, "%d:\t NODE_TYPE_ACTION '%s'\t(%s,%s)",
			  a, n->key->name, DUMP_THth(n->suffix), DUMP_FM(n->suffix));
		else if (n->type == NODE_TYPE_CHAR)
			elog(DEBUG_elog_output, "%d:\t NODE_TYPE_CHAR '%c'", a, n->character);
		else if (n->type == NODE_TYPE_END)
		{
			elog(DEBUG_elog_output, "%d:\t NODE_TYPE_END", a);
			return;
		}
		else
			elog(DEBUG_elog_output, "%d:\t UnKnown NODE !!!", a);

	}
}
#endif   /* DEBUG */

/*****************************************************************************
 *			Private utils
 *****************************************************************************/

/* ----------
 * Return ST/ND/RD/TH for simple (1..9) numbers
 * type --> 0 upper, 1 lower
 * ----------
 */
static char *
get_th(char *num, int type)
{
	int			len = strlen(num),
				last,
				seclast;

	last = *(num + (len - 1));
	if (!isdigit((unsigned char) last))
		elog(ERROR, "get_th: '%s' is not number.", num);

	/*
	 * All "teens" (<x>1[0-9]) get 'TH/th', while <x>[02-9][123] still get
	 * 'ST/st', 'ND/nd', 'RD/rd', respectively
	 */
	if ((len > 1) && ((seclast = num[len - 2]) == '1'))
		last = 0;

	switch (last)
	{
		case '1':
			if (type == TH_UPPER)
				return numTH[0];
			return numth[0];
		case '2':
			if (type == TH_UPPER)
				return numTH[1];
			return numth[1];
		case '3':
			if (type == TH_UPPER)
				return numTH[2];
			return numth[2];
		default:
			if (type == TH_UPPER)
				return numTH[3];
			return numth[3];
	}
	return NULL;
}

/* ----------
 * Convert string-number to ordinal string-number
 * type --> 0 upper, 1 lower
 * ----------
 */
static char *
str_numth(char *dest, char *num, int type)
{
	sprintf(dest, "%s%s", num, get_th(num, type));
	return dest;
}

/* ----------
 * Convert string to upper-string
 * ----------
 */
static char *
str_toupper(char *buff)
{
	char	   *p_buff = buff;

	while (*p_buff)
	{
		*p_buff = toupper((unsigned char) *p_buff);
		++p_buff;
	}
	return buff;
}

/* ----------
 * Convert string to lower-string
 * ----------
 */
static char *
str_tolower(char *buff)
{
	char	   *p_buff = buff;

	while (*p_buff)
	{
		*p_buff = tolower((unsigned char) *p_buff);
		++p_buff;
	}
	return buff;
}

/* ----------
 * Sequential search with to upper/lower conversion
 * ----------
 */
static int
seq_search(char *name, char **array, int type, int max, int *len)
{
	char	   *p,
			   *n,
			  **a;
	int			last,
				i;

	*len = 0;

	if (!*name)
		return -1;

	/* set first char */
	if (type == ONE_UPPER || type == ALL_UPPER)
		*name = toupper((unsigned char) *name);
	else if (type == ALL_LOWER)
		*name = tolower((unsigned char) *name);

	for (last = 0, a = array; *a != NULL; a++)
	{

		/* comperate first chars */
		if (*name != **a)
			continue;

		for (i = 1, p = *a + 1, n = name + 1;; n++, p++, i++)
		{

			/* search fragment (max) only */
			if (max && i == max)
			{
				*len = i;
				return a - array;
			}
			/* full size */
			if (*p == '\0')
			{
				*len = i;
				return a - array;
			}
			/* Not found in array 'a' */
			if (*n == '\0')
				break;

			/*
			 * Convert (but convert new chars only)
			 */
			if (i > last)
			{
				if (type == ONE_UPPER || type == ALL_LOWER)
					*n = tolower((unsigned char) *n);
				else if (type == ALL_UPPER)
					*n = toupper((unsigned char) *n);
				last = i;
			}

#ifdef DEBUG_TO_FROM_CHAR

			/*
			 * elog(DEBUG_elog_output, "N: %c, P: %c, A: %s (%s)", *n, *p,
			 * *a, name);
			 */
#endif
			if (*n != *p)
				break;
		}
	}

	return -1;
}


#ifdef DEBUG_TO_FROM_CHAR
/* -----------
 * DEBUG: Call for debug and for index checking; (Show ASCII char
 * and defined keyword for each used position
 * ----------
 */
static void
dump_index(KeyWord *k, int *index)
{
	int			i,
				count = 0,
				free_i = 0;

	elog(DEBUG_elog_output, "TO-FROM_CHAR: Dump KeyWord Index:");

	for (i = 0; i < KeyWord_INDEX_SIZE; i++)
	{
		if (index[i] != -1)
		{
			elog(DEBUG_elog_output, "\t%c: %s, ", i + 32, k[index[i]].name);
			count++;
		}
		else
		{
			free_i++;
			elog(DEBUG_elog_output, "\t(%d) %c %d", i, i + 32, index[i]);
		}
	}
	elog(DEBUG_elog_output, "\n\t\tUsed positions: %d,\n\t\tFree positions: %d",
		 count, free_i);
}
#endif   /* DEBUG */

/* ----------
 * Skip TM / th in FROM_CHAR
 * ----------
 */
#define SKIP_THth(_suf)		(S_THth(_suf) ? 2 : 0)


/* ----------
 * Global format option for DCH version
 * ----------
 */
static int
dch_global(int arg, char *inout, int suf, int flag, FormatNode *node, void *data)
{
	if (arg == DCH_FX)
		DCH_global_flag |= DCH_F_FX;
	return -1;
}

/* ----------
 * Return TRUE if next format picture is not digit value
 * ----------
 */
static bool
is_next_separator(FormatNode *n)
{
	if (n->type == NODE_TYPE_END)
		return FALSE;

	if (n->type == NODE_TYPE_ACTION && S_THth(n->suffix))
		return TRUE;

	/*
	 * Next node
	 */
	n++;

	if (n->type == NODE_TYPE_END)
		return FALSE;

	if (n->type == NODE_TYPE_ACTION)
	{
		if (n->key->isitdigit)
			return FALSE;

		return TRUE;
	}
	else if (isdigit((unsigned char) n->character))
		return FALSE;

	return TRUE;				/* some non-digit input (separator) */
}

static int
strdigits_len(char *str)
{
	char	   *p = str;
	int			len = 0;

	while (*p && isdigit((unsigned char) *p) && len <= DCH_MAX_ITEM_SIZ)
	{
		len++;
		p++;
	}
	return len;
}

#define AMPM_ERROR	elog(ERROR, "to_timestamp(): bad AM/PM string")

/* ----------
 * Master function of TIME for:
 *			  TO_CHAR	- write (inout) formated string
 *			  FROM_CHAR - scan (inout) string by course of FormatNode
 * ----------
 */
static int
dch_time(int arg, char *inout, int suf, int flag, FormatNode *node, void *data)
{
	char	   *p_inout = inout;
	struct tm  *tm = NULL;
	TmFromChar *tmfc = NULL;
	TmToChar   *tmtc = NULL;

	if (flag == TO_CHAR)
	{
		tmtc = (TmToChar *) data;
		tm = tmtcTm(tmtc);
	}
	else
		tmfc = (TmFromChar *) data;

	switch (arg)
	{
		case DCH_A_M:
		case DCH_P_M:
			if (flag == TO_CHAR)
			{
				strcpy(inout, ((tm->tm_hour > 11
							  && tm->tm_hour < 24) ? P_M_STR : A_M_STR));
				return 3;
			}
			else if (flag == FROM_CHAR)
			{
				if (strncmp(inout, P_M_STR, 4) == 0)
					tmfc->pm = TRUE;
				else if (strncmp(inout, A_M_STR, 4) == 0)
					tmfc->am = TRUE;
				else
					AMPM_ERROR;
				return 3;
			}
			break;
		case DCH_AM:
		case DCH_PM:
			if (flag == TO_CHAR)
			{
				strcpy(inout, ((tm->tm_hour > 11
								&& tm->tm_hour < 24) ? PM_STR : AM_STR));
				return 1;
			}
			else if (flag == FROM_CHAR)
			{
				if (strncmp(inout, PM_STR, 2) == 0)
					tmfc->pm = TRUE;
				else if (strncmp(inout, AM_STR, 2) == 0)
					tmfc->am = TRUE;
				else
					AMPM_ERROR;
				return 1;
			}
			break;
		case DCH_a_m:
		case DCH_p_m:
			if (flag == TO_CHAR)
			{
				strcpy(inout, ((tm->tm_hour > 11
							  && tm->tm_hour < 24) ? p_m_STR : a_m_STR));
				return 3;
			}
			else if (flag == FROM_CHAR)
			{
				if (strncmp(inout, p_m_STR, 4) == 0)
					tmfc->pm = TRUE;
				else if (strncmp(inout, a_m_STR, 4) == 0)
					tmfc->am = TRUE;
				else
					AMPM_ERROR;
				return 3;
			}
			break;
		case DCH_am:
		case DCH_pm:
			if (flag == TO_CHAR)
			{
				strcpy(inout, ((tm->tm_hour > 11
								&& tm->tm_hour < 24) ? pm_STR : am_STR));
				return 1;
			}
			else if (flag == FROM_CHAR)
			{
				if (strncmp(inout, pm_STR, 2) == 0)
					tmfc->pm = TRUE;
				else if (strncmp(inout, am_STR, 2) == 0)
					tmfc->am = TRUE;
				else
					AMPM_ERROR;
				return 1;
			}
			break;
		case DCH_HH:
		case DCH_HH12:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2,
						tm->tm_hour == 0 ? 12 :
					  tm->tm_hour < 13 ? tm->tm_hour : tm->tm_hour - 12);
				if (S_THth(suf))
					str_numth(p_inout, inout, 0);
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->hh);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->hh);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_HH24:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2, tm->tm_hour);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->hh);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->hh);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_MI:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2, tm->tm_min);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->mi);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->mi);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_SS:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2, tm->tm_sec);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->ss);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->ss);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_MS:			/* millisecond */
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%03d", (int) rint(tmtc->fsec * 1000));
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 2;

			}
			else if (flag == FROM_CHAR)
			{
				int			len,
							x;

				if (is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->ms);
					len = x = strdigits_len(inout);
				}
				else
				{
					sscanf(inout, "%03d", &tmfc->ms);
					x = strdigits_len(inout);
					len = x = x > 3 ? 3 : x;
				}

				/*
				 * 25 is 0.25 and 250 is 0.25 too; 025 is 0.025 and not
				 * 0.25
				 */
				tmfc->ms *= x == 1 ? 100 :
					x == 2 ? 10 : 1;

				/*
				 * elog(NOTICE, "X: %d, MS: %d, LEN: %d", x, tmfc->ms,
				 * len);
				 */
				return len - 1 + SKIP_THth(suf);
			}
			break;
		case DCH_US:			/* microsecond */
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%06d", (int) rint(tmtc->fsec * 1000000));
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 5;
			}
			else if (flag == FROM_CHAR)
			{
				int			len,
							x;

				if (is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->us);
					len = x = strdigits_len(inout);
				}
				else
				{
					sscanf(inout, "%06d", &tmfc->us);
					x = strdigits_len(inout);
					len = x = x > 6 ? 6 : x;
				}

				tmfc->us *= x == 1 ? 100000 :
					x == 2 ? 10000 :
					x == 3 ? 1000 :
					x == 4 ? 100 :
					x == 5 ? 10 : 1;

				/*
				 * elog(NOTICE, "X: %d, US: %d, LEN: %d", x, tmfc->us,
				 * len);
				 */
				return len - 1 + SKIP_THth(suf);
			}
			break;
		case DCH_SSSS:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%d", tm->tm_hour * 3600 +
						tm->tm_min * 60 +
						tm->tm_sec);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				return strlen(p_inout) - 1;
			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->ssss);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%05d", &tmfc->ssss);
					return 4 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_tz:
		case DCH_TZ:
			if (flag == TO_CHAR && tmtcTzn(tmtc))
			{
				int			siz = strlen(tmtcTzn(tmtc));

				if (arg == DCH_TZ)
					strcpy(inout, tmtcTzn(tmtc));
				else
				{
					char	   *p = palloc(siz);

					strcpy(p, tmtcTzn(tmtc));
					strcpy(inout, str_tolower(p));
					pfree(p);
				}
				return siz - 1;
			}
			else if (flag == FROM_CHAR)
				elog(ERROR, "to_timestamp(): TZ/tz not supported.");
	}
	return -1;
}

#define CHECK_SEQ_SEARCH(_l, _s) \
do { \
	if (_l <= 0) {							\
		elog(ERROR, "to_timestamp(): bad value for %s", _s);	\
	}								\
} while (0)


/* ----------
 * Master of DATE for:
 *		  TO_CHAR - write (inout) formated string
 *		  FROM_CHAR - scan (inout) string by course of FormatNode
 * ----------
 */
static int
dch_date(int arg, char *inout, int suf, int flag, FormatNode *node, void *data)
{
	char		buff[DCH_CACHE_SIZE],
			   *p_inout;
	int			i,
				len;
	struct tm  *tm = NULL;
	TmFromChar *tmfc = NULL;
	TmToChar   *tmtc = NULL;

	if (flag == TO_CHAR)
	{
		tmtc = (TmToChar *) data;
		tm = tmtcTm(tmtc);
	}
	else
		tmfc = (TmFromChar *) data;

	p_inout = inout;

	/*
	 * In the FROM-char is not difference between "January" or "JANUARY"
	 * or "january", all is before search convert to "first-upper". This
	 * convention is used for MONTH, MON, DAY, DY
	 */
	if (flag == FROM_CHAR)
	{
		if (arg == DCH_MONTH || arg == DCH_Month || arg == DCH_month)
		{
			tmfc->mm = seq_search(inout, months_full, ONE_UPPER, FULL_SIZ, &len) + 1;
			CHECK_SEQ_SEARCH(len, "MONTH/Month/month");
			if (S_FM(suf))
				return len - 1;
			else
				return 8;

		}
		else if (arg == DCH_MON || arg == DCH_Mon || arg == DCH_mon)
		{
			tmfc->mm = seq_search(inout, months, ONE_UPPER, MAX_MON_LEN, &len) + 1;
			CHECK_SEQ_SEARCH(len, "MON/Mon/mon");
			return 2;
		}
		else if (arg == DCH_DAY || arg == DCH_Day || arg == DCH_day)
		{
			tmfc->d = seq_search(inout, days, ONE_UPPER, FULL_SIZ, &len);
			CHECK_SEQ_SEARCH(len, "DAY/Day/day");
			if (S_FM(suf))
				return len - 1;
			else
				return 8;

		}
		else if (arg == DCH_DY || arg == DCH_Dy || arg == DCH_dy)
		{
			tmfc->d = seq_search(inout, days, ONE_UPPER, MAX_DY_LEN, &len);
			CHECK_SEQ_SEARCH(len, "DY/Dy/dy");
			return 2;
		}
	}

	switch (arg)
	{

		case DCH_A_D:
		case DCH_B_C:
			if (flag == TO_CHAR)
			{
				strcpy(inout, (tm->tm_year < 0 ? B_C_STR : A_D_STR));
				return 3;

			}
			else if (flag == FROM_CHAR)
			{
				if (strncmp(inout, B_C_STR, 4) == 0)
					tmfc->bc = TRUE;
				return 3;
			}
			break;
		case DCH_AD:
		case DCH_BC:
			if (flag == TO_CHAR)
			{
				strcpy(inout, (tm->tm_year < 0 ? BC_STR : AD_STR));
				return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (strncmp(inout, BC_STR, 2) == 0)
					tmfc->bc = TRUE;
				return 1;
			}
			break;
		case DCH_a_d:
		case DCH_b_c:
			if (flag == TO_CHAR)
			{
				strcpy(inout, (tm->tm_year < 0 ? b_c_STR : a_d_STR));
				return 3;

			}
			else if (flag == FROM_CHAR)
			{
				if (strncmp(inout, b_c_STR, 4) == 0)
					tmfc->bc = TRUE;
				return 3;
			}
			break;
		case DCH_ad:
		case DCH_bc:
			if (flag == TO_CHAR)
			{
				strcpy(inout, (tm->tm_year < 0 ? bc_STR : ad_STR));
				return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (strncmp(inout, bc_STR, 2) == 0)
					tmfc->bc = TRUE;
				return 1;
			}
			break;
		case DCH_MONTH:
			strcpy(inout, months_full[tm->tm_mon - 1]);
			sprintf(inout, "%*s", S_FM(suf) ? 0 : -9, str_toupper(inout));
			if (S_FM(suf))
				return strlen(p_inout) - 1;
			else
				return 8;

		case DCH_Month:
			sprintf(inout, "%*s", S_FM(suf) ? 0 : -9, months_full[tm->tm_mon - 1]);
			if (S_FM(suf))
				return strlen(p_inout) - 1;
			else
				return 8;

		case DCH_month:
			sprintf(inout, "%*s", S_FM(suf) ? 0 : -9, months_full[tm->tm_mon - 1]);
			*inout = tolower((unsigned char) *inout);
			if (S_FM(suf))
				return strlen(p_inout) - 1;
			else
				return 8;

		case DCH_MON:
			strcpy(inout, months[tm->tm_mon - 1]);
			inout = str_toupper(inout);
			return 2;

		case DCH_Mon:
			strcpy(inout, months[tm->tm_mon - 1]);
			return 2;

		case DCH_mon:
			strcpy(inout, months[tm->tm_mon - 1]);
			*inout = tolower((unsigned char) *inout);
			return 2;

		case DCH_MM:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2, tm->tm_mon);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->mm);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->mm);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_DAY:
			strcpy(inout, days[tm->tm_wday]);
			sprintf(inout, "%*s", S_FM(suf) ? 0 : -9, str_toupper(inout));
			if (S_FM(suf))
				return strlen(p_inout) - 1;
			else
				return 8;

		case DCH_Day:
			sprintf(inout, "%*s", S_FM(suf) ? 0 : -9, days[tm->tm_wday]);
			if (S_FM(suf))
				return strlen(p_inout) - 1;
			else
				return 8;

		case DCH_day:
			sprintf(inout, "%*s", S_FM(suf) ? 0 : -9, days[tm->tm_wday]);
			*inout = tolower((unsigned char) *inout);
			if (S_FM(suf))
				return strlen(p_inout) - 1;
			else
				return 8;

		case DCH_DY:
			strcpy(inout, days[tm->tm_wday]);
			inout = str_toupper(inout);
			return 2;

		case DCH_Dy:
			strcpy(inout, days[tm->tm_wday]);
			return 2;

		case DCH_dy:
			strcpy(inout, days[tm->tm_wday]);
			*inout = tolower((unsigned char) *inout);
			return 2;

		case DCH_DDD:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 3, tm->tm_yday);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 2;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->ddd);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%03d", &tmfc->ddd);
					return 2 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_DD:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2, tm->tm_mday);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->dd);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->dd);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_D:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%d", tm->tm_wday + 1);
				if (S_THth(suf))
				{
					str_numth(p_inout, inout, S_TH_TYPE(suf));
					return 2;
				}
				return 0;
			}
			else if (flag == FROM_CHAR)
			{
				sscanf(inout, "%1d", &tmfc->d);
				return 0 + SKIP_THth(suf);
			}
			break;
		case DCH_WW:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2,
						(tm->tm_yday - 1) / 7 + 1);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->ww);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->ww);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_IW:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2,
					 date2isoweek(tm->tm_year, tm->tm_mon, tm->tm_mday));
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				if (S_FM(suf) || S_THth(suf))
					return strlen(p_inout) - 1;
				else
					return 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->iw);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->iw);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_Q:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%d", (tm->tm_mon - 1) / 3 + 1);
				if (S_THth(suf))
				{
					str_numth(p_inout, inout, S_TH_TYPE(suf));
					return 2;
				}
				return 0;

			}
			else if (flag == FROM_CHAR)
			{
				sscanf(inout, "%1d", &tmfc->q);
				return 0 + SKIP_THth(suf);
			}
			break;
		case DCH_CC:
			if (flag == TO_CHAR)
			{
				i = tm->tm_year / 100 + 1;
				if (i <= 99 && i >= -99)
					sprintf(inout, "%0*d", S_FM(suf) ? 0 : 2, i);
				else
					sprintf(inout, "%d", i);
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				return strlen(p_inout) - 1;

			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->cc);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%02d", &tmfc->cc);
					return 1 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_Y_YYY:
			if (flag == TO_CHAR)
			{
				i = YEAR_ABS(tm->tm_year) / 1000;
				sprintf(inout, "%d,%03d", i, YEAR_ABS(tm->tm_year) - (i * 1000));
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				return strlen(p_inout) - 1;
			}
			else if (flag == FROM_CHAR)
			{
				int			cc;

				sscanf(inout, "%d,%03d", &cc, &tmfc->year);
				tmfc->year += (cc * 1000);

				return strdigits_len(inout) + 3 + SKIP_THth(suf);
			}
			break;
		case DCH_YYYY:
			if (flag == TO_CHAR)
			{
				if (tm->tm_year <= 9999 && tm->tm_year >= -9998)
					sprintf(inout, "%0*d", S_FM(suf) ? 0 : 4, YEAR_ABS(tm->tm_year));
				else
					sprintf(inout, "%d", YEAR_ABS(tm->tm_year));
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				return strlen(p_inout) - 1;
			}
			else if (flag == FROM_CHAR)
			{
				if (S_FM(suf) || is_next_separator(node))
				{
					sscanf(inout, "%d", &tmfc->year);
					return strdigits_len(inout) - 1 + SKIP_THth(suf);
				}
				else
				{
					sscanf(inout, "%04d", &tmfc->year);
					return 3 + SKIP_THth(suf);
				}
			}
			break;
		case DCH_YYY:
			if (flag == TO_CHAR)
			{
				sprintf(buff, "%03d", YEAR_ABS(tm->tm_year));
				i = strlen(buff);
				strcpy(inout, buff + (i - 3));
				if (S_THth(suf))
				{
					str_numth(p_inout, inout, S_TH_TYPE(suf));
					return 4;
				}
				return 2;

			}
			else if (flag == FROM_CHAR)
			{
				sscanf(inout, "%03d", &tmfc->year);
				
				/*
				 * 3-digit year:
				 *	'100' ... '999' = 1100 ... 1999
				 *	'000' ... '099' = 2000 ... 2099
				 */
				if (tmfc->year >= 100)
					tmfc->year += 1000;
				else
					tmfc->year += 2000;

				return 2 + SKIP_THth(suf);
			}
			break;
		case DCH_YY:
			if (flag == TO_CHAR)
			{
				sprintf(buff, "%02d", YEAR_ABS(tm->tm_year));
				i = strlen(buff);
				strcpy(inout, buff + (i - 2));
				if (S_THth(suf))
				{
					str_numth(p_inout, inout, S_TH_TYPE(suf));
					return 3;
				}
				return 1;

			}
			else if (flag == FROM_CHAR)
			{
				sscanf(inout, "%02d", &tmfc->year);

				/*
				 * 2-digit year:
				 * '00' ... '69'  = 2000 ... 2069
				 * '70' ... '99'  = 1970 ... 1999
		 		 */
				if (tmfc->year < 70)
					tmfc->year += 2000;
				else
					tmfc->year += 1900;

				return 1 + SKIP_THth(suf);
			}
			break;
		case DCH_Y:
			if (flag == TO_CHAR)
			{
				sprintf(buff, "%1d", YEAR_ABS(tm->tm_year));
				i = strlen(buff);
				strcpy(inout, buff + (i - 1));
				if (S_THth(suf))
				{
					str_numth(p_inout, inout, S_TH_TYPE(suf));
					return 2;
				}
				return 0;

			}
			else if (flag == FROM_CHAR)
			{
				sscanf(inout, "%1d", &tmfc->year);
				
				/*
				 * 1-digit year: always +2000
				 */
				tmfc->year += 2000;

				return 0 + SKIP_THth(suf);
			}
			break;
		case DCH_RM:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%*s", S_FM(suf) ? 0 : -4,
						rm_months_upper[12 - tm->tm_mon]);
				if (S_FM(suf))
					return strlen(p_inout) - 1;
				else
					return 3;

			}
			else if (flag == FROM_CHAR)
			{
				tmfc->mm = 12 - seq_search(inout, rm_months_upper, ALL_UPPER, FULL_SIZ, &len);
				CHECK_SEQ_SEARCH(len, "RM");
				if (S_FM(suf))
					return len - 1;
				else
					return 3;
			}
			break;
		case DCH_rm:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%*s", S_FM(suf) ? 0 : -4,
						rm_months_lower[12 - tm->tm_mon]);
				if (S_FM(suf))
					return strlen(p_inout) - 1;
				else
					return 3;

			}
			else if (flag == FROM_CHAR)
			{
				tmfc->mm = 12 - seq_search(inout, rm_months_lower, ALL_LOWER, FULL_SIZ, &len);
				CHECK_SEQ_SEARCH(len, "rm");
				if (S_FM(suf))
					return len - 1;
				else
					return 3;
			}
			break;
		case DCH_W:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%d", (tm->tm_mday - 1) / 7 + 1);
				if (S_THth(suf))
				{
					str_numth(p_inout, inout, S_TH_TYPE(suf));
					return 2;
				}
				return 0;
			}
			else if (flag == FROM_CHAR)
			{
				sscanf(inout, "%1d", &tmfc->w);
				return 0 + SKIP_THth(suf);
			}
			break;
		case DCH_J:
			if (flag == TO_CHAR)
			{
				sprintf(inout, "%d", date2j(tm->tm_year, tm->tm_mon, tm->tm_mday));
				if (S_THth(suf))
					str_numth(p_inout, inout, S_TH_TYPE(suf));
				return strlen(p_inout) - 1;
			}
			else if (flag == FROM_CHAR)
			{
				sscanf(inout, "%d", &tmfc->j);
				return strdigits_len(inout) - 1 + SKIP_THth(suf);
			}
			break;
	}
	return -1;
}

static DCHCacheEntry *
DCH_cache_getnew(char *str)
{
	DCHCacheEntry *ent = NULL;

	/* counter overload check  - paranoa? */
	if (DCHCounter + DCH_CACHE_FIELDS >= MAX_INT32)
	{
		DCHCounter = 0;

		for (ent = DCHCache; ent <= (DCHCache + DCH_CACHE_FIELDS); ent++)
			ent->age = (++DCHCounter);
	}

	/*
	 * Cache is full - needs remove any older entry
	 */
	if (n_DCHCache > DCH_CACHE_FIELDS)
	{

		DCHCacheEntry *old = DCHCache + 0;

#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "Cache is full (%d)", n_DCHCache);
#endif
		for (ent = DCHCache; ent <= (DCHCache + DCH_CACHE_FIELDS); ent++)
		{
			if (ent->age < old->age)
				old = ent;
		}
#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "OLD: '%s' AGE: %d", old->str, old->age);
#endif
		strcpy(old->str, str);	/* check str size before this func. */
		/* old->format fill parser */
		old->age = (++DCHCounter);
		return old;

	}
	else
	{
#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "NEW (%d)", n_DCHCache);
#endif
		ent = DCHCache + n_DCHCache;
		strcpy(ent->str, str);	/* check str size before this func. */
		/* ent->format fill parser */
		ent->age = (++DCHCounter);
		++n_DCHCache;
		return ent;
	}

	return (DCHCacheEntry *) NULL;		/* never */
}

static DCHCacheEntry *
DCH_cache_search(char *str)
{
	int			i = 0;
	DCHCacheEntry *ent;

	/* counter overload check  - paranoa? */
	if (DCHCounter + DCH_CACHE_FIELDS >= MAX_INT32)
	{
		DCHCounter = 0;

		for (ent = DCHCache; ent <= (DCHCache + DCH_CACHE_FIELDS); ent++)
			ent->age = (++DCHCounter);
	}

	for (ent = DCHCache; ent <= (DCHCache + DCH_CACHE_FIELDS); ent++)
	{
		if (i == n_DCHCache)
			break;
		if (strcmp(ent->str, str) == 0)
		{
			ent->age = (++DCHCounter);
			return ent;
		}
		i++;
	}

	return (DCHCacheEntry *) NULL;
}

static text *
datetime_to_char_body(TmToChar *tmtc, text *fmt)
{
	FormatNode *format;
	struct tm  *tm = NULL;
	char	   *str_fmt,
			   *result;
	bool		incache;
	int			len = VARSIZE(fmt) - VARHDRSZ;

	tm = tmtcTm(tmtc);
	tm->tm_wday = (date2j(tm->tm_year, tm->tm_mon, tm->tm_mday) + 1) % 7;
	tm->tm_yday = date2j(tm->tm_year, tm->tm_mon, tm->tm_mday) - date2j(tm->tm_year, 1, 1) + 1;

	/*
	 * Convert fmt to C string
	 */
	str_fmt = (char *) palloc(len + 1);
	memcpy(str_fmt, VARDATA(fmt), len);
	*(str_fmt + len) = '\0';

	/*
	 * Allocate result
	 */
	result = palloc((len * DCH_MAX_ITEM_SIZ) + 1);

	/*
	 * Allocate new memory if format picture is bigger than static cache
	 * and not use cache (call parser always) - incache=FALSE show this
	 * variant
	 */
	if (len > DCH_CACHE_SIZE)
	{
		format = (FormatNode *) palloc((len + 1) * sizeof(FormatNode));
		incache = FALSE;

		parse_format(format, str_fmt, DCH_keywords,
					 DCH_suff, DCH_index, DCH_TYPE, NULL);

		(format + len)->type = NODE_TYPE_END;	/* Paranoa? */

	}
	else
	{
		/*
		 * Use cache buffers
		 */
		DCHCacheEntry *ent;

		incache = TRUE;

		if ((ent = DCH_cache_search(str_fmt)) == NULL)
		{

			ent = DCH_cache_getnew(str_fmt);

			/*
			 * Not in the cache, must run parser and save a new
			 * format-picture to the cache.
			 */
			parse_format(ent->format, str_fmt, DCH_keywords,
						 DCH_suff, DCH_index, DCH_TYPE, NULL);

			(ent->format + len)->type = NODE_TYPE_END;	/* Paranoa? */

#ifdef DEBUG_TO_FROM_CHAR
			/* dump_node(ent->format, len); */
			/* dump_index(DCH_keywords, DCH_index);  */
#endif
		}
		format = ent->format;
	}

	DCH_processor(format, result, TO_CHAR, (void *) tmtc);

	if (!incache)
		pfree(format);

	pfree(str_fmt);

	/*
	 * for result is allocated max memory, which current format-picture
	 * needs, now it allocate result with real size
	 */
	if (!(len = strlen(result)))
		pfree(result);
	else
	{
		text	   *res = (text *) palloc(len + 1 + VARHDRSZ);

		memcpy(VARDATA(res), result, len);
		VARATT_SIZEP(res) = len + VARHDRSZ;
		return res;
	}
	return NULL;
}

/****************************************************************************
 *				Public routines
 ***************************************************************************/

/* -------------------
 * TIMESTAMP to_char()
 * -------------------
 */
Datum
timestamp_to_char(PG_FUNCTION_ARGS)
{
	Timestamp	dt = PG_GETARG_TIMESTAMP(0);
	text	   *fmt = PG_GETARG_TEXT_P(1),
			   *res;
	TmToChar	tmtc;
	int			r = 0;

	if ((VARSIZE(fmt) - VARHDRSZ) <= 0 || TIMESTAMP_NOT_FINITE(dt))
		PG_RETURN_NULL();

	ZERO_tmtc(&tmtc);

	r = timestamp2tm(dt, NULL, tmtcTm(&tmtc), &tmtcFsec(&tmtc), NULL);

	if (r != 0)
		elog(ERROR, "to_char(): Unable to convert timestamp to tm");

	if (!(res = datetime_to_char_body(&tmtc, fmt)))
		PG_RETURN_NULL();

	PG_RETURN_TEXT_P(res);
}

Datum
timestamptz_to_char(PG_FUNCTION_ARGS)
{
	TimestampTz dt = PG_GETARG_TIMESTAMP(0);
	text	   *fmt = PG_GETARG_TEXT_P(1),
			   *res;
	TmToChar	tmtc;
	int			tz,
				r = 0;

	if ((VARSIZE(fmt) - VARHDRSZ) <= 0 || TIMESTAMP_NOT_FINITE(dt))
		PG_RETURN_NULL();

	ZERO_tmtc(&tmtc);

	r = timestamp2tm(dt, &tz, tmtcTm(&tmtc), &tmtcFsec(&tmtc), &tmtcTzn(&tmtc));

	if (r != 0)
		elog(ERROR, "to_char(): Unable to convert timestamp to tm");

	if (!(res = datetime_to_char_body(&tmtc, fmt)))
		PG_RETURN_NULL();

	PG_RETURN_TEXT_P(res);
}


/* -------------------
 * INTERVAL to_char()
 * -------------------
 */
Datum
interval_to_char(PG_FUNCTION_ARGS)
{
	Interval   *it = PG_GETARG_INTERVAL_P(0);
	text	   *fmt = PG_GETARG_TEXT_P(1),
			   *res;
	TmToChar	tmtc;

	if ((VARSIZE(fmt) - VARHDRSZ) <= 0)
		PG_RETURN_NULL();

	ZERO_tmtc(&tmtc);

	if (interval2tm(*it, tmtcTm(&tmtc), &tmtcFsec(&tmtc)) != 0)
		PG_RETURN_NULL();

	if (!(res = datetime_to_char_body(&tmtc, fmt)))
		PG_RETURN_NULL();

	PG_RETURN_TEXT_P(res);
}

/* ---------------------
 * TO_TIMESTAMP()
 *
 * Make Timestamp from date_str which is formatted at argument 'fmt'
 * ( to_timestamp is reverse to_char() )
 * ---------------------
 */
Datum
to_timestamp(PG_FUNCTION_ARGS)
{
	text	   *date_txt = PG_GETARG_TEXT_P(0);
	text	   *fmt = PG_GETARG_TEXT_P(1);

	Timestamp	result;
	FormatNode *format;
	TmFromChar	tmfc;

	bool		incache;
	char	   *str;
	char	   *date_str;
	int			len,
				date_len,
				tz = 0;
	struct tm	tm;
	double		fsec = 0;

	ZERO_tm(&tm);
	ZERO_tmfc(&tmfc);

	len = VARSIZE(fmt) - VARHDRSZ;

	if (len)
	{
		str = (char *) palloc(len + 1);
		memcpy(str, VARDATA(fmt), len);
		*(str + len) = '\0';

		/*
		 * Allocate new memory if format picture is bigger than static
		 * cache and not use cache (call parser always) - incache=FALSE
		 * show this variant
		 */
		if (len > DCH_CACHE_SIZE)
		{
			format = (FormatNode *) palloc((len + 1) * sizeof(FormatNode));
			incache = FALSE;

			parse_format(format, str, DCH_keywords,
						 DCH_suff, DCH_index, DCH_TYPE, NULL);

			(format + len)->type = NODE_TYPE_END;		/* Paranoa? */
		}
		else
		{
			/*
			 * Use cache buffers
			 */
			DCHCacheEntry *ent;

			incache = 0;

			if ((ent = DCH_cache_search(str)) == NULL)
			{

				ent = DCH_cache_getnew(str);

				/*
				 * Not in the cache, must run parser and save a new
				 * format-picture to the cache.
				 */
				parse_format(ent->format, str, DCH_keywords,
							 DCH_suff, DCH_index, DCH_TYPE, NULL);

				(ent->format + len)->type = NODE_TYPE_END;		/* Paranoa? */
#ifdef DEBUG_TO_FROM_CHAR
				/* dump_node(ent->format, len); */
				/* dump_index(DCH_keywords, DCH_index); */
#endif
			}
			format = ent->format;
		}

		/*
		 * Call action for each node in FormatNode tree
		 */
#ifdef DEBUG_TO_FROM_CHAR
		/* dump_node(format, len); */
#endif

		/*
		 * Convert date to C string
		 */
		date_len = VARSIZE(date_txt) - VARHDRSZ;
		date_str = (char *) palloc(date_len + 1);
		memcpy(date_str, VARDATA(date_txt), date_len);
		*(date_str + date_len) = '\0';

		DCH_processor(format, date_str, FROM_CHAR, (void *) &tmfc);

		pfree(date_str);
		pfree(str);
		if (incache)
			pfree(format);
	}

	DEBUG_TMFC(&tmfc);

	/*
	 * Convert values that user define for FROM_CHAR
	 * (to_date/to_timestamp) to standard 'tm'
	 */
	if (tmfc.ssss)
	{
		int			x = tmfc.ssss;

		tm.tm_hour = x / 3600;
		x %= 3600;
		tm.tm_min = x / 60;
		x %= 60;
		tm.tm_sec = x;
	}

	if (tmfc.cc)
		tm.tm_year = (tmfc.cc - 1) * 100;

	if (tmfc.ww)
		tmfc.ddd = (tmfc.ww - 1) * 7 + 1;

	if (tmfc.w)
		tmfc.dd = (tmfc.w - 1) * 7 + 1;

	if (tmfc.ss)
		tm.tm_sec = tmfc.ss;
	if (tmfc.mi)
		tm.tm_min = tmfc.mi;
	if (tmfc.hh)
		tm.tm_hour = tmfc.hh;

	if (tmfc.pm || tmfc.am)
	{
		if (tm.tm_hour < 1 || tm.tm_hour > 12)
			elog(ERROR, "to_timestamp(): AM/PM hour must be between 1 and 12");

		if (tmfc.pm && tm.tm_hour < 12)
			tm.tm_hour += 12;

		else if (tmfc.am && tm.tm_hour == 12)
			tm.tm_hour = 0;
	}

	switch (tmfc.q)
	{
		case 1:
			tm.tm_mday = 1;
			tm.tm_mon = 1;
			break;
		case 2:
			tm.tm_mday = 1;
			tm.tm_mon = 4;
			break;
		case 3:
			tm.tm_mday = 1;
			tm.tm_mon = 7;
			break;
		case 4:
			tm.tm_mday = 1;
			tm.tm_mon = 10;
			break;
	}

	if (tmfc.year)
		tm.tm_year = tmfc.year;

	if (tmfc.bc)
	{
		if (tm.tm_year > 0)
			tm.tm_year = -(tm.tm_year - 1);
		else
			elog(ERROR, "Inconsistent use of year %04d and 'BC'", tm.tm_year);
	}

	if (tmfc.j)
		j2date(tmfc.j, &tm.tm_year, &tm.tm_mon, &tm.tm_mday);

	if (tmfc.iw)
		isoweek2date(tmfc.iw, &tm.tm_year, &tm.tm_mon, &tm.tm_mday);

	if (tmfc.d)
		tm.tm_wday = tmfc.d;
	if (tmfc.dd)
		tm.tm_mday = tmfc.dd;
	if (tmfc.ddd)
		tm.tm_yday = tmfc.ddd;
	if (tmfc.mm)
		tm.tm_mon = tmfc.mm;

	/*
	 * we don't ignore DDD
	 */
	if (tmfc.ddd && (tm.tm_mon <= 1 || tm.tm_mday <= 1))
	{
		/* count mday and mon from yday */
		int		   *y,
					i;

		int			ysum[2][13] = {
			{31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 0},
		{31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366, 0}};

		if (!tm.tm_year)
			elog(ERROR, "to_timestamp() cat't convert yday without year information");

		y = ysum[isleap(tm.tm_year)];

		for (i = 0; i <= 11; i++)
		{
			if (tm.tm_yday < y[i])
				break;
		}
		if (tm.tm_mon <= 1)
			tm.tm_mon = i + 1;

		if (tm.tm_mday <= 1)
			tm.tm_mday = i == 0 ? tm.tm_yday :
				tm.tm_yday - y[i - 1];
	}

	if (tmfc.ms)
		fsec += (double) tmfc.ms / 1000;
	if (tmfc.us)
		fsec += (double) tmfc.us / 1000000;

	/* -------------------------------------------------------------- */

	DEBUG_TM(&tm);
	tz = DetermineLocalTimeZone(&tm);

	if (tm2timestamp(&tm, fsec, &tz, &result) != 0)
		elog(ERROR, "to_timestamp(): can't convert 'tm' to timestamp.");

	PG_RETURN_TIMESTAMP(result);
}

/* ----------
 * TO_DATE
 *	Make Date from date_str which is formated at argument 'fmt'
 * ----------
 */
Datum
to_date(PG_FUNCTION_ARGS)
{
	/*
	 * Quick hack: since our inputs are just like to_timestamp, hand over
	 * the whole input info struct...
	 */
	return DirectFunctionCall1(timestamptz_date, to_timestamp(fcinfo));
}

/**********************************************************************
 *	the NUMBER version part
 *********************************************************************/


static char *
fill_str(char *str, int c, int max)
{
	memset(str, c, max);
	*(str + max + 1) = '\0';
	return str;
}

#define zeroize_NUM(_n) \
do { \
	(_n)->flag		= 0;	\
	(_n)->lsign		= 0;	\
	(_n)->pre		= 0;	\
	(_n)->post		= 0;	\
	(_n)->pre_lsign_num = 0;	\
	(_n)->need_locale	= 0;	\
	(_n)->multi		= 0;	\
	(_n)->zero_start	= 0;	\
	(_n)->zero_end		= 0;	\
} while(0)

static NUMCacheEntry *
NUM_cache_getnew(char *str)
{
	NUMCacheEntry *ent = NULL;

	/* counter overload check  - paranoa? */
	if (NUMCounter + NUM_CACHE_FIELDS >= MAX_INT32)
	{
		NUMCounter = 0;

		for (ent = NUMCache; ent <= (NUMCache + NUM_CACHE_FIELDS); ent++)
			ent->age = (++NUMCounter);
	}

	/*
	 * Cache is full - needs remove any older entry
	 */
	if (n_NUMCache > NUM_CACHE_FIELDS)
	{

		NUMCacheEntry *old = NUMCache + 0;

#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "Cache is full (%d)", n_NUMCache);
#endif

		for (ent = NUMCache; ent <= (NUMCache + NUM_CACHE_FIELDS); ent++)
		{
			/*
			 * entry removed via NUM_cache_remove() can be used here
			 */
			if (*ent->str == '\0')
			{
				old = ent;
				break;
			}
			if (ent->age < old->age)
				old = ent;
		}
#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "OLD: '%s' AGE: %d", old->str, old->age);
#endif
		strcpy(old->str, str);	/* check str size before this func. */
		/* old->format fill parser */
		old->age = (++NUMCounter);

		ent = old;

	}
	else
	{
#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "NEW (%d)", n_NUMCache);
#endif
		ent = NUMCache + n_NUMCache;
		strcpy(ent->str, str);	/* check str size before this func. */
		/* ent->format fill parser */
		ent->age = (++NUMCounter);
		++n_NUMCache;
	}

	zeroize_NUM(&ent->Num);

	last_NUMCacheEntry = ent;
	return ent;					/* never */
}

static NUMCacheEntry *
NUM_cache_search(char *str)
{
	int			i = 0;
	NUMCacheEntry *ent;

	/* counter overload check  - paranoa? */
	if (NUMCounter + NUM_CACHE_FIELDS >= MAX_INT32)
	{
		NUMCounter = 0;

		for (ent = NUMCache; ent <= (NUMCache + NUM_CACHE_FIELDS); ent++)
			ent->age = (++NUMCounter);
	}

	for (ent = NUMCache; ent <= (NUMCache + NUM_CACHE_FIELDS); ent++)
	{
		if (i == n_NUMCache)
			break;
		if (strcmp(ent->str, str) == 0)
		{
			ent->age = (++NUMCounter);
			last_NUMCacheEntry = ent;
			return ent;
		}
		i++;
	}

	return (NUMCacheEntry *) NULL;
}

static void
NUM_cache_remove(NUMCacheEntry *ent)
{
#ifdef DEBUG_TO_FROM_CHAR
	elog(DEBUG_elog_output, "REMOVING ENTRY (%s)", ent->str);
#endif
	*ent->str = '\0';
	ent->age = 0;
}

/* ----------
 * Cache routine for NUM to_char version
 * ----------
 */
static FormatNode *
NUM_cache(int len, NUMDesc *Num, char *pars_str, int *flag)
{
	FormatNode *format = NULL;
	char	   *str;

	/*
	 * Convert VARDATA() to string
	 */
	str = (char *) palloc(len + 1);
	memcpy(str, pars_str, len);
	*(str + len) = '\0';

	/*
	 * Allocate new memory if format picture is bigger than static cache
	 * and not use cache (call parser always) - flag=1 show this variant
	 */
	if (len > NUM_CACHE_SIZE)
	{

		format = (FormatNode *) palloc((len + 1) * sizeof(FormatNode));
		*flag = 1;

		zeroize_NUM(Num);

		parse_format(format, str, NUM_keywords,
					 NULL, NUM_index, NUM_TYPE, Num);

		(format + len)->type = NODE_TYPE_END;	/* Paranoa? */

	}
	else
	{
		/*
		 * Use cache buffers
		 */
		NUMCacheEntry *ent;

		flag = 0;

		if ((ent = NUM_cache_search(str)) == NULL)
		{

			ent = NUM_cache_getnew(str);

			/*
			 * Not in the cache, must run parser and save a new
			 * format-picture to the cache.
			 */
			parse_format(ent->format, str, NUM_keywords,
						 NULL, NUM_index, NUM_TYPE, &ent->Num);

			(ent->format + len)->type = NODE_TYPE_END;	/* Paranoa? */

		}

		format = ent->format;

		/*
		 * Copy cache to used struct
		 */
		Num->flag = ent->Num.flag;
		Num->lsign = ent->Num.lsign;
		Num->pre = ent->Num.pre;
		Num->post = ent->Num.post;
		Num->pre_lsign_num = ent->Num.pre_lsign_num;
		Num->need_locale = ent->Num.need_locale;
		Num->multi = ent->Num.multi;
		Num->zero_start = ent->Num.zero_start;
		Num->zero_end = ent->Num.zero_end;
	}

#ifdef DEBUG_TO_FROM_CHAR
	/* dump_node(format, len); */
	dump_index(NUM_keywords, NUM_index);
#endif

	pfree(str);
	return format;
}


static char *
int_to_roman(int number)
{
	int			len = 0,
				num = 0,
				set = 0;
	char	   *p = NULL,
			   *result,
				numstr[5];

	result = (char *) palloc(16);
	*result = '\0';

	if (number > 3999 || number < 1)
	{
		fill_str(result, '#', 15);
		return result;
	}
	len = snprintf(numstr, sizeof(numstr), "%d", number);

	for (p = numstr; *p != '\0'; p++, --len)
	{
		num = *p - 49;			/* 48 ascii + 1 */
		if (num < 0)
			continue;
		if (num == -1 && set == 0)
			continue;
		set = 1;

		if (len > 3)
		{
			while (num-- != -1)
				strcat(result, "M");
		}
		else
		{
			if (len == 3)
				strcat(result, rm100[num]);
			else if (len == 2)
				strcat(result, rm10[num]);
			else if (len == 1)
				strcat(result, rm1[num]);
		}
	}
	return result;
}



/* ----------
 * Locale
 * ----------
 */
static void
NUM_prepare_locale(NUMProc *Np)
{

#ifdef USE_LOCALE

	if (Np->Num->need_locale)
	{

		struct lconv *lconv;

		/*
		 * Get locales
		 */
		lconv = PGLC_localeconv();

		/*
		 * Positive / Negative number sign
		 */
		if (lconv->negative_sign && *lconv->negative_sign)
			Np->L_negative_sign = lconv->negative_sign;
		else
			Np->L_negative_sign = "-";

		if (lconv->positive_sign && *lconv->positive_sign)
			Np->L_positive_sign = lconv->positive_sign;
		else
			Np->L_positive_sign = "+";

		/*
		 * Number thousands separator
		 */
		if (lconv->thousands_sep && *lconv->thousands_sep)
			Np->L_thousands_sep = lconv->thousands_sep;
		else
			Np->L_thousands_sep = ",";

		/*
		 * Number decimal point
		 */
		if (lconv->decimal_point && *lconv->decimal_point)
			Np->decimal = lconv->decimal_point;
		else
			Np->decimal = ".";

		/*
		 * Currency symbol
		 */
		if (lconv->currency_symbol && *lconv->currency_symbol)
			Np->L_currency_symbol = lconv->currency_symbol;
		else
			Np->L_currency_symbol = " ";


		if (!IS_LDECIMAL(Np->Num))
			Np->decimal = ".";
	}
	else
	{
#endif

		/*
		 * Default values
		 */
		Np->L_negative_sign = "-";
		Np->L_positive_sign = "+";
		Np->decimal = ".";
		Np->L_thousands_sep = ",";
		Np->L_currency_symbol = " ";

#ifdef USE_LOCALE
	}
#endif
}

/* ----------
 * Return pointer of last relevant number after decimal point
 *	12.0500 --> last relevant is '5'
 * ----------
 */
static char *
get_last_relevant_decnum(char *num)
{
	char	   *result,
			   *p = strchr(num, '.');

#ifdef DEBUG_TO_FROM_CHAR
	elog(DEBUG_elog_output, "CALL: get_last_relevant_decnum()");
#endif

	if (!p)
		p = num;
	result = p;

	while (*(++p))
	{
		if (*p != '0')
			result = p;
	}

	return result;
}

/* ----------
 * Number extraction for TO_NUMBER()
 * ----------
 */
static void
NUM_numpart_from_char(NUMProc *Np, int id, int plen)
{

#ifdef DEBUG_TO_FROM_CHAR
	elog(DEBUG_elog_output, " --- scan start --- ");
#endif

	if (*Np->inout_p == ' ')
		Np->inout_p++;

#define OVERLOAD_TEST	(Np->inout_p >= Np->inout + plen)

	if (*Np->inout_p == ' ')
		Np->inout_p++;

	if (OVERLOAD_TEST)
		return;

	/*
	 * read sign
	 */
	if (*Np->number == ' ' && (id == NUM_0 || id == NUM_9 || NUM_S))
	{

#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "Try read sign (%c).", *Np->inout_p);
#endif

		/*
		 * locale sign
		 */
		if (IS_LSIGN(Np->Num))
		{

			int			x = strlen(Np->L_negative_sign);

#ifdef DEBUG_TO_FROM_CHAR
			elog(DEBUG_elog_output, "Try read locale sign (%c).", *Np->inout_p);
#endif
			if (!strncmp(Np->inout_p, Np->L_negative_sign, x))
			{
				Np->inout_p += x - 1;
				*Np->number = '-';
				return;
			}

			x = strlen(Np->L_positive_sign);
			if (!strncmp(Np->inout_p, Np->L_positive_sign, x))
			{
				Np->inout_p += x - 1;
				*Np->number = '+';
				return;
			}
		}

#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "Try read sipmle sign (%c).", *Np->inout_p);
#endif

		/*
		 * simple + - < >
		 */
		if (*Np->inout_p == '-' || (IS_BRACKET(Np->Num) &&
									*Np->inout_p == '<'))
		{

			*Np->number = '-';	/* set - */
			Np->inout_p++;

		}
		else if (*Np->inout_p == '+')
		{

			*Np->number = '+';	/* set + */
			Np->inout_p++;
		}
	}

	if (OVERLOAD_TEST)
		return;

	/*
	 * read digit
	 */
	if (isdigit((unsigned char) *Np->inout_p))
	{

		if (Np->read_dec && Np->read_post == Np->Num->post)
			return;

		*Np->number_p = *Np->inout_p;
		Np->number_p++;

		if (Np->read_dec)
			Np->read_post++;

#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "Read digit (%c).", *Np->inout_p);
#endif

		/*
		 * read decimal point
		 */
	}
	else if (IS_DECIMAL(Np->Num))
	{

#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "Try read decimal point (%c).", *Np->inout_p);
#endif
		if (*Np->inout_p == '.')
		{

			*Np->number_p = '.';
			Np->number_p++;
			Np->read_dec = TRUE;

		}
		else
		{
			int			x = strlen(Np->decimal);

#ifdef DEBUG_TO_FROM_CHAR
			elog(DEBUG_elog_output, "Try read locale point (%c).", *Np->inout_p);
#endif
			if (!strncmp(Np->inout_p, Np->decimal, x))
			{
				Np->inout_p += x - 1;
				*Np->number_p = '.';
				Np->number_p++;
				Np->read_dec = TRUE;
			}
		}
	}
}

/* ----------
 * Add digit or sign to number-string
 * ----------
 */
static void
NUM_numpart_to_char(NUMProc *Np, int id)
{
	if (IS_ROMAN(Np->Num))
		return;

	/* Note: in this elog() output not set '\0' in 'inout' */

#ifdef DEBUG_TO_FROM_CHAR

	/*
	 * Np->num_curr is number of current item in format-picture, it is not
	 * current position in inout!
	 */
	elog(DEBUG_elog_output,

		 "SIGN_WROTE: %d, CURRENT: %d, NUMBER_P: '%s', INOUT: '%s'",
		 Np->sign_wrote,
		 Np->num_curr,
		 Np->number_p,
		 Np->inout);
#endif
	Np->num_in = FALSE;

	/*
	 * Write sign
	 */
	if (Np->num_curr == Np->sign_pos && Np->sign_wrote == FALSE)
	{

#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "Writing sign to position: %d", Np->num_curr);
#endif
		if (IS_LSIGN(Np->Num))
		{
			/*
			 * Write locale SIGN
			 */
			if (Np->sign == '-')
				strcpy(Np->inout_p, Np->L_negative_sign);
			else
				strcpy(Np->inout_p, Np->L_positive_sign);
			Np->inout_p += strlen(Np->inout_p);

		}
		else if (IS_BRACKET(Np->Num))
		{
			*Np->inout_p = '<'; /* Write < */
			++Np->inout_p;

		}
		else if (Np->sign == '+')
		{
			*Np->inout_p = ' '; /* Write + */
			++Np->inout_p;

		}
		else if (Np->sign == '-')
		{						/* Write - */
			*Np->inout_p = '-';
			++Np->inout_p;
		}
		Np->sign_wrote = TRUE;

	}
	else if (Np->sign_wrote && IS_BRACKET(Np->Num) &&
			 (Np->num_curr == Np->num_count + (Np->num_pre ? 1 : 0)
			  + (IS_DECIMAL(Np->Num) ? 1 : 0)))
	{
		/*
		 * Write close BRACKET
		 */
#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "Writing bracket to position %d", Np->num_curr);
#endif
		*Np->inout_p = '>';		/* Write '>' */
		++Np->inout_p;
	}

	/*
	 * digits / FM / Zero / Dec. point
	 */
	if (id == NUM_9 || id == NUM_0 || id == NUM_D || id == NUM_DEC ||
		(id == NUM_S && Np->num_curr < Np->num_pre))
	{

		if (Np->num_curr < Np->num_pre &&
			(Np->Num->zero_start > Np->num_curr || !IS_ZERO(Np->Num)))
		{
			/*
			 * Write blank space
			 */
			if (!IS_FILLMODE(Np->Num))
			{
#ifdef DEBUG_TO_FROM_CHAR
				elog(DEBUG_elog_output, "Writing blank space to position %d", Np->num_curr);
#endif
				*Np->inout_p = ' ';		/* Write ' ' */
				++Np->inout_p;
			}

		}
		else if (IS_ZERO(Np->Num) &&
				 Np->num_curr < Np->num_pre &&
				 Np->Num->zero_start <= Np->num_curr)
		{
			/*
			 * Write ZERO
			 */
#ifdef DEBUG_TO_FROM_CHAR
			elog(DEBUG_elog_output, "Writing zero to position %d", Np->num_curr);
#endif
			*Np->inout_p = '0'; /* Write '0' */
			++Np->inout_p;
			Np->num_in = TRUE;

		}
		else
		{
			/*
			 * Write Decinal point
			 */
			if (*Np->number_p == '.')
			{

				if (!Np->last_relevant || *Np->last_relevant != '.')
				{
#ifdef DEBUG_TO_FROM_CHAR
					elog(DEBUG_elog_output, "Writing decimal point to position %d", Np->num_curr);
#endif
					strcpy(Np->inout_p, Np->decimal);	/* Write DEC/D */
					Np->inout_p += strlen(Np->inout_p);

					/*
					 * terrible Ora '0' -- 9.9 --> '0.'
					 */
				}
				else if (IS_FILLMODE(Np->Num) && *Np->number == '0' &&
						 Np->last_relevant && *Np->last_relevant == '.')
				{

					strcpy(Np->inout_p, Np->decimal);	/* Write DEC/D */
					Np->inout_p += strlen(Np->inout_p);
				}

			}
			else
			{
				/*
				 * Write Digits
				 */
				if (Np->last_relevant && Np->number_p > Np->last_relevant &&
					id != NUM_0)
					;

				/*
				 * terrible Ora format: '0.1' -- 9.9 --> '  .1'
				 */
				else if (!IS_ZERO(Np->Num) && *Np->number == '0' &&
						 Np->number == Np->number_p && Np->Num->post != 0)
				{

					if (!IS_FILLMODE(Np->Num))
					{
						*Np->inout_p = ' ';
						++Np->inout_p;

						/*
						 * total terible Ora: '0' -- FM9.9 --> '0.'
						 */
					}
					else if (Np->last_relevant && *Np->last_relevant == '.')
					{
						*Np->inout_p = '0';
						++Np->inout_p;
					}

				}
				else
				{
#ifdef DEBUG_TO_FROM_CHAR
					elog(DEBUG_elog_output, "Writing digit '%c' to position %d", *Np->number_p, Np->num_curr);
#endif
					*Np->inout_p = *Np->number_p;		/* Write DIGIT */
					++Np->inout_p;
					Np->num_in = TRUE;
				}
			}
			++Np->number_p;
		}
	}

	++Np->num_curr;
}

/*
 * Note: 'plen' is used in FROM_CHAR conversion and it's length of
 * input (inout). In TO_CHAR conversion it's space before first number.
 */
static char *
NUM_processor(FormatNode *node, NUMDesc *Num, char *inout, char *number,
			  int plen, int sign, int type)
{
	FormatNode *n;
	NUMProc		_Np,
			   *Np = &_Np;

	Np->Num = Num;
	Np->type = type;
	Np->number = number;
	Np->inout = inout;
	Np->last_relevant = NULL;
	Np->read_post = 0;
	Np->read_dec = FALSE;

	if (Np->Num->zero_start)
		--Np->Num->zero_start;

	/*
	 * Roman correction
	 */
	if (IS_ROMAN(Np->Num))
	{
		if (Np->type == FROM_CHAR)
			elog(ERROR, "to_number(): RN is not supported");

		Np->Num->lsign = Np->Num->pre_lsign_num = Np->Num->post =
			Np->Num->pre = Np->num_pre = Np->sign = 0;

		if (IS_FILLMODE(Np->Num))
		{
			Np->Num->flag = 0;
			Np->Num->flag |= NUM_F_FILLMODE;
		}
		else
			Np->Num->flag = 0;
		Np->Num->flag |= NUM_F_ROMAN;
	}

	/*
	 * Sign
	 */
	if (type == FROM_CHAR)
	{
		Np->sign = FALSE;
		Np->sign_pos = -1;
	}
	else
	{
		Np->sign = sign;

		if (Np->sign != '-')
		{
			Np->Num->flag &= ~NUM_F_BRACKET;
			Np->Num->flag &= ~NUM_F_MINUS;
		}
		else if (Np->sign != '+')
			Np->Num->flag &= ~NUM_F_PLUS;

		if (Np->sign == '+' && IS_FILLMODE(Np->Num) && !IS_LSIGN(Np->Num))
			Np->sign_wrote = TRUE;		/* needn't sign */
		else
			Np->sign_wrote = FALSE;		/* need sign */

		Np->sign_pos = -1;

		if (Np->Num->lsign == NUM_LSIGN_PRE && Np->Num->pre == Np->Num->pre_lsign_num)
			Np->Num->lsign = NUM_LSIGN_POST;

		/* MI/PL/SG - write sign itself and not in number */
		if (IS_PLUS(Np->Num) || IS_MINUS(Np->Num))
			Np->sign_wrote = TRUE;		/* needn't sign */
	}

	/*
	 * Count
	 */
	Np->num_count = Np->Num->post + Np->Num->pre - 1;

	if (type == TO_CHAR)
	{
		Np->num_pre = plen;

		if (IS_FILLMODE(Np->Num))
		{
			if (IS_DECIMAL(Np->Num))
				Np->last_relevant = get_last_relevant_decnum(
															 Np->number +
								 ((Np->Num->zero_end - Np->num_pre > 0) ?
								  Np->Num->zero_end - Np->num_pre : 0));
		}

		if (!Np->sign_wrote && Np->num_pre == 0)
			++Np->num_count;

		if (!Np->sign_wrote)
		{
			/*
			 * Set SING position
			 */
			if (Np->Num->lsign == NUM_LSIGN_POST)
			{
				Np->sign_pos = Np->num_count + (Np->num_pre ? 1 : 0);

				if (IS_DECIMAL(Np->Num))		/* decimal point correctio */
					++Np->sign_pos;
			}
			else if (IS_ZERO(Np->Num) && Np->num_pre > Np->Num->zero_start)
				Np->sign_pos = Np->Num->zero_start ? Np->Num->zero_start : 0;

			else
				Np->sign_pos = Np->num_pre && !IS_FILLMODE(Np->Num) ? Np->num_pre : 0;

			/*
			 * terrible Ora format
			 */
			if (!IS_ZERO(Np->Num) && *Np->number == '0' &&
				!IS_FILLMODE(Np->Num) && Np->Num->post != 0)
			{

				++Np->sign_pos;

				if (IS_LSIGN(Np->Num))
				{
					if (Np->Num->lsign == NUM_LSIGN_PRE)
						++Np->sign_pos;
					else
						--Np->sign_pos;
				}
			}
		}

	}
	else
	{
		Np->num_pre = 0;
		*Np->number = ' ';		/* sign space */
		*(Np->number + 1) = '\0';
	}

	Np->num_in = 0;
	Np->num_curr = 0;

#ifdef DEBUG_TO_FROM_CHAR
	elog(DEBUG_elog_output,

		 "\n\tNUM: '%s'\n\tPRE: %d\n\tPOST: %d\n\tNUM_COUNT: %d\n\tNUM_PRE: %d\n\tSIGN_POS: %d\n\tSIGN_WROTE: %s\n\tZERO: %s\n\tZERO_START: %d\n\tZERO_END: %d\n\tLAST_RELEVANT: %s",
		 Np->number,
		 Np->Num->pre,
		 Np->Num->post,
		 Np->num_count,
		 Np->num_pre,
		 Np->sign_pos,
		 Np->sign_wrote ? "Yes" : "No",
		 IS_ZERO(Np->Num) ? "Yes" : "No",
		 Np->Num->zero_start,
		 Np->Num->zero_end,
		 Np->last_relevant ? Np->last_relevant : "<not set>"
		);
#endif

	/*
	 * Locale
	 */
	NUM_prepare_locale(Np);

	/*
	 * Processor direct cycle
	 */
	if (Np->type == FROM_CHAR)
		Np->number_p = Np->number + 1;	/* first char is space for sign */
	else if (Np->type == TO_CHAR)
		Np->number_p = Np->number;

	for (n = node, Np->inout_p = Np->inout; n->type != NODE_TYPE_END; n++)
	{

		if (Np->type == FROM_CHAR)
		{
			/*
			 * Check non-string inout end
			 */
			if (Np->inout_p >= Np->inout + plen)
				break;
		}

		/*
		 * Format pictures actions
		 */
		if (n->type == NODE_TYPE_ACTION)
		{
			/*
			 * Create/reading digit/zero/blank/sing
			 */
			switch (n->key->id)
			{

				case NUM_9:
				case NUM_0:
				case NUM_DEC:
				case NUM_D:
				case NUM_S:
				case NUM_PR:
					if (Np->type == TO_CHAR)
					{
						NUM_numpart_to_char(Np, n->key->id);
						continue;		/* for() */
					}
					else
					{
						NUM_numpart_from_char(Np, n->key->id, plen);
						break;	/* switch() case: */
					}

				case NUM_COMMA:
					if (Np->type == TO_CHAR)
					{
						if (!Np->num_in)
						{
							if (IS_FILLMODE(Np->Num))
								continue;
							else
								*Np->inout_p = ' ';
						}
						else
							*Np->inout_p = ',';

					}
					else if (Np->type == FROM_CHAR)
					{
						if (!Np->num_in)
						{
							if (IS_FILLMODE(Np->Num))
								continue;
						}
					}
					break;

				case NUM_G:
					if (Np->type == TO_CHAR)
					{
						if (!Np->num_in)
						{
							if (IS_FILLMODE(Np->Num))
								continue;
							else
							{
								int			x = strlen(Np->L_thousands_sep);

								memset(Np->inout_p, ' ', x);
								Np->inout_p += x - 1;
							}
						}
						else
						{
							strcpy(Np->inout_p, Np->L_thousands_sep);
							Np->inout_p += strlen(Np->inout_p) - 1;
						}

					}
					else if (Np->type == FROM_CHAR)
					{
						if (!Np->num_in)
						{
							if (IS_FILLMODE(Np->Num))
								continue;
						}
						Np->inout_p += strlen(Np->L_thousands_sep) - 1;
					}
					break;

				case NUM_L:
					if (Np->type == TO_CHAR)
					{
						strcpy(Np->inout_p, Np->L_currency_symbol);
						Np->inout_p += strlen(Np->inout_p) - 1;

					}
					else if (Np->type == FROM_CHAR)
						Np->inout_p += strlen(Np->L_currency_symbol) - 1;
					break;

				case NUM_RN:
					if (IS_FILLMODE(Np->Num))
					{
						strcpy(Np->inout_p, Np->number_p);
						Np->inout_p += strlen(Np->inout_p) - 1;
					}
					else
					{
						sprintf(Np->inout_p, "%15s", Np->number_p);
						Np->inout_p += strlen(Np->inout_p) - 1;
					}
					break;

				case NUM_rn:
					if (IS_FILLMODE(Np->Num))
					{
						strcpy(Np->inout_p, str_tolower(Np->number_p));
						Np->inout_p += strlen(Np->inout_p) - 1;
					}
					else
					{
						sprintf(Np->inout_p, "%15s", str_tolower(Np->number_p));
						Np->inout_p += strlen(Np->inout_p) - 1;
					}
					break;

				case NUM_th:
					if (IS_ROMAN(Np->Num) || *Np->number == '#' ||
						Np->sign == '-' || IS_DECIMAL(Np->Num))
						continue;

					if (Np->type == TO_CHAR)
						strcpy(Np->inout_p, get_th(Np->number, TH_LOWER));
					Np->inout_p += 1;
					break;

				case NUM_TH:
					if (IS_ROMAN(Np->Num) || *Np->number == '#' ||
						Np->sign == '-' || IS_DECIMAL(Np->Num))
						continue;

					if (Np->type == TO_CHAR)
						strcpy(Np->inout_p, get_th(Np->number, TH_UPPER));
					Np->inout_p += 1;
					break;

				case NUM_MI:
					if (Np->type == TO_CHAR)
					{
						if (Np->sign == '-')
							*Np->inout_p = '-';
						else
							*Np->inout_p = ' ';

					}
					else if (Np->type == FROM_CHAR)
					{
						if (*Np->inout_p == '-')
							*Np->number = '-';
					}
					break;

				case NUM_PL:
					if (Np->type == TO_CHAR)
					{
						if (Np->sign == '+')
							*Np->inout_p = '+';
						else
							*Np->inout_p = ' ';

					}
					else if (Np->type == FROM_CHAR)
					{
						if (*Np->inout_p == '+')
							*Np->number = '+';
					}
					break;

				case NUM_SG:
					if (Np->type == TO_CHAR)
						*Np->inout_p = Np->sign;

					else if (Np->type == FROM_CHAR)
					{
						if (*Np->inout_p == '-')
							*Np->number = '-';
						else if (*Np->inout_p == '+')
							*Np->number = '+';
					}
					break;


				default:
					continue;
					break;
			}

		}
		else
		{
			/*
			 * Remove to output char from input in TO_CHAR
			 */
			if (Np->type == TO_CHAR)
				*Np->inout_p = n->character;
		}
		Np->inout_p++;
	}

	if (Np->type == TO_CHAR)
	{
		*Np->inout_p = '\0';
		return Np->inout;

	}
	else if (Np->type == FROM_CHAR)
	{

		if (*(Np->number_p - 1) == '.')
			*(Np->number_p - 1) = '\0';
		else
			*Np->number_p = '\0';

		/*
		 * Correction - precision of dec. number
		 */
		Np->Num->post = Np->read_post;

#ifdef DEBUG_TO_FROM_CHAR
		elog(DEBUG_elog_output, "TO_NUMBER (number): '%s'", Np->number);
#endif
		return Np->number;
	}
	else
		return NULL;

	return NULL;
}

/* ----------
 * MACRO: Start part of NUM - for all NUM's to_char variants
 *	(sorry, but I hate copy same code - macro is better..)
 * ----------
 */
#define NUM_TOCHAR_prepare \
do { \
	len = VARSIZE(fmt) - VARHDRSZ;					\
	if (len <= 0)							\
		return DirectFunctionCall1(textin, CStringGetDatum(""));	\
	result	= (text *) palloc( (len * NUM_MAX_ITEM_SIZ) + 1 + VARHDRSZ); \
	format	= NUM_cache(len, &Num, VARDATA(fmt), &flag);		\
} while (0)

/* ----------
 * MACRO: Finish part of NUM
 * ----------
 */
#define NUM_TOCHAR_finish \
do { \
	NUM_processor(format, &Num, VARDATA(result),			\
		numstr, plen, sign, TO_CHAR);				\
	pfree(orgnum);							\
									\
	if (flag)							\
		pfree(format);						\
									\
	/*
	 * for result is allocated max memory, which current format-picture\
	 * needs, now it must be re-allocate to result real size	\
	 */								\
	if (!(len = strlen(VARDATA(result))))				\
	{								\
		pfree(result);						\
		PG_RETURN_NULL();					\
	}								\
									\
	result_tmp	= result;					\
	result		= (text *) palloc( len + 1 + VARHDRSZ);		\
									\
	strcpy( VARDATA(result), VARDATA(result_tmp));			\
	VARATT_SIZEP(result) = len + VARHDRSZ;				\
	pfree(result_tmp);						\
} while(0)

/* -------------------
 * NUMERIC to_number() (convert string to numeric)
 * -------------------
 */
Datum
numeric_to_number(PG_FUNCTION_ARGS)
{
	text	   *value = PG_GETARG_TEXT_P(0);
	text	   *fmt = PG_GETARG_TEXT_P(1);
	NUMDesc		Num;
	Datum		result;
	FormatNode *format;
	char	   *numstr;
	int			flag = 0;
	int			len = 0;
	int			scale,
				precision;

	len = VARSIZE(fmt) - VARHDRSZ;

	if (len <= 0)
		PG_RETURN_NULL();

	format = NUM_cache(len, &Num, VARDATA(fmt), &flag);

	numstr = (char *) palloc((len * NUM_MAX_ITEM_SIZ) + 1);

	NUM_processor(format, &Num, VARDATA(value), numstr,
				  VARSIZE(value) - VARHDRSZ, 0, FROM_CHAR);

	scale = Num.post;
	precision = MAX(0, Num.pre) + scale;

	if (flag)
		pfree(format);

	result = DirectFunctionCall3(numeric_in,
								 CStringGetDatum(numstr),
								 ObjectIdGetDatum(InvalidOid),
				  Int32GetDatum(((precision << 16) | scale) + VARHDRSZ));
	pfree(numstr);
	return result;
}

/* ------------------
 * NUMERIC to_char()
 * ------------------
 */
Datum
numeric_to_char(PG_FUNCTION_ARGS)
{
	Numeric		value = PG_GETARG_NUMERIC(0);
	text	   *fmt = PG_GETARG_TEXT_P(1);
	NUMDesc		Num;
	FormatNode *format;
	text	   *result,
			   *result_tmp;
	int			flag = 0;
	int			len = 0,
				plen = 0,
				sign = 0;
	char	   *numstr,
			   *orgnum,
			   *p;
	Numeric		x;

	NUM_TOCHAR_prepare;

	/*
	 * On DateType depend part (numeric)
	 */
	if (IS_ROMAN(&Num))
	{
		x = DatumGetNumeric(DirectFunctionCall2(numeric_round,
												NumericGetDatum(value),
												Int32GetDatum(0)));
		numstr = orgnum =
			int_to_roman(DatumGetInt32(DirectFunctionCall1(numeric_int4,
												   NumericGetDatum(x))));
		pfree(x);
	}
	else
	{
		Numeric		val = value;

		if (IS_MULTI(&Num))
		{
			Numeric		a = DatumGetNumeric(DirectFunctionCall1(int4_numeric,
													 Int32GetDatum(10)));
			Numeric		b = DatumGetNumeric(DirectFunctionCall1(int4_numeric,
											  Int32GetDatum(Num.multi)));

			x = DatumGetNumeric(DirectFunctionCall2(numeric_power,
													NumericGetDatum(a),
													NumericGetDatum(b)));
			val = DatumGetNumeric(DirectFunctionCall2(numeric_mul,
												  NumericGetDatum(value),
													NumericGetDatum(x)));
			pfree(x);
			pfree(a);
			pfree(b);
			Num.pre += Num.multi;
		}

		x = DatumGetNumeric(DirectFunctionCall2(numeric_round,
												NumericGetDatum(val),
												Int32GetDatum(Num.post)));
		orgnum = DatumGetCString(DirectFunctionCall1(numeric_out,
													 NumericGetDatum(x)));
		pfree(x);

		if (*orgnum == '-')
		{						/* < 0 */
			sign = '-';
			numstr = orgnum + 1;
		}
		else
		{
			sign = '+';
			numstr = orgnum;
		}
		if ((p = strchr(numstr, '.')))
			len = p - numstr;
		else
			len = strlen(numstr);

		if (Num.pre > len)
			plen = Num.pre - len;

		else if (len > Num.pre)
		{
			fill_str(numstr, '#', Num.pre);
			*(numstr + Num.pre) = '.';
			fill_str(numstr + 1 + Num.pre, '#', Num.post);
		}

		if (IS_MULTI(&Num))
			pfree(val);
	}

	NUM_TOCHAR_finish;
	PG_RETURN_TEXT_P(result);
}

/* ---------------
 * INT4 to_char()
 * ---------------
 */
Datum
int4_to_char(PG_FUNCTION_ARGS)
{
	int32		value = PG_GETARG_INT32(0);
	text	   *fmt = PG_GETARG_TEXT_P(1);
	NUMDesc		Num;
	FormatNode *format;
	text	   *result,
			   *result_tmp;
	int			flag = 0;
	int			len = 0,
				plen = 0,
				sign = 0;
	char	   *numstr,
			   *orgnum;

	NUM_TOCHAR_prepare;

	/*
	 * On DateType depend part (int32)
	 */
	if (IS_ROMAN(&Num))
		numstr = orgnum = int_to_roman(value);
	else
	{
		if (IS_MULTI(&Num))
		{
			orgnum = DatumGetCString(DirectFunctionCall1(int4out,
														 Int32GetDatum(value * ((int32) pow((double) 10, (double) Num.multi)))));
			Num.pre += Num.multi;
		}
		else
		{
			orgnum = DatumGetCString(DirectFunctionCall1(int4out,
												  Int32GetDatum(value)));
		}
		len = strlen(orgnum);

		if (*orgnum == '-')
		{						/* < 0 */
			sign = '-';
			--len;
		}
		else
			sign = '+';

		if (Num.post)
		{
			int			i;

			numstr = (char *) palloc(len + Num.post + 2);
			strcpy(numstr, orgnum + (*orgnum == '-' ? 1 : 0));
			*(numstr + len) = '.';

			for (i = len + 1; i <= len + Num.post; i++)
				*(numstr + i) = '0';
			*(numstr + len + Num.post + 1) = '\0';
			pfree(orgnum);
			orgnum = numstr;
		}
		else
			numstr = orgnum + (*orgnum == '-' ? 1 : 0);

		if (Num.pre > len)
			plen = Num.pre - len;
		else if (len > Num.pre)
		{
			fill_str(numstr, '#', Num.pre);
			*(numstr + Num.pre) = '.';
			fill_str(numstr + 1 + Num.pre, '#', Num.post);
		}
	}

	NUM_TOCHAR_finish;
	PG_RETURN_TEXT_P(result);
}

/* ---------------
 * INT8 to_char()
 * ---------------
 */
Datum
int8_to_char(PG_FUNCTION_ARGS)
{
	int64		value = PG_GETARG_INT64(0);
	text	   *fmt = PG_GETARG_TEXT_P(1);
	NUMDesc		Num;
	FormatNode *format;
	text	   *result,
			   *result_tmp;
	int			flag = 0;
	int			len = 0,
				plen = 0,
				sign = 0;
	char	   *numstr,
			   *orgnum;

	NUM_TOCHAR_prepare;

	/*
	 * On DateType depend part (int32)
	 */
	if (IS_ROMAN(&Num))
	{
		/* Currently don't support int8 conversion to roman... */
		numstr = orgnum = int_to_roman(DatumGetInt32(
					  DirectFunctionCall1(int84, Int64GetDatum(value))));
	}
	else
	{
		if (IS_MULTI(&Num))
		{
			double		multi = pow((double) 10, (double) Num.multi);

			value = DatumGetInt64(DirectFunctionCall2(int8mul,
													Int64GetDatum(value),
											   DirectFunctionCall1(dtoi8,
												Float8GetDatum(multi))));
			Num.pre += Num.multi;
		}

		orgnum = DatumGetCString(DirectFunctionCall1(int8out,
												  Int64GetDatum(value)));
		len = strlen(orgnum);

		if (*orgnum == '-')
		{						/* < 0 */
			sign = '-';
			--len;
		}
		else
			sign = '+';

		if (Num.post)
		{
			int			i;

			numstr = (char *) palloc(len + Num.post + 2);
			strcpy(numstr, orgnum + (*orgnum == '-' ? 1 : 0));
			*(numstr + len) = '.';

			for (i = len + 1; i <= len + Num.post; i++)
				*(numstr + i) = '0';
			*(numstr + len + Num.post + 1) = '\0';
			pfree(orgnum);
			orgnum = numstr;
		}
		else
			numstr = orgnum + (*orgnum == '-' ? 1 : 0);

		if (Num.pre > len)
			plen = Num.pre - len;
		else if (len > Num.pre)
		{
			fill_str(numstr, '#', Num.pre);
			*(numstr + Num.pre) = '.';
			fill_str(numstr + 1 + Num.pre, '#', Num.post);
		}
	}

	NUM_TOCHAR_finish;
	PG_RETURN_TEXT_P(result);
}

/* -----------------
 * FLOAT4 to_char()
 * -----------------
 */
Datum
float4_to_char(PG_FUNCTION_ARGS)
{
	float4		value = PG_GETARG_FLOAT4(0);
	text	   *fmt = PG_GETARG_TEXT_P(1);
	NUMDesc		Num;
	FormatNode *format;
	text	   *result,
			   *result_tmp;
	int			flag = 0;
	int			len = 0,
				plen = 0,
				sign = 0;
	char	   *numstr,
			   *orgnum,
			   *p;

	NUM_TOCHAR_prepare;

	if (IS_ROMAN(&Num))
	{
		numstr = orgnum = int_to_roman((int) rint(value));

	}
	else
	{
		float4		val = value;

		if (IS_MULTI(&Num))
		{
			float		multi = pow((double) 10, (double) Num.multi);

			val = value * multi;
			Num.pre += Num.multi;
		}

		orgnum = (char *) palloc(MAXFLOATWIDTH + 1);
		sprintf(orgnum, "%.0f", fabs(val));
		len = strlen(orgnum);
		if (Num.pre > len)
			plen = Num.pre - len;
		if (len >= FLT_DIG)
			Num.post = 0;
		else if (Num.post + len > FLT_DIG)
			Num.post = FLT_DIG - len;
		sprintf(orgnum, "%.*f", Num.post, val);

		if (*orgnum == '-')
		{						/* < 0 */
			sign = '-';
			numstr = orgnum + 1;
		}
		else
		{
			sign = '+';
			numstr = orgnum;
		}
		if ((p = strchr(numstr, '.')))
			len = p - numstr;
		else
			len = strlen(numstr);

		if (Num.pre > len)
			plen = Num.pre - len;

		else if (len > Num.pre)
		{
			fill_str(numstr, '#', Num.pre);
			*(numstr + Num.pre) = '.';
			fill_str(numstr + 1 + Num.pre, '#', Num.post);
		}
	}

	NUM_TOCHAR_finish;
	PG_RETURN_TEXT_P(result);
}

/* -----------------
 * FLOAT8 to_char()
 * -----------------
 */
Datum
float8_to_char(PG_FUNCTION_ARGS)
{
	float8		value = PG_GETARG_FLOAT8(0);
	text	   *fmt = PG_GETARG_TEXT_P(1);
	NUMDesc		Num;
	FormatNode *format;
	text	   *result,
			   *result_tmp;
	int			flag = 0;
	int			len = 0,
				plen = 0,
				sign = 0;
	char	   *numstr,
			   *orgnum,
			   *p;

	NUM_TOCHAR_prepare;

	if (IS_ROMAN(&Num))
	{
		numstr = orgnum = int_to_roman((int) rint(value));

	}
	else
	{
		float8		val = value;

		if (IS_MULTI(&Num))
		{
			double		multi = pow((double) 10, (double) Num.multi);

			val = value * multi;
			Num.pre += Num.multi;
		}
		orgnum = (char *) palloc(MAXDOUBLEWIDTH + 1);
		len = sprintf(orgnum, "%.0f", fabs(val));
		if (Num.pre > len)
			plen = Num.pre - len;
		if (len >= DBL_DIG)
			Num.post = 0;
		else if (Num.post + len > DBL_DIG)
			Num.post = DBL_DIG - len;
		sprintf(orgnum, "%.*f", Num.post, val);

		if (*orgnum == '-')
		{						/* < 0 */
			sign = '-';
			numstr = orgnum + 1;
		}
		else
		{
			sign = '+';
			numstr = orgnum;
		}
		if ((p = strchr(numstr, '.')))
			len = p - numstr;
		else
			len = strlen(numstr);

		if (Num.pre > len)
			plen = Num.pre - len;

		else if (len > Num.pre)
		{
			fill_str(numstr, '#', Num.pre);
			*(numstr + Num.pre) = '.';
			fill_str(numstr + 1 + Num.pre, '#', Num.post);
		}
	}

	NUM_TOCHAR_finish;
	PG_RETURN_TEXT_P(result);
}
