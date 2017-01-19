/*-------------------------------------------------------------------------
 *
 * prepunion.c
 *	  Routines to plan set-operation queries.  The filename is a leftover
 *	  from a time when only UNIONs were implemented.
 *
 * There is also some code here to support planning of queries that use
 * inheritance (SELECT FROM foo*).	This no longer has much connection
 * to the processing of UNION queries, but it's still here.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/prep/prepunion.c,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <sys/types.h>

#include "catalog/pg_type.h"
#include "nodes/makefuncs.h"
#include "optimizer/clauses.h"
#include "optimizer/plancat.h"
#include "optimizer/planmain.h"
#include "optimizer/planner.h"
#include "optimizer/prep.h"
#include "optimizer/tlist.h"
#include "parser/parse_clause.h"
#include "parser/parse_coerce.h"
#include "parser/parsetree.h"
#include "utils/lsyscache.h"

/* macros borrowed from expression_tree_mutator */

#define FLATCOPY(newnode, node, nodetype)  \
	( (newnode) = makeNode(nodetype), \
	  memcpy((newnode), (node), sizeof(nodetype)) )

typedef struct
{
	Index		old_rt_index;
	Index		new_rt_index;
	Oid			old_relid;
	Oid			new_relid;
} adjust_inherited_attrs_context;

static Plan *recurse_set_operations(Node *setOp, Query *parse,
					   List *colTypes, bool junkOK,
					   int flag, List *refnames_tlist);
static Plan *generate_union_plan(SetOperationStmt *op, Query *parse,
					List *refnames_tlist);
static Plan *generate_nonunion_plan(SetOperationStmt *op, Query *parse,
					   List *refnames_tlist);
static List *recurse_union_children(Node *setOp, Query *parse,
					   SetOperationStmt *top_union,
					   List *refnames_tlist);
static List *generate_setop_tlist(List *colTypes, int flag,
					 bool hack_constants,
					 List *input_tlist,
					 List *refnames_tlist);
static List *generate_append_tlist(List *colTypes, bool flag,
					 List *input_plans,
					 List *refnames_tlist);
static bool tlist_same_datatypes(List *tlist, List *colTypes, bool junkOK);
static Node *adjust_inherited_attrs_mutator(Node *node,
							   adjust_inherited_attrs_context *context);


/*
 * plan_set_operations
 *
 *	  Plans the queries for a tree of set operations (UNION/INTERSECT/EXCEPT)
 *
 * This routine only deals with the setOperations tree of the given query.
 * Any top-level ORDER BY requested in parse->sortClause will be added
 * when we return to grouping_planner.
 */
Plan *
plan_set_operations(Query *parse)
{
	SetOperationStmt *topop = (SetOperationStmt *) parse->setOperations;
	Node	   *node;
	Query	   *leftmostQuery;

	Assert(topop && IsA(topop, SetOperationStmt));

	/*
	 * Find the leftmost component Query.  We need to use its column names
	 * for all generated tlists (else SELECT INTO won't work right).
	 */
	node = topop->larg;
	while (node && IsA(node, SetOperationStmt))
		node = ((SetOperationStmt *) node)->larg;
	Assert(node && IsA(node, RangeTblRef));
	leftmostQuery = rt_fetch(((RangeTblRef *) node)->rtindex,
							 parse->rtable)->subquery;
	Assert(leftmostQuery != NULL);

	/*
	 * Recurse on setOperations tree to generate plans for set ops. The
	 * final output plan should have just the column types shown as the
	 * output from the top-level node, plus possibly a resjunk working
	 * column (we can rely on upper-level nodes to deal with that).
	 */
	return recurse_set_operations((Node *) topop, parse,
								  topop->colTypes, true, -1,
								  leftmostQuery->targetList);
}

/*
 * recurse_set_operations
 *	  Recursively handle one step in a tree of set operations
 *
 * colTypes: integer list of type OIDs of expected output columns
 * junkOK: if true, child resjunk columns may be left in the result
 * flag: if >= 0, add a resjunk output column indicating value of flag
 * refnames_tlist: targetlist to take column names from
 */
