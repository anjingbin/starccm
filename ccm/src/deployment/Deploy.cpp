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

#include <iostream>
#include <string>
#include <Deployment.h>
#include <DeployDomainMgr.h>
#include <Configurator_impl.h>
#include <CCM.h>
#include <FileAccessor.h>
#include <CommonFunc.h>

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef WIN32
#	include <Winsock.h>
#	include <process.h>
#	include <Time.h>
#else
#	include <unistd.h>
#endif

using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;


//deleted by xiao heping 2004/05/27
//char * getHostIP();
//bool ObjectExist(CORBA::ORB_ptr orb_,const char *corbaloc,int timeout);
//end delete

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
	CORBA::String_var assembly_loc;    	
    for(int i=0;i<argc;i++)
    {
    	if(strcmp(argv[i], "-package") == 0)
    	{
    		assembly_loc=CORBA::string_dup(argv[i+1]);
//    		cout<<"Package:"<<packageName<<endl;
    		break;
    	}
    }

    if(assembly_loc==NULL)
    {
    	cout<<"Deploy package  miss -package."<<endl;
    	return EXIT_FAILURE;
    }


	CORBA::Object_var obj;
	
//modified by xiao heping 2004/07/08
	string IPaddr = HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");
    //fix the "blank in the path" problem
//    cout << "Please input the Main Host IP address: " << endl;
//	char  tmpstr[256];
//    cin.getline(tmpstr, 256);
//	IPaddr = tmpstr;
//end modified by xiao heping 2004/07/08
	//end by wsf 2004.6.25
	string port=HelpFun::GetCCMProfileString("domain","DeployDomainMgrPort","","starccm.properties");
	
	string DeployDomainMgrLoc="";
	DeployDomainMgrLoc=DeployDomainMgrLoc+"corbaloc::"+IPaddr+":"+port+"/DeployDomainMgr";
	cout<<"Deploy: the DeployDomainMgrLoc is "<<DeployDomainMgrLoc.c_str()<<endl;
	TRACE1(LEVEL6,"Deploy: the DeployDomainMgrLoc is %s. \n",DeployDomainMgrLoc.c_str());
	//get the DeployDomainMgr ref  	
	try
	{
	       TRACE0(LEVEL6,"Deploy: before string to object\n");
	       obj=orb->string_to_object(DeployDomainMgrLoc.c_str());
	       TRACE0(LEVEL6,"Deploy: after string to object\n");
        }
        catch(const CORBA::BAD_PARAM&)
        {
        }	
	
    	if(CORBA::is_nil(obj))
    	{
		cerr << argv[0] << ": cannot get the reference of DeployDomainMgr" << endl;
		return EXIT_FAILURE;
   	}
 	
    	DeployDomainMgr_var deploydomainmgr = DeployDomainMgr::_narrow(obj);
    	assert(!CORBA::is_nil(deploydomainmgr));
    	TRACE0(LEVEL6,"Deploy: after assert\n");
    	cout<<"Deploy: after assert\n";


	//
	//start FileAccessorFactory
	//
	TRACE0(LEVEL6,"Deploy: before fileaccessorfactory\n");
	CORBA::String_var FileAccessorFactroyCorbaloc;
	try
	{
		FileAccessorFactroyCorbaloc=deploydomainmgr->getObject(FILEACCESSORFACTORY,""); 
		TRACE1(LEVEL6,"Deploy: the FileAccessorFactroy corbaloc is %s.\n",FileAccessorFactroyCorbaloc.in());    		
    	}
    	catch(const CORBA::SystemException &)
    	{
    		throw;
    	}
    	catch(...)
    	{
    		TRACE0(LEVEL1,"Deploy: get fileaccessorfactory corbaloc failed.\n");    		
    		return EXIT_FAILURE;
    	}  	    	
    	
	CORBA::String_var ConsoleFileAccessorFactroyCorbaloc;

//modified by wsf 2004.7.6	
//	//modified by xiao heping 2004/05/27
//	//CORBA::String_var address=getHostIP();
//	CORBA::String_var address=HelpFun::getHostIP();
//	//end modify

