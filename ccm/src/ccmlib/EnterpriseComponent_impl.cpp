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
#include <EnterpriseComponent_impl.h>
#include <Trace.h>
//
// IDL:omg.org/Components:1.0
//

//
// IDL:omg.org/Components/EnterpriseComponent:1.0
//
Components::EnterpriseComponent_impl::EnterpriseComponent_impl()
{
}

Components::EnterpriseComponent_impl::~EnterpriseComponent_impl()
{
}

//
// IDL:omg.org/Components/EnterpriseComponent/get_pid:1.0
//
CORBA::OctetSeq*
Components::EnterpriseComponent_impl::get_pid()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CORBA::OctetSeq* _r = new CORBA::OctetSeq;
    return _r;
}

//
// IDL:omg.org/Components/ExecutorLocator:1.0
//
Components::ExecutorLocator_impl::ExecutorLocator_impl()
{
}

Components::ExecutorLocator_impl::~ExecutorLocator_impl()
{
}

//
// IDL:omg.org/Components/ExecutorLocator/obtain_executor:1.0
//
CORBA::Object_ptr
Components::ExecutorLocator_impl::obtain_executor(const char* name)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
    CORBA::Object_ptr _r = CORBA::Object::_nil();
    return _r;
}

//
// IDL:omg.org/Components/ExecutorLocator/release_executor:1.0
//
void
Components::ExecutorLocator_impl::release_executor(CORBA::Object_ptr exc)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/ExecutorLocator/configuration_complete:1.0
//
void
Components::ExecutorLocator_impl::configuration_complete()
    throw(Components::InvalidConfiguration,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/HomeExecutorBase:1.0
//
Components::HomeExecutorBase_impl::HomeExecutorBase_impl()
{
}

Components::HomeExecutorBase_impl::~HomeExecutorBase_impl()
{
}

//
// IDL:omg.org/Components/HomeExecutorBase/get_pid:1.0
//
CORBA::OctetSeq*
Components::HomeExecutorBase_impl::get_pid()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CORBA::OctetSeq* _r = new CORBA::OctetSeq;
    return _r;
}

//
// IDL:omg.org/Components/HomeExecutorBase/set_context:1.0
//
void
Components::HomeExecutorBase_impl::set_context(Components::CCM2Context_ptr ctx)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/Context2:1.0
//
Components::Context2_impl::Context2_impl()
{
}

Components::Context2_impl::~Context2_impl()
{
}

//
// IDL:omg.org/Components/Context2/resolve_object:1.0
//
CORBA::Object_ptr
Components::Context2_impl::resolve_object(const char* name)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    //SuLiang add.2003.8.21
    CORBA::Object_var _r = CORBA::Object::_nil();
    if(!CORBA::is_nil(ctx_))
    {
		_r = ctx_ -> resolve_object(name);
	}
	//end SuLiang add.2003.8.21   
	if(!CORBA::is_nil(_r))
    {
    	return _r._retn();
    }
    else 
    {
    	return 0;
    }
}

//
// IDL:omg.org/Components/CCMContext:1.0
//
Components::CCMContext_impl::CCMContext_impl()
{
}

Components::CCMContext_impl::~CCMContext_impl()
{
}

//
// IDL:omg.org/Components/CCMContext/get_caller_principal:1.0
//
Components::Principal_ptr
Components::CCMContext_impl::get_caller_principal()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    
    //SuLiang add.2003.8.21
    Components::CCMContext_var ccmctx = Components::CCMContext::_narrow(ctx_);
	return ccmctx -> get_caller_principal();
	//end SuLiang add.2003.8.21   

}

//
// IDL:omg.org/Components/CCMContext/get_CCM_home:1.0
//
Components::CCMHome_ptr
Components::CCMContext_impl::get_CCM_home()
    throw(CORBA::SystemException)
{
    //SuLiang add.2003.8.21
    Components::CCMContext_var ccmctx = Components::CCMContext::_narrow(ctx_);
	return ccmctx -> get_CCM_home();
	//end SuLiang add.2003.8.21   
}

//
// IDL:omg.org/Components/CCMContext/get_rollback_only:1.0
//
CORBA::Boolean
Components::CCMContext_impl::get_rollback_only()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation

    //SuLiang add.2003.8.21
