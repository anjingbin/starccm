/*-------------------------------------------------------------------------
 *
 * vacuumlazy.c
 *	  Concurrent ("lazy") vacuuming.
 *
 *
 * The major space usage for LAZY VACUUM is storage for the array of dead
 * tuple TIDs, with the next biggest need being storage for per-disk-page
 * free space info.  We want to ensure we can vacuum even the very largest
 * relations with finite memory space usage.  To do that, we set upper bounds
 * on the number of tuples and pages we will keep track of at once.
 *
 * We are willing to use at most VacuumMem memory space to keep track of
 * dead tuples.  We initially allocate an array of TIDs of that size.
 * If the array threatens to overflow, we suspend the heap scan phase
 * and perform a pass of index cleanup and page compaction, then resume
 * the heap scan with an empty TID array.
 *
 * We can limit the storage for page free space to MaxFSMPages entries,
 * since that's the most the free space map will be willing to remember
 * anyway.	If the relation has fewer than that many pages with free space,
 * life is easy: just build an array of per-page info.	If it has more,
 * we store the free space info as a heap ordered by amount of free space,
 * so that we can discard the pages with least free space to ensure we never
 * have more than MaxFSMPages entries in all.  The surviving page entries
 * are passed to the free space map at conclusion of the scan.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/commands/vacuumlazy.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/genam.h"
#include "access/heapam.h"
#include "access/xlog.h"
#include "commands/vacuum.h"
#include "miscadmin.h"
#include "storage/freespace.h"
#include "storage/sinval.h"
#include "storage/smgr.h"


/*
 * Space/time tradeoff parameters: do these need to be user-tunable?
 *
 * A page with less than PAGE_SPACE_THRESHOLD free space will be forgotten
 * immediately, and not even passed to the free space map.	Removing the
 * uselessly small entries early saves cycles, and in particular reduces
 * the amount of time we spend holding the FSM lock when we finally call
 * MultiRecordFreeSpace.  Since the FSM will ignore pages below its own
 * runtime threshold anyway, there's no point in making this really small.
 * XXX Is it worth trying to measure average tuple size, and using that to
 * set the threshold?  Problem is we don't know average tuple size very
 * accurately for the first few pages...
 *
 * To consider truncating the relation, we want there to be at least
 * relsize / REL_TRUNCATE_FRACTION potentially-freeable pages.
 */
#define PAGE_SPACE_THRESHOLD	((Size) (BLCKSZ / 32))

#define REL_TRUNCATE_FRACTION	16

/* MAX_TUPLES_PER_PAGE can be a conservative upper limit */
#define MAX_TUPLES_PER_PAGE		((int) (BLCKSZ / sizeof(HeapTupleHeaderData)))


typedef struct LVRelStats
{
	/* Overall statistics about rel */
	BlockNumber rel_pages;
	double		rel_tuples;
	BlockNumber nonempty_pages; /* actually, last nonempty page + 1 */
	/* List of TIDs of tuples we intend to delete */
	/* NB: this list is ordered by TID address */
	int			num_dead_tuples;	/* current # of entries */
	int			max_dead_tuples;	/* # slots allocated in array */
	ItemPointer dead_tuples;	/* array of ItemPointerData */
	/* Array or heap of per-page info about free space */
	/* We use a simple array until it fills up, then convert to heap */
	bool		fs_is_heap;		/* are we using heap organization? */
	int			num_free_pages; /* current # of entries */
	int			max_free_pages; /* # slots allocated in arrays */
	BlockNumber *free_pages;	/* array or heap of block numbers */
	Size	   *free_spaceavail;	/* array or heap of available space */
} LVRelStats;


static int	MESSAGE_LEVEL;		/* message level */

static TransactionId OldestXmin;
static TransactionId FreezeLimit;


/* non-export function prototypes */
static void lazy_scan_heap(Relation onerel, LVRelStats *vacrelstats,
			   Relation *Irel, int nindexes);
static void lazy_vacuum_heap(Relation onerel, LVRelStats *vacrelstats);
static void lazy_scan_index(Relation indrel, LVRelStats *vacrelstats);
static void lazy_vacuum_index(Relation indrel, LVRelStats *vacrelstats);
static int lazy_vacuum_page(Relation onerel, BlockNumber blkno, Buffer buffer,
				 int tupindex, LVRelStats *vacrelstats);
static void lazy_truncate_heap(Relation onerel, LVRelStats *vacrelstats);
static BlockNumber count_nondeletable_pages(Relation onerel,
						 LVRelStats *vacrelstats);
