/*-------------------------------------------------------------------------
 *
 * indxpath.c
 *	  Routines to determine which indices are usable for scanning a
 *	  given relation, and create IndexPaths accordingly.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/path/indxpath.c,v 1.2 2004/07/05 08:55:29 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <math.h>

#include "access/heapam.h"
#include "access/nbtree.h"
#include "catalog/catname.h"
#include "catalog/pg_amop.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "optimizer/clauses.h"
#include "optimizer/cost.h"
#include "optimizer/pathnode.h"
#include "optimizer/paths.h"
#include "optimizer/restrictinfo.h"
#include "optimizer/var.h"
#include "parser/parse_coerce.h"
#include "parser/parse_expr.h"
#include "parser/parse_oper.h"
#include "utils/builtins.h"
#include "utils/fmgroids.h"
#include "utils/lsyscache.h"
#include "utils/selfuncs.h"
#include "utils/syscache.h"


/*
 * DoneMatchingIndexKeys() - MACRO
 *
 * Determine whether we should continue matching index keys in a clause.
 * Depends on if there are more to match or if this is a functional index.
 * In the latter case we stop after the first match since there can
 * be only 1 key (i.e. the function's return value) and the attributes in
 * keys list represent the arguments to the function.  -mer 3 Oct. 1991
 */
#define DoneMatchingIndexKeys(indexkeys, index) \
		(indexkeys[0] == 0 || \
		 (index->indproc != InvalidOid))

#define is_indexable_operator(clause,opclass,indexkey_on_left) \
	(indexable_operator(clause,opclass,indexkey_on_left) != InvalidOid)


static void match_index_orclauses(RelOptInfo *rel, IndexOptInfo *index,
					  List *restrictinfo_list);
static List *match_index_orclause(RelOptInfo *rel, IndexOptInfo *index,
					 List *or_clauses,
					 List *other_matching_indices);
static bool match_or_subclause_to_indexkey(RelOptInfo *rel,
							   IndexOptInfo *index,
							   Expr *clause);
static List *group_clauses_by_indexkey(RelOptInfo *rel, IndexOptInfo *index,
						  int *indexkeys, Oid *classes,
						  List *restrictinfo_list);
static List *group_clauses_by_ikey_for_joins(RelOptInfo *rel,
								IndexOptInfo *index,
								int *indexkeys, Oid *classes,
								List *join_cinfo_list,
								List *restr_cinfo_list);
static bool match_clause_to_indexkey(RelOptInfo *rel, IndexOptInfo *index,
						 int indexkey, Oid opclass,
						 Expr *clause, bool join);
static bool pred_test(List *predicate_list, List *restrictinfo_list,
		  List *joininfo_list);
static bool pred_test_restrict_list(Expr *predicate, List *restrictinfo_list);
static bool pred_test_recurse_clause(Expr *predicate, Node *clause);
static bool pred_test_recurse_pred(Expr *predicate, Node *clause);
static bool pred_test_simple_clause(Expr *predicate, Node *clause);
static void indexable_joinclauses(RelOptInfo *rel, IndexOptInfo *index,
					  List *joininfo_list, List *restrictinfo_list,
					  List **clausegroups, List **outerrelids);
static List *index_innerjoin(Query *root, RelOptInfo *rel, IndexOptInfo *index,
				List *clausegroup_list, List *outerrelids_list);
static bool match_index_to_operand(int indexkey, Var *operand,
					   RelOptInfo *rel, IndexOptInfo *index);
static bool function_index_operand(Expr *funcOpnd, RelOptInfo *rel,
					   IndexOptInfo *index);
static bool match_special_index_operator(Expr *clause, Oid opclass,
							 bool indexkey_on_left);
static List *prefix_quals(Var *leftop, Oid expr_op,
			 char *prefix, Pattern_Prefix_Status pstatus);
static List *network_prefix_quals(Var *leftop, Oid expr_op, Datum rightop);
static Oid	find_operator(const char *opname, Oid datatype);
static Datum string_to_datum(const char *str, Oid datatype);
static Const *string_to_const(const char *str, Oid datatype);


/*
 * create_index_paths()
 *	  Generate all interesting index paths for the given relation.
 *	  Candidate paths are added to the rel's pathlist (using add_path).
 *	  Additional IndexPath nodes may also be added to rel's innerjoin list.
 *
 * To be considered for an index scan, an index must match one or more
 * restriction clauses or join clauses from the query's qual condition,
 * or match the query's ORDER BY condition.
 *
 * There are two basic kinds of index scans.  A "plain" index scan uses
 * only restriction clauses (possibly none at all) in its indexqual,
 * so it can be applied in any context.  An "innerjoin" index scan uses
 * join clauses (plus restriction clauses, if available) in its indexqual.
 * Therefore it can only be used as the inner relation of a nestloop
 * join against an outer rel that includes all the other rels mentioned
 * in its join clauses.  In that context, values for the other rels'
 * attributes are available and fixed during any one scan of the indexpath.
 *
 * An IndexPath is generated and submitted to add_path() for each index
 * this routine deems potentially interesting for the current query.
 * An innerjoin path is also generated for each interesting combination of
 * outer join relations.  The innerjoin paths are *not* passed to add_path(),
 * but are appended to the "innerjoin" list of the relation for later
 * consideration in nested-loop joins.
 *
 * 'rel' is the relation for which we want to generate index paths
 */
void
create_index_paths(Query *root, RelOptInfo *rel)
{
	List	   *restrictinfo_list = rel->baserestrictinfo;
	List	   *joininfo_list = rel->joininfo;
	List	   *ilist;

	foreach(ilist, rel->indexlist)
	{
		IndexOptInfo *index = (IndexOptInfo *) lfirst(ilist);
		List	   *restrictclauses;
		List	   *index_pathkeys;
		List	   *useful_pathkeys;
		bool		index_is_ordered;
		List	   *joinclausegroups;
		List	   *joinouterrelids;

		/*
		 * If this is a partial index, we can only use it if it passes the
		 * predicate test.
		 */
		if (index->indpred != NIL)
			if (!pred_test(index->indpred, restrictinfo_list, joininfo_list))
				continue;

		/*
		 * 1. Try matching the index against subclauses of restriction
		 * 'or' clauses (ie, 'or' clauses that reference only this
		 * relation). The restrictinfo nodes for the 'or' clauses are
		 * marked with lists of the matching indices.  No paths are
		 * actually created now; that will be done in orindxpath.c after
		 * all indexes for the rel have been examined.	(We need to do it
		 * that way because we can potentially use a different index for
		 * each subclause of an 'or', so we can't build a path for an 'or'
		 * clause until all indexes have been matched against it.)
		 *
		 * We don't even think about special handling of 'or' clauses that
		 * involve more than one relation (ie, are join clauses). Can we
		 * do anything useful with those?
		 */
		match_index_orclauses(rel, index, restrictinfo_list);

		/*
		 * 2. Match the index against non-'or' restriction clauses.
		 */
		restrictclauses = group_clauses_by_indexkey(rel,
													index,
													index->indexkeys,
													index->classlist,
													restrictinfo_list);

		/*
		 * 3. Compute pathkeys describing index's ordering, if any, then
		 * see how many of them are actually useful for this query.
		 */
		index_pathkeys = build_index_pathkeys(root, rel, index,
											  ForwardScanDirection);
		index_is_ordered = (index_pathkeys != NIL);
		useful_pathkeys = truncate_useless_pathkeys(root, rel,
													index_pathkeys);

		/*
		 * 4. Generate an indexscan path if there are relevant restriction
		 * clauses OR the index ordering is potentially useful for later
		 * merging or final output ordering.
		 *
		 * If there is a predicate, consider it anyway since the index
		 * predicate has already been found to match the query.  The
		 * selectivity of the predicate might alone make the index useful.
		 */
		if (restrictclauses != NIL ||
			useful_pathkeys != NIL ||
			index->indpred != NIL)
			add_path(rel, (Path *)
					 create_index_path(root, rel, index,
									   restrictclauses,
									   useful_pathkeys,
									   index_is_ordered ?
									   ForwardScanDirection :
									   NoMovementScanDirection));

		/*
		 * 5. If the index is ordered, a backwards scan might be
		 * interesting. Currently this is only possible for a DESC query
		 * result ordering.
		 */
		if (index_is_ordered)
		{
			index_pathkeys = build_index_pathkeys(root, rel, index,
												  BackwardScanDirection);
			useful_pathkeys = truncate_useless_pathkeys(root, rel,
														index_pathkeys);
			if (useful_pathkeys != NIL)
				add_path(rel, (Path *)
						 create_index_path(root, rel, index,
										   restrictclauses,
										   useful_pathkeys,
										   BackwardScanDirection));
		}

		/*
		 * 6. Create an innerjoin index path for each combination of other
		 * rels used in available join clauses.  These paths will be
		 * considered as the inner side of nestloop joins against those
		 * sets of other rels.	indexable_joinclauses() finds sets of
		 * clauses that can be used with each combination of outer rels,
		 * and index_innerjoin builds the paths themselves.  We add the
		 * paths to the rel's innerjoin list, NOT to the result list.
		 */
		indexable_joinclauses(rel, index,
							  joininfo_list, restrictinfo_list,
							  &joinclausegroups,
							  &joinouterrelids);
		if (joinclausegroups != NIL)
		{
			rel->innerjoin = nconc(rel->innerjoin,
								   index_innerjoin(root, rel, index,
												   joinclausegroups,
												   joinouterrelids));
		}
	}
}


