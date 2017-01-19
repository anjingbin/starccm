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

#include <CORBA.h>

#include <ComponentServer_impl.h>

#include <ConfigFile.h>
#include <Deployment.h>
//#include <Container_impl.h>
//#include <Containerbase.h>
//#include <iostream>
//#include <stdlib.h>
//#include <malloc.h>
#ifdef WIN32
#include <Time.h>
//#include <ComponentManager_impl.h>
//jxh
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#include <CommonFunc.h>

using namespace std;
using namespace Components;
//using namespace Components::Deployment;
using namespace STARCCM::Deployment;

//jxh--modify 0315
ComponentServer_impl::ComponentServer_impl(
										   CORBA::ORB_ptr orb,
										   ComponentManager_impl* componentmanager,
										   PortableServer::POA_ptr poa,
										   DeployDomainMgr_ptr manger,
										   const char * fatherId,
										   const char * id,
										   bool verbose)
	: orb_(CORBA::ORB::_duplicate(orb)),
	componentManager_(componentmanager),
	poa_(PortableServer::POA::_duplicate(poa)),
	DomainMgr_(DeployDomainMgr::_duplicate(manger)),
	serverActivatorUUID_(CORBA::string_dup(fatherId)),
	componentServerUUID_(CORBA::string_dup(id)),
	verbose_(verbose)
{
	if (verbose_) 
	{
		cout << "ComponentServer : ComponentServer for "
		     <<componentServerUUID_
		     <<" is running." 
		     << endl;
	};
}
 
ComponentServer_impl::~ComponentServer_impl ()
{
  	if (verbose_) 
  	{
    	cout << "ComponentServer: terminating." << endl;
  	}
}

//
//Accessing the configuration attribute returns the sequence of ConfigValue objects
//passed in to the create_component_server operation at the ComponentServer creation.
//
Components::ConfigValues* 
ComponentServer_impl::configuration()
	throw(CORBA::SystemException)
{
	if(verbose_)
	{
		cout<<"ComponentServer: get the ComponentServer's configvalues  "<<endl;
	}	
	
	//
	//get the confivalues of this ComponentServer
	//	
	ConfigValues_var cfValues;
	try
	{
		cfValues=DomainMgr_->getComponentServerConf(componentServerUUID_.in());
	}
	catch(...)
	{
		TRACE1(LEVEL1,"ComponentServer: get the ConfigValues of the ComponentServer %s failed.",componentServerUUID_);
		throw CORBA::UNKNOWN();
	}
	return cfValues._retn();
}

//
//The get_server_activator operation returns a reference to the ServerActivator
//object that has created this ComponentServer.
//
ServerActivator_ptr
ComponentServer_impl::get_server_activator()
        throw(CORBA::SystemException)
{
	if(verbose_)
	{
		cout<<"ComponentServer: get the ServerActivator who created container  "<<endl;
	}
	
	//
	//get the ref from the corbaloc of the ServerActivator
	//
	CORBA::Object_var obj;		
	
	CORBA::String_var serveractivatorCorbaloc;
	try
	{
		serveractivatorCorbaloc=DomainMgr_->getObject(SERVERACTIVATOR,serverActivatorUUID_.in());
	}
	catch(const CORBA::SystemException &)
	{
	    TRACE0(LEVEL1,"ComponentServer: get serveractivator corbaloc from DomainConf failed in system error.\n");        	
	    throw;
	}
	catch(...)
	{
		TRACE0(LEVEL1,"ComponentServer: get serveractivator corbaloc from DomainConf failed .\n");        	
		throw CORBA::UNKNOWN();
	}
		
	try
	{
	    obj  = orb_ -> string_to_object(serveractivatorCorbaloc);
	}
	catch(const CORBA::BAD_PARAM&)
	{
	    TRACE0(LEVEL1,"ComponentServer: invalid ServerActivator reference.\n");        	
	    throw;
	}
		
	if(CORBA::is_nil(obj))
	{
		TRACE0(LEVEL1,"ComponentServer: cannot get the reference of ServerActivator.\n");    		
		throw;
	}

	ServerActivator_var serverActivator=ServerActivator::_narrow(obj);
	assert(!CORBA::is_nil(serverActivator)); 

	return serverActivator._retn();
}      

