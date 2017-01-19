// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Dou Lei    fancydou@sina.com 
//
// **********************************************************************

// Version: 1.0.0

#include <CORBA.h>

#include <ServerActivator_impl.h>

#include <string>
#include <stdio.h>

#ifdef WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#include <fstream>
#include <stdlib.h>
#include <malloc.h>
//#include <Time.h>

//added by xiao heping 
#include <CommonFunc.h>
//end add

using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;


ServerActivator_impl::ServerActivator_impl(CORBA::ORB_ptr orb_,STARCCM::Deployment::DeployDomainMgr_ptr manager,const char * id,const char * dir,bool verbose_)
	:orb(CORBA::ORB::_duplicate(orb_)),verbose(verbose_)
{
	serverActivatorUUID=CORBA::string_dup(id);
	DomainMgr=STARCCM::Deployment::DeployDomainMgr::_duplicate(manager);
	rootDir=CORBA::string_dup(dir);
		
	if (verbose) {
		cout << "ServerActivator : ServerActivator of "
		     <<id
		     <<" is running." 
		     << endl;
	};	
}

ServerActivator_impl::~ServerActivator_impl ()
{
  if (verbose) {
    cout <<"ServerActivator: ServerActivator of "
         <<serverActivatorUUID
         <<" is terminated." << endl;
  };
}


