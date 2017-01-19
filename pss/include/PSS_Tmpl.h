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


#ifndef _PSS_Tmpl_h_
#define _PSS_Tmpl_h_

namespace PSS
{
	
// StorageObjectFactory
	
template<class T>
class StorageObjectFactory : public CosPersistentState::StorageObjectFactory
{
public:
	
	StorageObjectFactory();
	
	virtual void
	_add_ref();
	
	virtual void
	_remove_ref();
	
	virtual CosPersistentState::StorageObject*
	create() throw(CORBA::SystemException);
	
	
	virtual
	~StorageObjectFactory();
	
private:
	CORBA::ULong refcount_;
};
	
// StorageHomeFactory

template<class T>
class StorageHomeFactory : public CosPersistentState::StorageHomeFactory
{
public:
	
	StorageHomeFactory();
	
	virtual void
	_add_ref();
	
	virtual void
	_remove_ref();
	
	virtual CosPersistentState::StorageHomeBase_ptr
	create() throw(CORBA::SystemException);
	
	
	virtual
	~StorageHomeFactory();
	
private:
	CORBA::ULong refcount_;
};
	

template<class T>
class StorageObjectVar
{
public:
	  
	inline
	StorageObjectVar();
	  
	inline virtual
	~StorageObjectVar();
	  
	inline
	StorageObjectVar(
		T* ptr							   // adopts
	);
	  
	inline
	StorageObjectVar(
		const StorageObjectVar<T>& var // copies
	);

	inline StorageObjectVar<T>&
	operator=(							  
		T* ptr							   // adopts
	); 
	  
	  
	inline StorageObjectVar<T>&
	operator=(							   
		const StorageObjectVar<T>& var // copies
	);
	  
	inline T*
	operator->() const;
	  
	inline T&
	operator*() const;
	  
	inline T*
	ptr() const;
	
	inline 
	operator T*();

	inline T*
	in() const;
	  
	inline T*&
	inout();
	  
	inline T*&
	out();
	  
	inline T*
	_retn();
	  
private:
	  
	T* ptr_;

};


} // end of namespace PSS

//
// include implementation file here
//

#include <PSS_Tmpl_i.h>
	
#endif	/* #ifndef _PSS_Tmpl_h_ */
