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

#include <STAR/PI_init.h>
#include <STAR/PICurrent.h>
#include <STAR/STARORBInit.h>
#include <STAR/AssistAdaptorManager.h>
#include <STAR/PIORBInit.h>

#include <OTSIORInterceptor_impl.h>
#include <OTSPolicyFactory_impl.h>

#include <CosTransactionsOTS_impl.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

//renyi 6 17
#ifndef _DEBUG_
#define _DEBUG_
#endif

using namespace STAR;

/*namespace PortableInterceptor
{

//
// The OTS ORB initializer
//
class OTSServerORBInitializer_impl : public PortableInterceptor::ORBInitializer,
                                     CORBA::LocalObject //renyi 12 2 : added
{
    //
    // We want to avoid retaining state in the ORBinitializer since
    // there are no guarantees in the specification regarding
    // concurrency of pre_init and post_init calls.
    //

public:

    OTSServerORBInitializer_impl()
    {
    }
    
    ~OTSServerORBInitializer_impl()
    {
    }

    //
    // Standard IDL to C++ Mapping
    //
    virtual void
    pre_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
		#ifdef _DEBUG_
			cout << "[OTSServerORBInitializer_impl::pre_init begin!]" << endl;
		#endif

		//
		// Create the CDR codec
		//
        IOP::CodecFactory_var codecFactory = info -> codec_factory();

        IOP::Encoding encoding;
        encoding.format = IOP::ENCODING_CDR_ENCAPS;
        encoding.major_version = 1;
        encoding.minor_version = 2;
        IOP::Codec_var codec = codecFactory -> create_codec(encoding);

		//
		// Register the OTS IOR interceptor
		//
        PortableInterceptor::IORInterceptor_var iorInterceptor =
			new PortableInterceptor::OTSIORInterceptor_impl(codec);
        info -> add_ior_interceptor(iorInterceptor);

		//
		// Register a policy factory to create the OTS policies
		//
        PortableInterceptor::PolicyFactory_var txnPolicyFactory =
			new PortableInterceptor::OTSPolicyFactory_impl();
        info -> register_policy_factory(CosTransactions::OTS_POLICY_TYPE, txnPolicyFactory);

		#ifdef _DEBUG_
			cout << "[TOSServerORBInitializer_impl::pre_init end!]" << endl;
		#endif
    }
    
    virtual void
    post_init(PortableInterceptor::ORBInitInfo_ptr)
    {
    }
};

} // End namespace PortableInterceptor
*/  //renyi 12 18 : removed

int
main(int argc, char* argv[])
{
    CORBA::ORB_var orb;
    //
    //create recovery file
    //
    
    #ifdef _RECOVERY_
	const char* path = "./recovery_file";
	FILE *RecoveryFile = fopen(path,"w");
	
	if (RecoveryFile == NULL)
	{
		#ifdef _DEBUG_
			cout<<"[can't open file:  "<<path <<"  with mode of write!]"<<endl;
		#endif
		return 0; 
	}
	else
	{
		#ifdef _DEBUG_
			cout<<"[succeed open file:  "<<path <<"  with mode of write!]"<<endl;
		#endif
		
		int tran_num = 0;
		//when begin to start ots, the transaction number is 0.
		fwrite((int *)&tran_num,sizeof(int),1,RecoveryFile);
		fclose(RecoveryFile);
	}
    #endif
	
        
    try
    {
	/*
    //renyi 6 7:added
    PortableInterceptor::ORBInitializer_var orbInitializer =
	    new PortableInterceptor::OTSServerORBInitializer_impl();

	PortableInterceptor::register_orb_initializer(orbInitializer);
	*/ //renyi 12 18 : removed
	
	//
	// Create ORB and POA
	//
	orb = CORBA::ORB_init(argc, argv);

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
	CosTransactions::TransactionFactory_impl* TransactionFactoryImpl = new CosTransactions::TransactionFactory_impl(orb);
	PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId("TransactionFactory");
    child->activate_object_with_id(oid,TransactionFactoryImpl);
    CORBA::Object_var TF=child->id_to_reference(oid);
    //CosTransactions::TransactionFactory_var TF = TransactionFactoryImpl -> _this();

    //
    // Resolve the AssistAdaptorManager and register the server
    //
    CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
    STAR::AssistAdaptorManager_var assistAdaptorManager = STAR::AssistAdaptorManager::_narrow(obj);

    assistAdaptorManager -> binding_objref(oid, TF);
    //jxh

	//
	// Save reference
	//
	CORBA::String_var S_TF = orb -> object_to_string(TF);
	
	char* refFile = new char[50];
	strcpy(refFile, "ots.ref");
	
	if (argc == 3)
	{
		if (!strcmp(argv[1], "-ref"))
			strcpy(refFile, argv[2]);
	}

	ofstream out(refFile);

	if(out.fail())
	{
	    //extern int errno;
	    //cerr << argv[0] << ": can't open '" << refFile << "': "
	//	 << strerror(errno) << endl;
	    return 1;
	}
	
	out << S_TF << endl;
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

