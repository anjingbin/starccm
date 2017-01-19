/*-------------------------------------------------------------------------
 *
 * planmain.h
 *	  prototypes for various files in optimizer/plan
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: planmain.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PLANMAIN_H
#define PLANMAIN_H

#include "nodes/plannodes.h"
#include "nodes/relation.h"

/*
 * prototypes for plan/planmain.c
 */
extern Plan *query_planner(Query *root, List *tlist, double tuple_fraction);

/*
 * prototypes for plan/createplan.c
 */
extern Plan *create_plan(Query *root, Path *best_path);
extern SubqueryScan *make_subqueryscan(List *qptlist, List *qpqual,
				  Index scanrelid, Plan *subplan);
extern Append *make_append(List *appendplans, bool isTarget, List *tlist);
extern Sort *make_sort(Query *root, List *tlist,
		  Plan *lefttree, int keycount);
extern Sort *make_sort_from_pathkeys(Query *root, List *tlist,
						Plan *lefttree, List *pathkeys);
extern Agg *make_agg(List *tlist, List *qual, Plan *lefttree);
extern Group *make_group(List *tlist, bool tuplePerGroup, int ngrp,
		   AttrNumber *grpColIdx, Plan *lefttree);
extern Material *make_material(List *tlist, Plan *lefttree);
extern Unique *make_unique(List *tlist, Plan *lefttree, List *distinctList);
extern Limit *make_limit(List *tlist, Plan *lefttree,
		   Node *limitOffset, Node *limitCount);
extern SetOp *make_setop(SetOpCmd cmd, List *tlist, Plan *lefttree,
		   List *distinctList, AttrNumber flagColIdx);
extern Result *make_result(List *tlist, Node *resconstantqual, Plan *subplan);

/*
 * prototypes for plan/initsplan.c
 */
extern void build_base_rel_tlists(Query *root, List *tlist);
extern Relids distribute_quals_to_rels(Query *root, Node *jtnode);
extern List *add_missing_rels_to_query(Query *root, Node *jtnode);
extern void process_implied_equality(Query *root, Node *item1, Node *item2,
						 Oid sortop1, Oid sortop2);

/*
 * prototypes for plan/setrefs.c
 */
extern void set_plan_references(Plan *plan);
extern List *join_references(List *clauses, List *outer_tlist,
				List *inner_tlist, Index acceptable_rel);
extern void fix_opids(Node *node);

/*
 * prep/prepkeyset.c
 */
extern bool _use_keyset_query_optimizer;

extern void transformKeySetQuery(Query *origNode);

#endif   /* PLANMAIN_H */
