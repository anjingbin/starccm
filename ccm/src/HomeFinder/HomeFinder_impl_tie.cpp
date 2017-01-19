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
#include <HomeFinder_impl_tie.h>

using namespace Components;
using namespace std;
using namespace STARCCM;

//
// IDL:omg.org/Components:1.0
//

//
// IDL:omg.org/Components/HomeFinder:1.0
//

//modified by xiao heping 2004/02/14

//Components::HomeFinder_impl_tie::HomeFinder_impl_tie(CORBA::ORB_ptr orb,PortableServer::POA_ptr poa)
//    : poa_(PortableServer::POA::_duplicate(poa)),
//      orb_(CORBA::ORB::_duplicate(orb))
Components::HomeFinder_impl_tie::HomeFinder_impl_tie()

//end modified
{
    

}

Components::HomeFinder_impl_tie::~HomeFinder_impl_tie()
{
}

//PortableServer::POA_ptr
//Components::HomeFinder_impl_tie::_default_POA()
//{
//    return PortableServer::POA::_duplicate(poa_);
//}

//
// IDL:omg.org/Components/HomeFinder/find_home_by_component_type:1.0
//
Components::CCMHome_ptr
Components::HomeFinder_impl_tie::find_home_by_component_type(const char* comp_repid)
    throw(Components::HomeNotFound,CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_var _r = Components::CCMHome::_nil();
    
    std::string key(comp_repid);
    
    HomeFinderMap::const_iterator result;
    
    result = componentRepIdMap_.find(key.c_str());
    
    if( result != componentRepIdMap_.end() )
    {
        _r = Components::CCMHome::_duplicate(result->second);
    }
    else
    {
        throw new Components::HomeNotFound();
    }
    
    return _r._retn();
}

//
// IDL:omg.org/Components/HomeFinder/find_home_by_home_type:1.0
//
Components::CCMHome_ptr
Components::HomeFinder_impl_tie::find_home_by_home_type(const char* home_repid)
    throw(Components::HomeNotFound,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_var _r = Components::CCMHome::_nil();
    
    std::string key(home_repid);
    
    HomeFinderMap::const_iterator result;
    
    result = homeRepIdMap_.find(key.c_str());
    
    if( result != homeRepIdMap_.end() )
    {
        _r = Components::CCMHome::_duplicate(result->second);
    }
    else
    {
        throw new Components::HomeNotFound();
    }
    
    return _r._retn();
}

//
// IDL:omg.org/Components/HomeFinder/find_home_by_name:1.0
//
Components::CCMHome_ptr
Components::HomeFinder_impl_tie::find_home_by_name(const char* home_name)
    throw(Components::HomeNotFound,CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_var _r = Components::CCMHome::_nil();
    
    std::string key(home_name);
    
    HomeFinderMap::const_iterator result;
    
    result = homeNameMap_.find(key.c_str());
    
    if( result != homeNameMap_.end() )
    {
        _r = Components::CCMHome::_duplicate(result->second);
    }
    else
    {
        throw new Components::HomeNotFound();
    }
    
    return _r._retn();
}

//
// IDL:Homeregistration/register_home_by_component_type:1.0
//
void
Components::HomeFinder_impl_tie::register_home_by_component_type(const char* comp_repid,
                                                       Components::CCMHome_ptr home)
    throw(HomeHaveRegistered,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_var r = Components::CCMHome::_duplicate(home);
    
    std::string key(comp_repid);
    
    pair<HomeFinderMap::iterator, bool> result;
    
    result = componentRepIdMap_.insert(HomeFinderMap::value_type(key.c_str(),Components::CCMHome::_duplicate(r)));
    
    if( result.second == false)
	{
		throw new HomeHaveRegistered();
	}
}

//
// IDL:Homeregistration/register_home_by_home_type:1.0
//
void
Components::HomeFinder_impl_tie::register_home_by_home_type(const char* home_repid,
                                                  Components::CCMHome_ptr home)
    throw(HomeHaveRegistered,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_var r = Components::CCMHome::_duplicate(home);
    
    std::string key(home_repid);
    
    pair<HomeFinderMap::iterator, bool> result;
    
    result = homeRepIdMap_.insert(HomeFinderMap::value_type(key.c_str(),Components::CCMHome::_duplicate(r)));
    
    if( result.second == false)
	{
		throw new HomeHaveRegistered();
	}
}

//
// IDL:Homeregistration/register_home_by_name:1.0
//
void
Components::HomeFinder_impl_tie::register_home_by_name(const char* name,
                                             Components::CCMHome_ptr home)
    throw(HomeHaveRegistered,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::CCMHome_var r = Components::CCMHome::_duplicate(home);
    
    std::string key(name);
    
    pair<HomeFinderMap::iterator, bool> result;
    
    result = homeNameMap_.insert(HomeFinderMap::value_type(key.c_str(),Components::CCMHome::_duplicate(r)));
    
    if( result.second == false)
	{
		throw new HomeHaveRegistered();
	}
}

//
// IDL:HomeRegistration/unregister_home:1.0
//
void
Components::HomeFinder_impl_tie::unregister_home(Components::CCMHome_ptr home)
    throw(HomeNotRegistered,CORBA::SystemException)
{
    // TODO: Implementation
    // find all references and remove them.
    Components::CCMHome_var unregHome = Components::CCMHome::_duplicate(home);
    
    HomeFinderMap::iterator homeRepIdIterator;
    HomeFinderMap::iterator componentRepIdIterator;
    HomeFinderMap::iterator homeNameIterator;
    Components::CCMHome_var tempHome;
    
    bool isFind = false;
    
    homeRepIdIterator = homeRepIdMap_.begin();
    componentRepIdIterator = componentRepIdMap_.begin();
    homeNameIterator = homeNameMap_.begin();
    
    
    while (homeRepIdIterator != homeRepIdMap_.end())
    {
    	tempHome = Components::CCMHome::_duplicate((*homeRepIdIterator).second);
    	if( unregHome -> _is_equivalent(tempHome))
		{
			isFind = true;
			homeRepIdMap_.erase(homeRepIdIterator);
			break;
		}
		homeRepIdIterator++;
	}
	
	while (componentRepIdIterator != componentRepIdMap_.end())
	{
		tempHome = Components::CCMHome::_duplicate((*componentRepIdIterator).second);
		if (unregHome -> _is_equivalent(tempHome))
		{
			isFind = true;
			componentRepIdMap_.erase(componentRepIdIterator);
			break;
		}
		componentRepIdIterator++;
	}
    
    while(homeNameIterator != homeNameMap_.end())
    {
    	tempHome = Components::CCMHome::_duplicate((*homeNameIterator).second);
    	if(unregHome -> _is_equivalent(tempHome))
    	{
    		isFind = true;
    		homeNameMap_.erase(homeNameIterator);
    		break;
    	}
    	homeNameIterator++;
    }
    
    if( isFind == false )
    {
    	throw new STARCCM::HomeNotRegistered();
    }
    
}

void 
Components::HomeFinder_impl_tie::unregister_by_name(const char* name)
		throw(HomeNotRegistered,CORBA::SystemException)
{
	Components::CCMHome_var home_;
	try
	{
		home_ = find_home_by_name(name);
	}
	catch(...)
	{
		throw new STARCCM::HomeNotRegistered();
	}	
	unregister_home( home_._retn() );
}

