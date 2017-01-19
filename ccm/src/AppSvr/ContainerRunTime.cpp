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

#include <ContainerRunTime.h>
#include <ContainerBase.h>
#include <UuidGenerator.h>
#include <ExecutorInvoker.h>
#include <string>
#include <FileAccessor.h>
#include <DeployDomainMgr.h>

#ifdef WIN32
#include <conio.h>
#include <time.h>
#else
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <ConfigFile.h>
#include <CCM2Context_impl.h>
#include <UuidGenerator.h>
//added by xiao heping 2004/03/04
//end add

#include <Cookie.h>  //added by xiao heping

#include <stdlib.h>


#ifndef LITE_VERSION
#include <CosPersistentState.h>
#endif

extern "C"
{
	#include "uuid.h"
}

#include <CommonFunc.h>

using namespace Container;

ContainerRunTime::ContainerRunTime(/*const char *homeRepId,
								   Components::HomeExecutorBase_ptr home,
								   PortableServer::POA_ptr homePoa,
								   PortableServer::POA_ptr componentPoa*/)
{
//	homeRepId_ = CORBA::string_dup(homeRepId);
//	homePoa_ = PortableServer::POA::_duplicate(homePoa);
//	componentPoa_ = PortableServer::POA::_duplicate(componentPoa);
//	home_ = Components::HomeExecutorBase::_duplicate(home);

	runningComponentsPool_ = new ComponentsPool();
	
	Components::UuidGenerator_var gen = new Components::UuidGenerator();
	homeCompositionUUID_ = gen -> generateUuid();
	passivator_ = new PassivationArbitor();
	passivator_->setPassivatePolicy(COMPONENTCOUNT);
	passivator_->setComponentCount(100);
	
	resPool_ = NULL;
	//added by xiao heping 2004/03/24
	destroy_ = false;
}

ContainerRunTime::~ContainerRunTime()
{
	TRACE0(LEVEL6,"ContainerRunTime::~ContainerRunTime()!\n");
	executorInvoker_ -> _remove_ref();
	homeExecutorInvoker_ -> _remove_ref();
}


//void
//ContainerRunTime::setCompRepId(const char *compRepId)
//{
//	compRepId_ = CORBA::string_dup(compRepId);
//}

char*
ContainerRunTime::getCompRepId()
{
	return CORBA::string_dup(compRepId_);
}

Components::HomeExecutorBase_ptr
ContainerRunTime::getHomeExecutor()
{
	return homeComposition_ -> getHomeExecutor();
}

//void
//ContainerRunTime::setHomeExecutor(Components::HomeExecutorBase_ptr home)
//{
//	homeComposition_ -> registerHomeExecutor(home);
//}

//Components::EnterpriseComponent_ptr
//ContainerRunTime::getExecutor()
//{
//	return NULL;
//}

char*
ContainerRunTime::getHomeRepId()
{
	return CORBA::string_dup(homeRepId_);
}

//void
//ContainerRunTime::setHomeRepId(const char *homeRepId)
//{
//	homeRepId_ = CORBA::string_dup(homeRepId);
//}

PortableServer::POA_ptr
ContainerRunTime::getHomePoa()
{
	return PortableServer::POA::_duplicate(homePoa_);
}

PortableServer::POA_ptr
ContainerRunTime::getComponentPoa()
{
	return PortableServer::POA::_duplicate(componentPoa_);
}

//deleted by xiao heping 2004/03/18
/*
CORBA::Object_ptr
ContainerRunTime::getExecutor(PortableServer::ObjectId* oid)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	CORBA::Object_var executor;

	CORBA::String_var s = PortableServer::ObjectId_to_string(*oid);
	
	std::string id(s.in());
	
	PortableServer::ObjectId_var hId = homeComposition_ -> getObjectId();
	CORBA::String_var homeId = PortableServer::ObjectId_to_string(hId);
	
	std::string homeOid(homeId.in());
	
	if( strcmp(id.c_str(),homeOid.c_str()) == 0 )
	{
		return homeComposition_ -> getHomeExecutor();
	}
		
	executor = runningComponentsPool_->getExecutor(oid);
	
	return executor._retn();
}
*/
//end delete xiao heping 2004/03/18

PortableServer::Servant
ContainerRunTime::getServant(PortableServer::ObjectId* oid,
							 PortableServer::ServantLocator::Cookie& the_cookie,
							 const char* operation)
{
	//modified by xiao heping 2004/03/23
	CORBA::String_var id = PortableServer::ObjectId_to_string(*oid);
//	std::string id(s.in());
	PortableServer::ObjectId_var hId = homeComposition_ -> getObjectId();
	CORBA::String_var homeOid = PortableServer::ObjectId_to_string(hId);
//	std::string homeOid(homeId.in());
	
//	if( strcmp(id.c_str(),homeOid.c_str()) == 0 )

	//end modify xiao heping 2004/03/23

	if( strcmp(id.in(),homeOid.in()) == 0 )
	{
		TRACE0(LEVEL6,"Home servant found\n");

		//added by xiao heping 2004/03/25
		if(destroy_)
			return 0;
//		the_cookie = (void*)dynamic_cast<CompositionManager*>(homeComposition_.in());
		::Container::Cookie* cookie = new ::Container::Cookie(homeComposition_.in());
		the_cookie = (void*)cookie;
		homeComposition_ -> _add_ref();
		//end add xiao heping 2004/03/25

		return homeComposition_ -> getHomeServant();
	}
	return runningComponentsPool_ -> getServant(oid,the_cookie,operation);
}

PortableServer::Servant
ContainerRunTime::getComponentServant(PortableServer::ObjectId* oid)
{
	ComponentComposition_var componentComposition = runningComponentsPool_->getComposition(oid);
	
	if( componentComposition.in() == NULL )
	{
	    TRACE0(LEVEL3,"Component Composition not found\n");
	    return NULL;
	}
	return componentComposition->getComponentServant();
}

int
ContainerRunTime::getCompositionType(PortableServer::ObjectId* oid)
{
	//modified by xiao heping 2004/03/23
	CORBA::String_var id = PortableServer::ObjectId_to_string(*oid);
//	std::string id(s.in());
	
	PortableServer::ObjectId_var hId = homeComposition_ -> getObjectId();
	CORBA::String_var homeOid = PortableServer::ObjectId_to_string(hId);
//	std::string homeOid(homeId.in());
	
//	if( strcmp(id.c_str(),homeOid.c_str()) == 0 )

	//end modify xiao heping 2004/03/23
	if( strcmp(id.in(),homeOid.in()) == 0 )
	{
		return HOME;
	}
	return COMPONENT;
}

void
ContainerRunTime::passivate()
{
#ifndef LITE_VERSION
	//get the connector.
	CosPersistentState::Session_var _session = storageLoader_ -> getSession();
//	CORBA::String_var _connectorId= pssInitializer_->getConnectorId();
//	CosPersistentState::Connector_var _connector = _connectorReg -> find_connector(_connectorId.in());
//	
//	if(!CORBA::is_nil(_connector))
//	{
//		TRACE0(LEVEL1,"ContainerRunTime::passivate() cannot find the connector!");
//	}
//	
//	//get the session.
//	short _mode = pssInitializer_ -> getAccessMode();
//	CosPersistentState::ParameterList_var _paraList = pssInitializer_ -> getParaeters();
//	CosPersistentState::Session_var _session = _connector -> create_basic_session(_mode,_paraList);
//	
//	//register_persistent_factories().RunTime_pss.cpp.
//	register_persistent_factories(_connector);
	
	
	//get the homes from PSS.
	CosPersistentState::StorageHomeBase_var _runTimeHomeBasePSS = _session -> find_storage_home(ContainerPSS::RunTimeHome::id());
	ContainerPSS::RunTimeHome_var _runTimeHomePSS = ContainerPSS::RunTimeHome::_downcast(_runTimeHomeBasePSS);
	
//	CosPersistentState::StorageHomeBase_var _poolHomeBasePSS = _session -> find_storage_home("PSDL:ContainerPSS/PoolHome:1.0");
//	ContainerPSS::RunTimeHome_var _poolHomePSS = ContainerPSS::RunTimeHome::_narrow(_poolHomePSSBase);
//	
//	CosPersistentState::StorageHomeBase_var _componentHomeBasePSS = _session -> find_storage_home("PSDL:ContainerPSS/ComponentHome:1.0");
//	ContainerPSS::RunTimeHome_var _componentHomePSS = ContainerPSS::RunTimeHome::_narrow(_componentHomeBasePSS);
	
	//get the storageObjects from PSS.
	
	//save the runtime to PSS.
//	CosPersistentState::Pid_var _runTimePid = _connector -> get_pid(this);
//	
//	if(CORBA::is_nil(_runTimePid))
//	{
//		ContainerPSS::RunTime_var _runTime = _runTimeHomePSS -> create( uuid_.in() );
//	}
//	else
//	{
//		CosPersistentState::StorageObject_var _runTimeObj = _session -> find_by_pid(_runTimePid);
//		ContainerPSS::RunTime_var _runTime = ContainerPSS::RunTime::_narrow(_runTimeObj);
//		_runTime -> uuid( uuid_.in());
//	}
//	
	try
	{
		ContainerPSS::RunTime_var _runTime = ContainerPSS::RunTime::_downcast( _runTimeHomePSS -> find_by_uuid( uuid_.in() ));
		_runTime -> uuid( uuid_.in());
		_runTime -> homeRepositoryId( this -> getHomeRepId());
   	    _runTime -> componentRepositoryId ( this -> getCompRepId());
	
	    _runTime -> homePoaId(this -> homePOAName_.in());
	    _runTime -> componentPoaId(this -> componentPOAName_.in());
	
	    _runTime -> containerUuid(container_ -> getUUID());
		_runTime -> homeCompositionUuid(this -> homeCompositionUUID_.in());
		_session -> flush();
	}
	

	catch(CosPersistentState::NotFound&  )
//	if(CORBA::is_nil(_runTime))
	{
		ContainerPSS::RunTime_var _runTime = ContainerPSS::RunTime::_downcast( _runTimeHomePSS -> create( uuid_.in() ));
	
//	}
//	else
	{
		_runTime -> uuid( uuid_.in());
	}
	
	_runTime -> homeRepositoryId( this -> getHomeRepId());
	_runTime -> componentRepositoryId ( this -> getCompRepId());
	
	_runTime -> homePoaId(this -> homePOAName_.in());
	_runTime -> componentPoaId(this -> componentPOAName_.in());
	
	_runTime -> containerUuid(container_ -> getUUID());
	
//	PortableServer::ObjectId_var _poolId = PortableServer::POA::reference_to_id(runningComponentsPool_);
//	_runTime -> poolUuid(PortableServer::ObjectId_to_string(_poolId));
	
	_runTime -> homeCompositionUuid(this -> homeCompositionUUID_.in());
	
	//flush() the PSS.
	_session -> flush();
	}
#endif
	
}

