/*-------------------------------------------------------------------------
 *
 * nodeMergejoin.c
 *	  routines supporting merge joins
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/executor/nodeMergejoin.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * INTERFACE ROUTINES
 *		ExecMergeJoin			mergejoin outer and inner relations.
 *		ExecInitMergeJoin		creates and initializes run time states
 *		ExecEndMergeJoin		cleans up the node.
 *
 * NOTES
 *		Essential operation of the merge join algorithm is as follows:
 *
 *		Join {												   -
 *			get initial outer and inner tuples				INITIALIZE
 *			Skip Inner										SKIPINNER
 *			mark inner position								JOINMARK
 *			do forever {									   -
 *				while (outer == inner) {					JOINTEST
 *					join tuples								JOINTUPLES
 *					advance inner position					NEXTINNER
 *				}											   -
 *				advance outer position						NEXTOUTER
 *				if (outer == mark) {						TESTOUTER
 *					restore inner position to mark			TESTOUTER
 *					continue								   -
 *				} else {									   -
 *					Skip Outer								SKIPOUTER
 *					mark inner position						JOINMARK
 *				}											   -
 *			}												   -
 *		}													   -
 *
 *		Skip Outer {										SKIPOUTER_BEGIN
 *			if (inner == outer) Join Tuples					JOINTUPLES
 *			while (outer < inner)							SKIPOUTER_TEST
 *				advance outer								SKIPOUTER_ADVANCE
 *			if (outer > inner)								SKIPOUTER_TEST
 *				Skip Inner									SKIPINNER
 *		}													   -
 *
 *		Skip Inner {										SKIPINNER_BEGIN
 *			if (inner == outer) Join Tuples					JOINTUPLES
 *			while (outer > inner)							SKIPINNER_TEST
 *				advance inner								SKIPINNER_ADVANCE
 *			if (outer < inner)								SKIPINNER_TEST
 *				Skip Outer									SKIPOUTER
 *		}													   -
 *
 *		The merge join operation is coded in the fashion
 *		of a state machine.  At each state, we do something and then
 *		proceed to another state.  This state is stored in the node's
 *		execution state information and is preserved across calls to
 *		ExecMergeJoin. -cim 10/31/89
 *
 */
#include "postgres.h"

#include "access/heapam.h"
#include "access/printtup.h"
#include "catalog/pg_operator.h"
#include "executor/execdebug.h"
#include "executor/execdefs.h"
#include "executor/nodeMergejoin.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"


static bool MergeCompare(List *eqQual, List *compareQual, ExprContext *econtext);

#define MarkInnerTuple(innerTupleSlot, mergestate) \
( \
	ExecStoreTuple(heap_copytuple((innerTupleSlot)->val), \
				   (mergestate)->mj_MarkedTupleSlot, \
				   InvalidBuffer, \
				   true) \
)


/* ----------------------------------------------------------------
 *		MJFormSkipQual
 *
 *		This takes the mergeclause which is a qualification of the
 *		form ((= expr expr) (= expr expr) ...) and forms a new
 *		qualification like ((> expr expr) (> expr expr) ...) which
 *		is used by ExecMergeJoin() in order to determine if we should
 *		skip tuples.  The replacement operators are named either ">"
 *		or "<" according to the replaceopname parameter, and have the
 *		same operand data types as the "=" operators they replace.
 *		(We expect there to be such operators because the "=" operators
 *		were marked mergejoinable; however, there might be a different
 *		one needed in each qual clause.)
 * ----------------------------------------------------------------
 */
static List *
MJFormSkipQual(List *qualList, char *replaceopname)
{
	List	   *qualCopy;
	List	   *qualcdr;
	Expr	   *qual;
	Oper	   *op;
	HeapTuple	optup;
	Form_pg_operator opform;
	Oid			oprleft,
				oprright;

	/*
	 * qualList is a list: ((op .. ..) ...)
	 *
	 * first we make a copy of it.	copyObject() makes a deep copy so let's
	 * use it instead of the old fashoned lispCopy()...
	 */
	qualCopy = (List *) copyObject((Node *) qualList);

	foreach(qualcdr, qualCopy)
	{
		/*
		 * first get the current (op .. ..) list
		 */
		qual = lfirst(qualcdr);

		/*
		 * now get at the op
		 */
		op = (Oper *) qual->oper;
		if (!IsA(op, Oper))
			elog(ERROR, "MJFormSkipQual: op not an Oper!");

		/*
		 * Get the declared left and right operand types of the operator.
		 * Note we do *not* use the actual operand types, since those
		 * might be different in scenarios with binary-compatible data
		 * types. There should be "<" and ">" operators matching a
		 * mergejoinable "=" operator's declared operand types, but we
		 * might not find them if we search with the actual operand types.
		 */
		optup = SearchSysCache(OPEROID,
							   ObjectIdGetDatum(op->opno),
							   0, 0, 0);
		if (!HeapTupleIsValid(optup))	/* shouldn't happen */
			elog(ERROR, "MJFormSkipQual: operator %u not found", op->opno);
		opform = (Form_pg_operator) GETSTRUCT(optup);
		oprleft = opform->oprleft;
		oprright = opform->oprright;
		ReleaseSysCache(optup);

		/*
		 * Now look up the matching "<" or ">" operator.  If there isn't
		 * one, whoever marked the "=" operator mergejoinable was a loser.
		 */
		optup = SearchSysCache(OPERNAME,
							   PointerGetDatum(replaceopname),
							   ObjectIdGetDatum(oprleft),
							   ObjectIdGetDatum(oprright),
							   CharGetDatum('b'));
		if (!HeapTupleIsValid(optup))
			elog(ERROR,
			"MJFormSkipQual: mergejoin operator %u has no matching %s op",
				 op->opno, replaceopname);
		opform = (Form_pg_operator) GETSTRUCT(optup);

		/*
		 * And replace the data in the copied operator node.
		 */
		op->opno = optup->t_data->t_oid;
		op->opid = opform->oprcode;
		op->op_fcache = NULL;
		ReleaseSysCache(optup);
	}

	return qualCopy;
}

/* ----------------------------------------------------------------
 *		MergeCompare
 *
 *		Compare the keys according to 'compareQual' which is of the
 *		form: { (key1a > key2a) (key1b > key2b) ... }.
 *
 *		(actually, it could also be of the form (key1a < key2a)...)
 *
 *		This is different from calling ExecQual because ExecQual returns
 *		true only if ALL the comparison clauses are satisfied.
 *		However, there is an order of significance among the keys with
 *		the first keys being most significant. Therefore, the clauses
 *		are evaluated in order and the 'compareQual' is satisfied
 *		if (key1i > key2i) is true and (key1j = key2j) for 0 < j < i.
 *		We use the original mergeclause items to detect equality.
 * ----------------------------------------------------------------
 */
