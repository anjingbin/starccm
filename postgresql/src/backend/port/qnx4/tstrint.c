/*-------------------------------------------------------------------------
 *
 * tstrint.c
 *	  rint() test
 *
 * Copyright (c) 1999, repas AEG Automation GmbH
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/qnx4/tstrint.c,v 1.2 2004/07/05 09:13:24 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "c.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char **argv)
{
	double		x;

	if (argc != 2)
		exit(1);

	x = strtod(argv[1], NULL);
	printf("rint( %f ) = %f\n", x, rint(x));

	return 0;
}
