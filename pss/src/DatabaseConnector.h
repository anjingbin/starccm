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

#ifndef _DatabaseConnector_h_
#define _DatabaseConnector_h_

#include <PSS.h>

#include <ConnectorBase.h>

#include <string>
#include <map>

using namespace std;
using namespace CosPersistentState;

namespace PSS 
{

class DatabaseConnector: virtual public PSS::ConnectorBase
{
private:
	string id_;

public:
	DatabaseConnector();
	DatabaseConnector(const char* id);

	virtual char* implementation_id() throw(CORBA::SystemException);
	//
	// IDL:omg.org/CosPersistentState/Connector/get_pid:1.0
	//
	virtual CosPersistentState::Pid* 
		get_pid(CosPersistentState::StorageObjectBase* obj) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/get_short_pid:1.0
	//
	virtual CosPersistentState::ShortPid* 
		get_short_pid(CosPersistentState::StorageObjectBase* obj) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/create_basic_session:1.0
	//
	virtual CosPersistentState::Session_ptr 
		create_basic_session(CosPersistentState::AccessMode access_mode,
		const CosPersistentState::ParameterList& additional_parameters) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/create_transactional_session:1.0
	//
	virtual CosPersistentState::TransactionalSession_ptr 
		create_transactional_session(CosPersistentState::AccessMode access_mode,
									CosPersistentState::IsolationLevel default_isolation_level,
									CosPersistentState::EndOfAssociationCallback_ptr callback,
									const CosPersistentState::ParameterList& additional_parameters) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/create_session_pool:1.0
	//
	virtual CosPersistentState::SessionPool_ptr 
		create_session_pool(CosPersistentState::AccessMode access_mode,
						CosPersistentState::TransactionPolicy tx_policy,
						const CosPersistentState::ParameterList& additional_parameters) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/current_session:1.0
	//
	virtual CosPersistentState::TransactionalSession_ptr 
		current_session() = 0;
	
	//
	// IDL:omg.org/CosPersistentState/Connector/sessions:1.0
	//
	virtual CosPersistentState::TransactionalSessionList* 
		sessions(::CosTransactions::Coordinator_ptr transaction) = 0;


};


}
#endif /* #ifndef _DatabaseConnector_h_ */

