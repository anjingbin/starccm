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

#include <CORBA.h>

#include <ComponentServer_impl.h>          
#include <DeployDomainMgr.h>
#include <Configurator_impl.h>
#include <CCM.h>

//added by xiao heping 2004/04/03
#if defined(ORBacus)
	#include <OB/Properties.h>
#endif
#if defined(StarBus)
	#include <STAR/Properties.h>
#endif
//end add

#include <stdlib.h>
#include <errno.h>
#include <fstream>
#include <ConfigFile.h>
#include <Components.h>

#ifndef LITE_VERSION
	#include <PSSInit.h>
	#include <RunTime_psdl.h>
	#include <RunTime_pss.h>
	#include <string.h>
	#include <iostream>
#endif

#include <CosTransactions.h>
#include <CosTransactionsOTS.h>

#ifndef TAO
#include <ImplicitOTSInterceptor.h>
//jxh
#include <ComponentManager_impl.h>
#include <TransactionManager_impl.h>
#include <Manager.h>

#endif

//added by xiao heping 2004/04/02
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#ifdef WIN32
#include <crtdbg.h>
#endif
//end add


using namespace std;
using namespace Components::Deployment;
using namespace ManagerDomain;

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
	
    bool verbose=false;	    
    CORBA::String_var componentServerUUID;
    CORBA::String_var serverActivatorUUID;
    CORBA::String_var domainMgr; 
    	
    for(int i=0;i<argc;i++)
    {
    	if(strcmp(argv[i], "-package") == 0)
    	{
    		verbose=true;	
    	}
    	if(strcmp(argv[i], "-DomainMgr") == 0)
    	{
    		domainMgr=CORBA::string_dup(argv[i+1]);
    	}
		if(strcmp(argv[i], "-fatherId") == 0)
		{
    		serverActivatorUUID=CORBA::string_dup(argv[i+1]);
    	}    	
		if(strcmp(argv[i], "-id") == 0)
		{
    		componentServerUUID=CORBA::string_dup(argv[i+1]);
    	}
    }

    if((componentServerUUID==NULL)||(domainMgr==NULL)||(serverActivatorUUID==NULL))
    {
    	TRACE0(LEVEL1,"ComponentServer: start the ComponentServer miss -DomainMgr or -fatherId or -id.\n");
    	return EXIT_FAILURE;
    }

   //
   //resolve the DeployDomaingMgr refence from its corbaloc
   //
	CORBA::Object_var mangerobj;
	try
	{
		mangerobj=orb->string_to_object(domainMgr.in());
	}
    catch(const CORBA::BAD_PARAM&)
    {
    	TRACE0(LEVEL1,"ComponentServer: invalid DeployDomainMgr corbaloc.\n");        	
    	return EXIT_FAILURE;
    }
	
	if(CORBA::is_nil(mangerobj))
	{
    	TRACE0(LEVEL1,"ComponentServer: cannot get the reference of DeployDomainMgr.\n");    		
		return EXIT_FAILURE;
   	}
	
	DeployDomainMgr_var DomainMgr = DeployDomainMgr::_narrow(mangerobj);
    assert(!CORBA::is_nil(DomainMgr)); 
    
 
    //register config value factory
    CORBA::ValueFactoryBase_var factory = new ConfigValueFactory_impl();
    CORBA::ValueFactoryBase_var oldFactory =
        orb -> register_value_factory("IDL:omg.org/Components/ConfigValue:1.0", factory.in());
   
    //
    // Resolve Root POA
    //
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj);
    
    //
    // Get a reference to the POA manager
    //
    PortableServer::POAManager_var manager = rootPOA -> the_POAManager();
 
    //jxh--0311
	//create the persistent reference
    PortableServer::LifespanPolicy_var lifespanC=
      rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
      
    PortableServer::IdAssignmentPolicy_var idassignmentC=
      rootPOA->create_id_assignment_policy(PortableServer::USER_ID);
    
    CORBA::PolicyList policy_listC;
    policy_listC.length(2);
    policy_listC[0]=
      PortableServer::LifespanPolicy::_duplicate(lifespanC);    
    policy_listC[1]=
      PortableServer::IdAssignmentPolicy::_duplicate(idassignmentC);     
           
    PortableServer::POA_var componentManagerPoa =
      rootPOA->create_POA("managerpoa",manager,policy_listC);     
    
	lifespanC->destroy();
    idassignmentC->destroy();

    //
    // Create implementation object
    //
	ComponentManager_impl* ComponentManagerImpl = 
    	new ComponentManager_impl(orb,DomainMgr.in(),componentServerUUID.in(),verbose);    
    PortableServer::ServantBase_var servantComponentMgr = ComponentManagerImpl;
	//not use the implicit activation
    PortableServer::ObjectId_var ComponentManageroid=PortableServer::string_to_ObjectId("ComponentManager");
    componentManagerPoa->activate_object_with_id(ComponentManageroid,ComponentManagerImpl);
    CORBA::Object_var objectComponentMgr = componentManagerPoa->id_to_reference(ComponentManageroid);
	ComponentManager_var componentManager = ComponentManager::_narrow(objectComponentMgr);
	//jxh-- end
 
   // 06/21
     PortableServer::LifespanPolicy_var lifespanT=
      rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
      
    PortableServer::IdAssignmentPolicy_var idassignmentT=
      rootPOA->create_id_assignment_policy(PortableServer::USER_ID);
    
    CORBA::PolicyList policy_listT;
    policy_listT.length(2);
    policy_listT[0]=
      PortableServer::LifespanPolicy::_duplicate(lifespanT);    
    policy_listT[1]=
      PortableServer::IdAssignmentPolicy::_duplicate(idassignmentT);     
           
    PortableServer::POA_var transactionManagerPoa =
      rootPOA->create_POA("transmanagerpoa",manager,policy_listT);     
    
	lifespanT->destroy();
    idassignmentT->destroy();

    //
    // Create implementation object
    //
	TransactionManager_impl* TransactionManagerImpl = 
    	new TransactionManager_impl();    
    PortableServer::ServantBase_var TransactionManageMgr = TransactionManagerImpl;
	//not use the implicit activation
    PortableServer::ObjectId_var TransactionManageoid=PortableServer::string_to_ObjectId("TransactionManager");
    transactionManagerPoa->activate_object_with_id(TransactionManageoid,TransactionManagerImpl);
    CORBA::Object_var objectTransactionMgr = transactionManagerPoa->id_to_reference(TransactionManageoid);
	TransactionManager_var TransactionManager = TransactionManager::_narrow(objectTransactionMgr);
	//jxh-- end
 

    //create the persistent reference
    PortableServer::LifespanPolicy_var lifespan=
      rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
      
    PortableServer::IdAssignmentPolicy_var idassignment=
      rootPOA->create_id_assignment_policy(PortableServer::USER_ID);
    
    CORBA::PolicyList policy_list;
    policy_list.length(2);
    policy_list[0]=
      PortableServer::LifespanPolicy::_duplicate(lifespan);    
    policy_list[1]=
      PortableServer::IdAssignmentPolicy::_duplicate(idassignment);     
           
    PortableServer::POA_var child=
      rootPOA->create_POA("child",manager,policy_list);     
    
    lifespan->destroy();
    idassignment->destroy();
    
  
    //
    // Create implementation object
    //
    ComponentServer_impl* ComponentServerImpl = new ComponentServer_impl(orb,ComponentManagerImpl,child,DomainMgr.in(),serverActivatorUUID.in(),componentServerUUID.in(),verbose);    
	PortableServer::ServantBase_var servant = ComponentServerImpl;
    
    //not use the implicit activation
    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId("ComponentServer");
    child->activate_object_with_id(oid,ComponentServerImpl);
    CORBA::Object_var object=child->id_to_reference(oid);
	ComponentServer_var componentserver=ComponentServer::_narrow(object);

