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
#ifndef __ots_impl_h__
#define __ots_impl_h__

#include <CosTransactionsOTS_skel.h>

#include <CosTransactionsUser_skel.h>//renyi 12 26
#include <CosTransactionsUser.h>//renyi 12 26
#include <OTSHeader.h>
#include <signal.h>
#include <list>
//
// Set of synchronization objects
//
//using namespace std;

typedef std::list<CosTransactions::Synchronization_var> SynchronizationList;

//
// Used for management of Synchronization objects and running the
// before and asfter loops.
//
class SynchronizationManager
{
    //
    // The registered synchronization objects
    //
    SynchronizationList _syncs;

public:
    
    SynchronizationManager();
    ~SynchronizationManager();

    //
    // Call before and after completion
    //
    void beforeCompletion()
	throw();

    void afterCompletion(CosTransactions::Status)
	throw();

    //
    // Sync object management
    //
    void registerSync(CosTransactions::Synchronization_ptr)
	throw();

    //
    // Clear all sync objects
    //
    void clear()
	throw();

};



//
// Module declare ::CosTransactions
//
namespace CosTransactions
{

class TransactionFactory_impl : virtual public POA_CosTransactions::TransactionFactory,virtual public PortableServer::RefCountServantBase
{
    private:
    
    	// Used in create() function to get object reference.
		CORBA::ORB_var _orb_var;
    	
    	// Variable for sigaction() call.
//        struct sigaction _sa;
    	
    public:

		TransactionFactory_impl(CORBA::ORB_var orb);
        
        virtual Control_ptr create(CORBA::ULong time_out)
                throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);
};


class Control_impl : virtual public POA_CosTransactions::Control,virtual public PortableServer::RefCountServantBase
{
    private:
		
	// Used for recovery.
	CORBA::ORB_var  _orb_var;

	PortableServer::POA_var _poa_var;
		
	Terminator_var _Term_var;
	Coordinator_var _Coord_var;

    public:

		Control_impl(CORBA::ORB_var orb, PortableServer::POA_var poa);
        
        virtual Terminator_ptr get_terminator()
        throw(CosTransactions::Unavailable,
              CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);
        
        virtual Coordinator_ptr get_coordinator()
        throw(CosTransactions::Unavailable,
              CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);

        virtual void release ()
        throw(CORBA::SystemException);
};

class Terminator_impl : virtual public POA_CosTransactions::Terminator,virtual public PortableServer::RefCountServantBase
{
	private:
		Coordinator_var _Coord_var;

    public:

		Terminator_impl();
        
		Terminator_impl(CosTransactions::Coordinator_ptr c);
        
        virtual void commit(CORBA::Boolean report_heuristics)
        throw(CosTransactions::HeuristicMixed,
              CosTransactions::HeuristicHazard,
              CosTransactions::HeuristicRollback,
              CosTransactions::HeuristicCommit,
              CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);
        
        virtual void rollback()
        throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);

        virtual void release ()
        throw(CORBA::SystemException);
};

class Coordinator_impl : virtual public POA_CosTransactions::Coordinator,virtual public PortableServer::RefCountServantBase
{
	private:  
		Status	_TransStatus;
		CosTransactions::Coordinator::RCVs_var	_RCV_list_var;

		//
	    // The set of register synchronization objects
	    //
	    SynchronizationManager* _sm;//renyi 12 25

		
		// Used for recovery.
		CORBA::ORB_var	_orb_var;

    public:

		Coordinator_impl(CORBA::ORB_var  orb);
        
        virtual Status get_status()
        throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);
        
        virtual CORBA::Boolean is_same_transaction(Coordinator_ptr tc)
        throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);
        
        virtual RecoveryCoordinator_ptr register_resource(Resource_ptr r)
        throw(CosTransactions::Inactive,
              CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);

        virtual void register_synchronization(Synchronization_ptr sync)
        throw(CosTransactions::Inactive,
              CosTransactions::SynchronizationUnavailable,
              CORBA::SystemException);//renyi 12 25
        
        virtual void rollback_only()
        throw(CosTransactions::Inactive,
              CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);
        
        virtual void commit(CORBA::Boolean report_heuristics)
        throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);
        
        virtual void rollback()
        throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);
        
        virtual void change_resourcevote(Resource_ptr r,
			ResourceVote rv)
			throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);

        virtual void release ()
        throw(CORBA::SystemException);
};

class RecoveryCoordinator_impl : virtual public POA_CosTransactions::RecoveryCoordinator,virtual public PortableServer::RefCountServantBase
{
	private:
		Coordinator_var _Coord_var;

    public:

		RecoveryCoordinator_impl();
        
		RecoveryCoordinator_impl(CosTransactions::Coordinator_ptr c);

        virtual Status replay_completion(Resource_ptr r)
        throw(CosTransactions::NotPrepared,
              CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException);

        virtual void release ()
        throw(CORBA::SystemException);
};

class TransactionalObject_impl : virtual public POA_CosTransactions::TransactionalObject,virtual public PortableServer::RefCountServantBase
{
    public:

		TransactionalObject_impl();
};

//renyi 12 26
//
// Synchronization object.
//
class Synchronization_impl : virtual public POA_CosTransactions::Synchronization,virtual public PortableServer::RefCountServantBase
{
    //
    // The POA
    //
    PortableServer::POA_var _poa;

    //
    // The POA Current
    //
    PortableServer::Current_var _poaCurrent;

public:

    Synchronization_impl(CORBA::ORB_ptr,
				   PortableServer::POA_ptr);

    ~Synchronization_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual void
    before_completion()
    throw(CORBA::SystemException);

    virtual void
    after_completion(CosTransactions::Status s)
    throw(CORBA::SystemException);

    virtual PortableServer::POA_ptr
    _default_POA();
};


} // End of namespace CosTransactions


#endif
