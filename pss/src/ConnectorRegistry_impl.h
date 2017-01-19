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

#ifndef _ConnectorRegistry_impl_h_
#define _ConnectorRegistry_impl_h_

#include <CosPersistentState.h>
#include <string>
#include <map>

using namespace std;

//
// IDL:omg.org/CosPersistentState:1.0
//
namespace PSS
{

//
// IDL:omg.org/CosPersistentState/ConnectorRegistry:1.0
//
class ConnectorRegistry_impl : virtual public CORBA::LocalObject, virtual public CosPersistentState::ConnectorRegistry
{
    ConnectorRegistry_impl(const ConnectorRegistry_impl&);
    void operator=(const ConnectorRegistry_impl&);
	
	typedef map< string,CosPersistentState::Connector_var > ConnectorMap;
	ConnectorMap connectors_;
	ConnectorMap::iterator theIterator_;

public:

    ConnectorRegistry_impl();
	virtual ~ConnectorRegistry_impl();

    //
    // IDL:omg.org/CosPersistentState/ConnectorRegistry/find_connector:1.0
    //
    virtual CosPersistentState::Connector_ptr find_connector(const char* implementation_id)
        throw(CosPersistentState::NotFound,
              CORBA::SystemException);

    //
    // IDL:omg.org/CosPersistentState/ConnectorRegistry/register_connector:1.0
    //
    virtual void register_connector(CosPersistentState::Connector_ptr c,const char* implementation_id)
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/CosPersistentState/ConnectorRegistry/unregister_connector:1.0
    //
    virtual void unregister_connector(const char* implementation_id)
        throw(CosPersistentState::NotFound,
              CORBA::SystemException);
};

}

#endif /* #ifndef _ConnectorRegistry_impl_h_ */

