// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Dou Lei    fancydou@sina.com 
//
// **********************************************************************

// Version: 1.0.0

#ifndef __ServerActivator_IMPL_H
#define __ServerActivator_IMPL_H

#include <Deployment_skel.h>
#include <DeployDomainMgr.h>

#include <string>
#include <map>

using namespace std;
using namespace STARCCM::Deployment;

namespace Components
{
	namespace Deployment
	{


//deleted by xiao heping 2004/05/27 		
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
		
		
		class ServerActivator_impl : 
			virtual public POA_Components::Deployment::ServerActivator,
			virtual public PortableServer::RefCountServantBase
		{
		
		public:
			ServerActivator_impl (CORBA::ORB_ptr orb_,STARCCM::Deployment::DeployDomainMgr_ptr manager,const char * id,const char * dir,bool verbose_);
			~ServerActivator_impl ();
			
    			virtual Components::Deployment::ComponentServer_ptr create_component_server(const Components::ConfigValues& config)
        			throw(Components::CreateFailure,
              				Components::Deployment::InvalidConfiguration,
              				CORBA::SystemException);

			virtual void remove_component_server(Components::Deployment::ComponentServer_ptr server)
			        throw(Components::RemoveFailure,
			              CORBA::SystemException);

			virtual Components::Deployment::ComponentServers* get_component_servers()
        			throw(CORBA::SystemException);

		       	virtual void recover()
        			throw(Components::Deployment::RecoverFailure,
              				CORBA::SystemException);
			
			
			
		private:
			
			struct ComponentServerInf
			{
				CORBA::String_var id;
				CORBA::String_var port;
			};
			
			typedef map<string, ComponentServerInf> MapServerActivator;			
			
			MapServerActivator serveractivatorMap;
			
			CORBA::String_var serverActivatorUUID;
			
			bool verbose;	
			
			CORBA::ORB_var orb;
			
			DeployDomainMgr_var DomainMgr;	
			
			CORBA::String_var rootDir;
			
//deleted by xiao heping 			
//			CORBA::Object_ptr ObjectExist(const char * corbaloc)
//				throw (JudgeObjectExistFailure,ObjectNotExist);
//end delete

			
		};
	}
}
#endif


