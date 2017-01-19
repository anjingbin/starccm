/*-------------------------------------------------------------------------
 *
 * date.c
 *	  implements DATE and TIME data types specified in SQL-92 standard
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994-5, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/adt/date.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <float.h>

#include "access/hash.h"
#include "miscadmin.h"
#include "utils/builtins.h"
#include "utils/date.h"
#include "utils/nabstime.h"
#include "utils/timestamp.h"


static void AdjustTimeForTypmod(TimeADT *time, int32 typmod);

/*****************************************************************************
 *	 Date ADT
 *****************************************************************************/


/* date_in()
 * Given date text string, convert to internal date format.
 */
Datum
date_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	DateADT		date;
	double		fsec;
	struct tm	tt,
			   *tm = &tt;
	int			tzp;
	int			dtype;
	int			nf;
	char	   *field[MAXDATEFIELDS];
	int			ftype[MAXDATEFIELDS];
	char		lowstr[MAXDATELEN + 1];

	if ((ParseDateTime(str, lowstr, field, ftype, MAXDATEFIELDS, &nf) != 0)
	 || (DecodeDateTime(field, ftype, nf, &dtype, tm, &fsec, &tzp) != 0))
		elog(ERROR, "Bad date external representation '%s'", str);

	switch (dtype)
	{
		case DTK_DATE:
			break;

		case DTK_CURRENT:
			elog(ERROR, "Date CURRENT no longer supported"
				 "\n\tdate_in() internal coding error");
			GetCurrentTime(tm);
			break;

		case DTK_EPOCH:
			GetEpochTime(tm);
			break;

		default:
			elog(ERROR, "Unrecognized date external representation '%s'", str);
	}

	date = (date2j(tm->tm_year, tm->tm_mon, tm->tm_mday) - date2j(2000, 1, 1));

	PG_RETURN_DATEADT(date);
}

/* date_out()
 * Given internal format date, convert to text string.
 */
Datum
date_out(PG_FUNCTION_ARGS)
{
	DateADT		date = PG_GETARG_DATEADT(0);
	char	   *result;
	struct tm	tt,
			   *tm = &tt;
	char		buf[MAXDATELEN + 1];

	j2date((date + date2j(2000, 1, 1)),
		   &(tm->tm_year), &(tm->tm_mon), &(tm->tm_mday));

	EncodeDateOnly(tm, DateStyle, buf);

	result = pstrdup(buf);
	PG_RETURN_CSTRING(result);
}

Datum
date_eq(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_BOOL(dateVal1 == dateVal2);
}

Datum
date_ne(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_BOOL(dateVal1 != dateVal2);
}

Datum
date_lt(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_BOOL(dateVal1 < dateVal2);
}

Datum
date_le(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_BOOL(dateVal1 <= dateVal2);
}

Datum
date_gt(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_BOOL(dateVal1 > dateVal2);
}

Datum
date_ge(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_BOOL(dateVal1 >= dateVal2);
}

Datum
date_cmp(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	if (dateVal1 < dateVal2)
		PG_RETURN_INT32(-1);
	else if (dateVal1 > dateVal2)
		PG_RETURN_INT32(1);
	PG_RETURN_INT32(0);
}

Datum
date_larger(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_DATEADT((dateVal1 > dateVal2) ? dateVal1 : dateVal2);
}

Datum
date_smaller(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_DATEADT((dateVal1 < dateVal2) ? dateVal1 : dateVal2);
}

/* Compute difference between two dates in days.
 */
Datum
date_mi(PG_FUNCTION_ARGS)
{
	DateADT		dateVal1 = PG_GETARG_DATEADT(0);
	DateADT		dateVal2 = PG_GETARG_DATEADT(1);

	PG_RETURN_INT32((int32) (dateVal1 - dateVal2));
}

/* Add a number of days to a date, giving a new date.
 * Must handle both positive and negative numbers of days.
 */
Datum
date_pli(PG_FUNCTION_ARGS)
{
	DateADT		dateVal = PG_GETARG_DATEADT(0);
	int32		days = PG_GETARG_INT32(1);

	PG_RETURN_DATEADT(dateVal + days);
}

/* Subtract a number of days from a date, giving a new date.
 */
Datum
date_mii(PG_FUNCTION_ARGS)
{
	DateADT		dateVal = PG_GETARG_DATEADT(0);
	int32		days = PG_GETARG_INT32(1);

	PG_RETURN_DATEADT(dateVal - days);
}

/* date_timestamp()
 * Convert date to timestamp data type.
 */
Datum
date_timestamp(PG_FUNCTION_ARGS)
{
	DateADT		dateVal = PG_GETARG_DATEADT(0);
	Timestamp	result;

	/* date is days since 2000, timestamp is seconds since same... */
	result = dateVal * 86400.0;

	PG_RETURN_TIMESTAMP(result);
}


/* timestamp_date()
 * Convert timestamp to date data type.
 */
Datum
timestamp_date(PG_FUNCTION_ARGS)
{
	Timestamp	timestamp = PG_GETARG_TIMESTAMP(0);
	DateADT		result;
	struct tm	tt,
			   *tm = &tt;
	double		fsec;

	if (TIMESTAMP_NOT_FINITE(timestamp))
		PG_RETURN_NULL();

	if (timestamp2tm(timestamp, NULL, tm, &fsec, NULL) != 0)
		elog(ERROR, "Unable to convert timestamp to date");

	result = date2j(tm->tm_year, tm->tm_mon, tm->tm_mday) - date2j(2000, 1, 1);

	PG_RETURN_DATEADT(result);
}


