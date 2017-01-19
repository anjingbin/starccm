/*-------------------------------------------------------------------------
 *
 * analyze.h
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: analyze.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef ANALYZE_H
#define ANALYZE_H

#include "parser/parse_node.h"

extern List *parse_analyze(Node *parseTree, ParseState *parentParseState);

extern void CheckSelectForUpdate(Query *qry);

/* This was exported to allow ADD CONSTRAINT to make use of it */
extern char *makeObjectName(char *name1, char *name2, char *typename);

#endif   /* ANALYZE_H */