static Plan *
recurse_set_operations(Node *setOp, Query *parse,
					   List *colTypes, bool junkOK,
					   int flag, List *refnames_tlist)
{
	if (IsA(setOp, RangeTblRef))
	{
		RangeTblRef *rtr = (RangeTblRef *) setOp;
		RangeTblEntry *rte = rt_fetch(rtr->rtindex, parse->rtable);
		Query	   *subquery = rte->subquery;
		Plan	   *subplan,
				   *plan;

		Assert(subquery != NULL);

		/*
		 * Generate plan for primitive subquery
		 */
		subplan = subquery_planner(subquery,
								   -1.0 /* default case */ );

		/*
		 * Add a SubqueryScan with the caller-requested targetlist
		 */
		plan = (Plan *)
			make_subqueryscan(generate_setop_tlist(colTypes, flag, true,
												   subplan->targetlist,
												   refnames_tlist),
							  NIL,
							  rtr->rtindex,
							  subplan);
		return plan;
	}
	else if (IsA(setOp, SetOperationStmt))
	{
		SetOperationStmt *op = (SetOperationStmt *) setOp;
		Plan	   *plan;

		/* UNIONs are much different from INTERSECT/EXCEPT */
		if (op->op == SETOP_UNION)
			plan = generate_union_plan(op, parse, refnames_tlist);
		else
			plan = generate_nonunion_plan(op, parse, refnames_tlist);

		/*
		 * If necessary, add a Result node to project the caller-requested
		 * output columns.
		 *
		 * XXX you don't really want to know about this: setrefs.c will apply
		 * replace_vars_with_subplan_refs() to the Result node's tlist.
		 * This would fail if the Vars generated by generate_setop_tlist()
		 * were not exactly equal() to the corresponding tlist entries of
		 * the subplan.  However, since the subplan was generated by
		 * generate_union_plan() or generate_nonunion_plan(), and hence its
		 * tlist was generated by generate_append_tlist(), this will work.
		 */
		if (flag >= 0 ||
			!tlist_same_datatypes(plan->targetlist, colTypes, junkOK))
		{
			plan = (Plan *)
				make_result(generate_setop_tlist(colTypes, flag, false,
												 plan->targetlist,
												 refnames_tlist),
							NULL,
							plan);
		}
		return plan;
	}
	else
	{
		elog(ERROR, "recurse_set_operations: unexpected node %d",
			 (int) nodeTag(setOp));
		return NULL;			/* keep compiler quiet */
	}
}

/*
 * Generate plan for a UNION or UNION ALL node
 */
static Plan *
generate_union_plan(SetOperationStmt *op, Query *parse,
					List *refnames_tlist)
{
	List	   *planlist;
	List	   *tlist;
	Plan	   *plan;

	/*
	 * If any of my children are identical UNION nodes (same op, all-flag,
	 * and colTypes) then they can be merged into this node so that we
	 * generate only one Append and Sort for the lot.  Recurse to find
	 * such nodes and compute their children's plans.
	 */
	planlist = nconc(recurse_union_children(op->larg, parse,
											op, refnames_tlist),
					 recurse_union_children(op->rarg, parse,
											op, refnames_tlist));

	/*
	 * Generate tlist for Append plan node.
	 *
	 * The tlist for an Append plan isn't important as far as the Append is
	 * concerned, but we must make it look real anyway for the benefit of
	 * the next plan level up.
	 */
	tlist = generate_append_tlist(op->colTypes, false,
								  planlist, refnames_tlist);

	/*
	 * Append the child results together.
	 */
	plan = (Plan *) make_append(planlist, false, tlist);

	/*
	 * For UNION ALL, we just need the Append plan.  For UNION, need to
	 * add Sort and Unique nodes to produce unique output.
	 */
	if (!op->all)
	{
		List	   *sortList;

		tlist = new_unsorted_tlist(tlist);
		sortList = addAllTargetsToSortList(NIL, tlist);
		plan = make_sortplan(parse, tlist, plan, sortList);
		plan = (Plan *) make_unique(tlist, plan, copyObject(sortList));
	}
	return plan;
}

