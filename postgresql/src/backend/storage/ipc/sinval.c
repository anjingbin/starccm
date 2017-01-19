/*-------------------------------------------------------------------------
 *
 * sinval.c
 *	  POSTGRES shared cache invalidation communication code.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/storage/ipc/sinval.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <sys/types.h>

#include "storage/proc.h"
#include "storage/sinval.h"
#include "storage/sinvaladt.h"
#include "utils/tqual.h"
#include "miscadmin.h"


/****************************************************************************/
/*	CreateSharedInvalidationState()		 Initialize SI buffer				*/
/*																			*/
/*	should be called only by the POSTMASTER									*/
/****************************************************************************/
void
CreateSharedInvalidationState(int maxBackends)
{
	/* SInvalLock must be initialized already, during LWLock init */
	SIBufferInit(maxBackends);
}

/*
 * InitBackendSharedInvalidationState
 *		Initialize new backend's state info in buffer segment.
 */
void
InitBackendSharedInvalidationState(void)
{
	int			flag;

	LWLockAcquire(SInvalLock, LW_EXCLUSIVE);
	flag = SIBackendInit(shmInvalBuffer);
	LWLockRelease(SInvalLock);
	if (flag < 0)				/* unexpected problem */
		elog(FATAL, "Backend cache invalidation initialization failed");
	if (flag == 0)				/* expected problem: MaxBackends exceeded */
		elog(FATAL, "Sorry, too many clients already");
}

/*
 * SendSharedInvalidMessage
 *	Add a shared-cache-invalidation message to the global SI message queue.
 */
void
SendSharedInvalidMessage(SharedInvalidationMessage *msg)
{
	bool		insertOK;

	LWLockAcquire(SInvalLock, LW_EXCLUSIVE);
	insertOK = SIInsertDataEntry(shmInvalBuffer, msg);
	LWLockRelease(SInvalLock);
	if (!insertOK)
		elog(DEBUG, "SendSharedInvalidMessage: SI buffer overflow");
}

/*
 * ReceiveSharedInvalidMessages
 *		Process shared-cache-invalidation messages waiting for this backend
 */
void
ReceiveSharedInvalidMessages(
				  void (*invalFunction) (SharedInvalidationMessage *msg),
							 void (*resetFunction) (void))
{
	SharedInvalidationMessage data;
	int			getResult;
	bool		gotMessage = false;

	for (;;)
	{
		/*
		 * We can run SIGetDataEntry in parallel with other backends
		 * running SIGetDataEntry for themselves, since each instance will
		 * modify only fields of its own backend's ProcState, and no
		 * instance will look at fields of other backends' ProcStates.  We
		 * express this by grabbing SInvalLock in shared mode.	Note that
		 * this is not exactly the normal (read-only) interpretation of a
		 * shared lock! Look closely at the interactions before allowing
		 * SInvalLock to be grabbed in shared mode for any other reason!
		 *
		 * The routines later in this file that use shared mode are okay with
		 * this, because they aren't looking at the ProcState fields
		 * associated with SI message transfer; they only use the
		 * ProcState array as an easy way to find all the PROC structures.
		 */
		LWLockAcquire(SInvalLock, LW_SHARED);
		getResult = SIGetDataEntry(shmInvalBuffer, MyBackendId, &data);
		LWLockRelease(SInvalLock);

		if (getResult == 0)
			break;				/* nothing more to do */
		if (getResult < 0)
		{
			/* got a reset message */
			elog(DEBUG, "ReceiveSharedInvalidMessages: cache state reset");
			resetFunction();
		}
		else
		{
			/* got a normal data message */
			invalFunction(&data);
		}
		gotMessage = true;
	}

	/* If we got any messages, try to release dead messages */
	if (gotMessage)
	{
		LWLockAcquire(SInvalLock, LW_EXCLUSIVE);
		SIDelExpiredDataEntries(shmInvalBuffer);
		LWLockRelease(SInvalLock);
	}
}


