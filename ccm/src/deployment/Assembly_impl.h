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

#ifndef Assembly_IMPL_H
#define Assembly_IMPL_H

#include <CosNaming.h>
#include <Deployment_skel.h>
#include <DeployDomainMgr.h>
#include <XMLParser.h>
#include <HomeRegistration.h>
#include <CCMObject.h>

#include <map>
#include <string>

//add by wsf
#include <Architecture.h>
//end by wsf

using namespace std;
using namespace Components;
using namespace STARCCM::Deployment;
using namespace STARCCM;

namespace Components
{
	namespace Deployment
	{
		
		class InvalidConfigureFile
		{
		public:
		    InvalidConfigureFile(){};		    
		};			

		class ConfigureFailure
		{
		public:
		    ConfigureFailure(){};		    
		};

		class setConfigurationCompleteFailure
		{
		public:
		    setConfigurationCompleteFailure(){};		    
		};

		//add by wsf
		class setHelperFailure
		{
		public:
		    setHelperFailure(){};		    
		};	
		//end by wsf
		
		class BuildHomePlacementFailure
		{
		public:
		    BuildHomePlacementFailure(){};		    
		};

		class BuildProcessCollocationFailure
		{
		public:
		    BuildProcessCollocationFailure(){};		    
		};
		
		class BuildHostCollocationFailure
		{
		public:
		    BuildHostCollocationFailure(){};		    
		};						
		
		class ConnectFailure
		{
		public:
		    ConnectFailure(){};
		};								
		
		class DisConnectFailure
		{
		public:
		    DisConnectFailure(){};		    
		};	
		
											
		