//
//The create_container operation creates a new Container object within the
//component server process. The config parameter represents a sequence of
//ConfigValue objects that provide name value pairs used to configure the new
//Container. The operation raises an InvalidConfiguration exception if the
//configuration values are not valid. The operation raises a CreateFailure exception if
//a container could not be created for internal reasons such as insufficient resources.
//
Container_ptr 
ComponentServer_impl::create_container(const Components::ConfigValues& config)
        throw(Components::CreateFailure,
           	Components::Deployment::InvalidConfiguration,
              	CORBA::SystemException)
{
	//added by xiao heping 2004/03/21
	STARCCM_SYNCHRONIZED(sync_,*this)
	//end add xiao heping 2004/03/21

	if(verbose_)
	{
		cout<<"ComponentServer : create container. "<<endl;	
	}
	
	//
	//extract the UUID of the Container from the config
	//	
	CORBA::String_var containerUUID;
//	cout<<"before iterator.\n"<<endl;
	for(int i=0;i<config.length();i++)
	{
//	    cout<<"iterator :"<<i<<endl;
//		const char *configValue;
//		config[i]->value()>>=configValue;
//		cout<<"Position 1"<<endl;
//		CORBA::String_var value=CORBA::string_dup(configValue);
//		cout<<"Position 2"<<endl;
//		TRACE1(LEVEL6,"get config %s.\n", value.in());
		if(strcmp(config[i]->name(),"UUID")==0)
		{
//			cout<<"Position 3"<<endl;
//		    TRACE1(LEVEL6,"ComponentServer: create container with uuid %s.\n", value.in());

			//modified by xiao heping 2004/03/19
			const char *configValue;
			config[i]->value()>>=configValue;
			containerUUID = configValue;
			break;
			//end modify xiao heping 2004/03/19

//			containerUUID=value;
		}
//		cout<<"Position 4"<<endl;
	}
	
//	TRACE0(LEVEL1,"ComponentServer: create container 1.\n");
//	cout<<"Position 5"<<endl;

	//
	//The operation raises an InvalidConfiguration exception if the
	//configuration values are not valid.
	//
	if(containerUUID==NULL)
	{
		TRACE0(LEVEL1,"ComponentServer: create container failed in invalid configuration.\n");
		throw Components::Deployment::InvalidConfiguration();
	}

	//
	//judge whether the Container has existed, if so just return it's ref	
	//
	ContainersMap::iterator iter = containersMap_.find(string(containerUUID.in()));
	if(iter != containersMap_.end())
		return (*iter).second ->getReference();
/*	MapComponentServer::iterator iter=componentserverMap.begin();
	MapComponentServer::iterator iter_end=componentserverMap.end();
	while(iter!=iter_end)
	{
		CORBA::String_var id=(* iter).second;
		
		if(strcmp( id.in(),containerUUID.in() )==0)
		{
			if(verbose_)
			{
				cout<<"ComponentServer: the Container created has existed."<<endl;			
			}		

			//
			//get the container object reference
			//
			CORBA::String_var containerRef=CORBA::string_dup((* iter).first.c_str());
			CORBA::Object_var obj;		
							
			try
			{
				obj  = orb -> string_to_object(containerRef);
			}
			catch(const CORBA::BAD_PARAM&)
			{
			    TRACE0(LEVEL1,"ComponentServer: invalid container reference.\n");        	
			    throw;
			}
				
			if(CORBA::is_nil(obj))
			{
				TRACE0(LEVEL1,"ComponentServer: cannot get the reference of container.\n");    		
				throw new Components::CreateFailure;
			}
		
			Container_var container = Container::_narrow(obj);
			assert(!CORBA::is_nil(container)); 
			return container._retn();
		}
		iter++;
	}
*/	
//	TRACE0(LEVEL1,"ComponentServer: create container 3.\n");
	
	//
	// get the corbaloc of the ComponentServer 
	//
	CORBA::String_var componentServerCorbaloc;
	try
	{
		componentServerCorbaloc=DomainMgr_->getObject(COMPONENTSERVER,componentServerUUID_);		
	}
	catch(...)
	{
		TRACE0(LEVEL1,"ComponentServer: create container failed in getting ComponentServer corbaloc .\n");
		throw Components::CreateFailure();		
	}	
	
//	TRACE0(LEVEL1,"ComponentServer: create container 4.\n");
	
	//
	// get the ref of ComponentServer from the corbaloc
	//
	CORBA::Object_var componentServerObj;
	try
	{
		componentServerObj  = orb_ -> string_to_object(componentServerCorbaloc);
	}
	catch(const CORBA::BAD_PARAM&)
	{
		TRACE0(LEVEL1,"ComponentServer: invalid ComponentServer corbaloc.\n");        	
		throw;
	}
	if(CORBA::is_nil(componentServerObj))
	{
	    TRACE0(LEVEL1,"ComponentServer: cannot get the reference of ComponentServer.\n");    		
		throw Components::CreateFailure();
	}
	ComponentServer_var componentServer=::Components::Deployment::ComponentServer::_narrow(componentServerObj);
	assert(!CORBA::is_nil(componentServer)); 

	//
	//if the Container does not exist, create a new one
	//
	TRACE0(LEVEL5, "preparing startup container.\n");
    CORBA::String_var containerObjId=CORBA::string_dup(containerUUID);
    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId(containerUUID.in());
    	
    TRACE0(LEVEL5, "create a new container.\n");

	//modified by xiao heping 2004/03/19
    //::Container::ContainerBase_ptr containerImpl = 
	::Container::ContainerBase_var containerBase = new ::Container::ContainerBase(orb_.in(),
		oid.in(),componentServerUUID_.in(),componentServer.in(),DomainMgr_.in());
	//end modify xiao heping 2004/03/19

    TRACE0(LEVEL5, "startup container.\n");
    containerBase -> startup();    

    componentManager_->setContainerObj(oid.in(),containerBase.in());
    //jxh--end

	//
	//get reference of Container
	//
    Container_var container = containerBase -> getReference();
    	
    TRACE1(LEVEL5,"got reference of Container %s\n", containerUUID.in());

#ifdef ORBacus
    CORBA::Object_var obj = orb_ -> resolve_initial_references("BootManager");
    OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);
    bootManager -> add_binding(oid, container);
