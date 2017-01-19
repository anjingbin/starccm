/*-------------------------------------------------------------------------
 *
 * pathnode.c
 *	  Routines to manipulate pathlists and create path nodes
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/util/pathnode.c,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <math.h>

#include "nodes/plannodes.h"
#include "optimizer/cost.h"
#include "optimizer/pathnode.h"
#include "optimizer/paths.h"
#include "optimizer/restrictinfo.h"


/*****************************************************************************
 *		MISC. PATH UTILITIES
 *****************************************************************************/

/*
 * compare_path_costs
 *	  Return -1, 0, or +1 according as path1 is cheaper, the same cost,
 *	  or more expensive than path2 for the specified criterion.
 */
int
compare_path_costs(Path *path1, Path *path2, CostSelector criterion)
{
	if (criterion == STARTUP_COST)
	{
		if (path1->startup_cost < path2->startup_cost)
			return -1;
		if (path1->startup_cost > path2->startup_cost)
			return +1;

		/*
		 * If paths have the same startup cost (not at all unlikely),
		 * order them by total cost.
		 */
		if (path1->total_cost < path2->total_cost)
			return -1;
		if (path1->total_cost > path2->total_cost)
			return +1;
	}
	else
	{
		if (path1->total_cost < path2->total_cost)
			return -1;
		if (path1->total_cost > path2->total_cost)
			return +1;

		/*
		 * If paths have the same total cost, order them by startup cost.
		 */
		if (path1->startup_cost < path2->startup_cost)
			return -1;
		if (path1->startup_cost > path2->startup_cost)
			return +1;
	}
	return 0;
}

/*
 * compare_path_fractional_costs
 *	  Return -1, 0, or +1 according as path1 is cheaper, the same cost,
 *	  or more expensive than path2 for fetching the specified fraction
 *	  of the total tuples.
 *
 * If fraction is <= 0 or > 1, we interpret it as 1, ie, we select the
 * path with the cheaper total_cost.
 */
int
compare_fractional_path_costs(Path *path1, Path *path2,
							  double fraction)
{
	Cost		cost1,
				cost2;

	if (fraction <= 0.0 || fraction >= 1.0)
		return compare_path_costs(path1, path2, TOTAL_COST);
	cost1 = path1->startup_cost +
		fraction * (path1->total_cost - path1->startup_cost);
	cost2 = path2->startup_cost +
		fraction * (path2->total_cost - path2->startup_cost);
	if (cost1 < cost2)
		return -1;
	if (cost1 > cost2)
		return +1;
	return 0;
}

/*
 * set_cheapest
 *	  Find the minimum-cost paths from among a relation's paths,
 *	  and save them in the rel's cheapest-path fields.
 *
 * This is normally called only after we've finished constructing the path
 * list for the rel node.
 *
 * If we find two paths of identical costs, try to keep the better-sorted one.
 * The paths might have unrelated sort orderings, in which case we can only
 * guess which might be better to keep, but if one is superior then we
 * definitely should keep it.
 */
void
set_cheapest(RelOptInfo *parent_rel)
{
	List	   *pathlist = parent_rel->pathlist;
	List	   *p;
	Path	   *cheapest_startup_path;
	Path	   *cheapest_total_path;

	Assert(IsA(parent_rel, RelOptInfo));

	if (pathlist == NIL)
		elog(ERROR, "Unable to devise a query plan for the given query");

	cheapest_startup_path = cheapest_total_path = (Path *) lfirst(pathlist);

	foreach(p, lnext(pathlist))
	{
		Path	   *path = (Path *) lfirst(p);
		int			cmp;

		cmp = compare_path_costs(cheapest_startup_path, path, STARTUP_COST);
		if (cmp > 0 ||
			(cmp == 0 &&
			 compare_pathkeys(cheapest_startup_path->pathkeys,
							  path->pathkeys) == PATHKEYS_BETTER2))
			cheapest_startup_path = path;

		cmp = compare_path_costs(cheapest_total_path, path, TOTAL_COST);
		if (cmp > 0 ||
			(cmp == 0 &&
			 compare_pathkeys(cheapest_total_path->pathkeys,
							  path->pathkeys) == PATHKEYS_BETTER2))
			cheapest_total_path = path;
	}

	parent_rel->cheapest_startup_path = cheapest_startup_path;
	parent_rel->cheapest_total_path = cheapest_total_path;
}

