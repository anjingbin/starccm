/*-------------------------------------------------------------------------
 *
 * varlena.c
 *	  Functions for the variable-length built-in types.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/adt/varlena.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <ctype.h>

#include "mb/pg_wchar.h"
#include "miscadmin.h"
#include "utils/builtins.h"

static int	text_cmp(text *arg1, text *arg2);


/*****************************************************************************
 *	 USER I/O ROUTINES														 *
 *****************************************************************************/


#define VAL(CH)			((CH) - '0')
#define DIG(VAL)		((VAL) + '0')

/*
 *		byteain			- converts from printable representation of byte array
 *
 *		Non-printable characters must be passed as '\nnn' (octal) and are
 *		converted to internal form.  '\' must be passed as '\\'.
 *		elog(ERROR, ...) if bad form.
 *
 *		BUGS:
 *				The input is scaned twice.
 *				The error checking of input is minimal.
 */
Datum
byteain(PG_FUNCTION_ARGS)
{
	char	   *inputText = PG_GETARG_CSTRING(0);
	char	   *tp;
	char	   *rp;
	int			byte;
	bytea	   *result;

	for (byte = 0, tp = inputText; *tp != '\0'; byte++)
	{
		if (tp[0] != '\\')
			tp++;
		else if ((tp[0] == '\\') &&
				 (tp[1] >= '0' && tp[1] <= '3') &&
				 (tp[2] >= '0' && tp[2] <= '7') &&
				 (tp[3] >= '0' && tp[3] <= '7'))
			tp += 4;
		else if ((tp[0] == '\\') &&
				 (tp[1] == '\\'))
			tp += 2;
		else
		{
			/*
			 * one backslash, not followed by 0 or ### valid octal
			 */
			elog(ERROR, "Bad input string for type bytea");
		}
	}

	byte += VARHDRSZ;
	result = (bytea *) palloc(byte);
	result->vl_len = byte;		/* set varlena length */

	tp = inputText;
	rp = result->vl_dat;
	while (*tp != '\0')
	{
		if (tp[0] != '\\')
			*rp++ = *tp++;
		else if ((tp[0] == '\\') &&
				 (tp[1] >= '0' && tp[1] <= '3') &&
				 (tp[2] >= '0' && tp[2] <= '7') &&
				 (tp[3] >= '0' && tp[3] <= '7'))
		{
			byte = VAL(tp[1]);
			byte <<= 3;
			byte += VAL(tp[2]);
			byte <<= 3;
			*rp++ = byte + VAL(tp[3]);
			tp += 4;
		}
		else if ((tp[0] == '\\') &&
				 (tp[1] == '\\'))
		{
			*rp++ = '\\';
			tp += 2;
		}
		else
		{
			/*
			 * We should never get here. The first pass should not allow
			 * it.
			 */
			elog(ERROR, "Bad input string for type bytea");
		}
	}

	PG_RETURN_BYTEA_P(result);
}

/*
 *		byteaout		- converts to printable representation of byte array
 *
 *		Non-printable characters are inserted as '\nnn' (octal) and '\' as
 *		'\\'.
 *
 *		NULL vlena should be an error--returning string with NULL for now.
 */
Datum
byteaout(PG_FUNCTION_ARGS)
{
	bytea	   *vlena = PG_GETARG_BYTEA_P(0);
	char	   *result;
	char	   *vp;
	char	   *rp;
	int			val;			/* holds unprintable chars */
	int			i;
	int			len;

	len = 1;					/* empty string has 1 char */
	vp = vlena->vl_dat;
	for (i = vlena->vl_len - VARHDRSZ; i != 0; i--, vp++)
	{
		if (*vp == '\\')
			len += 2;
		else if (isprint((unsigned char) *vp))
			len++;
		else
			len += 4;
	}
	rp = result = (char *) palloc(len);
	vp = vlena->vl_dat;
	for (i = vlena->vl_len - VARHDRSZ; i != 0; i--, vp++)
	{
		if (*vp == '\\')
		{
			*rp++ = '\\';
			*rp++ = '\\';
		}
		else if (isprint((unsigned char) *vp))
			*rp++ = *vp;
		else
		{
			val = *vp;
			rp[0] = '\\';
			rp[3] = DIG(val & 07);
			val >>= 3;
			rp[2] = DIG(val & 07);
			val >>= 3;
			rp[1] = DIG(val & 03);
			rp += 4;
		}
	}
	*rp = '\0';
	PG_RETURN_CSTRING(result);
}


