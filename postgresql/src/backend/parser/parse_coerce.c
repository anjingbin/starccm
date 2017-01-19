/*-------------------------------------------------------------------------
 *
 * parse_coerce.c
 *		handle type coercions/conversions for parser
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/parser/parse_coerce.c,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "catalog/pg_proc.h"
#include "optimizer/clauses.h"
#include "parser/parse_coerce.h"
#include "parser/parse_expr.h"
#include "parser/parse_func.h"
#include "parser/parse_type.h"
#include "utils/builtins.h"
#include "utils/syscache.h"

Oid			DemoteType(Oid inType);
Oid			PromoteTypeToNext(Oid inType);

static Oid	PreferredType(CATEGORY category, Oid type);


/* coerce_type()
 * Convert a function argument to a different type.
 */
Node *
coerce_type(ParseState *pstate, Node *node, Oid inputTypeId,
			Oid targetTypeId, int32 atttypmod)
{
	Node	   *result;

	if (targetTypeId == inputTypeId ||
		targetTypeId == InvalidOid ||
		node == NULL)
	{
		/* no conversion needed */
		result = node;
	}
	else if (inputTypeId == UNKNOWNOID && IsA(node, Const))
	{
		/*
		 * Input is a string constant with previously undetermined type.
		 * Apply the target type's typinput function to it to produce a
		 * constant of the target type.
		 *
		 * NOTE: this case cannot be folded together with the other
		 * constant-input case, since the typinput function does not
		 * necessarily behave the same as a type conversion function. For
		 * example, int4's typinput function will reject "1.2", whereas
		 * float-to-int type conversion will round to integer.
		 *
		 * XXX if the typinput function is not cachable, we really ought to
		 * postpone evaluation of the function call until runtime. But
		 * there is no way to represent a typinput function call as an
		 * expression tree, because C-string values are not Datums.
		 */
		Const	   *con = (Const *) node;
		Const	   *newcon = makeNode(Const);
		Type		targetType = typeidType(targetTypeId);

		newcon->consttype = targetTypeId;
		newcon->constlen = typeLen(targetType);
		newcon->constbyval = typeByVal(targetType);
		newcon->constisnull = con->constisnull;
		newcon->constisset = false;

		if (!con->constisnull)
		{
			/* We know the source constant is really of type 'text' */
			char	   *val = DatumGetCString(DirectFunctionCall1(textout,
													   con->constvalue));

			newcon->constvalue = stringTypeDatum(targetType, val, atttypmod);
			pfree(val);
		}

		ReleaseSysCache(targetType);

		result = (Node *) newcon;
	}
	else if (IS_BINARY_COMPATIBLE(inputTypeId, targetTypeId))
	{
		/*
		 * We don't really need to do a conversion, but we do need to
		 * attach a RelabelType node so that the expression will be seen
		 * to have the intended type when inspected by higher-level code.
		 */
		RelabelType *relabel = makeNode(RelabelType);

		relabel->arg = node;
		relabel->resulttype = targetTypeId;

		/*
		 * XXX could we label result with exprTypmod(node) instead of
		 * default -1 typmod, to save a possible length-coercion later?
		 * Would work if both types have same interpretation of typmod,
		 * which is likely but not certain.
		 */
		relabel->resulttypmod = -1;

		result = (Node *) relabel;
	}
	else if (typeInheritsFrom(inputTypeId, targetTypeId))
	{
		/* Input class type is a subclass of target, so nothing to do */
		result = node;
	}
	else
	{
		/*
		 * Otherwise, find the appropriate type conversion function
		 * (caller should have determined that there is one), and generate
		 * an expression tree representing run-time application of the
		 * conversion function.
		 */
		FuncCall   *n = makeNode(FuncCall);

		n->funcname = typeidTypeName(targetTypeId);
		n->args = makeList1(node);
		n->agg_star = false;
		n->agg_distinct = false;

		result = transformExpr(pstate, (Node *) n, EXPR_COLUMN_FIRST);

		/* safety check that we got the right thing */
		if (exprType(result) != targetTypeId)
			elog(ERROR, "coerce_type: conversion function %s produced %s",
				 typeidTypeName(targetTypeId),
				 typeidTypeName(exprType(result)));

		/*
		 * If the input is a constant, apply the type conversion function
		 * now instead of delaying to runtime.	(We could, of course, just
		 * leave this to be done during planning/optimization; but it's a
		 * very frequent special case, and we save cycles in the rewriter
		 * if we fold the expression now.)
		 *
		 * Note that no folding will occur if the conversion function is not
		 * marked 'iscachable'.
		 *
		 * HACK: if constant is NULL, don't fold it here.  This is needed by
		 * make_subplan(), which calls this routine on placeholder Const
		 * nodes that mustn't be collapsed.  (It'd be a lot cleaner to
		 * make a separate node type for that purpose...)
		 */
		if (IsA(node, Const) &&!((Const *) node)->constisnull)
			result = eval_const_expressions(result);
	}

	return result;
}