#ifdef WITH_OTS
    Components::CCMContext_var ccmctx = Components::CCMContext::_narrow(ctx_);
	return ccmctx -> get_rollback_only();
	//end SuLiang add.2003.8.21   
#endif
	return true;
}

//
// IDL:omg.org/Components/CCMContext/get_user_transaction:1.0
//
Components::Transaction::UserTransaction_ptr
Components::CCMContext_impl::get_user_transaction()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation

    //SuLiang add.2003.8.21
#ifdef WITH_OTS
    Components::CCMContext_var ccmctx = Components::CCMContext::_narrow(ctx_);
	return ccmctx -> get_user_transaction();
	//end SuLiang add.2003.8.21   
#endif
	return NULL;
}

//
// IDL:omg.org/Components/CCMContext/is_caller_in_role:1.0
//
CORBA::Boolean
Components::CCMContext_impl::is_caller_in_role(const char* role)
    throw(CORBA::SystemException)
{
    // TODO: Implementation

    //SuLiang add.2003.8.21
    Components::CCMContext_var ccmctx = Components::CCMContext::_narrow(ctx_);
	return ccmctx -> is_caller_in_role(role);
	//end SuLiang add.2003.8.21   
}

//
// IDL:omg.org/Components/CCMContext/set_rollback_only:1.0
//
void
Components::CCMContext_impl::set_rollback_only()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation

    //SuLiang add.2003.8.21
#ifdef WITH_OTS
    Components::CCMContext_var ccmctx = Components::CCMContext::_narrow(ctx_);	
	ccmctx -> set_rollback_only();
#endif
	//end SuLiang add.2003.8.21   
	           
}

//
// IDL:omg.org/Components/SessionContext:1.0
//
Components::SessionContext_impl::SessionContext_impl()
{
}

Components::SessionContext_impl::~SessionContext_impl()
{
}

//
// IDL:omg.org/Components/SessionContext/get_CCM_object:1.0
//
CORBA::Object_ptr
Components::SessionContext_impl::get_CCM_object()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation
    
	//SuLiang add.2003.8.21
	Components::SessionContext_var sessionctx = Components::SessionContext::_narrow(ctx_);
	return sessionctx -> get_CCM_object();
	//end SuLiang add.2003.8.21   
}

//
// IDL:omg.org/Components/SessionComponent:1.0
//
Components::SessionComponent_impl::SessionComponent_impl()
{
}

Components::SessionComponent_impl::~SessionComponent_impl()
{
}

//
// IDL:omg.org/Components/SessionComponent/set_session_context:1.0
//
void
Components::SessionComponent_impl::set_session_context(Components::SessionContext_ptr ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/SessionComponent/ccm_activate:1.0
//
void
Components::SessionComponent_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/SessionComponent/ccm_passivate:1.0
//
void
Components::SessionComponent_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/SessionComponent/ccm_remove:1.0
//
void
Components::SessionComponent_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/SessionSynchronization:1.0
//
Components::SessionSynchronization_impl::SessionSynchronization_impl()
{
}

Components::SessionSynchronization_impl::~SessionSynchronization_impl()
{
}

//
// IDL:omg.org/Components/SessionSynchronization/after_begin:1.0
//
void
Components::SessionSynchronization_impl::after_begin()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/SessionSynchronization/before_completion:1.0
//
void
Components::SessionSynchronization_impl::before_completion()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/SessionSynchronization/after_completion:1.0
//
void
Components::SessionSynchronization_impl::after_completion(CORBA::Boolean committed)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/EntityContext:1.0
//
Components::EntityContext_impl::EntityContext_impl()
{
}

Components::EntityContext_impl::~EntityContext_impl()
{
}

//
// IDL:omg.org/Components/EntityContext/get_CCM_object:1.0
//
CORBA::Object_ptr
Components::EntityContext_impl::get_CCM_object()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation

	//SuLiang add.2003.8.21
#ifndef LITE_VERSION
	Components::EntityContext_var entityctx = Components::EntityContext::_narrow(ctx_);
	return entityctx -> get_CCM_object();
#endif
	//end SuLiang add.2003.8.21   
	return NULL;
}

