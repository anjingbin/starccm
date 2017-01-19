/*-------------------------------------------------------------------------
 *
 * nodeTidscan.c
 *	  Routines to support direct tid scans of relations
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/executor/nodeTidscan.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * INTERFACE ROUTINES
 *
 *		ExecTidScan		scans a relation using tids
 *		ExecInitTidScan		creates and initializes state info.
 *		ExecTidReScan		rescans the tid relation.
 *		ExecEndTidScan		releases all storage.
 *		ExecTidMarkPos		marks scan position.
 *
 */
#include "postgres.h"

#include "executor/execdebug.h"
#include "executor/nodeTidscan.h"
#include "access/heapam.h"
#include "parser/parsetree.h"

static int	TidListCreate(List *, ExprContext *, ItemPointerData[]);
static TupleTableSlot *TidNext(TidScan *node);

static int
TidListCreate(List *evalList, ExprContext *econtext, ItemPointerData tidList[])
{
	List	   *lst;
	ItemPointer itemptr;
	bool		isNull;
	int			numTids = 0;

	foreach(lst, evalList)
	{
		itemptr = (ItemPointer)
			DatumGetPointer(ExecEvalExprSwitchContext(lfirst(lst),
													  econtext,
													  &isNull,
													  NULL));
		if (!isNull && itemptr && ItemPointerIsValid(itemptr))
		{
			tidList[numTids] = *itemptr;
			numTids++;
		}
	}
	return numTids;
}

/* ----------------------------------------------------------------
 *		TidNext
 *
 *		Retrieve a tuple from the TidScan node's currentRelation
 *		using the tids in the TidScanState information.
 *
 * ----------------------------------------------------------------
 */
static TupleTableSlot *
TidNext(TidScan *node)
{
	EState	   *estate;
	CommonScanState *scanstate;
	TidScanState *tidstate;
	ScanDirection direction;
	Snapshot	snapshot;
	Relation	heapRelation;
	HeapTuple	tuple;
	TupleTableSlot *slot;
	Buffer		buffer = InvalidBuffer;
	int			numTids;

	bool		bBackward;
	int			tidNumber;
	ItemPointerData *tidList;

	/*
	 * extract necessary information from tid scan node
	 */
	estate = node->scan.plan.state;
	direction = estate->es_direction;
	snapshot = estate->es_snapshot;
	scanstate = node->scan.scanstate;
	tidstate = node->tidstate;
	heapRelation = scanstate->css_currentRelation;
	numTids = tidstate->tss_NumTids;
	tidList = tidstate->tss_TidList;
	slot = scanstate->css_ScanTupleSlot;

	/*
	 * Check if we are evaluating PlanQual for tuple of this relation.
	 * Additional checking is not good, but no other way for now. We could
	 * introduce new nodes for this case and handle TidScan --> NewNode
	 * switching in Init/ReScan plan...
	 */
	if (estate->es_evTuple != NULL &&
		estate->es_evTuple[node->scan.scanrelid - 1] != NULL)
	{
		ExecClearTuple(slot);
		if (estate->es_evTupleNull[node->scan.scanrelid - 1])
			return slot;		/* return empty slot */

		/*
		 * XXX shouldn't we check here to make sure tuple matches TID list?
		 * In runtime-key case this is not certain, is it?
		 */

		ExecStoreTuple(estate->es_evTuple[node->scan.scanrelid - 1],
					   slot, InvalidBuffer, false);

		/* Flag for the next call that no more tuples */
		estate->es_evTupleNull[node->scan.scanrelid - 1] = true;
		return (slot);
	}

	tuple = &(tidstate->tss_htup);

	/*
	 * ok, now that we have what we need, fetch an tid tuple. if scanning
	 * this tid succeeded then return the appropriate heap tuple.. else
	 * return NULL.
	 */
	bBackward = ScanDirectionIsBackward(direction);
	if (bBackward)
	{
		tidNumber = numTids - tidstate->tss_TidPtr - 1;
		if (tidNumber < 0)
		{
			tidNumber = 0;
			tidstate->tss_TidPtr = numTids - 1;
		}
	}
	else
	{
		if ((tidNumber = tidstate->tss_TidPtr) < 0)
		{
			tidNumber = 0;
			tidstate->tss_TidPtr = 0;
		}
	}
	while (tidNumber < numTids)
	{
		bool		slot_is_valid = false;

		tuple->t_datamcxt = NULL;
		tuple->t_data = NULL;
		tuple->t_self = tidList[tidstate->tss_TidPtr];
		heap_fetch(heapRelation, snapshot, tuple, &buffer, NULL);
		if (tuple->t_data != NULL)
		{
			bool		prev_matches = false;
			int			prev_tid;

			/*
			 * store the scanned tuple in the scan tuple slot of the scan
			 * state.  Eventually we will only do this and not return a
			 * tuple.  Note: we pass 'false' because tuples returned by
			 * amgetnext are pointers onto disk pages and were not created
			 * with palloc() and so should not be pfree()'d.
			 */
			ExecStoreTuple(tuple,		/* tuple to store */
						   slot,	/* slot to store in */
						   buffer,		/* buffer associated with tuple  */
						   false);		/* don't pfree */

			/*
			 * At this point we have an extra pin on the buffer, because
			 * ExecStoreTuple incremented the pin count. Drop our local
			 * pin.
			 */
			ReleaseBuffer(buffer);

			/*
			 * We must check to see if the current tuple would have been
			 * matched by an earlier tid, so we don't double report it. We
			 * do this by passing the tuple through ExecQual and look for
			 * failure with all previous qualifications.
			 */
			for (prev_tid = 0; prev_tid < tidstate->tss_TidPtr;
				 prev_tid++)
			{
				if (ItemPointerEquals(&tidList[prev_tid], &tuple->t_self))
				{
					prev_matches = true;
					break;
				}
			}
			if (!prev_matches)
				slot_is_valid = true;
			else
				ExecClearTuple(slot);
		}
		else if (BufferIsValid(buffer))
			ReleaseBuffer(buffer);
		tidNumber++;
		if (bBackward)
			tidstate->tss_TidPtr--;
		else
			tidstate->tss_TidPtr++;
		if (slot_is_valid)
			return slot;
	}

	/*
	 * if we get here it means the tid scan failed so we are at the end of
	 * the scan..
	 */
	return ExecClearTuple(slot);
}

