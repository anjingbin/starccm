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

#ifndef ___HomeFinder_impl_h__
#define ___HomeFinder_impl_h__

#include <CORBA.h>
#include <HomeFinder_skel.h>
#include <HomeRegistration_skel.h>
#include <Trace.h>
#include <CCMHome.h>

#include <stdlib.h>
#include <errno.h>
#include <map>
#include <string>

using namespace std;
using namespace STARCCM;

//
// IDL:omg.org/Components:1.0
//
namespace Components
{

typedef map<std::string, Components::CCMHome_var> HomeFinderMap;

//
// IDL:omg.org/Components/HomeFinder:1.0
//
class HomeFinder_impl_tie
{
    HomeFinder_impl_tie(const HomeFinder_impl_tie&);
    void operator=(const HomeFinder_impl_tie&);

	//deleted by xiao he ping 2004/02/14

	// PortableServer::POA_var poa_;
    //CORBA::ORB_var orb_;

	//end deleted
    
    HomeFinderMap homeRepIdMap_;
    HomeFinderMap componentRepIdMap_;
    HomeFinderMap homeNameMap_;
    

public:

    //HomeFinder_impl_tie(CORBA::ORB_ptr orb,PortableServer::POA_ptr);
	HomeFinder_impl_tie();
    virtual ~HomeFinder_impl_tie();

//    virtual PortableServer::POA_ptr _default_POA();

    //
    // IDL:omg.org/Components/HomeFinder/find_home_by_component_type:1.0
    //
    virtual Components::CCMHome_ptr find_home_by_component_type(const char* comp_repid)
        throw(Components::HomeNotFound,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/HomeFinder/find_home_by_home_type:1.0
    //
    virtual Components::CCMHome_ptr find_home_by_home_type(const char* home_repid)
        throw(Components::HomeNotFound,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/HomeFinder/find_home_by_name:1.0
    //
    virtual Components::CCMHome_ptr find_home_by_name(const char* home_name)
        throw(Components::HomeNotFound,
              CORBA::SystemException);
              
    //
    // IDL:Homeregistration/register_home_by_component_type:1.0
    //
    virtual void register_home_by_component_type(const char* comp_repid,
                                                 Components::CCMHome_ptr home)
        throw(STARCCM::HomeHaveRegistered,
              CORBA::SystemException);

    //
    // IDL:Homeregistration/register_home_by_home_type:1.0
    //
    virtual void register_home_by_home_type(const char* home_repid,
                                            Components::CCMHome_ptr home)
        throw(STARCCM::HomeHaveRegistered,
              CORBA::SystemException);

    //
    // IDL:Homeregistration/register_home_by_name:1.0
    //
    virtual void register_home_by_name(const char* name,
                                       Components::CCMHome_ptr home)
        throw(STARCCM::HomeHaveRegistered,
              CORBA::SystemException);
              
    //
    // IDL:HomeRegistration/unregister_home:1.0
    //
    virtual void unregister_home(Components::CCMHome_ptr home)
        throw(STARCCM::HomeNotRegistered,
        	  CORBA::SystemException);

	virtual void unregister_by_name(const char* name)
        throw(STARCCM::HomeNotRegistered,
	  		  CORBA::SystemException);
};

} // End of namespace Components

#endif
