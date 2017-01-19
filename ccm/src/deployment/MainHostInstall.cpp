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
#include <fstream>
#include <string>
#include <errno.h>
#include <vector>
#include <iostream>
#include <malloc.h>
#include <stdlib.h>
#include <iostream>

//added by xiao heping 
#include <CommonFunc.h>
//end add

#ifdef WIN32
//#	include <Winsock.h>
#	include <process.h>
#	include <direct.h>
//#	include <Time.h>
//#	include <Windows.h>
#else
#	include<unistd.h>
#define _MAX_PATH 256
#endif

using namespace std;

//deleted by xiao heping 2004/05/27
//bool ObjectExist(CORBA::ORB_ptr orb_,const char *corbaloc,int timeout);
//end delete

int main(int argc, char* argv[]/*, char*[]*/)
{
	
	string domainName="ccmdomain";
	char* home = getenv("CCM_HOME");
	if(home ==0 )
	{
		printf("please set CCM_HOME enviroment\n");
		exit(0);
	}
	printf("CCM_HOME=%s\n", home);
	string directory=(const char*)home;
	
//modified by xiao heping 2004/07/19
	string DeployDomainMgrPort=HelpFun::GetCCMProfileString("domain","DeployDomainMgrPort","","starccm.properties");
	string ComponentInstallationPort=HelpFun::GetCCMProfileString("domain","ComponentInstallationPort","","starccm.properties");;
	string AssemblyFactoryPort=HelpFun::GetCCMProfileString("domain","AssemblyFactoryPort","","starccm.properties");;
	string ServerActivatorPort=HelpFun::GetCCMProfileString("domain","ServerActivatorPort","","starccm.properties");;
	string FileAccessorFactoryPort=HelpFun::GetCCMProfileString("domain","FileAccessorFactoryPort","","starccm.properties");;
	string PerformanceManagerPort=HelpFun::GetCCMProfileString("domain","PerformanceManagerPort","","starccm.properties");;
	string TransactionManagerPort=HelpFun::GetCCMProfileString("domain","TransactionManagerPort","","starccm.properties");;
	string DynDeploymentMgrPort=HelpFun::GetCCMProfileString("domain","DynDeploymentMgrPort","","starccm.properties");;
	//end by wsf
//	string DeployDomainMgrPort="9000";
//	string ComponentInstallationPort="9001";
//	string AssemblyFactoryPort="9002";
//	string ServerActivatorPort="9003";
//	string FileAccessorFactoryPort="9004";
//	//jxh adding begin--040204
//	//string ComponentManagerPort="90005";
//	string PerformanceManagerPort="90006";
//	string TransactionManagerPort="90007";
//	//jxh end
//
//	//add by wsf
//	string DynDeploymentMgrPort="9008";
//	//end by wsf
//end by xiao heping 

	string confFileName="DomainConf.xml";

	//get the curren working directory
	char * buffer=(char *)malloc(_MAX_PATH);

#ifdef WIN32
	if( _getcwd( buffer, _MAX_PATH ) == NULL )
#else
	if(getcwd(buffer, _MAX_PATH) == NULL)
#endif
	{	
	   cout<<"get curren working directory failed."<<endl;
	   return -1;
	}
	
	free(buffer);
   
	//
	//ask for whether need verbose	
	//	
	bool verbose=true;

	//
	//get the timeout (second) 
	//
	int timeout=60000;

	//get the IP of the host
//modified by xiao heping 2004/05/27

//old code
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
//		cout<<"Winsock initiates failed."<<endl;
//		return -1;
//	};
//	
//	int size=strlen( szHostName );
//
//	if(gethostname( szHostName, 200)!=0)
//	{
//		cout<<"Gethostname failed."<<endl;
//		return -1;
//	};
//
//	host = gethostbyname(szHostName);
//
//	if(host!=NULL)
//	{
//		if((ptr = (struct in_addr *) host->h_addr_list[0])==NULL)
//		{
//			cout<<"Get the IP address from the host failed."<<endl;
//			return -1;
//		}
//	}
//	else
//	{
//		cout<<"Gethostbyname failed."<<endl;
//		return -1;
//	}
//
//	int a = ptr->S_un.S_un_b.s_b1; 
//	int b = ptr->S_un.S_un_b.s_b2; 
//	int c = ptr->S_un.S_un_b.s_b3; 
//	int d = ptr->S_un.S_un_b.s_b4; 
//	
//	//string IPAddr="";
//	string IPAddr;
//	char tmp[5]; 
//	_itoa( a, tmp, 10 );
//	//IPAddr=IPAddr+tmp+".";
//	IPAddr=tmp;
//	IPAddr=IPAddr+".";
//	_itoa( b, tmp, 10 );
//	IPAddr=IPAddr+tmp+".";
//	_itoa( c, tmp, 10 );
//	IPAddr=IPAddr+tmp+".";
//	_itoa( d, tmp, 10 );
//	IPAddr+=tmp;	
//
//#endif 

//modified by wsf 2004.7.6
////new code
//	string IPAddr=HelpFun::getHostIP();
////end modify

//modified by xiao heping 2004/07/08
	TRACE0(LEVEL6,"MaingHostInstall: before get ip address.\n");
	string IPAddr = HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");
    	TRACE1(LEVEL6,"MaingHostInstall:  get ip address %s.\n",IPAddr.c_str());
	//fix the "blank in the path" problem
//    cout << "Please input the Main Host IP address: " << endl;
//	char  tmpstr[256];
//    cin.getline(tmpstr, 256);
//	IPAddr = tmpstr;
//end modified by xiao heping 2004/07/08
//end by wsf 2004.7.6

	//create the DomainConf.xml
	TRACE0(LEVEL6,"MaingHostInstall: create the DomainConf.xml.\n");
	string confFileFullName=directory+PATH_DELILIMITOR+confFileName;	
	ofstream outfile(confFileFullName.c_str());
	if(!outfile)
	{
		cout<<"Can not create the DomainConf.xml ."<<endl;
		return -1;
	}

	//Input into the DomainConf.xml file
	outfile<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl<<endl;	
	

	//Input the object element into DomainConf.xml file
	string DeployDomainMgrLoc="";
	string ComponentInstallationLoc="";
	string AssemblyFactoryLoc="";
	string ServerActivatorLoc="";
	string FileAccessorFactoryLoc="";
	
	//jxh begin --040204
	string ComponentManagerLoc="";
	string PerformanceManagerLoc="";
	string TransactionManagerLoc="";
	//jxh end

	//add by wsf
	string DynDeploymentMgrLoc="";
	//end by wsf

	DeployDomainMgrLoc=DeployDomainMgrLoc+"corbaloc::"+IPAddr+":"+DeployDomainMgrPort+"/DeployDomainMgr";
	ComponentInstallationLoc=ComponentInstallationLoc+"corbaloc::"+IPAddr+":"+ComponentInstallationPort+"/ComponentInstallation";
	AssemblyFactoryLoc=AssemblyFactoryLoc+"corbaloc::"+IPAddr+":"+AssemblyFactoryPort+"/AssemblyFactory";
	ServerActivatorLoc=ServerActivatorLoc+"corbaloc::"+IPAddr+":"+ServerActivatorPort+"/ServerActivator";
	FileAccessorFactoryLoc=FileAccessorFactoryLoc+"corbaloc::"+IPAddr+":"+FileAccessorFactoryPort+"/FileAccessorFactory";

	//jxh 040204
	//ComponentManagerLoc=ComponentManagerLoc+"corbaloc::"+IPAddr+":"+ComponentManagerPort+"/ComponentManager";
	PerformanceManagerLoc=PerformanceManagerLoc+"corbaloc::"+IPAddr+":"+PerformanceManagerPort+"/PerformanceManagerer";
	TransactionManagerLoc=TransactionManagerLoc+"corbaloc::"+IPAddr+":"+TransactionManagerPort+"/TransactionManager";
	//jxh end

	//add by wsf
	DynDeploymentMgrLoc = DynDeploymentMgrLoc+"corbaloc::"+IPAddr+":"+DynDeploymentMgrPort+"/DynDeploymentMgr";
	//end

	outfile<<"<domainconf name = \""<<domainName<<"\" >"<<endl<<endl;

	outfile<<"	<deploydomainmgr corbaloc = \""<<DeployDomainMgrLoc<<"\" />"<<endl<<endl;

	//add by wsf
	outfile<<"		<dyndeploymentmgr corbaloc= \""<<DynDeploymentMgrLoc<<"\" />"<<endl<<endl;
	//end

	//jxh adding --0040204
	//outfile<<"	<componentManager corbaloc = \""<<ComponentManagerLoc<<"\" />"<<endl<<endl;
	outfile<<"	<PerformanceManager corbaloc = \""<<PerformanceManagerLoc<<"\" />"<<endl<<endl;
	outfile<<"	<TransactionManager corbaloc = \""<<TransactionManagerLoc<<"\" />"<<endl<<endl;
	//jxh end

	outfile<<"	<componentinstallation corbaloc = \""<<ComponentInstallationLoc<<"\" />"<<endl<<endl;

	outfile<<"	<assemblyfactory corbaloc = \""<<AssemblyFactoryLoc<<"\" />"<<endl<<endl;

	outfile<<"	<fileaccessorfactory corbaloc = \""<<FileAccessorFactoryLoc<<"\" />"<<endl<<endl;

	outfile<<"	<serveractivator id = \""<<IPAddr<<"\" corbaloc = \""<<ServerActivatorLoc<<"\" >"<<endl;
	outfile<<"	</serveractivator>"<<endl<<endl;
	
	outfile<<"	<rootdir directory = \""<<directory<<"\" />"<<endl<<endl;
	
	if(verbose)
	{	
		outfile<<"	<verbose open = \"true\" />"<<endl<<endl;
	}
	else
	{
		outfile<<"	<verbose open = \"false\" />"<<endl<<endl;
	}
	outfile<<"	<timeout time = \""<<timeout<<"\" />"<<endl<<endl;
	outfile<<"</domainconf>"<<endl;
	outfile.close();
	 

	//start the object at the port specified.
	
	int error;
	CORBA::ORB_var orb;
	try
	{
		orb = CORBA::ORB_init(argc, argv);
	}
  	catch(const CORBA::Exception &)
    {
		cout<<"MainHostInstall: ORB initialized failed;"<<endl;
		return -1;
    }
    //	used to debug ,don't delete these code
	//jxh
	//string targetFile=directory+"\\DeployDomainMgr.exe";
/*	CORBA::String_var targetFile1=CORBA::string_dup("msdev ");
	//CORBA::String_var targetFile1=CORBA::string_dup("");
	targetFile1+=CORBA::string_dup(directory.c_str());
	targetFile1+=CORBA::string_dup("\\DeployDomainMgr.exe");
        string cmdd = string(targetFile1.in());




    cmdd=cmdd+" -OAport "+DeployDomainMgrPort+" -verbose "+" -dir "+directory;

	
	int cmdLen=strlen(cmdd.c_str())+1;	
	char * cmd=(char *)malloc(cmdLen);	
//	TRACE1(LEVEL6,"ServerActivator: the processcmd is %s.\n",cmdd.c_str());
	memcpy(cmd,cmdd.c_str(),cmdLen);

//	TRACE1(LEVEL6,"ServerActivator: start ComponentServer cmd is %s.\n",cmd);
	
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
        	TRUE,            // Set handle inheritance to FALSE. 
        	CREATE_NEW_CONSOLE,                // No creation flags. 
        	NULL,             // Use parent's environment block. 
        	NULL,             // Use parent's starting directory. 
        	&si,              // Pointer to STARTUPINFO structure.
        	&pi )             // Pointer to PROCESS_INFORMATION structure.
    		) 
    	{        	
		TRACE0(LEVEL1,"ServerActivator: start ComponentServer process failed. \n");		
		free(cmd);
		return -1;	
    	}
    	free(cmd);	*/

	//start DeployDomainMgr.exe	
#ifdef WIN32
	string targetFileName = "DeployDomainMgr.exe";
#else
	string targetFileName = "DeployDomainMgr";
#endif	
		
	string targetFile = directory + PATH_DELILIMITOR + targetFileName;
	string port = "iiop://:";
	port = port + DeployDomainMgrPort.c_str();

	cout<<"spawnl DeployDomainMgr"<<endl;

#ifdef WIN32

	//modified by wsf 2004.6.25
	//fix the "blank in the path" problem
	string dirarg = "\""+directory+"\"";
	
#ifdef TAO
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-ORBEndpoint",port.c_str(),"-verbose","-dir",dirarg.c_str(),NULL);
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-ORBEndpoint",port.c_str(),"-dir",dirarg.c_str(),NULL);
	}
