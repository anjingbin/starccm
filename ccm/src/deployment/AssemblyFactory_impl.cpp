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

#include <AssemblyFactory_impl.h>
#include <CCM.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>

#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <malloc.h>
//#include <time.h>
#include <string.h>
#include <errno.h>

//added by xiao heping 
#include <CommonFunc.h>
//end add

#ifdef WIN32
#include <process.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#endif

using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;


AssemblyFactory_impl::AssemblyFactory_impl (CORBA::ORB_ptr orb_,DeployDomainMgr_ptr manager, const char* ipaddr, const char *directory,bool verbose_)
	:orb(CORBA::ORB::_duplicate(orb_)),verbose(verbose_)
{
	
	DomainMgr=DeployDomainMgr::_duplicate(manager);
	rootDir=directory;	
	this->IPAddr = CORBA::string_dup(ipaddr);
	
	if(verbose)
	{
		cout<<"AssemblyFactory: AssemblyFactory is running."<<endl;
	}
}

AssemblyFactory_impl::~AssemblyFactory_impl ()
{
	if(verbose)
		cout<<"AssemblyFactory: AssemblyFactory is stopped."<<endl;
}


//*******************************************************************************************
//create Assembly object according to the assembly_loc_ in the remote console host:
//1.download the assemlby package(.zip) to the rootdir\temp
//2.extract it 
//3.parse the cad,csd file,decide what will be needed by the assembly
//4.copy the files to the rootdir\assemblyUUID 
//  files including cad file ,componentproperties file, homeproperties file  
//5.copy the files to the rootdir\assemblyUUID\ComponentFileID
//  files including csd file, dependency\valuetypefactory dll file, descriptor\ccd file  
//6.invoke ComponentInstallation's install implementation
//  files including ccd file,dependency\valuetypefactory dll file,servantcode dll file,
//  code dll file, pss dll file, properties file(declared in the ccd file).
//7.parse the cad file to get the assemlbyport and get the timeout from the domainconf file.
//8.start the assembly process at the specified port with proper parameters
//9.judge whether the assembly object exist within the timeout,if true ok, otherwise exception
//10.register the assembly information into the asseblyFactoryMap
//11.register the assembly information into the DomainConf file.
//*******************************************************************************************
Components::Cookie* 
AssemblyFactory_impl::create(const char* assembly_loc_)
	throw(Components::Deployment::InvalidLocation,
	      Components::CreateFailure,
	      CORBA::SystemException)
{
	CORBA::Object_var obj;	
	
	if(verbose)
	{
		cout<<"AssemblyFactoroy:create new assembly of "<<assembly_loc_<<endl;
	}

	// add by wsf
	cType = CREATE;
	// end by wsf

	//
	//download the assembly package zip file from the console to the host 
	//and rename it as assembly.zip
	//rootdir\temp\assembly.zip
	//
    	
    	try
    	{
    		download(assembly_loc_);
    	}
    	catch(const CORBA::SystemException &)
    	{
    		throw;
    	}
    	catch(...)
    	{
    		TRACE1(LEVEL1,"AssemblyFactory: create assembly failed in download the assembly package from %s.\n ",assembly_loc_);
    		throw Components::Deployment::InvalidLocation();
    	}

	
	//
	//extract the assembly.zip and the component package zip file in it to rootDir\temp\assembly 
	//
	CORBA::String_var assemblyPkg=rootDir;

#ifdef STARCCMSTRINGADD
	assemblyPkg = HelpFun::CORBA_string_add(assemblyPkg, PATH_DELILIMITOR);
	assemblyPkg = HelpFun::CORBA_string_add(assemblyPkg, CORBA::string_dup("temp"));
	assemblyPkg = HelpFun::CORBA_string_add(assemblyPkg, PATH_DELILIMITOR);
	
#else	
	assemblyPkg+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyPkg+=CORBA::string_dup("temp");
	assemblyPkg+=CORBA::string_dup(PATH_DELILIMITOR);
#endif	

	CORBA::String_var assemblyDir=assemblyPkg;

#ifdef STARCCMSTRINGADD
	assemblyPkg = HelpFun::CORBA_string_add(assemblyPkg, CORBA::string_dup("assembly.zip"));
#else	
	assemblyPkg+=CORBA::string_dup("assembly.zip");
#endif	
	
	TRACE1(LEVEL6,"the assemblyPkg is %s \n",assemblyPkg.in());
	
	try
	{
		extractAssembly(assemblyPkg.in());
	}
	catch(...)
	{
		TRACE0(LEVEL1,"AssemblyFactroy: extract the assembly packge file assembly.zip failed\n");
		throw Components::CreateFailure();			
	}
	
	//
	//locate and read the csd file get the other information start  assembly  process
	//including: AssemblyUUID, port, timeout
	//
#ifdef STARCCMSTRINGADD
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, CORBA::string_dup("assembly"));
#else	
	assemblyDir+=CORBA::string_dup("assembly");
#endif	
	
	CORBA::String_var cadFilePath=assemblyDir;	
	
#ifdef STARCCMSTRINGADD
	cadFilePath = HelpFun::CORBA_string_add(cadFilePath, PATH_DELILIMITOR);
	cadFilePath = HelpFun::CORBA_string_add(cadFilePath, CORBA::string_dup("*.cad"));
#else		
	cadFilePath+=CORBA::string_dup(PATH_DELILIMITOR);
	cadFilePath+=CORBA::string_dup("*.cad");	
#endif		
	
	TRACE1(LEVEL6,"AssemblyFactory: the cadFilePath is %s \n",cadFilePath.in());
	
	CORBA::String_var findFileName=HelpFun::locateFile(cadFilePath.in());
	
	if(findFileName==NULL)
	{
		throw Components::CreateFailure();
	}
	
	//modified by wsf

	//
	//parse the cad file
	//
	try
	{
		parseCadFile(findFileName.in(),assemblyDir);
	}
	catch(...)
	{
		TRACE0(LEVEL1,"AssemblyFactory: parse the cad file failed.\n");
		throw  Components::CreateFailure();
	}

	//end by wsf

	//
	//start the assembly process at the specified port 
	//
	
	//
	//get the DeployDomainMgr reference of string
	//
	CORBA::String_var  DomainMgrRef= orb -> object_to_string(DomainMgr); 		
	 
    
    //Modified by wkb
    
    //modified by wsf 2004.6.25
    //fix the "blank in the path" problem
   
#ifdef WIN32
	// string msdev = "msdev \"";
	string msdev = "";
	CORBA::String_var cmdAssembly=CORBA::string_dup(msdev.c_str());      	
	CORBA::String_var  assemblyFileName = CORBA::string_dup("Assembly.exe");
#else
	string	msdev = "";
	CORBA::String_var cmdAssembly = CORBA::string_dup("");
	CORBA::String_var  assemblyFileName = CORBA::string_dup("Assembly");
#endif

	
#ifdef STARCCMSTRINGADD
	cmdAssembly = HelpFun::CORBA_string_add(cmdAssembly, rootDir);
	cmdAssembly = HelpFun::CORBA_string_add(cmdAssembly, PATH_DELILIMITOR);
	cmdAssembly = HelpFun::CORBA_string_add(cmdAssembly, assemblyFileName);
#else    
	cmdAssembly += rootDir;    
	cmdAssembly += CORBA::string_dup(PATH_DELILIMITOR);
	cmdAssembly += assemblyFileName;

        if (strcmp(msdev.c_str(),"msdev \"") ==0)
	{
		cmdAssembly+=CORBA::string_dup("\"");
	}	
	
	CORBA::String_var assemblyFilePath = cmdAssembly;	
	
#ifdef StarBus
	cmdAssembly+=CORBA::string_dup(" -ORBreactive");
#endif

#endif
	
	TRACE1(LEVEL6,"AssemblyFactory: start the assembly process of %s.\n",cmdAssembly.in());
	
#ifdef WIN32

	string dirarg = string(rootDir.in());
	dirarg = "\""+dirarg+"\"";
	string processCmd=string(cmdAssembly.in());

#ifdef TAO	
	processCmd=processCmd+" -ORBEndpoint iiop://:"+string(port.transcode())+" -DomainMgr "+string(DomainMgrRef.in())+" -dir "+dirarg+" -id "+string(assemblyUUID.transcode());
