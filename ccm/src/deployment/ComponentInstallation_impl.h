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

#ifndef ComponentInstallation_IMPL_H
#define ComponentInstallation_IMPL_H


#include <Deployment_skel.h>
#include <DeployDomainMgr.h>


#include <string>
#include <map>

using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;

namespace Components
{
	namespace Deployment
	{
		class ComponentInstallation_impl : 
			virtual public POA_Components::Deployment::ComponentInstallation,
			virtual public PortableServer::RefCountServantBase
		{

		public:
			ComponentInstallation_impl (DeployDomainMgr_ptr manager,const char *directory,bool _verbose);
			~ComponentInstallation_impl ();
			
			void install (const char * implUUID, const char * component_loc)
				throw(  Components::Deployment::InvalidLocation,
					Components::Deployment::InstallationFailure,
					CORBA::SystemException);
					
			void replace(const char* implUUID, const char* component_loc)
		        	throw(  Components::Deployment::InvalidLocation,
		              		Components::Deployment::InstallationFailure,
		              		CORBA::SystemException);
		
			void remove(const char* implUUID)
		        	throw(  Components::Deployment::UnknownImplId,
		              		Components::RemoveFailure,
		              		CORBA::SystemException);
		
			Components::Deployment::Location get_implementation(const char* implUUID)
		        	throw(  Components::Deployment::UnknownImplId,
		              		Components::Deployment::InstallationFailure,
		              		CORBA::SystemException);  
		        
		        virtual void recover()
        			throw(Components::Deployment::RecoverFailure,
              				CORBA::SystemException);	
              		
	
		
		private:			

			struct ImplInf {
				CORBA::String_var location;
				CORBA::String_var localLocation;	
			};
			
			typedef map<string, ImplInf,less<string> > MapImplInf;
			
			MapImplInf implMap;					
			
			DeployDomainMgr_var DomainMgr;
			
			bool verbose;
			
			CORBA::String_var rootDir;
			
			
			char * getLocalLocation(const char *implUUID,const char *dir);
			
			void download(const char *implUUID, const char *component_loc, const char *localLocation)
				throw(  Components::Deployment::InvalidLocation,
					Components::Deployment::InstallationFailure,
					CORBA::SystemException);
					
//deleted by xiao heping 
//			int deleteDir(const char *dir);			
//			int copyFile(const char *source, const char *target);	
//end delete		

		};
	}
}
#endif


