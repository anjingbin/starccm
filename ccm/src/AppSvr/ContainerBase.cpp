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

#include <ContainerBase.h>
#include <ExecutorInvoker.h>
#include <ContainerRunTime.h>
#include <ServantLoader.h>
#include <HomeServantBase.h>
#include <ExecutorLoader.h>
#include <ServantLoader.h>
#include <ConfigFile.h>
#include <ComponentServer_impl.h>

#include <CustomThreadPool.h>

//add by wsf
#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <resource.h>
typedef unsigned int (__stdcall *THREADROUTING)(void *);
#define SLEEP(sec) Sleep(sec*1000)
#else
#include <unistd.h>
#define SLEEP(sec) sleep(sec)
#endif
//end by wsf


using namespace Container;

//ContainerBase::ContainerBase(CORBA::ORB_ptr orb)
//{
//	
//	configedPolicy_ = new ::Container::ConfigedPolicy();
//	orb_ = CORBA::ORB::_duplicate(orb);
//	
//	CORBA::Object_var poaObject = orb_->resolve_initial_references("RootPOA");
//	
//	PortableServer::POA_var rootPoa = PortableServer::POA::_narrow(poaObject);
//	
//	CORBA::PolicyList policies;
//	policies.length(2);
//	
//	policies[0] = rootPoa -> create_lifespan_policy(PortableServer::PERSISTENT);
//	policies[1] = rootPoa -> create_id_assignment_policy(PortableServer::USER_ID);
//	
//	CORBA::String_var name = Components::UuidGenerator::generateUuid();
//	poa_ = rootPoa -> create_POA(name.in(), NULL, policies);
//	
//	deployed = 0;
//	ready = 0;
//
//}

ContainerBase::ContainerBase(
							CORBA::ORB_ptr orb, const PortableServer::ObjectId& oid,
							const char* componentServerUuid,
							Components::Deployment::ComponentServer_ptr server,
							STARCCM::Deployment::DeployDomainMgr_ptr manager)
	:orb_ (CORBA::ORB::_duplicate(orb)),
	containerId_ (new PortableServer::ObjectId(oid)),
	componentServerUuid_ (CORBA::string_dup(componentServerUuid)),
	server_ (Components::Deployment::ComponentServer::_duplicate(server)),	
	manager_(DeployDomainMgr::_duplicate(manager))	
{
	CORBA::Object_var poaObject = orb_->resolve_initial_references("RootPOA");
	PortableServer::POA_var rootPoa = PortableServer::POA::_narrow(poaObject);
	
	CORBA::PolicyList policies;
	policies.length(2);
	
	policies[0] = rootPoa -> create_lifespan_policy(PortableServer::PERSISTENT);
	policies[1] = rootPoa -> create_id_assignment_policy(PortableServer::USER_ID);
			       
	CORBA::String_var name = Components::UuidGenerator::generateUuid();
	poa_ = rootPoa -> create_POA(name.in(), NULL, policies);
	
//	deployed = 0;
	ready = 0;

	//added by xiao heping 2004/02/13
	ContainerTypeSetted = false;
	//end added by xiao heping 2004/02/13

//#ifndef LITE_VERSION
//	resPool_ = new ::STARCCM::ResourcePool_impl(this, 100);
//#endif
}

ContainerBase::~ContainerBase()
{
//	delete executorInvoker_;
//	delete homeExecutorInvoker_;

	//add by wsf
	runtimeMap.clear();
	//end by wsf
}


//deleted by xiao heping 2004/03/04
/*
PortableServer::Servant
ContainerBase::findServant(PortableServer::ObjectId* oid)
{
	PortableServer::Servant servant = NULL;
	
	TRACE0(LEVEL4,"Container search servant from Runtime infrastructure\n");
	
	ContainerRunTimeList::iterator pos;
	
	for( pos = runtimeList_.begin() ; pos != runtimeList_.end() ; pos++)
	{	
		servant = (*pos) -> getServant(oid);
		
		if(servant != NULL)
		{
			break;
		}
	}
	
	if(servant == NULL )
	{
		throw CORBA::OBJECT_NOT_EXIST();
	}
	
	return servant;
}
Components::EnterpriseComponent_ptr 
ContainerBase::findEComponent(const PortableServer::ObjectId& oid)
{
	return 0;//...?
}
*/
//end delete

