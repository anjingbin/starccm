/*-------------------------------------------------------------------------
 *
 * libpgtcl.h
 *
 *	libpgtcl is a tcl package for front-ends to interface with PostgreSQL.
 *	It's a Tcl wrapper for libpq.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: libpgtcl.h,v 1.2 2004/07/05 08:56:13 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef LIBPGTCL_H
#define LIBPGTCL_H

#include <tcl.h>

extern int	Pgtcl_Init(Tcl_Interp *interp);
extern int	Pgtcl_SafeInit(Tcl_Interp *interp);

#endif   /* LIBPGTCL_H */