#else	
	processCmd=processCmd+" -OAport "+string(port.transcode())+" -DomainMgr "+string(DomainMgrRef.in())+" -dir "+dirarg+" -id "+string(assemblyUUID.transcode());
#endif	
	
	if(verbose)
	{
		processCmd=processCmd+" -verbose ";
	}
	
	int cmdLen=strlen(processCmd.c_str())+1;	
	char * cmd=(char *)malloc(cmdLen);	
	TRACE1(LEVEL6,"AssemblyFactory: the processcmd is %s.\n",processCmd.c_str());
	memcpy(cmd,processCmd.c_str(),cmdLen);

	TRACE1(LEVEL6,"AssemblyFactory: start Assembly cmd is %s.\n",cmd);

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
		TRACE0(LEVEL1,"AssemblyFactory: start Assembly process failed. \n");		
		free(cmd);
		throw  Components::CreateFailure();   	
    	}
    	free(cmd);	
#else
	pid_t pid;

	if((pid = fork()) < 0 )
	{
		TRACE0(LEVEL1,"AssemblyFactory: start Assembly process failed. \n");
		throw Components::CreateFailure();
	}

	if(pid == 0)
	{
		execl(assemblyFilePath.in(),assemblyFileName.in(),"-OAport",port.transcode(),"-DomainMgr",DomainMgrRef.in(),"-dir",rootDir.in(),"-id",assemblyUUID.transcode(),"-verbose",0);
		
                TRACE0(LEVEL1,"AssemblyFactory: start Assembly process failed. \n");
		throw Components::CreateFailure();	
	}
#endif	
	
	
	//
	//construct the corbaloc of the assembly
	//

//deleted by wsf 2004.7.6
////modified by xiao heping 2004/05/27
////	CORBA::String_var IPAddr=getHostIP();	
//	CORBA::String_var IPAddr=HelpFun::getHostIP();	
////end modify
//end by wsf 2004.7.6

	if(IPAddr==NULL)
	{
		throw Components::CreateFailure();
	}
	
	CORBA::String_var corbaloc=CORBA::string_dup("corbaloc::");

#ifdef STARCCMSTRINGADD
	corbaloc = HelpFun::CORBA_string_add(corbaloc, IPAddr);
	corbaloc = HelpFun::CORBA_string_add(corbaloc, CORBA::string_dup(":"));
	corbaloc = HelpFun::CORBA_string_add(corbaloc, CORBA::string_dup(port.transcode()));
	corbaloc = HelpFun::CORBA_string_add(corbaloc, CORBA::string_dup("/Assembly"));	
#else	
	corbaloc+=IPAddr;
	corbaloc+=CORBA::string_dup(":");
	corbaloc+=CORBA::string_dup(port.transcode());
	corbaloc+=CORBA::string_dup("/Assembly");
#endif
	
	TRACE1(LEVEL6,"AssemblyFactory: the assemlby corbaloc is %s.\n",corbaloc.in());
	
	//
	//test whether the process started correctly 
	//using is_existent test whethter the object run in service within timeout
	
//deleted by xiao heping 2004/05/27

//	try
//	{
//	       obj=orb->string_to_object(corbaloc.in());
//        }
//        catch(const CORBA::BAD_PARAM&)
//        {
//        	TRACE1(LEVEL1,"AssemblyFactory: assembly corbaloc %s invalid \n ",corbaloc);        	
//        	throw Components::CreateFailure();
//        }
//	
//    	if(CORBA::is_nil(obj))
//    	{
//		TRACE0(LEVEL1,"AssemblyFactory: : cannot get the reference of Assembly. \n ");        			
//		throw Components::CreateFailure();
//   	}
//end delete 	
   	//
   	//get the timeout
   	//
	CORBA::String_var timestr;
	try
	{
		timestr=DomainMgr->getObjectValue(TIMEOUT);
	}
	catch(...)
	{		
		TRACE0(LEVEL1,"AssemblyFactory: : get TimeOut from the DeployDomainMgr failed. \n ");        			
		throw Components::CreateFailure();		
	}
	int timeout=atoi(timestr.in());

//deleted by xiao heping 2004/05/27	
//	time_t ltime;
//	time( &ltime );
//	int timestart=ltime;
//	int timenow=ltime;
//	bool exist=false;
//	TRACE1(LEVEL6,"AssemblyFactory: the start time is %d .\n",timenow);	
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
//    	
//    	}
//    	
//    	if(exist==false)
//    	{
//    		TRACE1(LEVEL6,"AssemblyFactory: now it is time %d .\n",timenow);
//    		TRACE0(LEVEL2,"AssemblyFactory: assembly dose not created correctly whithin timeout.\n");    		
//    		throw Components::CreateFailure();
//    	}
//end delete 

//added by xiao heping 2004/05/27
    	if(!HelpFun::ObjectExist(orb,corbaloc.in(),timeout))
	{
		TRACE0(LEVEL1,"DeployDomainMgr started failed. \n");		
		throw Components::CreateFailure();	
	}
//end add
    
    
    	obj=orb->string_to_object(corbaloc.in());
    	Assembly_var assembly = Assembly::_narrow(obj.in());
    	assert(!CORBA::is_nil(assembly));
    	    	
    	//
    	//add  the assembly info into the map
    	//
    
    	assemblyFactoryMap.insert(MapAssemblyFactory::value_type(string(assemblyUUID.transcode()),assembly));
    	
			    	
    	//
    	//register the assembly to the DomainMgr
    	//
	
    	try
    	{
    		DomainMgr->registerAssembly(assemblyUUID.transcode(),corbaloc.in(),implIds.in());
    	}
    	catch(const CORBA::SystemException &)
    	{
    		TRACE0(LEVEL1,"AssemblyFactory; register the assmelby to the DomainConf file failed.\n");
    		throw;
    	}
    	catch(...)
    	{
    		TRACE0(LEVEL1,"AssemblyFactory; register the assmelby to the DomainConf file failed.\n");
    		throw Components::CreateFailure();
    	}
	
	TRACE1(LEVEL6,"AssemblyFactory: construct cookie for assembly of %s.\n",assemblyUUID.transcode());
       	Components::Cookie_var cookie = new Components::Cookie_impl(assemblyUUID.transcode());
       
       	
       	//
       	//delete the directory :rootDir\temp\assemlby
       	// 
       	assemblyDir=rootDir;
       	
#ifdef STARCCMSTRINGADD
	assemblyDir=HelpFun::CORBA_string_add(assemblyDir, PATH_DELILIMITOR);
	assemblyDir = HelpFun::CORBA_string_add(assemlbyDir, CORBA::string_dup("temp"));
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, PATH_DELILIMITOR);
	assemblyDir=HelpFun::CORBA_string_add(assemlbyDir, CORBA::string_dup("assembly"));
#else      	
	assemblyDir += CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir += CORBA::string_dup("temp");
 	assemblyDir += CORBA::string_dup(PATH_DELILIMITOR);
 	assemblyDir += CORBA::string_dup("assembly"); 
#endif       	
      	
       	TRACE1(LEVEL6,"AssemblyFactory: delete assmely dir is %s \n",assemblyDir.in());  

#ifdef WIN32	
	if(_rmdir(assemblyDir.in())==-1)
	{
		if(errno!=ENOTEMPTY)
		{
			TRACE1(LEVEL1,"AssemblyFactory: remove assembly %s error.\n",assemblyDir.in());			
			throw Components::RemoveFailure();
		}
//modified by xiao heping 2004/05/27
//		if(deleteDir(assemblyDir.in())==-1)
		if(HelpFun::deleteDir(assemblyDir.in())==-1)
//end modify
		{
			TRACE1(LEVEL1,"AssemblyFactory: delete assembly directory %s failed.\n",assemblyDir.in());			
			throw Components::RemoveFailure();
		}
		
		if(_rmdir(assemblyDir.in())==-1)
		{
			TRACE1(LEVEL1,"AssemblyFactory: delete assembly directory %s failed.\n",assemblyDir.in());				
			throw Components::RemoveFailure();
		}
	}	    
#else
#endif
        return cookie._retn();    	          
}

