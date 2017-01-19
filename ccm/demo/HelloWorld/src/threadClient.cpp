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
#if defined(ORBacus)
	#include <OB/Properties.h>
#endif
#if defined(StarBus)
	#include <STAR/Properties.h>
#endif

#include <HelloWorld.h>
#include <stdlib.h>

#if defined(ORBacus) 
	#define CCM_MTL_INTERRUPTED_EXCEPTION			JTCInterruptedException
	#define STARCCM_COLON_THREAD					: public JTCThread
	#define STARCCM_THREAD_CLASS					JTCThread
	#define STARCCM_THREAD_HANDLE_CLASS				JTCThreadHandle
	#define STARCCM_THREAD_GROUP_HANDLE(handle)		JTCThreadGroupHandle handle;
	#define STARCCM_THREAD_GROUP_HANDLE_CLASS		JTCThreadGroupHandle
	#define STARCCM_THREAD_GROUP_CLASS				JTCThreadGroup
#endif
#if defined(StarBus)
	#define CCM_MTL_INTERRUPTED_EXCEPTION			MTLInterruptedException
	#define STARCCM_COLON_THREAD					: public MTLThread
	#define STARCCM_THREAD_CLASS					MTLThread
	#define STARCCM_THREAD_HANDLE_CLASS				MTLThreadHandle
	#define STARCCM_THREAD_GROUP_HANDLE(handle)		MTLThreadGroupHandle handle;
	#define STARCCM_THREAD_GROUP_HANDLE_CLASS		MTLThreadGroupHandle
	#define STARCCM_THREAD_GROUP_CLASS				MTLThreadGroup
#endif

#include <time.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

int LongExp(int n)
{
	int value =1;
	for(int i = 0;i < n; i++)
		value*=2;
	return value;
}
#define loopCount 8
#define TaskPerThread 4

class ClientThread STARCCM_COLON_THREAD
{
	HelloWorld::Person_var person_;

public:
	ClientThread(STARCCM_THREAD_GROUP_HANDLE_CLASS& group,
				HelloWorld::Person_ptr person)
	:STARCCM_THREAD_CLASS(group),
	person_(HelloWorld::Person::_duplicate(person))
	{
	}

    virtual void run()
    {
		for(int i=0;i<TaskPerThread;i++)
		{
			CORBA::String_var s= person_->sayHello();
			//printf("%s\n",s); 
			//cout<<person_->sayHello()<<endl;
			//cout<<person_->sayHello()<<endl;
		}
		
    }
};


int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{ 
#if defined(STARCCM_MTL)
	#if defined(ORBacus)
		JTCInitialize mtlInitialize;
	#endif
	#if defined(StarBus)
		MTLInitialize mtlInitialize;
	#endif
#endif
	Components::HomeFinder_var HomeFinder_;	
	HelloWorld::Person_var person,person1,person2,person3;  	
	CORBA::Object_var obj; 
	DWORD timestart;
	DWORD timeEnd;
	
    obj = orb->string_to_object("corbaloc::127.0.0.1:9992/HomeFinder");
  
    if( CORBA::is_nil(obj) )    
    {
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
    
    //create a component.
    try
    {   
		person = personHome->create(); 
//		person1 = personHome->create(); 
//		person2 = personHome->create(); 
//		person3 = personHome->create(); 		
    }
    catch(const CORBA::Exception& ex)
    {
    	cerr<<ex<<"\ncreate person failure\n";
    	return EXIT_FAILURE;
    }; 
	
	for(int count =0;count<loopCount;count++)
	{
		STARCCM_THREAD_GROUP_HANDLE(group_)
		group_ = new STARCCM_THREAD_GROUP_CLASS("group");
		timestart= GetTickCount();
		double currentThreadCount = LongExp(count);
		for(int threadCount=0;threadCount<currentThreadCount;threadCount++)
		{
			STARCCM_THREAD_HANDLE_CLASS myThread=new ClientThread(group_,person);
			myThread->start();
/*
			STARCCM_THREAD_HANDLE_CLASS myThread1=new ClientThread(group_,person1);
			STARCCM_THREAD_HANDLE_CLASS myThread2=new ClientThread(group_,person2);
			STARCCM_THREAD_HANDLE_CLASS myThread3=new ClientThread(group_,person3);
			myThread2->start();
			myThread3->start();
			myThread1->start();
*/
		}

		STARCCM_SYNCHRONIZED(groupSync,*group_)
		while(group_ -> activeCount() > 0)
		{
			try
			{
				group_ -> wait();
			}
			catch(const CCM_MTL_INTERRUPTED_EXCEPTION&)
			{
			}
		}
//		mtlInitialize.waitTermination();
		timeEnd = GetTickCount();
		cout << "Thread count:"
			 << currentThreadCount
			 << " task Count:"
			 << TaskPerThread*currentThreadCount
			 << "  time intervial is "
			 << timeEnd-timestart
			 << endl;

		group_ -> destroy();
	}

/*	for(int i=0;i<1000;i++)
	{
		char* s = person->sayHello();
		    //printf("%s\n",s); 
	}

*/
	person->remove();
//	person1->remove();
//	person2->remove();
//	person3->remove();
    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[], char*[])
{	
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;

	//
	// Set the communications concurrency model
	//
#if defined(STARCCM_MTL)
	#if defined(ORBacus)
	OB::Properties_var props = OB::Properties::getDefaultProperties();
	OB::ParseArgs(argc, argv, props);
	props -> setProperty("ooc.orb.conc_model", "threaded");
	props -> setProperty("ooc.orb.oa.conc_model", "threaded");
	#endif
	#if defined(StarBus)
	STAR::Properties_var props = STAR::Properties::getDefaultProperties();
	STAR::ParseArgs(argc, argv, props);
	props -> setProperty("nis.orb.conc_model", "threaded");
	props -> setProperty("nis.orb.oa.conc_model", "threaded");
	#endif
#endif
	cout<<"Client concurrency model is threaded.\n";

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
		catch(...)
		{
		    status = EXIT_FAILURE;
		}
    }
    
    return status;
}