//ConfigedPolicy_ptr
//ContainerBase::configedPolicy()
//{
//	return OBJDUPLICATE(ConfigedPolicy_ptr,configedPolicy_.in());
//}

PortableServer::POA_ptr
ContainerBase::_default_POA()
{
	return PortableServer::POA::_duplicate(poa_);
}
/*
char*
ContainerBase::getHomeDir()
{
	return CORBA::string_dup(homeDir_);
}

void
ContainerBase::setHomeDir(const char* homeDir)
{
	homeDir_ = CORBA::string_dup(homeDir);
}

char*
ContainerBase::getServerName()
{
	return CORBA::string_dup(serverName_);
}

void
ContainerBase::setServerName(const char *serverName)
{
	serverName_ = CORBA::string_dup(serverName);
}
*/

ContainerType
ContainerBase::getContainerType()
{
	return containerType_;
}

//added by xiao heping 2004/02/13
//
//in order to set the type of Container
//this function is invoked in ContainerRuntime
//
void 
ContainerBase::setContainerType(const ContainerType containerType)
{
	containerType_=containerType;
	ContainerTypeSetted = true;
}

bool ContainerBase::isContainerTypeSetted()
{
	return ContainerTypeSetted;
}
//end added by xiao heping 2004/02/13

/*
char*
ContainerBase::getContainerName()
{
	return CORBA::string_dup(name_);
}

void
ContainerBase::setContainerName(const char* containerName)
{
	name_ = CORBA::string_dup(containerName);
}
*/

PortableServer::POA_ptr
ContainerBase::createComponentPOA(/*int isMultiThread,*/char* uuid,PortableServer::POA_ptr parentPoa,
								  CustomDispatchThreadPool* pThreadpoolDs)
{
	TRACE0(LEVEL4,"Create component POA\n");
	TRACE1(LEVEL6,"POA id is %s \n",uuid);
	
	PortableServer::POAManager_var theManager = parentPoa->the_POAManager();
	CORBA::Object_var poaObject = orb_->resolve_initial_references("RootPOA");
	
	//modified by xiao heping 2004/02/16
#if defined(StarBus)
	STARPortableServer::POA_var rootPOA = STARPortableServer::POA::_narrow(poaObject);
#endif
#if defined(ORBacus)
	OBPortableServer::POA_var rootPOA = OBPortableServer::POA::_narrow(poaObject);
#endif
#if !defined(StarBus) && !defined(ORBacus)
	PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObject);
#endif
	//end of modify

	CORBA::PolicyList policies;
#ifdef WITH_OTS   
	policies.length(8);
#else
	policies.length(7);
#endif
	
//	if(isMultiThread)
//	{
	policies[0]= rootPOA -> create_thread_policy(PortableServer::ORB_CTRL_MODEL);
//	}
//	else
//	{
//		policies[0]=
//			rootPOA -> create_thread_policy(PortableServer::ORB_CTRL_MODEL);
//	}
	
    if( (containerType_ == SERVICECONTAINER) ||
    	(containerType_ == SESSIONCONTAINER) )
    {
    	policies[1] = rootPOA -> create_lifespan_policy(PortableServer::TRANSIENT);
    }
    else
    {
    	policies[1] = rootPOA -> create_lifespan_policy(PortableServer::PERSISTENT);
    }
    
    policies[2] = rootPOA -> create_id_assignment_policy(PortableServer::USER_ID);
	policies[3] = rootPOA -> create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
    policies[4] = rootPOA -> create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
    policies[5] = rootPOA -> create_servant_retention_policy(PortableServer::NON_RETAIN);
    policies[6] = rootPOA -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
       
#ifdef WITH_OTS   	
	//
    // The account requires a transaction be active
    //
    CORBA::Any any;
    any <<= CosTransactions::ADAPTS;    
    policies[7] = orb_ -> create_policy(CosTransactions::OTS_POLICY_TYPE,any);
#endif
	    
    	
    CORBA::String_var name = CORBA::string_dup(uuid);

