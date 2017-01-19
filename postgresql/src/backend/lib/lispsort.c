/*-------------------------------------------------------------------------
 *
 * lispsort.c
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/lib/lispsort.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include <sys/types.h>

#include "postgres.h"


#ifdef NOT_USED
/*
** lisp_qsort: Takes a lisp list as input, copies it into an array of lisp
**			   nodes which it sorts via qsort() with the comparison function
**			   as passed into lisp_qsort(), and returns a new list with
**			   the nodes sorted.  The old list is *not* freed or modified (?)
*/
List *
lisp_qsort(List *the_list,		/* the list to be sorted */
		   int (*compare) ())	/* function to compare two nodes */
{
	int			i;
	size_t		num;
	List	  **nodearray;
	List	   *tmp,
			   *output;

	/* find size of list */
	num = length(the_list);
	if (num < 2)
		return copyObject(the_list);

	/* copy elements of the list into an array */
	nodearray = (List **) palloc(num * sizeof(List *));

	for (tmp = the_list, i = 0; tmp != NIL; tmp = lnext(tmp), i++)
		nodearray[i] = copyObject(lfirst(tmp));

	/* sort the array */
	pg_qsort(nodearray, num, sizeof(List *), compare);

	/* lcons together the array elements */
	output = NIL;
	for (i = num - 1; i >= 0; i--)
		output = lcons(nodearray[i], output);

	return output;
}

#endif