//modified by xiao heping 2004/07/08
	CORBA::String_var address= HelpFun::GetCCMProfileString("domain","host_ip","","starccm.properties");
//	cout<<"Address is " <<address <<endl;
     //fix the "blank in the path" problem
//  cout << "Please input the Host IP address: " << endl;
//  cin.getline(tmpstr, 256);
//	address = CORBA::string_dup(tmpstr);
//end modified by xiao heping 2004/07/08
//end by wsf 2004.7.6
	
	if(address==NULL)
	{
		TRACE0(LEVEL1,"Deploy: get IP address failed.\n"); 
		return EXIT_FAILURE;
	}
	if(strcmp(address.in(),IPaddr.c_str())==0)
	{
		TRACE0(LEVEL5,"Deploy: console is the main host.\n"); 		
		ConsoleFileAccessorFactroyCorbaloc=FileAccessorFactroyCorbaloc;
	}
	else
	{		
		cout<<"please enter the port of the file accessor factory object ."<<endl;
		cin>>port;
		ConsoleFileAccessorFactroyCorbaloc=CORBA::string_dup("corbaloc::");

#ifdef STARCCMSTRINGADD
		ConsoleFileAccessorFactroyCorbaloc = HelpFun::CORBA_string_add(ConsoleFileAccessorFactroyCorbaloc, address);
		ConsoleFileAccessorFactroyCorbaloc = HelpFun::CORBA_string_add(ConsoleFileAccessorFactroyCorbaloc, CORBA::string_dup(port.c_str()));
		ConsoleFileAccessorFactroyCorbaloc = HelpFun::CORBA_string_add(ConsoleFileAccessorFactroyCorbaloc, CORBA::string_dup("/FileAccessorFactory"));
#else		
		ConsoleFileAccessorFactroyCorbaloc+=address;
		ConsoleFileAccessorFactroyCorbaloc+=CORBA::string_dup(port.c_str());
		ConsoleFileAccessorFactroyCorbaloc+=CORBA::string_dup("/FileAccessorFactory");
#endif		
		
		//
		//get the timeout
		//
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
	    		TRACE0(LEVEL1,"Deploy: get timeout failed.\n");	    		
	    		return EXIT_FAILURE;
	    	}  	

#ifdef WIN32	
		int error=_spawnl( _P_NOWAIT,"FileAccessorFactory.exe","FileAccessorFactory.exe","-OAport ",port.c_str(),NULL);
		if(error=-1)
		{
			TRACE0(LEVEL1,"Deploy: Console File Accessor Factory start failed.\n");			
			return EXIT_FAILURE;			
		}
#else
		pid_t consoleFileAccessorFactoryPid;

		if((consoleFileAccessorFactoryPid = fork()) < 0 )
		{
			TRACE0(LEVEL1,"Deploy: Console File Accessor Factory start failed.\n");
			return EXIT_FAILURE;
		}

		if(consoleFileAccessorFactoryPid == 0)
		{
			execl("FileAccessorFactory","FileAccessorFactory","-OAport ",port.c_str(),0);
			
			TRACE0(LEVEL1,"Deploy: Console File Accessor Factory start failed.\n");
			return EXIT_FAILURE;
		}
