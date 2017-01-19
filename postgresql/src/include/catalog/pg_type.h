/*-------------------------------------------------------------------------
 *
 * pg_type.h
 *	  definition of the system "type" relation (pg_type)
 *	  along with the relation's initial contents.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: pg_type.h,v 1.2 2004/07/05 08:55:58 SuLiang Exp $
 *
 * NOTES
 *	  the genbki.sh script reads this file and generates .bki
 *	  information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_TYPE_H
#define PG_TYPE_H

/* ----------------
 *		postgres.h contains the system type definitions and the
 *		CATALOG(), BOOTSTRAP and DATA() sugar words so this file
 *		can be read by both genbki.sh and the C compiler.
 * ----------------
 */

/* ----------------
 *		pg_type definition.  cpp turns this into
 *		typedef struct FormData_pg_type
 *
 *		Some of the values in a pg_type instance are copied into
 *		pg_attribute instances.  Some parts of Postgres use the pg_type copy,
 *		while others use the pg_attribute copy, so they must match.
 *		See struct FormData_pg_attribute for details.
 * ----------------
 */
CATALOG(pg_type) BOOTSTRAP
{
	NameData	typname;
	int4		typowner;

	/*
	 * For a fixed-size type, typlen is the number of bytes we use to
	 * represent a value of this type, e.g. 4 for an int4.	But for a
	 * variable-length type, typlen is -1.
	 */
	int2		typlen;

	/*
	 * typprtlen was once intended to be the length of the external
	 * representation of a datatype, with the same interpretation as for
	 * typlen.	But it's currently unused.
	 */
	int2		typprtlen;

	/*
	 * typbyval determines whether internal Postgres routines pass a value
	 * of this type by value or by reference.  typbyval had better be
	 * FALSE if the length is not 1, 2, or 4 (or 8 on 8-byte-Datum
	 * machines). Variable-length types are always passed by reference.
	 * Note that typbyval can be false even if the length would allow
	 * pass-by-value; this is currently true for type float4, for example.
	 */
	bool		typbyval;

	/*
	 * typtype is 'b' for a basic type and 'c' for a catalog type (ie a
	 * class). If typtype is 'c', typrelid is the OID of the class' entry
	 * in pg_class. (Why do we need an entry in pg_type for classes,
	 * anyway?)
	 */
	char		typtype;

	/*
	 * If typisdefined is false, the entry is only a placeholder (forward
	 * reference).	We know the type name, but not yet anything else about
	 * it.
	 */
	bool		typisdefined;

	char		typdelim;		/* delimiter for arrays of this type */
	Oid			typrelid;		/* 0 if not a class type */

	/*
	 * If typelem is not 0 then it identifies another row in pg_type. The
	 * current type can then be subscripted like an array yielding values
	 * of type typelem. A non-zero typelem does not guarantee this type to
	 * be a "real" array type; some ordinary fixed-length types can also
	 * be subscripted (e.g., oidvector). Variable-length types can *not*
	 * be turned into pseudo-arrays like that. Hence, the way to determine
	 * whether a type is a "true" array type is if:
	 *
	 * typelem != 0 and typlen < 0.
	 */
	Oid			typelem;

	/*
	 * I/O conversion procedures for the datatype.
	 */
	regproc		typinput;
	regproc		typoutput;
	regproc		typreceive;
	regproc		typsend;

	/* ----------------
	 * typalign is the alignment required when storing a value of this
	 * type.  It applies to storage on disk as well as most
	 * representations of the value inside Postgres.  When multiple values
	 * are stored consecutively, such as in the representation of a
	 * complete row on disk, padding is inserted before a datum of this
	 * type so that it begins on the specified boundary.  The alignment
	 * reference is the beginning of the first datum in the sequence.
	 *
	 * 'c' = CHAR alignment, ie no alignment needed.
	 * 's' = SHORT alignment (2 bytes on most machines).
	 * 'i' = INT alignment (4 bytes on most machines).
	 * 'd' = DOUBLE alignment (8 bytes on many machines, but by no means all).
	 *
	 * See include/utils/memutils.h for the macros that compute these
	 * alignment requirements.
	 *
	 * NOTE: for types used in system tables, it is critical that the
	 * size and alignment defined in pg_type agree with the way that the
	 * compiler will lay out the field in a struct representing a table row.
	 * ----------------
	 */
	char		typalign;

	/* ----------------
	 * typstorage tells if the type is prepared for toasting and what
	 * the default strategy for attributes of this type should be.
	 *
	 * 'p' PLAIN	  type not prepared for toasting
	 * 'e' EXTERNAL   external storage possible, don't try to compress
	 * 'x' EXTENDED   try to compress and store external if required
	 * 'm' MAIN		  like 'x' but try to keep in main tuple
	 * ----------------
	 */
	char		typstorage;

	/*
	 * typdefault is NULL if the type has no associated default value. If
	 * it's not NULL, it contains the external representation of the
	 * type's default value --- this default is used whenever no
	 * per-column default is specified for a column of the datatype.
	 */
	text		typdefault;		/* VARIABLE LENGTH FIELD */
} FormData_pg_type;

