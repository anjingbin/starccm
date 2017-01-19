#include "pet.h"
#include "fstream.h"
//#include <direct.h>
#include <Windows.h>

Components::HomeFinder_var HomeFinder_;
PetStore::BusinessOp_var businessOp;
PetStore::Sales_var sales;
PetStore::Cart_var cart;
CORBA::ORB_var orb;

using namespace std;

char* __fastcall GetCCMProfileString(const char* strSectionName,
								const char* keyName, 
								const char* defautValue,
								const char* fileName)
{
	const char* homeDir = getenv("CCM_HOME");
	string filepath = homeDir;
	filepath += "\\";
	filepath += fileName;

	FILE* iniFile = fopen(filepath.c_str(),"r");
	if(iniFile == NULL)
		return NULL;

	char *break_set = " \t\n=";
	char dataBuf[128];
	string section = "[";
	section += strSectionName;
	section += "]";
	
	CORBA::String_var strRtn = defautValue;

	while(!feof(iniFile))
	{
		char *tokp,*sp;
		sp = dataBuf;
		fgets(dataBuf,128,iniFile);
		tokp = strtok(sp,break_set);
		if(tokp == NULL)
			continue;

		if(strcmp(tokp,section.c_str()) == 0)
		{
			while(!feof(iniFile))
			{
				fgets(dataBuf,128,iniFile);
				sp = dataBuf;
				tokp = strtok(sp,break_set);
				if(tokp == NULL)
					continue;
				if(tokp[0] == '[')
					goto EXIT;
				if(strcmp(tokp,keyName)==0)
				{
					sp = NULL;
					tokp = strtok(sp,break_set);
					if(tokp != NULL) 
					{
						strRtn = CORBA::string_dup(tokp);
						goto EXIT;
					}
				}//for statement if(strcmp(tokp,keyName)==0)
			}//for statement while(!feof(iniFile))
		}//for statement if(strcmp(tokp,section.c_str()) == 0)
	}//for statement while(!feof(iniFile))
EXIT:
	fclose(iniFile);
	return strRtn._retn();
}


void __fastcall getComponents(void)
{
  //get the  sales component
//  Components::CCMHome_var home = HomeFinder_->find_home_by_component_type("IDL:PetStore/Sales:1.0");
//  PetStore::SalesHome_var salesHome = PetStore::SalesHome::_narrow(home);
//  sales = salesHome->create();

  //get the BusinessOp interface.
  int dummy = 0;

  orb = ::CORBA::ORB_init(dummy,(char* const*) NULL);

//get the HomFinder IP,and save it.
string homeIP_ = CORBA::string_dup("corbaloc::");
homeIP_ += GetCCMProfileString("domain","main_host_ip","","starccm.properties");
homeIP_ += CORBA::string_dup(":");
homeIP_ += GetCCMProfileString("domain","HomeFinderPort","","starccm.properties");	
homeIP_ += CORBA::string_dup("/HomeFinder");

  CORBA::Object_var obj =  orb->string_to_object(homeIP_.c_str());
  HomeFinder_ = Components::HomeFinder::_narrow(obj);

   assert(!CORBA::is_nil(HomeFinder_));
  //get the cart component.
  Components::CCMHome_var home = HomeFinder_->find_home_by_name("ShoppingCart");
  PetStore::CartHome_var cartHome = PetStore::CartHome::_narrow(home);

  assert(!CORBA::is_nil(cartHome));
  cart = cartHome->create();

  assert(!CORBA::is_nil(cart));


  //get the component ref.

  //CORBA::ORB_ptr orb;

/*
  CosNaming::NamingContext_var rootContext =
        CosNaming::NamingContext::_narrow(
          orb->resolve_initial_references("NameService"));
*/

//get the NameService IP,and save it.
string nameService_ = CORBA::string_dup("corbaloc::");
nameService_ += GetCCMProfileString("domain","main_host_ip","","starccm.properties");
nameService_ += CORBA::string_dup(":");
nameService_ += GetCCMProfileString("domain","NameServicePort","","starccm.properties");	
nameService_ += CORBA::string_dup("/NameService");

// s=ip+"9991/NameService";
CORBA::Object_var objName = orb->string_to_object(nameService_.c_str());
  CosNaming::NamingContext_var rootContext = CosNaming::NamingContext::_narrow(objName);


  CosNaming::Name name;
//      name.length(1);
//      name[0].id = (const char *) "ShoppingCart";
//      name[0].kind = (const char *) "";
//      PetStore::Cart_var CartHome =
//        PetStore::Cart::_narrow(rootContext->resolve(name));


      name.length(1);
      name[0].id = (const char *) "ShoppingSales";
      name[0].kind = (const char *) "";

      sales = PetStore::Sales::_narrow(rootContext->resolve(name));

      businessOp = sales->provide_for_client();
      assert(!CORBA::is_nil(businessOp));

}