/*
 * Generate plan for an INTERSECT, INTERSECT ALL, EXCEPT, or EXCEPT ALL node
 */
static Plan *
generate_nonunion_plan(SetOperationStmt *op, Query *parse,
					   List *refnames_tlist)
{
	Plan	   *lplan,
			   *rplan,
			   *plan;
	List	   *tlist,
			   *sortList,
			   *planlist;
	SetOpCmd	cmd;

	/* Recurse on children, ensuring their outputs are marked */
	lplan = recurse_set_operations(op->larg, parse,
								   op->colTypes, false, 0,
								   refnames_tlist);
	rplan = recurse_set_operations(op->rarg, parse,
								   op->colTypes, false, 1,
								   refnames_tlist);
	planlist = makeList2(lplan, rplan);

	/*
	 * Generate tlist for Append plan node.
	 *
	 * The tlist for an Append plan isn't important as far as the Append is
	 * concerned, but we must make it look real anyway for the benefit of
	 * the next plan level up.	In fact, it has to be real enough that the
	 * flag column is shown as a variable not a constant, else setrefs.c
	 * will get confused.
	 */
	tlist = generate_append_tlist(op->colTypes, true,
								  planlist, refnames_tlist);

	/*
	 * Append the child results together.
	 */
	plan = (Plan *) make_append(planlist, false, tlist);

	/*
	 * Sort the child results, then add a SetOp plan node to generate the
	 * correct output.
	 */
	tlist = new_unsorted_tlist(tlist);
	sortList = addAllTargetsToSortList(NIL, tlist);
	plan = make_sortplan(parse, tlist, plan, sortList);
	switch (op->op)
	{
		case SETOP_INTERSECT:
			cmd = op->all ? SETOPCMD_INTERSECT_ALL : SETOPCMD_INTERSECT;
			break;
		case SETOP_EXCEPT:
			cmd = op->all ? SETOPCMD_EXCEPT_ALL : SETOPCMD_EXCEPT;
			break;
		default:
			elog(ERROR, "generate_nonunion_plan: bogus operation code");
			cmd = SETOPCMD_INTERSECT;	/* keep compiler quiet */
			break;
	}
	plan = (Plan *) make_setop(cmd, tlist, plan, sortList,
							   length(op->colTypes) + 1);
	return plan;
}

/*
 * Pull up children of a UNION node that are identically-propertied UNIONs.
 *
 * NOTE: we can also pull a UNION ALL up into a UNION, since the distinct
 * output rows will be lost anyway.
 */
static List *
recurse_union_children(Node *setOp, Query *parse,
					   SetOperationStmt *top_union,
					   List *refnames_tlist)
{
	if (IsA(setOp, SetOperationStmt))
	{
		SetOperationStmt *op = (SetOperationStmt *) setOp;

		if (op->op == top_union->op &&
			(op->all == top_union->all || op->all) &&
			equali(op->colTypes, top_union->colTypes))
		{
			/* Same UNION, so fold children into parent's subplan list */
			return nconc(recurse_union_children(op->larg, parse,
												top_union,
												refnames_tlist),
						 recurse_union_children(op->rarg, parse,
												top_union,
												refnames_tlist));
		}
	}

	/*
	 * Not same, so plan this child separately.
	 *
	 * Note we disallow any resjunk columns in child results.  This is
	 * necessary since the Append node that implements the union won't do
	 * any projection, and upper levels will get confused if some of our
	 * output tuples have junk and some don't.  This case only arises when
	 * we have an EXCEPT or INTERSECT as child, else there won't be
	 * resjunk anyway.
	 */
	return makeList1(recurse_set_operations(setOp, parse,
											top_union->colTypes, false,
											-1, refnames_tlist));
}

