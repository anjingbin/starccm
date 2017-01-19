/*-------------------------------------------------------------------------
 *
 * smgr.c
 *	  public interface routines to storage manager switch.
 *
 *	  All file system operations in POSTGRES dispatch through these
 *	  routines.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/storage/smgr/smgr.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "storage/bufmgr.h"
#include "storage/freespace.h"
#include "storage/ipc.h"
#include "storage/smgr.h"
#include "utils/memutils.h"


static void smgrshutdown(void);

typedef struct f_smgr
{
	int			(*smgr_init) (void);	/* may be NULL */
	int			(*smgr_shutdown) (void);		/* may be NULL */
	int			(*smgr_create) (Relation reln);
	int			(*smgr_unlink) (RelFileNode rnode);
	int			(*smgr_extend) (Relation reln, BlockNumber blocknum,
											char *buffer);
	int			(*smgr_open) (Relation reln);
	int			(*smgr_close) (Relation reln);
	int			(*smgr_read) (Relation reln, BlockNumber blocknum,
										  char *buffer);
	int			(*smgr_write) (Relation reln, BlockNumber blocknum,
										   char *buffer);
	int			(*smgr_flush) (Relation reln, BlockNumber blocknum,
										   char *buffer);
	int			(*smgr_blindwrt) (RelFileNode rnode, BlockNumber blkno,
											  char *buffer, bool dofsync);
	int			(*smgr_markdirty) (Relation reln, BlockNumber blkno);
	int			(*smgr_blindmarkdirty) (RelFileNode, BlockNumber blkno);
	BlockNumber (*smgr_nblocks) (Relation reln);
	BlockNumber (*smgr_truncate) (Relation reln, BlockNumber nblocks);
	int			(*smgr_commit) (void);	/* may be NULL */
	int			(*smgr_abort) (void);	/* may be NULL */
	int			(*smgr_sync) (void);
} f_smgr;

/*
 *	The weird placement of commas in this init block is to keep the compiler
 *	happy, regardless of what storage managers we have (or don't have).
 */

static f_smgr smgrsw[] = {

	/* magnetic disk */
	{mdinit, NULL, mdcreate, mdunlink, mdextend, mdopen, mdclose,
		mdread, mdwrite, mdflush, mdblindwrt, mdmarkdirty, mdblindmarkdirty,
		mdnblocks, mdtruncate, mdcommit, mdabort, mdsync
	},

#ifdef STABLE_MEMORY_STORAGE
	/* main memory */
	{mminit, mmshutdown, mmcreate, mmunlink, mmextend, mmopen, mmclose,
		mmread, mmwrite, mmflush, mmblindwrt, mmmarkdirty, mmblindmarkdirty,
	mmnblocks, NULL, mmcommit, mmabort},
#endif
};

/*
 *	This array records which storage managers are write-once, and which
 *	support overwrite.	A 'true' entry means that the storage manager is
 *	write-once.  In the best of all possible worlds, there would be no
 *	write-once storage managers.
 */

#ifdef NOT_USED
static bool smgrwo[] = {
	false,						/* magnetic disk */
#ifdef STABLE_MEMORY_STORAGE
	false,						/* main memory */
#endif
};
#endif

static int	NSmgr = lengthof(smgrsw);

/*
 * We keep a list of all relations (represented as RelFileNode values)
 * that have been created or deleted in the current transaction.  When
 * a relation is created, we create the physical file immediately, but
 * remember it so that we can delete the file again if the current
 * transaction is aborted.	Conversely, a deletion request is NOT
 * executed immediately, but is just entered in the list.  When and if
 * the transaction commits, we can delete the physical file.
 *
 * NOTE: the list is kept in TopMemoryContext to be sure it won't disappear
 * unbetimes.  It'd probably be OK to keep it in TopTransactionContext,
 * but I'm being paranoid.
 */

typedef struct PendingRelDelete
{
	RelFileNode relnode;		/* relation that may need to be deleted */
	int16		which;			/* which storage manager? */
	bool		atCommit;		/* T=delete at commit; F=delete at abort */
	struct PendingRelDelete *next;		/* linked-list link */
} PendingRelDelete;

static PendingRelDelete *pendingDeletes = NULL; /* head of linked list */


/*
 *	smgrinit(), smgrshutdown() -- Initialize or shut down all storage
 *								  managers.
 *
 */
int
smgrinit()
{
	int			i;

	for (i = 0; i < NSmgr; i++)
	{
		if (smgrsw[i].smgr_init)
		{
			if ((*(smgrsw[i].smgr_init)) () == SM_FAIL)
				elog(FATAL, "initialization failed on %s: %m",
					 DatumGetCString(DirectFunctionCall1(smgrout,
													 Int16GetDatum(i))));
		}
	}

	/* register the shutdown proc */
	on_proc_exit(smgrshutdown, 0);

	return SM_SUCCESS;
}

