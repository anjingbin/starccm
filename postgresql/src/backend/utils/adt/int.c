/*-------------------------------------------------------------------------
 *
 * int.c
 *	  Functions for the built-in integer types (except int8).
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/adt/int.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * OLD COMMENTS
 *		I/O routines:
 *		 int2in, int2out, int2vectorin, int2vectorout, int4in, int4out
 *		Conversion routines:
 *		 itoi, int2_text, int4_text
 *		Boolean operators:
 *		 inteq, intne, intlt, intle, intgt, intge
 *		Arithmetic operators:
 *		 intpl, intmi, int4mul, intdiv
 *
 *		Arithmetic operators:
 *		 intmod, int4fac
 */

#include "postgres.h"

#include <ctype.h>
#include <limits.h>

#include "utils/builtins.h"

#ifndef SHRT_MAX
#define SHRT_MAX (0x7FFF)
#endif
#ifndef SHRT_MIN
#define SHRT_MIN (-0x8000)
#endif

/*****************************************************************************
 *	 USER I/O ROUTINES														 *
 *****************************************************************************/

/*
 *		int2in			- converts "num" to short
 */
Datum
int2in(PG_FUNCTION_ARGS)
{
	char	   *num = PG_GETARG_CSTRING(0);

	PG_RETURN_INT16(pg_atoi(num, sizeof(int16), '\0'));
}

/*
 *		int2out			- converts short to "num"
 */
Datum
int2out(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	char	   *result = (char *) palloc(7);	/* sign, 5 digits, '\0' */

	pg_itoa(arg1, result);
	PG_RETURN_CSTRING(result);
}

/*
 *		int2vectorin			- converts "num num ..." to internal form
 *
 *		Note: Fills any missing slots with zeroes.
 */
Datum
int2vectorin(PG_FUNCTION_ARGS)
{
	char	   *intString = PG_GETARG_CSTRING(0);
	int16	   *result = (int16 *) palloc(sizeof(int16[INDEX_MAX_KEYS]));
	int			slot;

	for (slot = 0; *intString && slot < INDEX_MAX_KEYS; slot++)
	{
		if (sscanf(intString, "%hd", &result[slot]) != 1)
			break;
		while (*intString && isspace((unsigned char) *intString))
			intString++;
		while (*intString && !isspace((unsigned char) *intString))
			intString++;
	}
	while (*intString && isspace((unsigned char) *intString))
		intString++;
	if (*intString)
		elog(ERROR, "int2vector value has too many values");
	while (slot < INDEX_MAX_KEYS)
		result[slot++] = 0;

	PG_RETURN_POINTER(result);
}

/*
 *		int2vectorout		- converts internal form to "num num ..."
 */
Datum
int2vectorout(PG_FUNCTION_ARGS)
{
	int16	   *int2Array = (int16 *) PG_GETARG_POINTER(0);
	int			num,
				maxnum;
	char	   *rp;
	char	   *result;

	/* find last non-zero value in vector */
	for (maxnum = INDEX_MAX_KEYS - 1; maxnum >= 0; maxnum--)
		if (int2Array[maxnum] != 0)
			break;

	/* assumes sign, 5 digits, ' ' */
	rp = result = (char *) palloc((maxnum + 1) * 7 + 1);
	for (num = 0; num <= maxnum; num++)
	{
		if (num != 0)
			*rp++ = ' ';
		pg_itoa(int2Array[num], rp);
		while (*++rp != '\0')
			;
	}
	*rp = '\0';
	PG_RETURN_CSTRING(result);
}

/*
 * We don't have a complete set of int2vector support routines,
 * but we need int2vectoreq for catcache indexing.
 */
