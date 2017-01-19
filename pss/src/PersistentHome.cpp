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
#include <PersistentHome.h>
#include <DatabaseConnector.h>
#include <DatabaseCatalog.h>

PSS::PersistentHome::~PersistentHome()
{

}

PSS::PersistentHome::PersistentHome()
{
}

//
// IDL:omg.org/CosPersistentState/StorageHomeBase/find_by_short_pid:1.0
//
CosPersistentState::StorageObjectBase_ptr 
PSS::PersistentHome::find_by_short_pid(	const CosPersistentState::ShortPid& short_pid) 
	throw(CosPersistentState::NotFound,CORBA::SystemException)
{
	CosPersistentState::StorageObjectBase* _ret = NULL;
	
	// find in object incarnations
	for( objectIterator_ = objects_.begin(); objectIterator_ != objects_.end(); ++objectIterator_)
	{
		CosPersistentState::StorageObject_ptr obj = (*objectIterator_);
		CosPersistentState::ShortPid_var target_spid = obj -> get_short_pid();
		PID a("", *target_spid);
		PID b("", short_pid);
	
		if( a == b)
			break;
	}

	if( objectIterator_ != objects_.end())
	{
		// found
		_ret = dynamic_cast< CosPersistentState::StorageObjectBase* >( (*objectIterator_));
	}
	else
	{	
		//
		// NOT found,so load it from datastore.
		//
		//	 if storage object specified by the short_pid was not found in 
		// the datastore,then throw CosPersistentState::NotFound exception.
		//
		DatabaseCatalog* db_catalog = dynamic_cast< DatabaseCatalog* >(catalog_);

		CosPersistentState::StorageObject_ptr obj = db_catalog -> load_object( this, short_pid);
		
		PersistentObject* pobj = dynamic_cast< PersistentObject* >(obj);
		assert( pobj != NULL);



		_ret = dynamic_cast< CosPersistentState::StorageObjectBase* >(obj);
		objects_.push_back( obj );
	}
	
	return _ret;
}

//
// IDL:omg.org/CosPersistentState/StorageHomeBase/get_catalog:1.0
//
CosPersistentState::CatalogBase_ptr 
PSS::PersistentHome::get_catalog() 
{
	return CosPersistentState::CatalogBase::_duplicate(catalog_);
}

void
PSS::PersistentHome::flush()
{
	for( objectIterator_ = objects_.begin(); objectIterator_ != objects_.end(); objectIterator_++ )
	{
		CosPersistentState::StorageObject* _sobj = (*objectIterator_);
		PersistentObject* _obj = dynamic_cast< PersistentObject* >(_sobj);
		if( _obj -> is_modified() )
			flush_object( _obj );
	}
}

void 
PSS::PersistentHome::refresh()
{
	for( objectIterator_ = objects_.begin(); objectIterator_ != objects_.end(); objectIterator_++ )
	{
		CosPersistentState::StorageObject* _sobj = (*objectIterator_);
		PersistentObject* _obj = dynamic_cast< PersistentObject* >(_sobj);
		refresh_object( _obj );
	}
}

void 
PSS::PersistentHome::free_all()
{
	for( objectIterator_ = objects_.begin(); objectIterator_ != objects_.end(); objectIterator_++ )
	{
		CosPersistentState::StorageObject* _sobj = (*objectIterator_);
		PersistentObject* _obj = dynamic_cast< PersistentObject* >(_sobj);

		delete _obj;
		objects_.erase(objectIterator_);
	}
}

void
PSS::PersistentHome::refresh_object(PSS::PersistentObject* obj)
{
	ObjectState* state = NULL;

	assert(catalog_ != NULL);

	CosPersistentState::ShortPid_var spid = obj -> get_short_pid();

	DatabaseCatalog* db_catalog = dynamic_cast< DatabaseCatalog* >(catalog_);

	state = db_catalog -> read_object_state(this, CosPersistentState::ShortPid(spid));

	obj -> set_state(state);

	delete state;
}

void 
PSS::PersistentHome::flush_object(PSS::PersistentObject* obj)
{
	if( !obj -> is_modified())
		return;

	ObjectState* state = NULL;

	assert( catalog_ != NULL);

	state = obj -> get_state();

	CosPersistentState::ShortPid_var spid = obj -> get_short_pid();

	DatabaseCatalog* db_catalog = dynamic_cast< DatabaseCatalog* >(catalog_);

	db_catalog -> write_object_state(this, CosPersistentState::ShortPid(spid), state);

	delete state;
	obj -> set_modified( false);
}
	
void 
PSS::PersistentHome::set_connector(CosPersistentState::Connector* connector) 
{
	assert( connector != NULL);
	connector_ = CosPersistentState::Connector::_duplicate(connector);
}

void 
PSS::PersistentHome::set_catalog(CosPersistentState::CatalogBase* catalog)
{
	assert(catalog != NULL);
	catalog_ = CosPersistentState::CatalogBase::_duplicate(catalog);
}

void 
PSS::PersistentHome::set_table(const char* table)
{
	table_ = table;
}

const char* 
PSS::PersistentHome::get_table() const
{
	return table_.c_str();
}