/*
 *		textin			- converts "..." to internal representation
 */
Datum
textin(PG_FUNCTION_ARGS)
{
	char	   *inputText = PG_GETARG_CSTRING(0);
	text	   *result;
	int			len;

#ifdef MULTIBYTE
	char	   *ermsg;
#endif

	len = strlen(inputText) + VARHDRSZ;

#ifdef MULTIBYTE
	if ((ermsg = pg_verifymbstr(inputText, len - VARHDRSZ)))
		elog(ERROR, "%s", ermsg);
#endif

	result = (text *) palloc(len);
	VARATT_SIZEP(result) = len;

	memcpy(VARDATA(result), inputText, len - VARHDRSZ);

#ifdef CYR_RECODE
	convertstr(VARDATA(result), len - VARHDRSZ, 0);
#endif

	PG_RETURN_TEXT_P(result);
}

/*
 *		textout			- converts internal representation to "..."
 */
Datum
textout(PG_FUNCTION_ARGS)
{
	text	   *t = PG_GETARG_TEXT_P(0);
	int			len;
	char	   *result;

	len = VARSIZE(t) - VARHDRSZ;
	result = (char *) palloc(len + 1);
	memcpy(result, VARDATA(t), len);
	result[len] = '\0';

#ifdef CYR_RECODE
	convertstr(result, len, 1);
#endif

	PG_RETURN_CSTRING(result);
}


/* ========== PUBLIC ROUTINES ========== */

/*
 * textlen -
 *	  returns the logical length of a text*
 *	   (which is less than the VARSIZE of the text*)
 */
Datum
textlen(PG_FUNCTION_ARGS)
{
	text	   *t = PG_GETARG_TEXT_P(0);

#ifdef MULTIBYTE
	/* optimization for single byte encoding */
	if (pg_database_encoding_max_length() <= 1)
		PG_RETURN_INT32(VARSIZE(t) - VARHDRSZ);

	PG_RETURN_INT32(
		pg_mbstrlen_with_len(VARDATA(t), VARSIZE(t) - VARHDRSZ)
		);
#else
	PG_RETURN_INT32(VARSIZE(t) - VARHDRSZ);
#endif
}

/*
 * textoctetlen -
 *	  returns the physical length of a text*
 *	   (which is less than the VARSIZE of the text*)
 */
Datum
textoctetlen(PG_FUNCTION_ARGS)
{
	text    *arg = PG_GETARG_TEXT_P(0);

	PG_RETURN_INT32(VARSIZE(arg) - VARHDRSZ);
}

/*
 * textcat -
 *	  takes two text* and returns a text* that is the concatenation of
 *	  the two.
 *
 * Rewritten by Sapa, sapa@hq.icb.chel.su. 8-Jul-96.
 * Updated by Thomas, Thomas.Lockhart@jpl.nasa.gov 1997-07-10.
 * Allocate space for output in all cases.
 * XXX - thomas 1997-07-10
 */
