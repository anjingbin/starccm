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

#include <CORBA.h>
#include <ConnectorRegistry_impl.h>

//
// IDL:omg.org/PSS/ConnectorRegistry:1.0
//
PSS::ConnectorRegistry_impl::ConnectorRegistry_impl()
{
}

PSS::ConnectorRegistry_impl::~ConnectorRegistry_impl()
{
}

//
// IDL:omg.org/PSS/ConnectorRegistry/find_connector:1.0
//
CosPersistentState::Connector_ptr
PSS::ConnectorRegistry_impl::find_connector(const char* implementation_id)
    throw(CosPersistentState::NotFound,CORBA::SystemException)
{
    CosPersistentState::Connector_var _r = CosPersistentState::Connector::_nil();

	theIterator_ = connectors_.find(implementation_id);

	if( theIterator_ == connectors_.end())
	{
		throw CosPersistentState::NotFound();
	}
	else
	{
		_r = connectors_[implementation_id];
	}
    return _r._retn();
}

//
// IDL:omg.org/PSS/ConnectorRegistry/register_connector:1.0
//
void
PSS::ConnectorRegistry_impl::register_connector(
	CosPersistentState::Connector_ptr c,
	const char* implementation_id)
    throw(CORBA::SystemException)
{
    connectors_[implementation_id] = c;
}

//
// IDL:omg.org/PSS/ConnectorRegistry/unregister_connector:1.0
//
void
PSS::ConnectorRegistry_impl::unregister_connector(const char* implementation_id)
    throw(CosPersistentState::NotFound,CORBA::SystemException)
{
	theIterator_ = connectors_.find(implementation_id);

	if( theIterator_ == connectors_.end())
	{
		throw CosPersistentState::NotFound();
	}
	else
	{
		connectors_.erase(implementation_id);
	}
}