#endif
		
		//modified by xiao heping 2004/05/27
		//if(!ObjectExist(orb,ConsoleFileAccessorFactroyCorbaloc.in(),timeout))
		if(!HelpFun::ObjectExist(orb,ConsoleFileAccessorFactroyCorbaloc.in(),timeout))
		//end modify
		{
			TRACE0(LEVEL1,"Deploy: Console File Accessor Factory start failed.\n");	
			return EXIT_FAILURE;
		}
	}
		
	try
	{
		deploydomainmgr->registerObject(CONSOLEFILEACCESSORFACTORY,"",ConsoleFileAccessorFactroyCorbaloc.in());    	    			
	}
	catch(const CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
	    	TRACE0(LEVEL1,"Deploy: register consolefileaccessorfactory corbaloc failed.\n");	
	    	return EXIT_FAILURE;
	}  	
	
	TRACE0(LEVEL6,"Deploy: before register the config value\n");
    	//register config value factory    	
    	CORBA::ValueFactoryBase_var configvaluefactory = new ConfigValueFactory_impl();
    	CORBA::ValueFactoryBase_var oldconfigvalueFactory =             
       		orb -> register_value_factory("IDL:omg.org/Components/ConfigValue:1.0", configvaluefactory.in());   	
	
	//register cookie value factory
    	CORBA::ValueFactoryBase_var cookiefactory = new Components::CookieFactory_impl();
    	CORBA::ValueFactoryBase_var oldcookieFactory =
        	orb -> register_value_factory("IDL:omg.org/Components/Cookie:1.0", cookiefactory.in());

	
	//
	//try to get ComponentInstallation object
	//try to get AssemblyFactory object  
	//
		TRACE0(LEVEL6,"Deploy: before get componentinstallation\n");
    	CORBA::String_var ComponentInstallationRef=deploydomainmgr->getObject(COMPONENTINSTALLATION,"");
    	TRACE1(LEVEL6,"Deploy: the ComponentInstallation ref is %s.\n",ComponentInstallationRef.in());    	

    	CORBA::String_var AssemblyFactoryRef=deploydomainmgr->getObject(ASSEMBLYFACTORY,"");    	
    	TRACE1(LEVEL6,"Deploy: the AssemblyFactory ref is %s.\n",AssemblyFactoryRef.in()); 	  	

	
	//
	//test the assembly factory 
	//
	try
	{
	       TRACE0(LEVEL6,"Deploy: before string to object of assemblyfactory\n");
	       obj=orb->string_to_object(AssemblyFactoryRef.in());
        }
        catch(const CORBA::BAD_PARAM&)
        {
        }

    	if(CORBA::is_nil(obj))
    	{
		cerr << argv[0] << ": cannot get the reference of AssemblyFactory." << endl;
		return EXIT_FAILURE;
   	}
    
    	if(obj->_non_existent())
    	{
    		cout<<"the assembly factory does not exist"<<endl;
    		return EXIT_FAILURE;
    	}
    	AssemblyFactory_var AssemblyFactoryObj = AssemblyFactory::_narrow(obj);
    	assert(!CORBA::is_nil(AssemblyFactoryObj)); 
    	
//   	string assembly_loc;
//    	cout<<"please input the Assembly Package that you want to deploy."<<endl;
	//modified by wsf 2004.6.25
	//fix the "blank in the path" problem	
//	char tmpstr[256];	
//	cin.getline(tmpstr, 256);
//	CORBA::String_var assembly_loc=CORBA::string_dup(getenv("CCM_HOME"));
//	cout<<"CCMHome = "<<assembly_loc.in()<<endl;
//	assembly_loc += CORBA::string_dup("/");
//	CORBA::String_var  assembly_loc = CORBA::string_dup(packageName.in());
//	cout<<"package address = "<<assembly_loc.in()<<endl;
	
