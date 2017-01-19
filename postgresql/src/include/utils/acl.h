/*-------------------------------------------------------------------------
 *
 * acl.h
 *	  Definition of (and support for) access control list data structures.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: acl.h,v 1.2 2004/07/05 08:56:08 SuLiang Exp $
 *
 * NOTES
 *	  For backward-compatibility purposes we have to allow there
 *	  to be a null ACL in a pg_class tuple.  This will be defined as
 *	  meaning "default protection" (i.e., whatever acldefault() returns).
 *
 *	  The AclItems in an ACL array are currently kept in sorted order.
 *	  Things will break hard if you change that without changing the
 *	  code wherever this is included.
 *-------------------------------------------------------------------------
 */
#ifndef ACL_H
#define ACL_H

#include "nodes/parsenodes.h"
#include "utils/array.h"
#include "utils/memutils.h"

/*
 * AclId		system identifier for the user, group, etc.
 *				XXX currently UNIX uid for users...
 */
typedef uint32 AclId;

#define ACL_ID_WORLD	0		/* placeholder for id in a WORLD acl item */

/*
 * AclIdType	tag that describes if the AclId is a user, group, etc.
 */
typedef uint8 AclIdType;

#define ACL_IDTYPE_WORLD		0x00
#define ACL_IDTYPE_UID			0x01	/* user id - from pg_shadow */
#define ACL_IDTYPE_GID			0x02	/* group id - from pg_group */

/*
 * AclMode		the actual permissions
 *				XXX should probably use bit.h routines.
 *				XXX should probably also stuff the modechg cruft in the
 *					high bits, too.
 */
typedef uint8 AclMode;

#define ACL_NO			0		/* no permissions */
#define ACL_INSERT		(1<<0)
#define ACL_SELECT		(1<<1)
#define ACL_UPDATE		(1<<2)
#define ACL_DELETE		(1<<3)
#define ACL_RULE		(1<<4)
#define ACL_REFERENCES	(1<<5)
#define ACL_TRIGGER		(1<<6)
#define N_ACL_MODES		7		/* 1 plus the last 1<<x */

/*
 * AclItem
 */
typedef struct AclItem
{
	AclId		ai_id;
	AclIdType	ai_idtype;
	AclMode		ai_mode;

	/*
	 * This is actually type 'aclitem', and we want a fixed size for all
	 * platforms, so we pad this with dummies.
	 */
	char		dummy1,
				dummy2;
} AclItem;

/* Note: if the size of AclItem changes,
   change the aclitem typlen in pg_type.h */


/*
 * Definitions for convenient access to Acl (array of AclItem) and IdList
 * (array of AclId).  These are standard Postgres arrays, but are restricted
 * to have one dimension.  We also ignore the lower bound when reading,
 * and set it to zero when writing.
 *
 * CAUTION: as of Postgres 7.1, these arrays are toastable (just like all
 * other array types).	Therefore, be careful to detoast them with the
 * macros provided, unless you know for certain that a particular array
 * can't have been toasted.  Presently, we do not provide toast tables for
 * pg_class or pg_group, so the entries in those tables won't have been
 * stored externally --- but they could have been compressed!
 */


/*
 * Acl			a one-dimensional POSTGRES array of AclItem
 */
typedef ArrayType Acl;

#define ACL_NUM(ACL)			(ARR_DIMS(ACL)[0])
#define ACL_DAT(ACL)			((AclItem *) ARR_DATA_PTR(ACL))
#define ACL_N_SIZE(N)			(ARR_OVERHEAD(1) + ((N) * sizeof(AclItem)))
#define ACL_SIZE(ACL)			ARR_SIZE(ACL)

/*
 * IdList		a one-dimensional POSTGRES array of AclId
 */
typedef ArrayType IdList;

#define IDLIST_NUM(IDL)			(ARR_DIMS(IDL)[0])
#define IDLIST_DAT(IDL)			((AclId *) ARR_DATA_PTR(IDL))
#define IDLIST_N_SIZE(N)		(ARR_OVERHEAD(1) + ((N) * sizeof(AclId)))
#define IDLIST_SIZE(IDL)		ARR_SIZE(IDL)

/*
 * fmgr macros for these types
 */
#define DatumGetAclItemP(X)		   ((AclItem *) DatumGetPointer(X))
#define PG_GETARG_ACLITEM_P(n)	   DatumGetAclItemP(PG_GETARG_DATUM(n))
#define PG_RETURN_ACLITEM_P(x)	   PG_RETURN_POINTER(x)