//wsf
Components::Cookie* 
AssemblyFactory_impl::dynCreate(const char* dyncreate_loc_)
	throw(Components::Deployment::InvalidLocation,
			              Components::CreateFailure,
			              CORBA::SystemException)
{

	if(verbose)
	{
		cout<<"AssemblyFactoroy: dynamic create for assembly "<<dyncreate_loc_<<endl;
	}
	cType = DYNCREATE;
	//
	//download the dynamic creation .zip file from the console to the host 
	//and rename it as assembly.zip
	//rootdir\temp\assembly.zip
	//	
    	try
    	{
    		download(dyncreate_loc_);
    	}
    	catch(const CORBA::SystemException &)
    	{
    		throw;
    	}
    	catch(...)
    	{
    		TRACE1(LEVEL1,"AssemblyFactoroy: create the dynamic creation .zip file failed in download the file from %s.\n ",dyncreate_loc_);
    		throw STARCCM::Deployment::CreateFailure();
    	}

	//
	//extract the assembly.zip and the component package zip file in it to rootDir\temp\assembly 
	//
	CORBA::String_var assemblyPkg=rootDir;
	assemblyPkg+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyPkg+=CORBA::string_dup("temp");
	assemblyPkg+=CORBA::string_dup(PATH_DELILIMITOR);
	CORBA::String_var assemblyDir=assemblyPkg;
	assemblyPkg+=CORBA::string_dup("assembly.zip");
	
	TRACE1(LEVEL6,"the assemblyPkg is %s \n",assemblyPkg);
	
	try
	{
		extractAssembly(assemblyPkg.in());
	}
	catch(...)
	{
		TRACE0(LEVEL1,"AssemblyFactroy: extract the assembly packge file assembly.zip failed\n");
		throw Components::CreateFailure();			
	}
	
	//
	//locate and read the csd file get the other information start  assembly  process
	//including: AssemblyUUID, port, timeout
	//
	assemblyDir+=CORBA::string_dup("assembly");
	CORBA::String_var dcdFilePath=assemblyDir;	
	dcdFilePath+=CORBA::string_dup(PATH_DELILIMITOR);
	dcdFilePath+=CORBA::string_dup("*.dcd");	
	
	TRACE1(LEVEL6,"AssemblyFactory: the dcdFilePath is %s \n",dcdFilePath);
	
	CORBA::String_var findFileName=HelpFun::locateFile(dcdFilePath.in());

	if(findFileName==NULL)
	{
		throw Components::CreateFailure();
	}

	//
	//parse the dcd file
	//
	try
	{
		parseCadFile(findFileName.in(),assemblyDir);
	}
	catch(...)
	{
		TRACE0(LEVEL1,"AssemblyFactory: parse the cad file failed.\n");
		throw  Components::CreateFailure();
	}
         	
       	//
       	//delete the directory :rootDir\temp\assemlby
       	// 
       	assemblyDir=rootDir;
       	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=CORBA::string_dup("temp");
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=CORBA::string_dup("assembly");
	
      	
       	TRACE1(LEVEL6,"AssemblyFactory: delete assmely dir is %s \n",assemblyDir.in());  

	TRACE1(LEVEL6,"AssemblyFactory: construct cookie for assembly of %s.\n",assemblyUUID.transcode());
       	Components::Cookie_var cookie = new Components::Cookie_impl(assemblyUUID.transcode());       

#ifdef WIN32	
	if(_rmdir(assemblyDir.in())==-1)
#else
	if(rmdir(assemblyDir.in()) == -1)
#endif
	{
		if(errno!=ENOTEMPTY)
		{
			TRACE1(LEVEL1,"AssemblyFactory: remove assembly %s error.\n",assemblyDir.in());			
			throw Components::RemoveFailure();
		}

//modified by xiao heping 2004/05/27
//		if(deleteDir(assemblyDir.in())==-1)
		if(HelpFun::deleteDir(assemblyDir.in())==-1)
//end modify
		{
			TRACE1(LEVEL1,"AssemblyFactory: delete assembly directory %s failed.\n",assemblyDir.in());			
			throw Components::RemoveFailure();
		}
#ifdef 	WIN32
		if(_rmdir(assemblyDir.in())==-1)
#else
		if(rmdir(assemblyDir.in()) == -1)
#endif
		{
			TRACE1(LEVEL1,"AssemblyFactory: delete assembly directory %s failed.\n",assemblyDir.in());				
			throw Components::RemoveFailure();
		}
	}	
	
	return cookie._retn(); 
}

Assembly_ptr 
AssemblyFactory_impl::lookup(Components::Cookie* cookie_)
	throw(Components::Deployment::InvalidAssembly,
	      CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"AssemblyFactory: in lookup ."<<endl;
	}
	
	Components::Cookie_impl* cookie = dynamic_cast<Components::Cookie_impl*>(cookie_);		
	CORBA::String_var assemblyUUID=cookie->toString();
	

	TRACE1(LEVEL6,"AssemblyFactory: lookup assembly of cookie %s.\n",assemblyUUID.in());

	if(verbose)
	{
		cout<<"AssemblyFactory:lookup cookie of "<<assemblyUUID<<endl;
	}
	
	//lookup in the map
	MapAssemblyFactory::iterator theIterator=assemblyFactoryMap.find(string(assemblyUUID.in()));
	
	if (theIterator != assemblyFactoryMap.end())
	{
		Assembly_var assembly=(*theIterator).second;
		return assembly._retn();
	}
	else
	{
		TRACE1(LEVEL1,"AssemblyFactory: can not find the assembly %s in the map. \n",assemblyUUID.in());
		
		throw Components::Deployment::InvalidAssembly();
	}

} 

//*************************************************************************************************
//destroy will destroy the assembly and all the information
//1.first check the state of the assembly if INSERVICE tear_down it.
//2.shutdown the assembly process
//3.if the implementation only belong to this assembly ,invoke the componentinstallation 's remove
//4.remove the rootdir\assemblyUUID directory
//5.delete the information from the assemblyFactoryMap
//**************************************************************************************************
   