/* date_timestamptz()
 * Convert date to timestamp with time zone data type.
 */
Datum
date_timestamptz(PG_FUNCTION_ARGS)
{
	DateADT		dateVal = PG_GETARG_DATEADT(0);
	TimestampTz result;
	struct tm	tt,
			   *tm = &tt;
	time_t		utime;

	j2date((dateVal + date2j(2000, 1, 1)), &(tm->tm_year), &(tm->tm_mon), &(tm->tm_mday));

	if (IS_VALID_UTIME(tm->tm_year, tm->tm_mon, tm->tm_mday))
	{
#if defined(HAVE_TM_ZONE) || defined(HAVE_INT_TIMEZONE)
		tm->tm_hour = 0;
		tm->tm_min = 0;
		tm->tm_sec = 0;
		tm->tm_isdst = -1;

		tm->tm_year -= 1900;
		tm->tm_mon -= 1;
		utime = mktime(tm);
		if (utime == -1)
			elog(ERROR, "Unable to convert date to tm");

		result = utime + ((date2j(1970, 1, 1) - date2j(2000, 1, 1)) * 86400.0);
#else
		result = dateVal * 86400.0 + CTimeZone;
#endif
	}
	else
	{
		/* Outside of range for timezone support, so assume UTC */
		result = dateVal * 86400.0;
	}

	PG_RETURN_TIMESTAMP(result);
}


/* timestamptz_date()
 * Convert timestamp with time zone to date data type.
 */
Datum
timestamptz_date(PG_FUNCTION_ARGS)
{
	TimestampTz timestamp = PG_GETARG_TIMESTAMP(0);
	DateADT		result;
	struct tm	tt,
			   *tm = &tt;
	double		fsec;
	int			tz;
	char	   *tzn;

	if (TIMESTAMP_NOT_FINITE(timestamp))
		PG_RETURN_NULL();

	if (timestamp2tm(timestamp, &tz, tm, &fsec, &tzn) != 0)
		elog(ERROR, "Unable to convert timestamp to date");

	result = date2j(tm->tm_year, tm->tm_mon, tm->tm_mday) - date2j(2000, 1, 1);

	PG_RETURN_DATEADT(result);
}


/* abstime_date()
 * Convert abstime to date data type.
 */
Datum
abstime_date(PG_FUNCTION_ARGS)
{
	AbsoluteTime abstime = PG_GETARG_ABSOLUTETIME(0);
	DateADT		result;
	struct tm	tt,
			   *tm = &tt;
	int			tz;

	switch (abstime)
	{
		case INVALID_ABSTIME:
		case NOSTART_ABSTIME:
		case NOEND_ABSTIME:
			elog(ERROR, "Unable to convert reserved abstime value to date");

			/*
			 * pretend to drop through to make compiler think that result
			 * will be set
			 */

		default:
			abstime2tm(abstime, &tz, tm, NULL);
			result = date2j(tm->tm_year, tm->tm_mon, tm->tm_mday) - date2j(2000, 1, 1);
			break;
	}

	PG_RETURN_DATEADT(result);
}


/* date_text()
 * Convert date to text data type.
 */
Datum
date_text(PG_FUNCTION_ARGS)
{
	/* Input is a Date, but may as well leave it in Datum form */
	Datum		date = PG_GETARG_DATUM(0);
	text	   *result;
	char	   *str;
	int			len;

	str = DatumGetCString(DirectFunctionCall1(date_out, date));

	len = (strlen(str) + VARHDRSZ);

	result = palloc(len);

	VARATT_SIZEP(result) = len;
	memmove(VARDATA(result), str, (len - VARHDRSZ));

	pfree(str);

	PG_RETURN_TEXT_P(result);
}


/* text_date()
 * Convert text string to date.
 * Text type is not null terminated, so use temporary string
 *	then call the standard input routine.
 */
Datum
text_date(PG_FUNCTION_ARGS)
{
	text	   *str = PG_GETARG_TEXT_P(0);
	int			i;
	char	   *sp,
			   *dp,
				dstr[MAXDATELEN + 1];

	if (VARSIZE(str) - VARHDRSZ > MAXDATELEN)
		elog(ERROR, "Bad date external representation (too long)");

	sp = VARDATA(str);
	dp = dstr;
	for (i = 0; i < (VARSIZE(str) - VARHDRSZ); i++)
		*dp++ = *sp++;
	*dp = '\0';

	return DirectFunctionCall1(date_in,
							   CStringGetDatum(dstr));
}


/*****************************************************************************
 *	 Time ADT
 *****************************************************************************/

Datum
time_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);

#ifdef NOT_USED
	Oid			typelem = PG_GETARG_OID(1);
#endif
	int32		typmod = PG_GETARG_INT32(2);
	TimeADT		result;
	double		fsec;
	struct tm	tt,
			   *tm = &tt;
	int			nf;
	char		lowstr[MAXDATELEN + 1];
	char	   *field[MAXDATEFIELDS];
	int			dtype;
	int			ftype[MAXDATEFIELDS];

	if ((ParseDateTime(str, lowstr, field, ftype, MAXDATEFIELDS, &nf) != 0)
	 || (DecodeTimeOnly(field, ftype, nf, &dtype, tm, &fsec, NULL) != 0))
		elog(ERROR, "Bad time external representation '%s'", str);

	result = ((((tm->tm_hour * 60) + tm->tm_min) * 60) + tm->tm_sec + fsec);

	AdjustTimeForTypmod(&result, typmod);

	PG_RETURN_TIMEADT(result);
}

