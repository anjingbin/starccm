// **********************************************************************
//
// Copyright (c) 2001-2002 
// StarMiddleware Group	
// www.StarMiddleware.org
// 
// All Rights Reserved
//
// Author: An Jingbin	anjb@sina.com	
// **********************************************************************

#include <PSS.h>

#include <PSSInit.h>
#include <ConnectorRegistry_impl.h>
#include <PostgreSQLConnector.h>

void 
PSS::initialize_persistent_service(CORBA::ORB_ptr orb)
{
	CosPersistentState::ConnectorRegistry * registry = new PSS::ConnectorRegistry_impl();
	registry -> register_connector( (CosPersistentState::Connector_ptr) new PSS::PostgreSQLConnector(),"PSS:connector:database:postgresql:1.0" );
	
	orb -> register_initial_reference("PSS",registry);
	
}