/* can_coerce_type()
 * Can input_typeids be coerced to func_typeids?
 *
 * There are a few types which are known apriori to be convertible.
 * We will check for those cases first, and then look for possible
 *	conversion functions.
 *
 * Notes:
 * This uses the same mechanism as the CAST() SQL construct in gram.y.
 * We should also check the function return type on candidate conversion
 *	routines just to be safe but we do not do that yet...
 * - thomas 1998-03-31
 */
bool
can_coerce_type(int nargs, Oid *input_typeids, Oid *func_typeids)
{
	int			i;
	HeapTuple	ftup;
	Form_pg_proc pform;
	Oid			oid_array[FUNC_MAX_ARGS];

	/* run through argument list... */
	for (i = 0; i < nargs; i++)
	{
		Oid			inputTypeId = input_typeids[i];
		Oid			targetTypeId = func_typeids[i];

		/* no problem if same type */
		if (inputTypeId == targetTypeId)
			continue;

		/*
		 * one of the known-good transparent conversions? then drop
		 * through...
		 */
		if (IS_BINARY_COMPATIBLE(inputTypeId, targetTypeId))
			continue;

		/* don't know what to do for the output type? then quit... */
		if (targetTypeId == InvalidOid)
			return false;
		/* don't know what to do for the input type? then quit... */
		if (inputTypeId == InvalidOid)
			return false;

		/*
		 * If input is an untyped string constant, assume we can convert
		 * it to anything except a class type.
		 */
		if (inputTypeId == UNKNOWNOID)
		{
			if (ISCOMPLEX(targetTypeId))
				return false;
			continue;
		}

		/*
		 * If input is a class type that inherits from target, no problem
		 */
		if (typeInheritsFrom(inputTypeId, targetTypeId))
			continue;

		/* don't choke on references to no-longer-existing types */
		if (!typeidIsValid(inputTypeId))
			return false;
		if (!typeidIsValid(targetTypeId))
			return false;

		/*
		 * Else, try for explicit conversion using functions: look for a
		 * single-argument function named with the target type name and
		 * accepting the source type.
		 */
		MemSet(oid_array, 0, FUNC_MAX_ARGS * sizeof(Oid));
		oid_array[0] = inputTypeId;

		ftup = SearchSysCache(PROCNAME,
						   PointerGetDatum(typeidTypeName(targetTypeId)),
							  Int32GetDatum(1),
							  PointerGetDatum(oid_array),
							  0);
		if (!HeapTupleIsValid(ftup))
			return false;
		/* Make sure the function's result type is as expected, too */
		pform = (Form_pg_proc) GETSTRUCT(ftup);
		if (pform->prorettype != targetTypeId)
		{
			ReleaseSysCache(ftup);
			return false;
		}
		ReleaseSysCache(ftup);
	}

	return true;
}

/* coerce_type_typmod()
 * Force a value to a particular typmod, if meaningful and possible.
 *
 * This is applied to values that are going to be stored in a relation
 * (where we have an atttypmod for the column) as well as values being
 * explicitly CASTed (where the typmod comes from the target type spec).
 *
 * The caller must have already ensured that the value is of the correct
 * type, typically by applying coerce_type.
 *
 * If the target column type possesses a function named for the type
 * and having parameter signature (columntype, int4), we assume that
 * the type requires coercion to its own length and that the said
 * function should be invoked to do that.
 *
 * "bpchar" (ie, char(N)) and "numeric" are examples of such types.
 */