//
// IDL:omg.org/Components/EntityContext/get_primary_key:1.0
//
Components::PrimaryKeyBase*
Components::EntityContext_impl::get_primary_key()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation

	//SuLiang add.2003.8.21
#ifndef LITE_VERSION
	Components::EntityContext_var entityctx = Components::EntityContext::_narrow(ctx_);
	return entityctx -> get_primary_key();
#endif
	//end SuLiang add.2003.8.21   
    return NULL;

}

//
// IDL:omg.org/Components/EntityComponent:1.0
//
Components::EntityComponent_impl::EntityComponent_impl()
{
}

Components::EntityComponent_impl::~EntityComponent_impl()
{
}

//
// IDL:omg.org/Components/EntityComponent/set_entity_context:1.0
//
void
Components::EntityComponent_impl::set_entity_context(Components::EntityContext_ptr ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation 
}

//
// IDL:omg.org/Components/EntityComponent/unset_entity_context:1.0
//
void
Components::EntityComponent_impl::unset_entity_context()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation   
}

//
// IDL:omg.org/Components/EntityComponent/ccm_activate:1.0
//
void
Components::EntityComponent_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/EntityComponent/ccm_load:1.0
//
void
Components::EntityComponent_impl::ccm_load()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation  
}

//
// IDL:omg.org/Components/EntityComponent/ccm_store:1.0
//
void
Components::EntityComponent_impl::ccm_store()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation 
}

//
// IDL:omg.org/Components/EntityComponent/ccm_passivate:1.0
//
void
Components::EntityComponent_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/EntityComponent/ccm_remove:1.0
//
void
Components::EntityComponent_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/CCM2Context:1.0
//
Components::CCM2Context_impl::CCM2Context_impl()
{
}

Components::CCM2Context_impl::~CCM2Context_impl()
{
}

