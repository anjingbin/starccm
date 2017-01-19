/*-------------------------------------------------------------------------
 *
 * joinrels.c
 *	  Routines to determine which relations should be joined
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/path/joinrels.c,v 1.2 2004/07/05 08:55:29 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "optimizer/pathnode.h"
#include "optimizer/paths.h"


static RelOptInfo *make_join_rel(Query *root, RelOptInfo *rel1,
			  RelOptInfo *rel2, JoinType jointype);


/*
 * make_rels_by_joins
 *	  Consider ways to produce join relations containing exactly 'level'
 *	  jointree items.  (This is one step of the dynamic-programming method
 *	  embodied in make_one_rel_by_joins.)  Join rel nodes for each feasible
 *	  combination of lower-level rels are created and returned in a list.
 *	  Implementation paths are created for each such joinrel, too.
 *
 * level: level of rels we want to make this time.
 * joinrels[j], 1 <= j < level, is a list of rels containing j items.
 */
List *
make_rels_by_joins(Query *root, int level, List **joinrels)
{
	List	   *result_rels = NIL;
	List	   *new_rels;
	List	   *nr;
	List	   *r;
	int			k;

	/*
	 * First, consider left-sided and right-sided plans, in which rels of
	 * exactly level-1 member relations are joined against initial
	 * relations. We prefer to join using join clauses, but if we find a
	 * rel of level-1 members that has no join clauses, we will generate
	 * Cartesian-product joins against all initial rels not already
	 * contained in it.
	 *
	 * In the first pass (level == 2), we try to join each initial rel to
	 * each initial rel that appears later in joinrels[1].	(The
	 * mirror-image joins are handled automatically by make_join_rel.)	In
	 * later passes, we try to join rels of size level-1 from
	 * joinrels[level-1] to each initial rel in joinrels[1].
	 */
	foreach(r, joinrels[level - 1])
	{
		RelOptInfo *old_rel = (RelOptInfo *) lfirst(r);
		List	   *other_rels;

		if (level == 2)
			other_rels = lnext(r);		/* only consider remaining initial
										 * rels */
		else
			other_rels = joinrels[1];	/* consider all initial rels */

		if (old_rel->joininfo != NIL)
		{
			/*
			 * Note that if all available join clauses for this rel
			 * require more than one other rel, we will fail to make any
			 * joins against it here.  That's OK; it'll be considered by
			 * "bushy plan" join code in a higher-level pass where we have
			 * those other rels collected into a join rel.	See also the
			 * last-ditch case below.
			 */
			new_rels = make_rels_by_clause_joins(root,
												 old_rel,
												 other_rels);
		}
		else
		{
			/*
			 * Oops, we have a relation that is not joined to any other
			 * relation.  Cartesian product time.
			 */
			new_rels = make_rels_by_clauseless_joins(root,
													 old_rel,
													 other_rels);
		}

		/*
		 * At levels above 2 we will generate the same joined relation in
		 * multiple ways --- for example (a join b) join c is the same
		 * RelOptInfo as (b join c) join a, though the second case will
		 * add a different set of Paths to it.	To avoid making extra work
		 * for subsequent passes, do not enter the same RelOptInfo into
		 * our output list multiple times.
		 */
		foreach(nr, new_rels)
		{
			RelOptInfo *jrel = (RelOptInfo *) lfirst(nr);

			if (!ptrMember(jrel, result_rels))
				result_rels = lcons(jrel, result_rels);
		}
	}

	/*
	 * Now, consider "bushy plans" in which relations of k initial rels
	 * are joined to relations of level-k initial rels, for 2 <= k <=
	 * level-2.
	 *
	 * We only consider bushy-plan joins for pairs of rels where there is a
	 * suitable join clause, in order to avoid unreasonable growth of
	 * planning time.
	 */
	for (k = 2;; k++)
	{
		int			other_level = level - k;

		/*
		 * Since make_join_rel(x, y) handles both x,y and y,x cases, we
		 * only need to go as far as the halfway point.
		 */
		if (k > other_level)
			break;

		foreach(r, joinrels[k])
		{
			RelOptInfo *old_rel = (RelOptInfo *) lfirst(r);
			List	   *other_rels;
			List	   *r2;

			if (old_rel->joininfo == NIL)
				continue;		/* we ignore clauseless joins here */

			if (k == other_level)
				other_rels = lnext(r);	/* only consider remaining rels */
			else
				other_rels = joinrels[other_level];

			foreach(r2, other_rels)
			{
				RelOptInfo *new_rel = (RelOptInfo *) lfirst(r2);

				if (nonoverlap_setsi(old_rel->relids, new_rel->relids))
				{
					List	   *i;

					/*
					 * OK, we can build a rel of the right level from this
					 * pair of rels.  Do so if there is at least one
					 * usable join clause.
					 */
					foreach(i, old_rel->joininfo)
					{
						JoinInfo   *joininfo = (JoinInfo *) lfirst(i);

						if (is_subseti(joininfo->unjoined_relids,
									   new_rel->relids))
						{
							RelOptInfo *jrel;

							jrel = make_join_rel(root, old_rel, new_rel,
												 JOIN_INNER);
							/* Avoid making duplicate entries ... */
							if (!ptrMember(jrel, result_rels))
								result_rels = lcons(jrel, result_rels);
							break;		/* need not consider more
										 * joininfos */
						}
					}
				}
			}
		}
	}

	/*
	 * Last-ditch effort: if we failed to find any usable joins so far,
	 * force a set of cartesian-product joins to be generated.	This
	 * handles the special case where all the available rels have join
	 * clauses but we cannot use any of the joins yet.	An example is
	 *
	 * SELECT * FROM a,b,c WHERE (a.f1 + b.f2 + c.f3) = 0;
	 *
	 * The join clause will be usable at level 3, but at level 2 we have no
	 * choice but to make cartesian joins.	We consider only left-sided
	 * and right-sided cartesian joins in this case (no bushy).
	 */
	if (result_rels == NIL)
	{
		/*
		 * This loop is just like the first one, except we always call
		 * make_rels_by_clauseless_joins().
		 */
		foreach(r, joinrels[level - 1])
		{
			RelOptInfo *old_rel = (RelOptInfo *) lfirst(r);
			List	   *other_rels;

			if (level == 2)
				other_rels = lnext(r);	/* only consider remaining initial
										 * rels */
			else
				other_rels = joinrels[1];		/* consider all initial
												 * rels */

			new_rels = make_rels_by_clauseless_joins(root,
													 old_rel,
													 other_rels);

			foreach(nr, new_rels)
			{
				RelOptInfo *jrel = (RelOptInfo *) lfirst(nr);

				if (!ptrMember(jrel, result_rels))
					result_rels = lcons(jrel, result_rels);
			}
		}

		if (result_rels == NIL)
			elog(ERROR, "make_rels_by_joins: failed to build any %d-way joins",
				 level);
	}

	return result_rels;
}

