/*-------------------------------------------------------------------------
 *
 * dynloader.c
 *	  dynamic loader for QNX4 using the shared library mechanism
 *
 * Copyright (c) 1999, repas AEG Automation GmbH
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/dynloader/qnx4.c,v 1.1 2003/12/30 00:06:08 AnJingBin Exp $
 *
 *	NOTES
 *
 *-------------------------------------------------------------------------
 */
/* System includes */
/*
#include <stdio.h>
#include <a.out.h>
#include <dl.h>
*/
#include "postgres.h"

#include "utils/dynamic_loader.h"
#include "dynloader.h"

void *
pg_dlopen(char *filename)
{
	return (void *) NULL;
}

PGFunction
pg_dlsym(void *handle, char *funcname)
{
	return NULL;
}

void
pg_dlclose(void *handle)
{
}

char *
pg_dlerror()
{
	static char errmsg[] = "Failed to load shared library due to lack of shared library support.";

	return errmsg;
}
