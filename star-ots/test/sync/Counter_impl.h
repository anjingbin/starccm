// **********************************************************************
//
// Copyright (c) 2000
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#ifndef COUNTER_IMPL_H
#define COUNTER_IMPL_H

#include <CosTransactions.h>

#include <Counter_skel.h>

//
// The counter implementation
//
class Counter_impl : public POA_Counter,
		     public PortableServer::RefCountServantBase
			 //,
		     //public MTLMonitor
{
    //
    // The ORB
    //
    CORBA::ORB_var orb_;

    //
    // The servants POA
    //
    PortableServer::POA_var poa_;

    //
    // The Resource POA
    //
    PortableServer::POA_var resourcePOA_;

    //
    // The last known committed count
    //
    CORBA::Long count_;

    //
    // Cached CosTransactions::Current
    //
    CosTransactions::Current_var current_;

    //
    // If the resource is already part of a transaction then this is
    // the transaction coordinator.
    //
    CosTransactions::Coordinator_var coordinator_;

    //
    // Join this transaction if necessary
    //
    void join();

public:

    Counter_impl(CORBA::ORB_ptr,
		 PortableServer::POA_ptr,
		 CosTransactions::Current_ptr);
    virtual ~Counter_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual CORBA::Long count()
	throw(CORBA::SystemException);

    virtual void increment()
	throw(CORBA::SystemException);

    virtual void decrement()
	throw(CORBA::SystemException);

    virtual PortableServer::POA_ptr _default_POA();

    //
    // Commit the transaction
    //
    void commit(CORBA::Long);

    //
    // End the transaction
    //
    void endTransaction();
};

#endif
