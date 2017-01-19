/*-------------------------------------------------------------------------
 *
 * parse_expr.c
 *	  handle expressions in parser
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/parser/parse_expr.c,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "catalog/pg_operator.h"
#include "catalog/pg_proc.h"
#include "nodes/makefuncs.h"
#include "nodes/params.h"
#include "parser/analyze.h"
#include "parser/gramparse.h"
#include "parser/parse.h"
#include "parser/parse_coerce.h"
#include "parser/parse_expr.h"
#include "parser/parse_func.h"
#include "parser/parse_oper.h"
#include "parser/parse_relation.h"
#include "parser/parse_target.h"
#include "parser/parse_type.h"
#include "utils/builtins.h"
#include "utils/syscache.h"


int			max_expr_depth = DEFAULT_MAX_EXPR_DEPTH;
static int	expr_depth_counter = 0;

bool		Transform_null_equals = false;

static Node *parser_typecast_constant(Value *expr, TypeName *typename);
static Node *parser_typecast_expression(ParseState *pstate,
						   Node *expr, TypeName *typename);
static Node *transformAttr(ParseState *pstate, Attr *att, int precedence);
static Node *transformIdent(ParseState *pstate, Ident *ident, int precedence);
static Node *transformIndirection(ParseState *pstate, Node *basenode,
					 List *indirection);


/*
 * Initialize for parsing a new query.
 *
 * We reset the expression depth counter here, in case it was left nonzero
 * due to elog()'ing out of the last parsing operation.
 */
void
parse_expr_init(void)
{
	expr_depth_counter = 0;
}


/*
 * transformExpr -
 *	  Analyze and transform expressions. Type checking and type casting is
 *	  done here. The optimizer and the executor cannot handle the original
 *	  (raw) expressions collected by the parse tree. Hence the transformation
 *	  here.
 *
 * NOTE: there are various cases in which this routine will get applied to
 * an already-transformed expression.  Some examples:
 *	1. At least one construct (BETWEEN/AND) puts the same nodes
 *	into two branches of the parse tree; hence, some nodes
 *	are transformed twice.
 *	2. Another way it can happen is that coercion of an operator or
 *	function argument to the required type (via coerce_type())
 *	can apply transformExpr to an already-transformed subexpression.
 *	An example here is "SELECT count(*) + 1.0 FROM table".
 * While it might be possible to eliminate these cases, the path of
 * least resistance so far has been to ensure that transformExpr() does
 * no damage if applied to an already-transformed tree.  This is pretty
 * easy for cases where the transformation replaces one node type with
 * another, such as A_Const => Const; we just do nothing when handed
 * a Const.  More care is needed for node types that are used as both
 * input and output of transformExpr; see SubLink for example.
 */