void
ContainerRunTime::incarnate(const char* uuid)
{
#ifndef LITE_VERSION	
	//SuLiang add . 2003.4.3.
	//in this codes ,they can be replaced.!!!
	
	/*
	//get the connector.
	CosPersistentState::Session_var _session = storageLoader_ -> getSession();
	
	//get the homes from PSS.
	CosPersistentState::StorageHomeBase_var _runTimeHomeBasePSS = _session -> find_storage_home(ContainerPSS::RunTimeHome::id());
	ContainerPSS::RunTimeHome_var _runTimeHomePSS = ContainerPSS::RunTimeHome::_downcast(_runTimeHomeBasePSS);
	ContainerPSS::RunTime_var _runTime = ContainerPSS::RunTime::_downcast( _runTimeHomePSS -> find_by_uuid( uuid ));
	
	homeRepId_ = _runTime -> homeRepositoryId();
    compRepId_ = _runTime -> componentRepositoryId ();

    homePoa_ = _runTime -> homePoaId();
    componentPoa_ = _runTime -> componentPoaId();

    container_ -> setUUID( _runTime -> containerUuid() );
	homeCompositionUUID_ = _runTime -> homeCompositionUuid();
	*/
	
	//SuLiang add . 2003.4.3.
	
	
/*	
	
    ConfigFile_var config = new ConfigFile();
    
    CORBA::String_var ret;
	try
	{
	    
        ret = config->getServantFile(uuid);
        servantsLoader_ = new ServantLoader(ret.in(), this);
        ret = config->getExecutorFile(uuid);
        executorsLoader_ = new ExecutorLoader(ret.in(), this);
        ret = config->getPersistentFile(uuid);
        storageLoader_ = new StorageObjectLoader(ret, this);
        
        storageLoader_->loadPSS();
        
        ret = config->getExecutorEntry(uuid);
        Components::HomeExecutorBase_var homeExecutor 
            = executorsLoader_ -> loadHomeExecutor(ret.in());
            
        HomeComposition_var homeComposition = new HomeComposition();
        homeComposition->registerHomeExecutor(homeExecutor);
    
    	this -> setHomeComposition(homeComposition);
    	

    	
    	//???
    	CORBA::String_var uniqueId = Components::UuidGenerator::generateUuid();
    	
    	PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(uniqueId);
    	//homeComposition->setObjectId(oid);
    	
    	ret = config->getServantEntry(uuid);
    	PortableServer::Servant homeServant
    	    = servantsLoader_->loadHomeServant(ret.in());
    	
    	if(homeServant == NULL)
    	{
    		TRACE0(LEVEL1,"Load Home servant from dll failed\n");
        }
    
    	homeComposition->registerHomeServant(oid,homeServant);
    	
    	
    	    	
    	
    	//SuLiang add . 2003.4.3.
    	//set the homeComposition RepId.
    	
    	
    	ret = config->getHomeRepId(uuid);
    	homeComposition->setRepId(ret.in());
    	
    	//registry value type and factories
    	CORBA::StringSeq_var repIds = config->getValueTypeFactoryRepId(uuid);
    	CORBA::StringSeq_var valueEntries = config->getValueTypeEntry(uuid);
    	int i;

    	for(i = 0; i < repIds->length(); ++i)
    	{
    	    executorsLoader_->loadValueFactory(valueEntries[i], repIds[i]);
    	}
    	
    	//set policies
    	CORBA::StringSeq_var policies;
    	
    	ret = config->getHomeRepId(uuid);
    	policies = config->getHomeMethodPolicies(uuid, ret.in());
    	
    	for(i = 0; i < policies->length(); ++i)
    	{
			executorInvoker_->setTxnPolicy(ret, policies[i*2], policies[i*2+1]);
    	}
    	
    	repIds = config->getComponentRepIds(uuid);
    	
    	for(i = 0; i < repIds->length(); ++i)
    	{
    	    policies = config->getComponentMethodPolicies(uuid, repIds[i]);

    	    for(int j = 0; j < policies->length(); ++j)
    	    {
    	        executorInvoker_->setTxnPolicy(repIds[i], policies[j*2], policies[j*2+1]);
    	    }
    	}
    	
	}
	catch(CORBA::Exception& ex)
	{
	        
	}
*/
#endif
	
}

void
ContainerRunTime::shutdown()
{
	
}

void
ContainerRunTime::cleanup()
{
	//modified by xiao heping 2003/03/13
	if (destroy_)
		return ;
	destroy_ = true;

	try
	{
		runningComponentsPool_->cleanup();
	}
	catch(Components::RemoveFailure& ex)
	{
		throw ex;
	}

	//
	//get home object from ContainerRuntime
	//
	Components::CCMHome_var home = getCcmHome();

	::STARCCM::HomeRegistration_var homeReg;
	try
	{
		CORBA::ORB_var orb=container_->getOrb();
		CORBA::Object_var objReg = orb->string_to_object(homeRegistration_);        
		homeReg = ::STARCCM::HomeRegistration::_narrow(objReg);
	}
	catch(...)
	{
		TRACE0(LEVEL2,"ERROR:HomeRegisrtation NOT found.\n");
		throw new Components::RemoveFailure ();
	}
	assert(!CORBA::is_nil(homeReg));

	//
	//unregister home object
	//
	try
	{
		homeReg -> unregister_home(home);
	}
	catch(...)
	{
		throw new Components::RemoveFailure();
	}
	//end modify xiao heping 2004/03/13

	
	//Unload the DLL.SuLiang add. 2004.2.25
	//deleted by xiao heping 2004/03/29
//	executorsLoader_ -> etherealize();
//	servantsLoader_ -> etherealize();
	//end deleted xiao heping 2004/03/29

	//Unload the DLL.SuLiang add. 2004.2.25
}

//void ContainerRunTime::removeHome(Components::CCMHome_ptr href)
//{
//	PortableServer::ObjectId_var oid;
//	
//	oid = homePoa_ -> reference_to_id(href);
//	
//	//do all cleanup works
//}

void
ContainerRunTime::onCreateComponent(ComponentComposition_ptr comp)
{
	//modified by jxh 03/22
	//runningComponentsPool_ -> addComponent(compRepId_,comp);
	runningComponentsPool_ -> addComponent(comp);
	
	passivator_->onCreate();
	
	if( passivator_->canPassiavte() )
	{
		//how to passivate?
	}
}

void
ContainerRunTime::onRemoveComponent(const PortableServer::ObjectId* oid)
{
	//deleted by wsf
	//runningComponentsPool_ -> removeComponent(oid);
	
	//passivator_->onRemove();
	//end by wsf
	
	//add by wsf
	//To delete the according archcomponent info
	deleteArchComponent(oid);
	//end by wsf
}

CIFHook_ptr
ContainerRunTime::getCifHook()
{
	if( cifHook_ == NULL)
	{
		cifHook_ = new CIFHook(this);
	}
	return OBJDUPLICATE(CIFHook_ptr,cifHook_);
}

void
ContainerRunTime::startup()
{
	homePoa_ -> the_POAManager() -> activate();
	componentPoa_ -> the_POAManager() -> activate();
}

void
ContainerRunTime::initialize(/*int isMultiThread*/)
{
	TRACE0(LEVEL6,"Begin to initialize container runtime\n");
	ConfigFile_var configFile = new ConfigFile();

	/*
	uuidtype uid;
	uuid_create(&uid);
	
	ConfigFile_var configFile = new ConfigFile();
	
	//
	//transfer the UUID to the string type
	//
	
	char *str =( char *)(malloc(37));
	
	int j=sprintf(str,"%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", uid.time_low, uid.time_mid,
        	uid.time_hi_and_version, uid.clock_seq_hi_and_reserved,
        	uid.clock_seq_low);
	
	for (int i = 0; i < 6; i++)
	{
        	j+=sprintf(str+j,"%2.2x", uid.node[i]);	
        }
	
	uuid_ = CORBA::string_dup(str);
	free(str);
	*/

	uuid_ = ::Components::UuidGenerator::generateUuid();

	
	
	TRACE0(LEVEL6,"Create runtime working directory\n");
	

	CORBA::String_var workingDir = configFile->getRepositoryLocation();
#ifdef STARCCMSTRINGADD
	workingDir = HelpFun::CORBA_string_add(workingDir.in(), PATH_DELILIMITOR);
	
	workingDir = HelpFun::CORBA_string_add(workingDir.in(), uuid_.in());
#else
	workingDir += CORBA::string_dup(PATH_DELILIMITOR);
	
	workingDir += CORBA::string_dup(uuid_);
#endif

#ifdef WIN32	
	if( _mkdir(workingDir.in()) != 0)
#else
	if( mkdir(workingDir.in(), 0775) != 0 )
#endif
	{
		TRACE0(LEVEL1,"FATAL ERROR: create directory error\n");
	}

#ifdef _DEBUG
	CORBA::String_var debugFile;
	debugFile = workingDir;

#ifdef STARCCMSTRINGADD
	debugFile = HelpFun::CORBA_string_add(debugFile.in(), PATH_DELILIMITOR);
	debugFile = HelpFun::CORBA_string_add(debugFile.in(), "DebugConfig.ini");
#else
	debugFile += CORBA::string_dup(PATH_DELILIMITOR);
	debugFile += CORBA::string_dup("DebugConfig.ini");
#endif
	
	CORBA::String_var targetFile = CORBA::string_dup(".");
	targetFile += CORBA::string_dup(PATH_DELILIMITOR);
	targetFile += CORBA::string_dup("DebugConfig.ini");

#ifdef WIN32	
	::CopyFile(targetFile,debugFile.in(),TRUE);
#else
	HelpFun::copyFile(targetFile,debugFile.in());
#endif

#endif
	//modified by xiao heping 2004/04/04	
//	homePOAName_ = CORBA::string_dup(::Components::UuidGenerator::generateUuid());
//	componentPOAName_ = CORBA::string_dup(::Components::UuidGenerator::generateUuid());
	homePOAName_ =::Components::UuidGenerator::generateUuid();
	componentPOAName_ =::Components::UuidGenerator::generateUuid();
	//end modified xiao heping 2004/04/04

//modified by xiao heping 
#if defined(STARCCM_MTL)
	threadPool_=new CustomThreadPool(1000);

	//?????????????
	threadPool_->createThreads(8);
	//????????????????

	CustomDispatchThreadPool* dsp_cus=new CustomDispatchThreadPool(threadPool_);	
	homePoa_ = container_->createHomePOA(/*isMultiThread,*/ homePOAName_,dsp_cus);
	componentPoa_ = container_->createComponentPOA(/*isMultiThread, */componentPOAName_, homePoa_,dsp_cus);
	dsp_cus->_remove_ref();
#else
	homePoa_ = container_->createHomePOA(/*isMultiThread,*/ homePOAName_);
	componentPoa_ = container_->createComponentPOA(/*isMultiThread,*/ componentPOAName_, homePoa_);
#endif

	executorInvoker_ = new ::Container::ExecutorInvoker(this);
	homeExecutorInvoker_ = new ::Container::ExecutorInvoker(this);
	
	homePoa_->set_servant_manager(homeExecutorInvoker_);
	componentPoa_->set_servant_manager(executorInvoker_);
	//end modify
}