/*
 * make_rels_by_clause_joins
 *	  Build joins between the given relation 'old_rel' and other relations
 *	  that are mentioned within old_rel's joininfo nodes (i.e., relations
 *	  that participate in join clauses that 'old_rel' also participates in).
 *	  The join rel nodes are returned in a list.
 *
 * 'old_rel' is the relation entry for the relation to be joined
 * 'other_rels': other rels to be considered for joining
 *
 * Currently, this is only used with initial rels in other_rels, but it
 * will work for joining to joinrels too, if the caller ensures there is no
 * membership overlap between old_rel and the rels in other_rels.  (We need
 * no extra test for overlap for initial rels, since the is_subset test can
 * only succeed when other_rel is not already part of old_rel.)
 */
List *
make_rels_by_clause_joins(Query *root,
						  RelOptInfo *old_rel,
						  List *other_rels)
{
	List	   *result = NIL;
	List	   *i,
			   *j;

	foreach(i, old_rel->joininfo)
	{
		JoinInfo   *joininfo = (JoinInfo *) lfirst(i);
		Relids		unjoined_relids = joininfo->unjoined_relids;

		foreach(j, other_rels)
		{
			RelOptInfo *other_rel = (RelOptInfo *) lfirst(j);

			if (is_subseti(unjoined_relids, other_rel->relids))
			{
				RelOptInfo *jrel;

				jrel = make_join_rel(root, old_rel, other_rel, JOIN_INNER);

				/*
				 * Avoid entering same joinrel into our output list more
				 * than once.  (make_rels_by_joins doesn't really care,
				 * but GEQO does.)
				 */
				if (!ptrMember(jrel, result))
					result = lcons(jrel, result);
			}
		}
	}

	return result;
}

/*
 * make_rels_by_clauseless_joins
 *	  Given a relation 'old_rel' and a list of other relations
 *	  'other_rels', create a join relation between 'old_rel' and each
 *	  member of 'other_rels' that isn't already included in 'old_rel'.
 *	  The join rel nodes are returned in a list.
 *
 * 'old_rel' is the relation entry for the relation to be joined
 * 'other_rels': other rels to be considered for joining
 *
 * Currently, this is only used with initial rels in other_rels, but it would
 * work for joining to joinrels too.
 */
