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

#ifndef __ContainerBase_H__
#define __ContainerBase_H__

#include <CORBA.h>
#include <string>
#include <list>
#include <map>
#include <Cookie.h>
#include <ExecutorInvoker_fwd.h>
#include <ContainerBase_fwd.h>
#include <EnterpriseComponent.h>
#include <UuidGenerator.h>
#include <ContainerRunTime_fwd.h>
#include <CompositionManager.h>
#include <DeployDomainMgr.h>
#include <ServantLoader_fwd.h>
#include <ExecutorLoader.h>
#include <Container_impl.h>
#include <HomeRegistration.h>
#include <ResourcePool_impl.h>
//add by wsf
#include <ArchObject.h>
//end by wsf

//added by xiao heping
#include <CustomThreadPool.h>
using namespace GreenThread;
//end add

#ifndef LITE_VERSION
#include <StorageObjectLoader.h>
#endif

using namespace std;

namespace Container
{
//deleted by xiao heping 2004/03/23
//typedef list<ServantLoader_var> ServantLoaderList;
//typedef list<ExecutorLoader_var> ExecutorLoaderList;

//#ifndef LITE_VERSION
//typedef list<StorageObjectLoader_var> StorageLoaderList;
//#endif
//end delete xiao heping 2004/03/23

typedef list<ContainerRunTime_var> ContainerRunTimeList;
//typedef map<string,ContainerRunTimeList::iterator> HomeObjectIdMap;

//SuLiang add. 2004.2.27
//typedef std::string homeRef;
//typedef map< string,ContainerRunTime_var > MapHomeRuntime;                    
//SuLiang add. 2004.2.27        

//add by wsf
typedef map<string,ContainerRunTime_var> MapRunTime;
//end by wsf

class ContainerBase : REF_COUNT_LOCAL_OBJECT
{
//SuLiang add. 2004.2.27        
//private:
//      MapHomeRuntime homeRuntimeMap_;
//SuLiang add. 2004.2.27
protected:
//	::STARCCM::ResourcePool_var resPool_;
//      ::STARCCM::HomeRegistration_var homeReg_;
//      Components::HomeFinder_var homeFinder_;

//	CORBA::String_var homeDir_;
//	CORBA::String_var serverName_;
        
//      deleted by xiao heping 2004/02/15

//      ServantLoaderList servantLoaderList_;
//      ExecutorLoaderList executorLoaderList_;
//#ifndef LITE_VERSION  
//      StorageLoaderList storageLoaderlist_;
//#endif

//      end deleted

	ContainerRunTimeList runtimeList_;
	STARCCM_RECURSIVE_MUTEX(runtimeListLock_)

	//deleted by xiao heping 2004/02/15
	//HomeObjectIdMap objectIdMap_;
	//end deleted

	ContainerType containerType_;
	//added by xiao heping 2004/02/13
	bool ContainerTypeSetted;
	//end added by xiao heping 2004/02/13

	CORBA::ORB_var orb_;
	PortableServer::POA_var poa_;
        
//	Components::UuidGenerator_var uuidGenerator_;

	Components::Deployment::ComponentServer_var server_;
	STARCCM::Deployment::DeployDomainMgr_var manager_;
	Components::ConfigValues_var configuration_;

	//added by xiao heping 2004/04/01
	PortableServer::ServantBase_var containerServant_;
	//end add
        
	int configurationChanged;
//	int deployed;
	int ready;
        
//	CORBA::String_var name_;
        
	Components::Deployment::Container_var containerRef_;
        
	PortableServer::ObjectId_var containerId_;
	CORBA::String_var componentServerUuid_;

	//add by wsf
	STARCCM::ArchHome ahome;
	STARCCM::ArchComponent acom;
	STARCCM::ArchConnection aconn;
	MapRunTime runtimeMap;
	//end by wsf

	//deleted by xiao heping 2004/03/04
//	PortableServer::Servant findServant(PortableServer::ObjectId* oid);
//	virtual Components::EnterpriseComponent_ptr findEComponent(const PortableServer::ObjectId& oid);
	//end delete
	
	virtual int isConfigurationChanged();

    //member variables to management


public:

//	ContainerBase(CORBA::ORB_ptr orb);
	ContainerBase(CORBA::ORB_ptr orb,
					const PortableServer::ObjectId& oid,
					const char* componentServerUuid,
					Components::Deployment::ComponentServer_ptr server,
					STARCCM::Deployment::DeployDomainMgr_ptr manager);
	virtual ~ContainerBase();

	void configuration(Components::ConfigValues* config);
	virtual void passivate();
	virtual void incarnate();
	virtual void onConfigurationChanged();

	//added by xiao heping 2004/02/13
	bool isContainerTypeSetted();
	//end added by xiao heping 2004/02/13

	char* getUUID();
	char* getComponentServerUuid();
	void startup();
//	void initialize();

	Components::Deployment::Container_ptr getReference();
	//modified by xiao heping 2004/03/30
	//ContainerRunTime_var initializeRunTime();
	ContainerRunTime_ptr initializeRunTime();
	//end modify xiao heping 2003/03/30

	void shutdown();
//	void cleanup();
	STARCCM::Deployment::DeployDomainMgr_ptr getDomainManager();
        
