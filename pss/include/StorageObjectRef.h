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

#ifndef _StorageObjectRef_h_
#define _StorageObjectRef_h_

#include <CosPersistentState.h>

namespace CosPersistentState
{

class StorageObjectRef
{
public:

	typedef StorageObject _target_type;

	StorageObjectRef(
		StorageObject* obj = 0
	) throw();

	StorageObjectRef(
		const StorageObjectRef& ref
	) throw();

	StorageObjectRef&
	operator=(
		const StorageObjectRef& ref
	) throw();

	StorageObjectRef&
	operator=(
		StorageObject* obj
	) throw();

	void
	release() throw();

	StorageObject*
	deref() throw (CORBA::SystemException);
	
	StorageObject*
	operator->() throw (CORBA::SystemException);

	// not const!
	void
	destroy_object() throw (CORBA::SystemException);

	Pid*
	get_pid() const throw (CORBA::SystemException);
	
	ShortPid*
	get_short_pid() const throw (CORBA::SystemException);
	
	CORBA::Boolean
	is_null() const throw();
	
	StorageHomeBase_ptr
	get_storage_home() const throw (CORBA::SystemException);

	static StorageObjectRef
	_duplicate(
		StorageObjectRef ref
	);

	static StorageObjectRef
	_downcast(
		StorageObjectRef ref
	);
};

} // end of namespace CosPersistentState

#endif /* #ifndef _StorageObjectRef_h_ */