//	assembly_loc += CORBA::string_dup(tmpstr);
    	//end by wsf 2004.6.25
    	Cookie_var cookie;
    	try
    	{
    		cout<<"create assembly"<<endl;
    		cookie=AssemblyFactoryObj->create(assembly_loc.in());
    		
    		cout<<"look up assembly"<<endl;
    		Assembly_var assembly=AssemblyFactoryObj->lookup(cookie.in());
    		
    		cout<<"get the assembly state now."<<endl;
    		try
    		{
    			if(assembly->get_state()==INSERVICE)
    			{
    				cout<<"assembly state is inservice"<<endl;
    			}
    			else if(assembly->get_state()==INACTIVE)
    			{
				cout<<"assembly state is inActive"<<endl;				
    			}
    			else 
    			{
    				cout<<"Assembly is wrong."<<endl;
    				return  EXIT_FAILURE;
    			}
    		}
    		catch(const CORBA::SystemException &)
    		{
    			throw;
    		}
    		catch(...)
    		{
    			cout<<"assembly is wrong ,get state failed"<<endl;
    			return EXIT_FAILURE; 
    		}
    		
    		cout<<"build the assembly"<<endl;
    		try
    		{
    			assembly->build();
    		}
    		catch(const CORBA::SystemException &)
    		{
    			TRACE0(LEVEL1,"Deploy: build assembly failed in CORBA System Exception.\n");
    			return EXIT_FAILURE; 
    		}
    		catch(const Components::CreateFailure &)
    		{
    			TRACE0(LEVEL1,"Deploy: build assembly failed in CreateFailure.\n");
    			return EXIT_FAILURE;     			
    		}
    		

    		cout<<"get the assembly state now."<<endl;
    		try
    		{
    			if(assembly->get_state()==INSERVICE)
    			{
    				cout<<"assembly state is inservice"<<endl;
    			}
    			else if(assembly->get_state()==INACTIVE)
    			{
				cout<<"assembly state is inActive"<<endl;				
    			}
    			else 
    			{
    				cout<<"Assembly is wrong."<<endl;
    				return  EXIT_FAILURE;
    			}
    		}
    		catch(const CORBA::SystemException &)
    		{
    			throw;
    		}
    		catch(...)
    		{
    			cout<<"assembly is wrong ,get state failed"<<endl;
    			return EXIT_FAILURE; 
    		}
    		
      	}
    	catch(const CORBA::SystemException &)
    	{
    		throw;
    	}	
    	catch(const Components::Deployment::InvalidLocation &)			              
    	{
    		cout<<"create assembly failed in invalid location."<<endl;
    		return EXIT_FAILURE;
    	}
    	catch(const Components::CreateFailure &) 
    	{
    		cout<<"create assembly failed in Components::CreateFailure."<<endl;
    		return EXIT_FAILURE;    		   		
    	}
    	catch(const Components::Deployment::InvalidAssembly &)
    	{
    		cout<<"lookup or destroy assembly failed in InvalidAssembly."<<endl;
    		return EXIT_FAILURE;    		   		    		
    	}    	
	catch(const Components::RemoveFailure &)
	{
    		cout<<"destroy assembly failed in remove failure."<<endl;
    		return EXIT_FAILURE;    		   		    					
	}
	
	return EXIT_SUCCESS;    
}

	
int
main(int argc, char* argv[]/*, char*[]*/)
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;

    try
    {
	orb = CORBA::ORB_init(argc, argv);
	status = run(orb, argc, argv);
    }
    catch(const CORBA::Exception& ex)
    {
	cerr << ex << endl;
	status = EXIT_FAILURE;
    }

    if(!CORBA::is_nil(orb))
    {
	try
	{
	    orb -> destroy();
	}
	catch(const CORBA::Exception& ex)
	{
	    cerr << ex << endl;
	    status = EXIT_FAILURE;
	}
    }
    
    return status;
}

//deleted by xiao heping 2004/05/27

//char * getHostIP()
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
//		return NULL;
//	};
//	
//	if(gethostname( szHostName, 200)!=0)
//	{
//		return NULL;
//	};
//
//	host = gethostbyname(szHostName);
//
//	if(host!=NULL)
//	{
//		if((ptr = (struct in_addr *) host->h_addr_list[(CORBA::ULong)0])==NULL)
//		{
//			return NULL;
//		}
//	}
//	else
//	{
//		return NULL;
//	}
//
//	int a = ptr->S_un.S_un_b.s_b1; 
//	int b = ptr->S_un.S_un_b.s_b2; 
//	int c = ptr->S_un.S_un_b.s_b3; 
//	int d = ptr->S_un.S_un_b.s_b4; 
//	
//	string IPAddr="";
//	char tmp[5];
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
//	CORBA::String_var address=CORBA::string_dup(IPAddr.c_str());
//	return  address._retn();
//}
	
	
//bool ObjectExist(CORBA::ORB_ptr orb_,const char *corbaloc,int timeout)
//{
//	CORBA::Object_var obj;
//	CORBA::ORB_var orb=CORBA::ORB::_duplicate(orb_);
//	try
//	{
//	       obj=orb->string_to_object(corbaloc);
//        }
//        catch(const CORBA::BAD_PARAM&)
//        {
//        	TRACE1(LEVEL1,"ObjectExist: Object corbaloc %s invalid \n ",corbaloc);        	
//        	return false;
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