/****************************************************************************/
/* Functions that need to scan the PROC structures of all running backends. */
/* It's a bit strange to keep these in sinval.c, since they don't have any	*/
/* direct relationship to shared-cache invalidation.  But the procState		*/
/* array in the SI segment is the only place in the system where we have	*/
/* an array of per-backend data, so it is the most convenient place to keep */
/* pointers to the backends' PROC structures.  We used to implement these	*/
/* functions with a slow, ugly search through the ShmemIndex hash table --- */
/* now they are simple loops over the SI ProcState array.					*/
/****************************************************************************/


/*
 * DatabaseHasActiveBackends -- are there any backends running in the given DB
 *
 * If 'ignoreMyself' is TRUE, ignore this particular backend while checking
 * for backends in the target database.
 *
 * This function is used to interlock DROP DATABASE against there being
 * any active backends in the target DB --- dropping the DB while active
 * backends remain would be a Bad Thing.  Note that we cannot detect here
 * the possibility of a newly-started backend that is trying to connect
 * to the doomed database, so additional interlocking is needed during
 * backend startup.
 */

bool
DatabaseHasActiveBackends(Oid databaseId, bool ignoreMyself)
{
	bool		result = false;
	SISeg	   *segP = shmInvalBuffer;
	ProcState  *stateP = segP->procState;
	int			index;

	LWLockAcquire(SInvalLock, LW_SHARED);

	for (index = 0; index < segP->lastBackend; index++)
	{
		SHMEM_OFFSET pOffset = stateP[index].procStruct;

		if (pOffset != INVALID_OFFSET)
		{
			PROC	   *proc = (PROC *) MAKE_PTR(pOffset);

			if (proc->databaseId == databaseId)
			{
				if (ignoreMyself && proc == MyProc)
					continue;

				result = true;
				break;
			}
		}
	}

	LWLockRelease(SInvalLock);

	return result;
}

/*
 * TransactionIdIsInProgress -- is given transaction running by some backend
 */
bool
TransactionIdIsInProgress(TransactionId xid)
{
	bool		result = false;
	SISeg	   *segP = shmInvalBuffer;
	ProcState  *stateP = segP->procState;
	int			index;

	LWLockAcquire(SInvalLock, LW_SHARED);

	for (index = 0; index < segP->lastBackend; index++)
	{
		SHMEM_OFFSET pOffset = stateP[index].procStruct;

		if (pOffset != INVALID_OFFSET)
		{
			PROC	   *proc = (PROC *) MAKE_PTR(pOffset);

			/* Fetch xid just once - see GetNewTransactionId */
			TransactionId pxid = proc->xid;

			if (TransactionIdEquals(pxid, xid))
			{
				result = true;
				break;
			}
		}
	}

	LWLockRelease(SInvalLock);

	return result;
}

/*
 * GetOldestXmin -- returns oldest transaction that was running
 *					when any current transaction was started.
 *
 * If allDbs is TRUE then all backends are considered; if allDbs is FALSE
 * then only backends running in my own database are considered.
 *
 * This is used by VACUUM to decide which deleted tuples must be preserved
 * in a table.	allDbs = TRUE is needed for shared relations, but allDbs =
 * FALSE is sufficient for non-shared relations, since only backends in my
 * own database could ever see the tuples in them.
 *
 * Note: we include the currently running xids in the set of considered xids.
 * This ensures that if a just-started xact has not yet set its snapshot,
 * when it does set the snapshot it cannot set xmin less than what we compute.
 */
TransactionId
GetOldestXmin(bool allDbs)
{
	SISeg	   *segP = shmInvalBuffer;
	ProcState  *stateP = segP->procState;
	TransactionId result;
	int			index;

	result = GetCurrentTransactionId();

	LWLockAcquire(SInvalLock, LW_SHARED);

	for (index = 0; index < segP->lastBackend; index++)
	{
		SHMEM_OFFSET pOffset = stateP[index].procStruct;

		if (pOffset != INVALID_OFFSET)
		{
			PROC	   *proc = (PROC *) MAKE_PTR(pOffset);

			if (allDbs || proc->databaseId == MyDatabaseId)
			{
				/* Fetch xid just once - see GetNewTransactionId */
				TransactionId xid = proc->xid;

				if (TransactionIdIsNormal(xid))
				{
					if (TransactionIdPrecedes(xid, result))
						result = xid;
					xid = proc->xmin;
					if (TransactionIdIsNormal(xid))
						if (TransactionIdPrecedes(xid, result))
							result = xid;
				}
			}
		}
	}

	LWLockRelease(SInvalLock);

	return result;
}

