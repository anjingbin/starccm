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
#include <DatabaseSession.h>

//
// IDL:omg.org/CosPersistentState/CatalogBase:1.0
//
PSS::DatabaseSession::DatabaseSession()
{	
}

PSS::DatabaseSession::DatabaseSession(CosPersistentState::AccessMode access_mode,CosPersistentState::Connector_ptr connector):
	accessMode_(access_mode),connector_(dynamic_cast< DatabaseConnector* >(connector))
{
}

PSS::DatabaseSession::~DatabaseSession()
{
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/access_mode:1.0
//
CosPersistentState::AccessMode
PSS::DatabaseSession::access_mode()
{
    return accessMode_;
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/find_storage_home:1.0
//
CosPersistentState::StorageHomeBase_ptr
PSS::DatabaseSession::find_storage_home(const char* storage_home_id)
    throw(CosPersistentState::NotFound,
          CORBA::SystemException)
{
	CosPersistentState::StorageHomeBase_ptr _r = CosPersistentState::StorageHomeBase::_nil();

	for( homeIterator_ = homes_.begin(); homeIterator_ != homes_.end(); ++homeIterator_)
	{
		_r = (*homeIterator_);
		PersistentHome* phome = dynamic_cast< PersistentHome* >(_r);
		string id = phome -> get_id();

		if( id == storage_home_id)
			break;
	}

	if( homeIterator_ != homes_.end() )
	{
		_r = (*homeIterator_);
	}
	else
	{
		CosPersistentState::StorageHomeFactory_ptr factory = connector_ -> get_storage_home_factory( storage_home_id);

		if( factory == NULL)
			throw CosPersistentState::NotFound();

		_r = factory -> create();
		factory -> _remove_ref();
		
		PersistentHome* phome = dynamic_cast< PersistentHome* >(_r);

		assert( phome != NULL);

		phome -> set_catalog( this );
		phome -> set_connector( connector_);

		char * table_name = marshal_table_name( phome -> get_id());
		phome -> set_table( (const char*)table_name);

		if( !table_exists(table_name))
		{
			create_table(phome);
		}

		homes_.push_back( _r );

	}
    
    return _r;
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/find_by_pid:1.0
//
CosPersistentState::StorageObjectBase_ptr
PSS::DatabaseSession::find_by_pid(const CosPersistentState::Pid& pid)
    throw(CosPersistentState::NotFound,
          CORBA::SystemException)
{
	CosPersistentState::StorageObjectBase_ptr _ret = NULL;

	PID _pid(pid);
	const char* home_id = _pid.get_home_id();
	CosPersistentState::ShortPid_var short_pid = _pid.get_short_pid();

	CosPersistentState::StorageHomeBase_ptr home = find_storage_home( home_id );

	_ret = home -> find_by_short_pid( CosPersistentState::ShortPid( short_pid ) );
		
    return _ret;
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/flush:1.0
//
void
PSS::DatabaseSession::flush()
    throw(CORBA::SystemException)
{
	for( homeIterator_ = homes_.begin(); homeIterator_ != homes_.end(); ++homeIterator_)
	{
		CosPersistentState::StorageHomeBase_ptr home = (*homeIterator_);
		PersistentHome* phome = dynamic_cast< PersistentHome* >( home);

		assert( phome != NULL);

		phome -> flush();
	}
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/refresh:1.0
//
void
PSS::DatabaseSession::refresh()
    throw(CORBA::SystemException)
{
	for( homeIterator_ = homes_.begin(); homeIterator_ != homes_.end(); ++homeIterator_)
	{
		CosPersistentState::StorageHomeBase_ptr home = (*homeIterator_);
		PersistentHome* phome = dynamic_cast< PersistentHome* >( home );

		assert( phome != NULL);

		phome -> refresh();
	}
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/free_all:1.0
//
void
PSS::DatabaseSession::free_all()
    throw(CORBA::SystemException)
{
	//TODO
}

//
// IDL:omg.org/CosPersistentState/CatalogBase/close:1.0
//
void
PSS::DatabaseSession::close()
    throw(CORBA::SystemException)
{
	flush();
}

CosPersistentState::StorageObject_ptr
PSS::DatabaseSession::load_object(PersistentHome* home, const CosPersistentState::ShortPid& short_pid)
		throw (CORBA::SystemException,
				CosPersistentState::NotFound)
{
	const char* table_name = home -> get_table();

	if( !table_exists( table_name ) )
	{
		PSSTRACE1("PSS::DatabaseSession::load_object: database table \"%s\" does not exist!", table_name);
		throw CORBA::PERSIST_STORE();
	}

	if( !object_exists( table_name, short_pid) )
		throw CosPersistentState::NotFound();

	CosPersistentState::StorageObject_ptr obj = home -> create_object(short_pid);
	PersistentObject* pobj = dynamic_cast< PersistentObject* >(obj);
	
	if( pobj == NULL)
	{
		PSSTRACE("PSS::DatabaseSession::load_object : create object failed,returned null pointer!");
		throw CORBA::PERSIST_STORE();
	}

	ObjectState* state = read_object_state( home, short_pid);

	pobj -> set_state( state);

	delete state;
	
	return obj;
}

bool 
PSS::DatabaseSession::object_exists( const PersistentHome* home, const CosPersistentState::ShortPid& short_pid) const
	throw (CORBA::SystemException)
{
	return object_exists( home -> get_table(), short_pid);
}

void
PSS::DatabaseSession::destroy_object( const PersistentHome* home, const CosPersistentState::ShortPid& short_pid)
	throw (CORBA::SystemException)
{
	destroy_object( home -> get_table(), short_pid);
}

CosPersistentState::ShortPid*
PSS::DatabaseSession::find_by_key( const PersistentHome* home , KeyState* key_state)
	throw(CosPersistentState::NotFound,
		CORBA::SystemException)
{
	return find_by_key( home -> get_table(),key_state);
}

CosPersistentState::ShortPidSeq*
PSS::DatabaseSession::find_all( const PersistentHome* home )
		throw(CORBA::SystemException)
{
	return find_all( home -> get_table());
}

