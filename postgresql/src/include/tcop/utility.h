/*-------------------------------------------------------------------------
 *
 * utility.h
 *	  prototypes for utility.c.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: utility.h,v 1.2 2004/07/05 08:56:08 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef UTILITY_H
#define UTILITY_H

#include "executor/execdesc.h"
//renyi 11 24 : changed : add prepare_res to 
//return the result of "xaprepare" command
extern void ProcessUtility(Node *parsetree, CommandDest dest,
						   char *completionTag,int *prepare_res);

#endif   /* UTILITY_H */
