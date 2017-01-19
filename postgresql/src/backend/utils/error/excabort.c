/*-------------------------------------------------------------------------
 *
 * excabort.c
 *	  Default exception abort code.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/error/excabort.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "utils/exc.h"

void
ExcAbort(const Exception *excP,
		 ExcDetail detail,
		 ExcData data,
		 ExcMessage message)
{
	/* dump core */
	abort();
}
