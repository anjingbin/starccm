/*-------------------------------------------------------------------------
 *
 * allpaths.c
 *	  Routines to find possible search paths for processing a query
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/path/allpaths.c,v 1.2 2004/07/05 08:55:29 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#ifdef OPTIMIZER_DEBUG
#include "nodes/print.h"
#endif
#include "optimizer/clauses.h"
#include "optimizer/cost.h"
#include "optimizer/geqo.h"
#include "optimizer/pathnode.h"
#include "optimizer/paths.h"
#include "optimizer/plancat.h"
#include "optimizer/planner.h"
#include "optimizer/prep.h"
#include "parser/parsetree.h"
#include "rewrite/rewriteManip.h"


bool		enable_geqo = true;
int			geqo_rels = DEFAULT_GEQO_RELS;


static void set_base_rel_pathlists(Query *root);
static void set_plain_rel_pathlist(Query *root, RelOptInfo *rel,
					   RangeTblEntry *rte);
static void set_inherited_rel_pathlist(Query *root, RelOptInfo *rel,
						   Index rti, RangeTblEntry *rte,
						   List *inheritlist);
static void set_subquery_pathlist(Query *root, RelOptInfo *rel,
					  Index rti, RangeTblEntry *rte);
static RelOptInfo *make_one_rel_by_joins(Query *root, int levels_needed,
					  List *initial_rels);


/*
 * make_one_rel
 *	  Finds all possible access paths for executing a query, returning a
 *	  single rel that represents the join of all base rels in the query.
 */
RelOptInfo *
make_one_rel(Query *root)
{
	RelOptInfo *rel;

	/*
	 * Generate access paths for the base rels.
	 */
	set_base_rel_pathlists(root);

	/*
	 * Generate access paths for the entire join tree.
	 */
	Assert(root->jointree != NULL && IsA(root->jointree, FromExpr));

	rel = make_fromexpr_rel(root, root->jointree);

	/*
	 * The result should join all the query's base rels.
	 */
	Assert(length(rel->relids) == length(root->base_rel_list));

	return rel;
}

/*
 * set_base_rel_pathlists
 *	  Finds all paths available for scanning each base-relation entry.
 *	  Sequential scan and any available indices are considered.
 *	  Each useful path is attached to its relation's 'pathlist' field.
 */
static void
set_base_rel_pathlists(Query *root)
{
	List	   *rellist;

	foreach(rellist, root->base_rel_list)
	{
		RelOptInfo *rel = (RelOptInfo *) lfirst(rellist);
		Index		rti;
		RangeTblEntry *rte;
		List	   *inheritlist;

		Assert(length(rel->relids) == 1);		/* better be base rel */
		rti = lfirsti(rel->relids);
		rte = rt_fetch(rti, root->rtable);

		if (rel->issubquery)
		{
			/* Subquery --- generate a separate plan for it */
			set_subquery_pathlist(root, rel, rti, rte);
		}
		else if ((inheritlist = expand_inherted_rtentry(root, rti, true))
				 != NIL)
		{
			/* Relation is root of an inheritance tree, process specially */
			set_inherited_rel_pathlist(root, rel, rti, rte, inheritlist);
		}
		else
		{
			/* Plain relation */
			set_plain_rel_pathlist(root, rel, rte);
		}

#ifdef OPTIMIZER_DEBUG
		debug_print_rel(root, rel);
#endif
	}
}

/*
 * set_plain_rel_pathlist
 *	  Build access paths for a plain relation (no subquery, no inheritance)
 */
static void
set_plain_rel_pathlist(Query *root, RelOptInfo *rel, RangeTblEntry *rte)
{
	/* Mark rel with estimated output rows, width, etc */
	set_baserel_size_estimates(root, rel);

	/*
	 * Generate paths and add them to the rel's pathlist.
	 *
	 * Note: add_path() will discard any paths that are dominated by another
	 * available path, keeping only those paths that are superior along at
	 * least one dimension of cost or sortedness.
	 */

	/* Consider sequential scan */
	add_path(rel, create_seqscan_path(root, rel));

	/* Consider TID scans */
	create_tidscan_paths(root, rel);

	/* Consider index paths for both simple and OR index clauses */
	create_index_paths(root, rel);

	/* create_index_paths must be done before create_or_index_paths */
	create_or_index_paths(root, rel);

	/* Now find the cheapest of the paths for this rel */
	set_cheapest(rel);
}