/*
 * add_path
 *	  Consider a potential implementation path for the specified parent rel,
 *	  and add it to the rel's pathlist if it is worthy of consideration.
 *	  A path is worthy if it has either a better sort order (better pathkeys)
 *	  or cheaper cost (on either dimension) than any of the existing old paths.
 *
 *	  Unless parent_rel->pruneable is false, we also remove from the rel's
 *	  pathlist any old paths that are dominated by new_path --- that is,
 *	  new_path is both cheaper and at least as well ordered.
 *
 *	  The pathlist is kept sorted by TOTAL_COST metric, with cheaper paths
 *	  at the front.  No code depends on that for correctness; it's simply
 *	  a speed hack within this routine.  Doing it that way makes it more
 *	  likely that we will reject an inferior path after a few comparisons,
 *	  rather than many comparisons.
 *
 *	  NOTE: discarded Path objects are immediately pfree'd to reduce planner
 *	  memory consumption.  We dare not try to free the substructure of a Path,
 *	  since much of it may be shared with other Paths or the query tree itself;
 *	  but just recycling discarded Path nodes is a very useful savings in
 *	  a large join tree.  We can recycle the List nodes of pathlist, too.
 *
 * 'parent_rel' is the relation entry to which the path corresponds.
 * 'new_path' is a potential path for parent_rel.
 *
 * Returns nothing, but modifies parent_rel->pathlist.
 */
void
add_path(RelOptInfo *parent_rel, Path *new_path)
{
	bool		accept_new = true;		/* unless we find a superior old
										 * path */
	List	   *insert_after = NIL;		/* where to insert new item */
	List	   *p1_prev = NIL;
	List	   *p1;

	/*
	 * Loop to check proposed new path against old paths.  Note it is
	 * possible for more than one old path to be tossed out because
	 * new_path dominates it.
	 */
	p1 = parent_rel->pathlist;	/* cannot use foreach here */
	while (p1 != NIL)
	{
		Path	   *old_path = (Path *) lfirst(p1);
		bool		remove_old = false; /* unless new proves superior */
		int			costcmp;

		costcmp = compare_path_costs(new_path, old_path, TOTAL_COST);

		/*
		 * If the two paths compare differently for startup and total
		 * cost, then we want to keep both, and we can skip the (much
		 * slower) comparison of pathkeys.	If they compare the same,
		 * proceed with the pathkeys comparison.  Note: this test relies
		 * on the fact that compare_path_costs will only return 0 if both
		 * costs are equal (and, therefore, there's no need to call it
		 * twice in that case).
		 */
		if (costcmp == 0 ||
			costcmp == compare_path_costs(new_path, old_path,
										  STARTUP_COST))
		{
			switch (compare_pathkeys(new_path->pathkeys, old_path->pathkeys))
			{
				case PATHKEYS_EQUAL:
					if (costcmp < 0)
						remove_old = true;		/* new dominates old */
					else
						accept_new = false;		/* old equals or dominates
												 * new */
					break;
				case PATHKEYS_BETTER1:
					if (costcmp <= 0)
						remove_old = true;		/* new dominates old */
					break;
				case PATHKEYS_BETTER2:
					if (costcmp >= 0)
						accept_new = false;		/* old dominates new */
					break;
				case PATHKEYS_DIFFERENT:
					/* keep both paths, since they have different ordering */
					break;
			}
		}

		/*
		 * Remove current element from pathlist if dominated by new,
		 * unless xfunc told us not to remove any paths.
		 */
		if (remove_old && parent_rel->pruneable)
		{
			List	   *p1_next = lnext(p1);

			if (p1_prev)
				lnext(p1_prev) = p1_next;
			else
				parent_rel->pathlist = p1_next;
			pfree(old_path);
			pfree(p1);			/* this is why we can't use foreach */
			p1 = p1_next;
		}
		else
		{
			/* new belongs after this old path if it has cost >= old's */
			if (costcmp >= 0)
				insert_after = p1;
			p1_prev = p1;
			p1 = lnext(p1);
		}

		/*
		 * If we found an old path that dominates new_path, we can quit
		 * scanning the pathlist; we will not add new_path, and we assume
		 * new_path cannot dominate any other elements of the pathlist.
		 */
		if (!accept_new)
			break;
	}

	if (accept_new)
	{
		/* Accept the new path: insert it at proper place in pathlist */
		if (insert_after)
			lnext(insert_after) = lcons(new_path, lnext(insert_after));
		else
			parent_rel->pathlist = lcons(new_path, parent_rel->pathlist);
	}
	else
	{
		/* Reject and recycle the new path */
		pfree(new_path);
	}
}


/*****************************************************************************
 *		PATH NODE CREATION ROUTINES
 *****************************************************************************/

/*
 * create_seqscan_path
 *	  Creates a path corresponding to a sequential scan, returning the
 *	  pathnode.
 */
Path *
create_seqscan_path(Query *root, RelOptInfo *rel)
{
	Path	   *pathnode = makeNode(Path);

	pathnode->pathtype = T_SeqScan;
	pathnode->parent = rel;
	pathnode->pathkeys = NIL;	/* seqscan has unordered result */

	cost_seqscan(pathnode, root, rel);

	return pathnode;
}