Datum
textcat(PG_FUNCTION_ARGS)
{
	text	   *t1 = PG_GETARG_TEXT_P(0);
	text	   *t2 = PG_GETARG_TEXT_P(1);
	int			len1,
				len2,
				len;
	text	   *result;
	char	   *ptr;

	len1 = (VARSIZE(t1) - VARHDRSZ);
	if (len1 < 0)
		len1 = 0;

	len2 = (VARSIZE(t2) - VARHDRSZ);
	if (len2 < 0)
		len2 = 0;

	len = len1 + len2 + VARHDRSZ;
	result = (text *) palloc(len);

	/* Set size of result string... */
	VARATT_SIZEP(result) = len;

	/* Fill data field of result string... */
	ptr = VARDATA(result);
	if (len1 > 0)
		memcpy(ptr, VARDATA(t1), len1);
	if (len2 > 0)
		memcpy(ptr + len1, VARDATA(t2), len2);

	PG_RETURN_TEXT_P(result);
}

/*
 * text_substr()
 * Return a substring starting at the specified position.
 * - thomas 1997-12-31
 *
 * Input:
 *	- string
 *	- starting position (is one-based)
 *	- string length
 *
 * If the starting position is zero or less, then return from the start of the string
 *	adjusting the length to be consistent with the "negative start" per SQL92.
 * If the length is less than zero, return the remaining string.
 *
 * Note that the arguments operate on octet length,
 *	so not aware of multi-byte character sets.
 *
 * Added multi-byte support.
 * - Tatsuo Ishii 1998-4-21
 * Changed behavior if starting position is less than one to conform to SQL92 behavior.
 * Formerly returned the entire string; now returns a portion.
 * - Thomas Lockhart 1998-12-10
 */
Datum
text_substr(PG_FUNCTION_ARGS)
{
	text	   *string = PG_GETARG_TEXT_P(0);
	int32		m = PG_GETARG_INT32(1);
	int32		n = PG_GETARG_INT32(2);
	text	   *ret;
	int			len;

#ifdef MULTIBYTE
	int			i;
	char	   *p;
#endif

	len = VARSIZE(string) - VARHDRSZ;
#ifdef MULTIBYTE
	len = pg_mbstrlen_with_len(VARDATA(string), len);
#endif

	/* starting position after the end of the string? */
	if (m > len)
	{
		m = 1;
		n = 0;
	}

	/*
	 * starting position before the start of the string? then offset into
	 * the string per SQL92 spec...
	 */
	else if (m < 1)
	{
		n += (m - 1);
		m = 1;
	}

	/* m will now become a zero-based starting position */
	m--;
	if (((m + n) > len) || (n < 0))
		n = (len - m);

#ifdef MULTIBYTE
	p = VARDATA(string);
	for (i = 0; i < m; i++)
		p += pg_mblen(p);
	m = p - VARDATA(string);
	for (i = 0; i < n; i++)
		p += pg_mblen(p);
	n = p - (VARDATA(string) + m);
#endif

	ret = (text *) palloc(VARHDRSZ + n);
	VARATT_SIZEP(ret) = VARHDRSZ + n;

	memcpy(VARDATA(ret), VARDATA(string) + m, n);

	PG_RETURN_TEXT_P(ret);
}

/*
 * textpos -
 *	  Return the position of the specified substring.
 *	  Implements the SQL92 POSITION() function.
 *	  Ref: A Guide To The SQL Standard, Date & Darwen, 1997
 * - thomas 1997-07-27
 *
 * Added multi-byte support.
 * - Tatsuo Ishii 1998-4-21
 */
