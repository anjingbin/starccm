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


#include <Utility.h>

#include <assert.h>


PSS::StateItem::StateItem():
	name_(""),data_(NULL),len_(0)
{
}

PSS::StateItem::StateItem(const StateItem& item):
	name_(""),data_(NULL),len_(0)
{
	assert( item.data_ != NULL);
	assert( item.len_ > 0 );

	if(data_ != NULL)
		delete data_;
	data_ = new unsigned char[item.len_];
	memcpy( data_, item.data_, item.len_);
	len_ = item.len_;
	name_ = item.name_;
}


const unsigned char*
PSS::StateItem::data() const
{
	return data_;
}

void
PSS::StateItem::data( const unsigned char* data, int len)
{
	assert( data != NULL);
	assert( len > 0 );

	if( data_ != NULL)
		delete[] data_;

	data_ = new unsigned char[len];
	memcpy( data_, data, len);
	len_ = len;
}

void 
PSS::StateItem::name(const char* name)
{
	assert( name != NULL);
	name_ = name;
}

const char* 
PSS::StateItem::name()
{
	return name_.c_str();
}

int 
PSS::StateItem::length()
{
	return len_;
}

