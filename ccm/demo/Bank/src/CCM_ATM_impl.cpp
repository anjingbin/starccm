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
#include <CCM_ATM_impl.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

Bank::ATMComposition::CCM_ATM_impl::CCM_ATM_impl()
{
}

Bank::ATMComposition::CCM_ATM_impl::~CCM_ATM_impl()
{
}

void
Bank::ATMComposition::CCM_ATM_impl::configuration_complete()
	throw (Components::InvalidConfiguration, CORBA::SystemException)
{
}

void
Bank::ATMComposition::CCM_ATM_impl::enter_dyn_deploy()
	throw(CORBA::SystemException)
{
}

void
Bank::ATMComposition::CCM_ATM_impl::leave_dyn_deploy()
	throw(CORBA::SystemException)
{
}

void
Bank::ATMComposition::CCM_ATM_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    #ifdef TAO
    ctx_ = Bank::CCM_ATM_Context::_duplicate(dynamic_cast<Bank::CCM_ATM_Context*>(ctx));
    #else
    ctx_ = Bank::CCM_ATM_Context::_narrow(ctx);
    #endif
}

void
Bank::ATMComposition::CCM_ATM_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Bank::ATMComposition::CCM_ATM_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Bank::ATMComposition::CCM_ATM_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


CORBA::ULong
Bank::ATMComposition::CCM_ATM_impl::getBalance(const char* id)
    throw(CORBA::SystemException)
{
    CORBA::ULong _r = 0;
    Bank::QueryOp_var q = ctx_ -> get_connection_query();
    _r = q -> getBalance(id);
    return _r;
}

void
Bank::ATMComposition::CCM_ATM_impl::deposit(const char* id,
                                            CORBA::ULong amount)
    throw(CORBA::SystemException)
{
    Bank::DepositOp_var d = ctx_ -> get_connection_deposit();
    d -> deposit(id, amount);
}

CORBA::ULong
Bank::ATMComposition::CCM_ATM_impl::withdraw(const char* id,
                                             CORBA::ULong amount)
    throw(CORBA::SystemException)
{
    CORBA::ULong _r = 0;
    Bank::WithdrawOp_var w = ctx_ -> get_connection_withdraw();
    _r = w -> withdraw(id, amount);
    return _r;
}

void
Bank::ATMComposition::CCM_ATM_impl::transfer(const char* fromId,
                                             const char* toId,
                                             CORBA::ULong amount)
    throw(CORBA::SystemException)
{
    Bank::WithdrawOp_var w = ctx_ -> get_connection_withdraw();
    Bank::DepositOp_var d = ctx_ -> get_connection_deposit();
    
    w -> withdraw(fromId, amount);
    d -> deposit(toId, amount);
}

void
Bank::ATMComposition::CCM_ATM_impl::changePassword(const char* id,
                                                   const char* newPassword)
    throw(CORBA::SystemException)
{
    Bank::AuthenticationOp_var a = ctx_ -> get_connection_authenticator();
    a -> changePassword(id, newPassword);
}

Bank::ATMComposition::CCM_ATMHome_impl::CCM_ATMHome_impl()
{
}

Bank::ATMComposition::CCM_ATMHome_impl::~CCM_ATMHome_impl()
{
}

Components::EnterpriseComponent_ptr
Bank::ATMComposition::CCM_ATMHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new Bank::ATMComposition::CCM_ATM_impl();
}

char*
Bank::ATMComposition::CCM_ATMHome_impl::get_storage_home_id()
{
    return 0;
}

