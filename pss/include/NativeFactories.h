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


#ifndef _NativeFactories_h_
#define _NativeFactories_h_

#include <CosPersistentState.h>
#include <StorageObject.h>

namespace CosPersistentState
{

class StorageObjectFactory
{
public:
	virtual CosPersistentState::StorageObject * create() 
		throw (CORBA::SystemException) =0;
	virtual void _add_ref() = 0;
	virtual void _remove_ref() = 0;
	virtual ~StorageObjectFactory() {}
};

class StorageHomeFactory
{
public:
	virtual CosPersistentState::StorageHomeBase * create()
		throw (CORBA::SystemException) = 0;
	virtual void _add_ref() = 0;
	virtual void _remove_ref() = 0;
	virtual ~StorageHomeFactory() {}
};

typedef StorageHomeFactory* StorageHomeFactory_ptr;
typedef StorageObjectFactory* StorageObjectFactory_ptr;
}

#endif /* #ifndef _NativeFactories_h_ */

