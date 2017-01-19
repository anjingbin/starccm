/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/psql/stringutils.h,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
 */
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

/* The cooler version of strtok() which knows about quotes and doesn't
 * overwrite your input */
extern char *strtokx(const char *s,
		const char *delim,
		const char *quote,
		int escape,
		char *was_quoted,
		unsigned int *token_pos,
		int encoding);

#endif   /* STRINGUTILS_H */
