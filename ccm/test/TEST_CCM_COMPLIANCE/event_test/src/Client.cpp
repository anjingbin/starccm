// **********************************************************************
//
// Copyright (c) 2003
// 
// StarCCM test example.
// Test the CCM Event.
//
// All Rights Reserved
//
// **********************************************************************

#include <CORBA.h>

#include <test.h>
#include <stdlib.h>
#include <fstream>
#include "UuidGenerator.h"

#include <CommonFunc.h>

//#include <Configurator_impl.h>
#include <test_ValueType_impl.h>
#include <CCMCommonType_impl.h>

Components::HomeFinder_var HomeFinder_;

Test::Philosopher_var phi;
Test::Philosopher_var phi2;  
Test::Observer_var  observer;
CORBA::ORB_var orb;

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif
void slog(const char* slog)
{
	ofstream f("event_test.out",ios_base::out | ios_base::app );
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
    /************      file init  **************************/
    ofstream f("event_test.out");
    f.close(); 
	slog("******************   test event begin! *********");
	cout<<"\n******************   test event begin! *********\n";
    

	/******************   get ip   ***********************/
   CORBA::String_var ip = CORBA::string_dup("corbaloc::");
	ip += HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");
	ip += CORBA::string_dup(":");
	ip += HelpFun::GetCCMProfileString("domain","HomeFinderPort","","starccm.properties");	
	ip += CORBA::string_dup("/HomeFinder");	


	/*************** get homefinder **********************/

	CORBA::Object_var obj = orb -> string_to_object(ip.in());    
    HomeFinder_ =Components::HomeFinder::_narrow(obj);
    assert(!CORBA::is_nil(HomeFinder_));
       
    
	/***************  Creating component Philosopher using homes  **********/

	Components::CCMHome_var home;  
   	try
    {
       //home = HomeFinder_->find_home_by_component_type("IDL:Test/Person:1.0");
       home = HomeFinder_->find_home_by_name("Example3");
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\n";
    	return EXIT_FAILURE;
    };    
		
	Test::PhilosopherHome_var phiHome = Test::PhilosopherHome::_narrow(home);
	phi = phiHome->create();

	slog("\nCreating component Philosopher using homes");
	cout<<"\nCreating component Philosopher using homes\n";
    

	/***************  Creating component Observer using homes  **********/
	try
    {
       	//home = HomeFinder_->find_home_by_component_type("IDL:Test/Observer:1.0");
	    home = HomeFinder_->find_home_by_name("Example4");
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\n";
    	return EXIT_FAILURE;
    };   

    Test::ObserverHome_var obHome = Test::ObserverHome::_narrow(home);
    observer = obHome->create();
	
    slog("Creating component Observer using homes");
	cout<<"Creating component Observer using homes\n";


	//建立连接
    ::Components::EventConsumerBase_var eventConsumerBase;    
    
   //返回一个事件使用者接口，该使用者仅接收在consume中声明的事件类型  
       
   CORBA::ValueFactoryBase_var factory = new Components::CookieFactory_impl();
   CORBA::ValueFactoryBase_var oldFactory =
        orb -> register_value_factory("IDL:omg.org/Components/Cookie:1.0", factory);

   CORBA::ValueFactoryBase_var factory2 = new Test::PhilosopherStateFactory_impl();
   CORBA::ValueFactoryBase_var oldFactory2 =
        orb -> register_value_factory("IDL:Test/PhilosopherState:1.0", factory2);
   

   /****************  test get_consumer_philosopher_state ************/  
   slog("\ntest get_consumer_philosopher_state()");
   cout<<"\ntest get_consumer_philosopher_state()\n";

    try
    {
    	 eventConsumerBase = observer -> get_consumer_philosopher_state();    
    }
    catch(const CORBA::Exception& ex)
    {
		slog("there is sth wrong in the method get_consumer_philosopher_state");
		cerr << ex << endl;
    };
    assert(!CORBA::is_nil(eventConsumerBase));

    ::Test::PhilosopherStateConsumer_var consumer 
    	= ::Test::PhilosopherStateConsumer::_narrow(eventConsumerBase);    

    assert(!CORBA::is_nil(consumer)); 
	
	slog("get_consumer_philosopher_state() is OK!");
	cout<<"get_consumer_philosopher_state() is OK!\n";
	
	
    //将事件使用者连接到由容器提供给发送构件的事件通道。负责publish的构件应是这个通道的唯一publisher
    //connects the consumer parameter to an event channel provided to th.e component implementation by the container. 
    //The component shall be the only publisher to that channel.


	/*************   test subscribe_phi_pub_state() *************/
    Components::Cookie_var ck;

    try
    {
       ck=phi -> subscribe_phi_pub_state(consumer.in());
    }
    catch(const Components::ExceededConnectionLimit& ex)
    {
    	cerr << ex << endl;
    }
    catch(const CORBA::Exception& ex)
    {
		slog("there is sth wrong in the method subscribe_phi_pub_state");
		cerr << ex << endl;
    };
     

	/****************    test publish ********************/
	
	slog("\ntest publish");
	cout<<"\ntest publish\n";

	slog("next should show 'the publish is ok!'");
	cout<<"next should show 'the publish is ok!' in the server window\n";

     try
    {
       phi -> send_message("pub"); 
    }
    catch(const CORBA::Exception& ex)
    {		
		cerr << ex << endl;
    };

     
//解除连接

     /********** unsubscribe_phi_pub_state()  **************/

     slog("\ntest unsubscribe_phi_pub_state()");
	 cout<<"\ntest unsubscribe_phi_pub_state()\n";

	 bool unsub_state = true ;

	 try
    {
       phi -> unsubscribe_phi_pub_state(ck);
    }
    catch(const Components::InvalidConnection& ex)
    {
		unsub_state = false;
    	cerr << ex << endl;
     }
    catch(const CORBA::Exception& ex)
    {
		unsub_state = false;
		slog("there is sth wrong in the method unsubscribe_phi_pub_state");
		cerr << ex << endl;
    };

	if(unsub_state)
	{
		slog("unsubscribe_phi_pub_state() is OK!");
		cout<<"unsubscribe_phi_pub_state() is OK!\n";
	}	

	 
	/**********************  test connect_phi_emit_state() *********/

	slog("\ntest connect_phi_emit_state()");
	cout<<"\ntest connect_phi_emit_state()\n";
	
	phi2 = 	phiHome->create();  
 
	bool connc_state = true ; 

    try
    {
       phi2 -> connect_phi_emit_state(consumer.in());
    }
    catch(const CORBA::Exception& ex)
    {
		connc_state = false ;
	    slog("there is sth wrong in the method connect_phi_emit_state");
    	cerr << ex << endl;
    };

	if(connc_state)
	{
		slog("connect_phi_emit_state() is OK!");
		cout<<"connect_phi_emit_state() is OK!\n";
	}

	/****************    test emit ********************/
	
	slog("\ntest emit");
	cout<<"\ntest emit\n";

	slog("next should show 'the emit is ok!'");
	cout<<"next should show 'the emit is ok!' in the server window\n";

    try
    {
       phi2 ->  send_message("emit"); 
    }
    catch(const CORBA::Exception& ex)
    {		
		cerr << ex << endl;
    };


	/***************  test disconnect_phi_emit_state() *************/  
	slog("\ntest disconnect_phi_emit_state()");
    cout<<"\ntest disconnect_phi_emit_state()\n";

    bool dis_state = true;

    try
    {
         phi2 -> disconnect_phi_emit_state();         
    }   
    catch(const CORBA::Exception& ex)
    {		
    	dis_state = false;
		slog("there is sth wrong in the method disconnect_phi_emit_state");
		cerr << ex << endl;
    }; 

	if(dis_state)
	{
		slog("disconnect_phi_emit_state() is OK!");
		cout<<"disconnect_phi_emit_state() is OK!\n";
	}
    
	
	slog("\n**************  test event end! ************");
	cout<<"\n**************  test event end! ************\n";
   
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