//
// IDL:omg.org/Components/CCM2Context/get_home_registration:1.0
//
Components::HomeRegistration_ptr
Components::CCM2Context_impl::get_home_registration()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    
	//SuLiang add.2003.8.21
	Components::CCM2Context_var ccm2ctx = Components::CCM2Context::_narrow(ctx_);
	return ccm2ctx -> get_home_registration();
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/CCM2Context/req_passivate:1.0
//
void
Components::CCM2Context_impl::req_passivate()
    throw(Components::PolicyMismatch,
          CORBA::SystemException)
{
    // TODO: Implementation
    
	//SuLiang add.2003.8.21
	Components::CCM2Context_var ccm2ctx = Components::CCM2Context::_narrow(ctx_);
	ccm2ctx -> req_passivate();
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/CCM2Context/get_persistence:1.0
//
CosPersistentState::CatalogBase_ptr
Components::CCM2Context_impl::get_persistence(const char* catalog_type_id)
    throw(Components::PersistenceNotAvailable,
          CORBA::SystemException)
{
	//SuLiang add.2003.8.21
#ifndef LITE_VERSION
	Components::CCM2Context_var ccm2ctx = Components::CCM2Context::_narrow(ctx_);
	CosPersistentState::CatalogBase_var session = ccm2ctx -> get_persistence(catalog_type_id);
	return session._retn();
#endif
	return NULL;
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/HomeRegistration:1.0
//
Components::HomeRegistration_impl::HomeRegistration_impl()
{
}

Components::HomeRegistration_impl::~HomeRegistration_impl()
{
}

//
// IDL:omg.org/Components/HomeRegistration/register_home:1.0
//
void
Components::HomeRegistration_impl::register_home(Components::CCMHome_ptr home_ref,
                                                 const char* home_name)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/HomeRegistration/unregister_home:1.0
//
void
Components::HomeRegistration_impl::unregister_home(Components::CCMHome_ptr home_ref)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/Components/ProxyHomeRegistration:1.0
//
Components::ProxyHomeRegistration_impl::ProxyHomeRegistration_impl()
{
}

Components::ProxyHomeRegistration_impl::~ProxyHomeRegistration_impl()
{
}

//
// IDL:omg.org/Components/ProxyHomeRegistration/register_proxy_home:1.0
//
void
Components::ProxyHomeRegistration_impl::register_proxy_home(Components::CCMHome_ptr rhome,
                                                            Components::CCMHome_ptr ahome)
    throw(Components::UnknownActualHome,
          Components::ProxyHomeNotSupported,
          CORBA::SystemException)
{
    // TODO: Implementation
}



void 
Components::Session2Context_impl::set_Context(Components::Context2_ptr ctx)
{
	TRACE0(LEVEL6,"____________set_context___________");
	ctx_ = Components::Context2::_duplicate(ctx);
}





//
// IDL:omg.org/Components/Session2Context:1.0
//
Components::Session2Context_impl::Session2Context_impl(Components::CCM2Context_ptr ctx)
{
	TRACE0(LEVEL6,"____________construct session2context_impl___________");
//	Components::CCM2Context_var tempccm2ctx = Components::CCM2Context::_duplicate(ctx);
//	Components::Session2Context_var tempctx = dynamic_cast<Components::Session2Context*> (tempccm2ctx.in());
	ctx_ = Components::Context2::_duplicate( ctx );
}

Components::Session2Context_impl::~Session2Context_impl()
{
}

//
// IDL:omg.org/Components/Session2Context/create_ref:1.0
//
CORBA::Object_ptr
Components::Session2Context_impl::create_ref(const char* repid)
    throw(CORBA::SystemException)
{	
	//SuLiang add.2003.8.21
	Components::Session2Context_var session2ctx = Components::Session2Context::_narrow(ctx_);
	return session2ctx -> create_ref(repid);
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/Session2Context/create_ref_from_oid:1.0
//
CORBA::Object_ptr
Components::Session2Context_impl::create_ref_from_oid(const CORBA::OctetSeq& oid,
                                                      const char* repid)
    throw(CORBA::SystemException)
{
	//SuLiang add.2003.8.21
	Components::Session2Context_var session2ctx = Components::Session2Context::_narrow(ctx_);
	return session2ctx -> create_ref_from_oid(oid,repid);
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/Session2Context/get_oid_from_ref:1.0
//
CORBA::OctetSeq*
Components::Session2Context_impl::get_oid_from_ref(CORBA::Object_ptr objref)
    throw(Components::IllegalState,
          Components::BadComponentReference,
          CORBA::SystemException)
{
	//SuLiang add.2003.8.21
	Components::Session2Context_var session2ctx = Components::Session2Context::_narrow(ctx_);
	return session2ctx -> get_oid_from_ref(objref);
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/StateIdFactory:1.0
//
Components::StateIdFactory_impl::StateIdFactory_impl()
{
}

Components::StateIdFactory_impl::~StateIdFactory_impl()
{
}

//
// IDL:omg.org/Components/StateIdFactory/create:1.0
//
Components::StateIdValue*
Components::StateIdFactory_impl::create(const Components::IdData& data)
    throw(Components::InvalidStateIdData,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::StateIdValue* _r = 0;
    return _r;
}

//
// IDL:omg.org/Components/PersistentIdValue:1.0
//
Components::PersistentIdValue_impl::PersistentIdValue_impl()
{
}

Components::PersistentIdValue_impl::~PersistentIdValue_impl()
{
}

CORBA::ValueBase*
Components::PersistentIdValue_impl::_copy_value()
{
    PersistentIdValue_impl* _r = new PersistentIdValue_impl;
    // TODO: Implementation
    return _r;
}

//
// IDL:omg.org/Components/StateIdValue/get_sid_type:1.0
//
Components::StateIdType
Components::PersistentIdValue_impl::get_sid_type()
{
    // TODO: Implementation
    Components::StateIdType _r = 0;
    return _r;
}

//
// IDL:omg.org/Components/StateIdValue/get_sid_data:1.0
//
Components::IdData*
Components::PersistentIdValue_impl::get_sid_data()
{
    // TODO: Implementation
    Components::IdData* _r = new Components::IdData;
    return _r;
}

//
// IDL:omg.org/Components/PersistentIdValue/get_pid:1.0
//
Components::PersistentId*
Components::PersistentIdValue_impl::get_pid()
{
    // TODO: Implementation
    Components::PersistentId* _r = new Components::PersistentId;
    return _r;
}

//
//#ifndef LITE_VERSION
////
//// IDL:omg.org/Components/PersistentIdValue/get_pid:1.0
////
//Components::PersistentId*
//Components::PersistentIdValue_impl::get_pid()
//{
//    // TODO: Implementation
//    Components::PersistentId* _r = new Components::PersistentId;
//    return _r;
//}
//#endif

//#ifdef LITE_VERSION
////
//// IDL:omg.org/Components/PersistentIdValue/get_pid:1.0
////
//Components::PersistentId
//Components::PersistentIdValue_impl::get_pid()
//{
//    // TODO: Implementation
//    Components::PersistentId* _r = new Components::PersistentId;
//    return *_r;
//}
//#endif

Components::PersistentIdValueFactory_impl::PersistentIdValueFactory_impl()
{
}

Components::PersistentIdValueFactory_impl::~PersistentIdValueFactory_impl()
{
}

CORBA::ValueBase*
Components::PersistentIdValueFactory_impl::create_for_unmarshal()
{
    // TODO: Implementation
    return new Components::PersistentIdValue_impl;
}

Components::PersistentIdValue*
Components::PersistentIdValueFactory_impl::init(const Components::PersistentId& pid)
{
    // TODO: Implementation
    return new Components::PersistentIdValue_impl;
}

//
// IDL:omg.org/Components/SegmentDescr:1.0
//
Components::SegmentDescr_impl::SegmentDescr_impl()
{
}

Components::SegmentDescr_impl::~SegmentDescr_impl()
{
}

CORBA::ValueBase*
Components::SegmentDescr_impl::_copy_value()
{
    SegmentDescr_impl* _r = new SegmentDescr_impl;
    // TODO: Implementation
    return _r;
}

//
// IDL:omg.org/Components/SegmentDescr/get_sid:1.0
//
Components::StateIdValue*
Components::SegmentDescr_impl::get_sid()
{
    // TODO: Implementation
    Components::StateIdValue* _r = 0;
    return _r;
}

//
// IDL:omg.org/Components/SegmentDescr/get_seg_id:1.0
//
Components::SegmentId
Components::SegmentDescr_impl::get_seg_id()
{
    // TODO: Implementation
    Components::SegmentId _r = 0;
    return _r;
}

Components::SegmentDescrFactory_impl::SegmentDescrFactory_impl()
{
}

Components::SegmentDescrFactory_impl::~SegmentDescrFactory_impl()
{
}

CORBA::ValueBase*
Components::SegmentDescrFactory_impl::create_for_unmarshal()
{
    // TODO: Implementation
    return new Components::SegmentDescr_impl;
}

Components::SegmentDescr*
Components::SegmentDescrFactory_impl::init(Components::StateIdValue* sid,
                                           Components::SegmentId seg)
{
    // TODO: Implementation
    return new Components::SegmentDescr_impl;
}

//
// IDL:omg.org/Components/ComponentId:1.0
//
Components::ComponentId_impl::ComponentId_impl()
{
}

Components::ComponentId_impl::~ComponentId_impl()
{
}

//
// IDL:omg.org/Components/ComponentId/get_target_facet:1.0
//
Components::FacetId
Components::ComponentId_impl::get_target_facet()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    Components::FacetId _r = 0;
    return _r;
}

//
// IDL:omg.org/Components/ComponentId/get_target_segment:1.0
//
Components::SegmentId
Components::ComponentId_impl::get_target_segment()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    Components::SegmentId _r = 0;
    return _r;
}

//
// IDL:omg.org/Components/ComponentId/get_target_state_id:1.0
//
Components::StateIdValue*
Components::ComponentId_impl::get_target_state_id(Components::StateIdFactory_ptr sid_factory)
    throw(Components::InvalidStateIdData,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::StateIdValue* _r = 0;
    return _r;
}

//
// IDL:omg.org/Components/ComponentId/get_segment_state_id:1.0
//
Components::StateIdValue*
Components::ComponentId_impl::get_segment_state_id(Components::SegmentId seg,
                                                   Components::StateIdFactory_ptr sid_factory)
    throw(Components::InvalidStateIdData,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::StateIdValue* _r = 0;
    return _r;
}

//
// IDL:omg.org/Components/ComponentId/create_with_new_target:1.0
//
Components::ComponentId_ptr
Components::ComponentId_impl::create_with_new_target(Components::FacetId new_target_facet,
                                                     Components::SegmentId new_target_segment)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    Components::ComponentId_ptr _r = Components::ComponentId::_nil();
    return _r;
}

//
// IDL:omg.org/Components/ComponentId/get_segment_descrs:1.0
//
Components::SegmentDescrSeq*
Components::ComponentId_impl::get_segment_descrs(Components::StateIdFactory_ptr sid_factory)
    throw(Components::InvalidStateIdData,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::SegmentDescrSeq* _r = new Components::SegmentDescrSeq;
    return _r;
}

//
// IDL:omg.org/Components/Entity2Context:1.0
//
Components::Entity2Context_impl::Entity2Context_impl(Components::CCM2Context_ptr ctx)
{
#ifndef LITE_VERSION
	Components::Entity2Context_var tempctx = dynamic_cast<Components::Entity2Context*> (ctx);
	ctx_ = Components::Context2::_duplicate(tempctx);
#endif
}

Components::Entity2Context_impl::~Entity2Context_impl()
{
}

//
// IDL:omg.org/Components/Entity2Context/get_component_id:1.0
//
Components::ComponentId_ptr
Components::Entity2Context_impl::get_component_id()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation  

	//SuLiang add.2003.8.21
#ifndef LITE_VERSION
	Components::Entity2Context_var entity2ctx = Components::Entity2Context::_narrow(ctx_);
	return entity2ctx -> get_component_id();
#endif
	return NULL;
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/Entity2Context/create_component_id:1.0
//
Components::ComponentId_ptr
Components::Entity2Context_impl::create_component_id(Components::FacetId target_facet,
                                                     Components::SegmentId target_segment,
                                                     const Components::SegmentDescrSeq& seq_descrs)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    
	//SuLiang add.2003.8.21
#ifndef LITE_VERSION
	Components::Entity2Context_var entity2ctx = Components::Entity2Context::_narrow(ctx_);
	return entity2ctx -> create_component_id(target_facet,target_segment,seq_descrs);
#endif
	return NULL;
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/Entity2Context/create_monolithic_component_id:1.0
//
Components::ComponentId_ptr
Components::Entity2Context_impl::create_monolithic_component_id(Components::FacetId target_facet,
                                                                Components::StateIdValue* sid)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    
	//SuLiang add.2003.8.21
#ifndef LITE_VERSION
	Components::Entity2Context_var entity2ctx = Components::Entity2Context::_narrow(ctx_);
	return entity2ctx -> create_monolithic_component_id(target_facet,sid);
#endif
	return NULL;
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/Entity2Context/create_ref_from_cid:1.0
//
CORBA::Object_ptr
Components::Entity2Context_impl::create_ref_from_cid(const char* repid,
                                                     Components::ComponentId_ptr cid)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    
	//SuLiang add.2003.8.21
#ifndef LITE_VERSION
	Components::Entity2Context_var entity2ctx = Components::Entity2Context::_narrow(ctx_);
	return entity2ctx -> create_ref_from_cid(repid,cid);
#endif
	return NULL;
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/Components/Entity2Context/get_cid_from_ref:1.0
//
Components::ComponentId_ptr
Components::Entity2Context_impl::get_cid_from_ref(CORBA::Object_ptr objref)
    throw(Components::BadComponentReference,
          CORBA::SystemException)
{
    // TODO: Implementation
    
	//SuLiang add.2003.8.21
#ifndef LITE_VERSION
	Components::Entity2Context_var entity2ctx = Components::Entity2Context::_narrow(ctx_);
	return entity2ctx -> get_cid_from_ref(objref);
#endif
	return NULL;
	//end SuLiang add.2003.8.21
}

//
// IDL:omg.org/SecurityLevel2:1.0
//

//
// IDL:omg.org/SecurityLevel2/Credentials:1.0
//
SecurityLevel2::Credentials_impl::Credentials_impl(PortableServer::POA_ptr poa)
    : poa_(PortableServer::POA::_duplicate(poa))
{
}

SecurityLevel2::Credentials_impl::~Credentials_impl()
{
}

PortableServer::POA_ptr
SecurityLevel2::Credentials_impl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}
