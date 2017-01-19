// **********************************************************************
//
// Copyright (c) 2002-2003 
// StarMiddleware.net	
// www.StarMiddleware.net
// 
// All Rights Reserved
//
// Author: Ren Yi	starots@sohu.com	
// **********************************************************************

// Version: 1.0.0

#ifndef __pg_res_impl_h__
#define __pg_res_impl_h__

#include <PostgreSQLResource_skel.h>
#include <OTSPostgreSQL.h>

#include <CosTransactionsOTS_impl.h>
#include <libpq-fe.h>

const Oid DisabledTransactionId = (Oid)(-1);


class PG_Resource_impl : virtual public POA_PG_Resource,virtual public PortableServer::RefCountServantBase
{
    private:
    	PortableServer::POA_var _poa_var;
        // Variables for OTS.
        CosTransactions::RecoveryCoordinator_var _RC_var;
        CosTransactions::Coordinator_var   _Coord_var;
        CosTransactions::Status _ResourceStatus;	
        CosTransactions::Vote _vote;
        
        // Variables for GalaxyDB connection.
        PGconn*	_PGconn;
        PGresult*	_PGres;
        Oid	_TransactionId;	// Current transaction's ID.
        				// Hold for recovery	
        
    	
    public:

        PG_Resource_impl(PortableServer::POA_var poa);
        
        PG_Resource_impl(CosTransactions::Coordinator_ptr c);
        
        virtual CosTransactions::Vote pg_prepare()
        	 throw (CORBA::SystemException, 
        	 CosTransactions::WRONG_TRANSACTION,
        	 CosTransactions::INVALID_TRANSACTION,
        	 CosTransactions::TRANSACTION_ROLLEDBACK,
        	 CosTransactions::TRANSACTION_REQUIRED,
        	 CosTransactions::HeuristicHazard,
        	 CosTransactions::HeuristicMixed);
        
        virtual void pg_rollback()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard,
        	CosTransactions::HeuristicMixed,
        	CosTransactions::HeuristicCommit);
        
        virtual void pg_commit()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard,
        	CosTransactions::HeuristicMixed,
        	CosTransactions::HeuristicRollback,
        	CosTransactions::NotPrepared);
        
        virtual void pg_commit_one_phase()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard);
        
        virtual void pg_forget()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED);
        
        virtual void pg_release()
        	throw (CORBA::SystemException);

        // Added to begin transaction in Coordinator::register_resource.
        virtual CORBA::Boolean pg_BeginTrans()
        	throw (CORBA::SystemException);
        
        virtual void pg_set_recoverycoordinator(CosTransactions::RecoveryCoordinator_ptr rc)
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED);
        
        virtual void pg_heuristic_decision()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED);
        
        //
        // Functions for "GalaxyDB": 32
        //
        virtual void pg_PQsetdb(const char* pghost,
                                const char* pgport,
                                const char* pgoptions,
                                const char* pgtty,
                                const char* dbName)
		throw(PG_Resource::PG_CONNECTTION_FAILED);

        
        virtual char* pg_PQdb()
        	throw (CORBA::SystemException);
        
        virtual char* pg_PQhost()
        	throw (CORBA::SystemException);
        
        virtual char* pg_PQoptions() 
        	throw (CORBA::SystemException);
        
        virtual char* pg_PQport() 
        	throw (CORBA::SystemException);
        
        virtual char* pg_PQtty() 
        	throw (CORBA::SystemException);
        
        virtual pg_ConnStatusType pg_PQstatus()
        	throw (CORBA::SystemException);
        
        virtual char* pg_PQerrorMessage()
        	throw (CORBA::SystemException);
        
        virtual void pg_PQfinish()
        	throw (CORBA::SystemException);
        
        virtual void pg_PQreset()
        	throw (CORBA::SystemException);
        
        virtual void pg_PQuntrace()
        	throw (CORBA::SystemException);
        
        virtual pg_ExecStatusType pg_PQresultStatus()
        	throw (CORBA::SystemException);
        
        virtual void pg_PQexec(const char* query)
        	throw (CORBA::SystemException);
        
        virtual CORBA::Long pg_PQntuples()
        	throw (CORBA::SystemException);
        
        virtual CORBA::Long pg_PQnfields()
        	throw (CORBA::SystemException);
        
        virtual char* pg_PQfname(CORBA::Long field_index)
        	throw (CORBA::SystemException);
        
        virtual CORBA::Long pg_PQfnumber(const char* field_name)
        	throw (CORBA::SystemException);
        
        virtual CORBA::Long pg_PQftype(CORBA::Long field_num)
        	throw (CORBA::SystemException);
        
        virtual CORBA::Long pg_PQfsize(CORBA::Long field_index)
        	throw (CORBA::SystemException);
        
        virtual char* pg_PQgetvalue(CORBA::Long tup_num,
                                    CORBA::Long field_num)
                throw (CORBA::SystemException);
        
        virtual CORBA::Long pg_PQgetlength(CORBA::Long tup_num,
                                          CORBA::Long field_num)
                throw (CORBA::SystemException);
        
        virtual char* pg_PQcmdStatus()
        	throw (CORBA::SystemException);
        
        virtual char* pg_PQoidStatus()
        	throw (CORBA::SystemException);
        
        virtual void pg_PQclear()
        	throw (CORBA::SystemException);
        
        virtual pg_PGnotify* pg_PQnotifies()
        	throw (CORBA::SystemException);
        
        virtual CORBA::Long pg_PQgetline(const char* str,
                                        CORBA::Long length)
                throw (CORBA::SystemException);
        
        virtual void pg_PQputline(const char* str)
        	throw (CORBA::SystemException);
        
        virtual CORBA::Long pg_PQendcopy()
        	throw (CORBA::SystemException);
        
        virtual char* pg_fe_getauthname(const char* errorMessage)
        	throw (CORBA::SystemException);
        
        virtual void pg_fe_setauthsvc(const char* name,
                                      const char* errorMessage)
                throw (CORBA::SystemException);
};

#endif
