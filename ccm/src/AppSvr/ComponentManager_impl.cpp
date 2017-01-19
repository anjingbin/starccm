#include <CORBA.h>
#include <ComponentManager_impl.h>
#include <Deployment_skel.h>
#include <DeployDomainMgr.h>
//
#include <ContainerBase_fwd.h>
#include <ContainerBase.h>

#ifdef WIN32
#include <Windows.h>
#endif

#include <list>
#include <ContainerRunTime_fwd.h>
#include <ContainerRunTime.h>

#include <CustomThreadPool.h>
#include <ResourcePool.h>

// Module declare ::ManagerDomain
//
using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;
using namespace ManagerDomain;

//
// Interface declare ::ManagerDomain::ComponentManager
//

	
ManagerDomain::ComponentManager_impl::ComponentManager_impl(PortableServer::POA_ptr poa)
    : poa_(PortableServer::POA::_duplicate(poa))
{
}
ManagerDomain::ComponentManager_impl::ComponentManager_impl()
{
}
ManagerDomain::ComponentManager_impl::ComponentManager_impl(CORBA::ORB_ptr orb_,DeployDomainMgr_ptr manger,const char * componentServerID,bool verbose_)
{
	orb = CORBA::ORB::_duplicate(orb_);
	DomainMgr = DeployDomainMgr::_duplicate(manger);	
	componentServerUUID=CORBA::string_dup(componentServerID);
	verbose = verbose_;
	
	if (verbose) {
		cout << " construct ComponentManager servant"<< endl;
	};
	
}
ManagerDomain::ComponentManager_impl::~ComponentManager_impl()
{
}

PortableServer::POA_ptr
ManagerDomain::ComponentManager_impl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}




//
// Operation declare GetComponentInfor
//
ManagerDomain::ComponentInfor*
ManagerDomain::ComponentManager_impl::GetComponentInfor()
    throw(CORBA::SystemException)
{
    // TODO: Implementation
    ManagerDomain::ComponentInfor* _r = new ManagerDomain::ComponentInfor;
    return _r;
}