CosPersistentState::StorageObject_ptr
PSS::PersistentHome::find_object( KeyState* state)
		throw ( CORBA::SystemException,
			    CosPersistentState::NotFound)
{
	assert( catalog_ != NULL);

	flush();

	DatabaseCatalog* db_catalog = dynamic_cast< DatabaseCatalog* >(catalog_);

	CosPersistentState::ShortPid_var spid = db_catalog -> find_by_key( this, state);

	CosPersistentState::StorageObjectBase* obj_base = find_by_short_pid( CosPersistentState::ShortPid(spid) );
	
	PersistentObject* pobj = dynamic_cast< PersistentObject* >(obj_base);

	assert( pobj != NULL);
	
	return pobj;
}

CosPersistentState::StorageObject_ptr
PSS::PersistentHome::create_object()
		throw ( CORBA::SystemException)
{
	assert( connector_ != NULL );

	const char* storage_type_id = get_storage_type_id();

	ConnectorBase* connector_base = dynamic_cast< ConnectorBase* >(connector_);

	CosPersistentState::StorageObjectFactory_ptr factory = connector_base -> get_storage_object_factory( storage_type_id );

	// if the target storage type factory not registered yet,then throw exception!
	if( factory == NULL)
	{
		PSSTRACE("PSS::PersistentHome::create_object : StorageObjectFactory not registered!");
		throw CORBA::PERSIST_STORE();
	}

	CosPersistentState::StorageObject_ptr obj = factory -> create();
	
	PersistentObject* pobj = dynamic_cast< PersistentObject* >(obj);
	
	assert( pobj != NULL);

	pobj -> set_home( this);
	
	PID pid = PID::create_pid( id_.c_str() );

	CosPersistentState::Pid_var _pid = pid.get_pid();
	pobj -> set_pid( _pid);

	CosPersistentState::ShortPid_var _spid = pid.get_short_pid();
	pobj -> set_short_pid( _spid );

	pobj -> set_modified(true);

	objects_.push_back( obj );

	return obj;
}

const char*
PSS::PersistentHome::get_storage_type_id() const
{
	return storage_type_id_.c_str();
}

CosPersistentState::StorageObject_ptr
PSS::PersistentHome::create_object(const CosPersistentState::ShortPid& short_pid )
	throw ( CORBA::SystemException)
{
	assert( connector_ != NULL );

	const char* storage_type_id = get_storage_type_id();

	ConnectorBase* connector_base = dynamic_cast< ConnectorBase* >(connector_);

	CosPersistentState::StorageObjectFactory_ptr factory = connector_base -> get_storage_object_factory( storage_type_id );

	if( factory == NULL)
	{
		PSSTRACE("PSS::PersistentHome::create_object : StorageObjectFactory not registered!");
		throw CORBA::PERSIST_STORE();
	}

	CosPersistentState::StorageObject_ptr obj = factory -> create();
	
	PersistentObject* pobj = dynamic_cast< PersistentObject* >(obj);
	
	assert( pobj != NULL);

	PID pid(get_id(), short_pid);

	CosPersistentState::Pid_var _pid = pid.get_pid();
	pobj -> set_pid( _pid);
	pobj -> set_short_pid( short_pid );
	pobj -> set_home(this);
	pobj -> set_modified(false);

	return obj;

}

bool
PSS::PersistentHome::object_exists(const CosPersistentState::ShortPid& short_pid) const 
	throw (CORBA::SystemException)
{
	DatabaseCatalog* db_catalog = dynamic_cast< DatabaseCatalog* >(catalog_);

	assert( db_catalog != NULL );
	
	return db_catalog -> object_exists( this, short_pid);
}

void
PSS::PersistentHome::destroy_object(const CosPersistentState::ShortPid& short_pid)  
		throw (CORBA::SystemException)
{
	DatabaseCatalog* db_catalog = dynamic_cast< DatabaseCatalog* >(catalog_);

	assert( db_catalog != NULL );

	db_catalog -> destroy_object( this, short_pid);
}

const char*
PSS::PersistentHome::get_id() const
{
	return id_.c_str();
}

CosPersistentState::ShortPidSeq* 
PSS::PersistentHome::find_all()
{
	assert( catalog_ != NULL);

	DatabaseCatalog* db_catalog = dynamic_cast< DatabaseCatalog* >(catalog_);

	CosPersistentState::ShortPidSeq_var _r = new CosPersistentState::ShortPidSeq;

	CORBA::ULong i = 0;
	for( objectIterator_ = objects_.begin(); objectIterator_ != objects_.end(); ++objectIterator_)
	{
		CosPersistentState::StorageObject_ptr obj = (*objectIterator_);
		CosPersistentState::ShortPid_var target_spid = obj -> get_short_pid();
		if( ! object_exists( CosPersistentState::ShortPid(target_spid)))
		{
			_r -> length(i+1);
			_r[i] = CosPersistentState::ShortPid(target_spid);
			i++;
		}
	}

	CosPersistentState::ShortPidSeq_var stored = db_catalog -> find_all( this );
	_r -> length( i + stored -> length());
	for( CORBA::ULong j = 0; j < stored -> length(); j++)
	{
		_r[i+j] = stored[j];
	}

	return _r._retn();
}

