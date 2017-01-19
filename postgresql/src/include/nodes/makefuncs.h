/*-------------------------------------------------------------------------
 *
 * makefuncs.h
 *	  prototypes for the creator functions (for primitive nodes)
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: makefuncs.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef MAKEFUNC_H
#define MAKEFUNC_H

#include "nodes/parsenodes.h"

extern Oper *makeOper(Oid opno,
		 Oid opid,
		 Oid opresulttype);

extern Var *makeVar(Index varno,
		AttrNumber varattno,
		Oid vartype,
		int32 vartypmod,
		Index varlevelsup);

extern TargetEntry *makeTargetEntry(Resdom *resdom, Node *expr);

extern Resdom *makeResdom(AttrNumber resno,
		   Oid restype,
		   int32 restypmod,
		   char *resname,
		   bool resjunk);

extern Const *makeConst(Oid consttype,
		  int constlen,
		  Datum constvalue,
		  bool constisnull,
		  bool constbyval,
		  bool constisset,
		  bool constiscast);

extern Const *makeNullConst(Oid consttype);

extern Attr *makeAttr(char *relname, char *attname);

#endif   /* MAKEFUNC_H */