//
// Interface declare ::ManagerDomain::PerformanceManager
//
void ManagerDomain::ComponentManager_impl::setContainerObj(const PortableServer::ObjectId& oid,ContainerBase_ptr container_)
{
	//ContainerBase_var container;
	//container = OBJDUPLICATE(ContainerBase_ptr,container);

	PortableServer::ObjectId_var temoid = new PortableServer::ObjectId(oid);
	CORBA::String_var containerUUID = PortableServer::ObjectId_to_string(temoid.in());
	//containerBaseinfor.id = containerUUID;
	//containerBaseinfor.containerRef = OBJDUPLICATE(ContainerBase_ptr,container_);
	ContainerBase_var containerRef = OBJDUPLICATE(ContainerBase_ptr,container_);
	cout<<containerUUID<<endl;	
	
	//containerBaseInforMap.insert(ContainerBaseInforMap::value_type(_itoa(containerBaseInforMap.size(),s,10),containerBaseinfor));
	//containerBaseInforMap.insert(ContainerBaseInforMap::value_type(containerUUID.in(),containerBaseinfor));
	containerBaseInforMap.insert(ContainerBaseInforMap::value_type(containerUUID.in(),containerRef));
	ContainerBaseInforMap::iterator theIterator = containerBaseInforMap.begin();
	string s = (*theIterator).first;
	if(!containerBaseInforMap.empty())
        cout << "containerBaseInforMap has " << containerBaseInforMap.size() << " entries" << endl;
    else
        cout << "containerBaseInforMap is empty" << endl;


}
//
// Operation declare GetThreadInfor
//
ManagerDomain::ThreadPoolInforseq*
ManagerDomain::ComponentManager_impl::GetThreadInfor(const char* id)
    throw(CORBA::SystemException)
{
    // TODO: Implementation

	#if defined (STARCCM_MTL)
	CORBA::String_var oid = id;
	std::string key(oid.in());
	TRACE1(LEVEL6,"Object ID is %s\n",key.c_str());
	ContainerBaseInforMap::iterator iter = containerBaseInforMap.find(key);
	TRACE0(LEVEL6,"Search...\n");	
	if( iter == containerBaseInforMap.end())
	{
		TRACE0(LEVEL6,"Have NOT find any matched container\n");
		return NULL;
	}
	else
	{
		TRACE0(LEVEL6,"Matched Container found\n");
		//ContainerBaseInfor tem;
                //tem = iter->second;
		
	
		int i=0;
		//runtimelist = tem.containerRef->getRuntimelist();
		ContainerRunTimeList runtimelist;
		runtimelist = iter->second->getRuntimelist();
		CORBA::ULong runtimelen = runtimelist.size();
		ThreadPoolInforseq_var threadPoolInforSeq = new ThreadPoolInforseq(runtimelen);
		threadPoolInforSeq->length(runtimelen);
		
		ContainerRunTimeList::iterator theiter = runtimelist.begin();
		if(theiter==runtimelist.end())
		{
			threadPoolInforSeq = 0;
		}
		while(theiter!=runtimelist.end())
		{
			GreenThread::CustomThreadPool_var customThreadPool = (*theiter)->GetCustomThreadPool();

			PortableServer::ObjectId* boid =(*theiter)->getHomeObjectId();
			std::string key = PortableServer::ObjectId_to_string(*boid);
			cout<<key<<endl;
		
			threadPoolInfor.homeRepId = CORBA::string_dup((*theiter)->getHomeRepId());
			threadPoolInfor.homeUUID =  key.c_str();

			threadPoolInfor.threadPoolName = customThreadPool->getThreadPoolName();
			threadPoolInfor.currentThreadPoolNumbers = runtimelen;
			threadPoolInfor.threadPoolcurrentNumber = customThreadPool->getPoolSize();
			threadPoolInfor.threadLifeSpan = customThreadPool->getKeepAliveTime();
		    threadPoolInfor.requestNumbers = customThreadPool->getQueueSize();
			threadPoolInfor.MaximumpoolSize = customThreadPool->getMaximumPoolSize();
			threadPoolInfor.MinimumpoolSize = customThreadPool->getMinimumPoolSize();
		
			threadPoolInforSeq[i].homeRepId = threadPoolInfor.homeRepId;
			threadPoolInforSeq[i].homeUUID = threadPoolInfor.homeUUID;

			threadPoolInforSeq[i].threadPoolName = threadPoolInfor.threadPoolName ;
			threadPoolInforSeq[i].currentThreadPoolNumbers = threadPoolInfor.currentThreadPoolNumbers;
			threadPoolInforSeq[i].threadPoolcurrentNumber = threadPoolInfor.threadPoolcurrentNumber ;
			threadPoolInforSeq[i].threadLifeSpan = threadPoolInfor.threadLifeSpan ;
		    threadPoolInforSeq[i].requestNumbers = threadPoolInfor.requestNumbers;
			threadPoolInforSeq[i].MaximumpoolSize = threadPoolInfor.MaximumpoolSize ;
			threadPoolInforSeq[i].MinimumpoolSize = threadPoolInfor.MinimumpoolSize;

			i++;
			theiter++;
		}		
		return threadPoolInforSeq._retn();
	}
   
	#endif	
	ThreadPoolInforseq_var threadPoolInforSeq = new ThreadPoolInforseq(0);
	threadPoolInforSeq->length(0);	
	return threadPoolInforSeq._retn();
    
}