Datum
textpos(PG_FUNCTION_ARGS)
{
	text	   *t1 = PG_GETARG_TEXT_P(0);
	text	   *t2 = PG_GETARG_TEXT_P(1);
	int			pos;
	int			px,
				p;
	int			len1,
				len2;
	pg_wchar   *p1,
			   *p2;

#ifdef MULTIBYTE
	pg_wchar   *ps1,
			   *ps2;
#endif

	if (VARSIZE(t2) <= VARHDRSZ)
		PG_RETURN_INT32(1);		/* result for empty pattern */

	len1 = (VARSIZE(t1) - VARHDRSZ);
	len2 = (VARSIZE(t2) - VARHDRSZ);
#ifdef MULTIBYTE
	ps1 = p1 = (pg_wchar *) palloc((len1 + 1) * sizeof(pg_wchar));
	(void) pg_mb2wchar_with_len((unsigned char *) VARDATA(t1), p1, len1);
	len1 = pg_wchar_strlen(p1);
	ps2 = p2 = (pg_wchar *) palloc((len2 + 1) * sizeof(pg_wchar));
	(void) pg_mb2wchar_with_len((unsigned char *) VARDATA(t2), p2, len2);
	len2 = pg_wchar_strlen(p2);
#else
	p1 = VARDATA(t1);
	p2 = VARDATA(t2);
#endif
	pos = 0;
	px = (len1 - len2);
	for (p = 0; p <= px; p++)
	{
#ifdef MULTIBYTE
		if ((*p2 == *p1) && (pg_wchar_strncmp(p1, p2, len2) == 0))
#else
		if ((*p2 == *p1) && (strncmp(p1, p2, len2) == 0))
#endif
		{
			pos = p + 1;
			break;
		};
		p1++;
	};
#ifdef MULTIBYTE
	pfree(ps1);
	pfree(ps2);
#endif
	PG_RETURN_INT32(pos);
}

/* varstr_cmp()
 * Comparison function for text strings with given lengths.
 * Includes locale support, but must copy strings to temporary memory
 *	to allow null-termination for inputs to strcoll().
 * Returns -1, 0 or 1
 */
int
varstr_cmp(char *arg1, int len1, char *arg2, int len2)
{
	int			result;
	char	   *a1p,
			   *a2p;

#ifdef USE_LOCALE
	a1p = (char *) palloc(len1 + 1);
	a2p = (char *) palloc(len2 + 1);

	memcpy(a1p, arg1, len1);
	*(a1p + len1) = '\0';
	memcpy(a2p, arg2, len2);
	*(a2p + len2) = '\0';

	result = strcoll(a1p, a2p);

	pfree(a1p);
	pfree(a2p);

#else

	a1p = arg1;
	a2p = arg2;

	result = strncmp(a1p, a2p, Min(len1, len2));
	if ((result == 0) && (len1 != len2))
		result = (len1 < len2) ? -1 : 1;
#endif

	return result;
}


/* text_cmp()
 * Internal comparison function for text strings.
 * Returns -1, 0 or 1
 */
static int
text_cmp(text *arg1, text *arg2)
{
	char	   *a1p,
			   *a2p;
	int			len1,
				len2;

	a1p = VARDATA(arg1);
	a2p = VARDATA(arg2);

	len1 = VARSIZE(arg1) - VARHDRSZ;
	len2 = VARSIZE(arg2) - VARHDRSZ;

	return varstr_cmp(a1p, len1, a2p, len2);
}

/*
 * Comparison functions for text strings.
 *
 * Note: btree indexes need these routines not to leak memory; therefore,
 * be careful to free working copies of toasted datums.  Most places don't
 * need to be so careful.
 */

Datum
texteq(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	bool		result;

	/* fast path for different-length inputs */
	if (VARSIZE(arg1) != VARSIZE(arg2))
		result = false;
	else
		result = (text_cmp(arg1, arg2) == 0);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL(result);
}

Datum
textne(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	bool		result;

	/* fast path for different-length inputs */
	if (VARSIZE(arg1) != VARSIZE(arg2))
		result = true;
	else
		result = (text_cmp(arg1, arg2) != 0);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL(result);
}

Datum
text_lt(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	bool		result;

	result = (text_cmp(arg1, arg2) < 0);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL(result);
}

Datum
text_le(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	bool		result;

	result = (text_cmp(arg1, arg2) <= 0);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL(result);
}

Datum
text_gt(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	bool		result;

	result = (text_cmp(arg1, arg2) > 0);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL(result);
}

Datum
text_ge(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	bool		result;

	result = (text_cmp(arg1, arg2) >= 0);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL(result);
}

