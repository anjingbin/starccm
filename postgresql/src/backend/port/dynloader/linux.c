/*-------------------------------------------------------------------------
 *
 * dynloader.c
 *	  Dynamic Loader for Postgres for Linux, generated from those for
 *	  Ultrix.
 *
 *	  You need to install the dld library on your Linux system!
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/dynloader/linux.c,v 1.1 2003/12/30 00:06:08 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#ifdef HAVE_DLD_H
#include "dld.h"
#endif

#ifdef NOT_USED
extern char pg_pathname[];

void *
pg_dlopen(char *filename)
{
#ifndef HAVE_DLD_H
	elog(ERROR, "dynamic load not supported");
	return NULL;
#else
	static int	dl_initialized = 0;

	/*
	 * initializes the dynamic loader with the executable's pathname.
	 * (only needs to do this the first time pg_dlopen is called.)
	 */
	if (!dl_initialized)
	{
		if (dld_init(dld_find_executable(pg_pathname)))
			return NULL;

		/*
		 * if there are undefined symbols, we want dl to search from the
		 * following libraries also.
		 */
		dl_initialized = 1;
	}

	/*
	 * link the file, then check for undefined symbols!
	 */
	if (dld_link(filename))
		return NULL;

	/*
	 * If undefined symbols: try to link with the C and math libraries!
	 * This could be smarter, if the dynamic linker was able to handle
	 * shared libs!
	 */
	if (dld_undefined_sym_count > 0)
	{
		if (dld_link("/usr/lib/libc.a"))
		{
			elog(NOTICE, "dld: Cannot link C library!");
			return NULL;
		}
		if (dld_undefined_sym_count > 0)
		{
			if (dld_link("/usr/lib/libm.a"))
			{
				elog(NOTICE, "dld: Cannot link math library!");
				return NULL;
			}
			if (dld_undefined_sym_count > 0)
			{
				int			count = dld_undefined_sym_count;
				char	  **list = dld_list_undefined_sym();

				/* list the undefined symbols, if any */
				elog(NOTICE, "dld: Undefined:");
				do
				{
					elog(NOTICE, "  %s", *list);
					list++;
					count--;
				} while (count > 0);

				dld_unlink_by_file(filename, 1);
				return NULL;
			}
		}
	}

	return (void *) strdup(filename);
#endif
}

char *
pg_dlerror()
{
#ifndef HAVE_DLD_H
	return "dynaloader unspported";
#else
	return dld_strerror(dld_errno);
#endif
}

#endif
