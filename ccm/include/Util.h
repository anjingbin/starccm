// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Kebo    mep@263.net
// Author: Huang Jie    huangjie@email.com
// Author: Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#ifndef __Util_h__
#define __Util_h__

#include <CORBA.h>
#include <string>

using namespace std;

class Util
{
	public:
		Util(){};
		~Util(){};
		
		CCM_IMPORT static int objectID2Int(PortableServer::ObjectId* oid);
		CCM_IMPORT static PortableServer::ObjectId* int2ObjectID(int i);
};


#endif