void 
AssemblyFactory_impl::destroy(Components::Cookie* cookie_)
	throw(Components::Deployment::InvalidAssembly,
	      Components::RemoveFailure,
              CORBA::SystemException)
{
	
	Components::Cookie_impl* cookie = dynamic_cast<Components::Cookie_impl*>(cookie_);		
	CORBA::String_var assemblyUUID=cookie->toString();
	
	//
	//first check whether the assembly in inservice
	//if the assembly is inservice then teardown it first
	//else remove all the installed files of the assembly 
	//
	MapAssemblyFactory::iterator theIterator=assemblyFactoryMap.find(string(assemblyUUID.in()));	
	Assembly_var assembly;
	
	if (theIterator != assemblyFactoryMap.end())
	{
		assembly=(*theIterator).second;		
	}
	else
	{
		TRACE1(LEVEL1,"AssemblyFactory: destroy assembly %s failed in finding it.\n",assemblyUUID);
		
		throw Components::Deployment::InvalidAssembly();
	}
	
	
	if((assembly->get_state())==INSERVICE)
	{
		
		try
		{
			assembly->tear_down();
		}
		catch(...)
		{
			TRACE1(LEVEL1,"AssemblyFactory:destroy assembly %s failed in tear_down. \n",assemblyUUID);			
			throw Components::RemoveFailure();
		}
			
	}
	
	//
	//shut down the assembly process
	//
	try
	{
		assembly->shutdown();
	}
	catch(...)
	{
		TRACE0(LEVEL1,"AssemblyFactory: shutdown assembly failed.\n");
		throw Components::RemoveFailure();
	}
	
	
	//
	//get the componentinstallation ref to remove the implementation
	//
	CORBA::Object_var obj=getCommonObj("COMPONENTINSTALLATION");
	if(CORBA::is_nil(obj))
	{
		TRACE1(LEVEL1,"AssemblyFactroy: destroy assembly %s failed in getting ComponentInstallation obj.\n ",assemblyUUID);
		throw Components::RemoveFailure();
	}
	ComponentInstallation_var componentinstallation = ComponentInstallation::_narrow(obj);
    	assert(!CORBA::is_nil(componentinstallation));
    	
	
	
	//
	//if no assembly need the implementation , remove it
	//
	//
	
	//deleted by wsf
//	ObjIds_var implIds;
	//end by wsf
	
	try
	{
		implIds=DomainMgr->getObjectUUIDs(IMPLEMENTATION);
	}
	catch(const CORBA::SystemException &)
	{
		TRACE1(LEVEL1,"AssemblyFactroy: destroy assembly %s failed in getting implementation ids.\n ",assemblyUUID);
		throw;
	}
	catch(...)
	{
		TRACE1(LEVEL1,"AssemblyFactroy: destroy assembly %s failed in getting implementation ids.\n ",assemblyUUID);
		throw Components::RemoveFailure();		
	}
	
	CORBA::String_var implsource;
	CORBA::String_var impllocallocation;
	ObjIds_var assemblyIds;
	
	for(int i=0;i<implIds->length();i++)
	{
		
		try
		{
			DomainMgr->getImplInfo(implIds[(CORBA::ULong)i].in(),implsource,impllocallocation,assemblyIds);
		}
		catch(const CORBA::SystemException &)
		{
			TRACE2(LEVEL1,"AssemblyFactroy: destroy assembly %s failed in getting implementation %s infos.\n ",assemblyUUID,implIds[(CORBA::ULong)i]);
			throw;
		}
		catch(...)
		{
			TRACE2(LEVEL1,"AssemblyFactroy: destroy assembly %s failed in getting implementation %s infos.\n ",assemblyUUID,implIds[(CORBA::ULong)i]);
			throw Components::RemoveFailure();		
		}
		
		if(assemblyIds->length()==0)
		{
			try
			{
				componentinstallation->remove(implIds[(CORBA::ULong)i].in());
			}
			catch(const CORBA::SystemException &)
			{
				TRACE2(LEVEL1,"AssemblyFactroy: destroy assembly %s failed in removing implementation %s.\n ",assemblyUUID,implIds[(CORBA::ULong)i]);
				throw;
			}
			catch(...)
			{
				TRACE2(LEVEL1,"AssemblyFactroy: destroy assembly %s failed in removing implementation %s infos.\n ",assemblyUUID,implIds[(CORBA::ULong)i]);
				throw Components::RemoveFailure();
			}
		}
		
	}
			

	//
	//delete the assembly directory installed while create assembly
	//	
	CORBA::String_var dir=rootDir;
	
#ifdef STARCCMSTRINGADD
	dir = HelpFun::CORBA_string_add(dir,PATH_DELILIMITOR);
	dir = HelpFun::CORBA_string_add(dir,CORBA::string_dup(assemblyUUID.in()));
#else	
	dir+=CORBA::string_dup(PATH_DELILIMITOR);
	dir+=CORBA::string_dup(assemblyUUID.in());
#endif	
	
	TRACE1(LEVEL6,"AssemblyFactory: destory the assmelby directory %s \n",dir.in());

#ifdef WIN32
	if(_rmdir(dir.in())==-1)
#else
	if(rmdir(dir.in())== -1)
#endif
	{
		if(errno!=ENOTEMPTY)
		{
			TRACE1(LEVEL1,"AssemblyFactory: remove assembly %s error.\n",dir.in());			
			throw Components::RemoveFailure();
		}
//modified by xiao heping 2004/05/27
//		if(deleteDir(assemblyDir.in())==-1)
		if(HelpFun::deleteDir(dir.in())==-1)
//end modify
		{
			TRACE1(LEVEL1,"AssemblyFactory: delete assembly directory %s failed.\n",dir.in());			
			throw Components::RemoveFailure();
		}
#ifdef WIN32
		if(_rmdir(dir.in())==-1)
#else
		if(rmdir(dir.in()) == -1)
#endif
		{
			TRACE1(LEVEL1,"AssemblyFactory: delete assembly directory %s failed.\n",dir.in());				
			throw Components::RemoveFailure();
		}
	}	

	//
	//delete the information of this assembly from the map	at last 
	//
	assemblyFactoryMap.erase(theIterator);	
						

}           

//*******************************************************
//recover the assemblyfactory state while it crash
//*******************************************************
void 
AssemblyFactory_impl::recover()
        throw(Components::Deployment::RecoverFailure,
              CORBA::SystemException)
{
	//left to be implemented
	return;
	
}

//*******************************************************
//download the assembly package zip file from the console
//if fileaccessor method wrong ,throw FileSystemError
//other error ,throw FileDownloadFailure
//*******************************************************
void
AssemblyFactory_impl::download(const char * assembly_loc_)
	throw(FileDownloadFailure,
		STARCCM::Deployment::FileSystemError,
		CORBA::SystemException)
{

	//
	//first
	//get the reference of the fileaccessorfactory object on console from the DomainMgr
	//    	
 	
	CORBA::Object_var obj=getCommonObj("CONSOLEFILEACCESSORFACTORY");
	if(CORBA::is_nil(obj))
	{
		TRACE0(LEVEL1,"AssemblyFactory: can not get the fileaccesorfactory object of the console.\n");		
		throw FileDownloadFailure();
	}
	
	FileAccessorFactory_var ConsoleFileAccessorFactoryObj = FileAccessorFactory::_narrow(obj);
    	assert(!CORBA::is_nil(ConsoleFileAccessorFactoryObj)); 
    	
    	//
    	//create the fileaccessor object 
    	//
	FileAccessor_var ConsoleFileAccessorObj=ConsoleFileAccessorFactoryObj->createFileAccessor();
	assert(!CORBA::is_nil(ConsoleFileAccessorObj)); 
	
	
	//
	//download the assembly package from the console to the target directory
	//the target dir is rootDir\temp
	//
    	
    	CORBA::String_var targetDir=rootDir;

#ifdef STARCCMSTRINGADD
	targetDir = HelpFun::CORBA_string_add(targetDir, PATH_DELILIMITOR);
	targetDir = HelpFun::CORBA_string_add(targetDir, CORBA::string_dup("temp"));
#else    	
	targetDir+=CORBA::string_dup(PATH_DELILIMITOR);
	targetDir+=CORBA::string_dup("temp");
#endif
    	
    	FILE *fileWriteStream;    	
    	CORBA::String_var path=assembly_loc_; 	
    	
    	try
    	{
    		TRACE0(LEVEL5,"AssemblyFactory: Locate file.\n");    		
    		ConsoleFileAccessorObj -> locate_file(path.in()); 

		TRACE1(LEVEL5,"AssemblyFactory: Transfer the file from remote host to the target directory %s.\n",targetDir.in());
		
#ifdef WIN32
		if( _chdir(targetDir.in()))
#else
		if( chdir(targetDir.in()))
#endif
   		{
			TRACE1(LEVEL1,"AssemblyFactory: Unable to locate the directory %s.",targetDir.in());			
	   		throw FileDownloadFailure();
     
	   	}   	 	
   		
		if( (fileWriteStream = fopen( "assembly.zip", "wb" )) == NULL )
		{
			TRACE0(LEVEL1,"AssemblyFactory: Unable to create the assembly.zip file.\n");			
		  	throw FileDownloadFailure();
		}    	
    		
		int from_octet=0;
    		int max_octets=1024000;
    		int writeSize;	  
		FileOctetSeq_var writeBuffer;    	
		
    		do
    		{
    		  			  
    			writeBuffer=ConsoleFileAccessorObj->get_octet_seq(from_octet,max_octets);
    			writeSize=writeBuffer->length();
    		
    			if(fseek( fileWriteStream, from_octet , SEEK_SET)!=0)
    			{
    				TRACE0(LEVEL1,"AssemblyFactory: File seek error. \n");    				
    				throw FileDownloadFailure();
    			}
    		
			fwrite(writeBuffer->get_buffer(), 1, writeSize, fileWriteStream);
			
    			from_octet+=writeSize;
    		
    		}
    		while(writeSize==max_octets);
    	}
    	catch(const FileSystemError &e)
    	{
    		TRACE1(LEVEL1,"AssemblyFactory: file transer error : %s .\n",e.desc);    		
    		throw;
    	}
    	catch(const CORBA::SystemException &)
    	{    		
    		throw;
    	}
	catch(...)
	{
	   	TRACE0(LEVEL1,"AssemblyFactory: file transfer happen unknown exception \n");    		
	   	throw CORBA::UNKNOWN();	   		    	
	};    
	    
	fclose(fileWriteStream);	
	
}