Datum
time_out(PG_FUNCTION_ARGS)
{
	TimeADT		time = PG_GETARG_TIMEADT(0);
	char	   *result;
	struct tm	tt,
			   *tm = &tt;
	double		fsec;
	double		trem;
	char		buf[MAXDATELEN + 1];

	trem = time;
	TMODULO(trem, tm->tm_hour, 3600e0);
	TMODULO(trem, tm->tm_min, 60e0);
	TMODULO(trem, tm->tm_sec, 1e0);
	fsec = trem;

	EncodeTimeOnly(tm, fsec, NULL, DateStyle, buf);

	result = pstrdup(buf);
	PG_RETURN_CSTRING(result);
}

/* time_scale()
 * Adjust time type for specified scale factor.
 * Used by PostgreSQL type system to stuff columns.
 */
Datum
time_scale(PG_FUNCTION_ARGS)
{
	TimeADT		time = PG_GETARG_TIMEADT(0);
	int32		typmod = PG_GETARG_INT32(1);
	TimeADT		result;

	result = time;
	AdjustTimeForTypmod(&result, typmod);

	PG_RETURN_TIMEADT(result);
}

static void
AdjustTimeForTypmod(TimeADT *time, int32 typmod)
{
	if ((typmod >= 0) && (typmod <= 13))
	{
		static double TimeScale = 1;
		static int32 TimeTypmod = 0;

		if (typmod != TimeTypmod)
		{
			TimeScale = pow(10.0, typmod);
			TimeTypmod = typmod;
		}

		*time = (rint(((double) *time) * TimeScale) / TimeScale);

		if (*time >= 86400)
			*time -= 86400;
	}

	return;
}


Datum
time_eq(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	PG_RETURN_BOOL(time1 == time2);
}

Datum
time_ne(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	PG_RETURN_BOOL(time1 != time2);
}

Datum
time_lt(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	PG_RETURN_BOOL(time1 < time2);
}

Datum
time_le(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	PG_RETURN_BOOL(time1 <= time2);
}

Datum
time_gt(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	PG_RETURN_BOOL(time1 > time2);
}

Datum
time_ge(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	PG_RETURN_BOOL(time1 >= time2);
}

Datum
time_cmp(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	if (time1 < time2)
		PG_RETURN_INT32(-1);
	if (time1 > time2)
		PG_RETURN_INT32(1);
	PG_RETURN_INT32(0);
}

Datum
time_larger(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	PG_RETURN_TIMEADT((time1 > time2) ? time1 : time2);
}

Datum
time_smaller(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);

	PG_RETURN_TIMEADT((time1 < time2) ? time1 : time2);
}

/* overlaps_time() --- implements the SQL92 OVERLAPS operator.
 *
 * Algorithm is per SQL92 spec.  This is much harder than you'd think
 * because the spec requires us to deliver a non-null answer in some cases
 * where some of the inputs are null.
 */
Datum
overlaps_time(PG_FUNCTION_ARGS)
{
	/*
	 * The arguments are TimeADT, but we leave them as generic Datums to
	 * avoid dereferencing nulls (TimeADT is pass-by-reference!)
	 */
	Datum		ts1 = PG_GETARG_DATUM(0);
	Datum		te1 = PG_GETARG_DATUM(1);
	Datum		ts2 = PG_GETARG_DATUM(2);
	Datum		te2 = PG_GETARG_DATUM(3);
	bool		ts1IsNull = PG_ARGISNULL(0);
	bool		te1IsNull = PG_ARGISNULL(1);
	bool		ts2IsNull = PG_ARGISNULL(2);
	bool		te2IsNull = PG_ARGISNULL(3);

#define TIMEADT_GT(t1,t2) \
	(DatumGetTimeADT(t1) > DatumGetTimeADT(t2))
#define TIMEADT_LT(t1,t2) \
	(DatumGetTimeADT(t1) < DatumGetTimeADT(t2))

	/*
	 * If both endpoints of interval 1 are null, the result is null
	 * (unknown). If just one endpoint is null, take ts1 as the non-null
	 * one. Otherwise, take ts1 as the lesser endpoint.
	 */
	if (ts1IsNull)
	{
		if (te1IsNull)
			PG_RETURN_NULL();
		/* swap null for non-null */
		ts1 = te1;
		te1IsNull = true;
	}
	else if (!te1IsNull)
	{
		if (TIMEADT_GT(ts1, te1))
		{
			Datum		tt = ts1;

			ts1 = te1;
			te1 = tt;
		}
	}

	/* Likewise for interval 2. */
	if (ts2IsNull)
	{
		if (te2IsNull)
			PG_RETURN_NULL();
		/* swap null for non-null */
		ts2 = te2;
		te2IsNull = true;
	}
	else if (!te2IsNull)
	{
		if (TIMEADT_GT(ts2, te2))
		{
			Datum		tt = ts2;

			ts2 = te2;
			te2 = tt;
		}
	}

	/*
	 * At this point neither ts1 nor ts2 is null, so we can consider three
	 * cases: ts1 > ts2, ts1 < ts2, ts1 = ts2
	 */
	if (TIMEADT_GT(ts1, ts2))
	{
		/*
		 * This case is ts1 < te2 OR te1 < te2, which may look redundant
		 * but in the presence of nulls it's not quite completely so.
		 */
		if (te2IsNull)
			PG_RETURN_NULL();
		if (TIMEADT_LT(ts1, te2))
			PG_RETURN_BOOL(true);
		if (te1IsNull)
			PG_RETURN_NULL();

		/*
		 * If te1 is not null then we had ts1 <= te1 above, and we just
		 * found ts1 >= te2, hence te1 >= te2.
		 */
		PG_RETURN_BOOL(false);
	}
	else if (TIMEADT_LT(ts1, ts2))
	{
		/* This case is ts2 < te1 OR te2 < te1 */
		if (te1IsNull)
			PG_RETURN_NULL();
		if (TIMEADT_LT(ts2, te1))
			PG_RETURN_BOOL(true);
		if (te2IsNull)
			PG_RETURN_NULL();

		/*
		 * If te2 is not null then we had ts2 <= te2 above, and we just
		 * found ts2 >= te1, hence te2 >= te1.
		 */
		PG_RETURN_BOOL(false);
	}
	else
	{
		/*
		 * For ts1 = ts2 the spec says te1 <> te2 OR te1 = te2, which is a
		 * rather silly way of saying "true if both are nonnull, else
		 * null".
		 */
		if (te1IsNull || te2IsNull)
			PG_RETURN_NULL();
		PG_RETURN_BOOL(true);
	}

#undef TIMEADT_GT
#undef TIMEADT_LT
}

