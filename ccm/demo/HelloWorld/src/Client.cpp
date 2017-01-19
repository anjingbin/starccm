// **********************************************************************
//
// Copyright (c) 2003
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// Author : CCM Team
//
// Version: 1.0.0
// All Rights Reserved
//
// **********************************************************************

//
//This demo encapsulates the CORBA famous program ---"Hello".
//

#include <CORBA.h>

#include <HelloWorld.h>
#include <stdlib.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{ 
	Components::HomeFinder_var HomeFinder_;	
	HelloWorld::Person_var person;  	
	CORBA::Object_var obj; 
	
    obj = orb->string_to_object("corbaloc::127.0.0.1:9992/HomeFinder");
  
    if( CORBA::is_nil(obj) )    
    {
		TRACE0(LEVEL1,"Can NOT get HomeFinder\n");
		return EXIT_FAILURE;
    }
    
    HomeFinder_ =Components::HomeFinder::_narrow(obj);
    assert(!CORBA::is_nil(HomeFinder_));
    
    Components::CCMHome_var home;    
    
    //find a PersonHome.
    try
    {
       home = HomeFinder_->find_home_by_name("HelloWorld");
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\n";
    	return EXIT_FAILURE;
    };
        
    assert(!CORBA::is_nil(home));    
    HelloWorld::PersonHome_var personHome = HelloWorld::PersonHome::_narrow(home);
    assert(!CORBA::is_nil(personHome));     
    TRACE0(LEVEL1,"Creating component person using homes\n");       
    
    //create a component.
    try
    {   
       person = personHome->create();   
    }
     catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\ncreate person failure\n";
    	return EXIT_FAILURE;
    };   
     
    //
    // Main loop
    //
    cout << "Enter 'h' to call remote sayhello() method or 'x' to exit:\n";
    char c;
    do
    {
		cout << "> ";
		cin >> c;
		if(c == 'h')
		{
			//support         
		    char* s = person->sayHello();
			cout<<s<<endl;
		}
    }
    while(cin.good() && c != 'x'); 
    
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
		    TRACE1(LEVEL1,"Exception occured : %s\n",ex);
		    status = EXIT_FAILURE;
		}
    }
    
    return status;
}
