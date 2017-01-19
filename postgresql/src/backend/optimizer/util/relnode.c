/*-------------------------------------------------------------------------
 *
 * relnode.c
 *	  Relation-node lookup/construction routines
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/util/relnode.c,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "optimizer/cost.h"
#include "optimizer/joininfo.h"
#include "optimizer/pathnode.h"
#include "optimizer/paths.h"
#include "optimizer/plancat.h"
#include "optimizer/tlist.h"
#include "parser/parsetree.h"


static RelOptInfo *make_base_rel(Query *root, int relid);
static List *new_join_tlist(List *tlist, int first_resdomno);
static List *build_joinrel_restrictlist(Query *root,
						   RelOptInfo *joinrel,
						   RelOptInfo *outer_rel,
						   RelOptInfo *inner_rel);
static void build_joinrel_joinlist(RelOptInfo *joinrel,
					   RelOptInfo *outer_rel,
					   RelOptInfo *inner_rel);
static List *subbuild_joinrel_restrictlist(RelOptInfo *joinrel,
							  List *joininfo_list);
static void subbuild_joinrel_joinlist(RelOptInfo *joinrel,
						  List *joininfo_list);


/*
 * build_base_rel
 *	  Returns relation entry corresponding to 'relid', creating a new one
 *	  if necessary.  This is for base relations.
 */
RelOptInfo *
build_base_rel(Query *root, int relid)
{
	List	   *rels;
	RelOptInfo *rel;

	/* Already made? */
	foreach(rels, root->base_rel_list)
	{
		rel = (RelOptInfo *) lfirst(rels);

		/* length(rel->relids) == 1 for all members of base_rel_list */
		if (lfirsti(rel->relids) == relid)
			return rel;
	}

	/* It should not exist as an "other" rel */
	foreach(rels, root->other_rel_list)
	{
		rel = (RelOptInfo *) lfirst(rels);

		if (lfirsti(rel->relids) == relid)
			elog(ERROR, "build_base_rel: rel already exists as 'other' rel");
	}

	/* No existing RelOptInfo for this base rel, so make a new one */
	rel = make_base_rel(root, relid);

	/* and add it to the list */
	root->base_rel_list = lcons(rel, root->base_rel_list);

	return rel;
}

/*
 * build_other_rel
 *	  Returns relation entry corresponding to 'relid', creating a new one
 *	  if necessary.  This is for 'other' relations, which are just like
 *	  base relations except that they live in a different list.
 */
RelOptInfo *
build_other_rel(Query *root, int relid)
{
	List	   *rels;
	RelOptInfo *rel;

	/* Already made? */
	foreach(rels, root->other_rel_list)
	{
		rel = (RelOptInfo *) lfirst(rels);

		/* length(rel->relids) == 1 for all members of other_rel_list */
		if (lfirsti(rel->relids) == relid)
			return rel;
	}

	/* It should not exist as a base rel */
	foreach(rels, root->base_rel_list)
	{
		rel = (RelOptInfo *) lfirst(rels);

		if (lfirsti(rel->relids) == relid)
			elog(ERROR, "build_other_rel: rel already exists as base rel");
	}

	/* No existing RelOptInfo for this other rel, so make a new one */
	rel = make_base_rel(root, relid);

	/* and add it to the list */
	root->other_rel_list = lcons(rel, root->other_rel_list);

	return rel;
}

/*
 * make_base_rel
 *	  Construct a base-relation RelOptInfo for the specified rangetable index.
 *
 * Common code for build_base_rel and build_other_rel.
 */
static RelOptInfo *
make_base_rel(Query *root, int relid)
{
	RelOptInfo *rel = makeNode(RelOptInfo);
	Oid			relationObjectId;

	rel->relids = makeListi1(relid);
	rel->rows = 0;
	rel->width = 0;
	rel->targetlist = NIL;
	rel->pathlist = NIL;
	rel->cheapest_startup_path = NULL;
	rel->cheapest_total_path = NULL;
	rel->pruneable = true;
	rel->issubquery = false;
	rel->indexlist = NIL;
	rel->pages = 0;
	rel->tuples = 0;
	rel->subplan = NULL;
	rel->baserestrictinfo = NIL;
	rel->baserestrictcost = 0;
	rel->outerjoinset = NIL;
	rel->joininfo = NIL;
	rel->innerjoin = NIL;

	/* Check rtable to see if it's a plain relation or a subquery */
	relationObjectId = getrelid(relid, root->rtable);

	if (relationObjectId != InvalidOid)
	{
		/* Plain relation --- retrieve statistics from the system catalogs */
		bool		indexed;

		get_relation_info(relationObjectId,
						  &indexed, &rel->pages, &rel->tuples);
		if (indexed)
			rel->indexlist = find_secondary_indexes(relationObjectId);
	}
	else
	{
		/* subquery --- mark it as such for later processing */
		rel->issubquery = true;
	}

	return rel;
}