#else
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",DeployDomainMgrPort.c_str(),"-verbose","-dir",dirarg.c_str(),NULL);
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",DeployDomainMgrPort.c_str(),"-dir",dirarg.c_str(),NULL);
	}
#endif
	//end by wsf 2004.6.25
	if(error==-1)
	{
		cout<<"DeployDomainMgr started failed. "<<endl;		
		return -1;
	}	

#else
	
	pid_t pid;

	if( (pid = fork()) < 0 )
	{
		cout<<"DeployDomainMgr started failed. "<<endl;
		return -1;
	}

	if(pid == 0)
	{
		if(verbose)
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",DeployDomainMgrPort.c_str(),"-verbose","-dir",directory.c_str(),0);
		}
		else
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",DeployDomainMgrPort.c_str(),"-dir",directory.c_str(),0);
			
		}

		cout<<"DeployDomainMgr started failed. "<<endl;
		return -1;
						
	}

#endif	
	
	TRACE0(LEVEL6, "Check if DeploymentManager is running...\n");
	
	if(!HelpFun::ObjectExist(orb,DeployDomainMgrLoc.c_str(),timeout))
	{
		cout<<"DeployDomainMgr started failed. "<<endl;		
		return -1;		
	}
	
	
	//
	//start ComponentInstallation.exe
	//