/****************************************************************************
 *		----  ROUTINES TO PROCESS 'OR' CLAUSES	----
 ****************************************************************************/


/*
 * match_index_orclauses
 *	  Attempt to match an index against subclauses within 'or' clauses.
 *	  Each subclause that does match is marked with the index's node.
 *
 *	  Essentially, this adds 'index' to the list of subclause indices in
 *	  the RestrictInfo field of each of the 'or' clauses where it matches.
 *	  NOTE: we can use storage in the RestrictInfo for this purpose because
 *	  this processing is only done on single-relation restriction clauses.
 *	  Therefore, we will never have indexes for more than one relation
 *	  mentioned in the same RestrictInfo node's list.
 *
 * 'rel' is the node of the relation on which the index is defined.
 * 'index' is the index node.
 * 'restrictinfo_list' is the list of available restriction clauses.
 */
static void
match_index_orclauses(RelOptInfo *rel,
					  IndexOptInfo *index,
					  List *restrictinfo_list)
{
	List	   *i;

	foreach(i, restrictinfo_list)
	{
		RestrictInfo *restrictinfo = (RestrictInfo *) lfirst(i);

		if (restriction_is_or_clause(restrictinfo))
		{
			/*
			 * Add this index to the subclause index list for each
			 * subclause that it matches.
			 */
			restrictinfo->subclauseindices =
				match_index_orclause(rel, index,
									 restrictinfo->clause->args,
									 restrictinfo->subclauseindices);
		}
	}
}

/*
 * match_index_orclause
 *	  Attempts to match an index against the subclauses of an 'or' clause.
 *
 *	  A match means that:
 *	  (1) the operator within the subclause can be used with the
 *		  index's specified operator class, and
 *	  (2) one operand of the subclause matches the index key.
 *
 *	  If a subclause is an 'and' clause, then it matches if any of its
 *	  subclauses is an opclause that matches.
 *
 * 'or_clauses' is the list of subclauses within the 'or' clause
 * 'other_matching_indices' is the list of information on other indices
 *		that have already been matched to subclauses within this
 *		particular 'or' clause (i.e., a list previously generated by
 *		this routine), or NIL if this routine has not previously been
 *		run for this 'or' clause.
 *
 * Returns a list of the form ((a b c) (d e f) nil (g h) ...) where
 * a,b,c are nodes of indices that match the first subclause in
 * 'or-clauses', d,e,f match the second subclause, no indices
 * match the third, g,h match the fourth, etc.
 */
static List *
match_index_orclause(RelOptInfo *rel,
					 IndexOptInfo *index,
					 List *or_clauses,
					 List *other_matching_indices)
{
	List	   *matching_indices;
	List	   *index_list;
	List	   *clist;

	/*
	 * first time through, we create list of same length as OR clause,
	 * containing an empty sublist for each subclause.
	 */
	if (!other_matching_indices)
	{
		matching_indices = NIL;
		foreach(clist, or_clauses)
			matching_indices = lcons(NIL, matching_indices);
	}
	else
		matching_indices = other_matching_indices;

	index_list = matching_indices;

	foreach(clist, or_clauses)
	{
		Expr	   *clause = lfirst(clist);

		if (match_or_subclause_to_indexkey(rel, index, clause))
		{
			/* OK to add this index to sublist for this subclause */
			lfirst(matching_indices) = lcons(index,
											 lfirst(matching_indices));
		}

		matching_indices = lnext(matching_indices);
	}

	return index_list;
}

/*
 * See if a subclause of an OR clause matches an index.
 *
 * We accept the subclause if it is an operator clause that matches the
 * index, or if it is an AND clause any of whose members is an opclause
 * that matches the index.
 *
 * For multi-key indexes, we only look for matches to the first key;
 * without such a match the index is useless.  If the clause is an AND
 * then we may be able to extract additional subclauses to use with the
 * later indexkeys, but we need not worry about that until
 * extract_or_indexqual_conditions() is called (if it ever is).
 */
static bool
match_or_subclause_to_indexkey(RelOptInfo *rel,
							   IndexOptInfo *index,
							   Expr *clause)
{
	int			indexkey = index->indexkeys[0];
	Oid			opclass = index->classlist[0];

	if (and_clause((Node *) clause))
	{
		List	   *item;

		foreach(item, clause->args)
		{
			if (match_clause_to_indexkey(rel, index, indexkey, opclass,
										 lfirst(item), false))
				return true;
		}
		return false;
	}
	else
		return match_clause_to_indexkey(rel, index, indexkey, opclass,
										clause, false);
}

/*----------
 * Given an OR subclause that has previously been determined to match
 * the specified index, extract a list of specific opclauses that can be
 * used as indexquals.
 *
 * In the simplest case this just means making a one-element list of the
 * given opclause.	However, if the OR subclause is an AND, we have to
 * scan it to find the opclause(s) that match the index.  (There should
 * be at least one, if match_or_subclause_to_indexkey succeeded, but there
 * could be more.)
 *
 * Also, we can look at other restriction clauses of the rel to discover
 * additional candidate indexquals: for example, consider
 *			... where (a = 11 or a = 12) and b = 42;
 * If we are dealing with an index on (a,b) then we can include the clause
 * b = 42 in the indexqual list generated for each of the OR subclauses.
 * Essentially, we are making an index-specific transformation from CNF to
 * DNF.  (NOTE: when we do this, we end up with a slightly inefficient plan
 * because create_indexscan_plan is not very bright about figuring out which
 * restriction clauses are implied by the generated indexqual condition.
 * Currently we'll end up rechecking both the OR clause and the transferred
 * restriction clause as qpquals.  FIXME someday.)
 *
 * Also, we apply expand_indexqual_conditions() to convert any special
 * matching opclauses to indexable operators.
 *
 * The passed-in clause is not changed.
 *----------
 */
List *
extract_or_indexqual_conditions(RelOptInfo *rel,
								IndexOptInfo *index,
								Expr *orsubclause)
{
	List	   *quals = NIL;
	int		   *indexkeys = index->indexkeys;
	Oid		   *classes = index->classlist;

	/*
	 * Extract relevant indexclauses in indexkey order.  This is
	 * essentially just like group_clauses_by_indexkey() except that the
	 * input and output are lists of bare clauses, not of RestrictInfo
	 * nodes.
	 */
	do
	{
		int			curIndxKey = indexkeys[0];
		Oid			curClass = classes[0];
		List	   *clausegroup = NIL;
		List	   *item;

		if (and_clause((Node *) orsubclause))
		{
			foreach(item, orsubclause->args)
			{
				Expr	   *subsubclause = (Expr *) lfirst(item);

				if (match_clause_to_indexkey(rel, index,
											 curIndxKey, curClass,
											 subsubclause, false))
					clausegroup = lappend(clausegroup, subsubclause);
			}
		}
		else if (match_clause_to_indexkey(rel, index,
										  curIndxKey, curClass,
										  orsubclause, false))
			clausegroup = makeList1(orsubclause);

		/*
		 * If we found no clauses for this indexkey in the OR subclause
		 * itself, try looking in the rel's top-level restriction list.
		 */
		if (clausegroup == NIL)
		{
			foreach(item, rel->baserestrictinfo)
			{
				RestrictInfo *rinfo = (RestrictInfo *) lfirst(item);

				if (match_clause_to_indexkey(rel, index,
											 curIndxKey, curClass,
											 rinfo->clause, false))
					clausegroup = lappend(clausegroup, rinfo->clause);
			}
		}

		/*
		 * If still no clauses match this key, we're done; we don't want
		 * to look at keys to its right.
		 */
		if (clausegroup == NIL)
			break;

		quals = nconc(quals, clausegroup);

		indexkeys++;
		classes++;
	} while (!DoneMatchingIndexKeys(indexkeys, index));

	if (quals == NIL)
		elog(ERROR, "extract_or_indexqual_conditions: no matching clause");

	return expand_indexqual_conditions(quals);
}


/****************************************************************************
 *				----  ROUTINES TO CHECK RESTRICTIONS  ----
 ****************************************************************************/