/*
 * set_inherited_rel_pathlist
 *	  Build access paths for a inheritance tree rooted at rel
 *
 * inheritlist is a list of RT indexes of all tables in the inheritance tree,
 * including a duplicate of the parent itself.	Note we will not come here
 * unless there's at least one child in addition to the parent.
 *
 * NOTE: the passed-in rel and RTE will henceforth represent the appended
 * result of the whole inheritance tree.  The members of inheritlist represent
 * the individual tables --- in particular, the inheritlist member that is a
 * duplicate of the parent RTE represents the parent table alone.
 * We will generate plans to scan the individual tables that refer to
 * the inheritlist RTEs, whereas Vars elsewhere in the plan tree that
 * refer to the original RTE are taken to refer to the append output.
 * In particular, this means we have separate RelOptInfos for the parent
 * table and for the append output, which is a good thing because they're
 * not the same size.
 */
static void
set_inherited_rel_pathlist(Query *root, RelOptInfo *rel,
						   Index rti, RangeTblEntry *rte,
						   List *inheritlist)
{
	int			parentRTindex = rti;
	Oid			parentOID = rte->relid;
	List	   *subpaths = NIL;
	List	   *il;

	/*
	 * XXX for now, can't handle inherited expansion of FOR UPDATE; can we
	 * do better?
	 */
	if (intMember(parentRTindex, root->rowMarks))
		elog(ERROR, "SELECT FOR UPDATE is not supported for inherit queries");

	/*
	 * The executor will check the parent table's access permissions when
	 * it examines the parent's inheritlist entry.  There's no need to
	 * check twice, so turn off access check bits in the original RTE.
	 */
	rte->checkForRead = false;
	rte->checkForWrite = false;

	/*
	 * Initialize to compute size estimates for whole inheritance tree
	 */
	rel->rows = 0;
	rel->width = 0;

	/*
	 * Generate access paths for each table in the tree (parent AND
	 * children), and pick the cheapest path for each table.
	 */
	foreach(il, inheritlist)
	{
		int			childRTindex = lfirsti(il);
		RangeTblEntry *childrte;
		Oid			childOID;
		RelOptInfo *childrel;

		childrte = rt_fetch(childRTindex, root->rtable);
		childOID = childrte->relid;

		/*
		 * Make a RelOptInfo for the child so we can do planning.  Do NOT
		 * attach the RelOptInfo to the query's base_rel_list, however,
		 * since the child is not part of the main join tree.  Instead,
		 * the child RelOptInfo is added to other_rel_list.
		 */
		childrel = build_other_rel(root, childRTindex);

		/*
		 * Copy the parent's targetlist and restriction quals to the
		 * child, with attribute-number adjustment as needed.  We don't
		 * bother to copy the join quals, since we can't do any joining of
		 * the individual tables.
		 */
		childrel->targetlist = (List *)
			adjust_inherited_attrs((Node *) rel->targetlist,
								   parentRTindex,
								   parentOID,
								   childRTindex,
								   childOID);
		childrel->baserestrictinfo = (List *)
			adjust_inherited_attrs((Node *) rel->baserestrictinfo,
								   parentRTindex,
								   parentOID,
								   childRTindex,
								   childOID);
		childrel->baserestrictcost = rel->baserestrictcost;

		/*
		 * Now compute child access paths, and save the cheapest.
		 */
		set_plain_rel_pathlist(root, childrel, childrte);

		subpaths = lappend(subpaths, childrel->cheapest_total_path);

		/* Also update total size estimates */
		rel->rows += childrel->rows;
		if (childrel->width > rel->width)
			rel->width = childrel->width;
	}

	/*
	 * Finally, build Append path and install it as the only access path
	 * for the parent rel.
	 */
	add_path(rel, (Path *) create_append_path(rel, subpaths));

	/* Select cheapest path (pretty easy in this case...) */
	set_cheapest(rel);
}

