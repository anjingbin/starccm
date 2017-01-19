/*-------------------------------------------------------------------------
 *
 * clausesel.c
 *	  Routines to compute clause selectivities
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/path/clausesel.c,v 1.2 2004/07/05 08:55:29 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "catalog/pg_operator.h"
#include "catalog/pg_type.h"
#include "nodes/makefuncs.h"
#include "optimizer/clauses.h"
#include "optimizer/cost.h"
#include "optimizer/plancat.h"
#include "optimizer/restrictinfo.h"
#include "parser/parsetree.h"
#include "utils/fmgroids.h"
#include "utils/lsyscache.h"
#include "utils/selfuncs.h"


/* note that pg_type.h hardwires size of bool as 1 ... duplicate it */
#define MAKEBOOLCONST(val,isnull) \
	((Node *) makeConst(BOOLOID, 1, (Datum) (val), \
						(isnull), true, false, false))


/*
 * Data structure for accumulating info about possible range-query
 * clause pairs in clauselist_selectivity.
 */
typedef struct RangeQueryClause
{
	struct RangeQueryClause *next;		/* next in linked list */
	Node	   *var;			/* The common variable of the clauses */
	bool		have_lobound;	/* found a low-bound clause yet? */
	bool		have_hibound;	/* found a high-bound clause yet? */
	Selectivity lobound;		/* Selectivity of a var > something clause */
	Selectivity hibound;		/* Selectivity of a var < something clause */
} RangeQueryClause;

static void addRangeClause(RangeQueryClause **rqlist, Node *clause,
			   bool varonleft, bool isLTsel, Selectivity s2);


/****************************************************************************
 *		ROUTINES TO COMPUTE SELECTIVITIES
 ****************************************************************************/

/*
 * restrictlist_selectivity -
 *	  Compute the selectivity of an implicitly-ANDed list of RestrictInfo
 *	  clauses.
 *
 * This is the same as clauselist_selectivity except for the representation
 * of the clause list.
 */
Selectivity
restrictlist_selectivity(Query *root,
						 List *restrictinfo_list,
						 int varRelid)
{
	List	   *clauselist = get_actual_clauses(restrictinfo_list);
	Selectivity result;

	result = clauselist_selectivity(root, clauselist, varRelid);
	freeList(clauselist);
	return result;
}

/*
 * clauselist_selectivity -
 *	  Compute the selectivity of an implicitly-ANDed list of boolean
 *	  expression clauses.  The list can be empty, in which case 1.0
 *	  must be returned.
 *
 * See clause_selectivity() for the meaning of the varRelid parameter.
 *
 * Our basic approach is to take the product of the selectivities of the
 * subclauses.	However, that's only right if the subclauses have independent
 * probabilities, and in reality they are often NOT independent.  So,
 * we want to be smarter where we can.

 * Currently, the only extra smarts we have is to recognize "range queries",
 * such as "x > 34 AND x < 42".  Clauses are recognized as possible range
 * query components if they are restriction opclauses whose operators have
 * scalarltsel() or scalargtsel() as their restriction selectivity estimator.
 * We pair up clauses of this form that refer to the same variable.  An
 * unpairable clause of this kind is simply multiplied into the selectivity
 * product in the normal way.  But when we find a pair, we know that the
 * selectivities represent the relative positions of the low and high bounds
 * within the column's range, so instead of figuring the selectivity as
 * hisel * losel, we can figure it as hisel + losel - 1.  (To visualize this,
 * see that hisel is the fraction of the range below the high bound, while
 * losel is the fraction above the low bound; so hisel can be interpreted
 * directly as a 0..1 value but we need to convert losel to 1-losel before
 * interpreting it as a value.	Then the available range is 1-losel to hisel.)
 * If the calculation yields zero or negative, however, we chicken out and
 * use a default estimate; that probably means that one or both
 * selectivities is a default estimate rather than an actual range value.
 * Of course this is all very dependent on the behavior of
 * scalarltsel/scalargtsel; perhaps some day we can generalize the approach.
 */
