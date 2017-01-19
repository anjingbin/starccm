// **********************************************************************
//
// Copyright (c) 2003
// 
// StarCCM test example.
// Test the CCM CMT.
//
// All Rights Reserved
//
// **********************************************************************

#include <CORBA.h>

#include <HelloWorld.h>
#include <stdlib.h>
#include <fstream>
#include <CosTransactions.h>

#include <CommonFunc.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif


//slog function is to write test result to file.
void slog(const char* loginfor)
{
	ofstream f("SMT_Client.txt",ios_base::out | ios_base::app );
	if(!f) 
	{
		cerr << "can't open log file!\n";
	    return ; 
	}
	f << loginfor << "\n";	
	f.close();
}

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
	Components::HomeFinder_var _HomeFinder;
	HelloWorld::PersonHome_var _personHome;
	HelloWorld::Person_var _person;     
	
    //file init
    ofstream f("SMT_Client.txt");
    f.close();
    slog("transaction test begin!");
        
    //get the HomFinder IP,and save it.
	CORBA::String_var ip = CORBA::string_dup("corbaloc::");
	ip += HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");
	ip += CORBA::string_dup(":");
	ip += HelpFun::GetCCMProfileString("domain","HomeFinderPort","","starccm.properties");	
	ip += CORBA::string_dup("/HomeFinder");
	
    //get the HomeFinder object.
	CORBA::Object_var obj = orb -> string_to_object(ip.in());  
    _HomeFinder = Components::HomeFinder::_narrow(obj);        
    assert(!CORBA::is_nil(_HomeFinder));        
    
    //find PersonHome from HomeFinder.
    Components::CCMHome_var home;
    try
    {
       home = _HomeFinder -> find_home_by_name("HelloWorld");
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr << ex << "\n";
    	return EXIT_FAILURE;
    };
       
    assert(!CORBA::is_nil(home));    
    _personHome = HelloWorld::PersonHome::_narrow(home);
    assert(!CORBA::is_nil(_personHome));
    
    //create a component.
    try
    {   
       _person = _personHome -> create();   
    }
     catch(const CORBA::Exception& ex)
    {
    	cerr<< ex <<"\ncreate person failure\n";
    	return EXIT_FAILURE;
    };   
     
    //
    // Main loop
    //    
    char c;
    do
    {
    	cout << "Enter 'h' to call remote sayhello() method or 'x' to exit:\n";
		cout << "> ";
		cin >> c;
		if(c == 'h')
		{
			try
			{       
			    char* s = _person -> sayHello();
			    slog("Client invoke the component sayHello() method without a transaction.\n");
				cout << s << endl;
			}
			catch(...)
			{
				cout << "The SMT policy raises other exeception." << endl;
				slog("The SMT policy raises other exeception.\n");
			}
			cout << "Method invoke is correct!" << endl;
			slog("Method invoke is correct!\n");
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
