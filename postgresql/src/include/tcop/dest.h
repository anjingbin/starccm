/*-------------------------------------------------------------------------
 *
 * dest.h
 *		Whenever the backend executes a query, the results
 *		have to go someplace.
 *
 *	  - stdout is the destination only when we are running a
 *		backend without a postmaster and are returning results
 *		back to an interactive user.
 *
 *	  - a remote process is the destination when we are
 *		running a backend with a frontend and the frontend executes
 *		PQexec() or PQfn().  In this case, the results are sent
 *		to the frontend via the functions in backend/libpq.
 *
 *	  - None is the destination when the system executes
 *		a query internally.  This is not used now but it may be
 *		useful for the parallel optimiser/executor.
 *
 * dest.c defines three functions that implement destination management:
 *
 * BeginCommand: initialize the destination.
 * DestToFunction: return a pointer to a struct of destination-specific
 * receiver functions.
 * EndCommand: clean up the destination when output is complete.
 *
 * The DestReceiver object returned by DestToFunction may be a statically
 * allocated object (for destination types that require no local state)
 * or can be a palloc'd object that has DestReceiver as its first field
 * and contains additional fields (see printtup.c for an example).	These
 * additional fields are then accessible to the DestReceiver functions
 * by casting the DestReceiver* pointer passed to them.
 * The palloc'd object is pfree'd by the DestReceiver's cleanup function.
 *
 * XXX FIXME: the initialization and cleanup code that currently appears
 * in-line in BeginCommand and EndCommand probably should be moved out
 * to routines associated with each destination receiver type.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: dest.h,v 1.2 2004/07/05 08:56:08 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef DEST_H
#define DEST_H

#include "access/htup.h"


/* buffer size to use for command completion tags */
#define COMPLETION_TAG_BUFSIZE	64


/* ----------------
 *		CommandDest is a simplistic means of identifying the desired
 *		destination.  Someday this will probably need to be improved.
 * ----------------
 */
typedef enum
{
	None,						/* results are discarded */
	Debug,						/* results go to debugging output */
	Remote,						/* results sent to frontend process */
	RemoteInternal,				/* results sent to frontend process in
								 * internal (binary) form */
	SPI							/* results sent to SPI manager */
} CommandDest;

/* ----------------
 *		DestReceiver is a base type for destination-specific local state.
 *		In the simplest cases, there is no state info, just the function
 *		pointers that the executor must call.
 * ----------------
 */
typedef struct _DestReceiver DestReceiver;

struct _DestReceiver
{
	/* Called for each tuple to be output: */
	void		(*receiveTuple) (HeapTuple tuple, TupleDesc typeinfo,
											 DestReceiver *self);
	/* Initialization and teardown: */
	void		(*setup) (DestReceiver *self, TupleDesc typeinfo);
	void		(*cleanup) (DestReceiver *self);
	/* Private fields might appear beyond this point... */
};

/* The primary destination management functions */

extern void BeginCommand(char *pname, int operation, TupleDesc attinfo,
			 bool isIntoRel, bool isIntoPortal, char *tag,
			 CommandDest dest);
extern DestReceiver *DestToFunction(CommandDest dest);
extern void EndCommand(const char *commandTag, CommandDest dest);

/* Additional functions that go with destination management, more or less. */

extern void SendCopyBegin(void);
extern void ReceiveCopyBegin(void);
extern void NullCommand(CommandDest dest);
extern void ReadyForQuery(CommandDest dest);

#endif   /* DEST_H */
