/*-------------------------------------------------------------------------
 *
 * paths.h
 *	  prototypes for various files in optimizer/path (were separate
 *	  header files)
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: paths.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PATHS_H
#define PATHS_H

#include "nodes/relation.h"

/* default GEQO threshold (default value for geqo_rels) */
/* If you change this, update backend/utils/misc/postgresql.sample.conf */
#define DEFAULT_GEQO_RELS 11


/*
 * allpaths.c
 */
extern bool enable_geqo;
extern int	geqo_rels;

extern RelOptInfo *make_one_rel(Query *root);
extern RelOptInfo *make_fromexpr_rel(Query *root, FromExpr *from);

#ifdef OPTIMIZER_DEBUG
extern void debug_print_rel(Query *root, RelOptInfo *rel);
#endif

/*
 * indxpath.c
 *	  routines to generate index paths
 */
extern void create_index_paths(Query *root, RelOptInfo *rel);
extern Oid indexable_operator(Expr *clause, Oid opclass,
				   bool indexkey_on_left);
extern List *extract_or_indexqual_conditions(RelOptInfo *rel,
								IndexOptInfo *index,
								Expr *orsubclause);
extern List *expand_indexqual_conditions(List *indexquals);

/*
 * orindxpath.c
 *	  additional routines for indexable OR clauses
 */
extern void create_or_index_paths(Query *root, RelOptInfo *rel);

/*
 * tidpath.h
 *	  routines to generate tid paths
 */
extern void create_tidscan_paths(Query *root, RelOptInfo *rel);

/*
 * joinpath.c
 *	   routines to create join paths
 */
extern void add_paths_to_joinrel(Query *root, RelOptInfo *joinrel,
					 RelOptInfo *outerrel,
					 RelOptInfo *innerrel,
					 JoinType jointype,
					 List *restrictlist);

/*
 * joinrels.c
 *	  routines to determine which relations to join
 */
extern List *make_rels_by_joins(Query *root, int level, List **joinrels);
extern List *make_rels_by_clause_joins(Query *root,
						  RelOptInfo *old_rel,
						  List *other_rels);
extern List *make_rels_by_clauseless_joins(Query *root,
							  RelOptInfo *old_rel,
							  List *other_rels);
extern RelOptInfo *make_jointree_rel(Query *root, Node *jtnode);

/*
 * pathkeys.c
 *	  utilities for matching and building path keys
 */
typedef enum
{
	PATHKEYS_EQUAL,				/* pathkeys are identical */
	PATHKEYS_BETTER1,			/* pathkey 1 is a superset of pathkey 2 */
	PATHKEYS_BETTER2,			/* vice versa */
	PATHKEYS_DIFFERENT			/* neither pathkey includes the other */
} PathKeysComparison;

extern void add_equijoined_keys(Query *root, RestrictInfo *restrictinfo);
extern void generate_implied_equalities(Query *root);
extern List *canonicalize_pathkeys(Query *root, List *pathkeys);
extern PathKeysComparison compare_pathkeys(List *keys1, List *keys2);
extern bool pathkeys_contained_in(List *keys1, List *keys2);
extern PathKeysComparison compare_noncanonical_pathkeys(List *keys1,
							  List *keys2);
extern bool noncanonical_pathkeys_contained_in(List *keys1, List *keys2);
extern Path *get_cheapest_path_for_pathkeys(List *paths, List *pathkeys,
							   CostSelector cost_criterion);
extern Path *get_cheapest_fractional_path_for_pathkeys(List *paths,
										  List *pathkeys,
										  double fraction);
extern List *build_index_pathkeys(Query *root, RelOptInfo *rel,
					 IndexOptInfo *index,
					 ScanDirection scandir);
extern List *build_join_pathkeys(Query *root,
					RelOptInfo *joinrel,
					List *outer_pathkeys);
extern List *make_pathkeys_for_sortclauses(List *sortclauses,
							  List *tlist);
extern void cache_mergeclause_pathkeys(Query *root,
						   RestrictInfo *restrictinfo);
extern List *find_mergeclauses_for_pathkeys(Query *root,
							   List *pathkeys,
							   List *restrictinfos);
extern List *make_pathkeys_for_mergeclauses(Query *root,
							   List *mergeclauses,
							   RelOptInfo *rel);
extern int pathkeys_useful_for_merging(Query *root,
							RelOptInfo *rel,
							List *pathkeys);
extern int	pathkeys_useful_for_ordering(Query *root, List *pathkeys);
extern List *truncate_useless_pathkeys(Query *root,
						  RelOptInfo *rel,
						  List *pathkeys);

#endif   /* PATHS_H */
