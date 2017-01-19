/*-------------------------------------------------------------------------
 *
 * async.c
 *	  Asynchronous notification: NOTIFY, LISTEN, UNLISTEN
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/commands/async.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

/*-------------------------------------------------------------------------
 * New Async Notification Model:
 * 1. Multiple backends on same machine.  Multiple backends listening on
 *	  one relation.  (Note: "listening on a relation" is not really the
 *	  right way to think about it, since the notify names need not have
 *	  anything to do with the names of relations actually in the database.
 *	  But this terminology is all over the code and docs, and I don't feel
 *	  like trying to replace it.)
 *
 * 2. There is a tuple in relation "pg_listener" for each active LISTEN,
 *	  ie, each relname/listenerPID pair.  The "notification" field of the
 *	  tuple is zero when no NOTIFY is pending for that listener, or the PID
 *	  of the originating backend when a cross-backend NOTIFY is pending.
 *	  (We skip writing to pg_listener when doing a self-NOTIFY, so the
 *	  notification field should never be equal to the listenerPID field.)
 *
 * 3. The NOTIFY statement itself (routine Async_Notify) just adds the target
 *	  relname to a list of outstanding NOTIFY requests.  Actual processing
 *	  happens if and only if we reach transaction commit.  At that time (in
 *	  routine AtCommit_Notify) we scan pg_listener for matching relnames.
 *	  If the listenerPID in a matching tuple is ours, we just send a notify
 *	  message to our own front end.  If it is not ours, and "notification"
 *	  is not already nonzero, we set notification to our own PID and send a
 *	  SIGUSR2 signal to the receiving process (indicated by listenerPID).
 *	  BTW: if the signal operation fails, we presume that the listener backend
 *	  crashed without removing this tuple, and remove the tuple for it.
 *
 * 4. Upon receipt of a SIGUSR2 signal, the signal handler can call inbound-
 *	  notify processing immediately if this backend is idle (ie, it is
 *	  waiting for a frontend command and is not within a transaction block).
 *	  Otherwise the handler may only set a flag, which will cause the
 *	  processing to occur just before we next go idle.
 *
 * 5. Inbound-notify processing consists of scanning pg_listener for tuples
 *	  matching our own listenerPID and having nonzero notification fields.
 *	  For each such tuple, we send a message to our frontend and clear the
 *	  notification field.  BTW: this routine has to start/commit its own
 *	  transaction, since by assumption it is only called from outside any
 *	  transaction.
 *
 * Although we grab AccessExclusiveLock on pg_listener for any operation,
 * the lock is never held very long, so it shouldn't cause too much of
 * a performance problem.
 *
 * An application that listens on the same relname it notifies will get
 * NOTIFY messages for its own NOTIFYs.  These can be ignored, if not useful,
 * by comparing be_pid in the NOTIFY message to the application's own backend's
 * PID.  (As of FE/BE protocol 2.0, the backend's PID is provided to the
 * frontend during startup.)  The above design guarantees that notifies from
 * other backends will never be missed by ignoring self-notifies.  Note,
 * however, that we do *not* guarantee that a separate frontend message will
 * be sent for every outside NOTIFY.  Since there is only room for one
 * originating PID in pg_listener, outside notifies occurring at about the
 * same time may be collapsed into a single message bearing the PID of the
 * first outside backend to perform the NOTIFY.
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "access/heapam.h"
#include "catalog/catname.h"
#include "catalog/pg_listener.h"
#include "commands/async.h"
#include "libpq/libpq.h"
#include "libpq/pqformat.h"
#include "miscadmin.h"
#include "tcop/tcopprot.h"
#include "utils/fmgroids.h"
#include "utils/ps_status.h"
#include "utils/syscache.h"


/* stuff that we really ought not be touching directly :-( */
extern TransactionState CurrentTransactionState;


/*
 * State for outbound notifies consists of a list of all relnames NOTIFYed
 * in the current transaction.	We do not actually perform a NOTIFY until
 * and unless the transaction commits.	pendingNotifies is NIL if no
 * NOTIFYs have been done in the current transaction.  The List nodes and
 * referenced strings are all palloc'd in TopTransactionContext.
 */
