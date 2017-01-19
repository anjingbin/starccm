// **********************************************************************
//
// Copyright (c) 2003
// 
// StarCCM test example.
// Test the CCM Facet.
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

//#include <direct.h>

Components::HomeFinder_var HomeFinder_;

Test::Person_var person;  
Test::Talk_var  talk;
CORBA::ORB_var orb;

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif
void slog(const char* slog)
{
	ofstream f("facet_test.out",ios_base::out | ios_base::app );
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
    ofstream f("facet_test.out");
    f.close();
    slog("**********facet  test begin!********\n");
    cout<<"\n***********facet  test begin!********\n";
    
    
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
    
         
    /***************    test provide   *********************/
    
    /*****   通过等价接口provide_<name>  **********/
    
    Test::Hello_var hello;
    slog("\ntest the provide--通过等价接口provide_<name>\n");
    cout<<"\ntest the provide--通过等价接口provide_<name>\n";
    
    try
    {
    	hello=talk->provide_the_hello();
    } 
    catch(const CORBA::Exception& )
    {
    	slog("can't get object through 'provide_the_hello'\n");
    }
    
    slog("next shuold show 'Hello',otherwise the facet can't be used");
    cout<<"next shuold show 'Hello',otherwise the facet can't be used\n";
    
    char* s=hello->sayHello();
    slog(s);
    cout<<s<<"\n";
    //printf("%s\n",s); 
    
       
    /********    通过导航接口的provide_facet  ************/
    
    slog("\ntest the provide--通过导航接口的provide_facet");
    cout<<"\ntest the provide--通过导航接口的provide_facet\n";
     
    try
    {
    	obj=talk->provide_facet("the_hello");
    } 
    catch(const CORBA::Exception& )
    {
    	slog("can't get object through 'provide_facet'\n");
    }
    
    assert(!CORBA::is_nil(obj));   
    hello=Test::Hello::_narrow(obj);     
    slog("next shuold show 'Hello',otherwise the provide_facet can't be used");
    cout<<"next shuold show 'Hello',otherwise the provide_facet can't be used\n";
    
    s=hello->sayHello();
    slog(s);  
    cout<<s<<"\n";
    
    
    /**********    test  get_all_facets()    ****************/ 
    
    slog("\ntest get_all_facets()");
    cout<<"\ntest get_all_facets()\n";
    
    bool bCompleted=TRUE;
    
    CORBA::ValueFactoryBase_var factory = new Components::FacetDescriptionFactory_impl();
    CORBA::ValueFactoryBase_var oldFactory =
        orb -> register_value_factory("IDL:omg.org/Components/FacetDescription:1.0", factory);
    Components::FacetDescriptions* pSqFacet;
    try
    {
    	pSqFacet=talk->get_all_facets();
    }
    catch(const CORBA::Exception& ex)
    {
    	bCompleted=FALSE;
    	slog("get_all_facets is not completed");
    	cerr<<ex<<"\n";
    }
    if(bCompleted)
    {
       slog("get_all_facets() is OK!");
       cout<<"get_all_facets() is OK!\n";
    }


    /*********     test get_named_facets (in NameList names) ****/
    
    slog("\ntest get_named_facets()");
    cout<<"\ntest get_named_facets()\n";
    
    bCompleted=TRUE;
    Components::NameList nameList;
    nameList.length(1);
    nameList[0]=CORBA::string_dup("the_hello");    
    try
    {
    	pSqFacet=talk->get_named_facets(nameList);
    }
    catch(const CORBA::Exception& ex)
    {
    	bCompleted=FALSE;
    	slog("get_named_facets is not completed");
    	cerr<<ex<<"\n";
    };
    if(bCompleted)
    {
       slog("get_named_facets() is OK!");
       cout<<"get_named_facets() is OK!\n";
    }  
   
    
    /*********  test same_component (in Object object_ref) ******/
    
    slog("\ntest same_component()");
    cout<<"\ntest same_component()\n";
    
    bool sameCom=false;
    try
    {
    	sameCom=talk->same_component(hello);
    }
    catch(const CORBA::Exception& ex)
    {
    	sameCom=false;
    	slog("same_component is not completed");
    	cerr<<ex<<"\n";
    }; 
    if(sameCom)
    {
        slog("same_component() is ok!");
        cout<<"same_component() is ok!"<<endl;
    }
    
    
    /*******    test exception of provide_facet   ********/
    
    slog("\ntest exception of provide_facet");
    slog("next should show 'InvalidName'");
    cout<<"\ntest exception of provide_facet\n";
    cout<<"next should show 'InvalidName'\n";
    
    try
    {
    	obj=talk->provide_facet("hello");
    }
    catch(const Components::InvalidName& ex)
    {
    	slog(ex._to_string());
    	cout<<ex<<"\n";
    }     
    catch(const CORBA::Exception& )
    {
    	slog("can't get object through 'provide_facet'\n");
    }
    
    slog("\n*******   test facet end!  ********");
    cout<<"\n*******   test facet end!  ********\n";
    
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
