// **********************************************************************
//
// Copyright (c) 2003
// 
// StarCCM test example.
// Test the CCM Session.
//
// All Rights Reserved
//
// **********************************************************************

#include <CORBA.h>

#include <test.h>
#include <stdlib.h>
#include <fstream>
#include "UuidGenerator.h"
#include <CCMCommonType_impl.h> 
#include <TEST_ValueType_impl.h>

#include <CommonFunc.h>

Components::HomeFinder_var HomeFinder_;

Test::Person_var person;  
CORBA::ORB_var orb;

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif
void slog(const char* slog)
{
	ofstream f("session_test.out",ios_base::out | ios_base::app );
	if(!f) 
	{
	      cerr<<"can't open log file!\n";
              return; 
	}
	f<<slog<<"\n";	
	f.close();
}
int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{    
    //file init
    ofstream f("session_test.out");
    f.close();
    slog("session  test\n");
   
     CORBA::String_var ip = CORBA::string_dup("corbaloc::");
	ip += HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");
	ip += CORBA::string_dup(":");
	ip += HelpFun::GetCCMProfileString("domain","HomeFinderPort","","starccm.properties");	
	ip += CORBA::string_dup("/HomeFinder");	

	CORBA::Object_var obj = orb -> string_to_object(ip.in());  
     HomeFinder_ =Components::HomeFinder::_narrow(obj);
        
    assert(!CORBA::is_nil(HomeFinder_));
    Components::CCMHome_var home;    
    //create component person
    try
    {
       home = HomeFinder_->find_home_by_name("Example");
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\n";
    	return EXIT_FAILURE;
    };    
    assert(!CORBA::is_nil(home));    
    Test::PersonHome_var personHome = Test::PersonHome::_narrow(home);
    assert(!CORBA::is_nil(personHome));     
    
    cout<<"\n see the session test ouput in the server window!\n";
           
    try
    {   
       person = personHome->create();   
    }
     catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\ncreate person failure\n";
    	return EXIT_FAILURE;
    };      
   
    
   //test interface Sessioncontext,the test code is in ccm_test_impl.cpp
   char* s=person->sayHello();
   
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