Datum
bttextcmp(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	int32		result;

	result = text_cmp(arg1, arg2);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_INT32(result);
}


Datum
text_larger(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	text	   *result;

	result = ((text_cmp(arg1, arg2) > 0) ? arg1 : arg2);

	PG_RETURN_TEXT_P(result);
}

Datum
text_smaller(PG_FUNCTION_ARGS)
{
	text	   *arg1 = PG_GETARG_TEXT_P(0);
	text	   *arg2 = PG_GETARG_TEXT_P(1);
	text	   *result;

	result = ((text_cmp(arg1, arg2) < 0) ? arg1 : arg2);

	PG_RETURN_TEXT_P(result);
}

/*-------------------------------------------------------------
 * byteaoctetlen
 *
 * get the number of bytes contained in an instance of type 'bytea'
 *-------------------------------------------------------------
 */
Datum
byteaoctetlen(PG_FUNCTION_ARGS)
{
	bytea	   *v = PG_GETARG_BYTEA_P(0);

	PG_RETURN_INT32(VARSIZE(v) - VARHDRSZ);
}

/*
 * byteacat -
 *	  takes two bytea* and returns a bytea* that is the concatenation of
 *	  the two.
 *
 * Cloned from textcat and modified as required.
 */
Datum
byteacat(PG_FUNCTION_ARGS)
{
	bytea	   *t1 = PG_GETARG_BYTEA_P(0);
	bytea	   *t2 = PG_GETARG_BYTEA_P(1);
	int			len1,
				len2,
				len;
	bytea	   *result;
	char	   *ptr;

	len1 = (VARSIZE(t1) - VARHDRSZ);
	if (len1 < 0)
		len1 = 0;

	len2 = (VARSIZE(t2) - VARHDRSZ);
	if (len2 < 0)
		len2 = 0;

	len = len1 + len2 + VARHDRSZ;
	result = (bytea *) palloc(len);

	/* Set size of result string... */
	VARATT_SIZEP(result) = len;

	/* Fill data field of result string... */
	ptr = VARDATA(result);
	if (len1 > 0)
		memcpy(ptr, VARDATA(t1), len1);
	if (len2 > 0)
		memcpy(ptr + len1, VARDATA(t2), len2);

	PG_RETURN_BYTEA_P(result);
}

/*
 * bytea_substr()
 * Return a substring starting at the specified position.
 * Cloned from text_substr and modified as required.
 *
 * Input:
 *	- string
 *	- starting position (is one-based)
 *	- string length
 *
 * If the starting position is zero or less, then return from the start of the string
 * adjusting the length to be consistent with the "negative start" per SQL92.
 * If the length is less than zero, return the remaining string.
 *
 */
Datum
bytea_substr(PG_FUNCTION_ARGS)
{
	bytea	   *string = PG_GETARG_BYTEA_P(0);
	int32		m = PG_GETARG_INT32(1);
	int32		n = PG_GETARG_INT32(2);
	bytea	   *ret;
	int			len;

	len = VARSIZE(string) - VARHDRSZ;

	/* starting position after the end of the string? */
	if (m > len)
	{
		m = 1;
		n = 0;
	}

	/*
	 * starting position before the start of the string? then offset into
	 * the string per SQL92 spec...
	 */
	else if (m < 1)
	{
		n += (m - 1);
		m = 1;
	}

	/* m will now become a zero-based starting position */
	m--;
	if (((m + n) > len) || (n < 0))
		n = (len - m);

	ret = (bytea *) palloc(VARHDRSZ + n);
	VARATT_SIZEP(ret) = VARHDRSZ + n;

	memcpy(VARDATA(ret), VARDATA(string) + m, n);

	PG_RETURN_BYTEA_P(ret);
}

/*
 * byteapos -
 *	  Return the position of the specified substring.
 *	  Implements the SQL92 POSITION() function.
 * Cloned from textpos and modified as required.
 */
