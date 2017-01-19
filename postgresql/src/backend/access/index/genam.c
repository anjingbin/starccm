/*-------------------------------------------------------------------------
 *
 * genam.c
 *	  general index access method routines
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/access/index/genam.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 * NOTES
 *	  many of the old access method routines have been turned into
 *	  macros and moved to genam.h -cim 4/30/91
 *
 *-------------------------------------------------------------------------
 */
/*
 * OLD COMMENTS
 * Scans are implemented as follows:
 *
 * `0' represents an invalid item pointer.
 * `-' represents an unknown item pointer.
 * `X' represents a known item pointers.
 * `+' represents known or invalid item pointers.
 * `*' represents any item pointers.
 *
 * State is represented by a triple of these symbols in the order of
 * previous, current, next.  Note that the case of reverse scans works
 * identically.
 *
 *		State	Result
 * (1)	+ + -	+ 0 0			(if the next item pointer is invalid)
 * (2)			+ X -			(otherwise)
 * (3)	* 0 0	* 0 0			(no change)
 * (4)	+ X 0	X 0 0			(shift)
 * (5)	* + X	+ X -			(shift, add unknown)
 *
 * All other states cannot occur.
 *
 * Note:
 *		It would be possible to cache the status of the previous and
 *		next item pointer using the flags.
 * ----------------------------------------------------------------
 */

#include "postgres.h"
#include "access/genam.h"

#include "pgstat.h"

/* ----------------------------------------------------------------
 *		general access method routines
 *
 *		All indexed access methods use an identical scan structure.
 *		We don't know how the various AMs do locking, however, so we don't
 *		do anything about that here.
 *
 *		The intent is that an AM implementor will define a beginscan routine
 *		that calls RelationGetIndexScan, to fill in the scan, and then does
 *		whatever kind of locking he wants.
 *
 *		At the end of a scan, the AM's endscan routine undoes the locking,
 *		but does *not* call IndexScanEnd --- the higher-level index_endscan
 *		routine does that.	(We can't do it in the AM because index_endscan
 *		still needs to touch the IndexScanDesc after calling the AM.)
 *
 *		Because of this, the AM does not have a choice whether to call
 *		RelationGetIndexScan or not; its beginscan routine must return an
 *		object made by RelationGetIndexScan.  This is kinda ugly but not
 *		worth cleaning up now.
 * ----------------------------------------------------------------
 */

/* ----------------
 *	RelationGetIndexScan -- Create and fill an IndexScanDesc.
 *
 *		This routine creates an index scan structure and sets its contents
 *		up correctly. This routine calls AMrescan to set up the scan with
 *		the passed key.
 *
 *		Parameters:
 *				relation -- index relation for scan.
 *				scanFromEnd -- if true, begin scan at one of the index's
 *							   endpoints.
 *				numberOfKeys -- count of scan keys.
 *				key -- the ScanKey for the starting position of the scan.
 *
 *		Returns:
 *				An initialized IndexScanDesc.
 * ----------------
 */
IndexScanDesc
RelationGetIndexScan(Relation relation,
					 bool scanFromEnd,
					 uint16 numberOfKeys,
					 ScanKey key)
{
	IndexScanDesc scan;

	if (!RelationIsValid(relation))
		elog(ERROR, "RelationGetIndexScan: relation invalid");

	scan = (IndexScanDesc) palloc(sizeof(IndexScanDescData));

	scan->relation = relation;
	scan->opaque = NULL;
	scan->numberOfKeys = numberOfKeys;

	ItemPointerSetInvalid(&scan->currentItemData);
	ItemPointerSetInvalid(&scan->currentMarkData);

	pgstat_initstats(&scan->xs_pgstat_info, relation);

	/*
	 * mark cached function lookup data invalid; it will be set on first
	 * use
	 */
	scan->fn_getnext.fn_oid = InvalidOid;

	if (numberOfKeys > 0)
		scan->keyData = (ScanKey) palloc(sizeof(ScanKeyData) * numberOfKeys);
	else
		scan->keyData = NULL;

	index_rescan(scan, scanFromEnd, key);

	return scan;
}

/* ----------------
 *	IndexScanEnd -- End an index scan.
 *
 *		This routine just releases the storage acquired by
 *		RelationGetIndexScan().  Any AM-level resources are
 *		assumed to already have been released by the AM's
 *		endscan routine.
 *
 *	Returns:
 *		None.
 * ----------------
 */
void
IndexScanEnd(IndexScanDesc scan)
{
	if (!IndexScanIsValid(scan))
		elog(ERROR, "IndexScanEnd: invalid scan");

	if (scan->keyData != NULL)
		pfree(scan->keyData);

	pfree(scan);
}

#ifdef NOT_USED
/* ----------------
 *	IndexScanRestart -- Restart an index scan.
 *
 *		This routine isn't used by any existing access method.  It's
 *		appropriate if relation level locks are what you want.
 *
 *	Returns:
 *		None.
 *
 *	Side Effects:
 *		None.
 * ----------------
 */
void
IndexScanRestart(IndexScanDesc scan,
				 bool scanFromEnd,
				 ScanKey key)
{
	if (!IndexScanIsValid(scan))
		elog(ERROR, "IndexScanRestart: invalid scan");

	ItemPointerSetInvalid(&scan->currentItemData);

	if (RelationGetNumberOfBlocks(scan->relation) == 0)
		scan->flags = ScanUnmarked;
	else if (scanFromEnd)
		scan->flags = ScanUnmarked | ScanUncheckedPrevious;
	else
		scan->flags = ScanUnmarked | ScanUncheckedNext;

	scan->scanFromEnd = (bool) scanFromEnd;

	if (scan->numberOfKeys > 0)
		memmove(scan->keyData,
				key,
				scan->numberOfKeys * sizeof(ScanKeyData));
}

/* ----------------
 *	IndexScanMarkPosition -- Mark current position in a scan.
 *
 *		This routine isn't used by any existing access method, but is the
 *		one that AM implementors should use, if they don't want to do any
 *		special locking.  If relation-level locking is sufficient, this is
 *		the routine for you.
 *
 *	Returns:
 *		None.
 *
 *	Side Effects:
 *		None.
 * ----------------
 */
void
IndexScanMarkPosition(IndexScanDesc scan)
{
	scan->currentMarkData = scan->currentItemData;

	scan->flags = 0x0;			/* XXX should have a symbolic name */
}

/* ----------------
 *	IndexScanRestorePosition -- Restore position on a marked scan.
 *
 *		This routine isn't used by any existing access method, but is the
 *		one that AM implementors should use if they don't want to do any
 *		special locking.  If relation-level locking is sufficient, then
 *		this is the one you want.
 *
 *	Returns:
 *		None.
 *
 *	Side Effects:
 *		None.
 * ----------------
 */
void
IndexScanRestorePosition(IndexScanDesc scan)
{
	if (scan->flags & ScanUnmarked)
		elog(ERROR, "IndexScanRestorePosition: no mark to restore");

	scan->currentItemData = scan->currentMarkData;

	scan->flags = 0x0;			/* XXX should have a symbolic name */
}

#endif