static List *pendingNotifies = NIL;

/*
 * State for inbound notifies consists of two flags: one saying whether
 * the signal handler is currently allowed to call ProcessIncomingNotify
 * directly, and one saying whether the signal has occurred but the handler
 * was not allowed to call ProcessIncomingNotify at the time.
 *
 * NB: the "volatile" on these declarations is critical!  If your compiler
 * does not grok "volatile", you'd be best advised to compile this file
 * with all optimization turned off.
 */
static volatile int notifyInterruptEnabled = 0;
static volatile int notifyInterruptOccurred = 0;

/* True if we've registered an on_shmem_exit cleanup */
static bool unlistenExitRegistered = false;

bool		Trace_notify = false;


static void Async_UnlistenAll(void);
static void Async_UnlistenOnExit(void);
static void ProcessIncomingNotify(void);
static void NotifyMyFrontEnd(char *relname, int32 listenerPID);
static bool AsyncExistsPendingNotify(const char *relname);
static void ClearPendingNotifies(void);


/*
 *--------------------------------------------------------------
 * Async_Notify
 *
 *		This is executed by the SQL notify command.
 *
 *		Adds the relation to the list of pending notifies.
 *		Actual notification happens during transaction commit.
 *		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 * Results:
 *		XXX
 *
 *--------------------------------------------------------------
 */
void
Async_Notify(char *relname)
{
	if (Trace_notify)
		elog(DEBUG, "Async_Notify: %s", relname);

	/* no point in making duplicate entries in the list ... */
	if (!AsyncExistsPendingNotify(relname))
	{
		/*
		 * The name list needs to live until end of transaction, so store
		 * it in the top transaction context.
		 */
		MemoryContext oldcontext;

		oldcontext = MemoryContextSwitchTo(TopTransactionContext);

		pendingNotifies = lcons(pstrdup(relname), pendingNotifies);

		MemoryContextSwitchTo(oldcontext);
	}
}

/*
 *--------------------------------------------------------------
 * Async_Listen
 *
 *		This is executed by the SQL listen command.
 *
 *		Register a backend (identified by its Unix PID) as listening
 *		on the specified relation.
 *
 * Results:
 *		XXX
 *
 * Side effects:
 *		pg_listener is updated.
 *
 *--------------------------------------------------------------
 */
void
Async_Listen(char *relname, int pid)
{
	Relation	lRel;
	HeapScanDesc scan;
	HeapTuple	tuple;
	Datum		values[Natts_pg_listener];
	char		nulls[Natts_pg_listener];
	int			i;
	bool		alreadyListener = false;

	if (Trace_notify)
		elog(DEBUG, "Async_Listen: %s", relname);

	lRel = heap_openr(ListenerRelationName, AccessExclusiveLock);

	/* Detect whether we are already listening on this relname */
	scan = heap_beginscan(lRel, 0, SnapshotNow, 0, (ScanKey) NULL);
	while (HeapTupleIsValid(tuple = heap_getnext(scan, 0)))
	{
		Form_pg_listener listener = (Form_pg_listener) GETSTRUCT(tuple);

		if (listener->listenerpid == pid &&
		  strncmp(NameStr(listener->relname), relname, NAMEDATALEN) == 0)
		{
			alreadyListener = true;
			/* No need to scan the rest of the table */
			break;
		}
	}
	heap_endscan(scan);

	if (alreadyListener)
	{
		heap_close(lRel, AccessExclusiveLock);
		elog(NOTICE, "Async_Listen: We are already listening on %s", relname);
		return;
	}

	/*
	 * OK to insert a new tuple
	 */

	for (i = 0; i < Natts_pg_listener; i++)
	{
		nulls[i] = ' ';
		values[i] = PointerGetDatum(NULL);
	}

	i = 0;
	values[i++] = (Datum) relname;
	values[i++] = (Datum) pid;
	values[i++] = (Datum) 0;	/* no notifies pending */

	tuple = heap_formtuple(RelationGetDescr(lRel), values, nulls);
	heap_insert(lRel, tuple);

#ifdef NOT_USED					/* currently there are no indexes */
	if (RelationGetForm(lRel)->relhasindex)
	{
		Relation	idescs[Num_pg_listener_indices];

		CatalogOpenIndices(Num_pg_listener_indices, Name_pg_listener_indices, idescs);
		CatalogIndexInsert(idescs, Num_pg_listener_indices, lRel, tuple);
		CatalogCloseIndices(Num_pg_listener_indices, idescs);
	}
#endif

	heap_freetuple(tuple);

	heap_close(lRel, AccessExclusiveLock);

	/*
	 * now that we are listening, make sure we will unlisten before dying.
	 */
	if (!unlistenExitRegistered)
	{
		on_shmem_exit(Async_UnlistenOnExit, 0);
		unlistenExitRegistered = true;
	}
}