#endif	

	//added by xiao heping 2004/02/15
#ifdef StarBus
    CORBA::Object_var obj = orb_ -> resolve_initial_references("AssistAdaptorManager");
    STAR::AssistAdaptorManager_var assistAdapterManager = STAR::AssistAdaptorManager::_narrow(obj);
    assistAdapterManager -> binding_objref(oid, container);
#endif
	//end added

	TRACE0(LEVEL5,"Container start up complete\n");
	
	//
	//construct the corbaloc of the Container
	//
	CORBA::String_var componentServerID=CORBA::string_dup(strrchr(componentServerCorbaloc.in(),'/'));
	int componentServerIDLen= strlen(componentServerID.in())-1;
	int componentServerCorbalocLen=strlen(componentServerCorbaloc.in());
	int bufferSize = componentServerCorbalocLen-componentServerIDLen;
	
	char *corbalocPrefix=(char *)malloc(bufferSize+1);
	strncpy(corbalocPrefix,componentServerCorbaloc.in(),bufferSize);
	corbalocPrefix[bufferSize] = NULL;		
	CORBA::String_var containerCorbaloc = CORBA::string_dup(corbalocPrefix);
	free(corbalocPrefix);		

#ifdef STARCCMSTRINGADD
	containerCorbaloc = HelpFun::CORBA_string_add(containerCorbaloc.in(), containerObjId.in());
#else
	containerCorbaloc+=CORBA::string_dup(containerObjId);
#endif
	
	TRACE1(LEVEL6,"ComponentServer: the container corbaloc is %s.\n",containerCorbaloc);	