/* ----------------
 *		Form_pg_type corresponds to a pointer to a row with
 *		the format of pg_type relation.
 * ----------------
 */
typedef FormData_pg_type *Form_pg_type;

/* ----------------
 *		compiler constants for pg_type
 * ----------------
 */
#define Natts_pg_type					17
#define Anum_pg_type_typname			1
#define Anum_pg_type_typowner			2
#define Anum_pg_type_typlen				3
#define Anum_pg_type_typprtlen			4
#define Anum_pg_type_typbyval			5
#define Anum_pg_type_typtype			6
#define Anum_pg_type_typisdefined		7
#define Anum_pg_type_typdelim			8
#define Anum_pg_type_typrelid			9
#define Anum_pg_type_typelem			10
#define Anum_pg_type_typinput			11
#define Anum_pg_type_typoutput			12
#define Anum_pg_type_typreceive			13
#define Anum_pg_type_typsend			14
#define Anum_pg_type_typalign			15
#define Anum_pg_type_typstorage			16
#define Anum_pg_type_typdefault			17

/* ----------------
 *		initial contents of pg_type
 * ----------------
 */

/* keep the following ordered by OID so that later changes can be made easier*/

/* Make sure the typlen, typbyval, and typalign values here match the initial
   values for attlen, attbyval, and attalign in both places in pg_attribute.h
   for every instance.
*/

/* OIDS 1 - 99 */
DATA(insert OID = 16 (	bool	   PGUID  1   1 t b t \054 0   0 boolin boolout boolin boolout c p _null_ ));
DESCR("boolean, 'true'/'false'");
#define BOOLOID			16

DATA(insert OID = 17 (	bytea	   PGUID -1  -1 f b t \054 0  0 byteain byteaout byteain byteaout i x _null_ ));
DESCR("variable-length string, binary values escaped");
#define BYTEAOID		17

DATA(insert OID = 18 (	char	   PGUID  1   1 t b t \054 0   0 charin charout charin charout c p _null_ ));
DESCR("single character");
#define CHAROID			18

DATA(insert OID = 19 (	name	   PGUID NAMEDATALEN NAMEDATALEN  f b t \054 0	18 namein nameout namein nameout i p _null_ ));
DESCR("31-character type for storing system identifiers");
#define NAMEOID			19

DATA(insert OID = 20 (	int8	   PGUID  8  20 f b t \054 0   0 int8in int8out int8in int8out d p _null_ ));
DESCR("~18 digit integer, 8-byte storage");
#define INT8OID			20

DATA(insert OID = 21 (	int2	   PGUID  2   5 t b t \054 0   0 int2in int2out int2in int2out s p _null_ ));
DESCR("-32 thousand to 32 thousand, 2-byte storage");
#define INT2OID			21

DATA(insert OID = 22 (	int2vector PGUID INDEX_MAX_KEYS*2 -1 f b t \054 0  21 int2vectorin int2vectorout int2vectorin int2vectorout i p _null_ ));
DESCR("array of INDEX_MAX_KEYS int2 integers, used in system tables");
#define INT2VECTOROID	22

