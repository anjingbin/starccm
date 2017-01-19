// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Shufeng    icestar_wang@163.com
//
// **********************************************************************

// Version: 1.0.0

#include <CORBA.h>
#include <CCM.h>
#include <Deployment.h>
#include <DeployDomainMgr.h>
#include <fstream>
#include <string>
#include <errno.h>
#include <vector>
#include <iostream>
#include <malloc.h>
#include <stdlib.h>

//modified by xiao heping 
#include <CommonFunc.h>
#ifdef WIN32
//#	include <Winsock.h>
#	include <process.h>
#	include <direct.h>
//#	include <Time.h>
//#	include <Windows.h>
#else
#	include <unistd.h>	
#define _MAX_PATH 256
#endif
//end modify

using namespace std;
using namespace STARCCM::Deployment;

//deleted by xiao heping 2004/05/27
//bool ObjectExist(CORBA::ORB_ptr orb_,const char *corbaloc,int timeout);
//end delete

int main(int argc, char* argv[], char*[])
{
/*
	string DeployDomainMgrIP="172.26.13.145";
	string DeployDomainMgrPort="9000";

	string directory="c:\\ccm";
	
	string hostIP="172.26.13.110";
	string ServerActivatorPort="9003";	
*/	
	//get the curren working directory
	char * buffer=(char *)malloc(_MAX_PATH);

#ifdef WIN32
	if( _getcwd( buffer, _MAX_PATH ) == NULL )
#else
	if(getcwd( buffer, _MAX_PATH) == NULL)
#endif
	{	
	   cout<<"get curren working directory failed."<<endl;
	   return -1;
	}

	string originDir=string(buffer);	
	free(buffer);
 

	//get the host's IP where the DeployDomainMgr object is
	string DeployDomainMgrIP=HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");;
	string DeployDomainMgrPort=HelpFun::GetCCMProfileString("domain","DeployDomainMgrPort","","starccm.properties");
	
//	cout<<"Please enter the host IP of the DeployDomainMgr object."<<endl;
//	cin>>DeployDomainMgrIP;

	
	string hostIP = HelpFun::GetCCMProfileString("domain","host_ip","","starccm.properties");;
	string ServerActivatorPort=HelpFun::GetCCMProfileString("domain","ServerActivatorPort","","starccm.properties");;	
	
	//get the port of the DeployDomainMgr object
//	cout<<"Please enter the IP of the current host."<<endl;
//	cin>>hostIP;


	//
	//get the install directory
	//
/*	
	cout<<"please input the install directory of the deploy system."<<endl;
	cin>>directory;
*/
	string directory=(const char*)getenv("CCM_HOME");
	
	CORBA::ORB_var orb;
	try
	{
		orb = CORBA::ORB_init(argc, argv);
	}
  	catch(const CORBA::Exception &)
	{
		cout<<"HostInstall: ORB initialized failed;"<<endl;
		return -1;
	}	
    	
	CORBA::Object_var obj;
	string DeployDomainMgrLoc="";
	DeployDomainMgrLoc=DeployDomainMgrLoc+"corbaloc::"+DeployDomainMgrIP+":"+DeployDomainMgrPort+"/DeployDomainMgr";
	TRACE1(LEVEL6,"HostInstall: the DeployDomainMgrLoc is %s. \n",DeployDomainMgrLoc.c_str());
	//get the DeployDomainMgr ref  	
	try
	{
	   TRACE0(LEVEL6,"HostInstall: before string to object\n");
	   obj=orb->string_to_object(DeployDomainMgrLoc.c_str());
	   TRACE0(LEVEL6,"HostInstall: after string to object\n");
    }
    catch(const CORBA::BAD_PARAM&)
    {
    }	
	
	try
	{
		if(obj->_non_existent())//CORBA::is_nil(obj))
		{
		cerr << argv[0] << "HostInstall: cannot get the reference of DeployDomainMgr" << endl;
		return -1;
   		}
	}
	catch(const CORBA::TRANSIENT &)
	{
		cerr << argv[0] << "HostInstall: The DeployDomainMgr has not been started!" << endl;
		return -1;
	}
 	
    DeployDomainMgr_var deploydomainmgr = DeployDomainMgr::_narrow(obj);
    assert(!CORBA::is_nil(deploydomainmgr));
	TRACE0(LEVEL6,"HostInstall: after assert\n");


	//ask for whether need verbose		
	bool verbose=false;
	char choice;
	cout<<"Do you want to display the information of the deploy system? y/n"<<endl;	
	do
	{
		cin>>choice;
		if((choice=='y')||(choice=='Y'))
		{
			verbose = true;
		}
		
	}while(cin.good()&&(choice!='N')&&(choice!='n')&&(choice!='y')&&(choice!='Y'));

/*
	//get whether need verbose
	bool verbose;
	try
	{   	
		CORBA::String_var verbosestr=deploydomainmgr->getObjectValue(VERBOSE);
		if (strcmp(verbosestr.in(),"true") == 0)
		{
			verbose=true;	
		}
		else if(strcmp(verbosestr.in(),"false") == 0)
		{
			verbose=false;	
		}
	}
	catch(const CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		TRACE0(LEVEL1,"HostInstall: get timeout failed.\n");	    		
		return;
	} 
*/	

	//get the value of timeout
	int timeout;
	try
	{   	
		CORBA::String_var timestr=deploydomainmgr->getObjectValue(TIMEOUT);		
		timeout=atoi(timestr.in());
	}
	catch(const CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		TRACE0(LEVEL1,"HostInstall: get timeout failed.\n");	    		
		return -1;
	} 

	string ServerActivatorLoc="";
	ServerActivatorLoc=ServerActivatorLoc+"corbaloc::"+hostIP+":"+ServerActivatorPort+"/ServerActivator";
	TRACE1(LEVEL6,"HostInstall: the ServerActivatorLoc is %s. \n",ServerActivatorLoc.c_str());

	//start the ServerActivator at the port specified.
	//first copy the file into the directory specified
	//then start the ServerActivator at the port specified
	
	int error;
	    	
	//
	//copy and start ServerActivator
	//
//	cout<<"Copy ServerActivator.exe"<<endl;

#ifdef WIN32
	string sourceFile=originDir+PATH_DELILIMITOR+"ServerActivator.exe";
	string targetFile=directory+PATH_DELILIMITOR+"ServerActivator.exe";
#else
	string sourceFile = originDir+PATH_DELILIMITOR+"ServerActivator";
	string targetFile=directory+PATH_DELILIMITOR+"ServerActivator";
#endif
		
	
/*
	if(CopyFile(sourceFile.c_str(),targetFile.c_str(),false)==0)
	{
		cout<<"Copy file into the directory specified failed."<<endl;
		return;
	}
	cout<<"copy sourcefile"<<sourceFile<<"to the targetfile "<<targetFile<<endl;
*/
	
	cout<<"Spawnl ServerActivator"<<endl;

#ifdef WIN32
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",ServerActivatorPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-id",hostIP.c_str(),"-dir",directory.c_str(),"-verbose",NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",ServerActivatorPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-id",hostIP.c_str(),"-dir",directory.c_str(),NULL);	
	}
	if(error==-1)
	{
		cout<<"ServerActivator started failed."<<endl;
		return -1;
	}
	