Selectivity
clauselist_selectivity(Query *root,
					   List *clauses,
					   int varRelid)
{
	Selectivity s1 = 1.0;
	RangeQueryClause *rqlist = NULL;
	List	   *clist;

	/*
	 * Initial scan over clauses.  Anything that doesn't look like a
	 * potential rangequery clause gets multiplied into s1 and forgotten.
	 * Anything that does gets inserted into an rqlist entry.
	 */
	foreach(clist, clauses)
	{
		Node	   *clause = (Node *) lfirst(clist);
		Selectivity s2;

		/*
		 * See if it looks like a restriction clause with a pseudoconstant
		 * on one side.  (Anything more complicated than that might not
		 * behave in the simple way we are expecting.)
		 *
		 * NB: for consistency of results, this fragment of code had better
		 * match what clause_selectivity() would do in the cases it
		 * handles.
		 */
		if (is_opclause(clause) &&
			(varRelid != 0 || NumRelids(clause) == 1))
		{
			Expr	   *expr = (Expr *) clause;

			if (length(expr->args) == 2)
			{
				bool		varonleft = true;

				if (is_pseudo_constant_clause(lsecond(expr->args)) ||
					(varonleft = false,
					 is_pseudo_constant_clause(lfirst(expr->args))))
				{
					Oid			opno = ((Oper *) expr->oper)->opno;
					RegProcedure oprrest = get_oprrest(opno);

					s2 = restriction_selectivity(root, opno,
												 expr->args, varRelid);

					/*
					 * If we reach here, we have computed the same result
					 * that clause_selectivity would, so we can just use
					 * s2 if it's the wrong oprrest.  But if it's the
					 * right oprrest, add the clause to rqlist for later
					 * processing.
					 */
					switch (oprrest)
					{
						case F_SCALARLTSEL:
							addRangeClause(&rqlist, clause,
										   varonleft, true, s2);
							break;
						case F_SCALARGTSEL:
							addRangeClause(&rqlist, clause,
										   varonleft, false, s2);
							break;
						default:
							/* Just merge the selectivity in generically */
							s1 = s1 * s2;
							break;
					}
					continue;	/* drop to loop bottom */
				}
			}
		}
		/* Not the right form, so treat it generically. */
		s2 = clause_selectivity(root, clause, varRelid);
		s1 = s1 * s2;
	}

	/*
	 * Now scan the rangequery pair list.
	 */
	while (rqlist != NULL)
	{
		RangeQueryClause *rqnext;

		if (rqlist->have_lobound && rqlist->have_hibound)
		{
			/* Successfully matched a pair of range clauses */
			Selectivity s2 = rqlist->hibound + rqlist->lobound - 1.0;

			/*
			 * A zero or slightly negative s2 should be converted into a
			 * small positive value; we probably are dealing with a very
			 * tight range and got a bogus result due to roundoff errors.
			 * However, if s2 is very negative, then we probably have
			 * default selectivity estimates on one or both sides of the
			 * range.  In that case, insert a not-so-wildly-optimistic
			 * default estimate.
			 */
			if (s2 <= 0.0)
			{
				if (s2 < -0.01)
				{
					/*
					 * No data available --- use a default estimate that
					 * is small, but not real small.
					 */
					s2 = 0.005;
				}
				else
				{
					/*
					 * It's just roundoff error; use a small positive
					 * value
					 */
					s2 = 1.0e-10;
				}
			}
			/* Merge in the selectivity of the pair of clauses */
			s1 *= s2;
		}
		else
		{
			/* Only found one of a pair, merge it in generically */
			if (rqlist->have_lobound)
				s1 *= rqlist->lobound;
			else
				s1 *= rqlist->hibound;
		}
		/* release storage and advance */
		rqnext = rqlist->next;
		pfree(rqlist);
		rqlist = rqnext;
	}

	return s1;
}