/*
 * group_clauses_by_indexkey
 *	  Generates a list of restriction clauses that can be used with an index.
 *
 * 'rel' is the node of the relation itself.
 * 'index' is a index on 'rel'.
 * 'indexkeys' are the index keys to be matched.
 * 'classes' are the classes of the index operators on those keys.
 * 'restrictinfo_list' is the list of available restriction clauses for 'rel'.
 *
 * Returns a list of all the RestrictInfo nodes for clauses that can be
 * used with this index.
 *
 * The list is ordered by index key.  (This is not depended on by any part
 * of the planner, as far as I can tell; but some parts of the executor
 * do assume that the indxqual list ultimately delivered to the executor
 * is so ordered.  One such place is _bt_orderkeys() in the btree support.
 * Perhaps that ought to be fixed someday --- tgl 7/00)
 *
 * Note that in a multi-key index, we stop if we find a key that cannot be
 * used with any clause.  For example, given an index on (A,B,C), we might
 * return (C1 C2 C3 C4) if we find that clauses C1 and C2 use column A,
 * clauses C3 and C4 use column B, and no clauses use column C.  But if
 * no clauses match B we will return (C1 C2), whether or not there are
 * clauses matching column C, because the executor couldn't use them anyway.
 */
static List *
group_clauses_by_indexkey(RelOptInfo *rel,
						  IndexOptInfo *index,
						  int *indexkeys,
						  Oid *classes,
						  List *restrictinfo_list)
{
	List	   *clausegroup_list = NIL;

	if (restrictinfo_list == NIL || indexkeys[0] == 0)
		return NIL;

	do
	{
		int			curIndxKey = indexkeys[0];
		Oid			curClass = classes[0];
		List	   *clausegroup = NIL;
		List	   *curCinfo;

		foreach(curCinfo, restrictinfo_list)
		{
			RestrictInfo *rinfo = (RestrictInfo *) lfirst(curCinfo);

			if (match_clause_to_indexkey(rel,
										 index,
										 curIndxKey,
										 curClass,
										 rinfo->clause,
										 false))
				clausegroup = lappend(clausegroup, rinfo);
		}

		/*
		 * If no clauses match this key, we're done; we don't want to look
		 * at keys to its right.
		 */
		if (clausegroup == NIL)
			break;

		clausegroup_list = nconc(clausegroup_list, clausegroup);

		indexkeys++;
		classes++;

	} while (!DoneMatchingIndexKeys(indexkeys, index));

	/* clausegroup_list holds all matched clauses ordered by indexkeys */
	return clausegroup_list;
}

/*
 * group_clauses_by_ikey_for_joins
 *	  Generates a list of join clauses that can be used with an index
 *	  to scan the inner side of a nestloop join.
 *
 * This is much like group_clauses_by_indexkey(), but we consider both
 * join and restriction clauses.  For each indexkey in the index, we
 * accept both join and restriction clauses that match it, since both
 * will make useful indexquals if the index is being used to scan the
 * inner side of a nestloop join.  But there must be at least one matching
 * join clause, or we return NIL indicating that this index isn't useful
 * for nestloop joining.
 */
static List *
group_clauses_by_ikey_for_joins(RelOptInfo *rel,
								IndexOptInfo *index,
								int *indexkeys,
								Oid *classes,
								List *join_cinfo_list,
								List *restr_cinfo_list)
{
	List	   *clausegroup_list = NIL;
	bool		jfound = false;

	if (join_cinfo_list == NIL || indexkeys[0] == 0)
		return NIL;

	do
	{
		int			curIndxKey = indexkeys[0];
		Oid			curClass = classes[0];
		List	   *clausegroup = NIL;
		List	   *curCinfo;

		foreach(curCinfo, join_cinfo_list)
		{
			RestrictInfo *rinfo = (RestrictInfo *) lfirst(curCinfo);

			if (match_clause_to_indexkey(rel,
										 index,
										 curIndxKey,
										 curClass,
										 rinfo->clause,
										 true))
			{
				clausegroup = lappend(clausegroup, rinfo);
				jfound = true;
			}
		}
		foreach(curCinfo, restr_cinfo_list)
		{
			RestrictInfo *rinfo = (RestrictInfo *) lfirst(curCinfo);

			if (match_clause_to_indexkey(rel,
										 index,
										 curIndxKey,
										 curClass,
										 rinfo->clause,
										 false))
				clausegroup = lappend(clausegroup, rinfo);
		}

		/*
		 * If no clauses match this key, we're done; we don't want to look
		 * at keys to its right.
		 */
		if (clausegroup == NIL)
			break;

		clausegroup_list = nconc(clausegroup_list, clausegroup);

		indexkeys++;
		classes++;

	} while (!DoneMatchingIndexKeys(indexkeys, index));

	/*
	 * if no join clause was matched then there ain't clauses for joins at
	 * all.
	 */
	if (!jfound)
	{
		freeList(clausegroup_list);
		return NIL;
	}

	/* clausegroup_list holds all matched clauses ordered by indexkeys */
	return clausegroup_list;
}


/*
 * match_clause_to_indexkey()
 *	  Determines whether a restriction or join clause matches
 *	  a key of an index.
 *
 *	  To match, the clause:
 *
 *	  (1a) for a restriction clause: must be in the form (indexkey op const)
 *		   or (const op indexkey), or
 *	  (1b) for a join clause: must be in the form (indexkey op others)
 *		   or (others op indexkey), where others is an expression involving
 *		   only vars of the other relation(s); and
 *	  (2)  must contain an operator which is in the same class as the index
 *		   operator for this key, or is a "special" operator as recognized
 *		   by match_special_index_operator().
 *
 *	  Presently, the executor can only deal with indexquals that have the
 *	  indexkey on the left, so we can only use clauses that have the indexkey
 *	  on the right if we can commute the clause to put the key on the left.
 *	  We do not actually do the commuting here, but we check whether a
 *	  suitable commutator operator is available.
 *
 *	  Note that in the join case, we already know that the clause as a
 *	  whole uses vars from the interesting set of relations.  But we need
 *	  to defend against expressions like (a.f1 OP (b.f2 OP a.f3)); that's
 *	  not processable by an indexscan nestloop join, whereas
 *	  (a.f1 OP (b.f2 OP c.f3)) is.
 *
 * 'rel' is the relation of interest.
 * 'index' is an index on 'rel'.
 * 'indexkey' is a key of 'index'.
 * 'opclass' is the corresponding operator class.
 * 'clause' is the clause to be tested.
 * 'join' is true if we are considering this clause for joins.
 *
 * Returns true if the clause can be used with this index key.
 *
 * NOTE:  returns false if clause is an OR or AND clause; it is the
 * responsibility of higher-level routines to cope with those.
 */
static bool
match_clause_to_indexkey(RelOptInfo *rel,
						 IndexOptInfo *index,
						 int indexkey,
						 Oid opclass,
						 Expr *clause,
						 bool join)
{
	Var		   *leftop,
			   *rightop;

	/* Clause must be a binary opclause. */
	if (!is_opclause((Node *) clause))
		return false;
	leftop = get_leftop(clause);
	rightop = get_rightop(clause);
	if (!leftop || !rightop)
		return false;

	if (!join)
	{
		/*
		 * Not considering joins, so check for clauses of the form:
		 * (indexkey operator constant) or (constant operator indexkey).
		 * Anything that is a "pseudo constant" expression will do.
		 */

		if (match_index_to_operand(indexkey, leftop, rel, index) &&
			is_pseudo_constant_clause((Node *) rightop))
		{
			if (is_indexable_operator(clause, opclass, true))
				return true;

			/*
			 * If we didn't find a member of the index's opclass, see
			 * whether it is a "special" indexable operator.
			 */
			if (match_special_index_operator(clause, opclass, true))
				return true;
			return false;
		}
		if (match_index_to_operand(indexkey, rightop, rel, index) &&
			is_pseudo_constant_clause((Node *) leftop))
		{
			if (is_indexable_operator(clause, opclass, false))
				return true;

			/*
			 * If we didn't find a member of the index's opclass, see
			 * whether it is a "special" indexable operator.
			 */
			if (match_special_index_operator(clause, opclass, false))
				return true;
			return false;
		}
	}
	else
	{
		/*
		 * Check for an indexqual that could be handled by a nestloop
		 * join. We need the index key to be compared against an
		 * expression that uses none of the indexed relation's vars and
		 * contains no non-cachable functions.
		 */
		if (match_index_to_operand(indexkey, leftop, rel, index))
		{
			List	   *othervarnos = pull_varnos((Node *) rightop);
			bool		isIndexable;

			isIndexable =
				!intMember(lfirsti(rel->relids), othervarnos) &&
				!contain_noncachable_functions((Node *) rightop) &&
				is_indexable_operator(clause, opclass, true);
			freeList(othervarnos);
			return isIndexable;
		}
		else if (match_index_to_operand(indexkey, rightop, rel, index))
		{
			List	   *othervarnos = pull_varnos((Node *) leftop);
			bool		isIndexable;

			isIndexable =
				!intMember(lfirsti(rel->relids), othervarnos) &&
				!contain_noncachable_functions((Node *) leftop) &&
				is_indexable_operator(clause, opclass, false);
			freeList(othervarnos);
			return isIndexable;
		}
	}

	return false;
}

