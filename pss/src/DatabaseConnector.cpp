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
#include <DatabaseConnector.h>

#include <string>
using namespace std;

PSS::DatabaseConnector::DatabaseConnector()
{
	id_ = "";
}

PSS::DatabaseConnector::DatabaseConnector(const char * id)
{
	id_ = id;
}

char *
PSS::DatabaseConnector::implementation_id() throw(CORBA::SystemException)
{
	char* ret = new char[id_.size()];
	return ret;
}

	