/*
 *--------------------------------------------------------------
 * Async_Unlisten
 *
 *		This is executed by the SQL unlisten command.
 *
 *		Remove the backend from the list of listening backends
 *		for the specified relation.
 *
 * Results:
 *		XXX
 *
 * Side effects:
 *		pg_listener is updated.
 *
 *--------------------------------------------------------------
 */
void
Async_Unlisten(char *relname, int pid)
{
	Relation	lRel;
	HeapScanDesc scan;
	HeapTuple	tuple;

	/* Handle specially the `unlisten "*"' command */
	if ((!relname) || (*relname == '\0') || (strcmp(relname, "*") == 0))
	{
		Async_UnlistenAll();
		return;
	}

	if (Trace_notify)
		elog(DEBUG, "Async_Unlisten %s", relname);

	lRel = heap_openr(ListenerRelationName, AccessExclusiveLock);

	scan = heap_beginscan(lRel, 0, SnapshotNow, 0, (ScanKey) NULL);
	while (HeapTupleIsValid(tuple = heap_getnext(scan, 0)))
	{
		Form_pg_listener listener = (Form_pg_listener) GETSTRUCT(tuple);

		if (listener->listenerpid == pid &&
		  strncmp(NameStr(listener->relname), relname, NAMEDATALEN) == 0)
		{
			/* Found the matching tuple, delete it */
			simple_heap_delete(lRel, &tuple->t_self);

			/*
			 * We assume there can be only one match, so no need to scan
			 * the rest of the table
			 */
			break;
		}
	}
	heap_endscan(scan);

	heap_close(lRel, AccessExclusiveLock);

	/*
	 * We do not complain about unlistening something not being listened;
	 * should we?
	 */
}

/*
 *--------------------------------------------------------------
 * Async_UnlistenAll
 *
 *		Unlisten all relations for this backend.
 *
 *		This is invoked by UNLISTEN "*" command, and also at backend exit.
 *
 * Results:
 *		XXX
 *
 * Side effects:
 *		pg_listener is updated.
 *
 *--------------------------------------------------------------
 */
static void
Async_UnlistenAll(void)
{
	Relation	lRel;
	TupleDesc	tdesc;
	HeapScanDesc scan;
	HeapTuple	lTuple;
	ScanKeyData key[1];

	if (Trace_notify)
		elog(DEBUG, "Async_UnlistenAll");

	lRel = heap_openr(ListenerRelationName, AccessExclusiveLock);
	tdesc = RelationGetDescr(lRel);

	/* Find and delete all entries with my listenerPID */
	ScanKeyEntryInitialize(&key[0], 0,
						   Anum_pg_listener_pid,
						   F_INT4EQ,
						   Int32GetDatum(MyProcPid));
	scan = heap_beginscan(lRel, 0, SnapshotNow, 1, key);

	while (HeapTupleIsValid(lTuple = heap_getnext(scan, 0)))
		simple_heap_delete(lRel, &lTuple->t_self);

	heap_endscan(scan);
	heap_close(lRel, AccessExclusiveLock);
}