DATA(insert OID = 23 (	int4	   PGUID  4  10 t b t \054 0   0 int4in int4out int4in int4out i p _null_ ));
DESCR("-2 billion to 2 billion integer, 4-byte storage");
#define INT4OID			23

DATA(insert OID = 24 (	regproc    PGUID  4  16 t b t \054 0   0 regprocin regprocout regprocin regprocout i p _null_ ));
DESCR("registered procedure");
#define REGPROCOID		24

DATA(insert OID = 25 (	text	   PGUID -1  -1 f b t \054 0  0 textin textout textin textout i x _null_ ));
DESCR("variable-length string, no limit specified");
#define TEXTOID			25

DATA(insert OID = 26 (	oid		   PGUID  4  10 t b t \054 0   0 oidin oidout oidin oidout i p _null_ ));
DESCR("object identifier(oid), maximum 4 billion");
#define OIDOID			26

DATA(insert OID = 27 (	tid		   PGUID  6  19 f b t \054 0   0 tidin tidout tidin tidout i p _null_ ));
DESCR("(Block, offset), physical location of tuple");
#define TIDOID		27

DATA(insert OID = 28 (	xid		   PGUID  4  12 t b t \054 0   0 xidin xidout xidin xidout i p _null_ ));
DESCR("transaction id");
#define XIDOID 28

DATA(insert OID = 29 (	cid		   PGUID  4  10 t b t \054 0   0 cidin cidout cidin cidout i p _null_ ));
DESCR("command identifier type, sequence in transaction id");
#define CIDOID 29

DATA(insert OID = 30 (	oidvector  PGUID INDEX_MAX_KEYS*4 -1 f b t \054 0  26 oidvectorin oidvectorout oidvectorin oidvectorout i p _null_ ));
DESCR("array of INDEX_MAX_KEYS oids, used in system tables");
#define OIDVECTOROID	30

DATA(insert OID = 32 (	SET		   PGUID -1  -1 f b t \054 0   0 textin textout textin textout i p _null_ ));
DESCR("set of tuples");

DATA(insert OID = 71 (	pg_type		 PGUID 4 4 t c t \054 1247 0 int4in int4out int4in int4out i p _null_));
DATA(insert OID = 75 (	pg_attribute PGUID 4 4 t c t \054 1249 0 int4in int4out int4in int4out i p _null_));
DATA(insert OID = 81 (	pg_proc		 PGUID 4 4 t c t \054 1255 0 int4in int4out int4in int4out i p _null_));
DATA(insert OID = 83 (	pg_class	 PGUID 4 4 t c t \054 1259 0 int4in int4out int4in int4out i p _null_));
DATA(insert OID = 86 (	pg_shadow	 PGUID 4 4 t c t \054 1260 0 int4in int4out int4in int4out i p _null_));
DATA(insert OID = 87 (	pg_group	 PGUID 4 4 t c t \054 1261 0 int4in int4out int4in int4out i p _null_));
DATA(insert OID = 88 (	pg_database  PGUID 4 4 t c t \054 1262 0 int4in int4out int4in int4out i p _null_));

/* OIDS 100 - 199 */

/* OIDS 200 - 299 */

DATA(insert OID = 210 (  smgr	   PGUID 2	12 t b t \054 0 0 smgrin smgrout smgrin smgrout s p _null_ ));
DESCR("storage manager");

/* OIDS 300 - 399 */

/* OIDS 400 - 499 */

/* OIDS 500 - 599 */