Datum
byteapos(PG_FUNCTION_ARGS)
{
	bytea	   *t1 = PG_GETARG_BYTEA_P(0);
	bytea	   *t2 = PG_GETARG_BYTEA_P(1);
	int			pos;
	int			px,
				p;
	int			len1,
				len2;
	char	   *p1,
			   *p2;

	if (VARSIZE(t2) <= VARHDRSZ)
		PG_RETURN_INT32(1);		/* result for empty pattern */

	len1 = (VARSIZE(t1) - VARHDRSZ);
	len2 = (VARSIZE(t2) - VARHDRSZ);

	p1 = VARDATA(t1);
	p2 = VARDATA(t2);

	pos = 0;
	px = (len1 - len2);
	for (p = 0; p <= px; p++)
	{
		if ((*p2 == *p1) && (memcmp(p1, p2, len2) == 0))
		{
			pos = p + 1;
			break;
		};
		p1++;
	};

	PG_RETURN_INT32(pos);
}

/*-------------------------------------------------------------
 * byteaGetByte
 *
 * this routine treats "bytea" as an array of bytes.
 * It returns the Nth byte (a number between 0 and 255).
 *-------------------------------------------------------------
 */
Datum
byteaGetByte(PG_FUNCTION_ARGS)
{
	bytea	   *v = PG_GETARG_BYTEA_P(0);
	int32		n = PG_GETARG_INT32(1);
	int			len;
	int			byte;

	len = VARSIZE(v) - VARHDRSZ;

	if (n < 0 || n >= len)
		elog(ERROR, "byteaGetByte: index %d out of range [0..%d]",
			 n, len - 1);

	byte = ((unsigned char *) VARDATA(v))[n];

	PG_RETURN_INT32(byte);
}

/*-------------------------------------------------------------
 * byteaGetBit
 *
 * This routine treats a "bytea" type like an array of bits.
 * It returns the value of the Nth bit (0 or 1).
 *
 *-------------------------------------------------------------
 */
Datum
byteaGetBit(PG_FUNCTION_ARGS)
{
	bytea	   *v = PG_GETARG_BYTEA_P(0);
	int32		n = PG_GETARG_INT32(1);
	int			byteNo,
				bitNo;
	int			len;
	int			byte;

	len = VARSIZE(v) - VARHDRSZ;

	if (n < 0 || n >= len * 8)
		elog(ERROR, "byteaGetBit: index %d out of range [0..%d]",
			 n, len * 8 - 1);

	byteNo = n / 8;
	bitNo = n % 8;

	byte = ((unsigned char *) VARDATA(v))[byteNo];

	if (byte & (1 << bitNo))
		PG_RETURN_INT32(1);
	else
		PG_RETURN_INT32(0);
}

/*-------------------------------------------------------------
 * byteaSetByte
 *
 * Given an instance of type 'bytea' creates a new one with
 * the Nth byte set to the given value.
 *
 *-------------------------------------------------------------
 */
Datum
byteaSetByte(PG_FUNCTION_ARGS)
{
	bytea	   *v = PG_GETARG_BYTEA_P(0);
	int32		n = PG_GETARG_INT32(1);
	int32		newByte = PG_GETARG_INT32(2);
	int			len;
	bytea	   *res;

	len = VARSIZE(v) - VARHDRSZ;

	if (n < 0 || n >= len)
		elog(ERROR, "byteaSetByte: index %d out of range [0..%d]",
			 n, len - 1);

	/*
	 * Make a copy of the original varlena.
	 */
	res = (bytea *) palloc(VARSIZE(v));
	memcpy((char *) res, (char *) v, VARSIZE(v));

	/*
	 * Now set the byte.
	 */
	((unsigned char *) VARDATA(res))[n] = newByte;

	PG_RETURN_BYTEA_P(res);
}