//added by xiao heping 
#if defined(STARCCM_MTL) 
	if(pThreadpoolDs!=NULL)
	{
		int policylistLen=policies.length()+1;
		policies.length(policylistLen);
		#ifdef StarBus
			policies[policylistLen-1] = rootPOA -> create_requestlevel_dispatch_policy(pThreadpoolDs);
		#endif
		#ifdef ORBacus
			policies[policylistLen-1] = rootPOA -> create_dispatch_strategy_policy(pThreadpoolDs);
		#endif
	}
#endif //for STARCCM_MTL
//end of add

    PortableServer::POA_var newPOA = parentPoa->create_POA(name, theManager, policies);
//	PortableServer::POA_var newPOA = parentPoa->create_POA(name, theManager, NULL);

	TRACE0(LEVEL4,"Ending of create component POA\n");
    
    return newPOA._retn();
}


PortableServer::POA_ptr
ContainerBase::createHomePOA(/*int isMultiThread,*/char* uuid,CustomDispatchThreadPool* pThreadpoolDs)
{
	TRACE0(LEVEL4,"Create Home POA\n");
	TRACE1(LEVEL6,"POA id is %s\n",uuid);
//	TRACE1(LEVEL6,"POA is MultiThread : %d\n",isMultiThread);
		
	//PortableServer::POAManager_var theManager = poa_->the_POAManager();
	CORBA::Object_var poaObject = orb_->resolve_initial_references("RootPOA");
	
	//modified by xiao heping 2004/02/16
#if defined(StarBus)
	STARPortableServer::POA_var rootPOA = STARPortableServer::POA::_narrow(poaObject);
#endif
#if defined(ORBacus)
	OBPortableServer::POA_var rootPOA = OBPortableServer::POA::_narrow(poaObject);
#endif
#if !defined(StarBus) && !defined(ORBacus)
	PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObject);
#endif
	//end of modify
	
	CORBA::PolicyList policies;
#ifdef WITH_OTS   
	policies.length(8);
#else
	policies.length(7);
#endif
	
//	if(isMultiThread)
//	{
	policies[0]= rootPOA -> create_thread_policy(PortableServer::ORB_CTRL_MODEL);
//	}
//	else
//	{
//		policies[0]=
//			rootPOA -> create_thread_policy(PortableServer::ORB_CTRL_MODEL);
//	}
	
    if( (containerType_ == SERVICECONTAINER) ||
    	(containerType_ == SESSIONCONTAINER) )
    {
    	policies[1] = rootPOA -> create_lifespan_policy(PortableServer::TRANSIENT);
    }
    else
    {
    	policies[1] = rootPOA -> create_lifespan_policy(PortableServer::PERSISTENT);
    }
    policies[2] = rootPOA -> create_id_assignment_policy(PortableServer::USER_ID);
	policies[3] = rootPOA -> create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
    policies[4] = rootPOA -> create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
    policies[5] = rootPOA -> create_servant_retention_policy(PortableServer::NON_RETAIN);
    policies[6] = rootPOA -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
 
    
#ifdef WITH_OTS   
    //
    // The account requires a transaction be active
    //
    CORBA::Any any;
    any <<= CosTransactions::ADAPTS;
    policies[7] = orb_ -> create_policy(CosTransactions::OTS_POLICY_TYPE,any);
#endif
	    
    
    
    	
    CORBA::String_var name = CORBA::string_dup(uuid);

//added by xiao heping 
#if defined(STARCCM_MTL) 
	if(pThreadpoolDs!=0)
	{
		int policylistLen=policies.length()+1;
		policies.length(policylistLen);
		#ifdef StarBus
			policies[policylistLen-1] = rootPOA -> create_requestlevel_dispatch_policy(pThreadpoolDs);
			cout<<"dffffffffffffffffffffffff"<<policylistLen<<endl;
		#endif
		#ifdef ORBacus
			policies[policylistLen-1] = rootPOA -> create_dispatch_strategy_policy(pThreadpoolDs);
		#endif
	}
#endif //for STARCCM_MTL
//end of add

    PortableServer::POA_var newPOA = poa_->create_POA(name, NULL, policies);
//	PortableServer::POA_var newPOA = poa_->create_POA(name, theManager, NULL);

	TRACE0(LEVEL4,"End of create home POA\n");
    
    return newPOA._retn();	
}

