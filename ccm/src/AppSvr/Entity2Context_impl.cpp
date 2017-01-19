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

#include <Entity2Context_impl.h>

using namespace Container;

Entity2Context_impl::Entity2Context_impl()
{
}

Entity2Context_impl::~Entity2Context_impl()
{
}

//
// IDL:omg.org/Components/Entity2Context/get_component_id:1.0
//
Components::ComponentId_ptr
Entity2Context_impl::get_component_id()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::ComponentId_ptr _r = Components::ComponentId::_nil();
    return _r;
}

//
// IDL:omg.org/Components/Entity2Context/create_component_id:1.0
//
Components::ComponentId_ptr
Entity2Context_impl::create_component_id(Components::FacetId target_facet,
                                                     Components::SegmentId target_segment,
                                                     const Components::SegmentDescrSeq& seq_descrs)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    Components::ComponentId_ptr _r = Components::ComponentId::_nil();
    return _r;
}

//
// IDL:omg.org/Components/Entity2Context/create_monolithic_component_id:1.0
//
Components::ComponentId_ptr
Entity2Context_impl::create_monolithic_component_id(Components::FacetId target_facet,
                                                                Components::StateIdValue* sid)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    Components::ComponentId_ptr _r = Components::ComponentId::_nil();
    return _r;
}

//
// IDL:omg.org/Components/Entity2Context/create_ref_from_cid:1.0
//
CORBA::Object_ptr
Entity2Context_impl::create_ref_from_cid(const char* repid,
                                                     Components::ComponentId_ptr cid)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CORBA::Object_ptr _r = CORBA::Object::_nil();
    return _r;
}

//
// IDL:omg.org/Components/Entity2Context/get_cid_from_ref:1.0
//
Components::ComponentId_ptr
Entity2Context_impl::get_cid_from_ref(CORBA::Object_ptr objref)
    throw(Components::BadComponentReference,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::ComponentId_ptr _r = Components::ComponentId::_nil();
    return _r;
}

