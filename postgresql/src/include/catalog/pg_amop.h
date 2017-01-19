/*-------------------------------------------------------------------------
 *
 * pg_amop.h
 *	  definition of the system "amop" relation (pg_amop)
 *	  along with the relation's initial contents.
 *
 * The amop table identifies the operators associated with each index opclass.
 *
 * Note: the primary key for this table is <amopclaid, amopstrategy>.
 * We also keep a unique index on <amopclaid, amopopr>, so that we can
 * use a syscache to quickly answer questions of the form "is this operator
 * in this opclass?".  This implies that the same operator cannot be listed
 * for multiple strategy numbers of a single opclass.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: pg_amop.h,v 1.2 2004/07/05 08:55:58 SuLiang Exp $
 *
 * NOTES
 *	 the genbki.sh script reads this file and generates .bki
 *	 information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_AMOP_H
#define PG_AMOP_H

/* ----------------
 *		postgres.h contains the system type definintions and the
 *		CATALOG(), BOOTSTRAP and DATA() sugar words so this file
 *		can be read by both genbki.sh and the C compiler.
 * ----------------
 */

/* ----------------
 *		pg_amop definition.  cpp turns this into
 *		typedef struct FormData_pg_amop
 * ----------------
 */
CATALOG(pg_amop) BKI_WITHOUT_OIDS
{
	Oid			amopclaid;		/* the index opclass this entry is for */
	int2		amopstrategy;	/* operator strategy number */
	bool		amopreqcheck;	/* index hit must be rechecked */
	Oid			amopopr;		/* the operator's pg_operator OID */
} FormData_pg_amop;

/* ----------------
 *		Form_pg_amop corresponds to a pointer to a tuple with
 *		the format of pg_amop relation.
 * ----------------
 */
typedef FormData_pg_amop *Form_pg_amop;

/* ----------------
 *		compiler constants for pg_amop
 * ----------------
 */
#define Natts_pg_amop					4
#define Anum_pg_amop_amopclaid			1
#define Anum_pg_amop_amopstrategy		2
#define Anum_pg_amop_amopreqcheck		3
#define Anum_pg_amop_amopopr			4

/* ----------------
 *		initial contents of pg_amop
 * ----------------
 */

/*
 *	rtree box_ops
 */

DATA(insert (	 425 1 f  493 ));
DATA(insert (	 425 2 f  494 ));
DATA(insert (	 425 3 f  500 ));
DATA(insert (	 425 4 f  495 ));
DATA(insert (	 425 5 f  496 ));
DATA(insert (	 425 6 f  499 ));
DATA(insert (	 425 7 f  498 ));
DATA(insert (	 425 8 f  497 ));

/*
 *	rtree bigbox_ops
 */

DATA(insert (	 422 1 f  493 ));
DATA(insert (	 422 2 f  494 ));
DATA(insert (	 422 3 f  500 ));
DATA(insert (	 422 4 f  495 ));
DATA(insert (	 422 5 f  496 ));
DATA(insert (	 422 6 f  499 ));
DATA(insert (	 422 7 f  498 ));
DATA(insert (	 422 8 f  497 ));

/*
 *	rtree poly_ops (supports polygons)
 */

DATA(insert (	1993 1 f  485 ));
DATA(insert (	1993 2 f  486 ));
DATA(insert (	1993 3 f  492 ));
DATA(insert (	1993 4 f  487 ));
DATA(insert (	1993 5 f  488 ));
DATA(insert (	1993 6 f  491 ));
DATA(insert (	1993 7 f  490 ));
DATA(insert (	1993 8 f  489 ));

/*
 *	btree int2_ops
 */

DATA(insert (	1976 1 f   95 ));
DATA(insert (	1976 2 f  522 ));
DATA(insert (	1976 3 f   94 ));
DATA(insert (	1976 4 f  524 ));
DATA(insert (	1976 5 f  520 ));

/*
 *	btree int4_ops
 */

DATA(insert (	1978 1 f   97 ));
DATA(insert (	1978 2 f  523 ));
DATA(insert (	1978 3 f   96 ));
DATA(insert (	1978 4 f  525 ));
DATA(insert (	1978 5 f  521 ));

/*
 *	btree int8_ops
 */

DATA(insert (	1980 1 f  412 ));
DATA(insert (	1980 2 f  414 ));
DATA(insert (	1980 3 f  410 ));
DATA(insert (	1980 4 f  415 ));
DATA(insert (	1980 5 f  413 ));