void
ContainerBase::passivate()
{
	/* save conainer's states to persistent store */
	
	/* Etherealize the DLLs */
	TRACE0(LEVEL3,"Passivate Container's states to persistent storage\n");
	
	//SuLiang add.2003.3.28
/*	ContainerRunTime_var _tempRunTime;
	
	Container::ContainerRunTimeList::iterator _tempIterator = runtimeList_.begin();
	
	while(_tempIterator != runtimeList_.end())
	{
		_tempRunTime = *_tempIterator;
		_tempRunTime -> passivate();
	}
*/	
}

void
ContainerBase::incarnate()
{
	//SuLiang add.2003.4.3.
	//incarnate using runTime incarnate().
	
	//SuLiang add.2003.4.3.
	
/*	
    TRACE0(LEVEL4,"Load installed Components\n");
	ConfigFile_var config = new ConfigFile();
	
	CORBA::String_var containerUuid = PortableServer::ObjectId_to_string(containerId_.in());
	
	TRACE0(LEVEL4,"Load installed Component ids from config\n");
	CORBA::StringSeq_var componentIds = 
	    config -> getComponentIds(containerUuid.in());
	
	for(int i = 0;i < componentIds->length();i++ )
	{
	    TRACE0(LEVEL3,"Incarnate Container object from persistent storage\n");
	    
	    try
	    {
	        
	        ContainerRunTime_var runtime = initializeRunTime();
	        runtime -> initialize(config->getComponentThreading(componentIds[(CORBA::ULong)i].in()) );
	        
	        runtime -> incarnate(componentIds[(CORBA::ULong)i]);
	        
        	runtime -> startup();
	    }
	    catch(CORBA::Exception& ex)
	    {
	        
	    }
	}
	
	TRACE0(LEVEL4,"Load installed Components complete\n");
*/
}

void
ContainerBase::startup()
{
//	incarnate();
    Container_impl* containerImpl = new Container_impl(this,poa_);

	//modified by xiao heping 2004/04/01
//  PortableServer::ServantBase_var servant = containerImpl;
	containerServant_ = containerImpl;
	//end modify
    
    //not use the implicit activation
    poa_ -> activate_object_with_id(containerId_,containerImpl);
    
    CORBA::Object_var object = poa_ -> id_to_reference(containerId_);
	containerRef_ = Components::Deployment::Container::_narrow(object);
	
	poa_ -> the_POAManager() -> activate();
	ready = 1;
    TRACE0(LEVEL5,"Components::Container ready\n");
}

void
ContainerBase::shutdown()
{
	TRACE0(LEVEL1,"The System is going to shutdown!\n");
	passivate();
}

/*
void
ContainerBase::cleanup()
{
	
}

void
ContainerBase::initialize()
{
	
}
*/

//modified by xiao heping 2004/03/30
//ContainerRunTime_var
ContainerRunTime_ptr
//end modify xiao heping 2004/03/30
ContainerBase::initializeRunTime()
{
	TRACE0(LEVEL2,"Container initializing\n");
	/* Should we spawn another thread to run deployment container object? */
	
	ContainerRunTime_var runtime = new ContainerRunTime();
	runtime -> setContainer(this);

//#ifndef LITE_VERSION
//	runtime -> setResourcePool(resPool_);
//#endif
	
	//deleted by xiao heping 2004/02/15
	//runtimeList_.push_front(runtime);
	//end deleted
	
	TRACE0(LEVEL2,"ContainerRunTime initialized\n");
	
	return runtime._retn();
}

//void
//ContainerBase::onCreateComponent(CompositionManager_ptr comp)
//{
//
//}

//ContainerRunTime_ptr
//ContainerBase::getContainerRunTime()
//{
//	return runtime_;
//}

CORBA::ORB_ptr
ContainerBase::getOrb()
{
	return CORBA::ORB::_duplicate(orb_);
}

void
ContainerBase::onConfigurationChanged()
{
	if( isConfigurationChanged() )
	{
		passivate();
	}
	configurationChanged = 0;
}

int
ContainerBase::isConfigurationChanged()
{
	return configurationChanged;
}

void
ContainerBase::after_begin(const PortableServer::ObjectId& oid)
{
	/**/
}

void
ContainerBase::before_completion(const PortableServer::ObjectId& oid)
{
	/**/
}