	//deleted by xiao heping 2004/02/15
	//void demo();
	//end deleted
        
//	ContainerRunTime_ptr getContainerRunTime();

	//void onCreateComponent(CompositionManager_ptr comp);

	virtual void after_begin(const PortableServer::ObjectId& oid);
	virtual void before_completion(const PortableServer::ObjectId& oid);
	virtual void after_completion(const PortableServer::ObjectId& oid,CORBA::Boolean committed);

	//deleted by xiao heping 2004/03/04
/*
    virtual PortableServer::Servant preinvoke(const PortableServer::ObjectId& oid,
                                                                                                PortableServer::POA_ptr adapter,
                                                                                                const char* operation,
                                                                                                PortableServer::ServantLocator::Cookie& the_cookie);

    virtual void postinvoke(const PortableServer::ObjectId& oid,
                                                        PortableServer::POA_ptr adapter,
                            const char* operation,
                            PortableServer::ServantLocator::Cookie the_cookie,
                            PortableServer::Servant the_servant);

*/       
	//end delete

/*
	char* getHomeDir();
	void setHomeDir(const char *homeDir);

	char* getServerName();
	void setServerName(const char *serverName);
*/

	CORBA::ORB_ptr getOrb();

	//modified by xiao heping 2004/03/22
	virtual PortableServer::POA_ptr createComponentPOA(/*int isMultiThread,*/char* uuid,PortableServer::POA_ptr parentPoa,
                                                                  CustomDispatchThreadPool* threadpoolDs = NULL);
	virtual PortableServer::POA_ptr createHomePOA(/*int isMultiThread,*/char* uuid,CustomDispatchThreadPool* threadpoolDs = NULL);
//	virtual PortableServer::POA_ptr createComponentPOA(int isMultiThread,char* uuid,PortableServer::POA_ptr parentPoa);
//	virtual PortableServer::POA_ptr createHomePOA(int isMultiThread,char* uuid);
	//end modify xiao heping 2004/03/22

	ContainerType getContainerType();

	//added by xiao heping 2004/02/13
	void setContainerType(const ContainerType);

/*
	char* getContainerName();
	void setContainerName(const char* containerName);
      
	CORBA::Object_ptr resolveInitialReferences(char* id);
	void registerInitialReferences(char* id, CORBA::Object_ptr obj);
*/        
    virtual PortableServer::POA_ptr _default_POA();

    virtual Components::ConfigValues* configuration()
        throw(CORBA::SystemException);

    virtual Components::Deployment::ComponentServer_ptr get_component_server()
        throw(CORBA::SystemException);

    virtual Components::CCMHome_ptr install_home(const char* id,
                                                const char* entrypt,
                                                 const Components::ConfigValues& config)
        throw(Components::Deployment::UnknownImplId,
              Components::Deployment::ImplEntryPointNotFound,
              Components::Deployment::InstallationFailure,
             Components::Deployment::InvalidConfiguration,
              CORBA::SystemException);

    virtual void remove_home(Components::CCMHome_ptr href)
        throw(Components::RemoveFailure,
              CORBA::SystemException);

    virtual Components::CCMHomes* get_homes()
        throw(CORBA::SystemException);

    virtual void remove()
        throw(Components::RemoveFailure,
              CORBA::SystemException);
             
    //jxh0316
    ContainerRunTimeList getRuntimelist();
    //jxh          
              
    //SuLiang add. 2003.4.3
    void setUUID(const char* containerUUID);
    //SuLiang add. 2003.4.3
    
    //add by wsf 
    virtual STARCCM::ArchHome* getArchHome()
    	throw(CORBA::SystemException);
    virtual STARCCM::ArchComponent* getArchCom()
    	throw(CORBA::SystemException);
    virtual STARCCM::ArchConnection* getArchConn()
    	throw(CORBA::SystemException);
    virtual void updateArchitecture(const STARCCM::UUIDs& homeUUIDs,
    						STARCCM::ArchComponents_out addedcoms, 
    						STARCCM::DeletedComs_out dcomslist, 
						STARCCM::ArchConnections_out addedconns, 
						STARCCM::ArchConnections_out dconnslist)
    	throw(CORBA::SystemException);
    virtual void cleanArchObjects(const STARCCM::UUIDs& homeUUIDs)
    	throw(CORBA::SystemException);

    
    virtual void setArchHome(STARCCM::ArchHome& ahome);
    virtual void setArchCom(STARCCM::ArchComponent& acom);
    virtual void setArchConn(STARCCM::ArchConnection& aconn);

    //end by wsf

    //add by wsf 2004.6.29
	virtual void setRedirectReqInfo(const char* homeUUID,
							const CORBA::StringSeq& oidList, 
							const Components::ObjectSeq& newObjList,
							CORBA::Long timeout)
		throw(CORBA::SystemException);

	virtual void passivateClient(const char* homeUUID,
							const CORBA::StringSeq& oidList, 
							const Components::Deployment::PassivateClientTypeSeq& typeList)
		throw(CORBA::SystemException);
	
	virtual void activateClient(const char* homeUUID,
							const CORBA::StringSeq& oidList)
		throw(CORBA::SystemException);
    //end by wsf 2004.6.29    
    
};      //End of class ContainerBase

}       //End of namespace Container
#endif