ComponentServer_ptr 
ServerActivator_impl::create_component_server(const Components::ConfigValues& config)
        throw(Components::CreateFailure,
        	Components::Deployment::InvalidConfiguration,
              	CORBA::SystemException)
{
	
	if(verbose)
	{
		cout<<"ServerActivator:create component server"<<endl;	
	}
  		
	
	//
	//extract the UUID and the port of the ComponentServer from the config
	//
	
	CORBA::String_var port;
	CORBA::String_var componentServerUUID;
	
	TRACE0(LEVEL5,"ServerAcitvator: get the configvalue.\n");
	for(int i=0;i<config.length();i++)
	{
		const char *configValue;		
		config[(CORBA::ULong)i]->value()>>=configValue;
		
		CORBA::String_var value=CORBA::string_dup(configValue);
		
		if(strcmp(config[(CORBA::ULong)i]->name(),"UUID")==0)
		{			
			componentServerUUID=value;
			TRACE1(LEVEL6,"ServerActivator: the config value UUID is %s.\n",componentServerUUID);
		}
		if(strcmp(config[(CORBA::ULong)i]->name(),"Port")==0)
		{
			port=value;
			TRACE1(LEVEL6,"ServerActivator: the config value port is %s.\n",port);
		}		
	}
	
	if(componentServerUUID==NULL)
	{
		TRACE0(LEVEL1,"ServerActivator: create component server failed in invalid configuration.\n");
		throw Components::Deployment::InvalidConfiguration();
	}


	//
	//judge whether the ComponentServer has existed, if so just return it's ref	
	//
	
	MapServerActivator::iterator iter=serveractivatorMap.begin();
	MapServerActivator::iterator iter_end=serveractivatorMap.end();
	
	while(iter!=iter_end)
	{			
		CORBA::String_var id=(* iter).second.id;
		CORBA::String_var componentServerPort=(* iter).second.port;

/*		
		bool portEqual=false;
		if(port!=NULL)
		{
			if(strcmp(componentServerPort,port)==0)
			{
				portEqual=true;
			}
		}
		
		if((strcmp(id,componentServerUUID)==0)||portEqual)
*/
		if(strcmp(id,componentServerUUID)==0)
		{
			if(verbose)
			{
				cout<<"ServerActivator: the ComponentServer created has existed."<<endl;			
			}		
			

			//
			//get the ComponentServer object ref
			//
			CORBA::String_var componentServerRef=CORBA::string_dup((* iter).first.c_str());
			CORBA::Object_var obj;		
							
			try
			{
			       obj  = orb -> string_to_object(componentServerRef);
			}
			catch(const CORBA::BAD_PARAM&)
			{
			       	TRACE0(LEVEL1,"ServerActivator: invalid ComponentServer reference.\n");        	
			       	throw;
			}
				
			if(CORBA::is_nil(obj))
			{
				TRACE0(LEVEL1,"ServerActivator: cannot get the reference of ComponentServer.\n");    		
				throw;
			}
		
			ComponentServer_var componentServer=ComponentServer::_narrow(obj);
			assert(!CORBA::is_nil(componentServer)); 
			
			return componentServer._retn();
			
		}
		
		iter++;
	}

	
	//
	//if the ComponentServer does not exist, create a new one
	//
	TRACE0(LEVEL6,"ServerActivator: create a new component server.\n");
	
	if(port==NULL)
	{
		TRACE0(LEVEL1,"ServerActivator: create component server failed in invalid configuration.\n");
		throw Components::Deployment::InvalidConfiguration();
	}
	 
	CORBA::String_var manager = orb -> object_to_string(DomainMgr);    
	
	//CORBA::String_var cmdComponentServer=CORBA::string_dup("msdev ");
	CORBA::String_var cmdComponentServer=CORBA::string_dup("");

#ifdef WIN32
	CORBA::String_var  componentServerFileName = CORBA::string_dup("ComponentServer.exe");
#else
	CORBA::String_var  componentServerFileName = CORBA::string_dup("ComponentServer");
#endif
	
#ifdef STARCCMSTRINGADD
	cmdComponentServer = HelpFun::CORBA_string_add(cmdComponentServer, rootDir);	
	cmdComponentServer = HelpFun::CORBA_string_add(cmdComponentServer, PATH_DELILIMITOR);
	cmdComponentServer = HelpFun::CORBA_string_add(cmdComponentServer, CORBA::string_dup("ComponentServer.exe "));
#else	
	cmdComponentServer+=rootDir;		
	cmdComponentServer+=CORBA::string_dup(PATH_DELILIMITOR);
#ifdef StarBus
//modified by xiao heping 2004/06/11
	//cmdComponentServer+=CORBA::string_dup("ComponentServer.exe -ORBreactive");	
	cmdComponentServer+=componentServerFileName;
//end modify	
#endif

#ifdef ORBacus
	cmdComponentServer+=componentServerFileName;	
#endif

#endif	
	
	TRACE1(LEVEL6,"ServerActivator: start componentserver %s.\n",cmdComponentServer);

	string processCmd=string(cmdComponentServer.in());

#ifdef WIN32	
#ifdef TAO	
	processCmd=processCmd+" -ORBEndpoint iiop://:"+string(port.in())+" -DomainMgr "+string(manager.in())+" -fatherId "+string(serverActivatorUUID.in())+" -id "+string(componentServerUUID.in());
#else
	processCmd=processCmd+ " -OAport "+string(port.in())+" -DomainMgr "+string(manager.in())+" -fatherId "+string(serverActivatorUUID.in())+" -id "+string(componentServerUUID.in());
#endif	
	
	if(verbose)
	{
		processCmd=processCmd+" -verbose ";
	}
	
	int cmdLen=strlen(processCmd.c_str())+1;	
	char * cmd=(char *)malloc(cmdLen);	
	TRACE1(LEVEL6,"ServerActivator: the processcmd is %s.\n",processCmd.c_str());
	memcpy(cmd,processCmd.c_str(),cmdLen);

	TRACE1(LEVEL6,"ServerActivator: start ComponentServer cmd is %s.\n",cmd);
	
    	STARTUPINFO si;
    	PROCESS_INFORMATION pi;

    	ZeroMemory( &si, sizeof(si) );
    	
    	si.cb = sizeof(si);
    	si.lpDesktop="";
    	si.lpTitle="";
    	si.dwFlags=STARTF_USESHOWWINDOW;
    	si.wShowWindow=SW_SHOWDEFAULT;
     	
    	ZeroMemory( &pi, sizeof(pi) );

    	// Start the child process. 
    	if( !CreateProcess( NULL, // No module name (use command line). 
        	cmd, // Command line. 
        	NULL,             // Process handle not inheritable. 
        	NULL,             // Thread handle not inheritable. 
        	FALSE,            // Set handle inheritance to FALSE. 
        	CREATE_NEW_CONSOLE,                // No creation flags. 
        	NULL,             // Use parent's environment block. 
        	NULL,             // Use parent's starting directory. 
        	&si,              // Pointer to STARTUPINFO structure.
        	&pi )             // Pointer to PROCESS_INFORMATION structure.
    		) 
    	{        	
		TRACE0(LEVEL1,"ServerActivator: start ComponentServer process failed. \n");		
		free(cmd);
		throw  Components::CreateFailure();   	
    	}
    	free(cmd);
	
#else

	pid_t componentServerPid;

	if((componentServerPid = fork()) < 0 )
	{
		TRACE0(LEVEL1,"ServerActivator: start ComponentServer process failed. \n");
		throw  Components::CreateFailure();
	}

	if(componentServerPid == 0)
	{
		execl(cmdComponentServer.in(),componentServerFileName.in(),"-OAport",port.in(),"-DomainMgr",manager.in(),"-fatherId",serverActivatorUUID.in(),"-id",componentServerUUID.in(),0);
		
		TRACE0(LEVEL1,"ServerActivator: start ComponentServer process failed. \n");
		throw  Components::CreateFailure();			
	}
	
#endif
	//
	//construct the corbaloc of the ComponentServer
	//
	CORBA::String_var componentServerCorbaloc=CORBA::string_dup("corbaloc::");
	//jxh--0317construct the corbaloc of the ComponentManager
	CORBA::String_var componentManagerCorbaloc = CORBA::string_dup("");
	//add by jxh 04/06/21
	//construct the corbaloc of the transactionmanager
	CORBA::String_var transactionmanagerCorbaloc = CORBA::string_dup("");
	
	//jxh--end
#ifdef STARCCMSTRINGADD
	componentServerCorbaloc = HelpFun::CORBA_string_add(componentServerCorbaloc, serverActivatorUUID);
	componentServerCorbaloc = HelpFun::CORBA_string_add(componentServerCorbaloc, CORBA::string_dup(":"));
	componentServerCorbaloc = HelpFun::CORBA_string_add(componentServerCorbaloc, port);
	//jxh--04/03/17
	componentManagerCorbaloc = HelpFun::CORBA_string_add(componentManagerCorbaloc,componentServerCorbaloc);
	//jxh--end
	componentServerCorbaloc = HelpFun::CORBA_string_add(componentServerCorbaloc, CORBA::string_dup("/ComponentServer"));
	//jxh--04/03/17
	componentManagerCorbaloc = HelpFun::CORBA_string_add(componentManagerCorbaloc,CORBA::string_dup("/ComponentManager"));
	
	//jxh--04/06/21
	transactionmanagerCorbaloc = HelpFun::CORBA_string_add(transactionmanagerCorbaloc,componentServerCorbaloc);	
	transactionmanagerCorbaloc = HelpFun::CORBA_string_add(transactionmanagerCorbaloc,CORBA::string_dup("/TransactionManager"));
	//jxh--end	
#else	
	componentServerCorbaloc+=serverActivatorUUID;
	componentServerCorbaloc+=CORBA::string_dup(":");
	componentServerCorbaloc+=port;
	//jxh--04/03/17
	componentManagerCorbaloc+=componentServerCorbaloc;
	transactionmanagerCorbaloc+=componentServerCorbaloc;
	//jxh--end
	componentServerCorbaloc+=CORBA::string_dup("/ComponentServer");
	//jxh
	componentManagerCorbaloc+=CORBA::string_dup("/ComponentManager");
	transactionmanagerCorbaloc+=CORBA::string_dup("/TransactionManager");

#endif	
		

	//
	//judge whether the new created ComponentServer started correcly within timeout
	//
//modified by xiao heping 
//old code 

//	CORBA::Object_var componentServerObj;
//	try
//	{
//		componentServerObj=ObjectExist(componentServerCorbaloc.in());
//	}
//	catch(const JudgeObjectExistFailure &)
//	{
//		TRACE0(LEVEL1,"ServerActivator: judge whether created the component successfully failed.\n");
//		throw  Components::CreateFailure();
//	}
//	catch(const ObjectNotExist &)
//	{
//		TRACE0(LEVEL1,"ServerActivator: created the component failed.\n");
//		throw  Components::CreateFailure();		
//	}

//new code 
	CORBA::String_var timestr;
	try
	{
		timestr=DomainMgr->getObjectValue(TIMEOUT);
	}
	catch(...)
	{		
		TRACE0(LEVEL1,"ServerActivator: get TimeOut from the DeployDomainMgr failed. \n ");        			
		throw  Components::CreateFailure();	
	}
	int timeout=atoi(timestr.in());	
	if(!HelpFun::ObjectExist(orb,componentServerCorbaloc.in(),timeout))
	{
		cout<<"DeployDomainMgr started failed. "<<endl;		
		throw  Components::CreateFailure();		
	}
//end modify
	
	
	ComponentServer_var componentServer=ComponentServer::_narrow(orb -> string_to_object(componentServerCorbaloc.in()));
	assert(!CORBA::is_nil(componentServer)); 
	
	//
	//add newly created ComponentServer info into the map
	//
	CORBA::String_var componentServerRef = orb -> object_to_string(componentServer);
	
	ComponentServerInf info;
	info.id=componentServerUUID;
	info.port=port;
	serveractivatorMap.insert(MapServerActivator::value_type(string(componentServerRef.in()),info));	
	
	
	//
	//register the newly created ComponentServer to the DeployDomainMgr
	//

	try
	{
		DomainMgr->registerComponentServer(componentServerUUID.in(),serverActivatorUUID.in(),componentServerCorbaloc.in(),config);
	}
	catch(const CORBA::SystemException &ex)
	{
		cerr << ex << endl;
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in CORBA System Exception.\n");
		throw Components::CreateFailure();		
	}
	catch(const STARCCM::Deployment::ObjectIDExist &)
	{
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in ObjectIDExist.\n");
		throw Components::CreateFailure();		
	}
	catch(const STARCCM::Deployment::InfoMissed &)
	{
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in InfoMissed.\n");
		throw Components::CreateFailure();				
	}
	catch(const STARCCM::Deployment::XMLParserError &)
	{
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in XMLParserError.\n");
		throw Components::CreateFailure();				
	}	
	catch(...)
	{
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in other unknonw exception.\n");
		throw CORBA::UNKNOWN();
	}
	//jxh---04/03/17
	//register the  ComponentManager to the DeployDomainMgr
	//
	//ObjectType objType;
	//objType =  COMPONENTMANAGER;
	try
	{
		DomainMgr->registerObject(COMPONENTMANAGER,componentServerUUID.in(),componentManagerCorbaloc.in());
		
		DomainMgr->registerObject(TRANSACTIONMANAGER,componentServerUUID.in(),transactionmanagerCorbaloc.in());
	}
	catch(const CORBA::SystemException &ex)
	{
		cerr << ex << endl;
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in CORBA System Exception.\n");
		throw Components::CreateFailure();		
	}

	catch(const STARCCM::Deployment::InfoMissed &)
	{
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in InfoMissed.\n");
		throw Components::CreateFailure();				
	}
	catch(const STARCCM::Deployment::XMLParserError &)
	{
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in XMLParserError.\n");
		throw Components::CreateFailure();				
	}	
	catch(...)
	{
		TRACE0(LEVEL1,"ServerActivator: register the ComponentServer to the DeployDomainMgr Failed in other unknonw exception.\n");
		throw CORBA::UNKNOWN();
	}
	//jxh--end
	return componentServer._retn();
	
}              



