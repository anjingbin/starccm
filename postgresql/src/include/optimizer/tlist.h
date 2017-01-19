/*-------------------------------------------------------------------------
 *
 * tlist.h
 *	  prototypes for tlist.c.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: tlist.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef TLIST_H
#define TLIST_H

#include "nodes/relation.h"

extern TargetEntry *tlistentry_member(Node *node, List *targetlist);
extern Resdom *tlist_member(Node *node, List *targetlist);

extern void add_var_to_tlist(RelOptInfo *rel, Var *var);
extern TargetEntry *create_tl_element(Var *var, int resdomno);

extern List *new_unsorted_tlist(List *targetlist);
extern List *flatten_tlist(List *tlist);
extern List *add_to_flat_tlist(List *tlist, List *vars);

extern Var *get_expr(TargetEntry *tle);

extern TargetEntry *get_sortgroupclause_tle(SortClause *sortClause,
						List *targetList);
extern Node *get_sortgroupclause_expr(SortClause *sortClause,
						 List *targetList);

#endif   /* TLIST_H */
