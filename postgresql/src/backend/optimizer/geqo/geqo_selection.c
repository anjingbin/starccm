/*-------------------------------------------------------------------------
 *
 * geqo_selection.c
 *	  linear selection scheme for the genetic query optimizer
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: geqo_selection.c,v 1.2 2004/07/05 08:55:29 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

/* contributed by:
   =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
   *  Martin Utesch				 * Institute of Automatic Control	   *
   =							 = University of Mining and Technology =
   *  utesch@aut.tu-freiberg.de  * Freiberg, Germany				   *
   =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
 */

/* this is adopted from D. Whitley's Genitor algorithm */

/*************************************************************/
/*															 */
/*	Copyright (c) 1990										 */
/*	Darrell L. Whitley										 */
/*	Computer Science Department								 */
/*	Colorado State University								 */
/*															 */
/*	Permission is hereby granted to copy all or any part of  */
/*	this program for free distribution.   The author's name  */
/*	and this copyright notice must be included in any copy.  */
/*															 */
/*************************************************************/

#include <math.h>

#include "postgres.h"
#include "optimizer/geqo_copy.h"
#include "optimizer/geqo_random.h"
#include "optimizer/geqo_selection.h"

static int	linear(int max, double bias);

/* geqo_selection
 *
 *	 according to bias described by input parameters,
 *	 second genes are selected from the pool
 */
void
geqo_selection(Chromosome *momma, Chromosome *daddy, Pool *pool, double bias)
{
	int			first,
				second;

	first = (int) linear(pool->size, bias);
	second = (int) linear(pool->size, bias);

	if (pool->size > 1)
	{
		while (first == second)
			second = (int) linear(pool->size, bias);
	}

	geqo_copy(momma, &pool->data[first], pool->string_length);
	geqo_copy(daddy, &pool->data[second], pool->string_length);
}

/* linear
 *	  generates random integer between 0 and input max number
 *	  using input linear bias
 *
 *	  probability distribution function is: f(x) = bias - 2(bias - 1)x
 *			 bias = (prob of first rule) / (prob of middle rule)
 *
 */

static int
linear(int pool_size, double bias)		/* bias is y-intercept of linear
										 * distribution */
{
	double		index;			/* index between 0 and pop_size */
	double		max = (double) pool_size;

	index = max * (bias - sqrt((bias * bias) - 4.0 * (bias - 1.0) * geqo_rand()))
		/ 2.0 / (bias - 1.0);

	return (int) index;
}
