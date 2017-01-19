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

#ifndef __CIFHook_h__
#define __CIFHook_h__

#include <CORBA.h>
#include <ContainerRunTime_fwd.h>
//#include <ContainerBase_fwd.h>
#include <EnterpriseComponent.h>
#include <ComponentComposition.h>
//add by wsf
#include <ArchObject.h>
//end by wsf

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
class CIFHook;
OBJVAR(CIFHook)
OBJPTR(CIFHook)

class CIFHook : REF_COUNT_LOCAL_OBJECT
{
protected:
	ContainerRunTime_ptr runtime_;

public:
	CIFHook(ContainerRunTime_ptr runtime);
	virtual ~CIFHook();

//	virtual void setContainer(ContainerBase_ptr container);

	virtual PortableServer::POA_ptr getHomePOA();
	virtual PortableServer::POA_ptr getComponentPOA();
	
	virtual ContainerRunTime_ptr getContainerRunTime();

	virtual void onCreateHome();
	
	virtual void onCreateComponent(Container::ComponentComposition_ptr comp);
	virtual void onRemoveComponent(const PortableServer::ObjectId& oid);
	virtual CORBA::Object_ptr onFind(CORBA::OctetSeq* pid);
	virtual CORBA::Object_ptr createReference(const PortableServer::ObjectId& oid,const char* repId);
	
	//There is two ways to generate component references.
	//One is using facetId and pid to generate reference
	//for entity component, using facetId and a uuid to 
	//generate reference for session and service component;
	//the other is using both uuid and facetId and a pid
	//which can be null to generate component reference
	virtual CORBA::Object_ptr createReference2(
	            const PortableServer::ObjectId& oid,int facetId,CORBA::OctetSeq* pid,const char* repId);
	
	virtual Components::HomeExecutorBase_ptr getHomeExecutorBase();
//	virtual Components::EnterpriseComponent_ptr getExecutor();
	
	virtual void hookContext(Components::CCMContext_ptr ctx);
    virtual CORBA::ORB_ptr getOrb();
    virtual PortableServer::ObjectId* createObjectId(
            const PortableServer::ObjectId& oid,int facetId,CORBA::OctetSeq* pid);
	//void CosPersistenceState::StorageObject_ptr getStorageHome();
	
	//SuLiang add. 2003.8.21
	virtual ::Components::Session2Context_ptr getSession2ctx();
	virtual ::Components::Entity2Context_ptr getEntity2ctx();

	//end SuLiang add. 2003.8.21

	//add by wsf
	virtual void setArchComp(Container::ComponentComposition_ptr comp);
	virtual void onConnect(STARCCM::ArchConnection& acon);
	virtual void onDisconnect(PortableServer::ObjectId* srcoid, const char* featurename, const char* ck);
	//end by wsf
		
};

}

#endif