static bool
MergeCompare(List *eqQual, List *compareQual, ExprContext *econtext)
{
	bool		result;
	MemoryContext oldContext;
	List	   *clause;
	List	   *eqclause;

	/*
	 * Do expression eval in short-lived context.
	 */
	oldContext = MemoryContextSwitchTo(econtext->ecxt_per_tuple_memory);

	/*
	 * for each pair of clauses, test them until our compare conditions
	 * are satisfied. if we reach the end of the list, none of our key
	 * greater-than conditions were satisfied so we return false.
	 */
	result = false;				/* assume 'false' result */

	eqclause = eqQual;
	foreach(clause, compareQual)
	{
		Datum		const_value;
		bool		isNull;

		/*
		 * first test if our compare clause is satisfied. if so then
		 * return true.
		 *
		 * A NULL result is considered false.
		 */
		const_value = ExecEvalExpr((Node *) lfirst(clause), econtext,
								   &isNull, NULL);

		if (DatumGetBool(const_value) && !isNull)
		{
			result = true;
			break;
		}

		/*-----------
		 * ok, the compare clause failed so we test if the keys are
		 * equal... if key1 != key2, we return false. otherwise
		 * key1 = key2 so we move on to the next pair of keys.
		 *-----------
		 */
		const_value = ExecEvalExpr((Node *) lfirst(eqclause),
								   econtext,
								   &isNull,
								   NULL);

		if (!DatumGetBool(const_value) || isNull)
			break;				/* return false */

		eqclause = lnext(eqclause);
	}

	MemoryContextSwitchTo(oldContext);

	return result;
}

/* ----------------------------------------------------------------
 *		ExecMergeTupleDump
 *
 *		This function is called through the MJ_dump() macro
 *		when EXEC_MERGEJOINDEBUG is defined
 * ----------------------------------------------------------------
 */
#ifdef EXEC_MERGEJOINDEBUG

static void
ExecMergeTupleDumpOuter(MergeJoinState *mergestate)
{
	TupleTableSlot *outerSlot = mergestate->mj_OuterTupleSlot;

	printf("==== outer tuple ====\n");
	if (TupIsNull(outerSlot))
		printf("(nil)\n");
	else
		MJ_debugtup(outerSlot->val,
					outerSlot->ttc_tupleDescriptor);
}

static void
ExecMergeTupleDumpInner(MergeJoinState *mergestate)
{
	TupleTableSlot *innerSlot = mergestate->mj_InnerTupleSlot;

	printf("==== inner tuple ====\n");
	if (TupIsNull(innerSlot))
		printf("(nil)\n");
	else
		MJ_debugtup(innerSlot->val,
					innerSlot->ttc_tupleDescriptor);
}

static void
ExecMergeTupleDumpMarked(MergeJoinState *mergestate)
{
	TupleTableSlot *markedSlot = mergestate->mj_MarkedTupleSlot;

	printf("==== marked tuple ====\n");
	if (TupIsNull(markedSlot))
		printf("(nil)\n");
	else
		MJ_debugtup(markedSlot->val,
					markedSlot->ttc_tupleDescriptor);
}

static void
ExecMergeTupleDump(MergeJoinState *mergestate)
{
	printf("******** ExecMergeTupleDump ********\n");

	ExecMergeTupleDumpOuter(mergestate);
	ExecMergeTupleDumpInner(mergestate);
	ExecMergeTupleDumpMarked(mergestate);

	printf("******** \n");
}
#endif