/*
 * indexable_operator
 *	  Does a binary opclause contain an operator matching the index opclass?
 *
 * If the indexkey is on the right, what we actually want to know
 * is whether the operator has a commutator operator that matches
 * the index's opclass.
 *
 * We try both the straightforward match and matches that rely on
 * recognizing binary-compatible datatypes.  For example, if we have
 * an expression like "oid = 123", the operator will be oideqint4,
 * which we need to replace with oideq in order to recognize it as
 * matching an oid_ops index on the oid field.	A variant case is where
 * the expression is like "oid::int4 = 123", where the given operator
 * will be int4eq and again we need to intuit that we want to use oideq.
 *
 * Returns the OID of the matching operator, or InvalidOid if no match.
 * Note that the returned OID will be different from the one in the given
 * expression if we used a binary-compatible substitution.	Also note that
 * if indexkey_on_left is FALSE (meaning we need to commute), the returned
 * OID is *not* commuted; it can be plugged directly into the given clause.
 */
Oid
indexable_operator(Expr *clause, Oid opclass, bool indexkey_on_left)
{
	Oid			expr_op = ((Oper *) clause->oper)->opno;
	Oid			commuted_op,
				new_op;
	Operator	oldoptup;
	Form_pg_operator oldopform;
	char	   *opname;
	Oid			ltype,
				rtype,
				indexkeytype;

	/* Get the commuted operator if necessary */
	if (indexkey_on_left)
		commuted_op = expr_op;
	else
		commuted_op = get_commutator(expr_op);
	if (commuted_op == InvalidOid)
		return InvalidOid;

	/* Done if the (commuted) operator is a member of the index's opclass */
	if (op_in_opclass(commuted_op, opclass))
		return expr_op;

	/*
	 * Maybe the index uses a binary-compatible operator set.
	 *
	 * Get the nominal input types of the given operator and the actual type
	 * (before binary-compatible relabeling) of the index key.
	 */
	oldoptup = SearchSysCache(OPEROID,
							  ObjectIdGetDatum(expr_op),
							  0, 0, 0);
	if (!HeapTupleIsValid(oldoptup))
		return InvalidOid;		/* probably can't happen */
	oldopform = (Form_pg_operator) GETSTRUCT(oldoptup);
	opname = pstrdup(NameStr(oldopform->oprname));
	ltype = oldopform->oprleft;
	rtype = oldopform->oprright;
	ReleaseSysCache(oldoptup);

	if (indexkey_on_left)
	{
		Node	   *leftop = (Node *) get_leftop(clause);

		if (leftop && IsA(leftop, RelabelType))
			leftop = ((RelabelType *) leftop)->arg;
		indexkeytype = exprType(leftop);
	}
	else
	{
		Node	   *rightop = (Node *) get_rightop(clause);

		if (rightop && IsA(rightop, RelabelType))
			rightop = ((RelabelType *) rightop)->arg;
		indexkeytype = exprType(rightop);
	}

	/*
	 * Make sure we have different but binary-compatible types.
	 */
	if (ltype == indexkeytype && rtype == indexkeytype)
		return InvalidOid;		/* no chance for a different operator */
	if (ltype != indexkeytype && !IS_BINARY_COMPATIBLE(ltype, indexkeytype))
		return InvalidOid;
	if (rtype != indexkeytype && !IS_BINARY_COMPATIBLE(rtype, indexkeytype))
		return InvalidOid;

	/*
	 * OK, look for operator of the same name with the indexkey's data
	 * type. (In theory this might find a non-semantically-comparable
	 * operator, but in practice that seems pretty unlikely for
	 * binary-compatible types.)
	 */
	new_op = compatible_oper_opid(opname, indexkeytype, indexkeytype, true);

	if (OidIsValid(new_op))
	{
		if (new_op != expr_op)
		{
			/*
			 * OK, we found a binary-compatible operator of the same name;
			 * now does it match the index?
			 */
			if (indexkey_on_left)
				commuted_op = new_op;
			else
				commuted_op = get_commutator(new_op);
			if (commuted_op == InvalidOid)
				return InvalidOid;

			if (op_in_opclass(commuted_op, opclass))
				return new_op;
		}
	}

	return InvalidOid;
}

/****************************************************************************
 *				----  ROUTINES TO DO PARTIAL INDEX PREDICATE TESTS	----
 ****************************************************************************/

/*
 * pred_test
 *	  Does the "predicate inclusion test" for partial indexes.
 *
 *	  Recursively checks whether the clauses in restrictinfo_list imply
 *	  that the given predicate is true.
 *
 *	  This routine (together with the routines it calls) iterates over
 *	  ANDs in the predicate first, then reduces the qualification
 *	  clauses down to their constituent terms, and iterates over ORs
 *	  in the predicate last.  This order is important to make the test
 *	  succeed whenever possible (assuming the predicate has been converted
 *	  to CNF format). --Nels, Jan '93
 */
static bool
pred_test(List *predicate_list, List *restrictinfo_list, List *joininfo_list)
{
	List	   *pred;

	/*
	 * Note: if Postgres tried to optimize queries by forming equivalence
	 * classes over equi-joined attributes (i.e., if it recognized that a
	 * qualification such as "where a.b=c.d and a.b=5" could make use of
	 * an index on c.d), then we could use that equivalence class info
	 * here with joininfo_list to do more complete tests for the usability
	 * of a partial index.	For now, the test only uses restriction
	 * clauses (those in restrictinfo_list). --Nels, Dec '92
	 *
	 * XXX as of 7.1, equivalence class info *is* available.  Consider
	 * improving this code as foreseen by Nels.
	 */

	if (predicate_list == NIL)
		return true;			/* no predicate: the index is usable */
	if (restrictinfo_list == NIL)
		return false;			/* no restriction clauses: the test must
								 * fail */

	foreach(pred, predicate_list)
	{
		/*
		 * if any clause is not implied, the whole predicate is not
		 * implied.  Note we assume that any sub-ANDs have been flattened
		 * when the predicate was fed through canonicalize_qual().
		 */
		if (!pred_test_restrict_list(lfirst(pred), restrictinfo_list))
			return false;
	}
	return true;
}


/*
 * pred_test_restrict_list
 *	  Does the "predicate inclusion test" for one conjunct of a predicate
 *	  expression.
 */
static bool
pred_test_restrict_list(Expr *predicate, List *restrictinfo_list)
{
	List	   *item;

	foreach(item, restrictinfo_list)
	{
		RestrictInfo *restrictinfo = (RestrictInfo *) lfirst(item);

		/* if any clause implies the predicate, return true */
		if (pred_test_recurse_clause(predicate,
									 (Node *) restrictinfo->clause))
			return true;
	}
	return false;
}


/*
 * pred_test_recurse_clause
 *	  Does the "predicate inclusion test" for a general restriction-clause
 *	  expression.  Here we recursively deal with the possibility that the
 *	  restriction clause is itself an AND or OR structure.
 */
static bool
pred_test_recurse_clause(Expr *predicate, Node *clause)
{
	List	   *items,
			   *item;

	Assert(clause != NULL);
	if (or_clause(clause))
	{
		items = ((Expr *) clause)->args;
		foreach(item, items)
		{
			/* if any OR item doesn't imply the predicate, clause doesn't */
			if (!pred_test_recurse_clause(predicate, lfirst(item)))
				return false;
		}
		return true;
	}
	else if (and_clause(clause))
	{
		items = ((Expr *) clause)->args;
		foreach(item, items)
		{
			/*
			 * if any AND item implies the predicate, the whole clause
			 * does
			 */
			if (pred_test_recurse_clause(predicate, lfirst(item)))
				return true;
		}
		return false;
	}
	else
		return pred_test_recurse_pred(predicate, clause);
}


/*
 * pred_test_recurse_pred
 *	  Does the "predicate inclusion test" for one conjunct of a predicate
 *	  expression for a simple restriction clause.  Here we recursively deal
 *	  with the possibility that the predicate conjunct is itself an AND or
 *	  OR structure.
 */
static bool
pred_test_recurse_pred(Expr *predicate, Node *clause)
{
	List	   *items,
			   *item;

	Assert(predicate != NULL);
	if (or_clause((Node *) predicate))
	{
		items = predicate->args;
		foreach(item, items)
		{
			/* if any item is implied, the whole predicate is implied */
			if (pred_test_recurse_pred(lfirst(item), clause))
				return true;
		}
		return false;
	}
	else if (and_clause((Node *) predicate))
	{
		items = predicate->args;
		foreach(item, items)
		{
			/*
			 * if any item is not implied, the whole predicate is not
			 * implied
			 */
			if (!pred_test_recurse_pred(lfirst(item), clause))
				return false;
		}
		return true;
	}
	else
		return pred_test_simple_clause(predicate, clause);
}