//**********************************************************************************
//extractAssembly will extract the assembly zip file specified by the assemblyPkg 
//including the componentpkg zip file in the assembly zip file
//
//***********************************************************************************
void  
AssemblyFactory_impl::extractAssembly(const char *assemblyPkg)
	throw (ExtractAssemblyFailure)
{

	//first get the extract target directory ,it is the rootdir\temp\assembly

	CORBA::String_var assemblyDir=rootDir;
	
	//the infozip
	CORBA::String_var izipPath=rootDir;

#ifdef STARCCMSTRINGADD
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, PATH_DELILIMITOR);
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, CORBA::string_dup("temp"));
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, PATH_DELILIMITOR);
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, CORBA::string_dup("assembly"));
	
	izipPath = HelpFun::CORBA_string_add(izipPath, PATH_DELILIMITOR);
	izipPath = HelpFun::CORBA_string_add(izipPath, CORBA::string_dup("unzip.exe"));	
#else	
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=CORBA::string_dup("temp");
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=CORBA::string_dup("assembly");
		
	izipPath+=CORBA::string_dup(PATH_DELILIMITOR);

#ifdef WIN32	
	izipPath+=CORBA::string_dup("unzip.exe");
#else
	izipPath+=CORBA::string_dup("unzip");
#endif
	
#endif	

#ifdef WIN32
	string dirarg = string(assemblyDir.in());
	dirarg = "\""+dirarg+"\"";
	string pkgarg = assemblyPkg;
	pkgarg = "\""+pkgarg+"\"";
	
	int error=_spawnl( _P_WAIT,izipPath.in(),"izipPath",pkgarg.c_str(),"-d",dirarg.c_str(),NULL);
#else
	
	string unzipCmd = "unzip ";
	unzipCmd = unzipCmd + " " + assemblyPkg+ " -d " + assemblyDir.in();
	
	if(system(unzipCmd.c_str()) == -1)
	{
		TRACE0(LEVEL1,"AssemblyFactory: start unzip process failed. \n");
	}
	
#endif
	//end by wsf 2004.6.25
#ifdef STARCCMSTRINGADD
        assemblyDir = HelpFun::CORBA_string_add(assemblyDir, PATH_DELILIMITOR);
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, CORBA::string_dup("assembly"));		
#else	
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=CORBA::string_dup("assembly");
#endif		
	
	//delete the zip file
	
#ifdef WIN32	
	if(!DeleteFile(assemblyPkg))
#else
	if(unlink(assemblyPkg) == -1)
#endif		
	{
		TRACE1(LEVEL2,"AssemblyFactory: delete file %s failed.\n",assemblyPkg);
	}
}


//*******************************************************************
//getCommonObj get the object specified by objName through DomainMgr
//if any error occured return nil else return the correct reference
//*******************************************************************
CORBA::Object_ptr 
AssemblyFactory_impl::getCommonObj(const char * objName)
{
	
	CORBA::String_var objRef;
	
	if(strcmp(objName,"CONSOLEFILEACCESSORFACTORY")==0)
	{
	 	objRef=DomainMgr->getObject(CONSOLEFILEACCESSORFACTORY,"");    	
	 	
	}
	else if(strcmp(objName,"COMPONENTINSTALLATION")==0)
	{
		objRef=DomainMgr->getObject(COMPONENTINSTALLATION,"");    	
	}
	 
    	TRACE1(LEVEL6,"AssemblyFactory: the object corbaloc is %s .\n",objRef.in());    	
    		
    	CORBA::Object_var obj;
	try
	{
	       obj=orb->string_to_object(objRef.in());
        }
        catch(const CORBA::BAD_PARAM&)
        {
        	TRACE1(LEVEL1,"AssemblyFactoyr: invalid corbaloc %s.\n",objRef);        	
			return CORBA::Object::_nil();
        }
        

    	if(CORBA::is_nil(obj))
    	{
		TRACE0(LEVEL1,"AssemblyFactoyr: cannot get the reference of object.\n");        			
		return CORBA::Object::_nil();
   	}	
   	
   	return obj._retn();
	
}

//deleted by xiao heping 2004/05/27
//*********************************************************
//locateFile locate the file specified by the fileName,
//if any error occured return NULL string, 
//else return the correct fileName not the full path
//*********************************************************

//char *
//AssemblyFactory_impl::locateFile(const char *fileName)
//{
//	
//	TRACE1(LEVEL6,"the cadFilePath is %s \n",fileName);
//	
//#ifdef WIN32
//	
//	WIN32_FIND_DATA FindFileData;
//	HANDLE hFind;
//		
//	hFind = FindFirstFile(fileName, &FindFileData);
//	
//	if (hFind == INVALID_HANDLE_VALUE) 
//	{
//    		TRACE1(LEVEL2,"AssemblyFactory: can not find the file %s .\n",fileName);     		
//    		return NULL;
//  	} 
//  	
//  	CORBA::String_var findFileName=CORBA::string_dup(FindFileData.cFileName);  	
//  	
//    	FindClose(hFind);
//#endif	
//	
//	TRACE1(LEVEL6,"AssemblyFactory: the file found is %s .\n",findFileName);	
//	
//	return findFileName._retn();
//}
//end delete

