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

#include <CORBA.h>
#include <HomeExecutorBase.h>

using namespace Executor;

HomeExecutorBase::HomeExecutorBase()
{
}

HomeExecutorBase::HomeExecutorBase(PortableServer::POA_ptr poa)
{
	poa_ = PortableServer::POA::_duplicate(poa);
}

HomeExecutorBase::~HomeExecutorBase()
{
}

CORBA::IRObject_ptr 
HomeExecutorBase::get_component_def()
	throw (CORBA::SystemException)
{
	return CORBA::IRObject::_nil();
}

CORBA::IRObject_ptr 
HomeExecutorBase::get_home_def()
	throw (CORBA::SystemException)
{
	return CORBA::IRObject::_nil();
}

void 
HomeExecutorBase::remove_component(Components::CCMObject_ptr comp)
	throw (CORBA::SystemException)
{
	comp -> remove();
}

void
HomeExecutorBase::setPOA(PortableServer::POA_ptr poa)
{
	poa_ = PortableServer::POA::_duplicate(poa);
}

