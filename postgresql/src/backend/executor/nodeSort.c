/*-------------------------------------------------------------------------
 *
 * nodeSort.c
 *	  Routines to handle sorting of relations.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/executor/nodeSort.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "executor/execdebug.h"
#include "executor/nodeSort.h"
#include "utils/tuplesort.h"

/* ----------------------------------------------------------------
 *		ExtractSortKeys
 *
 *		Extract the sorting key information from the plan node.
 *
 *		Returns two palloc'd arrays, one of sort operator OIDs and
 *		one of attribute numbers.
 * ----------------------------------------------------------------
 */
static void
ExtractSortKeys(Sort *sortnode,
				Oid **sortOperators,
				AttrNumber **attNums)
{
	List	   *targetList;
	int			keycount;
	Oid		   *sortOps;
	AttrNumber *attNos;
	List	   *tl;

	/*
	 * get information from the node
	 */
	targetList = sortnode->plan.targetlist;
	keycount = sortnode->keycount;

	/*
	 * first allocate space for results
	 */
	if (keycount <= 0)
		elog(ERROR, "ExtractSortKeys: keycount <= 0");
	sortOps = (Oid *) palloc(keycount * sizeof(Oid));
	MemSet(sortOps, 0, keycount * sizeof(Oid));
	*sortOperators = sortOps;
	attNos = (AttrNumber *) palloc(keycount * sizeof(AttrNumber));
	MemSet(attNos, 0, keycount * sizeof(AttrNumber));
	*attNums = attNos;

	/*
	 * extract info from the resdom nodes in the target list
	 */
	foreach(tl, targetList)
	{
		TargetEntry *target = (TargetEntry *) lfirst(tl);
		Resdom	   *resdom = target->resdom;
		Index		reskey = resdom->reskey;

		if (reskey > 0)			/* ignore TLEs that are not sort keys */
		{
			Assert(reskey <= keycount);
			sortOps[reskey - 1] = resdom->reskeyop;
			attNos[reskey - 1] = resdom->resno;
		}
	}
}

/* ----------------------------------------------------------------
 *		ExecSort
 *
 *		Sorts tuples from the outer subtree of the node using tuplesort,
 *		which saves the results in a temporary file or memory. After the
 *		initial call, returns a tuple from the file with each call.
 *
 *		Conditions:
 *		  -- none.
 *
 *		Initial States:
 *		  -- the outer child is prepared to return the first tuple.
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecSort(Sort *node)
{
	EState	   *estate;
	SortState  *sortstate;
	ScanDirection dir;
	Tuplesortstate *tuplesortstate;
	HeapTuple	heapTuple;
	TupleTableSlot *slot;
	bool		should_free;

	/*
	 * get state info from node
	 */
	SO1_printf("ExecSort: %s\n",
			   "entering routine");

	sortstate = node->sortstate;
	estate = node->plan.state;
	dir = estate->es_direction;
	tuplesortstate = (Tuplesortstate *) sortstate->tuplesortstate;

	/*
	 * If first time through, read all tuples from outer plan and pass
	 * them to tuplesort.c. Subsequent calls just fetch tuples from
	 * tuplesort.
	 */

	if (!sortstate->sort_Done)
	{
		Plan	   *outerNode;
		TupleDesc	tupDesc;
		Oid		   *sortOperators;
		AttrNumber *attNums;

		SO1_printf("ExecSort: %s\n",
				   "sorting subplan");

		/*
		 * Want to scan subplan in the forward direction while creating
		 * the sorted data.  (Does setting my direction actually affect
		 * the subplan?  I bet this is useless code...)
		 */
		estate->es_direction = ForwardScanDirection;

		/*
		 * Initialize tuplesort module.
		 */
		SO1_printf("ExecSort: %s\n",
				   "calling tuplesort_begin");

		outerNode = outerPlan((Plan *) node);
		tupDesc = ExecGetTupType(outerNode);

		ExtractSortKeys(node, &sortOperators, &attNums);

		tuplesortstate = tuplesort_begin_heap(tupDesc, node->keycount,
											  sortOperators, attNums,
											  true /* randomAccess */ );
		sortstate->tuplesortstate = (void *) tuplesortstate;

		pfree(sortOperators);
		pfree(attNums);

		/*
		 * Scan the subplan and feed all the tuples to tuplesort.
		 */

		for (;;)
		{
			slot = ExecProcNode(outerNode, (Plan *) node);

			if (TupIsNull(slot))
				break;

			tuplesort_puttuple(tuplesortstate, (void *) slot->val);
		}

		/*
		 * Complete the sort.
		 */
		tuplesort_performsort(tuplesortstate);

		/*
		 * restore to user specified direction
		 */
		estate->es_direction = dir;

		/*
		 * make sure the tuple descriptor is up to date (is this needed?)
		 */
		ExecAssignResultType(&sortstate->csstate.cstate, tupDesc, false);

		/*
		 * finally set the sorted flag to true
		 */
		sortstate->sort_Done = true;
		SO1_printf(stderr, "ExecSort: sorting done.\n");
	}

	SO1_printf("ExecSort: %s\n",
			   "retrieving tuple from tuplesort");

	/*
	 * Get the first or next tuple from tuplesort. Returns NULL if no more
	 * tuples.
	 */
	heapTuple = tuplesort_getheaptuple(tuplesortstate,
									   ScanDirectionIsForward(dir),
									   &should_free);

	slot = sortstate->csstate.cstate.cs_ResultTupleSlot;
	return ExecStoreTuple(heapTuple, slot, InvalidBuffer, should_free);
}