void
ContainerBase::after_completion(const PortableServer::ObjectId& oid,CORBA::Boolean committed)
{
	/**/
}
/*
CORBA::Object_ptr
ContainerBase::resolveInitialReferences(char* id)
{
	return NULL;
}

void
ContainerBase::registerInitialReferences(char* id, CORBA::Object_ptr obj)
{
}
*/
//deleted by xiao heping 2004/03/04
/*
PortableServer::Servant
ContainerBase::preinvoke(const PortableServer::ObjectId& oid,
						PortableServer::POA_ptr adapter,
						const char* operation,
						PortableServer::ServantLocator::Cookie& the_cookie)
{
	TRACE0(LEVEL3,"Container preinvoking\n");
	PortableServer::Servant servant;
	
	try
	{
		PortableServer::ObjectId* objectId = new PortableServer::ObjectId(oid);
		
		//modified by xiao heping 2004/02/13

		//servant = findServent(objectId);
		servant = findServant(objectId);

		//end modified by xiao heping 2004/02/13
		
		
		delete objectId;
	}
	catch(CORBA::OBJECT_NOT_EXIST&)
	{
		throw;
	}
	catch(...)
	{
		TRACE0(LEVEL1,"EXCEPTION:FINDING SERVANT\n");
	}
	
	TRACE0(LEVEL6,"containerBase preinvoke returning\n");
	return servant;
}

void
ContainerBase::postinvoke(const PortableServer::ObjectId& oid,
							PortableServer::POA_ptr adapter,
                            const char* operation,
                            PortableServer::ServantLocator::Cookie the_cookie,
                            PortableServer::Servant the_servant)
{
	TRACE0(LEVEL6,"Container postinvoking\n");
	
}
*/
//end delete

Components::ConfigValues*
ContainerBase::configuration()
	throw(CORBA::SystemException)
{
	if( configuration_ == NULL )
	{
		return NULL;
	}
	
	Components::ConfigValues* ret = new Components::ConfigValues(configuration_);
	return ret;
}

Components::Deployment::ComponentServer_ptr
ContainerBase::get_component_server()
	throw(CORBA::SystemException)
{
	return Components::Deployment::ComponentServer::_duplicate(server_);
}

//
//The install_home operation installs and returns a new CCMHome object. The id and
//entrypt parameters are used by the container to locate an implementation file and
//instantiate a new home object. The config parameter represents a sequence of
//ConfigValue objects that provide name value pairs used to configure the installation
//of the new home instance; for example, provide persistency source, transaction, and
//security policies that must be applied to the home and its components. The operation
//raises an UnknownImplId exception if the id parameter does not correspond to any
//component packages installed using the ComponentInstallation object. The
//operation raises an ImplEntryPointNotFound exception if the entrypt parameter
//cannot be found in the implementation returned from ComponentInstallation. The
//operation raises an InstallationFailure exception if the home could not be installed
//in the container for internal reasons such as insufficient resources or inadequate
//implementation for this container; for example, installing a C++ home implementation
//in a Java container. The operation raises an InvalidConfiguration exception if the
//config parameter does not contain valid configuration name value pairs.
//
Components::CCMHome_ptr
ContainerBase::install_home(const char* id,
				const char* entrypt,
				const Components::ConfigValues& config)
	throw(Components::Deployment::UnknownImplId,
		Components::Deployment::ImplEntryPointNotFound,
		Components::Deployment::InstallationFailure,
		Components::Deployment::InvalidConfiguration,
		CORBA::SystemException)
{
	//Saving configuration values
	TRACE0(LEVEL4,"Save configuration values for this component package\n");
	
//	if( deployed == 1)
//	{
//		TRACE0(LEVEL1,"The Container has been deployed\n");
//		throw new Components::Deployment::InstallationFailure();
//	}
	if( ready == 0 )
	{
		TRACE0(LEVEL1,"The Container hasnt been ready\n");
		throw new Components::CCMException();
	}
	
	ContainerRunTime_var runtime = initializeRunTime();

	//modified by xiao heping 2004/03/23
	Components::CCMHome_var home;
	try
	{
		home = runtime -> install_home(id,entrypt,config);
	}
	catch(Components::Deployment::UnknownImplId& ex)
	{
		throw ex;
	}
	catch(Components::Deployment::ImplEntryPointNotFound& ex)
	{
		throw ex;
	}
	catch(Components::Deployment::InstallationFailure& ex)
	{
		throw ex;
	}
	catch(Components::Deployment::InvalidConfiguration& ex)
	{
		throw ex;
	}
	catch(...)
	{
		throw;
	}
	//end modify xiao heping 2004/03/23
	
//	PortableServer::ObjectId_var oid = runtime->getHomeObjectId();
	
//	CORBA::String_var s = PortableServer::ObjectId_to_string(oid);
	
//	std::string key(s.in());

	//added by xiao heping 2003/03/23
	{
		STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)	

		runtimeList_.push_front(runtime);
		
		//add by wsf
		PortableServer::ObjectId_var oid = runtime->getHomeObjectId();
		CORBA::String_var homeuuid = PortableServer::ObjectId_to_string(oid.in());
		runtimeMap.insert(MapRunTime::value_type(string(homeuuid.in()), runtime));
		//end by wsf
	}
	//end add xiao heping 2003/03/23
	
	
