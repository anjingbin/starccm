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

#include <SynchronizationManager.h>

using namespace Container;

SynchronizationManager::SynchronizationManager(Components::SessionSynchronization_ptr component)
{
	component_ = Components::SessionSynchronization::_duplicate(component);
}

SynchronizationManager::~SynchronizationManager()
{
	
}

//void
//SynchronizationManager::after_begin(const PortableServer::ObjectId& oid)
//{
//	TRACE0(LEVEL5,"After transaction begin,sychronization component\n");
//}

void
SynchronizationManager::before_completion()
	throw(CORBA::SystemException)
{
	TRACE0(LEVEL5,"Before transaction completion,sychronization component\n");
	if( !CORBA::is_nil(component_) )
	{
		component_ -> before_completion();
	}
}

void
SynchronizationManager::after_completion(CosTransactions::Status s)
	throw(CORBA::SystemException)
{
	TRACE0(LEVEL5,"After transaction completion,sychronization component\n");
	if( !CORBA::is_nil(component_) )
	{
		if( s == CosTransactions::StatusCommitted )
		{
			component_ -> after_completion(true);
		}
		else if( (s == CosTransactions::StatusMarkedRollback) || 
				(s == CosTransactions::StatusRolledBack) ||
				(s = CosTransactions::StatusRollingBack) )
		{
			component_ -> after_completion(false);
		}
		else
		{
			//throw new CORBA::SystemException;
			TRACE1(LEVEL3,"Transaction status is not current : %d\n",s);
		}
	}
}

