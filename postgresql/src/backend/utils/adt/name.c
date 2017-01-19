/*-------------------------------------------------------------------------
 *
 * name.c
 *	  Functions for the built-in type "name".
 * name replaces char16 and is carefully implemented so that it
 * is a string of length NAMEDATALEN.  DO NOT use hard-coded constants anywhere
 * always use NAMEDATALEN as the symbolic constant!   - jolly 8/21/95
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/adt/name.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "miscadmin.h"
#include "utils/builtins.h"

/*****************************************************************************
 *	 USER I/O ROUTINES (none)												 *
 *****************************************************************************/


/*
 *		namein	- converts "..." to internal representation
 *
 *		Note:
 *				[Old] Currently if strlen(s) < NAMEDATALEN, the extra chars are nulls
 *				Now, always NULL terminated
 */
Datum
namein(PG_FUNCTION_ARGS)
{
	char	   *s = PG_GETARG_CSTRING(0);
	NameData   *result;
	int			len;

	result = (NameData *) palloc(NAMEDATALEN);
	/* always keep it null-padded */
	StrNCpy(NameStr(*result), s, NAMEDATALEN);
	len = strlen(NameStr(*result));
	while (len < NAMEDATALEN)
	{
		*(NameStr(*result) + len) = '\0';
		len++;
	}
	PG_RETURN_NAME(result);
}

/*
 *		nameout - converts internal representation to "..."
 */
Datum
nameout(PG_FUNCTION_ARGS)
{
	Name		s = PG_GETARG_NAME(0);

	PG_RETURN_CSTRING(pstrdup(NameStr(*s)));
}


/*****************************************************************************
 *	 PUBLIC ROUTINES														 *
 *****************************************************************************/

/*
 *		nameeq	- returns 1 iff arguments are equal
 *		namene	- returns 1 iff arguments are not equal
 *
 *		BUGS:
 *				Assumes that "xy\0\0a" should be equal to "xy\0b".
 *				If not, can do the comparison backwards for efficiency.
 *
 *		namelt	- returns 1 iff a < b
 *		namele	- returns 1 iff a <= b
 *		namegt	- returns 1 iff a < b
 *		namege	- returns 1 iff a <= b
 *
 */
Datum
nameeq(PG_FUNCTION_ARGS)
{
	Name		arg1 = PG_GETARG_NAME(0);
	Name		arg2 = PG_GETARG_NAME(1);

	PG_RETURN_BOOL(strncmp(NameStr(*arg1), NameStr(*arg2), NAMEDATALEN) == 0);
}

Datum
namene(PG_FUNCTION_ARGS)
{
	Name		arg1 = PG_GETARG_NAME(0);
	Name		arg2 = PG_GETARG_NAME(1);

	PG_RETURN_BOOL(strncmp(NameStr(*arg1), NameStr(*arg2), NAMEDATALEN) != 0);
}

Datum
namelt(PG_FUNCTION_ARGS)
{
	Name		arg1 = PG_GETARG_NAME(0);
	Name		arg2 = PG_GETARG_NAME(1);

	PG_RETURN_BOOL(strncmp(NameStr(*arg1), NameStr(*arg2), NAMEDATALEN) < 0);
}

Datum
namele(PG_FUNCTION_ARGS)
{
	Name		arg1 = PG_GETARG_NAME(0);
	Name		arg2 = PG_GETARG_NAME(1);

	PG_RETURN_BOOL(strncmp(NameStr(*arg1), NameStr(*arg2), NAMEDATALEN) <= 0);
}

Datum
namegt(PG_FUNCTION_ARGS)
{
	Name		arg1 = PG_GETARG_NAME(0);
	Name		arg2 = PG_GETARG_NAME(1);

	PG_RETURN_BOOL(strncmp(NameStr(*arg1), NameStr(*arg2), NAMEDATALEN) > 0);
}

Datum
namege(PG_FUNCTION_ARGS)
{
	Name		arg1 = PG_GETARG_NAME(0);
	Name		arg2 = PG_GETARG_NAME(1);

	PG_RETURN_BOOL(strncmp(NameStr(*arg1), NameStr(*arg2), NAMEDATALEN) >= 0);
}


/* (see char.c for comparison/operation routines) */

int
namecpy(Name n1, Name n2)
{
	if (!n1 || !n2)
		return -1;
	strncpy(NameStr(*n1), NameStr(*n2), NAMEDATALEN);
	return 0;
}

#ifdef NOT_USED
int
namecat(Name n1, Name n2)
{
	return namestrcat(n1, NameStr(*n2));		/* n2 can't be any longer
												 * than n1 */
}
#endif

#ifdef NOT_USED
int
namecmp(Name n1, Name n2)
{
	return strncmp(NameStr(*n1), NameStr(*n2), NAMEDATALEN);
}
#endif

int
namestrcpy(Name name, const char *str)
{
	if (!name || !str)
		return -1;
	StrNCpy(NameStr(*name), str, NAMEDATALEN);
	return 0;
}

#ifdef NOT_USED
int
namestrcat(Name name, const char *str)
{
	int			i;
	char	   *p,
			   *q;

	if (!name || !str)
		return -1;
	for (i = 0, p = NameStr(*name); i < NAMEDATALEN && *p; ++i, ++p)
		;
	for (q = str; i < NAMEDATALEN; ++i, ++p, ++q)
	{
		*p = *q;
		if (!*q)
			break;
	}
	return 0;
}
#endif

int
namestrcmp(Name name, const char *str)
{
	if (!name && !str)
		return 0;
	if (!name)
		return -1;				/* NULL < anything */
	if (!str)
		return 1;				/* NULL < anything */
	return strncmp(NameStr(*name), str, NAMEDATALEN);
}


/* SQL-functions CURRENT_USER and SESSION_USER */
Datum
current_user(PG_FUNCTION_ARGS)
{
	PG_RETURN_DATUM(DirectFunctionCall1(namein, CStringGetDatum(GetUserName(GetUserId()))));
}

Datum
session_user(PG_FUNCTION_ARGS)
{
	PG_RETURN_DATUM(DirectFunctionCall1(namein, CStringGetDatum(GetUserName(GetSessionUserId()))));
}


/*****************************************************************************
 *	 PRIVATE ROUTINES														 *
 *****************************************************************************/

#ifdef NOT_USED
uint32
NameComputeLength(Name name)
{
	char	   *charP;
	int			length;

	for (length = 0, charP = NameStr(*name);
		 length < NAMEDATALEN && *charP != '\0';
		 length++, charP++)
		;
	return (uint32) length;
}

#endif