void
ContainerRunTime::setContainer(ContainerBase_ptr container)
{
	container_ = OBJDUPLICATE(ContainerBase_ptr,container);
	
}


ContainerBase_ptr
ContainerRunTime::getContainer()
{
	return OBJDUPLICATE(ContainerBase_ptr,container_);
}

//void
//ContainerRunTime::setCcmHome(::Components::CCMHome_ptr ccmHome)
//{
//	ccmHome_ = ::Components::CCMHome::_duplicate(ccmHome);
//}

::Components::CCMHome_ptr
ContainerRunTime::getCcmHome()
{
	//modified by xiao heping 2004/03/12
	if(CORBA::is_nil (ccmHome_) )
	{
		PortableServer::ObjectId_var oid = homeComposition_ -> getObjectId();
		CORBA::String_var repId = homeComposition_ -> getRepId();
		
		CORBA::Object_var homeObj = homePoa_ -> create_reference_with_id(oid.in(),repId.in());

		ccmHome_ = ::Components::CCMHome::_narrow(homeObj);
	}
	return ::Components::CCMHome::_duplicate(ccmHome_);
	//end modify
}


PortableServer::ObjectId*
ContainerRunTime::getHomeObjectId()
{
	PortableServer::ObjectId_var oid = homeComposition_->getObjectId();
	
	PortableServer::ObjectId* ret = new PortableServer::ObjectId(oid.in());
	
	return ret;
}

void
ContainerRunTime::setHomeComposition(HomeComposition_ptr homeComposition)
{
	homeComposition_ = OBJDUPLICATE(HomeComposition_ptr,homeComposition);
}

PortableServer::Servant
ContainerRunTime::preinvoke(const PortableServer::ObjectId& oid,
						PortableServer::POA_ptr adapter,
						const char* operation,
						PortableServer::ServantLocator::Cookie& the_cookie)
{
	TRACE0(LEVEL5,"ContainerRunTime preinvoke\n");
	if (destroy_)
		return 0;

	//add by wsf 2004.6.30
	CORBA::String_var str = PortableServer::ObjectId_to_string(oid);

	//to reject request if needed
	{
		STARCCM_SYNCHRONIZED(sync_,rejectReqMapMonitor)
		if (!RejectReqMap.empty())
		{
			MapRejectReq::iterator iter = RejectReqMap.find(string(str.in()));
			if (iter != RejectReqMap.end())
				throw CORBA::OBJECT_NOT_EXIST();
		}
	}
	
	//to redirecte request if needed
	{
		STARCCM_SYNCHRONIZED(sync_,redirectReqMapMonitor)
		if (!redirectReqMap.empty())
		{
			MapRedirectReq::iterator iter = redirectReqMap.find(string(str.in()));
			if (iter != redirectReqMap.end())
			{
				CORBA::Object_var obj = CORBA::Object::_duplicate((*iter).second);
				throw PortableServer::ForwardRequest(obj);
			}
		}
	}
	//end by wsf 2004.6.30
	
	//
	//get servant
	//
	PortableServer::ObjectId* objectId = new PortableServer::ObjectId(oid);
	PortableServer::Servant servant = getServant(objectId,the_cookie,operation);
	delete objectId;

	//added by xiao heping 2004/03/25
	if(servant != 0)
	{
		::Container::Cookie* cookie = static_cast<Container::Cookie*>(the_cookie);
		CompositionManager* compositionManager = cookie->getCompositionManager();
//		CompositionManager* compositionManager = static_cast<CompositionManager*>(the_cookie);
	#if (defined(StarBus) || defined(ORBacus)) && defined(STARCCM_MTL)
		if(!isMultiThread_)
		{
			if((strcmp(operation,"_is_a") != 0 ) && (strcmp(operation,"_non_existent") != 0 ) && (strcmp(operation,"_is_equivalent") != 0 ))
				compositionManager -> lock();
		}
	#endif
	}
	//end add
	return servant;
}

void
ContainerRunTime::postinvoke(const PortableServer::ObjectId& oid,
							PortableServer::POA_ptr adapter,
                            const char* operation,
                            PortableServer::ServantLocator::Cookie the_cookie,
                            PortableServer::Servant the_servant)
{
	//added by xiao heping 2004/03/04
	TRACE0(LEVEL5,"ContainerRunTime postinvoke\n");

	::Container::Cookie* cookie = static_cast<Container::Cookie*>(the_cookie);
	CompositionManager* compositionManager = cookie->getCompositionManager();
//	CompositionManager* compositionManager = static_cast<CompositionManager*>(the_cookie);
#if (defined(StarBus) || defined(ORBacus)) && defined(STARCCM_MTL)
	if(!isMultiThread_)
	{
		if((strcmp(operation,"_is_a") != 0 ) && (strcmp(operation,"_non_existent") != 0 ) && (strcmp(operation,"_is_equivalent") != 0 ))
			compositionManager -> unlock();
	}
#endif
	the_servant -> _remove_ref();
	try
	{
		compositionManager -> _remove_ref();

	}
	catch(...)
	{
		TRACE0(LEVEL1,"\n^^^^^^^^^^^^^^^^ContainerRunTime::postinvoke^^^^^^^^^^^^^^^^^^\n");
	}//end add

	//deleted by xiao heping 2004/03/18
//	if((strcmp(operation,"remove") == 0)||(strcmp(operation,"remove_component") == 0))
//	{
//		the_servant -> _remove_ref();
//	}
	//end delete xiao heping 2004/03/18
}