Node *
coerce_type_typmod(ParseState *pstate, Node *node,
				   Oid targetTypeId, int32 atttypmod)
{
	char	   *funcname;
	Oid			oid_array[FUNC_MAX_ARGS];

	/*
	 * A negative typmod is assumed to mean that no coercion is wanted.
	 */
	if (atttypmod < 0 || atttypmod == exprTypmod(node))
		return node;

	funcname = typeidTypeName(targetTypeId);
	MemSet(oid_array, 0, FUNC_MAX_ARGS * sizeof(Oid));
	oid_array[0] = targetTypeId;
	oid_array[1] = INT4OID;

	/* attempt to find with arguments exactly as specified... */
	if (SearchSysCacheExists(PROCNAME,
							 PointerGetDatum(funcname),
							 Int32GetDatum(2),
							 PointerGetDatum(oid_array),
							 0))
	{
		A_Const    *cons = makeNode(A_Const);
		FuncCall   *func = makeNode(FuncCall);

		cons->val.type = T_Integer;
		cons->val.val.ival = atttypmod;

		func->funcname = funcname;
		func->args = makeList2(node, cons);
		func->agg_star = false;
		func->agg_distinct = false;

		node = transformExpr(pstate, (Node *) func, EXPR_COLUMN_FIRST);
	}

	return node;
}


/* coerce_to_boolean()
 *		Coerce an argument of a construct that requires boolean input
 *		(AND, OR, NOT, etc).
 *
 * If successful, update *pnode to be the transformed argument (if any
 * transformation is needed), and return TRUE.	If fail, return FALSE.
 * (The caller must check for FALSE and emit a suitable error message.)
 */
bool
coerce_to_boolean(ParseState *pstate, Node **pnode)
{
	Oid			inputTypeId = exprType(*pnode);
	Oid			targetTypeId;

	if (inputTypeId == BOOLOID)
		return true;			/* no work */
	targetTypeId = BOOLOID;
	if (!can_coerce_type(1, &inputTypeId, &targetTypeId))
		return false;			/* fail, but let caller choose error msg */
	*pnode = coerce_type(pstate, *pnode, inputTypeId, targetTypeId, -1);
	return true;
}


/* select_common_type()
 *		Determine the common supertype of a list of input expression types.
 *		This is used for determining the output type of CASE and UNION
 *		constructs.
 *
 * typeids is a nonempty integer list of type OIDs.  Note that earlier items
 * in the list will be preferred if there is doubt.
 * 'context' is a phrase to use in the error message if we fail to select
 * a usable type.
 *
 * XXX this code is WRONG, since (for example) given the input (int4,int8)
 * it will select int4, whereas according to SQL92 clause 9.3 the correct
 * answer is clearly int8.	To fix this we need a notion of a promotion
 * hierarchy within type categories --- something more complete than
 * just a single preferred type.
 */
Oid
select_common_type(List *typeids, const char *context)
{
	Oid			ptype;
	CATEGORY	pcategory;
	List	   *l;

	Assert(typeids != NIL);
	ptype = (Oid) lfirsti(typeids);
	pcategory = TypeCategory(ptype);
	foreach(l, lnext(typeids))
	{
		Oid			ntype = (Oid) lfirsti(l);

		/* move on to next one if no new information... */
		if ((ntype != InvalidOid) && (ntype != UNKNOWNOID) && (ntype != ptype))
		{
			if ((ptype == InvalidOid) || ptype == UNKNOWNOID)
			{
				/* so far, only nulls so take anything... */
				ptype = ntype;
				pcategory = TypeCategory(ptype);
			}
			else if (TypeCategory(ntype) != pcategory)
			{
				/*
				 * both types in different categories? then not much
				 * hope...
				 */
				elog(ERROR, "%s types \"%s\" and \"%s\" not matched",
				  context, typeidTypeName(ptype), typeidTypeName(ntype));
			}
			else if (IsPreferredType(pcategory, ntype)
					 && !IsPreferredType(pcategory, ptype)
					 && can_coerce_type(1, &ptype, &ntype))
			{
				/*
				 * new one is preferred and can convert? then take it...
				 */
				ptype = ntype;
				pcategory = TypeCategory(ptype);
			}
		}
	}

	/*
	 * If all the inputs were UNKNOWN type --- ie, unknown-type literals
	 * --- then resolve as type TEXT.  This situation comes up with
	 * constructs like SELECT (CASE WHEN foo THEN 'bar' ELSE 'baz' END);
	 * SELECT 'foo' UNION SELECT 'bar'; It might seem desirable to leave
	 * the construct's output type as UNKNOWN, but that really doesn't
	 * work, because we'd probably end up needing a runtime coercion from
	 * UNKNOWN to something else, and we usually won't have it.  We need
	 * to coerce the unknown literals while they are still literals, so a
	 * decision has to be made now.
	 */
	if (ptype == UNKNOWNOID)
		ptype = TEXTOID;

	return ptype;
}

