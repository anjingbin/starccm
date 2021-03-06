/*-------------------------------------------------------------------------
 *
 * libpq-fs.h
 *	  definitions for using Inversion file system routines (ie, large objects)
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: libpq-fs.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef LIBPQ_FS_H
#define LIBPQ_FS_H

/*
 *	Read/write mode flags for inversion (large object) calls
 */

#define INV_WRITE		0x00020000
#define INV_READ		0x00040000

#endif   /* LIBPQ_FS_H */