/*
 * Generate targetlist for a set-operation plan node
 *
 * colTypes: column datatypes for non-junk columns
 * flag: -1 if no flag column needed, 0 or 1 to create a const flag column
 * hack_constants: true to copy up constants (see comments in code)
 * input_tlist: targetlist of this node's input node
 * refnames_tlist: targetlist to take column names from
 */
static List *
generate_setop_tlist(List *colTypes, int flag,
					 bool hack_constants,
					 List *input_tlist,
					 List *refnames_tlist)
{
	List	   *tlist = NIL;
	int			resno = 1;
	List	   *i;
	Resdom	   *resdom;
	Node	   *expr;

	foreach(i, colTypes)
	{
		Oid			colType = (Oid) lfirsti(i);
		TargetEntry *inputtle = (TargetEntry *) lfirst(input_tlist);
		TargetEntry *reftle = (TargetEntry *) lfirst(refnames_tlist);
		int32		colTypmod;

		Assert(inputtle->resdom->resno == resno);
		Assert(reftle->resdom->resno == resno);
		Assert(!inputtle->resdom->resjunk);
		Assert(!reftle->resdom->resjunk);

		/*
		 * Generate columns referencing input columns and having
		 * appropriate data types and column names.  Insert datatype
		 * coercions where necessary.
		 *
		 * HACK: constants in the input's targetlist are copied up as-is
		 * rather than being referenced as subquery outputs.  This is
		 * mainly to ensure that when we try to coerce them to the output
		 * column's datatype, the right things happen for UNKNOWN
		 * constants.  But do this only at the first level of
		 * subquery-scan plans; we don't want phony constants appearing in
		 * the output tlists of upper-level nodes!
		 */
		if (hack_constants && inputtle->expr && IsA(inputtle->expr, Const))
			expr = inputtle->expr;
		else
			expr = (Node *) makeVar(0,
									inputtle->resdom->resno,
									inputtle->resdom->restype,
									inputtle->resdom->restypmod,
									0);
		if (inputtle->resdom->restype == colType)
		{
			/* no coercion needed, and believe the input typmod */
			colTypmod = inputtle->resdom->restypmod;
		}
		else
		{
			expr = coerce_to_common_type(NULL,
										 expr,
										 colType,
										 "UNION/INTERSECT/EXCEPT");
			colTypmod = -1;
		}
		resdom = makeResdom((AttrNumber) resno++,
							colType,
							colTypmod,
							pstrdup(reftle->resdom->resname),
							false);
		tlist = lappend(tlist, makeTargetEntry(resdom, expr));
		input_tlist = lnext(input_tlist);
		refnames_tlist = lnext(refnames_tlist);
	}

	if (flag >= 0)
	{
		/* Add a resjunk flag column */
		resdom = makeResdom((AttrNumber) resno++,
							INT4OID,
							-1,
							pstrdup("flag"),
							true);
		/* flag value is the given constant */
		expr = (Node *) makeConst(INT4OID,
								  sizeof(int4),
								  Int32GetDatum(flag),
								  false,
								  true,
								  false,
								  false);
		tlist = lappend(tlist, makeTargetEntry(resdom, expr));
	}

	return tlist;
}

/*
 * Generate targetlist for a set-operation Append node
 *
 * colTypes: column datatypes for non-junk columns
 * flag: true to create a flag column copied up from subplans
 * input_plans: list of sub-plans of the Append
 * refnames_tlist: targetlist to take column names from
 *
 * The entries in the Append's targetlist should always be simple Vars;
 * we just have to make sure they have the right datatypes and typmods.
 */
