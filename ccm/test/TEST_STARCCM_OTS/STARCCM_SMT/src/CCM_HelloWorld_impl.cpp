// **********************************************************************
//
// Generated by the CIDL Translator
//
// Copyright (c) 2001-2004
// StarMiddleware Group
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Huang Jie        huangjie@email.com
// Author: Chang Junsheng   cjs7908@163.com
// Author: Zhen Xianrong
//
// **********************************************************************

#include <CORBA.h>
#include <CCM_Helloworld_impl.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

HelloWorld::PersonImpl::CCM_Person_impl::CCM_Person_impl()
{
}

HelloWorld::PersonImpl::CCM_Person_impl::~CCM_Person_impl()
{
}

void
HelloWorld::PersonImpl::CCM_Person_impl::configuration_complete()
{
}

void
HelloWorld::PersonImpl::CCM_Person_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    #ifdef TAO
    ctx_ = HelloWorld::CCM_Person_Context::_duplicate(dynamic_cast<HelloWorld::CCM_Person_Context*>(ctx));
    #else
    ctx_ = HelloWorld::CCM_Person_Context::_narrow(ctx);
    #endif
}

void
HelloWorld::PersonImpl::CCM_Person_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
HelloWorld::PersonImpl::CCM_Person_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
HelloWorld::PersonImpl::CCM_Person_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


char*
HelloWorld::PersonImpl::CCM_Person_impl::sayHello()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CORBA::Object_var tempObj = ctx_ -> resolve_object("TransactionCurrent");
    CosTransactions::Current_var current_ = CosTransactions::Current::_narrow(tempObj);
    
    int bankMoney = 0;
    
    //begin a trasaction.
    current_ -> begin();
    bankMoney += 100;  
	current_ -> commit(false); 
	
    char* _r = CORBA::string_dup("----------------sayHello()-------------------\n");
    return _r;
}

char*
HelloWorld::PersonImpl::CCM_Person_impl::name()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    return name_._retn();
}

void
HelloWorld::PersonImpl::CCM_Person_impl::name(const char* a)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    name_ = CORBA::string_dup(a);
}

HelloWorld::PersonImpl::CCM_PersonHome_impl::CCM_PersonHome_impl()
{
}

HelloWorld::PersonImpl::CCM_PersonHome_impl::~CCM_PersonHome_impl()
{
}

Components::EnterpriseComponent_ptr
HelloWorld::PersonImpl::CCM_PersonHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new HelloWorld::PersonImpl::CCM_Person_impl();
}

char*
HelloWorld::PersonImpl::CCM_PersonHome_impl::get_storage_home_id()
{
    return 0;
}

