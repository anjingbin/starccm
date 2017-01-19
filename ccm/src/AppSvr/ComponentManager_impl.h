#ifndef ComponentManager_IMPL_H
#define ComponentManager_IMPL_H

#include <Manager_skel.h>
#include <map>
#include <string>
#include <Manager.h>
#include <ContainerRunTime_fwd.h>
#include <ContainerRunTime.h>
#include <list>

#include <Deployment_skel.h>
#include <DeployDomainMgr.h>
#include <ContainerBase.h>

//
// Module declare ::ManagerDomain
//


using namespace std;
using namespace Components;
//using namespace Components::Deployment;
using namespace STARCCM::Deployment;
// Module declare ::ManagerDomain
//
namespace ManagerDomain
{

//
// Interface declare ::ManagerDomain::ComponentManager
//
class ComponentManager_impl : virtual public POA_ManagerDomain::ComponentManager,
                              virtual public PortableServer::RefCountServantBase
{
    ComponentManager_impl(const ComponentManager_impl&);
    void operator=(const ComponentManager_impl&);
private:
        PortableServer::POA_var poa_;
        ContainerBase_var container;
	bool verbose;
			
	CORBA::ORB_var orb;		
			
        DeployDomainMgr_var DomainMgr;
			
	CORBA::String_var componentServerUUID;
			
	CORBA::String_var serverActivatorUUID;
	
	typedef list<ContainerRunTime_var> ContainerRunTimeList;
	
	//ThredPoolInforseq_var threadPoolInforSeq;
	ThreadPoolInfor threadPoolInfor;
		
	//Components::CCMObject_var componentRef;
	//CORBA::String_var componentName;
	ComponentInfor componentInfor;
	typedef map<string,CCMObject_var>ComponentMap;
	ComponentMap componentMap;
	
public:	
	/*typedef struct ContainerBaseInfor
	{
		CORBA::String_var  id;
		ContainerBase_ptr containerRef;
		
	};*/
	
	//typedef map<string,ContainerBaseInfor> ContainerBaseInforMap;
	typedef map<string,ContainerBase_var>ContainerBaseInforMap;
	ContainerBaseInforMap containerBaseInforMap;
	//ContainerBaseInfor containerBaseinfor;
	CORBA::ULong containerNumbers;
	CORBA::ULong threadPoolNumbers;
	//ManagerDomain::ThreaPoolNumbers threadNumsInfor;
	typedef list<ManagerDomain::ThreaPoolNumbers>ThreaPoolNumberLst;
	ThreaPoolNumberLst threaPoolNumberLst;
	

public:
	
   ComponentManager_impl(PortableServer::POA_ptr);
   ComponentManager_impl();
   ComponentManager_impl(CORBA::ORB_ptr orb_,DeployDomainMgr_ptr manger,
		const char* componentServerID,bool verbose_);
    ~ComponentManager_impl();

    virtual PortableServer::POA_ptr _default_POA();

   
    // Operation declare GetComponentInfor
    //
    virtual ManagerDomain::ComponentInfor*  GetComponentInfor()
        throw(CORBA::SystemException);

	//
	//
	//
    void setContainerObj(const PortableServer::ObjectId& oid,ContainerBase_ptr container_);
  
    
    
      //
    // Operation declare GetThreadInfor
    //
    virtual ManagerDomain::ThreadPoolInforseq* GetThreadInfor(const char* id)
        throw(CORBA::SystemException);

    //
  virtual ManagerDomain::ThreaPoolNumbers* GetThreadPoolNumbers(const char* containerUUID)
        throw(CORBA::SystemException);



    //
    // Operation declare GetContainerNumbers
    //
    virtual CORBA::ULong GetContainerNumbers()
        throw(CORBA::SystemException);

    //
    // Operation declare SetThteadPoolInfor
    //
    virtual CORBA::Boolean SetThreadPoolInfor(const char* id,
                                              const char* threadPoolName,
                                              const ManagerDomain::ThreadPoolInfor& threadInfor)
        throw(CORBA::SystemException); 
    //void setThreadNumbers();  
	void removeContainer(const char* containerUUID);
	
    //
    // Operation declare RegisterComponent
    //
    virtual CORBA::Boolean RegisterComponent(const char* homeRepId,
                                             const char* componentinstantiationref,
                                             Components::CCMObject_ptr componentRef)
        throw(CORBA::SystemException);

    //
    // Operation declare GetComponentRef
    //
    virtual CORBA::Boolean GetComponentRef(ManagerDomain::ComponentInfors_out componentInforSeq)
        throw(CORBA::SystemException);
  
  
      //
    // Operation declare getConnectionInfor
    //
    virtual ManagerDomain::ResourcePoolInforseq* getConnectionInfor(const char* ContainerUUid)
        throw(CORBA::SystemException);

    //
    // Operation declare setResourcePool
    //
    virtual void setResourcePool(const ManagerDomain::ResourcePoolInfor& recourcePool)
        throw(ManagerDomain::configResourcePoolError,
              CORBA::SystemException);
      //
    // Operation declare getServerhome
    //
    virtual char* getServerhome()
        throw(ManagerDomain::NotsetCCMhome,
              CORBA::SystemException);
              
};

}
#endif

