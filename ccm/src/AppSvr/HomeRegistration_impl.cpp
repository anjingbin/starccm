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

#include <HomeRegistration_impl.h>
#include <ContainerBase.h>

using namespace Container;

//
// IDL:omg.org/Components/HomeRegistration:1.0
//
HomeRegistration_impl::HomeRegistration_impl(ContainerBase_ptr container)
{
    container_ = OBJDUPLICATE(ContainerBase_ptr, container);

	//added by xiao heping 2004/02/14
	CORBA::ORB_var orb = container_->getOrb();
    
    try
    {
        CORBA::Object_var obj = orb->resolve_initial_references("HomeRegistration");
        
        registry_ = ::STARCCM::HomeRegistration::_narrow(obj);
    }
    catch(CORBA::SystemException&)
    {
        TRACE0(LEVEL2,"ERROR:HomeRegisrtation NOT found.\n");
    }
	//end added
}

HomeRegistration_impl::~HomeRegistration_impl()
{
}

//deleted by xiao he ping 2004/02/14
/*
void
HomeRegistration_impl::findHomeRegistry()
{
    CORBA::ORB_var orb = container_->getOrb();
    
    try
    {
        CORBA::Object_var obj = orb->resolve_initial_references("HomeRegistration");
        
        registry_ = ::STARCCM::HomeRegistration::_narrow(obj);
    }
    catch(CORBA::SystemException&)
    {
        TRACE0(LEVEL2,"ERROR:HomeRegisrtation NOT found.\n");
    }
}
*/
//end deleted

//
// IDL:omg.org/Components/HomeRegistration/register_home:1.0
//
void
HomeRegistration_impl::register_home(Components::CCMHome_ptr home_ref,
                                                 const char* home_name)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    
	//deleted by xiao he ping 2004/02/14
	//  findHomeRegistry();
    
    if( registry_.in() != NULL )
    {
        registry_->register_home_by_name(home_name, home_ref);
    }
}

//
// IDL:omg.org/Components/HomeRegistration/unregister_home:1.0
//
void
HomeRegistration_impl::unregister_home(Components::CCMHome_ptr home_ref)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    
	//deleted by xiao he ping 2004/02/14
	//   findHomeRegistry();
    
    if( registry_.in() != NULL )
    {
        registry_->unregister_home(home_ref);
    }
}

