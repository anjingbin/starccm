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

#include <CompositionManager.h>

using namespace Container;
using namespace std;

CompositionManager::CompositionManager()
{
	
}

CompositionManager::~CompositionManager()
{
	
}

PortableServer::Servant
CompositionManager::getServant()
{

	return servant_;
}

void
CompositionManager::setServant(PortableServer::Servant servant)
{
	
	servant_ = servant;
}

CORBA::Object_ptr
CompositionManager::getExecutor()
{
	
	return CORBA::Object::_duplicate(executor_);
}

void
CompositionManager::setExecutor(CORBA::Object_ptr executor)
{
	
	executor_ =
		CORBA::Object::_duplicate(executor);
}

//CosPersistenceState::StorageObject_ptr
//CompositionManager::getStorageHome()
//{
//	return CosPersistenceState::StorageObject::_duplicate(storage_);
//}

//void
//CompositionManager::setStorageHome(CosPersistenceState::StorageObject_ptr storageObject)
//{
//	storage_ =
//		CosPersistenceState::StorageObject::_duplicate(storageObject);
//}

CORBA::OctetSeq*
CompositionManager::getPid()
{
	if( pid_ == NULL )
	{
		return NULL;
	}
	else
	{
		return new CORBA::OctetSeq(pid_.in());
	}
}

void
CompositionManager::setPid(CORBA::OctetSeq* pid)
{
	
//	pid_ = OBJDUPLICATE(CORBA::OctetSeq*,pid);
	pid_ = new CORBA::OctetSeq(*pid);
}

CORBA::OctetSeq*
CompositionManager::getShortPid()
{
	if( shortPid_ == NULL )
	{
		return NULL;
	}
	else
	{
		return new CORBA::OctetSeq(shortPid_.in());
	}
}

void
CompositionManager::setShortPid(CORBA::OctetSeq* shortPid)
{
	shortPid_ = new CORBA::OctetSeq(*shortPid);
}

void
CompositionManager::setContext(Components::CCMContext_ptr context)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	context_ = Components::CCMContext::_duplicate(context);
}

Components::CCMContext_ptr
CompositionManager::getContext()
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	return Components::CCMContext::_duplicate(context_);
}

char*
CompositionManager::getUUID()
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	return CORBA::string_dup(uuid_);
}

void
CompositionManager::setUUID(char* uuid)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	uuid_ = CORBA::string_dup(uuid);
}

CompositionStatus
CompositionManager::getCompositionStatus()
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	return status_;
}

void
CompositionManager::setCompositionStatus(CompositionStatus status)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	status_ = status;
}

PortableServer::ObjectId*
CompositionManager::getObjectId()
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	if( objectId_ == NULL )
	{
		return NULL;
	}
	else
	{
		return new PortableServer::ObjectId(objectId_.in());
	}
}

void
CompositionManager::setObjectId(PortableServer::ObjectId* oid)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	objectId_ = new PortableServer::ObjectId(*oid);
}

PortableServer::Servant
CompositionManager::getSessionSynchronization()
{
    return sessionSynchronization_;
}

void
CompositionManager::setSessionSynchronization(PortableServer::Servant servant)
{
    sessionSynchronization_ = servant;
}

void
CompositionManager::passivate()
{
	
}

void
CompositionManager::activate()
{
	
}

