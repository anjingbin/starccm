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

#ifndef OB_OTS_CURRENT_IMPL_H
#define OB_OTS_CURRENT_IMPL_H

#include <OB/PICurrent_fwd.h>
#include "CosTransactionsOTS.h"
#include "CosTransactionsCurrent.h"


namespace CosTransactions//renyi 6 6:STARCosTransactions--->CosTransactions
{

class Current_impl : public Current,
	              CORBA::LocalObject //renyi 12 2 : added
{
    // 
    // The PICurrent reference
    // 
    PortableInterceptor::Current_var piCurrent_;

    // 
    // The Portable Interceptor slot used by the OTS service.
    // 
    CORBA::ULong slotId_;

    // 
    // Encapsulation used to allow service to access portable interceptor
    // slots for Current_impl
    // 
    CORBA::ULong defaultTimeout_;
    
    // 
    // The TransactionFactory
    // 
    CosTransactions::TransactionFactory_var txnFactory_;

	CosTransactions::Control_var control_;

	//
	//The transaction Control
	//
    //CosTransactions::Control_var control_; //renyi 6 6:added

	CosTransactions::Coordinator_ptr get_Coordinator();//renyi 6 8:added
	CosTransactions::Terminator_ptr get_Terminator(); //renyi 6 8:added
	
public:

    Current_impl(PortableInterceptor::Current_ptr,
                 CORBA::ULong,
                 CORBA::ULong);

    virtual ~Current_impl();

    // 
    // Set the TransactionFactory to use for begin() requests.
    //
	//renyi 6 6:added
    void setTransactionFactory(CosTransactions::TransactionFactory_ptr);//renyi 6 9
    //
    // Standard IDL to C++ Mapping
    //

    // 
    // Begin() a new transaction. If there is currently an implicit
    // transaction, the new transaction is a subtransaction.
    // 
    void begin()
        throw(CosTransactions::SubtransactionsUnavailable, //renyi 6 6:???????
               CORBA::SystemException);
    
    // 
    // Commit the current transaction.
    // 
    void commit(CORBA::Boolean report_heuristics)
         throw(CosTransactions::NoTransaction,
	       CosTransactions::HeuristicMixed,
	       CosTransactions::HeuristicHazard,
	       CORBA::SystemException);

    // 
    // Rollback the current transaction. 
    // 
    void rollback()
        throw(CosTransactions::NoTransaction,
	      CORBA::SystemException);

    // 
    // Mark the current transaction for rollback only.
    // 
    void rollback_only()
        throw(CosTransactions::NoTransaction,
	      CORBA::SystemException);
    
    // 
    // Get the status of the current transaction.
    // 
    CosTransactions::Status get_status()
        throw(CORBA::SystemException);

    // 
    // Get the name of transaction. This is for
    // debug purposes only.
    // 
    char* get_transaction_name()
        throw(CORBA::SystemException);

    // 
    // Set the timeout value used when beginning
    // new transactions.
    // 
    void set_timeout(CORBA::ULong seconds)
        throw(CORBA::SystemException);

    // 
    // Get the timeout value used when beginning
    // new transactions.
    // 
    CORBA::ULong get_timeout()
        throw(CORBA::SystemException);

    // 
    // Get the control ptr associated with the 
    // current transaction.
    // 
    CosTransactions::Control_ptr get_control()
        throw(CORBA::SystemException);
    
    // 
    // Suspend the current transaction.
    // 
    CosTransactions::Control_ptr suspend()
        throw(CORBA::SystemException);

    // 
    // Resume a transaction.
    // 
    void resume(CosTransactions::Control_ptr which)
        throw(CosTransactions::InvalidControl,
               CORBA::SystemException);

    //
    // STARCosTransactions::Current implementation.
    //
    void begin_named(const char*)
        throw(CosTransactions::SubtransactionsUnavailable,
               CORBA::SystemException);
    

    //CosTransactions::Control_ptr getPropagationContext();
    void setPropagationContext(const CORBA::Any OTSPropogationContext);

};

} // end namespace STARCosTransactions

#endif