/*
 *--------------------------------------------------------------
 * Async_UnlistenOnExit
 *
 *		Clean up the pg_listener table at backend exit.
 *
 *		This is executed if we have done any LISTENs in this backend.
 *		It might not be necessary anymore, if the user UNLISTENed everything,
 *		but we don't try to detect that case.
 *
 * Results:
 *		XXX
 *
 * Side effects:
 *		pg_listener is updated if necessary.
 *
 *--------------------------------------------------------------
 */
static void
Async_UnlistenOnExit(void)
{
	/*
	 * We need to start/commit a transaction for the unlisten, but if
	 * there is already an active transaction we had better abort that one
	 * first.  Otherwise we'd end up committing changes that probably
	 * ought to be discarded.
	 */
	AbortOutOfAnyTransaction();
	/* Now we can do the unlisten */
	StartTransactionCommand();
	Async_UnlistenAll();
	CommitTransactionCommand();
}

/*
 *--------------------------------------------------------------
 * AtCommit_Notify
 *
 *		This is called at transaction commit.
 *
 *		If there are outbound notify requests in the pendingNotifies list,
 *		scan pg_listener for matching tuples, and either signal the other
 *		backend or send a message to our own frontend.
 *
 *		NOTE: we are still inside the current transaction, therefore can
 *		piggyback on its committing of changes.
 *
 * Results:
 *		XXX
 *
 * Side effects:
 *		Tuples in pg_listener that have matching relnames and other peoples'
 *		listenerPIDs are updated with a nonzero notification field.
 *
 *--------------------------------------------------------------
 */
void
AtCommit_Notify(void)
{
	Relation	lRel;
	TupleDesc	tdesc;
	HeapScanDesc scan;
	HeapTuple	lTuple,
				rTuple;
	Datum		value[Natts_pg_listener];
	char		repl[Natts_pg_listener],
				nulls[Natts_pg_listener];

	if (pendingNotifies == NIL)
		return;					/* no NOTIFY statements in this
								 * transaction */

	/*
	 * NOTIFY is disabled if not normal processing mode. This test used to
	 * be in xact.c, but it seems cleaner to do it here.
	 */
	if (!IsNormalProcessingMode())
	{
		ClearPendingNotifies();
		return;
	}

	if (Trace_notify)
		elog(DEBUG, "AtCommit_Notify");

	/* preset data to update notify column to MyProcPid */
	nulls[0] = nulls[1] = nulls[2] = ' ';
	repl[0] = repl[1] = repl[2] = ' ';
	repl[Anum_pg_listener_notify - 1] = 'r';
	value[0] = value[1] = value[2] = (Datum) 0;
	value[Anum_pg_listener_notify - 1] = Int32GetDatum(MyProcPid);

	lRel = heap_openr(ListenerRelationName, AccessExclusiveLock);
	tdesc = RelationGetDescr(lRel);
	scan = heap_beginscan(lRel, 0, SnapshotNow, 0, (ScanKey) NULL);

	while (HeapTupleIsValid(lTuple = heap_getnext(scan, 0)))
	{
		Form_pg_listener listener = (Form_pg_listener) GETSTRUCT(lTuple);
		char	   *relname = NameStr(listener->relname);
		int32		listenerPID = listener->listenerpid;

		if (!AsyncExistsPendingNotify(relname))
			continue;

		if (listenerPID == MyProcPid)
		{
			/*
			 * Self-notify: no need to bother with table update. Indeed,
			 * we *must not* clear the notification field in this path, or
			 * we could lose an outside notify, which'd be bad for
			 * applications that ignore self-notify messages.
			 */

			if (Trace_notify)
				elog(DEBUG, "AtCommit_Notify: notifying self");

			NotifyMyFrontEnd(relname, listenerPID);
		}
		else
		{
			if (Trace_notify)
				elog(DEBUG, "AtCommit_Notify: notifying pid %d",
					 listenerPID);

			/*
			 * If someone has already notified this listener, we don't
			 * bother modifying the table, but we do still send a SIGUSR2
			 * signal, just in case that backend missed the earlier signal
			 * for some reason.  It's OK to send the signal first, because
			 * the other guy can't read pg_listener until we unlock it.
			 */
			if (kill(listenerPID, SIGUSR2) < 0)
			{
				/*
				 * Get rid of pg_listener entry if it refers to a PID that
				 * no longer exists.  Presumably, that backend crashed
				 * without deleting its pg_listener entries. This code
				 * used to only delete the entry if errno==ESRCH, but as
				 * far as I can see we should just do it for any failure
				 * (certainly at least for EPERM too...)
				 */
				simple_heap_delete(lRel, &lTuple->t_self);
			}
			else if (listener->notification == 0)
			{
				rTuple = heap_modifytuple(lTuple, lRel,
										  value, nulls, repl);
				simple_heap_update(lRel, &lTuple->t_self, rTuple);

#ifdef NOT_USED					/* currently there are no indexes */
				if (RelationGetForm(lRel)->relhasindex)
				{
					Relation	idescs[Num_pg_listener_indices];

					CatalogOpenIndices(Num_pg_listener_indices, Name_pg_listener_indices, idescs);
					CatalogIndexInsert(idescs, Num_pg_listener_indices, lRel, rTuple);
					CatalogCloseIndices(Num_pg_listener_indices, idescs);
				}
#endif
			}
		}
	}

	heap_endscan(scan);

	/*
	 * We do NOT release the lock on pg_listener here; we need to hold it
	 * until end of transaction (which is about to happen, anyway) to
	 * ensure that notified backends see our tuple updates when they look.
	 * Else they might disregard the signal, which would make the
	 * application programmer very unhappy.
	 */
	heap_close(lRel, NoLock);

	ClearPendingNotifies();

	if (Trace_notify)
		elog(DEBUG, "AtCommit_Notify: done");
}