/* ----------------------------------------------------------------
 *		ExecTidScan(node)
 *
 *		Scans the relation using tids and returns
 *		   the next qualifying tuple in the direction specified.
 *		It calls ExecScan() and passes it the access methods which returns
 *		the next tuple using the tids.
 *
 *		Conditions:
 *		  -- the "cursor" maintained by the AMI is positioned at the tuple
 *			 returned previously.
 *
 *		Initial States:
 *		  -- the relation indicated is opened for scanning so that the
 *			 "cursor" is positioned before the first qualifying tuple.
 *		  -- tidPtr points to the first tid.
 *		  -- state variable ruleFlag = nil.
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecTidScan(TidScan *node)
{
	/*
	 * use TidNext as access method
	 */
	return ExecScan(&node->scan, (ExecScanAccessMtd) TidNext);
}

/* ----------------------------------------------------------------
 *		ExecTidReScan(node)
 * ----------------------------------------------------------------
 */
void
ExecTidReScan(TidScan *node, ExprContext *exprCtxt, Plan *parent)
{
	EState	   *estate;
	TidScanState *tidstate;
	ItemPointerData *tidList;

	estate = node->scan.plan.state;
	tidstate = node->tidstate;
	tidList = tidstate->tss_TidList;

	/* If we are being passed an outer tuple, save it for runtime key calc */
	if (exprCtxt != NULL)
		node->scan.scanstate->cstate.cs_ExprContext->ecxt_outertuple =
			exprCtxt->ecxt_outertuple;

	/* do runtime calc of target TIDs, if needed */
	if (node->needRescan)
		tidstate->tss_NumTids =
			TidListCreate(node->tideval,
						  node->scan.scanstate->cstate.cs_ExprContext,
						  tidList);

	/* If this is re-scanning of PlanQual ... */
	if (estate->es_evTuple != NULL &&
		estate->es_evTuple[node->scan.scanrelid - 1] != NULL)
	{
		estate->es_evTupleNull[node->scan.scanrelid - 1] = false;
		return;
	}

	tidstate->tss_TidPtr = -1;

	/*
	 * perhaps return something meaningful
	 */
	return;
}

/* ----------------------------------------------------------------
 *		ExecEndTidScan
 *
 *		Releases any storage allocated through C routines.
 *		Returns nothing.
 * ----------------------------------------------------------------
 */
void
ExecEndTidScan(TidScan *node)
{
	CommonScanState *scanstate;
	TidScanState *tidstate;

	scanstate = node->scan.scanstate;
	tidstate = node->tidstate;
	if (tidstate && tidstate->tss_TidList)
		pfree(tidstate->tss_TidList);

	/*
	 * extract information from the node
	 */

	/*
	 * Free the projection info and the scan attribute info
	 *
	 * Note: we don't ExecFreeResultType(scanstate) because the rule manager
	 * depends on the tupType returned by ExecMain().  So for now, this is
	 * freed at end-transaction time.  -cim 6/2/91
	 */
	ExecFreeProjectionInfo(&scanstate->cstate);
	ExecFreeExprContext(&scanstate->cstate);

	/*
	 * close the heap and tid relations
	 */
	ExecCloseR((Plan *) node);

	/*
	 * clear out tuple table slots
	 */
	ExecClearTuple(scanstate->cstate.cs_ResultTupleSlot);
	ExecClearTuple(scanstate->css_ScanTupleSlot);
}

