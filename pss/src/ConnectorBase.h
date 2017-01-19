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

#ifndef _ConnectorBase_h_
#define _ConnectorBase_h_

#include <PSS.h>

#include <string>
#include <map>

using namespace std;

namespace PSS 
{

class ConnectorBase:	
		virtual public CosPersistentState::Connector,
		virtual public CORBA::LocalObject
{
	
	typedef map< string, CosPersistentState::StorageObjectFactory_ptr > ObjectFactoryMap;
	typedef map< string, CosPersistentState::StorageHomeFactory_ptr > HomeFactoryMap;
	
	ObjectFactoryMap objectFactories_;
	HomeFactoryMap homeFactories_;
	
	ObjectFactoryMap::iterator objectFactoryIterator_;
	HomeFactoryMap::iterator homeFactoryIterator_;

public:
	ConnectorBase();
	virtual char* implementation_id() = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/get_pid:1.0
	//
	virtual CosPersistentState::Pid* get_pid(CosPersistentState::StorageObjectBase_ptr obj) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/get_short_pid:1.0
	//
	virtual CosPersistentState::ShortPid* get_short_pid(CosPersistentState::StorageObjectBase_ptr obj) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/create_basic_session:1.0
	//
	virtual CosPersistentState::Session_ptr create_basic_session(CosPersistentState::AccessMode access_mode,
		const CosPersistentState::ParameterList& additional_parameters) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/create_transactional_session:1.0
	//
	virtual CosPersistentState::TransactionalSession_ptr create_transactional_session(CosPersistentState::AccessMode access_mode,
		CosPersistentState::IsolationLevel default_isolation_level,
		CosPersistentState::EndOfAssociationCallback_ptr callback,
		const CosPersistentState::ParameterList& additional_parameters) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/create_session_pool:1.0
	//
	virtual CosPersistentState::SessionPool_ptr create_session_pool(CosPersistentState::AccessMode access_mode,
		CosPersistentState::TransactionPolicy tx_policy,
		const CosPersistentState::ParameterList& additional_parameters) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/current_session:1.0
	//
	virtual CosPersistentState::TransactionalSession_ptr current_session() = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/sessions:1.0
	//
	virtual CosPersistentState::TransactionalSessionList* sessions(::CosTransactions::Coordinator_ptr transaction) = 0;

	//
	// IDL:omg.org/CosPersistentState/Connector/register_storage_object_factory:1.0
	//
	virtual CosPersistentState::StorageObjectFactory_ptr register_storage_object_factory(const char* storage_type_name,
		CosPersistentState::StorageObjectFactory* factory) ;

	//
	// IDL:omg.org/CosPersistentState/Connector/register_storage_home_factory:1.0
	//
	virtual CosPersistentState::StorageHomeFactory_ptr register_storage_home_factory(const char* storage_home_type_name,
		CosPersistentState::StorageHomeFactory* factory) ;
	
	CosPersistentState::StorageObjectFactory_ptr get_storage_object_factory(const char* storage_type_name);

	CosPersistentState::StorageHomeFactory_ptr get_storage_home_factory(const char* storage_home_name);

};


}
#endif /* #ifndef _ConnectorBase_h_ */