/*
 * set_subquery_pathlist
 *		Build the (single) access path for a subquery RTE
 */
static void
set_subquery_pathlist(Query *root, RelOptInfo *rel,
					  Index rti, RangeTblEntry *rte)
{
	Query	   *subquery = rte->subquery;

	/*
	 * If there are any restriction clauses that have been attached to the
	 * subquery relation, consider pushing them down to become HAVING
	 * quals of the subquery itself.  (Not WHERE clauses, since they may
	 * refer to subquery outputs that are aggregate results.  But
	 * planner.c will transfer them into the subquery's WHERE if they do
	 * not.)  This transformation is useful because it may allow us to
	 * generate a better plan for the subquery than evaluating all the
	 * subquery output rows and then filtering them.
	 *
	 * There are several cases where we cannot push down clauses:
	 *
	 * 1. If the subquery contains set ops (UNION/INTERSECT/EXCEPT) we do not
	 * push down any qual clauses, since the planner doesn't support quals
	 * at the top level of a setop.  (With suitable analysis we could try
	 * to push the quals down into the component queries of the setop, but
	 * getting it right seems nontrivial.  Work on this later.)
	 *
	 * 2. If the subquery has a LIMIT clause or a DISTINCT ON clause, we must
	 * not push down any quals, since that could change the set of rows
	 * returned.  (Actually, we could push down quals into a DISTINCT ON
	 * subquery if they refer only to DISTINCT-ed output columns, but
	 * checking that seems more work than it's worth.  In any case, a
	 * plain DISTINCT is safe to push down past.)
	 *
	 * 3. If the subquery has any ITER nodes (ie, functions returning sets)
	 * in its target list, we do not push down any quals, since the quals
	 * might refer to those tlist items, which would mean we'd introduce
	 * functions-returning-sets into the subquery's WHERE/HAVING quals.
	 * (It'd be sufficient to not push down quals that refer to those
	 * particular tlist items, but that's much clumsier to check.)
	 *
	 * 4. We do not push down clauses that contain subselects, mainly because
	 * I'm not sure it will work correctly (the subplan hasn't yet
	 * transformed sublinks to subselects).
	 *
	 * Non-pushed-down clauses will get evaluated as qpquals of the
	 * SubqueryScan node.
	 *
	 * XXX Are there any cases where we want to make a policy decision not to
	 * push down, because it'd result in a worse plan?
	 */
	if (subquery->setOperations == NULL &&
		subquery->limitOffset == NULL &&
		subquery->limitCount == NULL &&
		!has_distinct_on_clause(subquery) &&
		!contain_iter_clause((Node *) subquery->targetList))
	{
		/* OK to consider pushing down individual quals */
		List	   *upperrestrictlist = NIL;
		List	   *lst;

		foreach(lst, rel->baserestrictinfo)
		{
			RestrictInfo *rinfo = (RestrictInfo *) lfirst(lst);
			Node	   *clause = (Node *) rinfo->clause;

			if (contain_subplans(clause))
			{
				/* Keep it in the upper query */
				upperrestrictlist = lappend(upperrestrictlist, rinfo);
			}
			else
			{
				/*
				 * We need to replace Vars in the clause (which must refer
				 * to outputs of the subquery) with copies of the
				 * subquery's targetlist expressions.  Note that at this
				 * point, any uplevel Vars in the clause should have been
				 * replaced with Params, so they need no work.
				 */
				clause = ResolveNew(clause, rti, 0,
									subquery->targetList,
									CMD_SELECT, 0);
				subquery->havingQual = make_and_qual(subquery->havingQual,
													 clause);

				/*
				 * We need not change the subquery's hasAggs or
				 * hasSublinks flags, since we can't be pushing down any
				 * aggregates that weren't there before, and we don't push
				 * down subselects at all.
				 */
			}
		}
		rel->baserestrictinfo = upperrestrictlist;
	}

	/* Generate the plan for the subquery */
	rel->subplan = subquery_planner(subquery,
									-1.0 /* default case */ );

	/* Copy number of output rows from subplan */
	rel->tuples = rel->subplan->plan_rows;

	/* Mark rel with estimated output rows, width, etc */
	set_baserel_size_estimates(root, rel);

	/* Generate appropriate path */
	add_path(rel, create_subqueryscan_path(rel));

	/* Select cheapest path (pretty easy in this case...) */
	set_cheapest(rel);
}