/*
 * Define an "operator implication table" for btree operators ("strategies").
 * The "strategy numbers" are:	(1) <	(2) <=	 (3) =	 (4) >=   (5) >
 *
 * The interpretation of:
 *
 *		test_op = BT_implic_table[given_op-1][target_op-1]
 *
 * where test_op, given_op and target_op are strategy numbers (from 1 to 5)
 * of btree operators, is as follows:
 *
 *	 If you know, for some ATTR, that "ATTR given_op CONST1" is true, and you
 *	 want to determine whether "ATTR target_op CONST2" must also be true, then
 *	 you can use "CONST1 test_op CONST2" as a test.  If this test returns true,
 *	 then the target expression must be true; if the test returns false, then
 *	 the target expression may be false.
 *
 * An entry where test_op==0 means the implication cannot be determined, i.e.,
 * this test should always be considered false.
 */

static const StrategyNumber
			BT_implic_table[BTMaxStrategyNumber][BTMaxStrategyNumber] = {
	{2, 2, 0, 0, 0},
	{1, 2, 0, 0, 0},
	{1, 2, 3, 4, 5},
	{0, 0, 0, 4, 5},
	{0, 0, 0, 4, 4}
};


/*
 * pred_test_simple_clause
 *	  Does the "predicate inclusion test" for a "simple clause" predicate
 *	  and a "simple clause" restriction.
 *
 *	  We have two strategies for determining whether one simple clause
 *	  implies another.	A simple and general way is to see if they are
 *	  equal(); this works for any kind of expression.  (Actually, there
 *	  is an implied assumption that the functions in the expression are
 *	  cachable, ie dependent only on their input arguments --- but this
 *	  was checked for the predicate by CheckPredicate().)
 *
 *	  Our other way works only for (binary boolean) operators that are
 *	  in some btree operator class.  We use the above operator implication
 *	  table to be able to derive implications between nonidentical clauses.
 *
 *	  Eventually, rtree operators could also be handled by defining an
 *	  appropriate "RT_implic_table" array.
 */
static bool
pred_test_simple_clause(Expr *predicate, Node *clause)
{
	Var		   *pred_var,
			   *clause_var;
	Const	   *pred_const,
			   *clause_const;
	Oid			pred_op,
				clause_op,
				test_op;
	Oid			opclass_id = InvalidOid;
	StrategyNumber pred_strategy = 0,
				clause_strategy,
				test_strategy;
	Oper	   *test_oper;
	Expr	   *test_expr;
	Datum		test_result;
	bool		isNull;
	Relation	relation;
	HeapScanDesc scan;
	HeapTuple	tuple;
	ScanKeyData entry[1];
	Form_pg_amop aform;
	ExprContext *econtext;

	/* First try the equal() test */
	if (equal((Node *) predicate, clause))
		return true;

	/*
	 * Can't do anything more unless they are both binary opclauses with a
	 * Var on the left and a Const on the right.
	 */
	if (!is_opclause((Node *) predicate))
		return false;
	pred_var = (Var *) get_leftop(predicate);
	pred_const = (Const *) get_rightop(predicate);

	if (!is_opclause(clause))
		return false;
	clause_var = (Var *) get_leftop((Expr *) clause);
	clause_const = (Const *) get_rightop((Expr *) clause);

	if (!IsA(clause_var, Var) ||
		clause_const == NULL ||
		!IsA(clause_const, Const) ||
		!IsA(pred_var, Var) ||
		pred_const == NULL ||
		!IsA(pred_const, Const))
		return false;

	/*
	 * The implication can't be determined unless the predicate and the
	 * clause refer to the same attribute.
	 */
	if (clause_var->varno != pred_var->varno ||
		clause_var->varattno != pred_var->varattno)
		return false;

	/* Get the operators for the two clauses we're comparing */
	pred_op = ((Oper *) ((Expr *) predicate)->oper)->opno;
	clause_op = ((Oper *) ((Expr *) clause)->oper)->opno;

	/*
	 * 1. Find a "btree" strategy number for the pred_op
	 *
	 * The following assumes that any given operator will only be in a single
	 * btree operator class.  This is true at least for all the
	 * pre-defined operator classes.  If it isn't true, then whichever
	 * operator class happens to be returned first for the given operator
	 * will be used to find the associated strategy numbers for the test.
	 * --Nels, Jan '93
	 */
	ScanKeyEntryInitialize(&entry[0], 0x0,
						   Anum_pg_amop_amopopr,
						   F_OIDEQ,
						   ObjectIdGetDatum(pred_op));

	relation = heap_openr(AccessMethodOperatorRelationName, AccessShareLock);
	scan = heap_beginscan(relation, false, SnapshotNow, 1, entry);

	while (HeapTupleIsValid(tuple = heap_getnext(scan, 0)))
	{
		aform = (Form_pg_amop) GETSTRUCT(tuple);
		if (opclass_is_btree(aform->amopclaid))
		{
			/* Get the predicate operator's btree strategy number (1 to 5) */
			pred_strategy = (StrategyNumber) aform->amopstrategy;
			Assert(pred_strategy >= 1 && pred_strategy <= 5);

			/*
			 * Remember which operator class this strategy number came
			 * from
			 */
			opclass_id = aform->amopclaid;
			break;
		}
	}

	heap_endscan(scan);
	heap_close(relation, AccessShareLock);

	if (!OidIsValid(opclass_id))
	{
		/* predicate operator isn't btree-indexable */
		return false;
	}

	/*
	 * 2. From the same opclass, find a strategy num for the clause_op
	 */
	tuple = SearchSysCache(AMOPOPID,
						   ObjectIdGetDatum(opclass_id),
						   ObjectIdGetDatum(clause_op),
						   0, 0);
	if (!HeapTupleIsValid(tuple))
	{
		/* clause operator isn't btree-indexable, or isn't in this opclass */
		return false;
	}
	aform = (Form_pg_amop) GETSTRUCT(tuple);

	/* Get the restriction clause operator's strategy number (1 to 5) */
	clause_strategy = (StrategyNumber) aform->amopstrategy;
	Assert(clause_strategy >= 1 && clause_strategy <= 5);

	ReleaseSysCache(tuple);

	/*
	 * 3. Look up the "test" strategy number in the implication table
	 */
	test_strategy = BT_implic_table[clause_strategy - 1][pred_strategy - 1];
	if (test_strategy == 0)
	{
		return false;			/* the implication cannot be determined */
	}

	/*
	 * 4. From the same opclass, find the operator for the test strategy
	 */
	tuple = SearchSysCache(AMOPSTRATEGY,
						   ObjectIdGetDatum(opclass_id),
						   Int16GetDatum(test_strategy),
						   0, 0);
	if (!HeapTupleIsValid(tuple))
	{
		/* this probably shouldn't fail? */
		elog(DEBUG, "pred_test_simple_clause: unknown test_op");
		return false;
	}
	aform = (Form_pg_amop) GETSTRUCT(tuple);

	/* Get the test operator */
	test_op = aform->amopopr;

	ReleaseSysCache(tuple);

	/*
	 * 5. Evaluate the test
	 */
	test_oper = makeOper(test_op,		/* opno */
						 InvalidOid,	/* opid */
						 BOOLOID);		/* opresulttype */
	replace_opid(test_oper);
	test_expr = make_opclause(test_oper,
							  (Var *) clause_const,
							  (Var *) pred_const);

	econtext = MakeExprContext(NULL, TransactionCommandContext);
	test_result = ExecEvalExprSwitchContext((Node *) test_expr, econtext,
											&isNull, NULL);
	FreeExprContext(econtext);

	if (isNull)
	{
		elog(DEBUG, "pred_test_simple_clause: null test result");
		return false;
	}
	return DatumGetBool(test_result);
}


/****************************************************************************
 *				----  ROUTINES TO CHECK JOIN CLAUSES  ----
 ****************************************************************************/

/*
 * indexable_joinclauses
 *	  Finds all groups of join clauses from among 'joininfo_list' that can
 *	  be used in conjunction with 'index' for the inner scan of a nestjoin.
 *
 *	  Each clause group comes from a single joininfo node plus the current
 *	  rel's restrictinfo list.  Therefore, every clause in the group references
 *	  the current rel plus the same set of other rels (except for the restrict
 *	  clauses, which only reference the current rel).  Therefore, this set
 *	  of clauses could be used as an indexqual if the relation is scanned
 *	  as the inner side of a nestloop join when the outer side contains
 *	  (at least) all those "other rels".
 *
 *	  XXX Actually, given that we are considering a join that requires an
 *	  outer rel set (A,B,C), we should use all qual clauses that reference
 *	  any subset of these rels, not just the full set or none.	This is
 *	  doable with a doubly nested loop over joininfo_list; is it worth it?
 *
 * Returns two parallel lists of the same length: the clause groups,
 * and the required outer rel set for each one.
 *
 * 'rel' is the relation for which 'index' is defined
 * 'joininfo_list' is the list of JoinInfo nodes for 'rel'
 * 'restrictinfo_list' is the list of restriction clauses for 'rel'
 * '*clausegroups' receives a list of clause sublists
 * '*outerrelids' receives a list of relid lists
 */
