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

#ifndef _PostgreSQLConnector_h_
#define _PostgreSQLConnector_h_

#include <PSS.h>
#include <DatabaseConnector.h>

#include <string>
#include <map>

using namespace std;

namespace PSS 
{

class PostgreSQLConnector: virtual public PSS::DatabaseConnector
{
public:
	PostgreSQLConnector();

	virtual CosPersistentState::Pid* 
		get_pid(CosPersistentState::StorageObjectBase* )  throw(CORBA::SystemException);

	virtual CosPersistentState::ShortPid* 
		get_short_pid(CosPersistentState::StorageObjectBase* ) throw(CORBA::SystemException);

	virtual CosPersistentState::Session_ptr 
		create_basic_session(CosPersistentState::AccessMode ,
							const CosPersistentState::ParameterList& ) throw(CORBA::SystemException) ;

	virtual CosPersistentState::TransactionalSession_ptr 
		create_transactional_session(CosPersistentState::AccessMode ,
		CosPersistentState::IsolationLevel ,
									CosPersistentState::EndOfAssociationCallback_ptr ,
									const CosPersistentState::ParameterList& )  throw(CORBA::SystemException);

	virtual CosPersistentState::SessionPool_ptr 
		create_session_pool(CosPersistentState::AccessMode ,
							CosPersistentState::TransactionPolicy ,
							const CosPersistentState::ParameterList& )  throw(CORBA::SystemException);

	virtual CosPersistentState::TransactionalSession_ptr 
		current_session() throw(CORBA::SystemException) ;

	virtual CosPersistentState::TransactionalSessionList* 
		sessions(CosTransactions::Coordinator_ptr ) throw(CORBA::SystemException) ;

};


}
#endif /* #ifndef _PostgreSQLConnector_h_ */
