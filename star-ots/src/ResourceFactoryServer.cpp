// **********************************************************************
//
// Copyright (c) 2002
// S613.NUDT.                    
// CHANGSHA,CHINA    
//
// All Rights Reserved
//
// **********************************************************************

#include <STAR/CORBA.h>
#include <STAR/AssistAdaptorManager.h>

#include <CosTransactionsOTS.h>
#include <Resource_impl.h>

#include <stdlib.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

int
main(int argc, char* argv[])
{
    try
    {
	//
	// Create ORB and BOA
	//
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

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
	ResourceFactory_impl* ResourceFactoryImpl = new ResourceFactory_impl(orb);
	PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId("ResourceFactory");
    child->activate_object_with_id(oid,ResourceFactoryImpl);
    CORBA::Object_var RF=child->id_to_reference(oid);
    //ResourceFactory_var RF = ResourceFactoryImpl -> _this();	

    //
    // Resolve the AssistAdaptorManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
    STAR::AssistAdaptorManager_var assistAdaptorManager = STAR::AssistAdaptorManager::_narrow(obj);

    assistAdaptorManager -> binding_objref(oid, RF);
    //jxh


	//
	// Save reference
	//
	CORBA::String_var S_RF = orb -> object_to_string(RF);
	
	char* refFile = new char[50];
	strcpy(refFile, "rf.ref");
	
	if (argc == 3)
	{
		if (!strcmp(argv[1], "-ref"))
			strcpy(refFile, argv[2]);
	}

	ofstream out(refFile);

	if(out.fail())
	{
	    //extern int errno;
	    //cerr << argv[0] << ": can't open `" << refFile << "': "
		// << strerror(errno) << endl;
	    return 1;
	}
	
	out << S_RF << endl;
	out.close();
	
	//
	// Run implementation
	//
	
	manager -> activate();
	orb -> run();
	return EXIT_SUCCESS;
    }

#ifdef __GNUG__
    catch(CORBA::COMM_FAILURE& ex)
#else
    catch(CORBA::SystemException& ex)
#endif
    {
	cerr << ex << endl;
	return 1;
    }

    return 0;
}

