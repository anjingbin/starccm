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

#ifndef ComponentServer_IMPL_H
#define ComponentServer_IMPL_H

//#include <Deployment_skel.h>
#include <DeployDomainMgr.h>
#include <ContainerBase_fwd.h>

#include <map>
#include <string>
//jxh0315
#include <ComponentManager_impl.h>
//#include <Manager_skel.h>


using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;
using namespace ManagerDomain;
namespace Components
{
	namespace Deployment
	{

//deleted by xiao heping 2004/05/28		
//		class JudgeObjectExistFailure
//		{
//		public:
//		    JudgeObjectExistFailure(){};		    
//		};								
//		
//		class ObjectNotExist
//		{
//		public:
//		    ObjectNotExist(){};		    
//		};	
//end delete				
		
		class ComponentServer_impl : 
			virtual public POA_Components::Deployment::ComponentServer,
			virtual public PortableServer::RefCountServantBase
			STARCCM_COMMA_RECURSIVE_MUTEX
		{

		public:
		        //modified by jxh 04/03/15
			ComponentServer_impl (CORBA::ORB_ptr orb_,ComponentManager_impl* componentmanager_,PortableServer::POA_ptr poa_,DeployDomainMgr_ptr manger,const char * fatherId,const char * id,bool verbose_);
			~ComponentServer_impl ();
			
    		virtual Components::ConfigValues* configuration()
        		throw(CORBA::SystemException);

        	virtual Components::Deployment::ServerActivator_ptr get_server_activator()
        		throw(CORBA::SystemException);

    		virtual Components::Deployment::Container_ptr create_container(const Components::ConfigValues& config)
        		throw(Components::CreateFailure,
              			Components::Deployment::InvalidConfiguration,
              			CORBA::SystemException);

    		virtual void remove_container(Components::Deployment::Container_ptr cref)
        		throw(Components::RemoveFailure,
              				CORBA::SystemException);

			virtual Components::Deployment::Containers* get_containers()
			        throw(CORBA::SystemException);

    		virtual void remove()
        		throw(Components::RemoveFailure,
              			CORBA::SystemException);

		    virtual void recover()
        		throw(Components::Deployment::RecoverFailure,
              			CORBA::SystemException);
              				
            virtual void loadConfig()
        		throw(Components::Deployment::RecoverFailure,
              			CORBA::SystemException);
              				
            virtual void flush();
	
		private:						  
			
			//modified by xiao heping 2004/03/31
//			typedef map<string,CORBA::String_var> MapComponentServer;
//			MapComponentServer componentserverMap;
			typedef map<string,::Container::ContainerBase_var> ContainersMap;
			ContainersMap containersMap_;
			//end modify
			
			bool verbose_;
			
			CORBA::ORB_var orb_;
			
			PortableServer::POA_var poa_;
			
			DeployDomainMgr_var DomainMgr_;
			
			CORBA::String_var componentServerUUID_;
			
			CORBA::String_var serverActivatorUUID_;
			
			//jxh--0315
			ComponentManager_impl* componentManager_;
			//jxh--end

//deleted by xiao heping 2004/05/28			
//			CORBA::Object_ptr ObjectExist(const char * corbaloc)
//				throw (JudgeObjectExistFailure,Components::Deployment::ObjectNotExist);
//end delete
		};
	}
}
#endif