/* coerce_to_common_type()
 *		Coerce an expression to the given type.
 *
 * This is used following select_common_type() to coerce the individual
 * expressions to the desired type.  'context' is a phrase to use in the
 * error message if we fail to coerce.
 *
 * NOTE: pstate may be NULL.
 */
Node *
coerce_to_common_type(ParseState *pstate, Node *node,
					  Oid targetTypeId,
					  const char *context)
{
	Oid			inputTypeId = exprType(node);

	if (inputTypeId == targetTypeId)
		return node;			/* no work */
	if (can_coerce_type(1, &inputTypeId, &targetTypeId))
		node = coerce_type(pstate, node, inputTypeId, targetTypeId, -1);
	else
	{
		elog(ERROR, "%s unable to convert to type \"%s\"",
			 context, typeidTypeName(targetTypeId));
	}
	return node;
}


/* TypeCategory()
 * Assign a category to the specified OID.
 * XXX This should be moved to system catalog lookups
 * to allow for better type extensibility.
 * - thomas 2001-09-30
 */
CATEGORY
TypeCategory(Oid inType)
{
	CATEGORY	result;

	switch (inType)
	{
		case (BOOLOID):
			result = BOOLEAN_TYPE;
			break;

		case (CHAROID):
		case (NAMEOID):
		case (BPCHAROID):
		case (VARCHAROID):
		case (TEXTOID):
			result = STRING_TYPE;
			break;

		case (BITOID):
		case (VARBITOID):
			result = BITSTRING_TYPE;
			break;

		case (OIDOID):
		case (REGPROCOID):
		case (INT2OID):
		case (INT4OID):
		case (INT8OID):
		case (FLOAT4OID):
		case (FLOAT8OID):
		case (NUMERICOID):
		case (CASHOID):
			result = NUMERIC_TYPE;
			break;

		case (DATEOID):
		case (TIMEOID):
		case (TIMETZOID):
		case (ABSTIMEOID):
		case (TIMESTAMPOID):
		case (TIMESTAMPTZOID):
			result = DATETIME_TYPE;
			break;

		case (RELTIMEOID):
		case (TINTERVALOID):
		case (INTERVALOID):
			result = TIMESPAN_TYPE;
			break;

		case (POINTOID):
		case (LSEGOID):
		case (PATHOID):
		case (BOXOID):
		case (POLYGONOID):
		case (LINEOID):
		case (CIRCLEOID):
			result = GEOMETRIC_TYPE;
			break;

		case (INETOID):
		case (CIDROID):
			result = NETWORK_TYPE;
			break;

		case (UNKNOWNOID):
		case (InvalidOid):
			result = UNKNOWN_TYPE;
			break;

		default:
			result = USER_TYPE;
			break;
	}
	return result;
}	/* TypeCategory() */


/* IsPreferredType()
 * Check if this type is a preferred type.
 * XXX This should be moved to system catalog lookups
 * to allow for better type extensibility.
 * - thomas 2001-09-30
 */
bool
IsPreferredType(CATEGORY category, Oid type)
{
	return (type == PreferredType(category, type));
}	/* IsPreferredType() */


/* PreferredType()
 * Return the preferred type OID for the specified category.
 * XXX This should be moved to system catalog lookups
 * to allow for better type extensibility.
 * - thomas 2001-09-30
 */
static Oid
PreferredType(CATEGORY category, Oid type)
{
	Oid			result;

	switch (category)
	{
		case (BOOLEAN_TYPE):
			result = BOOLOID;
			break;

		case (STRING_TYPE):
			result = TEXTOID;
			break;

		case (BITSTRING_TYPE):
			result = VARBITOID;
			break;

		case (NUMERIC_TYPE):
			if (type == OIDOID)
				result = OIDOID;
			else if (type == NUMERICOID)
				result = NUMERICOID;
			else
				result = FLOAT8OID;
			break;

		case (DATETIME_TYPE):
			if (type == DATEOID)
				result = TIMESTAMPOID;
			else
				result = TIMESTAMPTZOID;
			break;

		case (TIMESPAN_TYPE):
			result = INTERVALOID;
			break;

		case (NETWORK_TYPE):
			result = INETOID;
			break;

		case (GEOMETRIC_TYPE):
		case (USER_TYPE):
			result = type;
			break;

		default:
			result = UNKNOWNOID;
			break;
	}
	return result;
}	/* PreferredType() */
