#include <CORBA.h>
#include <CCM.h>
#include <stdlib.h>
#include <CCMObject.h>
#include <Deployment.h>

#include <HelloWorld.h>

#include <iostream>

using namespace std;
using namespace Components::Deployment;

//#ifdef HAVE_STD_IOSTREAM
//using namespace std;
//#endif

int
run(CORBA::ORB_ptr orb, int /* argc */, char* argv[])
{
	
    Components::HomeFinder_var HomeFinder_;	
	HelloWorld::Person_var person;  	
	CORBA::Object_var objHomeFinder; 

	//Get the object reference of HomeFinder!	
    objHomeFinder = orb->string_to_object("corbaloc::127.0.0.1:9992/HomeFinder");
  
    if( CORBA::is_nil(objHomeFinder) )    
    {
		TRACE0(LEVEL1,"Can NOT get HomeFinder\n");
		return EXIT_FAILURE;
    }
    
    HomeFinder_ =Components::HomeFinder::_narrow(objHomeFinder);
    assert(!CORBA::is_nil(HomeFinder_));
    
    Components::CCMHome_var home;    
    
    //Find the home objcet of PersonHome.
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
    // Get "Assembly" object
    //
    CORBA::Object_var objAssembly;

    try
    {
        objAssembly  = orb -> string_to_object("relfile:/c:/ccm/Assembly.ref");
    }
    catch(const CORBA::BAD_PARAM&)
    {
    }

    if(CORBA::is_nil(objAssembly))
    {
	cerr << argv[0] << ": cannot read IOR from Assembly.ref" << endl;
	return EXIT_FAILURE;
    }
    
    Assembly_var Assembly =Assembly::_narrow(objAssembly);

    assert(!CORBA::is_nil(Assembly));

//	Assembly -> tear_down(); 
 
 
 
 	    
    //person is the component!
    //personHome is the component home!
    //Assembly is the assembly object!
    
    //
    // Main loop
    //
    cout << "-----------person is the component!-------------" << endl;
    cout << "--------personHome is the component home!-------" << endl;
    cout << "-------Assembly is the assembly object!---------" << endl << endl;
    
    cout << "-----'s' for person invoke sayHello()," << endl;
    cout << "-----'h' for person remove component:person," << endl;
    cout << "-----'p' for personHome remove component:person," << endl;
//    cout << "-----'r' for personHome remove()." << endl;
    cout << "-----'t' for Assembly tear_down()." << endl;
    cout << "-----'x' for exit." << endl;
    
    char c;
	
    do
    {
	cout << "> ";
	cin >> c;
	
	if(c == 'h')
	    person -> remove();
	else if(c == 'p')
	    personHome -> remove_component(person);
//	else if(c == 'r')
//		personHome -> remove();
	else if(c == 't')
		Assembly -> tear_down();	
	else if(c == 's')
	{
		//support         
	    char* s = person -> sayHello();
	    cout << s << endl;
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
	orb = CORBA::ORB_init(argc, argv, "tear_down");
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