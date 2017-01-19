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

#ifndef __CCMContext_impl_h__
#define __CCMContext_impl_h__

#include <CORBA.h>

#include <UserTransaction_impl.h>
#include <EnterpriseComponent.h>
#include <Context2_impl.h>


namespace Container
{

class CCMContext_impl : virtual public ::Components::CCMContext,
                        virtual public ::Container::Context2_impl
{

	
public:
	CCMContext_impl();
	virtual ~CCMContext_impl();
	

	//
    // IDL:omg.org/Components/CCMContext/get_caller_principal:1.0
    //
    virtual ::Components::Principal_ptr get_caller_principal();

    //
    // IDL:omg.org/Components/CCMContext/get_CCM_home:1.0
    //
    virtual ::Components::CCMHome_ptr get_CCM_home();

    //
    // IDL:omg.org/Components/CCMContext/get_rollback_only:1.0
    //
    virtual CORBA::Boolean get_rollback_only();

    //
    // IDL:omg.org/Components/CCMContext/get_user_transaction:1.0
    //
    virtual ::Components::Transaction::UserTransaction_ptr get_user_transaction();

    //
    // IDL:omg.org/Components/CCMContext/is_caller_in_role:1.0
    //
    virtual CORBA::Boolean is_caller_in_role(const char* role);

    //
    // IDL:omg.org/Components/CCMContext/set_rollback_only:1.0
    //
    virtual void set_rollback_only();
};

}
#endif

