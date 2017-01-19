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

#include <HomeComposition.h>

using namespace Container;
using namespace std;

HomeComposition::HomeComposition()
{
	
}

HomeComposition::~HomeComposition()
{
	
}

PortableServer::Servant
HomeComposition::getHomeServant()
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	return getServant();
}

void
HomeComposition::registerHomeServant(const PortableServer::ObjectId& oid,PortableServer::Servant servant)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	PortableServer::ObjectId* objectId = new PortableServer::ObjectId(oid);
	setObjectId(objectId);
	setServant(servant);
	
	delete objectId;
}

Components::HomeExecutorBase_ptr
HomeComposition::getHomeExecutor()
{
//	Guard<FastRecursiveMutex> guard(executorLock_);
	
	Components::HomeExecutorBase_var homeExecutor
		= Components::HomeExecutorBase::_narrow(getExecutor());
	
	return Components::HomeExecutorBase::_duplicate(homeExecutor);
}

void
HomeComposition::registerHomeExecutor(Components::HomeExecutorBase_ptr homeExecutor)
{
//	Guard<FastRecursiveMutex> guard(executorLock_);
	
	CORBA::Object_var obj
		= Components::HomeExecutorBase::_duplicate(homeExecutor);

	setExecutor(obj.in());

}

int
HomeComposition::compositionType()
{
	return HOME;
}

char*
HomeComposition::getRepId()
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	return CORBA::string_dup(repId_);
}

void
HomeComposition::setRepId(const char* repId)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	repId_ = CORBA::string_dup(repId);
}
