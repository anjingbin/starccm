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

#ifndef __Context2_impl_h__
#define __Context2_impl_h__

#include <CORBA.h>
#include <CosTransactions.h>

#ifdef WITH_OTS
#include <ResourcePool.h>
#endif

#include <Context2.h>
#include <ContainerRunTime_fwd.h>
#include <ComponentComposition.h>

namespace Container
{

class Context2_impl : virtual public ::Components::Context2
                    , REF_COUNT_LOCAL_OBJECT
{
protected:
	//modified by xiao heping 2004/03/30
	//ContainerRunTime_var runtime_;
	ContainerRunTime_ptr runtime_;
	//end modify xiao heping 2004/03/30
	ComponentComposition_var current_;

#ifdef WITH_OTS
	::STARCCM::ResourcePool_var resPool_;
	::CosTransactions::Current_var cur_;
#endif 

public:
    Context2_impl();
    virtual ~Context2_impl();
    
    virtual CORBA::Object_ptr resolve_object(const char* name);
    
	//private operation for container
	virtual void setContainerRunTime(ContainerRunTime_ptr runtime);
	virtual void setComponentComposition(ComponentComposition_ptr current);
	virtual ContainerRunTime_ptr getContainerRunTime();

#ifdef WITH_OTS
	virtual void setResourcePool(::STARCCM::ResourcePool_ptr res);
	virtual void setTransactionCurrent(::CosTransactions::Current_ptr cur);
#endif

};   
    
}

#endif