//wsf
void 
AssemblyFactory_impl::parseCadFile(const char *cadFindName,const char *assemblyDir)
				throw(STARCCM::Deployment::XMLInitializeFailed,
					InvalidConfigureFile,
					FileDownloadFailure)
{
	CORBA::String_var cadFileName=assemblyDir;

#ifdef STARCCMSTRINGADD
	cadFileName = HelpFun::CORBA_string_add(cadFileName, PATH_DELILIMITOR);
	cadFileName = HelpFun::CORBA_string_add(cadFileName, cadFindName);
#else	
	cadFileName+=CORBA::string_dup(PATH_DELILIMITOR);
	cadFileName+=cadFindName;
#endif	
	
	TRACE1(LEVEL6,"AssemblyFactory: the cad file is %s .\n",cadFileName.in());	

	
	//
	//XMLParser initialilze
	//
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(cadFileName.in());
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE0(LEVEL1,"AssemblyFactory: XMLParser initialized failed. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::CreateFailure();
	}    
	
	DOM_Node root = parser->getRootNode("componentassembly");
	
	if(root==NULL)
	{
		TRACE0(LEVEL1,"AssemblyFactory: Invalid assembly descriptor file: have no componentassembly element. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::CreateFailure();
	}	
	
	//
	//get the assembly UUID
	//
	
	assemblyUUID=parser->getAttr(root,"id");
	if(assemblyUUID==NULL)
	{
		TRACE0(LEVEL2,"AssemblyFactory: invalid cad file \n");
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		throw Components::CreateFailure();
	}
	
	//
	//make target install directory
	//
	CORBA::String_var targetDir=rootDir;
	
#ifdef STARCCMSTRINGADD
	targetDir = HelpFun::CORBA_string_add(targetDir,PATH_DELILIMITOR);
	targetDir = HelpFun::CORBA_string_add(targetDir, CORBA::string_dup(assemblyUUID.transcode()));
#else	
	targetDir+=CORBA::string_dup(PATH_DELILIMITOR);
	targetDir+=CORBA::string_dup(assemblyUUID.transcode());
#endif	
	
	TRACE1(LEVEL6,"AssemblyFactory: the target directory of the assembly is %s \n",targetDir.in());
		
	//modified by wsf
	if (cType == CREATE)
	{
		
#ifdef WIN32
		int result = _mkdir(targetDir);
#else
		int result = mkdir(targetDir,0775);
#endif		
		if(result!=0)
		{
			TRACE1(LEVEL2,"AssemblyFactory: create the target directory %s of the assembly failed. \n",targetDir.in());
			throw Components::CreateFailure();
		}
	}
	//end by wsf

	CORBA::String_var targetFile=targetDir;

#ifdef STARCCMSTRINGADD
	targetFile = HelpFun::CORBA_string_add(targetDir,PATH_DELILIMITOR);
	targetFile = HelpFun::CORBA_string_add(targetFile, cadFindName);
#else	
	targetFile+=CORBA::string_dup(PATH_DELILIMITOR);
	targetFile+=cadFindName;	
#endif	

//modified by xiao heping 2004/05/27	
//	if(copyFile(cadFileName.in(),targetFile.in())==-1)
	if(HelpFun::copyFile(cadFileName.in(),targetFile.in())==-1)
//end modify
	{
		throw Components::CreateFailure();
	}


	//
	//read the cad file,install what the assembly need to the proper dir,including:
	//property file
	//componentfile:csd file,dependency:valuetypefactory,descriptor file
	//invoke the componentinstallation to install each component implementation
	//
	//read the cad file get the port it will start
	//
	
	DOM_Node portNode=parser->findChild(root,"objectport");
	if(portNode==NULL)
	{
		TRACE0(LEVEL2,"AssemblyFactory: invalid cad file: have no objectport element \n");
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		throw Components::CreateFailure();
	}
	
	port=parser->getAttr(portNode,"port");
	if(port==NULL)
	{
		TRACE0(LEVEL2,"AssemblyFactory: invalid cad file portnode element have no port attr. \n");
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		throw Components::CreateFailure();
	}
		
 	DOM_NodeList rootChildren=root.getChildNodes();
        unsigned int rootChildrenLen=rootChildren.getLength();	
	
	DOM_Node rootChild;
	DOMString rootChildName;
	implIds=new ObjIds;	
	CORBA::String_var sourceFile;
	
	for(unsigned int iterator=0;iterator<rootChildrenLen;iterator++)
	{
		rootChild=rootChildren.item(iterator);
		rootChildName=rootChild.getNodeName();	
		
		
		if(rootChildName.equals("componentfiles"))
		{			
 			DOM_NodeList componentfilesChildren=rootChild.getChildNodes();
        		unsigned int componentfilesChildrenLen=componentfilesChildren.getLength();
        		
        		for(int k=0;k<componentfilesChildrenLen;k++)
        		{
        			DOM_Node componentfilesChild=componentfilesChildren.item(k);
        			DOMString componentfilesChildName=componentfilesChild.getNodeName();

					sourceFile=assemblyDir;
					targetFile=targetDir;
				
#ifdef STARCCMSTRINGADD
					sourceFile = HelpFun::CORBA_string_add(sourceFile, PATH_DELILIMITOR);
					targetFile = HelpFun::CORBA_string_add(targetFile, PATH_DELILIMITOR);
#else				
					sourceFile+=CORBA::string_dup(PATH_DELILIMITOR);
					targetFile+=CORBA::string_dup(PATH_DELILIMITOR);
										
#endif				
        			
        			if(componentfilesChildName.equals("componentfile"))
        			{        				
        				DOMString id=parser->getAttr(componentfilesChild,"id");
					if(id==NULL)
					{
						TRACE0(LEVEL2,"AssemblyFactory: componentfiles have no id attr \n");
				
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}
				
						throw  Components::CreateFailure();		
					}				

#ifdef STARCCMSTRINGADD
					sourceFile = HelpFun::CORBA_string_add(sourceFile, CORBA::string_dup(id.transcode()));
					targetFile = HelpFun::CORBA_string_add(targetFile, CORBA::string_dup(id.transcode()));
#else									
					sourceFile+=CORBA::string_dup(id.transcode());
					targetFile+=CORBA::string_dup(id.transcode());
#endif					
					
					//
					//make the componentfile directory: rootDir\assemlbyUUID\Componentfileid
					//
					
					TRACE1(LEVEL6,"AssemblyFactory: the target file is %s.\n",targetFile.in());
					//modified by wsf
#ifdef WIN32
					int result = _mkdir(targetFile.in());
#else
					int result = mkdir(targetFile.in(),0775);
#endif
					if (cType == CREATE)
					{
						if(result!=0)
						{
							TRACE2(LEVEL2,"AssemblyFactory: create the componentfile %s directory %s of the assembly failed. \n",id.transcode(),targetDir);
							throw Components::CreateFailure();
						}
					}
					//end by wsf	
					
					DOMString name=parser->parseFileInArchive(componentfilesChild);
			
					if(name==NULL)
					{
						TRACE0(LEVEL1,"AssemblyFactory: componentfiles have invalid fileinarchive attr \n");
				
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}
						
						throw  Components::CreateFailure();
					}

#ifdef STARCCMSTRINGADD
					sourceFile = HelpFun::CORBA_string_add(sourceFile, PATH_DELILIMITOR);
					sourceFile = HelpFun::CORBA_string_add(sourceFile, CORBA::string_dup(name.transcode()));
					
					targetFile = HelpFun::CORBA_string_add(targetFile, PATH_DELILIMITOR)
					targetFile = HelpFun::CORBA_string_add(targetFile, CORBA::string_dup(name.transcode()));
#else					
					sourceFile+=CORBA::string_dup(PATH_DELILIMITOR);
					sourceFile+=CORBA::string_dup(name.transcode());
					
					targetFile+=CORBA::string_dup(PATH_DELILIMITOR);
					targetFile+=CORBA::string_dup(name.transcode());
#endif
					
					//
					//copy the csd file to the target directory
					//
					
					//modified by xiao heping 2004/05/27			
					//if(copyFile(sourceFile.in(),targetFile.in())==-1)
					if(HelpFun::copyFile(sourceFile.in(),targetFile.in())==-1)
					//end modify
					{
								
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}
							
						throw Components::CreateFailure();
					}
			
					//parse the csd file to copy the dependency and the descriptor
					CORBA::String_var csdFile=sourceFile;
					
					sourceFile=assemblyDir;
					targetFile=targetDir;
					
#ifdef STARCCMSTRINGADD
					targetFile = HelpFun::CORBA_string_add(targetFile, PATH_DELILIMITOR);
					targetFile = HelpFun::CORBA_string_add(targetFile, CORBA::string_dup(id.transcode()));
#else					
					targetFile+=CORBA::string_dup(PATH_DELILIMITOR);
					targetFile+=CORBA::string_dup(id.transcode());
#endif
										
					try
					{
						parseCsdFile(csdFile.in(),sourceFile.in(),targetFile.in());
					}
					catch(...)
					{
						TRACE0(LEVEL1,"AssemblyFactory: parse the csd file failed.\n");
						throw  Components::CreateFailure();
					}
					
        			}
        		
        		
        		}
	
		}
		else if (rootChildName.equals("partitioning"))
		{
			DOM_NodeList partitionChildren=rootChild.getChildNodes();
        		unsigned int partitionChildrenLen=partitionChildren.getLength();
        		
        		DOM_Node partitionChild;
        		DOMString partitionChildName;

				sourceFile=assemblyDir;
				targetFile=targetDir;

#ifdef STARCCMSTRINGADD
				sourceFile = HelpFun::CORBA_string_add(sourceFile, PATH_DELILIMITOR);
				targetFile = HelpFun::CORBA_string_add(targetFile, PATH_DELILIMITOR);
								
#else			
				sourceFile+=CORBA::string_dup(PATH_DELILIMITOR);
				targetFile+=CORBA::string_dup(PATH_DELILIMITOR);
								
#endif			
        		
        		for(unsigned int j=0;j<partitionChildrenLen;j++)
        		{
        			partitionChild=partitionChildren.item(j);				
				partitionChildName=partitionChild.getNodeName();
				
				if(partitionChildName.equals("homeplacement"))
				{
					
					try
					{
						parseHomePlacement(parser,root,partitionChild,sourceFile.in(),targetFile.in(),implIds.inout());
					}
					catch(const CORBA::SystemException &)
					{
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}
						
						throw;
					}
					catch(...)
					{
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}						
						throw Components::CreateFailure();
					}
					
				}
				else if(partitionChildName.equals("processcollocation"))
				{

					try
					{
						parseProcessCollocation(parser,root,partitionChild,sourceFile.in(),targetFile.in(),implIds.inout());
					}
					catch(const CORBA::SystemException &)
					{
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}
						
						throw;
					}
					catch(...)
					{
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}						
						throw Components::CreateFailure();
					}

				}
				else if(partitionChildName.equals("hostcollocation"))
				{
					try
					{
						parseHostCollocation(parser,root,partitionChild,sourceFile.in(),targetFile.in(),implIds.inout());
					}
					catch(const CORBA::SystemException &)
					{
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}
						
						throw;
					}
					catch(...)
					{
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}						
						throw Components::CreateFailure();
					}

				}
			}				
			
		}
		
	}//end of for
		
	//
	//free XMLParser resource
	//
	
	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}	

}

//***********************************************************
//parseCsdFile will use the XMLParser to parse the csd file 
//locate the valuetypefactory and csd file location 
//and copy these file to the target directory
//
//***********************************************************
void
AssemblyFactory_impl::parseCsdFile(const char *csdFileName,const char *sourceDir,const char *targetDir)
		throw(STARCCM::Deployment::XMLInitializeFailed,
			InvalidConfigureFile,
			FileDownloadFailure)			
			
