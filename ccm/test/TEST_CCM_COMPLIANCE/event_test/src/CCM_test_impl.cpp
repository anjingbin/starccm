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
#include <CCM_test_impl.h>
#include <fstream>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

Test::PhilosopherImpl::CCM_Philosopher_impl::CCM_Philosopher_impl()
{
}

Test::PhilosopherImpl::CCM_Philosopher_impl::~CCM_Philosopher_impl()
{
}

void
Test::PhilosopherImpl::CCM_Philosopher_impl::configuration_complete()
{
}

void
Test::PhilosopherImpl::CCM_Philosopher_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    #ifdef TAO
    ctx_ = Test::CCM_Philosopher_Context::_duplicate(dynamic_cast<Test::CCM_Philosopher_Context*>(ctx));
    #else
    ctx_ = Test::CCM_Philosopher_Context::_narrow(ctx);
    #endif
}

void
Test::PhilosopherImpl::CCM_Philosopher_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Test::PhilosopherImpl::CCM_Philosopher_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Test::PhilosopherImpl::CCM_Philosopher_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}


void
Test::PhilosopherImpl::CCM_Philosopher_impl::send_message(const char* s)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    ::Test::PhilosopherState_var state = new ::Test::PhilosopherState_impl(s);
	if(strcmp(s,"pub")==0)	
	{
	   printf("\n ִ�� publish\n");
	   ctx_ -> push_phi_pub_state(state.in());	
	}
	else
	{
	   printf("\n ִ��emit\n");
	   ctx_ -> push_phi_emit_state(state.in());
	}
}

Test::PhilosopherImpl::CCM_PhilosopherHome_impl::CCM_PhilosopherHome_impl()
{
}

Test::PhilosopherImpl::CCM_PhilosopherHome_impl::~CCM_PhilosopherHome_impl()
{
}

Components::EnterpriseComponent_ptr
Test::PhilosopherImpl::CCM_PhilosopherHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new Test::PhilosopherImpl::CCM_Philosopher_impl();
}

char*
Test::PhilosopherImpl::CCM_PhilosopherHome_impl::get_storage_home_id()
{
    return 0;
}

Test::ObserverImpl::CCM_Observer_impl::CCM_Observer_impl()
{
}

Test::ObserverImpl::CCM_Observer_impl::~CCM_Observer_impl()
{
}

void
Test::ObserverImpl::CCM_Observer_impl::configuration_complete()
{
}

void
Test::ObserverImpl::CCM_Observer_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    #ifdef TAO
    ctx_ = Test::CCM_Observer_Context::_duplicate(dynamic_cast<Test::CCM_Observer_Context*>(ctx));
    #else
    ctx_ = Test::CCM_Observer_Context::_narrow(ctx);
    #endif
}

void
Test::ObserverImpl::CCM_Observer_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Test::ObserverImpl::CCM_Observer_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Test::ObserverImpl::CCM_Observer_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Test::ObserverImpl::CCM_Observer_impl::push_philosopher_state(::Test::PhilosopherState* event)
    throw(CORBA::SystemException)
{
	//add
	printf("receive!  event->name=%s\n",event->name());	
	ofstream f("event_test.out",ios::app);
	if(strcmp(event->name(),"pub")==0)
	{
		f<<"the publish  is ok!"<<"\n";
		cout<<"the publish  is ok!"<<endl;
	}
	else
	{
	        f<<"the emit is ok!"<<"\n";
	        cout<<"the emit  is ok!"<<endl;
	}
	f.close();
}


Test::ObserverImpl::CCM_ObserverHome_impl::CCM_ObserverHome_impl()
{
}

Test::ObserverImpl::CCM_ObserverHome_impl::~CCM_ObserverHome_impl()
{
}

Components::EnterpriseComponent_ptr
Test::ObserverImpl::CCM_ObserverHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new Test::ObserverImpl::CCM_Observer_impl();
}

char*
Test::ObserverImpl::CCM_ObserverHome_impl::get_storage_home_id()
{
    return 0;
}