//	objectIdMap_.insert(HomeObjectIdMap::value_type(key,runtimeList_.begin()));
	
//	runtime -> startup();

	//SuLiang add. 2004.2.27
	//to remove the home,we add a map to save the home and runtime.
//	CORBA::String_var homeChar = orb_ -> object_to_string(home);
//	std::string homeRef(homeChar.in()); 
//	homeRuntimeMap_.insert(MapHomeRuntime::value_type(homeRef,runtime));
	//SuLiang add. 2004.2.27
	
	TRACE0(LEVEL6, "install home complete\n");
	
	return home._retn();
}

//
//The remove_home operation removes a home from the container and all components
//that were created by this home. The operation raises a BAD_PARAM system
//exception if the href parameter was not created by this container. 
//The operation raises a RemoveFailure exception if the home or any of its components 
//could not be removed from the container for any internal reasons.
//
void
ContainerBase::remove_home(Components::CCMHome_ptr href)
	throw(Components::RemoveFailure,CORBA::SystemException)
{
	//modified by xiao heping 2003/03/13

	STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)

	//
	//the href parameter was created by this container?
	//
	ContainerRunTimeList::iterator iter= runtimeList_.begin();
	while(iter != runtimeList_.end())
	{
		ContainerRunTime_var runtime = (*iter);

		//
		//get CCMHome object from containerRuntime
		//
		Components::CCMHome_var home = runtime -> getCcmHome();
		
		if( home -> _is_equivalent(href) )
		{
			try
			{
				//
				//this operation may be throw Components::RemoveFailure exception
				//
				runtime -> cleanup();
			}
			catch(Components::RemoveFailure& ex)
			{
				throw ex;
			}
			
			//add by wsf
			PortableServer::ObjectId_var oid = runtime->getHomeObjectId();
			CORBA::String_var homeuuid = PortableServer::ObjectId_to_string(oid);
			MapRunTime::iterator runtime_iter = runtimeMap.find(string(homeuuid));
			runtimeMap.erase(runtime_iter);
			//end by wsf
			
			runtimeList_.erase(iter);

			//added by xiao heping 2004/03/30
			PortableServer::POA_var homePoa = runtime -> getHomePoa();
			PortableServer::POA_var componentPoa = runtime -> getComponentPoa();
			componentPoa ->destroy(false,false);
			homePoa -> destroy(false,false);
		#if defined(STARCCM_MTL)
			GreenThread::CustomThreadPool_var customThreadPool = runtime->GetCustomThreadPool();
			customThreadPool->destroy();
		#endif
			break;
			//end add xiao heping 2004/03/30
		}
		iter++;
	}

	if(iter == runtimeList_.end())
	{
		TRACE0(LEVEL1,"Fatal Error : the home doesn't exit in the container.\n");
		throw new CORBA::BAD_PARAM();
	}

	//end modify xiao heping 2003/03/13
