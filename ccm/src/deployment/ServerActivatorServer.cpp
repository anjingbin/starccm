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
#include <Configurator_impl.h>      

#include <stdlib.h>
#include <errno.h>
#include <fstream>
#include <iostream>
//jxh
#include <Manager.h>
#ifdef WIN32
#include <PerformanceManager_impl.h>
#endif


using namespace std;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;
using namespace ManagerDomain;

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
	
   //parse the  argument
   bool verbose=false;	    
   CORBA::String_var serverActivatorUUID;
   CORBA::String_var domainMgr;
   CORBA::String_var directory;     

   for(int i=0;i<argc;i++)
    {
    	if(strcmp(argv[i], "-verbose") == 0)
    	{
    		verbose=true;	
    	}
	if(strcmp(argv[i], "-id") == 0)	
	{
    		serverActivatorUUID=CORBA::string_dup(argv[i+1]);    		    		
    	}
	if(strcmp(argv[i], "-DomainMgr") == 0)	
	{
    		domainMgr=CORBA::string_dup(argv[i+1]);    		
    	}    	
	if(strcmp(argv[i], "-dir") == 0)	
	{
    		directory=CORBA::string_dup(argv[i+1]);    		
    	}    	    	
    }
    
    if((serverActivatorUUID==NULL)||(domainMgr==NULL))
    {
    	TRACE0(LEVEL1,"ServerActivator: the parameter to start the AssemblyFactoy miss -DomainMgr or -id. \n");
    	return  EXIT_FAILURE;
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
        	TRACE0(LEVEL1,"ServerActivator: invalid DeployDomainMgr corbaloc.\n");        	
        	return EXIT_FAILURE;
        }
	
    	if(CORBA::is_nil(mangerobj))
    	{
        	TRACE0(LEVEL1,"ServerActivator: cannot get the reference of DeployDomainMgr.\n");    		
		return EXIT_FAILURE;
   	}
	
	DeployDomainMgr_var DomainMgr = DeployDomainMgr::_narrow(mangerobj);
    	assert(!CORBA::is_nil(DomainMgr));      
 
 
    	//register config value factory    	
    	CORBA::ValueFactoryBase_var configvaluefactory = new ConfigValueFactory_impl();
    	CORBA::ValueFactoryBase_var oldconfigvalueFactory =             
       		orb -> register_value_factory("IDL:omg.org/Components/ConfigValue:1.0", configvaluefactory.in());   	 
 
 
    //
    // Resolve Root POA
    //
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj);
    
    //
    // Get a reference to the POA manager
    //
    PortableServer::POAManager_var manager = rootPOA -> the_POAManager();
    
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
    ServerActivator_impl* ServerActivatorImpl = 
    	new ServerActivator_impl(orb,DomainMgr,serverActivatorUUID.in(),directory.in(),verbose);    
    PortableServer::ServantBase_var servant = ServerActivatorImpl;
    
    //not use the implicit activation
    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId("ServerActivator");
    child->activate_object_with_id(oid,ServerActivatorImpl);
    CORBA::Object_var object=child->id_to_reference(oid);
	ServerActivator_var serveractivator=ServerActivator::_narrow(object);
	
       //jxh 0412
    	//jxh---040227
#ifdef WIN32   
      PortableServer::LifespanPolicy_var lifespanp=
      rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
      
    PortableServer::IdAssignmentPolicy_var idassignmentp=
      rootPOA->create_id_assignment_policy(PortableServer::USER_ID);
    
    CORBA::PolicyList policy_listp;
    policy_listp.length(2);
    policy_listp[0]=
      PortableServer::LifespanPolicy::_duplicate(lifespanp);    
    policy_listp[1]=
      PortableServer::IdAssignmentPolicy::_duplicate(idassignmentp);     
    // PortableServer::POAManager::_nil()      
    PortableServer::POA_var childp=
		rootPOA->create_POA("performanceManager",manager,policy_listp);     

    	lifespanp->destroy();
    idassignmentp->destroy();
    
	PerformanceManager_impl*  PerformanceManagerImpl= new PerformanceManager_impl(verbose);
	if (!PerformanceManagerImpl)
	{
		cout<<"PerformanceManagerImpl is null!!"<<endl;
	}
	PortableServer::ServantBase_var performanceservant = PerformanceManagerImpl;

	PortableServer::ObjectId_var performanceoid=PortableServer::string_to_ObjectId("PerformanceManager");
	childp->activate_object_with_id(performanceoid,PerformanceManagerImpl);
	CORBA::Object_var pobject=childp->id_to_reference(performanceoid);

	CORBA::String_var perfor = orb->object_to_string(pobject);
	cout<<"performance reference is:"<<perfor<<endl;

	PerformanceManager_var performanceManager = PerformanceManager::_narrow(pobject);
//jxh-end
#endif	

#ifdef ORBacus
    //
    // Resolve the BootManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
    OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);

    bootManager -> add_binding(oid, serveractivator);
#ifdef WIN32
    //jxh
    bootManager -> add_binding(performanceoid, performanceManager); 
#endif
#endif 

#ifdef StarBus
    //
    // Resolve the AssistAdaptorManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
    STAR::AssistAdaptorManager_var assistAdaptorManager = STAR::AssistAdaptorManager::_narrow(obj);

    assistAdaptorManager -> binding_objref(oid, serveractivator);
#ifdef WIN32
    //jxh
    assistAdaptorManager -> binding_objref(performanceoid, performanceManager); 
#endif
#endif 

#ifdef TAO
    CORBA::Object_var table_object = orb -> resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
      
    IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

	CORBA::String_var iorStr = orb -> object_to_string(serveractivator);	
	
    adapter->bind ("ServerActivator", iorStr.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
#endif       
        
    //
    // Run implementation
    //
    manager -> activate();
    orb -> run();

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[]/*, char* argv[]*/)
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