/*
 * create_index_path
 *	  Creates a path node for an index scan.
 *
 * 'rel' is the parent rel
 * 'index' is an index on 'rel'
 * 'restriction_clauses' is a list of RestrictInfo nodes
 *			to be used as index qual conditions in the scan.
 * 'pathkeys' describes the ordering of the path.
 * 'indexscandir' is ForwardScanDirection or BackwardScanDirection
 *			for an ordered index, or NoMovementScanDirection for
 *			an unordered index.
 *
 * Returns the new path node.
 */
IndexPath *
create_index_path(Query *root,
				  RelOptInfo *rel,
				  IndexOptInfo *index,
				  List *restriction_clauses,
				  List *pathkeys,
				  ScanDirection indexscandir)
{
	IndexPath  *pathnode = makeNode(IndexPath);
	List	   *indexquals;

	pathnode->path.pathtype = T_IndexScan;
	pathnode->path.parent = rel;
	pathnode->path.pathkeys = pathkeys;

	indexquals = get_actual_clauses(restriction_clauses);
	/* expand special operators to indexquals the executor can handle */
	indexquals = expand_indexqual_conditions(indexquals);

	/*
	 * We are making a pathnode for a single-scan indexscan; therefore,
	 * both indexinfo and indexqual should be single-element lists.
	 */
	pathnode->indexinfo = makeList1(index);
	pathnode->indexqual = makeList1(indexquals);

	pathnode->indexscandir = indexscandir;

	/*
	 * This routine is only used to generate "standalone" indexpaths, not
	 * nestloop inner indexpaths.  So joinrelids is always NIL and the
	 * number of rows is the same as the parent rel's estimate.
	 */
	pathnode->joinrelids = NIL; /* no join clauses here */
	pathnode->alljoinquals = false;
	pathnode->rows = rel->rows;

	/*
	 * Not sure if this is necessary, but it should help if the statistics
	 * are too far off
	 */
	if (index->indpred && index->tuples < pathnode->rows)
		pathnode->rows = index->tuples;

	cost_index(&pathnode->path, root, rel, index, indexquals, false);

	return pathnode;
}

/*
 * create_tidscan_path
 *	  Creates a path corresponding to a tid_direct scan, returning the
 *	  pathnode.
 */
TidPath *
create_tidscan_path(Query *root, RelOptInfo *rel, List *tideval)
{
	TidPath    *pathnode = makeNode(TidPath);

	pathnode->path.pathtype = T_TidScan;
	pathnode->path.parent = rel;
	pathnode->path.pathkeys = NIL;
	pathnode->tideval = copyObject(tideval);	/* is copy really
												 * necessary? */
	pathnode->unjoined_relids = NIL;

	cost_tidscan(&pathnode->path, root, rel, tideval);

	/*
	 * divide selectivity for each clause to get an equal selectivity as
	 * IndexScan does OK ?
	 */

	return pathnode;
}

/*
 * create_append_path
 *	  Creates a path corresponding to an Append plan, returning the
 *	  pathnode.
 *
 */
AppendPath *
create_append_path(RelOptInfo *rel, List *subpaths)
{
	AppendPath *pathnode = makeNode(AppendPath);
	List	   *l;

	pathnode->path.pathtype = T_Append;
	pathnode->path.parent = rel;
	pathnode->path.pathkeys = NIL;		/* result is always considered
										 * unsorted */
	pathnode->subpaths = subpaths;

	pathnode->path.startup_cost = 0;
	pathnode->path.total_cost = 0;
	foreach(l, subpaths)
	{
		Path	   *subpath = (Path *) lfirst(l);

		if (l == subpaths)		/* first node? */
			pathnode->path.startup_cost = subpath->startup_cost;
		pathnode->path.total_cost += subpath->total_cost;
	}

	return pathnode;
}

/*
 * create_subqueryscan_path
 *	  Creates a path corresponding to a sequential scan of a subquery,
 *	  returning the pathnode.
 */
Path *
create_subqueryscan_path(RelOptInfo *rel)
{
	Path	   *pathnode = makeNode(Path);

	pathnode->pathtype = T_SubqueryScan;
	pathnode->parent = rel;
	pathnode->pathkeys = NIL;	/* for now, assume unordered result */

	/* just copy the subplan's cost estimates */
	pathnode->startup_cost = rel->subplan->startup_cost;
	pathnode->total_cost = rel->subplan->total_cost;

	return pathnode;
}

/*
 * create_nestloop_path
 *	  Creates a pathnode corresponding to a nestloop join between two
 *	  relations.
 *
 * 'joinrel' is the join relation.
 * 'jointype' is the type of join required
 * 'outer_path' is the outer path
 * 'inner_path' is the inner path
 * 'restrict_clauses' are the RestrictInfo nodes to apply at the join
 * 'pathkeys' are the path keys of the new join path
 *
 * Returns the resulting path node.
 */
