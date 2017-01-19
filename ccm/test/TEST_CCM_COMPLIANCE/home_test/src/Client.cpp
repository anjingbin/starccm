// **********************************************************************
//
// Copyright (c) 2003
// 
// StarCCM test example.
// Test the CCM Home.
//
// All Rights Reserved
//
// **********************************************************************

#include <CORBA.h>

#include <test.h>
#include <stdlib.h>
#include <fstream>
#include "UuidGenerator.h"
#include <TEST_ValueType_impl.h>
#include <CCMCommonType_impl.h>

#include <CommonFunc.h>

Components::HomeFinder_var HomeFinder_;

Test::Product_var product;  
CORBA::ORB_var orb;

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif
void slog(const char* slog)
{
	ofstream f("home_test.out",ios_base::out | ios_base::app );
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
    
    
    /***************   file init ****************************/
    ofstream f("home_test.out");
    f.close();
    
    slog("\n**********  home test begin !**********");
    cout<<"\n**********  home test begin !**********\n";
    
    slog("\nHome  test,in this test use an entity component!\n");
    cout<<"\nHome  test,in this test use an entity component!\n";
    
    
    /**************   get   homefinder **********************/
	
    CORBA::String_var ip = CORBA::string_dup("corbaloc::");
	ip += HelpFun::GetCCMProfileString("domain","main_host_ip","","starccm.properties");
	ip += CORBA::string_dup(":");
	ip += HelpFun::GetCCMProfileString("domain","HomeFinderPort","","starccm.properties");	
	ip += CORBA::string_dup("/HomeFinder");		

    CORBA::Object_var obj = orb -> string_to_object(ip.in());    
	
    if( CORBA::is_nil(obj) )    
    {
	TRACE0(LEVEL1,"Can NOT get HomeFinder\n");
	return EXIT_FAILURE;
    }
    
    HomeFinder_ =Components::HomeFinder::_narrow(obj);
	
    assert(!CORBA::is_nil(HomeFinder_));
	 
	
    /***********   create product using home *************/	
	 
    Components::CCMHome_var home;    
    try
    {
       //home = HomeFinder_->find_home_by_component_type("IDL:Test/Product:1.0");
       home = HomeFinder_->find_home_by_name("Example");
    }
    catch(const CORBA::Exception& )
    {
    	slog("find_home_by_name  is wrong");
    };
    
    assert(!CORBA::is_nil(home));
    
    Test::ProductHome_var productHome = Test::ProductHome::_narrow(home);

    assert(!CORBA::is_nil(productHome));
    
    cout<<"\nCreating product using homes\n";
    slog("\nCreating product using homes");    
  
   
   
    ::Test::KeyID* pKey=new Test::KeyID_impl(); 
    
    

    /***********  The first step is to give the product name ***********/
 
    slog("\nThe first step is to create 2 components and give the product name,");
    slog("they should be saved into the database!");
    
    cout<<"\nThe first step is to create 2 component and give the product name,\n";
    cout<<"they should be saved into the database!\n";
 
    /*******  create the first component ***********/
          
    pKey->id((const char*) "35"); 
    slog("\npKey->id() is OK!");
    cout<<"\npKey->id() is OK!\n"; 
	
    product = productHome->create(pKey);
    slog("productHome->create(pKey) is OK!\n");
    cout<<"productHome->create(pKey) is OK!\n";
     
    slog("\n1st product  created");
    cout<<"\n1st product  created\n"; 
           
    product->product_name("dog");
    slog("product_1 name = dog");
    cout<<"product_1 name = dog\n";
    
    
    /*******  create the second component ***********/
    
    pKey->id((const char*) "37");       
    product = productHome->create(pKey); 
        
    slog("\n2st product  created");
    cout<<"\n2st product  created\n";
     
    product->product_name("monkey");
    slog("product_2 name = monkey");
    cout<<"product_2 name = monkey\n";
  
       
    /*******    The second step is to get the product name  *********/
    
    slog("\nThe second step is to get the product name from the database");
    cout<<"\nThe second step is to get the product name from the database\n";
   

    /***********  test find_by_primary_key ********/
    
    slog("\ntest find_by_primary_key\n");
    cout<<"\ntest find_by_primary_key\n";
       
    try
    {
    	 product = productHome->find_by_primary_key(pKey);  
    }
    catch(const CORBA::Exception &e)
    {
    	cerr<<e<<"\n";
    	slog("find_by_primary_key is not impleted!\n");
    };
    
    slog("if next show monkey,find_by_primary_key is OK!");
    cout<<"if next show monkey,find_by_primary_key is OK!\n";
      
    printf("%s\n",product->product_name());
    slog(product->product_name());
    
    
   /*******   The Third step is to romove the product  **********/
   
   slog("\nThe Third step is to romove the product");
   cout<<"\nThe Third step is to romove the product\n";
   
   slog("\nnext test remove\n");
   cout<<"\nnext test remove\n";
   
   bool b=TRUE;
   try
   {
   	    productHome->remove(pKey);
   	     
	    pKey->id((const char*) "35");
	     
	    productHome->remove(pKey); 
   }
   catch(const CORBA::Exception &e)
   {
    	b=FALSE;
    	cerr<<e<<"\n";
    	slog("remove is not impleted!\n");
    };
    
    //next should raise exception 
    if(b)
    {
	    slog("if remove is OK,next should show exception,otherwise romove is not completed!");
	    cout<<"if remove is OK,next should show exception,otherwise romove is not completed!\n";
	    
	    try
	    {
	    	    product = productHome->find_by_primary_key(pKey);    
	    }
	    catch(Components::CCMException &e)
	    {
	    	cerr<<e<<"\n";
	    	slog(e._to_string());
	    } 
	     catch(const CORBA::Exception &e)
	    {
	    	cerr<<e<<"\n";
	    	slog(e._to_string());
	    };
     }
     
    slog("\n**********  home test end !**********");
    cout<<"\n**********  home test end !**********\n";
     
     
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
