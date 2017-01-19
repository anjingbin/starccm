/*-------------------------------------------------------------------------
 *
 * nodeMergejoin.h
 *
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeMergejoin.h,v 1.2 2004/07/05 08:55:58 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEMERGEJOIN_H
#define NODEMERGEJOIN_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecMergeJoin(MergeJoin *node);
extern bool ExecInitMergeJoin(MergeJoin *node, EState *estate, Plan *parent);
extern int	ExecCountSlotsMergeJoin(MergeJoin *node);
extern void ExecEndMergeJoin(MergeJoin *node);
extern void ExecReScanMergeJoin(MergeJoin *node, ExprContext *exprCtxt, Plan *parent);

#endif   /* NODEMERGEJOIN_H; */
