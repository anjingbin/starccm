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
#include <HomeFinder_impl.h>

using namespace Components;

//
// IDL:omg.org/Components:1.0
//

//
// IDL:omg.org/Components/HomeFinder:1.0
//
Components::HomeFinder_impl::HomeFinder_impl(CORBA::ORB_ptr orb,PortableServer::POA_ptr poa)
    : poa_(PortableServer::POA::_duplicate(poa)),
      orb_(CORBA::ORB::_duplicate(orb))
{
	
	CORBA::Object_var obj;
	
	try
	{
		orb -> resolve_initial_references("NameService");
	}
	catch(const CORBA::ORB::InvalidName&)
	{
		TRACE0(LEVEL1,"Can NOT resolve name service\n");
	}
	
	if(CORBA::is_nil(obj.in()))
    {
        TRACE0(LEVEL1,"Can NOT resolve name service\n");
    }
    
    rootNamingContext =
    	CosNaming::NamingContext::_narrow(obj.in());
    
    

}

Components::HomeFinder_impl::~HomeFinder_impl()
{
}

PortableServer::POA_ptr
Components::HomeFinder_impl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}

//
// IDL:omg.org/Components/HomeFinder/find_home_by_component_type:1.0
//
Components::CCMHome_ptr
Components::HomeFinder_impl::find_home_by_component_type(const char* comp_repid)
    throw(Components::HomeNotFound,CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_ptr _r = Components::CCMHome::_nil();
    return _r;
}

//
// IDL:omg.org/Components/HomeFinder/find_home_by_home_type:1.0
//
Components::CCMHome_ptr
Components::HomeFinder_impl::find_home_by_home_type(const char* home_repid)
    throw(Components::HomeNotFound,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_ptr _r = Components::CCMHome::_nil();
    return _r;
}

//
// IDL:omg.org/Components/HomeFinder/find_home_by_name:1.0
//
Components::CCMHome_ptr
Components::HomeFinder_impl::find_home_by_name(const char* home_name)
    throw(Components::HomeNotFound,CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_ptr _r = Components::CCMHome::_nil();
    return _r;
}
