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

#ifndef _DatabaseCatalog_h_
#define _DatabaseCatalog_h_

#include <PSS.h>

#include <string>

using namespace std;
using namespace CosPersistentState;

namespace PSS 
{

class DatabaseCatalog : virtual public CatalogBase,
	virtual public CORBA::LocalObject
{
public:

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/access_mode:1.0
	//
	virtual AccessMode access_mode() = 0;

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/find_storage_home:1.0
	//
	virtual StorageHomeBase_ptr 
		find_storage_home(const char*) 
		throw( NotFound,CORBA::SystemException)	= 0;

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/find_by_pid:1.0
	//
	virtual StorageObjectBase_ptr find_by_pid(const Pid& pid) = 0;

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/flush:1.0
	//
	virtual void flush() = 0;

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/refresh:1.0
	//
	virtual void refresh() = 0;

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/free_all:1.0
	//
	virtual void free_all() = 0;

	//
	// IDL:omg.org/CosPersistentState/CatalogBase/close:1.0
	//
	virtual void close() = 0;

	virtual void 
		write_object_state(const PersistentHome* , const CosPersistentState::ShortPid& , ObjectState* )
		throw (CORBA::SystemException ) = 0;

	virtual ObjectState* 
		read_object_state(const PersistentHome*, const CosPersistentState::ShortPid&)
		throw (CORBA::SystemException) = 0;
	
	virtual CosPersistentState::StorageObject_ptr
		load_object(PersistentHome* , const CosPersistentState::ShortPid&)
		throw (CORBA::SystemException,
				CosPersistentState::NotFound) = 0;

	virtual bool 
		object_exists( const PersistentHome*, const CosPersistentState::ShortPid&) const
		throw (CORBA::SystemException) = 0;

	virtual void
		destroy_object( const PersistentHome*, const CosPersistentState::ShortPid&)
			throw (CORBA::SystemException) = 0;

	virtual CosPersistentState::ShortPid*
		find_by_key( const PersistentHome*, KeyState*) 
        throw(CosPersistentState::NotFound,
              CORBA::SystemException) = 0;

	virtual CosPersistentState::ShortPidSeq*
		find_all( const PersistentHome* )
		throw(CORBA::SystemException) = 0;
	
};

}


#endif /* #ifndef _DatabaseCatalog_h_ */
