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

#ifndef ___EnterpriseComponent_impl_h__
#define ___EnterpriseComponent_impl_h__

#include <CCMObject.h>
#include <UserTransaction.h>

#ifndef LITE_VERSION
#include <CosPersistentState.h>
#endif

#include <EnterpriseComponent_skel.h>

//
// IDL:omg.org/Components:1.0
//
namespace Components
{

//
// IDL:omg.org/Components/EnterpriseComponent:1.0
//
class EnterpriseComponent_impl : virtual public Components::EnterpriseComponent, public CORBA::LocalObject

{
    EnterpriseComponent_impl(const EnterpriseComponent_impl&);
    void operator=(const EnterpriseComponent_impl&);

public:

    EnterpriseComponent_impl();
    ~EnterpriseComponent_impl();

    //
    // IDL:omg.org/Components/EnterpriseComponent/get_pid:1.0
    //
    virtual CORBA::OctetSeq* get_pid()
        throw(CORBA::SystemException);
};

//
// IDL:omg.org/Components/ExecutorLocator:1.0
//
class ExecutorLocator_impl : virtual public Components::ExecutorLocator,
                             virtual public Components::EnterpriseComponent_impl
{
    ExecutorLocator_impl(const ExecutorLocator_impl&);
    void operator=(const ExecutorLocator_impl&);

public:

    ExecutorLocator_impl();
    ~ExecutorLocator_impl();

    //
    // IDL:omg.org/Components/ExecutorLocator/obtain_executor:1.0
    //
    virtual CORBA::Object_ptr obtain_executor(const char* name)
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/ExecutorLocator/release_executor:1.0
    //
    virtual void release_executor(CORBA::Object_ptr exc)
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/ExecutorLocator/configuration_complete:1.0
    //
    virtual void configuration_complete()
        throw(Components::InvalidConfiguration,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/HomeExecutorBase:1.0
//
class HomeExecutorBase_impl : virtual public Components::HomeExecutorBase, public CORBA::LocalObject

{
    HomeExecutorBase_impl(const HomeExecutorBase_impl&);
    void operator=(const HomeExecutorBase_impl&);

public:

    HomeExecutorBase_impl();
    ~HomeExecutorBase_impl();

    //
    // IDL:omg.org/Components/HomeExecutorBase/get_pid:1.0
    //
    virtual CORBA::OctetSeq* get_pid()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/HomeExecutorBase/set_context:1.0
    //
    virtual void set_context(Components::CCM2Context_ptr ctx)
        throw(CORBA::SystemException);
};

//
// IDL:omg.org/Components/Context2:1.0
//
class Context2_impl : virtual public Components::Context2, public CORBA::LocalObject

{
    Context2_impl(const Context2_impl&);
    void operator=(const Context2_impl&);    
public:	
	
    Context2_impl();
    ~Context2_impl();
    
	Components::Context2_var ctx_;	
	
    //
    // IDL:omg.org/Components/Context2/resolve_object:1.0
    //
    virtual CORBA::Object_ptr resolve_object(const char* name)
        throw(CORBA::SystemException);
};

//
// IDL:omg.org/Components/CCMContext:1.0
//
class CCMContext_impl : virtual public Components::CCMContext,
                        virtual public Components::Context2_impl
{
    CCMContext_impl(const CCMContext_impl&);
    void operator=(const CCMContext_impl&);	    
public:

    CCMContext_impl();
    ~CCMContext_impl();

    //
    // IDL:omg.org/Components/CCMContext/get_caller_principal:1.0
    //
    virtual Components::Principal_ptr get_caller_principal()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/CCMContext/get_CCM_home:1.0
    //
    virtual Components::CCMHome_ptr get_CCM_home()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/CCMContext/get_rollback_only:1.0
    //
    virtual CORBA::Boolean get_rollback_only()
        throw(Components::IllegalState,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/CCMContext/get_user_transaction:1.0
    //
    virtual Components::Transaction::UserTransaction_ptr get_user_transaction()
        throw(Components::IllegalState,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/CCMContext/is_caller_in_role:1.0
    //
    virtual CORBA::Boolean is_caller_in_role(const char* role)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/CCMContext/set_rollback_only:1.0
    //
    virtual void set_rollback_only()
        throw(Components::IllegalState,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/SessionContext:1.0
//
class SessionContext_impl : virtual public Components::SessionContext,
                            virtual public Components::CCMContext_impl
{
    SessionContext_impl(const SessionContext_impl&);
    void operator=(const SessionContext_impl&);

public:

    SessionContext_impl();
    ~SessionContext_impl();

    //
    // IDL:omg.org/Components/SessionContext/get_CCM_object:1.0
    //
    virtual CORBA::Object_ptr get_CCM_object()
        throw(Components::IllegalState,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/SessionComponent:1.0
//
class SessionComponent_impl : virtual public Components::SessionComponent,
                              virtual public Components::EnterpriseComponent_impl
{
    SessionComponent_impl(const SessionComponent_impl&);
    void operator=(const SessionComponent_impl&);

public:

    SessionComponent_impl();
    ~SessionComponent_impl();

    //
    // IDL:omg.org/Components/SessionComponent/set_session_context:1.0
    //
    virtual void set_session_context(Components::SessionContext_ptr ctx)
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/SessionComponent/ccm_activate:1.0
    //
    virtual void ccm_activate()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/SessionComponent/ccm_passivate:1.0
    //
    virtual void ccm_passivate()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/SessionComponent/ccm_remove:1.0
    //
    virtual void ccm_remove()
        throw(Components::CCMException,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/SessionSynchronization:1.0
//
class SessionSynchronization_impl : virtual public Components::SessionSynchronization, public CORBA::LocalObject

{
    SessionSynchronization_impl(const SessionSynchronization_impl&);
    void operator=(const SessionSynchronization_impl&);

public:

    SessionSynchronization_impl();
    ~SessionSynchronization_impl();

    //
    // IDL:omg.org/Components/SessionSynchronization/after_begin:1.0
    //
    virtual void after_begin()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/SessionSynchronization/before_completion:1.0
    //
    virtual void before_completion()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/SessionSynchronization/after_completion:1.0
    //
    virtual void after_completion(CORBA::Boolean committed)
        throw(Components::CCMException,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/EntityContext:1.0
//
class EntityContext_impl : virtual public Components::EntityContext,
                           virtual public Components::CCMContext_impl
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

//
// IDL:omg.org/Components/EntityComponent:1.0
//
class EntityComponent_impl : virtual public Components::EntityComponent,
                             virtual public Components::EnterpriseComponent_impl
{
    EntityComponent_impl(const EntityComponent_impl&);
    void operator=(const EntityComponent_impl&);

public:

    EntityComponent_impl();
    ~EntityComponent_impl();

    //
    // IDL:omg.org/Components/EntityComponent/set_entity_context:1.0
    //
    virtual void set_entity_context(Components::EntityContext_ptr ctx)
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/EntityComponent/unset_entity_context:1.0
    //
    virtual void unset_entity_context()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/EntityComponent/ccm_activate:1.0
    //
    virtual void ccm_activate()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/EntityComponent/ccm_load:1.0
    //
    virtual void ccm_load()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/EntityComponent/ccm_store:1.0
    //
    virtual void ccm_store()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/EntityComponent/ccm_passivate:1.0
    //
    virtual void ccm_passivate()
        throw(Components::CCMException,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/EntityComponent/ccm_remove:1.0
    //
    virtual void ccm_remove()
        throw(Components::CCMException,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/CCM2Context:1.0
//
class CCM2Context_impl : virtual public Components::CCM2Context,
                         virtual public Components::CCMContext_impl
{
    CCM2Context_impl(const CCM2Context_impl&);
    void operator=(const CCM2Context_impl&);

public:

    CCM2Context_impl();
    ~CCM2Context_impl();

    //
    // IDL:omg.org/Components/CCM2Context/get_home_registration:1.0
    //
    virtual Components::HomeRegistration_ptr get_home_registration()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/CCM2Context/req_passivate:1.0
    //
    virtual void req_passivate()
        throw(Components::PolicyMismatch,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/CCM2Context/get_persistence:1.0
    //
    virtual CosPersistentState::CatalogBase_ptr get_persistence(const char* catalog_type_id)
        throw(Components::PersistenceNotAvailable,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/HomeRegistration:1.0
//
class HomeRegistration_impl : virtual public Components::HomeRegistration, public CORBA::LocalObject

{
    HomeRegistration_impl(const HomeRegistration_impl&);
    void operator=(const HomeRegistration_impl&);

public:

    HomeRegistration_impl();
    ~HomeRegistration_impl();

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

//
// IDL:omg.org/Components/ProxyHomeRegistration:1.0
//
class ProxyHomeRegistration_impl : virtual public Components::ProxyHomeRegistration,
                                   virtual public Components::HomeRegistration_impl
{
    ProxyHomeRegistration_impl(const ProxyHomeRegistration_impl&);
    void operator=(const ProxyHomeRegistration_impl&);

public:

    ProxyHomeRegistration_impl();
    ~ProxyHomeRegistration_impl();

    //
    // IDL:omg.org/Components/ProxyHomeRegistration/register_proxy_home:1.0
    //
    virtual void register_proxy_home(Components::CCMHome_ptr rhome,
                                     Components::CCMHome_ptr ahome)
        throw(Components::UnknownActualHome,
              Components::ProxyHomeNotSupported,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/Session2Context:1.0
//
class Session2Context_impl : virtual public Components::Session2Context,
                             virtual public Components::SessionContext_impl,
                             virtual public Components::CCM2Context_impl
{
    Session2Context_impl(const Session2Context_impl&);
    void operator=(const Session2Context_impl&);
	
public:

    Session2Context_impl(Components::CCM2Context_ptr ctx);
    ~Session2Context_impl();
    
    void set_Context(Components::Context2_ptr ctx);

    //
    // IDL:omg.org/Components/Session2Context/create_ref:1.0
    //
    virtual CORBA::Object_ptr create_ref(const char* repid)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/Session2Context/create_ref_from_oid:1.0
    //
    virtual CORBA::Object_ptr create_ref_from_oid(const CORBA::OctetSeq& oid,
                                                  const char* repid)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/Session2Context/get_oid_from_ref:1.0
    //
    virtual CORBA::OctetSeq* get_oid_from_ref(CORBA::Object_ptr objref)
        throw(Components::IllegalState,
              Components::BadComponentReference,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/StateIdFactory:1.0
//
class StateIdFactory_impl : virtual public Components::StateIdFactory, public CORBA::LocalObject

{
    StateIdFactory_impl(const StateIdFactory_impl&);
    void operator=(const StateIdFactory_impl&);

public:

    StateIdFactory_impl();
    ~StateIdFactory_impl();

    //
    // IDL:omg.org/Components/StateIdFactory/create:1.0
    //
    virtual Components::StateIdValue* create(const Components::IdData& data)
        throw(Components::InvalidStateIdData,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/PersistentIdValue:1.0
//
class PersistentIdValue_impl : virtual public OBV_Components::PersistentIdValue,
                               virtual public CORBA::DefaultValueRefCountBase
{
    PersistentIdValue_impl(const PersistentIdValue_impl&);
    void operator=(const PersistentIdValue_impl&);

public:

    PersistentIdValue_impl();
    ~PersistentIdValue_impl();

    virtual CORBA::ValueBase* _copy_value();

    //
    // IDL:omg.org/Components/StateIdValue/get_sid_type:1.0
    //
    virtual Components::StateIdType get_sid_type();

    //
    // IDL:omg.org/Components/StateIdValue/get_sid_data:1.0
    //
    virtual Components::IdData* get_sid_data();

//#ifndef LITE_VERSION
    //
    // IDL:omg.org/Components/PersistentIdValue/get_pid:1.0
    //
    virtual Components::PersistentId* get_pid();
//#endif
//
//#ifdef LITE_VERSION
//    //
//    // IDL:omg.org/Components/PersistentIdValue/get_pid:1.0
//    //
//    virtual Components::PersistentId get_pid();
//#endif

};

class PersistentIdValueFactory_impl : virtual public PersistentIdValue_init
{
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    PersistentIdValueFactory_impl();
    virtual ~PersistentIdValueFactory_impl();

    virtual PersistentIdValue* init(const Components::PersistentId& pid);
};

//
// IDL:omg.org/Components/SegmentDescr:1.0
//
class SegmentDescr_impl : virtual public OBV_Components::SegmentDescr,
                          virtual public CORBA::DefaultValueRefCountBase
{
    SegmentDescr_impl(const SegmentDescr_impl&);
    void operator=(const SegmentDescr_impl&);

public:

    SegmentDescr_impl();
    ~SegmentDescr_impl();

    virtual CORBA::ValueBase* _copy_value();

    //
    // IDL:omg.org/Components/SegmentDescr/get_sid:1.0
    //
    virtual Components::StateIdValue* get_sid();

    //
    // IDL:omg.org/Components/SegmentDescr/get_seg_id:1.0
    //
    virtual Components::SegmentId get_seg_id();
};

class SegmentDescrFactory_impl : virtual public SegmentDescr_init
{
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    SegmentDescrFactory_impl();
    virtual ~SegmentDescrFactory_impl();

    virtual SegmentDescr* init(Components::StateIdValue* sid,
                               Components::SegmentId seg);
};

//
// IDL:omg.org/Components/ComponentId:1.0
//
class ComponentId_impl : virtual public Components::ComponentId, public CORBA::LocalObject

{
    ComponentId_impl(const ComponentId_impl&);
    void operator=(const ComponentId_impl&);

public:

    ComponentId_impl();
    ~ComponentId_impl();

    //
    // IDL:omg.org/Components/ComponentId/get_target_facet:1.0
    //
    virtual Components::FacetId get_target_facet()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/ComponentId/get_target_segment:1.0
    //
    virtual Components::SegmentId get_target_segment()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/ComponentId/get_target_state_id:1.0
    //
    virtual Components::StateIdValue* get_target_state_id(Components::StateIdFactory_ptr sid_factory)
        throw(Components::InvalidStateIdData,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/ComponentId/get_segment_state_id:1.0
    //
    virtual Components::StateIdValue* get_segment_state_id(Components::SegmentId seg,
                                                           Components::StateIdFactory_ptr sid_factory)
        throw(Components::InvalidStateIdData,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/ComponentId/create_with_new_target:1.0
    //
    virtual Components::ComponentId_ptr create_with_new_target(Components::FacetId new_target_facet,
                                                               Components::SegmentId new_target_segment)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/ComponentId/get_segment_descrs:1.0
    //
    virtual Components::SegmentDescrSeq* get_segment_descrs(Components::StateIdFactory_ptr sid_factory)
        throw(Components::InvalidStateIdData,
              CORBA::SystemException);
};

//
// IDL:omg.org/Components/Entity2Context:1.0
//
class Entity2Context_impl : virtual public Components::Entity2Context,
                            virtual public Components::EntityContext_impl,
                            virtual public Components::CCM2Context_impl
{
    Entity2Context_impl(const Entity2Context_impl&);
    void operator=(const Entity2Context_impl&);

public:

    Entity2Context_impl(Components::CCM2Context_ptr ctx);
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

} // End of namespace Components

//
// IDL:omg.org/SecurityLevel2:1.0
//
namespace SecurityLevel2
{

//
// IDL:omg.org/SecurityLevel2/Credentials:1.0
//
class Credentials_impl : virtual public POA_SecurityLevel2::Credentials,
                         virtual public PortableServer::RefCountServantBase
{
    Credentials_impl(const Credentials_impl&);
    void operator=(const Credentials_impl&);

    PortableServer::POA_var poa_;

public:

    Credentials_impl(PortableServer::POA_ptr);
    ~Credentials_impl();

    virtual PortableServer::POA_ptr _default_POA();
};

} // End of namespace SecurityLevel2

#endif