Node *
transformExpr(ParseState *pstate, Node *expr, int precedence)
{
	Node	   *result = NULL;

	if (expr == NULL)
		return NULL;

	/*
	 * Guard against an overly complex expression leading to coredump due
	 * to stack overflow here, or in later recursive routines that
	 * traverse expression trees.  Note that this is very unlikely to
	 * happen except with pathological queries; but we don't want someone
	 * to be able to crash the backend quite that easily...
	 */
	if (++expr_depth_counter > max_expr_depth)
		elog(ERROR, "Expression too complex: nesting depth exceeds max_expr_depth = %d",
			 max_expr_depth);

	switch (nodeTag(expr))
	{
		case T_Attr:
			{
				result = transformAttr(pstate, (Attr *) expr, precedence);
				break;
			}
		case T_A_Const:
			{
				A_Const    *con = (A_Const *) expr;
				Value	   *val = &con->val;

				if (con->typename != NULL)
					result = parser_typecast_constant(val, con->typename);
				else
					result = (Node *) make_const(val);
				break;
			}
		case T_ParamNo:
			{
				ParamNo    *pno = (ParamNo *) expr;
				int			paramno = pno->number;
				Oid			toid = param_type(paramno);
				Param	   *param = makeNode(Param);

				if (!OidIsValid(toid))
					elog(ERROR, "Parameter '$%d' is out of range", paramno);
				param->paramkind = PARAM_NUM;
				param->paramid = (AttrNumber) paramno;
				param->paramname = "<unnamed>";
				param->paramtype = toid;
				result = transformIndirection(pstate, (Node *) param,
											  pno->indirection);
				/* cope with typecast applied to param */
				if (pno->typename != NULL)
					result = parser_typecast_expression(pstate, result,
														pno->typename);
				break;
			}
		case T_TypeCast:
			{
				TypeCast   *tc = (TypeCast *) expr;
				Node	   *arg = transformExpr(pstate, tc->arg, precedence);

				result = parser_typecast_expression(pstate, arg, tc->typename);
				break;
			}
		case T_A_Expr:
			{
				A_Expr	   *a = (A_Expr *) expr;

				switch (a->oper)
				{
					case OP:
						{
							/*
							 * Special-case "foo = NULL" and "NULL = foo"
							 * for compatibility with standards-broken
							 * products (like Microsoft's).  Turn these
							 * into IS NULL exprs.
							 */
							if (Transform_null_equals &&
								strcmp(a->opname, "=") == 0 &&
								(exprIsNullConstant(a->lexpr) ||
								 exprIsNullConstant(a->rexpr)))
							{
								NullTest   *n = makeNode(NullTest);

								n->nulltesttype = IS_NULL;

								if (exprIsNullConstant(a->lexpr))
									n->arg = a->rexpr;
								else
									n->arg = a->lexpr;

								result = transformExpr(pstate,
													   (Node *) n,
													   precedence);
							}
							else
							{
								Node	   *lexpr = transformExpr(pstate,
																a->lexpr,
															 precedence);
								Node	   *rexpr = transformExpr(pstate,
																a->rexpr,
															 precedence);

								result = (Node *) make_op(a->opname,
														  lexpr,
														  rexpr);
							}
						}
						break;
					case AND:
						{
							Node	   *lexpr = transformExpr(pstate,
															  a->lexpr,
															  precedence);
							Node	   *rexpr = transformExpr(pstate,
															  a->rexpr,
															  precedence);
							Expr	   *expr = makeNode(Expr);

							if (!coerce_to_boolean(pstate, &lexpr))
								elog(ERROR, "left-hand side of AND is type '%s', not '%s'",
									 format_type_be(exprType(lexpr)),
									 format_type_be(BOOLOID));

							if (!coerce_to_boolean(pstate, &rexpr))
								elog(ERROR, "right-hand side of AND is type '%s', not '%s'",
									 format_type_be(exprType(rexpr)),
									 format_type_be(BOOLOID));

							expr->typeOid = BOOLOID;
							expr->opType = AND_EXPR;
							expr->args = makeList2(lexpr, rexpr);
							result = (Node *) expr;
						}
						break;
					case OR:
						{
							Node	   *lexpr = transformExpr(pstate,
															  a->lexpr,
															  precedence);
							Node	   *rexpr = transformExpr(pstate,
															  a->rexpr,
															  precedence);
							Expr	   *expr = makeNode(Expr);

							if (!coerce_to_boolean(pstate, &lexpr))
								elog(ERROR, "left-hand side of OR is type '%s', not '%s'",
									 format_type_be(exprType(lexpr)),
									 format_type_be(BOOLOID));

							if (!coerce_to_boolean(pstate, &rexpr))
								elog(ERROR, "right-hand side of OR is type '%s', not '%s'",
									 format_type_be(exprType(rexpr)),
									 format_type_be(BOOLOID));

							expr->typeOid = BOOLOID;
							expr->opType = OR_EXPR;
							expr->args = makeList2(lexpr, rexpr);
							result = (Node *) expr;
						}
						break;
					case NOT:
						{
							Node	   *rexpr = transformExpr(pstate,
															  a->rexpr,
															  precedence);
							Expr	   *expr = makeNode(Expr);

							if (!coerce_to_boolean(pstate, &rexpr))
								elog(ERROR, "argument to NOT is type '%s', not '%s'",
									 format_type_be(exprType(rexpr)),
									 format_type_be(BOOLOID));

							expr->typeOid = BOOLOID;
							expr->opType = NOT_EXPR;
							expr->args = makeList1(rexpr);
							result = (Node *) expr;
						}
						break;
				}
				break;
			}
		case T_Ident:
			{
				result = transformIdent(pstate, (Ident *) expr, precedence);
				break;
			}
		case T_FuncCall:
			{
				FuncCall   *fn = (FuncCall *) expr;
				List	   *args;

				/* transform the list of arguments */
				foreach(args, fn->args)
					lfirst(args) = transformExpr(pstate,
												 (Node *) lfirst(args),
												 precedence);
				result = ParseFuncOrColumn(pstate,
										   fn->funcname,
										   fn->args,
										   fn->agg_star,
										   fn->agg_distinct,
										   precedence);
				break;
			}
		case T_SubLink:
			{
				SubLink    *sublink = (SubLink *) expr;
				List	   *qtrees;
				Query	   *qtree;

				/* If we already transformed this node, do nothing */
				if (IsA(sublink->subselect, Query))
				{
					result = expr;
					break;
				}
				pstate->p_hasSubLinks = true;
				qtrees = parse_analyze(sublink->subselect, pstate);
				if (length(qtrees) != 1)
					elog(ERROR, "Bad query in subselect");
				qtree = (Query *) lfirst(qtrees);
				if (qtree->commandType != CMD_SELECT ||
					qtree->resultRelation != 0)
					elog(ERROR, "Bad query in subselect");
				sublink->subselect = (Node *) qtree;

				if (sublink->subLinkType == EXISTS_SUBLINK)
				{
					/*
					 * EXISTS needs no lefthand or combining operator.
					 * These fields should be NIL already, but make sure.
					 */
					sublink->lefthand = NIL;
					sublink->oper = NIL;
				}
				else if (sublink->subLinkType == EXPR_SUBLINK)
				{
					List	   *tlist = qtree->targetList;

					/*
					 * Make sure the subselect delivers a single column
					 * (ignoring resjunk targets).
					 */
					if (tlist == NIL ||
						((TargetEntry *) lfirst(tlist))->resdom->resjunk)
						elog(ERROR, "Subselect must have a field");
					while ((tlist = lnext(tlist)) != NIL)
					{
						if (!((TargetEntry *) lfirst(tlist))->resdom->resjunk)
							elog(ERROR, "Subselect must have only one field");
					}

					/*
					 * EXPR needs no lefthand or combining operator. These
					 * fields should be NIL already, but make sure.
					 */
					sublink->lefthand = NIL;
					sublink->oper = NIL;
				}
				else
				{
					/* ALL, ANY, or MULTIEXPR: generate operator list */
					List	   *left_list = sublink->lefthand;
					List	   *right_list = qtree->targetList;
					char	   *op;
					List	   *elist;

					foreach(elist, left_list)
						lfirst(elist) = transformExpr(pstate, lfirst(elist),
													  precedence);

					Assert(IsA(sublink->oper, A_Expr));
					op = ((A_Expr *) sublink->oper)->opname;
					sublink->oper = NIL;

					/* Combining operators other than =/<> is dubious... */
					if (length(left_list) != 1 &&
						strcmp(op, "=") != 0 && strcmp(op, "<>") != 0)
						elog(ERROR, "Row comparison cannot use '%s'",
							 op);

					/*
					 * Scan subquery's targetlist to find values that will
					 * be matched against lefthand values.	We need to
					 * ignore resjunk targets, so doing the outer
					 * iteration over right_list is easier than doing it
					 * over left_list.
					 */
					while (right_list != NIL)
					{
						TargetEntry *tent = (TargetEntry *) lfirst(right_list);
						Node	   *lexpr;
						Operator	optup;
						Form_pg_operator opform;
						Oper	   *newop;

						right_list = lnext(right_list);
						if (tent->resdom->resjunk)
							continue;

						if (left_list == NIL)
							elog(ERROR, "Subselect has too many fields");
						lexpr = lfirst(left_list);
						left_list = lnext(left_list);

						/*
						 * It's OK to use oper() not compatible_oper()
						 * here, because make_subplan() will insert type
						 * coercion calls if needed.
						 */
						optup = oper(op,
									 exprType(lexpr),
									 exprType(tent->expr),
									 false);
						opform = (Form_pg_operator) GETSTRUCT(optup);

						if (opform->oprresult != BOOLOID)
							elog(ERROR, "'%s' result type of '%s' must return '%s'"
								 " to be used with quantified predicate subquery",
								 op, typeidTypeName(opform->oprresult),
								 typeidTypeName(BOOLOID));

						newop = makeOper(oprid(optup),	/* opno */
										 InvalidOid,	/* opid */
										 opform->oprresult);
						sublink->oper = lappend(sublink->oper, newop);
						ReleaseSysCache(optup);
					}
					if (left_list != NIL)
						elog(ERROR, "Subselect has too few fields");
				}
				result = (Node *) expr;
				break;
			}

		case T_CaseExpr:
			{
				CaseExpr   *c = (CaseExpr *) expr;
				CaseExpr   *newc = makeNode(CaseExpr);
				List	   *newargs = NIL;
				List	   *typeids = NIL;
				List	   *args;
				Node	   *defresult;
				Oid			ptype;

				/* transform the list of arguments */
				foreach(args, c->args)
				{
					CaseWhen   *w = (CaseWhen *) lfirst(args);
					CaseWhen   *neww = makeNode(CaseWhen);
					Node	   *warg;

					Assert(IsA(w, CaseWhen));

					warg = w->expr;
					if (c->arg != NULL)
					{
						/* shorthand form was specified, so expand... */
						A_Expr	   *a = makeNode(A_Expr);

						a->oper = OP;
						a->opname = "=";
						a->lexpr = c->arg;
						a->rexpr = warg;
						warg = (Node *) a;
					}
					neww->expr = transformExpr(pstate, warg, precedence);

					if (!coerce_to_boolean(pstate, &neww->expr))
						elog(ERROR, "WHEN clause must have a boolean result");

					/*
					 * result is NULL for NULLIF() construct - thomas
					 * 1998-11-11
					 */
					warg = w->result;
					if (warg == NULL)
					{
						A_Const    *n = makeNode(A_Const);

						n->val.type = T_Null;
						warg = (Node *) n;
					}
					neww->result = transformExpr(pstate, warg, precedence);

					newargs = lappend(newargs, neww);
					typeids = lappendi(typeids, exprType(neww->result));
				}

				newc->args = newargs;

				/*
				 * It's not shorthand anymore, so drop the implicit
				 * argument. This is necessary to keep any re-application
				 * of transformExpr from doing the wrong thing.
				 */
				newc->arg = NULL;

				/* transform the default clause */
				defresult = c->defresult;
				if (defresult == NULL)
				{
					A_Const    *n = makeNode(A_Const);

					n->val.type = T_Null;
					defresult = (Node *) n;
				}
				newc->defresult = transformExpr(pstate, defresult, precedence);

				/*
				 * Note: default result is considered the most significant
				 * type in determining preferred type.	This is how the
				 * code worked before, but it seems a little bogus to me
				 * --- tgl
				 */
				typeids = lconsi(exprType(newc->defresult), typeids);

				ptype = select_common_type(typeids, "CASE");
				newc->casetype = ptype;

				/* Convert default result clause, if necessary */
				newc->defresult = coerce_to_common_type(pstate,
														newc->defresult,
														ptype,
														"CASE/ELSE");

				/* Convert when-clause results, if necessary */
				foreach(args, newc->args)
				{
					CaseWhen   *w = (CaseWhen *) lfirst(args);

					w->result = coerce_to_common_type(pstate,
													  w->result,
													  ptype,
													  "CASE/WHEN");
				}

				result = (Node *) newc;
				break;
			}

		case T_NullTest:
			{
				NullTest   *n = (NullTest *) expr;

				n->arg = transformExpr(pstate, n->arg, precedence);
				/* the argument can be any type, so don't coerce it */
				result = expr;
				break;
			}

		case T_BooleanTest:
			{
				BooleanTest *b = (BooleanTest *) expr;

				b->arg = transformExpr(pstate, b->arg, precedence);

				if (!coerce_to_boolean(pstate, &b->arg))
				{
					const char *clausename;

					switch (b->booltesttype)
					{
						case IS_TRUE:
							clausename = "IS TRUE";
							break;
						case IS_NOT_TRUE:
							clausename = "IS NOT TRUE";
							break;
						case IS_FALSE:
							clausename = "IS FALSE";
							break;
						case IS_NOT_FALSE:
							clausename = "IS NOT FALSE";
							break;
						case IS_UNKNOWN:
							clausename = "IS UNKNOWN";
							break;
						case IS_NOT_UNKNOWN:
							clausename = "IS NOT UNKNOWN";
							break;
						default:
							elog(ERROR, "transformExpr: unexpected booltesttype %d",
								 (int) b->booltesttype);
							clausename = NULL;	/* keep compiler quiet */
					}

					elog(ERROR, "Argument of %s must be boolean",
						 clausename);
				}
				result = expr;
				break;
			}

			/*
			 * Quietly accept node types that may be presented when we are
			 * called on an already-transformed tree.
			 *
			 * Do any other node types need to be accepted?  For now we are
			 * taking a conservative approach, and only accepting node
			 * types that are demonstrably necessary to accept.
			 */
		case T_Expr:
		case T_Var:
		case T_Const:
		case T_Param:
		case T_Aggref:
		case T_ArrayRef:
		case T_FieldSelect:
		case T_RelabelType:
			{
				result = (Node *) expr;
				break;
			}

		default:
			/* should not reach here */
			elog(ERROR, "transformExpr: does not know how to transform node %d"
				 " (internal error)", nodeTag(expr));
			break;
	}

	expr_depth_counter--;

	return result;
}

