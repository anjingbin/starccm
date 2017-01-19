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

#ifndef _PSS_Tmpl_i_h_
#define _PSS_Tmpl_i_h_

namespace PSS
{

// Implementation of StorageObjectFactory

template<class T>
StorageObjectFactory<T>::StorageObjectFactory() :
	refcount_(1)
{
}

template<class T>
void
StorageObjectFactory<T>::_add_ref()

{
	refcount_++;
}

template<class T>
void
StorageObjectFactory<T>::_remove_ref()
{
	if (--refcount_ == 0)
	{
		delete this;
	}
}

template<class T>
CosPersistentState::StorageObject*
StorageObjectFactory<T>::create() 
	throw(CORBA::SystemException)
{
	return new T;
}

template<class T>  
StorageObjectFactory<T>::~StorageObjectFactory()
{

}

// Implementation of StorageHomeFactory

template<class T>
StorageHomeFactory<T>::StorageHomeFactory() :
	refcount_(1)
{}

template<class T>
void
StorageHomeFactory<T>::_add_ref()
{
	refcount_++;
}

template<class T>
void
StorageHomeFactory<T>::_remove_ref()
{
	if (--refcount_ == 0)
	{
		delete this;
	}
}

template<class T>
CosPersistentState::StorageHomeBase_ptr
StorageHomeFactory<T>::create() 
	throw(CORBA::SystemException)
{
	return new T;
}

template<class T>  
StorageHomeFactory<T>::~StorageHomeFactory()
{

}

//
// StorageObjectVar<T> inline function definitions 
//

template<class T>
StorageObjectVar<T>::StorageObjectVar() :
	ptr_(NULL)
{

}

template<class T>
StorageObjectVar<T>::~StorageObjectVar()
{
	if( ptr_ != NULL)
		ptr_ -> _remove_ref();
}

template<class T>
StorageObjectVar<T>::StorageObjectVar(
	T* ptr
	) :
	ptr_(ptr)
{

}

template<class T>
StorageObjectVar<T>&
StorageObjectVar<T>::operator=( 						   
	T* ptr
	)
{
	if( ptr_ != NULL)
		ptr_ -> _remove_ref();

	ptr_ = ptr;

	return *this;
}

template<class T>
StorageObjectVar<T>::StorageObjectVar(
	const StorageObjectVar<T>& var
	)
{
	ptr_ = var.ptr_;

	if( ptr_ != NULL)
			ptr_ -> _add_ref();
}


template<class T>
StorageObjectVar<T>&
StorageObjectVar<T>::operator=( 							
	const StorageObjectVar<T>& var
	)
{
	if( ptr_ != var.ptr_)
	{
		if( ptr_ != NULL)
			ptr_ -> _remove_ref();

		ptr_ = var.ptr_;

		if( ptr_ != NULL)
			ptr_ -> _add_ref();
	}

	return *this;
}

template<class T>
T*
StorageObjectVar<T>::operator->() const
{
	return ptr_;
}

template<class T>
T&
StorageObjectVar<T>::operator*() const
{
	assert(ptr_ != NULL);
	return *ptr_;
}

template<class T>
T*
StorageObjectVar<T>::in() const
{
	assert(ptr_ != 0);
	return ptr_;
}

template<class T>
T*
StorageObjectVar<T>::ptr() const
{
	return ptr_;
}

template<class T>
StorageObjectVar<T>::operator T*()
{
	return ptr_; 
}

template<class T>
T*&
StorageObjectVar<T>::inout()
{
	return ptr_;
}

template<class T>
T*&
StorageObjectVar<T>::out()
{
	return ptr_;
}

template<class T>
T*
StorageObjectVar<T>::_retn()
{
	T* ret = ptr_;
	ptr_ = NULL;

	return ret;
}



} // end of namespace PSS

#endif	/* #ifndef _PSS_Tmpl_i_h_ */