static void
indexable_joinclauses(RelOptInfo *rel, IndexOptInfo *index,
					  List *joininfo_list, List *restrictinfo_list,
					  List **clausegroups, List **outerrelids)
{
	List	   *cg_list = NIL;
	List	   *relid_list = NIL;
	List	   *i;

	foreach(i, joininfo_list)
	{
		JoinInfo   *joininfo = (JoinInfo *) lfirst(i);
		List	   *clausegroup;

		clausegroup = group_clauses_by_ikey_for_joins(rel,
													  index,
													  index->indexkeys,
													  index->classlist,
											joininfo->jinfo_restrictinfo,
													  restrictinfo_list);

		if (clausegroup != NIL)
		{
			cg_list = lappend(cg_list, clausegroup);
			relid_list = lappend(relid_list, joininfo->unjoined_relids);
		}
	}

	*clausegroups = cg_list;
	*outerrelids = relid_list;
}

/****************************************************************************
 *				----  PATH CREATION UTILITIES  ----
 ****************************************************************************/

/*
 * index_innerjoin
 *	  Creates index path nodes corresponding to paths to be used as inner
 *	  relations in nestloop joins.
 *
 * 'rel' is the relation for which 'index' is defined
 * 'clausegroup_list' is a list of lists of restrictinfo nodes which can use
 * 'index'.  Each sublist refers to the same set of outer rels.
 * 'outerrelids_list' is a list of the required outer rels for each sublist
 * of join clauses.
 *
 * Returns a list of index pathnodes.
 */
static List *
index_innerjoin(Query *root, RelOptInfo *rel, IndexOptInfo *index,
				List *clausegroup_list, List *outerrelids_list)
{
	List	   *path_list = NIL;
	List	   *i;

	foreach(i, clausegroup_list)
	{
		List	   *clausegroup = lfirst(i);
		IndexPath  *pathnode = makeNode(IndexPath);
		List	   *indexquals = NIL;
		bool		alljoinquals = true;
		List	   *temp;

		/* XXX this code ought to be merged with create_index_path? */

		pathnode->path.pathtype = T_IndexScan;
		pathnode->path.parent = rel;

		/*
		 * There's no point in marking the path with any pathkeys, since
		 * it will only ever be used as the inner path of a nestloop, and
		 * so its ordering does not matter.
		 */
		pathnode->path.pathkeys = NIL;

		/* extract bare indexqual clauses, check whether all from JOIN/ON */
		foreach(temp, clausegroup)
		{
			RestrictInfo *clause = (RestrictInfo *) lfirst(temp);

			indexquals = lappend(indexquals, clause->clause);
			if (clause->ispusheddown)
				alljoinquals = false;
		}

		/* expand special operators to indexquals the executor can handle */
		indexquals = expand_indexqual_conditions(indexquals);

		/*
		 * Note that we are making a pathnode for a single-scan indexscan;
		 * therefore, both indexinfo and indexqual should be
		 * single-element lists.
		 */
		pathnode->indexinfo = makeList1(index);
		pathnode->indexqual = makeList1(indexquals);

		/* We don't actually care what order the index scans in ... */
		pathnode->indexscandir = NoMovementScanDirection;

		/* joinrelids saves the rels needed on the outer side of the join */
		pathnode->joinrelids = lfirst(outerrelids_list);

		pathnode->alljoinquals = alljoinquals;

		/*
		 * We must compute the estimated number of output rows for the
		 * indexscan.  This is less than rel->rows because of the
		 * additional selectivity of the join clauses.	Since clausegroup
		 * may contain both restriction and join clauses, we have to do a
		 * set union to get the full set of clauses that must be
		 * considered to compute the correct selectivity.  (We can't just
		 * nconc the two lists; then we might have some restriction
		 * clauses appearing twice, which'd mislead
		 * restrictlist_selectivity into double-counting their
		 * selectivity.)
		 */
		pathnode->rows = rel->tuples *
			restrictlist_selectivity(root,
									 set_union(rel->baserestrictinfo,
											   clausegroup),
									 lfirsti(rel->relids));
		/* Like costsize.c, force estimate to be at least one row */
		if (pathnode->rows < 1.0)
			pathnode->rows = 1.0;

		cost_index(&pathnode->path, root, rel, index, indexquals, true);

		path_list = lappend(path_list, pathnode);
		outerrelids_list = lnext(outerrelids_list);
	}
	return path_list;
}

/****************************************************************************
 *				----  ROUTINES TO CHECK OPERANDS  ----
 ****************************************************************************/

/*
 * match_index_to_operand()
 *	  Generalized test for a match between an index's key
 *	  and the operand on one side of a restriction or join clause.
 *	  Now check for functional indices as well.
 */
static bool
match_index_to_operand(int indexkey,
					   Var *operand,
					   RelOptInfo *rel,
					   IndexOptInfo *index)
{
	/*
	 * Ignore any RelabelType node above the indexkey.	This is needed to
	 * be able to apply indexscanning in binary-compatible-operator cases.
	 * Note: we can assume there is at most one RelabelType node;
	 * eval_const_expressions() will have simplified if more than one.
	 */
	if (operand && IsA(operand, RelabelType))
		operand = (Var *) ((RelabelType *) operand)->arg;

	if (index->indproc == InvalidOid)
	{
		/*
		 * Simple index.
		 */
		if (operand && IsA(operand, Var) &&
			lfirsti(rel->relids) == operand->varno &&
			indexkey == operand->varattno)
			return true;
		else
			return false;
	}

	/*
	 * Functional index.
	 */
	return function_index_operand((Expr *) operand, rel, index);
}

static bool
function_index_operand(Expr *funcOpnd, RelOptInfo *rel, IndexOptInfo *index)
{
	int			relvarno = lfirsti(rel->relids);
	Func	   *function;
	List	   *funcargs;
	int		   *indexKeys = index->indexkeys;
	List	   *arg;
	int			i;

	/*
	 * sanity check, make sure we know what we're dealing with here.
	 */
	if (funcOpnd == NULL || !IsA(funcOpnd, Expr) ||
		funcOpnd->opType != FUNC_EXPR ||
		funcOpnd->oper == NULL || indexKeys == NULL)
		return false;

	function = (Func *) funcOpnd->oper;
	funcargs = funcOpnd->args;

	if (function->funcid != index->indproc)
		return false;

	/*----------
	 * Check that the arguments correspond to the same arguments used to
	 * create the functional index.  To do this we must check that
	 *	1. they refer to the right relation.
	 *	2. the args have the right attr. numbers in the right order.
	 * We must ignore RelabelType nodes above the argument Vars in order
	 * to recognize binary-compatible-function cases correctly.
	 *----------
	 */
	i = 0;
	foreach(arg, funcargs)
	{
		Var		   *var = (Var *) lfirst(arg);

		if (var && IsA(var, RelabelType))
			var = (Var *) ((RelabelType *) var)->arg;
		if (var == NULL || !IsA(var, Var))
			return false;
		if (indexKeys[i] == 0)
			return false;
		if (var->varno != relvarno || var->varattno != indexKeys[i])
			return false;

		i++;
	}

	if (indexKeys[i] != 0)
		return false;			/* not enough arguments */

	return true;
}

/****************************************************************************
 *			----  ROUTINES FOR "SPECIAL" INDEXABLE OPERATORS  ----
 ****************************************************************************/

/*----------
 * These routines handle special optimization of operators that can be
 * used with index scans even though they are not known to the executor's
 * indexscan machinery.  The key idea is that these operators allow us
 * to derive approximate indexscan qual clauses, such that any tuples
 * that pass the operator clause itself must also satisfy the simpler
 * indexscan condition(s).	Then we can use the indexscan machinery
 * to avoid scanning as much of the table as we'd otherwise have to,
 * while applying the original operator as a qpqual condition to ensure
 * we deliver only the tuples we want.	(In essence, we're using a regular
 * index as if it were a lossy index.)
 *
 * An example of what we're doing is
 *			textfield LIKE 'abc%'
 * from which we can generate the indexscanable conditions
 *			textfield >= 'abc' AND textfield < 'abd'
 * which allow efficient scanning of an index on textfield.
 * (In reality, character set and collation issues make the transformation
 * from LIKE to indexscan limits rather harder than one might think ...
 * but that's the basic idea.)
 *
 * Two routines are provided here, match_special_index_operator() and
 * expand_indexqual_conditions().  match_special_index_operator() is
 * just an auxiliary function for match_clause_to_indexkey(); after
 * the latter fails to recognize a restriction opclause's operator
 * as a member of an index's opclass, it asks match_special_index_operator()
 * whether the clause should be considered an indexqual anyway.
 * expand_indexqual_conditions() converts a list of "raw" indexqual
 * conditions (with implicit AND semantics across list elements) into
 * a list that the executor can actually handle.  For operators that
 * are members of the index's opclass this transformation is a no-op,
 * but operators recognized by match_special_index_operator() must be
 * converted into one or more "regular" indexqual conditions.
 *----------
 */

/*
 * match_special_index_operator
 *	  Recognize restriction clauses that can be used to generate
 *	  additional indexscanable qualifications.
 *
 * The given clause is already known to be a binary opclause having
 * the form (indexkey OP pseudoconst) or (pseudoconst OP indexkey),
 * but the OP proved not to be one of the index's opclass operators.
 * Return 'true' if we can do something with it anyway.
 */
