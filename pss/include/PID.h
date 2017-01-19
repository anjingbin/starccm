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


#ifndef _PID_h_
#define _PID_h_

namespace PSS
{

class PID
{
private:

	unsigned char* pid_;
	int len_;
public:

	explicit PID(const CosPersistentState::Pid& pid);
	PID(const char* home_id, const CosPersistentState::ShortPid& short_pid);
	PID(const unsigned char* PID,int length);
	PID():pid_(NULL),len_(0){}

	~PID();

public:
	static PID 
		create_pid(const char* home_id);

	bool 
		operator==(const PID&);

	const char* 
		get_home_id();

	CosPersistentState::ShortPid* 
		get_short_pid();

	CosPersistentState::Pid*
		get_pid();

};

}

#endif /* #ifndef _PID_h_ */