//added by xiao heping 2004/05/28	
	//
	//get the TimeOut from the DeployDomainMgr
	//
	CORBA::String_var timestr;
	try
	{
		timestr=DomainMgr_->getObjectValue(TIMEOUT);
	}
	catch(...)
	{		
		TRACE0(LEVEL1,"ComponentServer: get TimeOut from the DeployDomainMgr failed. \n ");        			
		throw Components::CreateFailure();		
	}
	int timeout=atoi(timestr.in());		
//end add

//modified by xiao heping 2004/05/28
	//
	//judge whether the newly created Container started correcly within timeout
	//
	if(!HelpFun::ObjectExist(orb_,containerCorbaloc.in(),timeout))
		throw  Components::CreateFailure();
	
//	CORBA::Object_var containerObj;
//	try
//	{
//		//modified by xiao heping 2004/02/15
//		//containerObj=ObjectExist(componentServerCorbaloc.in());
//		containerObj=ObjectExist(containerCorbaloc.in());
//		//end modified
//	}
//	catch(const JudgeObjectExistFailure &)
//	{
//		TRACE0(LEVEL1,"ComponentServer: judge whether created the Container successfully failed.\n");
//		throw  Components::CreateFailure();
//	}
//	catch(const ObjectNotExist &)
//	{
//		TRACE0(LEVEL1,"ComponentServer: created the Container failed.\n");
//		throw  Components::CreateFailure();		
//	}
//end add

	//
	//add newly created Container info into the map
	//
	//modified by xiao heping 2004/03/31
//	CORBA::String_var containerRef = orb -> object_to_string(container);
//	TRACE0(LEVEL5,"Register Container to ComponentServer\n");
//	componentserverMap.insert(MapComponentServer::value_type(string(containerRef.in()),containerUUID));	
	containersMap_.insert(ContainersMap::value_type(string(containerUUID.in()),containerBase));
	//end modify

	//
	//added by xiao heping 2004/03/12
	//for test remove_home method
	//
//#ifdef _DEBUG	
//	//
//    // Save reference
//    //
//	CORBA::String_var containerRef = orb_ -> object_to_string(container);
//
//	CORBA::String_var refFile = (const char*)getenv("CCM_HOME");
//    refFile += PATH_DELILIMITOR;
//    refFile += "Container.ref";
//
//    //const char* refFile = "c:\\ccm\\Container.ref";
//    ofstream out(refFile);
//    if(out.fail())
//    {
//	cerr << ": can't open '" << refFile << "': "
//	     << strerror(errno) << endl;
//	return 0;
//    }
//    out << containerRef << endl;
//    out.close();
//#endif	
	//end add

	//
	//register the newly created Container to the DeployDomainMgr
	//
	try
	{
		DomainMgr_->registerContainer(containerUUID.in(),componentServerUUID_.in(),containerCorbaloc.in(),config);
	}
	catch(...)
	{
		TRACE0(LEVEL1,"ComponentServer: register the Container to the DeployDomainMgr Failed.\n");
		throw Components::CreateFailure();
	}
	
	::Container::ConfigFile_var configFile = new ::Container::ConfigFile();
	
	//modified by xiao heping 2004/02/15
	//configFile->setUuid(componentServerUUID_.in());
	//configFile->addContainer(containerUUID.in());
	configFile->addContainer(componentServerUUID_.in(),containerUUID.in());
	//end modified
	
	return container._retn();	
}	                         