static bool
match_special_index_operator(Expr *clause, Oid opclass,
							 bool indexkey_on_left)
{
	bool		isIndexable = false;
	Var		   *leftop,
			   *rightop;
	Oid			expr_op;
	Datum		constvalue;
	char	   *patt;
	char	   *prefix;
	char	   *rest;

	/*
	 * Currently, all known special operators require the indexkey on the
	 * left, but this test could be pushed into the switch statement if
	 * some are added that do not...
	 */
	if (!indexkey_on_left)
		return false;

	/* we know these will succeed */
	leftop = get_leftop(clause);
	rightop = get_rightop(clause);
	expr_op = ((Oper *) clause->oper)->opno;

	/* again, required for all current special ops: */
	if (!IsA(rightop, Const) ||
		((Const *) rightop)->constisnull)
		return false;
	constvalue = ((Const *) rightop)->constvalue;

	switch (expr_op)
	{
		case OID_TEXT_LIKE_OP:
		case OID_BPCHAR_LIKE_OP:
		case OID_VARCHAR_LIKE_OP:
		case OID_NAME_LIKE_OP:
			if (locale_is_like_safe())
			{
				/* the right-hand const is type text for all of these */
				patt = DatumGetCString(DirectFunctionCall1(textout,
														   constvalue));
				isIndexable = pattern_fixed_prefix(patt, Pattern_Type_Like,
								  &prefix, &rest) != Pattern_Prefix_None;
				if (prefix)
					pfree(prefix);
				pfree(patt);
			}
			break;

		case OID_TEXT_ICLIKE_OP:
		case OID_BPCHAR_ICLIKE_OP:
		case OID_VARCHAR_ICLIKE_OP:
		case OID_NAME_ICLIKE_OP:
			if (locale_is_like_safe())
			{
				/* the right-hand const is type text for all of these */
				patt = DatumGetCString(DirectFunctionCall1(textout,
														   constvalue));
				isIndexable = pattern_fixed_prefix(patt, Pattern_Type_Like_IC,
								  &prefix, &rest) != Pattern_Prefix_None;
				if (prefix)
					pfree(prefix);
				pfree(patt);
			}
			break;

		case OID_TEXT_REGEXEQ_OP:
		case OID_BPCHAR_REGEXEQ_OP:
		case OID_VARCHAR_REGEXEQ_OP:
		case OID_NAME_REGEXEQ_OP:
			if (locale_is_like_safe())
			{
				/* the right-hand const is type text for all of these */
				patt = DatumGetCString(DirectFunctionCall1(textout,
														   constvalue));
				isIndexable = pattern_fixed_prefix(patt, Pattern_Type_Regex,
								  &prefix, &rest) != Pattern_Prefix_None;
				if (prefix)
					pfree(prefix);
				pfree(patt);
			}
			break;

		case OID_TEXT_ICREGEXEQ_OP:
		case OID_BPCHAR_ICREGEXEQ_OP:
		case OID_VARCHAR_ICREGEXEQ_OP:
		case OID_NAME_ICREGEXEQ_OP:
			if (locale_is_like_safe())
			{
				/* the right-hand const is type text for all of these */
				patt = DatumGetCString(DirectFunctionCall1(textout,
														   constvalue));
				isIndexable = pattern_fixed_prefix(patt, Pattern_Type_Regex_IC,
								  &prefix, &rest) != Pattern_Prefix_None;
				if (prefix)
					pfree(prefix);
				pfree(patt);
			}
			break;

		case OID_INET_SUB_OP:
		case OID_INET_SUBEQ_OP:
		case OID_CIDR_SUB_OP:
		case OID_CIDR_SUBEQ_OP:
			isIndexable = true;
			break;
	}

	/* done if the expression doesn't look indexable */
	if (!isIndexable)
		return false;

	/*
	 * Must also check that index's opclass supports the operators we will
	 * want to apply.  (A hash index, for example, will not support ">=".)
	 * We cheat a little by not checking for availability of "=" ... any
	 * index type should support "=", methinks.
	 */
	switch (expr_op)
	{
		case OID_TEXT_LIKE_OP:
		case OID_TEXT_ICLIKE_OP:
		case OID_TEXT_REGEXEQ_OP:
		case OID_TEXT_ICREGEXEQ_OP:
			if (!op_in_opclass(find_operator(">=", TEXTOID), opclass) ||
				!op_in_opclass(find_operator("<", TEXTOID), opclass))
				isIndexable = false;
			break;

		case OID_BPCHAR_LIKE_OP:
		case OID_BPCHAR_ICLIKE_OP:
		case OID_BPCHAR_REGEXEQ_OP:
		case OID_BPCHAR_ICREGEXEQ_OP:
			if (!op_in_opclass(find_operator(">=", BPCHAROID), opclass) ||
				!op_in_opclass(find_operator("<", BPCHAROID), opclass))
				isIndexable = false;
			break;

		case OID_VARCHAR_LIKE_OP:
		case OID_VARCHAR_ICLIKE_OP:
		case OID_VARCHAR_REGEXEQ_OP:
		case OID_VARCHAR_ICREGEXEQ_OP:
			if (!op_in_opclass(find_operator(">=", VARCHAROID), opclass) ||
				!op_in_opclass(find_operator("<", VARCHAROID), opclass))
				isIndexable = false;
			break;

		case OID_NAME_LIKE_OP:
		case OID_NAME_ICLIKE_OP:
		case OID_NAME_REGEXEQ_OP:
		case OID_NAME_ICREGEXEQ_OP:
			if (!op_in_opclass(find_operator(">=", NAMEOID), opclass) ||
				!op_in_opclass(find_operator("<", NAMEOID), opclass))
				isIndexable = false;
			break;

		case OID_INET_SUB_OP:
		case OID_INET_SUBEQ_OP:
			/* for SUB we actually need ">" not ">=", but this should do */
			if (!op_in_opclass(find_operator(">=", INETOID), opclass) ||
				!op_in_opclass(find_operator("<=", INETOID), opclass))
				isIndexable = false;
			break;

		case OID_CIDR_SUB_OP:
		case OID_CIDR_SUBEQ_OP:
			/* for SUB we actually need ">" not ">=", but this should do */
			if (!op_in_opclass(find_operator(">=", CIDROID), opclass) ||
				!op_in_opclass(find_operator("<=", CIDROID), opclass))
				isIndexable = false;
			break;
	}

	return isIndexable;
}

/*
 * expand_indexqual_conditions
 *	  Given a list of (implicitly ANDed) indexqual clauses,
 *	  expand any "special" index operators into clauses that the indexscan
 *	  machinery will know what to do with.	Clauses that were not
 *	  recognized by match_special_index_operator() must be passed through
 *	  unchanged.
 */
List *
expand_indexqual_conditions(List *indexquals)
{
	List	   *resultquals = NIL;
	List	   *q;

	foreach(q, indexquals)
	{
		Expr	   *clause = (Expr *) lfirst(q);

		/* we know these will succeed */
		Var		   *leftop = get_leftop(clause);
		Var		   *rightop = get_rightop(clause);
		Oid			expr_op = ((Oper *) clause->oper)->opno;
		Datum		constvalue;
		char	   *patt;
		char	   *prefix;
		char	   *rest;
		Pattern_Prefix_Status pstatus;

		switch (expr_op)
		{
				/*
				 * LIKE and regex operators are not members of any index
				 * opclass, so if we find one in an indexqual list we can
				 * assume that it was accepted by
				 * match_special_index_operator().
				 */
			case OID_TEXT_LIKE_OP:
			case OID_BPCHAR_LIKE_OP:
			case OID_VARCHAR_LIKE_OP:
			case OID_NAME_LIKE_OP:
				/* the right-hand const is type text for all of these */
				constvalue = ((Const *) rightop)->constvalue;
				patt = DatumGetCString(DirectFunctionCall1(textout,
														   constvalue));
				pstatus = pattern_fixed_prefix(patt, Pattern_Type_Like,
											   &prefix, &rest);
				resultquals = nconc(resultquals,
									prefix_quals(leftop, expr_op,
												 prefix, pstatus));
				if (prefix)
					pfree(prefix);
				pfree(patt);
				break;

			case OID_TEXT_ICLIKE_OP:
			case OID_BPCHAR_ICLIKE_OP:
			case OID_VARCHAR_ICLIKE_OP:
			case OID_NAME_ICLIKE_OP:
				/* the right-hand const is type text for all of these */
				constvalue = ((Const *) rightop)->constvalue;
				patt = DatumGetCString(DirectFunctionCall1(textout,
														   constvalue));
				pstatus = pattern_fixed_prefix(patt, Pattern_Type_Like_IC,
											   &prefix, &rest);
				resultquals = nconc(resultquals,
									prefix_quals(leftop, expr_op,
												 prefix, pstatus));
				if (prefix)
					pfree(prefix);
				pfree(patt);
				break;

			case OID_TEXT_REGEXEQ_OP:
			case OID_BPCHAR_REGEXEQ_OP:
			case OID_VARCHAR_REGEXEQ_OP:
			case OID_NAME_REGEXEQ_OP:
				/* the right-hand const is type text for all of these */
				constvalue = ((Const *) rightop)->constvalue;
				patt = DatumGetCString(DirectFunctionCall1(textout,
														   constvalue));
				pstatus = pattern_fixed_prefix(patt, Pattern_Type_Regex,
											   &prefix, &rest);
				resultquals = nconc(resultquals,
									prefix_quals(leftop, expr_op,
												 prefix, pstatus));
				if (prefix)
					pfree(prefix);
				pfree(patt);
				break;

			case OID_TEXT_ICREGEXEQ_OP:
			case OID_BPCHAR_ICREGEXEQ_OP:
			case OID_VARCHAR_ICREGEXEQ_OP:
			case OID_NAME_ICREGEXEQ_OP:
				/* the right-hand const is type text for all of these */
				constvalue = ((Const *) rightop)->constvalue;
				patt = DatumGetCString(DirectFunctionCall1(textout,
														   constvalue));
				pstatus = pattern_fixed_prefix(patt, Pattern_Type_Regex_IC,
											   &prefix, &rest);
				resultquals = nconc(resultquals,
									prefix_quals(leftop, expr_op,
												 prefix, pstatus));
				if (prefix)
					pfree(prefix);
				pfree(patt);
				break;

			case OID_INET_SUB_OP:
			case OID_INET_SUBEQ_OP:
			case OID_CIDR_SUB_OP:
			case OID_CIDR_SUBEQ_OP:
				constvalue = ((Const *) rightop)->constvalue;
				resultquals = nconc(resultquals,
									network_prefix_quals(leftop, expr_op,
														 constvalue));
				break;

			default:
				resultquals = lappend(resultquals, clause);
				break;
		}
	}

	return resultquals;
}