/*
 * make_fromexpr_rel
 *	  Build access paths for a FromExpr jointree node.
 */
RelOptInfo *
make_fromexpr_rel(Query *root, FromExpr *from)
{
	int			levels_needed;
	List	   *initial_rels = NIL;
	List	   *jt;

	/*
	 * Count the number of child jointree nodes.  This is the depth of the
	 * dynamic-programming algorithm we must employ to consider all ways
	 * of joining the child nodes.
	 */
	levels_needed = length(from->fromlist);

	if (levels_needed <= 0)
		return NULL;			/* nothing to do? */

	/*
	 * Construct a list of rels corresponding to the child jointree nodes.
	 * This may contain both base rels and rels constructed according to
	 * explicit JOIN directives.
	 */
	foreach(jt, from->fromlist)
	{
		Node	   *jtnode = (Node *) lfirst(jt);

		initial_rels = lappend(initial_rels,
							   make_jointree_rel(root, jtnode));
	}

	if (levels_needed == 1)
	{
		/*
		 * Single jointree node, so we're done.
		 */
		return (RelOptInfo *) lfirst(initial_rels);
	}
	else
	{
		/*
		 * Consider the different orders in which we could join the rels,
		 * using either GEQO or regular optimizer.
		 */
		if (enable_geqo && levels_needed >= geqo_rels)
			return geqo(root, levels_needed, initial_rels);
		else
			return make_one_rel_by_joins(root, levels_needed, initial_rels);
	}
}

/*
 * make_one_rel_by_joins
 *	  Find all possible joinpaths for a query by successively finding ways
 *	  to join component relations into join relations.
 *
 * 'levels_needed' is the number of iterations needed, ie, the number of
 *		independent jointree items in the query.  This is > 1.
 *
 * 'initial_rels' is a list of RelOptInfo nodes for each independent
 *		jointree item.	These are the components to be joined together.
 *
 * Returns the final level of join relations, i.e., the relation that is
 * the result of joining all the original relations together.
 */
static RelOptInfo *
make_one_rel_by_joins(Query *root, int levels_needed, List *initial_rels)
{
	List	  **joinitems;
	int			lev;
	RelOptInfo *rel;

	/*
	 * We employ a simple "dynamic programming" algorithm: we first find
	 * all ways to build joins of two jointree items, then all ways to
	 * build joins of three items (from two-item joins and single items),
	 * then four-item joins, and so on until we have considered all ways
	 * to join all the items into one rel.
	 *
	 * joinitems[j] is a list of all the j-item rels.  Initially we set
	 * joinitems[1] to represent all the single-jointree-item relations.
	 */
	joinitems = (List **) palloc((levels_needed + 1) * sizeof(List *));
	MemSet(joinitems, 0, (levels_needed + 1) * sizeof(List *));

	joinitems[1] = initial_rels;

	for (lev = 2; lev <= levels_needed; lev++)
	{
		List	   *x;

		/*
		 * Determine all possible pairs of relations to be joined at this
		 * level, and build paths for making each one from every available
		 * pair of lower-level relations.
		 */
		joinitems[lev] = make_rels_by_joins(root, lev, joinitems);

		/*
		 * Do cleanup work on each just-processed rel.
		 */
		foreach(x, joinitems[lev])
		{
			rel = (RelOptInfo *) lfirst(x);

#ifdef NOT_USED

			/*
			 * * for each expensive predicate in each path in each
			 * distinct rel, * consider doing pullup  -- JMH
			 */
			if (XfuncMode != XFUNC_NOPULL && XfuncMode != XFUNC_OFF)
				xfunc_trypullup(rel);
#endif

			/* Find and save the cheapest paths for this rel */
			set_cheapest(rel);

#ifdef OPTIMIZER_DEBUG
			debug_print_rel(root, rel);
#endif
		}
	}

	/*
	 * We should have a single rel at the final level.
	 */
	Assert(length(joinitems[levels_needed]) == 1);

	rel = (RelOptInfo *) lfirst(joinitems[levels_needed]);

	return rel;
}

