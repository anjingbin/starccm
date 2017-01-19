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

#include <ComponentInstallation_impl.h>        
#include <DeployDomainMgr.h>  

#include <stdlib.h>
#include <errno.h>
#include <fstream>

using namespace std;

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{


	//
	//get the argument of ComponentInstallation constructor
	//
	bool verbose=false;	
	CORBA::String_var directory;
	CORBA::String_var domainMgr;
	
    	bool DomainMgrArg=false;
    	bool dirArg=false;
    for(int i=0;i<argc;i++)
    {
    	if(strcmp(argv[i], "-verbose") == 0)
    		verbose=true;
    		
    	if(strcmp(argv[i], "-DomainMgr") == 0)  
    	{  	
    		DomainMgrArg=true;
    		domainMgr=CORBA::string_dup(argv[i+1]);
    	}
    	   	
    	if(strcmp(argv[i], "-dir") == 0)
    	{
    		directory=CORBA::string_dup(argv[i+1]);    			
    		dirArg=true;
    	}
	
    };
    
    if((!DomainMgrArg)||(!dirArg))
    {
    	TRACE0(LEVEL1,"ComponentInstallationServer : arguement of -DomainMgr or -dir is missed.\n");
    	return EXIT_FAILURE;
    }
   
    //
    //resolve DeployDomainMgr refence form its corbaloc 
    //
	CORBA::Object_var mangerobj;
	try
	{
	       mangerobj=orb->string_to_object(domainMgr.in());
        }
        catch(const CORBA::BAD_PARAM&)
        {
        	TRACE0(LEVEL1,"ComponentInstallationServer: invalid DeployDomainMgr corbaloc.\n");        	
        	return EXIT_FAILURE;
        }
	
    	if(CORBA::is_nil(mangerobj))
    	{
        	TRACE0(LEVEL1,"ComponentInstallationServer: cannot get the reference of DeployDomainMgr.\n");    		
		return EXIT_FAILURE;
   	}
	
	DeployDomainMgr_var DomainMgr = DeployDomainMgr::_narrow(mangerobj);
    	assert(!CORBA::is_nil(DomainMgr));    
    
       
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
    Components::Deployment::ComponentInstallation_impl* ComponentInstallationImpl = 
    	new Components::Deployment::ComponentInstallation_impl(DomainMgr.in(),directory.in(),verbose);    
    PortableServer::ServantBase_var servant = ComponentInstallationImpl;
    
    //not use the implicit activation
    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId("ComponentInstallation");
    child->activate_object_with_id(oid,ComponentInstallationImpl);
    CORBA::Object_var object=child->id_to_reference(oid);
	Components::Deployment::ComponentInstallation_var ComponentInstallation=Components::Deployment::ComponentInstallation::_narrow(object);

#ifdef ORBacus
    //
    // Resolve the BootManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
    OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);

    bootManager -> add_binding(oid, ComponentInstallation);
#endif   
    
#ifdef StarBus
    //
    // Resolve the AssistAdaptorManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
    STAR::AssistAdaptorManager_var assistAdaptorManager = STAR::AssistAdaptorManager::_narrow(obj);

    assistAdaptorManager -> binding_objref(oid, ComponentInstallation);
#endif       

#ifdef TAO
    CORBA::Object_var table_object = orb -> resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
      
    IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

	CORBA::String_var iorStr = orb -> object_to_string(ComponentInstallation);	
	
    adapter->bind ("ComponentInstallation", iorStr.in () ACE_ENV_ARG_PARAMETER);
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

