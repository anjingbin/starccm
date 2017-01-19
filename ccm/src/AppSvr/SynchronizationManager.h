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

#ifndef __SynchronizationManager_h__
#define __SynchronizationManager_h__

#include <CORBA.h>
#include <EnterpriseComponent.h>
#include <CosTransactions.h>

namespace Container
{

class SynchronizationManager //: virtual public POA_CosTransactions::Synchronization
{
protected:
	Components::SessionSynchronization_var component_;
	
public:
	SynchronizationManager(Components::SessionSynchronization_ptr component);
	virtual ~SynchronizationManager();
	
//	void after_begin(const PortableServer::ObjectId& oid);
	virtual void before_completion()
		throw(CORBA::SystemException);
	virtual void after_completion(CosTransactions::Status s)
		throw(CORBA::SystemException);
	
};

}

#endif