/* ----------------------------------------------------------------
 *		ExecMergeJoin
 *
 * old comments
 *		Details of the merge-join routines:
 *
 *		(1) ">" and "<" operators
 *
 *		Merge-join is done by joining the inner and outer tuples satisfying
 *		the join clauses of the form ((= outerKey innerKey) ...).
 *		The join clauses is provided by the query planner and may contain
 *		more than one (= outerKey innerKey) clauses (for composite key).
 *
 *		However, the query executor needs to know whether an outer
 *		tuple is "greater/smaller" than an inner tuple so that it can
 *		"synchronize" the two relations. For e.g., consider the following
 *		relations:
 *
 *				outer: (0 ^1 1 2 5 5 5 6 6 7)	current tuple: 1
 *				inner: (1 ^3 5 5 5 5 6)			current tuple: 3
 *
 *		To continue the merge-join, the executor needs to scan both inner
 *		and outer relations till the matching tuples 5. It needs to know
 *		that currently inner tuple 3 is "greater" than outer tuple 1 and
 *		therefore it should scan the outer relation first to find a
 *		matching tuple and so on.
 *
 *		Therefore, when initializing the merge-join node, the executor
 *		creates the "greater/smaller" clause by substituting the "="
 *		operator in the join clauses with the corresponding ">" operator.
 *		The opposite "smaller/greater" clause is formed by substituting "<".
 *
 *		Note: prior to v6.5, the relational clauses were formed using the
 *		sort op used to sort the inner relation, which of course would fail
 *		if the outer and inner keys were of different data types.
 *		In the current code, we instead assume that operators named "<" and ">"
 *		will do the right thing.  This should be true since the mergejoin "="
 *		operator's pg_operator entry will have told the planner to sort by
 *		"<" for each of the left and right sides.
 *
 *		(2) repositioning inner "cursor"
 *
 *		Consider the above relations and suppose that the executor has
 *		just joined the first outer "5" with the last inner "5". The
 *		next step is of course to join the second outer "5" with all
 *		the inner "5's". This requires repositioning the inner "cursor"
 *		to point at the first inner "5". This is done by "marking" the
 *		first inner 5 and restore the "cursor" to it before joining
 *		with the second outer 5. The access method interface provides
 *		routines to mark and restore to a tuple.
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecMergeJoin(MergeJoin *node)
{
	EState	   *estate;
	MergeJoinState *mergestate;
	ScanDirection direction;
	List	   *innerSkipQual;
	List	   *outerSkipQual;
	List	   *mergeclauses;
	List	   *joinqual;
	List	   *otherqual;
	bool		qualResult;
	bool		compareResult;
	Plan	   *innerPlan;
	TupleTableSlot *innerTupleSlot;
	Plan	   *outerPlan;
	TupleTableSlot *outerTupleSlot;
	ExprContext *econtext;
	bool		doFillOuter;
	bool		doFillInner;

	/*
	 * get information from node
	 */
	mergestate = node->mergestate;
	estate = node->join.plan.state;
	direction = estate->es_direction;
	innerPlan = innerPlan((Plan *) node);
	outerPlan = outerPlan((Plan *) node);
	econtext = mergestate->jstate.cs_ExprContext;
	mergeclauses = node->mergeclauses;
	joinqual = node->join.joinqual;
	otherqual = node->join.plan.qual;

	switch (node->join.jointype)
	{
		case JOIN_INNER:
			doFillOuter = false;
			doFillInner = false;
			break;
		case JOIN_LEFT:
			doFillOuter = true;
			doFillInner = false;
			break;
		case JOIN_FULL:
			doFillOuter = true;
			doFillInner = true;
			break;
		case JOIN_RIGHT:
			doFillOuter = false;
			doFillInner = true;
			break;
		default:
			elog(ERROR, "ExecMergeJoin: unsupported join type %d",
				 (int) node->join.jointype);
			doFillOuter = false;	/* keep compiler quiet */
			doFillInner = false;
			break;
	}

	if (ScanDirectionIsForward(direction))
	{
		outerSkipQual = mergestate->mj_OuterSkipQual;
		innerSkipQual = mergestate->mj_InnerSkipQual;
	}
	else
	{
		outerSkipQual = mergestate->mj_InnerSkipQual;
		innerSkipQual = mergestate->mj_OuterSkipQual;
	}

	/*
	 * Check to see if we're still projecting out tuples from a previous
	 * join tuple (because there is a function-returning-set in the
	 * projection expressions).  If so, try to project another one.
	 */
	if (mergestate->jstate.cs_TupFromTlist)
	{
		TupleTableSlot *result;
		ExprDoneCond isDone;

		result = ExecProject(mergestate->jstate.cs_ProjInfo, &isDone);
		if (isDone == ExprMultipleResult)
			return result;
		/* Done with that source tuple... */
		mergestate->jstate.cs_TupFromTlist = false;
	}

	/*
	 * Reset per-tuple memory context to free any expression evaluation
	 * storage allocated in the previous tuple cycle.  Note this can't
	 * happen until we're done projecting out tuples from a join tuple.
	 */
	ResetExprContext(econtext);

	/*
	 * ok, everything is setup.. let's go to work
	 */
	for (;;)
	{
		/*
		 * get the current state of the join and do things accordingly.
		 * Note: The join states are highlighted with 32-* comments for
		 * improved readability.
		 */
		MJ_dump(mergestate);

		switch (mergestate->mj_JoinState)
		{
				/*
				 * EXEC_MJ_INITIALIZE means that this is the first time
				 * ExecMergeJoin() has been called and so we have to fetch
				 * the first tuple for both outer and inner subplans. If
				 * we fail to get a tuple here, then that subplan is
				 * empty, and we either end the join or go to one of the
				 * fill-remaining-tuples states.
				 */
			case EXEC_MJ_INITIALIZE:
				MJ_printf("ExecMergeJoin: EXEC_MJ_INITIALIZE\n");

				outerTupleSlot = ExecProcNode(outerPlan, (Plan *) node);
				mergestate->mj_OuterTupleSlot = outerTupleSlot;
				if (TupIsNull(outerTupleSlot))
				{
					MJ_printf("ExecMergeJoin: outer subplan is empty\n");
					if (doFillInner)
					{
						/*
						 * Need to emit right-join tuples for remaining
						 * inner tuples.  We set MatchedInner = true to
						 * force the ENDOUTER state to advance inner.
						 */
						mergestate->mj_JoinState = EXEC_MJ_ENDOUTER;
						mergestate->mj_MatchedInner = true;
						break;
					}
					/* Otherwise we're done. */
					return NULL;
				}

				innerTupleSlot = ExecProcNode(innerPlan, (Plan *) node);
				mergestate->mj_InnerTupleSlot = innerTupleSlot;
				if (TupIsNull(innerTupleSlot))
				{
					MJ_printf("ExecMergeJoin: inner subplan is empty\n");
					if (doFillOuter)
					{
						/*
						 * Need to emit left-join tuples for all outer
						 * tuples, including the one we just fetched.  We
						 * set MatchedOuter = false to force the ENDINNER
						 * state to emit this tuple before advancing
						 * outer.
						 */
						mergestate->mj_JoinState = EXEC_MJ_ENDINNER;
						mergestate->mj_MatchedOuter = false;
						break;
					}
					/* Otherwise we're done. */
					return NULL;
				}

				/*
				 * OK, we have the initial tuples.	Begin by skipping
				 * unmatched inner tuples.
				 */
				mergestate->mj_JoinState = EXEC_MJ_SKIPINNER_BEGIN;
				break;

				/*
				 * EXEC_MJ_JOINMARK means we have just found a new outer
				 * tuple and a possible matching inner tuple. This is the
				 * case after the INITIALIZE, SKIPOUTER or SKIPINNER
				 * states.
				 */
			case EXEC_MJ_JOINMARK:
				MJ_printf("ExecMergeJoin: EXEC_MJ_JOINMARK\n");

				ExecMarkPos(innerPlan);

				MarkInnerTuple(mergestate->mj_InnerTupleSlot, mergestate);

				mergestate->mj_JoinState = EXEC_MJ_JOINTEST;
				break;

				/*
				 * EXEC_MJ_JOINTEST means we have two tuples which might
				 * satisfy the merge clause, so we test them.
				 *
				 * If they do satisfy, then we join them and move on to the
				 * next inner tuple (EXEC_MJ_JOINTUPLES).
				 *
				 * If they do not satisfy then advance to next outer tuple.
				 */
			case EXEC_MJ_JOINTEST:
				MJ_printf("ExecMergeJoin: EXEC_MJ_JOINTEST\n");

				ResetExprContext(econtext);

				outerTupleSlot = mergestate->mj_OuterTupleSlot;
				econtext->ecxt_outertuple = outerTupleSlot;
				innerTupleSlot = mergestate->mj_InnerTupleSlot;
				econtext->ecxt_innertuple = innerTupleSlot;

				qualResult = ExecQual(mergeclauses, econtext, false);
				MJ_DEBUG_QUAL(mergeclauses, qualResult);

				if (qualResult)
					mergestate->mj_JoinState = EXEC_MJ_JOINTUPLES;
				else
					mergestate->mj_JoinState = EXEC_MJ_NEXTOUTER;
				break;

				/*
				 * EXEC_MJ_JOINTUPLES means we have two tuples which
				 * satisfied the merge clause so we join them and then
				 * proceed to get the next inner tuple (EXEC_NEXT_INNER).
				 */
			case EXEC_MJ_JOINTUPLES:
				MJ_printf("ExecMergeJoin: EXEC_MJ_JOINTUPLES\n");

				mergestate->mj_JoinState = EXEC_MJ_NEXTINNER;

				/*
				 * Check the extra qual conditions to see if we actually
				 * want to return this join tuple.	If not, can proceed
				 * with merge.	We must distinguish the additional
				 * joinquals (which must pass to consider the tuples
				 * "matched" for outer-join logic) from the otherquals
				 * (which must pass before we actually return the tuple).
				 *
				 * We don't bother with a ResetExprContext here, on the
				 * assumption that we just did one before checking the
				 * merge qual.	One per tuple should be sufficient.  Also,
				 * the econtext's tuple pointers were set up before
				 * checking the merge qual, so we needn't do it again.
				 */
				qualResult = (joinqual == NIL ||
							  ExecQual(joinqual, econtext, false));
				MJ_DEBUG_QUAL(joinqual, qualResult);

				if (qualResult)
				{
					mergestate->mj_MatchedOuter = true;
					mergestate->mj_MatchedInner = true;

					qualResult = (otherqual == NIL ||
								  ExecQual(otherqual, econtext, false));
					MJ_DEBUG_QUAL(otherqual, qualResult);

					if (qualResult)
					{
						/*
						 * qualification succeeded.  now form the desired
						 * projection tuple and return the slot containing
						 * it.
						 */
						TupleTableSlot *result;
						ExprDoneCond isDone;

						MJ_printf("ExecMergeJoin: returning tuple\n");

						result = ExecProject(mergestate->jstate.cs_ProjInfo,
											 &isDone);

						if (isDone != ExprEndResult)
						{
							mergestate->jstate.cs_TupFromTlist =
								(isDone == ExprMultipleResult);
							return result;
						}
					}
				}
				break;

				/*
				 * EXEC_MJ_NEXTINNER means advance the inner scan to the
				 * next tuple. If the tuple is not nil, we then proceed to
				 * test it against the join qualification.
				 *
				 * Before advancing, we check to see if we must emit an
				 * outer-join fill tuple for this inner tuple.
				 */
			case EXEC_MJ_NEXTINNER:
				MJ_printf("ExecMergeJoin: EXEC_MJ_NEXTINNER\n");

				if (doFillInner && !mergestate->mj_MatchedInner)
				{
					/*
					 * Generate a fake join tuple with nulls for the outer
					 * tuple, and return it if it passes the non-join
					 * quals.
					 */
					mergestate->mj_MatchedInner = true; /* do it only once */

					ResetExprContext(econtext);

					outerTupleSlot = mergestate->mj_NullOuterTupleSlot;
					econtext->ecxt_outertuple = outerTupleSlot;
					innerTupleSlot = mergestate->mj_InnerTupleSlot;
					econtext->ecxt_innertuple = innerTupleSlot;

					if (ExecQual(otherqual, econtext, false))
					{
						/*
						 * qualification succeeded.  now form the desired
						 * projection tuple and return the slot containing
						 * it.
						 */
						TupleTableSlot *result;
						ExprDoneCond isDone;

						MJ_printf("ExecMergeJoin: returning fill tuple\n");

						result = ExecProject(mergestate->jstate.cs_ProjInfo,
											 &isDone);

						if (isDone != ExprEndResult)
						{
							mergestate->jstate.cs_TupFromTlist =
								(isDone == ExprMultipleResult);
							return result;
						}
					}
				}

				/*
				 * now we get the next inner tuple, if any
				 */
				innerTupleSlot = ExecProcNode(innerPlan, (Plan *) node);
				mergestate->mj_InnerTupleSlot = innerTupleSlot;
				MJ_DEBUG_PROC_NODE(innerTupleSlot);
				mergestate->mj_MatchedInner = false;

				if (TupIsNull(innerTupleSlot))
					mergestate->mj_JoinState = EXEC_MJ_NEXTOUTER;
				else
					mergestate->mj_JoinState = EXEC_MJ_JOINTEST;
				break;

				/*-------------------------------------------
				 * EXEC_MJ_NEXTOUTER means
				 *
				 *				outer inner
				 * outer tuple -  5		5  - marked tuple
				 *				  5		5
				 *				  6		6  - inner tuple
				 *				  7		7
				 *
				 * we know we just bumped into the
				 * first inner tuple > current outer tuple
				 * so get a new outer tuple and then
				 * proceed to test it against the marked tuple
				 * (EXEC_MJ_TESTOUTER)
				 *
				 * Before advancing, we check to see if we must emit an
				 * outer-join fill tuple for this outer tuple.
				 *------------------------------------------------
				 */
			case EXEC_MJ_NEXTOUTER:
				MJ_printf("ExecMergeJoin: EXEC_MJ_NEXTOUTER\n");

				if (doFillOuter && !mergestate->mj_MatchedOuter)
				{
					/*
					 * Generate a fake join tuple with nulls for the inner
					 * tuple, and return it if it passes the non-join
					 * quals.
					 */
					mergestate->mj_MatchedOuter = true; /* do it only once */

					ResetExprContext(econtext);

					outerTupleSlot = mergestate->mj_OuterTupleSlot;
					econtext->ecxt_outertuple = outerTupleSlot;
					innerTupleSlot = mergestate->mj_NullInnerTupleSlot;
					econtext->ecxt_innertuple = innerTupleSlot;

					if (ExecQual(otherqual, econtext, false))
					{
						/*
						 * qualification succeeded.  now form the desired
						 * projection tuple and return the slot containing
						 * it.
						 */
						TupleTableSlot *result;
						ExprDoneCond isDone;

						MJ_printf("ExecMergeJoin: returning fill tuple\n");

						result = ExecProject(mergestate->jstate.cs_ProjInfo,
											 &isDone);

						if (isDone != ExprEndResult)
						{
							mergestate->jstate.cs_TupFromTlist =
								(isDone == ExprMultipleResult);
							return result;
						}
					}
				}

				/*
				 * now we get the next outer tuple, if any
				 */
				outerTupleSlot = ExecProcNode(outerPlan, (Plan *) node);
				mergestate->mj_OuterTupleSlot = outerTupleSlot;
				MJ_DEBUG_PROC_NODE(outerTupleSlot);
				mergestate->mj_MatchedOuter = false;

				/*
				 * if the outer tuple is null then we are done with the
				 * join, unless we have inner tuples we need to null-fill.
				 */
				if (TupIsNull(outerTupleSlot))
				{
					MJ_printf("ExecMergeJoin: end of outer subplan\n");
					innerTupleSlot = mergestate->mj_InnerTupleSlot;
					if (doFillInner && !TupIsNull(innerTupleSlot))
					{
						/*
						 * Need to emit right-join tuples for remaining
						 * inner tuples.
						 */
						mergestate->mj_JoinState = EXEC_MJ_ENDOUTER;
						break;
					}
					/* Otherwise we're done. */
					return NULL;
				}

				mergestate->mj_JoinState = EXEC_MJ_TESTOUTER;
				break;

				/*--------------------------------------------------------
				 * EXEC_MJ_TESTOUTER If the new outer tuple and the marked
				 * tuple satisfy the merge clause then we know we have
				 * duplicates in the outer scan so we have to restore the
				 * inner scan to the marked tuple and proceed to join the
				 * new outer tuples with the inner tuples (EXEC_MJ_JOINTEST)
				 *
				 * This is the case when
				 *						  outer inner
				 *							4	  5  - marked tuple
				 *			 outer tuple -	5	  5
				 *		 new outer tuple -	5	  5
				 *							6	  8  - inner tuple
				 *							7	 12
				 *
				 *				new outer tuple = marked tuple
				 *
				 *		If the outer tuple fails the test, then we know we have
				 *		to proceed to skip outer tuples until outer >= inner
				 *		(EXEC_MJ_SKIPOUTER).
				 *
				 *		This is the case when
				 *
				 *						  outer inner
				 *							5	  5  - marked tuple
				 *			 outer tuple -	5	  5
				 *		 new outer tuple -	6	  8  - inner tuple
				 *							7	 12
				 *
				 *
				 *		 new outer tuple > marked tuple
				 *
				 *---------------------------------------------------------
				 */
			case EXEC_MJ_TESTOUTER:
				MJ_printf("ExecMergeJoin: EXEC_MJ_TESTOUTER\n");

				/*
				 * here we compare the outer tuple with the marked inner
				 * tuple
				 */
				ResetExprContext(econtext);

				outerTupleSlot = mergestate->mj_OuterTupleSlot;
				econtext->ecxt_outertuple = outerTupleSlot;
				innerTupleSlot = mergestate->mj_MarkedTupleSlot;
				econtext->ecxt_innertuple = innerTupleSlot;

				qualResult = ExecQual(mergeclauses, econtext, false);
				MJ_DEBUG_QUAL(mergeclauses, qualResult);

				if (qualResult)
				{
					/*
					 * the merge clause matched so now we restore the
					 * inner scan position to the first mark, and loop
					 * back to JOINTEST.  Actually, since we know the
					 * mergeclause matches, we can skip JOINTEST and go
					 * straight to JOINTUPLES.
					 *
					 * NOTE: we do not need to worry about the MatchedInner
					 * state for the rescanned inner tuples.  We know all
					 * of them will match this new outer tuple and
					 * therefore won't be emitted as fill tuples.  This
					 * works *only* because we require the extra joinquals
					 * to be nil when doing a right or full join ---
					 * otherwise some of the rescanned tuples might fail
					 * the extra joinquals.
					 */
					ExecRestrPos(innerPlan);
					mergestate->mj_JoinState = EXEC_MJ_JOINTUPLES;
				}
				else
				{
					/* ----------------
					 *	if the inner tuple was nil and the new outer
					 *	tuple didn't match the marked outer tuple then
					 *	we have the case:
					 *
					 *			 outer inner
					 *			   4	 4	- marked tuple
					 * new outer - 5	 4
					 *			   6	nil - inner tuple
					 *			   7
					 *
					 *	which means that all subsequent outer tuples will be
					 *	larger than our marked inner tuples.  So we're done.
					 * ----------------
					 */
					innerTupleSlot = mergestate->mj_InnerTupleSlot;
					if (TupIsNull(innerTupleSlot))
					{
						if (doFillOuter)
						{
							/*
							 * Need to emit left-join tuples for remaining
							 * outer tuples.
							 */
							mergestate->mj_JoinState = EXEC_MJ_ENDINNER;
							break;
						}
						/* Otherwise we're done. */
						return NULL;
					}

					/* continue on to skip outer tuples */
					mergestate->mj_JoinState = EXEC_MJ_SKIPOUTER_BEGIN;
				}
				break;

				/*----------------------------------------------------------
				 * EXEC_MJ_SKIPOUTER means skip over tuples in the outer plan
				 * until we find an outer tuple >= current inner tuple.
				 *
				 * For example:
				 *
				 *				outer inner
				 *				  5		5
				 *				  5		5
				 * outer tuple -  6		8  - inner tuple
				 *				  7    12
				 *				  8    14
				 *
				 * we have to advance the outer scan
				 * until we find the outer 8.
				 *
				 * To avoid redundant tests, we divide this into three
				 * sub-states: BEGIN, TEST, ADVANCE.
				 *----------------------------------------------------------
				 */
			case EXEC_MJ_SKIPOUTER_BEGIN:
				MJ_printf("ExecMergeJoin: EXEC_MJ_SKIPOUTER_BEGIN\n");

				/*
				 * before we advance, make sure the current tuples do not
				 * satisfy the mergeclauses.  If they do, then we update
				 * the marked tuple and go join them.
				 */
				ResetExprContext(econtext);

				outerTupleSlot = mergestate->mj_OuterTupleSlot;
				econtext->ecxt_outertuple = outerTupleSlot;
				innerTupleSlot = mergestate->mj_InnerTupleSlot;
				econtext->ecxt_innertuple = innerTupleSlot;

				qualResult = ExecQual(mergeclauses, econtext, false);
				MJ_DEBUG_QUAL(mergeclauses, qualResult);

				if (qualResult)
				{
					ExecMarkPos(innerPlan);

					MarkInnerTuple(innerTupleSlot, mergestate);

					mergestate->mj_JoinState = EXEC_MJ_JOINTUPLES;
					break;
				}

				mergestate->mj_JoinState = EXEC_MJ_SKIPOUTER_TEST;
				break;

			case EXEC_MJ_SKIPOUTER_TEST:
				MJ_printf("ExecMergeJoin: EXEC_MJ_SKIPOUTER_TEST\n");

				/*
				 * ok, now test the skip qualification
				 */
				outerTupleSlot = mergestate->mj_OuterTupleSlot;
				econtext->ecxt_outertuple = outerTupleSlot;
				innerTupleSlot = mergestate->mj_InnerTupleSlot;
				econtext->ecxt_innertuple = innerTupleSlot;

				compareResult = MergeCompare(mergeclauses,
											 outerSkipQual,
											 econtext);

				MJ_DEBUG_MERGE_COMPARE(outerSkipQual, compareResult);

				/*
				 * compareResult is true as long as we should continue
				 * skipping outer tuples.
				 */
				if (compareResult)
				{
					mergestate->mj_JoinState = EXEC_MJ_SKIPOUTER_ADVANCE;
					break;
				}

				/*
				 * now check the inner skip qual to see if we should now
				 * skip inner tuples... if we fail the inner skip qual,
				 * then we know we have a new pair of matching tuples.
				 */
				compareResult = MergeCompare(mergeclauses,
											 innerSkipQual,
											 econtext);

				MJ_DEBUG_MERGE_COMPARE(innerSkipQual, compareResult);

				if (compareResult)
					mergestate->mj_JoinState = EXEC_MJ_SKIPINNER_BEGIN;
				else
					mergestate->mj_JoinState = EXEC_MJ_JOINMARK;
				break;

				/*
				 * Before advancing, we check to see if we must emit an
				 * outer-join fill tuple for this outer tuple.
				 */
			case EXEC_MJ_SKIPOUTER_ADVANCE:
				MJ_printf("ExecMergeJoin: EXEC_MJ_SKIPOUTER_ADVANCE\n");

				if (doFillOuter && !mergestate->mj_MatchedOuter)
				{
					/*
					 * Generate a fake join tuple with nulls for the inner
					 * tuple, and return it if it passes the non-join
					 * quals.
					 */
					mergestate->mj_MatchedOuter = true; /* do it only once */

					ResetExprContext(econtext);

					outerTupleSlot = mergestate->mj_OuterTupleSlot;
					econtext->ecxt_outertuple = outerTupleSlot;
					innerTupleSlot = mergestate->mj_NullInnerTupleSlot;
					econtext->ecxt_innertuple = innerTupleSlot;

					if (ExecQual(otherqual, econtext, false))
					{
						/*
						 * qualification succeeded.  now form the desired
						 * projection tuple and return the slot containing
						 * it.
						 */
						TupleTableSlot *result;
						ExprDoneCond isDone;

						MJ_printf("ExecMergeJoin: returning fill tuple\n");

						result = ExecProject(mergestate->jstate.cs_ProjInfo,
											 &isDone);

						if (isDone != ExprEndResult)
						{
							mergestate->jstate.cs_TupFromTlist =
								(isDone == ExprMultipleResult);
							return result;
						}
					}
				}

				/*
				 * now we get the next outer tuple, if any
				 */
				outerTupleSlot = ExecProcNode(outerPlan, (Plan *) node);
				mergestate->mj_OuterTupleSlot = outerTupleSlot;
				MJ_DEBUG_PROC_NODE(outerTupleSlot);
				mergestate->mj_MatchedOuter = false;

				/*
				 * if the outer tuple is null then we are done with the
				 * join, unless we have inner tuples we need to null-fill.
				 */
				if (TupIsNull(outerTupleSlot))
				{
					MJ_printf("ExecMergeJoin: end of outer subplan\n");
					innerTupleSlot = mergestate->mj_InnerTupleSlot;
					if (doFillInner && !TupIsNull(innerTupleSlot))
					{
						/*
						 * Need to emit right-join tuples for remaining
						 * inner tuples.
						 */
						mergestate->mj_JoinState = EXEC_MJ_ENDOUTER;
						break;
					}
					/* Otherwise we're done. */
					return NULL;
				}

				/*
				 * otherwise test the new tuple against the skip qual.
				 */
				mergestate->mj_JoinState = EXEC_MJ_SKIPOUTER_TEST;
				break;

				/*-----------------------------------------------------------
				 * EXEC_MJ_SKIPINNER means skip over tuples in the inner plan
				 * until we find an inner tuple >= current outer tuple.
				 *
				 * For example:
				 *
				 *				outer inner
				 *				  5		5
				 *				  5		5
				 * outer tuple - 12		8  - inner tuple
				 *				 14    10
				 *				 17    12
				 *
				 * we have to advance the inner scan
				 * until we find the inner 12.
				 *
				 * To avoid redundant tests, we divide this into three
				 * sub-states: BEGIN, TEST, ADVANCE.
				 *-------------------------------------------------------
				 */
			case EXEC_MJ_SKIPINNER_BEGIN:
				MJ_printf("ExecMergeJoin: EXEC_MJ_SKIPINNER_BEGIN\n");

				/*
				 * before we advance, make sure the current tuples do not
				 * satisfy the mergeclauses.  If they do, then we update
				 * the marked tuple and go join them.
				 */
				ResetExprContext(econtext);

				outerTupleSlot = mergestate->mj_OuterTupleSlot;
				econtext->ecxt_outertuple = outerTupleSlot;
				innerTupleSlot = mergestate->mj_InnerTupleSlot;
				econtext->ecxt_innertuple = innerTupleSlot;

				qualResult = ExecQual(mergeclauses, econtext, false);
				MJ_DEBUG_QUAL(mergeclauses, qualResult);

				if (qualResult)
				{
					ExecMarkPos(innerPlan);

					MarkInnerTuple(innerTupleSlot, mergestate);

					mergestate->mj_JoinState = EXEC_MJ_JOINTUPLES;
					break;
				}

				mergestate->mj_JoinState = EXEC_MJ_SKIPINNER_TEST;
				break;

			case EXEC_MJ_SKIPINNER_TEST:
				MJ_printf("ExecMergeJoin: EXEC_MJ_SKIPINNER_TEST\n");

				/*
				 * ok, now test the skip qualification
				 */
				outerTupleSlot = mergestate->mj_OuterTupleSlot;
				econtext->ecxt_outertuple = outerTupleSlot;
				innerTupleSlot = mergestate->mj_InnerTupleSlot;
				econtext->ecxt_innertuple = innerTupleSlot;

				compareResult = MergeCompare(mergeclauses,
											 innerSkipQual,
											 econtext);

				MJ_DEBUG_MERGE_COMPARE(innerSkipQual, compareResult);

				/*
				 * compareResult is true as long as we should continue
				 * skipping inner tuples.
				 */
				if (compareResult)
				{
					mergestate->mj_JoinState = EXEC_MJ_SKIPINNER_ADVANCE;
					break;
				}

				/*
				 * now check the outer skip qual to see if we should now
				 * skip outer tuples... if we fail the outer skip qual,
				 * then we know we have a new pair of matching tuples.
				 */
				compareResult = MergeCompare(mergeclauses,
											 outerSkipQual,
											 econtext);

				MJ_DEBUG_MERGE_COMPARE(outerSkipQual, compareResult);

				if (compareResult)
					mergestate->mj_JoinState = EXEC_MJ_SKIPOUTER_BEGIN;
				else
					mergestate->mj_JoinState = EXEC_MJ_JOINMARK;
				break;

				/*
				 * Before advancing, we check to see if we must emit an
				 * outer-join fill tuple for this inner tuple.
				 */
			case EXEC_MJ_SKIPINNER_ADVANCE:
				MJ_printf("ExecMergeJoin: EXEC_MJ_SKIPINNER_ADVANCE\n");

				if (doFillInner && !mergestate->mj_MatchedInner)
				{
					/*
					 * Generate a fake join tuple with nulls for the outer
					 * tuple, and return it if it passes the non-join
					 * quals.
					 */
					mergestate->mj_MatchedInner = true; /* do it only once */

					ResetExprContext(econtext);

					outerTupleSlot = mergestate->mj_NullOuterTupleSlot;
					econtext->ecxt_outertuple = outerTupleSlot;
					innerTupleSlot = mergestate->mj_InnerTupleSlot;
					econtext->ecxt_innertuple = innerTupleSlot;

					if (ExecQual(otherqual, econtext, false))
					{
						/*
						 * qualification succeeded.  now form the desired
						 * projection tuple and return the slot containing
						 * it.
						 */
						TupleTableSlot *result;
						ExprDoneCond isDone;

						MJ_printf("ExecMergeJoin: returning fill tuple\n");

						result = ExecProject(mergestate->jstate.cs_ProjInfo,
											 &isDone);

						if (isDone != ExprEndResult)
						{
							mergestate->jstate.cs_TupFromTlist =
								(isDone == ExprMultipleResult);
							return result;
						}
					}
				}

				/*
				 * now we get the next inner tuple, if any
				 */
				innerTupleSlot = ExecProcNode(innerPlan, (Plan *) node);
				mergestate->mj_InnerTupleSlot = innerTupleSlot;
				MJ_DEBUG_PROC_NODE(innerTupleSlot);
				mergestate->mj_MatchedInner = false;

				/*
				 * if the inner tuple is null then we are done with the
				 * join, unless we have outer tuples we need to null-fill.
				 */
				if (TupIsNull(innerTupleSlot))
				{
					MJ_printf("ExecMergeJoin: end of inner subplan\n");
					outerTupleSlot = mergestate->mj_OuterTupleSlot;
					if (doFillOuter && !TupIsNull(outerTupleSlot))
					{
						/*
						 * Need to emit left-join tuples for remaining
						 * outer tuples.
						 */
						mergestate->mj_JoinState = EXEC_MJ_ENDINNER;
						break;
					}
					/* Otherwise we're done. */
					return NULL;
				}

				/*
				 * otherwise test the new tuple against the skip qual.
				 */
				mergestate->mj_JoinState = EXEC_MJ_SKIPINNER_TEST;
				break;

				/*
				 * EXEC_MJ_ENDOUTER means we have run out of outer tuples,
				 * but are doing a right/full join and therefore must
				 * null- fill any remaing unmatched inner tuples.
				 */
			case EXEC_MJ_ENDOUTER:
				MJ_printf("ExecMergeJoin: EXEC_MJ_ENDOUTER\n");

				Assert(doFillInner);

				if (!mergestate->mj_MatchedInner)
				{
					/*
					 * Generate a fake join tuple with nulls for the outer
					 * tuple, and return it if it passes the non-join
					 * quals.
					 */
					mergestate->mj_MatchedInner = true; /* do it only once */

					ResetExprContext(econtext);

					outerTupleSlot = mergestate->mj_NullOuterTupleSlot;
					econtext->ecxt_outertuple = outerTupleSlot;
					innerTupleSlot = mergestate->mj_InnerTupleSlot;
					econtext->ecxt_innertuple = innerTupleSlot;

					if (ExecQual(otherqual, econtext, false))
					{
						/*
						 * qualification succeeded.  now form the desired
						 * projection tuple and return the slot containing
						 * it.
						 */
						TupleTableSlot *result;
						ExprDoneCond isDone;

						MJ_printf("ExecMergeJoin: returning fill tuple\n");

						result = ExecProject(mergestate->jstate.cs_ProjInfo,
											 &isDone);

						if (isDone != ExprEndResult)
						{
							mergestate->jstate.cs_TupFromTlist =
								(isDone == ExprMultipleResult);
							return result;
						}
					}
				}

				/*
				 * now we get the next inner tuple, if any
				 */
				innerTupleSlot = ExecProcNode(innerPlan, (Plan *) node);
				mergestate->mj_InnerTupleSlot = innerTupleSlot;
				MJ_DEBUG_PROC_NODE(innerTupleSlot);
				mergestate->mj_MatchedInner = false;

				if (TupIsNull(innerTupleSlot))
				{
					MJ_printf("ExecMergeJoin: end of inner subplan\n");
					return NULL;
				}

				/* Else remain in ENDOUTER state and process next tuple. */
				break;

				/*
				 * EXEC_MJ_ENDINNER means we have run out of inner tuples,
				 * but are doing a left/full join and therefore must null-
				 * fill any remaing unmatched outer tuples.
				 */
			case EXEC_MJ_ENDINNER:
				MJ_printf("ExecMergeJoin: EXEC_MJ_ENDINNER\n");

				Assert(doFillOuter);

				if (!mergestate->mj_MatchedOuter)
				{
					/*
					 * Generate a fake join tuple with nulls for the inner
					 * tuple, and return it if it passes the non-join
					 * quals.
					 */
					mergestate->mj_MatchedOuter = true; /* do it only once */

					ResetExprContext(econtext);

					outerTupleSlot = mergestate->mj_OuterTupleSlot;
					econtext->ecxt_outertuple = outerTupleSlot;
					innerTupleSlot = mergestate->mj_NullInnerTupleSlot;
					econtext->ecxt_innertuple = innerTupleSlot;

					if (ExecQual(otherqual, econtext, false))
					{
						/*
						 * qualification succeeded.  now form the desired
						 * projection tuple and return the slot containing
						 * it.
						 */
						TupleTableSlot *result;
						ExprDoneCond isDone;

						MJ_printf("ExecMergeJoin: returning fill tuple\n");

						result = ExecProject(mergestate->jstate.cs_ProjInfo,
											 &isDone);

						if (isDone != ExprEndResult)
						{
							mergestate->jstate.cs_TupFromTlist =
								(isDone == ExprMultipleResult);
							return result;
						}
					}
				}

				/*
				 * now we get the next outer tuple, if any
				 */
				outerTupleSlot = ExecProcNode(outerPlan, (Plan *) node);
				mergestate->mj_OuterTupleSlot = outerTupleSlot;
				MJ_DEBUG_PROC_NODE(outerTupleSlot);
				mergestate->mj_MatchedOuter = false;

				if (TupIsNull(outerTupleSlot))
				{
					MJ_printf("ExecMergeJoin: end of outer subplan\n");
					return NULL;
				}

				/* Else remain in ENDINNER state and process next tuple. */
				break;

				/*
				 * if we get here it means our code is fouled up and so we
				 * just end the join prematurely.
				 */
			default:
				elog(NOTICE, "ExecMergeJoin: invalid join state %d, aborting",
					 mergestate->mj_JoinState);
				return NULL;
		}
	}
}

