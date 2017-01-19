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

#ifndef _TransactionalDatabaseSession_h_
#define _TransactionalDatabaseSession_h_

#include <PSS.h>
#include <DatabaseSession.h>

using namespace CosPersistentState;

namespace PSS
{
class TransactionalDatabaseSession:
		virtual public DatabaseSession,
		virtual public TransactionalSession
{
public:
	TransactionalDatabaseSession(CosPersistentState::AccessMode ,CosPersistentState::Connector_ptr );
	virtual ~TransactionalDatabaseSession();
	
	//
	// IDL:omg.org/CosPersistentState/CatalogBase/access_mode:1.0
	//
	virtual AccessMode access_mode() throw(CORBA::SystemException) ;

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/find_storage_home:1.0
	//
	virtual StorageHomeBase_ptr find_storage_home(const char* storage_home_id)
		throw(CosPersistentState::NotFound,CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/find_by_pid:1.0
	//
	virtual StorageObjectBase_ptr find_by_pid(const Pid& the_pid)
		throw(CosPersistentState::NotFound,CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/flush:1.0
	//
	virtual void flush()  throw(CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/refresh:1.0
	//
	virtual void refresh()  throw(CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/free_all:1.0
	//
	virtual void free_all() throw(CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/close:1.0
	//
	virtual void close() throw(CORBA::SystemException);


    //
    // IDL:omg.org/CosPersistentState/TransactionalSession/start:1.0
    //
    virtual void start(::CosTransactions::Coordinator_ptr transaction)  throw(CORBA::SystemException);

    //
    // IDL:omg.org/CosPersistentState/TransactionalSession/suspend:1.0
    //
    virtual void suspend(::CosTransactions::Coordinator_ptr transaction)  throw(CORBA::SystemException);

    //
    // IDL:omg.org/CosPersistentState/TransactionalSession/end:1.0
    //
    virtual void end(::CosTransactions::Coordinator_ptr transaction,
                     CORBA::Boolean success) throw(CORBA::SystemException);

    //
    // IDL:omg.org/CosPersistentState/TransactionalSession/get_association_status:1.0
    //
    virtual AssociationStatus get_association_status() throw(CORBA::SystemException);

    //
    // IDL:omg.org/CosPersistentState/TransactionalSession/transaction:1.0
    //
    virtual ::CosTransactions::Coordinator_ptr transaction() throw(CORBA::SystemException);

    //
    // IDL:omg.org/CosPersistentState/TransactionalSession/default_isolation_level:1.0
    //
    virtual IsolationLevel default_isolation_level()
        throw(CORBA::SystemException);

};

}


#endif /* #ifndef _TransactionalDatabaseSession_h_ */

