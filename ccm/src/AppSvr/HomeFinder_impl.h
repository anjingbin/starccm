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

#include <HomeFinder_skel.h>
#include <CosNaming.h>
//#include <Trace.h>


//
// IDL:omg.org/Components:1.0
//
namespace Components
{

//
// IDL:omg.org/Components/HomeFinder:1.0
//
class HomeFinder_impl : virtual public POA_Components::HomeFinder,
                        virtual public PortableServer::RefCountServantBase
{
    HomeFinder_impl(const HomeFinder_impl&);
    void operator=(const HomeFinder_impl&);

    PortableServer::POA_var poa_;
    CORBA::ORB_var orb_;
    
protected:
	CosNaming::NamingContext_var rootNamingContext;

public:

    HomeFinder_impl(CORBA::ORB_ptr orb,PortableServer::POA_ptr);
    ~HomeFinder_impl();

    virtual PortableServer::POA_ptr _default_POA();

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
};

} // End of namespace Components

#endif
