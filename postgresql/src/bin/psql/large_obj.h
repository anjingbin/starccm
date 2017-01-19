/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/psql/large_obj.h,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
 */
#ifndef LARGE_OBJ_H
#define LARGE_OBJ_H

bool		do_lo_export(const char *loid_arg, const char *filename_arg);
bool		do_lo_import(const char *filename_arg, const char *comment_arg);
bool		do_lo_unlink(const char *loid_arg);
bool		do_lo_list(void);

#endif   /* LARGE_OBJ_H */
