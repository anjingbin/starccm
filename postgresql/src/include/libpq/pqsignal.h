/*-------------------------------------------------------------------------
 *
 * pqsignal.h
 *	  prototypes for the reliable BSD-style signal(2) routine.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: pqsignal.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 * NOTES
 *	  This shouldn't be in libpq, but the monitor and some other
 *	  things need it...
 *
 *-------------------------------------------------------------------------
 */
#ifndef PQSIGNAL_H
#define PQSIGNAL_H

#include <signal.h>

#ifdef HAVE_SIGPROCMASK
extern sigset_t UnBlockSig,
			BlockSig,
			AuthBlockSig;

#define PG_SETMASK(mask)	sigprocmask(SIG_SETMASK, mask, NULL)
#else
extern int	UnBlockSig,
			BlockSig,
			AuthBlockSig;

#define PG_SETMASK(mask)	sigsetmask(*((int*)(mask)))
#endif

typedef void (*pqsigfunc) (int);

extern void pqinitmask(void);

extern pqsigfunc pqsignal(int signo, pqsigfunc func);

#endif   /* PQSIGNAL_H */