/*
 *--------------------------------------------------------------
 * AtAbort_Notify
 *
 *		This is called at transaction abort.
 *
 *		Gets rid of pending outbound notifies that we would have executed
 *		if the transaction got committed.
 *
 * Results:
 *		XXX
 *
 *--------------------------------------------------------------
 */
void
AtAbort_Notify(void)
{
	ClearPendingNotifies();
}

/*
 *--------------------------------------------------------------
 * Async_NotifyHandler
 *
 *		This is the signal handler for SIGUSR2.
 *
 *		If we are idle (notifyInterruptEnabled is set), we can safely invoke
 *		ProcessIncomingNotify directly.  Otherwise, just set a flag
 *		to do it later.
 *
 * Results:
 *		none
 *
 * Side effects:
 *		per above
 *--------------------------------------------------------------
 */
void
Async_NotifyHandler(SIGNAL_ARGS)
{
	int			save_errno = errno;

	/*
	 * Note: this is a SIGNAL HANDLER.	You must be very wary what you do
	 * here. Some helpful soul had this routine sprinkled with TPRINTFs,
	 * which would likely lead to corruption of stdio buffers if they were
	 * ever turned on.
	 */

	if (notifyInterruptEnabled)
	{
		/*
		 * I'm not sure whether some flavors of Unix might allow another
		 * SIGUSR2 occurrence to recursively interrupt this routine. To
		 * cope with the possibility, we do the same sort of dance that
		 * EnableNotifyInterrupt must do --- see that routine for
		 * comments.
		 */
		notifyInterruptEnabled = 0;		/* disable any recursive signal */
		notifyInterruptOccurred = 1;	/* do at least one iteration */
		for (;;)
		{
			notifyInterruptEnabled = 1;
			if (!notifyInterruptOccurred)
				break;
			notifyInterruptEnabled = 0;
			if (notifyInterruptOccurred)
			{
				/* Here, it is finally safe to do stuff. */
				if (Trace_notify)
					elog(DEBUG, "Async_NotifyHandler: perform async notify");

				ProcessIncomingNotify();

				if (Trace_notify)
					elog(DEBUG, "Async_NotifyHandler: done");
			}
		}
	}
	else
	{
		/*
		 * In this path it is NOT SAFE to do much of anything, except
		 * this:
		 */
		notifyInterruptOccurred = 1;
	}

	errno = save_errno;
}