/*
 * Given a fixed prefix that all the "leftop" values must have,
 * generate suitable indexqual condition(s).  expr_op is the original
 * LIKE or regex operator; we use it to deduce the appropriate comparison
 * operators.
 */
static List *
prefix_quals(Var *leftop, Oid expr_op,
			 char *prefix, Pattern_Prefix_Status pstatus)
{
	List	   *result;
	Oid			datatype;
	Oid			oproid;
	Const	   *con;
	Oper	   *op;
	Expr	   *expr;
	char	   *greaterstr;

	Assert(pstatus != Pattern_Prefix_None);

	switch (expr_op)
	{
		case OID_TEXT_LIKE_OP:
		case OID_TEXT_ICLIKE_OP:
		case OID_TEXT_REGEXEQ_OP:
		case OID_TEXT_ICREGEXEQ_OP:
			datatype = TEXTOID;
			break;

		case OID_BPCHAR_LIKE_OP:
		case OID_BPCHAR_ICLIKE_OP:
		case OID_BPCHAR_REGEXEQ_OP:
		case OID_BPCHAR_ICREGEXEQ_OP:
			datatype = BPCHAROID;
			break;

		case OID_VARCHAR_LIKE_OP:
		case OID_VARCHAR_ICLIKE_OP:
		case OID_VARCHAR_REGEXEQ_OP:
		case OID_VARCHAR_ICREGEXEQ_OP:
			datatype = VARCHAROID;
			break;

		case OID_NAME_LIKE_OP:
		case OID_NAME_ICLIKE_OP:
		case OID_NAME_REGEXEQ_OP:
		case OID_NAME_ICREGEXEQ_OP:
			datatype = NAMEOID;
			break;

		default:
			elog(ERROR, "prefix_quals: unexpected operator %u", expr_op);
			return NIL;
	}

	/*
	 * If we found an exact-match pattern, generate an "=" indexqual.
	 */
	if (pstatus == Pattern_Prefix_Exact)
	{
		oproid = find_operator("=", datatype);
		if (oproid == InvalidOid)
			elog(ERROR, "prefix_quals: no = operator for type %u", datatype);
		con = string_to_const(prefix, datatype);
		op = makeOper(oproid, InvalidOid, BOOLOID);
		expr = make_opclause(op, leftop, (Var *) con);
		result = makeList1(expr);
		return result;
	}

	/*
	 * Otherwise, we have a nonempty required prefix of the values.
	 *
	 * We can always say "x >= prefix".
	 */
	oproid = find_operator(">=", datatype);
	if (oproid == InvalidOid)
		elog(ERROR, "prefix_quals: no >= operator for type %u", datatype);
	con = string_to_const(prefix, datatype);
	op = makeOper(oproid, InvalidOid, BOOLOID);
	expr = make_opclause(op, leftop, (Var *) con);
	result = makeList1(expr);

	/*-------
	 * If we can create a string larger than the prefix, we can say
	 * "x < greaterstr".
	 *-------
	 */
	greaterstr = make_greater_string(prefix, datatype);
	if (greaterstr)
	{
		oproid = find_operator("<", datatype);
		if (oproid == InvalidOid)
			elog(ERROR, "prefix_quals: no < operator for type %u", datatype);
		con = string_to_const(greaterstr, datatype);
		op = makeOper(oproid, InvalidOid, BOOLOID);
		expr = make_opclause(op, leftop, (Var *) con);
		result = lappend(result, expr);
		pfree(greaterstr);
	}

	return result;
}

/*
 * Given a leftop and a rightop, and a inet-class sup/sub operator,
 * generate suitable indexqual condition(s).  expr_op is the original
 * operator.
 */
static List *
network_prefix_quals(Var *leftop, Oid expr_op, Datum rightop)
{
	bool		is_eq;
	char	   *opr1name;
	Datum		opr1right;
	Datum		opr2right;
	Oid			opr1oid;
	Oid			opr2oid;
	List	   *result;
	Oid			datatype;
	Oper	   *op;
	Expr	   *expr;

	switch (expr_op)
	{
		case OID_INET_SUB_OP:
			datatype = INETOID;
			is_eq = false;
			break;
		case OID_INET_SUBEQ_OP:
			datatype = INETOID;
			is_eq = true;
			break;
		case OID_CIDR_SUB_OP:
			datatype = CIDROID;
			is_eq = false;
			break;
		case OID_CIDR_SUBEQ_OP:
			datatype = CIDROID;
			is_eq = true;
			break;
		default:
			elog(ERROR, "network_prefix_quals: unexpected operator %u",
				 expr_op);
			return NIL;
	}

	/*
	 * create clause "key >= network_scan_first( rightop )", or ">" if the
	 * operator disallows equality.
	 */

	opr1name = is_eq ? ">=" : ">";
	opr1oid = find_operator(opr1name, datatype);
	if (opr1oid == InvalidOid)
		elog(ERROR, "network_prefix_quals: no %s operator for type %u",
			 opr1name, datatype);

	opr1right = network_scan_first(rightop);

	op = makeOper(opr1oid, InvalidOid, BOOLOID);
	expr = make_opclause(op, leftop,
						 (Var *) makeConst(datatype, -1, opr1right,
										   false, false, false, false));
	result = makeList1(expr);

	/* create clause "key <= network_scan_last( rightop )" */

	opr2oid = find_operator("<=", datatype);
	if (opr2oid == InvalidOid)
		elog(ERROR, "network_prefix_quals: no <= operator for type %u",
			 datatype);

	opr2right = network_scan_last(rightop);

	op = makeOper(opr2oid, InvalidOid, BOOLOID);
	expr = make_opclause(op, leftop,
						 (Var *) makeConst(datatype, -1, opr2right,
										   false, false, false, false));
	result = lappend(result, expr);

	return result;
}

/*
 * Handy subroutines for match_special_index_operator() and friends.
 */

/* See if there is a binary op of the given name for the given datatype */
static Oid
find_operator(const char *opname, Oid datatype)
{
	return GetSysCacheOid(OPERNAME,
						  PointerGetDatum(opname),
						  ObjectIdGetDatum(datatype),
						  ObjectIdGetDatum(datatype),
						  CharGetDatum('b'));
}

/*
 * Generate a Datum of the appropriate type from a C string.
 * Note that all of the supported types are pass-by-ref, so the
 * returned value should be pfree'd if no longer needed.
 */
static Datum
string_to_datum(const char *str, Oid datatype)
{
	/*
	 * We cheat a little by assuming that textin() will do for bpchar and
	 * varchar constants too...
	 */
	if (datatype == NAMEOID)
		return DirectFunctionCall1(namein, CStringGetDatum(str));
	else
		return DirectFunctionCall1(textin, CStringGetDatum(str));
}

/*
 * Generate a Const node of the appropriate type from a C string.
 */
static Const *
string_to_const(const char *str, Oid datatype)
{
	Datum		conval = string_to_datum(str, datatype);

	return makeConst(datatype, ((datatype == NAMEOID) ? NAMEDATALEN : -1),
					 conval, false, false, false, false);
}