/* OIDS 600 - 699 */
DATA(insert OID = 600 (  point	   PGUID 16  24 f b t \054 0 701 point_in point_out point_in point_out d p _null_ ));
DESCR("geometric point '(x, y)'");
#define POINTOID		600
DATA(insert OID = 601 (  lseg	   PGUID 32  48 f b t \054 0 600 lseg_in lseg_out lseg_in lseg_out d p _null_ ));
DESCR("geometric line segment '(pt1,pt2)'");
#define LSEGOID			601
DATA(insert OID = 602 (  path	   PGUID -1  -1 f b t \054 0 0 path_in path_out path_in path_out d x _null_ ));
DESCR("geometric path '(pt1,...)'");
#define PATHOID			602
DATA(insert OID = 603 (  box	   PGUID 32 100 f b t \073 0 600 box_in box_out box_in box_out d p _null_ ));
DESCR("geometric box '(lower left,upper right)'");
#define BOXOID			603
DATA(insert OID = 604 (  polygon   PGUID -1  -1 f b t \054 0   0 poly_in poly_out poly_in poly_out d x _null_ ));
DESCR("geometric polygon '(pt1,...)'");
#define POLYGONOID		604

DATA(insert OID = 628 (  line	   PGUID 32  48 f b t \054 0 701 line_in line_out line_in line_out d p _null_ ));
DESCR("geometric line '(pt1,pt2)'");
#define LINEOID			628
DATA(insert OID = 629 (  _line	   PGUID  -1 -1 f b t \054 0 628 array_in array_out array_in array_out d x _null_ ));
DESCR("");

/* OIDS 700 - 799 */

DATA(insert OID = 700 (  float4    PGUID  4  12 f b t \054 0   0 float4in float4out float4in float4out i p _null_ ));
DESCR("single-precision floating point number, 4-byte storage");
#define FLOAT4OID 700
DATA(insert OID = 701 (  float8    PGUID  8  24 f b t \054 0   0 float8in float8out float8in float8out d p _null_ ));
DESCR("double-precision floating point number, 8-byte storage");
#define FLOAT8OID 701
DATA(insert OID = 702 (  abstime   PGUID  4  20 t b t \054 0   0 nabstimein nabstimeout nabstimein nabstimeout i p _null_ ));
DESCR("absolute, limited-range date and time (Unix system time)");
#define ABSTIMEOID		702
DATA(insert OID = 703 (  reltime   PGUID  4  20 t b t \054 0   0 reltimein reltimeout reltimein reltimeout i p _null_ ));
DESCR("relative, limited-range time interval (Unix delta time)");
#define RELTIMEOID		703
DATA(insert OID = 704 (  tinterval PGUID 12  47 f b t \054 0   0 tintervalin tintervalout tintervalin tintervalout i p _null_ ));
DESCR("(abstime,abstime), time interval");
#define TINTERVALOID	704
DATA(insert OID = 705 (  unknown   PGUID -1  -1 f b t \054 0   0 textin textout textin textout i p _null_ ));
DESCR("");
#define UNKNOWNOID		705