/*
 * --------------------------------------------------------------
 * EnableNotifyInterrupt
 *
 *		This is called by the PostgresMain main loop just before waiting
 *		for a frontend command.  If we are truly idle (ie, *not* inside
 *		a transaction block), then process any pending inbound notifies,
 *		and enable the signal handler to process future notifies directly.
 *
 *		NOTE: the signal handler starts out disabled, and stays so until
 *		PostgresMain calls this the first time.
 * --------------------------------------------------------------
 */
void
EnableNotifyInterrupt(void)
{
	if (CurrentTransactionState->blockState != TRANS_DEFAULT)
		return;					/* not really idle */

	/*
	 * This code is tricky because we are communicating with a signal
	 * handler that could interrupt us at any point.  If we just checked
	 * notifyInterruptOccurred and then set notifyInterruptEnabled, we
	 * could fail to respond promptly to a signal that happens in between
	 * those two steps.  (A very small time window, perhaps, but Murphy's
	 * Law says you can hit it...)	Instead, we first set the enable flag,
	 * then test the occurred flag.  If we see an unserviced interrupt has
	 * occurred, we re-clear the enable flag before going off to do the
	 * service work.  (That prevents re-entrant invocation of
	 * ProcessIncomingNotify() if another interrupt occurs.) If an
	 * interrupt comes in between the setting and clearing of
	 * notifyInterruptEnabled, then it will have done the service work and
	 * left notifyInterruptOccurred zero, so we have to check again after
	 * clearing enable.  The whole thing has to be in a loop in case
	 * another interrupt occurs while we're servicing the first. Once we
	 * get out of the loop, enable is set and we know there is no
	 * unserviced interrupt.
	 *
	 * NB: an overenthusiastic optimizing compiler could easily break this
	 * code.  Hopefully, they all understand what "volatile" means these
	 * days.
	 */
	for (;;)
	{
		notifyInterruptEnabled = 1;
		if (!notifyInterruptOccurred)
			break;
		notifyInterruptEnabled = 0;
		if (notifyInterruptOccurred)
		{
			if (Trace_notify)
				elog(DEBUG, "EnableNotifyInterrupt: perform async notify");

			ProcessIncomingNotify();

			if (Trace_notify)
				elog(DEBUG, "EnableNotifyInterrupt: done");
		}
	}
}

/*
 * --------------------------------------------------------------
 * DisableNotifyInterrupt
 *
 *		This is called by the PostgresMain main loop just after receiving
 *		a frontend command.  Signal handler execution of inbound notifies
 *		is disabled until the next EnableNotifyInterrupt call.
 * --------------------------------------------------------------
 */
void
DisableNotifyInterrupt(void)
{
	notifyInterruptEnabled = 0;
}

/*
 * --------------------------------------------------------------
 * ProcessIncomingNotify
 *
 *		Deal with arriving NOTIFYs from other backends.
 *		This is called either directly from the SIGUSR2 signal handler,
 *		or the next time control reaches the outer idle loop.
 *		Scan pg_listener for arriving notifies, report them to my front end,
 *		and clear the notification field in pg_listener until next time.
 *
 *		NOTE: since we are outside any transaction, we must create our own.
 *
 * Results:
 *		XXX
 *
 * --------------------------------------------------------------
 */
