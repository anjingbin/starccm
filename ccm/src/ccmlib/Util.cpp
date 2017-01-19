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

#include <Util.h>

int
Util::objectID2Int(PortableServer::ObjectId* oid)
{
	return 0;
}

PortableServer::ObjectId*
Util::int2ObjectID(int i)
{
	PortableServer::ObjectId_var oid = new PortableServer::ObjectId();
	
	return oid._retn();
}