/*
 * addRangeClause --- add a new range clause for clauselist_selectivity
 *
 * Here is where we try to match up pairs of range-query clauses
 */
static void
addRangeClause(RangeQueryClause **rqlist, Node *clause,
			   bool varonleft, bool isLTsel, Selectivity s2)
{
	RangeQueryClause *rqelem;
	Node	   *var;
	bool		is_lobound;

	if (varonleft)
	{
		var = (Node *) get_leftop((Expr *) clause);
		is_lobound = !isLTsel;	/* x < something is high bound */
	}
	else
	{
		var = (Node *) get_rightop((Expr *) clause);
		is_lobound = isLTsel;	/* something < x is low bound */
	}

	for (rqelem = *rqlist; rqelem; rqelem = rqelem->next)
	{
		/*
		 * We use full equal() here because the "var" might be a function
		 * of one or more attributes of the same relation...
		 */
		if (!equal(var, rqelem->var))
			continue;
		/* Found the right group to put this clause in */
		if (is_lobound)
		{
			if (!rqelem->have_lobound)
			{
				rqelem->have_lobound = true;
				rqelem->lobound = s2;
			}
			else
			{

				/*------
				 * We have found two similar clauses, such as
				 * x < y AND x < z.
				 * Keep only the more restrictive one.
				 *------
				 */
				if (rqelem->lobound > s2)
					rqelem->lobound = s2;
			}
		}
		else
		{
			if (!rqelem->have_hibound)
			{
				rqelem->have_hibound = true;
				rqelem->hibound = s2;
			}
			else
			{

				/*------
				 * We have found two similar clauses, such as
				 * x > y AND x > z.
				 * Keep only the more restrictive one.
				 *------
				 */
				if (rqelem->hibound > s2)
					rqelem->hibound = s2;
			}
		}
		return;
	}

	/* No matching var found, so make a new clause-pair data structure */
	rqelem = (RangeQueryClause *) palloc(sizeof(RangeQueryClause));
	rqelem->var = var;
	if (is_lobound)
	{
		rqelem->have_lobound = true;
		rqelem->have_hibound = false;
		rqelem->lobound = s2;
	}
	else
	{
		rqelem->have_lobound = false;
		rqelem->have_hibound = true;
		rqelem->hibound = s2;
	}
	rqelem->next = *rqlist;
	*rqlist = rqelem;
}


/*
 * clause_selectivity -
 *	  Compute the selectivity of a general boolean expression clause.
 *
 * varRelid is either 0 or a rangetable index.
 *
 * When varRelid is not 0, only variables belonging to that relation are
 * considered in computing selectivity; other vars are treated as constants
 * of unknown values.  This is appropriate for estimating the selectivity of
 * a join clause that is being used as a restriction clause in a scan of a
 * nestloop join's inner relation --- varRelid should then be the ID of the
 * inner relation.
 *
 * When varRelid is 0, all variables are treated as variables.	This
 * is appropriate for ordinary join clauses and restriction clauses.
 */
