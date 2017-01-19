/*-------------------------------------------------------------------------
 *
 * pg_database.h
 *	  definition of the system "database" relation (pg_database)
 *	  along with the relation's initial contents.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: pg_database.h,v 1.2 2004/07/05 08:55:58 SuLiang Exp $
 *
 * NOTES
 *	  the genbki.sh script reads this file and generates .bki
 *	  information from the DATA() statements.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_DATABASE_H
#define PG_DATABASE_H

/* ----------------
 *		postgres.h contains the system type definintions and the
 *		CATALOG(), BOOTSTRAP and DATA() sugar words so this file
 *		can be read by both genbki.sh and the C compiler.
 * ----------------
 */

/* ----------------
 *		pg_database definition.  cpp turns this into
 *		typedef struct FormData_pg_database
 * ----------------
 */
CATALOG(pg_database) BOOTSTRAP
{
	NameData	datname;		/* database name */
	int4		datdba;			/* sysid of owner */
	int4		encoding;		/* multibyte encoding, if any */
	bool		datistemplate;	/* allowed as CREATE DATABASE template? */
	bool		datallowconn;	/* new connections allowed? */
	Oid			datlastsysoid;	/* highest OID to consider a system OID */
	TransactionId datvacuumxid; /* all XIDs before this are vacuumed */
	TransactionId datfrozenxid; /* all XIDs before this are frozen */
	text		datpath;		/* VARIABLE LENGTH FIELD */
} FormData_pg_database;

/* ----------------
 *		Form_pg_database corresponds to a pointer to a tuple with
 *		the format of pg_database relation.
 * ----------------
 */
typedef FormData_pg_database *Form_pg_database;

/* ----------------
 *		compiler constants for pg_database
 * ----------------
 */
#define Natts_pg_database				9
#define Anum_pg_database_datname		1
#define Anum_pg_database_datdba			2
#define Anum_pg_database_encoding		3
#define Anum_pg_database_datistemplate	4
#define Anum_pg_database_datallowconn	5
#define Anum_pg_database_datlastsysoid	6
#define Anum_pg_database_datvacuumxid	7
#define Anum_pg_database_datfrozenxid	8
#define Anum_pg_database_datpath		9

DATA(insert OID = 1 (  template1 PGUID ENCODING t t 0 0 0 "" ));
DESCR("Default template database");

#define TemplateDbOid			1

/* Just to mark OID as used for unused_oid script -:) */
#define DATAMARKOID(x)

DATAMARKOID(= 2)
#define RecoveryDb	2

#undef DATAMARKOID

#endif   /* PG_DATABASE_H */
