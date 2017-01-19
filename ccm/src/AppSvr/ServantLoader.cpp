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

#include <ServantLoader.h>
#include <Loader.h>
#include <ContainerRunTime.h>
#include <ContainerBase.h>


using namespace Container;

ServantLoader::ServantLoader(const char * dllUrl,ContainerRunTime_ptr runtime)
	:Loader(dllUrl, runtime)
{
	
}

ServantLoader::~ServantLoader()
{
	
}

Components::CCMContext*
ServantLoader::loadContext(const char* entry)
{
	Components::CCMContext* context = NULL;
	
	typedef Components::CCMContext* (*EntryPointFunction)(CIFHook_ptr cifHook);
	
	try
	{
		if( hDll_ != NULL )
		{
			EntryPointFunction pFunction;
			pFunction = (EntryPointFunction)GETPROCADDRESS(hDll_,entry);
			
			if(pFunction != NULL)
			{
				context = dynamic_cast<Components::CCMContext*>((*pFunction)(cifHook_));
			}
			else
			{
				TRACE1(LEVEL3,"Can NOT get entry point from home servant DLL : %s\n" , entry);
			}
		}
		else
		{
			TRACE0(LEVEL3,"Can NOT load home servant DLL\n");
		}
	}
	catch(CORBA::Exception& ex)
	{
#ifndef TAO
		TRACE1(LEVEL3,"Exception occured while loading Context : %s\n",ex._to_string());
#else
		TRACE0(LEVEL3,"Exception occured while loading Context\n");
#endif
	}
	
	return context;	
}

PortableServer::Servant
ServantLoader::loadHomeServant(const char* entry)
{
	PortableServer::Servant home = NULL;
	
	typedef PortableServer::Servant (*EntryPointFunction)(CIFHook_ptr cifHook);
	
	try
	{
		if( hDll_ != NULL )
		{
			EntryPointFunction pFunction;
			pFunction = (EntryPointFunction)GETPROCADDRESS(hDll_,entry);
			
			if(pFunction != NULL)
			{
				home = dynamic_cast<PortableServer::Servant>((*pFunction)(cifHook_));
			}
			else
			{
				TRACE1(LEVEL3,"Can NOT get entry point from home servant DLL : %s\n",entry);
			}
		}
		else
		{
			TRACE0(LEVEL3,"Can NOT load home servant DLL\n");
		}
	}
	catch(CORBA::Exception& ex)
	{
#ifndef TAO
		TRACE1(LEVEL3,"Exception occured while loading home servant : %s\n",ex._to_string());
#else
		TRACE0(LEVEL3,"Exception occured while loading home servant\n");
#endif
	}
	
	return home;
}

