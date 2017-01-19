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

#ifndef __ContainerRunTime_h__
#define __ContainerRunTime_h__

#include <CORBA.h>
#include <ContainerRunTime_fwd.h>
#include <ContainerBase_fwd.h>
#include <EnterpriseComponent.h>
#include <HomeExecutorBase.h>
#include <ExecutorSegmentBase.h>
#include <ComponentsPool.h>
#include <CIFHook.h>
#include <Configurator.h>
#include <CCMHome.h>
#include <HomeComposition.h>
#include <ExecutorInvoker_fwd.h>
#include <ExecutorLoader.h>
#include <ServantLoader.h>
#include <FileAccessor.h>
#include <Deployment.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#include <direct.h>
#endif

#include <ResourcePool.h>
#include <PassivationArbitor.h>
//add by wsf
#include <ArchObject.h>
//end by wsf

//added by xiao heping
#include <CustomThreadPool.h>
using namespace GreenThread;
//end add

#ifndef LITE_VERSION
	#include <PSSInitializer.h>
	#include <StorageObjectLoader.h>
	#include <RunTime_psdl.h>
#endif

using namespace STARCCM::Deployment;

namespace Container
{

class ContainerRunTime : REF_COUNT_LOCAL_OBJECT
{
protected:
	ServantLoader_var servantsLoader_;
	ExecutorLoader_var executorsLoader_;

	ExecutorInvoker_ptr executorInvoker_;
	ExecutorInvoker_ptr homeExecutorInvoker_;

	::STARCCM::ResourcePool_var resPool_;

	//added by xiaoheping 2004/03/06
#if defined(STARCCM_MTL)
	::GreenThread::CustomThreadPool_var threadPool_;
#endif
	//end add
	
	CORBA::String_var homeRepId_;
	CORBA::String_var compRepId_;
//	Components::HomeExecutorBase_var home_;
	PortableServer::POA_var homePoa_;
	PortableServer::POA_var componentPoa_;
	HomeComposition_var homeComposition_;

	ComponentsPool_var runningComponentsPool_;
//	ComponentsPool_var freeComponentsPool_;

	CIFHook_var cifHook_;
	ContainerBase_var container_;
	
	Components::Deployment::ComponentInstallation_var locator_;
	CORBA::String_var uuid_;

	//add by wsf
	STARCCM::ArchComponents aComponents;
	STARCCM::DeletedComs deletedComslist;
	
	STARCCM::ArchConnections aConnections;
	STARCCM::ArchConnections deletedConnslist;
	//end by wsf

	//add by wsf 2004.6.30
	//for redirecting request 
	typedef map<string, CORBA::Object_var> MapRedirectReq;
	MapRedirectReq redirectReqMap;
	
	STARCCM_MONITOR(redirectReqMapMonitor)

	typedef map<string,string> MapRejectReq;
	MapRejectReq RejectReqMap;
	STARCCM_MONITOR(rejectReqMapMonitor)
	void setRejectReqInfo(const CORBA::StringSeq& oidList);
	void removeRejectReqInfo(const CORBA::StringSeq& oidList);
	//end by wsf 2004.6.30

//	added by xiao heping 2004/03/03
	int isMultiThread_;
	::Components::CCMHome_var ccmHome_;
	CORBA::String_var homeRegistration_;
	bool destroy_;
//	end add xiao heping 2004/03/03
	
#ifndef LITE_VERSION	
	PSSInitializer_var pssInitializer_;
	StorageObjectLoader_var storageLoader_;
#endif

	//SuLiang add the homePOAName  and componentPOAName that are their uuid.
	//2003.3.27
	CORBA::String_var homePOAName_;
	CORBA::String_var componentPOAName_; 
	CORBA::String_var homeCompositionUUID_;
	PassivationArbitor_var passivator_;
	
	//add by jxh 06/22
	CORBA::String_var resourceName;

public:
	ContainerRunTime(/*
		Components::HomeExecutorBase_ptr home,
		PortableServer::POA_ptr homePoa,
		PortableServer::POA_ptr copmonentPoa*/);

	virtual ~ContainerRunTime();

//	void setCompRepId(const char *compRepId);
	char* getCompRepId();
	char* getHomeRepId();
//	void setHomeRepId(const char *homeRepId);
	virtual Components::HomeExecutorBase_ptr getHomeExecutor();
//	virtual void setHomeExecutor(Components::HomeExecutorBase_ptr home);

//	virtual Components::EnterpriseComponent_ptr getExecutor();

//	ExecutorInvoker_ptr getExecutorInvoker();
//	ExecutorInvoker_ptr getHomeExecutorInvoker();
	
	void setContainer(ContainerBase_ptr container);
	ContainerBase_ptr getContainer();
	PortableServer::POA_ptr getHomePoa();
	PortableServer::POA_ptr getComponentPoa();
	PortableServer::ObjectId* getHomeObjectId();
	