#ifdef ORBacus
    //
    // Resolve the BootManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
    OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);

    bootManager -> add_binding(oid, componentserver);
    //jxh
	bootManager -> add_binding(ComponentManageroid, componentManager);
	bootManager -> add_binding(TransactionManageoid, TransactionManager);
	
#endif
    
#ifdef StarBus
    CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
    STAR::AssistAdaptorManager_var assistAdapterManager = STAR::AssistAdaptorManager::_narrow(obj);

    assistAdapterManager -> binding_objref(oid, componentserver);
    //jxh
	assistAdapterManager -> binding_objref(ComponentManageroid, componentManager);
	assistAdapterManager -> binding_objref(TransactionManageoid, TransactionManager);
	
#endif

#ifdef TAO
    CORBA::Object_var table_object = orb -> resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
      
    IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

	CORBA::String_var iorStr = orb -> object_to_string(componentserver);	
	
    adapter->bind ("ComponentServer", iorStr.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;    
#endif
    //update configuration file
    ::Container::ConfigFile_var configFile = new ::Container::ConfigFile();
    
    configFile->addComponentServer(componentServerUUID.in());

	//
	//added by xiao heping 2004/03/13
	//for test remove_home method
	//
//#ifdef _DEBUG	
//	//
//    // Save reference
//    //
//	CORBA::String_var str = orb -> object_to_string(componentserver); 
//
//	CORBA::String_var refFile = (const char*)getenv("CCM_HOME");
//    refFile += PATH_DELILIMITOR;
//    refFile += "ComponentServer.ref";
//
//    //const char* refFile = "c:\\ccm\\ComponentServer.ref";
//    ofstream out(refFile);
//    if(out.fail())
//    {
//	cerr << ": can't open '" << refFile << "': "
//	     << strerror(errno) << endl;
//	return 0;
//    }
//    out << str << endl;
//    out.close();
//#endif	
	//end add

#ifndef LITE_VERSION    
    PSS::initialize_persistent_service(orb);
    
    //initialize all StorageHome factories
    CORBA::Object_var crObj = orb -> resolve_initial_references("PSS");

	// Initialize PersitentState Service
    CosPersistentState::ConnectorRegistry_var connector_registry = CosPersistentState::ConnectorRegistry::_narrow(crObj);
	assert(!CORBA::is_nil(connector_registry));
	
	// Retrieve the connector
	cout << "Retrieving the connector for database store..." << endl;

	CosPersistentState::Connector_var connector = 
		connector_registry -> find_connector("PSS:connector:database:postgresql:1.0");

	assert(!CORBA::is_nil(connector));

	register_persistent_factories(connector);
//
//	// Create basic session
//	cout << "Creating basic session via connector...." << endl;
//
//	CosPersistentState::ParameterList parameters;
//	parameters.length(4);
//	
//	ifstream fin("config.ini");
//	char special ;
//	char temp[1024];
//	char host[1024];
//	char database[1024];
//	char user[1024];
//	char password[1024];
//	char port[1024];
//	int count = 0;
//	fin.get(special);
//	while( special != EOF)
//	{
//		switch(special)
//		{
//			case '=' :
//				{
//					fin.getline(temp,1024);
//					count++;
//					switch(count)
//					{
//						case 1:
//							::strcpy(host,temp);
//							break;
//						case 2:
//							::strcpy(database,temp);
//							break;
//						case 3:
//							::strcpy(user,temp);
//							break;
//						case 4:
//							::strcpy(password,temp);
//							break;
//						case 5:
//							::strcpy(port,temp);
//							break;
//						default:
//							break;
//					}
//				break;
//				}
//			case '#':
//				fin.getline(temp,1024);
//				break;
//			case ' ':
//				fin.getline(temp,1024);
//				break;
//			default:
//				break;
//		}	
//		if (count == 5)
//			break;
//		fin.get(special);
//	}
//
//	cout <<"@@@@@@@@@@@@@@@\n"<< host<<endl << database<<endl<< user<<endl << password <<endl<< port<<endl;	
//	char* Host = host;
//	char* Database = database;
//	char* User = user;
//	char* Password = password;
//	parameters[0].name = CORBA::string_dup("host");
//	parameters[0].val <<= Host;
//	parameters[1].name = CORBA::string_dup("database");
//	parameters[1].val <<= Database;
//
//	parameters[2].name = CORBA::string_dup("user");
//	parameters[2].val <<= User;
//	parameters[3].name = CORBA::string_dup("password");
//	parameters[3].val <<= Password;
#endif
   
    //
    // Run implementation
    //
    manager -> activate();
    orb -> run();

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[]/*, char*[]*/)
{
	//added by xiao heping 2004/04/02
#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//end add

    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;


#ifdef WITH_OTS

#ifdef ORBacus
    OB::OTSInit(argc, argv);
#endif
    
#ifdef StarBus
    STAR::OTSInit(argc, argv);
#endif

#endif

    try
    {
    	//added by xiao heping 2004/04/03

	//
	// Set the communications concurrency model
	//
#if defined(STARCCM_MTL)
	#if defined(ORBacus)
		OB::Properties_var props = OB::Properties::getDefaultProperties();
		OB::ParseArgs(argc, argv, props);
		props -> setProperty("ooc.orb.conc_model", "threaded");
		props -> setProperty("ooc.orb.oa.conc_model", "thread_per_request");
	#endif
	#if defined(StarBus)
		STAR::Properties_var props = STAR::Properties::getDefaultProperties();
		STAR::ParseArgs(argc, argv, props);
		props -> setProperty("nis.orb.conc_model", "threaded");
		props -> setProperty("nis.orb.oa.conc_model","thread_per_request");
//		props -> setProperty("nis.orb.oa.conc_model", "threaded");
	#endif
#endif
	//end add xiao heping 2004/04/03
    	
		orb = CORBA::ORB_init(argc, argv);

		CCM::init(orb);
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

