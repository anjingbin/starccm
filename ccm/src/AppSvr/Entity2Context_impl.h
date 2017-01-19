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

#ifndef __Entity2Context_impl_h__
#define __Entity2Context_impl_h__

#include <CORBA.h>
#include <EnterpriseComponent_skel.h>
#include <EntityContext_impl.h>
#include <CCM2Context_impl.h>

namespace Container
{

class Entity2Context_impl : virtual public Components::Entity2Context,
                            virtual public EntityContext_impl,
                            virtual public CCM2Context_impl
{
    Entity2Context_impl(const Entity2Context_impl&);
    void operator=(const Entity2Context_impl&);

public:

    Entity2Context_impl();
    ~Entity2Context_impl();

    //
    // IDL:omg.org/Components/Entity2Context/get_component_id:1.0
    //
    virtual Components::ComponentId_ptr get_component_id()
        throw(Components::IllegalState,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Entity2Context/create_component_id:1.0
    //
    virtual Components::ComponentId_ptr create_component_id(Components::FacetId target_facet,
                                                            Components::SegmentId target_segment,
                                                            const Components::SegmentDescrSeq& seq_descrs)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/Entity2Context/create_monolithic_component_id:1.0
    //
    virtual Components::ComponentId_ptr create_monolithic_component_id(Components::FacetId target_facet,
                                                                       Components::StateIdValue* sid)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/Entity2Context/create_ref_from_cid:1.0
    //
    virtual CORBA::Object_ptr create_ref_from_cid(const char* repid,
                                                  Components::ComponentId_ptr cid)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/Entity2Context/get_cid_from_ref:1.0
    //
    virtual Components::ComponentId_ptr get_cid_from_ref(CORBA::Object_ptr objref)
        throw(Components::BadComponentReference,
              CORBA::SystemException);
};

}

#endif