/*----------
 * GetSnapshotData -- returns information about running transactions.
 *
 * The returned snapshot includes xmin (lowest still-running xact ID),
 * xmax (next xact ID to be assigned), and a list of running xact IDs
 * in the range xmin <= xid < xmax.  It is used as follows:
 *		All xact IDs < xmin are considered finished.
 *		All xact IDs >= xmax are considered still running.
 *		For an xact ID xmin <= xid < xmax, consult list to see whether
 *		it is considered running or not.
 * This ensures that the set of transactions seen as "running" by the
 * current xact will not change after it takes the snapshot.
 *----------
 */
Snapshot
GetSnapshotData(bool serializable)
{
	Snapshot	snapshot = (Snapshot) malloc(sizeof(SnapshotData));
	SISeg	   *segP = shmInvalBuffer;
	ProcState  *stateP = segP->procState;
	int			index;
	int			count = 0;

	if (snapshot == NULL)
		elog(ERROR, "Memory exhausted in GetSnapshotData");

	/*
	 * Allocating space for MaxBackends xids is usually overkill;
	 * lastBackend would be sufficient.  But it seems better to do the
	 * malloc while not holding the lock, so we can't look at lastBackend.
	 */
	snapshot->xip = (TransactionId *)
		malloc(MaxBackends * sizeof(TransactionId));
	if (snapshot->xip == NULL)
		elog(ERROR, "Memory exhausted in GetSnapshotData");

	snapshot->xmin = GetCurrentTransactionId();

	/*
	 * If we are going to set MyProc->xmin then we'd better get exclusive
	 * lock; if not, this is a read-only operation so it can be shared.
	 */
	LWLockAcquire(SInvalLock, serializable ? LW_EXCLUSIVE : LW_SHARED);

	/*--------------------
	 * Unfortunately, we have to call ReadNewTransactionId() after acquiring
	 * SInvalLock above.  It's not good because ReadNewTransactionId() does
	 * LWLockAcquire(XidGenLock), but *necessary*.	We need to be sure that
	 * no transactions exit the set of currently-running transactions
	 * between the time we fetch xmax and the time we finish building our
	 * snapshot.  Otherwise we could have a situation like this:
	 *
	 *		1. Tx Old is running (in Read Committed mode).
	 *		2. Tx S reads new transaction ID into xmax, then
	 *		   is swapped out before acquiring SInvalLock.
	 *		3. Tx New gets new transaction ID (>= S' xmax),
	 *		   makes changes and commits.
	 *		4. Tx Old changes some row R changed by Tx New and commits.
	 *		5. Tx S finishes getting its snapshot data.  It sees Tx Old as
	 *		   done, but sees Tx New as still running (since New >= xmax).
	 *
	 * Now S will see R changed by both Tx Old and Tx New, *but* does not
	 * see other changes made by Tx New.  If S is supposed to be in
	 * Serializable mode, this is wrong.
	 *
	 * By locking SInvalLock before we read xmax, we ensure that TX Old
	 * cannot exit the set of running transactions seen by Tx S.  Therefore
	 * both Old and New will be seen as still running => no inconsistency.
	 *--------------------
	 */

	snapshot->xmax = ReadNewTransactionId();

	for (index = 0; index < segP->lastBackend; index++)
	{
		SHMEM_OFFSET pOffset = stateP[index].procStruct;

		if (pOffset != INVALID_OFFSET)
		{
			PROC	   *proc = (PROC *) MAKE_PTR(pOffset);

			/* Fetch xid just once - see GetNewTransactionId */
			TransactionId xid = proc->xid;

			/*
			 * Ignore my own proc (dealt with my xid above), procs not
			 * running a transaction, and xacts started since we read the
			 * next transaction ID.  There's no need to store XIDs above
			 * what we got from ReadNewTransactionId, since we'll treat
			 * them as running anyway.
			 */
			if (proc == MyProc ||
				!TransactionIdIsNormal(xid) ||
				TransactionIdFollowsOrEquals(xid, snapshot->xmax))
				continue;

			if (TransactionIdPrecedes(xid, snapshot->xmin))
				snapshot->xmin = xid;
			snapshot->xip[count] = xid;
			count++;
		}
	}

	if (serializable)
		MyProc->xmin = snapshot->xmin;

	LWLockRelease(SInvalLock);

	/* Serializable snapshot must be computed before any other... */
	Assert(TransactionIdIsValid(MyProc->xmin));

	snapshot->xcnt = count;
	return snapshot;
}

