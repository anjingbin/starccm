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

#ifndef DeployDomainMgr_IMPL_H
#define DeployDomainMgr_IMPL_H

#include <DeployDomainMgr_skel.h>
#include <XMLParser.h>

namespace STARCCM
{
	namespace Deployment
	{
		
		class InvalidDomainConfFile
		{
		public:
		    InvalidDomainConfFile(){};
		    ~InvalidDomainConfFile(){};
		};


		
		class DeployDomainMgr_impl : 
			virtual public POA_STARCCM::Deployment::DeployDomainMgr,
			virtual public PortableServer::RefCountServantBase
		{
		public:
			DeployDomainMgr_impl (bool verbose_,const char * file);
			
			~DeployDomainMgr_impl ();
			
			void initialize()
				throw(STARCCM::Deployment::InvalidDomainConfFile);
				
			
			//
			//common operation
			//
			void registerObject(STARCCM::Deployment::ObjectType type,			                                
			                                const char* id,
			                                const char* loc)
			        throw(STARCCM::Deployment::UnknownObjectType,
			              STARCCM::Deployment::ObjectIDExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);
			
			void removeObject(STARCCM::Deployment::ObjectType type,
			                  const char* id)
			        throw(STARCCM::Deployment::UnknownObjectType,
			              STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);
			
			STARCCM::Deployment::ObjLoc getObject(STARCCM::Deployment::ObjectType type,
			                                      const char* id)
			        throw(STARCCM::Deployment::UnknownObjectType,
			              STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);

		      	char* getObjectValue(STARCCM::Deployment::ObjectType type)
        			throw(STARCCM::Deployment::UnknownObjectType,
              				STARCCM::Deployment::ObjectNotExist,
				        STARCCM::Deployment::InfoMissed,
				        STARCCM::Deployment::XMLParserError,
				        CORBA::SystemException);

			STARCCM::Deployment::ObjIds* getObjectUUIDs(STARCCM::Deployment::ObjectType type)
			        throw(STARCCM::Deployment::UnknownObjectType,
			              STARCCM::Deployment::InfoMissed,
			              CORBA::SystemException);			
			
			//
			//operation for assembly
			//
			void registerAssembly(const char* id,
			                      const char* loc,
				              const STARCCM::Deployment::ObjIds& implids)
			        throw(STARCCM::Deployment::ObjectIDExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);
			
   			void registerAssemblyInfo(const char* id,
                        	              const STARCCM::Deployment::HomeInfos& homeInformations,
                                	      const STARCCM::Deployment::ConnectInfos& connectInformations)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);
			
			STARCCM::Deployment::ObjIds* getAssemblyImplIds(const char* id)
        			throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException) ;
			
			void getAssemblyInfo(const char* id,
                                 	STARCCM::Deployment::ObjLoc_out loc,
                                 	STARCCM::Deployment::HomeInfos_out homeInformations,
                                 	STARCCM::Deployment::ConnectInfos_out connectInformations)
        			throw(STARCCM::Deployment::ObjectNotExist,
              				STARCCM::Deployment::InfoMissed,
              				STARCCM::Deployment::XMLParserError,
              				CORBA::SystemException);
			
			void tearDownAssembly(const char* id)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);			              
			
			
			
			//
			//operation for implementation
			//
			void registerImpl(const char* id,
			                  const char* source,
			                  const char* locallocation)
			        throw(STARCCM::Deployment::ObjectIDExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);
			

			void getImplInfo(const char* id,
			                 CORBA::String_out source,
			                 CORBA::String_out locallocation,
			                 STARCCM::Deployment::ObjIds_out assemblyids)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);



			//
			//operation for ServerActivator
			//

			void getServerActivatorInfo(const char* id,
			                            STARCCM::Deployment::ObjLoc_out loc,
			                            STARCCM::Deployment::ObjIds_out componentserverids,
			                            STARCCM::Deployment::ObjIds_out assemblyids)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);



			//
			//operation for ComponentServer
			//

    			void registerComponentServer(const char* id,
	                                         const char* fatherid,	                                         
	                                         const char* loc,
	                                         const Components::ConfigValues& config)
			        throw(STARCCM::Deployment::ObjectIDExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);

		   	STARCCM::Deployment::ObjIds* getComponentServerAssemlbyIds(const char* id)
		        	throw(STARCCM::Deployment::ObjectNotExist,
		              		STARCCM::Deployment::InfoMissed,
		              		STARCCM::Deployment::XMLParserError,
		              		CORBA::SystemException);
			

			void getComponentServerInfo(const char* id,
			                            STARCCM::Deployment::ObjLoc_out loc,
			                            STARCCM::Deployment::ObjId_out serveractivatorid,
			                            STARCCM::Deployment::ObjIds_out containerids,
			                            STARCCM::Deployment::ObjIds_out assemblyids)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);

		    	Components::ConfigValues* getComponentServerConf(const char* id)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);



			//
			//operation for container
			//
    			void registerContainer(const char* id,
	                                   const char* fatherid,	                                   
	                                   const char* loc,
	                                   const Components::ConfigValues& config)
			        throw(STARCCM::Deployment::ObjectIDExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);

			void registerInstallHome(const char* containerId,
			                        const char* id,
						const Components::ConfigValues& config)
				 throw(STARCCM::Deployment::ObjectIDExist,
				       STARCCM::Deployment::InfoMissed,
				       STARCCM::Deployment::XMLParserError,
				      CORBA::SystemException);

    			void removeInstallHome(const char* containerId, const char* id)
        			throw(STARCCM::Deployment::ObjectNotExist,
              				STARCCM::Deployment::InfoMissed,
              				STARCCM::Deployment::XMLParserError,
              				CORBA::SystemException);

    			STARCCM::Deployment::ObjIds* getContainerAssemlbyIds(const char* id)
        			throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);
			

			void getContainerInfo(const char* id,
			                      STARCCM::Deployment::ObjLoc_out loc,
			                      STARCCM::Deployment::ObjId_out componentserverid,
			                      STARCCM::Deployment::ObjIds_out assemblyids)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);

    			Components::ConfigValues* getContainerConf(const char* id)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);
			              
			//add by wsf
			void getHomeInfo(const char* id, STARCCM::Deployment::HomeInfo_out homeInformation)
				throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException);
			//en by wsf
			              
			//added by jxh 03/22
			
						
		  STARCCM::Deployment::AppInforSeq* getApplications(STARCCM::Deployment::ObjectType type)
					  throw(STARCCM::Deployment::UnknownObjectType,
					           STARCCM::Deployment::InfoMissed,
					           CORBA::SystemException);	
						

        			
        	private:
        	
        		CORBA::String_var DomainConfFile;
        		
        		bool verbose;
        		
        		XMLParser *parser; 
        		
        		DOM_Node root;   		
			
		};
	}
}
#endif


