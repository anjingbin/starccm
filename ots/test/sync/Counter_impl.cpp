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

#include <OB/CORBA.h>
#include <CosTransactionsUser_skel.h>
#include <iostream.h>

#include <Counter_impl.h>

//#include <Resource_skel.h>

//
// The counter resource implementation. Note that this resource does
// not implement the semantics of a recoverable resource. Most
// notably:
//
// - The resource is not a persistent object-reference.
//
// - The resource does not log the prepare VoteCommit decision to
// stable store.
//
// - The resource does not implement recovery.
//

class Sync_sun: public POA_CosTransactions::Synchronization
{
	
public:
	
	Sync_sun(){}

	~Sync_sun(){}

	void 
    before_completion()
    {
    	printf("	[before_completion is ok!]\n");
    }

    void
    after_completion(CosTransactions::Status status)
    {
	  	printf("	[after_completion is ok!]\n");
    }

};

// ----------------------------------------------------------------------
// Counter_impl private member implementation
// ----------------------------------------------------------------------

void
Counter_impl::join()
{
	#ifdef _DEBUG_
		cout << "[Counter_impl::join() begin!]" << endl;
	#endif

    try
    {
        CosTransactions::Control_var control = 
	    current_ -> get_control();

	if(CORBA::is_nil(control))
	    throw CORBA::TRANSACTION_REQUIRED();
	printf("	[get control object from current object is ok!]\n");

	CosTransactions::Coordinator_var coordinator;
	try
	{
	    coordinator = control -> get_coordinator();
	}
	catch(const CORBA::OBJECT_NOT_EXIST&)
	{
	    throw CORBA::TRANSACTION_REQUIRED();
	}
	printf("	[get coordinator object from control object is ok!]\n");

	/*
	try
	{

	    //
	    // Is this resource is already part of a transaction?
	    //
	    while(!CORBA::is_nil(coordinator_))
	    {
		//
		// If the transaction isn't the same as the transaction
		// that the resource is already involved with then wait
		// for that transaction to terminate.
		//
		if(coordinator -> is_same_transaction(coordinator_))
		    break;
	    }
	}
	catch(const CORBA::OBJECT_NOT_EXIST&)
	{
	    //
	    // If the coordinator goes away then the transaction aborted.
	    //
	    endTransaction();
	}
	*/

	//
	// Is this resource joining the transaction for the first time?
	//
	if(CORBA::is_nil(coordinator_))
	{
	    coordinator_ = coordinator;

		Sync_sun * sync_sun=new Sync_sun();
		CosTransactions::Synchronization_var synchr;
		synchr =sync_sun -> _this();
	    coordinator_ -> register_synchronization(synchr);
	    cout<<"	[register Synchronization to coordinator is ok!]"<<endl;
	}
    }
    catch(const CosTransactions::Inactive&)
    {
        // 
        // The transaction is not active, marked for rollback,
        // already preparing.
        // 
        throw CORBA::INVALID_TRANSACTION();
    }

    #ifdef _DEBUG_
		cout << "[Counter_impl::join() end!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// Counter_impl constructor and destructor
// ----------------------------------------------------------------------

Counter_impl::Counter_impl(CORBA::ORB_ptr orb,
			   PortableServer::POA_ptr poa, 
                           CosTransactions::Current_ptr current)
    : orb_(CORBA::ORB::_duplicate(orb)),
      poa_(PortableServer::POA::_duplicate(poa)), 
      count_(0),
      current_(CosTransactions::Current::_duplicate(current))
{
    resourcePOA_ = PortableServer::POA::_narrow(
	CORBA::Object_var(orb_ -> resolve_initial_references("RootPOA")));
}

Counter_impl::~Counter_impl()
{
}

// ----------------------------------------------------------------------
// Counter_impl public member implementation
// ----------------------------------------------------------------------

//
// Counter interface methods
//
CORBA::Long
Counter_impl::count()
    throw(CORBA::SystemException)
{
    join();
	return count_;
}

void
Counter_impl::increment()
    throw(CORBA::SystemException)
{
    join();
}

void
Counter_impl::decrement()
    throw(CORBA::SystemException)
{
    join();
}


PortableServer::POA_ptr
Counter_impl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}

void
Counter_impl::commit(CORBA::Long count)
{
    count_ = count;
    endTransaction();
}

void
Counter_impl::endTransaction()
{
    //
    // Reset the coordinator to allow the Counter to join a new
    // transaction
    //
    coordinator_ = CosTransactions::Coordinator::_nil();
}
