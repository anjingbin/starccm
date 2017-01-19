/*-------------------------------------------------------------------------
 *
 * tqual.c
 *	  POSTGRES "time" qualification code, ie, tuple visibility rules.
 *
 * NOTE: all the HeapTupleSatisfies routines will update the tuple's
 * "hint" status bits if we see that the inserting or deleting transaction
 * has now committed or aborted.  The caller is responsible for noticing any
 * change in t_infomask and scheduling a disk write if so.  Note that the
 * caller must hold at least a shared buffer context lock on the buffer
 * containing the tuple.  (VACUUM FULL assumes it's sufficient to have
 * exclusive lock on the containing relation, instead.)
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/time/tqual.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "storage/sinval.h"
#include "utils/tqual.h"


static SnapshotData SnapshotDirtyData;
Snapshot	SnapshotDirty = &SnapshotDirtyData;

Snapshot	QuerySnapshot = NULL;
Snapshot	SerializableSnapshot = NULL;

bool		ReferentialIntegritySnapshotOverride = false;


/*
 * HeapTupleSatisfiesItself
 *		True iff heap tuple is valid "for itself".
 *
 *	Here, we consider the effects of:
 *		all committed transactions (as of the current instant)
 *		previous commands of this transaction
 *		changes made by the current command
 *
 * Note:
 *		Assumes heap tuple is valid.
 *
 * The satisfaction of "itself" requires the following:
 *
 * ((Xmin == my-transaction &&				the row was updated by the current transaction, and
 *		(Xmax is null						it was not deleted
 *		 [|| Xmax != my-transaction)])			[or it was deleted by another transaction]
 * ||
 *
 * (Xmin is committed &&					the row was modified by a committed transaction, and
 *		(Xmax is null ||					the row has not been deleted, or
 *			(Xmax != my-transaction &&			the row was deleted by another transaction
 *			 Xmax is not committed)))			that has not been committed
 */
