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


#ifndef _PersistentHome_h_
#define _PersistentHome_h_

#include <PSS.h>

#include <string>
#include <list>


using namespace std;

namespace PSS
{
	
class PersistentHome: virtual public CosPersistentState::StorageHomeBase,virtual public CORBA::LocalObject
{
private:

    PersistentHome(const PersistentHome&);
    void operator=(const PersistentHome&);

	typedef list< CosPersistentState::StorageObject_ptr > ObjectIncarnationList;

	ObjectIncarnationList objects_;
	ObjectIncarnationList::iterator objectIterator_;

	CosPersistentState::Connector_ptr connector_;
	CosPersistentState::CatalogBase_ptr  catalog_;

	string table_;

protected:
	string id_;
	string storage_type_id_;

public:

    PersistentHome();
	virtual ~PersistentHome();

    //
    // IDL:omg.org/CosPersistentState/StorageHomeBase/find_by_short_pid:1.0
    //
    virtual CosPersistentState::StorageObjectBase* 
		find_by_short_pid(const CosPersistentState::ShortPid& )
        throw(CosPersistentState::NotFound,
              CORBA::SystemException);
    //
    // IDL:omg.org/CosPersistentState/StorageHomeBase/find_all:1.0
    //
    virtual CosPersistentState::ShortPidSeq* find_all();

    //
    // IDL:omg.org/CosPersistentState/StorageHomeBase/get_catalog:1.0
    //
    virtual CosPersistentState::CatalogBase_ptr 
		get_catalog();

	void 
		set_connector(CosPersistentState::Connector* );

    	void 
		set_catalog(CosPersistentState::CatalogBase* );

	void 
		set_table(const char* );

	const char* 
		get_table() const;

	const char* 
		get_id() const;

	const char*
		get_storage_type_id() const;

	void
		refresh_object(PSS::PersistentObject* obj);

	void 
		flush_object(PSS::PersistentObject* obj);

    void
		free_all();
    
    void
		flush();
    
    void 
	    refresh();

	CosPersistentState::StorageObject_ptr
		find_object( KeyState* )
		throw ( CORBA::SystemException,
			    CosPersistentState::NotFound); 	

	CosPersistentState::StorageObject_ptr
		create_object() 
		throw ( CORBA::SystemException);
	
	CosPersistentState::StorageObject_ptr
		create_object(const CosPersistentState::ShortPid& )
		throw ( CORBA::SystemException);

	bool
		object_exists(const CosPersistentState::ShortPid& ) const 
		throw (CORBA::SystemException);

	void
		destroy_object(const CosPersistentState::ShortPid&)
		throw (CORBA::SystemException);

	//
	// implemented in generated cpp files by PSDL compiler
	//
	virtual HomeMembers*
		get_home_members() const = 0;

	virtual int
		get_state_count() const = 0;
};

} // end of namespace PSS

#endif /* #ifndef _PersistentHome_h_ */