#else

	pid_t serverActivatorPid;

	if( (serverActivatorPid = fork()) < 0 )
	{
		cout<<"ServerActivator started failed."<<endl;
		return -1;
	}

	if(serverActivatorPid == 0)
	{
		if(verbose)
		{
			execl(targetFile.c_str(),"ServerActivator","-OAport ",ServerActivatorPort.c_str(),"-DomainMgr", DeployDomainMgrLoc.c_str(),"-id",hostIP.c_str(),"-dir",directory.c_str(),"-verbose",0);
		}
		else
		{
			execl(targetFile.c_str(),"ServerActivator","-OAport ",ServerActivatorPort.c_str(),"-DomainMgr", DeployDomainMgrLoc.c_str(),"-id",hostIP.c_str(),"-dir",directory.c_str(),0);
			
		}

		cout<<"ServerActivator started failed."<<endl;
		return -1;
	}
	
#endif

/*
	processCmd=targetFile.c_str();
	processCmd=processCmd+" -OAport "+ServerActivatorPort+" -DomainMgr "+DeployDomainMgrLoc+" -id "+IPAddr+" -verbose "+" -dir "+directory;
	cmdLen=strlen(processCmd.c_str())+1;
	
	cmd=(char *)malloc(cmdLen);	
	TRACE1(LEVEL6,"HostInstall: the processcmd is %s.\n",processCmd.c_str());
	
	memcpy(cmd,processCmd.c_str(),cmdLen);
	
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
		TRACE0(LEVEL1,"HostInstall: start ServerActivator process failed. \n");		
		free(cmd);
		return;        	
    	}
*/		
	if(!HelpFun::ObjectExist(orb,ServerActivatorLoc.c_str(),timeout))
	{
		cout<<"ServerActivator started failed. "<<endl;		
		return -1;		
	}	
	
	//register the ServerActivator object to DeployDomainMgr

	try
	{
		deploydomainmgr->registerObject(SERVERACTIVATOR,hostIP.c_str(),ServerActivatorLoc.c_str());    	    			
	}
	catch(const CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
    	TRACE0(LEVEL1,"HostInstall: register ServerActivator corbaloc failed.\n");	
    	return -1;
	}

	return 1;	

}

//deleted by xiao heping 

//bool ObjectExist(CORBA::ORB_ptr orb_,const char *corbaloc,int timeout)
//{
//	CORBA::Object_var obj;
//	CORBA::ORB_var orb=CORBA::ORB::_duplicate(orb_);
//	
//	try
//	{
//	       obj=orb->string_to_object(corbaloc);
//        }
//        catch(const CORBA::BAD_PARAM&)
//        {
//        	TRACE1(LEVEL1,"ObjectExist: Object corbaloc %s invalid \n ",corbaloc);        	
//        	return false;
//        	
//        }
//	
//    	if(CORBA::is_nil(obj))
//    	{
//		TRACE0(LEVEL1,"ObjectExist: Cannot get the reference of Assembly. \n ");        			
//		return false;
//   	}
//   	
//
//	time_t ltime;
//	time( &ltime );
//	int timestart=ltime;
//	int timenow=ltime;
//	bool exist=false;
//	TRACE1(LEVEL6,"ObjectExist: the start time is %d .\n",timenow);	
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
//    		TRACE1(LEVEL6,"ObjectExist: now it is time %d .\n",timenow);
//    		TRACE0(LEVEL2,"ObjectExist: object dose not created correctly whithin timeout.\n");    		
//    		return false;
//    	}
//    	
//    	return true;
//
//}

//end delete 
