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

#ifndef _DatabaseSession_h_
#define _DatabaseSession_h_

#include <PSS.h>
#include <DatabaseCatalog.h>
#include <DatabaseConnector.h>

#include <string>
#include <list>

using namespace std;

namespace PSS
{

class DatabaseSession:
		virtual public PSS::DatabaseCatalog,
		virtual public CosPersistentState::Session
{
private:
	CosPersistentState::AccessMode accessMode_;
	
	typedef list< StorageHomeBase_ptr > HomeIncarnationList;

	HomeIncarnationList homes_;
	HomeIncarnationList::iterator homeIterator_;

	DatabaseConnector* connector_;

	DatabaseSession();

public:
	DatabaseSession(CosPersistentState::AccessMode access_mode,CosPersistentState::Connector_ptr connector);

	virtual ~DatabaseSession();

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/access_mode:1.0
	//
	virtual CosPersistentState::AccessMode 
		access_mode();

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/find_storage_home:1.0
	//
	virtual StorageHomeBase_ptr 
		find_storage_home(const char*) 
		throw( CosPersistentState::NotFound,CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/find_by_pid:1.0
	//
	virtual CosPersistentState::StorageObjectBase_ptr 
 		find_by_pid(const CosPersistentState::Pid& pid)
    		throw(CosPersistentState::NotFound,
          		CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/flush:1.0
	//
	virtual void flush() throw(CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/refresh:1.0
	//
	virtual void refresh() throw(CORBA::SystemException); 

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/free_all:1.0
	//
	virtual void free_all() throw(CORBA::SystemException);

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/close:1.0
	//
	virtual void close() throw(CORBA::SystemException);


	virtual CosPersistentState::StorageObject_ptr
		load_object(PersistentHome*, const CosPersistentState::ShortPid&)
		throw (CORBA::SystemException,
				CosPersistentState::NotFound);

public:
	virtual void 
		write_object_state(const PersistentHome* , const CosPersistentState::ShortPid& , ObjectState* ) 
		throw (CORBA::SystemException ) = 0;

	virtual ObjectState* 
		read_object_state(const PersistentHome*, const CosPersistentState::ShortPid&)
		throw (CORBA::SystemException) = 0;

	virtual char* 
		marshal_column_name( const char *) = 0;

	virtual char*
		marshal_table_name( const char *) = 0;

	virtual bool 
		table_exists( const char*) const = 0;

	virtual bool 
		object_exists( const char*, const CosPersistentState::ShortPid&) const 
		throw (CORBA::SystemException) = 0;

	virtual bool 
		object_exists( const PersistentHome*, const CosPersistentState::ShortPid&) const
		throw (CORBA::SystemException);

	virtual void 
		destroy_object( const char*, const CosPersistentState::ShortPid&) 
		throw (CORBA::SystemException) = 0;

	virtual void
		destroy_object( const PersistentHome*, const CosPersistentState::ShortPid&)
		throw (CORBA::SystemException);

	virtual CosPersistentState::ShortPid*
		find_by_key( const PersistentHome*, KeyState*)
		throw(CosPersistentState::NotFound,
			CORBA::SystemException);

	virtual CosPersistentState::ShortPid*
		find_by_key( const char*, KeyState*)
		throw(CosPersistentState::NotFound,
			CORBA::SystemException) = 0;

	virtual CosPersistentState::ShortPidSeq*
		find_all( const PersistentHome* )
		throw(CORBA::SystemException);

	virtual CosPersistentState::ShortPidSeq*
		find_all( const char * )
		throw(CORBA::SystemException) = 0;

	virtual void
		create_table(const PersistentHome*) 
		throw(CORBA::SystemException) = 0;

};

}

#endif /* #ifndef _DatabaseSession_h_ */