/* timestamp_time()
 * Convert timestamp to time data type.
 */
Datum
timestamp_time(PG_FUNCTION_ARGS)
{
	Timestamp	timestamp = PG_GETARG_TIMESTAMP(0);
	TimeADT		result;
	struct tm	tt,
			   *tm = &tt;
	double		fsec;

	if (TIMESTAMP_NOT_FINITE(timestamp))
		PG_RETURN_NULL();

	if (timestamp2tm(timestamp, NULL, tm, &fsec, NULL) != 0)
		elog(ERROR, "Unable to convert timestamp to date");

	result = ((((tm->tm_hour * 60) + tm->tm_min) * 60) + tm->tm_sec + fsec);

	PG_RETURN_TIMEADT(result);
}

/* datetime_timestamp()
 * Convert date and time to timestamp data type.
 */
Datum
datetime_timestamp(PG_FUNCTION_ARGS)
{
	DateADT		date = PG_GETARG_DATEADT(0);
	TimeADT		time = PG_GETARG_TIMEADT(1);
	Timestamp	result;

	result = DatumGetTimestamp(DirectFunctionCall1(date_timestamp,
												 DateADTGetDatum(date)));
	result += time;

	PG_RETURN_TIMESTAMP(result);
}

/* time_interval()
 * Convert time to interval data type.
 */
Datum
time_interval(PG_FUNCTION_ARGS)
{
	TimeADT		time = PG_GETARG_TIMEADT(0);
	Interval   *result;

	result = (Interval *) palloc(sizeof(Interval));

	result->time = time;
	result->month = 0;

	PG_RETURN_INTERVAL_P(result);
}

/* interval_time()
 * Convert interval to time data type.
 */
Datum
interval_time(PG_FUNCTION_ARGS)
{
	Interval   *span = PG_GETARG_INTERVAL_P(0);
	TimeADT		result;
	Interval	span1;

	result = span->time;
	TMODULO(result, span1.time, 86400e0);

	PG_RETURN_TIMEADT(result);
}

/* time_mi_time()
 * Subtract two times to produce an interval.
 */
Datum
time_mi_time(PG_FUNCTION_ARGS)
{
	TimeADT		time1 = PG_GETARG_TIMEADT(0);
	TimeADT		time2 = PG_GETARG_TIMEADT(1);
	Interval   *result;

	result = (Interval *) palloc(sizeof(Interval));

	result->time = time1 - time2;
	result->month = 0;

	PG_RETURN_INTERVAL_P(result);
}

/* time_pl_interval()
 * Add interval to time.
 */
Datum
time_pl_interval(PG_FUNCTION_ARGS)
{
	TimeADT		time = PG_GETARG_TIMEADT(0);
	Interval   *span = PG_GETARG_INTERVAL_P(1);
	TimeADT		result;
	TimeADT		time1;

	result = (time + span->time);
	TMODULO(result, time1, 86400e0);
	if (result < 0)
		result += 86400;

	PG_RETURN_TIMEADT(result);
}

/* time_mi_interval()
 * Subtract interval from time.
 */
Datum
time_mi_interval(PG_FUNCTION_ARGS)
{
	TimeADT		time = PG_GETARG_TIMEADT(0);
	Interval   *span = PG_GETARG_INTERVAL_P(1);
	TimeADT		result;
	TimeADT		time1;

	result = (time - span->time);
	TMODULO(result, time1, 86400e0);
	if (result < 0)
		result += 86400;

	PG_RETURN_TIMEADT(result);
}

/* interval_pl_time()
 * Add time to interval.
 */
Datum
interval_pl_time(PG_FUNCTION_ARGS)
{
	Datum		span = PG_GETARG_DATUM(0);
	Datum		time = PG_GETARG_DATUM(1);

	return DirectFunctionCall2(time_pl_interval, time, span);
}


/* time_text()
 * Convert time to text data type.
 */