		class Assembly_impl : 
			virtual public POA_Components::Deployment::Assembly,
			virtual public PortableServer::RefCountServantBase
		{
		public:
			Assembly_impl (CORBA::ORB_ptr orb_,PortableServer::POA_ptr poa_,DeployDomainMgr_ptr manager,const char * id,const char * dir,bool verbose_);
			~Assembly_impl ();
			
			virtual void build()
        			throw(Components::CreateFailure,
              				CORBA::SystemException) ;

			//add by wsf
			virtual void dynBuild()
				throw(Components::CreateFailure,
              				CORBA::SystemException) ;

			virtual void dynLink(const char* fatherNodeName)
				throw(Components::CreateFailure,
              				CORBA::SystemException) ;

			virtual void dynUnLink(const char* fatherNodeName)
        			throw(Components::RemoveFailure,
              				CORBA::SystemException);

			virtual void updateArchitecture()
				throw(CORBA::SystemException);

			virtual void setComponentPropertis(const char* comid, const char* cpfname)
				throw(Components::Deployment::SetPropertiesFailure,
					CORBA::SystemException);
			//end by wsf
				
    			virtual void tear_down()
        			throw(Components::RemoveFailure,
              				CORBA::SystemException);
		
			virtual Components::Deployment::AssemblyState get_state()
        			throw(CORBA::SystemException);		
					        	
		       	virtual void recover()
        			throw(Components::Deployment::RecoverFailure,
              				CORBA::SystemException);

			virtual void shutdown()
        			throw(Components::RemoveFailure,CORBA::SystemException);					
			
		private:
			//add by wsf
			Architecture* arch;
			//end by wsf
			
//modified by wsf 2004.7.21
/*			
			struct FacilityInf
			{
				CORBA::String_var id;
				CORBA::Object_var ref;
                
                FacilityInf() {}
                
                FacilityInf(const FacilityInf& f)
                {
                    id = CORBA::string_dup(f.id);
                    ref = CORBA::Object::_duplicate(f.ref);
                }
                
                FacilityInf& operator=(const FacilityInf& f)
                {
                    id = CORBA::string_dup(f.id);
                    ref = CORBA::Object::_duplicate(f.ref);
                    
                    return *this;
                }
};


			struct HomeInf
			{
				CCMHome_var HomeRef;
				FacilityInf ServerActivatorInf;
				FacilityInf ComponentServerInf;
				FacilityInf ContainerInf;					 
                
                HomeInf() {}
                
                HomeInf(const HomeInf& h)
                {
                    HomeRef = Components::CCMHome::_duplicate(h.HomeRef);
                    ServerActivatorInf = h.ServerActivatorInf;
                    ComponentServerInf = h.ComponentServerInf;
                    ContainerInf = h.ContainerInf;
                }
                
                HomeInf& operator=(const HomeInf& h)
                {
                    HomeRef = Components::CCMHome::_duplicate(h.HomeRef);
                    ServerActivatorInf = h.ServerActivatorInf;
                    ComponentServerInf = h.ComponentServerInf;
                    ContainerInf = h.ContainerInf;
                    
                    return *this;
                }
			};
			
			typedef map<string,HomeInf> MapHomeInf;			
			
			MapHomeInf homeMap;
*/
//end by wsf 2004.7.21

			struct ComponentInf
			{
				CCMObject_var ComponentRef;
				bool ConfigurationComplete;
				bool SetConfigurationComplete;
				CORBA::StringSeq_var depend;
                
                ComponentInf() {}
                
                ComponentInf(const ComponentInf& c)
                {
                    ComponentRef = CCMObject::_duplicate(c.ComponentRef);
                    ConfigurationComplete = c.ConfigurationComplete;
                    SetConfigurationComplete = c.SetConfigurationComplete;
                    depend = new CORBA::StringSeq(c.depend);
                }
                
                ComponentInf& operator=(const ComponentInf& c)
                {
                    ComponentRef = CCMObject::_duplicate(c.ComponentRef);
                    ConfigurationComplete = c.ConfigurationComplete;
                    SetConfigurationComplete = c.SetConfigurationComplete;
                    depend = new CORBA::StringSeq(c.depend);
                    
                    return *this;
                }

			};
			
			typedef map<string,ComponentInf> MapComponentInf;			
			
			MapComponentInf componentMap;
						
			
//deleted by wsf 2004.7.21			
//			typedef map<string, CORBA::String_var> MapConnectInf;
			
//			MapConnectInf connectMap;
//end by wsf 2004.7.21	
			
								
			bool verbose;	
			
			CORBA::ORB_var orb;
			
			PortableServer::POA_var poa;
			
			DeployDomainMgr_var DomainMgr;
			
			CORBA::String_var assemblyUUID;		
			
			CORBA::String_var rootDir;
			
			Components::Deployment::AssemblyState State;	
			
			HomeRegistration_var homeRegistrationObj;
			//(assembly_impl.cpp:1282,1873)
			
			CosNaming::NamingContext_var nameServiceContextObj;
			
			CORBA::StringSeq_var registerNamings;			
			
			CORBA::StringSeq_var registerWithHomeFinders;
			
			CORBA::StringSeq_var registerWithNamings;	
			
			CORBA::StringSeq_var registerComponentId;	
			
			CORBA::StringSeq_var registerComponent;				
			
			CORBA::StringSeq_var registerFacetName;	
	
			CORBA::StringSeq_var facetRegisterName;
						

			void buildHomePlacement(XMLParser *parser,DOM_Node root,DOM_Node homeplacementNode)
				throw (InvalidConfigureFile, BuildHomePlacementFailure);
				
			void buildProcessCollocation(XMLParser *parser,DOM_Node root,DOM_Node processcollocationNode)
				throw (InvalidConfigureFile, BuildProcessCollocationFailure);
				
			void buildHostCollocation(XMLParser *parser,DOM_Node root,DOM_Node hostcollocationNode)
				throw (InvalidConfigureFile, BuildHostCollocationFailure);			

			//add by wsf
			void dynBuildHomePlacement(XMLParser *parser_,DOM_Node root_,DOM_Node homeplacementNode)
				throw (InvalidConfigureFile, BuildHomePlacementFailure);

			void dynBuildProcessCollocation(XMLParser *parser,DOM_Node root,DOM_Node processcollocationNode)
				throw (InvalidConfigureFile, BuildProcessCollocationFailure);
				
			void dynBuildHostCollocation(XMLParser *parser,DOM_Node root,DOM_Node hostcollocationNode)
				throw (InvalidConfigureFile, BuildHostCollocationFailure);	
			//end by wsf

			//modified by wsf 2004.6.23
			void getInstallHomeCfValues(XMLParser *parser,DOM_Node root,DOM_Node homeplacementNode,const char *implUUID,ConfigValues & cfValues, ConfigValues &dyncfValues)
				throw (InvalidConfigureFile);

			void getCsdInstallHomeCfValues(const char * csdFile,const char *implUUID, ConfigValues & cfValues, ConfigValues &dyncfValues)
				throw ( InvalidConfigureFile);

			void getCcdInstallHomeCfValues(const char * ccdFile,ConfigValues & cfValues, ConfigValues &dyncfValues)
				throw ( InvalidConfigureFile);
			//end by wsf 2004.6.23
			
			void parseHomeFeatures(XMLParser *parser, DOM_Node root,const char * homerepid, CORBA::StringSeq &txnRepids,CORBA::StringSeq &txnNames, CORBA::StringSeq &txnPolicies)
				throw ( InvalidConfigureFile);

			void parseComponentFeatures(XMLParser *parser, 
				 			DOM_Node root,
				 			const char * id,
				 			CORBA::StringSeq &txnRepids, 
							CORBA::StringSeq &txnNames, 
							CORBA::StringSeq &txnPolicies,					
							CORBA::StringSeq &eventPortNames, 
							CORBA::StringSeq &eventPortPolicies)
				throw ( InvalidConfigureFile);
			
			void parseInterface(XMLParser *parser, DOM_Node root,const char * id, CORBA::StringSeq &txnNames, CORBA::StringSeq &txnPolicies)
				throw ( InvalidConfigureFile);
			
			void parseOperationPolicies(XMLParser *parser,DOM_Node node,CORBA::StringSeq &txnNames, CORBA::StringSeq &txnPolicies)
				throw (InvalidConfigureFile);		
			
			char * getCsdFile(XMLParser *parser, DOM_Node root, DOM_Node homeplacementNode)
				throw ( InvalidConfigureFile);

			char *  getCcdFile(XMLParser *parser, DOM_Node root,const char *implUUID,const char *componentDir)
				throw ( InvalidConfigureFile);
			
			void configHome(const char *ccdPropFile, const char *cadPropFile,Components::CCMObject_ptr obj)
				throw( InvalidConfigureFile,ConfigureFailure);
			
			void configHomeComponent(const char * propFile,Components::CCMHome_ptr home,const char *id)
				throw( InvalidConfigureFile,ConfigureFailure);
			
			void configComponent(const char *ccdPropFile, const char *cadPropFile,Components::CCMObject_ptr obj)
				throw( InvalidConfigureFile,ConfigureFailure);
	
			ConfigValue * getPrimaryKeyProp(const char * propFile)
				throw (InvalidConfigureFile);
	
			ConfigValues * getProp(const char * propFile)
				throw(InvalidConfigureFile);

			ConfigValue * getNodeProp(XMLParser *parser,DOM_Node node,bool seqMem)
				throw(InvalidConfigureFile);
			
			char * getValue(XMLParser *parser,DOM_Node node)
				throw(InvalidConfigureFile);
	
			void connectComponents(XMLParser *parser,DOM_Node root)
				throw (InvalidConfigureFile,ConnectFailure);

			void setDynamicConfigurator(XMLParser *parser,DOM_Node node)
				throw (InvalidConfigureFile,ConnectFailure);				
			
			void disconnectComponents(XMLParser *parser,DOM_Node root,const char* nodename)
				throw (InvalidConfigureFile,DisConnectFailure);
			
			void setConfigurationComplete()
				throw (setConfigurationCompleteFailure);

			bool circleExist(MapComponentInf::iterator comparedNodeIter,MapComponentInf::iterator targetNodeIter)
				throw (setConfigurationCompleteFailure);				

//deleted by xiao heping 2004/05/27			
//			char * locateFile(const char *fileName);
//			char * uuidGenerator();
//end delete

			//add by wsf
			void setHelper(MapComponentInf::iterator iter)
				throw (setHelperFailure);
				
			//__deleted 2004.7.21
			//void delElementInSeq(const char* element, const CORBA::StringSeq & srcSeq,CORBA::StringSeq & newSeq);
			//end by wsf	
			
		};
	}
}

#endif