/*
 * find_base_rel
 *	  Find a base or other relation entry, which must already exist
 *	  (since we'd have no idea which list to add it to).
 */
RelOptInfo *
find_base_rel(Query *root, int relid)
{
	List	   *rels;
	RelOptInfo *rel;

	foreach(rels, root->base_rel_list)
	{
		rel = (RelOptInfo *) lfirst(rels);

		/* length(rel->relids) == 1 for all members of base_rel_list */
		if (lfirsti(rel->relids) == relid)
			return rel;
	}

	foreach(rels, root->other_rel_list)
	{
		rel = (RelOptInfo *) lfirst(rels);

		if (lfirsti(rel->relids) == relid)
			return rel;
	}

	elog(ERROR, "find_base_rel: no relation entry for relid %d", relid);

	return NULL;				/* keep compiler quiet */
}

/*
 * find_join_rel
 *	  Returns relation entry corresponding to 'relids' (a list of RT indexes),
 *	  or NULL if none exists.  This is for join relations.
 *
 * Note: there is probably no good reason for this to be called from
 * anywhere except build_join_rel, but keep it as a separate routine
 * just in case.
 */
static RelOptInfo *
find_join_rel(Query *root, Relids relids)
{
	List	   *joinrels;

	foreach(joinrels, root->join_rel_list)
	{
		RelOptInfo *rel = (RelOptInfo *) lfirst(joinrels);

		if (sameseti(rel->relids, relids))
			return rel;
	}

	return NULL;
}

/*
 * build_join_rel
 *	  Returns relation entry corresponding to the union of two given rels,
 *	  creating a new relation entry if none already exists.
 *
 * 'outer_rel' and 'inner_rel' are relation nodes for the relations to be
 *		joined
 * 'jointype': type of join (inner/outer)
 * 'restrictlist_ptr': result variable.  If not NULL, *restrictlist_ptr
 *		receives the list of RestrictInfo nodes that apply to this
 *		particular pair of joinable relations.
 *
 * restrictlist_ptr makes the routine's API a little grotty, but it saves
 * duplicated calculation of the restrictlist...
 */
RelOptInfo *
build_join_rel(Query *root,
			   RelOptInfo *outer_rel,
			   RelOptInfo *inner_rel,
			   JoinType jointype,
			   List **restrictlist_ptr)
{
	List	   *joinrelids;
	RelOptInfo *joinrel;
	List	   *restrictlist;
	List	   *new_outer_tlist;
	List	   *new_inner_tlist;

	/* We should never try to join two overlapping sets of rels. */
	Assert(nonoverlap_setsi(outer_rel->relids, inner_rel->relids));

	/*
	 * See if we already have a joinrel for this set of base rels.
	 *
	 * nconc(listCopy(x), y) is an idiom for making a new list without
	 * changing either input list.
	 */
	joinrelids = nconc(listCopy(outer_rel->relids), inner_rel->relids);
	joinrel = find_join_rel(root, joinrelids);

	if (joinrel)
	{
		/*
		 * Yes, so we only need to figure the restrictlist for this
		 * particular pair of component relations.
		 */
		if (restrictlist_ptr)
			*restrictlist_ptr = build_joinrel_restrictlist(root,
														   joinrel,
														   outer_rel,
														   inner_rel);
		return joinrel;
	}

	/*
	 * Nope, so make one.
	 */
	joinrel = makeNode(RelOptInfo);
	joinrel->relids = joinrelids;
	joinrel->rows = 0;
	joinrel->width = 0;
	joinrel->targetlist = NIL;
	joinrel->pathlist = NIL;
	joinrel->cheapest_startup_path = NULL;
	joinrel->cheapest_total_path = NULL;
	joinrel->pruneable = true;
	joinrel->issubquery = false;
	joinrel->indexlist = NIL;
	joinrel->pages = 0;
	joinrel->tuples = 0;
	joinrel->subplan = NULL;
	joinrel->baserestrictinfo = NIL;
	joinrel->baserestrictcost = 0;
	joinrel->outerjoinset = NIL;
	joinrel->joininfo = NIL;
	joinrel->innerjoin = NIL;

	/*
	 * Create a new tlist by removing irrelevant elements from both tlists
	 * of the outer and inner join relations and then merging the results
	 * together.
	 *
	 * NOTE: the tlist order for a join rel will depend on which pair of
	 * outer and inner rels we first try to build it from.	But the
	 * contents should be the same regardless.
	 *
	 * XXX someday: consider pruning vars from the join's targetlist if they
	 * are needed only to evaluate restriction clauses of this join, and
	 * will never be accessed at higher levels of the plantree.
	 */
	new_outer_tlist = new_join_tlist(outer_rel->targetlist, 1);
	new_inner_tlist = new_join_tlist(inner_rel->targetlist,
									 length(new_outer_tlist) + 1);
	joinrel->targetlist = nconc(new_outer_tlist, new_inner_tlist);

	/*
	 * Construct restrict and join clause lists for the new joinrel. (The
	 * caller might or might not need the restrictlist, but I need it
	 * anyway for set_joinrel_size_estimates().)
	 */
	restrictlist = build_joinrel_restrictlist(root,
											  joinrel,
											  outer_rel,
											  inner_rel);
	if (restrictlist_ptr)
		*restrictlist_ptr = restrictlist;
	build_joinrel_joinlist(joinrel, outer_rel, inner_rel);

	/*
	 * Set estimates of the joinrel's size.
	 */
	set_joinrel_size_estimates(root, joinrel, outer_rel, inner_rel,
							   jointype, restrictlist);

	/*
	 * Add the joinrel to the query's joinrel list.
	 */
	root->join_rel_list = lcons(joinrel, root->join_rel_list);

	return joinrel;
}