Datum
time_text(PG_FUNCTION_ARGS)
{
	/* Input is a Time, but may as well leave it in Datum form */
	Datum		time = PG_GETARG_DATUM(0);
	text	   *result;
	char	   *str;
	int			len;

	str = DatumGetCString(DirectFunctionCall1(time_out, time));

	len = (strlen(str) + VARHDRSZ);

	result = palloc(len);

	VARATT_SIZEP(result) = len;
	memmove(VARDATA(result), str, (len - VARHDRSZ));

	pfree(str);

	PG_RETURN_TEXT_P(result);
}


/* text_time()
 * Convert text string to time.
 * Text type is not null terminated, so use temporary string
 *	then call the standard input routine.
 */
Datum
text_time(PG_FUNCTION_ARGS)
{
	text	   *str = PG_GETARG_TEXT_P(0);
	int			i;
	char	   *sp,
			   *dp,
				dstr[MAXDATELEN + 1];

	if (VARSIZE(str) - VARHDRSZ > MAXDATELEN)
		elog(ERROR, "Bad time external representation (too long)");

	sp = VARDATA(str);
	dp = dstr;
	for (i = 0; i < (VARSIZE(str) - VARHDRSZ); i++)
		*dp++ = *sp++;
	*dp = '\0';

	return DirectFunctionCall3(time_in,
							   CStringGetDatum(dstr),
							   ObjectIdGetDatum(InvalidOid),
							   Int32GetDatum(-1));
}


/*****************************************************************************
 *	 Time With Time Zone ADT
 *****************************************************************************/

Datum
timetz_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);

#ifdef NOT_USED
	Oid			typelem = PG_GETARG_OID(1);
#endif
	int32		typmod = PG_GETARG_INT32(2);
	TimeTzADT  *result;
	double		fsec;
	struct tm	tt,
			   *tm = &tt;
	int			tz;
	int			nf;
	char		lowstr[MAXDATELEN + 1];
	char	   *field[MAXDATEFIELDS];
	int			dtype;
	int			ftype[MAXDATEFIELDS];

	if ((ParseDateTime(str, lowstr, field, ftype, MAXDATEFIELDS, &nf) != 0)
	  || (DecodeTimeOnly(field, ftype, nf, &dtype, tm, &fsec, &tz) != 0))
		elog(ERROR, "Bad time external representation '%s'", str);

	result = (TimeTzADT *) palloc(sizeof(TimeTzADT));

	result->time = ((((tm->tm_hour * 60) + tm->tm_min) * 60) + tm->tm_sec + fsec);
	result->zone = tz;

	AdjustTimeForTypmod(&(result->time), typmod);

	PG_RETURN_TIMETZADT_P(result);
}

Datum
timetz_out(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time = PG_GETARG_TIMETZADT_P(0);
	char	   *result;
	struct tm	tt,
			   *tm = &tt;
	double		fsec;
	int			tz;
	double		trem;
	char		buf[MAXDATELEN + 1];

	trem = time->time;
	TMODULO(trem, tm->tm_hour, 3600e0);
	TMODULO(trem, tm->tm_min, 60e0);
	TMODULO(trem, tm->tm_sec, 1e0);
	fsec = trem;

	tz = time->zone;

	EncodeTimeOnly(tm, fsec, &tz, DateStyle, buf);

	result = pstrdup(buf);
	PG_RETURN_CSTRING(result);
}

/* timetz_scale()
 * Adjust time type for specified scale factor.
 * Used by PostgreSQL type system to stuff columns.
 */
Datum
timetz_scale(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time = PG_GETARG_TIMETZADT_P(0);
	int32		typmod = PG_GETARG_INT32(1);
	TimeTzADT  *result;

	result = (TimeTzADT *) palloc(sizeof(TimeTzADT));

	result->time = time->time;
	result->zone = time->zone;

	AdjustTimeForTypmod(&(result->time), typmod);

	PG_RETURN_TIMETZADT_P(result);
}


static int
timetz_cmp_internal(TimeTzADT *time1, TimeTzADT *time2)
{
	double		t1,
				t2;

	/* Primary sort is by true (GMT-equivalent) time */
	t1 = time1->time + time1->zone;
	t2 = time2->time + time2->zone;

	if (t1 > t2)
		return 1;
	if (t1 < t2)
		return -1;

	/*
	 * If same GMT time, sort by timezone; we only want to say that two
	 * timetz's are equal if both the time and zone parts are equal.
	 */
	if (time1->zone > time2->zone)
		return 1;
	if (time1->zone < time2->zone)
		return -1;

	return 0;
}

Datum
timetz_eq(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	PG_RETURN_BOOL(timetz_cmp_internal(time1, time2) == 0);
}

Datum
timetz_ne(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	PG_RETURN_BOOL(timetz_cmp_internal(time1, time2) != 0);
}

Datum
timetz_lt(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	PG_RETURN_BOOL(timetz_cmp_internal(time1, time2) < 0);
}

Datum
timetz_le(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	PG_RETURN_BOOL(timetz_cmp_internal(time1, time2) <= 0);
}

Datum
timetz_gt(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	PG_RETURN_BOOL(timetz_cmp_internal(time1, time2) > 0);
}

Datum
timetz_ge(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	PG_RETURN_BOOL(timetz_cmp_internal(time1, time2) >= 0);
}

Datum
timetz_cmp(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	PG_RETURN_INT32(timetz_cmp_internal(time1, time2));
}

