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
#include <TransactionalDatabaseSession.h>

//
// IDL:omg.org/CosPersistentState/CatalogBase:1.0
//
PSS::TransactionalDatabaseSession::TransactionalDatabaseSession(CosPersistentState::AccessMode access_mode,CosPersistentState::Connector_ptr connector):
	DatabaseSession(access_mode,connector)
{
}

PSS::TransactionalDatabaseSession::~TransactionalDatabaseSession()
{
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/access_mode:1.0
//
CosPersistentState::AccessMode
PSS::TransactionalDatabaseSession::access_mode()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CosPersistentState::AccessMode _r = 0;
    return _r;
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/find_storage_home:1.0
//
CosPersistentState::StorageHomeBase_ptr
PSS::TransactionalDatabaseSession::find_storage_home(const char* storage_home_id)
    throw(CosPersistentState::NotFound,
          CORBA::SystemException)
{
    // TODO: Implementation
    CosPersistentState::StorageHomeBase_ptr _r = CosPersistentState::StorageHomeBase::_nil();
    return _r;
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/find_by_pid:1.0
//
CosPersistentState::StorageObjectBase_ptr
PSS::TransactionalDatabaseSession::find_by_pid(const CosPersistentState::Pid& the_pid)
    throw(CosPersistentState::NotFound,
          CORBA::SystemException)
{
    return NULL;
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/flush:1.0
//
void
PSS::TransactionalDatabaseSession::flush()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/refresh:1.0
//
void
PSS::TransactionalDatabaseSession::refresh()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/free_all:1.0
//
void
PSS::TransactionalDatabaseSession::free_all()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/close:1.0
//
void
PSS::TransactionalDatabaseSession::close()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/CosPersistentState/TransactionalSession/default_isolation_level:1.0
//
CosPersistentState::IsolationLevel
PSS::TransactionalDatabaseSession::default_isolation_level()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CosPersistentState::IsolationLevel _r = 0;
    return _r;
}

//
// IDL:omg.org/CosPersistentState/TransactionalSession/start:1.0
//
void
PSS::TransactionalDatabaseSession::start(CosTransactions::Coordinator_ptr transaction)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/CosPersistentState/TransactionalSession/suspend:1.0
//
void
PSS::TransactionalDatabaseSession::suspend(CosTransactions::Coordinator_ptr transaction)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/CosPersistentState/TransactionalSession/end:1.0
//
void
PSS::TransactionalDatabaseSession::end(CosTransactions::Coordinator_ptr transaction,
                                                   CORBA::Boolean success)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
}

//
// IDL:omg.org/CosPersistentState/TransactionalSession/get_association_status:1.0
//
CosPersistentState::TransactionalSession::AssociationStatus
PSS::TransactionalDatabaseSession::get_association_status()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CosPersistentState::TransactionalSession::AssociationStatus _r = 0;
    return _r;
}

//
// IDL:omg.org/CosPersistentState/TransactionalSession/transaction:1.0
//
CosTransactions::Coordinator_ptr
PSS::TransactionalDatabaseSession::transaction()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    CosTransactions::Coordinator_ptr _r = CosTransactions::Coordinator::_nil();
    return _r;
}

