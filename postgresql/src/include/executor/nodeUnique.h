/*-------------------------------------------------------------------------
 *
 * nodeUnique.h
 *
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeUnique.h,v 1.2 2004/07/05 08:56:03 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEUNIQUE_H
#define NODEUNIQUE_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecUnique(Unique *node);
extern bool ExecInitUnique(Unique *node, EState *estate, Plan *parent);
extern int	ExecCountSlotsUnique(Unique *node);
extern void ExecEndUnique(Unique *node);
extern void ExecReScanUnique(Unique *node, ExprContext *exprCtxt, Plan *parent);

#endif   /* NODEUNIQUE_H */