void 
ServerActivator_impl::remove_component_server(Components::Deployment::ComponentServer_ptr server)
	throw(Components::RemoveFailure,  CORBA::SystemException)
{
	
	if(verbose)
	{
		cout<<"ServerActivator: remove componentserver "<<endl;			
	}

	//
	//call the ComponentServer to shutdown
	//
	ComponentServer_var compnentServer=ComponentServer::_duplicate(server);
	try
	{
		compnentServer->remove();
	}
	catch(...)
	{
		TRACE0(LEVEL1,"ServerActivator: remove the ComponentServer failed in shutdown it.\n");
	}
	
	
	//
	//locate the ComponentServer in the map
	//
	CORBA::String_var ref = orb -> object_to_string(compnentServer);
	
	MapServerActivator::iterator theIterator;
	
	theIterator=serveractivatorMap.find(ref.in());	
	
	if (theIterator != serveractivatorMap.end())
	{		
		serveractivatorMap.erase(theIterator);	
	}
	else
	{
		TRACE0(LEVEL1,"ServerActivator: remove ComponentServer failed in locate it failure.\n");
		throw Components::RemoveFailure();	
	}

}	      

ComponentServers* 
ServerActivator_impl::get_component_servers()
        throw(CORBA::SystemException)
{
	
	if(verbose)
	{
		cout<<"ServerActivator: get all the componentserver created by the ServerActivator "<<endl;	
	}
	
	
	ComponentServers_var componentServerList=new ComponentServers();
	CORBA::ULong len = componentServerList->length();
	
	MapServerActivator::iterator theIterator=serveractivatorMap.begin();
	
	for( ;theIterator!=serveractivatorMap.end();++theIterator)	
	{
		CORBA::String_var componentServerRef=CORBA::string_dup(theIterator->first.c_str());
		
		CORBA::Object_var obj;		
		
		try
		{
		       obj  = orb -> string_to_object(componentServerRef);
	        }
	        catch(const CORBA::BAD_PARAM&)
	        {
	        	TRACE0(LEVEL1,"ServerActivator: invalid ComponentServer reference.\n");        	
	        	throw;
	        }
		
	    	if(CORBA::is_nil(obj))
	    	{
	        	TRACE0(LEVEL1,"ServerActivator: cannot get the reference of ComponentServer.\n");    		
			throw;
	   	}
		

		ComponentServer_var componentServer=ComponentServer::_narrow(obj);
		assert(!CORBA::is_nil(componentServer)); 
		
		componentServerList->length(len + 1);            	
            	componentServerList[(CORBA::ULong)len]= componentServer.in();
            	len++;
	}	
	
	return componentServerList._retn();
}	       