{

	TRACE1(LEVEL6,"AssemblyFactory:parseCsdFile: the csd file name is %s. \n",csdFileName);
	
	//XMLParser initialized	 
	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(csdFileName);
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE0(LEVEL2,"AssemblyFactory: XMLParser to parser the csd file initialized fail.\n");				
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw;
	}    
	
	DOM_Node root = parser->getRootNode("softpkg");
	
	if(root==NULL)
	{
		TRACE0(LEVEL2,"AssemblyFactory: Invalid csd file: have no softpkg element.\n");	
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw InvalidConfigureFile();
	}	
	
	CORBA::String_var sourceLocation;
	CORBA::String_var targetLocation;
	
	DOM_Node dependencyNode=parser->findChild(root,"dependency");
	if(dependencyNode!=NULL)
	{
		DOM_Node valuefactoryNode=parser->findChild(dependencyNode,"valuetypefactory");
		if(valuefactoryNode!=NULL)			
		{
			DOMString location=parser->parseFileInArchive(valuefactoryNode);	
									
			if(location==NULL)
			{	
				throw InvalidConfigureFile();
			}					
			
			sourceLocation=sourceDir;
			targetLocation=targetDir;

#ifdef STARCCMSTRINGADD
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
			
			targetLocation = HelpFun::CORBA_string_add(targetLocation, PATH_DELILIMITOR);
			targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else			
			sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
			sourceLocation+=CORBA::string_dup(location.transcode());

			targetLocation+=CORBA::string_dup(PATH_DELILIMITOR);
			targetLocation+=CORBA::string_dup(location.transcode());
#endif			
			//modified by xiao heping 2004/05/27	
			//if(copyFile(sourceLocation.in(),targetLocation.in())==-1)
			if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
			//end modify
			{
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
					
				throw FileDownloadFailure();
			}	
		}
	}
	
	DOM_Node descriptorNode=parser->findChild(root,"descriptor");
	if(descriptorNode!=NULL)
	{
		DOMString location=parser->parseFileInArchive(descriptorNode);	
							
		if(location==NULL)
		{	
			throw InvalidConfigureFile();
		}		
			
		sourceLocation=sourceDir;
		targetLocation=targetDir;

#ifdef STARCCMSTRINGADD
		sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
		sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
		
		targetLocation = HelpFun::CORBA_string_add(targetLocation, PATH_DELILIMITOR);
		targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else		
		sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
		sourceLocation+=CORBA::string_dup(location.transcode());				
		
		targetLocation+=CORBA::string_dup(PATH_DELILIMITOR);
		targetLocation+=CORBA::string_dup(location.transcode());
#endif
		//modified by xiao heping 2004/05/27		
		//if(copyFile(sourceLocation.in(),targetLocation.in())==-1)
		if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
		//end modify
		{				
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}
					
			throw FileDownloadFailure();
		}	
		
	}
	
	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}
							
	
}

//*************************************************************************
//parserHomePlacement will parse the homeplacement element
//locate the properties file and the component implementation
//copy the properties file to the target dir
//invoke the ComponentInstallation install the implementation
//if any error occure return -1 ,else return 0
//*************************************************************************
void 
AssemblyFactory_impl::parseHomePlacement(XMLParser *parser,DOM_Node root,
					DOM_Node homeplacementNode,
					const char * sourceDir,
					const char * targetDir,
					ObjIds &implIds)
		throw (FileDownloadFailure,
			InvalidConfigureFile,
			ComponentInstallFailure,
			CORBA::SystemException)
{
	
	DOM_NodeList homeplacementChildren=homeplacementNode.getChildNodes();
        unsigned int homeplacementChildrenLen=homeplacementChildren.getLength();
	
	DOM_Node homeplacementChild;
	DOMString homeplacementChildName;	
	
	CORBA::String_var sourceLocation;
	CORBA::String_var targetLocation;
	
	for(unsigned int iterator=0;iterator<homeplacementChildrenLen;iterator++)
	{
		sourceLocation=sourceDir;
		targetLocation=targetDir;
		
		homeplacementChild=homeplacementChildren.item(iterator);
		homeplacementChildName=homeplacementChild.getNodeName();
		
		if(homeplacementChildName.equals("homeproperties"))
		{
			DOMString location=parser->parseFileInArchive(homeplacementChild);
			if(location==NULL)
			{		
				throw InvalidConfigureFile();
			}

#ifdef STARCCMSTRINGADD
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
			targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else						
			sourceLocation+=CORBA::string_dup(location.transcode());						
			targetLocation+=CORBA::string_dup(location.transcode());	
#endif			
			//modified by xiao heping 2004/05/27
			//if(copyFile(sourceLocation.in(),targetLocation.in())==-1)
			if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
			//end modify 
			{
				throw FileDownloadFailure();
			}
			
		}
		else if(homeplacementChildName.equals("componentproperties"))		
		{
			DOMString location=parser->parseFileInArchive(homeplacementChild);
			if(location==NULL)
			{
				throw InvalidConfigureFile();
			}

#ifdef STARCCMSTRINGADD
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
			targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else			
			sourceLocation+=CORBA::string_dup(location.transcode());			
			targetLocation+=CORBA::string_dup(location.transcode());	
#endif			
			//modified by xiao heping 2004/05/27
			//if(copyFile(sourceLocation.in(),targetLocation.in())==-1)
			if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
			//end modify 
			{								
				
				throw FileDownloadFailure();
			}						
		}
		else if(homeplacementChildName.equals("componentinstantiation"))
		{
			DOM_Node componentpropertiesNode=parser->findChild(homeplacementChild,"componentproperties");
			if(componentpropertiesNode!=NULL)
			{
				DOMString location=parser->parseFileInArchive(componentpropertiesNode);
				if(location==NULL)
				{
					throw InvalidConfigureFile();
				}

#ifdef STARCCMSTRINGADD
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
				targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else				
				sourceLocation+=CORBA::string_dup(location.transcode());
				targetLocation+=CORBA::string_dup(location.transcode());	
#endif				
				
				//modified by xiao heping 2004/05/27
				//if(copyFile(sourceLocation.in(),targetLocation.in())==-1)
				if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
				//end modify 
				{
					throw FileDownloadFailure();
				}			
			}	
				
		}
		else if (homeplacementChildName.equals("componentimplref"))
		{
			DOMString implUUID=parser->getAttr(homeplacementChild,"idref");
			if(implUUID==NULL)
			{
				TRACE0(LEVEL2,"AssemblyFactory: componentimpleref have no idref attr \n");
				throw InvalidConfigureFile();
			}
			
			//add the implUUID to the implIds 			
			CORBA::ULong implIdsLen=implIds.length();
			implIds.length(implIdsLen+1);
			implIds[(CORBA::ULong)implIdsLen]=CORBA::string_dup(implUUID.transcode());
						
			//invoke the ComponentInstallation to install the implementation
			DOM_Node componentfilerefNode=parser->findChild(homeplacementNode,"componentfileref");
			if(componentfilerefNode==NULL)
			{
				TRACE0(LEVEL2,"AssemblyFactory: componentfileref missed \n");
				throw InvalidConfigureFile();						
			}
			DOMString fileRef=parser->getAttr(componentfilerefNode,"idref");

#ifdef STARCCMSTRINGADD
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(fileRef.transcode()));
#else			
			sourceLocation+=CORBA::string_dup(fileRef.transcode());			
#endif			


			CORBA::Object_var obj=getCommonObj("COMPONENTINSTALLATION");
			if(CORBA::is_nil(obj))
			{
				TRACE0(LEVEL2,"AssemblyFactory: get the ComponentInstallation object failed\n");
				
				throw ComponentInstallFailure();
			}
			ComponentInstallation_var componentinstallation = ComponentInstallation::_narrow(obj);
    			assert(!CORBA::is_nil(componentinstallation));

			TRACE2(LEVEL6,"AssemblyFactory: the component %s location is %s.\n",implUUID.transcode(),sourceLocation.in());
			
			//add by wsf
			DOMString replace = parser -> getAttr(homeplacementChild,"replace");			
			//end by wsf
			
			try
			{
				componentinstallation->install(implUUID.transcode(),sourceLocation.in());
				
				//add by wsf
				if (replace != NULL)
				{
					componentinstallation->replace(implUUID.transcode(),sourceLocation.in());
				}
				//end by wsf
				
			}
			catch(const CORBA::SystemException &)
			{
				throw;
			}
			catch(...)
			{
				TRACE1(LEVEL2,"AssemblyFactory: invoke ComponentInstallation install the implementation %s failed. \n",implUUID.transcode());
				throw ComponentInstallFailure();
			}		
			
		}

	}//end of for
	
}

