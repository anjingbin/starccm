/*-------------------------------------------------------------------------
 *
 * excid.c
 *	  POSTGRES known exception identifier code.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/error/excid.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

/*****************************************************************************
 *	 Generic Recoverable Exceptions											 *
 *****************************************************************************/


/*
 * FailedAssertion
 *		Indicates an Assert(...) failed.
 */
Exception	FailedAssertion = {"Failed Assertion"};

/*
 * BadState
 *		Indicates a function call request is inconsistent with module state.
 */
Exception	BadState = {"Bad State for Function Call"};

/*
 * BadArg
 *		Indicates a function call argument or arguments is out-of-bounds.
 */
Exception	BadArg = {"Bad Argument to Function Call"};

/*****************************************************************************
 *	 Specific Recoverable Exceptions										 *
 *****************************************************************************/

/*
 * Unimplemented
 *		Indicates a function call request requires unimplemented code.
 */
Exception	Unimplemented = {"Unimplemented Functionality"};

Exception	CatalogFailure = {"Catalog failure"};		/* XXX inconsistent */
Exception	InternalError = {"Internal Error"}; /* XXX inconsistent */
Exception	SemanticError = {"Semantic Error"}; /* XXX inconsistent */
Exception	SystemError = {"System Error"};		/* XXX inconsistent */