#ifdef WIN32
       targetFileName = "ComponentInstallation.exe";	
#else
       targetFileName = "ComponentInstallation";
#endif	
	
	targetFile=directory+PATH_DELILIMITOR+targetFileName;
	port = "iiop://:";
	port = port + ComponentInstallationPort.c_str();

	cout<<"Spawnl ComponentInstallation"<<endl;	

#ifdef WIN32

	dirarg = "\""+directory+"\"";
	
#ifdef TAO
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str()," -ORBEndpoint",port.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",dirarg.c_str(),"-verbose",NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str()," -ORBEndpoint",port.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",dirarg.c_str(),NULL);	
	}
#else	
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",ComponentInstallationPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",dirarg.c_str(),"-verbose",NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",ComponentInstallationPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",dirarg.c_str(),NULL);	
	}
#endif
	//end by wsf 2004.6.25
	if(error==-1)
	{
		cout<<"ComponentInstallation started failed."<<endl;
		return -1;
	}
#else

	pid_t componentInstallationPid;
	if( (componentInstallationPid = fork()) < 0 )
	{
		cout<<"ComponentInstallation started failed. "<<endl;
		return -1;
	}
	if(componentInstallationPid == 0)
	{
		if(verbose)
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",ComponentInstallationPort.c_str(),"-verbose","-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",directory.c_str(),0);
		
		}
		else
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",ComponentInstallationPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",directory.c_str(),0);
		
		}
		
		cout<<"ComponentInstallation started failed. "<<endl;
		return -1;
	}
	