/*
 * CountActiveBackends --- count backends (other than myself) that are in
 *		active transactions.  This is used as a heuristic to decide if
 *		a pre-XLOG-flush delay is worthwhile during commit.
 *
 * An active transaction is something that has written at least one XLOG
 * record; read-only transactions don't count.  Also, do not count backends
 * that are blocked waiting for locks, since they are not going to get to
 * run until someone else commits.
 */
int
CountActiveBackends(void)
{
	SISeg	   *segP = shmInvalBuffer;
	ProcState  *stateP = segP->procState;
	int			count = 0;
	int			index;

	/*
	 * Note: for speed, we don't acquire SInvalLock.  This is a little bit
	 * bogus, but since we are only testing xrecoff for zero or nonzero,
	 * it should be OK.  The result is only used for heuristic purposes
	 * anyway...
	 */
	for (index = 0; index < segP->lastBackend; index++)
	{
		SHMEM_OFFSET pOffset = stateP[index].procStruct;

		if (pOffset != INVALID_OFFSET)
		{
			PROC	   *proc = (PROC *) MAKE_PTR(pOffset);

			if (proc == MyProc)
				continue;		/* do not count myself */
			if (proc->logRec.xrecoff == 0)
				continue;		/* do not count if not in a transaction */
			if (proc->waitLock != NULL)
				continue;		/* do not count if blocked on a lock */
			count++;
		}
	}

	return count;
}

/*
 * GetUndoRecPtr -- returns oldest PROC->logRec.
 */
XLogRecPtr
GetUndoRecPtr(void)
{
	SISeg	   *segP = shmInvalBuffer;
	ProcState  *stateP = segP->procState;
	XLogRecPtr	urec = {0, 0};
	XLogRecPtr	tempr;
	int			index;

	LWLockAcquire(SInvalLock, LW_SHARED);

	for (index = 0; index < segP->lastBackend; index++)
	{
		SHMEM_OFFSET pOffset = stateP[index].procStruct;

		if (pOffset != INVALID_OFFSET)
		{
			PROC	   *proc = (PROC *) MAKE_PTR(pOffset);

			tempr = proc->logRec;
			if (tempr.xrecoff == 0)
				continue;
			if (urec.xrecoff != 0 && XLByteLT(urec, tempr))
				continue;
			urec = tempr;
		}
	}

	LWLockRelease(SInvalLock);

	return (urec);
}

/*
 * BackendIdGetProc - given a BackendId, find its PROC structure
 *
 * This is a trivial lookup in the ProcState array.  We assume that the caller
 * knows that the backend isn't going to go away, so we do not bother with
 * locking.
 */
struct PROC *
BackendIdGetProc(BackendId procId)
{
	SISeg	   *segP = shmInvalBuffer;

	if (procId > 0 && procId <= segP->lastBackend)
	{
		ProcState  *stateP = &segP->procState[procId - 1];
		SHMEM_OFFSET pOffset = stateP->procStruct;

		if (pOffset != INVALID_OFFSET)
		{
			PROC	   *proc = (PROC *) MAKE_PTR(pOffset);

			return proc;
		}
	}

	return NULL;
}
