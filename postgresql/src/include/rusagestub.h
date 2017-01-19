/*-------------------------------------------------------------------------
 *
 * rusagestub.h
 *	  Stubs for getrusage(3).
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: rusagestub.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef RUSAGESTUB_H
#define RUSAGESTUB_H

#include <sys/time.h>			/* for struct timeval */
#include <sys/times.h>			/* for struct tms */
#include <limits.h>				/* for CLK_TCK */

#define RUSAGE_SELF		0
#define RUSAGE_CHILDREN -1

struct rusage
{
	struct timeval ru_utime;	/* user time used */
	struct timeval ru_stime;	/* system time used */
};

extern int	getrusage(int who, struct rusage * rusage);

#endif   /* RUSAGESTUB_H */