List *
make_rels_by_clauseless_joins(Query *root,
							  RelOptInfo *old_rel,
							  List *other_rels)
{
	List	   *result = NIL;
	List	   *i;

	foreach(i, other_rels)
	{
		RelOptInfo *other_rel = (RelOptInfo *) lfirst(i);

		if (nonoverlap_setsi(other_rel->relids, old_rel->relids))
		{
			RelOptInfo *jrel;

			jrel = make_join_rel(root, old_rel, other_rel, JOIN_INNER);

			/*
			 * As long as given other_rels are distinct, don't need to
			 * test to see if jrel is already part of output list.
			 */
			result = lcons(jrel, result);
		}
	}

	return result;
}


/*
 * make_jointree_rel
 *		Find or build a RelOptInfo join rel representing a specific
 *		jointree item.	For JoinExprs, we only consider the construction
 *		path that corresponds exactly to what the user wrote.
 */
RelOptInfo *
make_jointree_rel(Query *root, Node *jtnode)
{
	if (IsA(jtnode, RangeTblRef))
	{
		int			varno = ((RangeTblRef *) jtnode)->rtindex;

		return build_base_rel(root, varno);
	}
	else if (IsA(jtnode, FromExpr))
	{
		FromExpr   *f = (FromExpr *) jtnode;

		/* Recurse back to multi-way-join planner */
		return make_fromexpr_rel(root, f);
	}
	else if (IsA(jtnode, JoinExpr))
	{
		JoinExpr   *j = (JoinExpr *) jtnode;
		RelOptInfo *rel,
				   *lrel,
				   *rrel;

		/* Recurse */
		lrel = make_jointree_rel(root, j->larg);
		rrel = make_jointree_rel(root, j->rarg);

		/* Make this join rel */
		rel = make_join_rel(root, lrel, rrel, j->jointype);

		/*
		 * Since we are only going to consider this one way to do it,
		 * we're done generating Paths for this joinrel and can now select
		 * the cheapest.  In fact we *must* do so now, since next level up
		 * will need it!
		 */
		set_cheapest(rel);

#ifdef OPTIMIZER_DEBUG
		debug_print_rel(root, rel);
#endif

		return rel;
	}
	else
		elog(ERROR, "make_jointree_rel: unexpected node type %d",
			 nodeTag(jtnode));
	return NULL;				/* keep compiler quiet */
}


/*
 * make_join_rel
 *	   Find or create a join RelOptInfo that represents the join of
 *	   the two given rels, and add to it path information for paths
 *	   created with the two rels as outer and inner rel.
 *	   (The join rel may already contain paths generated from other
 *	   pairs of rels that add up to the same set of base rels.)
 */
static RelOptInfo *
make_join_rel(Query *root, RelOptInfo *rel1, RelOptInfo *rel2,
			  JoinType jointype)
{
	RelOptInfo *joinrel;
	List	   *restrictlist;

	/*
	 * Find or build the join RelOptInfo, and compute the restrictlist
	 * that goes with this particular joining.
	 */
	joinrel = build_join_rel(root, rel1, rel2, jointype, &restrictlist);

	/*
	 * Consider paths using each rel as both outer and inner.
	 */
	switch (jointype)
	{
		case JOIN_INNER:
			add_paths_to_joinrel(root, joinrel, rel1, rel2, JOIN_INNER,
								 restrictlist);
			add_paths_to_joinrel(root, joinrel, rel2, rel1, JOIN_INNER,
								 restrictlist);
			break;
		case JOIN_LEFT:
			add_paths_to_joinrel(root, joinrel, rel1, rel2, JOIN_LEFT,
								 restrictlist);
			add_paths_to_joinrel(root, joinrel, rel2, rel1, JOIN_RIGHT,
								 restrictlist);
			break;
		case JOIN_FULL:
			add_paths_to_joinrel(root, joinrel, rel1, rel2, JOIN_FULL,
								 restrictlist);
			add_paths_to_joinrel(root, joinrel, rel2, rel1, JOIN_FULL,
								 restrictlist);
			break;
		case JOIN_RIGHT:
			add_paths_to_joinrel(root, joinrel, rel1, rel2, JOIN_RIGHT,
								 restrictlist);
			add_paths_to_joinrel(root, joinrel, rel2, rel1, JOIN_LEFT,
								 restrictlist);
			break;
		default:
			elog(ERROR, "make_join_rel: unsupported join type %d",
				 (int) jointype);
			break;
	}

	return joinrel;
}
