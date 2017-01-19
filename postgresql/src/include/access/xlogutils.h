/*
 * xlogutils.h
 *
 * PostgreSQL transaction log manager utility routines
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: xlogutils.h,v 1.2 2004/07/05 08:55:58 SuLiang Exp $
 */
#ifndef XLOG_UTILS_H
#define XLOG_UTILS_H

#include "access/rmgr.h"
#include "storage/buf.h"
#include "storage/itemptr.h"
#include "utils/rel.h"

extern int XLogIsOwnerOfTuple(RelFileNode hnode, ItemPointer iptr,
				   TransactionId xid, CommandId cid);
extern bool XLogIsValidTuple(RelFileNode hnode, ItemPointer iptr);

extern void XLogInitRelationCache(void);
extern void XLogCloseRelationCache(void);

extern Relation XLogOpenRelation(bool redo, RmgrId rmid, RelFileNode rnode);
extern Buffer XLogReadBuffer(bool extend, Relation reln, BlockNumber blkno);

#endif
