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


#ifndef _PersistentObject_h_
#define _PersistentObject_h_

#include <CosPersistentState.h>
#include <Utility.h>

namespace PSS
{
	
class PersistentHome;
	
class PersistentObject : virtual public CosPersistentState::StorageObject
{
	PersistentObject(const PersistentObject&);
	void operator=(const PersistentObject&);
public:

    PersistentObject();
	virtual ~PersistentObject();

	void 
		set_modified( bool modified = true );

	bool
		is_modified();

	void
		set_home(PersistentHome*);

	void 
		set_pid( CosPersistentState::Pid& );

	void 
		set_short_pid( const CosPersistentState::ShortPid& );
	
    virtual void 
		_add_ref(); 
    
    virtual void 
		_remove_ref();

    virtual void 
		destroy_object() 
		throw(CORBA::SystemException);
    
    virtual CORBA::Boolean 
		object_exists() const 
		throw (CORBA::SystemException);
    
    virtual CosPersistentState::Pid* 
		get_pid() const 
		throw(CORBA::SystemException);
    
    virtual CosPersistentState::ShortPid* 
		get_short_pid() const 
		throw(CORBA::SystemException);
    
    virtual CosPersistentState::StorageHomeBase* 
		get_storage_home() const 
		throw(CORBA::SystemException);	
	
	virtual ObjectState*
		get_state() const =0;

	virtual void
		set_state( ObjectState*) =0;

	
private:
	
	PersistentHome* home_;
	CosPersistentState::ShortPid_var short_pid_;
	CosPersistentState::Pid_var	pid_;
	
	CORBA::Long refcount_;
	CORBA::Boolean destroyed_;	
	CORBA::Boolean modified_;

};

} // end of namespace PSS

#endif /* #ifndef _PersistentObject_h_ */	