/*-------------------------------------------------------------
 * byteaSetBit
 *
 * Given an instance of type 'bytea' creates a new one with
 * the Nth bit set to the given value.
 *
 *-------------------------------------------------------------
 */
Datum
byteaSetBit(PG_FUNCTION_ARGS)
{
	bytea	   *v = PG_GETARG_BYTEA_P(0);
	int32		n = PG_GETARG_INT32(1);
	int32		newBit = PG_GETARG_INT32(2);
	bytea	   *res;
	int			len;
	int			oldByte,
				newByte;
	int			byteNo,
				bitNo;

	len = VARSIZE(v) - VARHDRSZ;

	if (n < 0 || n >= len * 8)
		elog(ERROR, "byteaSetBit: index %d out of range [0..%d]",
			 n, len * 8 - 1);

	byteNo = n / 8;
	bitNo = n % 8;

	/*
	 * sanity check!
	 */
	if (newBit != 0 && newBit != 1)
		elog(ERROR, "byteaSetBit: new bit must be 0 or 1");

	/*
	 * Make a copy of the original varlena.
	 */
	res = (bytea *) palloc(VARSIZE(v));
	memcpy((char *) res, (char *) v, VARSIZE(v));

	/*
	 * Update the byte.
	 */
	oldByte = ((unsigned char *) VARDATA(res))[byteNo];

	if (newBit == 0)
		newByte = oldByte & (~(1 << bitNo));
	else
		newByte = oldByte | (1 << bitNo);

	((unsigned char *) VARDATA(res))[byteNo] = newByte;

	PG_RETURN_BYTEA_P(res);
}


/* text_name()
 * Converts a text type to a Name type.
 */
Datum
text_name(PG_FUNCTION_ARGS)
{
	text	   *s = PG_GETARG_TEXT_P(0);
	Name		result;
	int			len;

	len = VARSIZE(s) - VARHDRSZ;

	/* Truncate oversize input */
	if (len >= NAMEDATALEN)
		len = NAMEDATALEN - 1;

#ifdef STRINGDEBUG
	printf("text- convert string length %d (%d) ->%d\n",
		   VARSIZE(s) - VARHDRSZ, VARSIZE(s), len);
#endif

	result = (Name) palloc(NAMEDATALEN);
	memcpy(NameStr(*result), VARDATA(s), len);

	/* now null pad to full length... */
	while (len < NAMEDATALEN)
	{
		*(NameStr(*result) + len) = '\0';
		len++;
	}

	PG_RETURN_NAME(result);
}

/* name_text()
 * Converts a Name type to a text type.
 */
Datum
name_text(PG_FUNCTION_ARGS)
{
	Name		s = PG_GETARG_NAME(0);
	text	   *result;
	int			len;

	len = strlen(NameStr(*s));

#ifdef STRINGDEBUG
	printf("text- convert string length %d (%d) ->%d\n",
		   VARSIZE(s) - VARHDRSZ, VARSIZE(s), len);
#endif

	result = palloc(VARHDRSZ + len);
	VARATT_SIZEP(result) = VARHDRSZ + len;
	memcpy(VARDATA(result), NameStr(*s), len);

	PG_RETURN_TEXT_P(result);
}


/*****************************************************************************
 *	Comparison Functions used for bytea
 *
 * Note: btree indexes need these routines not to leak memory; therefore,
 * be careful to free working copies of toasted datums.  Most places don't
 * need to be so careful.
 *****************************************************************************/

Datum
byteaeq(PG_FUNCTION_ARGS)
{
	bytea	   *arg1 = PG_GETARG_BYTEA_P(0);
	bytea	   *arg2 = PG_GETARG_BYTEA_P(1);
	int			len1,
				len2;
	bool		result;

	len1 = VARSIZE(arg1) - VARHDRSZ;
	len2 = VARSIZE(arg2) - VARHDRSZ;

	/* fast path for different-length inputs */
	if (len1 != len2)
		result = false;
	else
		result = (memcmp(VARDATA(arg1), VARDATA(arg2), len1) == 0);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL(result);
}

