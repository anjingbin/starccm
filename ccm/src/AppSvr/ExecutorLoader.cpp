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

#include <ExecutorLoader.h>
#include <ContainerBase.h>
#include <ContainerRunTime.h>

using namespace Container;

ExecutorLoader::ExecutorLoader(const char * dllUrl,ContainerRunTime_ptr runtime)
	:Loader(dllUrl, runtime)
{
	
}

ExecutorLoader::~ExecutorLoader()
{
	
}

Components::HomeExecutorBase*
ExecutorLoader::loadHomeExecutor(const char* entry)
{
	Components::HomeExecutorBase* homeExecutor = NULL;
	
	typedef Components::HomeExecutorBase* (*EntryPointFunction)();
	
	try
	{
		if( hDll_ != NULL )
		{
			EntryPointFunction pFunction;
			pFunction = (EntryPointFunction)GETPROCADDRESS(hDll_,entry);
			
			if(pFunction != NULL)
			{
				homeExecutor = dynamic_cast<Components::HomeExecutorBase*>((*pFunction)());
			}
			else
			{
				TRACE1(LEVEL3,"Can NOT get entry point from home executor DLL : %s\n" , entry);
			}
		}
		else
		{
			TRACE0(LEVEL3,"Can NOT load home executor DLL\n");
		}
	}
	catch(CORBA::Exception& ex)
	{
#ifndef TAO
		TRACE1(LEVEL3,"Exception occured while loading home executor : %s\n",ex._to_string());
#else
		TRACE0(LEVEL3,"Exception occured while loading home executor\n");
#endif
	}
	
	return homeExecutor;		
}

CORBA::ValueFactoryBase*
ExecutorLoader::loadValueFactory(const char* entry,const char* repId)
{
	CORBA::ValueFactoryBase* factory = NULL;
	
	typedef CORBA::ValueFactoryBase* (*EntryPointFunction)();
	
	try
	{
		if( hDll_ != NULL )
		{
			EntryPointFunction pFunction;
			pFunction = (EntryPointFunction)GETPROCADDRESS(hDll_,entry);
			
			if(pFunction != NULL)
			{
				factory = dynamic_cast<CORBA::ValueFactoryBase*>((*pFunction)());
			}
			else
			{
				TRACE1(LEVEL3,"Can NOT get value factory entry point from home executor DLL : %s\n" , entry);
			}
			
			//register value factory
			CORBA::ORB_var orb = runtime_ ->getContainer() -> getOrb();
			
			orb -> register_value_factory(repId,factory);
			
		}
		else
		{
			TRACE0(LEVEL3,"Can NOT load home executor DLL\n");
		}
		
		
	}
	catch(CORBA::Exception& ex)
	{
#ifndef TAO
		TRACE1(LEVEL3,"Exception occured while loading value factory : %s\n",ex._to_string());
#else
		TRACE0(LEVEL3,"Exception occured while loading value factory\n");
#endif
	}
	
	return factory;		
}