static Node *
transformIndirection(ParseState *pstate, Node *basenode, List *indirection)
{
	if (indirection == NIL)
		return basenode;
	return (Node *) transformArraySubscripts(pstate,
											 basenode, exprType(basenode),
											 indirection, false, NULL);
}

static Node *
transformAttr(ParseState *pstate, Attr *att, int precedence)
{
	Node	   *basenode;

	basenode = ParseNestedFuncOrColumn(pstate, att, precedence);
	return transformIndirection(pstate, basenode, att->indirection);
}

static Node *
transformIdent(ParseState *pstate, Ident *ident, int precedence)
{
	Node	   *result = NULL;
	int			sublevels_up;

	/*
	 * try to find the ident as a relation ... but not if subscripts
	 * appear
	 */
	if (ident->indirection == NIL &&
	 refnameRangeOrJoinEntry(pstate, ident->name, &sublevels_up) != NULL)
	{
		ident->isRel = TRUE;
		result = (Node *) ident;
	}

	if (result == NULL || precedence == EXPR_COLUMN_FIRST)
	{
		/* try to find the ident as a column */
		Node	   *var = colnameToVar(pstate, ident->name);

		if (var != NULL)
		{
			ident->isRel = FALSE;
			result = transformIndirection(pstate, var, ident->indirection);
		}
	}

	if (result == NULL)
		elog(ERROR, "Attribute '%s' not found", ident->name);

	return result;
}