#endif
	
	if(!HelpFun::ObjectExist(orb,ComponentInstallationLoc.c_str(),timeout))
	{
		cout<<"ComponentInstallation started failed. "<<endl;		
		return -1;		
	}
	


	//
	//start AssemblyFactory.exe
	//
#ifdef WIN32
	targetFileName = "AssemblyFactory.exe";
#else
	targetFileName = "AssemblyFactory";
#endif
	
	targetFile=directory+PATH_DELILIMITOR+targetFileName;
	port = "iiop://:";
	port = port + AssemblyFactoryPort.c_str();

	cout<<"Spawnl AssemblyFactory"<<endl;

#ifdef WIN32

	dirarg = "\""+directory+"\"";
	
#ifdef TAO
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-ORBEndpoint",port.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-ip",IPAddr.c_str(),"-dir",dirarg.c_str(),"-verbose",NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-ORBEndpoint",port.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-ip",IPAddr.c_str(),"-dir",dirarg.c_str(),NULL);	
	}
#else
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",AssemblyFactoryPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-ip",IPAddr.c_str(),"-dir",dirarg.c_str(),"-verbose",NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",AssemblyFactoryPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-ip",IPAddr.c_str(),"-dir",dirarg.c_str(),NULL);	
	}
#endif
	//end by wsf 2004.6.25
	if(error==-1)
	{
		cout<<"AssemblyFactory started failed."<<endl;
		return -1;
	}
	