static void
smgrshutdown(void)
{
	int			i;

	for (i = 0; i < NSmgr; i++)
	{
		if (smgrsw[i].smgr_shutdown)
		{
			if ((*(smgrsw[i].smgr_shutdown)) () == SM_FAIL)
				elog(FATAL, "shutdown failed on %s: %m",
					 DatumGetCString(DirectFunctionCall1(smgrout,
													 Int16GetDatum(i))));
		}
	}
}

/*
 *	smgrcreate() -- Create a new relation.
 *
 *		This routine takes a reldesc, creates the relation on the appropriate
 *		device, and returns a file descriptor for it.
 */
int
smgrcreate(int16 which, Relation reln)
{
	int			fd;
	PendingRelDelete *pending;

	if ((fd = (*(smgrsw[which].smgr_create)) (reln)) < 0)
		elog(ERROR, "cannot create %s: %m", RelationGetRelationName(reln));

	/* Add the relation to the list of stuff to delete at abort */
	pending = (PendingRelDelete *)
		MemoryContextAlloc(TopMemoryContext, sizeof(PendingRelDelete));
	pending->relnode = reln->rd_node;
	pending->which = which;
	pending->atCommit = false;	/* delete if abort */
	pending->next = pendingDeletes;
	pendingDeletes = pending;

	return fd;
}

/*
 *	smgrunlink() -- Unlink a relation.
 *
 *		The relation is removed from the store.  Actually, we just remember
 *		that we want to do this at transaction commit.
 */
int
smgrunlink(int16 which, Relation reln)
{
	PendingRelDelete *pending;

	/* Make sure the file is closed */
	if (reln->rd_fd >= 0)
		smgrclose(which, reln);

	/* Add the relation to the list of stuff to delete at commit */
	pending = (PendingRelDelete *)
		MemoryContextAlloc(TopMemoryContext, sizeof(PendingRelDelete));
	pending->relnode = reln->rd_node;
	pending->which = which;
	pending->atCommit = true;	/* delete if commit */
	pending->next = pendingDeletes;
	pendingDeletes = pending;

	/*
	 * NOTE: if the relation was created in this transaction, it will now
	 * be present in the pending-delete list twice, once with atCommit
	 * true and once with atCommit false.  Hence, it will be physically
	 * deleted at end of xact in either case (and the other entry will be
	 * ignored by smgrDoPendingDeletes, so no error will occur).  We could
	 * instead remove the existing list entry and delete the physical file
	 * immediately, but for now I'll keep the logic simple.
	 */

	return SM_SUCCESS;
}

/*
 *	smgrextend() -- Add a new block to a file.
 *
 *		The semantics are basically the same as smgrwrite(): write at the
 *		specified position.  However, we are expecting to extend the
 *		relation (ie, blocknum is the current EOF), and so in case of
 *		failure we clean up by truncating.
 *
 *		Returns SM_SUCCESS on success; aborts the current transaction on
 *		failure.
 */
int
smgrextend(int16 which, Relation reln, BlockNumber blocknum, char *buffer)
{
	int			status;

	status = (*(smgrsw[which].smgr_extend)) (reln, blocknum, buffer);

	if (status == SM_FAIL)
		elog(ERROR, "cannot extend %s: %m.\n\tCheck free disk space.",
			 RelationGetRelationName(reln));

	return status;
}

/*
 *	smgropen() -- Open a relation using a particular storage manager.
 *
 *		Returns the fd for the open relation on success.
 *
 *		On failure, returns -1 if failOK, else aborts the transaction.
 */
int
smgropen(int16 which, Relation reln, bool failOK)
{
	int			fd;

	if (reln->rd_rel->relkind == RELKIND_VIEW)
		return -1;
	if ((fd = (*(smgrsw[which].smgr_open)) (reln)) < 0)
		if (!failOK)
			elog(ERROR, "cannot open %s: %m", RelationGetRelationName(reln));

	return fd;
}

/*
 *	smgrclose() -- Close a relation.
 *
 *		Returns SM_SUCCESS on success, aborts on failure.
 */
int
smgrclose(int16 which, Relation reln)
{
	if ((*(smgrsw[which].smgr_close)) (reln) == SM_FAIL)
		elog(ERROR, "cannot close %s: %m", RelationGetRelationName(reln));

	return SM_SUCCESS;
}