/*
 *	exprType -
 *	  returns the Oid of the type of the expression. (Used for typechecking.)
 */
Oid
exprType(Node *expr)
{
	Oid			type = (Oid) InvalidOid;

	if (!expr)
		return type;

	switch (nodeTag(expr))
	{
		case T_Func:
			type = ((Func *) expr)->functype;
			break;
		case T_Iter:
			type = ((Iter *) expr)->itertype;
			break;
		case T_Var:
			type = ((Var *) expr)->vartype;
			break;
		case T_Expr:
			type = ((Expr *) expr)->typeOid;
			break;
		case T_Const:
			type = ((Const *) expr)->consttype;
			break;
		case T_ArrayRef:
			type = ((ArrayRef *) expr)->refelemtype;
			break;
		case T_Aggref:
			type = ((Aggref *) expr)->aggtype;
			break;
		case T_Param:
			type = ((Param *) expr)->paramtype;
			break;
		case T_FieldSelect:
			type = ((FieldSelect *) expr)->resulttype;
			break;
		case T_RelabelType:
			type = ((RelabelType *) expr)->resulttype;
			break;
		case T_SubLink:
			{
				SubLink    *sublink = (SubLink *) expr;

				if (sublink->subLinkType == EXPR_SUBLINK)
				{
					/* get the type of the subselect's first target column */
					Query	   *qtree = (Query *) sublink->subselect;
					TargetEntry *tent;

					if (!qtree || !IsA(qtree, Query))
						elog(ERROR, "Cannot get type for untransformed sublink");
					tent = (TargetEntry *) lfirst(qtree->targetList);
					type = tent->resdom->restype;
				}
				else
				{
					/* for all other sublink types, result is boolean */
					type = BOOLOID;
				}
			}
			break;
		case T_CaseExpr:
			type = ((CaseExpr *) expr)->casetype;
			break;
		case T_CaseWhen:
			type = exprType(((CaseWhen *) expr)->result);
			break;
		case T_NullTest:
			type = BOOLOID;
			break;
		case T_BooleanTest:
			type = BOOLOID;
			break;
		case T_Ident:
			/* XXX is this right? */
			type = UNKNOWNOID;
			break;
		default:
			elog(ERROR, "Do not know how to get type for %d node",
				 nodeTag(expr));
			break;
	}
	return type;
}