/*
ExecutorInvoker_ptr
ContainerRunTime::getExecutorInvoker()
{
	return executorInvoker_;
}

ExecutorInvoker_ptr
ContainerRunTime::getHomeExecutorInvoker()
{
	return homeExecutorInvoker_;
}
*/

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
ContainerRunTime::install_home(const char* id,const char* entrypt,
                               const Components::ConfigValues& config)
					    throw(Components::Deployment::UnknownImplId,
					          Components::Deployment::ImplEntryPointNotFound,
					          Components::Deployment::InstallationFailure,
							  Components::Deployment::InvalidConfiguration,
					          CORBA::SystemException)
{
	int len = config.length();
//	int isMultiThread;
	
	CORBA::String_var homeServantEntry;
	CORBA::String_var contextEntry;
	CORBA::String_var homeServantDll;
//	CORBA::String_var homeRepId;
//	CORBA::String_var componentRepId;

	//added by xiao heping 2004/03/28
//	ServantLoader_var servantsLoader;
//	ExecutorLoader_var executorsLoader;	
	//end add xiao heping 2004/03/28
	
	CORBA::String_var homeExecutorEntry;
	CORBA::String_var homeExecutorDll;
	CORBA::StringSeq_var valueFactoryEntries;
	CORBA::StringSeq_var valueFactoryRepIds;
	int hasValueFactory = 0;
	
	CORBA::String_var storageHomeEntry;
	CORBA::String_var storageHomeDll;
	
	int componentKind;
	int configurationCompleted = 0;
	
	CORBA::String_var componentTxnPolicy = CORBA::string_dup("");
	
	CORBA::StringSeq_var homeRepIds;
	CORBA::StringSeq_var homeTxnPolicies;
	CORBA::StringSeq_var homeMethods;
	int hasHomeMethodTxnPolicy = 0;
	
	CORBA::StringSeq_var componentRepIds;
	CORBA::StringSeq_var componentTxnPolicies;
	CORBA::StringSeq_var componentMethods;
	int hasComponentMethodTxnPolicy = 0;
	
	FileAccessorFactory_var fileaccessorfactory;
	
	// CORBA service initial references
	CORBA::String_var nameService = CORBA::string_dup("");
	CORBA::String_var homeFinder = CORBA::string_dup("");
	homeRegistration_ = CORBA::string_dup("");
	CORBA::String_var transactionService = CORBA::string_dup("");
	CORBA::String_var notificationService = CORBA::string_dup("");
	
	//persistent policies
	int isCmp = 0;
	int isTransactional = 0;
	int accessMode = 1;
	int isolationLevel = 3;
	CORBA::String_var storageHomeId = CORBA::string_dup("");
	CORBA::String_var pssFileName = CORBA::string_dup("");
	
	//resourcepool
	CORBA::StringSeq_var resources;
	int hasToCreateResources = 0;
	//jxh 06/22
	//CORBA::String_var resourceName;
	
	ConfigFile_var configFile = new ConfigFile();

	for(int i = 0;i<len;i++)
	{
		if( config[i] != NULL )
		{
			if( config[i]->name() != NULL )
			{
				if( strcmp(config[i]->name(),"ServantDllName") ==0 )
				{
					const char *msg;
					config[i]->value() >>= msg;
					homeServantDll = msg;
					TRACE1(LEVEL6,"Seravnt DLL name is %s\n",homeServantDll);
				}
				else if( strcmp(config[i]->name(),"FileAccessorFactory") ==0 )
				{
//					CORBA::Object_var factoryObj;
//					TRACE0(LEVEL6,"File accessor process begin.\n");
//					config[i]->value() >>= CORBA::Any::to_object(factoryObj);
//					TRACE0(LEVEL6,"File accessor got from the any.\n");
//					
//					fileaccessorfactory = STARCCM::Deployment::FileAccessorFactory::_narrow(factoryObj);			
//					
//					TRACE0(LEVEL6,"File Accessor Factory \n");

					STARCCM::Deployment::FileAccessorFactory_ptr factoryObj;
					TRACE0(LEVEL6,"File accessor process begin.\n");
					config[i]->value() >>= factoryObj;
					TRACE0(LEVEL6,"File accessor got from the any.\n");
					
					fileaccessorfactory = STARCCM::Deployment::FileAccessorFactory::_duplicate(factoryObj);			
					
//					CORBA::ORB_var orb = container_ -> getOrb();
//					CORBA::String_var s1 = orb -> object_to_string(fileaccessorfactory);
//                    
//                    
//				    const char* refFile1 = "C:\\ccm\\File.ref";
//				    ofstream out1(refFile1);
//				    if(out1.fail())
//				    {
//						TRACE0(LEVEL3,"Can NOT create reference files\n");
//				    }
//				
//				    out1 << s1 << endl;
//				    out1.close();
					
					TRACE0(LEVEL6,"File Accessor Factory \n");
				}
				else if( strcmp(config[i]->name(),"HomeServantEntryPoint") ==0 )
				{
					const char *msg;
					config[i]->value() >>= msg;
					homeServantEntry = msg;
					TRACE1(LEVEL6,"Home seravnt entry point is %s\n",homeServantEntry);
				}
				else if( strcmp(config[i]->name(),"ContextEntryPoint") ==0 )
				{
					const char *msg;
					config[i]->value() >>= msg;
					contextEntry = msg;
					TRACE1(LEVEL6,"Context entry point is %s\n",contextEntry);
				}
				else if( strcmp(config[i]->name(),"ExecutorDllName") ==0 )
				{
					const char *msg;
					config[i]->value() >>= msg;
					homeExecutorDll = msg;
					TRACE1(LEVEL6,"Home executor DLL name is %s\n",homeExecutorDll);
				}
				else if( strcmp(config[i]->name(),"HomeExecutorEntryPoint") ==0 )
				{
					const char *msg;
					config[i]->value() >>= msg;
					homeExecutorEntry = msg;
					TRACE1(LEVEL6,"Home executor entry point is %s\n",homeExecutorEntry);
				}
				else if( strcmp(config[i]->name(),"ValueFactoryEntryPoints") ==0 )
				{
					const CORBA::StringSeq* msg;
					config[i]->value() >>= msg;
					valueFactoryEntries = new CORBA::StringSeq(*msg);
					hasValueFactory = 1;
					for(int j = 0 ; j < valueFactoryEntries->length() ; j++)
					{
						TRACE1(LEVEL6,"ValueFactory entry name is %s\n",valueFactoryEntries[(CORBA::ULong)j].in());
					}
				}
				else if( strcmp(config[i]->name(),"ValueFactoryRepIds") ==0 )
				{
					const CORBA::StringSeq* msg;
					config[i]->value() >>= msg;
					valueFactoryRepIds =new CORBA::StringSeq(*msg);
					hasValueFactory = 1;
					for(int j = 0 ; j < valueFactoryRepIds->length() ; j++)
					{
						TRACE1(LEVEL6,"Value Factory repository id is %s\n",valueFactoryRepIds[(CORBA::ULong)j].in());
					}
				}
				else if( strcmp(config[i]->name(),"HomeRepositoryId") ==0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
//					homeRepId = msg;
					homeRepId_ = msg;
					
					TRACE1(LEVEL6,"Home Repository ID is %s\n",homeRepId_);
				}
				else if( strcmp(config[i]->name(),"ComponentRepositoryId") ==0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
//					componentRepId = msg;
					compRepId_ = msg;
					
					TRACE1(LEVEL6,"Component Repository ID is %s\n",compRepId_);
				}
				else if( strcmp(config[i]->name(),"ComponentKind") ==0 )
				{
					ContainerType type;
					CORBA::String_var kind;
					const char* msg;
					config[i]->value() >>= msg;
					kind = msg;
					 
					if( strcmp(kind,"service" ) ==0 )
					{
						type = SERVICECONTAINER;
						componentKind = 0;
						TRACE0(LEVEL6,"component kind : service\n");
					}
					else if( strcmp(kind,"session" ) ==0 )
					{
						type = SESSIONCONTAINER;
						componentKind = 1;
						TRACE0(LEVEL6,"component kind : session\n");
					}
					else if( strcmp(kind,"entity" ) ==0 )
					{
						type = ENTITYCONTAINER;
						componentKind = 2;
						TRACE0(LEVEL6,"component kind : entity\n");
					}
					else if( strcmp(kind,"process" ) ==0 )
					{
						type = PROCESSCONTAINER;
						componentKind = 3;
						TRACE0(LEVEL6,"component kind : process\n");
					}
					else
					{
						TRACE0(LEVEL1,"Wrong component kind\n");

						//added by xiao heping 2004/03/23
						throw new Components::Deployment::InvalidConfiguration;
						//end add xiao heping 2004/03/23
					}

					//added by xiao heping 2004/02/13
					if(!container_->isContainerTypeSetted())
						container_->setContainerType(type) ;
					else
					{
						if( type != container_->getContainerType() )
						{
							TRACE0(LEVEL1,"FATAL ERROR : Container type and cmponent kind NOT match\n");
							throw new Components::Deployment::InvalidConfiguration;
						}
					}
					//added by xiao heping 2004/02/13
				}
				else if( strcmp(config[i]->name(),"HomeTxnRepId") ==0 )
				{
					const CORBA::StringSeq* msg;
					hasHomeMethodTxnPolicy = 1;
					config[i]->value() >>= msg;
					homeRepIds = new CORBA::StringSeq(*msg);
					for(int j = 0 ; j < homeRepIds->length() ; j++)
					{
						TRACE1(LEVEL6,"Home repository id is %s\n",homeRepIds[(CORBA::ULong)j].in());
					}
				}
				else if( strcmp(config[i]->name(),"HomeMethodTxnNames") ==0 )
				{
					const CORBA::StringSeq* msg;
					hasHomeMethodTxnPolicy = 1;
					config[i]->value() >>= msg;
					homeMethods = new CORBA::StringSeq(*msg);
					for(int j = 0 ; j < homeMethods->length() ; j++)
					{
						TRACE1(LEVEL6,"Home Mothod name is %s\n",homeMethods[(CORBA::ULong)j].in());
					}
				}
				else if( strcmp(config[i]->name(),"HomeMethodTxnPolicies") ==0 )
				{
					const CORBA::StringSeq* msg;
					hasHomeMethodTxnPolicy = 1;
					config[i]->value() >>= msg;
					homeTxnPolicies = new CORBA::StringSeq(*msg);
					for(int j = 0 ; j < homeTxnPolicies->length() ; j++)
					{
						TRACE1(LEVEL6,"Home Mothod policy is %s\n",homeTxnPolicies[(CORBA::ULong)j].in());
					}
				}
				else if( strcmp(config[i]->name(),"ComponentTxnRepId") ==0 )
				{
					const CORBA::StringSeq* msg;
					hasComponentMethodTxnPolicy = 1;
					config[i]->value() >>= msg;
					componentRepIds = new CORBA::StringSeq(*msg);
					for(int j = 0 ; j < componentRepIds->length() ; j++)
					{
						TRACE1(LEVEL6,"Component repository id is %s\n",componentRepIds[(CORBA::ULong)j].in());
					}			
				}
				else if( strcmp(config[i]->name(),"ComponentMethodTxnNames") ==0 )
				{
					const CORBA::StringSeq* msg;
					hasComponentMethodTxnPolicy = 1;
					config[i]->value() >>= msg;
					componentMethods = new CORBA::StringSeq(*msg);
					for(int j = 0 ; j < componentMethods->length() ; j++)
					{
						TRACE1(LEVEL6,"Component Mothod name is %s\n",componentMethods[(CORBA::ULong)j].in());
					}			
				}
				else if( strcmp(config[i]->name(),"ComponentMethodTxnPolicies") ==0 )
				{
					const CORBA::StringSeq* msg;
					hasComponentMethodTxnPolicy = 1;
					config[i]->value() >>= msg;
					componentTxnPolicies = new CORBA::StringSeq(*msg);
					for(int j = 0 ; j < componentTxnPolicies->length() ; j++)
					{
						TRACE1(LEVEL6,"Component Mothod policy is %s\n",componentTxnPolicies[(CORBA::ULong)j].in());
					}
				}
				else if( strcmp(config[i]->name(),"Threading") ==0 )
				{
					const char* msg;
					
					config[i]->value() >>= msg;
					
					if( strcmp(msg,"multithread") == 0 )
					{
						isMultiThread_ = 1;
					}
					else
					{
						isMultiThread_ = 0;
					}
				}
				else if( strcmp(config[i]->name(),"ComponentTxnPolicy") ==0 )
				{
					const char* msg;
					hasComponentMethodTxnPolicy = 1;
					config[i]->value() >>= msg;
					componentTxnPolicy = msg;					
					
				}				
				else if( strcmp(config[i]->name(),"ConfigurationComplete") ==0 )
				{
				    const char* msg;
					
					config[i]->value() >>= msg;
					
					if( strcmp(msg,"true") == 0 )
					{
						configurationCompleted = 1;
					}
					else
					{
						configurationCompleted = 0;
					}
				}
				else if( strcmp(config[i]->name(),"NameService") == 0 )
				{
				    const char* msg;
					config[i]->value() >>= msg;
					nameService = msg;
					
					TRACE0(LEVEL6,"NameService resolved\n");
				}
				else if( strcmp(config[i]->name(),"HomeFinder") == 0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
					homeFinder = msg;
					
					TRACE0(LEVEL6,"HomeFinder resolved\n");
				}
				else if( strcmp(config[i]->name(),"TransactionService") == 0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
					transactionService = msg;
					
					TRACE0(LEVEL6,"TransactionService resolved\n");
				}
				else if( strcmp(config[i]->name(),"HomeRegistration") == 0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
					homeRegistration_ = msg;
					
					TRACE0(LEVEL6,"HomeRegistration resolved\n");
				}
				else if( strcmp(config[i]->name(),"NotificationService") == 0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
					notificationService = msg;
					
					TRACE0(LEVEL6,"NotificationService resolved\n");
				}
				else if( strcmp(config[i]->name(),"PSSFileName") == 0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
					pssFileName = msg;
				}
				else if( strcmp(config[i]->name(),"StorageHomeId") == 0 )
				{
					const char* obj;
					config[i]->value() >>= obj;
					storageHomeId = obj;
				}
				else if( strcmp(config[i]->name(),"PSSAccessMode") == 0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
					
					if( strcmp(msg, "ReadOnly") )
					{
					    accessMode = 0;
					}
					else
					{
					    accessMode = 1;
					}
				}
				else if( strcmp(config[i]->name(),"PSSCMP") == 0 )
				{
				    const char* msg;
					config[i]->value() >>= msg;
					
					if( strcmp(msg, "Container") )
					{
					    isCmp = 1;
					}
					else
					{
					    isCmp = 0;
					}
				}
				else if( strcmp(config[i]->name(),"PSSTxnPolicy") == 0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
					
					if( strcmp(msg, "Transactional") )
					{
					    isTransactional = 1;
					}
					else
					{
					    isTransactional = 0;
					}
				}
				else if( strcmp(config[i]->name(),"PSSTxnIsolationLevel") == 0 )
				{
					const char* msg;
					config[i]->value() >>= msg;
					
					if( strcmp(msg, "ReadUncommitted") )
					{
					    isolationLevel = 0;
					}
					else if( strcmp(msg, "ReadCommitted") )
					{
					    isolationLevel = 1;
					}
					else if( strcmp(msg, "RepeatableRead") )
					{
					    isolationLevel = 2;
					}
					else
					{
					    isolationLevel = 3;
					}
				}
				else if( strcmp(config[i]->name(),"CreateResourcePool") == 0 )
				{
				    const CORBA::StringSeq* msg;
				    config[i]->value() >>= msg;
				    resources = new CORBA::StringSeq(*msg);
				    
				    hasToCreateResources = 1;
				}
				else if( strcmp(config[i]->name(),"ResourcePoolName") == 0 )
				{
				    const char* msg;
				    config[i]->value() >>= msg;
				    resourceName = msg;
				}
				else
				{
					TRACE1(LEVEL1,"Unknow config value : %s\n",config[i]->name());
				}
			}
			else
			{
				TRACE0(LEVEL6,"NULL config value name\n");
			}
		}
		else
		{
			TRACE0(LEVEL6,"NULL config value\n");
		}
	}
	TRACE0(LEVEL6,"All of the config values have been saved\n");
	TRACE0(LEVEL6,"Out of the iteration\n");
	//End of saving
	
	initialize(/*isMultiThread*/);
	
	//Load dlls
	assert(!(homeServantDll.in() == NULL));
	assert(!(homeExecutorDll.in() == NULL));
	assert(!(homeExecutorEntry.in() == NULL));
	assert(!(homeServantEntry.in() == NULL));
	
	//Download the component implementation from the ComponentInstallation
	CORBA::String_var servantLocation;
	CORBA::String_var executorLocation;
	CORBA::String_var persistenterLocation;
	
	STARCCM::Deployment::DeployDomainMgr_var manager = container_ -> getDomainManager();
	
	try
	{
		CORBA::String_var objLoc;
		objLoc = manager -> getObject(STARCCM::Deployment::COMPONENTINSTALLATION,"");
		
		CORBA::ORB_var orb =  container_ -> getOrb();
		CORBA::Object_var obj = orb -> string_to_object(objLoc.in());
		
		locator_ = Components::Deployment::ComponentInstallation::_narrow(obj);

		servantLocation = locator_ -> get_implementation(id);
		executorLocation = locator_ -> get_implementation(id);
		persistenterLocation = locator_ -> get_implementation(id);
	}
	catch(Components::Deployment::UnknownImplId&)
	{
		TRACE1(LEVEL2,"Unknow component package UUID:%s\n",id);
		throw;
	}
	catch(Components::Deployment::InstallationFailure&)
	{
		TRACE1(LEVEL2,"Unknow component package UUID:%s\n",id);
		throw;
	}
	
#ifdef STARCCMSTRINGADD
	servantLocation = HelpFun::CORBA_string_add(servantLocation.in(), PATH_DELILIMITOR);
	servantLocation= HelpFun::CORBA_string_add(servantLocation.in(), homeServantDll.in());
	
	executorLocation = HelpFun::CORBA_string_add(executorLocation.in(), PATH_DELILIMITOR);
	executorLocation= HelpFun::CORBA_string_add(executorLocation.in(), homeExecutorDll.in());
	
	persistenterLocation = HelpFun::CORBA_string_add(persistenterLocation.in(), PATH_DELILIMITOR);
	persistenterLocation= HelpFun::CORBA_string_add(persistenterLocation.in(), pssFileName.in());
	
	CORBA::String_var servantPath = configFile->getRepositoryLocation();
	servantPath = HelpFun::CORBA_string_add(servantPath.in(), PATH_DELILIMITOR);
    CORBA::String_var executorPath = configFile->getRepositoryLocation();
    executorPath =  HelpFun::CORBA_string_add(executorPath.in(), PATH_DELILIMITOR);
    CORBA::String_var persistenterPath = configFile->getRepositoryLocation();
    persistenterPath =  HelpFun::CORBA_string_add(persistenterPath.in(), PATH_DELILIMITOR);
    
	servantPath = HelpFun::CORBA_string_add(servantPath.in(), uuid_.in());
	executorPath = HelpFun::CORBA_string_add(executorPath.in(), uuid_.in());
	persistenterPath = HelpFun::CORBA_string_add(persistenterPath.in(), uuid_.in());
	
	servantPath = HelpFun::CORBA_string_add(servantPath.in(), PATH_DELILIMITOR);
	executorPath = HelpFun::CORBA_string_add(executorPath.in(), PATH_DELILIMITOR);
	persistenterPath = HelpFun::CORBA_string_add(persistenterPath.in(), PATH_DELILIMITOR);
	
	servantPath = HelpFun::CORBA_string_add(servantPath.in(), homeServantDll.in());
	executorPath = HelpFun::CORBA_string_add(executorPath.in(), homeExecutorDll.in());
	persistenterPath = HelpFun::CORBA_string_add(executorPath.in(), pssFileName.in());
#else
	servantLocation += CORBA::string_dup(PATH_DELILIMITOR);
	servantLocation += CORBA::string_dup(homeServantDll);
	
	executorLocation += CORBA::string_dup(PATH_DELILIMITOR);
	executorLocation += CORBA::string_dup(homeExecutorDll);
	
	persistenterLocation += CORBA::string_dup(PATH_DELILIMITOR);
	persistenterLocation  += CORBA::string_dup(pssFileName);


	CORBA::String_var servantPath = configFile->getRepositoryLocation();
	servantPath += CORBA::string_dup(PATH_DELILIMITOR);
    CORBA::String_var executorPath = configFile->getRepositoryLocation();
    executorPath +=  CORBA::string_dup(PATH_DELILIMITOR);
    CORBA::String_var persistenterPath = configFile->getRepositoryLocation();
    persistenterPath +=  CORBA::string_dup(PATH_DELILIMITOR);
    
	servantPath += uuid_;
	executorPath += uuid_;
	persistenterPath += uuid_;
	
	servantPath += CORBA::string_dup(PATH_DELILIMITOR);
	executorPath += CORBA::string_dup(PATH_DELILIMITOR);
	persistenterPath += CORBA::string_dup(PATH_DELILIMITOR);
	
	servantPath += homeServantDll;
	executorPath += homeExecutorDll;
	persistenterPath += pssFileName;
#endif

	assert(!CORBA::is_nil(fileaccessorfactory));
    
    FileAccessor_var fileaccessor = fileaccessorfactory->createFileAccessor();
    assert( !CORBA::is_nil(fileaccessor) );
    
    FILE *fileWriteStream;
    
    CORBA::String_var workingDir = configFile->getRepositoryLocation();

#ifdef STARCCMSTRINGADD
    workingDir = HelpFun::CORBA_string_add(workingDir.in(), PATH_DELILIMITOR);
    workingDir = HelpFun::CORBA_string_add(workingDir.in(), uuid_.in());
#else
    workingDir += CORBA::string_dup(PATH_DELILIMITOR);
    workingDir += uuid_;
#endif

    //Download the servant dll
    try
    {
    	fileaccessor -> locate_file( servantLocation.in() );	

	TRACE1(LEVEL6,"donwload servant dll from directory of %s. \n",workingDir.in());
	
#ifdef WIN32
		if( _chdir(workingDir.in()))
	   	{
			TRACE0(LEVEL1,"Unable to locate the directory\n");
			throw Components::Deployment::InstallationFailure();
	   	}
#else
		if( chdir(workingDir.in()))
	        {
		        TRACE0(LEVEL1,"Unable to locate the directory\n");
                        throw Components::Deployment::InstallationFailure();
		}

#endif   	 	
    	
    	TRACE1(LEVEL6,"save servant in local file %s\n", servantPath.in());
		if( (fileWriteStream = fopen( servantPath.in(), "wb" )) == NULL )
		{
			 TRACE0(LEVEL1,"Unable to locate the file\n");
			 fclose(fileWriteStream);
			 throw Components::Deployment::InstallationFailure();
		}    	
		
	    int from_octet=0;
	    int max_octets=1024000;
	    int writeSize;
	    
		FileOctetSeq_var writeBuffer;
    	do
    	{
    		writeBuffer=fileaccessor -> get_octet_seq(from_octet,max_octets);
    		writeSize=writeBuffer -> length();
    		
    		if(fseek( fileWriteStream, from_octet , SEEK_SET)!=0)
    		{
    			TRACE0(LEVEL1,"File seek error\n");
    			throw Components::Deployment::InstallationFailure();
    		}
    		fwrite(writeBuffer -> get_buffer(), 1, writeSize, fileWriteStream);
    		from_octet += writeSize;
    	}
    	while(writeSize == max_octets);
    }
    catch(const FileSystemError &e)
    {
    	TRACE1(LEVEL3,"File system error:%s\n",e.desc);
    	fclose(fileWriteStream);
    	throw Components::Deployment::InstallationFailure();
    }
    catch(...)
    {
    	TRACE0(LEVEL1,"Other Non Corba Exception happened\n");
    	fclose(fileWriteStream);
    	throw Components::Deployment::InstallationFailure();
    };
    
    fclose(fileWriteStream);
    TRACE0(LEVEL6,"File transfer succeed\n");
    
    //Download the executor dll
    try
    {
    	fileaccessor -> locate_file(executorLocation.in());	
		
#ifdef WIN32
		if( _chdir(workingDir.in()))
	   	{
			TRACE0(LEVEL1,"Unable to locate the directory\n");
			throw Components::Deployment::InstallationFailure();
	   	}
#else
		if( chdir(workingDir.in()))
	   	{
			TRACE0(LEVEL1,"Unable to locate the directory\n");
			throw Components::Deployment::InstallationFailure();
	   	}
#endif   	 	
    	
		if( (fileWriteStream = fopen( homeExecutorDll.in(), "wb" )) == NULL )
		{
			  TRACE0(LEVEL1,"Unable to locate the file\n");
			  fclose(fileWriteStream);
			  throw Components::Deployment::InstallationFailure();
		}    	
		
	    int from_octet=0;
	    int max_octets=1024000;
	    int writeSize;
	    
		FileOctetSeq_var writeBuffer;
	
    	do
    	{
    		writeBuffer=fileaccessor -> get_octet_seq(from_octet,max_octets);
    		writeSize=writeBuffer -> length();
    		
    		if(fseek( fileWriteStream, from_octet , SEEK_SET)!=0)
    		{
    			TRACE0(LEVEL1,"File seek error\n");
    			fclose(fileWriteStream);
    			throw Components::Deployment::InstallationFailure();
    		}
    		fwrite(writeBuffer -> get_buffer(), 1, writeSize, fileWriteStream);
    		from_octet += writeSize;
    	}
    	while(writeSize == max_octets);
    }
    catch(const FileSystemError &e)
    {
    	TRACE1(LEVEL3,"File system error:%s\n",e.desc);
    	fclose(fileWriteStream);
    	throw Components::Deployment::InstallationFailure();
    }
    catch(...)
    {
    	TRACE0(LEVEL1,"Other Non Corba Exception happened\n");
    	fclose(fileWriteStream);
    	throw Components::Deployment::InstallationFailure();
    };
    fclose(fileWriteStream);
    TRACE0(LEVEL6,"File transfer succeed\n");
    
	servantsLoader_ = new ServantLoader(servantPath, this);
	executorsLoader_ = new ExecutorLoader(executorPath, this);
	
	//
	//construct HomeComposition object
	//
	HomeComposition_var homeComposition = new HomeComposition();
	Components::HomeExecutorBase_ptr homeExecutor = executorsLoader_->loadHomeExecutor(homeExecutorEntry);
	if(homeExecutor == NULL)
	{
		TRACE0(LEVEL1,"Load Home Executor from dll failed\n");
		throw Components::Deployment::InstallationFailure();
	}
	homeComposition->registerHomeExecutor(homeExecutor);
	setHomeComposition(homeComposition);
	Components::UuidGenerator_var gen = new Components::UuidGenerator();
	homeCompositionUUID_ = gen -> generateUuid();
	PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(homeCompositionUUID_.in());
	PortableServer::Servant homeServant = servantsLoader_->loadHomeServant(homeServantEntry);	
	if(homeServant == NULL)
	{
		TRACE0(LEVEL1,"Load Home servant from dll failed\n");
		throw Components::Deployment::InstallationFailure();
	}
	homeComposition->registerHomeServant(oid,homeServant);
	homeComposition->setRepId(homeRepId_);	

	//load if there is any value factory
	if( hasValueFactory && ( valueFactoryEntries->length() != 0 ) )
	{
		if( valueFactoryEntries->length() != valueFactoryRepIds->length() )
		{
			throw new Components::Deployment::InvalidConfiguration();
		}
		for(int k = 0 ; k < valueFactoryEntries->length() ; k++)
		{
			//modified by xiao heping 2004/03/23
			//in order to release memory space
			CORBA::ValueFactoryBase_var valuefactory = 	executorsLoader_
				-> loadValueFactory(valueFactoryEntries[(CORBA::ULong)k],valueFactoryRepIds[(CORBA::ULong)k]);
		}
	}

#ifdef WITH_OTS

	//set home transaction policy if there is any
	if( hasHomeMethodTxnPolicy && ( homeTxnPolicies->length() != 0 ) )
	{
		if( homeTxnPolicies->length() != homeMethods->length() )
		{
			throw new Components::Deployment::InvalidConfiguration();
		}
		
		for(int k = 0 ; k < homeTxnPolicies->length() ; k++)
		{
			homeExecutorInvoker_-> setTxnPolicy((char*)homeRepIds[(CORBA::ULong)k].in(),
					        (char*)homeMethods[(CORBA::ULong)k].in(),
					        (char*)homeTxnPolicies[(CORBA::ULong)k].in());
		}
	}

	//set component transaction policy if there is any
	if( hasComponentMethodTxnPolicy && ( componentTxnPolicies->length() != 0 ) )
	{
		if( componentTxnPolicies->length() != componentMethods->length() )
		{
			throw new Components::Deployment::InvalidConfiguration();
		}
		
		//component policy is set to all operations.
		if (!strcmp(componentTxnPolicy,"*"))
		executorInvoker_-> setTxnPolicy((char*)compRepId_.in(),
				                (char*)"*",
				                (char*)componentTxnPolicy.in());
		
		//component policy is "self-managed",and the length of componentTxnPolicies must be 1.
		if (!strcmp(componentTxnPolicy,"self-managed"))
		{
//			if(componentTxnPolicies->length() == 0)
				executorInvoker_-> setTxnPolicy((char*)compRepId_.in(),
					                			(char*)"self-managed",
					                			(char*)componentTxnPolicy.in());
//			else
//			{
//				throw new Components::Deployment::InvalidConfiguration();
//			}
		}			
					                		
		for(int k = 0 ; k < componentTxnPolicies->length() ; k++)
		{
			executorInvoker_-> setTxnPolicy((char*)componentRepIds[(CORBA::ULong)k].in(),
				                (char*)componentMethods[(CORBA::ULong)k].in(),
				                (char*)componentTxnPolicies[(CORBA::ULong)k].in());
		}
	}

#endif

    TRACE0(LEVEL6, "preparing for loading PSS\n");
    //Download the PSS dll
//    if( strcmp(pssFileName.in(), "") != 0 )
//    {
//        try
//        {
//        	fileaccessor -> locate_file(persistenterLocation.in());	
//    		
//#ifdef WIN32
//    		if( _chdir(workingDir.in()))
//    	   	{
//    			TRACE0(LEVEL1,"Unable to locate the directory\n");
//    			throw Components::Deployment::InstallationFailure();
//    	   	}
//#else
//#endif   	 	
//        	
//    		if( (fileWriteStream = fopen( persistenterPath.in(), "wb" )) == NULL )
//    		{
//    			  TRACE0(LEVEL1,"Unable to locate the file\n");
//    			  fclose(fileWriteStream);
//    			  throw Components::Deployment::InstallationFailure();
//    		}    	
//    		
//    		
//    	    int from_octet=0;
//    	    int max_octets=1024000;
//    	    int writeSize;
//    	    
//    		FileOctetSeq_var writeBuffer;
//    	
//        	do
//        	{
//        		
//        		writeBuffer=fileaccessor -> get_octet_seq(from_octet,max_octets);
//        		writeSize=writeBuffer -> length();
//        		
//        		if(fseek( fileWriteStream, from_octet , SEEK_SET)!=0)
//        		{
//        			TRACE0(LEVEL1,"File seek error\n");
//        			fclose(fileWriteStream);
//        			throw Components::Deployment::InstallationFailure();
//        		}
//        		
//        		fwrite(writeBuffer -> get_buffer(), 1, writeSize, fileWriteStream);
//        		
//        		from_octet += writeSize;
//        		
//        	}
//        	while(writeSize == max_octets);
//        }
//        catch(const FileSystemError &e)
//        {
//        	TRACE1(LEVEL3,"File system error:%s\n",e.desc);
//        	fclose(fileWriteStream);
//        	throw Components::Deployment::InstallationFailure();
//        }
//        catch(...)
//        {
//        	TRACE0(LEVEL1,"Other Non Corba Exception happened\n");
//        	fclose(fileWriteStream);
//        	throw Components::Deployment::InstallationFailure();
//        };
//    
//        fclose(fileWriteStream);
//        TRACE0(LEVEL6,"File transfer succeed\n");
//        
//        //load persistenter
//        storageLoader_ = new StorageObjectLoader(pssFileName, this);
//        storageLoader_->loadPSS();
//    }
    
	Components::CCMHome_var home;

	TRACE0(LEVEL6,"installation complete\n");
	
	//
	//activate POAManager
	//
	startup();
	
	TRACE0(LEVEL6,"Container Runtime is ready\n");
	
	//
	//get CCMHome object reference
	//
	home = getCcmHome();
	
	//
    //save installed componet configuration into configfile
	//
	configFile->addInstalledComponent(container_->getUUID(), id);
	configFile->addExecutorFile(id, homeExecutorEntry, homeRepId_, compRepId_, homeExecutorDll);
	configFile->addServantFile(id, homeServantEntry, contextEntry, homeServantDll);
	TRACE0(LEVEL6, "store value type configs to config file\n");
	
	if( hasValueFactory && ( valueFactoryEntries->length() != 0 ) )
	{
		if( valueFactoryEntries->length() != valueFactoryRepIds->length() )
		{
			throw new Components::Deployment::InvalidConfiguration();
		}
		
		for(int k = 0 ; k < valueFactoryEntries->length() ; k++)
		{
		    configFile->addValueTypeFile(id, "null", valueFactoryEntries[(CORBA::ULong)k], valueFactoryRepIds[(CORBA::ULong)k], homeExecutorDll);
		}
	}
	
	TRACE0(LEVEL6, "store home transaction policies configs to config file\n");
	
	if( hasHomeMethodTxnPolicy && ( homeMethods->length() != 0 ) )
	{
	    if( homeMethods->length() != homeTxnPolicies->length() )
		{
			throw new Components::Deployment::InvalidConfiguration();
		}
		
		TRACE0(LEVEL6,"storing ...\n");
		for(int k = 0; k < homeMethods->length(); k++)
		{
		    TRACE1(LEVEL6, "iterator %d\n", k);
		    TRACE1(LEVEL6, "home mothod name : %s\n", homeMethods[(CORBA::ULong)k].in());
		    TRACE1(LEVEL6, "home repository id : %s\n", homeRepIds[(CORBA::ULong)k].in());
		    TRACE1(LEVEL6, "home mothod policy : %s\n", homeTxnPolicies[(CORBA::ULong)k].in());
		    configFile->addHomeTransactionPolicy(id,homeMethods[(CORBA::ULong)k].in(), homeRepIds[(CORBA::ULong)k].in(), homeTxnPolicies[(CORBA::ULong)k].in());
		}
	}
	
	TRACE0(LEVEL6, "store component transaction policies configs to config file\n");
	
	if( hasComponentMethodTxnPolicy && ( componentMethods->length() != 0 ) )
	{
	    if( componentMethods->length() != componentTxnPolicies->length() )
		{
			throw new Components::Deployment::InvalidConfiguration();
		}
		
		TRACE0(LEVEL6,"storing ...\n");
		for(int k = 0; k < componentMethods->length(); k++)
		{
		    TRACE1(LEVEL6, "iterator %d\n", k);
		    TRACE1(LEVEL6, "component mothod name : %s\n", componentMethods[(CORBA::ULong)k].in());
		    TRACE1(LEVEL6, "component repository id : %s\n", componentRepIds[(CORBA::ULong)k].in());
		    TRACE1(LEVEL6, "component mothod policy : %s\n", componentTxnPolicies[(CORBA::ULong)k].in());
		    configFile->addCompTransactionPolicy(id,componentMethods[(CORBA::ULong)k].in(), componentRepIds[(CORBA::ULong)k].in(), componentTxnPolicies[(CORBA::ULong)k].in());
		}
	}
    
	configFile->addComponentKind(id, componentKind);
	configFile->addComponentThreading(id, isMultiThread_);
	configFile->addConfigurationComplete(id, configurationCompleted);
	
	configFile->setPersistentPolicy(id, isCmp);
	configFile->setPersistentTxnPolicy(id, isTransactional);
	configFile->setAccessMode(id, accessMode);
	configFile->setIsolationLevel(id, isolationLevel);
	configFile->setStorageHomeId(id, storageHomeId.in());
	configFile->setPersistentFile(id, pssFileName.in());
	
	CORBA::String_var serverId = container_->getComponentServerUuid();
	
	CORBA::StringSeq_var myResources;
	
	CosPersistentState::ParameterList_var parameters;
	if( hasToCreateResources )
	{
		configFile->addReourcePool(serverId.in(), resources);
    
	    myResources = configFile->getResourcePoolProperties(serverId.in(), resourceName);

#ifndef LITE_VERSION	    
	    parameters = new CosPersistentState::ParameterList();
	    parameters->length(4);
	    
	    parameters[0].name = CORBA::string_dup("host");
	    parameters[0].val <<= CORBA::string_dup(myResources[0]);
	    parameters[1].name = CORBA::string_dup("database");
	    parameters[1].val <<= CORBA::string_dup(myResources[1]);
	    
	    parameters[2].name = CORBA::string_dup("user");
	    parameters[2].val <<= CORBA::string_dup(myResources[2]);
	    parameters[3].name = CORBA::string_dup("password");
	    parameters[3].val <<= CORBA::string_dup(myResources[3]);
	    
		pssInitializer_ = new PSSInitializer("PSS:connector:database:postgresql:1.0", (short)accessMode, parameters);
#endif		
	}
	
#ifndef LITE_VERSION	
	if( strcmp(pssFileName.in(), "") != 0 )
	{
        storageLoader_ = new StorageObjectLoader(persistenterPath, this);
        storageLoader_->loadPSS();
    }
#endif	
	
	//
	//set HomeExecutor context
	//
	TRACE0(LEVEL6, "setting context for home executor\n");
	CCM2Context_impl* ctxPtr = new CCM2Context_impl();
	ctxPtr -> setContainerRunTime(this);
	
	CORBA::String_var initialconnections;
	CORBA::String_var maxconnections;
	CORBA::String_var increment;

	
#ifdef WITH_OTS
	if( hasToCreateResources )
	{
		initialconnections = CORBA::string_dup(myResources[4]);
		maxconnections = CORBA::string_dup(myResources[5]);
		increment = CORBA::string_dup(myResources[6]);
		
		CORBA::ULong _initialCon;
		CORBA::ULong _maxCon;
		CORBA::ULong _inc;
		
		_initialCon = atoi(initialconnections.in());
		_maxCon = atoi(maxconnections.in());
		_inc = atoi(increment.in());
		
		resPool_ = ::STARCCM::ResourcePool_impl::getResPoolInstance(this,_initialCon,_maxCon,_inc);
			
	    ctxPtr -> setResourcePool(resPool_);  
	}
#endif

	Components::CCM2Context_var ctx = ctxPtr;
	homeExecutor->set_context( ctx );
	
	TRACE0(LEVEL6, "end of setting context for home executor\n");
	
	//register initial reference
	CORBA::ORB_var orb = container_->getOrb();
	CORBA::Object_var initial;
	
	if( strcmp(nameService, "") != 0 )
	{
    	TRACE0(LEVEL6, "register NameService\n");
    	
    	configFile->addCosRef(serverId.in(), "NameService", nameService);
    	
    	try
    	{
    	    initial = orb->string_to_object(nameService);
    	    
    	    if( (CORBA::is_nil(initial)) )
    	    {
    	        orb->register_initial_reference("NameService", initial);
    	    }
    	    
    	}
    	catch(CORBA::ORB::InvalidName&)
    	{
    	    TRACE0(LEVEL1, "Invalid service name : NameService\n");
    	}
    	catch(CORBA::BAD_PARAM&)
    	{
    	    TRACE1(LEVEL1, "Bad NameService object reference : %s\n", nameService);
    	}
    }
    	
    if( strcmp(homeFinder, "") != 0 )
    {
        TRACE0(LEVEL6, "register HomeFinder\n");
        
    	configFile->addCosRef(serverId.in(), "HomeFinder", homeFinder);
    	
    	try
    	{
    	    initial = orb->string_to_object(homeFinder);
    	    
    	    if( (CORBA::is_nil(initial)) )
    	    {
    	        orb->register_initial_reference("HomeFinder", initial);
    	    }
    	    
    	}
    	catch(CORBA::ORB::InvalidName&)
    	{
    	    TRACE0(LEVEL1, "Invalid service name : HomeFinder\n");
    	}
    	catch(CORBA::BAD_PARAM&)
    	{
    	    TRACE1(LEVEL1, "Bad HomeFinder object reference : %s\n", homeFinder);
    	}
    }
    
    if( strcmp(homeRegistration_, "") != 0 )
    {
        TRACE0(LEVEL6, "register HomeRegistration\n");
        
    	configFile->addCosRef(serverId.in(), "HomeRegistration", homeRegistration_);
    	
    	try
    	{
    	    initial = orb->string_to_object(homeRegistration_);
    	    
    	    if( (CORBA::is_nil(initial)) )
    	    {
    	        orb->register_initial_reference("HomeRegistration", initial);
    	    }
    	    
    	}
    	catch(CORBA::ORB::InvalidName&)
    	{
    	    TRACE0(LEVEL1, "Invalid service name : HomeRegistration\n");
    	}
    	catch(CORBA::BAD_PARAM&)
    	{
    	    TRACE1(LEVEL1, "Bad HomeRegistration object reference : %s\n", homeRegistration_);
    	}
    }
    
    if( strcmp(transactionService, "") != 0 )
    {
        TRACE0(LEVEL6, "register TransactionService\n");
        
    	configFile->addCosRef(serverId.in(), "TransactionService", transactionService);
    	
    	try
    	{
    	    initial = orb->string_to_object(transactionService);
    	    
    	    if( (CORBA::is_nil(initial)) )
    	    {
    	        orb->register_initial_reference("TransactionService", initial);
    	    }
    	    
    	}
    	catch(CORBA::ORB::InvalidName&)
    	{
    	    TRACE0(LEVEL1, "Invalid service name : TransactionService\n");
    	}
    	catch(CORBA::BAD_PARAM&)
    	{
    	    TRACE1(LEVEL1, "Bad TransactionService object reference : %s\n", transactionService);
    	}
    }
    
    if( strcmp(notificationService, "") != 0 )
    {
        TRACE0(LEVEL6, "register NotificationService\n");
        
    	configFile->addCosRef(serverId.in(), "NotificationService", notificationService);
    	
    	try
    	{
    	    initial = orb->string_to_object(notificationService);
    	    
    	    if( (CORBA::is_nil(initial)) )
    	    {
    	        orb->register_initial_reference("NotificationService", initial);
    	    }
    	    
    	}
    	catch(CORBA::ORB::InvalidName&)
    	{
    	    TRACE0(LEVEL1, "Invalid service name : NotificationService\n");
    	}
    	catch(CORBA::BAD_PARAM&)
    	{
    	    TRACE1(LEVEL1, "Bad NotificationService object reference : %s\n", notificationService);
    	}
    }

	//add by wsf
	//to recorde the according archhome info
	STARCCM::ArchHome ahome;
	ahome.id = homeCompositionUUID_;
	ahome.uuid = homeCompositionUUID_;
	switch (componentKind)
	{
		case 0 : ahome.type = CORBA::string_dup("service"); 
				break;
		case 1 : ahome.type = CORBA::string_dup("session");
				break;
		case 2 : ahome.type = CORBA::string_dup("entity");
				break;
		case 3 : ahome.type = CORBA::string_dup("process");
				break;
	}
	ahome.homeref = CORBA::Object::_duplicate(home);
	ahome.homerepid = homeRepId_;
	ahome.componentrepid = compRepId_;
	/*
	ahome.serveractivatorid = NULL;
	ahome.serveractivatorref = NULL;
	ahome.componentserverid = NULL;
	ahome.componentserverref = NULL;
	*/
	ahome.containerid = container_->getUUID();
	ahome.containerref = CORBA::Object::_duplicate(container_->getReference());
	ahome.coms.length(0);
	ahome.links.length(0);

	container_ -> setArchHome(ahome);
	//end by wsf

	return home._retn();
}