Datum
int2vectoreq(PG_FUNCTION_ARGS)
{
	int16	   *arg1 = (int16 *) PG_GETARG_POINTER(0);
	int16	   *arg2 = (int16 *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(memcmp(arg1, arg2, INDEX_MAX_KEYS * sizeof(int16)) == 0);
}

/*
 * Type int44 has no real-world use, but the regression tests use it.
 * It's a four-element vector of int4's.
 */

/*
 *		int44in			- converts "num num ..." to internal form
 *
 *		Note: Fills any missing positions with zeroes.
 */
Datum
int44in(PG_FUNCTION_ARGS)
{
	char	   *input_string = PG_GETARG_CSTRING(0);
	int32	   *result = (int32 *) palloc(4 * sizeof(int32));
	int			i;

	i = sscanf(input_string,
			   "%d, %d, %d, %d",
			   &result[0],
			   &result[1],
			   &result[2],
			   &result[3]);
	while (i < 4)
		result[i++] = 0;

	PG_RETURN_POINTER(result);
}

/*
 *		int44out		- converts internal form to "num num ..."
 */
Datum
int44out(PG_FUNCTION_ARGS)
{
	int32	   *an_array = (int32 *) PG_GETARG_POINTER(0);
	char	   *result = (char *) palloc(16 * 4);		/* Allow 14 digits +
														 * sign */
	int			i;
	char	   *walk;

	walk = result;
	for (i = 0; i < 4; i++)
	{
		pg_ltoa(an_array[i], walk);
		while (*++walk != '\0')
			;
		*walk++ = ' ';
	}
	*--walk = '\0';
	PG_RETURN_CSTRING(result);
}


/*****************************************************************************
 *	 PUBLIC ROUTINES														 *
 *****************************************************************************/

/*
 *		int4in			- converts "num" to int4
 */
Datum
int4in(PG_FUNCTION_ARGS)
{
	char	   *num = PG_GETARG_CSTRING(0);

	PG_RETURN_INT32(pg_atoi(num, sizeof(int32), '\0'));
}

/*
 *		int4out			- converts int4 to "num"
 */
Datum
int4out(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	char	   *result = (char *) palloc(12);	/* sign, 10 digits, '\0' */

	pg_ltoa(arg1, result);
	PG_RETURN_CSTRING(result);
}


/*
 *		===================
 *		CONVERSION ROUTINES
 *		===================
 */

Datum
i2toi4(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);

	PG_RETURN_INT32((int32) arg1);
}

Datum
i4toi2(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);

	if (arg1 < SHRT_MIN)
		elog(ERROR, "i4toi2: '%d' causes int2 underflow", arg1);
	if (arg1 > SHRT_MAX)
		elog(ERROR, "i4toi2: '%d' causes int2 overflow", arg1);

	PG_RETURN_INT16((int16) arg1);
}

Datum
int2_text(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	text	   *result = (text *) palloc(7 + VARHDRSZ); /* sign,5 digits, '\0' */

	pg_itoa(arg1, VARDATA(result));
	VARATT_SIZEP(result) = strlen(VARDATA(result)) + VARHDRSZ;
	PG_RETURN_TEXT_P(result);
}

Datum
text_int2(PG_FUNCTION_ARGS)
{
	text	   *string = PG_GETARG_TEXT_P(0);
	Datum		result;
	int			len;
	char	   *str;

	len = VARSIZE(string) - VARHDRSZ;

	str = palloc(len + 1);
	memcpy(str, VARDATA(string), len);
	*(str + len) = '\0';

	result = DirectFunctionCall1(int2in, CStringGetDatum(str));
	pfree(str);

	return result;
}

Datum
int4_text(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	text	   *result = (text *) palloc(12 + VARHDRSZ);		/* sign,10 digits,'\0' */

	pg_ltoa(arg1, VARDATA(result));
	VARATT_SIZEP(result) = strlen(VARDATA(result)) + VARHDRSZ;
	PG_RETURN_TEXT_P(result);
}

Datum
text_int4(PG_FUNCTION_ARGS)
{
	text	   *string = PG_GETARG_TEXT_P(0);
	Datum		result;
	int			len;
	char	   *str;

	len = VARSIZE(string) - VARHDRSZ;

	str = palloc(len + 1);
	memcpy(str, VARDATA(string), len);
	*(str + len) = '\0';

	result = DirectFunctionCall1(int4in, CStringGetDatum(str));
	pfree(str);

	return result;
}


/*
 *		============================
 *		COMPARISON OPERATOR ROUTINES
 *		============================
 */

/*
 *		inteq			- returns 1 iff arg1 == arg2
 *		intne			- returns 1 iff arg1 != arg2
 *		intlt			- returns 1 iff arg1 < arg2
 *		intle			- returns 1 iff arg1 <= arg2
 *		intgt			- returns 1 iff arg1 > arg2
 *		intge			- returns 1 iff arg1 >= arg2
 */

Datum
int4eq(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 == arg2);
}

Datum
int4ne(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 != arg2);
}

Datum
int4lt(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 < arg2);
}

Datum
int4le(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 <= arg2);
}

Datum
int4gt(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 > arg2);
}

Datum
int4ge(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 >= arg2);
}

Datum
int2eq(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 == arg2);
}

Datum
int2ne(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 != arg2);
}

Datum
int2lt(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 < arg2);
}

Datum
int2le(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 <= arg2);
}

Datum
int2gt(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 > arg2);
}

Datum
int2ge(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 >= arg2);
}

Datum
int24eq(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 == arg2);
}

Datum
int24ne(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 != arg2);
}

Datum
int24lt(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 < arg2);
}

Datum
int24le(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 <= arg2);
}

Datum
int24gt(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 > arg2);
}

Datum
int24ge(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_BOOL(arg1 >= arg2);
}

Datum
int42eq(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 == arg2);
}

Datum
int42ne(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 != arg2);
}

Datum
int42lt(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 < arg2);
}

Datum
int42le(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 <= arg2);
}

Datum
int42gt(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 > arg2);
}