#else
	
	pid_t assemblyFactoryPid;

	if( (assemblyFactoryPid = fork()) < 0 )
	{
                cout<<"AssemblyFactory started failed."<<endl;
		return -1;		
	}
	
	if( assemblyFactoryPid == 0 )
	{
		if(verbose)
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",AssemblyFactoryPort.c_str(),"-verbose","-DomainMgr",DeployDomainMgrLoc.c_str(),"-ip",IPAddr.c_str(),"-dir",directory.c_str(),0);
		}
		else
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",AssemblyFactoryPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-ip",IPAddr.c_str(),"-dir",directory.c_str(),0);
		}

		cout<<"AssemblyFactory started failed."<<endl;
		return -1;
						
	}
#endif
	
	if(!HelpFun::ObjectExist(orb,AssemblyFactoryLoc.c_str(),timeout))
	{
		cout<<"AssemblyFactory started failed. "<<endl;		
		return -1;		
	}		
	
	
	//
	//start ServerActivator
	//

#ifdef WIN32
	targetFileName = "ServerActivator.exe";
#else
	targetFileName = "ServerActivator";
#endif
	
	targetFile=directory+PATH_DELILIMITOR+targetFileName;
	port = "iiop://:";
	port = port + ServerActivatorPort.c_str();

	cout<<"Spawnl ServerActivator"<<endl;

#ifdef WIN32

	dirarg = "\""+directory+"\"";
	
#ifdef TAO
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-ORBEndpoint",port.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-id",IPAddr.c_str(),"-dir",dirarg.c_str(),"-verbose",NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-ORBEndpoint",port.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-id",IPAddr.c_str(),"-dir",dirarg.c_str(),NULL);	
	}
#else	
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",ServerActivatorPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-id",IPAddr.c_str(),"-dir",dirarg.c_str(),"-verbose",NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",ServerActivatorPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-id",IPAddr.c_str(),"-dir",dirarg.c_str(),NULL);	
	}
#endif
	//end by wsf 2004.6.25
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

	if( serverActivatorPid == 0 )
	{
		if(verbose)
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",ServerActivatorPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-id",IPAddr.c_str(),"-dir",directory.c_str(),"-verbose",0);
		}
		else
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",ServerActivatorPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-id",IPAddr.c_str(),"-dir",directory.c_str(),0);
			
		}

		cout<<"ServerActivator started failed."<<endl;
		return -1;
	}
	
#endif
	
	if(!HelpFun::ObjectExist(orb,ServerActivatorLoc.c_str(),timeout))
	{
		cout<<"ServerActivator started failed. "<<endl;		
		return -1;		
	}	
	
	
	//
	//spawnl fileaccessorfactory
	//

#ifdef WIN32
	targetFileName = "FileAccessorFactory.exe";
#else
	targetFileName = "FileAccessorFactory";
#endif
	
	targetFile=directory+PATH_DELILIMITOR+targetFileName;
	
	port = "iiop://:";
	port = port + FileAccessorFactoryPort.c_str();	

	cout<<"Spawnl FileAccessor"<<endl;

#ifdef WIN32
	
#ifdef TAO
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-ORBEndpoint",port.c_str(),NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-ORBEndpoint",port.c_str(),NULL);	
	}
#else	
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",FileAccessorFactoryPort.c_str(),NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",FileAccessorFactoryPort.c_str(),NULL);	
	}
#endif	
	
	if(error==-1)
	{
		cout<<"FileAccessorFactory started failed."<<endl;
		return -1;
	}
	
#else

	pid_t fileAccessorFactoryPid;

	if(( fileAccessorFactoryPid = fork() ) < 0)
	{
		cout<<"FileAccessorFactory started failed."<<endl;
		return -1;
	}

	if(fileAccessorFactoryPid == 0)
	{
		if(verbose)
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",FileAccessorFactoryPort.c_str(),0);
		}
		else
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",FileAccessorFactoryPort.c_str(),0);
		}

		cout<<"FileAccessorFactory started failed."<<endl;
		return -1;
	}
#endif

	if(!HelpFun::ObjectExist(orb,FileAccessorFactoryLoc.c_str(),timeout))
	{
		cout<<"fileaccessorfactory started failed. "<<endl;		
		return -1;		
	}
	//jxh
	
	//add by wsf

	//
	//spawnl DynDeploymentMgr
	//

	//modified 2004.6.25
    	//fix the "blank in the path" problem

#ifdef WIN32	
		//string msdev = "msdev \"";
	string msdev = "";
	targetFileName = "DynDeploymentMgr.exe";