ContainerType
ContainerRunTime::getContainerType()
{
    return container_ -> getContainerType();
}

#ifndef LITE_VERSION	
PSSInitializer_ptr
ContainerRunTime::getPssInitializer()
{
    return OBJDUPLICATE(PSSInitializer_ptr, pssInitializer_);  
}
#endif
  
//void 
//ContainerRunTime::setResourcePool(::STARCCM::ResourcePool_ptr res)
//{
//#ifndef LITE_VERSION	
//	resPool_ = STARCCM::ResourcePool::_duplicate(res);
//#endif
//}

//added by xiao heping 2004/03/16
::GreenThread::CustomThreadPool_ptr 
ContainerRunTime::GetCustomThreadPool()
{
#if defined(STARCCM_MTL)
	return ::GreenThread::CustomThreadPool::_duplicate (threadPool_);
#else
	return 0;
#endif
}
//end added xiao heping 2004/03/16

//to get the archcomponent infomation for the component just been created.
//wsf
void
ContainerRunTime::setArchComp(ComponentComposition_ptr comp)
{
	//To add the according archcomponent info
	STARCCM::ArchComponent acom;
	PortableServer::ObjectId_var oid = comp->getObjectId();
	acom.id = PortableServer::ObjectId_to_string(oid.in());
	acom.uuid = PortableServer::ObjectId_to_string(oid.in());
	acom.comref = CORBA::Object::_duplicate(comp->getComponentRef());
	PortableServer::ObjectId_var hoid = getHomeObjectId();
	acom.homeuuid = PortableServer::ObjectId_to_string(hoid.in());

	acom.ports.length(0);
	//get providesports info
	int plen;
	CORBA::Object_var obj= comp->getComponentRef();
	Components::CCMObject_var ccmobj = Components::CCMObject::_narrow(obj);
	Components::FacetDescriptions_var facets = ccmobj->get_all_facets();
	int flen = facets->length();
	STARCCM::Port port;
	port.type = CORBA::string_dup("providesport");
	int i = 0;
	for (i=0; i<flen; i++)
	{
		port.name = facets[i]->name();
		port.reference = CORBA::Object::_duplicate(facets[i]->facet_ref());
		plen = acom.ports.length();
		acom.ports.length(plen+1);
		acom.ports[plen] = port;
	}
	//get consumesport info
	Components::ConsumerDescriptions_var consumers = ccmobj->get_all_consumers();
	int clen = consumers->length();
	port.type = CORBA::string_dup("consumesport");
	for (i=0; i<clen; i++)
	{
		port.name = consumers[i]->name();
		port.reference = CORBA::Object::_duplicate(consumers[i]->consumer());
		plen = acom.ports.length();
		acom.ports.length(plen+1);
		acom.ports[plen] = port;
	}
	
	acom.links.length(0);

	addArchComponent(acom);

}