/*
 *	exprTypmod -
 *	  returns the type-specific attrmod of the expression, if it can be
 *	  determined.  In most cases, it can't and we return -1.
 */
int32
exprTypmod(Node *expr)
{
	if (!expr)
		return -1;

	switch (nodeTag(expr))
	{
		case T_Var:
			return ((Var *) expr)->vartypmod;
		case T_Const:
			{
				/* Be smart about string constants... */
				Const	   *con = (Const *) expr;

				switch (con->consttype)
				{
					case BPCHAROID:
						if (!con->constisnull)
							return VARSIZE(DatumGetPointer(con->constvalue));
						break;
					default:
						break;
				}
			}
			break;
		case T_Expr:
			{
				int32		coercedTypmod;

				/* Be smart about length-coercion functions... */
				if (exprIsLengthCoercion(expr, &coercedTypmod))
					return coercedTypmod;
			}
			break;
		case T_FieldSelect:
			return ((FieldSelect *) expr)->resulttypmod;
			break;
		case T_RelabelType:
			return ((RelabelType *) expr)->resulttypmod;
			break;
		case T_CaseExpr:
			{
				/*
				 * If all the alternatives agree on type/typmod, return
				 * that typmod, else use -1
				 */
				CaseExpr   *cexpr = (CaseExpr *) expr;
				Oid			casetype = cexpr->casetype;
				int32		typmod;
				List	   *arg;

				if (!cexpr->defresult)
					return -1;
				if (exprType(cexpr->defresult) != casetype)
					return -1;
				typmod = exprTypmod(cexpr->defresult);
				if (typmod < 0)
					return -1;	/* no point in trying harder */
				foreach(arg, cexpr->args)
				{
					CaseWhen   *w = (CaseWhen *) lfirst(arg);

					Assert(IsA(w, CaseWhen));
					if (exprType(w->result) != casetype)
						return -1;
					if (exprTypmod(w->result) != typmod)
						return -1;
				}
				return typmod;
			}
			break;
		default:
			break;
	}
	return -1;
}