bool
HeapTupleSatisfiesItself(HeapTupleHeader tuple)
{
	if (!(tuple->t_infomask & HEAP_XMIN_COMMITTED))
	{
		if (tuple->t_infomask & HEAP_XMIN_INVALID)
			return false;

		if (tuple->t_infomask & HEAP_MOVED_OFF)
		{
			if (TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
				return false;
			if (!TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
				tuple->t_infomask |= HEAP_XMIN_COMMITTED;
			}
		}
		else if (tuple->t_infomask & HEAP_MOVED_IN)
		{
			if (!TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
					return false;
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
					tuple->t_infomask |= HEAP_XMIN_COMMITTED;
				else
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
			}
		}
		else if (TransactionIdIsCurrentTransactionId(tuple->t_xmin))
		{
			if (tuple->t_infomask & HEAP_XMAX_INVALID)	/* xid invalid */
				return true;

			Assert(TransactionIdIsCurrentTransactionId(tuple->t_xmax));

			if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
				return true;

			return false;
		}
		else if (!TransactionIdDidCommit(tuple->t_xmin))
		{
			if (TransactionIdDidAbort(tuple->t_xmin))
				tuple->t_infomask |= HEAP_XMIN_INVALID; /* aborted */
			return false;
		}
		else
			tuple->t_infomask |= HEAP_XMIN_COMMITTED;
	}

	/* by here, the inserting transaction has committed */

	if (tuple->t_infomask & HEAP_XMAX_INVALID)	/* xid invalid or aborted */
		return true;

	if (tuple->t_infomask & HEAP_XMAX_COMMITTED)
	{
		if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
			return true;
		return false;			/* updated by other */
	}

	if (TransactionIdIsCurrentTransactionId(tuple->t_xmax))
	{
		if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
			return true;
		return false;
	}

	if (!TransactionIdDidCommit(tuple->t_xmax))
	{
		if (TransactionIdDidAbort(tuple->t_xmax))
			tuple->t_infomask |= HEAP_XMAX_INVALID;		/* aborted */
		return true;
	}

	/* xmax transaction committed */
	tuple->t_infomask |= HEAP_XMAX_COMMITTED;

	if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
		return true;

	return false;
}

/*
 * HeapTupleSatisfiesNow
 *		True iff heap tuple is valid "now".
 *
 *	Here, we consider the effects of:
 *		all committed transactions (as of the current instant)
 *		previous commands of this transaction
 *
 * Note we do _not_ include changes made by the current command.  This
 * solves the "Halloween problem" wherein an UPDATE might try to re-update
 * its own output tuples.
 *
 * Note:
 *		Assumes heap tuple is valid.
 *
 * The satisfaction of "now" requires the following:
 *
 * ((Xmin == my-transaction &&				changed by the current transaction
 *	 Cmin != my-command &&					but not by this command, and
 *		(Xmax is null ||						the row has not been deleted, or
 *			(Xmax == my-transaction &&			it was deleted by the current transaction
 *			 Cmax != my-command)))				but not by this command,
 * ||										or
 *
 *	(Xmin is committed &&					the row was modified by a committed transaction, and
 *		(Xmax is null ||					the row has not been deleted, or
 *			(Xmax == my-transaction &&			the row is being deleted by this command, or
 *			 Cmax == my-command) ||
 *			(Xmax is not committed &&			the row was deleted by another transaction
 *			 Xmax != my-transaction))))			that has not been committed
 *
 *		mao says 17 march 1993:  the tests in this routine are correct;
 *		if you think they're not, you're wrong, and you should think
 *		about it again.  i know, it happened to me.  we don't need to
 *		check commit time against the start time of this transaction
 *		because 2ph locking protects us from doing the wrong thing.
 *		if you mess around here, you'll break serializability.  the only
 *		problem with this code is that it does the wrong thing for system
 *		catalog updates, because the catalogs aren't subject to 2ph, so
 *		the serializability guarantees we provide don't extend to xacts
 *		that do catalog accesses.  this is unfortunate, but not critical.
 */
bool
HeapTupleSatisfiesNow(HeapTupleHeader tuple)
{
	if (AMI_OVERRIDE)
		return true;

	if (!(tuple->t_infomask & HEAP_XMIN_COMMITTED))
	{
		if (tuple->t_infomask & HEAP_XMIN_INVALID)
			return false;

		if (tuple->t_infomask & HEAP_MOVED_OFF)
		{
			if (TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
				return false;
			if (!TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
				tuple->t_infomask |= HEAP_XMIN_COMMITTED;
			}
		}
		else if (tuple->t_infomask & HEAP_MOVED_IN)
		{
			if (!TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
					return false;
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
					tuple->t_infomask |= HEAP_XMIN_COMMITTED;
				else
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
			}
		}
		else if (TransactionIdIsCurrentTransactionId(tuple->t_xmin))
		{
			if (CommandIdGEScanCommandId(tuple->t_cmin))
				return false;	/* inserted after scan started */

			if (tuple->t_infomask & HEAP_XMAX_INVALID)	/* xid invalid */
				return true;

			Assert(TransactionIdIsCurrentTransactionId(tuple->t_xmax));

			if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
				return true;

			if (CommandIdGEScanCommandId(tuple->t_cmax))
				return true;	/* deleted after scan started */
			else
				return false;	/* deleted before scan started */
		}
		else if (!TransactionIdDidCommit(tuple->t_xmin))
		{
			if (TransactionIdDidAbort(tuple->t_xmin))
				tuple->t_infomask |= HEAP_XMIN_INVALID; /* aborted */
			return false;
		}
		else
			tuple->t_infomask |= HEAP_XMIN_COMMITTED;
	}

	/* by here, the inserting transaction has committed */

	if (tuple->t_infomask & HEAP_XMAX_INVALID)	/* xid invalid or aborted */
		return true;

	if (tuple->t_infomask & HEAP_XMAX_COMMITTED)
	{
		if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
			return true;
		return false;
	}

	if (TransactionIdIsCurrentTransactionId(tuple->t_xmax))
	{
		if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
			return true;
		if (CommandIdGEScanCommandId(tuple->t_cmax))
			return true;		/* deleted after scan started */
		else
			return false;		/* deleted before scan started */
	}

	if (!TransactionIdDidCommit(tuple->t_xmax))
	{
		if (TransactionIdDidAbort(tuple->t_xmax))
			tuple->t_infomask |= HEAP_XMAX_INVALID;		/* aborted */
		return true;
	}

	/* xmax transaction committed */
	tuple->t_infomask |= HEAP_XMAX_COMMITTED;

	if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
		return true;

	return false;
}

/*
 * HeapTupleSatisfiesToast
 *		True iff heap tuple is valid as a TOAST row.
 *
 * This is a simplified version that only checks for VACUUM moving conditions.
 * It's appropriate for TOAST usage because TOAST really doesn't want to do
 * its own time qual checks; if you can see the main table row that contains
 * a TOAST reference, you should be able to see the TOASTed value.  However,
 * vacuuming a TOAST table is independent of the main table, and in case such
 * a vacuum fails partway through, we'd better do this much checking.
 *
 * Among other things, this means you can't do UPDATEs of rows in a TOAST
 * table.
 */
bool
HeapTupleSatisfiesToast(HeapTupleHeader tuple)
{
	if (!(tuple->t_infomask & HEAP_XMIN_COMMITTED))
	{
		if (tuple->t_infomask & HEAP_XMIN_INVALID)
			return false;

		if (tuple->t_infomask & HEAP_MOVED_OFF)
		{
			if (TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
				return false;
			if (!TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
				tuple->t_infomask |= HEAP_XMIN_COMMITTED;
			}
		}
		else if (tuple->t_infomask & HEAP_MOVED_IN)
		{
			if (!TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
					return false;
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
					tuple->t_infomask |= HEAP_XMIN_COMMITTED;
				else
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
			}
		}
	}

	/* otherwise assume the tuple is valid for TOAST. */
	return true;
}

/*
 * HeapTupleSatisfiesUpdate
 *
 *	Same logic as HeapTupleSatisfiesNow, but returns a more detailed result
 *	code, since UPDATE needs to know more than "is it visible?".
 */
int
HeapTupleSatisfiesUpdate(HeapTuple htuple)
{
	HeapTupleHeader tuple = htuple->t_data;

	if (AMI_OVERRIDE)
		return HeapTupleMayBeUpdated;

	if (!(tuple->t_infomask & HEAP_XMIN_COMMITTED))
	{
		if (tuple->t_infomask & HEAP_XMIN_INVALID)
			return HeapTupleInvisible;

		if (tuple->t_infomask & HEAP_MOVED_OFF)
		{
			if (TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
				return HeapTupleInvisible;
			if (!TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return HeapTupleInvisible;
				}
				tuple->t_infomask |= HEAP_XMIN_COMMITTED;
			}
		}
		else if (tuple->t_infomask & HEAP_MOVED_IN)
		{
			if (!TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
					return HeapTupleInvisible;
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
					tuple->t_infomask |= HEAP_XMIN_COMMITTED;
				else
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return HeapTupleInvisible;
				}
			}
		}
		else if (TransactionIdIsCurrentTransactionId(tuple->t_xmin))
		{
			if (CommandIdGEScanCommandId(tuple->t_cmin))
				return HeapTupleInvisible;		/* inserted after scan
												 * started */

			if (tuple->t_infomask & HEAP_XMAX_INVALID)		/* xid invalid */
				return HeapTupleMayBeUpdated;

			Assert(TransactionIdIsCurrentTransactionId(tuple->t_xmax));

			if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
				return HeapTupleMayBeUpdated;

			if (CommandIdGEScanCommandId(tuple->t_cmax))
				return HeapTupleSelfUpdated;	/* updated after scan
												 * started */
			else
				return HeapTupleInvisible;		/* updated before scan
												 * started */
		}
		else if (!TransactionIdDidCommit(tuple->t_xmin))
		{
			if (TransactionIdDidAbort(tuple->t_xmin))
				tuple->t_infomask |= HEAP_XMIN_INVALID;	/* aborted */
			return HeapTupleInvisible;
		}
		else
			tuple->t_infomask |= HEAP_XMIN_COMMITTED;
	}

	/* by here, the inserting transaction has committed */

	if (tuple->t_infomask & HEAP_XMAX_INVALID)		/* xid invalid or aborted */
		return HeapTupleMayBeUpdated;

	if (tuple->t_infomask & HEAP_XMAX_COMMITTED)
	{
		if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
			return HeapTupleMayBeUpdated;
		return HeapTupleUpdated;	/* updated by other */
	}

	if (TransactionIdIsCurrentTransactionId(tuple->t_xmax))
	{
		if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
			return HeapTupleMayBeUpdated;
		if (CommandIdGEScanCommandId(tuple->t_cmax))
			return HeapTupleSelfUpdated;		/* updated after scan
												 * started */
		else
			return HeapTupleInvisible;	/* updated before scan started */
	}

	if (!TransactionIdDidCommit(tuple->t_xmax))
	{
		if (TransactionIdDidAbort(tuple->t_xmax))
		{
			tuple->t_infomask |= HEAP_XMAX_INVALID;		/* aborted */
			return HeapTupleMayBeUpdated;
		}
		/* running xact */
		return HeapTupleBeingUpdated;	/* in updation by other */
	}

	/* xmax transaction committed */
	tuple->t_infomask |= HEAP_XMAX_COMMITTED;

	if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
		return HeapTupleMayBeUpdated;

	return HeapTupleUpdated;	/* updated by other */
}

/*
 * HeapTupleSatisfiesDirty
 *		True iff heap tuple is valid including effects of open transactions.
 *
 *	Here, we consider the effects of:
 *		all committed and in-progress transactions (as of the current instant)
 *		previous commands of this transaction
 *		changes made by the current command
 *
 * This is essentially like HeapTupleSatisfiesItself as far as effects of
 * the current transaction and committed/aborted xacts are concerned.
 * However, we also include the effects of other xacts still in progress.
 *
 * Returns extra information in the global variable SnapshotDirty, namely
 * xids of concurrent xacts that affected the tuple.  Also, the tuple's
 * t_ctid (forward link) is returned if it's being updated.
 */
bool
HeapTupleSatisfiesDirty(HeapTupleHeader tuple)
{
	SnapshotDirty->xmin = SnapshotDirty->xmax = InvalidTransactionId;
	ItemPointerSetInvalid(&(SnapshotDirty->tid));

	if (AMI_OVERRIDE)
		return true;

	if (!(tuple->t_infomask & HEAP_XMIN_COMMITTED))
	{
		if (tuple->t_infomask & HEAP_XMIN_INVALID)
			return false;

		if (tuple->t_infomask & HEAP_MOVED_OFF)
		{
			if (TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
				return false;
			if (!TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
				tuple->t_infomask |= HEAP_XMIN_COMMITTED;
			}
		}
		else if (tuple->t_infomask & HEAP_MOVED_IN)
		{
			if (!TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
					return false;
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
					tuple->t_infomask |= HEAP_XMIN_COMMITTED;
				else
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
			}
		}
		else if (TransactionIdIsCurrentTransactionId(tuple->t_xmin))
		{
			if (tuple->t_infomask & HEAP_XMAX_INVALID)	/* xid invalid */
				return true;

			Assert(TransactionIdIsCurrentTransactionId(tuple->t_xmax));

			if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
				return true;

			return false;
		}
		else if (!TransactionIdDidCommit(tuple->t_xmin))
		{
			if (TransactionIdDidAbort(tuple->t_xmin))
			{
				tuple->t_infomask |= HEAP_XMIN_INVALID;
				return false;
			}
			SnapshotDirty->xmin = tuple->t_xmin;
			/* XXX shouldn't we fall through to look at xmax? */
			return true;		/* in insertion by other */
		}
		else
			tuple->t_infomask |= HEAP_XMIN_COMMITTED;
	}

	/* by here, the inserting transaction has committed */

	if (tuple->t_infomask & HEAP_XMAX_INVALID)	/* xid invalid or aborted */
		return true;

	if (tuple->t_infomask & HEAP_XMAX_COMMITTED)
	{
		if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
			return true;
		SnapshotDirty->tid = tuple->t_ctid;
		return false;			/* updated by other */
	}

	if (TransactionIdIsCurrentTransactionId(tuple->t_xmax))
	{
		if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
			return true;
		return false;
	}

	if (!TransactionIdDidCommit(tuple->t_xmax))
	{
		if (TransactionIdDidAbort(tuple->t_xmax))
		{
			tuple->t_infomask |= HEAP_XMAX_INVALID;		/* aborted */
			return true;
		}
		/* running xact */
		SnapshotDirty->xmax = tuple->t_xmax;
		return true;			/* in updation by other */
	}

	/* xmax transaction committed */
	tuple->t_infomask |= HEAP_XMAX_COMMITTED;

	if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
		return true;

	SnapshotDirty->tid = tuple->t_ctid;
	return false;				/* updated by other */
}

/*
 * HeapTupleSatisfiesSnapshot
 *		True iff heap tuple is valid for the given snapshot.
 *
 *	Here, we consider the effects of:
 *		all transactions committed as of the time of the given snapshot
 *		previous commands of this transaction
 *
 *	Does _not_ include:
 *		transactions shown as in-progress by the snapshot
 *		transactions started after the snapshot was taken
 *		changes made by the current command
 *
 * This is the same as HeapTupleSatisfiesNow, except that transactions that
 * were in progress or as yet unstarted when the snapshot was taken will
 * be treated as uncommitted, even if they have committed by now.
 *
 * (Notice, however, that the tuple status hint bits will be updated on the
 * basis of the true state of the transaction, even if we then pretend we
 * can't see it.)
 */
bool
HeapTupleSatisfiesSnapshot(HeapTupleHeader tuple, Snapshot snapshot)
{
	if (AMI_OVERRIDE)
		return true;

	/* XXX this is horribly ugly: */
	if (ReferentialIntegritySnapshotOverride)
		return HeapTupleSatisfiesNow(tuple);

	if (!(tuple->t_infomask & HEAP_XMIN_COMMITTED))
	{
		if (tuple->t_infomask & HEAP_XMIN_INVALID)
			return false;

		if (tuple->t_infomask & HEAP_MOVED_OFF)
		{
			if (TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
				return false;
			if (!TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
				tuple->t_infomask |= HEAP_XMIN_COMMITTED;
			}
		}
		else if (tuple->t_infomask & HEAP_MOVED_IN)
		{
			if (!TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
			{
				if (TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
					return false;
				if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
					tuple->t_infomask |= HEAP_XMIN_COMMITTED;
				else
				{
					tuple->t_infomask |= HEAP_XMIN_INVALID;
					return false;
				}
			}
		}
		else if (TransactionIdIsCurrentTransactionId(tuple->t_xmin))
		{
			if (CommandIdGEScanCommandId(tuple->t_cmin))
				return false;	/* inserted after scan started */

			if (tuple->t_infomask & HEAP_XMAX_INVALID)	/* xid invalid */
				return true;

			Assert(TransactionIdIsCurrentTransactionId(tuple->t_xmax));

			if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
				return true;

			if (CommandIdGEScanCommandId(tuple->t_cmax))
				return true;	/* deleted after scan started */
			else
				return false;	/* deleted before scan started */
		}
		else if (!TransactionIdDidCommit(tuple->t_xmin))
		{
			if (TransactionIdDidAbort(tuple->t_xmin))
				tuple->t_infomask |= HEAP_XMIN_INVALID;
			return false;
		}
		else
			tuple->t_infomask |= HEAP_XMIN_COMMITTED;
	}

	/*
	 * By here, the inserting transaction has committed - have to check
	 * when...
	 */

	if (TransactionIdFollowsOrEquals(tuple->t_xmin, snapshot->xmax))
		return false;
	if (TransactionIdFollowsOrEquals(tuple->t_xmin, snapshot->xmin))
	{
		uint32		i;

		for (i = 0; i < snapshot->xcnt; i++)
		{
			if (TransactionIdEquals(tuple->t_xmin, snapshot->xip[i]))
				return false;
		}
	}

	if (tuple->t_infomask & HEAP_XMAX_INVALID)	/* xid invalid or aborted */
		return true;

	if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
		return true;

	if (!(tuple->t_infomask & HEAP_XMAX_COMMITTED))
	{
		if (TransactionIdIsCurrentTransactionId(tuple->t_xmax))
		{
			if (CommandIdGEScanCommandId(tuple->t_cmax))
				return true;	/* deleted after scan started */
			else
				return false;	/* deleted before scan started */
		}

		if (!TransactionIdDidCommit(tuple->t_xmax))
		{
			if (TransactionIdDidAbort(tuple->t_xmax))
				tuple->t_infomask |= HEAP_XMAX_INVALID; /* aborted */
			return true;
		}

		/* xmax transaction committed */
		tuple->t_infomask |= HEAP_XMAX_COMMITTED;
	}

	if (TransactionIdFollowsOrEquals(tuple->t_xmax, snapshot->xmax))
		return true;
	if (TransactionIdFollowsOrEquals(tuple->t_xmax, snapshot->xmin))
	{
		uint32		i;

		for (i = 0; i < snapshot->xcnt; i++)
		{
			if (TransactionIdEquals(tuple->t_xmax, snapshot->xip[i]))
				return true;
		}
	}

	return false;
}


/*
 * HeapTupleSatisfiesVacuum
 *
 *	Determine the status of tuples for VACUUM purposes.  Here, what
 *	we mainly want to know is if a tuple is potentially visible to *any*
 *	running transaction.  If so, it can't be removed yet by VACUUM.
 *
 * OldestXmin is a cutoff XID (obtained from GetOldestXmin()).	Tuples
 * deleted by XIDs >= OldestXmin are deemed "recently dead"; they might
 * still be visible to some open transaction, so we can't remove them,
 * even if we see that the deleting transaction has committed.
 */
HTSV_Result
HeapTupleSatisfiesVacuum(HeapTupleHeader tuple, TransactionId OldestXmin)
{
	/*
	 * Has inserting transaction committed?
	 *
	 * If the inserting transaction aborted, then the tuple was never visible
	 * to any other transaction, so we can delete it immediately.
	 *
	 * NOTE: must check TransactionIdIsInProgress (which looks in PROC array)
	 * before TransactionIdDidCommit/TransactionIdDidAbort (which look in
	 * pg_clog).  Otherwise we have a race condition where we might decide
	 * that a just-committed transaction crashed, because none of the
	 * tests succeed.  xact.c is careful to record commit/abort in pg_clog
	 * before it unsets MyProc->xid in PROC array.
	 */
	if (!(tuple->t_infomask & HEAP_XMIN_COMMITTED))
	{
		if (tuple->t_infomask & HEAP_XMIN_INVALID)
			return HEAPTUPLE_DEAD;
		else if (tuple->t_infomask & HEAP_MOVED_OFF)
		{
			if (TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
				return HEAPTUPLE_DELETE_IN_PROGRESS;
			if (TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
				return HEAPTUPLE_DELETE_IN_PROGRESS;
			if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
			{
				tuple->t_infomask |= HEAP_XMIN_INVALID;
				return HEAPTUPLE_DEAD;
			}
			tuple->t_infomask |= HEAP_XMIN_COMMITTED;
		}
		else if (tuple->t_infomask & HEAP_MOVED_IN)
		{
			if (TransactionIdIsCurrentTransactionId((TransactionId) tuple->t_cmin))
				return HEAPTUPLE_INSERT_IN_PROGRESS;
			if (TransactionIdIsInProgress((TransactionId) tuple->t_cmin))
				return HEAPTUPLE_INSERT_IN_PROGRESS;
			if (TransactionIdDidCommit((TransactionId) tuple->t_cmin))
				tuple->t_infomask |= HEAP_XMIN_COMMITTED;
			else
			{
				tuple->t_infomask |= HEAP_XMIN_INVALID;
				return HEAPTUPLE_DEAD;
			}
		}
		else if (TransactionIdIsInProgress(tuple->t_xmin))
			return HEAPTUPLE_INSERT_IN_PROGRESS;
		else if (TransactionIdDidCommit(tuple->t_xmin))
			tuple->t_infomask |= HEAP_XMIN_COMMITTED;
		else if (TransactionIdDidAbort(tuple->t_xmin))
		{
			tuple->t_infomask |= HEAP_XMIN_INVALID;
			return HEAPTUPLE_DEAD;
		}
		else
		{
			/*
			 * Not in Progress, Not Committed, Not Aborted - so it's from
			 * crashed process. - vadim 11/26/96
			 */
			tuple->t_infomask |= HEAP_XMIN_INVALID;
			return HEAPTUPLE_DEAD;
		}
		/* Should only get here if we set XMIN_COMMITTED */
		Assert(tuple->t_infomask & HEAP_XMIN_COMMITTED);
	}

	/*
	 * Okay, the inserter committed, so it was good at some point.	Now
	 * what about the deleting transaction?
	 */
	if (tuple->t_infomask & HEAP_XMAX_INVALID)
		return HEAPTUPLE_LIVE;

	if (tuple->t_infomask & HEAP_MARKED_FOR_UPDATE)
	{
		/*
		 * "Deleting" xact really only marked it for update, so the tuple
		 * is live in any case.  However, we must make sure that either
		 * XMAX_COMMITTED or XMAX_INVALID gets set once the xact is gone;
		 * otherwise it is unsafe to recycle CLOG status after vacuuming.
		 */
		if (!(tuple->t_infomask & HEAP_XMAX_COMMITTED))
		{
			if (TransactionIdIsInProgress(tuple->t_xmax))
				return HEAPTUPLE_LIVE;
			if (TransactionIdDidCommit(tuple->t_xmax))
				tuple->t_infomask |= HEAP_XMAX_COMMITTED;
			else				/* it's either aborted or crashed */
				tuple->t_infomask |= HEAP_XMAX_INVALID;
		}
		return HEAPTUPLE_LIVE;
	}

	if (!(tuple->t_infomask & HEAP_XMAX_COMMITTED))
	{
		if (TransactionIdIsInProgress(tuple->t_xmax))
			return HEAPTUPLE_DELETE_IN_PROGRESS;
		else if (TransactionIdDidCommit(tuple->t_xmax))
			tuple->t_infomask |= HEAP_XMAX_COMMITTED;
		else if (TransactionIdDidAbort(tuple->t_xmax))
		{
			tuple->t_infomask |= HEAP_XMAX_INVALID;
			return HEAPTUPLE_LIVE;
		}
		else
		{
			/*
			 * Not in Progress, Not Committed, Not Aborted - so it's from
			 * crashed process. - vadim 06/02/97
			 */
			tuple->t_infomask |= HEAP_XMAX_INVALID;
			return HEAPTUPLE_LIVE;
		}
		/* Should only get here if we set XMAX_COMMITTED */
		Assert(tuple->t_infomask & HEAP_XMAX_COMMITTED);
	}

	/*
	 * Deleter committed, but check special cases.
	 */

	if (TransactionIdEquals(tuple->t_xmin, tuple->t_xmax))
	{
		/*
		 * inserter also deleted it, so it was never visible to anyone
		 * else
		 */
		return HEAPTUPLE_DEAD;
	}

	if (!TransactionIdPrecedes(tuple->t_xmax, OldestXmin))
	{
		/* deleting xact is too recent, tuple could still be visible */
		return HEAPTUPLE_RECENTLY_DEAD;
	}

	/* Otherwise, it's dead and removable */
	return HEAPTUPLE_DEAD;
}


/*
 * SetQuerySnapshot
 *		Initialize query snapshot for a new query
 *
 * The SerializableSnapshot is the first one taken in a transaction.
 * In serializable mode we just use that one throughout the transaction.
 * In read-committed mode, we take a new snapshot at the start of each query.
 */
void
SetQuerySnapshot(void)
{
	/* Initialize snapshot overriding to false */
	ReferentialIntegritySnapshotOverride = false;

	/* 1st call in xaction? */
	if (SerializableSnapshot == NULL)
	{
		SerializableSnapshot = GetSnapshotData(true);
		QuerySnapshot = SerializableSnapshot;
		Assert(QuerySnapshot != NULL);
		return;
	}

	if (QuerySnapshot != SerializableSnapshot)
	{
		free(QuerySnapshot->xip);
		free(QuerySnapshot);
	}

	if (XactIsoLevel == XACT_SERIALIZABLE)
		QuerySnapshot = SerializableSnapshot;
	else
		QuerySnapshot = GetSnapshotData(false);

	Assert(QuerySnapshot != NULL);
}

/*
 * FreeXactSnapshot
 *		Free snapshot(s) at end of transaction.
 */
void
FreeXactSnapshot(void)
{
	if (QuerySnapshot != NULL && QuerySnapshot != SerializableSnapshot)
	{
		free(QuerySnapshot->xip);
		free(QuerySnapshot);
	}

	QuerySnapshot = NULL;

	if (SerializableSnapshot != NULL)
	{
		free(SerializableSnapshot->xip);
		free(SerializableSnapshot);
	}

	SerializableSnapshot = NULL;
}
