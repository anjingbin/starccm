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

#ifndef __HomeExecutorBase_h__
#define __HomeExecutorBase_h__

#include <CCMHome_skel.h>
#include <CCMObject_skel.h>
#include <EnterpriseComponent.h>

namespace Executor
{

class HomeExecutorBase : virtual public POA_Components::CCMHome,
			virtual public Components::HomeExecutorBase
			, LOCAL_OBJECT_BASE
{
protected:
	PortableServer::POA_var poa_;

	virtual CORBA::IRObject_ptr get_component_def()throw (CORBA::SystemException);
	virtual CORBA::IRObject_ptr get_home_def()throw (CORBA::SystemException);
	virtual void remove_component(Components::CCMObject_ptr comp)throw (CORBA::SystemException);
public:
	HomeExecutorBase();
	HomeExecutorBase(PortableServer::POA_ptr poa);
	virtual ~HomeExecutorBase();

	virtual void setPOA(PortableServer::POA_ptr poa);
};  //End of class HomeExecutorBase

}	//End of namespace Executor

#endif