/*
 * new_join_tlist
 *	  Builds a join relation's target list by keeping those elements that
 *	  will be in the final target list and any other elements that are still
 *	  needed for future joins.	For a target list entry to still be needed
 *	  for future joins, its 'joinlist' field must not be empty after removal
 *	  of all relids in 'other_relids'.
 *
 *	  XXX the above comment refers to code that is long dead and gone;
 *	  we don't keep track of joinlists for individual targetlist entries
 *	  anymore.	For now, all vars present in either input tlist will be
 *	  emitted in the join's tlist.
 *
 * 'tlist' is the target list of one of the join relations
 * 'first_resdomno' is the resdom number to use for the first created
 *				target list entry
 *
 * Returns the new target list.
 */
static List *
new_join_tlist(List *tlist,
			   int first_resdomno)
{
	int			resdomno = first_resdomno - 1;
	List	   *t_list = NIL;
	List	   *i;

	foreach(i, tlist)
	{
		TargetEntry *xtl = lfirst(i);

		resdomno += 1;
		t_list = lappend(t_list,
						 create_tl_element(get_expr(xtl), resdomno));
	}

	return t_list;
}

/*
 * build_joinrel_restrictlist
 * build_joinrel_joinlist
 *	  These routines build lists of restriction and join clauses for a
 *	  join relation from the joininfo lists of the relations it joins.
 *
 *	  These routines are separate because the restriction list must be
 *	  built afresh for each pair of input sub-relations we consider, whereas
 *	  the join lists need only be computed once for any join RelOptInfo.
 *	  The join lists are fully determined by the set of rels making up the
 *	  joinrel, so we should get the same results (up to ordering) from any
 *	  candidate pair of sub-relations.	But the restriction list is whatever
 *	  is not handled in the sub-relations, so it depends on which
 *	  sub-relations are considered.
 *
 *	  If a join clause from an input relation refers to base rels still not
 *	  present in the joinrel, then it is still a join clause for the joinrel;
 *	  we put it into an appropriate JoinInfo list for the joinrel.	Otherwise,
 *	  the clause is now a restrict clause for the joined relation, and we
 *	  return it to the caller of build_joinrel_restrictlist() to be stored in
 *	  join paths made from this pair of sub-relations.	(It will not need to
 *	  be considered further up the join tree.)
 *
 *	  When building a restriction list, we eliminate redundant clauses.
 *	  We don't try to do that for join clause lists, since the join clauses
 *	  aren't really doing anything, just waiting to become part of higher
 *	  levels' restriction lists.
 *
 * 'joinrel' is a join relation node
 * 'outer_rel' and 'inner_rel' are a pair of relations that can be joined
 *		to form joinrel.
 *
 * build_joinrel_restrictlist() returns a list of relevant restrictinfos,
 * whereas build_joinrel_joinlist() stores its results in the joinrel's
 * joininfo lists.	One or the other must accept each given clause!
 *
 * NB: Formerly, we made deep(!) copies of each input RestrictInfo to pass
 * up to the join relation.  I believe this is no longer necessary, because
 * RestrictInfo nodes are no longer context-dependent.	Instead, just include
 * the original nodes in the lists made for the join relation.
 */