/*
 *	btree oid_ops
 */

DATA(insert (	1989 1 f  609 ));
DATA(insert (	1989 2 f  611 ));
DATA(insert (	1989 3 f  607 ));
DATA(insert (	1989 4 f  612 ));
DATA(insert (	1989 5 f  610 ));

/*
 *	btree oidvector_ops
 */

DATA(insert (	1991 1 f  645 ));
DATA(insert (	1991 2 f  647 ));
DATA(insert (	1991 3 f  649 ));
DATA(insert (	1991 4 f  648 ));
DATA(insert (	1991 5 f  646 ));

/*
 *	btree float4_ops
 */

DATA(insert (	1970 1 f  622 ));
DATA(insert (	1970 2 f  624 ));
DATA(insert (	1970 3 f  620 ));
DATA(insert (	1970 4 f  625 ));
DATA(insert (	1970 5 f  623 ));

/*
 *	btree float8_ops
 */

DATA(insert (	1972 1 f  672 ));
DATA(insert (	1972 2 f  673 ));
DATA(insert (	1972 3 f  670 ));
DATA(insert (	1972 4 f  675 ));
DATA(insert (	1972 5 f  674 ));

/*
 *	btree char_ops
 */

DATA(insert (	 429 1 f  631 ));
DATA(insert (	 429 2 f  632 ));
DATA(insert (	 429 3 f   92 ));
DATA(insert (	 429 4 f  634 ));
DATA(insert (	 429 5 f  633 ));

/*
 *	btree name_ops
 */

DATA(insert (	1986 1 f  660 ));
DATA(insert (	1986 2 f  661 ));
DATA(insert (	1986 3 f   93 ));
DATA(insert (	1986 4 f  663 ));
DATA(insert (	1986 5 f  662 ));

/*
 *	btree text_ops
 */

DATA(insert (	1994 1 f  664 ));
DATA(insert (	1994 2 f  665 ));
DATA(insert (	1994 3 f   98 ));
DATA(insert (	1994 4 f  667 ));
DATA(insert (	1994 5 f  666 ));

/*
 *	btree bpchar_ops
 */

DATA(insert (	 426 1 f 1058 ));
DATA(insert (	 426 2 f 1059 ));
DATA(insert (	 426 3 f 1054 ));
DATA(insert (	 426 4 f 1061 ));
DATA(insert (	 426 5 f 1060 ));

/*
 *	btree varchar_ops
 */

DATA(insert (	2003 1 f 1066 ));
DATA(insert (	2003 2 f 1067 ));
DATA(insert (	2003 3 f 1062 ));
DATA(insert (	2003 4 f 1069 ));
DATA(insert (	2003 5 f 1068 ));

/*
 *	btree bytea_ops
 */

DATA(insert (	 428 1 f 1957 ));
DATA(insert (	 428 2 f 1958 ));
DATA(insert (	 428 3 f 1955 ));
DATA(insert (	 428 4 f 1960 ));
DATA(insert (	 428 5 f 1959 ));

/*
 *	btree abstime_ops
 */

DATA(insert (	 421 1 f  562 ));
DATA(insert (	 421 2 f  564 ));
DATA(insert (	 421 3 f  560 ));
DATA(insert (	 421 4 f  565 ));
DATA(insert (	 421 5 f  563 ));

/*
 *	btree date_ops
 */

DATA(insert (	 434 1 f 1095 ));
DATA(insert (	 434 2 f 1096 ));
DATA(insert (	 434 3 f 1093 ));
DATA(insert (	 434 4 f 1098 ));
DATA(insert (	 434 5 f 1097 ));

/*
 *	btree time_ops
 */

DATA(insert (	1996 1 f 1110 ));
DATA(insert (	1996 2 f 1111 ));
DATA(insert (	1996 3 f 1108 ));
DATA(insert (	1996 4 f 1113 ));
DATA(insert (	1996 5 f 1112 ));

/*
 *	btree timetz_ops
 */

DATA(insert (	2000 1 f 1552 ));
DATA(insert (	2000 2 f 1553 ));
DATA(insert (	2000 3 f 1550 ));
DATA(insert (	2000 4 f 1555 ));
DATA(insert (	2000 5 f 1554 ));

/*
 *	btree timestamp_ops
 */

DATA(insert (	2039 1 f 2062 ));
DATA(insert (	2039 2 f 2063 ));
DATA(insert (	2039 3 f 2060 ));
DATA(insert (	2039 4 f 2065 ));
DATA(insert (	2039 5 f 2064 ));