//wsf
void 
ContainerRunTime::onConnect(STARCCM::ArchConnection& acon)
{
	addArchConnection(acon);
}

//wsf
void 
ContainerRunTime::onDisconnect(PortableServer::ObjectId* srcoid, 
								const char* featurename, 
								const char* ck)
{
	deleteArchConnection(srcoid, featurename, ck);
}

//wsf
void 
ContainerRunTime::addArchComponent(STARCCM::ArchComponent& archcom)
{
	int len = aComponents.length();
	aComponents.length(len+1);
	aComponents[len] = archcom;

	container_ -> setArchCom(archcom);
}

//wsf
void 
ContainerRunTime::deleteArchComponent(const PortableServer::ObjectId* oid)
{
	CORBA::String_var s = PortableServer::ObjectId_to_string(*oid);
	int len = aComponents.length();
	int i = 0;
	for (i=0; i<len; i++)
		if (strcmp(s.in(),aComponents[i].uuid) ==0 )
			break;

	if (i == len)
	{
		//add the component uuid to deletedcomslist
		int dlen = deletedComslist.length();
		deletedComslist.length(dlen+1);
		deletedComslist[dlen] = s;
	}
	else
	{
		//To delete the component in aComponents
		for (int j=i; j<len-1; j++)
			aComponents[j] = aComponents[j+1];

		aComponents.length(len-1);
	}

}

