// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Kebo    mep@263.net
// Author: Huang Jie    huangjie@email.com
// Author: Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#ifndef __UserTransaction_impl_h__
#define __UserTransaction_impl_h__

#include <CORBA.h>
#include <UserTransaction.h>
#include <CosTransactions.h>
#include <ContainerBase.h>


#include <stack>

//
// IDL:omg.org/Components:1.0
//
namespace Components
{

//
// IDL:omg.org/Components/Transaction:1.0
//
namespace Transaction
{

//
// IDL:omg.org/Components/Transaction/UserTransaction:1.0
//
class UserTransaction_impl : virtual public Components::Transaction::UserTransaction , REF_COUNT_LOCAL_OBJECT
{
private:
//	UserTransaction_impl(const UserTransaction_impl&);
//	void operator=(const UserTransaction_impl&);

	::Container::ContainerBase_ptr container_;
	CosTransactions::Current_var txnCurrent_;
	CosTransactions::Control_var control_;
          
          
//	typedef stack<CosTransactions::Control_ptr> stack_control;
//	stack_control stack_control_;

public:
	UserTransaction_impl(::Container::ContainerBase_ptr container);
	~UserTransaction_impl();


    //
    // IDL:omg.org/Components/Transaction/UserTransaction/begin:1.0
    //
    virtual void begin()
        throw(Components::Transaction::NotSupported,
              Components::Transaction::SystemError,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Transaction/UserTransaction/commit:1.0
    //
    virtual void commit()
        throw(Components::Transaction::RollbackError,
              Components::Transaction::NoTransaction,
              Components::Transaction::HeuristicMixed,
              Components::Transaction::HeuristicRollback,
              Components::Transaction::Security,
              Components::Transaction::SystemError,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Transaction/UserTransaction/rollback:1.0
    //
    virtual void rollback()
        throw(Components::Transaction::NoTransaction,
              Components::Transaction::Security,
              Components::Transaction::SystemError,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Transaction/UserTransaction/set_rollback_only:1.0
    //
    virtual void set_rollback_only()
        throw(Components::Transaction::NoTransaction,
              Components::Transaction::SystemError,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Transaction/UserTransaction/get_status:1.0
    //
    virtual Components::Transaction::Status get_status()
        throw(Components::Transaction::SystemError,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Transaction/UserTransaction/set_timeout:1.0
    //
    virtual void set_timeout(CORBA::Long to);
//        throw(Components::Transaction::SystemError,
//              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Transaction/UserTransaction/suspend:1.0
    //
    virtual Components::Transaction::TranToken* suspend()
        throw(Components::Transaction::NoTransaction,
              Components::Transaction::SystemError,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Transaction/UserTransaction/resume:1.0
    //
    virtual void resume(const Components::Transaction::TranToken& txtoken)
        throw(Components::Transaction::InvalidToken,
              Components::Transaction::SystemError,
              CORBA::SystemException);
};

} // End of namespace Transaction

} // End of namespace Components

#endif