/*
 * exprIsLengthCoercion
 *		Detect whether an expression tree is an application of a datatype's
 *		typmod-coercion function.  Optionally extract the result's typmod.
 *
 * If coercedTypmod is not NULL, the typmod is stored there if the expression
 * is a length-coercion function, else -1 is stored there.
 *
 * We assume that a two-argument function named for a datatype, whose
 * output and first argument types are that datatype, and whose second
 * input is an int32 constant, represents a forced length coercion.
 *
 * XXX It'd be better if the parsetree retained some explicit indication
 * of the coercion, so we didn't need these heuristics.
 */
bool
exprIsLengthCoercion(Node *expr, int32 *coercedTypmod)
{
	Func	   *func;
	Const	   *second_arg;
	HeapTuple	procTuple;
	HeapTuple	typeTuple;
	Form_pg_proc procStruct;
	Form_pg_type typeStruct;

	if (coercedTypmod != NULL)
		*coercedTypmod = -1;	/* default result on failure */

	/* Is it a function-call at all? */
	if (expr == NULL ||
		!IsA(expr, Expr) ||
		((Expr *) expr)->opType != FUNC_EXPR)
		return false;
	func = (Func *) (((Expr *) expr)->oper);
	Assert(IsA(func, Func));

	/*
	 * If it's not a two-argument function with the second argument being
	 * an int4 constant, it can't have been created from a length
	 * coercion.
	 */
	if (length(((Expr *) expr)->args) != 2)
		return false;
	second_arg = (Const *) lsecond(((Expr *) expr)->args);
	if (!IsA(second_arg, Const) ||
		second_arg->consttype != INT4OID ||
		second_arg->constisnull)
		return false;

	/*
	 * Lookup the function in pg_proc
	 */
	procTuple = SearchSysCache(PROCOID,
							   ObjectIdGetDatum(func->funcid),
							   0, 0, 0);
	if (!HeapTupleIsValid(procTuple))
		elog(ERROR, "cache lookup for proc %u failed", func->funcid);
	procStruct = (Form_pg_proc) GETSTRUCT(procTuple);

	/*
	 * It must be a function with two arguments where the first is of the
	 * same type as the return value and the second is an int4. Also, just
	 * to be sure, check return type agrees with expr node.
	 */
	if (procStruct->pronargs != 2 ||
		procStruct->prorettype != procStruct->proargtypes[0] ||
		procStruct->proargtypes[1] != INT4OID ||
		procStruct->prorettype != ((Expr *) expr)->typeOid)
	{
		ReleaseSysCache(procTuple);
		return false;
	}

	/*
	 * Furthermore, the name of the function must be the same as the
	 * argument/result type's name.
	 */
	typeTuple = SearchSysCache(TYPEOID,
							   ObjectIdGetDatum(procStruct->prorettype),
							   0, 0, 0);
	if (!HeapTupleIsValid(typeTuple))
		elog(ERROR, "cache lookup for type %u failed",
			 procStruct->prorettype);
	typeStruct = (Form_pg_type) GETSTRUCT(typeTuple);
	if (strncmp(NameStr(procStruct->proname),
				NameStr(typeStruct->typname),
				NAMEDATALEN) != 0)
	{
		ReleaseSysCache(procTuple);
		ReleaseSysCache(typeTuple);
		return false;
	}

	/*
	 * OK, it is indeed a length-coercion function.
	 */
	if (coercedTypmod != NULL)
		*coercedTypmod = DatumGetInt32(second_arg->constvalue);

	ReleaseSysCache(procTuple);
	ReleaseSysCache(typeTuple);
	return true;
}