static void lazy_space_alloc(LVRelStats *vacrelstats, BlockNumber relblocks);
static void lazy_record_dead_tuple(LVRelStats *vacrelstats,
					   ItemPointer itemptr);
static void lazy_record_free_space(LVRelStats *vacrelstats,
					   BlockNumber page, Size avail);
static bool lazy_tid_reaped(ItemPointer itemptr, void *state);
static bool dummy_tid_reaped(ItemPointer itemptr, void *state);
static void lazy_update_fsm(Relation onerel, LVRelStats *vacrelstats);
static int	vac_cmp_itemptr(const void *left, const void *right);


/*
 *	lazy_vacuum_rel() -- perform LAZY VACUUM for one heap relation
 *
 *		This routine vacuums a single heap, cleans out its indexes, and
 *		updates its num_pages and num_tuples statistics.
 *
 *		At entry, we have already established a transaction and opened
 *		and locked the relation.
 */
void
lazy_vacuum_rel(Relation onerel, VacuumStmt *vacstmt)
{
	LVRelStats *vacrelstats;
	Relation   *Irel;
	int			nindexes;
	bool		hasindex;
	BlockNumber possibly_freeable;

	/* initialize */
	if (vacstmt->verbose)
		MESSAGE_LEVEL = NOTICE;
	else
		MESSAGE_LEVEL = DEBUG;

	vacuum_set_xid_limits(vacstmt, onerel->rd_rel->relisshared,
						  &OldestXmin, &FreezeLimit);

	vacrelstats = (LVRelStats *) palloc(sizeof(LVRelStats));
	MemSet(vacrelstats, 0, sizeof(LVRelStats));

	/* Open all indexes of the relation */
	vac_open_indexes(onerel, &nindexes, &Irel);
	hasindex = (nindexes > 0);

	/* Do the vacuuming */
	lazy_scan_heap(onerel, vacrelstats, Irel, nindexes);

	/* Done with indexes */
	vac_close_indexes(nindexes, Irel);

	/*
	 * Optionally truncate the relation.
	 *
	 * Don't even think about it unless we have a shot at releasing a goodly
	 * number of pages.  Otherwise, the time taken isn't worth it.
	 */
	possibly_freeable = vacrelstats->rel_pages - vacrelstats->nonempty_pages;
	if (possibly_freeable > vacrelstats->rel_pages / REL_TRUNCATE_FRACTION)
		lazy_truncate_heap(onerel, vacrelstats);

	/* Update shared free space map with final free space info */
	lazy_update_fsm(onerel, vacrelstats);

	/* Update statistics in pg_class */
	vac_update_relstats(RelationGetRelid(onerel), vacrelstats->rel_pages,
						vacrelstats->rel_tuples, hasindex);
}


/*
 *	lazy_scan_heap() -- scan an open heap relation
 *
 *		This routine sets commit status bits, builds lists of dead tuples
 *		and pages with free space, and calculates statistics on the number
 *		of live tuples in the heap.  When done, or when we run low on space
 *		for dead-tuple TIDs, invoke vacuuming of indexes and heap.
 */