void 
ServerActivator_impl::recover()
    	throw(Components::Deployment::RecoverFailure,
              	CORBA::SystemException)
{
	//left to be implemented
	if(verbose)
	{
		cout<<"ServerActivator: recover the ServerActivator. "<<endl;	
	}
}	       


//deleted by xiao heping 2004/05/27

//*****************************************************************************************
//judge the Object newly created exist or not whithin the TimeOut
//if the Object exist ,then return its reference
//if any error occured during the judge process ,throw JudgeObjectExistFailure
//if the Object not exist, throw ObjectNotExist
//*****************************************************************************************

//CORBA::Object_ptr
//ServerActivator_impl::ObjectExist(const char * corbaloc)
//	throw (JudgeObjectExistFailure,Components::Deployment::ObjectNotExist)
//{
//
//	//
//	//get the object ref of the Object
//	//
//	CORBA::Object_var obj;
//	try
//	{
//	       obj=orb->string_to_object(corbaloc);
//        }
//        catch(const CORBA::BAD_PARAM&)
//        {
//        	TRACE1(LEVEL1,"ServerActivator: object corbaloc %s invalid \n ",corbaloc);        	
//        	throw JudgeObjectExistFailure();
//        }
//	
//    	if(CORBA::is_nil(obj))
//    	{
//		TRACE0(LEVEL1,"ServerActivator: : cannot get the reference of object. \n ");        			
//		throw JudgeObjectExistFailure();
//   	}
//	
//	//
//	//get the TimeOut from the DeployDomainMgr
//	//
//	CORBA::String_var timestr;
//	try
//	{
//		timestr=DomainMgr->getObjectValue(TIMEOUT);
//	}
//	catch(...)
//	{		
//		TRACE0(LEVEL1,"ServerActivator: get TimeOut from the DeployDomainMgr failed. \n ");        			
//		throw JudgeObjectExistFailure();		
//	}
//	
//	int timeout=atoi(timestr.in());	
//	
//		
//	time_t ltime;
//	time( &ltime );
//	int timestart=ltime;
//	int timenow=ltime;
//	bool exist=false;
//	TRACE1(LEVEL6,"ServerActivator: the start time is %d .\n",timenow);	
//	
//    	while(((timenow-timestart)<timeout)&&(exist==false))
//    	{
//    		try
//    		{
//    			if(!(obj->_non_existent()))
//    			{
//    				exist=true;
//    			}
//    		}
//    		catch(const CORBA::TRANSIENT &)
//    		{
//    		}
//    		
//    		
//    		timenow=time( &ltime );
//    	}
//    	
//    	if(exist==false)
//    	{
//    		TRACE1(LEVEL6,"ServerActivator: now it is time %d .\n",timenow);
//    		TRACE0(LEVEL2,"ServerActivator: object dose not created correctly whithin timeout.\n");    		
//    		throw Components::Deployment::ObjectNotExist();
//    	}
//    
//
//    	
//    	return obj._retn();
//    	
//}

//end delete
