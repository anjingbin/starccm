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

#ifndef __ComponentComposition_h__
#define __ComponentComposition_h__

#include <CORBA.h>
#include <ExecutorSegmentBase.h>
#include <EnterpriseComponent.h>
#include <CompositionManager.h>
#include <HomeComposition.h>
#include <map>
#include <string>
#include <Util.h>
#include <CCM.h>

using namespace std;
//using namespace ZThread;

//Comment is added by huangjie 2003.2.27
//The following four lines will be no use when compiling.
//But they are useful for Microsoft Visual Studio to recognise 
//some types which will be defined by us.
#if (!defined OBJVAR)&&(!defined OBJPTR)
#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;
#endif

namespace Container
{
class ComponentComposition;

OBJVAR(ComponentComposition)
OBJPTR(ComponentComposition)

class ComponentComposition : public virtual CompositionManager
{

protected:
	//CORBA::String_var clientInfo_;

private:
	ServantMap portServants_;
	

	CORBA::Object_var componentRef_;
	
	/* Home features */
	HomeComposition_var homeComposition_;


public:
	ComponentComposition();
	virtual ~ComponentComposition();



	Components::EnterpriseComponent_ptr getComponentExecutor();
//		throw(Synchronization_Exception);
	void registerComponentExecutor(Components::EnterpriseComponent_ptr executor);
//		throw(Synchronization_Exception);
//	Components::HomeExecutorBase_ptr getHomeExecutor()
//		throw(Synchronization_Exception);
//	PortableServer::Servant getHomeServant()
//		throw(Synchronization_Exception);
//	PortableServer::ObjectId* getHomeObjectId()
//		throw(Synchronization_Exception);

	PortableServer::Servant getComponentServant();
//		throw(Synchronization_Exception);
	void registerComponentServant(const PortableServer::ObjectId& oid,PortableServer::Servant servant);
//		throw(Synchronization_Exception);
	PortableServer::Servant getServant(PortableServer::ObjectId* oid);
//		throw(Synchronization_Exception);
//    virtual PortableServer::Servant getSessionSynchronization();
    
	void registerPortServant(const PortableServer::ObjectId& oid,PortableServer::Servant servant);
//		throw(Synchronization_Exception);
	int isPort(PortableServer::ObjectId* oid);
//		throw(Synchronization_Exception);
	ObjectIdVector* getPortObjectIdVector();
//		throw(Synchronization_Exception);



//SuLiang add 2003.4.2
	void registerComponentRef(CORBA::Object_ptr comp);
	CORBA::Object_ptr getComponentRef();
//SuLiang add 2003.4.2	
	
	
	virtual int compositionType();
	
	virtual void passivate();
    virtual void activate();

};

}

#endif