/*
 *	smgrread() -- read a particular block from a relation into the supplied
 *				  buffer.
 *
 *		This routine is called from the buffer manager in order to
 *		instantiate pages in the shared buffer cache.  All storage managers
 *		return pages in the format that POSTGRES expects.  This routine
 *		dispatches the read.  On success, it returns SM_SUCCESS.  On failure,
 *		the current transaction is aborted.
 */
int
smgrread(int16 which, Relation reln, BlockNumber blocknum, char *buffer)
{
	int			status;

	status = (*(smgrsw[which].smgr_read)) (reln, blocknum, buffer);

	if (status == SM_FAIL)
		elog(ERROR, "cannot read block %d of %s: %m",
			 blocknum, RelationGetRelationName(reln));

	return status;
}

/*
 *	smgrwrite() -- Write the supplied buffer out.
 *
 *		This is not a synchronous write -- the interface for that is
 *		smgrflush().  The buffer is written out via the appropriate
 *		storage manager.  This routine returns SM_SUCCESS or aborts
 *		the current transaction.
 */
int
smgrwrite(int16 which, Relation reln, BlockNumber blocknum, char *buffer)
{
	int			status;

	status = (*(smgrsw[which].smgr_write)) (reln, blocknum, buffer);

	if (status == SM_FAIL)
		elog(ERROR, "cannot write block %d of %s: %m",
			 blocknum, RelationGetRelationName(reln));

	return status;
}

/*
 *	smgrflush() -- A synchronous smgrwrite().
 */
int
smgrflush(int16 which, Relation reln, BlockNumber blocknum, char *buffer)
{
	int			status;

	status = (*(smgrsw[which].smgr_flush)) (reln, blocknum, buffer);

	if (status == SM_FAIL)
		elog(ERROR, "cannot flush block %d of %s to stable store: %m",
			 blocknum, RelationGetRelationName(reln));

	return status;
}

/*
 *	smgrblindwrt() -- Write a page out blind.
 *
 *		In some cases, we may find a page in the buffer cache that we
 *		can't make a reldesc for.  This happens, for example, when we
 *		want to reuse a dirty page that was written by a transaction
 *		that has not yet committed, which created a new relation.  In
 *		this case, the buffer manager will call smgrblindwrt() with
 *		the name and OID of the database and the relation to which the
 *		buffer belongs.  Every storage manager must be able to force
 *		this page down to stable storage in this circumstance.	The
 *		write should be synchronous if dofsync is true.
 */
int
smgrblindwrt(int16 which,
			 RelFileNode rnode,
			 BlockNumber blkno,
			 char *buffer,
			 bool dofsync)
{
	int			status;

	status = (*(smgrsw[which].smgr_blindwrt)) (rnode, blkno, buffer, dofsync);

	if (status == SM_FAIL)
		elog(ERROR, "cannot write block %d of %u/%u blind: %m",
			 blkno, rnode.tblNode, rnode.relNode);

	return status;
}

/*
 *	smgrmarkdirty() -- Mark a page dirty (needs fsync).
 *
 *		Mark the specified page as needing to be fsync'd before commit.
 *		Ordinarily, the storage manager will do this implicitly during
 *		smgrwrite().  However, the buffer manager may discover that some
 *		other backend has written a buffer that we dirtied in the current
 *		transaction.  In that case, we still need to fsync the file to be
 *		sure the page is down to disk before we commit.
 */
int
smgrmarkdirty(int16 which,
			  Relation reln,
			  BlockNumber blkno)
{
	int			status;

	status = (*(smgrsw[which].smgr_markdirty)) (reln, blkno);

	if (status == SM_FAIL)
		elog(ERROR, "cannot mark block %d of %s: %m",
			 blkno, RelationGetRelationName(reln));

	return status;
}

/*
 *	smgrblindmarkdirty() -- Mark a page dirty, "blind".
 *
 *		Just like smgrmarkdirty, except we don't have a reldesc.
 */
int
smgrblindmarkdirty(int16 which,
				   RelFileNode rnode,
				   BlockNumber blkno)
{
	int			status;

	status = (*(smgrsw[which].smgr_blindmarkdirty)) (rnode, blkno);

	if (status == SM_FAIL)
		elog(ERROR, "cannot mark block %d of %u/%u blind: %m",
			 blkno, rnode.tblNode, rnode.relNode);

	return status;
}

/*
 *	smgrnblocks() -- Calculate the number of POSTGRES blocks in the
 *					 supplied relation.
 *
 *		Returns the number of blocks on success, aborts the current
 *		transaction on failure.
 */