//
//The remove_container operation removes a previously created container and all
//homes and components that were created in the container. The cref parameter refers to
//the container that should be removed. The operation raises a BAD_PARAM system
//exception if the cref parameter was not created by this ComponentServer. The
//operation raises a RemoveFailure exception if the container or any of its homes
//could not be removed for any internal reasons.
//
void 
ComponentServer_impl::remove_container(Components::Deployment::Container_ptr cref)
        throw(Components::RemoveFailure,
          	CORBA::SystemException)
{
	//added by xiao heping 2004/03/21
	STARCCM_SYNCHRONIZED(sync_,*this)
	//end add xiao heping 2004/03/21

	//modified by xiao heping 2004/03/13

	if(verbose_)
	{
		cout << "ComponentServer: remove Container " << endl;			
	}

	//
	//whether the cref parameter was created by this componentServer or not?
	//
	Container_var container = Components::Deployment::Container::_duplicate(cref);
	ContainersMap::iterator iter=containersMap_.begin();
	ContainersMap::iterator iter_end=containersMap_.end();
	::Container::ContainerBase_var containerBase ;
	while(iter!=iter_end)
	{
		containerBase = (* iter).second;
		Container_var containerInMap = containerBase -> getReference();
//		CORBA::String_var strContainerRef=(* iter).first.c_str();
//		CORBA::Object_var containerObj = orb -> string_to_object (strContainerRef);
//		Container_var containerSaved = Container::_narrow(containerObj);
		if(containerInMap -> _is_equivalent(container))
			break;
		iter++;
	}

	//
	//if the cref parameter was not created by this componentServer,
	//the opration will raise a BAD_PARAM system exception
	//
	if (iter == iter_end)
	{
		TRACE0(LEVEL1,"ComponentServer: the Container is not in this ComponentServer.\n");
		throw new CORBA::BAD_PARAM();		
	}

	//
	//removes the container and all homes and components that were created in the container.
	//The operation raises a RemoveFailure exception if the container or any of its homes
	//could not be removed for any internal reasons.
	//
	try
	{
		CORBA::String_var containerUUID = containerBase -> getUUID();
		componentManager_ -> removeContainer(containerUUID.in());
		containerBase -> remove();
	}
	catch(Components::RemoveFailure&)
	{
		TRACE0(LEVEL1,"ComponentServer: remove the Container failed in container remove.\n");
		throw;
	}
	catch(...)
	{
		TRACE0(LEVEL1,"ComponentServer: remove the Container failed in container remove.\n");
		throw new Components::RemoveFailure();
	}
	
	
	//
	//erase the container in the MapComponentServer
	//
	try
	{
		containersMap_.erase(iter);	
	}
	catch(...)
	{
		TRACE0(LEVEL1,"ComponentServer: remove Container failed in locate it failure.\n");
		throw new Components::RemoveFailure();	
	}

	//end modify xiao heping 2004/03/13
}

//
//The get_containers operation returns a sequence of all Container objects created
//by this ComponentServer
//
Containers* 
ComponentServer_impl::get_containers()
        throw(CORBA::SystemException)
{
	//added by xiao heping 2004/03/21
	STARCCM_SYNCHRONIZED(sync_,*this)
	//end add xiao heping 2004/03/21

	if(verbose_)
	{
		cout<<"ComponentServer: get all the Container created by the ServerActivator "<<endl;	
	}
	
	Containers_var containerList=new Containers();
	CORBA::ULong len = containerList->length();
	
	//modified by xiao heping 2004/03/31
	ContainersMap::iterator theIterator = containersMap_.begin();
	for( ;theIterator!=containersMap_.end();++theIterator)	
	{
//		CORBA::String_var containerRef=CORBA::string_dup(theIterator->first.c_str());
//		CORBA::Object_var obj;		
//		try
//		{
//		    obj  = orb -> string_to_object(containerRef);
//	    }
//	    catch(const CORBA::BAD_PARAM&)
//	    {
//	        TRACE0(LEVEL1,"ComponentServer: invalid Container reference.\n");        	
//	        throw;
//	    }
//	    if(CORBA::is_nil(obj))
//	    {
//	        TRACE0(LEVEL1,"ComponentServer: cannot get the reference of Container.\n");    		
//			throw;
//	   	}

//		Container_var container=Container::_narrow(obj);
//		assert(!CORBA::is_nil(container)); 
		
		containerList->length(len + 1);            	
//      containerList[len]= container.in();
		containerList[len]=  (* theIterator).second-> getReference();
        len++;
	}	
	return containerList._retn();
}	
        
