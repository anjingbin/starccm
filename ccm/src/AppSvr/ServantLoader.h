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

#ifndef __ServantLoader_h__
#define __ServantLoader_h__

#include <ServantLoader_fwd.h>


namespace Container
{

class ServantLoader : public virtual Loader
{

	
public:
	ServantLoader(const char * dllUrl,ContainerRunTime_ptr runtime);
	virtual ~ServantLoader();
	
	Components::CCMContext* loadContext(const char* entry);
	PortableServer::Servant loadHomeServant(const char* entry);
	
};	//End of class Loader

}	//End of namespace Container

#endif

