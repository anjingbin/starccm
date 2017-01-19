/* $Id: srandom.c,v 1.2 2004/07/05 09:13:23 SuLiang Exp $ */

#include "c.h"

#include <stdlib.h>
#include <math.h>
#include <errno.h>

void
srandom(unsigned int seed)
{
	srand48((long int) seed);
}
