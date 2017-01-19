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
#include <CosTransactionsOTS.h>
#include <ImplicitOTSInterceptor.h>

#include <CommonFunc.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif


//slog function is to write test result to file.
void slog(const char* loginfor)
{
	ofstream f("NOT_SUPPORTED_Client.txt",ios_base::out | ios_base::app );
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
    ofstream f("NOT_SUPPORTED_Client.txt");
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
    // Get the CosTransactions::Current
    //
    CosTransactions::Current_var current =
	CosTransactions::Current::_narrow(
	    CORBA::Object_var(
		orb -> resolve_initial_references("TransactionCurrent")));
    cout << "	[get current object is ok!]" << endl;
    
     
    //
    // Main loop
    //

    char c;
    do
    {
	    cout << "Enter 'y' to call remote sayhello() method within a Transaction," << endl;
	    cout << "Enter 'n' to call remote sayhello() method without a Transaction," << endl;
	    cout << "or 'x' to exit!" << endl;
		cout << "> ";
		cin >> c;
		if(c == 'y')
		{
			try
			{   
				current -> begin();
				slog("Client begin a new transaction.\n");    
			    char* s = _person -> sayHello();
			    slog("Client invoke the component sayHello() method within a transaction.\n");
				cout << s << endl;
				current -> commit(false);
				slog("Client commit a transaction.\n");				
			}
			catch(...)
			{
				cout << "The not_supported policy raises other exeception." << endl;
				slog("The not_supported policy raises other exeception.\n");
			}
			cout << "Method invoke is within a transaction,and correct!" << endl;
			slog("Method invoke is within a trancation,and correct!\n");
			slog("\n\n");
		}
		if(c == 'n')
		{
			try
			{       
			    char* s = _person -> sayHello();
			    slog("Client invoke the component sayHello() method without a transaction.\n");
				cout << s << endl;
			}
			catch(...)
			{
				cout << "The not_supported policy raises other exeception." << endl;
				slog("The not_supported policy raises other exeception.\n");
			}
			cout << "Method invoke is without a transaction,and correct!" << endl;
			slog("Method invoke is without a trancation,and correct!\n");
			slog("\n\n");		
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
    	STAR::OTSInit(argc, argv);
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