static List *
generate_append_tlist(List *colTypes, bool flag,
					  List *input_plans,
					  List *refnames_tlist)
{
	List	   *tlist = NIL;
	int			resno = 1;
	List	   *curColType;
	int			colindex;
	Resdom	   *resdom;
	Node	   *expr;
	List	   *planl;
	int32	   *colTypmods;

	/*
	 * First extract typmods to use.
	 *
	 * If the inputs all agree on type and typmod of a particular column,
	 * use that typmod; else use -1.
	 */
	colTypmods = (int32 *) palloc(length(colTypes) * sizeof(int32));

	foreach(planl, input_plans)
	{
		Plan   *subplan = (Plan *) lfirst(planl);
		List   *subtlist;

		curColType = colTypes;
		colindex = 0;
		foreach(subtlist, subplan->targetlist)
		{
			TargetEntry *subtle = (TargetEntry *) lfirst(subtlist);

			if (subtle->resdom->resjunk)
				continue;
			Assert(curColType != NIL);
			if (subtle->resdom->restype == (Oid) lfirsti(curColType))
			{
				/* If first subplan, copy the typmod; else compare */
				if (planl == input_plans)
					colTypmods[colindex] = subtle->resdom->restypmod;
				else if (subtle->resdom->restypmod != colTypmods[colindex])
					colTypmods[colindex] = -1;
			}
			else
			{
				/* types disagree, so force typmod to -1 */
				colTypmods[colindex] = -1;
			}
			curColType = lnext(curColType);
			colindex++;
		}
		Assert(curColType == NIL);
	}

	/*
	 * Now we can build the tlist for the Append.
	 */
	colindex = 0;
	foreach(curColType, colTypes)
	{
		Oid			colType = (Oid) lfirsti(curColType);
		int32		colTypmod = colTypmods[colindex++];
		TargetEntry *reftle = (TargetEntry *) lfirst(refnames_tlist);

		Assert(reftle->resdom->resno == resno);
		Assert(!reftle->resdom->resjunk);
		expr = (Node *) makeVar(0,
								resno,
								colType,
								colTypmod,
								0);
		resdom = makeResdom((AttrNumber) resno++,
							colType,
							colTypmod,
							pstrdup(reftle->resdom->resname),
							false);
		tlist = lappend(tlist, makeTargetEntry(resdom, expr));
		refnames_tlist = lnext(refnames_tlist);
	}

	if (flag)
	{
		/* Add a resjunk flag column */
		resdom = makeResdom((AttrNumber) resno++,
							INT4OID,
							-1,
							pstrdup("flag"),
							true);
		/* flag value is shown as copied up from subplan */
		expr = (Node *) makeVar(0,
								resdom->resno,
								INT4OID,
								-1,
								0);
		tlist = lappend(tlist, makeTargetEntry(resdom, expr));
	}

	pfree(colTypmods);

	return tlist;
}

/*
 * Does tlist have same datatypes as requested colTypes?
 *
 * Resjunk columns are ignored if junkOK is true; otherwise presence of
 * a resjunk column will always cause a 'false' result.
 */
static bool
tlist_same_datatypes(List *tlist, List *colTypes, bool junkOK)
{
	List	   *i;

	foreach(i, tlist)
	{
		TargetEntry *tle = (TargetEntry *) lfirst(i);

		if (tle->resdom->resjunk)
		{
			if (!junkOK)
				return false;
		}
		else
		{
			if (colTypes == NIL)
				return false;
			if (tle->resdom->restype != (Oid) lfirsti(colTypes))
				return false;
			colTypes = lnext(colTypes);
		}
	}
	if (colTypes != NIL)
		return false;
	return true;
}


/*
 * find_all_inheritors -
 *		Returns an integer list of relids including the given rel plus
 *		all relations that inherit from it, directly or indirectly.
 */
List *
find_all_inheritors(Oid parentrel)
{
	List	   *examined_relids = NIL;
	List	   *unexamined_relids = makeListi1(parentrel);

	/*
	 * While the queue of unexamined relids is nonempty, remove the first
	 * element, mark it examined, and find its direct descendants. NB:
	 * cannot use foreach(), since we modify the queue inside loop.
	 */
	while (unexamined_relids != NIL)
	{
		Oid			currentrel = lfirsti(unexamined_relids);
		List	   *currentchildren;

		unexamined_relids = lnext(unexamined_relids);
		examined_relids = lappendi(examined_relids, currentrel);
		currentchildren = find_inheritance_children(currentrel);

		/*
		 * Add to the queue only those children not already seen. This
		 * avoids making duplicate entries in case of multiple inheritance
		 * paths from the same parent.	(It'll also keep us from getting
		 * into an infinite loop, though theoretically there can't be any
		 * cycles in the inheritance graph anyway.)
		 */
		currentchildren = set_differencei(currentchildren, examined_relids);
		unexamined_relids = set_unioni(unexamined_relids, currentchildren);
	}

	return examined_relids;
}

