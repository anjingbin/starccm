/*-------------------------------------------------------------------------
 *
 *   FILE
 *	pgtransdb.cpp
 *
 *   DESCRIPTION
 *      implementation of the PgTransaction class.
 *   PgConnection encapsulates a transaction querying to backend
 *
 * Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/interfaces/libpq__/pgtransdb.cc,v 1.1 2003/12/30 00:07:52 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */
 
#include "pgtransdb.h"

// ****************************************************************
//
// PgTransaction Implementation
//
// ****************************************************************
// Make a connection to the specified database with default environment
// See PQconnectdb() for conninfo usage. 
PgTransaction::PgTransaction(const char* conninfo)
   : PgDatabase(conninfo),
     pgCommitted(true)
{
	BeginTransaction();
}

// Destructor: End the transaction block
PgTransaction::~PgTransaction()
{
	if (!pgCommitted) Exec("ABORT");
}

// Begin the transaction block
ExecStatusType PgTransaction::BeginTransaction()
{
	pgCommitted = false;
	return Exec("BEGIN");
} // End BeginTransaction()

// Begin the transaction block
ExecStatusType PgTransaction::EndTransaction()
{
	pgCommitted = true;
	return Exec("END");
} // End EndTransaction()

