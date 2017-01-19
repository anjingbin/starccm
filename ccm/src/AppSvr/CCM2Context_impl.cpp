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

#include <CCM2Context_impl.h>
#include <ContainerRunTime.h>
#include <ContainerBase.h>

#ifndef LITE_VERSION
#include <CosPersistentState.h>
#include <PSSInitializer.h>
#endif

using namespace Container;

CCM2Context_impl::CCM2Context_impl()
{
//    registry_ = new ::Container::HomeRegistration_impl(runtime_->getContainer());
}

CCM2Context_impl::~CCM2Context_impl()
{
}

//
// IDL:omg.org/Components/CCM2Context/get_home_registration:1.0
//
::Components::HomeRegistration_ptr
CCM2Context_impl::get_home_registration()
{
    if( CORBA::is_nil(registry_) )
    {
		::Container::ContainerBase_var contaierBase = runtime_->getContainer();
        registry_ = new ::Container::HomeRegistration_impl(contaierBase.in());
    }
	return ::Components::HomeRegistration::_duplicate(registry_.in());
}

//
// IDL:omg.org/Components/CCM2Context/req_passivate:1.0
//
void
CCM2Context_impl::req_passivate()
{
	//TO DO:flush to DB
}

//
// IDL:omg.org/Components/CCM2Context/get_persistence:1.0
//
::Components::CatalogBase_ptr
CCM2Context_impl::get_persistence(const char* catalog_type_id)
{
#ifndef LITE_VERSION 
    ContainerBase_var container = runtime_->getContainer();
    CORBA::ORB_var orb = container->getOrb();
    
    CORBA::Object_var crObj = orb -> resolve_initial_references("PSS");

    CosPersistentState::ConnectorRegistry_var connectorRegistry = CosPersistentState::ConnectorRegistry::_narrow(crObj);
    
    if( CORBA::is_nil(connectorRegistry) )
    {
        TRACE0(LEVEL1, "Persistent State Service NOT found.\n");
        
        return NULL;
    }
    
    PSSInitializer_var initializer = runtime_->getPssInitializer();
    
    CORBA::String_var connectorId = initializer->getConnectorId();
    
    CosPersistentState::Connector_var connector = 
        connectorRegistry -> find_connector(connectorId.in());
    
    if( CORBA::is_nil(connector) )
    {
        TRACE0(LEVEL1, "Persistent State Service Connector NOT found.\n");
    }
    
    CosPersistentState::Session_var mySession;
    
    
    short mode = initializer->getAccessMode();
    CosPersistentState::ParameterList_var parameters = initializer->getPatemeters();
    
//    CosPersistentState::ParameterList param(parameters);
    
    mySession = connector->create_basic_session(mode, /*catalog_type_id,*/ parameters);
    
	return mySession._retn();
#endif

	return NULL;
}