static List *
build_joinrel_restrictlist(Query *root,
						   RelOptInfo *joinrel,
						   RelOptInfo *outer_rel,
						   RelOptInfo *inner_rel)
{
	List	   *result = NIL;
	List	   *rlist;
	List	   *item;

	/*
	 * Collect all the clauses that syntactically belong at this level.
	 */
	rlist = nconc(subbuild_joinrel_restrictlist(joinrel,
												outer_rel->joininfo),
				  subbuild_joinrel_restrictlist(joinrel,
												inner_rel->joininfo));

	/*
	 * Eliminate duplicate and redundant clauses.
	 *
	 * We must eliminate duplicates, since we will see many of the same
	 * clauses arriving from both input relations.	Also, if a clause is a
	 * mergejoinable clause, it's possible that it is redundant with
	 * previous clauses (see optimizer/README for discussion).	We detect
	 * that case and omit the redundant clause from the result list.
	 *
	 * We can detect redundant mergejoinable clauses very cheaply by using
	 * their left and right pathkeys, which uniquely identify the sets of
	 * equijoined variables in question.  All the members of a pathkey set
	 * that are in the left relation have already been forced to be equal;
	 * likewise for those in the right relation.  So, we need to have only
	 * one clause that checks equality between any set member on the left
	 * and any member on the right; by transitivity, all the rest are then
	 * equal.
	 */
	foreach(item, rlist)
	{
		RestrictInfo *rinfo = (RestrictInfo *) lfirst(item);

		/* eliminate duplicates */
		if (member(rinfo, result))
			continue;

		/* check for redundant merge clauses */
		if (rinfo->mergejoinoperator != InvalidOid)
		{
			bool		redundant = false;
			List	   *olditem;

			cache_mergeclause_pathkeys(root, rinfo);

			foreach(olditem, result)
			{
				RestrictInfo *oldrinfo = (RestrictInfo *) lfirst(olditem);

				if (oldrinfo->mergejoinoperator != InvalidOid &&
					rinfo->left_pathkey == oldrinfo->left_pathkey &&
					rinfo->right_pathkey == oldrinfo->right_pathkey)
				{
					redundant = true;
					break;
				}
			}

			if (redundant)
				continue;
		}

		/* otherwise, add it to result list */
		result = lappend(result, rinfo);
	}

	freeList(rlist);

	return result;
}

static void
build_joinrel_joinlist(RelOptInfo *joinrel,
					   RelOptInfo *outer_rel,
					   RelOptInfo *inner_rel)
{
	subbuild_joinrel_joinlist(joinrel, outer_rel->joininfo);
	subbuild_joinrel_joinlist(joinrel, inner_rel->joininfo);
}

static List *
subbuild_joinrel_restrictlist(RelOptInfo *joinrel,
							  List *joininfo_list)
{
	List	   *restrictlist = NIL;
	List	   *xjoininfo;

	foreach(xjoininfo, joininfo_list)
	{
		JoinInfo   *joininfo = (JoinInfo *) lfirst(xjoininfo);

		if (is_subseti(joininfo->unjoined_relids, joinrel->relids))
		{
			/*
			 * Clauses in this JoinInfo list become restriction clauses
			 * for the joinrel, since they refer to no outside rels.
			 *
			 * We must copy the list to avoid disturbing the input relation,
			 * but we can use a shallow copy.
			 */
			restrictlist = nconc(restrictlist,
								 listCopy(joininfo->jinfo_restrictinfo));
		}
		else
		{
			/*
			 * These clauses are still join clauses at this level, so we
			 * ignore them in this routine.
			 */
		}
	}

	return restrictlist;
}

static void
subbuild_joinrel_joinlist(RelOptInfo *joinrel,
						  List *joininfo_list)
{
	List	   *xjoininfo;

	foreach(xjoininfo, joininfo_list)
	{
		JoinInfo   *joininfo = (JoinInfo *) lfirst(xjoininfo);
		Relids		new_unjoined_relids;

		new_unjoined_relids = set_differencei(joininfo->unjoined_relids,
											  joinrel->relids);
		if (new_unjoined_relids == NIL)
		{
			/*
			 * Clauses in this JoinInfo list become restriction clauses
			 * for the joinrel, since they refer to no outside rels. So we
			 * can ignore them in this routine.
			 */
		}
		else
		{
			/*
			 * These clauses are still join clauses at this level, so find
			 * or make the appropriate JoinInfo item for the joinrel, and
			 * add the clauses to it (eliminating duplicates).
			 */
			JoinInfo   *new_joininfo;

			new_joininfo = find_joininfo_node(joinrel, new_unjoined_relids);
			new_joininfo->jinfo_restrictinfo =
				set_union(new_joininfo->jinfo_restrictinfo,
						  joininfo->jinfo_restrictinfo);
		}
	}
}
