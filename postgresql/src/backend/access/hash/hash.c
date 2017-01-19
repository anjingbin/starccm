/*-------------------------------------------------------------------------
 *
 * hash.c
 *	  Implementation of Margo Seltzer's Hashing package for postgres.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/access/hash/hash.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 * NOTES
 *	  This file contains only the public interface routines.
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/genam.h"
#include "access/hash.h"
#include "access/heapam.h"
#include "access/xlogutils.h"
#include "catalog/index.h"
#include "executor/executor.h"
#include "miscadmin.h"


bool		BuildingHash = false;


/* Working state for hashbuild and its callback */
typedef struct
{
	double		indtuples;
} HashBuildState;

static void hashbuildCallback(Relation index,
				  HeapTuple htup,
				  Datum *attdata,
				  char *nulls,
				  bool tupleIsAlive,
				  void *state);


/*
 *	hashbuild() -- build a new hash index.
 *
 *		We use a global variable to record the fact that we're creating
 *		a new index.  This is used to avoid high-concurrency locking,
 *		since the index won't be visible until this transaction commits
 *		and since building is guaranteed to be single-threaded.
 */
Datum
hashbuild(PG_FUNCTION_ARGS)
{
	Relation	heap = (Relation) PG_GETARG_POINTER(0);
	Relation	index = (Relation) PG_GETARG_POINTER(1);
	IndexInfo  *indexInfo = (IndexInfo *) PG_GETARG_POINTER(2);
	double		reltuples;
	HashBuildState buildstate;

	/* set flag to disable locking */
	BuildingHash = true;

	/*
	 * We expect to be called exactly once for any index relation. If
	 * that's not the case, big trouble's what we have.
	 */
	if (RelationGetNumberOfBlocks(index) != 0)
		elog(ERROR, "%s already contains data",
			 RelationGetRelationName(index));

	/* initialize the hash index metadata page */
	_hash_metapinit(index);

	/* build the index */
	buildstate.indtuples = 0;

	/* do the heap scan */
	reltuples = IndexBuildHeapScan(heap, index, indexInfo,
								hashbuildCallback, (void *) &buildstate);

	/* all done */
	BuildingHash = false;

	/*
	 * Since we just counted the tuples in the heap, we update its stats
	 * in pg_class to guarantee that the planner takes advantage of the
	 * index we just created.  But, only update statistics during normal
	 * index definitions, not for indices on system catalogs created
	 * during bootstrap processing.  We must close the relations before
	 * updating statistics to guarantee that the relcache entries are
	 * flushed when we increment the command counter in UpdateStats(). But
	 * we do not release any locks on the relations; those will be held
	 * until end of transaction.
	 */
	if (IsNormalProcessingMode())
	{
		Oid			hrelid = RelationGetRelid(heap);
		Oid			irelid = RelationGetRelid(index);

		heap_close(heap, NoLock);
		index_close(index);
		UpdateStats(hrelid, reltuples);
		UpdateStats(irelid, buildstate.indtuples);
	}

	PG_RETURN_VOID();
}

/*
 * Per-tuple callback from IndexBuildHeapScan
 */
static void
hashbuildCallback(Relation index,
				  HeapTuple htup,
				  Datum *attdata,
				  char *nulls,
				  bool tupleIsAlive,
				  void *state)
{
	HashBuildState *buildstate = (HashBuildState *) state;
	IndexTuple	itup;
	HashItem	hitem;
	InsertIndexResult res;

	/* form an index tuple and point it at the heap tuple */
	itup = index_formtuple(RelationGetDescr(index), attdata, nulls);
	itup->t_tid = htup->t_self;

	/* Hash indexes don't index nulls, see notes in hashinsert */
	if (IndexTupleHasNulls(itup))
	{
		pfree(itup);
		return;
	}

	hitem = _hash_formitem(itup);

	res = _hash_doinsert(index, hitem);

	if (res)
		pfree(res);

	buildstate->indtuples += 1;

	pfree(hitem);
	pfree(itup);
}

/*
 *	hashinsert() -- insert an index tuple into a hash table.
 *
 *	Hash on the index tuple's key, find the appropriate location
 *	for the new tuple, put it there, and return an InsertIndexResult
 *	to the caller.
 */
