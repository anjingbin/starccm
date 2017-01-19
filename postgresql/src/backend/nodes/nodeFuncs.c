/*-------------------------------------------------------------------------
 *
 * nodeFuncs.c
 *	  All node routines more complicated than simple access/modification
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/nodes/nodeFuncs.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include <sys/types.h>

#include "postgres.h"

#include "nodes/nodeFuncs.h"
#include "utils/lsyscache.h"

static bool var_is_inner(Var *var);

/*
 * single_node -
 *	  Returns t if node corresponds to a single-noded expression
 */
bool
single_node(Node *node)
{
	if (IsA(node, Ident) ||IsA(node, Const) ||IsA(node, Var) ||IsA(node, Param))
		return true;
	else
		return false;
}

/*****************************************************************************
 *		VAR nodes
 *****************************************************************************/

/*
 *		var_is_outer
 *		var_is_inner
 *		var_is_mat
 *		var_is_rel
 *
 *		Returns t iff the var node corresponds to (respectively):
 *		the outer relation in a join
 *		the inner relation of a join
 *		a materialized relation
 *		a base relation (i.e., not an attribute reference, a variable from
 *				some lower join level, or a sort result)
 *		var node is an array reference
 *
 */
bool
var_is_outer(Var *var)
{
	return (bool) (var->varno == OUTER);
}

static bool
var_is_inner(Var *var)
{
	return (bool) (var->varno == INNER);
}

bool
var_is_rel(Var *var)
{
	return (bool)
		!(var_is_inner(var) || var_is_outer(var));
}

/*****************************************************************************
 *		OPER nodes
 *****************************************************************************/

/*
 * replace_opid -
 *
 *		Given a oper node, resets the opfid field with the
 *		procedure OID (regproc id).
 *
 *		Returns the modified oper node.
 *
 */
Oper *
replace_opid(Oper *oper)
{
	oper->opid = get_opcode(oper->opno);
	oper->op_fcache = NULL;
	return oper;
}

/*****************************************************************************
 *		constant (CONST, PARAM) nodes
 *****************************************************************************/

#ifdef NOT_USED
/*
 * non_null -
 *		Returns t if the node is a non-null constant, e.g., if the node has a
 *		valid `constvalue' field.
 *
 */
bool
non_null(Expr *c)
{

	if (IsA(c, Const) &&!((Const *) c)->constisnull)
		return true;
	else
		return false;
}

#endif