/* ----------------------------------------------------------------
 *		ExecTidMarkPos
 *
 *		Marks scan position by marking the current tid.
 *		Returns nothing.
 * ----------------------------------------------------------------
 */
void
ExecTidMarkPos(TidScan *node)
{
	TidScanState *tidstate;

	tidstate = node->tidstate;
	tidstate->tss_MarkTidPtr = tidstate->tss_TidPtr;
}

#ifdef NOT_USED
/* ----------------------------------------------------------------
 *		ExecTidRestrPos
 *
 *		Restores scan position by restoring the current tid.
 *		Returns nothing.
 *
 *		XXX Assumes previously marked scan position belongs to current tid
 * ----------------------------------------------------------------
 */
void
ExecTidRestrPos(TidScan *node)
{
	TidScanState *tidstate;

	tidstate = node->tidstate;
	tidstate->tss_TidPtr = tidstate->tss_MarkTidPtr;
}
#endif

/* ----------------------------------------------------------------
 *		ExecInitTidScan
 *
 *		Initializes the tid scan's state information, creates
 *		scan keys, and opens the base and tid relations.
 *
 *		Parameters:
 *		  node: TidNode node produced by the planner.
 *		  estate: the execution state initialized in InitPlan.
 * ----------------------------------------------------------------
 */
bool
ExecInitTidScan(TidScan *node, EState *estate, Plan *parent)
{
	TidScanState *tidstate;
	CommonScanState *scanstate;
	ItemPointerData *tidList;
	int			numTids;
	int			tidPtr;
	List	   *rangeTable;
	RangeTblEntry *rtentry;
	Oid			relid;
	Oid			reloid;
	Relation	currentRelation;
	List	   *execParam = NIL;

	/*
	 * assign execution state to node
	 */
	node->scan.plan.state = estate;

	/*
	 * Part 1)	initialize scan state
	 *
	 * create new CommonScanState for node
	 */
	scanstate = makeNode(CommonScanState);
	node->scan.scanstate = scanstate;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &scanstate->cstate);

#define TIDSCAN_NSLOTS 2

	/*
	 * tuple table initialization
	 */
	ExecInitResultTupleSlot(estate, &scanstate->cstate);
	ExecInitScanTupleSlot(estate, scanstate);

	/*
	 * initialize projection info.	result type comes from scan desc
	 * below..
	 */
	ExecAssignProjectionInfo((Plan *) node, &scanstate->cstate);

	/*
	 * Part 2)	initialize tid scan state
	 *
	 * create new TidScanState for node
	 */
	tidstate = makeNode(TidScanState);
	node->tidstate = tidstate;

	/*
	 * get the tid node information
	 */
	tidList = (ItemPointerData *) palloc(length(node->tideval) * sizeof(ItemPointerData));
	numTids = 0;
	if (!node->needRescan)
		numTids = TidListCreate(node->tideval,
								scanstate->cstate.cs_ExprContext,
								tidList);
	tidPtr = -1;

	CXT1_printf("ExecInitTidScan: context is %d\n", CurrentMemoryContext);

	tidstate->tss_NumTids = numTids;
	tidstate->tss_TidPtr = tidPtr;
	tidstate->tss_TidList = tidList;

	/*
	 * get the range table and direction information from the execution
	 * state (these are needed to open the relations).
	 */
	rangeTable = estate->es_range_table;

	/*
	 * open the base relation
	 */
	relid = node->scan.scanrelid;
	rtentry = rt_fetch(relid, rangeTable);
	reloid = rtentry->relid;

	currentRelation = heap_open(reloid, AccessShareLock);
	scanstate->css_currentRelation = currentRelation;
	scanstate->css_currentScanDesc = 0;

	/*
	 * get the scan type from the relation descriptor.
	 */
	ExecAssignScanType(scanstate, RelationGetDescr(currentRelation), false);
	ExecAssignResultTypeFromTL((Plan *) node, &scanstate->cstate);

	/*
	 * if there are some PARAM_EXEC in skankeys then force tid rescan on
	 * first scan.
	 */
	((Plan *) node)->chgParam = execParam;

	/*
	 * all done.
	 */
	return TRUE;
}

int
ExecCountSlotsTidScan(TidScan *node)
{
	return ExecCountSlotsNode(outerPlan((Plan *) node)) +
		ExecCountSlotsNode(innerPlan((Plan *) node)) + TIDSCAN_NSLOTS;
}
