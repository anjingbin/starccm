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

#include <DeployDomainMgr_impl.h>
#include <Configurator_impl.h>

#include <stdlib.h>
#include <errno.h>
#include <fstream>

//jxh
//#include <PerformanceManager_impl.h>

//#include <Manager.h>

using namespace std;
using namespace STARCCM::Deployment;
//using namespace ManagerDomain;

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
	
    bool verbose=false;	   
    CORBA::String_var directory;     
    
    for(int i=0;i<argc;i++)
    {
    	if(strcmp(argv[i], "-verbose") == 0)
    		verbose=true;
    	if(strcmp(argv[i], "-dir") == 0)
    		directory=CORBA::string_dup(argv[i+1]);    			
	
    };
    
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
    
    DeployDomainMgr_impl* DeployDomainMgrImpl = new DeployDomainMgr_impl(verbose,directory.in());
    try
    {
	DeployDomainMgrImpl->initialize();    	  
    }
    catch(const STARCCM::Deployment::InvalidDomainConfFile &)
    {
    	cout<<"DeployDomainMgr initialized failed : DomainConf file invalid."<<endl;
    	if(DeployDomainMgrImpl!=NULL)
    	{
    		delete DeployDomainMgrImpl;
    	}
    	return EXIT_FAILURE;
    }
        		
    PortableServer::ServantBase_var servant = DeployDomainMgrImpl;
    
    //not use the implicit activation
    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId("DeployDomainMgr");
    child->activate_object_with_id(oid,DeployDomainMgrImpl);
    CORBA::Object_var object=child->id_to_reference(oid);
	DeployDomainMgr_var DeployDomainMgr=DeployDomainMgr::_narrow(object);
    
        //
	//jxh---040227
   /* PortableServer::LifespanPolicy_var lifespanp=
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
		child->create_POA("performanceManager",manager,policy_listp);     

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
*/
    	
    //
    // Resolve the BootManager and register the server
    //
#ifdef ORBacus    
    CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
    OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);

    bootManager -> add_binding(oid, DeployDomainMgr);   
    //jxh
    //bootManager -> add_binding(performanceoid, performanceManager); 
#endif   
        
#ifdef StarBus    
    CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
    STAR::AssistAdaptorManager_var assistAdaptorManager = STAR::AssistAdaptorManager::_narrow(obj);

    assistAdaptorManager -> binding_objref(oid, DeployDomainMgr);    
    //jxh
    //assistAdaptorManager -> binding_objref(performanceoid, performanceManager); 
#endif  

#ifdef TAO
    CORBA::Object_var table_object = orb -> resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
      
    IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

	CORBA::String_var iorStr = orb -> object_to_string(DeployDomainMgr);	
	
    adapter->bind ("DeployDomainMgr", iorStr.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
#endif     
    
    //
    ///
    //
//    CORBA::String_var s = orb -> object_to_string(DeployDomainMgr);
//    const char* refFile = "DeployDomainMgr.ref";
//    ofstream out(refFile);
//    if(out.fail())    
//    {	
//	    cout <<": can't open `" << refFile << "': "	<< strerror(errno) << endl;	
//	    return EXIT_FAILURE;
//    }    
//    
//    out << s << endl;    
//    out.close();    
    
    //
    // Run implementation
    //
//#ifdef ORBacus
    manager -> activate();
//#endif
    
    orb -> run();

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[]/*, char* argv[]*/)
{
    
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;
    
    for (int i=0;i<argc;i++)
    {
//	TRACE1(LEVEL6, "DeploymentManager's parameters: %s\n", argv[i]);
	cout << "Parameters is " << argv[i] <<"*"<< endl;
    }
    
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

