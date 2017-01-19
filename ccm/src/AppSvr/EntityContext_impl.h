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

#ifndef __EntityContext_impl_h__
#define __EntityContext_impl_h__

#include <CORBA.h>
#include <EnterpriseComponent_skel.h>
#include <CCMContext_impl.h>

namespace Container
{

class EntityContext_impl : virtual public Components::EntityContext,
                           virtual public CCMContext_impl
{
    EntityContext_impl(const EntityContext_impl&);
    void operator=(const EntityContext_impl&);

public:

    EntityContext_impl();
    ~EntityContext_impl();

    //
    // IDL:omg.org/Components/EntityContext/get_CCM_object:1.0
    //
    virtual CORBA::Object_ptr get_CCM_object()
        throw(Components::IllegalState,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/EntityContext/get_primary_key:1.0
    //
    virtual Components::PrimaryKeyBase* get_primary_key()
        throw(Components::IllegalState,
              CORBA::SystemException);
};

}

#endif
