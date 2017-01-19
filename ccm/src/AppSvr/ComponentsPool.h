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

#ifndef __ComponentsPool_h__
#define __ComponentsPool_h__

#include <CORBA.h>
#include <map>
#include <string>
#include <list>
#include <EnterpriseComponent.h>
#include <ComponentComposition.h>
#include <CCM.h>



using namespace std;

//Comment is added by huangjie 2003.2.27
//The following four lines will be no use when compiling.
//But it is useful for Microsoft Visual Studio to recognise 
//some type which we will define.
#if (!defined OBJVAR)&&(!defined OBJPTR)
#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;
#endif

namespace Container
{

typedef map<string,ComponentComposition_var> ActiveObjectMap;
//typedef list<string> LRU;

class ComponentsPool;

OBJVAR(ComponentsPool)
OBJPTR(ComponentsPool)

class ComponentsPool : REF_COUNT_LOCAL_OBJECT STARCCM_COMMA_RECURSIVE_MUTEX
{
protected:
//	FastRecursiveMutex lock_;
	
	ActiveObjectMap activeObjectMap_;
//	LRU lru_;
	

public:
	
	ComponentsPool();
	virtual ~ComponentsPool();
	
	ComponentComposition_ptr getComposition(PortableServer::ObjectId* oid);

	//deleted by xiao heping 2004/03/18
//	CORBA::Object_ptr getExecutor(PortableServer::ObjectId* oid);
	//end delete xiao heping 2004/03/18

	//modified by xiao heping 2004/03/28
	PortableServer::Servant getServant(PortableServer::ObjectId* oid,
		PortableServer::ServantLocator::Cookie& the_cookie,
		const char* operation = 0);
	//end modified 2004/03/28

	void addComponent(ComponentComposition_ptr comp);
	void removeComponent(const PortableServer::ObjectId* oid);
	
	virtual void cleanup();
	
	void passivate();
	void partPassivate();
	void activate();
	
};


}


#endif