Datum
int42ge(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_BOOL(arg1 >= arg2);
}

/*
 *		int[24]pl		- returns arg1 + arg2
 *		int[24]mi		- returns arg1 - arg2
 *		int[24]mul		- returns arg1 * arg2
 *		int[24]div		- returns arg1 / arg2
 */

Datum
int4um(PG_FUNCTION_ARGS)
{
	int32		arg = PG_GETARG_INT32(0);

	PG_RETURN_INT32(-arg);
}

Datum
int4up(PG_FUNCTION_ARGS)
{
	int32		arg = PG_GETARG_INT32(0);

	PG_RETURN_INT32(arg);
}

Datum
int4pl(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 + arg2);
}

Datum
int4mi(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 - arg2);
}

Datum
int4mul(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 * arg2);
}

Datum
int4div(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 / arg2);
}

Datum
int4inc(PG_FUNCTION_ARGS)
{
	int32		arg = PG_GETARG_INT32(0);

	PG_RETURN_INT32(arg + 1);
}

Datum
int2um(PG_FUNCTION_ARGS)
{
	int16		arg = PG_GETARG_INT16(0);

	PG_RETURN_INT16(-arg);
}

Datum
int2up(PG_FUNCTION_ARGS)
{
	int16		arg = PG_GETARG_INT16(0);

	PG_RETURN_INT16(arg);
}

Datum
int2pl(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16(arg1 + arg2);
}

Datum
int2mi(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16(arg1 - arg2);
}

Datum
int2mul(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16(arg1 * arg2);
}

Datum
int2div(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16(arg1 / arg2);
}

Datum
int24pl(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 + arg2);
}

Datum
int24mi(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 - arg2);
}

Datum
int24mul(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 * arg2);
}

Datum
int24div(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 / arg2);
}

Datum
int42pl(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT32(arg1 + arg2);
}

Datum
int42mi(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT32(arg1 - arg2);
}

Datum
int42mul(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT32(arg1 * arg2);
}

Datum
int42div(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT32(arg1 / arg2);
}

Datum
int4mod(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 % arg2);
}

Datum
int2mod(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16(arg1 % arg2);
}

Datum
int24mod(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 % arg2);
}

Datum
int42mod(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT32(arg1 % arg2);
}

/* int[24]fac()
 * Factorial
 */
Datum
int4fac(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		result;

	if (arg1 < 1)
		result = 0;
	else
		for (result = 1; arg1 > 0; --arg1)
			result *= arg1;
	PG_RETURN_INT32(result);
}

Datum
int2fac(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		result;

	if (arg1 < 1)
		result = 0;
	else
		for (result = 1; arg1 > 0; --arg1)
			result *= arg1;
	PG_RETURN_INT32(result);
}

/* int[24]abs()
 * Absolute value
 */
Datum
int4abs(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);

	PG_RETURN_INT32((arg1 < 0) ? -arg1 : arg1);
}

Datum
int2abs(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);

	PG_RETURN_INT16((arg1 < 0) ? -arg1 : arg1);
}

Datum
int2larger(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16((arg1 > arg2) ? arg1 : arg2);
}

Datum
int2smaller(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16((arg1 < arg2) ? arg1 : arg2);
}

Datum
int4larger(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32((arg1 > arg2) ? arg1 : arg2);
}

Datum
int4smaller(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32((arg1 < arg2) ? arg1 : arg2);
}

/* Binary arithmetics
 *
 *		int[24]and		- returns arg1 & arg2
 *		int[24]or		- returns arg1 | arg2
 *		int[24]xor		- returns arg1 # arg2
 *		int[24]not		- returns ~arg1
 *		int[24]shl		- returns arg1 << arg2
 *		int[24]shr		- returns arg1 >> arg2
 */

Datum
int4and(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 & arg2);
}

Datum
int4or(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 | arg2);
}

Datum
int4xor(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 ^ arg2);
}

Datum
int4shl(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 << arg2);
}

Datum
int4shr(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT32(arg1 >> arg2);
}

Datum
int4not(PG_FUNCTION_ARGS)
{
	int32		arg1 = PG_GETARG_INT32(0);

	PG_RETURN_INT32(~arg1);
}

Datum
int2and(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16(arg1 & arg2);
}

Datum
int2or(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16(arg1 | arg2);
}

Datum
int2xor(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int16		arg2 = PG_GETARG_INT16(1);

	PG_RETURN_INT16(arg1 ^ arg2);
}

Datum
int2not(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);

	PG_RETURN_INT16(~arg1);
}


Datum
int2shl(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT16(arg1 << arg2);
}

Datum
int2shr(PG_FUNCTION_ARGS)
{
	int16		arg1 = PG_GETARG_INT16(0);
	int32		arg2 = PG_GETARG_INT32(1);

	PG_RETURN_INT16(arg1 >> arg2);
}