BlockNumber
smgrnblocks(int16 which, Relation reln)
{
	BlockNumber nblocks;

	nblocks = (*(smgrsw[which].smgr_nblocks)) (reln);

	/*
	 * NOTE: if a relation ever did grow to 2^32-1 blocks, this code would
	 * fail --- but that's a good thing, because it would stop us from
	 * extending the rel another block and having a block whose number
	 * actually is InvalidBlockNumber.
	 */
	if (nblocks == InvalidBlockNumber)
		elog(ERROR, "cannot count blocks for %s: %m",
			 RelationGetRelationName(reln));

	return nblocks;
}

/*
 *	smgrtruncate() -- Truncate supplied relation to a specified number
 *						of blocks
 *
 *		Returns the number of blocks on success, aborts the current
 *		transaction on failure.
 */
BlockNumber
smgrtruncate(int16 which, Relation reln, BlockNumber nblocks)
{
	BlockNumber newblks;

	newblks = nblocks;
	if (smgrsw[which].smgr_truncate)
	{
		/*
		 * Tell the free space map to forget anything it may have stored
		 * for the about-to-be-deleted blocks.	We want to be sure it
		 * won't return bogus block numbers later on.
		 */
		MultiRecordFreeSpace(&reln->rd_node,
							 nblocks, MaxBlockNumber,
							 0, NULL, NULL);

		newblks = (*(smgrsw[which].smgr_truncate)) (reln, nblocks);
		if (newblks == InvalidBlockNumber)
			elog(ERROR, "cannot truncate %s to %u blocks: %m",
				 RelationGetRelationName(reln), nblocks);
	}

	return newblks;
}

/*
 * smgrDoPendingDeletes() -- take care of relation deletes at end of xact.
 */
int
smgrDoPendingDeletes(bool isCommit)
{
	while (pendingDeletes != NULL)
	{
		PendingRelDelete *pending = pendingDeletes;

		pendingDeletes = pending->next;
		if (pending->atCommit == isCommit)
		{
			/*
			 * Get rid of any leftover buffers for the rel (shouldn't be
			 * any in the commit case, but there can be in the abort
			 * case).
			 */
			DropRelFileNodeBuffers(pending->relnode);

			/*
			 * Tell the free space map to forget this relation.  It won't
			 * be accessed any more anyway, but we may as well recycle the
			 * map space quickly.
			 */
			FreeSpaceMapForgetRel(&pending->relnode);

			/*
			 * And delete the physical files.
			 *
			 * Note: we treat deletion failure as a NOTICE, not an error,
			 * because we've already decided to commit or abort the
			 * current xact.
			 */
			if ((*(smgrsw[pending->which].smgr_unlink)) (pending->relnode) == SM_FAIL)
				elog(NOTICE, "cannot unlink %u/%u: %m",
					 pending->relnode.tblNode, pending->relnode.relNode);
		}
		pfree(pending);
	}

	return SM_SUCCESS;
}

/*
 *	smgrcommit(), smgrabort() -- Commit or abort changes made during the
 *								 current transaction.
 */
int
smgrcommit()
{
	int			i;

	for (i = 0; i < NSmgr; i++)
	{
		if (smgrsw[i].smgr_commit)
		{
			if ((*(smgrsw[i].smgr_commit)) () == SM_FAIL)
				elog(FATAL, "transaction commit failed on %s: %m",
					 DatumGetCString(DirectFunctionCall1(smgrout,
													 Int16GetDatum(i))));
		}
	}

	return SM_SUCCESS;
}

int
smgrabort()
{
	int			i;

	for (i = 0; i < NSmgr; i++)
	{
		if (smgrsw[i].smgr_abort)
		{
			if ((*(smgrsw[i].smgr_abort)) () == SM_FAIL)
				elog(FATAL, "transaction abort failed on %s: %m",
					 DatumGetCString(DirectFunctionCall1(smgrout,
													 Int16GetDatum(i))));
		}
	}

	return SM_SUCCESS;
}

int
smgrsync()
{
	int			i;

	for (i = 0; i < NSmgr; i++)
	{
		if (smgrsw[i].smgr_sync)
		{
			if ((*(smgrsw[i].smgr_sync)) () == SM_FAIL)
				elog(STOP, "storage sync failed on %s: %m",
					 DatumGetCString(DirectFunctionCall1(smgrout,
													 Int16GetDatum(i))));
		}
	}

	return SM_SUCCESS;
}

#ifdef NOT_USED
bool
smgriswo(int16 smgrno)
{
	if (smgrno < 0 || smgrno >= NSmgr)
		elog(ERROR, "illegal storage manager number %d", smgrno);

	return smgrwo[smgrno];
}
#endif

void
smgr_redo(XLogRecPtr lsn, XLogRecord *record)
{
}

void
smgr_undo(XLogRecPtr lsn, XLogRecord *record)
{
}

void
smgr_desc(char *buf, uint8 xl_info, char *rec)
{
}
