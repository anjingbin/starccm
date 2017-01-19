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

#ifndef __Session2Context_impl_h__
#define __Session2Context_impl_h__

#include <CORBA.h>
#include <EnterpriseComponent.h>
#include <CCM2Context_impl.h>
#include <SessionContext_impl.h>

namespace Container
{

class Session2Context_impl : virtual public ::Components::Session2Context,
							 virtual public SessionContext_impl,
							 virtual public CCM2Context_impl
{

public:
	Session2Context_impl();
	virtual ~Session2Context_impl();

    //
    // IDL:omg.org/Components/Session2Context/create_ref:1.0
    //
    virtual CORBA::Object_ptr create_ref(const char* repid);

    //
    // IDL:omg.org/Components/Session2Context/create_ref_from_oid:1.0
    //
    virtual CORBA::Object_ptr create_ref_from_oid(const ::CORBA::OctetSeq& oid,
                                                  const char* repid);

    //
    // IDL:omg.org/Components/Session2Context/get_oid_from_ref:1.0
    //
    virtual ::CORBA::OctetSeq* get_oid_from_ref(CORBA::Object_ptr objref);

};

}

#endif

