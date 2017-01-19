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

#ifndef __CompositionManger_h__
#define __CompositionManger_h__

#include <CORBA.h>
#include <Util.h>
#include <string>
#include <map>
#include <vector>
#include <EnterpriseComponent.h>
#include <CCM.h>

using namespace std;
//using namespace ZThread;

namespace Container
{

typedef map<string, PortableServer::Servant> ServantMap;
typedef vector<string> ObjectIdVector;

enum CompositionStatus
{
	FREE,
	BUSY,
	ALIVE,
	PASSIVATED
};

enum CompositionType
{
	HOME,
	COMPONENT
};

//Comment is added by huangjie 2003.2.27
//The following four lines will be no use when compiling.
//But they are useful for Microsoft Visual Studio to recognise 
//some types which will be defined by us.
#if (!defined OBJVAR)&&(!defined OBJPTR)
#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;
#endif

class CompositionManager;

OBJVAR(CompositionManager)
OBJPTR(CompositionManager)
 
class CompositionManager : REF_COUNT_LOCAL_OBJECT  STARCCM_COMMA_RECURSIVE_MUTEX
{
	protected:
//		FastRecursiveMutex lock_;
	
		CompositionStatus status_;
//		FastRecursiveMutex executorLock_;
		CORBA::Object_var executor_;
	
	protected:
		CompositionManager();
		virtual ~CompositionManager();
		
		
		CORBA::String_var uuid_;
		
		PortableServer::Servant servant_;
		
		Components::CCMContext_var context_;
		//CosPersistenceState::StorageObject_var storage_;
		
		
		CORBA::OctetSeq_var pid_;
		
		CORBA::OctetSeq_var shortPid_;
		
		PortableServer::ObjectId_var objectId_;
		PortableServer::Servant sessionSynchronization_;
		
	public:
		PortableServer::Servant getServant();
//			throw(Synchronization_Exception);
		void setServant(PortableServer::Servant servant);
//			throw(Synchronization_Exception);
		virtual CORBA::Object_ptr getExecutor();
//			throw(Synchronization_Exception);
		virtual void setExecutor(CORBA::Object_ptr executor);
//			throw(Synchronization_Exception);
	

		Components::CCMContext_ptr getContext();
//			throw(Synchronization_Exception);
		void setContext(Components::CCMContext_ptr context);
//			throw(Synchronization_Exception);
		
		virtual int compositionType() = 0;
		
		//CosPersistenceState::StorageObject_ptr getStorage()
		//	 throw(Synchronization_Exception);
		//void setStorage(CosPersistenceState::StorageObject_ptr storageObject)
		//	 throw(Synchronization_Exception);
		CORBA::OctetSeq* getPid();
//			throw(Synchronization_Exception);
		void setPid(CORBA::OctetSeq* pid);
//			throw(Synchronization_Exception);
		CORBA::OctetSeq* getShortPid();
//			throw(Synchronization_Exception);
		void setShortPid(CORBA::OctetSeq* shortpid);
//			throw(Synchronization_Exception);
		PortableServer::ObjectId* getObjectId();
//			throw(Synchronization_Exception);
		void setObjectId(PortableServer::ObjectId*);
//			throw(Synchronization_Exception);
		char* getUUID();
//			throw(Synchronization_Exception);
		void setUUID(char* uuid);
//			throw(Synchronization_Exception);
		
		
		CompositionStatus getCompositionStatus();
//			 throw(Synchronization_Exception);
		void setCompositionStatus(CompositionStatus status);
//			 throw(Synchronization_Exception);
        
        PortableServer::Servant getSessionSynchronization();
        void setSessionSynchronization(PortableServer::Servant servant);
        virtual void passivate();
        virtual void activate();
};
	
}

#endif

