// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Kebo    mep@263.net
// Author: Huang Jie    huangjie@email.com
// Author: Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#include <ResourcePool_impl.h>
#include <ContainerBase.h>


using namespace Container;
using namespace STARCCM;

ResourcePool_var STARCCM::ResourcePool_impl::resPool_ = 0;

STARCCM::ResourcePool_impl::ResourcePool_impl(::Container::ContainerRunTime_ptr runtime, 
											  CORBA::ULong initialCon, 
											  CORBA::ULong maxCon, 
											  CORBA::ULong inc)
                         :currentCon_(initialCon),initialCon_(initialCon),maxCon_(maxCon),inc_(inc)
{
	//modified by xiao heping 2004/03/31
//	container_ = OBJDUPLICATE(::Container::ContainerBase_ptr,container);
	container_ = runtime -> getContainer();
	//end modify xiao heping 2004/03/31
	CORBA::ORB_var orb = container_ -> getOrb();

	const char* path = getenv("CCM_HOME");

	CORBA::String_var reffile = (const char*)"relfile:/";
	reffile += path;
	reffile += "\\rf.ref";
	CORBA::Object_var obj;

	try
	{
		obj = orb -> string_to_object(reffile.in());
	}
	catch(CORBA::SystemException& ex)
	{
		ex;
		//problem???  by huangjie
	}

	if (CORBA::is_nil(obj))
    {
    	TRACE0(LEVEL1,"ResourcePool can not get the initial reference of ResourceFactory.\n"); 
    }
	    
#ifndef TAO
    resourceFactory_ = ::ResourceFactory::_narrow(obj);  

    if (CORBA::is_nil(resourceFactory_))
    {
    	TRACE0(LEVEL1,"ResourcePool can not narrow the object to ResourceFactory.\n");
    }
#endif

	//create the initial connects.
	int con = 0;

	resInfor tempResInfor;
	Resource_var tempRes;
	while(con < initialCon_)
	{
				
#ifndef TAO		
		tempRes = resourceFactory_ -> create(Postgre);
#endif
		tempResInfor.res = tempRes;
		tempResInfor.isUsed = false;
		resList_.push_front(tempResInfor);
		con++;
	}
}

STARCCM::ResourcePool_impl::~ResourcePool_impl()
{
}

::Resource_ptr
STARCCM::ResourcePool_impl::getResource()
	throw(STARCCM::NoResource)
{
//	ifstream fin("config.ini");
//	char special ;
//	char temp[1024];
//	char host[1024];
//	char database[1024];
//	char user[1024];
//	char password[1024];
//	char port[1024];
//	int count = 0;
//	fin.get(special);
//	while( special != EOF)
//	{
//		switch(special)
//		{
//			case '=' :
//				{
//					fin.getline(temp,1024);
//					count++;
//					switch(count)
//					{
//						case 1:
//							::strcpy(host,temp);
//							break;
//						case 2:
//							::strcpy(database,temp);
//							break;
//						case 3:
//							::strcpy(user,temp);
//							break;
//						case 4:
//							::strcpy(password,temp);
//							break;
//						case 5:
//							::strcpy(port,temp);
//							break;
//						default:
//							break;
//					}
//				break;
//				}
//			case '#':
//				fin.getline(temp,1024);
//				break;
//			case ' ':
//				fin.getline(temp,1024);
//				break;
//			default:
//				break;
//		}
//		if (count == 5)
//			break;	
//		fin.get(special);
//	}
//
//	cout <<"@@@@@@@@@@@@@@@\n"<< host<<endl << database<<endl<< user<<endl << password <<endl<< port<<endl;	
//	char* Host = host;
//	char* Database = database;
//	char* User = user;
//	char* Password = password;
//	char* Port = port;
//	
//	pghost = "";	// host name of the backend server
//	pgport = "";	// port of the backend server
//	pgoptions = "";	// special options to start up the backend server
//	pgtty = "";	// debugging tty for the backend server	
//	//renyi andded 7 1 2003
//	login = "";	// the login account 
//	pwd = "";	// the password	
//	
//	
//	
//	char *pgoptions = 0;
//	char *pgtty = 0;
	
	if (currentCon_ <= maxCon_) 
	{	
		ResList::iterator tempIterator = resList_.begin();	
		while(tempIterator != resList_.end())
		{
			if(!(*tempIterator).isUsed)
				return (*tempIterator).res;
			tempIterator++;	
		}	
		resInfor tempResInfor;
		Resource_var tempRes;

#ifndef TAO		
		tempRes = resourceFactory_ -> create(Postgre);
#endif
		tempResInfor.res = tempRes;
		tempResInfor.isUsed = true;
		resList_.push_front(tempResInfor);
		currentCon_++;
		return tempRes;
		cout << "\n\t[Create resource succeed!]" << endl;

//#ifndef TAO
//    Resource_var resource = resourceFactory_->create(Postgre);
//#else
//	Resource_var resource;
//#endif
    	//resource -> pg_PQsetdbLogin(Host, Port, pgoptions, pgtty, Database,User,Password);
//    	currentCon_++;
//    	return resource._retn();
    }
    else
    {
    	throw new STARCCM::NoResource();
    }
                             
}

void
STARCCM::ResourcePool_impl::removeResource(::Resource_ptr res)
	throw(STARCCM::RemoveResFailed)
{
//	Resource_var removeRes = ::Resource::_duplicate(res);
//	try
//	{
//#ifndef TAO
//		removeRes -> pg_PQfinish();
//#endif
//	}
//	catch(...)
//	{
//		throw( STARCCM::RemoveResFailed() );
//	}
////	connectionCount_--;
	ResList::iterator tempIterator = resList_.begin();	
	while(tempIterator != resList_.end())
	{
		if(res == (*tempIterator).res)
		{
			(*tempIterator).isUsed = false;			
		}
		tempIterator++;	
	}
	if(tempIterator == resList_.end())
	{		
		throw new STARCCM::RemoveResFailed();
	}
}

ResourcePool_ptr
STARCCM::ResourcePool_impl::getResPoolInstance(::Container::ContainerRunTime_ptr runtime, CORBA::ULong initialCon, CORBA::ULong maxCon, CORBA::ULong inc)
{
	if (resPool_ == 0)
		resPool_ = new ResourcePool_impl(runtime, initialCon, maxCon, inc);
	return resPool_;
//	return 0;
}


//After creating the resourcePool object ,you can invoke getResPool() method.
//That is to say,after the containerruntime invoke the getResPoolInstance(*,*,*,*),you can invoke getResPool() method.
//Adding this method is to get the resourcePool object easily, you have not the parameters. 
ResourcePool_ptr
STARCCM::ResourcePool_impl::getResPool()
{
	return resPool_;
}





//add by jxh
CORBA::ULong  STARCCM::ResourcePool_impl::getinc()

{
	return  inc_;
}
CORBA::ULong  STARCCM::ResourcePool_impl::getmaxcon()
{
	return maxCon_;
}
CORBA::ULong STARCCM::ResourcePool_impl::getcurrentcon()
{
	return currentCon_;
}
CORBA::ULong STARCCM::ResourcePool_impl::getinitialcom()
{
	return initialCon_;
}