Datum
byteane(PG_FUNCTION_ARGS)
{
	bytea	   *arg1 = PG_GETARG_BYTEA_P(0);
	bytea	   *arg2 = PG_GETARG_BYTEA_P(1);
	int			len1,
				len2;
	bool		result;

	len1 = VARSIZE(arg1) - VARHDRSZ;
	len2 = VARSIZE(arg2) - VARHDRSZ;

	/* fast path for different-length inputs */
	if (len1 != len2)
		result = true;
	else
		result = (memcmp(VARDATA(arg1), VARDATA(arg2), len1) != 0);

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL(result);
}

Datum
bytealt(PG_FUNCTION_ARGS)
{
	bytea	   *arg1 = PG_GETARG_BYTEA_P(0);
	bytea	   *arg2 = PG_GETARG_BYTEA_P(1);
	int			len1,
				len2;
	int			cmp;

	len1 = VARSIZE(arg1) - VARHDRSZ;
	len2 = VARSIZE(arg2) - VARHDRSZ;

	cmp = memcmp(VARDATA(arg1), VARDATA(arg2), Min(len1, len2));

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL((cmp < 0) || ((cmp == 0) && (len1 < len2)));
}

Datum
byteale(PG_FUNCTION_ARGS)
{
	bytea	   *arg1 = PG_GETARG_BYTEA_P(0);
	bytea	   *arg2 = PG_GETARG_BYTEA_P(1);
	int			len1,
				len2;
	int			cmp;

	len1 = VARSIZE(arg1) - VARHDRSZ;
	len2 = VARSIZE(arg2) - VARHDRSZ;

	cmp = memcmp(VARDATA(arg1), VARDATA(arg2), Min(len1, len2));

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL((cmp < 0) || ((cmp == 0) && (len1 <= len2)));
}

Datum
byteagt(PG_FUNCTION_ARGS)
{
	bytea	   *arg1 = PG_GETARG_BYTEA_P(0);
	bytea	   *arg2 = PG_GETARG_BYTEA_P(1);
	int			len1,
				len2;
	int			cmp;

	len1 = VARSIZE(arg1) - VARHDRSZ;
	len2 = VARSIZE(arg2) - VARHDRSZ;

	cmp = memcmp(VARDATA(arg1), VARDATA(arg2), Min(len1, len2));

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL((cmp > 0) || ((cmp == 0) && (len1 > len2)));
}

Datum
byteage(PG_FUNCTION_ARGS)
{
	bytea	   *arg1 = PG_GETARG_BYTEA_P(0);
	bytea	   *arg2 = PG_GETARG_BYTEA_P(1);
	int			len1,
				len2;
	int			cmp;

	len1 = VARSIZE(arg1) - VARHDRSZ;
	len2 = VARSIZE(arg2) - VARHDRSZ;

	cmp = memcmp(VARDATA(arg1), VARDATA(arg2), Min(len1, len2));

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_BOOL((cmp > 0) || ((cmp == 0) && (len1 >= len2)));
}

Datum
byteacmp(PG_FUNCTION_ARGS)
{
	bytea	   *arg1 = PG_GETARG_BYTEA_P(0);
	bytea	   *arg2 = PG_GETARG_BYTEA_P(1);
	int			len1,
				len2;
	int			cmp;

	len1 = VARSIZE(arg1) - VARHDRSZ;
	len2 = VARSIZE(arg2) - VARHDRSZ;

	cmp = memcmp(VARDATA(arg1), VARDATA(arg2), Min(len1, len2));
	if ((cmp == 0) && (len1 != len2))
		cmp = (len1 < len2) ? -1 : 1;

	PG_FREE_IF_COPY(arg1, 0);
	PG_FREE_IF_COPY(arg2, 1);

	PG_RETURN_INT32(cmp);
}
