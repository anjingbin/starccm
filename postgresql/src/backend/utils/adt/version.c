/*-------------------------------------------------------------------------
 *
 * version.c
 *	 Returns the PostgreSQL version string
 *
 * IDENTIFICATION
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/adt/version.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "utils/builtins.h"


Datum
pgsql_version(PG_FUNCTION_ARGS)
{
	int			n = strlen(PG_VERSION_STR);
	text	   *ret = (text *) palloc(n + VARHDRSZ);

	VARATT_SIZEP(ret) = n + VARHDRSZ;
	memcpy(VARDATA(ret), PG_VERSION_STR, n);

	PG_RETURN_TEXT_P(ret);
}