/*
 *	btree timestamptz_ops
 */

DATA(insert (	1998 1 f 1322 ));
DATA(insert (	1998 2 f 1323 ));
DATA(insert (	1998 3 f 1320 ));
DATA(insert (	1998 4 f 1325 ));
DATA(insert (	1998 5 f 1324 ));

/*
 *	btree interval_ops
 */

DATA(insert (	1982 1 f 1332 ));
DATA(insert (	1982 2 f 1333 ));
DATA(insert (	1982 3 f 1330 ));
DATA(insert (	1982 4 f 1335 ));
DATA(insert (	1982 5 f 1334 ));

/*
 *	btree macaddr
 */

DATA(insert (	1984 1 f 1222 ));
DATA(insert (	1984 2 f 1223 ));
DATA(insert (	1984 3 f 1220 ));
DATA(insert (	1984 4 f 1225 ));
DATA(insert (	1984 5 f 1224 ));

/*
 *	btree inet
 */

DATA(insert (	1974 1 f 1203 ));
DATA(insert (	1974 2 f 1204 ));
DATA(insert (	1974 3 f 1201 ));
DATA(insert (	1974 4 f 1206 ));
DATA(insert (	1974 5 f 1205 ));

/*
 *	btree cidr
 */

DATA(insert (	 432 1 f  822 ));
DATA(insert (	 432 2 f  823 ));
DATA(insert (	 432 3 f  820 ));
DATA(insert (	 432 4 f  825 ));
DATA(insert (	 432 5 f  824 ));

/*
 *	btree numeric
 */

DATA(insert (	1988 1 f 1754 ));
DATA(insert (	1988 2 f 1755 ));
DATA(insert (	1988 3 f 1752 ));
DATA(insert (	1988 4 f 1757 ));
DATA(insert (	1988 5 f 1756 ));

/*
 *	btree bool
 */

DATA(insert (	 424 1 f   58 ));
DATA(insert (	 424 2 f 1694 ));
DATA(insert (	 424 3 f   91 ));
DATA(insert (	 424 4 f 1695 ));
DATA(insert (	 424 5 f   59 ));

/*
 *	btree bit
 */

DATA(insert (	 423 1 f 1786 ));
DATA(insert (	 423 2 f 1788 ));
DATA(insert (	 423 3 f 1784 ));
DATA(insert (	 423 4 f 1789 ));
DATA(insert (	 423 5 f 1787 ));

/*
 *	btree varbit
 */

DATA(insert (	2002 1 f 1806 ));
DATA(insert (	2002 2 f 1808 ));
DATA(insert (	2002 3 f 1804 ));
DATA(insert (	2002 4 f 1809 ));
DATA(insert (	2002 5 f 1807 ));


/*
 *	hash index _ops
 */

/* bpchar_ops */
DATA(insert (	 427 1 f 1054 ));
/* char_ops */
DATA(insert (	 431 1 f   92 ));
/* cidr_ops */
DATA(insert (	 433 1 f  820 ));
/* date_ops */
DATA(insert (	 435 1 f 1093 ));
/* float4_ops */
DATA(insert (	1971 1 f  620 ));
/* float8_ops */
DATA(insert (	1973 1 f  670 ));
/* inet_ops */
DATA(insert (	1975 1 f 1201 ));
/* int2_ops */
DATA(insert (	1977 1 f   94 ));
/* int4_ops */
DATA(insert (	1979 1 f   96 ));
/* int8_ops */
DATA(insert (	1981 1 f  410 ));
/* interval_ops */
DATA(insert (	1983 1 f 1330 ));
/* macaddr_ops */
DATA(insert (	1985 1 f 1220 ));
/* name_ops */
DATA(insert (	1987 1 f   93 ));
/* oid_ops */
DATA(insert (	1990 1 f  607 ));
/* oidvector_ops */
DATA(insert (	1992 1 f  649 ));
/* text_ops */
DATA(insert (	1995 1 f   98 ));
/* time_ops */
DATA(insert (	1997 1 f 1108 ));
/* timestamptz_ops */
DATA(insert (	1999 1 f 1320 ));
/* timetz_ops */
DATA(insert (	2001 1 f 1550 ));
/* varchar_ops */
DATA(insert (	2004 1 f 1062 ));
/* timestamp_ops */
DATA(insert (	2040 1 f 2060 ));

#endif   /* PG_AMOP_H */