Datum
hashinsert(PG_FUNCTION_ARGS)
{
	Relation	rel = (Relation) PG_GETARG_POINTER(0);
	Datum	   *datum = (Datum *) PG_GETARG_POINTER(1);
	char	   *nulls = (char *) PG_GETARG_POINTER(2);
	ItemPointer ht_ctid = (ItemPointer) PG_GETARG_POINTER(3);

#ifdef NOT_USED
	Relation	heapRel = (Relation) PG_GETARG_POINTER(4);
#endif
	InsertIndexResult res;
	HashItem	hitem;
	IndexTuple	itup;

	/* generate an index tuple */
	itup = index_formtuple(RelationGetDescr(rel), datum, nulls);
	itup->t_tid = *ht_ctid;

	/*
	 * If the single index key is null, we don't insert it into the index.
	 * Hash tables support scans on '='. Relational algebra says that A =
	 * B returns null if either A or B is null.  This means that no
	 * qualification used in an index scan could ever return true on a
	 * null attribute.	It also means that indices can't be used by ISNULL
	 * or NOTNULL scans, but that's an artifact of the strategy map
	 * architecture chosen in 1986, not of the way nulls are handled here.
	 */
	if (IndexTupleHasNulls(itup))
	{
		pfree(itup);
		PG_RETURN_POINTER((InsertIndexResult) NULL);
	}

	hitem = _hash_formitem(itup);

	res = _hash_doinsert(rel, hitem);

	pfree(hitem);
	pfree(itup);

	PG_RETURN_POINTER(res);
}


/*
 *	hashgettuple() -- Get the next tuple in the scan.
 */
Datum
hashgettuple(PG_FUNCTION_ARGS)
{
	IndexScanDesc scan = (IndexScanDesc) PG_GETARG_POINTER(0);
	ScanDirection dir = (ScanDirection) PG_GETARG_INT32(1);
	RetrieveIndexResult res;

	/*
	 * If we've already initialized this scan, we can just advance it in
	 * the appropriate direction.  If we haven't done so yet, we call a
	 * routine to get the first item in the scan.
	 */

	if (ItemPointerIsValid(&(scan->currentItemData)))
		res = _hash_next(scan, dir);
	else
		res = _hash_first(scan, dir);

	PG_RETURN_POINTER(res);
}


/*
 *	hashbeginscan() -- start a scan on a hash index
 */
Datum
hashbeginscan(PG_FUNCTION_ARGS)
{
	Relation	rel = (Relation) PG_GETARG_POINTER(0);
	bool		fromEnd = PG_GETARG_BOOL(1);
	uint16		keysz = PG_GETARG_UINT16(2);
	ScanKey		scankey = (ScanKey) PG_GETARG_POINTER(3);
	IndexScanDesc scan;
	HashScanOpaque so;

	scan = RelationGetIndexScan(rel, fromEnd, keysz, scankey);
	so = (HashScanOpaque) palloc(sizeof(HashScanOpaqueData));
	so->hashso_curbuf = so->hashso_mrkbuf = InvalidBuffer;
	scan->opaque = so;
	scan->flags = 0x0;

	/* register scan in case we change pages it's using */
	_hash_regscan(scan);

	PG_RETURN_POINTER(scan);
}

/*
 *	hashrescan() -- rescan an index relation
 */
Datum
hashrescan(PG_FUNCTION_ARGS)
{
	IndexScanDesc scan = (IndexScanDesc) PG_GETARG_POINTER(0);

#ifdef NOT_USED					/* XXX surely it's wrong to ignore this? */
	bool		fromEnd = PG_GETARG_BOOL(1);
#endif
	ScanKey		scankey = (ScanKey) PG_GETARG_POINTER(2);
	ItemPointer iptr;
	HashScanOpaque so;

	so = (HashScanOpaque) scan->opaque;

	/* we hold a read lock on the current page in the scan */
	if (ItemPointerIsValid(iptr = &(scan->currentItemData)))
	{
		_hash_relbuf(scan->relation, so->hashso_curbuf, HASH_READ);
		so->hashso_curbuf = InvalidBuffer;
		ItemPointerSetInvalid(iptr);
	}
	if (ItemPointerIsValid(iptr = &(scan->currentMarkData)))
	{
		_hash_relbuf(scan->relation, so->hashso_mrkbuf, HASH_READ);
		so->hashso_mrkbuf = InvalidBuffer;
		ItemPointerSetInvalid(iptr);
	}

	/* reset the scan key */
	if (scan->numberOfKeys > 0)
	{
		memmove(scan->keyData,
				scankey,
				scan->numberOfKeys * sizeof(ScanKeyData));
	}

	PG_RETURN_VOID();
}

/*
 *	hashendscan() -- close down a scan
 */
Datum
hashendscan(PG_FUNCTION_ARGS)
{
	IndexScanDesc scan = (IndexScanDesc) PG_GETARG_POINTER(0);
	ItemPointer iptr;
	HashScanOpaque so;

	so = (HashScanOpaque) scan->opaque;

	/* release any locks we still hold */
	if (ItemPointerIsValid(iptr = &(scan->currentItemData)))
	{
		_hash_relbuf(scan->relation, so->hashso_curbuf, HASH_READ);
		so->hashso_curbuf = InvalidBuffer;
		ItemPointerSetInvalid(iptr);
	}

	if (ItemPointerIsValid(iptr = &(scan->currentMarkData)))
	{
		if (BufferIsValid(so->hashso_mrkbuf))
			_hash_relbuf(scan->relation, so->hashso_mrkbuf, HASH_READ);
		so->hashso_mrkbuf = InvalidBuffer;
		ItemPointerSetInvalid(iptr);
	}

	/* don't need scan registered anymore */
	_hash_dropscan(scan);

	/* be tidy */
	pfree(scan->opaque);

	PG_RETURN_VOID();
}

