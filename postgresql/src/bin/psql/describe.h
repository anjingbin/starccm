/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/psql/describe.h,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
 */
#ifndef DESCRIBE_H
#define DESCRIBE_H

#include "settings.h"

/* \da */
bool		describeAggregates(const char *name);

/* \df */
bool		describeFunctions(const char *name, bool verbose);

/* \dT */
bool		describeTypes(const char *name, bool verbose);

/* \do */
bool		describeOperators(const char *name);

/* \du */
bool		describeUsers(const char *name);

/* \z (or \dp) */
bool		permissionsList(const char *name);

/* \dd */
bool		objectDescription(const char *object);

/* \d foo */
bool		describeTableDetails(const char *name, bool desc);

/* \l */
bool		listAllDbs(bool desc);

/* \dt, \di, \ds, \dS, etc. */
bool		listTables(const char *infotype, const char *name, bool desc);

#endif   /* DESCRIBE_H */