/* ----------------------------------------------------------------
 *		ExecInitMergeJoin
 *
 * old comments
 *		Creates the run-time state information for the node and
 *		sets the relation id to contain relevant decriptors.
 * ----------------------------------------------------------------
 */
bool
ExecInitMergeJoin(MergeJoin *node, EState *estate, Plan *parent)
{
	MergeJoinState *mergestate;
	List	   *joinclauses;

	MJ1_printf("ExecInitMergeJoin: %s\n",
			   "initializing node");

	/*
	 * assign the node's execution state and get the range table and
	 * direction from it
	 */
	node->join.plan.state = estate;

	/*
	 * create new merge state for node
	 */
	mergestate = makeNode(MergeJoinState);
	node->mergestate = mergestate;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &mergestate->jstate);

	/*
	 * initialize subplans
	 */
	ExecInitNode(outerPlan((Plan *) node), estate, (Plan *) node);
	ExecInitNode(innerPlan((Plan *) node), estate, (Plan *) node);

#define MERGEJOIN_NSLOTS 4

	/*
	 * tuple table initialization
	 */
	ExecInitResultTupleSlot(estate, &mergestate->jstate);

	mergestate->mj_MarkedTupleSlot = ExecInitExtraTupleSlot(estate);
	ExecSetSlotDescriptor(mergestate->mj_MarkedTupleSlot,
						  ExecGetTupType(innerPlan((Plan *) node)),
						  false);

	switch (node->join.jointype)
	{
		case JOIN_INNER:
			break;
		case JOIN_LEFT:
			mergestate->mj_NullInnerTupleSlot =
				ExecInitNullTupleSlot(estate,
							   ExecGetTupType(innerPlan((Plan *) node)));
			break;
		case JOIN_RIGHT:
			mergestate->mj_NullOuterTupleSlot =
				ExecInitNullTupleSlot(estate,
							   ExecGetTupType(outerPlan((Plan *) node)));

			/*
			 * Can't handle right or full join with non-nil extra
			 * joinclauses.
			 */
			if (node->join.joinqual != NIL)
				elog(ERROR, "RIGHT JOIN is only supported with mergejoinable join conditions");
			break;
		case JOIN_FULL:
			mergestate->mj_NullOuterTupleSlot =
				ExecInitNullTupleSlot(estate,
							   ExecGetTupType(outerPlan((Plan *) node)));
			mergestate->mj_NullInnerTupleSlot =
				ExecInitNullTupleSlot(estate,
							   ExecGetTupType(innerPlan((Plan *) node)));

			/*
			 * Can't handle right or full join with non-nil extra
			 * joinclauses.
			 */
			if (node->join.joinqual != NIL)
				elog(ERROR, "FULL JOIN is only supported with mergejoinable join conditions");
			break;
		default:
			elog(ERROR, "ExecInitMergeJoin: unsupported join type %d",
				 (int) node->join.jointype);
	}

	/*
	 * initialize tuple type and projection info
	 */
	ExecAssignResultTypeFromTL((Plan *) node, &mergestate->jstate);
	ExecAssignProjectionInfo((Plan *) node, &mergestate->jstate);

	/*
	 * form merge skip qualifications
	 */
	joinclauses = node->mergeclauses;
	mergestate->mj_OuterSkipQual = MJFormSkipQual(joinclauses, "<");
	mergestate->mj_InnerSkipQual = MJFormSkipQual(joinclauses, ">");

	MJ_printf("\nExecInitMergeJoin: OuterSkipQual is ");
	MJ_nodeDisplay(mergestate->mj_OuterSkipQual);
	MJ_printf("\nExecInitMergeJoin: InnerSkipQual is ");
	MJ_nodeDisplay(mergestate->mj_InnerSkipQual);
	MJ_printf("\n");

	/*
	 * initialize join state
	 */
	mergestate->mj_JoinState = EXEC_MJ_INITIALIZE;
	mergestate->jstate.cs_TupFromTlist = false;
	mergestate->mj_MatchedOuter = false;
	mergestate->mj_MatchedInner = false;
	mergestate->mj_OuterTupleSlot = NULL;
	mergestate->mj_InnerTupleSlot = NULL;

	/*
	 * initialization successful
	 */
	MJ1_printf("ExecInitMergeJoin: %s\n",
			   "node initialized");

	return TRUE;
}