/*
 *	hashmarkpos() -- save current scan position
 *
 */
Datum
hashmarkpos(PG_FUNCTION_ARGS)
{
	IndexScanDesc scan = (IndexScanDesc) PG_GETARG_POINTER(0);
	ItemPointer iptr;
	HashScanOpaque so;

	so = (HashScanOpaque) scan->opaque;

	/* release lock on old marked data, if any */
	if (ItemPointerIsValid(iptr = &(scan->currentMarkData)))
	{
		_hash_relbuf(scan->relation, so->hashso_mrkbuf, HASH_READ);
		so->hashso_mrkbuf = InvalidBuffer;
		ItemPointerSetInvalid(iptr);
	}

	/* bump lock on currentItemData and copy to currentMarkData */
	if (ItemPointerIsValid(&(scan->currentItemData)))
	{
		so->hashso_mrkbuf = _hash_getbuf(scan->relation,
								 BufferGetBlockNumber(so->hashso_curbuf),
										 HASH_READ);
		scan->currentMarkData = scan->currentItemData;
	}

	PG_RETURN_VOID();
}

/*
 *	hashrestrpos() -- restore scan to last saved position
 */
Datum
hashrestrpos(PG_FUNCTION_ARGS)
{
	IndexScanDesc scan = (IndexScanDesc) PG_GETARG_POINTER(0);
	ItemPointer iptr;
	HashScanOpaque so;

	so = (HashScanOpaque) scan->opaque;

	/* release lock on current data, if any */
	if (ItemPointerIsValid(iptr = &(scan->currentItemData)))
	{
		_hash_relbuf(scan->relation, so->hashso_curbuf, HASH_READ);
		so->hashso_curbuf = InvalidBuffer;
		ItemPointerSetInvalid(iptr);
	}

	/* bump lock on currentMarkData and copy to currentItemData */
	if (ItemPointerIsValid(&(scan->currentMarkData)))
	{
		so->hashso_curbuf = _hash_getbuf(scan->relation,
								 BufferGetBlockNumber(so->hashso_mrkbuf),
										 HASH_READ);

		scan->currentItemData = scan->currentMarkData;
	}

	PG_RETURN_VOID();
}

/*
 * Bulk deletion of all index entries pointing to a set of heap tuples.
 * The set of target tuples is specified via a callback routine that tells
 * whether any given heap tuple (identified by ItemPointer) is being deleted.
 *
 * Result: a palloc'd struct containing statistical info for VACUUM displays.
 */
Datum
hashbulkdelete(PG_FUNCTION_ARGS)
{
	Relation	rel = (Relation) PG_GETARG_POINTER(0);
	IndexBulkDeleteCallback callback = (IndexBulkDeleteCallback) PG_GETARG_POINTER(1);
	void	   *callback_state = (void *) PG_GETARG_POINTER(2);
	IndexBulkDeleteResult *result;
	BlockNumber num_pages;
	double		tuples_removed;
	double		num_index_tuples;
	RetrieveIndexResult res;
	IndexScanDesc iscan;

	tuples_removed = 0;
	num_index_tuples = 0;

	/*
	 * XXX generic implementation --- should be improved!
	 */

	/* walk through the entire index */
	iscan = index_beginscan(rel, false, 0, (ScanKey) NULL);

	while ((res = index_getnext(iscan, ForwardScanDirection))
		   != (RetrieveIndexResult) NULL)
	{
		ItemPointer heapptr = &res->heap_iptr;

		if (callback(heapptr, callback_state))
		{
			ItemPointer indexptr = &res->index_iptr;

			/* adjust any active scans that will be affected by deletion */
			/* (namely, my own scan) */
			_hash_adjscans(rel, indexptr);

			/* delete the data from the page */
			_hash_pagedel(rel, indexptr);

			tuples_removed += 1;
		}
		else
			num_index_tuples += 1;

		pfree(res);
	}

	index_endscan(iscan);

	/* return statistics */
	num_pages = RelationGetNumberOfBlocks(rel);

	result = (IndexBulkDeleteResult *) palloc(sizeof(IndexBulkDeleteResult));
	result->num_pages = num_pages;
	result->tuples_removed = tuples_removed;
	result->num_index_tuples = num_index_tuples;

	PG_RETURN_POINTER(result);
}


void
hash_redo(XLogRecPtr lsn, XLogRecord *record)
{
	elog(STOP, "hash_redo: unimplemented");
}

void
hash_undo(XLogRecPtr lsn, XLogRecord *record)
{
	elog(STOP, "hash_undo: unimplemented");
}

void
hash_desc(char *buf, uint8 xl_info, char *rec)
{
}