/*
	//
	//Undo all registration work!
	//Stop POA
	//
	PortableServer::POA_var homePoa = runtime -> getHomePoa();
	PortableServer::POA_var componentPoa = runtime -> getComponentPoa();

	PortableServer::POAManager_var homePoaManager = homePoa -> the_POAManager();
	PortableServer::POAManager_var componentPoaManager = componentPoa -> the_POAManager();

	
	try
	{
		componentPoaManager -> deactivate(false,true);
		homePoaManager -> deactivate(false,true);
	}
	catch(...)
	{
		TRACE0(LEVEL2,"ERROR:Home POA Error!!\n");
		throw new Components::CCMException();
	}
	*/
}

Components::CCMHomes*
ContainerBase::get_homes()
	throw(CORBA::SystemException)
{
	//added by xiao heping 2003/03/23
	STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)
	//end add xiao heping 2003/03/23

	Components::CCMHomes_var ccmHomes = new Components::CCMHomes();
	ccmHomes -> length(runtimeList_.size());
	
	ContainerRunTimeList::iterator pos;

	int i = 0 ;
	for( pos = runtimeList_.begin(); pos != runtimeList_.end() ; pos++ , i++ )
	{
		ccmHomes[i] =  (*pos) -> getCcmHome();
	}
	
	return ccmHomes._retn();
}


//
//The remove operation removes the container and all homes and components created
//in the container. The operation raises a RemoveFailure exception if the container or
//any of its homes could not be removed for any internal reasons.
//
void
ContainerBase::remove()	throw(Components::RemoveFailure,CORBA::SystemException)
{
//	poa_ -> the_POAManager() -> deactivate(false,true);
	
	//modified by xiao heping 2004/03/13

	//
	//get all homes created by this container
	//
	Components::CCMHomes_var ccmHomes = get_homes();
	int len = ccmHomes->length();
	
	for(int i = 0;i < len;i++)
	{
		try
		{
			remove_home(ccmHomes[i]);
		}
		catch(...)
		{
			throw;
		}
	}
	poa_->deactivate_object(containerId_);
	//end modify xiao he ping 2004/03/13
}

void
ContainerBase::configuration(Components::ConfigValues* config)
{
	configuration_ = new Components::ConfigValues(*config);
}

Components::Deployment::Container_ptr
ContainerBase::getReference()
{
	return Components::Deployment::Container::_duplicate(containerRef_.in());
}

STARCCM::Deployment::DeployDomainMgr_ptr
ContainerBase::getDomainManager()
{
	return STARCCM::Deployment::DeployDomainMgr::_duplicate(manager_);
}

char*
ContainerBase::getUUID()
{
    CORBA::String_var ret = PortableServer::ObjectId_to_string(containerId_.in());
    return ret._retn();
}

char*
ContainerBase::getComponentServerUuid()
{
    return CORBA::string_dup(componentServerUuid_);
}

void 
ContainerBase::setUUID(const char* containerUUID)
{
	containerId_ = PortableServer::string_to_ObjectId(containerUUID);
}

//end add

ContainerRunTimeList ContainerBase::getRuntimelist()
{
	STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)
	return runtimeList_;
}

//wsf
void 
ContainerBase::setArchHome(STARCCM::ArchHome& ahome)
{
	this->ahome = ahome; 
}

//wsf
STARCCM::ArchHome* 
ContainerBase::getArchHome()
	throw(CORBA::SystemException)
{
	STARCCM::ArchHome* tmp = new STARCCM::ArchHome(ahome);
	return tmp;
}

//wsf
void 
ContainerBase::setArchCom(STARCCM::ArchComponent& acom)
{
	this->acom = acom;
}

//wsf
STARCCM::ArchComponent* 
ContainerBase::getArchCom()
    	throw(CORBA::SystemException)
{
	STARCCM::ArchComponent* tmp = new STARCCM::ArchComponent(acom);
	return tmp;
}

//wsf
void 
ContainerBase::setArchConn(STARCCM::ArchConnection& aconn)
{
	this->aconn = aconn;
}

//wsf
STARCCM::ArchConnection* 
ContainerBase::getArchConn()
    	throw(CORBA::SystemException)
{
	STARCCM::ArchConnection* tmp = new STARCCM::ArchConnection(aconn);
	return tmp;
}