static void
lazy_scan_heap(Relation onerel, LVRelStats *vacrelstats,
			   Relation *Irel, int nindexes)
{
	BlockNumber nblocks,
				blkno;
	HeapTupleData tuple;
	char	   *relname;
	BlockNumber empty_pages,
				changed_pages;
	double		num_tuples,
				tups_vacuumed,
				nkeep,
				nunused;
	bool		did_vacuum_index = false;
	int			i;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);

	relname = RelationGetRelationName(onerel);
	elog(MESSAGE_LEVEL, "--Relation %s--", relname);

	empty_pages = changed_pages = 0;
	num_tuples = tups_vacuumed = nkeep = nunused = 0;

	nblocks = RelationGetNumberOfBlocks(onerel);
	vacrelstats->rel_pages = nblocks;
	vacrelstats->nonempty_pages = 0;

	lazy_space_alloc(vacrelstats, nblocks);

	for (blkno = 0; blkno < nblocks; blkno++)
	{
		Buffer		buf;
		Page		page;
		OffsetNumber offnum,
					maxoff;
		bool		pgchanged,
					tupgone,
					hastup;
		int			prev_dead_count;

		CHECK_FOR_INTERRUPTS();

		/*
		 * If we are close to overrunning the available space for
		 * dead-tuple TIDs, pause and do a cycle of vacuuming before we
		 * tackle this page.
		 */
		if ((vacrelstats->max_dead_tuples - vacrelstats->num_dead_tuples) < MAX_TUPLES_PER_PAGE &&
			vacrelstats->num_dead_tuples > 0)
		{
			/* Remove index entries */
			for (i = 0; i < nindexes; i++)
				lazy_vacuum_index(Irel[i], vacrelstats);
			did_vacuum_index = true;
			/* Remove tuples from heap */
			lazy_vacuum_heap(onerel, vacrelstats);
			/* Forget the now-vacuumed tuples, and press on */
			vacrelstats->num_dead_tuples = 0;
		}

		buf = ReadBuffer(onerel, blkno);

		/* In this phase we only need shared access to the buffer */
		LockBuffer(buf, BUFFER_LOCK_SHARE);

		page = BufferGetPage(buf);

		if (PageIsNew(page))
		{
			/* Not sure we still need to handle this case, but... */
			LockBuffer(buf, BUFFER_LOCK_UNLOCK);
			LockBuffer(buf, BUFFER_LOCK_EXCLUSIVE);
			if (PageIsNew(page))
			{
				elog(NOTICE, "Rel %s: Uninitialized page %u - fixing",
					 relname, blkno);
				PageInit(page, BufferGetPageSize(buf), 0);
				lazy_record_free_space(vacrelstats, blkno,
									   PageGetFreeSpace(page));
			}
			LockBuffer(buf, BUFFER_LOCK_UNLOCK);
			WriteBuffer(buf);
			continue;
		}

		if (PageIsEmpty(page))
		{
			empty_pages++;
			lazy_record_free_space(vacrelstats, blkno,
								   PageGetFreeSpace(page));
			LockBuffer(buf, BUFFER_LOCK_UNLOCK);
			ReleaseBuffer(buf);
			continue;
		}

		pgchanged = false;
		hastup = false;
		prev_dead_count = vacrelstats->num_dead_tuples;
		maxoff = PageGetMaxOffsetNumber(page);
		for (offnum = FirstOffsetNumber;
			 offnum <= maxoff;
			 offnum = OffsetNumberNext(offnum))
		{
			ItemId		itemid;
			uint16		sv_infomask;

			itemid = PageGetItemId(page, offnum);

			if (!ItemIdIsUsed(itemid))
			{
				nunused += 1;
				continue;
			}

			tuple.t_datamcxt = NULL;
			tuple.t_data = (HeapTupleHeader) PageGetItem(page, itemid);
			tuple.t_len = ItemIdGetLength(itemid);
			ItemPointerSet(&(tuple.t_self), blkno, offnum);

			tupgone = false;
			sv_infomask = tuple.t_data->t_infomask;

			switch (HeapTupleSatisfiesVacuum(tuple.t_data, OldestXmin))
			{
				case HEAPTUPLE_DEAD:
					tupgone = true;		/* we can delete the tuple */
					break;
				case HEAPTUPLE_LIVE:

					/*
					 * Tuple is good.  Consider whether to replace its
					 * xmin value with FrozenTransactionId.
					 *
					 * NB: Since we hold only a shared buffer lock here, we
					 * are assuming that TransactionId read/write is
					 * atomic.	This is not the only place that makes such
					 * an assumption.  It'd be possible to avoid the
					 * assumption by momentarily acquiring exclusive lock,
					 * but for the moment I see no need to.
					 */
					if (TransactionIdIsNormal(tuple.t_data->t_xmin) &&
						TransactionIdPrecedes(tuple.t_data->t_xmin,
											  FreezeLimit))
					{
						tuple.t_data->t_xmin = FrozenTransactionId;
						/* infomask should be okay already */
						Assert(tuple.t_data->t_infomask & HEAP_XMIN_COMMITTED);
						pgchanged = true;
					}
					break;
				case HEAPTUPLE_RECENTLY_DEAD:

					/*
					 * If tuple is recently deleted then we must not
					 * remove it from relation.
					 */
					nkeep += 1;
					break;
				case HEAPTUPLE_INSERT_IN_PROGRESS:
					/* This is an expected case during concurrent vacuum */
					break;
				case HEAPTUPLE_DELETE_IN_PROGRESS:
					/* This is an expected case during concurrent vacuum */
					break;
				default:
					elog(ERROR, "Unexpected HeapTupleSatisfiesVacuum result");
					break;
			}

			/* check for hint-bit update by HeapTupleSatisfiesVacuum */
			if (sv_infomask != tuple.t_data->t_infomask)
				pgchanged = true;

			/*
			 * Other checks...
			 */
			if (!OidIsValid(tuple.t_data->t_oid) &&
				onerel->rd_rel->relhasoids)
				elog(NOTICE, "Rel %s: TID %u/%u: OID IS INVALID. TUPGONE %d.",
					 relname, blkno, offnum, (int) tupgone);

			if (tupgone)
			{
				lazy_record_dead_tuple(vacrelstats, &(tuple.t_self));
				tups_vacuumed += 1;
			}
			else
			{
				num_tuples += 1;
				hastup = true;
			}
		}						/* scan along page */

		/*
		 * If we remembered any tuples for deletion, then the page will be
		 * visited again by lazy_vacuum_heap, which will compute and
		 * record its post-compaction free space.  If not, then we're done
		 * with this page, so remember its free space as-is.
		 */
		if (vacrelstats->num_dead_tuples == prev_dead_count)
		{
			lazy_record_free_space(vacrelstats, blkno,
								   PageGetFreeSpace(page));
		}

		/* Remember the location of the last page with nonremovable tuples */
		if (hastup)
			vacrelstats->nonempty_pages = blkno + 1;

		LockBuffer(buf, BUFFER_LOCK_UNLOCK);

		if (pgchanged)
		{
			SetBufferCommitInfoNeedsSave(buf);
			changed_pages++;
		}

		ReleaseBuffer(buf);
	}

	/* save stats for use later */
	vacrelstats->rel_tuples = num_tuples;

	/* If any tuples need to be deleted, perform final vacuum cycle */
	/* XXX put a threshold on min nuber of tuples here? */
	if (vacrelstats->num_dead_tuples > 0)
	{
		/* Remove index entries */
		for (i = 0; i < nindexes; i++)
			lazy_vacuum_index(Irel[i], vacrelstats);
		/* Remove tuples from heap */
		lazy_vacuum_heap(onerel, vacrelstats);
	}
	else if (!did_vacuum_index)
	{
		/* Scan indexes just to update pg_class statistics about them */
		for (i = 0; i < nindexes; i++)
			lazy_scan_index(Irel[i], vacrelstats);
	}

	elog(MESSAGE_LEVEL, "Pages %u: Changed %u, Empty %u; \
Tup %.0f: Vac %.0f, Keep %.0f, UnUsed %.0f.\n\tTotal %s",
		 nblocks, changed_pages, empty_pages,
		 num_tuples, tups_vacuumed, nkeep, nunused,
		 vac_show_rusage(&ru0));
}


