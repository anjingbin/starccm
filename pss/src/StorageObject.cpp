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
#include <StorageObject.h>

//
// PSDL:CosPersistentState/StorageObject:1.0
//
CosPersistentState::StorageObject*
CosPersistentState::StorageObject::_duplicate(CosPersistentState::StorageObject* _obj)
{
	if( _obj != NULL)
		_obj -> _add_ref();

	return _obj;
}
CosPersistentState::StorageObject*
CosPersistentState::StorageObject::_downcast(CosPersistentState::StorageObject* _obj)
{
	return _obj;
}

//
// PSDL:CosPersistentState/StorageObjectRef:1.0
//

// Constructors
CosPersistentState::StorageObjectRef::StorageObjectRef()
{
    target_ = NULL;
}

CosPersistentState::StorageObjectRef::StorageObjectRef( StorageObject* target )
{
    target_ = NULL;
    if( target != NULL)
        target_ = target;
}

CosPersistentState::StorageObjectRef::StorageObjectRef( const StorageObjectRef& ref )
{
    target_ = ref.deref();
}

// Destructor
CosPersistentState::StorageObjectRef::~StorageObjectRef()
{
    if( target_ != NULL)
        target_ -> _remove_ref();
    target_ = NULL;
}

// Assignment operators
CosPersistentState::StorageObjectRef&
CosPersistentState::StorageObjectRef::operator=( const StorageObjectRef& ref )
{
    if( target_ != ref.target_ )
    {
        if( target_ != NULL)
            target_ -> _remove_ref();

        target_ = ref.target_;

        if( target_ != NULL)
            target_ -> _add_ref();
    }

    return *this;
}

CosPersistentState::StorageObjectRef&
CosPersistentState::StorageObjectRef::operator=( StorageObject* obj )
{
    if( target_ != NULL)
        target_ -> _remove_ref();

    target_ = obj;

    return *this;
}

// Other general methods
CosPersistentState::StorageObject*
CosPersistentState::StorageObjectRef::operator->() const throw (CORBA::SystemException)
{
    return target_;
}

CosPersistentState::StorageObject*
CosPersistentState::StorageObjectRef::deref() const throw (CORBA::SystemException)
{
    return target_;
}

void
CosPersistentState::StorageObjectRef::release()
{
    if( target_ != NULL)
        target_ -> _remove_ref();
    target_ = NULL;
}

void
CosPersistentState::StorageObjectRef::destroy_object() throw (CORBA::SystemException)
{
    if( target_ != NULL)
        target_ -> destroy_object();
}

CosPersistentState::Pid*
CosPersistentState::StorageObjectRef::get_pid() const throw (CORBA::SystemException)
{
    CosPersistentState::Pid* ret = NULL;

    if( target_ != NULL)
        ret  = target_ -> get_pid();

    return ret;
}

CosPersistentState::ShortPid*
CosPersistentState::StorageObjectRef::get_short_pid() const throw (CORBA::SystemException)
{
    CosPersistentState::ShortPid* ret = NULL;

    if( target_ != NULL)
        ret = target_ -> get_short_pid();

    return ret;
}

CORBA::Boolean
CosPersistentState::StorageObjectRef::is_null() const
{
    return (target_ == NULL);
}

CosPersistentState::StorageHomeBase*
CosPersistentState::StorageObjectRef::get_storage_home() const throw (CORBA::SystemException)
{
    CosPersistentState::StorageHomeBase* ret = NULL ;

    if( target_ != NULL)
        ret = target_ -> get_storage_home();

    return ret;
}

CosPersistentState::StorageObjectRef
CosPersistentState::StorageObjectRef::_duplicate( const StorageObjectRef ref )
{
    return StorageObjectRef(ref);
}


