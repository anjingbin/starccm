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
#include <CCM_InterestComputer_impl.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

Bank::InterestComputerComposition::CCM_InterestComputer_impl::CCM_InterestComputer_impl()
{
}

Bank::InterestComputerComposition::CCM_InterestComputer_impl::~CCM_InterestComputer_impl()
{
}

void
Bank::InterestComputerComposition::CCM_InterestComputer_impl::configuration_complete()
	throw (Components::InvalidConfiguration, CORBA::SystemException)
{
}

void
Bank::InterestComputerComposition::CCM_InterestComputer_impl::enter_dyn_deploy()
	throw(CORBA::SystemException)
{
}

void
Bank::InterestComputerComposition::CCM_InterestComputer_impl::leave_dyn_deploy()
	throw(CORBA::SystemException)
{
}

void
Bank::InterestComputerComposition::CCM_InterestComputer_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    #ifdef TAO
    ctx_ = Bank::CCM_InterestComputer_Context::_duplicate(dynamic_cast<Bank::CCM_InterestComputer_Context*>(ctx));
    #else
    ctx_ = Bank::CCM_InterestComputer_Context::_narrow(ctx);
    #endif
}

void
Bank::InterestComputerComposition::CCM_InterestComputer_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Bank::InterestComputerComposition::CCM_InterestComputer_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Bank::InterestComputerComposition::CCM_InterestComputer_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


CORBA::ULong
Bank::InterestComputerComposition::CCM_InterestComputer_impl::getInterest(const char* id,
                                                                          CORBA::ULong amount)
    throw(CORBA::SystemException)
{
    CORBA::ULong _r = 0;
    _r = (CORBA::ULong)(amount * 1.2);
    return _r;
}

Bank::InterestComputerComposition::CCM_InterestComputerHome_impl::CCM_InterestComputerHome_impl()
{
}

Bank::InterestComputerComposition::CCM_InterestComputerHome_impl::~CCM_InterestComputerHome_impl()
{
}

Components::EnterpriseComponent_ptr
Bank::InterestComputerComposition::CCM_InterestComputerHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new Bank::InterestComputerComposition::CCM_InterestComputer_impl();
}

char*
Bank::InterestComputerComposition::CCM_InterestComputerHome_impl::get_storage_home_id()
{
    return 0;
}

