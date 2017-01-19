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

#include <stdlib.h>
#include <errno.h>
#include <fstream>

#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <DynDeploymentMgr_impl.h>

using namespace std;
using namespace STARCCM::Deployment;
using namespace STARCCM::DynDeployment;

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{	
   //parse the  argument
   bool verbose=false;	    
   CORBA::String_var domainMgr;
   CORBA::String_var directory;
    
   bool DomainMgrArg=false;
   bool dirArg=false;
    
   for(int i=0;i<argc;i++)
   {
		if(strcmp(argv[i], "-verbose") == 0)
    	{
    		verbose=true;	
    	}
		if(strcmp(argv[i], "-DomainMgr") == 0)
		{
	    	domainMgr=CORBA::string_dup(argv[i+1]);
	    	DomainMgrArg=true;
    	}
		if(strcmp(argv[i], "-dir") == 0)
	{
    		directory=CORBA::string_dup(argv[i+1]);
    		dirArg=true;
    	}
   	    	
    };
 		
    if((!dirArg)||(!DomainMgrArg))
    {
    	TRACE0(LEVEL1,"DynDeploymentMgr: the parameter to start the DynDeploymentMgr miss -dir or -DomainMgr. \n");
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
        	TRACE0(LEVEL1,"AssemblyFactoryServer: invalid DeployDomainMgr corbaloc.\n");        	
        	return EXIT_FAILURE;
        }
	
    	if(CORBA::is_nil(mangerobj))
    	{
        	TRACE0(LEVEL1,"AssemblyFactoryServer: cannot get the reference of DeployDomainMgr.\n");    		
		return EXIT_FAILURE;
   	}
	
	DeployDomainMgr_var DomainMgr = DeployDomainMgr::_narrow(mangerobj);
    	assert(!CORBA::is_nil(DomainMgr));   
  
    //register cookie value factory
    CORBA::ValueFactoryBase_var cookiefactory = new Components::CookieFactory_impl();
    CORBA::ValueFactoryBase_var oldcookieFactory =
    	orb -> register_value_factory("IDL:omg.org/Components/Cookie:1.0", cookiefactory);  
   
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
    DynDeploymentMgr_impl* DynDeploymentMgrImpl = 
    	new DynDeploymentMgr_impl(orb,child,DomainMgr.in(),directory.in(),verbose);    
    PortableServer::ServantBase_var servant = DynDeploymentMgrImpl;
    
    //not use the implicit activation
    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId("DynDeploymentMgr");
    child->activate_object_with_id(oid,DynDeploymentMgrImpl);
    CORBA::Object_var object=child->id_to_reference(oid);
	DynDeploymentMgr_var DynDeployMgr=DynDeploymentMgr::_narrow(object);

#ifdef ORBacus
    //
    // Resolve the BootManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
    OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);

    bootManager -> add_binding(oid, DynDeployMgr);
#endif    

#ifdef StarBus
    //
    // Resolve the AssistAdaptorManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
    STAR::AssistAdaptorManager_var assistAdaptorManager = STAR::AssistAdaptorManager::_narrow(obj);

    assistAdaptorManager -> binding_objref(oid, DynDeployMgr);
#endif   
        
    //
    // Run implementation
    //
    manager -> activate();
    orb -> run();

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[], char*[])
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