/*
 * timetz, being an unusual size, needs a specialized hash function.
 */
Datum
timetz_hash(PG_FUNCTION_ARGS)
{
	TimeTzADT  *key = PG_GETARG_TIMETZADT_P(0);

	/*
	 * Specify hash length as sizeof(double) + sizeof(int4), not as
	 * sizeof(TimeTzADT), so that any garbage pad bytes in the structure
	 * won't be included in the hash!
	 */
	return hash_any((char *) key, sizeof(double) + sizeof(int4));
}

Datum
timetz_larger(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	if (DatumGetBool(DirectFunctionCall2(timetz_gt,
										 TimeTzADTPGetDatum(time1),
										 TimeTzADTPGetDatum(time2))))
		PG_RETURN_TIMETZADT_P(time1);
	PG_RETURN_TIMETZADT_P(time2);
}

Datum
timetz_smaller(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time1 = PG_GETARG_TIMETZADT_P(0);
	TimeTzADT  *time2 = PG_GETARG_TIMETZADT_P(1);

	if (DatumGetBool(DirectFunctionCall2(timetz_lt,
										 TimeTzADTPGetDatum(time1),
										 TimeTzADTPGetDatum(time2))))
		PG_RETURN_TIMETZADT_P(time1);
	PG_RETURN_TIMETZADT_P(time2);
}

/* timetz_pl_interval()
 * Add interval to timetz.
 */
Datum
timetz_pl_interval(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time = PG_GETARG_TIMETZADT_P(0);
	Interval   *span = PG_GETARG_INTERVAL_P(1);
	TimeTzADT  *result;
	TimeTzADT	time1;

	result = (TimeTzADT *) palloc(sizeof(TimeTzADT));

	result->time = (time->time + span->time);
	TMODULO(result->time, time1.time, 86400e0);
	if (result->time < 0)
		result->time += 86400;
	result->zone = time->zone;

	PG_RETURN_TIMETZADT_P(result);
}

/* timetz_mi_interval()
 * Subtract interval from timetz.
 */
Datum
timetz_mi_interval(PG_FUNCTION_ARGS)
{
	TimeTzADT  *time = PG_GETARG_TIMETZADT_P(0);
	Interval   *span = PG_GETARG_INTERVAL_P(1);
	TimeTzADT  *result;
	TimeTzADT	time1;

	result = (TimeTzADT *) palloc(sizeof(TimeTzADT));

	result->time = (time->time - span->time);
	TMODULO(result->time, time1.time, 86400e0);
	if (result->time < 0)
		result->time += 86400;
	result->zone = time->zone;

	PG_RETURN_TIMETZADT_P(result);
}

/* overlaps_timetz() --- implements the SQL92 OVERLAPS operator.
 *
 * Algorithm is per SQL92 spec.  This is much harder than you'd think
 * because the spec requires us to deliver a non-null answer in some cases
 * where some of the inputs are null.
 */
Datum
overlaps_timetz(PG_FUNCTION_ARGS)
{
	/*
	 * The arguments are TimeTzADT *, but we leave them as generic Datums
	 * for convenience of notation --- and to avoid dereferencing nulls.
	 */
	Datum		ts1 = PG_GETARG_DATUM(0);
	Datum		te1 = PG_GETARG_DATUM(1);
	Datum		ts2 = PG_GETARG_DATUM(2);
	Datum		te2 = PG_GETARG_DATUM(3);
	bool		ts1IsNull = PG_ARGISNULL(0);
	bool		te1IsNull = PG_ARGISNULL(1);
	bool		ts2IsNull = PG_ARGISNULL(2);
	bool		te2IsNull = PG_ARGISNULL(3);

#define TIMETZ_GT(t1,t2) \
	DatumGetBool(DirectFunctionCall2(timetz_gt,t1,t2))
#define TIMETZ_LT(t1,t2) \
	DatumGetBool(DirectFunctionCall2(timetz_lt,t1,t2))

	/*
	 * If both endpoints of interval 1 are null, the result is null
	 * (unknown). If just one endpoint is null, take ts1 as the non-null
	 * one. Otherwise, take ts1 as the lesser endpoint.
	 */
	if (ts1IsNull)
	{
		if (te1IsNull)
			PG_RETURN_NULL();
		/* swap null for non-null */
		ts1 = te1;
		te1IsNull = true;
	}
	else if (!te1IsNull)
	{
		if (TIMETZ_GT(ts1, te1))
		{
			Datum		tt = ts1;

			ts1 = te1;
			te1 = tt;
		}
	}

	/* Likewise for interval 2. */
	if (ts2IsNull)
	{
		if (te2IsNull)
			PG_RETURN_NULL();
		/* swap null for non-null */
		ts2 = te2;
		te2IsNull = true;
	}
	else if (!te2IsNull)
	{
		if (TIMETZ_GT(ts2, te2))
		{
			Datum		tt = ts2;

			ts2 = te2;
			te2 = tt;
		}
	}

	/*
	 * At this point neither ts1 nor ts2 is null, so we can consider three
	 * cases: ts1 > ts2, ts1 < ts2, ts1 = ts2
	 */
	if (TIMETZ_GT(ts1, ts2))
	{
		/*
		 * This case is ts1 < te2 OR te1 < te2, which may look redundant
		 * but in the presence of nulls it's not quite completely so.
		 */
		if (te2IsNull)
			PG_RETURN_NULL();
		if (TIMETZ_LT(ts1, te2))
			PG_RETURN_BOOL(true);
		if (te1IsNull)
			PG_RETURN_NULL();

		/*
		 * If te1 is not null then we had ts1 <= te1 above, and we just
		 * found ts1 >= te2, hence te1 >= te2.
		 */
		PG_RETURN_BOOL(false);
	}
	else if (TIMETZ_LT(ts1, ts2))
	{
		/* This case is ts2 < te1 OR te2 < te1 */
		if (te1IsNull)
			PG_RETURN_NULL();
		if (TIMETZ_LT(ts2, te1))
			PG_RETURN_BOOL(true);
		if (te2IsNull)
			PG_RETURN_NULL();

		/*
		 * If te2 is not null then we had ts2 <= te2 above, and we just
		 * found ts2 >= te1, hence te2 >= te1.
		 */
		PG_RETURN_BOOL(false);
	}
	else
	{
		/*
		 * For ts1 = ts2 the spec says te1 <> te2 OR te1 = te2, which is a
		 * rather silly way of saying "true if both are nonnull, else
		 * null".
		 */
		if (te1IsNull || te2IsNull)
			PG_RETURN_NULL();
		PG_RETURN_BOOL(true);
	}

#undef TIMETZ_GT
#undef TIMETZ_LT
}