Selectivity
clause_selectivity(Query *root,
				   Node *clause,
				   int varRelid)
{
	Selectivity s1 = 1.0;		/* default for any unhandled clause type */

	if (clause == NULL)
		return s1;
	if (IsA(clause, Var))
	{
		Var		   *var = (Var *) clause;

		/*
		 * We probably shouldn't ever see an uplevel Var here, but if we
		 * do, return the default selectivity...
		 */
		if (var->varlevelsup == 0 &&
			(varRelid == 0 || varRelid == (int) var->varno))
		{
			RangeTblEntry *rte = rt_fetch(var->varno, root->rtable);

			if (rte->subquery)
			{
				/*
				 * XXX not smart about subquery references... any way to
				 * do better?
				 */
				s1 = 0.5;
			}
			else
			{
				/*
				 * A Var at the top of a clause must be a bool Var. This
				 * is equivalent to the clause reln.attribute = 't', so we
				 * compute the selectivity as if that is what we have.
				 */
				s1 = restriction_selectivity(root,
											 BooleanEqualOperator,
											 makeList2(var,
													   MAKEBOOLCONST(true,
																 false)),
											 varRelid);
			}
		}
	}
	else if (IsA(clause, Param))
	{
		/* XXX any way to do better? */
		s1 = 1.0;
	}
	else if (IsA(clause, Const))
	{
		/* bool constant is pretty easy... */
		s1 = ((bool) ((Const *) clause)->constvalue) ? 1.0 : 0.0;
	}
	else if (not_clause(clause))
	{
		/* inverse of the selectivity of the underlying clause */
		s1 = 1.0 - clause_selectivity(root,
							  (Node *) get_notclausearg((Expr *) clause),
									  varRelid);
	}
	else if (and_clause(clause))
	{
		/* share code with clauselist_selectivity() */
		s1 = clauselist_selectivity(root,
									((Expr *) clause)->args,
									varRelid);
	}
	else if (or_clause(clause))
	{
		/*
		 * Selectivities for an 'or' clause are computed as s1+s2 - s1*s2
		 * to account for the probable overlap of selected tuple sets. XXX
		 * is this too conservative?
		 */
		List	   *arg;

		s1 = 0.0;
		foreach(arg, ((Expr *) clause)->args)
		{
			Selectivity s2 = clause_selectivity(root,
												(Node *) lfirst(arg),
												varRelid);

			s1 = s1 + s2 - s1 * s2;
		}
	}
	else if (is_opclause(clause))
	{
		Oid			opno = ((Oper *) ((Expr *) clause)->oper)->opno;
		bool		is_join_clause;

		if (varRelid != 0)
		{
			/*
			 * If we are considering a nestloop join then all clauses are
			 * restriction clauses, since we are only interested in the
			 * one relation.
			 */
			is_join_clause = false;
		}
		else
		{
			/*
			 * Otherwise, it's a join if there's more than one relation
			 * used.
			 */
			is_join_clause = (NumRelids(clause) > 1);
		}

		if (is_join_clause)
		{
			/* Estimate selectivity for a join clause. */
			s1 = join_selectivity(root, opno,
								  ((Expr *) clause)->args);
		}
		else
		{
			/* Estimate selectivity for a restriction clause. */
			s1 = restriction_selectivity(root, opno,
									  ((Expr *) clause)->args, varRelid);
		}
	}
	else if (is_funcclause(clause))
	{
		/*
		 * This is not an operator, so we guess at the selectivity. THIS
		 * IS A HACK TO GET V4 OUT THE DOOR.  FUNCS SHOULD BE ABLE TO HAVE
		 * SELECTIVITIES THEMSELVES.	   -- JMH 7/9/92
		 */
		s1 = (Selectivity) 0.3333333;
	}
	else if (is_subplan(clause))
	{
		/*
		 * Just for the moment! FIX ME! - vadim 02/04/98
		 */
		s1 = (Selectivity) 0.5;
	}
	else if (IsA(clause, NullTest))
	{
		/* Use node specific selectivity calculation function */
		s1 = nulltestsel(root, (NullTest *) clause, varRelid);
	}
	else if (IsA(clause, BooleanTest))
	{
		/* Use node specific selectivity calculation function */
		s1 = booltestsel(root, (BooleanTest *) clause, varRelid);
	}
	else if (IsA(clause, RelabelType))
	{
		/* Not sure this case is needed, but it can't hurt */
		s1 = clause_selectivity(root,
								((RelabelType *) clause)->arg,
								varRelid);
	}

#ifdef SELECTIVITY_DEBUG
	elog(NOTICE, "clause_selectivity: s1 %f", s1);
#endif   /* SELECTIVITY_DEBUG */

	return s1;
}
