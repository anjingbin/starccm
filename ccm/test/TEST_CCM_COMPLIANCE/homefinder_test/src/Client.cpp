// **********************************************************************
//
// Copyright (c) 2003
// 
// StarCCM test example.
// Test the CCM Homefinder
//
// All Rights Reserved
//
// **********************************************************************



#include <CORBA.h>



#include <plus.h>

#include <stdlib.h>

#include <fstream>

//#include <cstdlib>

#include "UuidGenerator.h"
#include <CCMCommonType_impl.h>

#include <CommonFunc.h>


Components::HomeFinder_var HomeFinder_;



Example::Plus_var plus;  

CORBA::ORB_var orb;



#ifdef HAVE_STD_IOSTREAM

using namespace std;

#endif

void slog(const char* slog)

{

	ofstream f("homefinder_test.out",ios_base::app);

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
    ofstream f("homefinder_test.out",ios_base::trunc);

	f.close();

   	slog("\n***********  HomeFinder  test  begin! *************");
   	cout<<"\n***********  HomeFinder  test  begin! *************\n";

   	
       //get the HomFinder IP,and save it.
	CORBA::String_var ip = CORBA::string_dup("corbaloc::");
	ip += HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");
	ip += CORBA::string_dup(":");
	ip += HelpFun::GetCCMProfileString("domain","HomeFinderPort","","starccm.properties");	
	ip += CORBA::string_dup("/HomeFinder");		


      //get the HomeFinder object.
	CORBA::Object_var obj = orb -> string_to_object(ip.in());  

    HomeFinder_ =Components::HomeFinder::_narrow(obj);

	assert(!CORBA::is_nil(HomeFinder_));


      //find PlusHome from HomeFinder.
	 Components::CCMHome_var home;



       //test the homefinder interface


     //find_home_by_home_name right

   slog("\ntest find_home_by_home_name ");
   cout<<"\ntest find_home_by_home_name\n";

   bool b=TRUE;
   
     try

    {

          home = HomeFinder_->find_home_by_name("Example");
    }

    catch(const CORBA::Exception& ex)

    { 

    	b=FALSE;    	

    	slog("\nfind_home_by_name is not completed\n");

    	cerr<<ex<<"\n";

    }; 

    if(b)
    {

      slog("find_home_by_name is OK!!!");
      cout<<"find_home_by_name is OK!!!\n";
    }

    assert(!CORBA::is_nil(home));

 
    Example::PlusHome_var plusHome = Example::PlusHome::_narrow(home);



    assert(!CORBA::is_nil(plusHome));   



    cout<<"\nCreating plus using homes\n";
    slog("\nCreating plus using homes");
    
    plus = plusHome->create();   

    cout<<"plus  created\n";
    slog("plus  created");   


    long value;

    printf("value=plus.Add(3,5)=");
    slog("value=plus.Add(3,5)=");

    value=plus->Add(3,5);

    printf("%d\n",value);

    char a[4];

    _itoa(value,a,10);

    slog(a);

    slog("\n***********  HomeFinder  test  end! *************");
    cout<<"\n***********  HomeFinder  test  end! *************\n";

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