int
ExecCountSlotsMergeJoin(MergeJoin *node)
{
	return ExecCountSlotsNode(outerPlan((Plan *) node)) +
		ExecCountSlotsNode(innerPlan((Plan *) node)) +
		MERGEJOIN_NSLOTS;
}

/* ----------------------------------------------------------------
 *		ExecEndMergeJoin
 *
 * old comments
 *		frees storage allocated through C routines.
 * ----------------------------------------------------------------
 */
void
ExecEndMergeJoin(MergeJoin *node)
{
	MergeJoinState *mergestate;

	MJ1_printf("ExecEndMergeJoin: %s\n",
			   "ending node processing");

	/*
	 * get state information from the node
	 */
	mergestate = node->mergestate;

	/*
	 * Free the projection info and the scan attribute info
	 *
	 * Note: we don't ExecFreeResultType(mergestate) because the rule manager
	 * depends on the tupType returned by ExecMain().  So for now, this is
	 * freed at end-transaction time.  -cim 6/2/91
	 */
	ExecFreeProjectionInfo(&mergestate->jstate);
	ExecFreeExprContext(&mergestate->jstate);

	/*
	 * shut down the subplans
	 */
	ExecEndNode((Plan *) innerPlan((Plan *) node), (Plan *) node);
	ExecEndNode((Plan *) outerPlan((Plan *) node), (Plan *) node);

	/*
	 * clean out the tuple table
	 */
	ExecClearTuple(mergestate->jstate.cs_ResultTupleSlot);
	ExecClearTuple(mergestate->mj_MarkedTupleSlot);

	MJ1_printf("ExecEndMergeJoin: %s\n",
			   "node processing ended");
}

void
ExecReScanMergeJoin(MergeJoin *node, ExprContext *exprCtxt, Plan *parent)
{
	MergeJoinState *mergestate = node->mergestate;

	ExecClearTuple(mergestate->mj_MarkedTupleSlot);

	mergestate->mj_JoinState = EXEC_MJ_INITIALIZE;
	mergestate->jstate.cs_TupFromTlist = false;
	mergestate->mj_MatchedOuter = false;
	mergestate->mj_MatchedInner = false;
	mergestate->mj_OuterTupleSlot = NULL;
	mergestate->mj_InnerTupleSlot = NULL;

	/*
	 * if chgParam of subnodes is not null then plans will be re-scanned
	 * by first ExecProcNode.
	 */
	if (((Plan *) node)->lefttree->chgParam == NULL)
		ExecReScan(((Plan *) node)->lefttree, exprCtxt, (Plan *) node);
	if (((Plan *) node)->righttree->chgParam == NULL)
		ExecReScan(((Plan *) node)->righttree, exprCtxt, (Plan *) node);

}