/*****************************************************************************
 *
 *****************************************************************************/

#ifdef OPTIMIZER_DEBUG

static void
print_relids(Relids relids)
{
	List	   *l;

	foreach(l, relids)
	{
		printf("%d", lfirsti(l));
		if (lnext(l))
			printf(" ");
	}
}

static void
print_restrictclauses(Query *root, List *clauses)
{
	List	   *l;

	foreach(l, clauses)
	{
		RestrictInfo *c = lfirst(l);

		print_expr((Node *) c->clause, root->rtable);
		if (lnext(l))
			printf(", ");
	}
}

static void
print_path(Query *root, Path *path, int indent)
{
	const char *ptype;
	bool		join;
	int			i;

	switch (nodeTag(path))
	{
		case T_Path:
			ptype = "SeqScan";
			join = false;
			break;
		case T_IndexPath:
			ptype = "IdxScan";
			join = false;
			break;
		case T_TidPath:
			ptype = "TidScan";
			join = false;
			break;
		case T_NestPath:
			ptype = "Nestloop";
			join = true;
			break;
		case T_MergePath:
			ptype = "MergeJoin";
			join = true;
			break;
		case T_HashPath:
			ptype = "HashJoin";
			join = true;
			break;
		default:
			ptype = "???Path";
			join = false;
			break;
	}

	for (i = 0; i < indent; i++)
		printf("\t");
	printf("%s(", ptype);
	print_relids(path->parent->relids);
	printf(") rows=%.0f cost=%.2f..%.2f\n",
		   path->parent->rows, path->startup_cost, path->total_cost);

	if (path->pathkeys)
	{
		for (i = 0; i < indent; i++)
			printf("\t");
		printf("  pathkeys: ");
		print_pathkeys(path->pathkeys, root->rtable);
	}

	if (join)
	{
		JoinPath   *jp = (JoinPath *) path;

		for (i = 0; i < indent; i++)
			printf("\t");
		printf("  clauses: ");
		print_restrictclauses(root, jp->joinrestrictinfo);
		printf("\n");

		if (nodeTag(path) == T_MergePath)
		{
			MergePath  *mp = (MergePath *) path;

			if (mp->outersortkeys || mp->innersortkeys)
			{
				for (i = 0; i < indent; i++)
					printf("\t");
				printf("  sortouter=%d sortinner=%d\n",
					   ((mp->outersortkeys) ? 1 : 0),
					   ((mp->innersortkeys) ? 1 : 0));
			}
		}

		print_path(root, jp->outerjoinpath, indent + 1);
		print_path(root, jp->innerjoinpath, indent + 1);
	}
}

void
debug_print_rel(Query *root, RelOptInfo *rel)
{
	List	   *l;

	printf("RELOPTINFO (");
	print_relids(rel->relids);
	printf("): rows=%.0f width=%d\n", rel->rows, rel->width);

	if (rel->baserestrictinfo)
	{
		printf("\tbaserestrictinfo: ");
		print_restrictclauses(root, rel->baserestrictinfo);
		printf("\n");
	}

	foreach(l, rel->joininfo)
	{
		JoinInfo   *j = (JoinInfo *) lfirst(l);

		printf("\tjoininfo (");
		print_relids(j->unjoined_relids);
		printf("): ");
		print_restrictclauses(root, j->jinfo_restrictinfo);
		printf("\n");
	}

	printf("\tpath list:\n");
	foreach(l, rel->pathlist)
		print_path(root, lfirst(l), 1);
	printf("\n\tcheapest startup path:\n");
	print_path(root, rel->cheapest_startup_path, 1);
	printf("\n\tcheapest total path:\n");
	print_path(root, rel->cheapest_total_path, 1);
	printf("\n");
	fflush(stdout);
}

#endif   /* OPTIMIZER_DEBUG */