/*
 *	lazy_vacuum_heap() -- second pass over the heap
 *
 *		This routine marks dead tuples as unused and compacts out free
 *		space on their pages.  Pages not having dead tuples recorded from
 *		lazy_scan_heap are not visited at all.
 *
 * Note: the reason for doing this as a second pass is we cannot remove
 * the tuples until we've removed their index entries, and we want to
 * process index entry removal in batches as large as possible.
 */
static void
lazy_vacuum_heap(Relation onerel, LVRelStats *vacrelstats)
{
	int			tupindex;
	int			npages;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);
	npages = 0;

	tupindex = 0;
	while (tupindex < vacrelstats->num_dead_tuples)
	{
		BlockNumber tblk;
		Buffer		buf;
		Page		page;

		CHECK_FOR_INTERRUPTS();

		tblk = ItemPointerGetBlockNumber(&vacrelstats->dead_tuples[tupindex]);
		buf = ReadBuffer(onerel, tblk);
		LockBufferForCleanup(buf);
		tupindex = lazy_vacuum_page(onerel, tblk, buf, tupindex, vacrelstats);
		/* Now that we've compacted the page, record its available space */
		page = BufferGetPage(buf);
		lazy_record_free_space(vacrelstats, tblk,
							   PageGetFreeSpace(page));
		LockBuffer(buf, BUFFER_LOCK_UNLOCK);
		WriteBuffer(buf);
		npages++;
	}

	elog(MESSAGE_LEVEL, "Removed %d tuples in %d pages.\n\t%s",
		 tupindex, npages,
		 vac_show_rusage(&ru0));
}

/*
 *	lazy_vacuum_page() -- free dead tuples on a page
 *					 and repair its fragmentation.
 *
 * Caller is expected to handle reading, locking, and writing the buffer.
 *
 * tupindex is the index in vacrelstats->dead_tuples of the first dead
 * tuple for this page.  We assume the rest follow sequentially.
 * The return value is the first tupindex after the tuples of this page.
 */
