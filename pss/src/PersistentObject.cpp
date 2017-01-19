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


#include <PSS.h>
#include <PersistentObject.h>

PSS::PersistentObject::PersistentObject():
	home_(NULL), refcount_(1), destroyed_(false), modified_(false)
{
	

}

PSS::PersistentObject::~PersistentObject()
{
}

void
PSS::PersistentObject::set_modified( bool modified)
{
	modified_ = modified;
}

bool
PSS::PersistentObject::is_modified()
{
	return modified_;
}

void
PSS::PersistentObject::_add_ref()
{
	refcount_ ++;
}

void
PSS::PersistentObject::_remove_ref()
{
	refcount_--;
}

CosPersistentState::Pid*
PSS::PersistentObject::get_pid() const
    throw(CORBA::SystemException)
{
	CosPersistentState::Pid_var pid = new CosPersistentState::Pid;
	pid -> length( pid_ -> length());

	for( CORBA::ULong i = 0; i < pid_ -> length(); i++)
	{
		pid[i] = pid_[i];
	}

	return pid._retn();
}

CosPersistentState::ShortPid*
PSS::PersistentObject::get_short_pid() const
    throw(CORBA::SystemException)
{
	CosPersistentState::ShortPid_var short_pid = new CosPersistentState::ShortPid;
	short_pid -> length( short_pid_ -> length());

	for( CORBA::ULong i = 0; i < short_pid_ -> length(); i++)
	{
		short_pid[i] = short_pid_[i];
	}

	return short_pid._retn();
}

CosPersistentState::StorageHomeBase*
PSS::PersistentObject::get_storage_home() const
    throw(CORBA::SystemException)
{
	CosPersistentState::StorageHomeBase_var home = home_;
	return home._retn();
}

void
PSS::PersistentObject::set_home(PersistentHome* home)
{
	home_ = home;
}

void 
PSS::PersistentObject::set_pid( CosPersistentState::Pid& pid)
{
	pid_ = new CosPersistentState::Pid;
	int len = pid.length();
	pid_ -> length(len);
	for( int i = 0; i < len; i++)
	{
		pid_[i] = pid[i];
	}
}

void 
PSS::PersistentObject::set_short_pid(const CosPersistentState::ShortPid& short_pid)
{
	short_pid_ = new CosPersistentState::ShortPid;
	int len = short_pid.length();
	short_pid_ -> length(len);
	for( int i = 0; i < len; i++)
	{
		short_pid_[i] = short_pid[i];
	}
}

void 
PSS::PersistentObject::destroy_object() 
	throw(CORBA::SystemException)
{
	assert( home_ != NULL);

	CosPersistentState::ShortPid_var short_pid = get_short_pid();

	home_ -> destroy_object( short_pid);

	destroyed_ = true;
}

CORBA::Boolean 
PSS::PersistentObject::object_exists() const 
	throw (CORBA::SystemException)
{
	assert( home_ != NULL);

	CosPersistentState::ShortPid_var short_pid = get_short_pid();

	return home_ -> object_exists( CosPersistentState::ShortPid( short_pid));
}
