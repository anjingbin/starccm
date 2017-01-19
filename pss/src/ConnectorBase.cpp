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
#include <PSS.h>
#include <ConnectorBase.h>

PSS::ConnectorBase::ConnectorBase()
{
}


//
// IDL:omg.org/CosPersistentState/Connector/register_storage_object_factory:1.0
//
CosPersistentState::StorageObjectFactory_ptr 
PSS::ConnectorBase::register_storage_object_factory(
	const char* storage_type_name,
	CosPersistentState::StorageObjectFactory * factory) 
{
	objectFactories_[storage_type_name] = factory;
	return factory;
}

//
// IDL:omg.org/CosPersistentState/Connector/register_storage_home_factory:1.0
//
CosPersistentState::StorageHomeFactory_ptr 
PSS::ConnectorBase::register_storage_home_factory(
	const char* storage_home_type_name,
	CosPersistentState::StorageHomeFactory* factory) 
{
	homeFactories_[storage_home_type_name] = factory;
	return factory;
}

CosPersistentState::StorageObjectFactory_ptr
PSS::ConnectorBase::get_storage_object_factory(
	const char* storage_object_type_name)
{	
	CosPersistentState::StorageObjectFactory* _ret = NULL;

	objectFactoryIterator_ = objectFactories_.find(storage_object_type_name);
	if( objectFactoryIterator_ != objectFactories_.end())
	{
		_ret = objectFactories_[storage_object_type_name];
		_ret -> _add_ref();
	}

	return _ret;
}


CosPersistentState::StorageHomeFactory_ptr
PSS::ConnectorBase::get_storage_home_factory(
	const char* storage_home_type_name)
{	
	CosPersistentState::StorageHomeFactory* _ret = NULL;

	homeFactoryIterator_ = homeFactories_.find(storage_home_type_name);
	if( homeFactoryIterator_ != homeFactories_.end())
	{
		_ret = homeFactories_[storage_home_type_name];
		_ret -> _add_ref();
	}

	return _ret;
}