	void initialize(/*int isMultiThread*/);
//	void setCcmHome(::Components::CCMHome_ptr ccmHome);
	::Components::CCMHome_ptr getCcmHome();
	void setHomeComposition(HomeComposition_ptr homeComposition);
	
//	void setComponentsPool(ComponentsPool_ptr pool);
//	ComponentsPool_ptr getComponentsPool();

	//deleted by xiao heping 2004/03/18
//	CORBA::Object_ptr getExecutor(PortableServer::ObjectId* oid);
	//end delete xiao heping 2004/03/18
		
	PortableServer::Servant getServant(PortableServer::ObjectId* oid,
										PortableServer::ServantLocator::Cookie& the_cookie,
										const char* operation = 0);
	PortableServer::Servant getComponentServant(PortableServer::ObjectId* oid);

#ifndef LITE_VERSION	
	PSSInitializer_ptr getPssInitializer();
#endif
	
	int getCompositionType(PortableServer::ObjectId* oid);
	
	CIFHook_ptr getCifHook();
	
	/* to passivate and activate component instances from disks */
	virtual void passivate();
	virtual void incarnate(const char* uuid);
	virtual void shutdown();
//	virtual void removeHome(Components::CCMHome_ptr href);
	virtual void cleanup();
	virtual void startup();
	
	ContainerType getContainerType();
	
	void onCreateComponent(ComponentComposition_ptr comp);
	void onRemoveComponent(const PortableServer::ObjectId* oid);
	
	
    virtual PortableServer::Servant preinvoke(const PortableServer::ObjectId& oid,
												PortableServer::POA_ptr adapter,
												const char* operation,
												PortableServer::ServantLocator::Cookie& the_cookie);

    virtual void postinvoke(const PortableServer::ObjectId& oid,
							PortableServer::POA_ptr adapter,
                            const char* operation,
                            PortableServer::ServantLocator::Cookie the_cookie,
                            PortableServer::Servant the_servant);
                            
    virtual Components::CCMHome_ptr install_home(const char* id,
                                            const char* entrypt,
                                             const Components::ConfigValues& config)
						    throw(Components::Deployment::UnknownImplId,
						          Components::Deployment::ImplEntryPointNotFound,
						          Components::Deployment::InstallationFailure,
						         Components::Deployment::InvalidConfiguration,
						          CORBA::SystemException);
//	virtual void setResourcePool(::STARCCM::ResourcePool_ptr res);

	//added  by xiao heping 2004/03/16
	CustomThreadPool_ptr GetCustomThreadPool();
	//end add xiao heping 2004/03/16
	
	//add by wsf
	virtual void setArchComp(ComponentComposition_ptr comp);
	virtual void onConnect(STARCCM::ArchConnection& acon);
	virtual void onDisconnect(PortableServer::ObjectId* srcoid, const char* featurename, const char* ck);

	virtual void updateArchitecture(STARCCM::ArchComponents_out addedcoms, 
						STARCCM::DeletedComs_out dcomslist, 
						STARCCM::ArchConnections_out addedconns, 
						STARCCM::ArchConnections_out dconnslist)
		throw(CORBA::SystemException);
	virtual void cleanArchObjects()
		throw(CORBA::SystemException);

	virtual void addArchComponent(STARCCM::ArchComponent& archcom);
	virtual void deleteArchComponent(const PortableServer::ObjectId* oid);

	virtual void addArchConnection(STARCCM::ArchConnection& connect);
	virtual void deleteArchConnection(PortableServer::ObjectId* srcoid, const char* featurename, const char* cookie);
    	//end by wsf

	//add by wsf 2004.6.29
	virtual void delRedirectReqInfo(const CORBA::StringSeq& oidList);
	virtual void setRedirectReqInfo(const CORBA::StringSeq& oidList, 
							const Components::ObjectSeq& newObjList,
							CORBA::Long timeout)
		throw(CORBA::SystemException);

	virtual void passivateClient(const CORBA::StringSeq& oidList, 
							const Components::Deployment::PassivateClientTypeSeq& typeList)
		throw(CORBA::SystemException);
	
	virtual void activateClient(const CORBA::StringSeq& oidList)
		throw(CORBA::SystemException);
	//end by wsf 2004.6.29  
	
    //SuLiang add . 2003.8.28
    virtual ::STARCCM::ResourcePool_ptr getResourcePool();
    //SuLiang add . 2003.8.28
    
    //add by jxh 06/22
     char* getResourceName();

};

class InfoDeleter STARCCM_COLON_THREAD
{
	CORBA::Long timeout_;
	CORBA::StringSeq oidlist_;
	ContainerRunTime_var runtime_;

public:
	InfoDeleter(ContainerRunTime_ptr runtime, const CORBA::StringSeq& oidlist, CORBA::Long timeout)
	:runtime_(runtime),oidlist_(oidlist)
	{
		if (timeout > 0)
			timeout_ = timeout;
		else
			timeout_ = 600;
	}
	virtual void run()
	{
		sleep(timeout_);
		runtime_ -> delRedirectReqInfo(oidlist_);
	}//for run
};//for class InfoDeleter

}

#endif


