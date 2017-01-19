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

#ifndef __Loader_h__
#define __Loader_h__

#include <Loader_fwd.h>
#include <ContainerBase_fwd.h>
#include <ContainerRunTime_fwd.h>
#include <CIFHook.h>

namespace Container
{

class Loader : REF_COUNT_LOCAL_OBJECT
{
	
protected:
	CORBA::String_var dllURL_;
	HANDLEOFDLL hDll_;
	
	//modified by xiao heping 2004/03/29
//	ContainerRunTime_var runtime_;
	ContainerRunTime_ptr runtime_;
	//end modify xiao heping 2004/03/29

	CIFHook_var cifHook_;
	
public:
	Loader(const char * dllUrl,ContainerRunTime_ptr runtime);
	virtual ~Loader();
	
	void etherealize();
	
//	Components loadImplementation();
//	int unloadImplementation();
	

};	//End of class Loader

}	//End of namespace Container

#endif

