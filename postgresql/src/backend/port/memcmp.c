/*-------------------------------------------------------------------------
 *
 * memcmp.c
 *	  compares memory bytes
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/memcmp.c,v 1.2 2004/07/05 09:13:23 SuLiang Exp $
 *
 * This file was taken from NetBSD and is used by SunOS because memcmp
 * on that platform does not properly compare negative bytes.
 *
 *-------------------------------------------------------------------------
 */

#include <string.h>

/*
 * Compare memory regions.
 */
int
memcmp(const void *s1, const void *s2, size_t n)
{
	if (n != 0) {
		const unsigned char *p1 = s1, *p2 = s2;

		do {
			if (*p1++ != *p2++)
				return (*--p1 - *--p2);
		} while (--n != 0);
	}
	return 0;
}