//wsf
void 
ContainerBase::updateArchitecture(const STARCCM::UUIDs& homeUUIDs,
				STARCCM::ArchComponents_out addedcoms, 
				STARCCM::DeletedComs_out dcomslist, 
				STARCCM::ArchConnections_out addedconns, 
				STARCCM::ArchConnections_out dconnslist)
    throw(CORBA::SystemException)
{
	STARCCM::ArchComponents aComponents;
	STARCCM::DeletedComs deletedComslist;
	STARCCM::ArchConnections aConnections;
	STARCCM::ArchConnections deletedConnslist;

	STARCCM::ArchComponents_var acoms;
	STARCCM::DeletedComs_var dcoms;
	STARCCM::ArchConnections_var aconns;
	STARCCM::ArchConnections_var dconns;

	for (int i=0; i<homeUUIDs.length(); i++)
	{
		{
			STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)
			MapRunTime::iterator runtime_iter = runtimeMap.find(string(homeUUIDs[i]));
			ContainerRunTime_var runtime = (*runtime_iter).second;
			runtime->updateArchitecture(acoms.out(),dcoms.out(),aconns.out(),dconns.out());
			runtime->cleanArchObjects();
		}

		int len,alen;
		len = acoms->length();
		alen = aComponents.length();
		aComponents.length(alen+len);
		int k=0;
		int j;
		for (j=alen; j<alen+len; j++)
			aComponents[j] = acoms[k++];

		len = dcoms->length();
		alen = deletedComslist.length();
		deletedComslist.length(alen+len);
		k=0;
		for (j=alen; j<alen+len; j++)
			deletedComslist[j] = dcoms[k++];

		len = aconns->length();
		alen = aConnections.length();
		aConnections.length(alen+len);
		k=0;
		for (j=alen; j<alen+len; j++)
			aConnections[j] = aconns[k++];

		len = dconns->length();
		alen = deletedConnslist.length();
		deletedConnslist.length(alen+len);
		k=0;
		for (j=alen; j<alen+len; j++)
			deletedConnslist[j] = dconns[k++];
	}

	addedcoms = new STARCCM::ArchComponents(aComponents);
	dcomslist = new STARCCM::DeletedComs(deletedComslist);
	addedconns = new STARCCM::ArchConnections(aConnections);
	dconnslist = new STARCCM::ArchConnections(deletedConnslist);

	cleanArchObjects(homeUUIDs);
}

//wsf
void 
ContainerBase::cleanArchObjects(const STARCCM::UUIDs& homeUUIDs)
    	throw(CORBA::SystemException)
{
	STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)
	for (int i=0; i<homeUUIDs.length(); i++)
	{
		MapRunTime::iterator runtime_iter = runtimeMap.find(string(homeUUIDs[i]));
		ContainerRunTime_var runtime = (*runtime_iter).second;
		runtime->cleanArchObjects();

	}
}

//add by wsf 2004.6.29
void 
ContainerBase::setRedirectReqInfo(const char* homeUUID,
								const CORBA::StringSeq& oidList, 
								const Components::ObjectSeq& newObjList,
								CORBA::Long timeout)
	throw(CORBA::SystemException)
{
	STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)
	MapRunTime::iterator runtime_iter = runtimeMap.find(string(homeUUID));
	if (runtime_iter != runtimeMap.end())
	{
		ContainerRunTime_var runtime = (*runtime_iter).second;
		runtime->setRedirectReqInfo(oidList, newObjList, timeout);
	}
}

void 
ContainerBase::passivateClient(const char* homeUUID,
							const CORBA::StringSeq& oidList, 
							const Components::Deployment::PassivateClientTypeSeq& typeList)
	throw(CORBA::SystemException)
{
	STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)
	MapRunTime::iterator runtime_iter = runtimeMap.find(string(homeUUID));
	if (runtime_iter != runtimeMap.end())
	{
		ContainerRunTime_var runtime = (*runtime_iter).second;
		runtime->passivateClient(oidList, typeList);
	}
}

void 
ContainerBase::activateClient(const char* homeUUID,
							const CORBA::StringSeq& oidList)
	throw(CORBA::SystemException)
{
	STARCCM_SYNCHRONIZED(sync_,runtimeListLock_)
	MapRunTime::iterator runtime_iter = runtimeMap.find(string(homeUUID));
	if (runtime_iter != runtimeMap.end())
	{
		ContainerRunTime_var runtime = (*runtime_iter).second;
		runtime->activateClient(oidList);
	}
}
//end by wsf 2004.6.29