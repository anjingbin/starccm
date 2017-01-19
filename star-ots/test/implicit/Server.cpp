// **********************************************************************
//
// Copyright (c) 2000
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#include <STAR/CORBA.h>
#include <STAR/Properties.h>

#include <CosTransactionsOTS.h>
#include <ImplicitOTSInterceptor.h>
#include <Resource.h>

#include <math.h>
#include <OTSHeader.h>

#include <Account_impl.h>

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

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
	cout << endl;
	cout << "***********************************************************" << endl;
	cout << "test implicit propagation server side program is running..." << endl;
	cout << "***********************************************************" << endl;
	cout << endl;

    //
    // Resolve Root POA
    //
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(
	CORBA::Object_var(orb -> resolve_initial_references("RootPOA")));

    // 
    // Resolve CosTransactions::Current
    // 
    CosTransactions::Current_var current = 
        CosTransactions::Current::_narrow(
            CORBA::Object_var(orb -> 
                              resolve_initial_references(
                                  "TransactionCurrent")));
    cout << "[get server side current object is ok!]" << endl;
	//
    // Get a reference to the POA manager
    //
    PortableServer::POAManager_var manager = rootPOA -> the_POAManager();

    // 
    // Create a POA with a transactional policy
    // 
    CORBA::Any any;
    CORBA::PolicyList policies(2);
    policies.length(2);

    any <<= PortableServer::IMPLICIT_ACTIVATION;
    policies[0] = 
        orb -> create_policy(PortableServer::IMPLICIT_ACTIVATION_POLICY_ID,
                             any);

    //
    // The account requires a transaction be active
    //
    any <<= CosTransactions::REQUIRES;
    policies[1] = orb -> create_policy(CosTransactions::OTS_POLICY_TYPE, 
				       any);

    PortableServer::POA_var txnPOA = 
        rootPOA -> create_POA("TransactionPOA", manager, policies);
    
    //
    // Create implementation object
    //
    Account_impl* accountImpl = new Account_impl(orb,txnPOA,current);
    PortableServer::ServantBase_var servant = accountImpl;
    Account_var account = accountImpl -> _this();
    
    //
    // Save reference
    //
    CORBA::String_var s = orb -> object_to_string(account);
    
    const char* refFile = "Account.ref";
    ofstream out(refFile);
    if(out.fail())
    {
	cerr << argv[0] << ": can't open `" << refFile << "': "
	     << strerror(errno) << endl;
	return EXIT_FAILURE;
    }
    
    out << s << endl;
    out.close();

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
        STAR::OTSInit(argc, argv);

        //
        // Create a set of ORB level custom properties
        //
        STAR::Properties_var properties = STAR::Properties::getDefaultProperties();
        STAR::ParseArgs(argc, argv, properties);

        //
		// Set the thread_pool concurrency model, if necessary
		//
		properties -> setProperty("nis.orb.conc_model", "threaded");
		const char* concModel =
		properties -> getProperty("nis.orb.oa.conc_model");
		if(concModel != 0 && strcmp(concModel, "thread_per_request") != 0)
		{
			properties -> setProperty("nis.orb.oa.conc_model",
						  "thread_pool");
			if(properties -> getProperty("nis.orb.oa.thread_pool") == 0)
			properties -> setProperty("nis.orb.oa.thread_pool", "10");
		}

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