#else
	string msdev = "";
	targetFileName = "DynDeploymentMgr";
#endif
	
	targetFile=msdev+directory+PATH_DELILIMITOR+targetFileName;
//	if (strcmp(msdev.c_str(),"msdev \"") == 0)
//		targetFile=targetFile+"\"";

	cout<<"Spawnl DynDeploymentMgr"<<endl;

#ifdef WIN32

	dirarg = "\""+directory+"\"";
	
	if(verbose)
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",DynDeploymentMgrPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",dirarg.c_str(),"-verbose",NULL);	
	}
	else
	{
		error=_spawnl( _P_NOWAIT,targetFile.c_str(),targetFile.c_str(),"-OAport ",DynDeploymentMgrPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",dirarg.c_str(),NULL);	
	}
	if(error==-1)
	{
		cout<<"DynDeploymentMgr started failed."<<endl;
		return -1;
	}
	
#else

	pid_t dynDeploymentMgrPid;

	if((dynDeploymentMgrPid = fork()) < 0)
	{
		cout<<"DynDeploymentMgr started failed."<<endl;
		return -1;
	}

	if( dynDeploymentMgrPid == 0)
	{
		if(verbose)
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",DynDeploymentMgrPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",directory.c_str(),"-verbose",0);
		}
		else
		{
			execl(targetFile.c_str(),targetFileName.c_str(),"-OAport",DynDeploymentMgrPort.c_str(),"-DomainMgr",DeployDomainMgrLoc.c_str(),"-dir",directory.c_str(),0);
			
		}

		cout<<"DynDeploymentMgr started failed."<<endl;
		return -1;
	}

#endif
	


	//
	//new a process for DynDeploymentMgr
	//
	
//	string processCmd=targetFile;
//	processCmd=processCmd+" -OAport "+DynDeploymentMgrPort+" -DomainMgr "+DeployDomainMgrLoc+" -dir "+dirarg;
//	//end 2004.6.25
//	if(verbose)
//	{
//		processCmd=processCmd+" -verbose ";
//	}
//	
//	int cmdLen=strlen(processCmd.c_str())+1;	
//	char * cmd=(char *)malloc(cmdLen);	
//	TRACE1(LEVEL6,"MainHostInstall: the processcmd is %s.\n",processCmd.c_str());
//	memcpy(cmd,processCmd.c_str(),cmdLen);
//
//	STARTUPINFO si;
//    	PROCESS_INFORMATION pi;
//
//    	ZeroMemory( &si, sizeof(si) );
//    	
//    	si.cb = sizeof(si);
//    	si.lpDesktop="";
//    	si.lpTitle="";
//    	si.dwFlags=STARTF_USESHOWWINDOW;
//    	si.wShowWindow=SW_SHOWDEFAULT;
//    	
//    	ZeroMemory( &pi, sizeof(pi) );
//
//    	// Start the child process. 
//    	if( !CreateProcess( NULL, // No module name (use command line). 
//        	cmd, // Command line. 
//        	NULL,             // Process handle not inheritable. 
//        	NULL,             // Thread handle not inheritable. 
//        	FALSE,            // Set handle inheritance to FALSE. 
//        	CREATE_NEW_CONSOLE,                // No creation flags. 
//        	NULL,             // Use parent's environment block. 
//        	NULL,             // Use parent's starting directory. 
//        	&si,              // Pointer to STARTUPINFO structure.
//        	&pi )             // Pointer to PROCESS_INFORMATION structure.
//    		) 
//    	{        	
//		TRACE0(LEVEL1,"MainHostInstall: start DynDeploymentMgr process failed. \n");		
//		free(cmd);
//    	}
//    	free(cmd);

	//end

	if(!HelpFun::ObjectExist(orb,DynDeploymentMgrLoc.c_str(),timeout))
	{
		cout<<"DynDeploymentMgr started failed. "<<endl;		
		return -1;		
	}	
	
	return 1;

	
}


//deleted by xiao heping 2004/05/27
//
//bool ObjectExist(CORBA::ORB_ptr orb_,const char *corbaloc,int timeout)
//{
//	TRACE1(LEVEL6,"ObjectExist: the CORBA loc is %s .\n",corbaloc);	
//	
//	CORBA::Object_var obj;
//	CORBA::ORB_var orb=CORBA::ORB::_duplicate(orb_);
//	
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
//