Datum
timetz_time(PG_FUNCTION_ARGS)
{
	TimeTzADT  *timetz = PG_GETARG_TIMETZADT_P(0);
	TimeADT		result;

	/* swallow the time zone and just return the time */
	result = timetz->time;

	PG_RETURN_TIMEADT(result);
}


Datum
time_timetz(PG_FUNCTION_ARGS)
{
	TimeADT		time = PG_GETARG_TIMEADT(0);
	TimeTzADT  *result;
	struct tm	tt,
			   *tm = &tt;
	int			tz;

	GetCurrentTime(tm);
	tz = DetermineLocalTimeZone(tm);

	result = (TimeTzADT *) palloc(sizeof(TimeTzADT));

	result->time = time;
	result->zone = tz;

	PG_RETURN_TIMETZADT_P(result);
}


/* timestamptz_timetz()
 * Convert timestamp to timetz data type.
 */
Datum
timestamptz_timetz(PG_FUNCTION_ARGS)
{
	TimestampTz timestamp = PG_GETARG_TIMESTAMP(0);
	TimeTzADT  *result;
	struct tm	tt,
			   *tm = &tt;
	int			tz;
	double		fsec;
	char	   *tzn;

	if (TIMESTAMP_NOT_FINITE(timestamp))
		PG_RETURN_NULL();

	if (timestamp2tm(timestamp, &tz, tm, &fsec, &tzn) != 0)
		elog(ERROR, "Unable to convert timestamp to date");

	result = (TimeTzADT *) palloc(sizeof(TimeTzADT));

	result->time = ((((tm->tm_hour * 60) + tm->tm_min) * 60) + tm->tm_sec + fsec);
	result->zone = tz;

	PG_RETURN_TIMETZADT_P(result);
}


/* datetimetz_timestamptz()
 * Convert date and timetz to timestamp with time zone data type.
 * Timestamp is stored in GMT, so add the time zone
 * stored with the timetz to the result.
 * - thomas 2000-03-10
 */
Datum
datetimetz_timestamptz(PG_FUNCTION_ARGS)
{
	DateADT		date = PG_GETARG_DATEADT(0);
	TimeTzADT  *time = PG_GETARG_TIMETZADT_P(1);
	TimestampTz result;

	result = date * 86400.0 + time->time + time->zone;

	PG_RETURN_TIMESTAMP(result);
}


/* timetz_text()
 * Convert timetz to text data type.
 */
Datum
timetz_text(PG_FUNCTION_ARGS)
{
	/* Input is a Timetz, but may as well leave it in Datum form */
	Datum		timetz = PG_GETARG_DATUM(0);
	text	   *result;
	char	   *str;
	int			len;

	str = DatumGetCString(DirectFunctionCall1(timetz_out, timetz));

	len = (strlen(str) + VARHDRSZ);

	result = palloc(len);

	VARATT_SIZEP(result) = len;
	memmove(VARDATA(result), str, (len - VARHDRSZ));

	pfree(str);

	PG_RETURN_TEXT_P(result);
}


/* text_timetz()
 * Convert text string to timetz.
 * Text type is not null terminated, so use temporary string
 *	then call the standard input routine.
 */
Datum
text_timetz(PG_FUNCTION_ARGS)
{
	text	   *str = PG_GETARG_TEXT_P(0);
	int			i;
	char	   *sp,
			   *dp,
				dstr[MAXDATELEN + 1];

	if (VARSIZE(str) - VARHDRSZ > MAXDATELEN)
		elog(ERROR, "Bad timetz external representation (too long)");

	sp = VARDATA(str);
	dp = dstr;
	for (i = 0; i < (VARSIZE(str) - VARHDRSZ); i++)
		*dp++ = *sp++;
	*dp = '\0';

	return DirectFunctionCall3(timetz_in,
							   CStringGetDatum(dstr),
							   ObjectIdGetDatum(InvalidOid),
							   Int32GetDatum(-1));
}

/* timetz_part()
 * Extract specified field from time type.
 */