static int
lazy_vacuum_page(Relation onerel, BlockNumber blkno, Buffer buffer,
				 int tupindex, LVRelStats *vacrelstats)
{
	OffsetNumber unbuf[BLCKSZ / sizeof(OffsetNumber)];
	OffsetNumber *unused = unbuf;
	int			uncnt;
	Page		page = BufferGetPage(buffer);
	ItemId		itemid;

	START_CRIT_SECTION();
	for (; tupindex < vacrelstats->num_dead_tuples; tupindex++)
	{
		BlockNumber tblk;
		OffsetNumber toff;

		tblk = ItemPointerGetBlockNumber(&vacrelstats->dead_tuples[tupindex]);
		if (tblk != blkno)
			break;				/* past end of tuples for this block */
		toff = ItemPointerGetOffsetNumber(&vacrelstats->dead_tuples[tupindex]);
		itemid = PageGetItemId(page, toff);
		itemid->lp_flags &= ~LP_USED;
	}

	uncnt = PageRepairFragmentation(page, unused);

	{
		XLogRecPtr	recptr;

		recptr = log_heap_clean(onerel, buffer, (char *) unused,
						  (char *) (&(unused[uncnt])) - (char *) unused);
		PageSetLSN(page, recptr);
		PageSetSUI(page, ThisStartUpID);
	}
	END_CRIT_SECTION();

	return tupindex;
}

/*
 *	lazy_scan_index() -- scan one index relation to update pg_class statistic.
 *
 * We use this when we have no deletions to do.
 */
static void
lazy_scan_index(Relation indrel, LVRelStats *vacrelstats)
{
	IndexBulkDeleteResult *stats;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);

	/*
	 * If the index is not partial, skip the scan, and just assume it has
	 * the same number of tuples as the heap.
	 */
	if (!vac_is_partial_index(indrel))
	{
		vac_update_relstats(RelationGetRelid(indrel),
							RelationGetNumberOfBlocks(indrel),
							vacrelstats->rel_tuples,
							false);
		return;
	}

	/*
	 * If index is unsafe for concurrent access, must lock it; but a
	 * shared lock should be sufficient.
	 */
	if (!indrel->rd_am->amconcurrent)
		LockRelation(indrel, AccessShareLock);

	/*
	 * Even though we're not planning to delete anything, use the
	 * ambulkdelete call, so that the scan happens within the index AM for
	 * more speed.
	 */
	stats = index_bulk_delete(indrel, dummy_tid_reaped, NULL);

	/*
	 * Release lock acquired above.
	 */
	if (!indrel->rd_am->amconcurrent)
		UnlockRelation(indrel, AccessShareLock);

	if (!stats)
		return;

	/* now update statistics in pg_class */
	vac_update_relstats(RelationGetRelid(indrel),
						stats->num_pages, stats->num_index_tuples,
						false);

	elog(MESSAGE_LEVEL, "Index %s: Pages %u; Tuples %.0f.\n\t%s",
		 RelationGetRelationName(indrel),
		 stats->num_pages, stats->num_index_tuples,
		 vac_show_rusage(&ru0));

	pfree(stats);
}

/*
 *	lazy_vacuum_index() -- vacuum one index relation.
 *
 *		Delete all the index entries pointing to tuples listed in
 *		vacrelstats->dead_tuples.
 *
 *		Finally, we arrange to update the index relation's statistics in
 *		pg_class.
 */
static void
lazy_vacuum_index(Relation indrel, LVRelStats *vacrelstats)
{
	IndexBulkDeleteResult *stats;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);

	/*
	 * If index is unsafe for concurrent access, must lock it.
	 */
	if (!indrel->rd_am->amconcurrent)
		LockRelation(indrel, AccessExclusiveLock);

	/* Do bulk deletion */
	stats = index_bulk_delete(indrel, lazy_tid_reaped, (void *) vacrelstats);

	/*
	 * Release lock acquired above.
	 */
	if (!indrel->rd_am->amconcurrent)
		UnlockRelation(indrel, AccessExclusiveLock);

	/* now update statistics in pg_class */
	if (stats)
	{
		vac_update_relstats(RelationGetRelid(indrel),
							stats->num_pages, stats->num_index_tuples,
							false);

		elog(MESSAGE_LEVEL, "Index %s: Pages %u; Tuples %.0f: Deleted %.0f.\n\t%s",
			 RelationGetRelationName(indrel), stats->num_pages,
			 stats->num_index_tuples, stats->tuples_removed,
			 vac_show_rusage(&ru0));

		pfree(stats);
	}
}

/*
 * lazy_truncate_heap - try to truncate off any empty pages at the end
 */
