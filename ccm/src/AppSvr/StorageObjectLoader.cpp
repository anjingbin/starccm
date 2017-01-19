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

#include <StorageObjectLoader.h>
#include <ContainerRunTime.h>
#include <ContainerBase.h>

using namespace Container;

StorageObjectLoader::StorageObjectLoader(char * dllUrl,ContainerRunTime_ptr runtime)
	:Loader(dllUrl, runtime)
{
	pssInitializer_ = runtime_->getPssInitializer();
}

StorageObjectLoader::~StorageObjectLoader()
{
	
}

void
StorageObjectLoader::loadPSS()
{
    //load PSS dll and call its init function
    ContainerBase_var container = runtime_->getContainer();
    CORBA::ORB_var orb = container->getOrb();
    
    CORBA::Object_var crObj = orb -> resolve_initial_references("PSS");
    
    CosPersistentState::ConnectorRegistry_var connector_registry = CosPersistentState::ConnectorRegistry::_narrow(crObj);
    
	CORBA::String_var connectorId= pssInitializer_->getConnectorId();
    
    connector_ = connector_registry -> find_connector(connectorId);
			
    typedef int EntryPointFunction(CosPersistentState::Connector*); 
    
    try
	{
		if( hDll_ != NULL )
		{
			char* entry="register_persistent_factories";
			EntryPointFunction* pFunction;
			pFunction = (EntryPointFunction*)GETPROCADDRESS(hDll_,entry);
			
			if(pFunction != NULL)
			{
				//调用pss Dll的init初始化
				int returnValue=(* pFunction)(connector_);		
				TRACE1(LEVEL3,"pssDll Init reuturn value is : %d\n" ,returnValue);		
			}
			else
			{
				TRACE1(LEVEL3,"Can NOT get  entry point from home pss DLL : %s\n" ,entry );
			}
		}
		else
		{
			TRACE0(LEVEL3,"Can NOT load pss DLL\n");
		}			
	}
	catch(CORBA::Exception& ex)
	{
		TRACE1(LEVEL3,"Exception occured while loading pss : %s\n",ex._to_string());
	}    
}


//SuLiang add 2003.3.31
CosPersistentState::Session_ptr 
StorageObjectLoader::getSession()
{
		//get the session.
	short _mode = pssInitializer_ -> getAccessMode();
	CosPersistentState::ParameterList_var _paraList = pssInitializer_ -> getPatemeters();
	CosPersistentState::Session_var session = connector_ -> create_basic_session(_mode,_paraList);
	return session._retn();
}
