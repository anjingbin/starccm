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

#ifndef __res_impl_h__
#define __res_impl_h__

#include <Resource_skel.h>

#include <CosTransactionsOTS_impl.h>
#include <OTSHeader.h>

#include <PostgreSQLResource_impl.h>

//#include "ifxtypes.h"
//#include "xa.h"

class ResourceFactory_impl : virtual public POA_ResourceFactory,virtual public PortableServer::RefCountServantBase
                             
{
	private:
	
    	// Used in create() function to get object reference.
	CORBA::ORB_var _orb_var;

	// Used to set transcation identifier counter
		
	int _global_id_counter;
	int _local_id_counter;

	// Added to get transcation global identifer
	int get_global_id(void);
 		
    public:

        ResourceFactory_impl(CORBA::ORB_var orb);
        
        virtual Resource_ptr create(CORBA::Short dbtype)
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED);
};

class Resource_impl : virtual public POA_Resource,
                      virtual public PG_Resource_impl
{
    private:
	
		CORBA::Short	_dbtype;
	
    public:

		//renyi 11 19 : changed
        //Resource_impl(PortableServer::POA_var poa, CORBA::Short dbtype, XID id);
        
        //Resource_impl(CosTransactions::Coordinator_ptr c, CORBA::Short dbtype, XID id);
        
		Resource_impl(PortableServer::POA_var poa, CORBA::Short dbtype);
        
        Resource_impl(CosTransactions::Coordinator_ptr c, CORBA::Short dbtype);
        
	virtual CosTransactions::Vote prepare()
		throw (CORBA::SystemException,
		CosTransactions::WRONG_TRANSACTION,
		CosTransactions::INVALID_TRANSACTION,
		CosTransactions::TRANSACTION_ROLLEDBACK,
		CosTransactions::TRANSACTION_REQUIRED,
		CosTransactions::HeuristicHazard,
		CosTransactions::HeuristicMixed);
        
        virtual void rollback()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard,
        	CosTransactions::HeuristicMixed,
        	CosTransactions::HeuristicCommit);
        
        virtual void commit()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard,
        	CosTransactions::HeuristicMixed,
        	CosTransactions::HeuristicRollback,
        	CosTransactions::NotPrepared);
        
        virtual void commit_one_phase()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard);
        
        virtual void forget()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED);
        
        virtual void release()
        	throw (CORBA::SystemException);
        
        virtual CORBA::Boolean BeginTrans()
        	throw (CORBA::SystemException);
        
        virtual void set_recoverycoordinator(CosTransactions::RecoveryCoordinator_ptr rc)
        	throw (CORBA::SystemException,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED);
        
        virtual void heuristic_decision()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED);
};

#endif