/*
 * Produce an appropriate Const node from a constant value produced
 * by the parser and an explicit type name to cast to.
 */
static Node *
parser_typecast_constant(Value *expr, TypeName *typename)
{
	Type		tp;
	Datum		datum;
	Const	   *con;
	char	   *const_string = NULL;
	bool		string_palloced = false;
	bool		isNull = false;

	tp = typenameType(TypeNameToInternalName(typename));

	switch (nodeTag(expr))
	{
		case T_Integer:
			const_string = DatumGetCString(DirectFunctionCall1(int4out,
										 Int32GetDatum(expr->val.ival)));
			string_palloced = true;
			break;
		case T_Float:
		case T_String:
		case T_BitString:
			const_string = expr->val.str;
			break;
		case T_Null:
			isNull = true;
			break;
		default:
			elog(ERROR, "Cannot cast this expression to type '%s'",
				 typeTypeName(tp));
	}

	if (isNull)
		datum = (Datum) NULL;
	else
		datum = stringTypeDatum(tp, const_string, typename->typmod);

	con = makeConst(typeTypeId(tp),
					typeLen(tp),
					datum,
					isNull,
					typeByVal(tp),
					false,		/* not a set */
					true /* is cast */ );

	if (string_palloced)
		pfree(const_string);

	ReleaseSysCache(tp);

	return (Node *) con;
}

/*
 * Handle an explicit CAST applied to a non-constant expression.
 * (Actually, this works for constants too, but gram.y won't generate
 * a TypeCast node if the argument is just a constant.)
 *
 * The given expr has already been transformed, but we need to lookup
 * the type name and then apply any necessary coercion function(s).
 */
static Node *
parser_typecast_expression(ParseState *pstate,
						   Node *expr, TypeName *typename)
{
	Oid			inputType = exprType(expr);
	Oid			targetType;

	targetType = typenameTypeId(TypeNameToInternalName(typename));

	if (inputType == InvalidOid)
		return expr;			/* do nothing if NULL input */

	if (inputType != targetType)
	{
		expr = CoerceTargetExpr(pstate, expr, inputType,
								targetType, typename->typmod);
		if (expr == NULL)
			elog(ERROR, "Cannot cast type '%s' to '%s'",
				 format_type_be(inputType),
				 format_type_be(targetType));
	}

	/*
	 * If the target is a fixed-length type, it may need a length coercion
	 * as well as a type coercion.
	 */
	expr = coerce_type_typmod(pstate, expr,
							  targetType, typename->typmod);

	return expr;
}

/*
 * Given a TypeName node as returned by the grammar, generate the internal
 * name of the corresponding type.	Note this does NOT check if the type
 * exists or not.
 */
char *
TypeNameToInternalName(TypeName *typename)
{
	Assert(typename->attrname == NULL);
	if (typename->arrayBounds != NIL)
	{
		/*
		 * By convention, the name of an array type is the name of its
		 * element type with "_" prepended.
		 */
		char	   *arrayname = palloc(strlen(typename->name) + 2);

		sprintf(arrayname, "_%s", typename->name);
		return arrayname;
	}
	else
		return typename->name;
}
