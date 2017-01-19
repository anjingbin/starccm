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

#ifndef _StorageObject_h_
#define _StorageObject_h_

#include <CosPersistentState.h>
#include <StorageObjectBase.h>

namespace CosPersistentState
{

class StorageObjectRef;
class StorageObject_var;
class StorageObject;
typedef StorageObject* StorageObject_ptr;


class StorageObject : public virtual StorageObjectBase 
{
  public:

    typedef StorageObjectRef  _ref_type;
    typedef StorageObject_var _var_type;

    virtual void 
    _add_ref() = 0;
    
    virtual void 
    _remove_ref() = 0;

    virtual void 
    destroy_object() throw(CORBA::SystemException) = 0;
    
    virtual CORBA::Boolean 
    object_exists() const throw (CORBA::SystemException) = 0;
    
    virtual Pid* 
    get_pid() const throw(CORBA::SystemException) = 0;
    
    virtual ShortPid* 
    get_short_pid() const throw(CORBA::SystemException) = 0;
    
    virtual StorageHomeBase* 
    get_storage_home() const throw(CORBA::SystemException) = 0;

    static StorageObject* 
    _duplicate(
		StorageObject* _obj	
    );

    static StorageObject*
    _downcast(
		StorageObject* _obj
    );

  protected:
      
    virtual 
    ~StorageObject() {}
};

class StorageObjectRef
{
private:
    StorageObject * target_;
public:

    typedef StorageObject _target_type;

    // Constructors
    StorageObjectRef(); 
    StorageObjectRef( StorageObject* target );
    StorageObjectRef( const StorageObjectRef& ref );

    // Destructor
	virtual ~StorageObjectRef();

    // Assignment operators
    StorageObjectRef& operator=( const StorageObjectRef& ref );
    StorageObjectRef& operator=( StorageObject* obj );

    //Other general member functions
    StorageObject* operator->() const throw (CORBA::SystemException);
    StorageObject* deref() const throw (CORBA::SystemException);
    void release();
    void destroy_object() throw (CORBA::SystemException);

    CosPersistentState::Pid*
    get_pid() const throw (CORBA::SystemException);

    CosPersistentState::ShortPid*
    get_short_pid() const throw (CORBA::SystemException);

    CORBA::Boolean is_null() const;

    CosPersistentState::StorageHomeBase_ptr
    get_storage_home() const throw (CORBA::SystemException);

    // Conversion operators

    // Static member functions
    static StorageObjectRef
    _duplicate( const StorageObjectRef ref );
};

} //end of namespace of CosPersistentState

#endif /* #ifndef _StorageObject_h_ */