//
//The remove operation removes the component server and all containers, homes, and
//components that were created in the component server. The operation raises a
//RemoveFailure exception if the component server or any of its containers could not
//be removed for any internal reasons.
//
void 
ComponentServer_impl::remove()
       throw(Components::RemoveFailure,CORBA::SystemException)
{
	//added by xiao heping 2004/03/21
	STARCCM_SYNCHRONIZED(sync_,*this)
	//end add xiao heping 2004/03/21

	//modified by xiao heping 

	if(verbose_)
	{
		cout<<"ComponentServer: remove the ComponentServer. "<<endl;		
	}

	//
	//remove the all the Container created by this ComponentServer 
	//
	ContainersMap::iterator iter=containersMap_.begin();
	while(iter != containersMap_.end())
	{
		
		//
		//get the container ref from its stringified ref
		//
/*		CORBA::String_var containerRef=CORBA::string_dup((* iter).first.c_str());
		CORBA::Object_var obj;		
		try
		{
			obj  = orb -> string_to_object(containerRef);
		}
		catch(const CORBA::BAD_PARAM&)
		{
		    TRACE0(LEVEL1,"ComponentServer: invalid container reference.\n");        	
		    throw;
		}
		if(CORBA::is_nil(obj))
		{
			TRACE0(LEVEL1,"ComponentServer: cannot get the reference of container.\n");    		
			throw;
		}
		
		Container_var container=Container::_narrow(obj);
		assert(!CORBA::is_nil(container)); 
*/		
		::Container::ContainerBase_var containerBase = (* iter).second;
		Container_var container = containerBase -> getReference();
		//
		//remove the container
		//
		try
		{
			remove_container(container.in());
		}
		catch(...)
		{
			TRACE0(LEVEL1,"ComponentServer: remove failed in remove single container.\n");    		
			throw Components::RemoveFailure();
		}
		iter=containersMap_.begin();
	}
	
	//
	//remove the ComponentServer from the DomainConf
	//
/*	try
	{
		//DomainMgr->removeObject(COMPONENTSERVER,componentServerUUID_.in());
	}
	catch(...)
	{
		TRACE0(LEVEL1,"ComponentServer: remove  failed in remove ComponentServer from the DomainConf.\n");
		throw Components::RemoveFailure();			
	}
*/	
	//
	//shutdown the ComponentServerProcess
	//
	orb_ -> shutdown(false);
}	             
			

void 
ComponentServer_impl::recover()
    	throw(Components::Deployment::RecoverFailure,
              	CORBA::SystemException)
{
	//left to be implemented
	if(verbose_)
	{
		cout<<"ComponentServer: recover the ComponentServer. "<<endl;	
	}
}
	
//deleted by xiao heping 
//*****************************************************************************************
//judge the Object newly created exist or not whithin the TimeOut
//if the Object exist ,then return its reference
//if any error occured during the judge process ,throw JudgeObjectExistFailure
//if the Object not exist, throw ObjectNotExist
//*****************************************************************************************
//CORBA::Object_ptr
//ComponentServer_impl::ObjectExist(const char * corbaloc)
//	throw (JudgeObjectExistFailure,Components::Deployment::ObjectNotExist)
//{
//	//
//	//get the object ref of the Object
//	//
//	CORBA::Object_var obj;
//	try
//	{
//		obj=orb_->string_to_object(corbaloc);
//    }
//    catch(const CORBA::BAD_PARAM&)
//    {
//        TRACE1(LEVEL1,"ComponentServer: object corbaloc %s invalid \n ",corbaloc);        	
//        throw JudgeObjectExistFailure();
//    }
//	
//    if(CORBA::is_nil(obj))
//    {
//		TRACE0(LEVEL1,"ComponentServer: : cannot get the reference of object. \n ");        			
//		throw JudgeObjectExistFailure();
//   	}
//	
//	//
//	//get the TimeOut from the DeployDomainMgr
//	//
//	CORBA::String_var timestr;
//	try
//	{
//		timestr=DomainMgr_->getObjectValue(TIMEOUT);
//	}
//	catch(...)
//	{		
//		TRACE0(LEVEL1,"ComponentServer: get TimeOut from the DeployDomainMgr failed. \n ");        			
//		throw JudgeObjectExistFailure();		
//	}
//	int timeout=atoi(timestr.in());	
//		
//	time_t ltime;
//	time( &ltime );
//	int timestart=ltime;
//	int timenow=ltime;
//	bool exist=false;
//	TRACE1(LEVEL6,"ComponentServer: the start time is %d .\n",timenow);	
//	
//    while(((timenow-timestart)<timeout)&&(exist==false))
//    {
//    	try
//    	{
//    		if(!(obj->_non_existent()))
//    		{
//    			exist=true;
//    		}
//    	}
//    	catch(const CORBA::TRANSIENT &)
//    	{
//    	}
//    		
//    	timenow=time( &ltime );
//    }
//    	
//    if(exist==false)
//    {
//    	TRACE1(LEVEL6,"ComponentServer: now it is time %d .\n",timenow);
//    	TRACE0(LEVEL2,"ComponentServer: object dose not created correctly whithin timeout.\n");    		
//    	throw Components::Deployment::ObjectNotExist();
//    }
//    	
//    return obj._retn();
//}
//end delete

