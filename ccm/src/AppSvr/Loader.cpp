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

#include <Loader.h>
#include <ContainerBase.h>
#include <ContainerRunTime.h>

using namespace Container;

Loader::Loader(const char * dllUrl,ContainerRunTime_ptr runtime)
{
	//modified by xiao heping 2004/03/29
//	runtime_ = OBJDUPLICATE(ContainerRunTime_ptr, runtime);
	runtime_ = runtime;
	//end modified

	dllURL_ = CORBA::string_dup(dllUrl);
	
	TRACE1(LEVEL6,"DLL path is %s\n",dllURL_.in());

	hDll_ = LOADLIBRARY(dllURL_);

#ifndef WIN32
	if(hDll_ == NULL)
	{
		TRACE1(LEVEL6,"Load DLL path failed for  %s\n",dlerror());
	}
#endif
	
	cifHook_ = runtime_ -> getCifHook();
}

Loader::~Loader()
{
	etherealize();
}

void
Loader::etherealize()
{
	try
	{
		CLOSEDLL(hDll_);
	}
	catch(...)
	{
		TRACE0(LEVEL1,"Exception occured while unloading DLLs\n");
		throw;
	}
}