/* ----------------------------------------------------------------
 *		ExecInitSort
 *
 *		Creates the run-time state information for the sort node
 *		produced by the planner and initailizes its outer subtree.
 * ----------------------------------------------------------------
 */
bool
ExecInitSort(Sort *node, EState *estate, Plan *parent)
{
	SortState  *sortstate;
	Plan	   *outerPlan;

	SO1_printf("ExecInitSort: %s\n",
			   "initializing sort node");

	/*
	 * assign the node's execution state
	 */
	node->plan.state = estate;

	/*
	 * create state structure
	 */
	sortstate = makeNode(SortState);
	sortstate->sort_Done = false;
	sortstate->tuplesortstate = NULL;

	node->sortstate = sortstate;

	/*
	 * Miscellaneous initialization
	 *
	 * Sort nodes don't initialize their ExprContexts because they never call
	 * ExecQual or ExecProject.
	 */

#define SORT_NSLOTS 2

	/*
	 * tuple table initialization
	 *
	 * sort nodes only return scan tuples from their sorted relation.
	 */
	ExecInitResultTupleSlot(estate, &sortstate->csstate.cstate);
	ExecInitScanTupleSlot(estate, &sortstate->csstate);

	/*
	 * initializes child nodes
	 */
	outerPlan = outerPlan((Plan *) node);
	ExecInitNode(outerPlan, estate, (Plan *) node);

	/*
	 * initialize tuple type.  no need to initialize projection info
	 * because this node doesn't do projections.
	 */
	ExecAssignResultTypeFromOuterPlan((Plan *) node, &sortstate->csstate.cstate);
	ExecAssignScanTypeFromOuterPlan((Plan *) node, &sortstate->csstate);
	sortstate->csstate.cstate.cs_ProjInfo = NULL;

	SO1_printf("ExecInitSort: %s\n",
			   "sort node initialized");

	return TRUE;
}

int
ExecCountSlotsSort(Sort *node)
{
	return ExecCountSlotsNode(outerPlan((Plan *) node)) +
		ExecCountSlotsNode(innerPlan((Plan *) node)) +
		SORT_NSLOTS;
}

/* ----------------------------------------------------------------
 *		ExecEndSort(node)
 * ----------------------------------------------------------------
 */
void
ExecEndSort(Sort *node)
{
	SortState  *sortstate;
	Plan	   *outerPlan;

	/*
	 * get info from the sort state
	 */
	SO1_printf("ExecEndSort: %s\n",
			   "shutting down sort node");

	sortstate = node->sortstate;

	/*
	 * shut down the subplan
	 */
	outerPlan = outerPlan((Plan *) node);
	ExecEndNode(outerPlan, (Plan *) node);

	/*
	 * clean out the tuple table
	 */
	ExecClearTuple(sortstate->csstate.css_ScanTupleSlot);

	/*
	 * Release tuplesort resources
	 */
	if (sortstate->tuplesortstate != NULL)
		tuplesort_end((Tuplesortstate *) sortstate->tuplesortstate);
	sortstate->tuplesortstate = NULL;

	pfree(sortstate);
	node->sortstate = NULL;

	SO1_printf("ExecEndSort: %s\n",
			   "sort node shutdown");
}

/* ----------------------------------------------------------------
 *		ExecSortMarkPos
 *
 *		Calls tuplesort to save the current position in the sorted file.
 * ----------------------------------------------------------------
 */
void
ExecSortMarkPos(Sort *node)
{
	SortState  *sortstate = node->sortstate;

	/*
	 * if we haven't sorted yet, just return
	 */
	if (!sortstate->sort_Done)
		return;

	tuplesort_markpos((Tuplesortstate *) sortstate->tuplesortstate);
}

/* ----------------------------------------------------------------
 *		ExecSortRestrPos
 *
 *		Calls tuplesort to restore the last saved sort file position.
 * ----------------------------------------------------------------
 */
void
ExecSortRestrPos(Sort *node)
{
	SortState  *sortstate = node->sortstate;

	/*
	 * if we haven't sorted yet, just return.
	 */
	if (!sortstate->sort_Done)
		return;

	/*
	 * restore the scan to the previously marked position
	 */
	tuplesort_restorepos((Tuplesortstate *) sortstate->tuplesortstate);
}

void
ExecReScanSort(Sort *node, ExprContext *exprCtxt, Plan *parent)
{
	SortState  *sortstate = node->sortstate;

	/*
	 * If we haven't sorted yet, just return. If outerplan' chgParam is
	 * not NULL then it will be re-scanned by ExecProcNode, else - no
	 * reason to re-scan it at all.
	 */
	if (!sortstate->sort_Done)
		return;

	ExecClearTuple(sortstate->csstate.cstate.cs_ResultTupleSlot);

	/*
	 * If subnode is to be rescanned then we forget previous sort results;
	 * we have to re-read the subplan and re-sort.
	 *
	 * Otherwise we can just rewind and rescan the sorted output.
	 */
	if (((Plan *) node)->lefttree->chgParam != NULL)
	{
		sortstate->sort_Done = false;
		tuplesort_end((Tuplesortstate *) sortstate->tuplesortstate);
		sortstate->tuplesortstate = NULL;
	}
	else
		tuplesort_rescan((Tuplesortstate *) sortstate->tuplesortstate);
}
