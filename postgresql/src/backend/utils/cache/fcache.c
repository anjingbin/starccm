/*-------------------------------------------------------------------------
 *
 * fcache.c
 *	  Code for the 'function cache' used in Oper and Func nodes.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/cache/fcache.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "utils/fcache.h"


/*
 * Build a 'FunctionCache' struct given the PG_PROC oid.
 */
FunctionCachePtr
init_fcache(Oid foid, int nargs, MemoryContext fcacheCxt)
{
	FunctionCachePtr retval;

	/* Safety check (should never fail, as parser should check sooner) */
	if (nargs > FUNC_MAX_ARGS)
		elog(ERROR, "init_fcache: too many arguments");

	/* Create fcache entry in the desired context */
	retval = (FunctionCachePtr) MemoryContextAlloc(fcacheCxt,
												   sizeof(FunctionCache));
	MemSet(retval, 0, sizeof(FunctionCache));

	/* Set up the primary fmgr lookup information */
	fmgr_info_cxt(foid, &(retval->func), fcacheCxt);

	/* Initialize additional info */
	retval->setArgsValid = false;

	return retval;
}