static void
lazy_truncate_heap(Relation onerel, LVRelStats *vacrelstats)
{
	BlockNumber old_rel_pages = vacrelstats->rel_pages;
	BlockNumber new_rel_pages;
	BlockNumber *pages;
	Size	   *spaceavail;
	int			n;
	int			i,
				j;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);

	/*
	 * We need full exclusive lock on the relation in order to do
	 * truncation. If we can't get it, give up rather than waiting --- we
	 * don't want to block other backends, and we don't want to deadlock
	 * (which is quite possible considering we already hold a lower-grade
	 * lock).
	 */
	if (!ConditionalLockRelation(onerel, AccessExclusiveLock))
		return;

	/*
	 * Now that we have exclusive lock, look to see if the rel has grown
	 * whilst we were vacuuming with non-exclusive lock.  If so, give up;
	 * the newly added pages presumably contain non-deletable tuples.
	 */
	new_rel_pages = RelationGetNumberOfBlocks(onerel);
	if (new_rel_pages != old_rel_pages)
	{
		/* might as well use the latest news when we update pg_class stats */
		vacrelstats->rel_pages = new_rel_pages;
		UnlockRelation(onerel, AccessExclusiveLock);
		return;
	}

	/*
	 * Scan backwards from the end to verify that the end pages actually
	 * contain nothing we need to keep.  This is *necessary*, not
	 * optional, because other backends could have added tuples to these
	 * pages whilst we were vacuuming.
	 */
	new_rel_pages = count_nondeletable_pages(onerel, vacrelstats);

	if (new_rel_pages >= old_rel_pages)
	{
		/* can't do anything after all */
		UnlockRelation(onerel, AccessExclusiveLock);
		return;
	}

	/*
	 * Okay to truncate.
	 *
	 * First, flush any shared buffers for the blocks we intend to delete.
	 * FlushRelationBuffers is a bit more than we need for this, since it
	 * will also write out dirty buffers for blocks we aren't deleting,
	 * but it's the closest thing in bufmgr's API.
	 */
	i = FlushRelationBuffers(onerel, new_rel_pages);
	if (i < 0)
		elog(ERROR, "VACUUM (lazy_truncate_heap): FlushRelationBuffers returned %d",
			 i);

	/*
	 * Do the physical truncation.
	 */
	new_rel_pages = smgrtruncate(DEFAULT_SMGR, onerel, new_rel_pages);
	onerel->rd_nblocks = new_rel_pages; /* update relcache immediately */
	onerel->rd_targblock = InvalidBlockNumber;
	vacrelstats->rel_pages = new_rel_pages;		/* save new number of
												 * blocks */

	/*
	 * Drop free-space info for removed blocks; these must not get entered
	 * into the FSM!
	 */
	pages = vacrelstats->free_pages;
	spaceavail = vacrelstats->free_spaceavail;
	n = vacrelstats->num_free_pages;
	j = 0;
	for (i = 0; i < n; i++)
	{
		if (pages[i] < new_rel_pages)
		{
			pages[j] = pages[i];
			spaceavail[j] = spaceavail[i];
			j++;
		}
	}
	vacrelstats->num_free_pages = j;

	/*
	 * We keep the exclusive lock until commit (perhaps not necessary)?
	 */

	elog(MESSAGE_LEVEL, "Truncated %u --> %u pages.\n\t%s",
		 old_rel_pages, new_rel_pages,
		 vac_show_rusage(&ru0));
}

/*
 * Rescan end pages to verify that they are (still) empty of needed tuples.
 *
 * Returns number of nondeletable pages (last nonempty page + 1).
 */
