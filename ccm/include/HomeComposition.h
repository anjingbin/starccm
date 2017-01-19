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

#ifndef __HomeComposition_h__
#define __HomeComposition_h__

#include <CORBA.h>
#include <Util.h>
#include <string>
#include <EnterpriseComponent.h>
#include <CompositionManager.h>

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

class HomeComposition;

OBJVAR(HomeComposition)
OBJPTR(HomeComposition)

class HomeComposition : public virtual CompositionManager
{
	protected:
//		FastRecursiveMutex repIdLock_;
		CORBA::String_var repId_;
	
	public:
		HomeComposition();
		virtual ~HomeComposition();
		
		PortableServer::Servant getHomeServant();
		void registerHomeServant(const PortableServer::ObjectId& oid,PortableServer::Servant servant);
		Components::HomeExecutorBase_ptr getHomeExecutor();
		void registerHomeExecutor(Components::HomeExecutorBase_ptr homeExecutor);

		char* getRepId();
		void setRepId(const char* repId);
		
		virtual int compositionType();
        
//        virtual PortableServer::Servant getSessionSynchronization();
};
	
}

#endif

