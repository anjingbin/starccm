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

#ifndef _Utility_h_
#define _Utility_h_

#include <string>
#include <list>
#include <vector>

using namespace std;
namespace PSS
{

class StateItem
{
	string name_;
	unsigned char * data_;
	int len_;
public:
	StateItem(const StateItem&);
	StateItem();
	virtual ~StateItem()
	{
		if(data_ != NULL)
			delete data_;
	}

	void data( const unsigned char*,int);
	const unsigned char* data() const;
	
	void name(const char*);
	const char* name();

	int length();
	unsigned char* _retn()
	{
		unsigned char* _r = data_;
		data_ = NULL;
		len_ = 0;
		return _r;
	}

};

typedef list< StateItem > ObjectState;
typedef list< ObjectState > ObjectStateList;
typedef list< StateItem > KeyState;
typedef vector< string > HomeMembers;
 

}
#endif /* #ifndef _Utility_h_ */

