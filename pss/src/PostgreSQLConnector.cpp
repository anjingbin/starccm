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

#include <PostgreSQLConnector.h>
#include <PostgreSQLSession.h>

PSS::PostgreSQLConnector::PostgreSQLConnector():DatabaseConnector( "PSS:connector:database:postgresql:1.0" )
{
	
}

//
// IDL:omg.org/CosPersistentState/Connector/get_pid:1.0
//
CosPersistentState::Pid*
PSS::PostgreSQLConnector::get_pid(CosPersistentState::StorageObjectBase_ptr obj)
    throw(CORBA::SystemException)
{
	CosPersistentState::Pid* _r = NULL;

	PersistentObject * pobj = dynamic_cast< PersistentObject * >(obj);

	if( pobj != NULL )
	{
			 _r = pobj -> get_pid();
	}
	else
	{
		PSSTRACE("CosPersistentState::Connector::get_pid : invalid storage object specified!");
		throw CORBA::PERSIST_STORE();
	}
    
    return _r;
}

//
// IDL:omg.org/CosPersistentState/Connector/get_short_pid:1.0
//
CosPersistentState::ShortPid*
PSS::PostgreSQLConnector::get_short_pid(CosPersistentState::StorageObjectBase_ptr obj)
    throw(CORBA::SystemException)
{
	CosPersistentState::ShortPid* _r = NULL;
	PersistentObject * pobj = dynamic_cast< PersistentObject * >(obj);
	if( pobj != NULL )
	{
		_r = pobj -> get_short_pid();
	}
	else
	{
		PSSTRACE("CosPersistentState::Connector::get_short_pid : invalid storage object specified!");
		throw CORBA::PERSIST_STORE();
	}
    
    return _r;
}

//
// IDL:omg.org/CosPersistentState/Connector/create_basic_session:1.0
//
CosPersistentState::Session_ptr
PSS::PostgreSQLConnector::create_basic_session(CosPersistentState::AccessMode access_mode,
                                                         const CosPersistentState::ParameterList& parameters)
    throw(CORBA::SystemException)
{
	PostgreSQLSession* session = new PostgreSQLSession(access_mode, this);

	session -> connect( parameters );

    return session;
}

//
// IDL:omg.org/CosPersistentState/Connector/create_transactional_session:1.0
//
CosPersistentState::TransactionalSession_ptr
PSS::PostgreSQLConnector::create_transactional_session(CosPersistentState::AccessMode access_mode,
		CosPersistentState::IsolationLevel default_isolation_level,
		CosPersistentState::EndOfAssociationCallback_ptr callback,
		const CosPersistentState::ParameterList& additional_parameters)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CosPersistentState::TransactionalSession_ptr _r = CosPersistentState::TransactionalSession::_nil();
    return _r;
}

//
// IDL:omg.org/CosPersistentState/Connector/create_session_pool:1.0
//
CosPersistentState::SessionPool_ptr
PSS::PostgreSQLConnector::create_session_pool(CosPersistentState::AccessMode access_mode,
        CosPersistentState::TransactionPolicy tx_policy,
        const CosPersistentState::ParameterList& additional_parameters)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CosPersistentState::SessionPool_ptr _r = CosPersistentState::SessionPool::_nil();
    return _r;
}

//
// IDL:omg.org/CosPersistentState/Connector/current_session:1.0
//
CosPersistentState::TransactionalSession_ptr
PSS::PostgreSQLConnector::current_session()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CosPersistentState::TransactionalSession_ptr _r = CosPersistentState::TransactionalSession::_nil();
    return _r;
}

//
// IDL:omg.org/CosPersistentState/Connector/sessions:1.0
//
CosPersistentState::TransactionalSessionList*
PSS::PostgreSQLConnector::sessions(CosTransactions::Coordinator_ptr transaction)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CosPersistentState::TransactionalSessionList* _r = new CosPersistentState::TransactionalSessionList;
    return _r;
}