//wsf
void 
ContainerRunTime::addArchConnection(STARCCM::ArchConnection& connect)
{
	int len = aConnections.length();
	aConnections.length(len+1);
	aConnections[len] = connect;

	container_ -> setArchConn(connect);
}

//wsf
void 
ContainerRunTime::deleteArchConnection(PortableServer::ObjectId* srcoid, const char* featurename, const char* cookie)
{
	CORBA::String_var s = PortableServer::ObjectId_to_string(*srcoid);
	int len = aConnections.length();
	int i = 0;
	for (i=0; i<len; i++)
	{
		if (cookie == NULL)
		{
			if ((strcmp(s.in(),aConnections[i].srccomuuid) ==0)&&(strcmp(featurename, aConnections[i].srcport) == 0))
				break;
		}
		else
		{
			if (strcmp(cookie,aConnections[i].cookie) ==0)
				break;
		}	
	}

	if (i == len)
	{
		//add connection info to deletedConnslist
		STARCCM::ArchConnection dcon;
		dcon.srccomuuid = s;
		dcon.srcport = CORBA::string_dup(featurename);
		dcon.cookie = CORBA::string_dup(cookie);
		int dlen = deletedConnslist.length();
		deletedConnslist.length(dlen+1);
		deletedConnslist[dlen] = dcon;
	}
	else
	{
		//To delete the connection in aConnections
		for (int j=i; j<len-1; j++)
			aConnections[j] = aConnections[j+1];

		aConnections.length(len-1);
	}

}