static BlockNumber
count_nondeletable_pages(Relation onerel, LVRelStats *vacrelstats)
{
	BlockNumber blkno;
	HeapTupleData tuple;

	/* Strange coding of loop control is needed because blkno is unsigned */
	blkno = vacrelstats->rel_pages;
	while (blkno > vacrelstats->nonempty_pages)
	{
		Buffer		buf;
		Page		page;
		OffsetNumber offnum,
					maxoff;
		bool		pgchanged,
					tupgone,
					hastup;

		CHECK_FOR_INTERRUPTS();

		blkno--;

		buf = ReadBuffer(onerel, blkno);

		/* In this phase we only need shared access to the buffer */
		LockBuffer(buf, BUFFER_LOCK_SHARE);

		page = BufferGetPage(buf);

		if (PageIsNew(page) || PageIsEmpty(page))
		{
			/* PageIsNew robably shouldn't happen... */
			LockBuffer(buf, BUFFER_LOCK_UNLOCK);
			ReleaseBuffer(buf);
			continue;
		}

		pgchanged = false;
		hastup = false;
		maxoff = PageGetMaxOffsetNumber(page);
		for (offnum = FirstOffsetNumber;
			 offnum <= maxoff;
			 offnum = OffsetNumberNext(offnum))
		{
			ItemId		itemid;
			uint16		sv_infomask;

			itemid = PageGetItemId(page, offnum);

			if (!ItemIdIsUsed(itemid))
				continue;

			tuple.t_datamcxt = NULL;
			tuple.t_data = (HeapTupleHeader) PageGetItem(page, itemid);
			tuple.t_len = ItemIdGetLength(itemid);
			ItemPointerSet(&(tuple.t_self), blkno, offnum);

			tupgone = false;
			sv_infomask = tuple.t_data->t_infomask;

			switch (HeapTupleSatisfiesVacuum(tuple.t_data, OldestXmin))
			{
				case HEAPTUPLE_DEAD:
					tupgone = true;		/* we can delete the tuple */
					break;
				case HEAPTUPLE_LIVE:
					/* Shouldn't be necessary to re-freeze anything */
					break;
				case HEAPTUPLE_RECENTLY_DEAD:

					/*
					 * If tuple is recently deleted then we must not
					 * remove it from relation.
					 */
					break;
				case HEAPTUPLE_INSERT_IN_PROGRESS:
					/* This is an expected case during concurrent vacuum */
					break;
				case HEAPTUPLE_DELETE_IN_PROGRESS:
					/* This is an expected case during concurrent vacuum */
					break;
				default:
					elog(ERROR, "Unexpected HeapTupleSatisfiesVacuum result");
					break;
			}

			/* check for hint-bit update by HeapTupleSatisfiesVacuum */
			if (sv_infomask != tuple.t_data->t_infomask)
				pgchanged = true;

			if (!tupgone)
			{
				hastup = true;
				break;			/* can stop scanning */
			}
		}						/* scan along page */

		LockBuffer(buf, BUFFER_LOCK_UNLOCK);

		if (pgchanged)
			WriteBuffer(buf);
		else
			ReleaseBuffer(buf);

		/* Done scanning if we found a tuple here */
		if (hastup)
			return blkno + 1;
	}

	/*
	 * If we fall out of the loop, all the previously-thought-to-be-empty
	 * pages really are; we need not bother to look at the last
	 * known-nonempty page.
	 */
	return vacrelstats->nonempty_pages;
}

/*
 * lazy_space_alloc - space allocation decisions for lazy vacuum
 *
 * See the comments at the head of this file for rationale.
 */
static void
lazy_space_alloc(LVRelStats *vacrelstats, BlockNumber relblocks)
{
	int			maxtuples;
	int			maxpages;

	maxtuples = (int) ((VacuumMem * 1024L) / sizeof(ItemPointerData));
	/* stay sane if small VacuumMem */
	if (maxtuples < MAX_TUPLES_PER_PAGE)
		maxtuples = MAX_TUPLES_PER_PAGE;

	vacrelstats->num_dead_tuples = 0;
	vacrelstats->max_dead_tuples = maxtuples;
	vacrelstats->dead_tuples = (ItemPointer)
		palloc(maxtuples * sizeof(ItemPointerData));

	maxpages = MaxFSMPages;
	/* No need to allocate more pages than the relation has blocks */
	if (relblocks < (BlockNumber) maxpages)
		maxpages = (int) relblocks;
	/* avoid palloc(0) */
	if (maxpages < 1)
		maxpages = 1;

	vacrelstats->fs_is_heap = false;
	vacrelstats->num_free_pages = 0;
	vacrelstats->max_free_pages = maxpages;
	vacrelstats->free_pages = (BlockNumber *)
		palloc(maxpages * sizeof(BlockNumber));
	vacrelstats->free_spaceavail = (Size *)
		palloc(maxpages * sizeof(Size));
}

/*
 * lazy_record_dead_tuple - remember one deletable tuple
 */
static void
lazy_record_dead_tuple(LVRelStats *vacrelstats,
					   ItemPointer itemptr)
{
	/*
	 * The array shouldn't overflow under normal behavior, but perhaps it
	 * could if we are given a really small VacuumMem. In that case, just
	 * forget the last few tuples.
	 */
	if (vacrelstats->num_dead_tuples < vacrelstats->max_dead_tuples)
	{
		vacrelstats->dead_tuples[vacrelstats->num_dead_tuples] = *itemptr;
		vacrelstats->num_dead_tuples++;
	}
}

/*
 * lazy_record_free_space - remember free space on one page
 */
