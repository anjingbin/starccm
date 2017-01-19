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
#include <HomeFinder_impl_tie.h>
#include <HomeFinder_skel_tie.h>
#include <HomeRegistration_skel_tie.h>

#include <stdlib.h>
#include <errno.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

using namespace Components;
using namespace STARCCM;

class POA_HomeFinder_refcount_tie : public POA_Components::HomeFinder_tie< HomeFinder_impl_tie >,
			       public PortableServer::RefCountServantBase
{
public:

    POA_HomeFinder_refcount_tie(HomeFinder_impl_tie* impl, PortableServer::POA_ptr poa)
	: POA_Components::HomeFinder_tie< HomeFinder_impl_tie >(impl, poa, false)
    {
    }

    virtual
    ~POA_HomeFinder_refcount_tie()
    {
    }
};

class POA_HomeRegistration_refcount_tie : public POA_STARCCM::HomeRegistration_tie< HomeFinder_impl_tie >,
			       public PortableServer::RefCountServantBase
{
public:

    POA_HomeRegistration_refcount_tie(HomeFinder_impl_tie* impl, PortableServer::POA_ptr poa)
	: POA_STARCCM::HomeRegistration_tie< HomeFinder_impl_tie >(impl, poa, true)
    {
    }

    virtual
    ~POA_HomeRegistration_refcount_tie()
    {
    }
};

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
    //
    // Resolve Root POA
    //
//    printf("HELLO\n");
//    TRACE0(LEVEL6, "Entering run...\n");
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj);
    
    //
    // Get a reference to the POA manager
    //
    PortableServer::POAManager_var manager = rootPOA -> the_POAManager();
    
    //create the persistent reference
    PortableServer::LifespanPolicy_var lifespan =
      rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
      
    PortableServer::IdAssignmentPolicy_var idassignment =
      rootPOA->create_id_assignment_policy(PortableServer::USER_ID);
    
    CORBA::PolicyList policy_list;
    policy_list.length(2);
    policy_list[0] =
      PortableServer::LifespanPolicy::_duplicate(lifespan);    
    policy_list[1] =
      PortableServer::IdAssignmentPolicy::_duplicate(idassignment);     
           
    PortableServer::POA_var child =
      rootPOA->create_POA("child",manager,policy_list);

	//added by xiao heping 2004/02/15
	lifespan->destroy();
    idassignment->destroy();
	//end added
     
	//modified by xiao heping 2004/02/14

    // HomeFinder_impl_tie* homeFinderTie = new HomeFinder_impl_tie(orb, child.in());
	HomeFinder_impl_tie* homeFinderTie = new HomeFinder_impl_tie();
	
	//end modified
    
    {
        //
        // Create implementation object
        //
        POA_HomeFinder_refcount_tie* homeFinderImpl
        	= new POA_HomeFinder_refcount_tie(homeFinderTie, rootPOA);
        PortableServer::ServantBase_var servant = homeFinderImpl;

		//added by xiao heping 2004/02/15
		PortableServer::ObjectId_var oid =PortableServer::string_to_ObjectId("HomeFinder");
		child->activate_object_with_id(oid,homeFinderImpl);
		CORBA::Object_var object=child->id_to_reference(oid);
		HomeFinder_var homeFinder=HomeFinder::_narrow(object);
		//end added

//        CORBA::Object_var ref = child -> servant_to_reference(servant);
//        HomeFinder_var homeFinder = HomeFinder::_narrow(ref);
//        servant -> _remove_ref();

		//deleted by xiao heping 2004/02/15
        //HomeFinder_var homeFinder = homeFinderImpl -> _this();
		//end deleted
    
        //
        // Resolve the BootManager and register the server
        //

		#ifdef ORBacus
        CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
        OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);

		//deleted by xiao heping 2004/02/15
		//PortableServer::ObjectId_var oid =
        //    PortableServer::string_to_ObjectId("HomeFinder");
		//end deleted
        bootManager -> add_binding(oid, homeFinder);
		#endif

		#ifdef StarBus

        CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
        STAR::AssistAdaptorManager_var bootManager = STAR::AssistAdaptorManager::_narrow(obj);
    
		//deleted by xiao heping 2004/02/15
		//PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId("HomeFinder");
		//end deleted
     
        bootManager -> binding_objref(oid, homeFinder);
		#endif 

        //
        // Save reference
        //

		//deleted by xiao heping 2004/02/15
		/*
        CORBA::String_var s = orb -> object_to_string(homeFinder);
        
        const char* refFile = "HomeFinder.ref";
        ofstream out(refFile);
        if(out.fail())
        {
    	cerr << argv[0] << ": can't open `" << refFile << "': "
    	     << strerror(errno) << endl;
    	return EXIT_FAILURE;
        }
    
        out << s << endl;
        out.close();
		*/
		//end deleted
    }

    {
        //
        // Create implementation object
        //

		//modified by xiao heping 2004/02/15
        //POA_HomeRegistration_refcount_tie* homeRegistrationImpl
        //	= new POA_HomeRegistration_refcount_tie(homeFinderTie, rootPOA);
		POA_HomeRegistration_refcount_tie* homeRegistrationImpl
        	= new POA_HomeRegistration_refcount_tie(homeFinderTie, child);
		//end modified

        PortableServer::ServantBase_var servant = homeRegistrationImpl;

		//deleted by xiao heping 2004/02/15
        //HomeRegistration_var homeRegistration = homeRegistrationImpl -> _this();

		//added by xiao heping 2004/02/15
		PortableServer::ObjectId_var oid =PortableServer::string_to_ObjectId("HomeRegistration");
		child->activate_object_with_id(oid,homeRegistrationImpl);
		CORBA::Object_var object=child->id_to_reference(oid);
		HomeRegistration_var homeRegistration=HomeRegistration::_narrow(object);
		//end added
    
        //
        // Resolve the BootManager and register the server
        //
		#ifdef ORBacus
        CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
        OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);

		//deleted by xiao heping 2004/02/15
		//PortableServer::ObjectId_var oid =
        //    PortableServer::string_to_ObjectId("HomeRegistration");
		//end deleted

        bootManager -> add_binding(oid, homeRegistration);
		#endif

		#ifdef StarBus

        CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
        STAR::AssistAdaptorManager_var bootManager = STAR::AssistAdaptorManager::_narrow(obj);
    
		//deleted by xiao heping 2004/02/15
        //PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId("HomeRegistration");
		//

        bootManager -> binding_objref(oid, homeRegistration);
		#endif 

        //
        // Save reference
        //

		//deleted by xiao heping 2004/02/15
		/*
        CORBA::String_var s = orb -> object_to_string(homeRegistration);
        
        const char* refFile = "HomeRegistration.ref";
        ofstream out(refFile);
        if(out.fail())
        {
    	cerr << argv[0] << ": can't open `" << refFile << "': "
    	     << strerror(errno) << endl;
    	return EXIT_FAILURE;
        }
    
        out << s << endl;
        out.close();
		*/
		//end deleted
    }
    
    //
    // Run implementation
    //
    manager -> activate();
    orb -> run();

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;
    

    try
    {
//        printf("in main\n");
//	TRACE0(LEVEL6,"yes!\n");
//	printf("before orb init\n");
	orb = CORBA::ORB_init(argc, argv);
//	printf("after orb init\n");
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