#define DatumGetAclP(X)			   ((Acl *) PG_DETOAST_DATUM(X))
#define DatumGetAclPCopy(X)		   ((Acl *) PG_DETOAST_DATUM_COPY(X))
#define PG_GETARG_ACL_P(n)		   DatumGetAclP(PG_GETARG_DATUM(n))
#define PG_GETARG_ACL_P_COPY(n)    DatumGetAclPCopy(PG_GETARG_DATUM(n))
#define PG_RETURN_ACL_P(x)		   PG_RETURN_POINTER(x)

#define DatumGetIdListP(X)		   ((IdList *) PG_DETOAST_DATUM(X))
#define DatumGetIdListPCopy(X)	   ((IdList *) PG_DETOAST_DATUM_COPY(X))
#define PG_GETARG_IDLIST_P(n)	   DatumGetIdListP(PG_GETARG_DATUM(n))
#define PG_GETARG_IDLIST_P_COPY(n) DatumGetIdListPCopy(PG_GETARG_DATUM(n))
#define PG_RETURN_IDLIST_P(x)	   PG_RETURN_POINTER(x)


/*
 * ACL modification opcodes
 */
#define ACL_MODECHG_ADD			1
#define ACL_MODECHG_DEL			2
#define ACL_MODECHG_EQL			3

/* mode indicators for I/O */
#define ACL_MODECHG_STR			"+-="	/* list of valid characters */
#define ACL_MODECHG_ADD_CHR		'+'
#define ACL_MODECHG_DEL_CHR		'-'
#define ACL_MODECHG_EQL_CHR		'='
#define ACL_MODE_STR			"arwdRxt"		/* list of valid
												 * characters */
#define ACL_MODE_INSERT_CHR		'a'		/* formerly known as "append" */
#define ACL_MODE_SELECT_CHR		'r'		/* formerly known as "read" */
#define ACL_MODE_UPDATE_CHR		'w'		/* formerly known as "write" */
#define ACL_MODE_DELETE_CHR		'd'
#define ACL_MODE_RULE_CHR		'R'
#define ACL_MODE_REFERENCES_CHR 'x'
#define ACL_MODE_TRIGGER_CHR	't'

/* result codes for pg_aclcheck */
#define ACLCHECK_OK				  0
#define ACLCHECK_NO_PRIV		  1
#define ACLCHECK_NO_CLASS		  2
#define ACLCHECK_NOT_OWNER		  3

/* error messages (index by ACL_CHECK_* result code).  set in aclchk.c. */
extern char *aclcheck_error_strings[];

/*
 * routines used internally
 */
extern Acl *acldefault(const char *relname, AclId ownerid);
extern Acl *aclinsert3(const Acl *old_acl, const AclItem *mod_aip, unsigned modechg);

/*
 * routines used by the parser
 */
extern char *aclmakepriv(const char *old_privlist, char new_priv);
extern char *aclmakeuser(const char *user_type, const char *user);

/*
 * exported routines (from acl.c)
 */
extern Acl *makeacl(int n);
extern Datum aclitemin(PG_FUNCTION_ARGS);
extern Datum aclitemout(PG_FUNCTION_ARGS);
extern Datum aclinsert(PG_FUNCTION_ARGS);
extern Datum aclremove(PG_FUNCTION_ARGS);
extern Datum aclcontains(PG_FUNCTION_ARGS);
extern const char *aclparse(const char *s, AclItem *aip, unsigned *modechg);
extern char *makeAclString(const char *privileges, const char *grantee, char grant_or_revoke);

/*
 * prototypes for functions in aclchk.c
 */
extern void ExecuteGrantStmt(GrantStmt *stmt);
extern AclId get_grosysid(char *groname);
extern char *get_groname(AclId grosysid);

extern int32 pg_aclcheck(char *relname, Oid userid, AclMode mode);

extern bool pg_ownercheck(Oid userid, const char *name, int cacheid);
extern bool pg_oper_ownercheck(Oid userid, Oid oprid);
extern bool pg_func_ownercheck(Oid userid, char *funcname,
				   int nargs, Oid *arglist);
extern bool pg_aggr_ownercheck(Oid userid, char *aggname,
				   Oid basetypeID);

#endif   /* ACL_H */
