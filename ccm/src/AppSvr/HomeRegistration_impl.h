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

#ifndef __HomeRegistration_impl_h__
#define __HomeRegistration_impl_h__

#include <CORBA.h>
#include <EnterpriseComponent_skel.h>
#include <HomeRegistration.h>
#include <ContainerBase_fwd.h>

namespace Container
{

class HomeRegistration_impl : virtual public Components::HomeRegistration, public CORBA::LocalObject
{
    HomeRegistration_impl(const HomeRegistration_impl&);
    void operator=(const HomeRegistration_impl&);
    
    ContainerBase_var container_;
    ::STARCCM::HomeRegistration_var registry_;

public:

    HomeRegistration_impl(ContainerBase_ptr container);
    ~HomeRegistration_impl();

    //deleted by xiao heping 2004/02/14
	//void findHomeRegistry();
	

    //
    // IDL:omg.org/Components/HomeRegistration/register_home:1.0
    //
    virtual void register_home(Components::CCMHome_ptr home_ref,
                               const char* home_name)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/HomeRegistration/unregister_home:1.0
    //
    virtual void unregister_home(Components::CCMHome_ptr home_ref)
        throw(CORBA::SystemException);
    
};

}

#endif