void 
ComponentServer_impl::loadConfig()
    	throw(Components::Deployment::RecoverFailure,
              	CORBA::SystemException)
{
	//left to be implemented
	if(verbose_)
	{
		TRACE0(LEVEL3, "ComponentServer: load the ComponentServer configurations. \n");	
	}
	
	//load the configuration file
	::Container::ConfigFile_var config = new ::Container::ConfigFile();
	
	CORBA::StringSeq_var containerIds = config->getContainerIds(componentServerUUID_.in());
	
	//
	// get the corbaloc of the ComponentServer 
	//
	CORBA::String_var componentServerCorbaloc;
	try
	{
		componentServerCorbaloc=DomainMgr_->getObject(COMPONENTSERVER,componentServerUUID_);
	}
	catch(...)
	{
		TRACE0(LEVEL1,"ComponentServer: create container failed in getting ComponentServer corbaloc .\n");
		
	}
	
	//
	// get the ref of ComponentServer from the corbaloc
	//
	CORBA::Object_var componentServerObj;
	try
	{
	    componentServerObj  = orb_ -> string_to_object(componentServerCorbaloc);
	}
	catch(const CORBA::BAD_PARAM&)
	{
	    TRACE0(LEVEL1,"ComponentServer: invalid ComponentServer corbaloc.\n");        	
	       	
	}
	
	ComponentServer_var componentServer=::Components::Deployment::ComponentServer::_narrow(componentServerObj);
	assert(!CORBA::is_nil(componentServer)); 
		
	if(CORBA::is_nil(componentServerObj))
	{
	     TRACE0(LEVEL1,"ComponentServer: cannot get the reference of ComponentServer.\n");    		
		
	}
	
	for(int i = 0;i < containerIds->length();i++)
	{
	    //incarnate component container
	    CORBA::String_var containerObjId=CORBA::string_dup(containerIds[(CORBA::ULong)i]);
	    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId(containerObjId.in());

    	::Container::ContainerBase_var containerImpl = new ::Container::ContainerBase(orb_.in(),
			oid.in(),componentServerUUID_.in(),componentServer.in(),DomainMgr_.in());

    	containerImpl -> startup();
    	
    	Container_var container = containerImpl -> getReference();
    	
#ifdef ORBacus

        CORBA::Object_var obj = orb_ -> resolve_initial_references("BootManager");
    	OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);
  
  	    bootManager -> add_binding(oid, container);  	
#endif

#ifdef StarBus

        CORBA::Object_var obj = orb_ -> resolve_initial_references("AssistAdaptorManager");
    	STAR::AssistAdaptorManager_var assistAdapterManager = STAR::AssistAdaptorManager::_narrow(obj);
  
  	    assistAdapterManager -> binding_objref(oid, container);  	
#endif
    	
	}
	
}

void
ComponentServer_impl::flush()
{
    
}