/*
 * expand_inherted_rtentry
 *		Check whether a rangetable entry represents an inheritance set.
 *		If so, add entries for all the child tables to the query's
 *		rangetable, and return an integer list of RT indexes for the
 *		whole inheritance set (parent and children).
 *		If not, return NIL.
 *
 * When dup_parent is false, the initially given RT index is part of the
 * returned list (if any).	When dup_parent is true, the given RT index
 * is *not* in the returned list; a duplicate RTE will be made for the
 * parent table.
 *
 * A childless table is never considered to be an inheritance set; therefore
 * the result will never be a one-element list.  It'll be either empty
 * or have two or more elements.
 *
 * NOTE: after this routine executes, the specified RTE will always have
 * its inh flag cleared, whether or not there were any children.  This
 * ensures we won't expand the same RTE twice, which would otherwise occur
 * for the case of an inherited UPDATE/DELETE target relation.
 */
List *
expand_inherted_rtentry(Query *parse, Index rti, bool dup_parent)
{
	RangeTblEntry *rte = rt_fetch(rti, parse->rtable);
	Oid			parentOID = rte->relid;
	List	   *inhOIDs;
	List	   *inhRTIs;
	List	   *l;

	/* Does RT entry allow inheritance? */
	if (!rte->inh)
		return NIL;
	Assert(parentOID != InvalidOid && rte->subquery == NULL);
	/* Always clear the parent's inh flag, see above comments */
	rte->inh = false;
	/* Fast path for common case of childless table */
	if (!has_subclass(parentOID))
		return NIL;
	/* Scan for all members of inheritance set */
	inhOIDs = find_all_inheritors(parentOID);

	/*
	 * Check that there's at least one descendant, else treat as no-child
	 * case.  This could happen despite above has_subclass() check, if
	 * table once had a child but no longer does.
	 */
	if (lnext(inhOIDs) == NIL)
		return NIL;
	/* OK, it's an inheritance set; expand it */
	if (dup_parent)
		inhRTIs = NIL;
	else
		inhRTIs = makeListi1(rti);		/* include original RTE in result */

	foreach(l, inhOIDs)
	{
		Oid			childOID = (Oid) lfirsti(l);
		RangeTblEntry *childrte;
		Index		childRTindex;

		/* parent will be in the list too; skip it if not dup requested */
		if (childOID == parentOID && !dup_parent)
			continue;

		/*
		 * Build an RTE for the child, and attach to query's rangetable
		 * list. We copy most fields of the parent's RTE, but replace
		 * relation real name and OID.	Note that inh will be false at
		 * this point.
		 */
		childrte = copyObject(rte);
		childrte->relname = get_rel_name(childOID);
		childrte->relid = childOID;
		parse->rtable = lappend(parse->rtable, childrte);
		childRTindex = length(parse->rtable);

		inhRTIs = lappendi(inhRTIs, childRTindex);
	}

	return inhRTIs;
}

/*
 * adjust_inherited_attrs
 *	  Copy the specified query or expression and translate Vars referring
 *	  to old_rt_index to refer to new_rt_index.
 *
 * We also adjust varattno to match the new table by column name, rather
 * than column number.	This hack makes it possible for child tables to have
 * different column positions for the "same" attribute as a parent, which
 * helps ALTER TABLE ADD COLUMN.  Unfortunately this isn't nearly enough to
 * make it work transparently; there are other places where things fall down
 * if children and parents don't have the same column numbers for inherited
 * attributes.	It'd be better to rip this code out and fix ALTER TABLE...
 */
