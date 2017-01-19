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

#ifndef AssemblyFactory_IMPL_H
#define AssemblyFactory_IMPL_H 

#include <Deployment_skel.h>
#include <DeployDomainMgr.h>

#include <FileAccessor.h>
#include <XMLParser.h>

#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace STARCCM::Deployment;

namespace Components
{
	namespace Deployment
	{
		
		class FileDownloadFailure
		{
		public:
		    FileDownloadFailure(){};		    
		};
		
		class ExtractAssemblyFailure
		{
		public:
		    ExtractAssemblyFailure(){};		    
		};									

		class ComponentInstallFailure
		{
		public:
		    ComponentInstallFailure(){};		    
		};				

		class InvalidConfigureFile
		{
		public:
		    InvalidConfigureFile(){};		    
		};											
					
		
		class AssemblyFactory_impl : 
			virtual public POA_Components::Deployment::AssemblyFactory,
			virtual public PortableServer::RefCountServantBase
		{
			
		public:
			AssemblyFactory_impl (CORBA::ORB_ptr orb_,DeployDomainMgr_ptr manager, const char* ipaddr, const char *directory,bool verbose_);
			~AssemblyFactory_impl ();
			
			Components::Cookie* create(const char* assembly_loc_)
			        throw(Components::Deployment::InvalidLocation,
			              Components::CreateFailure,
			              CORBA::SystemException);

			// add by wsf
			Components::Cookie* dynCreate(const char* dyncreate_loc_)
				throw(Components::Deployment::InvalidLocation,
			              Components::CreateFailure,
			              CORBA::SystemException);
			// end by wsf
    
			Components::Deployment::Assembly_ptr lookup(Components::Cookie* cookie_)
			        throw(Components::Deployment::InvalidAssembly,
			              CORBA::SystemException) ;

    
			void destroy(Components::Cookie* cookie_)
			        throw(Components::Deployment::InvalidAssembly,
			              Components::RemoveFailure,
			              CORBA::SystemException);
			void recover()
        			throw(Components::Deployment::RecoverFailure,
              				CORBA::SystemException);
			
		private:	
			
			typedef map<string,Assembly_var> MapAssemblyFactory;
			
			MapAssemblyFactory assemblyFactoryMap;				
						
			CORBA::ORB_var orb;
			
			DeployDomainMgr_var DomainMgr;
			
			CORBA::String_var rootDir;
			
			//add by wsf 2004.7.6
			CORBA::String_var IPAddr;
			//end by wsf 2004.7.6
			
			bool verbose;	

			//add by wsf
			DOMString assemblyUUID;
			DOMString port;
			ObjIds_var implIds;
			
			enum CreateType
			{
			    CREATE,
			    DYNCREATE
			} cType;
			//end by wsf			
									
			void download(const char * assembly_loc_)
				throw(FileDownloadFailure,
					STARCCM::Deployment::FileSystemError,
					CORBA::SystemException);	
			
			void extractAssembly(const char *assemblyPkg)
				throw(ExtractAssemblyFailure);
			
			CORBA::Object_ptr getCommonObj(const char * objName);						

			//add by wsf
			void parseCadFile(const char *cadFindName,const char *assemblyDir)
				throw(STARCCM::Deployment::XMLInitializeFailed,
					InvalidConfigureFile,
					FileDownloadFailure);
			//end by wsf
		
			void parseCsdFile(const char *csdFileName,const char *sourceDir,const char *targetDir)
				throw(STARCCM::Deployment::XMLInitializeFailed,
					InvalidConfigureFile,
					FileDownloadFailure);
					
			void parseHomePlacement(XMLParser *parser,DOM_Node root,
						DOM_Node homeplacementNode,
						const char * sourceDir,
						const char * targetDir,
						ObjIds &implIds)
				throw (FileDownloadFailure,
					InvalidConfigureFile,
					ComponentInstallFailure,
					CORBA::SystemException);
			
			void parseProcessCollocation(XMLParser *parser,DOM_Node root,
							DOM_Node processcollocationNode,
							const char *sourceDir,
							const char *targetDir,
							ObjIds &implIds)			
				throw (FileDownloadFailure,
					InvalidConfigureFile,
					ComponentInstallFailure,
					CORBA::SystemException);
			
			void parseHostCollocation(XMLParser *parser,DOM_Node root,
						DOM_Node hostcollocationNode,
						const char *sourceDir,
						const char *targetDir,
						ObjIds &implIds)
				throw (FileDownloadFailure,
					InvalidConfigureFile,
					ComponentInstallFailure,
					CORBA::SystemException);			

//modified by xiao heping 2004/05/27						
//			char * locateFile(const char *fileName);
//		
//			char * getHostIP();
//			
//			int copyFile(const char *source, const char *target);
//			
//			int deleteDir(const char *dir);	
//end modify							
			
		};
	}
}
#endif