//wsf
void 
ContainerRunTime::updateArchitecture(STARCCM::ArchComponents_out addedcoms, 
						STARCCM::DeletedComs_out dcomslist, 
						STARCCM::ArchConnections_out addedconns, 
						STARCCM::ArchConnections_out dconnslist)
    throw(CORBA::SystemException)
{
	addedcoms = new STARCCM::ArchComponents(aComponents);
	dcomslist = new STARCCM::DeletedComs(deletedComslist);
	addedconns = new STARCCM::ArchConnections(aConnections);
	dconnslist = new STARCCM::ArchConnections(deletedConnslist);

	cleanArchObjects();
}

//wsf
void 
ContainerRunTime::cleanArchObjects()
    	throw(CORBA::SystemException)
{
	aComponents.length(0);
	deletedComslist.length(0);
	aConnections.length(0);
	deletedConnslist.length(0);
}

::STARCCM::ResourcePool_ptr 
ContainerRunTime::getResourcePool()
{
	
#ifndef LITE_VERSION
	return ::STARCCM::ResourcePool::_duplicate(resPool_);
#else
	return NULL;
#endif

}
//add by jxh 06/22
char* ContainerRunTime::getResourceName()
{
	return CORBA::string_dup(resourceName.in());
}

//add by wsf 2004.6.30
void 
ContainerRunTime::setRedirectReqInfo(const CORBA::StringSeq& oidList, 
					const Components::ObjectSeq& newObjList,
					CORBA::Long timeout)
	throw(CORBA::SystemException)
{
	STARCCM_SYNCHRONIZED(sync_,redirectReqMapMonitor)

	int len = oidList.length();
	for (int i=0; i<len; i++)
	{
		CORBA::Object_var obj = CORBA::Object::_duplicate(newObjList[i]);
		redirectReqMap.insert(MapRedirectReq::value_type(string(oidList[i]),obj));
	}

	STARCCM_THREAD_HANDLE(thread)
	thread = new InfoDeleter(this,oidList,timeout);
    	thread->start();
}

void 
ContainerRunTime::delRedirectReqInfo(const CORBA::StringSeq& oidList)
{
	STARCCM_SYNCHRONIZED(sync_,redirectReqMapMonitor)

	int len = oidList.length();
	MapRedirectReq::iterator iter;
	for (int i=0; i<len; i++)
	{
		iter = redirectReqMap.find(string(oidList[i]));
		if (iter != redirectReqMap.end())
		{
			redirectReqMap.erase(iter);
		}
	}
}

void 
ContainerRunTime::setRejectReqInfo(const CORBA::StringSeq& oidList)
{
	STARCCM_SYNCHRONIZED(sync_,rejectReqMapMonitor)
	int len = oidList.length();
	for (int i=0; i<len; i++)
	{
		RejectReqMap.insert(MapRejectReq::value_type(string(oidList[i]),string(oidList[i])));
	}
}

void 
ContainerRunTime::removeRejectReqInfo(const CORBA::StringSeq& oidList)
{
	STARCCM_SYNCHRONIZED(sync_,rejectReqMapMonitor)
	int len = oidList.length();
	MapRejectReq::iterator iter;
	for (int i=0; i<len; i++)
	{
		iter = RejectReqMap.find(string(oidList[i]));
		if (iter != RejectReqMap.end())
			RejectReqMap.erase(iter);
	}
}

void 
ContainerRunTime::passivateClient(const CORBA::StringSeq& oidList, 
				const Components::Deployment::PassivateClientTypeSeq& typeList)
	throw(CORBA::SystemException)
{
	CORBA::StringSeq_var rejectList;
	CORBA::StringSeq_var holdList;
	int len = typeList.length();
	int r=0;
	int h=0;
	for (int i=0; i<len; i++)
	{
		switch (typeList[i])
		{
		case Components::Deployment::REJECTREQUEST:
			rejectList[r++] = oidList[i];
			break;
		case Components::Deployment::HOLDREQUEST:
			holdList[h++] = oidList[i];
			break;
		}
	}

	setRejectReqInfo(rejectList);
//	threadPool_->setHoldClientReqInfo(holdList);
	
//	noRespReq(oidList);
}
	
void 
ContainerRunTime::activateClient(const CORBA::StringSeq& oidList)
	throw(CORBA::SystemException)
{
//	threadPool_->removeHoldClientReqInfo(oidList);
	removeRejectReqInfo(oidList);
}
//end by wsf 2004.6.30