//************************************************************************
//parseProcessCollocation will parse the processcollocation element
//and use parseHomepLacement process the homeplacement elements
//
//************************************************************************
void 
AssemblyFactory_impl::parseProcessCollocation(XMLParser *parser,DOM_Node root,
						DOM_Node processcollocationNode,
						const char *sourceDir,
						const char *targetDir,
						ObjIds &implIds)
				throw (FileDownloadFailure,
					InvalidConfigureFile,
					ComponentInstallFailure,
					CORBA::SystemException)						
{
	DOM_NodeList processcollocationChildren=processcollocationNode.getChildNodes();
        unsigned int processcollocationChildrenLen=processcollocationChildren.getLength();
	
	DOM_Node processcollocationChild;
	DOMString processcollocationChildName;	
	
	for(unsigned int iterator=0;iterator<processcollocationChildrenLen;iterator++)
	{
	
		processcollocationChild=processcollocationChildren.item(iterator);
		processcollocationChildName=processcollocationChild.getNodeName();	
		
		if(processcollocationChildName.equals("homeplacement"))
		{
			try
			{
				
				parseHomePlacement(parser,root,processcollocationChild,sourceDir,targetDir,implIds);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"AssemblyFactory: parse process collocation failed.\n");
				throw;
			}
			
		}
	}

}

//*************************************************************************
//parseHostCollocation will parse the hostcollocation element
//and use parseProcessCollocation process the processcollocation elements
//and use parseHomepLacement process the homeplacement elements
//
//*************************************************************************
void 
AssemblyFactory_impl::parseHostCollocation(XMLParser *parser,DOM_Node root,
						DOM_Node hostcollocationNode,
						const char *sourceDir,
						const char *targetDir,
						ObjIds &implIds)
				throw (FileDownloadFailure,
					InvalidConfigureFile,
					ComponentInstallFailure,
					CORBA::SystemException)	
{
	
	DOM_NodeList hostcollocationChildren=hostcollocationNode.getChildNodes();
        unsigned int hostcollocationChildrenLen=hostcollocationChildren.getLength();
	
	DOM_Node hostcollocationChild;
	DOMString hostcollocationChildName;	
	
	for(unsigned int iterator=0;iterator<hostcollocationChildrenLen;iterator++)
	{
	
		hostcollocationChild=hostcollocationChildren.item(iterator);
		hostcollocationChildName=hostcollocationChild.getNodeName();
		
		if(hostcollocationChildName.equals("homeplacement"))
		{
			try
			{
				parseHomePlacement(parser,root,hostcollocationChild,sourceDir,targetDir,implIds);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"AssemblyFactory: parse host collocation failed.\n");
				throw;
			}
		}
		else if(hostcollocationChildName.equals("processcollocation"))
		{
			try
			{
				parseProcessCollocation(parser,root,hostcollocationChild,sourceDir,targetDir,implIds);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"AssemblyFactory: parse host collocation failed.\n");
				throw;
			}

		}
	}
	
}


//****************************************************
//copyFile copy the source file specified by source 
//to the target file specified by target
//if any error occured return -1, else return 0
//****************************************************
//int 
//AssemblyFactory_impl::copyFile(const char *source, const char *target)
//{
//
//	TRACE1(LEVEL6,"AssemblyFactory: copy file the source file is %s \n",source);
//	TRACE1(LEVEL6,"AssemblyFactory: copy file the target file is %s \n",target);	
//
//#ifdef WIN32	
//
//	if(CopyFile(source,target,false)==0)
//	{
//		TRACE0(LEVEL1,"AssemblyFactory: Copy file into the directory specified failed.\n");		
//		return -1;
//	}
//#endif
//	return 0;
//}

//*********************************************************
//deleteDir delete the directory specified by the dir
//if any error occured return -1, else return 0
//*********************************************************
//int 
//AssemblyFactory_impl::deleteDir(const char *dir)
//{
//	
//	string directory="";
//	directory=directory+dir+"\\*.*";
//	
//	WIN32_FIND_DATA FindFileData;
//	HANDLE hFind;
//
//	hFind = FindFirstFile(directory.c_str(), &FindFileData);
//
//	while(FindNextFile(hFind,&FindFileData)!=0)
//	{
//		
//		TRACE1(LEVEL6,"deleteDir: the file name to be deleted is %s.\n",FindFileData.cFileName);
//	
//		string fileName=FindFileData.cFileName;
//
//		if((fileName!=".")&&(fileName!=".."))
//		{
//			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//			{
//				string subDir="";
//				subDir=subDir+dir+"\\"+fileName;
//
//				if(deleteDir(subDir.c_str())==-1)
//				{						
//					TRACE1(LEVEL1,"deleteDir: delete dir %s failed.\n",FindFileData.cFileName);
//					FindClose(hFind);
//					return -1;
//				}
//					
//				if(_rmdir(subDir.c_str())==-1)
//				{
//					TRACE1(LEVEL1,"deleteDir: delete dir %s failed.\n",FindFileData.cFileName);
//					FindClose(hFind);
//					return -1;
//				}
//
//			}
//			else
//			{
//				string subFile="";
//				subFile=subFile+dir+"\\"+fileName;
//				if(DeleteFile(subFile.c_str())==0)
//				{
//					cout<<"error delete"<<endl;  
//					TRACE0(LEVEL1,"deleteDir: delete error\n");
//					FindClose(hFind);
//					return -1;
//				}
//			}
//		}//end of if to skip . and ..
//	} //end of while
//  
//	 FindClose(hFind);
//	 return 0;
//}


//********************************************************************************************
//this method return the IP address of the host
//if any error occured return NULL,else return the IP address
//********************************************************************************************
//char * 
//AssemblyFactory_impl::getHostIP()
//{
//	
//
//#ifdef WIN32	
//	
//	char szHostName[200];
//	struct hostent *host;
//	struct in_addr *ptr; 
//
//	//socket initialized
//	WSADATA wsaData;
//	if(WSAStartup(MAKEWORD(2,0),&wsaData)!=0)
//	{
//		TRACE0(LEVEL1,"AssemblyFactory: getHostIP: Winsock initiates failed.\n");		
//		return NULL;
//	};
//	
//	if(gethostname( szHostName, 200)!=0)
//	{
//		TRACE0(LEVEL1,"AssemblyFactory: getHostIP: Gethostname failed.\n");				
//		return NULL;
//	};
//
//	host = gethostbyname(szHostName);
//
//	if(host!=NULL)
//	{
//		if((ptr = (struct in_addr *) host->h_addr_list[(CORBA::ULong)0])==NULL)
//		{
//			TRACE0(LEVEL1,"AssemblyFactory: getHostIP: Get the IP address from the host failed.\n");			
//			return NULL;
//		}
//	}
//	else
//	{
//		TRACE0(LEVEL1,"AssemblyFactory: getHostIP: Gethostbyname failed.\n");		
//		return NULL;
//	}
//
//	int a = ptr->S_un.S_un_b.s_b1; 
//	int b = ptr->S_un.S_un_b.s_b2; 
//	int c = ptr->S_un.S_un_b.s_b3; 
//	int d = ptr->S_un.S_un_b.s_b4; 
//	
//	string IPAddr="";
//	char tmp[(CORBA::ULong)5];
//	_itoa( a, tmp, 10 );
//	IPAddr=IPAddr+tmp+".";
//	_itoa( b, tmp, 10 );
//	IPAddr=IPAddr+tmp+".";
//	_itoa( c, tmp, 10 );
//	IPAddr=IPAddr+tmp+".";
//	_itoa( d, tmp, 10 );
//	IPAddr+=tmp;	
//
//#endif 	
//	
//	TRACE1(LEVEL6,"AssemblyFactory: the IP address of the host is %s .\n",IPAddr.c_str());
//	
//	CORBA::String_var address=CORBA::string_dup(IPAddr.c_str());
//	return  address._retn();
//}
//