Node *
adjust_inherited_attrs(Node *node,
					   Index old_rt_index, Oid old_relid,
					   Index new_rt_index, Oid new_relid)
{
	adjust_inherited_attrs_context context;

	/* Handle simple case simply... */
	if (old_rt_index == new_rt_index)
	{
		Assert(old_relid == new_relid);
		return copyObject(node);
	}

	context.old_rt_index = old_rt_index;
	context.new_rt_index = new_rt_index;
	context.old_relid = old_relid;
	context.new_relid = new_relid;

	/*
	 * Must be prepared to start with a Query or a bare expression tree.
	 */
	if (node && IsA(node, Query))
	{
		Query	   *query = (Query *) node;
		Query	   *newnode;

		FLATCOPY(newnode, query, Query);
		if (newnode->resultRelation == old_rt_index)
			newnode->resultRelation = new_rt_index;
		query_tree_mutator(newnode, adjust_inherited_attrs_mutator,
						   (void *) &context, false);
		return (Node *) newnode;
	}
	else
		return adjust_inherited_attrs_mutator(node, &context);
}

static Node *
adjust_inherited_attrs_mutator(Node *node,
							   adjust_inherited_attrs_context *context)
{
	if (node == NULL)
		return NULL;
	if (IsA(node, Var))
	{
		Var		   *var = (Var *) copyObject(node);

		if (var->varlevelsup == 0 &&
			var->varno == context->old_rt_index)
		{
			var->varno = context->new_rt_index;
			if (var->varattno > 0)
				var->varattno = get_attnum(context->new_relid,
										   get_attname(context->old_relid,
													   var->varattno));
		}
		return (Node *) var;
	}
	if (IsA(node, RangeTblRef))
	{
		RangeTblRef *rtr = (RangeTblRef *) copyObject(node);

		if (rtr->rtindex == context->old_rt_index)
			rtr->rtindex = context->new_rt_index;
		return (Node *) rtr;
	}

	/*
	 * We have to process RestrictInfo nodes specially: we do NOT want to
	 * copy the original subclauseindices list, since the new rel may have
	 * different indices.  The list will be rebuilt during later planning.
	 */
	if (IsA(node, RestrictInfo))
	{
		RestrictInfo *oldinfo = (RestrictInfo *) node;
		RestrictInfo *newinfo = makeNode(RestrictInfo);

		/* Copy all flat-copiable fields */
		memcpy(newinfo, oldinfo, sizeof(RestrictInfo));

		newinfo->clause = (Expr *)
			adjust_inherited_attrs_mutator((Node *) oldinfo->clause, context);

		newinfo->subclauseindices = NIL;
		newinfo->eval_cost = -1;	/* reset these too */
		newinfo->this_selec = -1;
		newinfo->left_pathkey = NIL;	/* and these */
		newinfo->right_pathkey = NIL;
		newinfo->left_bucketsize = -1;
		newinfo->right_bucketsize = -1;

		return (Node *) newinfo;
	}

	/*
	 * NOTE: we do not need to recurse into sublinks, because they should
	 * already have been converted to subplans before we see them.
	 */

	/*
	 * BUT: although we don't need to recurse into subplans, we do need to
	 * make sure that they are copied, not just referenced as
	 * expression_tree_mutator will do by default.	Otherwise we'll have
	 * the same subplan node referenced from each arm of the inheritance
	 * APPEND plan, which will cause trouble in the executor.  This is a
	 * kluge that should go away when we redesign querytrees.
	 */
	if (is_subplan(node))
	{
		SubPlan    *subplan;

		/* Copy the node and process subplan args */
		node = expression_tree_mutator(node, adjust_inherited_attrs_mutator,
									   (void *) context);
		/* Make sure we have separate copies of subplan and its rtable */
		subplan = (SubPlan *) ((Expr *) node)->oper;
		subplan->plan = copyObject(subplan->plan);
		subplan->rtable = copyObject(subplan->rtable);
		return node;
	}

	return expression_tree_mutator(node, adjust_inherited_attrs_mutator,
								   (void *) context);
}