static void
lazy_record_free_space(LVRelStats *vacrelstats,
					   BlockNumber page,
					   Size avail)
{
	BlockNumber *pages;
	Size	   *spaceavail;
	int			n;

	/* Ignore pages with little free space */
	if (avail < PAGE_SPACE_THRESHOLD)
		return;

	/* Copy pointers to local variables for notational simplicity */
	pages = vacrelstats->free_pages;
	spaceavail = vacrelstats->free_spaceavail;
	n = vacrelstats->max_free_pages;

	/* If we haven't filled the array yet, just keep adding entries */
	if (vacrelstats->num_free_pages < n)
	{
		pages[vacrelstats->num_free_pages] = page;
		spaceavail[vacrelstats->num_free_pages] = avail;
		vacrelstats->num_free_pages++;
		return;
	}

	/*----------
	 * The rest of this routine works with "heap" organization of the
	 * free space arrays, wherein we maintain the heap property
	 *			spaceavail[(j-1) div 2] <= spaceavail[j]  for 0 < j < n.
	 * In particular, the zero'th element always has the smallest available
	 * space and can be discarded to make room for a new page with more space.
	 * See Knuth's discussion of heap-based priority queues, sec 5.2.3;
	 * but note he uses 1-origin array subscripts, not 0-origin.
	 *----------
	 */

	/* If we haven't yet converted the array to heap organization, do it */
	if (!vacrelstats->fs_is_heap)
	{
		/*
		 * Scan backwards through the array, "sift-up" each value into its
		 * correct position.  We can start the scan at n/2-1 since each
		 * entry above that position has no children to worry about.
		 */
		int			l = n / 2;

		while (--l >= 0)
		{
			BlockNumber R = pages[l];
			Size		K = spaceavail[l];
			int			i;		/* i is where the "hole" is */

			i = l;
			for (;;)
			{
				int			j = 2 * i + 1;

				if (j >= n)
					break;
				if (j + 1 < n && spaceavail[j] > spaceavail[j + 1])
					j++;
				if (K <= spaceavail[j])
					break;
				pages[i] = pages[j];
				spaceavail[i] = spaceavail[j];
				i = j;
			}
			pages[i] = R;
			spaceavail[i] = K;
		}

		vacrelstats->fs_is_heap = true;
	}

	/* If new page has more than zero'th entry, insert it into heap */
	if (avail > spaceavail[0])
	{
		/*
		 * Notionally, we replace the zero'th entry with the new data, and
		 * then sift-up to maintain the heap property.	Physically, the
		 * new data doesn't get stored into the arrays until we find the
		 * right location for it.
		 */
		int			i = 0;		/* i is where the "hole" is */

		for (;;)
		{
			int			j = 2 * i + 1;

			if (j >= n)
				break;
			if (j + 1 < n && spaceavail[j] > spaceavail[j + 1])
				j++;
			if (avail <= spaceavail[j])
				break;
			pages[i] = pages[j];
			spaceavail[i] = spaceavail[j];
			i = j;
		}
		pages[i] = page;
		spaceavail[i] = avail;
	}
}

/*
 *	lazy_tid_reaped() -- is a particular tid deletable?
 *
 *		This has the right signature to be an IndexBulkDeleteCallback.
 *
 *		Assumes dead_tuples array is in sorted order.
 */
static bool
lazy_tid_reaped(ItemPointer itemptr, void *state)
{
	LVRelStats *vacrelstats = (LVRelStats *) state;
	ItemPointer res;

	res = (ItemPointer) bsearch((void *) itemptr,
								(void *) vacrelstats->dead_tuples,
								vacrelstats->num_dead_tuples,
								sizeof(ItemPointerData),
								vac_cmp_itemptr);

	return (res != NULL);
}

/*
 * Dummy version for lazy_scan_index.
 */
static bool
dummy_tid_reaped(ItemPointer itemptr, void *state)
{
	return false;
}

/*
 * Update the shared Free Space Map with the info we now have about
 * free space in the relation, discarding any old info the map may have.
 */
static void
lazy_update_fsm(Relation onerel, LVRelStats *vacrelstats)
{
	/*
	 * Since MultiRecordFreeSpace doesn't currently impose any
	 * restrictions on the ordering of the input, we can just pass it the
	 * arrays as-is, whether they are in heap or linear order.
	 */
	MultiRecordFreeSpace(&onerel->rd_node,
						 0, MaxBlockNumber,
						 vacrelstats->num_free_pages,
						 vacrelstats->free_pages,
						 vacrelstats->free_spaceavail);
}

/*
 * Comparator routines for use with qsort() and bsearch().
 */
static int
vac_cmp_itemptr(const void *left, const void *right)
{
	BlockNumber lblk,
				rblk;
	OffsetNumber loff,
				roff;

	lblk = ItemPointerGetBlockNumber((ItemPointer) left);
	rblk = ItemPointerGetBlockNumber((ItemPointer) right);

	if (lblk < rblk)
		return -1;
	if (lblk > rblk)
		return 1;

	loff = ItemPointerGetOffsetNumber((ItemPointer) left);
	roff = ItemPointerGetOffsetNumber((ItemPointer) right);

	if (loff < roff)
		return -1;
	if (loff > roff)
		return 1;

	return 0;
}
