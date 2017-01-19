/*-------------------------------------------------------------------------
 *
 * nodeAppend.h
 *
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeAppend.h,v 1.2 2004/07/05 08:55:58 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEAPPEND_H
#define NODEAPPEND_H

#include "nodes/plannodes.h"

extern bool ExecInitAppend(Append *node, EState *estate, Plan *parent);
extern int	ExecCountSlotsAppend(Append *node);
extern TupleTableSlot *ExecProcAppend(Append *node);
extern void ExecEndAppend(Append *node);
extern void ExecReScanAppend(Append *node, ExprContext *exprCtxt, Plan *parent);

#endif   /* NODEAPPEND_H */
