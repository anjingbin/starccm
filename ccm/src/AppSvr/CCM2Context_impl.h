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

#ifndef __CCM2Context_impl_h__
#define __CCM2Context_impl_h__

#include <CORBA.h>
#include <CCMContext_impl.h>
#include <HomeRegistration_impl.h>

namespace Container
{

class CCM2Context_impl : virtual public ::Components::CCM2Context,
						virtual public ::Container::CCMContext_impl
{
    ::Components::HomeRegistration_var registry_;

public:
	CCM2Context_impl();
	virtual ~CCM2Context_impl();

    //
    // IDL:omg.org/Components/CCM2Context/get_home_registration:1.0
    //
    virtual ::Components::HomeRegistration_ptr get_home_registration();

    //
    // IDL:omg.org/Components/CCM2Context/req_passivate:1.0
    //
    virtual void req_passivate();

    //
    // IDL:omg.org/Components/CCM2Context/get_persistence:1.0
    //
    virtual ::Components::CatalogBase_ptr get_persistence(const char* catalog_type_id);

};

}

#endif