NestPath *
create_nestloop_path(Query *root,
					 RelOptInfo *joinrel,
					 JoinType jointype,
					 Path *outer_path,
					 Path *inner_path,
					 List *restrict_clauses,
					 List *pathkeys)
{
	NestPath   *pathnode = makeNode(NestPath);

	pathnode->path.pathtype = T_NestLoop;
	pathnode->path.parent = joinrel;
	pathnode->jointype = jointype;
	pathnode->outerjoinpath = outer_path;
	pathnode->innerjoinpath = inner_path;
	pathnode->joinrestrictinfo = restrict_clauses;
	pathnode->path.pathkeys = pathkeys;

	cost_nestloop(&pathnode->path, root, outer_path, inner_path,
				  restrict_clauses);

	return pathnode;
}

/*
 * create_mergejoin_path
 *	  Creates a pathnode corresponding to a mergejoin join between
 *	  two relations
 *
 * 'joinrel' is the join relation
 * 'jointype' is the type of join required
 * 'outer_path' is the outer path
 * 'inner_path' is the inner path
 * 'restrict_clauses' are the RestrictInfo nodes to apply at the join
 * 'pathkeys' are the path keys of the new join path
 * 'mergeclauses' are the RestrictInfo nodes to use as merge clauses
 *		(this should be a subset of the restrict_clauses list)
 * 'outersortkeys' are the sort varkeys for the outer relation
 * 'innersortkeys' are the sort varkeys for the inner relation
 */
MergePath *
create_mergejoin_path(Query *root,
					  RelOptInfo *joinrel,
					  JoinType jointype,
					  Path *outer_path,
					  Path *inner_path,
					  List *restrict_clauses,
					  List *pathkeys,
					  List *mergeclauses,
					  List *outersortkeys,
					  List *innersortkeys)
{
	MergePath  *pathnode = makeNode(MergePath);

	/*
	 * If the given paths are already well enough ordered, we can skip
	 * doing an explicit sort.
	 */
	if (outersortkeys &&
		pathkeys_contained_in(outersortkeys, outer_path->pathkeys))
		outersortkeys = NIL;
	if (innersortkeys &&
		pathkeys_contained_in(innersortkeys, inner_path->pathkeys))
		innersortkeys = NIL;

	pathnode->jpath.path.pathtype = T_MergeJoin;
	pathnode->jpath.path.parent = joinrel;
	pathnode->jpath.jointype = jointype;
	pathnode->jpath.outerjoinpath = outer_path;
	pathnode->jpath.innerjoinpath = inner_path;
	pathnode->jpath.joinrestrictinfo = restrict_clauses;
	pathnode->jpath.path.pathkeys = pathkeys;
	pathnode->path_mergeclauses = mergeclauses;
	pathnode->outersortkeys = outersortkeys;
	pathnode->innersortkeys = innersortkeys;

	cost_mergejoin(&pathnode->jpath.path,
				   root,
				   outer_path,
				   inner_path,
				   restrict_clauses,
				   mergeclauses,
				   outersortkeys,
				   innersortkeys);

	return pathnode;
}

/*
 * create_hashjoin_path
 *	  Creates a pathnode corresponding to a hash join between two relations.
 *
 * 'joinrel' is the join relation
 * 'jointype' is the type of join required
 * 'outer_path' is the cheapest outer path
 * 'inner_path' is the cheapest inner path
 * 'restrict_clauses' are the RestrictInfo nodes to apply at the join
 * 'hashclauses' is a list of the hash join clause (always a 1-element list)
 *		(this should be a subset of the restrict_clauses list)
 */
HashPath *
create_hashjoin_path(Query *root,
					 RelOptInfo *joinrel,
					 JoinType jointype,
					 Path *outer_path,
					 Path *inner_path,
					 List *restrict_clauses,
					 List *hashclauses)
{
	HashPath   *pathnode = makeNode(HashPath);

	pathnode->jpath.path.pathtype = T_HashJoin;
	pathnode->jpath.path.parent = joinrel;
	pathnode->jpath.jointype = jointype;
	pathnode->jpath.outerjoinpath = outer_path;
	pathnode->jpath.innerjoinpath = inner_path;
	pathnode->jpath.joinrestrictinfo = restrict_clauses;
	/* A hashjoin never has pathkeys, since its ordering is unpredictable */
	pathnode->jpath.path.pathkeys = NIL;
	pathnode->path_hashclauses = hashclauses;

	cost_hashjoin(&pathnode->jpath.path,
				  root,
				  outer_path,
				  inner_path,
				  restrict_clauses,
				  hashclauses);

	return pathnode;
}