//
// Operation declare SetThteadPoolInfor
//
CORBA::Boolean
ManagerDomain::ComponentManager_impl::SetThreadPoolInfor(const char* id,
                                              const char* threadPoolName,
                                              const ManagerDomain::ThreadPoolInfor& threadInfor)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
	#if defined (STARCCM_MTL)
	CORBA::String_var oid = id;
	std::string key(oid.in());
	TRACE1(LEVEL6,"Object ID is %s\n",key.c_str());
	ContainerBaseInforMap::iterator iter = containerBaseInforMap.find(key);
	TRACE0(LEVEL6,"Search...\n");	
	if( iter == containerBaseInforMap.end())
	{
		TRACE0(LEVEL6,"Have NOT find any matched container\n");
		return false;
	}
	else
	{		
		
		TRACE0(LEVEL6,"Matched Container found\n");
		CORBA::String_var homeId = threadInfor.homeUUID;
		CORBA::String_var homeRepId = threadInfor.homeRepId;

	
		//ContainerBaseInfor tem;
                //tem = iter->second;		
		//runtimelist = tem.containerRef->getRuntimelist();
		ContainerRunTimeList runtimelist;
		runtimelist = iter->second->getRuntimelist();
		CORBA::ULong runtimelen = runtimelist.size();
		::Container::ContainerRunTimeList::iterator theiter;
		theiter = runtimelist.begin();
		if(theiter==runtimelist.end())
		{
			return false;
		}
		while(theiter!=runtimelist.end())
		{
				ContainerRunTime_var runtime;
				runtime = *theiter;
				PortableServer::ObjectId* boid =runtime->getHomeObjectId(); 
	
				std::string homeuuid = PortableServer::ObjectId_to_string(*boid);			
				cout<<homeuuid<<endl;
	    		CORBA::String_var homeRepId = runtime->getHomeRepId();
				GreenThread::CustomThreadPool_var customThreadPool= runtime->GetCustomThreadPool();
				if(strcmp(threadInfor.homeUUID.in(),homeuuid.c_str())==0 && strcmp(homeRepId.in(),threadInfor.homeRepId.in())==0)
				{
				
     				customThreadPool->setThreadPoolName(threadPoolName);
					customThreadPool->setKeepAliveTime(threadInfor.threadLifeSpan);    	
					customThreadPool->setMaximumPoolSize(threadInfor.MaximumpoolSize);
					customThreadPool->setMinimumPoolSize(threadInfor.MinimumpoolSize);

					return true;
				}
				theiter++;
		}	
   
	}
	#endif
    return false;


}  
ManagerDomain::ThreaPoolNumbers* ManagerDomain::ComponentManager_impl::GetThreadPoolNumbers(const char* containerUUID)
        throw(CORBA::SystemException)
{
    


	#if defined (STARCCM_MTL)
	ContainerBaseInforMap::iterator iter = containerBaseInforMap.find(containerUUID);
	TRACE0(LEVEL6,"Search...\n");	
	if( iter == containerBaseInforMap.end())
	{
		TRACE0(LEVEL6,"Have NOT find any matched container\n");
		return NULL;
	}
	else
	{
		TRACE0(LEVEL6,"Matched Container found\n");
		ManagerDomain::ThreaPoolNumbers* threadNumsInfor = new ManagerDomain::ThreaPoolNumbers ;
		//ContainerBaseInfor tem;
                //tem = iter->second;		
		//runtimelist = tem.containerRef->getRuntimelist();
		ContainerRunTimeList runtimelist;
		runtimelist = iter->second->getRuntimelist();
		CORBA::ULong runtimelen = runtimelist.size();
		threadNumsInfor->threadNum=runtimelen;
		threadNumsInfor->containerUUID=CORBA::string_dup(containerUUID);
		return threadNumsInfor;
	
	}
    #endif
    return 0;
}
  
CORBA::ULong ManagerDomain::ComponentManager_impl::GetContainerNumbers()
        throw(CORBA::SystemException)
{
	ContainerBaseInforMap::iterator theIterator = containerBaseInforMap.begin();
	
	containerNumbers = containerBaseInforMap.size();
	return containerNumbers;
}
/*void ComponentManager_impl::setThreadNumbers()
{
	ContainerBaseInforMap::iterator theIterator = containerBaseInforMap.begin();

	while (theIterator!=containerBaseInforMap.end())
	{
		ContainerBaseInfor tem;
        tem = (*theIterator).second;
		runtimelist = tem.containerRef->getRuntimelist();
		CORBA::ULong runtimelen = runtimelist.size();	
		
		
		//threadNumsInfor.containerUUID = CORBA::string_dup(*theIterator).first.c_str());

		CORBA::String_var s =(*theIterator).first.c_str();
	    threadNumsInfor.containerUUID =s;
		threadNumsInfor.threadNum = runtimelen ;
		threaPoolNumberLst.push_back(threadNumsInfor);
		theIterator++;
	
	}

}*/
void ManagerDomain::ComponentManager_impl::removeContainer(const char* containerUUID)
{
	ContainerBaseInforMap::iterator iter = containerBaseInforMap.find(containerUUID);
	if(iter==containerBaseInforMap.end())
	{
		return;		
	}
	containerBaseInforMap.erase(iter);

}

 //
