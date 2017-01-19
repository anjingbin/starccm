/*-------------------------------------------------------------------------
 *
 * date.h
 *	  Definitions for the SQL92 "date" and "time" types.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: date.h,v 1.2 2004/07/05 08:56:08 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef DATE_H
#define DATE_H

#include "fmgr.h"


typedef int32 DateADT;

typedef float8 TimeADT;

typedef struct
{
	double		time;			/* all time units other than months and
								 * years */
	int32		zone;			/* numeric time zone, in seconds */
} TimeTzADT;

/*
 * Macros for fmgr-callable functions.
 *
 * For TimeADT, we make use of the same support routines as for float8.
 * Therefore TimeADT is pass-by-reference if and only if float8 is!
 */
#define DatumGetDateADT(X)	  ((DateADT) DatumGetInt32(X))
#define DatumGetTimeADT(X)	  ((TimeADT) DatumGetFloat8(X))
#define DatumGetTimeTzADTP(X) ((TimeTzADT *) DatumGetPointer(X))

#define DateADTGetDatum(X)	  Int32GetDatum(X)
#define TimeADTGetDatum(X)	  Float8GetDatum(X)
#define TimeTzADTPGetDatum(X) PointerGetDatum(X)

#define PG_GETARG_DATEADT(n)	 DatumGetDateADT(PG_GETARG_DATUM(n))
#define PG_GETARG_TIMEADT(n)	 DatumGetTimeADT(PG_GETARG_DATUM(n))
#define PG_GETARG_TIMETZADT_P(n) DatumGetTimeTzADTP(PG_GETARG_DATUM(n))

#define PG_RETURN_DATEADT(x)	 return DateADTGetDatum(x)
#define PG_RETURN_TIMEADT(x)	 return TimeADTGetDatum(x)
#define PG_RETURN_TIMETZADT_P(x) return TimeTzADTPGetDatum(x)


/* date.c */
extern Datum date_in(PG_FUNCTION_ARGS);
extern Datum date_out(PG_FUNCTION_ARGS);
extern Datum date_eq(PG_FUNCTION_ARGS);
extern Datum date_ne(PG_FUNCTION_ARGS);
extern Datum date_lt(PG_FUNCTION_ARGS);
extern Datum date_le(PG_FUNCTION_ARGS);
extern Datum date_gt(PG_FUNCTION_ARGS);
extern Datum date_ge(PG_FUNCTION_ARGS);
extern Datum date_cmp(PG_FUNCTION_ARGS);
extern Datum date_larger(PG_FUNCTION_ARGS);
extern Datum date_smaller(PG_FUNCTION_ARGS);
extern Datum date_mi(PG_FUNCTION_ARGS);
extern Datum date_pli(PG_FUNCTION_ARGS);
extern Datum date_mii(PG_FUNCTION_ARGS);
extern Datum date_timestamp(PG_FUNCTION_ARGS);
extern Datum timestamp_date(PG_FUNCTION_ARGS);
extern Datum date_timestamptz(PG_FUNCTION_ARGS);
extern Datum timestamptz_date(PG_FUNCTION_ARGS);
extern Datum datetime_timestamp(PG_FUNCTION_ARGS);
extern Datum abstime_date(PG_FUNCTION_ARGS);
extern Datum text_date(PG_FUNCTION_ARGS);
extern Datum date_text(PG_FUNCTION_ARGS);

extern Datum time_in(PG_FUNCTION_ARGS);
extern Datum time_out(PG_FUNCTION_ARGS);
extern Datum time_scale(PG_FUNCTION_ARGS);
extern Datum time_eq(PG_FUNCTION_ARGS);
extern Datum time_ne(PG_FUNCTION_ARGS);
extern Datum time_lt(PG_FUNCTION_ARGS);
extern Datum time_le(PG_FUNCTION_ARGS);
extern Datum time_gt(PG_FUNCTION_ARGS);
extern Datum time_ge(PG_FUNCTION_ARGS);
extern Datum time_cmp(PG_FUNCTION_ARGS);
extern Datum overlaps_time(PG_FUNCTION_ARGS);
extern Datum time_larger(PG_FUNCTION_ARGS);
extern Datum time_smaller(PG_FUNCTION_ARGS);
extern Datum time_mi_time(PG_FUNCTION_ARGS);
extern Datum timestamp_time(PG_FUNCTION_ARGS);
extern Datum time_interval(PG_FUNCTION_ARGS);
extern Datum interval_time(PG_FUNCTION_ARGS);
extern Datum text_time(PG_FUNCTION_ARGS);
extern Datum time_text(PG_FUNCTION_ARGS);
extern Datum time_pl_interval(PG_FUNCTION_ARGS);
extern Datum time_mi_interval(PG_FUNCTION_ARGS);
extern Datum interval_pl_time(PG_FUNCTION_ARGS);

extern Datum timetz_in(PG_FUNCTION_ARGS);
extern Datum timetz_out(PG_FUNCTION_ARGS);
extern Datum timetz_scale(PG_FUNCTION_ARGS);
extern Datum timetz_eq(PG_FUNCTION_ARGS);
extern Datum timetz_ne(PG_FUNCTION_ARGS);
extern Datum timetz_lt(PG_FUNCTION_ARGS);
extern Datum timetz_le(PG_FUNCTION_ARGS);
extern Datum timetz_gt(PG_FUNCTION_ARGS);
extern Datum timetz_ge(PG_FUNCTION_ARGS);
extern Datum timetz_cmp(PG_FUNCTION_ARGS);
extern Datum timetz_hash(PG_FUNCTION_ARGS);
extern Datum overlaps_timetz(PG_FUNCTION_ARGS);
extern Datum timetz_larger(PG_FUNCTION_ARGS);
extern Datum timetz_smaller(PG_FUNCTION_ARGS);
extern Datum timetz_time(PG_FUNCTION_ARGS);
extern Datum time_timetz(PG_FUNCTION_ARGS);
extern Datum timestamptz_timetz(PG_FUNCTION_ARGS);
extern Datum datetimetz_timestamptz(PG_FUNCTION_ARGS);
extern Datum text_timetz(PG_FUNCTION_ARGS);
extern Datum timetz_text(PG_FUNCTION_ARGS);
extern Datum timetz_part(PG_FUNCTION_ARGS);
extern Datum timetz_zone(PG_FUNCTION_ARGS);
extern Datum timetz_izone(PG_FUNCTION_ARGS);
extern Datum timetz_pl_interval(PG_FUNCTION_ARGS);
extern Datum timetz_mi_interval(PG_FUNCTION_ARGS);

#endif   /* DATE_H */