static void
ProcessIncomingNotify(void)
{
	Relation	lRel;
	TupleDesc	tdesc;
	ScanKeyData key[1];
	HeapScanDesc scan;
	HeapTuple	lTuple,
				rTuple;
	Datum		value[Natts_pg_listener];
	char		repl[Natts_pg_listener],
				nulls[Natts_pg_listener];

	if (Trace_notify)
		elog(DEBUG, "ProcessIncomingNotify");

	set_ps_display("async_notify");

	notifyInterruptOccurred = 0;

	StartTransactionCommand();

	lRel = heap_openr(ListenerRelationName, AccessExclusiveLock);
	tdesc = RelationGetDescr(lRel);

	/* Scan only entries with my listenerPID */
	ScanKeyEntryInitialize(&key[0], 0,
						   Anum_pg_listener_pid,
						   F_INT4EQ,
						   Int32GetDatum(MyProcPid));
	scan = heap_beginscan(lRel, 0, SnapshotNow, 1, key);

	/* Prepare data for rewriting 0 into notification field */
	nulls[0] = nulls[1] = nulls[2] = ' ';
	repl[0] = repl[1] = repl[2] = ' ';
	repl[Anum_pg_listener_notify - 1] = 'r';
	value[0] = value[1] = value[2] = (Datum) 0;
	value[Anum_pg_listener_notify - 1] = Int32GetDatum(0);

	while (HeapTupleIsValid(lTuple = heap_getnext(scan, 0)))
	{
		Form_pg_listener listener = (Form_pg_listener) GETSTRUCT(lTuple);
		char	   *relname = NameStr(listener->relname);
		int32		sourcePID = listener->notification;

		if (sourcePID != 0)
		{
			/* Notify the frontend */

			if (Trace_notify)
				elog(DEBUG, "ProcessIncomingNotify: received %s from %d",
					 relname, (int) sourcePID);

			NotifyMyFrontEnd(relname, sourcePID);
			/* Rewrite the tuple with 0 in notification column */
			rTuple = heap_modifytuple(lTuple, lRel, value, nulls, repl);
			simple_heap_update(lRel, &lTuple->t_self, rTuple);

#ifdef NOT_USED					/* currently there are no indexes */
			if (RelationGetForm(lRel)->relhasindex)
			{
				Relation	idescs[Num_pg_listener_indices];

				CatalogOpenIndices(Num_pg_listener_indices, Name_pg_listener_indices, idescs);
				CatalogIndexInsert(idescs, Num_pg_listener_indices, lRel, rTuple);
				CatalogCloseIndices(Num_pg_listener_indices, idescs);
			}
#endif
		}
	}
	heap_endscan(scan);

	/*
	 * We do NOT release the lock on pg_listener here; we need to hold it
	 * until end of transaction (which is about to happen, anyway) to
	 * ensure that other backends see our tuple updates when they look.
	 * Otherwise, a transaction started after this one might mistakenly
	 * think it doesn't need to send this backend a new NOTIFY.
	 */
	heap_close(lRel, NoLock);

	CommitTransactionCommand();

	/*
	 * Must flush the notify messages to ensure frontend gets them
	 * promptly.
	 */
	pq_flush();

	set_ps_display("idle");

	if (Trace_notify)
		elog(DEBUG, "ProcessIncomingNotify: done");
}

/*
 * Send NOTIFY message to my front end.
 */
static void
NotifyMyFrontEnd(char *relname, int32 listenerPID)
{
	if (whereToSendOutput == Remote)
	{
		StringInfoData buf;

		pq_beginmessage(&buf);
		pq_sendbyte(&buf, 'A');
		pq_sendint(&buf, listenerPID, sizeof(int32));
		pq_sendstring(&buf, relname);
		pq_endmessage(&buf);

		/*
		 * NOTE: we do not do pq_flush() here.	For a self-notify, it will
		 * happen at the end of the transaction, and for incoming notifies
		 * ProcessIncomingNotify will do it after finding all the
		 * notifies.
		 */
	}
	else
		elog(NOTICE, "NOTIFY for %s", relname);
}

/* Does pendingNotifies include the given relname? */
static bool
AsyncExistsPendingNotify(const char *relname)
{
	List	   *p;

	foreach(p, pendingNotifies)
	{
		/* Use NAMEDATALEN for relname comparison.	  DZ - 26-08-1996 */
		if (strncmp((const char *) lfirst(p), relname, NAMEDATALEN) == 0)
			return true;
	}

	return false;
}

/* Clear the pendingNotifies list. */
static void
ClearPendingNotifies(void)
{
	/*
	 * We used to have to explicitly deallocate the list members and
	 * nodes, because they were malloc'd.  Now, since we know they are
	 * palloc'd in TopTransactionContext, we need not do that --- they'll
	 * go away automatically at transaction exit.  We need only reset the
	 * list head pointer.
	 */
	pendingNotifies = NIL;
}
