// **********************************************************************
//
// Copyright (c) 2002-2003 
// StarMiddleware.net	
// www.StarMiddleware.net
// 
// All Rights Reserved
//
// Author: Ren Yi	starots@sohu.com	
// **********************************************************************

// Version: 1.0.0
#include <OB/CORBA.h>

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
	
	//
	// Create implementation object
	//
	ResourceFactory_impl* ResourceFactoryImpl = new ResourceFactory_impl(orb);
    ResourceFactory_var RF = ResourceFactoryImpl -> _this();	
		
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

