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

#include <PID.h>

#include <stdio.h>
#include "sysdep.h"
#include "uuid.h"

PSS::PID::PID(const CosPersistentState::Pid& pid)
{
	pid_ = (unsigned char*) new unsigned char[pid.length()];
	memcpy(	pid_, pid.get_buffer(),pid.length());
	len_ = pid.length();
}

PSS::PID::PID(const unsigned char* pid,int length)
{
	pid_ = (unsigned char*) new unsigned char[length];
	memcpy(	pid_, pid,length);
	len_ = length;
}	

PSS::PID::PID(const char* home_id, const CosPersistentState::ShortPid& short_pid)
{
	int len = strlen(home_id) + 1;
	len += short_pid.length();

	pid_ = new unsigned char[len];
	
	memcpy(pid_, home_id,strlen(home_id) +1);
	memcpy(pid_ + strlen(home_id) +1, short_pid.get_buffer(), short_pid.length());

	len_ = len;
}

PSS::PID::~PID()
{
	if( pid_ != NULL)
		delete[] pid_;
}

const char*
PSS::PID::get_home_id()
{
	return (char* )pid_;
}

CosPersistentState::ShortPid*
PSS::PID::get_short_pid()
{
	CosPersistentState::ShortPid_var _ret = new CosPersistentState::ShortPid;
	_ret -> length(sizeof(uuid_t) + sizeof(unsigned int));

	char* _home_id = (char*) pid_;

	unsigned char* _short_pid = pid_ + strlen(_home_id) +1;

	for( CORBA::ULong i =0; i < sizeof(uuid_t) + sizeof(unsigned int); i++)
		_ret[i] =  *(_short_pid +i);

	return  _ret._retn();
}

PSS::PID
PSS::PID::create_pid(const char* home_id)
{
	static unsigned int index = 0;

	PID * _ret = new PID;

	int len = strlen(home_id) +1 + sizeof(uuid_t) +sizeof(unsigned int);

	uuid_t u;
	uuid_create(&u);
	
/*
    int i;
	
    printf("%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", u.time_low, u.time_mid,
        u.time_hi_and_version, u.clock_seq_hi_and_reserved,
        u.clock_seq_low);
    for (i = 0; i < 6; i++)
        printf("%2.2x", u.node[i]);
	printf("--%8.8x",index);
    printf("\n");
*/

	_ret -> pid_ = new unsigned char[len];

	memcpy(_ret -> pid_, home_id,strlen(home_id) +1);
	memcpy(_ret -> pid_ + strlen(home_id) +1, &u, sizeof(uuid_t));
	memcpy(_ret -> pid_ + strlen(home_id) +1 + sizeof(uuid_t), &index, sizeof(unsigned int));

	_ret -> len_ = len;

	index++;
	return (*_ret);
}

bool 
PSS::PID::operator==(const PID& pid)
{
	assert( pid_ != NULL);
		
	assert( pid.pid_ != NULL);

	bool _ret = true;

	int len = len_;

	if( len > pid.len_ )
		len = pid.len_;

	int i;
	for( i = 0; i < len; i++)
	{
		if( pid_[i]	!= pid.pid_[i])
		{
			_ret = false;
			break;
		}
	}
			
	return _ret;
} 

CosPersistentState::Pid*
PSS::PID::get_pid()
{
	CosPersistentState::Pid_var _ret = new CosPersistentState::Pid;

	_ret -> length(len_);

	for( int i =0; i < len_; i++)
		_ret[i] =  *(pid_ + i);

	return  _ret._retn();	
}