DATA(insert OID = 718 (  circle    PGUID  24 47 f b t \054 0	0 circle_in circle_out circle_in circle_out d p _null_ ));
DESCR("geometric circle '(center,radius)'");
#define CIRCLEOID		718
DATA(insert OID = 719 (  _circle   PGUID  -1 -1 f b t \054 0  718 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 790 (  money	   PGUID   4 24 f b t \054 0	0 cash_in cash_out cash_in cash_out i p _null_ ));
DESCR("$d,ddd.cc, money");
#define CASHOID 790
DATA(insert OID = 791 (  _money    PGUID  -1 -1 f b t \054 0  790 array_in array_out array_in array_out i x _null_ ));

/* OIDS 800 - 899 */
DATA(insert OID = 829 ( macaddr    PGUID  6 -1 f b t \054 0 0 macaddr_in macaddr_out macaddr_in macaddr_out i p _null_ ));
DESCR("XX:XX:XX:XX:XX:XX, MAC address");
#define MACADDROID 829
DATA(insert OID = 869 ( inet	   PGUID  -1 -1 f b t \054 0 0 inet_in inet_out inet_in inet_out i p _null_ ));
DESCR("IP address/netmask, host address, netmask optional");
#define INETOID 869
DATA(insert OID = 650 ( cidr	   PGUID  -1 -1 f b t \054 0 0 cidr_in cidr_out cidr_in cidr_out i p _null_ ));
DESCR("network IP address/netmask, network address");
#define CIDROID 650

/* OIDS 900 - 999 */

/* OIDS 1000 - 1099 */
DATA(insert OID = 1000 (  _bool		 PGUID -1  -1 f b t \054 0	16 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1001 (  _bytea	 PGUID -1  -1 f b t \054 0	17 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1002 (  _char		 PGUID -1  -1 f b t \054 0	18 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1003 (  _name		 PGUID -1  -1 f b t \054 0	19 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1005 (  _int2		 PGUID -1  -1 f b t \054 0	21 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1006 (  _int2vector PGUID -1 -1 f b t \054 0	22 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1007 (  _int4		 PGUID -1  -1 f b t \054 0	23 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1008 (  _regproc	 PGUID -1  -1 f b t \054 0	24 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1009 (  _text		 PGUID -1  -1 f b t \054 0	25 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1028 (  _oid		 PGUID -1  -1 f b t \054 0	26 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1010 (  _tid		 PGUID -1  -1 f b t \054 0	27 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1011 (  _xid		 PGUID -1  -1 f b t \054 0	28 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1012 (  _cid		 PGUID -1  -1 f b t \054 0	29 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1013 (  _oidvector PGUID -1  -1 f b t \054 0	30 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1014 (  _bpchar	 PGUID -1  -1 f b t \054 0 1042 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1015 (  _varchar	 PGUID -1  -1 f b t \054 0 1043 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1016 (  _int8		 PGUID -1  -1 f b t \054 0	20 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1017 (  _point	 PGUID -1  -1 f b t \054 0 600 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1018 (  _lseg		 PGUID -1  -1 f b t \054 0 601 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1019 (  _path		 PGUID -1  -1 f b t \054 0 602 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1020 (  _box		 PGUID -1  -1 f b t \073 0 603 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1021 (  _float4	 PGUID -1  -1 f b t \054 0 700 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1022 (  _float8	 PGUID -1  -1 f b t \054 0 701 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1023 (  _abstime	 PGUID -1  -1 f b t \054 0 702 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1024 (  _reltime	 PGUID -1  -1 f b t \054 0 703 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1025 (  _tinterval PGUID -1  -1 f b t \054 0 704 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1027 (  _polygon	 PGUID -1  -1 f b t \054 0 604 array_in array_out array_in array_out d x _null_ ));
/*
 *	Note: the size of aclitem needs to match sizeof(AclItem) in acl.h.
 *	Thanks to some padding, this will be 8 on all platforms.
 *	We also have an Assert to make sure.
 */
#define ACLITEMSIZE 8
DATA(insert OID = 1033 (  aclitem	 PGUID 8   -1 f b t \054 0 0 aclitemin aclitemout aclitemin aclitemout i p _null_ ));
DESCR("access control list");
DATA(insert OID = 1034 (  _aclitem	 PGUID -1 -1 f b t \054 0 1033 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1040 (  _macaddr	 PGUID -1 -1 f b t \054 0  829 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1041 (  _inet    PGUID -1 -1 f b t \054 0  869 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 651  (  _cidr    PGUID -1 -1 f b t \054 0  650 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1042 ( bpchar		 PGUID -1  -1 f b t \054 0	0 bpcharin bpcharout bpcharin bpcharout i x _null_ ));
DESCR("char(length), blank-padded string, fixed storage length");
#define BPCHAROID		1042
DATA(insert OID = 1043 ( varchar	 PGUID -1  -1 f b t \054 0	0 varcharin varcharout varcharin varcharout i x _null_ ));
DESCR("varchar(length), non-blank-padded string, variable storage length");
#define VARCHAROID		1043

DATA(insert OID = 1082 ( date		 PGUID	4  10 t b t \054 0	0 date_in date_out date_in date_out i p _null_ ));
DESCR("ANSI SQL date");
#define DATEOID			1082
DATA(insert OID = 1083 ( time		 PGUID	8  16 f b t \054 0	0 time_in time_out time_in time_out d p _null_ ));
DESCR("hh:mm:ss, ANSI SQL time");
#define TIMEOID			1083

/* OIDS 1100 - 1199 */
DATA(insert OID = 1114 ( timestamp	 PGUID	8  47 f b t \054 0	0 timestamp_in timestamp_out timestamp_in timestamp_out d p _null_ ));
DESCR("date and time");
#define TIMESTAMPOID	1114
DATA(insert OID = 1115 ( _timestamp  PGUID	-1 -1 f b t \054 0	1184 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1182 ( _date		 PGUID	-1 -1 f b t \054 0	1082 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1183 ( _time		 PGUID	-1 -1 f b t \054 0	1083 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1184 ( timestamptz PGUID	8  47 f b t \054 0	0 timestamptz_in timestamptz_out timestamptz_in timestamptz_out d p _null_ ));
DESCR("date and time with time zone");
#define TIMESTAMPTZOID	1184
DATA(insert OID = 1185 ( _timestamptz PGUID -1 -1 f b t \054 0	1184 array_in array_out array_in array_out d x _null_ ));
DATA(insert OID = 1186 ( interval	 PGUID 12  47 f b t \054 0	0 interval_in interval_out interval_in interval_out d p _null_ ));
DESCR("@ <number> <units>, time interval");
#define INTERVALOID		1186
DATA(insert OID = 1187 ( _interval	 PGUID	-1 -1 f b t \054 0	1186 array_in array_out array_in array_out d x _null_ ));

/* OIDS 1200 - 1299 */
DATA(insert OID = 1231 (  _numeric	 PGUID -1  -1 f b t \054 0	1700 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1266 ( timetz		 PGUID 12  22 f b t \054 0	0 timetz_in timetz_out timetz_in timetz_out d p _null_ ));
DESCR("hh:mm:ss, ANSI SQL time");
#define TIMETZOID		1266
DATA(insert OID = 1270 ( _timetz	 PGUID	-1 -1 f b t \054 0	1266 array_in array_out array_in array_out d x _null_ ));

/* OIDS 1500 - 1599 */
DATA(insert OID = 1560 ( bit		 PGUID -1  -1 f b t \054 0	0 bit_in bit_out bit_in bit_out i x _null_ ));
DESCR("fixed-length bit string");
#define BITOID	 1560
DATA(insert OID = 1561 ( _bit		 PGUID	-1 -1 f b t \054 0	1560 array_in array_out array_in array_out i x _null_ ));
DATA(insert OID = 1562 ( varbit		 PGUID -1  -1 f b t \054 0	0 varbit_in varbit_out varbit_in varbit_out i x _null_ ));
DESCR("variable-length bit string");
#define VARBITOID	  1562
DATA(insert OID = 1563 ( _varbit	 PGUID	-1 -1 f b t \054 0	1562 array_in array_out array_in array_out i x _null_ ));

/* OIDS 1600 - 1699 */

/* OIDS 1700 - 1799 */
DATA(insert OID = 1700 ( numeric	   PGUID -1  -1 f b t \054 0  0 numeric_in numeric_out numeric_in numeric_out i m _null_ ));
DESCR("numeric(precision, decimal), arbitrary precision number");
#define NUMERICOID		1700

/* OID 1790 */
DATA(insert OID = 1790 ( refcursor	   PGUID -1  -1 f b t \054 0  0 textin textout textin textout i x _null_ ));
DESCR("reference cursor (portal name)");
#define REFCURSOROID	1790


/*
 * prototypes for functions in pg_type.c
 */
extern Oid	TypeGet(char *typeName, bool *defined);
extern Oid	TypeShellMake(char *typeName);
extern Oid TypeCreate(char *typeName,
		   Oid assignedTypeOid,
		   Oid relationOid,
		   int16 internalSize,
		   int16 externalSize,
		   char typeType,
		   char typDelim,
		   char *inputProcedure,
		   char *outputProcedure,
		   char *receiveProcedure,
		   char *sendProcedure,
		   char *elementTypeName,
		   char *defaultTypeValue,
		   bool passedByValue,
		   char alignment,
		   char storage);
extern void TypeRename(const char *oldTypeName, const char *newTypeName);
extern char *makeArrayTypeName(char *typeName);

#endif   /* PG_TYPE_H */
