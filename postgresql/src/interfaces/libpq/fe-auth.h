/*-------------------------------------------------------------------------
 *
 * fe-auth.h
 *
 *	  Definitions for network authentication routines
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: fe-auth.h,v 1.2 2004/07/05 08:56:13 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef FE_AUTH_H
#define FE_AUTH_H

#include "libpq-fe.h"
#include "libpq-int.h"


/*----------------------------------------------------------------
 * Common routines and definitions
 *----------------------------------------------------------------
 */

/* what we call "no authentication system" */
#define UNAUTHNAME				"unauth"

/* what a frontend uses by default */
#if !defined(KRB4) && !defined(KRB5)
#define DEFAULT_CLIENT_AUTHSVC	UNAUTHNAME
#else							/* KRB4 || KRB5 */
#define DEFAULT_CLIENT_AUTHSVC	"kerberos"
#endif   /* KRB4 || KRB5 */

//renyi 11 20 : changed
#ifdef __cplusplus
extern		"C"
{
#endif

extern int fe_sendauth(AuthRequest areq, PGconn *conn, const char *hostname,
			const char *password, char *PQerrormsg);
extern MsgType fe_getauthsvc(char *PQerrormsg);
extern void fe_setauthsvc(const char *name, char *PQerrormsg);
extern char *fe_getauthname(char *PQerrormsg);

#ifdef __cplusplus
}
#endif

#define PG_KRB4_VERSION "PGVER4.1"		/* at most KRB_SENDAUTH_VLEN chars */
#define PG_KRB5_VERSION "PGVER5.1"

#endif   /* FE_AUTH_H */