// Operation declare RegisterComponent
//
CORBA::Boolean
ManagerDomain::ComponentManager_impl::RegisterComponent(const char* homeRepId,
                                                        const char* componentinstantiationref,
                                                        Components::CCMObject_ptr componentRef)
    throw(CORBA::SystemException)
{
    
	CCMObject_var componentobj = CCMObject::_duplicate(componentRef);
	CORBA::String_var s = orb->object_to_string(componentobj);
	TRACE1(LEVEL6,"component ref is %s.\n",s);
	CORBA::String_var componentName = CORBA::string_dup(componentinstantiationref);
	TRACE1(LEVEL6,"component name is %s.\n",componentinstantiationref);
	componentMap.insert(ComponentMap::value_type(componentName.in(),componentobj));
	componentInfor.homeRepId = CORBA::string_dup(homeRepId);
    CORBA::Boolean _r = true;
    return _r;
}

//
// Operation declare GetComponentRef
//
CORBA::Boolean
ManagerDomain::ComponentManager_impl::GetComponentRef(ManagerDomain::ComponentInfors_out componentInforSeq)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
	/*componentInfor temcomponent;
	ContainerBaseInforMap::iterator iter = containerBaseInforMap.begin();
	TRACE0(LEVEL6,"Search...\n");
	int conlen = containerBaseInforMap.size();
	if( iter == containerBaseInforMap.end())
	{
		TRACE0(LEVEL6,"Have NOT find any matched container\n");
		return NULL;
	}
	else
	{
		for(int i=0;i<conlen;i++)
		{
				ContainerRunTimeList runtimelist;
	        	runtimelist = iter->second->getRuntimelist();
				CORBA::ULong runtimelen = runtimelist.size();
				::Container::ContainerRunTimeList::iterator theiter;
	         	theiter = runtimelist.begin();
				if(theiter==runtimelist.end())
				{
					return false;
				}
				while(theiter!=runtimelist.end())
				{
					ContainerRunTime_var runtime;
					runtime = *theiter;
					//PortableServer::ObjectId* boid =runtime->getHomeObjectId(); 
			
					//std::string homeuuid = PortableServer::ObjectId_to_string(*boid);			
					//cout<<homeuuid<<endl;
	    			CORBA::String_var homeRepId = runtime->getHomeRepId();
					runtime->
				}		


		}
	}*/					
	//componentInforSeq = new ManagerDomain::ComponentInfors;
	//int componentNums = componentMap.size();	
	
	ComponentMap::iterator ite = componentMap.begin();
	int len = componentMap.size();
	cout<<componentMap.size()<<endl;
	componentInforSeq = new ManagerDomain::ComponentInfors(len);
	componentInforSeq->length(len);
	int count =0;
	while(ite!=componentMap.end())
	{
		
		//componentInfor->ComponentName = CORBA::string_dup((ite->first).c_str());
		componentInforSeq[count].ComponentName = CORBA::string_dup((ite->first).c_str());
		TRACE1(LEVEL6,"component name is %s.\n",componentInforSeq[count].ComponentName);
		componentInforSeq[count].componentRef = ite->second;
		CORBA::String_var s = orb->object_to_string(componentInforSeq[count].componentRef);
		TRACE1(LEVEL6,"component ref is %s.\n",s);
		//componentInfor->ComponentName =componentMap->first;
		componentInforSeq[count].ComponentID = CORBA::string_dup("");
		componentInforSeq[count].homeUuid=CORBA::string_dup("");
		componentInforSeq[count].homeRepId=componentInfor.homeRepId;
		
		ManagerDomain::ComponentStates state=ACTIVATION;
		componentInforSeq[count].ComponentSta=state;
		//ManagerDomain::ComponentTypes type = SERVICE;
		//componentInforSeq[count].ComponentType=type;
		//componentInforSeq[count].ComponentActivationTime=CORBA::string_dup("111");
		//componentInforSeq[count].ComponentPassivationTime=CORBA::string_dup("lll");
		//ManagerDomain::ServantLifetimePolicy policy = METHOD;
		//componentInforSeq[count].ServantPolicy=policy;
		ite++;
		count++;
	   
	}
	return true;
	
}
//
// Operation declare getConnectionInfor
//
ManagerDomain::ResourcePoolInforseq*
ManagerDomain::ComponentManager_impl::getConnectionInfor(const char* ContainerUUid)
    throw(CORBA::SystemException)
{
    // TODO: Implementation
	#if defined (WITH_OTS)

	CORBA::String_var oid = ContainerUUid;
	std::string key(oid.in());
	TRACE1(LEVEL6,"Object ID is %s\n",key.c_str());
	ContainerBaseInforMap::iterator iter = containerBaseInforMap.find(key);
	TRACE0(LEVEL6,"Search...\n");	
	if( iter == containerBaseInforMap.end())
	{
		TRACE0(LEVEL6,"Have NOT find any matched container\n");
		return NULL;
	}
	else
	{
		TRACE0(LEVEL6,"Matched Container found\n");
		//ContainerBaseInfor tem;
                //tem = iter->second;
		
	
		int i=0;
		//runtimelist = tem.containerRef->getRuntimelist();
		ContainerRunTimeList runtimelist;
		runtimelist = iter->second->getRuntimelist();
		CORBA::ULong runtimelen = runtimelist.size();
	    ManagerDomain::ResourcePoolInforseq_var  _r = new ManagerDomain::ResourcePoolInforseq(runtimelen);
		_r->length(runtimelen);
		
		ContainerRunTimeList::iterator theiter = runtimelist.begin();
		if(theiter==runtimelist.end())
		{
			_r = NULL;
			
		}
		while(theiter!=runtimelist.end())
		{
			//GreenThread::CustomThreadPool_var customThreadPool = (*theiter)->GetCustomThreadPool();

			//::STARCCM::ResourcePool_impl* resPool = new ::STARCCM::ResourcePool_impl();
			#ifndef LITE_VERSION
			::STARCCM::ResourcePool_impl* resPool = dynamic_cast<STARCCM::ResourcePool_impl*>((*theiter) -> getResourcePool());
			if(resPool == NULL){
				ManagerDomain::ResourcePoolInforseq_var  _r = new ManagerDomain::ResourcePoolInforseq(0);
				_r->length(0);
				return _r._retn();
			}
			PortableServer::ObjectId* boid =(*theiter)->getHomeObjectId();
			std::string key = PortableServer::ObjectId_to_string(*boid);
			cout<<key<<endl;		
			//_r.homeRepId = CORBA::string_dup((*theiter)->getHomeRepId());		
            _r[i].ContainerUUid =  CORBA::string_dup((*theiter)->getResourceName());
			_r[i].homeRepId = CORBA::string_dup((*theiter)->getHomeRepId());
			_r[i].initialcon = resPool->getinitialcom();
			_r[i].maxcon = resPool->getmaxcon();
			_r[i].currentcon = resPool->getcurrentcon();
		    _r[i].incon = resPool->getinc();	

			i++;
			theiter++;
			#else
			return NULL;
	    	#endif
		}		
		return _r._retn();
	}
   
	#endif		
	ManagerDomain::ResourcePoolInforseq_var  _r = new ManagerDomain::ResourcePoolInforseq(0);
	_r->length(0);
	return _r._retn();

   
    
}

//
// Operation declare setResourcePool
//
void
ManagerDomain::ComponentManager_impl::setResourcePool(const ManagerDomain::ResourcePoolInfor& recourcePool)
    throw(ManagerDomain::configResourcePoolError,
          CORBA::SystemException)
{
    // TODO: Implementation
}
//
// Operation declare getServerhome
//
char*
ManagerDomain::ComponentManager_impl::getServerhome()
    throw(ManagerDomain::NotsetCCMhome,
          CORBA::SystemException)
{
    // TODO: Implementation
    //CORBA::String_var ccmhome = getenv("CCM_HOME");
    CORBA::String_var ccmhome = CORBA::string_dup(getenv("CCM_HOME"));
    TRACE1(LEVEL1,"-----------------CCMHome------------------%s\n",ccmhome.in());
    //char* _r = getenv("CCM_HOME");
    return ccmhome._retn();
//    return getenv("CCM_HOME");
}

