// **********************************************************************
//
// Copyright (c) 2003
// 
// StarCCM test example.
// Test the CCM Receptacle.
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

//#include <direct.h>

#include <CommonFunc.h>

Components::HomeFinder_var HomeFinder_;

Test::Person_var person;  
Test::Talk_var  talk;
CORBA::ORB_var orb;

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif
void slog(const char* slog)
{
	ofstream f("receptacle_test.out",ios_base::out | ios_base::app );
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
	
    /*********   outfut file init   *****************/
    ofstream f("receptacle_test.out");
    f.close();
    
    slog("\n********  test  receptacle begin ! ********\n");
    cout<<"\n********  test  receptacle begin ! ********\n";
    
    
    /***************   get ip   *********************/
    CORBA::String_var ip = CORBA::string_dup("corbaloc::");
	ip += HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");
	ip += CORBA::string_dup(":");
	ip += HelpFun::GetCCMProfileString("domain","HomeFinderPort","","starccm.properties");	
	ip += CORBA::string_dup("/HomeFinder");	
    	

    /***********   get homefinder  ******************/
    CORBA::Object_var obj = orb -> string_to_object(ip.in());   
    
    if( CORBA::is_nil(obj) )    
    {
	TRACE0(LEVEL1,"Can NOT get HomeFinder\n");
	return EXIT_FAILURE;
    }
    HomeFinder_ =Components::HomeFinder::_narrow(obj);
    assert(!CORBA::is_nil(HomeFinder_));
    
    
    /*********  create component person   ************/
    Components::CCMHome_var home;
    try
    {
       //home = HomeFinder_->find_home_by_component_type("IDL:Test/Person:1.0");
       home = HomeFinder_->find_home_by_name("Example1");
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\n";
    	return EXIT_FAILURE;
    };    
    assert(!CORBA::is_nil(home));    
    Test::PersonHome_var personHome = Test::PersonHome::_narrow(home);
    assert(!CORBA::is_nil(personHome));
         
    slog("Creating component person using homes");
    cout<<"\nCreating component person using homes\n"; 
           
    try
    {   
       person = personHome->create();   
    }
     catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\ncreate person failure\n";
    	return EXIT_FAILURE;
    }; 
     
     
    /********   create component talk   ***********/
    try
    {
       //home = HomeFinder_->find_home_by_component_type("IDL:Test/Talk:1.0");
       home = HomeFinder_->find_home_by_name("Example2");
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\n";
    	return EXIT_FAILURE;
    };    
    assert(!CORBA::is_nil(home));    
    Test::TalkHome_var talkHome = Test::TalkHome::_narrow(home);
    assert(!CORBA::is_nil(talkHome)); 
        
    slog("Creating component talk using homes");
    cout<<"Creating component talk using homes\n";    
     
    try
    {   
       talk = talkHome->create();   
    }
     catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\ncreate component talk failure\n";
    	return EXIT_FAILURE;
    }; 
    
             
    Test::Hello_var hello;
    
    hello=talk->provide_the_hello();
    
    char* s=hello->sayHello();
    

    obj=talk->provide_facet("the_hello");
    assert(!CORBA::is_nil(obj));   
    hello=Test::Hello::_narrow(obj);     
   
    s=hello->sayHello();
  
       
    CORBA::ValueFactoryBase_var factory = new Components::FacetDescriptionFactory_impl();
    CORBA::ValueFactoryBase_var oldFactory =
        orb -> register_value_factory("IDL:omg.org/Components/FacetDescription:1.0", factory);
    Components::FacetDescriptions* pSqFacet;
    
    pSqFacet=talk->get_all_facets();
    
   
    Components::NameList nameList;
    nameList.length(1);
    nameList[0]=CORBA::string_dup("the_hello");    
    
    pSqFacet=talk->get_named_facets(nameList);
    
  
    /*********   test connect **************/
    
    slog("\ntest connect");
    cout<<"\ntest connect\n";
    
    bool bCompleted=true;
    try
	{
    person -> connect_the_mouth(hello.in());
	}
   	catch(const CORBA::Exception& ex)
    {
    	bCompleted=false;
    	slog("get_named_facets is not completed");
    	cerr<<ex<<"\n";
    };

    if(bCompleted)
    {
       slog("connect_the_mouth() is OK!");
       cout<<"connect_the_mouth() is OK!\n";
    }


    slog("\ntest excetion of connect");
    cout<<"\ntest excetion of connect\n";
    slog("next should show 'InvalidConnection' excetion ");
    cout<<"next should show 'InvalidConnection' excetion \n"; 

     Test::Hello_var phello;
	 try
    {
    	person -> connect_the_mouth(phello.in());
    }
    catch(Components::InvalidConnection& ex)
    {
    	slog(ex._to_string());
    	cerr<<ex<<"\n";
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\n";
    };
    
    slog("\nnext should show 'AlreadyConnected' excetion "); 
	cout<<"\nnext should show 'AlreadyConnected' excetion "<<endl;

    try
    {
    	person -> connect_the_mouth(hello.in());
    }
    catch(Components::AlreadyConnected& ex)
    {
    	slog(ex._to_string());
    	cerr<<ex<<"\n";
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\n";
    }; 
    
     
    /******   test get_connection_  ****************/
    slog("\ntest get_connection_ ");
	cout<<"\ntest get_connection_ "<<endl;

    phello=person ->get_connection_the_mouth ();
    s="";
    s=phello -> sayHello();

    slog("if next show 'hello',get_connection_ is OK!");
    cout<<"if next show 'hello',get_connection_ is OK!\n";
    slog(s);
    cout<<s<<"\n";
    
    slog("\n********  test  receptacle end ! ********");
    cout<<"\n********  test  receptacle end ! ********\n"; 
    
    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;

	//char cwd[64];
	//cout << "pwd = " << getcwd(cwd, 64) << endl;

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