Datum
timetz_part(PG_FUNCTION_ARGS)
{
	text	   *units = PG_GETARG_TEXT_P(0);
	TimeTzADT  *time = PG_GETARG_TIMETZADT_P(1);
	float8		result;
	int			type,
				val;
	int			i;
	char	   *up,
			   *lp,
				lowunits[MAXDATELEN + 1];

	if (VARSIZE(units) - VARHDRSZ > MAXDATELEN)
		elog(ERROR, "TIMETZ units '%s' not recognized",
			 DatumGetCString(DirectFunctionCall1(textout,
											   PointerGetDatum(units))));
	up = VARDATA(units);
	lp = lowunits;
	for (i = 0; i < (VARSIZE(units) - VARHDRSZ); i++)
		*lp++ = tolower((unsigned char) *up++);
	*lp = '\0';

	type = DecodeUnits(0, lowunits, &val);
	if (type == UNKNOWN_FIELD)
		type = DecodeSpecial(0, lowunits, &val);

	if (type == UNITS)
	{
		double		trem;
		double		dummy;
		int			tz;
		double		fsec;
		struct tm	tt,
				   *tm = &tt;

		trem = time->time;
		TMODULO(trem, tm->tm_hour, 3600e0);
		TMODULO(trem, tm->tm_min, 60e0);
		TMODULO(trem, tm->tm_sec, 1e0);
		fsec = trem;
		tz = time->zone;

		switch (val)
		{
			case DTK_TZ:
				result = tz;
				break;

			case DTK_TZ_MINUTE:
				result = tz / 60;
				TMODULO(result, dummy, 60e0);
				break;

			case DTK_TZ_HOUR:
				dummy = tz;
				TMODULO(dummy, result, 3600e0);
				break;

			case DTK_MICROSEC:
				result = ((tm->tm_sec + fsec) * 1000000);
				break;

			case DTK_MILLISEC:
				result = ((tm->tm_sec + fsec) * 1000);
				break;

			case DTK_SECOND:
				result = (tm->tm_sec + fsec);
				break;

			case DTK_MINUTE:
				result = tm->tm_min;
				break;

			case DTK_HOUR:
				result = tm->tm_hour;
				break;

			case DTK_DAY:
			case DTK_MONTH:
			case DTK_QUARTER:
			case DTK_YEAR:
			case DTK_DECADE:
			case DTK_CENTURY:
			case DTK_MILLENNIUM:
			default:
				elog(ERROR, "TIMETZ units '%s' not supported",
					 DatumGetCString(DirectFunctionCall1(textout,
											   PointerGetDatum(units))));
				result = 0;
		}
	}
	else if ((type == RESERV) && (val == DTK_EPOCH))
		result = time->time - time->zone;
	else
	{
		elog(ERROR, "TIMETZ units '%s' not recognized",
			 DatumGetCString(DirectFunctionCall1(textout,
											   PointerGetDatum(units))));
		result = 0;
	}

	PG_RETURN_FLOAT8(result);
}

/* timetz_zone()
 * Encode time with time zone type with specified time zone.
 */
Datum
timetz_zone(PG_FUNCTION_ARGS)
{
	text	   *zone = PG_GETARG_TEXT_P(0);
	TimeTzADT  *time = PG_GETARG_TIMETZADT_P(1);
	TimeTzADT  *result;
	TimeADT		time1;
	int			tz;
	int			type,
				val;
	int			i;
	char	   *up,
			   *lp,
				lowzone[MAXDATELEN + 1];

	if (VARSIZE(zone) - VARHDRSZ > MAXDATELEN)
		elog(ERROR, "Time zone '%s' not recognized",
			 DatumGetCString(DirectFunctionCall1(textout,
												 PointerGetDatum(zone))));
	up = VARDATA(zone);
	lp = lowzone;
	for (i = 0; i < (VARSIZE(zone) - VARHDRSZ); i++)
		*lp++ = tolower((unsigned char) *up++);
	*lp = '\0';

	type = DecodeSpecial(0, lowzone, &val);

	result = (TimeTzADT *) palloc(sizeof(TimeTzADT));

	if ((type == TZ) || (type == DTZ))
	{
		tz = val * 60;
		time1 = time->time - time->zone + tz;
		TMODULO(result->time, time1, 86400e0);
		if (result->time < 0)
			result->time += 86400;
		result->zone = tz;
	}
	else
	{
		elog(ERROR, "Time zone '%s' not recognized", lowzone);
		PG_RETURN_NULL();
	}

	PG_RETURN_TIMETZADT_P(result);
}	/* timetz_zone() */

/* timetz_izone()
 * Encode time with time zone type with specified time interval as time zone.
 */
Datum
timetz_izone(PG_FUNCTION_ARGS)
{
	Interval   *zone = PG_GETARG_INTERVAL_P(0);
	TimeTzADT  *time = PG_GETARG_TIMETZADT_P(1);
	TimeTzADT  *result;
	int			tz;

	if (zone->month != 0)
		elog(ERROR, "INTERVAL time zone '%s' not legal (month specified)",
			 DatumGetCString(DirectFunctionCall1(interval_out,
												 PointerGetDatum(zone))));

	tz = -(zone->time);

	result = (TimeTzADT *) palloc(sizeof(TimeTzADT));

	result->time = (time->time + (time->zone - tz));
	while (result->time < 0)
		result->time += 86400;
	while (result->time >= 86400)
		result->time -= 86400;

	result->zone = tz;

	PG_RETURN_TIMETZADT_P(result);
}	/* timetz_izone() */
