/*-------------------------------------------------------------------------
 *
 * nodeSubqueryscan.c
 *	  Support routines for scanning subqueries (subselects in rangetable).
 *
 * This is just enough different from sublinks (nodeSubplan.c) to mean that
 * we need two sets of code.  Ought to look at trying to unify the cases.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/executor/nodeSubqueryscan.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * INTERFACE ROUTINES
 *		ExecSubqueryScan			scans a subquery.
 *		ExecSubqueryNext			retrieve next tuple in sequential order.
 *		ExecInitSubqueryScan		creates and initializes a subqueryscan node.
 *		ExecEndSubqueryScan			releases any storage allocated.
 *		ExecSubqueryReScan			rescans the relation
 *
 */
#include "postgres.h"

#include "catalog/pg_type.h"
#include "executor/execdebug.h"
#include "executor/execdefs.h"
#include "executor/execdesc.h"
#include "executor/nodeSubqueryscan.h"
#include "parser/parsetree.h"
#include "tcop/pquery.h"

static TupleTableSlot *SubqueryNext(SubqueryScan *node);

/* ----------------------------------------------------------------
 *						Scan Support
 * ----------------------------------------------------------------
 */
/* ----------------------------------------------------------------
 *		SubqueryNext
 *
 *		This is a workhorse for ExecSubqueryScan
 * ----------------------------------------------------------------
 */
static TupleTableSlot *
SubqueryNext(SubqueryScan *node)
{
	SubqueryScanState *subquerystate;
	EState	   *estate;
	ScanDirection direction;
	TupleTableSlot *slot;

	/*
	 * get information from the estate and scan state
	 */
	estate = node->scan.plan.state;
	subquerystate = (SubqueryScanState *) node->scan.scanstate;
	direction = estate->es_direction;

	/*
	 * We need not support EvalPlanQual here, since we are not scanning a
	 * real relation.
	 */

	/*
	 * get the next tuple from the sub-query
	 */
	subquerystate->sss_SubEState->es_direction = direction;

	slot = ExecProcNode(node->subplan, (Plan *) node);

	subquerystate->csstate.css_ScanTupleSlot = slot;

	return slot;
}

/* ----------------------------------------------------------------
 *		ExecSubqueryScan(node)
 *
 *		Scans the subquery sequentially and returns the next qualifying
 *		tuple.
 *		It calls the ExecScan() routine and passes it the access method
 *		which retrieve tuples sequentially.
 *
 */

TupleTableSlot *
ExecSubqueryScan(SubqueryScan *node)
{
	/*
	 * use SubqueryNext as access method
	 */
	return ExecScan(&node->scan, (ExecScanAccessMtd) SubqueryNext);
}

/* ----------------------------------------------------------------
 *		ExecInitSubqueryScan
 * ----------------------------------------------------------------
 */
bool
ExecInitSubqueryScan(SubqueryScan *node, EState *estate, Plan *parent)
{
	SubqueryScanState *subquerystate;
	RangeTblEntry *rte;
	EState	   *sp_estate;

	/*
	 * SubqueryScan should not have any "normal" children.
	 */
	Assert(outerPlan((Plan *) node) == NULL);
	Assert(innerPlan((Plan *) node) == NULL);

	/*
	 * assign the node's execution state
	 */
	node->scan.plan.state = estate;

	/*
	 * create new SubqueryScanState for node
	 */
	subquerystate = makeNode(SubqueryScanState);
	node->scan.scanstate = (CommonScanState *) subquerystate;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &subquerystate->csstate.cstate);

#define SUBQUERYSCAN_NSLOTS 1

	/*
	 * tuple table initialization
	 */
	ExecInitResultTupleSlot(estate, &subquerystate->csstate.cstate);

	/*
	 * initialize subquery
	 *
	 * This should agree with ExecInitSubPlan
	 */
	rte = rt_fetch(node->scan.scanrelid, estate->es_range_table);
	Assert(rte->subquery != NULL);

	sp_estate = CreateExecutorState();
	subquerystate->sss_SubEState = sp_estate;

	sp_estate->es_range_table = rte->subquery->rtable;
	sp_estate->es_param_list_info = estate->es_param_list_info;
	sp_estate->es_param_exec_vals = estate->es_param_exec_vals;
	sp_estate->es_tupleTable =
		ExecCreateTupleTable(ExecCountSlotsNode(node->subplan) + 10);
	sp_estate->es_snapshot = estate->es_snapshot;

	if (!ExecInitNode(node->subplan, sp_estate, (Plan *) node))
		return false;

	subquerystate->csstate.css_ScanTupleSlot = NULL;
	subquerystate->csstate.cstate.cs_TupFromTlist = false;

	/*
	 * initialize tuple type
	 */
	ExecAssignResultTypeFromTL((Plan *) node, &subquerystate->csstate.cstate);
	ExecAssignProjectionInfo((Plan *) node, &subquerystate->csstate.cstate);

	return TRUE;
}

int
ExecCountSlotsSubqueryScan(SubqueryScan *node)
{
	/*
	 * The subplan has its own tuple table and must not be counted here!
	 */
	return ExecCountSlotsNode(outerPlan(node)) +
		ExecCountSlotsNode(innerPlan(node)) +
		SUBQUERYSCAN_NSLOTS;
}

/* ----------------------------------------------------------------
 *		ExecEndSubqueryScan
 *
 *		frees any storage allocated through C routines.
 * ----------------------------------------------------------------
 */
void
ExecEndSubqueryScan(SubqueryScan *node)
{
	SubqueryScanState *subquerystate;

	/*
	 * get information from node
	 */
	subquerystate = (SubqueryScanState *) node->scan.scanstate;

	/*
	 * Free the projection info and the scan attribute info
	 *
	 * Note: we don't ExecFreeResultType(subquerystate) because the rule
	 * manager depends on the tupType returned by ExecMain().  So for now,
	 * this is freed at end-transaction time.  -cim 6/2/91
	 */
	ExecFreeProjectionInfo(&subquerystate->csstate.cstate);
	ExecFreeExprContext(&subquerystate->csstate.cstate);

	/*
	 * close down subquery
	 */
	ExecEndNode(node->subplan, (Plan *) node);

	/*
	 * clean up subquery's tuple table
	 */
	subquerystate->csstate.css_ScanTupleSlot = NULL;
	ExecDropTupleTable(subquerystate->sss_SubEState->es_tupleTable, true);

	/* XXX we seem to be leaking the sub-EState... */

	/*
	 * clean out the upper tuple table
	 */
	ExecClearTuple(subquerystate->csstate.cstate.cs_ResultTupleSlot);
}

/* ----------------------------------------------------------------
 *		ExecSubqueryReScan
 *
 *		Rescans the relation.
 * ----------------------------------------------------------------
 */
void
ExecSubqueryReScan(SubqueryScan *node, ExprContext *exprCtxt, Plan *parent)
{
	SubqueryScanState *subquerystate;
	EState	   *estate;

	subquerystate = (SubqueryScanState *) node->scan.scanstate;
	estate = node->scan.plan.state;

	/*
	 * ExecReScan doesn't know about my subplan, so I have to do
	 * changed-parameter signaling myself.
	 */
	if (node->scan.plan.chgParam != NULL)
		SetChangedParamList(node->subplan, node->scan.plan.chgParam);

	/*
	 * if chgParam of subnode is not null then plan will be re-scanned by
	 * first ExecProcNode.
	 */
	if (node->subplan->chgParam == NULL)
		ExecReScan(node->subplan, NULL, (Plan *) node);

	subquerystate->csstate.css_ScanTupleSlot = NULL;
}
