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

#include <Context2_impl.h>
#include <ContainerRunTime.h>
#include <ContainerBase.h>

using namespace Container;

Context2_impl::Context2_impl()
{
	
//#ifdef WITH_OTS
//resPool_ = ::STARCCM::ResourcePool_impl::getResPool();
//#endif
    
}

Context2_impl::~Context2_impl()
{
    
}

CORBA::Object_ptr
Context2_impl::resolve_object(const char* name)
{
    CORBA::Object_var ref;
    
    TRACE0(LEVEL6, "____________enter the resolve_object in componentServer_____");
    
    ContainerBase_var container = runtime_ -> getContainer();
    CORBA::ORB_var orb = container -> getOrb();

    if( strcmp(name, "ORB") == 0 )
    {
    	return orb._retn();
    }
 
//modified by xiao heping 2004/06/30   
#ifdef WITH_OTS
    if( strcmp(name, "ResourcePool") == 0 )
    {
    	return resPool_._retn();
    }  
#endif
//end modify  
    
    TRACE0(LEVEL6, "______________________end of resolve_____________");
    
    ref = orb -> resolve_initial_references(name);
    
    return ref._retn();
}

void
Context2_impl::setContainerRunTime(ContainerRunTime_ptr runtime)
{
	//modified by xiao heping 2004/03/30
//	runtime_ = OBJDUPLICATE(ContainerRunTime_ptr,runtime);
	runtime_ = runtime;
	//end modify
}

void
Context2_impl::setComponentComposition(ComponentComposition_ptr current)
{
	current_ = OBJDUPLICATE(ComponentComposition_ptr,current);
}

ContainerRunTime_ptr
Context2_impl::getContainerRunTime()
{
    return OBJDUPLICATE(ContainerRunTime_ptr,runtime_);
}

#ifdef WITH_OTS
void 
Context2_impl::setResourcePool(::STARCCM::ResourcePool_ptr res)
{
	resPool_ = STARCCM::ResourcePool::_duplicate(res);
}

void 
Context2_impl::setTransactionCurrent(::CosTransactions::Current_ptr cur)
{
	cur_ = CosTransactions::Current::_duplicate(cur);
}
#endif

