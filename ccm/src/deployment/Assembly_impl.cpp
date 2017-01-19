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

#include <CORBA.h> 
#include <Assembly_impl.h>
#include <Configurator_impl.h>
#include <FileAccessor.h>
#include <CCM.h>
//#include <UuidGenerator.h>
//jxh
#include <Manager.h>
//end
#include <iostream>
//added by xiao heping 
#include <CommonFunc.h>

//end add

//extern "C"
//{
//	#include <uuid.h>
//}

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>

#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>

#include <iostream>

//add by wsf
#ifdef WIN32
#include <conio.h>
#include <Windows.h>
#include <process.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif
//end by wsf

using namespace DynamicAny;
using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;
using namespace ManagerDomain;
//add by wsf
using namespace STARCCM;
//end by wsf

Assembly_impl::Assembly_impl(CORBA::ORB_ptr orb_,PortableServer::POA_ptr poa_,DeployDomainMgr_ptr manager,const char * id,const char * dir,bool verbose_)
	:orb(CORBA::ORB::_duplicate(orb_)), poa(PortableServer::POA::_duplicate(poa_)),DomainMgr(DeployDomainMgr::_duplicate(manager)),verbose(verbose_)
{
		
	assemblyUUID=CORBA::string_dup(id);
	rootDir=CORBA::string_dup(dir);
	
	State=INACTIVE;
	registerNamings = new CORBA::StringSeq;
	
	//add by wsf
	arch = new Architecture(orb_, id, rootDir.in());
	//end by wsf
	
	if (verbose) {
		cout << "Assembly : Assembly  "
		     <<assemblyUUID
		     <<" is running." 
		     << endl;
	};
}

Assembly_impl::~Assembly_impl ()
{
	//add by wsf
	delete arch;
	arch = NULL;
	//end by wsf
	
  if (verbose) 
  {
    cout << "Assembly: "
    	<<assemblyUUID
    	<<"terminating." 
    	<< endl;
  }
}

 
void 
Assembly_impl::build()
	throw(Components::CreateFailure,
        	CORBA::SystemException)
{
	
	CORBA::Object_var obj;
	ServerActivator_var serveractivator;
	ComponentServer_var componentserver;
	Container_var container;
	ConfigValues_var cflist;
	CCMHome_var ccmhome;
	
	
	if(verbose)
	{
		cout<<"Assembly:begin build assembly "<<assemblyUUID<<endl;
		
	}
	


	
	//
	//get the cad file of assembly
	//
	
	CORBA::String_var assemblyDir=rootDir;

#ifdef STARCCMSTRINGADD
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, PATH_DELILIMITOR);
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, assemblyUUID);
#else	
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=assemblyUUID;
#endif	
	
	CORBA::String_var cadFilePath=assemblyDir;
	
#ifdef STARCCMSTRINGADD
	cadFilePath = HelpFun::CORBA_string_add(cadFilePath, PATH_DELILIMITOR);
	cadFilePath = HelpFun::CORBA_string_add(cadFilePath, CORBA::string_dup("*.cad"));
#else	
	cadFilePath+=CORBA::string_dup(PATH_DELILIMITOR);
	cadFilePath+=CORBA::string_dup("*.cad");
#endif
	
	CORBA::String_var cadFileName=HelpFun::locateFile(cadFilePath.in());
	
	TRACE1(LEVEL6,"Assembly: build assembly: the cad file name is %s.\n",cadFileName.in());
	//added by jxh	
	ObjectType objtype;
	objtype=APPLICATIONS;
    string s=cadFileName.in();
	int strLen=s.size();	
	string tem=s.substr(0,strLen-4);

	DomainMgr->registerObject(objtype,tem.c_str(),assemblyUUID.in());	
	TRACE1(LEVEL2,"Assembly: build assembly: register Application name which is the file name of *.cad is %s.\n",cadFileName);

	//DomainMgr->registerObject(objtype,tem.c_str(),"");	
	//TRACE1(LEVEL2,"Assembly: build assembly: register Application name which is the file name of *.cad is %s.\n",cadFileName.in());

	//add end
	if(cadFileName==NULL)
	{
		TRACE1(LEVEL1,"Assembly: build assembly %s failed in locating cad file.\n",assemblyUUID.in());
		throw Components::CreateFailure();		
	}
 
	CORBA::String_var cadFile=assemblyDir;

#ifdef STARCCMSTRINGADD
	 cadFile = HelpFun::CORBA_string_add(cadFile, PATH_DELILIMITOR);
	cadFile = HelpFun::CORBA_string_add(cadFile, cadFileName);
#else		
	cadFile+=CORBA::string_dup(PATH_DELILIMITOR);	
	cadFile+=cadFileName;	
#endif	
	
	TRACE2(LEVEL6,"Assembly: build assemlby %s ,the cad file is %s.\n",assemblyUUID,cadFile.in());
	
	
	//
	//XMLParser initialilze
	//
	
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(cadFile.in());
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: build assemlby %s failed in XMLParser initializ. \n",assemblyUUID.in());		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::CreateFailure();
	}    
	
	DOM_Node root = parser->getRootNode("componentassembly");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: build assembly %s failed in Invalid cad file: miss componentassembly element. \n",assemblyUUID);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::CreateFailure();
	}
	
	
	//
	//parse the hostcollocation,processcollocation,homeplacement and build the component
	//
	
	DOM_Node partitionNode=parser->findChild(root,"partitioning");
	if(partitionNode==NULL)
	{
		TRACE1(LEVEL1,"Assembly: build assembly %s failed in Invalid cad file: miss componentassembly element. \n",assemblyUUID);		

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::CreateFailure();		
	}
	
 	DOM_NodeList partitionChildren=partitionNode.getChildNodes();
        unsigned int partitionChildrenLen=partitionChildren.getLength();	
	
	DOM_Node partitionChild;
	DOMString partitionChildName;
	
	for(unsigned int iterator=0;iterator<partitionChildrenLen;iterator++)
	{
		partitionChild=partitionChildren.item(iterator);
		partitionChildName=partitionChild.getNodeName();	
		
		if(partitionChildName.equals("homeplacement"))
		{
			try
			{
				//add by wsf

				//
				//judge whether the homeplacementId has existed.
				//

				//
				//get the homeplacement id
				//
				DOMString homeplacementId=parser->getAttr(partitionChild,"id");
				if(homeplacementId==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid cad file homeplacementNode miss id attribute. \n");
					throw InvalidConfigureFile();					
				}				
	
				//modified 2004.7.21
				/*
				MapHomeInf::iterator iter=homeMap.begin();
				MapHomeInf::iterator iter_end=homeMap.end();
				while(iter!=iter_end)
				{
					CORBA::String_var id=CORBA::string_dup((* iter).first.c_str());
					
					if(strcmp(id.in(),homeplacementId.transcode())==0)
					{
						if(verbose)
						{
							cout<<"Assembly: the homeplacement Id has existed."<<endl;			
						}	
						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}						
						throw Components::CreateFailure();
					}	
					iter++;
				}	
				*/
				ArchHome* ahome = arch->getArchHome(homeplacementId.transcode());
				if (ahome != NULL)
				{
					if(verbose)
					{
						cout<<"Assembly: the homeplacement Id has existed."<<endl;			
					}	
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					delete ahome;
					ahome = NULL;
					throw Components::CreateFailure();
				}
				//end by wsf
								
				buildHomePlacement(parser,root,partitionChild);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"Assembly: build assembly faild in build home placement .\n");
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				
				throw Components::CreateFailure();
			}
		}
		else if(partitionChildName.equals("processcollocation"))
		{
			try
			{
				buildProcessCollocation(parser,root,partitionChild);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"Assembly: build assembly faild in build process collocation .\n");
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				
				throw;
			}	
			
		}
		else if(partitionChildName.equals("hostcollocation"))
		{		

			try
			{
				buildHostCollocation(parser,root,partitionChild);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"Assembly: build assembly faild in build host collocation .\n");
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				
				throw Components::CreateFailure();
			}	
					
		}
	}
	
	//
	//connect the component according to the cad
	//
	TRACE0(LEVEL3,"Assembly: connect the component.\n");
	try
	{
		connectComponents(parser,root);
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE0(LEVEL1,"Assembly: build assembly faild in connect components : InvalidConfigureFile .\n");
				
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
			
		throw Components::CreateFailure();		
	}
	catch(const ConnectFailure &)
	{
		TRACE0(LEVEL1,"Assembly: build assembly faild in connect components : ConnectFailure .\n");
				
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
			
		throw Components::CreateFailure();				
	}
	
	//
	//call component configuration complete according to the depende relationship
	//
	TRACE0(LEVEL3,"Assembly: set configuration Complete.\n");
	try
	{
		setConfigurationComplete();
	}
	catch(const setConfigurationCompleteFailure &)
	{

		TRACE0(LEVEL1,"Assembly: build assembly faild in set component configuration complete .\n");
				
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
			
		throw Components::CreateFailure();
		
	}	
	
	//
	//register the assembly information to the DomainConf file
	//
	
	//deleted by wsf 2004.7.21
/*	
	//
	//get the home information
	//
	TRACE0(LEVEL5,"Assembly: register the home information.\n");
	STARCCM::Deployment::HomeInfos_var homeInformations=new STARCCM::Deployment::HomeInfos;
	
	MapHomeInf::iterator iter=homeMap.begin();
	
	MapHomeInf::iterator iter_end=homeMap.end();
	
	while(iter!=iter_end)
	{
		HomeInfo_var homeInformation=new HomeInfo;
		
		homeInformation->id=CORBA::string_dup(((* iter).first).c_str());
		
		//
		//change the object ref to stringified object ref
		//
		CORBA::String_var homeRef = orb -> object_to_string((* iter).second.HomeRef); 
		homeInformation->ref=homeRef;
		
		TRACE1(LEVEL5,"Assembly: the serveractivator id is %s.\n",((* iter).second.ServerActivatorInf.id).in()); 
		homeInformation->serveractivatorid=(* iter).second.ServerActivatorInf.id;
		
		TRACE1(LEVEL5,"Assembly: the ComponentServer id is %s.\n",((* iter).second.ComponentServerInf.id).in());
		homeInformation->componentserverid=(* iter).second.ComponentServerInf.id;

		TRACE1(LEVEL5,"Assembly: the container id is %s.\n",((* iter).second.ContainerInf.id).in());
		homeInformation->containerid=(* iter).second.ContainerInf.id;
		
		int len=homeInformations->length();
		
		homeInformations->length(len+1);
		
		homeInformations[(CORBA::ULong)len]=homeInformation;
		
		iter++;
	}	

	//
	//get the connect information
	//
	TRACE0(LEVEL5,"Assembly: register the connect information.\n");
	STARCCM::Deployment::ConnectInfos_var connectInformations=new STARCCM::Deployment::ConnectInfos;
	
	MapConnectInf::iterator connect_iter=connectMap.begin();
	
	MapConnectInf::iterator connect_iter_end=connectMap.end();
	
	while(connect_iter!=connect_iter_end)
	{
		ConnectInfo_var connectInformation=new ConnectInfo;
		
		connectInformation->id=CORBA::string_dup(((* connect_iter).first).c_str());
		
		connectInformation->cookie=(* connect_iter).second;		
		
		TRACE2(LEVEL5,"Assembly: the connect %s 's cookie is %s.\n",((* connect_iter).first).c_str(),((* connect_iter).second).in());
		
		int len=connectInformations->length();
		
		connectInformations->length(len+1);
		
		connectInformations[(CORBA::ULong)len]=connectInformation;
		
		connect_iter++	;	
	}	
	

	try
	{
		DomainMgr->registerAssemblyInfo(assemblyUUID.in(),homeInformations.in(),connectInformations.in());
	}
	catch(const CORBA::SystemException &)
	{
		TRACE0(LEVEL1,"Assembly: register the assembly information to the DomainMgr failed in SystemException.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw ;
	}
	catch(const STARCCM::Deployment::ObjectNotExist &)
	{
		TRACE0(LEVEL1,"Assembly: register the assembly information to the DomainMgr failed in ObjectNotExist.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}		
		
		throw Components::CreateFailure();		
		
	}
	catch(const STARCCM::Deployment::InfoMissed &)
	{
		TRACE0(LEVEL1,"Assembly: register the assembly information to the DomainMgr failed in InfoMissed.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}		
		
		throw Components::CreateFailure();		
		
	}
	catch(const STARCCM::Deployment::XMLParserError &)
	{
		TRACE0(LEVEL1,"Assembly: register the assembly information to the DomainMgr failed in XMLParserError.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}		
		
		throw Components::CreateFailure();		
		
	}
	*/
	//end by wsf 2004.7.21
	

	State=INSERVICE;

	//add by wsf
	arch->clean();
	arch->printToFile();
	arch->printToRelationFile();
	//end by wsf

	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}

	if(verbose)
	{
		cout<<"Assembly: succeed to build assembly  "<<assemblyUUID<<endl;
	}
	
}

//wsf
void 
Assembly_impl::dynBuild()
	throw(Components::CreateFailure,
		CORBA::SystemException) 
{
	CORBA::Object_var obj;
	ServerActivator_var serveractivator;
	ComponentServer_var componentserver;
	Container_var container;
	ConfigValues_var cflist;
	CCMHome_var ccmhome;
	
	
	if(verbose)
	{
		cout<<"Assembly:begin to dynbuild assembly "<<assemblyUUID<<endl;
	}
	
	//
	//get the dcd file of assembly
	//
	
	CORBA::String_var assemblyDir=rootDir;
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=assemblyUUID;
	

	CORBA::String_var dcdFilePath=assemblyDir;
	dcdFilePath+=CORBA::string_dup(PATH_DELILIMITOR);
	dcdFilePath+=CORBA::string_dup("*.dcd");
	CORBA::String_var dcdFileName=HelpFun::locateFile(dcdFilePath.in());
	
	TRACE1(LEVEL6,"Assembly: dynbuild assembly: the dcd file name is %s.\n",dcdFileName);
	if(dcdFileName==NULL)
	{
		TRACE1(LEVEL1,"Assembly: dynbuild assembly %s failed in locating dcd file.\n",assemblyUUID.in());
		throw Components::CreateFailure();		
	}
 
	CORBA::String_var dcdFile=assemblyDir;
	dcdFile+=CORBA::string_dup(PATH_DELILIMITOR);	
	dcdFile+=dcdFileName;	
	
	TRACE2(LEVEL6,"Assembly: dynbuild assemlby %s ,the dcd file is %s.\n",assemblyUUID,dcdFile.in());

	
	//
	//XMLParser initialilze
	//
	
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(dcdFile.in());
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: dynbuild assemlby %s failed in XMLParser initializ. \n",assemblyUUID.in());		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::CreateFailure();
	}    
	
	DOM_Node root = parser->getRootNode("componentassembly");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: dynbuild assembly %s failed in Invalid dcd file: miss componentassembly element. \n",assemblyUUID);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::CreateFailure();
	}
	
	
	//
	//parse the hostcollocation,processcollocation,homeplacement and build the component
	//
	
	DOM_Node partitionNode=parser->findChild(root,"partitioning");
	if(partitionNode==NULL)
	{
		TRACE1(LEVEL1,"Assembly: dynbuild assembly %s failed in Invalid dcd file: miss componentassembly element. \n",assemblyUUID);		

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::CreateFailure();		
	}
	
 	DOM_NodeList partitionChildren=partitionNode.getChildNodes();
        unsigned int partitionChildrenLen=partitionChildren.getLength();	
	
	DOM_Node partitionChild;
	DOMString partitionChildName;
	
	for(unsigned int iterator=0;iterator<partitionChildrenLen;iterator++)
	{
		partitionChild=partitionChildren.item(iterator);
		partitionChildName=partitionChild.getNodeName();	
		
		if(partitionChildName.equals("homeplacement"))
		{
			try
			{
				dynBuildHomePlacement(parser,root,partitionChild);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"Assembly: dynbuild assembly faild in dynbuild home placement .\n");
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				
				throw Components::CreateFailure();
			}
		}
		else if(partitionChildName.equals("processcollocation"))
		{
			try
			{
				dynBuildProcessCollocation(parser,root,partitionChild);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"Assembly: dynbuild assembly faild in build process collocation .\n");
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				
				throw;
			}	
			
		}
		else if(partitionChildName.equals("hostcollocation"))
		{		

			try
			{
				dynBuildHostCollocation(parser,root,partitionChild);
			}
			catch(...)
			{
				TRACE0(LEVEL1,"Assembly: dynbuild assembly faild in build host collocation .\n");
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				
				throw Components::CreateFailure();
			}	
					
		}
	}

	//
	//connect the component according to the cad
	//
	DOM_Node connectionsNode = parser->findChild(root, "connections");
	if (connectionsNode != NULL)
	{
		TRACE0(LEVEL3,"Assembly: connect the component.\n");
		try
		{
			connectComponents(parser,root);
		}
		catch(const InvalidConfigureFile &)
		{
			TRACE0(LEVEL1,"Assembly: build assembly faild in connect components : InvalidConfigureFile .\n");
					
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}
				
			throw Components::CreateFailure();		
		}
		catch(const ConnectFailure &)
		{
			TRACE0(LEVEL1,"Assembly: build assembly faild in connect components : ConnectFailure .\n");
					
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}
				
			throw Components::CreateFailure();				
		}
	}
	
	//delete the dcd file in rootDir/assemblyUUID/*.dcd	
#ifdef WIN32
		if(!DeleteFile(dcdFile.in()))
#else
		if( unlink(dcdFile.in()) == -1 )
#endif
		{
			TRACE0(LEVEL1,"XMLParser: delete the temp dtd file failed.");
			
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}
			
			throw Components::CreateFailure();	
		}

	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}	

	arch->clean();
	arch->printToFile();
	arch->printToRelationFile();

	if(verbose)
	{
		cout<<"Assembly: succeed to dynbuild assembly  "<<assemblyUUID<<endl;
	}
	

}

//wsf
void 
Assembly_impl::dynLink(const char* fatherNodeName)
	throw(Components::CreateFailure, CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"Assembly:begin to dynlink for assembly "<<assemblyUUID<<endl;
	}
	
	
	//
	//get the drd file of assembly
	//
	
	CORBA::String_var assemblyDir=rootDir;
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=assemblyUUID;
	
	CORBA::String_var drdFilePath=assemblyDir;
	drdFilePath+=CORBA::string_dup(PATH_DELILIMITOR);
	drdFilePath+=CORBA::string_dup("*.drd");
	CORBA::String_var drdFileName=HelpFun::locateFile(drdFilePath.in());
	
	TRACE1(LEVEL6,"Assembly: dynlink assembly : the drd file name is %s.\n",drdFileName);
	if(drdFileName==NULL)
	{
		TRACE1(LEVEL1,"Assembly: dynlink assembly %s failed in locating drd file.\n",assemblyUUID.in());
		throw Components::CreateFailure();		
	}
 
	CORBA::String_var drdFile=assemblyDir;
	drdFile+=CORBA::string_dup(PATH_DELILIMITOR);	
	drdFile+=drdFileName;	
	
	TRACE2(LEVEL6,"Assembly: dynlink assemlby %s ,the drd file is %s.\n",assemblyUUID,drdFile.in());
	
	
	//
	//XMLParser initialilze
	//
	
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(drdFile.in());
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: dynlink assemlby %s failed in XMLParser initializ. \n",assemblyUUID.in());		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::CreateFailure();
	}    
	
	DOM_Node root = parser->getRootNode("DynReconfig");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: dynlink assembly %s failed in Invalid drd file: miss componentassembly element. \n",assemblyUUID);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::CreateFailure();
	}

	DOM_Node fatherNode = parser -> findChild (root, fatherNodeName);	

	//
	//connect the component according to the drd file
	//
	TRACE0(LEVEL3,"Assembly: connect the component.\n");
	try
	{
		connectComponents(parser,fatherNode);
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE0(LEVEL1,"Assembly: dynlink assembly faild in connect components : InvalidConfigureFile .\n");
				
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
			
		throw Components::CreateFailure();		
	}
	catch(const ConnectFailure &)
	{
		TRACE0(LEVEL1,"Assembly: dynlink assembly faild in connect components : ConnectFailure .\n");
				
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
			
		throw Components::CreateFailure();				
	}

	if(verbose)
	{
		cout<<"Assembly: succeed to dynlink assembly  "<<assemblyUUID<<endl;
	}

	arch->clean();
	arch->printToFile();
	arch->printToRelationFile();
	
	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}	
}

//wsf
void 
Assembly_impl::dynUnLink(const char* fatherNodeName)
		throw(Components::RemoveFailure, CORBA::SystemException)
{
	if (verbose)
	{
		cout<<"Assembly:begin to dynunlink."<<endl;
	}		
	
	//
	//get the drd file of assembly
	//
	
	CORBA::String_var assemblyDir=rootDir;
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=assemblyUUID;
	
	CORBA::String_var drdFilePath=assemblyDir;
	drdFilePath+=CORBA::string_dup(PATH_DELILIMITOR);
	drdFilePath+=CORBA::string_dup("*.drd");
	CORBA::String_var drdFileName=HelpFun::locateFile(drdFilePath.in());
	if(drdFileName==NULL)
	{
		TRACE1(LEVEL1,"Assembly: dynunlink assembly %s failed in locating drd file.\n",assemblyUUID.in());
		throw Components::RemoveFailure();		
	}

	CORBA::String_var drdFile=assemblyDir;
	drdFile+=CORBA::string_dup(PATH_DELILIMITOR);	
	drdFile+=drdFileName;
	
		
	//
	//XMLParser initialilze
	//
	
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(drdFile.in());
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: dynunlink assemlby %s failed in XMLParser initializ. \n",assemblyUUID.in());		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::RemoveFailure();	
	}    
	
	DOM_Node root = parser->getRootNode("DynReconfig");
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: dynunlink assembly %s failed in Invalid drd file: miss componentassembly element. \n",assemblyUUID);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::RemoveFailure();
	}

	DOM_Node fatherNode = parser -> findChild (root, fatherNodeName);

	//
	//disconnect the component according to the drd file
	//
	TRACE0(LEVEL3,"Assembly: disconnect the component.\n");
	try
	{
		disconnectComponents(parser,fatherNode,"disconnections");
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE0(LEVEL1,"Assembly: dynunlink assembly faild in connect components : InvalidConfigureFile .\n");
				
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
			
		throw Components::RemoveFailure();		
	}
	catch(const ConnectFailure &)
	{
		TRACE0(LEVEL1,"Assembly: dynunlink assembly faild in connect components : ConnectFailure .\n");
				
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
			
		throw Components::RemoveFailure();				
	}
	catch(const CORBA::SystemException &)
	{
		TRACE0(LEVEL1,"Assembly: dynunlink assembly faild in connect components : CORBA SystemExceptions .\n");
				
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
			
		throw Components::RemoveFailure();	
	}

	if(verbose)
	{
		cout<<"Assembly: succeed to dynunlink assembly  "<<assemblyUUID<<endl;
	}	

	arch->clean();
	arch->printToFile();
	arch->printToRelationFile();

	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}	
	
}

//wsf
void 
Assembly_impl::updateArchitecture()
	throw(CORBA::SystemException)
{
	arch->update();
}

//wsf
void 
Assembly_impl::setComponentPropertis(const char* comid, const char* cpfname)
	throw(Components::Deployment::SetPropertiesFailure,
		CORBA::SystemException)
{
	ArchComponent* acom = arch->getArchComponent(comid);
	if (acom != NULL)
	{
		CCMObject_var ccmobj = ::Components::CCMObject::_narrow(acom->comref);
		try
		{
			configComponent(cpfname,"",ccmobj.in());
		}
		catch(InvalidConfigureFile&)
		{
			cout << "Assemly: configComponent fails in InvalidConfigureFile!" << endl;
			delete acom;
			acom = NULL;
			throw Components::Deployment::SetPropertiesFailure();
		}
		catch(ConfigureFailure&)
		{
			cout << "Assemly: configComponent fails in ConfigureFailure!" << endl;
			delete acom;
			acom = NULL;
			throw Components::Deployment::SetPropertiesFailure();
		}
		catch(CORBA::SystemException&)
		{
			cout << "Assemly: configComponent fails in SystemException!" << endl;
			delete acom;
			acom = NULL;
			throw;
		}
	}
	else
	{
		cout << "Assembly: setComponentPropertis fails in invalid comid: " << comid << endl;
		throw Components::Deployment::SetPropertiesFailure();
	}

	delete acom;
	acom = NULL;
}

void 
Assembly_impl::tear_down()
	throw(Components::RemoveFailure,CORBA::SystemException)
{
	
	if (verbose)
	{
		cout<<"Assembly:begin tear_down assembly"<<endl;
	}
	
	State=INACTIVE;		
	
	//
	//get the cad file of assembly
	//
	
	CORBA::String_var assemblyDir=rootDir;

#ifdef STARCCMSTRINGADD
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, PATH_DELILIMITOR);
	assemblyDir = HelpFun::CORBA_string_add(assemblyDir, assemblyUUID);
#else	
	assemblyDir+=CORBA::string_dup(PATH_DELILIMITOR);
	assemblyDir+=assemblyUUID;
#endif	
	
	CORBA::String_var cadFilePath=assemblyDir;

#ifdef STARCCMSTRINGADD
	cadFilePath = HelpFun::CORBA_string_add(cadFilePath, PATH_DELILIMITOR);
	cadFilePath = HelpFun::CORBA_string_add(cadFilePath, CORBA::string_dup("*.cad"));
#else	
	cadFilePath+=CORBA::string_dup(PATH_DELILIMITOR);
	cadFilePath+=CORBA::string_dup("*.cad");
#endif
	
	CORBA::String_var cadFileName=HelpFun::locateFile(cadFilePath.in());
	if(cadFileName==NULL)
	{
		TRACE1(LEVEL1,"Assembly: tear down assembly %s failed in locating cad file.\n",assemblyUUID.in());
		throw Components::RemoveFailure();		
	}

	CORBA::String_var cadFile=assemblyDir;

#ifdef STARCCMSTRINGADD
	cadFile = HelpFun::CORBA_string_add(cadFile, PATH_DELILIMITOR);
	cadFile = HelpFun::CORBA_string_add(cadFile, cadFileName);
#else	
	cadFile+=CORBA::string_dup(PATH_DELILIMITOR);
	cadFile+=cadFileName;
#endif	
	
		
	//
	//XMLParser initialilze
	//
	
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(cadFile.in());
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: tear down assemlby %s failed in XMLParser initializ. \n",assemblyUUID.in());		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::RemoveFailure();	
	}    
	
	DOM_Node root = parser->getRootNode("componentassembly");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: tear down assembly %s failed in Invalid cad file: miss componentassembly element. \n",assemblyUUID);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::RemoveFailure();	
	}	


	//
	//disconnect the components
	//
	try
	{
		disconnectComponents(parser,root,"");
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE1(LEVEL1,"Assembly: teardown assembly %s failed in disconnect components: invalid cad file. \n",assemblyUUID);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::RemoveFailure();	
	}
	catch(const DisConnectFailure &)
	{
		TRACE1(LEVEL1,"Assembly: teardown assembly %s failed in disconnect components: DisConnectFailure. \n",assemblyUUID);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::RemoveFailure();			
	}

	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}	
	
	//
	//unregister from the homefinder and namingservice
	//
	try
	{
		
		if(registerWithHomeFinders != NULL)
		{
			for(int j=0;j<registerWithHomeFinders->length();j++)
			{			
				homeRegistrationObj->unregister_by_name(registerWithHomeFinders[(CORBA::ULong)j]);			
			}
		}

		
		//
		//unregister the home from the namingservice
		//
		if(registerWithNamings != NULL)
		{
			for(int j=0;j<registerWithNamings->length();j++)			
			{	
				CosNaming::Name aName;
				aName.length(1);
				aName[0].id = CORBA::string_dup(registerWithNamings[(CORBA::ULong)j]);
				TRACE1(LEVEL6,"Assembly: unregister home from nameing service with name  %s. \n",aName[0].id);
		    	aName[0].kind = CORBA::string_dup("");
				nameServiceContextObj -> unbind(aName);
			}
		}

		//
		//unregister the component from the namingservice
		//		
		if(registerComponent != NULL)
		{					
			for(int j=0;j<registerComponent->length();j++)
			{
				CosNaming::Name aName;						
				aName.length(1);
				aName[0].id = CORBA::string_dup(registerComponent[(CORBA::ULong)j]);
			    aName[0].kind = CORBA::string_dup("");
					    
			    TRACE1(LEVEL6,"Assembly: unregister Component from naming service with the name is %s. \n",aName[0].id);
				nameServiceContextObj -> unbind(aName);
			}

		}

		//
		//unregister the facet from the namingservice
		//
		if(facetRegisterName != NULL)
		{						
			for(int j=0;j<facetRegisterName->length();j++)
			{						
				CosNaming::Name aName;
				aName.length(1);
				aName[0].id = CORBA::string_dup(facetRegisterName[(CORBA::ULong)j]);
			    aName[0].kind = CORBA::string_dup("");
			    TRACE1(LEVEL6,"Assembly: unregister Facet from naming service with the name is %s. \n",aName[0].id);
				nameServiceContextObj -> unbind(aName);
			}					
		}
		
	}
	catch(const CosNaming::NamingContext::NotFound& ex)
	{		
		switch(ex.why)
		{
			case CosNaming::NamingContext::missing_node:
						TRACE0(LEVEL1,"Assembly: unregiste from the NamingService failed for Got a `NotFound' exception (missing node). \n");
						break;

			case CosNaming::NamingContext::not_context:
						TRACE0(LEVEL1,"Assembly: unregiste from the NamingService failed for Got a `NotFound' exception (not context). \n");
						break;

			case CosNaming::NamingContext::not_object:
						TRACE0(LEVEL1,"Assembly: unregiste from the NamingService failed for Got a `NotFound' exception (not object). \n");
						break;
		}
		
		throw InvalidConfigureFile();
	}
	catch(const CosNaming::NamingContext::CannotProceed&)
	{
		TRACE0(LEVEL1,"Assembly: unregiste from the NamingService failed for Got a `CannotProceed' exception. \n");
		throw InvalidConfigureFile();
	}
	catch(const CosNaming::NamingContext::InvalidName&)
	{
		TRACE0(LEVEL1,"Assembly: unregiste from the NamingService failed for Got a `InvalidName' exception. \n");
		throw InvalidConfigureFile();
	}
	
	//
	//remove home
	//
	//modified by wsf 2004.7.21
	/*
	MapHomeInf::iterator iter=homeMap.begin();
	MapHomeInf::iterator iter_end=homeMap.end();
	
	while(iter!=iter_end)
	{
		Components::CCMHome_var home=(*iter).second.HomeRef;
		CORBA::Object_var containerObj=(*iter).second.ContainerInf.ref;
		Components::Deployment::Container_var container=Components::Deployment::Container::_narrow(containerObj);
		
		try
		{
			container->remove_home(home);
		}
		catch(const CORBA::SystemException &)
		{
			TRACE0(LEVEL1,"Assembly: tear down assembly failed in remove home.\n");
			throw ;				
		}		
		catch(const Components::RemoveFailure &)
		{
			TRACE0(LEVEL1,"Assembly: tear down assembly  failed in remove home.\n");
			throw Components::RemoveFailure();
		}		
		
		iter++;
	}
	*/
	ArchHomes_var ahomes = arch->getAllArchHomes();
	int i;
	for (i=0; i<ahomes->length(); i++)
	{
		CORBA::Object_var obj = ahomes[i].homeref;
		Components::CCMHome_var home = Components::CCMHome::_narrow(obj);
		obj = ahomes[i].containerref;
		Components::Deployment::Container_var container=Components::Deployment::Container::_narrow(obj);
		try
		{
			container->remove_home(home);
		}
		catch(const CORBA::SystemException &)
		{
			TRACE0(LEVEL1,"Assembly: tear down assembly failed in remove home.\n");
			throw ;				
		}		
		catch(const Components::RemoveFailure &)
		{
			TRACE0(LEVEL1,"Assembly: tear down assembly  failed in remove home.\n");
			throw Components::RemoveFailure();
		}
	}
	//end by wsf 2004.7.21
	


	//
	//remove the assembly corresponding information in the DeployDomainConf file.
	//

	
	try
	{
		DomainMgr->tearDownAssembly(assemblyUUID.in());
	}
	catch(const CORBA::SystemException &)
	{
		TRACE1(LEVEL1,"Assembly: tear down assembly %s failed in remove information from the DomainConf flle.\n",assemblyUUID.in());
		throw;
	}
	catch(...)
	{
		TRACE1(LEVEL1,"Assembly: tear down assembly %s failed in remove information from the DomainConf flle.\n",assemblyUUID.in());
		throw Components::RemoveFailure();
	}	


	//
	//remove container, if no assembly deployed on this container 
	//
	//modified by wsf 2004.7.21
	//iter=homeMap.begin();
	//while(iter!=iter_end)
	for (i=0; i<ahomes->length(); i++)
	{		
		//CORBA::String_var containerUUID=(*iter).second.ContainerInf.id;	
		//CORBA::Object_var containerObj=(*iter).second.ContainerInf.ref;
		CORBA::String_var containerUUID=ahomes[i].containerid;	
		CORBA::Object_var containerObj=ahomes[i].containerref;	
		Components::Deployment::Container_var container=Components::Deployment::Container::_narrow(containerObj);
		
		//
		//get the assembly id list which deployed on the container
		//
		STARCCM::Deployment::ObjIds_var idList;
		try
		{	
			idList=DomainMgr->getContainerAssemlbyIds(containerUUID.in());
		}
		catch(const CORBA::SystemException &)
		{
			TRACE0(LEVEL1,"Assembly: tear down assembly failed in getContainerAssemlbyIds  .\n");
			throw Components::RemoveFailure();			
		}
		catch(...)
		{
			TRACE0(LEVEL1,"Assembly: tear down assembly failed in getContainerAssemlbyIds  .\n");
			throw Components::RemoveFailure();
		}
		
		//
		//if the container deploy no assembly ,then remove the container
		//
		int len=idList->length();
		if(len==0)
		{
			//CORBA::Object_var componentServerObj=(* iter).second.ComponentServerInf.ref;
			CORBA::Object_var componentServerObj=ahomes[i].componentserverref;
			Components::Deployment::ComponentServer_var componentServer=Components::Deployment::ComponentServer::_narrow(componentServerObj);
			try
			{
				componentServer->remove_container(container);
			}
			catch(const CORBA::SystemException &)
			{
				TRACE0(LEVEL1,"Assembly: tear down assembly  failed in remove container.\n");
				throw ;				
			}
			catch(const Components::RemoveFailure &)
			{
				TRACE0(LEVEL1,"Assembly: tear down assembly failed in remove container.\n");
				throw Components::RemoveFailure();
			}					
		}
		
		//iter++;
	}	
	
	//
	//remove componentserver, if no Assembly deployed on this ComponentServer
	//
	//modified by wsf 2004.7.21
	//iter=homeMap.begin();
	//while(iter!=iter_end)
	for (i=0; i<ahomes->length(); i++)
	{		
		//CORBA::String_var componentServerUUID=(*iter).second.ComponentServerInf.id;	
		//CORBA::Object_var componentServerObj=(*iter).second.ComponentServerInf.ref;	
		CORBA::String_var componentServerUUID=ahomes[i].componentserverid;	
		CORBA::Object_var componentServerObj=ahomes[i].componentserverref;	
		Components::Deployment::ComponentServer_var componentServer=Components::Deployment::ComponentServer::_narrow(componentServerObj);
			
		//
		//get the assembly id list which deployed on the componentserver
		//
		STARCCM::Deployment::ObjIds_var idList;
		try
		{	
			idList=DomainMgr->getComponentServerAssemlbyIds(componentServerUUID.in());
		}
		catch(const CORBA::SystemException &)
		{
			TRACE0(LEVEL1,"Assembly: tear down assembly failed in getComponentServerAssemlbyIds  .\n");
			throw Components::RemoveFailure();			
		}
		catch(...)
		{
			TRACE0(LEVEL1,"Assembly: tear down assembly failed in getComponentServerAssemlbyIds  .\n");
			throw Components::RemoveFailure();
		}
		
		//
		//if the componentserver deploy no assembly ,then remove the componentserver
		//
		int len=idList->length();
		if(len==0)
		{
			//CORBA::Object_var serverActivatorObj=(* iter).second.ServerActivatorInf.ref;
			CORBA::Object_var serverActivatorObj=ahomes[i].serveractivatorref;
			Components::Deployment::ServerActivator_var serverActivator=Components::Deployment::ServerActivator::_narrow(serverActivatorObj);
			try
			{
				serverActivator->remove_component_server(componentServer);
			}
			catch(const CORBA::SystemException &)
			{
				TRACE0(LEVEL1,"Assembly: tear down assembly  failed in remove componentserver.\n");
				throw ;				
			}
			catch(const Components::RemoveFailure &)
			{
				TRACE0(LEVEL1,"Assembly: tear down assembly failed in remove componentserver.\n");
				throw Components::RemoveFailure();
			}					
		}
		
		//iter++;
	}

	//
	//clear the homeMap,componentMap,connectMap
	//
//	homeMap.clear();
	componentMap.clear();
//	connectMap.clear();

	//end by wsf 2004.7.21
}     

		
AssemblyState 
Assembly_impl::get_state()
	throw(CORBA::SystemException)
{
	return State;	
}
			  	

void 
Assembly_impl::recover()
        throw(Components::Deployment::RecoverFailure,
              	CORBA::SystemException)
{
	
	//left to be implemented
	if(verbose)
	{
		cout<<"Assembly:recover"<<endl;	
	}
}              	

void 
Assembly_impl::shutdown()
       	throw(Components::RemoveFailure,CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"Assembly: shut down"<<endl;
	}

	//
	//remove the assembly information completely from the DomainConf
	//
	try
	{
		DomainMgr->removeObject(ASSEMBLY,assemblyUUID.in());
	}
	catch(...)
	{
		TRACE0(LEVEL1,"Assembly: shutdown assembly failed in remove the information from the DomainConf file.\n");
		throw Components::RemoveFailure();
		
	}	

	//
	//shut down the ORB
	//
	orb -> shutdown(false);
	
}   


void
Assembly_impl::buildHomePlacement(XMLParser *parser,
					DOM_Node root,
					DOM_Node homeplacementNode)
	throw (InvalidConfigureFile, BuildHomePlacementFailure)
{
	
	//
	//get the homeplacement id
	//
	DOMString homeplacementId=parser->getAttr(homeplacementNode,"id");
	if(homeplacementId==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file homeplacementNode miss id attribute. \n");
		throw InvalidConfigureFile();					
	}
	
	
	//
	//get the reference of serveractivator 
	//
	
	//
	//get the ServerActivator IP
	//
	DOM_Node destinationNode=parser->findChild(homeplacementNode,"destination");
	if(destinationNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: detination Node missed.\n");
		throw InvalidConfigureFile();
	}
	
	DOM_Node serveractivatorNode=parser->findChild(destinationNode,"serveractivator");
	if(serveractivatorNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: serveractivator Node of detination missed.\n");
		throw InvalidConfigureFile();
	}
	
	DOMString serveractivatorId=parser->getAttr(serveractivatorNode ,"id");
	if(serveractivatorId==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: serveractivator Node  missed id attribute.\n");
		throw InvalidConfigureFile();		
	}
	
	
	//
	//get the corbaloc of the ServerActivator
	//
	
	CORBA::String_var serverActivatorRef;
	try
	{
		TRACE1(LEVEL5,"Assembly: get ServerActivator of id %s from the DomainConf.\n",serveractivatorId.transcode());
		serverActivatorRef=DomainMgr->getObject(SERVERACTIVATOR,serveractivatorId.transcode());
	}
	catch(const CORBA::SystemException &)
	{
		TRACE1(LEVEL1,"Assembly: get server activator object %s from DeployDomainMgr failed in SystemException.\n",serveractivatorId.transcode());
		throw BuildHomePlacementFailure();
	}
	catch(const STARCCM::Deployment::ObjectNotExist &)
	{
		TRACE1(LEVEL1,"Assembly: get server activator object %s from DeployDomainMgr failed in object not exist.\n",serveractivatorId.transcode());
		throw BuildHomePlacementFailure();		
	}
	catch(const STARCCM::Deployment::UnknownObjectType &)
	{
		TRACE1(LEVEL1,"Assembly: get server activator object %s from DeployDomainMgr failed in unknown object type.\n",serveractivatorId.transcode());
		throw BuildHomePlacementFailure();				
		
	}
	catch(...)
	{
		TRACE1(LEVEL1,"Assembly: get server activator object %s from DeployDomainMgr failed in invalid DomainConf File.\n",serveractivatorId.transcode());
		throw BuildHomePlacementFailure();				
	}
	
	//
	//get the ServerActivator ref from its corbaloc
	//
	TRACE1(LEVEL5,"Assembly: get the ServerActivatorRef of ServerActivator %s.\n",serverActivatorRef.in());
    	CORBA::Object_var serverActivatorObj;

	try
    	{
        	serverActivatorObj  = orb -> string_to_object(serverActivatorRef.in());
    	}
   	catch(const CORBA::BAD_PARAM&)
    	{
		TRACE0(LEVEL1,"Assembly: get server activator object failed in invalid ServerAcitvator corbaloc.\n");
		throw BuildHomePlacementFailure();    		
    	}

    	if(CORBA::is_nil(serverActivatorObj))
    	{
		TRACE0(LEVEL1,"Assembly: get server activator object failed in get ServerAcitvator ref from its corbaloc.\n");
		throw BuildHomePlacementFailure();
    	}
    
	
    	Components::Deployment::ServerActivator_var serverActivator=Components::Deployment::ServerActivator::_narrow(serverActivatorObj);
    	assert(!CORBA::is_nil(serverActivator));
	
	
	//
	//add the serveractivator information to the homeInfo
	//		 
	//deleted by wsf 2004.7.21
	/*
	HomeInf homeInformation;
	FacilityInf serverActivatorInformation;	
	
	serverActivatorInformation.id=CORBA::string_dup(serveractivatorId.transcode());
	serverActivatorInformation.ref=serverActivatorObj;
	
	homeInformation.ServerActivatorInf=serverActivatorInformation;
	*/
	
	//	
	//get componentserver conf:oaport ,id, ORB configvalues
	//
	TRACE0(LEVEL5,"Assembly: get componentserver conf.\n");
	DOM_Node componentserverNode=parser->findChild(destinationNode,"componentserver");
	if(componentserverNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentserver Node of detination missed.\n");
		throw InvalidConfigureFile();
	}
	
	DOMString componentserverId=parser->getAttr(componentserverNode ,"id");
	if(componentserverId==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentserver Node of destination missed ComponentServerId attribute.\n");
		throw InvalidConfigureFile();							
	}
	
	DOMString componentserverPort=parser->getAttr(componentserverNode ,"port");
	if(componentserverPort==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentserver Node missed port attribute.\n");
		throw InvalidConfigureFile();					
	}			

		
	//
	//create config values to create component server
	//
	TRACE0(LEVEL5,"Assembly: create config values to create component server.\n");
	ConfigValues_var componentServerCfValues=new ConfigValues;
	
	CORBA::String_var componentserverUUID=CORBA::string_dup(componentserverId.transcode());		
	CORBA::Any_var anyId=new CORBA::Any;
	anyId<<=componentserverUUID;
	
	CORBA::String_var portStr=CORBA::string_dup(componentserverPort.transcode());	
	CORBA::Any_var anyPort=new CORBA::Any;
	anyPort<<=portStr.in();
		
	ConfigValue_var cfValueId=new ConfigValue_impl("UUID",anyId.in());		
	ConfigValue_var cfValuePort=new ConfigValue_impl("Port",anyPort.in());
		
	componentServerCfValues->length(2);
	componentServerCfValues[(CORBA::ULong)0]=cfValueId;
	componentServerCfValues[(CORBA::ULong)1]=cfValuePort;	
	
 
	//
	//invoke the serveracitvator to create componentserver with config values
	//
	TRACE0(LEVEL4,"Assembly: invoke ServerActivator to create componentserver with config values.\n");
	ComponentServer_var componentServer;
	try
	{
		componentServer=serverActivator->create_component_server(componentServerCfValues);		
	}
	catch(const CORBA::SystemException &)
	{
		TRACE0(LEVEL1,"Assembly: create the componentserver failed in CORBA System Exception.\n");
		throw BuildHomePlacementFailure();
	}
	catch(const CreateFailure &)
	{
		TRACE0(LEVEL1,"Assembly: create the componentserver failed.\n");
		throw BuildHomePlacementFailure();
	}
	catch(const InvalidConfiguration &)
	{
		TRACE0(LEVEL1,"Assembly: create the componentserver failed in Invalid Configuration.\n");
		throw BuildHomePlacementFailure();		
	}
	//add jxh
	CORBA::String_var componentManagerLoc = CORBA::string_dup("corbaloc::");
	CORBA::String_var componentManagerIp = CORBA::string_dup(serveractivatorId.transcode());
	CORBA::String_var componentManagerPort;
	componentManagerPort = CORBA::string_dup(componentserverPort.transcode());
#ifdef STARCCMSTRINGADD
	componentManagerLoc = HelpFun::CORBA_string_add(componentManagerLoc, componentManagerIp);
	componentManagerLoc = HelpFun::CORBA_string_add(componentManagerLoc, CORBA::string_dup(":"));
	componentManagerLoc = HelpFun::CORBA_string_add(componentManagerLoc, componentManagerPort);
	componentManagerLoc = HelpFun::CORBA_string_add(componentManagerLoc, CORBA::String_dup("/ComponentManager"));
#else	
	componentManagerLoc+=componentManagerIp;
	componentManagerLoc+=":";
	componentManagerLoc+=componentManagerPort;
	componentManagerLoc+="/ComponentManager";
#endif
	//end
	
	//
	//add the componentserver information to the homeInfo
	//
	//deleted by wsf 2004.7.21
	/*
	FacilityInf componentServerInformation;
	componentServerInformation.id=componentserverUUID.in();	
	componentServerInformation.ref=Components::Deployment::ComponentServer::_duplicate(componentServer);
	homeInformation.ComponentServerInf=componentServerInformation;
	*/
	//end by wsf 2004.7.21	
	
	
	
	
	//
	//construct the create container configvalues
	//
	
	TRACE0(LEVEL5,"Assembly: construct the create container configvalues.\n");		
	ConfigValues_var containerCfValues=new ConfigValues;	
	int containerCfValuesLen=containerCfValues->length();	
	CORBA::Any_var anyCfValue=new CORBA::Any;
	
	//
	//get the container  UUID
	//
	
	TRACE0(LEVEL5,"Assembly: get the containerUUID.\n");
	DOM_Node containerNode=parser->findChild(destinationNode,"container");

	if(containerNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: container Node of destination missed .\n");
		throw InvalidConfigureFile();		
	}

	DOMString containerId=parser->getAttr(containerNode ,"id");	
	if(containerId==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: container Node missed id attribute.\n");
		throw InvalidConfigureFile();
	}
		
	CORBA::String_var containerUUID=CORBA::string_dup(containerId.transcode());
			
	anyCfValue<<=containerUUID;

	ConfigValue_var cfValue=new ConfigValue_impl("UUID",anyCfValue.in());	
	
	containerCfValues->length(containerCfValuesLen+1);
	containerCfValues[(CORBA::ULong)containerCfValuesLen]=cfValue;	
	containerCfValuesLen++;		

	
	//
	//invoke ComponentServer to create the container with the config values
	//
	TRACE0(LEVEL4,"Assembly: invoke ComponentServer to create the container with the config values.\n");
	Container_var container;
	try
	{
		container=componentServer->create_container(containerCfValues);
	}
	catch(const CORBA::SystemException &)
	{
//		TRACE1(LEVEL1,"Assembly: create the container failed in CORBA System Exception:%s\n",ex._to_string());
		throw BuildHomePlacementFailure();
	}
	catch(const CreateFailure &)
	{
		TRACE0(LEVEL1,"Assembly: create the container failed.\n");
		throw BuildHomePlacementFailure();
	}
	catch(const InvalidConfiguration &)
	{
		TRACE0(LEVEL1,"Assembly: create the container failed in Invalid Configuration.\n");
		throw InvalidConfigureFile();		
	}
	
	//
	//add the container information to the homeInfo
	//
	//deleted by wsf 2004.7.21
	/*
	FacilityInf containerInformation;
	containerInformation.id=containerUUID;
	containerInformation.ref=Components::Deployment::Container::_duplicate(container);
	homeInformation.ContainerInf=containerInformation;	
	*/
	//end by wsf 2004.7.21	
	
	
	
	//
	//construct the  installhome config values
	//
	TRACE0(LEVEL5,"Assembly: construct the  installhome config values.\n");	
	ConfigValues_var homeCfValues=new ConfigValues;
	int homeCfValuesLen=homeCfValues->length();
	
	//
	//add the naming service corbaloc
	//
	TRACE0(LEVEL5,"Assembly: get the naming service corbaloc.\n");
	DOM_Node nameserviceNode=parser->findChild(root,"namingservice");
	if(nameserviceNode!=NULL)
	{
		DOMString nameserviceLoc=parser->getAttr(nameserviceNode,"corbaloc");
		if(nameserviceLoc==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : naming service node miss the corbaloc attribute.\n");
			throw InvalidConfigureFile();		
		}	
		
		CORBA::String_var nameservieLocStr=CORBA::string_dup(nameserviceLoc.transcode());
		anyCfValue<<=nameservieLocStr.in();
	
		cfValue=new ConfigValue_impl("NameService",anyCfValue.in());
			
		homeCfValues->length(homeCfValuesLen+1);
		homeCfValues[(CORBA::ULong)homeCfValuesLen]=cfValue;	
		homeCfValuesLen++;
		
		//
		//get the nameServiceContextObj from the nameservice corbaloc
		//
		TRACE1(LEVEL6,"Assembly: get the nameServiceContext object from corbaloc %s.\n",nameservieLocStr.in());
		CORBA::Object_var obj;
		try
		{
			obj=orb->string_to_object(nameservieLocStr.in());		
		}
		catch(const CORBA::BAD_PARAM&)
		{
			TRACE1(LEVEL2,"Assembly: can not get the nameServiceContext object from the corbaloc %s.\n",nameservieLocStr.in());
			throw InvalidConfigureFile();
		}	
	
    if(CORBA::is_nil(obj))
    {
			TRACE1(LEVEL2,"Assembly: can not get the nameServiceContext object from the corbaloc %s.\n",nameservieLocStr.in());
			throw InvalidConfigureFile();
   	}
 	
    
    nameServiceContextObj = CosNaming::NamingContext::_narrow(obj);
    assert(!CORBA::is_nil(nameServiceContextObj));  
		
	}
	
	//
	//add the transaction service corbaloc
	//
	TRACE0(LEVEL5,"Assembly: get the transaction service corbaloc.\n");
	DOM_Node txnserviceNode=parser->findChild(root,"transactionservice");
	if(txnserviceNode != NULL)
	{
		DOMString txnserviceLoc=parser->getAttr(txnserviceNode,"corbaloc");	
		if(txnserviceLoc==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : transaction service node miss the corbaloc attribute.\n");
			throw InvalidConfigureFile();		
		}
		
		CORBA::String_var txnserviceLocStr=CORBA::string_dup(txnserviceLoc.transcode());	
		anyCfValue<<=txnserviceLocStr;
		
		cfValue=new ConfigValue_impl("TransactionService",anyCfValue.in());
		
		homeCfValues->length(homeCfValuesLen+1);
		homeCfValues[(CORBA::ULong)homeCfValuesLen]=cfValue;	
		homeCfValuesLen++;		
	}
	

	//
	//add the notification service corbaloc
	//
	TRACE0(LEVEL5,"Assembly: get the notification service corbaloc.\n");
	DOM_Node notifyserviceNode=parser->findChild(root,"notificationservice");
	if(notifyserviceNode != NULL)
	{
		DOMString notifyserviceLoc=parser->getAttr(notifyserviceNode,"corbaloc");	
		if(notifyserviceLoc==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : notification service node miss the corbaloc attribute.\n");
			throw InvalidConfigureFile();		
		}
		
		CORBA::String_var notifyserviceLocStr=CORBA::string_dup(notifyserviceLoc.transcode());	
		anyCfValue<<=notifyserviceLocStr;
		
		cfValue=new ConfigValue_impl("NotificationService",anyCfValue.in());	
		
		homeCfValues->length(homeCfValuesLen+1);
		homeCfValues[(CORBA::ULong)homeCfValuesLen]=cfValue;	
		homeCfValuesLen++;		
	}
	
	//
	//add the HomeFinder corbaloc
	//
	TRACE0(LEVEL5,"Assembly: get the HomeFinder corbaloc.\n");
	DOM_Node homeFinderNode=parser->findChild(root,"homefinder");
	if(homeFinderNode != NULL)
	{
		DOMString homeFinderLoc=parser->getAttr(homeFinderNode,"corbaloc");	
		if(homeFinderLoc==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : homefinder node miss the corbaloc attribute.\n");
			throw InvalidConfigureFile();		
		}
		
		CORBA::String_var homeFinderLocStr=CORBA::string_dup(homeFinderLoc.transcode());	
		anyCfValue<<=homeFinderLocStr;
		
		cfValue=new ConfigValue_impl("HomeFinder",anyCfValue.in());	
		
		homeCfValues->length(homeCfValuesLen+1);
		homeCfValues[(CORBA::ULong)homeCfValuesLen]=cfValue;	
		homeCfValuesLen++;		
	}

	//
	//add the HomeRegistration corbaloc
	//
	TRACE0(LEVEL5,"Assembly: get the HomeRegistration corbaloc.\n");
	DOM_Node homeRegistrationNode=parser->findChild(root,"homeregistration");
	if(homeRegistrationNode != NULL)
	{
		DOMString homeRegistrationLoc=parser->getAttr(homeRegistrationNode,"corbaloc");	
		if(homeRegistrationLoc==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : homeregistration node miss the corbaloc attribute.\n");
			throw InvalidConfigureFile();		
		}
		
		CORBA::String_var homeRegistrationLocStr=CORBA::string_dup(homeRegistrationLoc.transcode());	
		anyCfValue<<=homeRegistrationLocStr;
		
		cfValue=new ConfigValue_impl("HomeRegistration",anyCfValue.in());	
		
		homeCfValues->length(homeCfValuesLen+1);
		homeCfValues[(CORBA::ULong)homeCfValuesLen]=cfValue;	
		homeCfValuesLen++;		
		

		TRACE1(LEVEL6,"Assembly: get the HomeRegistration object from corbaloc %s.\n",homeRegistrationLocStr.in());
		CORBA::Object_var obj;
		try
		{
			obj=orb->string_to_object(homeRegistrationLocStr.in());		
		}
		catch(const CORBA::BAD_PARAM&)
		{
			TRACE1(LEVEL2,"Assembly: can not get the HomeRegistration object from the corbaloc %s.\n",homeRegistrationLocStr.in());
			throw InvalidConfigureFile();
		}	
	
    if(CORBA::is_nil(obj))
    {
			TRACE1(LEVEL2,"Assembly: can not get the HomeRegistration object from the corbaloc %s.\n",homeRegistrationLocStr.in());
			throw InvalidConfigureFile();
   	}
 	
 	//modified by wsf
 	//homeRegistrationObj = HomeRegistration::_narrow(obj);
    	homeRegistrationObj = STARCCM::HomeRegistration::_narrow(obj);
 	//end by wsf
   	 assert(!CORBA::is_nil(homeRegistrationObj));    	

	}	

	//
	//add the ResourcePoolName name
	//
	TRACE0(LEVEL5,"Assembly: get the ResourcePoolName name.\n");
	DOM_Node resourcePoolNameNode=parser->findChild(root,"resourcepoolname");
	if(resourcePoolNameNode != NULL)
	{
		DOMString resourcePoolNameName=parser->getAttr(resourcePoolNameNode,"name");	
		if(resourcePoolNameName==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : ResourcePoolName node miss the name attribute.\n");
			throw InvalidConfigureFile();		
		}
		
		CORBA::String_var resourcePoolNameStr=CORBA::string_dup(resourcePoolNameName.transcode());	
		anyCfValue<<=resourcePoolNameStr;
		
		cfValue=new ConfigValue_impl("ResourcePoolName",anyCfValue.in());	
		
		homeCfValues->length(homeCfValuesLen+1);
		homeCfValues[(CORBA::ULong)homeCfValuesLen]=cfValue;	
		homeCfValuesLen++;		
	}	

	//
	//add the CreateResourcePool 
	//
	TRACE0(LEVEL5,"Assembly: get the CreateResourcePool.\n");
	DOM_Node createResourcePoolNode=parser->findChild(root,"createresourcepool");
	if(createResourcePoolNode != NULL)
	{
		CORBA::StringSeq_var createResourcePoolInfo = new CORBA::StringSeq;
		int createResourcePoolInfoLen=createResourcePoolInfo->length();
		
		//
		//add name
		//
		DOMString resourcePoolName=parser->getAttr(createResourcePoolNode,"name");	
		if(resourcePoolName==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : CreateResourcePool node miss the name attribute.\n");
			throw InvalidConfigureFile();		
		}
		CORBA::String_var resourcePoolNameStr=CORBA::string_dup(resourcePoolName.transcode());
		
		createResourcePoolInfo->length(createResourcePoolInfoLen+1);
		createResourcePoolInfo[(CORBA::ULong)createResourcePoolInfoLen]=resourcePoolNameStr;		
		createResourcePoolInfoLen++;
		
		//
		//add host
		//
		DOMString resourcePoolHost=parser->getAttr(createResourcePoolNode,"host");	
		if(resourcePoolHost==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : CreateResourcePool node miss the host attribute.\n");
			throw InvalidConfigureFile();		
		}
		CORBA::String_var resourcePoolHostStr=CORBA::string_dup(resourcePoolHost.transcode());
		
		createResourcePoolInfo->length(createResourcePoolInfoLen+1);
		createResourcePoolInfo[(CORBA::ULong)createResourcePoolInfoLen]=resourcePoolHostStr;		
		createResourcePoolInfoLen++;

		//
		//add database
		//
		DOMString resourcePoolDataBase=parser->getAttr(createResourcePoolNode,"database");	
		if(resourcePoolDataBase==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : CreateResourcePool node miss the database attribute.\n");
			throw InvalidConfigureFile();		
		}
		CORBA::String_var resourcePoolDataBaseStr=CORBA::string_dup(resourcePoolDataBase.transcode());
		
		createResourcePoolInfo->length(createResourcePoolInfoLen+1);
		createResourcePoolInfo[(CORBA::ULong)createResourcePoolInfoLen]=resourcePoolDataBaseStr;		
		createResourcePoolInfoLen++;

		//
		//add user
		//
		DOMString resourcePoolUser=parser->getAttr(createResourcePoolNode,"user");	
		if(resourcePoolUser==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : CreateResourcePool node miss the user attribute.\n");
			throw InvalidConfigureFile();		
		}
		CORBA::String_var resourcePoolUserStr=CORBA::string_dup(resourcePoolUser.transcode());
		
		createResourcePoolInfo->length(createResourcePoolInfoLen+1);
		createResourcePoolInfo[(CORBA::ULong)createResourcePoolInfoLen]=resourcePoolUserStr;		
		createResourcePoolInfoLen++;

		//
		//add password
		//
		DOMString resourcePoolPassword=parser->getAttr(createResourcePoolNode,"password");	
		if(resourcePoolPassword==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : CreateResourcePool node miss the password attribute.\n");
			throw InvalidConfigureFile();		
		}
		CORBA::String_var resourcePoolPasswordStr=CORBA::string_dup(resourcePoolPassword.transcode());
		
		createResourcePoolInfo->length(createResourcePoolInfoLen+1);
		createResourcePoolInfo[(CORBA::ULong)createResourcePoolInfoLen]=resourcePoolPasswordStr;		
		createResourcePoolInfoLen++;
		
		//
		//add initialconnections
		//
		DOMString resourcePoolInitialConnections=parser->getAttr(createResourcePoolNode,"initialconnections");	
		if(resourcePoolInitialConnections==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : CreateResourcePool node miss the initialconnections attribute.\n");
			throw InvalidConfigureFile();		
		}
		CORBA::String_var resourcePoolInitialConnectionsStr=CORBA::string_dup(resourcePoolInitialConnections.transcode());
		
		createResourcePoolInfo->length(createResourcePoolInfoLen+1);
		createResourcePoolInfo[(CORBA::ULong)createResourcePoolInfoLen]=resourcePoolInitialConnectionsStr;		
		createResourcePoolInfoLen++;

		//
		//add maxconnections
		//
		DOMString resourcePoolMaxConnections=parser->getAttr(createResourcePoolNode,"maxconnections");	
		if(resourcePoolMaxConnections==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : CreateResourcePool node miss the maxconnections attribute.\n");
			throw InvalidConfigureFile();		
		}
		CORBA::String_var resourcePoolMaxConnectionsStr=CORBA::string_dup(resourcePoolMaxConnections.transcode());
		
		createResourcePoolInfo->length(createResourcePoolInfoLen+1);
		createResourcePoolInfo[(CORBA::ULong)createResourcePoolInfoLen]=resourcePoolMaxConnectionsStr;		
		createResourcePoolInfoLen++;

		//
		//add increment
		//
		DOMString resourcePoolIncrement=parser->getAttr(createResourcePoolNode,"increment");	
		if(resourcePoolIncrement==NULL)
		{
			TRACE0(LEVEL2,"Assembly: invalid cad file : CreateResourcePool node miss the increment attribute.\n");
			throw InvalidConfigureFile();		
		}
		CORBA::String_var resourcePoolIncrementStr=CORBA::string_dup(resourcePoolIncrement.transcode());
		
		createResourcePoolInfo->length(createResourcePoolInfoLen+1);
		createResourcePoolInfo[(CORBA::ULong)createResourcePoolInfoLen]=resourcePoolIncrementStr;												

		
		anyCfValue<<=createResourcePoolInfo.in();		
		cfValue=new ConfigValue_impl("CreateResourcePool",anyCfValue.in());					
		
		homeCfValues->length(homeCfValuesLen+1);
		homeCfValues[(CORBA::ULong)homeCfValuesLen]=cfValue;	
		homeCfValuesLen++;		
	}	
	
	//
	//locate the implUUID
	//	
	DOM_Node componentImplRefNode=parser->findChild(homeplacementNode,"componentimplref");
	if(componentImplRefNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentimplref node missed.\n");
		throw InvalidConfigureFile();
	}
	DOMString componentImplRef=parser->getAttr(componentImplRefNode,"idref");	
	if(componentImplRef==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentimplref node miss idref attribute.\n");
		throw InvalidConfigureFile();		
	}	

	//add by wsf 2004.6.23
	ConfigValues_var dynCfValues = new ConfigValues;
	//end by wsf 2004.6.23		
	TRACE0(LEVEL5,"Assembly: construct installhome configvalues using getInstallHomeCfValues.\n");
	try
	{
		getInstallHomeCfValues(parser,root,homeplacementNode,componentImplRef.transcode(),homeCfValues.inout(),dynCfValues.inout());		
	}
	catch(const InvalidConfiguration &)
	{
		TRACE0(LEVEL1,"Assembly: create the container failed in get install home ConfigValues.\n");
		throw InvalidConfigureFile();			
	}
	
	//
	//add for debug
	//
	TRACE0(LEVEL4,"Assembly: display the install home  configvalue.\n");
	for(int counter= 0;counter<homeCfValues->length();counter++)
	{
		TRACE2(LEVEL6,"Assembly: the install home ConfigValues %d value is %s.\n",counter,homeCfValues[(CORBA::ULong)counter]->name() );
	}
	
	//
	//get the homeproperties and componentproperties file from ccd out of the cfValues
	//the the homeproperties and componentproperties file confiValue is the last two of the cfValues
	//
	TRACE0(LEVEL5,"Assembly: get the homeproperties and componentproperties file from ccd out of the cfValues.\n");
	const char *ccdPropFileName;
	CORBA::String_var ccdHomePropFileName;
	CORBA::String_var ccdComponentPropFileName;	

	
	//
	//test whether the last two is the homeproperties and componentproperties file 
	//
	homeCfValuesLen = homeCfValues->length();
	int location=0;
	int i = 1;
	for (i=1;i<3;i++)
	{
//		cfValue = homeCfValues[(CORBA::ULong)(homeCfValuesLen - i)];
		
//		if(strcmp(cfValue->name(),"HomePropertyFile")==0)	   
		if(strcmp(homeCfValues[(CORBA::ULong)(homeCfValuesLen-i)]->name(),"HomePropertyFile")==0)	   
		{
			
			(homeCfValues[(CORBA::ULong)(homeCfValuesLen-i)]->value())>>=ccdPropFileName;
			
			ccdHomePropFileName=CORBA::string_dup(ccdPropFileName);
			
			location=i;		
		}
		else if(strcmp(homeCfValues[(CORBA::ULong)(homeCfValuesLen-i)]->name(),"ComponentPropertyFile")==0)		
		{
			
			(homeCfValues[(CORBA::ULong)(homeCfValuesLen-i)]->value())>>=ccdPropFileName;
			
			ccdComponentPropFileName=CORBA::string_dup(ccdPropFileName);				
			
			location=i;
		}
	}
	
	//
	//truncate the homeCfValues ,get out of the property file
	//
	
	homeCfValues->length(homeCfValuesLen-location);
	
	//
	//get the ConfigurationComplete value
	//
	const char * setConfigurationComplete;
	int j;
	for(j=0;j<homeCfValues->length();j++)
	{
		if(strcmp(homeCfValues[(CORBA::ULong)j]->name(),"ConfigurationComplete")==0)
		{
			(homeCfValues[(CORBA::ULong)j]->value())>>=setConfigurationComplete;
			
		}
	}
	
	
	//
	//get the ref of FileAccessorFactory
	//
	CORBA::String_var fileAccessorRef;
	try
	{
		TRACE0(LEVEL5,"Assembly: get FileAccessorFactory of DomainConf.\n");
		fileAccessorRef=DomainMgr->getObject(FILEACCESSORFACTORY,"");
	}
	catch(const CORBA::SystemException &)
	{
		TRACE1(LEVEL1,"Assembly: get fileAccessorRef corbaloc from DeployDomainMgr failed in SystemException.\n",serveractivatorId.transcode());
		throw BuildHomePlacementFailure();
	}
	catch(const STARCCM::Deployment::ObjectNotExist &)
	{
		TRACE1(LEVEL1,"Assembly: get fileAccessorRef corbaloc from DeployDomainMgr failed  in object not exist.\n",serveractivatorId.transcode());
		throw BuildHomePlacementFailure();		
	}
	catch(const STARCCM::Deployment::UnknownObjectType &)
	{
		TRACE1(LEVEL1,"Assembly: get fileAccessorRef corbaloc from DeployDomainMgr failed  in unknown object type.\n",serveractivatorId.transcode());
		throw BuildHomePlacementFailure();				
		
	}
	catch(...)
	{
		TRACE1(LEVEL1,"Assembly: get fileAccessorRef corbaloc from DeployDomainMgr failed  in invalid DomainConf File.\n",serveractivatorId.transcode());
		throw BuildHomePlacementFailure();				
	}

   	CORBA::Object_var fileAccessorObj;

	try
    	{
        	fileAccessorObj  = orb -> string_to_object(fileAccessorRef.in());
    	}
   	catch(const CORBA::BAD_PARAM&)
    	{
		TRACE0(LEVEL1,"Assembly: get fileAccessor object failed in invalid fileAccessor corbaloc.\n");
		throw BuildHomePlacementFailure();    		
    	}

    	if(CORBA::is_nil(fileAccessorObj))
    	{
		TRACE0(LEVEL1,"Assembly: get fileAccessor object failed in get fileAccessor ref from its corbaloc.\n");
		throw BuildHomePlacementFailure();
    	}
    
	
    	STARCCM::Deployment::FileAccessorFactory_var  fileaccessorfactory=
    		STARCCM::Deployment::FileAccessorFactory::_narrow(fileAccessorObj);
    	
    	assert(!CORBA::is_nil(fileaccessorfactory));	
	
	//
	//add the fileaccessorfactory to the configValues
	//
#ifdef TAO
	CORBA::Any fileAccessorValue;
	fileAccessorValue<<=fileaccessorfactory;
	ConfigValue_var cfValueFileAccessor=new ConfigValue_impl("FileAccessorFactory",fileAccessorValue);
#else
	CORBA::Any_var fileAccessorValue=new CORBA::Any;
	fileAccessorValue<<=fileaccessorfactory;
	ConfigValue_var cfValueFileAccessor=new ConfigValue_impl("FileAccessorFactory",fileAccessorValue.in());	
#endif		
		
	homeCfValuesLen = homeCfValues->length();
	homeCfValues->length(homeCfValuesLen+1);
	homeCfValues[(CORBA::ULong)homeCfValuesLen]=cfValueFileAccessor;
	
	
	//
	//install home
	//
	TRACE0(LEVEL4,"Assembly: install home.\n");
	TRACE0(LEVEL4,"Assembly: display the install home  configvalue.\n");
		for(int homeCfValuesCounter= 0;homeCfValuesCounter<homeCfValues->length();homeCfValuesCounter++)
	{
		TRACE2(LEVEL6,"Assembly: the install home ConfigValues %d value is %s.\n",homeCfValuesCounter,homeCfValues[(CORBA::ULong)homeCfValuesCounter]->name() );
	}
	
	CCMHome_var home;
	try
	{
		home=container->install_home(componentImplRef.transcode(),"",homeCfValues.in());
		TRACE1(LEVEL6,"Assembly: install home of component implementation %s.\n",componentImplRef.transcode());
	}
	catch(const CORBA::SystemException & ex)
	{
		TRACE0(LEVEL1,"Assembly: install home failed in  CORBA::SystemException\n");
		//
		//add by doulei for debug
		//
		cerr<<ex;
		throw BuildHomePlacementFailure();
	}
	catch(const UnknownImplId &)
	{
		TRACE1(LEVEL1,"Assembly: install home failed in  invalid implUUID %s\n",componentImplRef.transcode());
		throw BuildHomePlacementFailure();		
	}
	catch(const ImplEntryPointNotFound &)
	{
		TRACE0(LEVEL1,"Assembly: install home failed in  ImplEntryPointNotFound. \n");
		throw BuildHomePlacementFailure();				
	}
	catch(const InstallationFailure &)
	{
		TRACE0(LEVEL1,"Assembly: install home failed in  InstallationFailure. \n");
		throw BuildHomePlacementFailure();						
	}
	catch(const InvalidConfiguration &)
	{
		TRACE0(LEVEL1,"Assembly: install home failed in  InvalidConfiguration. \n");
		throw InvalidConfigureFile();		
	}
	
	//
	//add the home information into the homeInfo
	//
	//deleted by wsf 2004.7.21
	//homeInformation.HomeRef=home;

	//add by wsf 2004.6.23

	//
	//add according ArchHome info to arch object
	//
	ArchHome_var ahome = container->getArchHome();
	ahome->id = CORBA::string_dup(homeplacementId.transcode());
	ahome->serveractivatorid = CORBA::string_dup(serveractivatorId.transcode());
	ahome->serveractivatorref = CORBA::Object::_duplicate(serverActivator);
	ahome->componentserverid = CORBA::string_dup(componentserverId.transcode());
	ahome->componentserverref = CORBA::Object::_duplicate(componentServer);

	
	//find the dynmic deployment info of homeplacement in cad file
	DOM_Node boundableNode=parser->findChild(homeplacementNode,"boundable");
	if (boundableNode !=NULL)
	{
		DOMString boundableset = parser->getAttr(boundableNode, "set");
		if (boundableset == NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid cad file boundable miss set attribute. \n");
			throw InvalidConfigureFile();
		}
		else
		{
			if (strcmp("true",boundableset.transcode()) == 0)
				ahome->isboundable = CORBA::string_dup("true");
			else
				ahome->isboundable = CORBA::string_dup("false");
		}
	}

	DOM_Node componentboundaryportsNode = parser->findChild(homeplacementNode, "componentboundaryports");
	if (componentboundaryportsNode != NULL)
	{
		DOM_NodeList childrenNodes = componentboundaryportsNode.getChildNodes();
        	unsigned int childrenLen=childrenNodes.getLength();
	
		DOM_Node boundaryportNode;
		DOMString boundaryportNodeName;	
		DOMString boundaryportName;	

		PortNameSeq_var tmpseq = new PortNameSeq;
		int len = 0;
		for(unsigned int iterator=0;iterator<childrenLen;iterator++)
		{
			boundaryportNode = childrenNodes.item(iterator);
			boundaryportNodeName = boundaryportNode.getNodeName();
			if (boundaryportNodeName.equals("boundaryport"))
			{
				boundaryportName = parser->getAttr(boundaryportNode,"name");
				if (boundaryportName == NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid cad file boundaryport miss name attribute. \n");
					throw InvalidConfigureFile();
				}
				tmpseq->length(len+1);
				tmpseq[len++] = CORBA::string_dup(boundaryportName.transcode());
			}
		}
		if (len > 0)
			ahome->componentboundaryports = tmpseq;
	}

	//get register info
	ahome->homefinder = CORBA::Object::_duplicate(homeRegistrationObj);

	if (registerWithHomeFinders != NULL)
	{
		int rlen = registerWithHomeFinders->length();
		int hlen = 0;
		int i = 0;
		for (i=0; i<rlen; i++)
		{
			ahome->withhomefinder.length(hlen+1);
			ahome->withhomefinder[hlen] = registerWithHomeFinders[i];
			hlen++;
		}
	}


	ahome->nameservice = CORBA::Object::_duplicate(nameServiceContextObj);
	if ( registerWithNamings != NULL)
	{
		int rlen = registerWithNamings->length();
		int hlen = 0;
		i = 0;
		for (i=0; i<rlen; i++)
		{
			ahome->withnaming.length(hlen+1);
			ahome->withnaming [hlen] = registerWithNamings[i];
			hlen++;
		}
	}

	//get other info that get from ccd file
	if (strcmp(setConfigurationComplete,"true") == 0)
		ahome->configurationcomplete = CORBA::string_dup("true");
	else
		ahome->configurationcomplete = CORBA::string_dup("false");
	
	int len = dynCfValues->length();
	for (i=0; i<len; i++)
	{
		if (strcmp(dynCfValues[(CORBA::ULong)i]->name(),"homeinitiative")==0)	
		{
			const char * set;
			(dynCfValues[(CORBA::ULong)i]->value())>>=set;
			if (strcmp(set,"true") == 0)
				ahome->homeinitiative = CORBA::string_dup("true");
			else
				ahome->homeinitiative = CORBA::string_dup("false");
		}

		if (strcmp(dynCfValues[(CORBA::ULong)i]->name(),"componentinitiative")==0)
		{
			const char* set;
			(dynCfValues[(CORBA::ULong)i]->value())>>=set;
			if (strcmp(set,"true") == 0)
				ahome->componentinitiative = CORBA::string_dup("true");
			else
				ahome->componentinitiative = CORBA::string_dup("false");
		}
		
		if(strcmp(dynCfValues[(CORBA::ULong)i]->name(),"runcomplete")==0)	
		{
			const char* set;
			(dynCfValues[(CORBA::ULong)i]->value())>>=set;
			if (strcmp(set,"true") == 0)
				ahome->runcomplete = CORBA::string_dup("true");
			else
				ahome->runcomplete = CORBA::string_dup("false");
		}

		if(strcmp(dynCfValues[(CORBA::ULong)i]->name(),"runcompleteusesport")==0)	
		{
			const char* portname;
			(dynCfValues[(CORBA::ULong)i]->value())>>=portname;
			int plen= (ahome->runcompleteusesports).length();
			(ahome->runcompleteusesports).length(plen+1);
			ahome->runcompleteusesports[plen] = CORBA::string_dup(portname);
		}

		if(strcmp(dynCfValues[(CORBA::ULong)i]->name(),"configurationcompleteusesport")==0)	
		{
			const char* portname;
			(dynCfValues[(CORBA::ULong)i]->value())>>=portname;
			int plen= (ahome->configurationcompleteusesports).length();
			(ahome->configurationcompleteusesports).length(plen+1);
			ahome->configurationcompleteusesports[plen] = CORBA::string_dup(portname);
		}

		if (strcmp(dynCfValues[(CORBA::ULong)i]->name(),"initialrequestport")==0)	
		{
			const char* portname;
			(dynCfValues[(CORBA::ULong)i]->value())>>=portname;
			int plen= (ahome->initialreqports).length();
			(ahome->initialreqports).length(plen+1);
			ahome->initialreqports[plen] = CORBA::string_dup(portname);
		}

		if (strcmp(dynCfValues[(CORBA::ULong)i]->name(),"crypticconnection")==0)
		{
			const CrypticConnection* conn;
			(dynCfValues[(CORBA::ULong)i]->value())>>=conn;
			int clen = (ahome->crypticconnections).length();
			(ahome->crypticconnections).length(clen+1);
			ahome->crypticconnections[clen] = *conn;
			
		}
	}
	
	arch->addArchHome(ahome);

	//end by wsf 2004.6.23

	//end by wsf
	
	//
	//get homeproperties to configure home
	//
	TRACE0(LEVEL4,"Assembly: get homeproperties to configure home.\n");	
	CORBA::String_var ccdPropFile=rootDir;

#ifdef STARCCMSTRINGADD
	ccdPropFile = HelpFun::CORBA_string_add(ccdPropFile, PATH_DELILIMITOR);
#else	
	ccdPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
#endif	
		
	CORBA::String_var cadPropFile=ccdPropFile;

#ifdef STARCCMSTRINGADD
	ccdPropFile = HelpFun::CORBA_string_add(ccdPropFile, CORBA::string_dup(componentImplRef.transcode()));
	ccdPropFile = HelpFun::CORBA_string_add(ccdPropFile, PATH_DELILIMITOR);
	ccdPropFile = HelpFun::CORBA_string_add(ccdPropFile, assemblyUUID);
	ccdPropFile = HelpFun::CORBA_string_add(ccdPropFile, PATH_DELILIMITOR);
#else		
	ccdPropFile+=CORBA::string_dup(componentImplRef.transcode());
	ccdPropFile+=CORBA::string_dup(PATH_DELILIMITOR);		
	cadPropFile+=assemblyUUID;
	ccdPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
#endif	
	
	DOM_Node homePropNode=parser->findChild(homeplacementNode,"homeproperties");
	if(homePropNode!=NULL)
	{
		DOMString fileName=parser->parseFileInArchive(homePropNode);
		if(fileName==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid cad file homeproperties miss fileinarchive node. \n");
			throw InvalidConfigureFile();			
		}
		
		CORBA::String_var cadHomePropFileName=CORBA::string_dup(fileName.transcode());
		
		//
		//configure home, some problem how to change the CCMHome to CCMObject
		//

/*		
		cadPropFile+=cadHomePropFileName;
			
		try
		{			
			if(ccdHomePropFileName==NULL)
			{
				configHome("",cadPropFile.in(),home);				
			}
			else
			{
				ccdPropFile+=ccdHomePropFileName;
				
				configHome(ccdPropFile.in(),cadPropFile.in(),home);
			}

		}
		catch(const CORBA::SystemException&)
		{
			TRACE1(LEVEL1,"Assembly: configure home failed in CORBA SystemException. \n");
			throw BuildHomePlacementFailure();								
		}
		catch(const InvalidConfigureFile &)
		{
			TRACE1(LEVEL1,"Assembly: configure home failed in InvalidConfigureFile. \n");
			throw InvalidConfigureFile();				
		}
		catch(const ConfigureFailure &)
		{
			TRACE1(LEVEL1,"Assembly: configure home failed in ConfigureFailure. \n");
			throw BuildHomePlacementFailure();								
		}		
*/	
	}
	else
	{
		//
		//configure home, some problem how to change the CCMHome to CCMObject
		//
/*		
		if(ccdHomePropFileName!=NULL)
		{
			ccdPropFile+=ccdHomePropFileName;	
		
			try
			{
				configHome(ccdPropFile.in(),"",home);				
			}
			catch(const CORBA::SystemException&)
			{
				TRACE1(LEVEL1,"Assembly: configure home failed in CORBA SystemException. \n");
				throw BuildHomePlacementFailure();								
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE1(LEVEL1,"Assembly: configure home failed in InvalidConfigureFile. \n");
				throw InvalidConfigureFile();				
			}
			catch(const ConfigureFailure &)
			{
				TRACE1(LEVEL1,"Assembly: configure home failed in ConfigureFailure. \n");
				throw BuildHomePlacementFailure();								
			}	
		}
*/				
	}



	//
	//get home configuration properties and set home configurator
	//
	TRACE0(LEVEL4,"Assembly: get home configuration properties and set home configurator.\n");
	cadPropFile=rootDir;

#ifdef STARCCMSTRINGADD
	cadPropFile = HelpFun::CORBA_string_add(cadPropFile, PATH_DELILIMITOR);
	cadPropFile = HelpFun::CORBA_string_add(cadPropFile, assemblyUUID);
	cadPropFile = HelpFun::CORBA_string_add(cadPropFile, PATH_DELILIMITOR);
#else	
	cadPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
	cadPropFile+=assemblyUUID;
	cadPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
#endif
	
	DOM_Node homeComponentPropNode=parser->findChild(homeplacementNode,"componentproperties");
	if(homeComponentPropNode!=NULL)
	{
		DOMString fileName=parser->parseFileInArchive(homeComponentPropNode);
		if(fileName==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid cad file componentproperties miss fileinarchive node. \n");
			throw InvalidConfigureFile();			
		}
		
		CORBA::String_var cadHomeComponentPropFileName=CORBA::string_dup(fileName.transcode());

#ifdef STARCCMSTRINGADD
		cadPropFile = HelpFun::CORBA_string_add(cadPropFile, cadHomeComponentPropFileName);
#else		
		cadPropFile+=cadHomeComponentPropFileName;
#endif		
	
		try
		{
			configHomeComponent(cadPropFile.in(),home,homeplacementId.transcode());
		}
		catch(const CORBA::SystemException&)
		{
			TRACE0(LEVEL1,"Assembly: configure home failed in CORBA SystemException. \n");
			throw BuildHomePlacementFailure();								
		}
		catch(const InvalidConfigureFile &)
		{
			TRACE0(LEVEL1,"Assembly: configure home failed in InvalidConfigureFile. \n");
			throw InvalidConfigureFile();				
		}
		catch(const ConfigureFailure &)
		{
			TRACE0(LEVEL1,"Assembly: configure home failed in ConfigureFailure. \n");
			throw BuildHomePlacementFailure();								
		}	
		
	}

	//
	//get the registration information from the homeCfValues and register the home 
	//
	CORBA::String_var homeRepId;
	CORBA::String_var componentRepId;
	
	
	for(j=0;j<homeCfValues->length();j++)
	{
		if(strcmp(homeCfValues[(CORBA::ULong)j]->name(),"HomeRepositoryId")==0)
		{
			const char * repId;	
			(homeCfValues[(CORBA::ULong)j]->value())>>=repId;
			homeRepId = CORBA::string_dup(repId);			
		}
		else if(strcmp(homeCfValues[(CORBA::ULong)j]->name(),"ComponentRepositoryId")==0)
		{
			const char * repId;	
			(homeCfValues[(CORBA::ULong)j]->value())>>=repId;
			componentRepId = CORBA::string_dup(repId);
		}
		else if(strcmp(homeCfValues[(CORBA::ULong)j]->name(),"HomeRegisterWithHomeFinders")==0)
		{
			const CORBA::StringSeq * registerInfos;	
			(homeCfValues[(CORBA::ULong)j]->value())>>=registerInfos;			
			registerWithHomeFinders = new CORBA::StringSeq(*registerInfos);
		}		
		else if(strcmp(homeCfValues[(CORBA::ULong)j]->name(),"HomeRegisterWithNamings")==0)
		{
			const CORBA::StringSeq * registerInfos;	
			(homeCfValues[(CORBA::ULong)j]->value())>>=registerInfos;			
			registerWithNamings = new CORBA::StringSeq(*registerInfos);
		}		
		else if(strcmp(homeCfValues[(CORBA::ULong)j]->name(),"RegisterComponents")==0)
		{
			const CORBA::StringSeq * registerInfos;	
			(homeCfValues[(CORBA::ULong)j]->value())>>=registerInfos;			
			registerComponent = new CORBA::StringSeq(*registerInfos);
		}		
		else if(strcmp(homeCfValues[(CORBA::ULong)j]->name(),"RegisterFacetNames")==0)
		{
			const CORBA::StringSeq * registerInfos;	
			(homeCfValues[(CORBA::ULong)j]->value())>>=registerInfos;
			registerFacetName = new CORBA::StringSeq(*registerInfos);
		}			
		else if(strcmp(homeCfValues[(CORBA::ULong)j]->name(),"FacetRegisterNames")==0)
		{
			const CORBA::StringSeq * registerInfos;	
			(homeCfValues[(CORBA::ULong)j]->value())>>=registerInfos;			
			facetRegisterName = new CORBA::StringSeq(*registerInfos);
		}								
	}
	
	//
	//validate these registration variable
	//
	try
	{
		//
		//register the home into the homefinder
		//
		
		//deleted by wsf
		//homeRegistrationObj->register_home_by_component_type(componentRepId.in(),home.in());
		//homeRegistrationObj->register_home_by_home_type(homeRepId.in(),home.in());
		//end by wsf
		
		if(registerWithHomeFinders != NULL)
		{
			for(int j=0;j<registerWithHomeFinders->length();j++)
			{			
				homeRegistrationObj->register_home_by_name(registerWithHomeFinders[(CORBA::ULong)j],home.in());			
			}
		}

		
		//
		//register the home into the namingservice
		//
		if(registerWithNamings != NULL)
		{
			for(int j=0;j<registerWithNamings->length();j++)			
			{	
				TRACE1(LEVEL6,"Assembly: registerWithNamings length is %d. \n",registerWithNamings->length());
				CosNaming::Name aName;
				aName.length(1);
				aName[0].id = CORBA::string_dup(registerWithNamings[(CORBA::ULong)j]);
				TRACE1(LEVEL6,"Assembly: name is %d. \n",aName[0].id);
		    aName[0].kind = CORBA::string_dup("");
				nameServiceContextObj -> bind(aName, home.in());
			}
		}
	}
	catch(const STARCCM::HomeHaveRegistered & /*e*/)
	{
		TRACE0(LEVEL1,"Assembly: registe the home into the HomeRegistration failed for HomeHaveRegistered. \n");
		throw InvalidConfigureFile();
	}

	catch(const CosNaming::NamingContext::NotFound& ex)
	{		
		switch(ex.why)
		{
			case CosNaming::NamingContext::missing_node:
						TRACE0(LEVEL1,"Assembly: registe the home into the NamingService failed for Got a `NotFound' exception (missing node). \n");
						break;

			case CosNaming::NamingContext::not_context:
						TRACE0(LEVEL1,"Assembly: registe the home into the NamingService failed for Got a `NotFound' exception (not context). \n");
						break;

			case CosNaming::NamingContext::not_object:
						TRACE0(LEVEL1,"Assembly: registe the home into the NamingService failed for Got a `NotFound' exception (not object). \n");
						break;
		}
		
		throw InvalidConfigureFile();
	}
	catch(const CosNaming::NamingContext::CannotProceed&)
	{
		TRACE0(LEVEL1,"Assembly: registe the home into the NamingService failed for Got a `CannotProceed' exception. \n");
		throw InvalidConfigureFile();
	}
	catch(const CosNaming::NamingContext::InvalidName&)
	{
		TRACE0(LEVEL1,"Assembly: registe the home into the NamingService failed for Got a `InvalidName' exception. \n");
		throw InvalidConfigureFile();
	}
	catch(const CosNaming::NamingContext::AlreadyBound&)
	{
		TRACE0(LEVEL1,"Assembly: registe the home into the NamingService failed for Got a `AlreadyBound' exception. \n");
		throw InvalidConfigureFile();
	}	

	//add by jxh
	CORBA::Object_var ComponentManagerObj ;
	try
    	{
        	cout<<componentManagerLoc<<endl;
        	ComponentManagerObj  = orb -> string_to_object(componentManagerLoc.in());
    	}
   	catch(const CORBA::BAD_PARAM&)
    	{
		TRACE0(LEVEL1,"Assembly: get ComponentManger object failed in invalid ComponentManger corbaloc.\n");
		throw BuildHomePlacementFailure();    		
    	}

    	if(CORBA::is_nil(ComponentManagerObj))
    	{
		TRACE0(LEVEL1,"Assembly: get ComponentManger object failed in get ComponentManger ref from its corbaloc.\n");
		throw BuildHomePlacementFailure();
    	}
    	
	ManagerDomain::ComponentManager_var componentManager = ManagerDomain::ComponentManager::_narrow(ComponentManagerObj);
	assert(!CORBA::is_nil(componentManager));
	//end
	
	//
	//get the componentinstantiation and process it one by one 
	//	
	TRACE0(LEVEL5,"Assembly: get the componentinstantiation and process it one by one .\n");
	DOM_NodeList componentInstantiationChildren=homeplacementNode.getChildNodes();
        unsigned int componentInstantiationChildrenLen=componentInstantiationChildren.getLength();
	
	DOM_Node componentInstantiationChild;
	DOMString componentInstantiationChildName;	
	
	for(unsigned int iterator=0;iterator<componentInstantiationChildrenLen;iterator++)
	{
		componentInstantiationChild=componentInstantiationChildren.item(iterator);
		componentInstantiationChildName=componentInstantiationChild.getNodeName();		
		
		if(componentInstantiationChildName.equals("componentinstantiation"))
		{			
			//
			//get the component instantiation Id
			//
			DOMString componentinstantiationId=parser->getAttr(componentInstantiationChild,"id");
			if(componentinstantiationId==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid cad file : componentinstantiation node miss id attribute . \n");
				throw InvalidConfigureFile();										
			}	
			
			//add by wsf
			//
			//judge whether the componentinstantiationId existed
			//
			MapComponentInf::iterator iter=componentMap.begin();
			MapComponentInf::iterator iter_end=componentMap.end();
			while(iter!=iter_end)
			{
				CORBA::String_var id=CORBA::string_dup((* iter).first.c_str());
				
				if(strcmp(id.in(),componentinstantiationId.transcode())==0)
				{
					if(verbose)
					{
						cout<<"Assembly: componentinstantiation Id: " <<componentinstantiationId.transcode() << "has existed."<<endl;			
					}		
					throw InvalidConfigureFile();
					
				}	
				iter++;
			}		
			//end by wsf
			
			TRACE1(LEVEL1,"Assembly: processing componentInstantiation %s. \n",componentinstantiationId.transcode());
			
			//
			//get the Component Property file, the primary key will be spedified here
			//
			CORBA::String_var componentInstantiationPropFile=rootDir;

#ifdef STARCCMSTRINGADD
			componentInstantiationPropFile = HelpFun::CORBA_string_add(componentInstantiationPropFile, PATH_DELILIMITOR);
			componentInstantiationPropFile = HelpFun::CORBA_string_add(componentInstantiationPropFile, assemblyUUID);
			componentInstantiationPropFile = HelpFun::CORBA_string_add(componentInstantiationPropFile, PATH_DELILIMITOR
					 ;
					 
#else			
			componentInstantiationPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
			componentInstantiationPropFile+=assemblyUUID;
			componentInstantiationPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
#endif			
			
			DOM_Node componentpropertiesNode=parser->findChild(componentInstantiationChild,"componentproperties");
			if(componentpropertiesNode!=NULL)
			{
				DOMString componentInstantiationPropFileName=parser->parseFileInArchive(componentpropertiesNode);
				if(componentInstantiationPropFileName==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid cad file : invalid fileinarchive node of componentproperties  node. \n");
					throw InvalidConfigureFile();						
				}

#ifdef STARCCMSTRINGADD
				componentInstantiationPropFile = HelpFun::CORBA_string_add(componentInstantiationPropFile, CORBA::string_dup(componentInstantiationPropFileName.transcode()));
#else				
				componentInstantiationPropFile+=CORBA::string_dup(componentInstantiationPropFileName.transcode());
#endif				
				
			}
			
			//
			//get the primary key from the property file
			//
			ConfigValue_var pkValue;
			if(componentpropertiesNode!=NULL)
			{
				try
				{
					pkValue=getPrimaryKeyProp(componentInstantiationPropFile.in());
	
				}
				catch(const InvalidConfigureFile &)
				{
					TRACE1(LEVEL1,"Assembly: invalid cad file : get primary key from property file %s failed. \n",componentInstantiationPropFile.in());
					throw InvalidConfigureFile();														
				}
			}
				
			bool pkSpecified=true;
			if(pkValue== NULL)
			{
				pkSpecified=false;
			}
			
			//
			//create component ,different from whether the component have pk or not
			//
			TRACE0(LEVEL4,"Assembly: create component.\n");
			Components::CCMObject_var componentObj;
			if(!pkSpecified)
			{				
				Components::KeylessCCMHome_var keylessHome=Components::KeylessCCMHome::_narrow(home);
				if(CORBA::is_nil(keylessHome))
				{
					TRACE0(LEVEL6,"Assembly: keyless home is NULL.\n");
					throw BuildHomePlacementFailure();
				}
				
				try
				{					
					TRACE0(LEVEL6,"Assembly: keyless home create component.\n");

					componentObj=keylessHome->create_component();	
									
					
					//add by jxh
					componentManager->RegisterComponent(homeRepId.in(),componentinstantiationId.transcode(),componentObj.in());
					CORBA::String_var s = orb->object_to_string(componentObj);
					
	                                TRACE1(LEVEL6,"component ref is %s.\n",s);
	                                if(componentObj->_non_existent())
	                                {
	                                	TRACE0(LEVEL6,"component obj is not exist");
	                                }
	                                else
	                                {
	                                	TRACE0(LEVEL6,"component obj is exist");
	                                }
	                               
	                                //end
				}
				catch(const CORBA::SystemException &)
				{
					TRACE0(LEVEL1,"Assembly:  create component failed in CORBA SystemException. \n");
					throw BuildHomePlacementFailure();											
				}
				catch(const Components::CreateFailure &)
				{
					TRACE0(LEVEL1,"Assembly: create component failed in CreateFailure. \n");
					throw BuildHomePlacementFailure();						
				}	

				if(CORBA::is_nil(componentObj))
				{
					TRACE0(LEVEL6,"Assembly: the component created by keyless home is NULL.\n");
					throw BuildHomePlacementFailure();
				}				
				
			}
			else
			{
				
				//
				//construct the DII request parameter and invoke create operation
				//
				
				CORBA::Context_var ctx;
				orb ->get_default_context(ctx.out());
				
				CORBA::NamedValue_var result;
				orb ->create_named_value(result.out());
//				CORBA::Any& outValue = *(result->value());
//				outValue.replace(Components::_tc_CCMObject, 0);
				
				CORBA::ExceptionList_var exceptions;
				orb ->create_exception_list(exceptions.out());
				exceptions -> add(Components::_tc_CreateFailure);
				exceptions -> add(Components::_tc_DuplicateKeyValue);
				exceptions -> add(Components::_tc_InvalidKey);
				
				CORBA::ContextList_var cl;
				orb ->create_context_list(cl.out());
				
				CORBA::NVList_var argList;
				orb ->create_list(0,argList);				
				CORBA::NamedValue_ptr arg;			
				arg = argList ->add(CORBA::ARG_IN);				
				*(arg ->value()) = pkValue->value();
				
				CORBA::Request_var request;	

				home->_create_request(ctx,"create",argList,result,exceptions,cl,request,0);
				
				request ->invoke();	
				
#ifdef TAO
				if( (request -> exceptions()) != 0)							
#else		
				if( (request -> env() -> exception()) != 0)							
#endif	
				{					
					//
					//judge the exception type and print out the error msg
					//
					TRACE0(LEVEL1,"Assembly: create component failed . \n");
					throw BuildHomePlacementFailure();						
					
				}
				else
				{
					request -> return_value() >>= componentObj;
					//add by jxh
					componentManager->RegisterComponent(homeRepId.in(),componentinstantiationId.transcode(),componentObj.in());
					CORBA::String_var s = orb->object_to_string(componentObj);
					
	                                TRACE1(LEVEL6,"component ref is %s.\n",s);
	                                if(componentObj->_non_existent())
	                                {
	                                	TRACE0(LEVEL6,"component obj is not exist");
	                                }
	                                else
	                                {
	                                	TRACE0(LEVEL6,"component obj is exist");
	                                }
	                               
	                                //end						
				}
				
			}
		
		
			//
			//configure the component
			//
			TRACE0(LEVEL4,"Assembly: configure the component.\n");
			try
			{
				if(componentpropertiesNode!=NULL)
				{			
					
					if(ccdComponentPropFileName!=NULL)
					{
						CORBA::String_var ccdComponentPropFile=rootDir;

#ifdef STARCCMSTRINGADD
						ccdComponentPropFile = HelpFun::CORBA_string_add(ccdComponentPropFile, PATH_DELILIMITOR);
						
						ccdComponentPropFile = HelpFun::CORBA_string_add(ccdComponentPropFile, CORBA::string_dup(componentImplRef.transcode()));
						ccdComponentPropFile = HelpFun::CORBA_string_add(ccdComponentPropFile, PATH_DELILIM
								TOR);
						
						ccdComponentPropFile = HelpFun::CORBA_string_add(ccdComponentPropFile, ccdComponentPropFileName);
#else						
						ccdComponentPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
						ccdComponentPropFile+=CORBA::string_dup(componentImplRef.transcode());
						ccdComponentPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
						ccdComponentPropFile+=ccdComponentPropFileName; 
#endif						
						
						TRACE1(LEVEL6,"Assembly: the cad property fils is %s.\n",componentInstantiationPropFile.in());
						TRACE1(LEVEL6,"Assembly: the ccd property fils is %s.\n",ccdComponentPropFile.in());
						configComponent(ccdComponentPropFile.in(),componentInstantiationPropFile.in(),componentObj.in());
						TRACE2(LEVEL6,"Assembly: configComponent with ccd file %s and cad file %s.\n",ccdComponentPropFile.in(),componentInstantiationPropFile.in());
					}
					else
					{
						
						configComponent("",componentInstantiationPropFile.in(),componentObj.in());
						TRACE1(LEVEL6,"Assembly: configComponent with cad file %s.\n",componentInstantiationPropFile.in());						
					}
				}
				else
				{
					if(ccdComponentPropFileName!=NULL)
					{
						CORBA::String_var ccdComponentPropFile=rootDir;

#ifdef STARCCMSTRINGADD
						ccdComponentPropFile = HelpFun::CORBA_string_add(ccdComponentPropFile, PATH_DELILIMITOR);
						
						ccdComponentPropFile = HelpFun::CORBA_string_add(ccdComponentPropFile, CORBA::string_dup(componentImplRef.transcode()));
						 ccdComponentPropFile = HelpFun::CORBA_string_add(ccdComponentPropFile, PATH_DELILIM
								 TOR);
						 
						ccdComponentPropFile = HelpFun::CORBA_string_add(ccdComponentPropFile, ccdComponentPropFileName);
#else						
						ccdComponentPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
						ccdComponentPropFile+=CORBA::string_dup(componentImplRef.transcode());
						ccdComponentPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
						ccdComponentPropFile+=ccdComponentPropFileName;
#endif						

						configComponent(ccdComponentPropFile.in(),"",componentObj.in());
						TRACE1(LEVEL6,"Assembly: configComponent with ccd file %s.\n",ccdComponentPropFile.in());												
					}				
				}
			}
			catch(const CORBA::SystemException &)
			{
				TRACE0(LEVEL1,"Assembly: configure the CCMObject failed in CORBA SystemException . \n");
				throw BuildHomePlacementFailure();									
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE0(LEVEL1,"Assembly:configure the CCMObject failed in InvalidConfigureFile . \n");
				throw InvalidConfigureFile();									
			}
			catch(const ConfigureFailure &)
			{
				TRACE0(LEVEL1,"Assembly:configure the CCMObject failed in ConfigureFailure . \n");
				throw BuildHomePlacementFailure();									
			}
			
			//
			//register the component and its facets
			//			
			try
			{
				//
				//register the Component into the NamingService
				//
				TRACE0(LEVEL5,"Assembly: registerComponent. \n");			
				if(registerComponentId != NULL)
				{					
					for(int j=0;j<registerComponentId->length();j++)
					{
						if(strcmp(registerComponentId[(CORBA::ULong)j],componentinstantiationId.transcode()) == 0)
						{
							CosNaming::Name aName;						
							aName.length(1);
							aName[0].id = CORBA::string_dup(registerComponent[(CORBA::ULong)j]);
					    aName[0].kind = CORBA::string_dup("");
					    
					    TRACE1(LEVEL6,"Assembly: registerComponent name is %s. \n",aName[0].id);
							nameServiceContextObj -> bind(aName, componentObj.in());
						}
					}
				}

				//
				//register the facet into the namingservice
				//
				TRACE0(LEVEL5,"Assembly: registerFacet. \n");				
				if(registerFacetName != NULL)
				{	
					
					for(int j=0;j<registerFacetName->length();j++)
					{
						TRACE1(LEVEL6,"Assembly: registerFacet length is %d. \n",registerFacetName->length());
						
						CORBA::Object_var facetObj = componentObj->provide_facet(registerFacetName[(CORBA::ULong)j]);
						
						CosNaming::Name aName;
						aName.length(1);
						aName[0].id = CORBA::string_dup(facetRegisterName[(CORBA::ULong)j]);
				    aName[0].kind = CORBA::string_dup("");
				    TRACE1(LEVEL6,"Assembly: registerFacet name is %s. \n",aName[0].id);
						nameServiceContextObj -> bind(aName, facetObj.in());
					}					
				}	
			}			
			catch(const CosNaming::NamingContext::NotFound& ex)
			{        
				switch(ex.why)
				{
					case CosNaming::NamingContext::missing_node:
								TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `NotFound' exception (missing node). \n");
								break;
		
					case CosNaming::NamingContext::not_context:
								TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `NotFound' exception (not context). \n");
								break;
		
					case CosNaming::NamingContext::not_object:
								TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `NotFound' exception (not object). \n");
								break;
				}
				
				throw InvalidConfigureFile();
			}
			catch(const CosNaming::NamingContext::CannotProceed&)
			{
				TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `CannotProceed' exception. \n");
				throw InvalidConfigureFile();
			}
			catch(const CosNaming::NamingContext::InvalidName&)
			{
				TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `InvalidName' exception. \n");
				throw InvalidConfigureFile();
			}
			catch(const CosNaming::NamingContext::AlreadyBound&)
			{
				TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `AlreadyBound' exception. \n");
				throw InvalidConfigureFile();
			}	
	
			//add by wsf

			//
			//add the according archcomponent info to the arch object
			//

			ArchComponent_var acom = container->getArchCom();
			acom->id = CORBA::string_dup(componentinstantiationId.transcode());

			//2004.6.23
			//find the dynmic deployment info of componentinstantiation in cad file
			 boundableNode=parser->findChild(componentInstantiationChild,"boundable");
			if (boundableNode !=NULL)
			{
				DOMString boundableset = parser->getAttr(boundableNode, "set");
				if (boundableset == NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid cad file boundable miss set attribute. \n");
					throw InvalidConfigureFile();
				}
				else
				{
					if (strcmp("true",boundableset.transcode()) == 0)
						acom->isboundable = CORBA::string_dup("true");
					else
						acom->isboundable = CORBA::string_dup("false");
				}
			}

			DOM_Node boundaryportsNode = parser->findChild(componentInstantiationChild, "boundaryports");
			if (boundaryportsNode != NULL)
			{
				DOM_NodeList childrenNodes = boundaryportsNode.getChildNodes();
		        	unsigned int childrenLen=childrenNodes.getLength();
			
				DOM_Node boundaryportNode;
				DOMString boundaryportNodeName;	
				DOMString boundaryportName;	

				PortNameSeq_var tmpseq = new PortNameSeq;
				int len = 0;
				for(unsigned int iterator=0;iterator<childrenLen;iterator++)
				{
					boundaryportNode = childrenNodes.item(iterator);
					boundaryportNodeName = boundaryportNode.getNodeName();
					if (boundaryportNodeName.equals("boundaryport"))
					{
						boundaryportName = parser->getAttr(boundaryportNode,"name");
						if (boundaryportName == NULL)
						{
							TRACE0(LEVEL1,"Assembly: invalid cad file boundaryport miss name attribute. \n");
							throw InvalidConfigureFile();
						}
						tmpseq->length(len+1);
						tmpseq[len++] = CORBA::string_dup(boundaryportName.transcode());
					}
				}
				if (len > 0)
					acom->boundaryports = tmpseq;
			}

			//get register info
			acom->nameservice = CORBA::Object::_duplicate(nameServiceContextObj);
			if (registerComponentId != NULL)
			{
				int len = registerComponentId->length();
				RegisterNameSeq_var tmpseq = new RegisterNameSeq;
				int tlen = 0;
				for (i=0; i<len; i++)
				{
					if (strcmp(registerComponentId[i], componentinstantiationId.transcode()) ==0)
					{
						tmpseq->length(tlen+1);
						tmpseq[tlen++] = registerComponent[i];
					}
						
				}
				if (tlen > 0)
					acom->withnaming = tmpseq;
			}

			if (registerFacetName != NULL)
			{
				int len = registerFacetName->length();
				PortWithNamingSeq_var portseq = new PortWithNamingSeq;
				int plen = 0;
				int tlen = 0;
				for (i=0; i<len; i++)
				{
					int j;
					for (j=0; j<plen; j++)
					{
						if (strcmp(registerFacetName[i], portseq[j].portname) == 0)
							break;
					}
					
					if (j<plen)
					{
						tlen = portseq[j].withnaming.length();
						portseq[j].withnaming.length(tlen+1);
						portseq[j].withnaming[tlen] = facetRegisterName[i];
					}
					else
					{
						portseq->length(plen+1);
						portseq[plen].portname = registerFacetName[i];
						portseq[plen].withnaming.length(1);
						portseq[plen].withnaming[0] = facetRegisterName[i];	
						plen++;
					}


				}
				if (plen > 0)
					acom->portwithnamings = portseq;
			}

			
			arch->addArchComponent(acom);	
			//end 2004.6.23

			//end by wsf		
			
			//
			//add the ComponentInfo into ComponentMap
			//
			TRACE0(LEVEL5,"Assembly: add the ComponentInfo into ComponentMap. \n");
			ComponentInf componentInformation;
			componentInformation.ComponentRef=componentObj;
			componentInformation.ConfigurationComplete=false;
			CORBA::StringSeq_var dependList=new CORBA::StringSeq;
			componentInformation.depend=dependList;	
			if(strcmp(setConfigurationComplete,"true")==0)
			{			
				componentInformation.SetConfigurationComplete=true;
			}
			else
			{
				componentInformation.SetConfigurationComplete=false;
			}

			componentMap.insert(MapComponentInf::value_type(string(componentinstantiationId.transcode()),componentInformation));
		}
			
	}
	
	//
	//add the homeInfo into homeMap
	//
	//deleted by wsf 2004.7.21
	//TRACE0(LEVEL5,"Assembly: add the homeInfo into ComponentMap. \n");
	//homeMap.insert(MapHomeInf::value_type(string(homeplacementId.transcode()),homeInformation));
	
	//
	//add the register information into registerNamings used to unregister from the NamingService 
	//		
	TRACE0(LEVEL5,"Assembly: add the register information into registerNamings. \n");	
	int registerNamingsLen = registerNamings->length();
	
	TRACE0(LEVEL5,"Assembly: add registerWithNamings. \n");	
	if(registerWithNamings != NULL)
	{
		for(int j=0;j<registerWithNamings->length();j++)
		{
			registerNamings->length(registerNamingsLen+1);
			registerNamings[(CORBA::ULong)registerNamingsLen] = registerWithNamings[(CORBA::ULong)j];
			registerNamingsLen++;
		}
	}
	
	TRACE0(LEVEL5,"Assembly: add registerComponent. \n");
	if(registerComponent != NULL)
	{
		for(int j=0;j<registerComponent->length();j++)
		{
			registerNamings->length(registerNamingsLen+1);
			registerNamings[(CORBA::ULong)registerNamingsLen] = registerComponent[(CORBA::ULong)j];
			registerNamingsLen++;
		}
	}
	
	TRACE0(LEVEL5,"Assembly: add facetRegisterName. \n");
	if(facetRegisterName != NULL)
	{
		for(int j=0;j<facetRegisterName->length();j++)
		{
			registerNamings->length(registerNamingsLen+1);
			registerNamings[(CORBA::ULong)registerNamingsLen] = facetRegisterName[(CORBA::ULong)j];
			registerNamingsLen++;
		}		
	}

	TRACE0(LEVEL5,"Assembly: end of buildhomeplacement. \n");
}

void
Assembly_impl::buildProcessCollocation(XMLParser *parser,
 					DOM_Node root,
					DOM_Node processcollocationNode)
		throw (InvalidConfigureFile, BuildProcessCollocationFailure)					
{
	DOM_NodeList processcollocationChildren=processcollocationNode.getChildNodes();
        unsigned int processcollocationChildrenLen=processcollocationChildren.getLength();
	
	DOM_Node processcollocationChild;
	DOMString processcollocationChildName;	
	
	for(unsigned int iterator=0;iterator<processcollocationChildrenLen;iterator++)
	{
	
		processcollocationChild=processcollocationChildren.item(iterator);
		processcollocationChildName=processcollocationChild.getNodeName();	
		
		if(processcollocationChildName.equals("homeplacement"))
		{
			try
			{				
				buildHomePlacement(parser,root,processcollocationChild);				
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE0(LEVEL1,"Assembly: build process collocation failed in invalid configuration file.\n");
				throw;
			}
			catch(const BuildHomePlacementFailure &)
			{
				TRACE0(LEVEL1,"Assembly: build process collocation failed in invalid configuration file.\n");
				throw BuildProcessCollocationFailure();				
			}
			catch(const CORBA::SystemException &)
			{
				TRACE0(LEVEL1,"Assembly: build process collocation failed in invalid configuration file.\n");
				throw BuildProcessCollocationFailure();				
			}			
		}
	}	
	
}

void
Assembly_impl::buildHostCollocation(XMLParser *parser,
 					DOM_Node root,
					DOM_Node hostcollocationNode)
		throw (InvalidConfigureFile, BuildHostCollocationFailure)
{
	
	DOM_NodeList hostcollocationChildren=hostcollocationNode.getChildNodes();
        unsigned int hostcollocationChildrenLen=hostcollocationChildren.getLength();
	
	DOM_Node hostcollocationChild;
	DOMString hostcollocationChildName;	
	
	for(unsigned int iterator=0;iterator<hostcollocationChildrenLen;iterator++)
	{
	
		hostcollocationChild=hostcollocationChildren.item(iterator);
		hostcollocationChildName=hostcollocationChild.getNodeName();
		
		if(hostcollocationChildName.equals("homeplacement"))
		{
			try
			{
				buildHomePlacement(parser,root,hostcollocationChild);
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE0(LEVEL1,"Assembly: build host collocation failed in invalid configuration file.\n");
				throw;
			}
			catch(const BuildHomePlacementFailure &)
			{
				TRACE0(LEVEL1,"Assembly: build host collocation failed in invalid configuration file.\n");
				throw BuildHostCollocationFailure();				
			}
		}
		else if(hostcollocationChildName.equals("processcollocation"))
		{
			try
			{
				buildProcessCollocation(parser,root,hostcollocationChild);
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE0(LEVEL1,"Assembly: build host collocation failed in invalid configuration file.\n");
				throw;
			}
			catch(const BuildProcessCollocationFailure &)
			{
				TRACE0(LEVEL1,"Assembly: build host collocation failed in invalid configuration file.\n");
				throw BuildHostCollocationFailure();				
			}

		}
	}
		
	
}

//wsf
void 
Assembly_impl::dynBuildHomePlacement(XMLParser *parser_,DOM_Node root_,DOM_Node homeplacementNode)
				throw (InvalidConfigureFile, BuildHomePlacementFailure)
{
	//
	//get the homeplacement id
	//
	DOMString homeplacementId=parser_->getAttr(homeplacementNode,"id");
	if(homeplacementId==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file homeplacementNode miss id attribute. \n");
		throw InvalidConfigureFile();					
	}
	
	bool homeexisted = false;
	CCMHome_var home;
	
	//
	//judge whether the homeplacementId has existed.
	//

	ArchHome* ahome = arch->getArchHome(homeplacementId.transcode());
	if (ahome != NULL)
	{
		if(verbose)
		{
			cout<<"Assembly: the homeplacement Id has existed."<<endl;			
		}	
		homeexisted = true;
		CORBA::Object_var obj = CORBA::Object::_duplicate(ahome->homeref);
		home = Components::CCMHome::_narrow(obj);
		delete ahome;
		ahome = NULL;
	}
	
	//modified
	/*
	MapHomeInf::iterator iter=homeMap.begin();
	MapHomeInf::iterator iter_end=homeMap.end();
	while(iter!=iter_end)
	{
		CORBA::String_var id=CORBA::string_dup((* iter).first.c_str());
		
		if(strcmp(id.in(),homeplacementId.transcode())==0)
		{
			if(verbose)
			{
				cout<<"Assembly: the homeplacement Id has existed."<<endl;			
			}	
			homeexisted = true;
			home = Components::CCMHome::_duplicate((*iter).second.HomeRef);
			break;
		}	
		iter++;
	}	
	*/
	//end modified

	if (homeexisted == false)
	{
		try
		{
			buildHomePlacement(parser_,root_,homeplacementNode);
		}
		catch(...)
		{
			throw;
		}
		return;
	}
	

	//To get information for create componentinstantiations

	//get componentImplRef
	DOM_Node componentimplrefNode = parser_ -> findChild(homeplacementNode,"componentimplref");
	DOMString componentImplRef = parser_ -> getAttr(componentimplrefNode, "idref");	

	//get componentFileRef
	DOM_Node componentfilerefNode = parser_ -> findChild(homeplacementNode,"componentimplref");
	DOMString componentFileRef = parser_  -> getAttr(componentimplrefNode, "idref");	

	//
	//get the HomeRegistration and namingservice obj.
	//
	registerComponentId=new CORBA::StringSeq;
	registerComponent=new CORBA::StringSeq;	
	registerFacetName=new CORBA::StringSeq;	
	facetRegisterName=new CORBA::StringSeq;	

	DOM_NodeList children=homeplacementNode.getChildNodes();
	unsigned int childrenLen=children.getLength();	

	DOM_Node child;
	DOMString childName;
	
	cout << "Assembly:get the HomeRegistration and namingservice information ,the length is " << childrenLen << endl;
	
	for(unsigned int iterator=0;iterator<childrenLen;iterator++)
	{
		child=children.item(iterator);
		childName=child.getNodeName();	
		
		if(childName.equals("componentinstantiation"))
		{
			TRACE0(LEVEL5,"Assembly: enter componentinstantiation .\n");
			
			DOM_NodeList componentInstChildren=child.getChildNodes();
			unsigned int componentInstChildrenLen=componentInstChildren.getLength();	

			for(unsigned int i=0;i<componentInstChildrenLen;i++)
			{
				DOM_Node componentInstChild=componentInstChildren.item(i);
				DOMString componentInstChildName=componentInstChild.getNodeName();	
			
				DOMString componentInstId=parser_ ->getAttr(child,"id");
				if(componentInstId==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid cad file: componentinstantiation miss id attribute .\n");
					throw InvalidConfigureFile();					
				}
				
				if(componentInstChildName.equals("registercomponent"))
				{
					DOM_Node providesidentifierNode=parser_ ->findChild(componentInstChild,"providesidentifier");
					if(providesidentifierNode==NULL)
					{						
						//
						//add register component information
						//
			            
			            TRACE0(LEVEL5,"Assembly: enter registercomponent .\n");
			                
						DOM_NodeList registercomponentChildren=componentInstChild.getChildNodes();
						unsigned int registercomponentChildrenLen=registercomponentChildren.getLength();							
									
						for(unsigned int i=0;i<registercomponentChildrenLen;i++)
						{
							DOM_Node registercomponentChild=registercomponentChildren.item(i);
							DOMString registercomponentChildName=registercomponentChild.getNodeName();	
							
							if(registercomponentChildName.equals("registerwithnaming"))
							{								
								DOMString name=parser_ ->getAttr(registercomponentChild,"name");
								if(name==NULL)
								{
									TRACE0(LEVEL1,"Assembly: invalid cad file: registerwithnaming miss name attribute .\n");
									throw InvalidConfigureFile();					
								}
																							
								int len=registerComponentId->length();
								
								registerComponentId->length(len+1);
								registerComponentId[len]=CORBA::string_dup(componentInstId.transcode());	
								
								registerComponent->length(len+1);
								registerComponent[len]=CORBA::string_dup(name.transcode());	
								
								TRACE2(LEVEL6,"Assembly: get registerwithnaming of %s for component instantiation %s.\n",name.transcode(),componentInstId.transcode());												
							}
						}						
					}
					else
					{
						//
						//add register facet information
						//
						TRACE0(LEVEL5,"Assembly: enter register facet information .\n");
						
						DOM_Node  providesIdentifierText=providesidentifierNode.getFirstChild();				
						DOMString providesIdentifier=providesIdentifierText.getNodeValue();
						
						DOM_NodeList registercomponentChildren=componentInstChild.getChildNodes();
						unsigned int registercomponentChildrenLen=registercomponentChildren.getLength();	
						
						for(unsigned int i=0;i<childrenLen;i++)
						{
							DOM_Node registercomponentChild=registercomponentChildren.item(i);
							DOMString registercomponentChildName=registercomponentChild.getNodeName();	
							
							if(registercomponentChildName.equals("registerwithnaming"))
							{
			
								DOMString name=parser_ ->getAttr(registercomponentChild,"name");
								if(name==NULL)
								{
									TRACE0(LEVEL1,"Assembly: invalid cad file: registerwithnaming miss name attribute .\n");
									throw InvalidConfigureFile();					
								}						
								
								int len=registerFacetName->length();
								registerFacetName->length(len+1);
								registerFacetName[len]=CORBA::string_dup(providesIdentifier.transcode());			
								
								facetRegisterName->length(len+1);
								facetRegisterName[len]=CORBA::string_dup(name.transcode());			
												
							}
						}
					}
				}					
			}
		}	
	}	

	//
	//locate the csd file
	//		
	TRACE0(LEVEL5,"Assembly: locate the csd file.\n");
	CORBA::String_var csdFile;
	try
	{
		csdFile=getCsdFile(parser_ ,root_ ,homeplacementNode);
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE0(LEVEL1,"Assembly: get csd file failed in invalid cad file.\n");
		throw InvalidConfigureFile();		
	}
	
	//
	//XMLParser initialized
	//
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(csdFile);
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: XMLParser initialized failed for csd file %s. \n",csdFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();
	}    

  	//
	//locate the csd file path: rootDir\assemblyUUID\componentId
	//
	
	CORBA::String_var csdFileDir=rootDir;
	csdFileDir+=CORBA::string_dup(PATH_DELILIMITOR);
	csdFileDir+=assemblyUUID;
	csdFileDir+=CORBA::string_dup(PATH_DELILIMITOR);
	csdFileDir+=CORBA::string_dup(componentFileRef.transcode());
	csdFileDir+=CORBA::string_dup(PATH_DELILIMITOR);
	
	DOM_Node root = parser->getRootNode("softpkg");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: Invalid csd file %s : have no softpkg element. \n",csdFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw InvalidConfigureFile();
	}

	CORBA::String_var ccdFile;	
	try
	{
		ccdFile=getCcdFile(parser,root,componentImplRef.transcode(),csdFileDir.in());		
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE1(LEVEL1,"Assembly: get the ccd file failed in invalid csd file %s.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
			
		throw InvalidConfigureFile();
	}	

	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}	

	//paser ccdfile
	//
	//XMLParser initialized
	//
  	parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(ccdFile);
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: XMLParser initialized failed for csd file %s. \n",ccdFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();
	}    
	
	root = parser->getRootNode("corbacomponent");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: Invalid ccd file %s : have no corbacomponent element. \n",ccdFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw InvalidConfigureFile();
	}	

	CORBA::String_var ccdComponentPropFileName;
	DOM_Node componentpropertiesNode=parser->findChild(root,"componentproperties");
	if(componentpropertiesNode!=NULL)
	{	
		DOMString componentpropertiesFile=parser->parseFileInArchive(componentpropertiesNode);
		if(componentpropertiesFile==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: invalid file in archive node of componentproperties.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();
		}	
	
		ccdComponentPropFileName=CORBA::string_dup(componentpropertiesFile.transcode());	
	}
	
	//
	//add configuration complete 
	//	
	CORBA::String_var setConfigurationComplete;

	DOM_Node configurationcompleteNode=parser->findChild(root,"configurationcomplete");
	if(configurationcompleteNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: configurationcomplete node missed.\n");
	
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
	
		throw InvalidConfigureFile();
	}
	
	DOMString setStr=parser->getAttr(configurationcompleteNode,"set");
	if(setStr==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: configurationcomplete node miss set attribute.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw InvalidConfigureFile();
	}	
	
	setConfigurationComplete =CORBA::string_dup(setStr.transcode());	

	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}		

	//
	//get the componentinstantiation and process it one by one 
	//	
	TRACE0(LEVEL5,"Assembly: get the componentinstantiation and process it one by one .\n");
	DOM_NodeList componentInstantiationChildren=homeplacementNode.getChildNodes();
        unsigned int componentInstantiationChildrenLen=componentInstantiationChildren.getLength();
	
	DOM_Node componentInstantiationChild;
	DOMString componentInstantiationChildName;	
	
	for(unsigned int instIterator=0;instIterator<componentInstantiationChildrenLen;instIterator++)
	{
		componentInstantiationChild=componentInstantiationChildren.item(instIterator);
		componentInstantiationChildName=componentInstantiationChild.getNodeName();		
		
		if(componentInstantiationChildName.equals("componentinstantiation"))
		{			
			//
			//get the component instantiation Id
			//
			DOMString componentinstantiationId=parser->getAttr(componentInstantiationChild,"id");
			if(componentinstantiationId==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid cad file : componentinstantiation node miss id attribute . \n");
				throw InvalidConfigureFile();										
			}	

			//
			//judge whether the componentinstantiationId existed
			//

			ArchComponent* acom = arch->getArchComponent(componentinstantiationId.transcode());
			if (acom != NULL)
			{
				if(verbose)
				{
					cout<<"Assembly: componentinstantiation Id: " <<componentinstantiationId.transcode() << "has existed."<<endl;			
				}		
				throw InvalidConfigureFile();
				delete acom;
				acom = NULL;
			}


			//modified
			/*
			MapComponentInf::iterator iter=componentMap.begin();
			MapComponentInf::iterator iter_end=componentMap.end();
			while(iter!=iter_end)
			{
				CORBA::String_var id=CORBA::string_dup((* iter).first.c_str());
				
				if(strcmp(id.in(),componentinstantiationId.transcode())==0)
				{
					if(verbose)
					{
						cout<<"Assembly: componentinstantiation Id: " <<componentinstantiationId.transcode() << "has existed."<<endl;			
					}		
					throw InvalidConfigureFile();
					
				}	
				iter++;
			}		
			*/
			
			TRACE1(LEVEL1,"Assembly: processing componentInstantiation %s. \n",componentinstantiationId.transcode());
			
			//
			//get the Component Property file, the primary key will be spedified here
			//
			CORBA::String_var componentInstantiationPropFile=rootDir;
			componentInstantiationPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
			componentInstantiationPropFile+=assemblyUUID;
			componentInstantiationPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
			
			DOM_Node componentpropertiesNode=parser->findChild(componentInstantiationChild,"componentproperties");
			if(componentpropertiesNode!=NULL)
			{
				DOMString componentInstantiationPropFileName=parser->parseFileInArchive(componentpropertiesNode);
				if(componentInstantiationPropFileName==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid cad file : invalid fileinarchive node of componentproperties  node. \n");
					throw InvalidConfigureFile();						
				}

				componentInstantiationPropFile+=CORBA::string_dup(componentInstantiationPropFileName.transcode());
				
			}
			
			//
			//get the primary key from the property file
			//
			ConfigValue_var pkValue;
			if(componentpropertiesNode!=NULL)
			{
				try
				{
					pkValue=getPrimaryKeyProp(componentInstantiationPropFile.in());
	
				}
				catch(const InvalidConfigureFile &)
				{
					TRACE1(LEVEL1,"Assembly: invalid cad file : get primary key from property file %s failed. \n",componentInstantiationPropFile.in());
					throw InvalidConfigureFile();														
				}
			}
				
			bool pkSpecified=true;
			if(pkValue== NULL)
			{
				pkSpecified=false;
			}
			
			//
			//create component ,different from whether the component have pk or not
			//
			TRACE0(LEVEL4,"Assembly: create component.\n");
			Components::CCMObject_var componentObj;
			if(!pkSpecified)
			{				
				Components::KeylessCCMHome_var keylessHome=Components::KeylessCCMHome::_narrow(home);
				if(CORBA::is_nil(keylessHome))
				{
					TRACE0(LEVEL6,"Assembly: keyless home is NULL.\n");
					throw BuildHomePlacementFailure();
				}
				
				try
				{					
					TRACE0(LEVEL6,"Assembly: keyless home create component.\n");

					componentObj=keylessHome->create_component();	
					//jxh				
					
				}
				catch(const CORBA::SystemException &)
				{
					TRACE0(LEVEL1,"Assembly:  create component failed in CORBA SystemException. \n");
					throw BuildHomePlacementFailure();											
				}
				catch(const Components::CreateFailure &)
				{
					TRACE0(LEVEL1,"Assembly: create component failed in CreateFailure. \n");
					throw BuildHomePlacementFailure();						
				}	

				if(CORBA::is_nil(componentObj))
				{
					TRACE0(LEVEL6,"Assembly: the component created by keyless home is NULL.\n");
					throw BuildHomePlacementFailure();
				}				
				
			}
			else
			{
				
				//
				//construct the DII request parameter and invoke create operation
				//
				
				CORBA::Context_var ctx;
				orb ->get_default_context(ctx.out());
				
				CORBA::NamedValue_var result;
				orb ->create_named_value(result.out());
				CORBA::Any& outValue = *(result->value());
				outValue.replace(Components::_tc_CCMObject, 0);
				
				CORBA::ExceptionList_var exceptions;
				orb ->create_exception_list(exceptions.out());
				exceptions -> add(Components::_tc_CreateFailure);
				exceptions -> add(Components::_tc_DuplicateKeyValue);
				exceptions -> add(Components::_tc_InvalidKey);
				
				CORBA::ContextList_var cl;
				orb ->create_context_list(cl.out());
				
				CORBA::NVList_var argList;
				orb ->create_list(0,argList);				
				CORBA::NamedValue_ptr arg;			
				arg = argList ->add(CORBA::ARG_IN);				
				*(arg ->value()) = pkValue->value();
				
				CORBA::Request_var request;	

				home->_create_request(ctx,"create",argList,result,exceptions,cl,request,0);
				
				request ->invoke();	
				
				CORBA::Exception* ex = request -> env() -> exception();
				if (ex != NULL)
				{					
					//
					//judge the exception type and print out the error msg
					//
					TRACE0(LEVEL1,"Assembly: create component failed . \n");
					throw BuildHomePlacementFailure();						
					
				}
				else
				{
					request -> return_value() >>= componentObj;						
				}
				
			}
		
		
			//
			//configure the component
			//
			TRACE0(LEVEL4,"Assembly: configure the component.\n");
			try
			{
				if(componentpropertiesNode!=NULL)
				{			
					
					if(ccdComponentPropFileName!=NULL)
					{
						CORBA::String_var ccdComponentPropFile=rootDir;
						ccdComponentPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
						ccdComponentPropFile+=CORBA::string_dup(componentImplRef.transcode());
						ccdComponentPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
						ccdComponentPropFile+=ccdComponentPropFileName; 
						
						TRACE1(LEVEL6,"Assembly: the cad property fils is %s.\n",componentInstantiationPropFile.in());
						TRACE1(LEVEL6,"Assembly: the ccd property fils is %s.\n",ccdComponentPropFile.in());
						configComponent(ccdComponentPropFile.in(),componentInstantiationPropFile.in(),componentObj.in());
						TRACE2(LEVEL6,"Assembly: configComponent with ccd file %s and cad file %s.\n",ccdComponentPropFile.in(),componentInstantiationPropFile.in());
					}
					else
					{
						
						configComponent("",componentInstantiationPropFile.in(),componentObj.in());
						TRACE1(LEVEL6,"Assembly: configComponent with cad file %s.\n",componentInstantiationPropFile.in());						
					}
				}
				else
				{
					if(ccdComponentPropFileName!=NULL)
					{
						CORBA::String_var ccdComponentPropFile=rootDir;
						ccdComponentPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
						ccdComponentPropFile+=CORBA::string_dup(componentImplRef.transcode());
						ccdComponentPropFile+=CORBA::string_dup(PATH_DELILIMITOR);
						ccdComponentPropFile+=ccdComponentPropFileName;

						configComponent(ccdComponentPropFile.in(),"",componentObj.in());
						TRACE1(LEVEL6,"Assembly: configComponent with ccd file %s.\n",ccdComponentPropFile.in());												
					}				
				}
			}
			catch(const CORBA::SystemException &)
			{
				TRACE0(LEVEL1,"Assembly: configure the CCMObject failed in CORBA SystemException . \n");
				throw BuildHomePlacementFailure();									
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE0(LEVEL1,"Assembly:configure the CCMObject failed in InvalidConfigureFile . \n");
				throw InvalidConfigureFile();									
			}
			catch(const ConfigureFailure &)
			{
				TRACE0(LEVEL1,"Assembly:configure the CCMObject failed in ConfigureFailure . \n");
				throw BuildHomePlacementFailure();									
			}
			
			//
			//register the component and its facets
			//			
			try
			{
				//
				//register the Component into the NamingService
				//
				TRACE0(LEVEL5,"Assembly: registerComponent. \n");			
				if(registerComponentId != NULL)
				{					
					for(int j=0;j<registerComponentId->length();j++)
					{
						if(strcmp(registerComponentId[j],componentinstantiationId.transcode()) == 0)
						{
							CosNaming::Name aName;						
							aName.length(1);
							aName[0].id = CORBA::string_dup(registerComponent[j]);
					    aName[0].kind = CORBA::string_dup("");
					    
					    TRACE1(LEVEL6,"Assembly: registerComponent name is %s. \n",aName[0].id);
							nameServiceContextObj -> bind(aName, componentObj.in());
						}
					}
				}

				//
				//register the facet into the namingservice
				//
				TRACE0(LEVEL5,"Assembly: registerFacet. \n");				
				if(registerFacetName != NULL)
				{	
					
					for(int j=0;j<registerFacetName->length();j++)
					{
						TRACE1(LEVEL6,"Assembly: registerFacet length is %d. \n",registerFacetName->length());
						
						CORBA::Object_var facetObj = componentObj->provide_facet(registerFacetName[j]);
						
						CosNaming::Name aName;
						aName.length(1);
						aName[0].id = CORBA::string_dup(facetRegisterName[j]);
				    aName[0].kind = CORBA::string_dup("");
				    TRACE1(LEVEL6,"Assembly: registerFacet name is %s. \n",aName[0].id);
						nameServiceContextObj -> bind(aName, facetObj.in());
					}					
				}	
			}			
			catch(const CosNaming::NamingContext::NotFound& ex)
			{        
				switch(ex.why)
				{
					case CosNaming::NamingContext::missing_node:
								TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `NotFound' exception (missing node). \n");
								break;
		
					case CosNaming::NamingContext::not_context:
								TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `NotFound' exception (not context). \n");
								break;
		
					case CosNaming::NamingContext::not_object:
								TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `NotFound' exception (not object). \n");
								break;
				}
				
				throw InvalidConfigureFile();
			}
			catch(const CosNaming::NamingContext::CannotProceed&)
			{
				TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `CannotProceed' exception. \n");
				throw InvalidConfigureFile();
			}
			catch(const CosNaming::NamingContext::InvalidName&)
			{
				TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `InvalidName' exception. \n");
				throw InvalidConfigureFile();
			}
			catch(const CosNaming::NamingContext::AlreadyBound&)
			{
				TRACE0(LEVEL1,"Assembly: registe the component and facet into the NamingService failed for Got a `AlreadyBound' exception. \n");
				throw InvalidConfigureFile();
			}	

			CORBA::Object_var conobj = CORBA::Object::_duplicate(ahome->containerref);
			Container_var container = Components::Deployment::Container::_narrow(conobj);

			ArchComponent_var archcom = container->getArchCom();
			archcom->id = CORBA::string_dup(componentinstantiationId.transcode());

			//2004.6.23
			//find the dynmic deployment info of componentinstantiation in cad file
			DOM_Node boundableNode=parser->findChild(componentInstantiationChild,"boundable");
			if (boundableNode !=NULL)
			{
				DOMString boundableset = parser->getAttr(boundableNode, "set");
				if (boundableset == NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid cad file boundable miss set attribute. \n");
					throw InvalidConfigureFile();
				}
				else
				{
					if (strcmp("true",boundableset.transcode()) == 0)
						acom->isboundable = CORBA::string_dup("true");
					else
						acom->isboundable = CORBA::string_dup("false");
				}
			}

			DOM_Node boundaryportsNode = parser->findChild(componentInstantiationChild, "boundaryports");
			if (boundaryportsNode != NULL)
			{
				DOM_NodeList childrenNodes = boundaryportsNode.getChildNodes();
		        	unsigned int childrenLen=childrenNodes.getLength();
			
				DOM_Node boundaryportNode;
				DOMString boundaryportNodeName;	
				DOMString boundaryportName;	

				PortNameSeq_var tmpseq = new PortNameSeq;
				int len = 0;
				for(unsigned int iterator=0;iterator<childrenLen;iterator++)
				{
					boundaryportNode = childrenNodes.item(iterator);
					boundaryportNodeName = boundaryportNode.getNodeName();
					if (boundaryportNodeName.equals(" boundaryport"))
					{
						boundaryportName = parser->getAttr(boundaryportNode,"name");
						if (boundaryportName == NULL)
						{
							TRACE0(LEVEL1,"Assembly: invalid cad file boundaryport miss name attribute. \n");
							throw InvalidConfigureFile();
						}
						tmpseq->length(len+1);
						tmpseq[len++] = CORBA::string_dup(boundaryportName.transcode());
					}
				}
				if (len > 0)
					acom->boundaryports = tmpseq;
			}

			//get register info
			acom->nameservice = CORBA::Object::_duplicate(nameServiceContextObj);
			if (registerComponentId != NULL)
			{
				int len = registerComponentId->length();
				RegisterNameSeq_var tmpseq = new RegisterNameSeq;
				int tlen = 0;
				int i;
				for (i=0; i<len; i++)
				{
					if (strcmp(registerComponentId[i], componentinstantiationId.transcode()) ==0)
					{
						tmpseq->length(tlen+1);
						tmpseq[tlen++] = registerComponent[i];
					}
						
				}
				if (tlen > 0)
					acom->withnaming = tmpseq;
			}

			if (registerFacetName != NULL)
			{
				int len = registerFacetName->length();
				PortWithNamingSeq_var portseq = new PortWithNamingSeq;
				int plen = 0;
				int tlen = 0;
				int i = 0;
				for (i=0; i<len; i++)
				{
					int j;
					for (j=0; j<plen; j++)
					{
						if (strcmp(registerFacetName[i], portseq[j].portname) == 0)
							break;
					}

					if (j<plen)
					{
						tlen = portseq[j].withnaming.length();
						portseq[j].withnaming.length(tlen+1);
						portseq[j].withnaming[tlen] = facetRegisterName[i];
					}
					else
					{
						portseq->length(plen+1);
						portseq[plen].portname = registerFacetName[i];
						portseq[plen].withnaming.length(1);
						portseq[plen].withnaming[0] = facetRegisterName[i];	
						plen++;
					}


				}

				if (plen > 0)
					acom->portwithnamings = portseq;
			}
			
			arch->addArchComponent(archcom);
			//end by wsf 2004.6.23
			
			//
			//add the ComponentInfo into ComponentMap
			//
			TRACE0(LEVEL5,"Assembly: add the ComponentInfo into ComponentMap. \n");
			ComponentInf componentInformation;
			componentInformation.ComponentRef=componentObj;
			componentInformation.ConfigurationComplete=false;
			CORBA::StringSeq_var dependList=new CORBA::StringSeq;
			componentInformation.depend=dependList;	
			if(strcmp(setConfigurationComplete.in(),"true")==0)
			{			
				componentInformation.SetConfigurationComplete=true;
			}
			else
			{
				componentInformation.SetConfigurationComplete=false;
			}

			componentMap.insert(MapComponentInf::value_type(string(componentinstantiationId.transcode()),componentInformation));
		}
			
	}
	
	//
	//add the register information into registerNamings used to unregister from the NamingService 
	//		
	TRACE0(LEVEL5,"Assembly: add the register information into registerNamings. \n");	
	int registerNamingsLen = registerNamings->length();

	TRACE0(LEVEL5,"Assembly: add registerComponent. \n");
	if(registerComponent != NULL)
	{
		for(int j=0;j<registerComponent->length();j++)
		{
			registerNamings->length(registerNamingsLen+1);
			registerNamings[registerNamingsLen] = registerComponent[j];
			registerNamingsLen++;
		}
	}
	
	TRACE0(LEVEL5,"Assembly: add facetRegisterName. \n");
	if(facetRegisterName != NULL)
	{
		for(int j=0;j<facetRegisterName->length();j++)
		{
			registerNamings->length(registerNamingsLen+1);
			registerNamings[registerNamingsLen] = facetRegisterName[j];
			registerNamingsLen++;
		}		
	}

	TRACE0(LEVEL5,"Assembly: end of dynbuildhomeplacement. \n");
	
}

//wsf
void
Assembly_impl::dynBuildProcessCollocation(XMLParser *parser,
 					DOM_Node root,
					DOM_Node processcollocationNode)
		throw (InvalidConfigureFile, BuildProcessCollocationFailure)					
{
	DOM_NodeList processcollocationChildren=processcollocationNode.getChildNodes();
        unsigned int processcollocationChildrenLen=processcollocationChildren.getLength();
	
	DOM_Node processcollocationChild;
	DOMString processcollocationChildName;	
	
	for(unsigned int iterator=0;iterator<processcollocationChildrenLen;iterator++)
	{
	
		processcollocationChild=processcollocationChildren.item(iterator);
		processcollocationChildName=processcollocationChild.getNodeName();	
		
		if(processcollocationChildName.equals("homeplacement"))
		{
			try
			{				
				dynBuildHomePlacement(parser,root,processcollocationChild);				
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE0(LEVEL1,"Assembly: build process collocation failed in invalid configuration file.\n");
				throw;
			}
			catch(const BuildHomePlacementFailure &)
			{
				TRACE0(LEVEL1,"Assembly: build process collocation failed in invalid configuration file.\n");
				throw BuildProcessCollocationFailure();				
			}
			catch(const CORBA::SystemException &)
			{
				TRACE0(LEVEL1,"Assembly: build process collocation failed in invalid configuration file.\n");
				throw BuildProcessCollocationFailure();				
			}			
		}
	}	
	
}

//wsf
void
Assembly_impl::dynBuildHostCollocation(XMLParser *parser,
 					DOM_Node root,
					DOM_Node hostcollocationNode)
		throw (InvalidConfigureFile, BuildHostCollocationFailure)
{
	
	DOM_NodeList hostcollocationChildren=hostcollocationNode.getChildNodes();
        unsigned int hostcollocationChildrenLen=hostcollocationChildren.getLength();
	
	DOM_Node hostcollocationChild;
	DOMString hostcollocationChildName;	
	
	for(unsigned int iterator=0;iterator<hostcollocationChildrenLen;iterator++)
	{
	
		hostcollocationChild=hostcollocationChildren.item(iterator);
		hostcollocationChildName=hostcollocationChild.getNodeName();
		
		if(hostcollocationChildName.equals("homeplacement"))
		{
			try
			{
				dynBuildHomePlacement(parser,root,hostcollocationChild);
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE0(LEVEL1,"Assembly: build host collocation failed in invalid configuration file.\n");
				throw;
			}
			catch(const BuildHomePlacementFailure &)
			{
				TRACE0(LEVEL1,"Assembly: build host collocation failed in invalid configuration file.\n");
				throw BuildHostCollocationFailure();				
			}
		}
		else if(hostcollocationChildName.equals("processcollocation"))
		{
			try
			{
				dynBuildProcessCollocation(parser,root,hostcollocationChild);
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE0(LEVEL1,"Assembly: build host collocation failed in invalid configuration file.\n");
				throw;
			}
			catch(const BuildProcessCollocationFailure &)
			{
				TRACE0(LEVEL1,"Assembly: build host collocation failed in invalid configuration file.\n");
				throw BuildHostCollocationFailure();				
			}

		}
	}
	
}

//****************************************************************************
//get config values for install home from the cad ,csd and ccd file
//*****************************************************************************
void 
Assembly_impl::getInstallHomeCfValues(XMLParser *parser,DOM_Node root,DOM_Node homeplacementNode,const char *implUUID,ConfigValues & cfValues, ConfigValues &dyncfValues)
	throw (InvalidConfigureFile)
{
	
	TRACE0(LEVEL5,"Assembly: in getInstallHomeCfValues.\n");
	
	//
	//locate the csd file
	//	
	
	TRACE0(LEVEL5,"Assembly: GetInstallHomeCfValues: locate the csd file.\n");
	CORBA::String_var csdFile;
	try
	{
		csdFile=getCsdFile(parser,root,homeplacementNode);
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE0(LEVEL1,"Assembly: get csd file failed in invalid cad file.\n");
		throw InvalidConfigureFile();		
	}
	
	//
	//get the config values from the cad file: register information
	//
	TRACE0(LEVEL5,"Assembly: GetInstallHomeCfValues: get the config values from the cad file.\n");
	
	//
	//get the HomeRegistration and namingservice obj.
	//
	registerWithHomeFinders=new CORBA::StringSeq;
	registerWithNamings=new CORBA::StringSeq;	
	registerComponentId=new CORBA::StringSeq;
	registerComponent=new CORBA::StringSeq;	
	registerFacetName=new CORBA::StringSeq;	
	facetRegisterName=new CORBA::StringSeq;	
	
	cout << "Assembly: Get homeplacementNode" << endl;
	
	DOM_NodeList children=homeplacementNode.getChildNodes();
	unsigned int childrenLen=children.getLength();	
	
	cout << "Assembly: End of get homeplacementNode" << endl;
	
	DOM_Node child;
	DOMString childName;
	
//	TRACE1(LEVEL6,"Assembly:get the HomeRegistration and namingservice information ,the length is %d.\n",childrenLen);
	
	cout << "Assembly:get the HomeRegistration and namingservice information ,the length is " << childrenLen << endl;
	
	for(unsigned int iterator=0;iterator<childrenLen;iterator++)
	{
		child=children.item(iterator);
		childName=child.getNodeName();		
		
		if(childName.equals("registerwithhomefinder"))
		{
			TRACE0(LEVEL5,"Assembly: enter registerwithhomefinder .\n");
			
			DOMString name=parser->getAttr(child,"name");
			if(name==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid cad file: registerwithhomefinder miss name attribute .\n");
				throw InvalidConfigureFile();					
			}
			
			int len=registerWithHomeFinders->length();
			registerWithHomeFinders->length(len+1);
			registerWithHomeFinders[(CORBA::ULong)len]=CORBA::string_dup(name.transcode());
			
		}
		else if(childName.equals("registerwithnaming"))
		{
			TRACE0(LEVEL5,"Assembly: enter registerwithnaming .\n");
			
			DOMString name=parser->getAttr(child,"name");
			if(name==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid cad file: registerwithnaming miss name attribute .\n");
				throw InvalidConfigureFile();					
			}
			
			int len=registerWithNamings->length();
			registerWithNamings->length(len+1);
			registerWithNamings[(CORBA::ULong)len]=CORBA::string_dup(name.transcode());	
				
		}	
		else if(childName.equals("componentinstantiation"))
		{
			TRACE0(LEVEL5,"Assembly: enter componentinstantiation .\n");
			
			DOM_NodeList componentInstChildren=child.getChildNodes();
			unsigned int componentInstChildrenLen=componentInstChildren.getLength();	
			
			for(unsigned int i=0;i<componentInstChildrenLen;i++)
			{
				DOM_Node componentInstChild=componentInstChildren.item(i);
				DOMString componentInstChildName=componentInstChild.getNodeName();	

				DOMString componentInstId=parser->getAttr(child,"id");
				if(componentInstId==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid cad file: componentinstantiation miss id attribute .\n");
					throw InvalidConfigureFile();					
				}
				
				if(componentInstChildName.equals("registercomponent"))
				{
					DOM_Node providesidentifierNode=parser->findChild(componentInstChild,"providesidentifier");
					if(providesidentifierNode==NULL)
					{						
						//
						//add register component information
						//
                        
                        TRACE0(LEVEL5,"Assembly: enter registercomponent .\n");
                            
						DOM_NodeList registercomponentChildren=componentInstChild.getChildNodes();
						unsigned int registercomponentChildrenLen=registercomponentChildren.getLength();							
									
						for(unsigned int i=0;i<registercomponentChildrenLen;i++)
						{
							DOM_Node registercomponentChild=registercomponentChildren.item(i);
							DOMString registercomponentChildName=registercomponentChild.getNodeName();	
							
							if(registercomponentChildName.equals("registerwithnaming"))
							{								
								DOMString name=parser->getAttr(registercomponentChild,"name");
								if(name==NULL)
								{
									TRACE0(LEVEL1,"Assembly: invalid cad file: registerwithnaming miss name attribute .\n");
									throw InvalidConfigureFile();					
								}
																							
								int len=registerComponentId->length();
								
								registerComponentId->length(len+1);
								registerComponentId[(CORBA::ULong)len]=CORBA::string_dup(componentInstId.transcode());	
								
								registerComponent->length(len+1);
								registerComponent[(CORBA::ULong)len]=CORBA::string_dup(name.transcode());	
								
								TRACE2(LEVEL6,"Assembly: get registerwithnaming of %s for component instantiation %s.\n",name.transcode(),componentInstId.transcode());												
							}
						}						
					}
					else
					{
						//
						//add register facet information
						//
						TRACE0(LEVEL5,"Assembly: enter register facet information .\n");
						
						DOM_Node  providesIdentifierText=providesidentifierNode.getFirstChild();				
						DOMString providesIdentifier=providesIdentifierText.getNodeValue();
						
						DOM_NodeList registercomponentChildren=componentInstChild.getChildNodes();
						unsigned int registercomponentChildrenLen=registercomponentChildren.getLength();	
						
						for(unsigned int i=0;i<childrenLen;i++)
						{
							DOM_Node registercomponentChild=registercomponentChildren.item(i);
							DOMString registercomponentChildName=registercomponentChild.getNodeName();	
							
							if(registercomponentChildName.equals("registerwithnaming"))
							{

								DOMString name=parser->getAttr(registercomponentChild,"name");
								if(name==NULL)
								{
									TRACE0(LEVEL1,"Assembly: invalid cad file: registerwithnaming miss name attribute .\n");
									throw InvalidConfigureFile();					
								}						
								
								int len=registerFacetName->length();
								registerFacetName->length(len+1);
								registerFacetName[(CORBA::ULong)len]=CORBA::string_dup(providesIdentifier.transcode());			
								
								facetRegisterName->length(len+1);
								facetRegisterName[(CORBA::ULong)len]=CORBA::string_dup(name.transcode());			
												
							}
						}
					}					
				}
			}			
		}
	}
	

	
	//
	//add the register information into the cfValues
	//
	TRACE0(LEVEL5,"Assembly: GetInstallHomeCfValues: add the register information into the cfValues.\n");
	
	CORBA::Any_var anyValue=new CORBA::Any;
	anyValue<<=registerWithHomeFinders.in();
	ConfigValue_var cfValue=new ConfigValue_impl("HomeRegisterWithHomeFinders",anyValue.in());	 
	
	TRACE0(LEVEL6,"Assembly: GetInstallHomeCfValues:before get the cfValues length \n");
	int len=cfValues.length();
	
	cfValues.length(len+1);		
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;
	
	anyValue<<=registerWithNamings.in();	
	cfValue=new ConfigValue_impl("HomeRegisterWithNamings",anyValue.in());
	cfValues.length(len+1);	
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;	

	anyValue<<=registerComponentId.in();	
	cfValue=new ConfigValue_impl("RegisterComponentIds",anyValue.in());	
	cfValues.length(len+1);	
	cfValues[(CORBA::ULong)len]=cfValue;		
	len++;
	
	anyValue<<=registerComponent.in();	
	cfValue=new ConfigValue_impl("RegisterComponents",anyValue.in());	
	cfValues.length(len+1);	
	cfValues[(CORBA::ULong)len]=cfValue;		
	len++;	

	anyValue<<=registerFacetName.in();	
	cfValue=new ConfigValue_impl("RegisterFacetNames",anyValue.in());	
	cfValues.length(len+1);	
	cfValues[(CORBA::ULong)len]=cfValue;		
	len++;

	anyValue<<=facetRegisterName.in();	
	cfValue=new ConfigValue_impl("FacetRegisterNames",anyValue.in());	
	cfValues.length(len+1);	
	cfValues[(CORBA::ULong)len]=cfValue;		
	len++;
		
	//
	//get the config values from the csd file and nested get the config values from the ccd file
	//	
	
	TRACE0(LEVEL5,"Assembly: GetInstallHomeCfValues: get the config values from the csd.\n");
	
	try
	{
		getCsdInstallHomeCfValues(csdFile.in(),implUUID,cfValues,dyncfValues);	
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE0(LEVEL1,"Assembly: get config values from the csd file failed.\n");
		throw InvalidConfigureFile();		
	}

}


//*****************************************************************************
//get the csd file according to the cad file with the help of XMLParser
//return the csd file full path
//*****************************************************************************
char * 
Assembly_impl::getCsdFile(XMLParser *parser, DOM_Node root, DOM_Node homeplacementNode)
	throw ( InvalidConfigureFile)
{
	
	DOM_Node componentFileRefNode=parser->findChild(homeplacementNode ,"componentfileref");
	if(componentFileRefNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentfileref node missed.\n");
		throw InvalidConfigureFile();
	}
	DOMString componentFileId=parser->getAttr(componentFileRefNode,"idref");
	if(componentFileId==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentfileref node miss idref attribute.\n");
		throw InvalidConfigureFile();
	}	
	
	DOM_Node componentFilesNode=parser->findChild(root,"componentfiles");
	if(componentFilesNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentfiles node missed.\n");
		throw InvalidConfigureFile();
	}
	
	DOM_Node componentFileNode=parser->findChildByAttr(componentFilesNode,"componentfile","id",componentFileId.transcode());
	if(componentFileNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentfile node missed.\n");
		throw InvalidConfigureFile();
	}	
	
	DOMString componentId=parser->getAttr(componentFileNode,"id");
	if(componentId==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file: componentfile node miss the id attribute.\n");
		throw InvalidConfigureFile();		
	}
	
	DOMString csdFileName=parser->parseFileInArchive(componentFileNode);
	if(csdFileName==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid cad file to specify the csd file.\n");
		throw InvalidConfigureFile();				
	}
	
	//
	//locate the csd file path: rootDir\assemblyUUID\componentId\csdfile
	//
	
	CORBA::String_var csdFileDir=rootDir;

#ifdef STARCCMSTRINGADD
	csdFileDir = HelpFun::CORBA_string_add(csdFileDir, PATH_DELILIMITOR);
	csdFileDir = HelpFun::CORBA_string_add(csdFileDir, assemblyUUID);
	csdFileDir = HelpFun::CORBA_string_add(csdFileDir, PATH_DELILIMITOR);
	csdFileDir = HelpFun::CORBA_string_add(csdFileDir, CORBA::string_dup(componentId.transcode()));
	csdFileDir = HelpFun::CORBA_string_add(csdFileDir, PATH_DELILIMITOR);
	csdFileDir = HelpFun::CORBA_string_add(csdFileDir, CORBA::string_dup(csdFileName.transcode()));
#else	
	csdFileDir+=CORBA::string_dup(PATH_DELILIMITOR);
	csdFileDir+=assemblyUUID;
	csdFileDir+=CORBA::string_dup(PATH_DELILIMITOR);
	csdFileDir+=CORBA::string_dup(componentId.transcode());
	csdFileDir+=CORBA::string_dup(PATH_DELILIMITOR);
	csdFileDir+=CORBA::string_dup(csdFileName.transcode());
#endif	
	
	return csdFileDir._retn();
	
}

//****************************************************************************
//get config values for install home from the csd file
//*****************************************************************************

void 
Assembly_impl::getCsdInstallHomeCfValues(const char * csdFile,const char *implUUID, ConfigValues & cfValues, ConfigValues &dyncfValues)
	throw ( InvalidConfigureFile)
{
	
	
	TRACE0(LEVEL5,"Assembly: getCsdInstallHomeCfValues.\n");
	//
	//XMLParser initialized
	//
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(csdFile);
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: XMLParser initialized failed for csd file %s. \n",csdFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();
	}    
	
	DOM_Node root = parser->getRootNode("softpkg");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: Invalid csd file %s : have no softpkg element. \n",csdFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw InvalidConfigureFile();
	}	


	//
	//add the configvalue from the csd file
	//
	
	//
	//parse valuetypefactory from the softpkg dependency 
	//

	CORBA::StringSeq_var spValueFactoryNames=new CORBA::StringSeq;
	CORBA::StringSeq_var spValueFactoryFactoryEntrys=new CORBA::StringSeq;
	CORBA::StringSeq_var spValueFactoryEntrys=new CORBA::StringSeq;
	CORBA::StringSeq_var spValueFactoryRepids=new CORBA::StringSeq;
	
	DOM_NodeList children=root.getChildNodes();
        unsigned int childrenLen=children.getLength();	
	
	DOM_Node child;
	DOMString childName;

	for(unsigned int iterator=0;iterator<childrenLen;iterator++)
	{
		child=children.item(iterator);
		childName=child.getNodeName();			

		if(childName.equals("dependency"))
		{

			DOM_Node valuetypefactoryNode=parser->findChild(child,"valuetypefactory");
			if(valuetypefactoryNode!=NULL)
			{
				DOMString valuetypefactoryName=parser->parseFileInArchive(valuetypefactoryNode);
				if(valuetypefactoryName==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid csd file:invalid fileinarchive node of valuetypefactory node .\n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					
					throw InvalidConfigureFile();					
				}
				
				DOMString valuetypefactoryRrepid=parser->getAttr(valuetypefactoryNode,"repid");
				if(valuetypefactoryRrepid==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid csd file: valuetypefactory node miss repid attribute.\n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					
					throw InvalidConfigureFile();					
				}
				
				DOMString valueEntryPoint=parser->getAttr(valuetypefactoryNode,"valueentrypoint");
				if(valueEntryPoint==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid csd file: valuetypefactory node miss valueentrypoint attribute.\n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					
					throw InvalidConfigureFile();					
				}

				DOMString factoryEntryPoint=parser->getAttr(valuetypefactoryNode,"factoryentrypoint");
				if(factoryEntryPoint==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid csd file: valuetypefactory node miss factoryentrypoint attribute.\n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					
					throw InvalidConfigureFile();					
				}				
				
				CORBA::String_var valueFactoryName=CORBA::string_dup(valuetypefactoryName.transcode());
				CORBA::String_var valueFactoryRepid=CORBA::string_dup(valuetypefactoryRrepid.transcode());
				CORBA::String_var valueFactoryEntryPoint=CORBA::string_dup(valueEntryPoint.transcode());
				CORBA::String_var valueFactoryFactoryEntryPoint=CORBA::string_dup(factoryEntryPoint.transcode());
								
				int len=spValueFactoryNames->length();
				
				spValueFactoryNames->length(len+1);
				spValueFactoryFactoryEntrys->length(len+1);
				spValueFactoryEntrys->length(len+1);
				spValueFactoryRepids->length(len+1);
				
				
				spValueFactoryRepids[(CORBA::ULong)len]=valueFactoryRepid;								
				TRACE1(LEVEL6,"Assembly: the value factory repid is %s.\n",valueFactoryRepid);
				
				spValueFactoryNames[(CORBA::ULong)len]=valueFactoryName;
				TRACE1(LEVEL6,"Assembly: the value factory name is %s.\n",valueFactoryName);
				
				spValueFactoryFactoryEntrys[(CORBA::ULong)len]=valueFactoryFactoryEntryPoint;
				TRACE1(LEVEL6,"Assembly: the value factory FactoryEntryPoint is %s.\n",valueFactoryFactoryEntryPoint);
								
				spValueFactoryEntrys[(CORBA::ULong)len]=valueFactoryEntryPoint;
				TRACE1(LEVEL6,"Assembly: the value factory EntryPoint is %s.\n",valueFactoryEntryPoint);
				
			}
						
		}
		
		
	}
	
	//
	//parse the valuetypefactory declared in the implementation dependency
	//
	CORBA::StringSeq_var ValueFactoryNames=spValueFactoryNames;
	CORBA::StringSeq_var ValueFactoryFactoryEntrys=spValueFactoryFactoryEntrys;
	CORBA::StringSeq_var ValueFactoryEntrys=spValueFactoryEntrys;
	CORBA::StringSeq_var ValueFactoryRepids=spValueFactoryRepids;
	
	
	DOM_Node implementationNode=parser->findChildByAttr(root,"implementation","id",implUUID);
	if(implementationNode==NULL)
	{
		TRACE2(LEVEL1,"Assembly: invalid csd file %s: implementation node %s missed.\n",csdFile,implUUID);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();		
	}
	
	DOM_NodeList implChildren=implementationNode.getChildNodes();
        unsigned int implChildrenLen=implChildren.getLength();	
	
	DOM_Node implChild;
	DOMString implChildName;

	TRACE1(LEVEL6,"Assembly: the impl child number is %d .\n",implChildrenLen);
	for(unsigned int implIterator=0;implIterator<implChildrenLen;implIterator++)
	{
		TRACE1(LEVEL6,"Assembly: the iterator  is %d .\n",implIterator);		

		implChild=implChildren.item(implIterator);
		implChildName=implChild.getNodeName();	

		CORBA::String_var implChildNameStr=CORBA::string_dup(implChildName.transcode());
		TRACE1(LEVEL6,"Assembly: the impl child is %s .\n",implChildNameStr);
		
		
		if(implChildName.equals("dependency"))
		{

			TRACE0(LEVEL6,"Assembly: the impl child is dependency .\n");
			
			DOM_Node valuetypefactoryNode=parser->findChild(implChild,"valuetypefactory");
			if(valuetypefactoryNode!=NULL)
			{
				DOMString valuetypefactoryName=parser->parseFileInArchive(valuetypefactoryNode);
				if(valuetypefactoryName==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid csd file:invalid fileinarchive node of valuetypefactory node .\n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					
					throw InvalidConfigureFile();					
				}
				
				DOMString valuetypefactoryRrepid=parser->getAttr(valuetypefactoryNode,"repid");
				if(valuetypefactoryRrepid==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid csd file: valuetypefactory node miss repid attribute.\n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					
					throw InvalidConfigureFile();					
				}
				
				DOMString valueEntryPoint=parser->getAttr(valuetypefactoryNode,"valueentrypoint");
				if(valueEntryPoint==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid csd file: valuetypefactory node miss valueentrypoint attribute.\n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					
					throw InvalidConfigureFile();					
				}

				DOMString factoryEntryPoint=parser->getAttr(valuetypefactoryNode,"factoryentrypoint");
				if(factoryEntryPoint==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid csd file: valuetypefactory node miss factoryentrypoint attribute.\n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}	
					
					throw InvalidConfigureFile();					
				}				
				
				CORBA::String_var valueFactoryName=CORBA::string_dup(valuetypefactoryName.transcode());
				TRACE1(LEVEL6,"Assembly: the value factory name is %s.\n",valueFactoryName);
				
				CORBA::String_var valueFactoryRepid=CORBA::string_dup(valuetypefactoryRrepid.transcode());
				TRACE1(LEVEL6,"Assembly: the value factory Repid is %s.\n",valueFactoryRepid);
				
				CORBA::String_var valueFactoryEntryPoint=CORBA::string_dup(valueEntryPoint.transcode());
				TRACE1(LEVEL6,"Assembly: the value factory EntryPoint is %s.\n",valueFactoryEntryPoint);
				
				CORBA::String_var valueFactoryFactoryEntryPoint=CORBA::string_dup(factoryEntryPoint.transcode());
				TRACE1(LEVEL6,"Assembly: the value factory FactoryEntryPoint is %s.\n",valueFactoryFactoryEntryPoint);
				
				
				bool cover=false;
				int i=0;
				
				while((!cover)&&(i<spValueFactoryRepids->length()))				
				{
					if(strcmp(spValueFactoryRepids[(CORBA::ULong)i],valueFactoryRepid)==0)
					{
						ValueFactoryNames[(CORBA::ULong)i]=valueFactoryName;
						ValueFactoryFactoryEntrys[(CORBA::ULong)i]=valueFactoryFactoryEntryPoint;
						ValueFactoryEntrys[(CORBA::ULong)i]=valueFactoryEntryPoint;
						
						cover=true;
					}
				}

				TRACE0(LEVEL5,"Assembly: after while.\n");
				
				if(!cover)
				{					
					int len=ValueFactoryRepids->length();
					
					ValueFactoryNames->length(len+1);
					ValueFactoryFactoryEntrys->length(len+1);
					ValueFactoryEntrys->length(len+1);
					ValueFactoryRepids->length(len+1);				
					
					ValueFactoryRepids[(CORBA::ULong)len]=valueFactoryRepid;				
					ValueFactoryNames[(CORBA::ULong)len]=valueFactoryName;
					ValueFactoryFactoryEntrys[(CORBA::ULong)len]=valueFactoryFactoryEntryPoint;
					ValueFactoryEntrys[(CORBA::ULong)len]=valueFactoryEntryPoint;
				}
				TRACE0(LEVEL5,"Assembly: after cover.\n");				
				
			}
						
		}
		
		TRACE0(LEVEL6,"Assembly: the impl child is not dependency .\n");
		
	}	
	
	//
	//add the value factory information into the cfValues
	//
	TRACE0(LEVEL5,"Assembly: add the value factory information into the cfValues.\n");
	CORBA::String_var value;
	CORBA::Any_var anyValue=new CORBA::Any;
	ConfigValue_var cfValue;
	int len=cfValues.length();
	
	if((ValueFactoryNames->length())>0)
	{	
		//
		//add ValueFactoryDllNames
		//

		anyValue<<=ValueFactoryNames.in();	
		cfValue=new ConfigValue_impl("ValueFactoryDllNames",anyValue.in());	
		
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;
		
		//
		//add ValueFactoryFactoryEntryPoint
		//
		
		for(int j=0;j<ValueFactoryFactoryEntrys->length();j++)
		{			
			TRACE1(LEVEL6,"Assembly: the factory entry point is %s.\n",ValueFactoryFactoryEntrys[(CORBA::ULong)j].in());
		}
				
		
		anyValue<<=ValueFactoryFactoryEntrys.in();
		cfValue=new ConfigValue_impl("ValueFactoryEntryPoints",anyValue.in());		
		
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;
		
		//
		//add ValueEntryPoints
		//
		anyValue<<=ValueFactoryEntrys.in();
		cfValue=new ConfigValue_impl("ValueEntryPoints",anyValue.in());
		
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;
		
		//
		//add ValueFactoryRepIds
		//
		anyValue<<=ValueFactoryRepids.in();
		cfValue=new ConfigValue_impl("ValueFactoryRepIds",anyValue.in());			
		
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;
		
	}				
	
	//
	//add excutor dll info
	//
	TRACE0(LEVEL5,"Assembly: add excutor dll info.\n");
	DOM_Node codeNode=parser->findChild(implementationNode,"code");
	if(codeNode==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: code node missed.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();			
	}
	
	DOMString codeName=parser->parseFileInArchive(codeNode);
	if(codeName==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: invalid fileinarchive node of code node.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();					
	}
	
	DOM_Node codeEntry=parser->findChild(codeNode,"entrypoint");
	if(codeEntry==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: code node missed entrypoint node.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();							
	}
	
	DOM_Node entryPoint=codeEntry.getFirstChild();
	if(entryPoint==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: invalid entrypoint node.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();									
	}
	DOMString entryPointValue=entryPoint.getNodeValue();
	
	//
	//add code name
	//
	TRACE0(LEVEL5,"Assembly: add code name info.\n");	
	value=CORBA::string_dup(codeName.transcode());
	anyValue<<=value;
	
	cfValue=new ConfigValue_impl("ExecutorDllName",anyValue.in());
			
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;
	
	//
	//add code entry point
	//		
	TRACE0(LEVEL5,"Assembly: add code entry point.\n");
	value=CORBA::string_dup(entryPointValue.transcode());
	anyValue<<=value;
	
	cfValue=new ConfigValue_impl("HomeExecutorEntryPoint",anyValue.in());
			
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;
	
	
	
	//
	//add servant information
	//
	TRACE0(LEVEL5,"Assembly: add servant entry point.\n");
	DOM_Node servantcodeNode=parser->findChild(implementationNode,"servantcode");
	if(servantcodeNode==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: servantcode node missed.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();			
	}
	
	DOMString servantcodeName=parser->parseFileInArchive(servantcodeNode);
	if(servantcodeName==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: invalid fileinarchive node of servantcode node.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();					
	}
	
	DOM_Node servantcodeEntry=parser->findChild(servantcodeNode,"servantentrypoint");
	if(servantcodeEntry==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: code node missed servantentrypoint node.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();							
	}
	
	DOM_Node servantEntryPoint=servantcodeEntry.getFirstChild();
	if(servantEntryPoint==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: invalid servantentrypoint node.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();									
	}
	DOMString servantEntryPointValue=servantEntryPoint.getNodeValue();

	DOM_Node contextEntry=parser->findChild(servantcodeNode,"contextentrypoint");
	if(contextEntry==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: code node missed contextentrypoint node.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();							
	}
	
	DOM_Node contextEntryPoint=contextEntry.getFirstChild();
	if(contextEntryPoint==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file %s: invalid contextentrypoint node.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();									
	}
	DOMString contextEntryPointValue=contextEntryPoint.getNodeValue();	
	
	//
	//add servant code name
	//
	TRACE0(LEVEL5,"Assembly: add servant code name.\n");
	value=CORBA::string_dup(servantcodeName.transcode());
	anyValue<<=value;
	
	cfValue=new ConfigValue_impl("ServantDllName",anyValue.in());
			
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;
	
	//
	//add servant code entry point
	//		
	TRACE0(LEVEL5,"Assembly: add servant code entry point.\n");
	value=CORBA::string_dup(servantEntryPointValue.transcode());
	anyValue<<=value;
	
	cfValue=new ConfigValue_impl("HomeServantEntryPoint",anyValue.in());	
			
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;		

	//
	//add context code entry point
	//	
	TRACE0(LEVEL5,"Assembly: add context code entry point.\n");
	value=CORBA::string_dup(contextEntryPointValue.transcode());
	anyValue<<=value;

	cfValue=new ConfigValue_impl("ContextEntryPoint",anyValue.in());		
			
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;		
	
	
	//
	//add pss dll info
	//
	TRACE0(LEVEL5,"Assembly: add pss dll info.\n");
	DOM_Node psscodeNode=parser->findChild(implementationNode,"psscode");
	if(psscodeNode!=NULL)
	{
		DOMString psscodeName=parser->parseFileInArchive(psscodeNode);
		if(psscodeName==NULL)
		{
			TRACE1(LEVEL1,"Assembly: invalid csd file %s: invalid fileinarchive node of psscode node.\n",csdFile);
	
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}	
			
			throw InvalidConfigureFile();					
		}
		
		DOM_Node psscodeEntry=parser->findChild(psscodeNode,"entrypoint");
		if(psscodeEntry==NULL)
		{
			TRACE1(LEVEL1,"Assembly: invalid csd file %s: psscode node missed entrypoint node.\n",csdFile);
	
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}	
			
			throw InvalidConfigureFile();							
		}
		
		DOM_Node pssEntryPoint=psscodeEntry.getFirstChild();
		if(pssEntryPoint==NULL)
		{
			TRACE1(LEVEL1,"Assembly: invalid csd file %s: invalid entrypoint node of pssnode.\n",csdFile);
	
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}	
			
			throw InvalidConfigureFile();									
		}
		DOMString pssEntryPointValue=pssEntryPoint.getNodeValue();
		
		//
		//add psscode name
		//
		TRACE0(LEVEL5,"Assembly: add pss name.\n");
		value=CORBA::string_dup(psscodeName.transcode());
		anyValue<<=value;
		
		cfValue=new ConfigValue_impl("PSSFileName",anyValue.in());
				
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;
		
		//
		//add psscode entry point
		//	
		TRACE0(LEVEL5,"Assembly: add psscode entry point.\n");
		value=CORBA::string_dup(pssEntryPointValue.transcode());
		anyValue<<=value;

		cfValue=new ConfigValue_impl("PssEntryPoint",anyValue.in());		
				
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;

		
	}
		

	//
	//locate the ccd file 
	//
	TRACE0(LEVEL5,"Assembly: locate the ccd file .\n");
	CORBA::String_var csdFileName=CORBA::string_dup(strrchr(csdFile,PATH_DELILIMITOR_CHAR));
	
	int csdFileNameLen=strlen(csdFileName.in());
	int fullCsdFileLen=strlen(csdFile);	
	int compoentDirLen=fullCsdFileLen-csdFileNameLen;
	
	char *componentDirTemp=(char *)malloc(compoentDirLen+1);
	strncpy(componentDirTemp,csdFile,compoentDirLen);
	componentDirTemp[(CORBA::ULong)compoentDirLen]=NULL;		
	CORBA::String_var componentDir=CORBA::string_dup(componentDirTemp);
	free(componentDirTemp);	
	
	CORBA::String_var ccdFile;
	
	try
	{
		ccdFile=getCcdFile(parser,root,implUUID,componentDir.in());		
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE1(LEVEL1,"Assembly: get the ccd file failed in invalid csd file %s.\n",csdFile);

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
			
		throw InvalidConfigureFile();
	}
	
	
	//
	//get the confivalue from the ccd file
	//
	TRACE0(LEVEL5,"Assembly: get the confivalue from the ccd file .\n");
	try
	{
		getCcdInstallHomeCfValues(ccdFile.in(),cfValues,dyncfValues);
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE0(LEVEL1,"Assembly: get confivalue for install home from ccd file failed in invalid ccd file.\n");
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}				

		throw InvalidConfigureFile();								
	}
	
	TRACE0(LEVEL5,"Assembly: end of getcsdinstallhomecfvalues .\n");
	
}

//****************************************************************************
//get config values for install home from the ccd file
//*****************************************************************************

void 
Assembly_impl::getCcdInstallHomeCfValues(const char * ccdFile,ConfigValues & cfValues, ConfigValues &dyncfValues)
	throw ( InvalidConfigureFile)
{

	TRACE0(LEVEL5,"Assembly: getCcdInstallHomeCfValues.\n");
	
	//
	//XMLParser initialized
	//
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(ccdFile);
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: XMLParser initialized failed for csd file %s. \n",ccdFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();
	}    
	
	DOM_Node root = parser->getRootNode("corbacomponent");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: Invalid ccd file %s : have no corbacomponent element. \n",ccdFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw InvalidConfigureFile();
	}	
	
	//
	//add componentrepid
	//
	TRACE0(LEVEL5,"Assembly: getCcdInstallHomeCfValues:add componentrepid.\n");
	DOM_Node componentrepidNode=parser->findChild(root,"componentrepid");
	if(componentrepidNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: componentrepid node missed.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw InvalidConfigureFile();
	}
	
	DOMString componentrepidStr=parser->getAttr(componentrepidNode,"repid");
	if(componentrepidStr==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: componentrepid node miss repid attribute.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw InvalidConfigureFile();
	}	
	
	CORBA::String_var value=CORBA::string_dup(componentrepidStr.transcode());
	CORBA::Any_var anyValue=new CORBA::Any;
	anyValue<<=value;	
	ConfigValue_var cfValue=new ConfigValue_impl("ComponentRepositoryId",anyValue.in());
	
	int len=cfValues.length();
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;
	
	
	//
	//add homerepid
	//
	TRACE0(LEVEL5,"Assembly: getCcdInstallHomeCfValues:add homerepid.\n");
	DOM_Node homerepidNode=parser->findChild(root,"homerepid");
	if(homerepidNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: homerepid node missed.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw InvalidConfigureFile();
	}
	
	DOMString homerepidStr=parser->getAttr(homerepidNode,"repid");
	if(homerepidStr==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: homerepid node miss repid attribute.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw InvalidConfigureFile();
	}	
	
	value=CORBA::string_dup(homerepidStr.transcode());
	anyValue<<=value;	

	cfValue=new ConfigValue_impl("HomeRepositoryId",anyValue.in());

	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;	
	
	
	//
	//add componentkind 
	//
	TRACE0(LEVEL5,"Assembly: getCcdInstallHomeCfValues:add componentkind.\n");
	DOM_Node componentkindNode=parser->findChild(root,"componentkind");
	if(componentkindNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: componentkind node missed.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw InvalidConfigureFile();
	}	
	
	DOM_Node componentKindChild=parser->findChild(componentkindNode,"service");	
	if(componentKindChild==NULL)
	{
		componentKindChild=parser->findChild(componentkindNode,"session");
		if(componentKindChild==NULL)
		{
			componentKindChild=parser->findChild(componentkindNode,"process");
			if(componentKindChild==NULL)
			{
				componentKindChild=parser->findChild(componentkindNode,"entity");
				if(componentKindChild==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid ccd file: componentkind node missed specific kind node.\n");

					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}

					throw InvalidConfigureFile();
				}
				else
				{
					value=CORBA::string_dup("entity");
				}
			}
			else
			{
				value=CORBA::string_dup("process");
			}
		}
		else
		{
			value=CORBA::string_dup("session");
		}
	}
	else
	{
		value=CORBA::string_dup("service");
	}

	anyValue<<=value;	
	
	cfValue=new ConfigValue_impl("ComponentKind",anyValue.in());
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;		
	
	//
	//add servant life time
	//
	
	if(strcmp(value.in(),"service")!=0)
	{
		DOM_Node servantNode=parser->findChild(componentKindChild,"servant");
		if(servantNode==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: servant node missed .\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}

			throw InvalidConfigureFile();			
		}
		
		DOMString servantLifeTime=parser->getAttr(servantNode,"lifetime");
		if(servantLifeTime==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: servant node missed lifetime attribute.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}

			throw InvalidConfigureFile();			
		}		
		
		value=CORBA::string_dup(servantLifeTime.transcode());		
		anyValue<<=value;
		
		cfValue=new ConfigValue_impl("ServantLifeTime",anyValue.in());	
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;				
		
	}
	
	//
	//add threading
	//
	
	DOM_Node threadingNode=parser->findChild(root,"threading");
	if(threadingNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: threading node missed.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw InvalidConfigureFile();
	}
	
	DOMString policyStr=parser->getAttr(threadingNode,"policy");
	if(policyStr==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: threading node miss policy attribute.\n");
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}		
		
		throw InvalidConfigureFile();
	}	
	
	value=CORBA::string_dup(policyStr.transcode());
	anyValue<<=value;
	
	cfValue=new ConfigValue_impl("Threading",anyValue.in());

	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;			

	//
	//add transaction policy for all component
	//
	
	DOM_Node transactionNode=parser->findChild(root,"transaction");
	if(transactionNode!=NULL)
	{		
		DOMString useStr=parser->getAttr(transactionNode,"use");
		
		if(useStr==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: transaction node miss use attribute.\n");
			
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			
			
			throw InvalidConfigureFile();
		}	
		
		value=CORBA::string_dup(useStr.transcode());
		anyValue<<=value;
		
		cfValue=new ConfigValue_impl("ComponentTxnPolicy",anyValue.in());			
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;		
		
	}

	
	//
	//add configuration complete 
	//	
		
	DOM_Node configurationcompleteNode=parser->findChild(root,"configurationcomplete");
	if(configurationcompleteNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: configurationcomplete node missed.\n");
	
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
	
		throw InvalidConfigureFile();
	}
	
	DOMString setStr=parser->getAttr(configurationcompleteNode,"set");
	if(setStr==NULL)
	{
		TRACE0(LEVEL1,"Assembly: invalid ccd file: configurationcomplete node miss set attribute.\n");

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}

		throw InvalidConfigureFile();
	}	
	
	value=CORBA::string_dup(setStr.transcode());
	anyValue<<=value;	
	
	cfValue=new ConfigValue_impl("ConfigurationComplete",anyValue.in());	

	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;		
	
	
	//
	//add pss setting if the component need containermanaged persistence
	//
	DOM_Node segmentNode=parser->findChild(root,"segment");
	if(segmentNode!=NULL)
	{
		DOM_Node containermanagedpersistenceNode=parser->findChild(segmentNode,"containermanagedpersistence");
		if(containermanagedpersistenceNode==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: containermanagedpersistence node missed.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}

			throw InvalidConfigureFile();			
		}
		
		//
		//add storage home
		//
		DOM_Node storagehomeNode=parser->findChild(containermanagedpersistenceNode,"storagehome");
		if(storagehomeNode==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: containermanagedpersistence node missed storage home node.\n");
	
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}	
	
			throw InvalidConfigureFile();						
		}
		DOMString storagehomeStr=parser->getAttr(storagehomeNode,"id");
		if(storagehomeStr==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: storage home miss id attribute.\n");
			
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			
			
			throw InvalidConfigureFile();									
		}
		
		value=CORBA::string_dup(storagehomeStr.transcode());
		anyValue<<=value;	
		
		cfValue=new ConfigValue_impl("StorageHomeId",anyValue.in());	
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;			
		
		
		//
		//add access mode
		//		
		DOM_Node accessmodeNode=parser->findChild(containermanagedpersistenceNode,"accessmode");
		if(accessmodeNode==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: accessmode node missed.\n");
			
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			
			
			throw InvalidConfigureFile();						
		}
		DOMString modeStr=parser->getAttr(accessmodeNode,"mode");
		if(modeStr==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: accessmode miss mode attribute.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();									
		}
		
		value=CORBA::string_dup(modeStr.transcode());
		anyValue<<=value;	

		cfValue=new ConfigValue_impl("PSSAccessMode",anyValue.in());			
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;			

		//
		//add pss transaction
		//		
		DOM_Node psstransactionNode=parser->findChild(containermanagedpersistenceNode,"psstransaction");
		if(psstransactionNode==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: psstransaction node missed.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();						
		}
		DOMString psstransactionStr=parser->getAttr(psstransactionNode,"policy");
		if(psstransactionStr==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: pss transacton miss policy attribute.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();									
		}
		
		value=CORBA::string_dup(psstransactionStr.transcode());
		anyValue<<=value;
		
		cfValue=new ConfigValue_impl("PSSTxnPolicy",anyValue.in());	
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;			
		
		//
		//add pss isolation level
		//
		if(psstransactionStr.equals("TRANSACTIONAL"))
		{		
			DOM_Node psstransactionisolationlevelNode=parser->findChild(psstransactionNode,"psstransactionisolationlevel");
			if(psstransactionisolationlevelNode==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid ccd file: psstransactionisolationlevelNode node missed.\n");
		
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}					
		
				throw InvalidConfigureFile();						
			}
			DOMString psstransactionisolationlevelStr=parser->getAttr(psstransactionisolationlevelNode,"level");
			if(psstransactionisolationlevelStr==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid ccd file: psstransactionisolationlevel miss level attribute.\n");
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}							
				
				throw InvalidConfigureFile();									
			}
			
			value=CORBA::string_dup(psstransactionisolationlevelStr.transcode());
			anyValue<<=value;	
			
			cfValue=new ConfigValue_impl("PSSTxnIsolationLevel",anyValue.in());				
					
			cfValues.length(len+1);
			cfValues[(CORBA::ULong)len]=cfValue;
			len++;	
		}
		
		//
		//add pss parameter ,since it has not been decided ,so left to be implemented in the future
		//
		
		//
		//add pss cmp
		//
		value=CORBA::string_dup("Container");
		anyValue<<=value;	

		cfValue=new ConfigValue_impl("PSSCMP",anyValue.in());			
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;
				
	}
	else
	{
		//
		//add pss cmp
		//
		value=CORBA::string_dup("Self ");
		anyValue<<=value;	

		cfValue=new ConfigValue_impl("PSSCMP",anyValue.in());			
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;		
	}
	
	//
	//add home transaction policy
	//	
	CORBA::StringSeq_var homeTxnRepids=new CORBA::StringSeq;
	CORBA::StringSeq_var homeTxnNames=new CORBA::StringSeq;
	CORBA::StringSeq_var homeTxnPolicies=new CORBA::StringSeq;	
	
	try
	{
		parseHomeFeatures(parser,root,homerepidStr.transcode(),homeTxnRepids.inout(),homeTxnNames.inout(),homeTxnPolicies.inout());
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE1(LEVEL1,"Assembly: invalid ccd file: parse the homefeatures %s failed.\n",homerepidStr.transcode());
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}			
		
		throw InvalidConfigureFile();											
	}

	anyValue<<=homeTxnRepids.in();	
	
	cfValue=new ConfigValue_impl("HomeTxnRepId",anyValue.in());					
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;
	
	anyValue<<=homeTxnNames.in();	
	
	cfValue=new ConfigValue_impl("HomeMethodTxnNames",anyValue.in());					
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;
		
	anyValue<<=homeTxnPolicies.in();	
	
	cfValue=new ConfigValue_impl("HomeMethodTxnPolicies",anyValue.in());	
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;	
	len++;	
	
	//
	//parse component policy ,including transaction policy and event policy
	//	
	CORBA::StringSeq_var componentTxnRepids=new CORBA::StringSeq;
	CORBA::StringSeq_var componentTxnNames=new CORBA::StringSeq;
	CORBA::StringSeq_var componentTxnPolicies=new CORBA::StringSeq;
		
	CORBA::StringSeq_var eventPortNames=new CORBA::StringSeq;
	CORBA::StringSeq_var eventPortPolicies=new CORBA::StringSeq;	
	
	try
	{
		parseComponentFeatures(parser,root,componentrepidStr.transcode(),componentTxnRepids.inout(),componentTxnNames.inout(),componentTxnPolicies.inout(),eventPortNames.inout(),eventPortPolicies.inout());
	}
	catch(const InvalidConfigureFile &)
	{
		TRACE1(LEVEL1,"Assembly: invalid ccd file: parse the componentfeatures %s failed.\n",componentrepidStr.transcode());

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}			

		throw InvalidConfigureFile();	
	}

	anyValue<<=componentTxnRepids.in();	

	cfValue=new ConfigValue_impl("ComponentTxnRepId",anyValue.in());		
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;
	
	anyValue<<=componentTxnNames.in();	

	cfValue=new ConfigValue_impl("ComponentMethodTxnNames",anyValue.in());		
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;	
		
	anyValue<<=componentTxnPolicies.in();	

	cfValue=new ConfigValue_impl("ComponentMethodTxnPolicies",anyValue.in());			
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;	
	len++;	

	anyValue<<=eventPortNames.in();	
	
	cfValue=new ConfigValue_impl("ComponentEventPortNames",anyValue.in());				
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;
	len++;	
		
	anyValue<<=eventPortPolicies.in();
		
	cfValue=new ConfigValue_impl("ComponentEventPortPolicies",anyValue.in());					
	
	cfValues.length(len+1);
	cfValues[(CORBA::ULong)len]=cfValue;	
	len++;
		
	//
	//add home property for assembly not for install home 
	//the home property and componentproperty are the last two of the config values
	//while assembly get them, it will remove them from the confivalues
	//
	
	DOM_Node homepropertiesNode=parser->findChild(root,"homeproperties");
	if(homepropertiesNode!=NULL)
	{	
		DOMString homepropertiesFile=parser->parseFileInArchive(homepropertiesNode);
		if(homepropertiesFile==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: invalid file in archive node of homeproperties.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();
		}	
	
		value=CORBA::string_dup(homepropertiesFile.transcode());
		anyValue<<=value;
		
		cfValue=new ConfigValue_impl("HomePropertyFile",anyValue.in());			
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;			
	}

	DOM_Node componentpropertiesNode=parser->findChild(root,"componentproperties");
	if(componentpropertiesNode!=NULL)
	{	
		DOMString componentpropertiesFile=parser->parseFileInArchive(componentpropertiesNode);
		if(componentpropertiesFile==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: invalid file in archive node of componentproperties.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();
		}	
	
		value=CORBA::string_dup(componentpropertiesFile.transcode());
		anyValue<<=value;	
		
		cfValue=new ConfigValue_impl("ComponentPropertyFile",anyValue.in());					
	
		cfValues.length(len+1);
		cfValues[(CORBA::ULong)len]=cfValue;
		len++;			
	}	
	
	//add by wsf 2004.6.23
	//get dynamic deploy info in ccd file
	DOM_Node homedyndeploypropertiesNode = parser->findChild(root, "homedyndeployproperties");
	if (homedyndeploypropertiesNode != NULL)
	{
		DOM_Node initiativeNode = parser->findChild(homedyndeploypropertiesNode, "initiative");

		DOMString set = parser->getAttr(initiativeNode, "set");
		if (set == NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: initiative node has no set attribute.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();
		}	

		value=CORBA::string_dup(set.transcode());
		anyValue<<=value;	
		cfValue=new ConfigValue_impl("homeInitiative",anyValue.in());
		
		len=dyncfValues.length();
		dyncfValues.length(len+1);
		dyncfValues[(CORBA::ULong)len]=cfValue;
		len++;
	}

	DOM_Node componentdyndeploypropertiesNode = parser->findChild(root, "componentdyndeployproperties");
	if (componentdyndeploypropertiesNode != NULL)
	{
		DOM_Node initiativeNode = parser->findChild(componentdyndeploypropertiesNode, "initiative");

		DOMString set = parser->getAttr(initiativeNode, "set");
		if (set == NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: initiative node has no set attribute.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();
		}	

		value=CORBA::string_dup(set.transcode());
		anyValue<<=value;	
		cfValue=new ConfigValue_impl("componentInitiative",anyValue.in());
		
		len=dyncfValues.length();
		dyncfValues.length(len+1);
		dyncfValues[(CORBA::ULong)len]=cfValue;
		len++;

		DOM_Node runcompleteNode = parser->findChild(componentdyndeploypropertiesNode, "runcomplete");
		set = parser->getAttr(initiativeNode, "set");
		if (set == NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: initiative node has no set attribute.\n");

			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}			

			throw InvalidConfigureFile();
		}	

		value=CORBA::string_dup(set.transcode());
		anyValue<<=value;	
		cfValue=new ConfigValue_impl("runcomplete",anyValue.in());
		
		dyncfValues.length(len+1);
		dyncfValues[(CORBA::ULong)len]=cfValue;
		len++;	

		DOM_NodeList children;
       	unsigned int childrenLen;		
		
		DOM_Node child;
		DOMString childName;
		DOMString portname;
		
		DOM_Node runcompleteusesportsNode = parser->findChild(runcompleteNode, "usesports");
		if (runcompleteusesportsNode != NULL)
		{
			children=runcompleteusesportsNode.getChildNodes();
	       	childrenLen=children.getLength();		
				
			for(unsigned int iterator=0;iterator<childrenLen;iterator++)
			{
				child = children.item(iterator);
				childName = child.getNodeName();

				if (childName.equals("port"))
				{
					portname = parser->getAttr(child, "name");
					if (portname == NULL)
					{
						TRACE0(LEVEL1,"Assembly: invalid ccd file: port node has no name attribute.\n");

						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}			

						throw InvalidConfigureFile();					
					}
					value=CORBA::string_dup(portname.transcode());
					anyValue<<=value;	
					cfValue=new ConfigValue_impl("runcompleteusesport",anyValue.in());
					
					dyncfValues.length(len+1);
					dyncfValues[(CORBA::ULong)len]=cfValue;
					len++;	
				}	
			}		
		}

		//add configurationcompleteusesports
		DOM_Node configurationcompleteusesportsNode = parser->findChild(configurationcompleteNode, "usesports");
		if (configurationcompleteusesportsNode !=NULL )
		{
			children=configurationcompleteusesportsNode.getChildNodes();
	       	childrenLen=children.getLength();		
				
			for(unsigned int iterator=0;iterator<childrenLen;iterator++)
			{
				child = children.item(iterator);
				childName = child.getNodeName();

				if (childName.equals("port"))
				{
					portname = parser->getAttr(child, "name");
					if (portname == NULL)
					{
						TRACE0(LEVEL1,"Assembly: invalid ccd file: port node has no name attribute.\n");

						if(parser!=NULL)
						{
							delete parser;
							parser=NULL;
						}			

						throw InvalidConfigureFile();					
					}
					value=CORBA::string_dup(portname.transcode());
					anyValue<<=value;	
					cfValue=new ConfigValue_impl("configurationcompleteusesport",anyValue.in());
					
					dyncfValues.length(len+1);
					dyncfValues[(CORBA::ULong)len]=cfValue;
					len++;	
				}	
			}		
		}	

		DOM_Node initialrequestportsNode = parser->findChild(componentdyndeploypropertiesNode, "initialrequestports");

		children=initialrequestportsNode.getChildNodes();
       	childrenLen=children.getLength();		
		unsigned int iterator = 0;	
		for(iterator=0;iterator<childrenLen;iterator++)
		{
			child = children.item(iterator);
			childName = child.getNodeName();

			if (childName.equals("port"))
			{
				portname = parser->getAttr(child, "name");
				if (portname == NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid ccd file: port node has no name attribute.\n");

					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}			

					throw InvalidConfigureFile();					
				}
				value=CORBA::string_dup(portname.transcode());
				anyValue<<=value;	
				cfValue=new ConfigValue_impl("initialrequestport",anyValue.in());
				
				dyncfValues.length(len+1);
				dyncfValues[(CORBA::ULong)len]=cfValue;
				len++;	
			}	
		}

		DOM_Node crypticconnectionsNode = parser->findChild(componentdyndeploypropertiesNode, "crypticconnections");
		children = crypticconnectionsNode.getChildNodes();
		childrenLen = children.getLength();

		CrypticConnection conn;
		unsigned int interator = 0;
		for(iterator=0;iterator<childrenLen;iterator++)
		{
			child = children.item(iterator);
			childName = child.getNodeName();

			if (childName.equals("connection"))
			{
			 	DOM_Node portNode = parser->findChild(child, "port");
				DOMString name = parser->getAttr(portNode, "name");
				if (name == NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid ccd file: port node has no name attribute.\n");

					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}			

					throw InvalidConfigureFile();	
				}
				conn.srcport = CORBA::string_dup(name.transcode());

				DOM_Node targetportsNode = parser->findChild(child, "targetports");
				DOM_NodeList targetportsChildren = targetportsNode.getChildNodes();
				unsigned int targetportsChildrenLen = targetportsChildren.getLength();
				DOM_Node targetportsChild;
				DOMString targetportsChildName;

				int clen = 0;
				for (unsigned int iter=0; iter<targetportsChildrenLen; iter++)
				{
					targetportsChild = targetportsChildren.item(iter);
					targetportsChildName = targetportsChild.getNodeName();
					if (targetportsChildName.equals("port"))
					{
						name = parser->getAttr(targetportsChild, "name");
						if (name == NULL)
						{
							TRACE0(LEVEL1,"Assembly: invalid ccd file: port node has no name attribute.\n");

							if(parser!=NULL)
							{
								delete parser;
								parser=NULL;
							}			

							throw InvalidConfigureFile();	
						}

						conn.targetports.length(clen+1);
						conn.targetports[clen++] = CORBA::string_dup(name.transcode());
					}
				}

				anyValue<<=conn;	
				cfValue=new ConfigValue_impl("crypticconnection",anyValue.in());
				
				dyncfValues.length(len+1);
				dyncfValues[(CORBA::ULong)len]=cfValue;
				len++;					
			}
		}
	}	
	//end by wsf 2004.6.23
	
}


//*****************************************************************************
//get the ccd file according to the cad file with the help of XMLParser
//return the ccd file full path
//*****************************************************************************
char * 
Assembly_impl::getCcdFile(XMLParser *parser, DOM_Node root,const char *implUUID,const char *componentDir)
	throw ( InvalidConfigureFile)
{

	CORBA::String_var ccdFile;
	
	DOMString ccdFileName;
	
	DOM_Node descriptorNode=parser->findChild(root,"descriptor");
	if(descriptorNode!=NULL)
	{
		DOMString ccdFileName=parser-> parseFileInArchive(descriptorNode);
		if(ccdFileName==NULL)
		{		
			TRACE0(LEVEL1,"Assembly: invalid csd file: descriptor node miss find in archive node.\n");
			throw InvalidConfigureFile();		
		}
		
		CORBA::String_var ccdFileDir=componentDir;
		
		ccdFile=ccdFileDir;

#ifdef STARCCMSTRINGADD
		ccdFile = HelpFun::CORBA_string_add(ccdFile, CORBA::string_dup(ccdFileName.transcode()));
#else		
		ccdFile+=CORBA::string_dup(ccdFileName.transcode());
#endif		
	}	
	
	
	DOM_Node implNode=parser->findChildByAttr(root,"implementation","id",implUUID);
	if(implNode==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid csd file: implementation node %s missed.\n",implUUID);
		throw InvalidConfigureFile();				
	}
	
	DOM_Node implDescriptorNode=parser->findChild(implNode,"descriptor");
	if(implDescriptorNode==NULL)
	{
		if(descriptorNode==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid csd file: descriptor node  missed.\n");
			throw InvalidConfigureFile();						
		}		
	}
	else
	{
		ccdFileName=parser-> parseFileInArchive(implDescriptorNode);
		if(ccdFileName==NULL)
		{		
			TRACE0(LEVEL1,"Assembly: invalid csd file: descriptor node miss find in archive node.\n");
			throw InvalidConfigureFile();		
		}
		
		CORBA::String_var ccdFileDir=rootDir;

#ifdef STARCCMSTRINGADD
		ccdFileDir = HelpFun::CORBA_string_add(ccdFileDir, PATH_DELILIMITOR);
		ccdFileDir = HelpFun::CORBA_string_add(ccdFileDir, implUUID);
		ccdFileDir = HelpFun::CORBA_string_add(ccdFileDir, PATH_DELILIMITOR);
#else		
		ccdFileDir+=CORBA::string_dup(PATH_DELILIMITOR);
		ccdFileDir+=implUUID;
		ccdFileDir+=CORBA::string_dup(PATH_DELILIMITOR);
#endif		
		
		ccdFile=ccdFileDir;

#ifdef STARCCMSTRINGADD
		ccdFile = HelpFun::CORBA_string_add(ccdFile, PATH_DELILIMITOR);
		ccdFile = HelpFun::CORBA_string_add(ccdFile, CORBA::string_dup(ccdFileName.transcode()));
#else		
		ccdFile+=CORBA::string_dup(PATH_DELILIMITOR);
		ccdFile+=CORBA::string_dup(ccdFileName.transcode());
#endif		
		
	}	
	
	return ccdFile._retn();
	
}

//*******************************************************************************
//parseHomoeFeatures get the config value for install home from the home featues
//only includeing transaction policy now, security policy in the future
//*******************************************************************************

void 
Assembly_impl::parseHomeFeatures(XMLParser *parser, DOM_Node root,const char * homerepid, CORBA::StringSeq &txnRepids, CORBA::StringSeq &txnNames, CORBA::StringSeq &txnPolicies)
	throw ( InvalidConfigureFile)
{
	DOM_Node homefeaturesNode=parser->findChildByAttr(root,"homefeatures","repid",homerepid);
	if(homefeaturesNode==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid ccd file: homefeatures node of %s missed.\n",homerepid);
		throw InvalidConfigureFile();		
	}
	
	
	//
	//process the inherits home
	//
	
	DOM_Node inheritshomeNode=parser->findChild(homefeaturesNode,"inheritshome");
	if(inheritshomeNode!=NULL)
	{
		
		DOMString inheritshomerepid=parser->getAttr(inheritshomeNode,"repid");
		if(inheritshomerepid==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid ccd file: homerepid missed.\n");
			throw InvalidConfigureFile();					
		}

		try
		{
			parseHomeFeatures(parser,root,inheritshomerepid.transcode(),txnRepids,txnNames,txnPolicies);
		}
		catch(const InvalidConfigureFile &)
		{
			TRACE1(LEVEL1,"Assembly: invalid ccd file: parse home featues %s failed.\n",inheritshomerepid);
			throw InvalidConfigureFile();								
		}		
	}
	
	//
	//add the txn repid ,name and policy
	//
	
	DOM_Node operationPoliciesNode=parser->findChild(homefeaturesNode,"operationpolicies");
	if(operationPoliciesNode!=NULL)
	{

		try
		{
			parseOperationPolicies(parser,operationPoliciesNode,txnNames,txnPolicies);
			
			//
			//construct the txtRepids
			//
			for(int listNum=0;listNum<txnNames.length();listNum++)
			{
				int txnRepidsLen = txnRepids.length();
				txnRepids.length(txnRepidsLen+1);
				txnRepids[(CORBA::ULong)txnRepidsLen] = homerepid;
			}
			
		}
		catch(const InvalidConfigureFile &)
		{

			TRACE0(LEVEL1,"Assembly: invalid ccd file: parse operationpolicies failed.\n");
			throw InvalidConfigureFile();
				
		}	
		
	}
	
}	

//****************************************************************************************
//parseComponentFeatures get the config value for install home from the component featues
//only includeing transaction policy and eventpolicy now, security policy in the future
//****************************************************************************************

void 
Assembly_impl::parseComponentFeatures(XMLParser *parser, 
					DOM_Node root,const char * id,
					CORBA::StringSeq &txnRepids, 
					CORBA::StringSeq &txnNames, 
					CORBA::StringSeq &txnPolicies,					
					CORBA::StringSeq &eventPortNames, 
					CORBA::StringSeq &eventPortPolicies)
	throw ( InvalidConfigureFile)
{
	
	DOM_Node componentfeaturesNode=parser->findChildByAttr(root,"componentfeatures","repid",id);
	if(componentfeaturesNode==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid ccd file: componentfeatures node of %s missed.\n",id);
		throw InvalidConfigureFile();		
	}
	

 	DOM_NodeList children=componentfeaturesNode.getChildNodes();
        unsigned int childrenLen=children.getLength();	
	
	DOM_Node child;
	DOMString childName;
		
	for(unsigned int iterator=0;iterator<childrenLen;iterator++)
	{
		child=children.item(iterator);
		childName=child.getNodeName();	
		
		if(childName.equals("inheritscomponent"))
		{
			DOMString inheritscomponentrepid=parser->getAttr(child,"repid");
			if(inheritscomponentrepid==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid ccd file: componentrepid missed.\n");
				throw InvalidConfigureFile();					
			}
	
			try
			{
				parseComponentFeatures(parser,root,inheritscomponentrepid.transcode(),txnRepids,txnNames,txnPolicies,eventPortNames,eventPortPolicies);
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE1(LEVEL1,"Assembly: invalid ccd file: parse component featues %s failed.\n",inheritscomponentrepid);
				throw InvalidConfigureFile();								
			}			
		}
		else if(childName.equals("supportsinterface"))
		{
			
			//
			//get the transaction policies from the interfaces defination
			//
			
			DOMString interfaceRepid=parser->getAttr(child,"repid");
			if(interfaceRepid==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid ccd file: supportinterface misse repid attribute.\n");
				throw InvalidConfigureFile();	
			}
						
			CORBA::StringSeq_var interfaceTxnNames=new CORBA::StringSeq;
			CORBA::StringSeq_var interfaceTxnPolicies=new CORBA::StringSeq;
			
			try
			{
				parseInterface(parser,root,interfaceRepid.transcode(),interfaceTxnNames.inout(),interfaceTxnPolicies.inout());				
				
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE1(LEVEL1,"Assembly: invalid ccd file: parse interface %s failed.\n",interfaceRepid);
				throw InvalidConfigureFile();								
			}	
			
			//
			//get the transaction policies from the operation policies
			//
			CORBA::StringSeq_var operationTxnNames=new CORBA::StringSeq;
			CORBA::StringSeq_var operationTxnPolicies=new CORBA::StringSeq;			
			
			DOM_Node operationPoliciesNode=parser->findChild(child,"operationpolicies");
			if(operationPoliciesNode!=NULL)
			{		
				
				try
				{
					parseOperationPolicies(parser,operationPoliciesNode,operationTxnNames.inout(),operationTxnPolicies.inout());				
					
				}
				catch(const InvalidConfigureFile &)
				{
					TRACE0(LEVEL1,"Assembly: invalid ccd file: parse operationpolicies failed.\n");
					throw InvalidConfigureFile();								
				}					
			}
			
			
			//
			//merge this two list ,the operationTxnPolicies will cover the interfaceTxnPolicies
			//
			
			int methodNum = 0;
					
			for(int i=0;i<operationTxnNames->length();i++)
			{				
				
				bool cover=false;
				int j=0;
				
				while((!cover)&&(j<interfaceTxnNames->length()))
				{
					if(strcmp(interfaceTxnNames[(CORBA::ULong)j],operationTxnNames[(CORBA::ULong)i])==0)
					{						
						interfaceTxnPolicies[(CORBA::ULong)j]=operationTxnPolicies[(CORBA::ULong)i];
						cover=true;
					}
					j++;
				}
				
				if(!cover)
				{
					int len=txnNames.length();
					txnNames.length(len+1);
					txnPolicies.length(len+1);
					methodNum++;
					
					txnNames[(CORBA::ULong)len]=operationTxnNames[(CORBA::ULong)i];					
					txnPolicies[(CORBA::ULong)len]=operationTxnPolicies[(CORBA::ULong)i];
														
				}				
			}
			
			int len=txnNames.length();						
			for(int j=0;j<interfaceTxnNames->length();j++)
			{
				txnNames.length(len+1);
				txnPolicies.length(len+1);
				methodNum++;
				
				txnNames[(CORBA::ULong)len]=interfaceTxnNames[(CORBA::ULong)j];
				txnPolicies[(CORBA::ULong)len]=interfaceTxnPolicies[(CORBA::ULong)j];				
				
				len++;
			}
			
			//
			//construct the txnRepids
			//
			
			//modified at 2004.4.19
//			for(int listNum=0;listNum<operationTxnNames->length();listNum++)
//			for(int listNum=0;listNum<methodNum;listNum++)
//			{
//				int txnRepidsLen = txnRepids.length();
//				txnRepids.length(txnRepidsLen+1);
////				CORBA::String_var txnRepid = CORBA::string_dup(interfaceRepid.transcode());
//				txnRepids[(CORBA::ULong)txnRepidsLen] = CORBA::string_dup(interfaceRepid.transcode());
//			}			

			
			for(int listNum=0;listNum<methodNum;listNum++)
			{
				int txnRepidsLen = txnRepids.length();
				txnRepids.length(txnRepidsLen+1);
//				CORBA::String_var txnRepid = CORBA::string_dup(interfaceRepid.transcode());
				txnRepids[(CORBA::ULong)txnRepidsLen] = CORBA::string_dup(id);
			}				
								
		}
		else if(childName.equals("ports"))
		{
		 	DOM_NodeList portChildren=child.getChildNodes();
		        unsigned int portChildrenLen=portChildren.getLength();	
			
			DOM_Node portChild;
			DOMString portChildName;
				
			for(unsigned int iterator=0;iterator<portChildrenLen;iterator++)
			{
				portChild=portChildren.item(iterator);
				portChildName=portChild.getNodeName();			
				
				if(portChildName.equals("provides"))
				{
					
					//
					//get the transaction policies from the interfaces defination
					//
					
					DOMString interfaceRepid=parser->getAttr(portChild,"repid");
					if(interfaceRepid==NULL)
					{
						TRACE0(LEVEL1,"Assembly: invalid ccd file: provide miss repid attribute.\n");
						throw InvalidConfigureFile();	
					}
					
					CORBA::StringSeq_var interfaceTxnNames=new CORBA::StringSeq;
					CORBA::StringSeq_var interfaceTxnPolicies=new CORBA::StringSeq;
					
					try
					{
						parseInterface(parser,root,interfaceRepid.transcode(),interfaceTxnNames.inout(),interfaceTxnPolicies.inout());				
						
					}
					catch(const InvalidConfigureFile &)
					{
						TRACE1(LEVEL1,"Assembly: invalid ccd file: parse interface %s failed.\n",interfaceRepid);
						throw InvalidConfigureFile();								
					}	
					
					//
					//get the transaction policies from the operation policies
					//
					CORBA::StringSeq_var operationTxnNames=new CORBA::StringSeq;
					CORBA::StringSeq_var operationTxnPolicies=new CORBA::StringSeq;					
					
					DOM_Node operationPoliciesNode=parser->findChild(portChild,"operationpolicies");
					if(operationPoliciesNode!=NULL)
					{		

						
						try
						{
							parseOperationPolicies(parser,operationPoliciesNode,operationTxnNames.inout(),operationTxnPolicies.inout());				
							
						}
						catch(const InvalidConfigureFile &)
						{
							TRACE0(LEVEL1,"Assembly: invalid ccd file: parse operationpolicies failed.\n");
							throw InvalidConfigureFile();								
						}					
					}
					
					
					//
					//merge this two list ,the operationTxnPolicies will cover the interfaceTxnPolicies
					//
					int methodNum = 0;
							
					for(int i=0;i<operationTxnNames->length();i++)
					{				
						
						bool cover=false;
						int j=0;
						
						while((!cover)&&(j<interfaceTxnNames->length()))
						{
							if(strcmp(interfaceTxnNames[(CORBA::ULong)j],operationTxnNames[(CORBA::ULong)i])==0)
							{						
								interfaceTxnPolicies[(CORBA::ULong)j]=operationTxnPolicies[(CORBA::ULong)i];
								cover=true;
							}
							j++;
						}
						
						if(!cover)
						{
							int len=txnNames.length();
							txnNames.length(len+1);
							txnPolicies.length(len+1);
							methodNum++;
							
							txnNames[(CORBA::ULong)len]=operationTxnNames[(CORBA::ULong)i];					
							txnPolicies[(CORBA::ULong)len]=operationTxnPolicies[(CORBA::ULong)i];
																
						}				
					}
					
					int len=txnNames.length();						
					for(int j=0;j<interfaceTxnNames->length();j++)
					{
						txnNames.length(len+1);
						txnPolicies.length(len+1);
						methodNum++;
						
						txnNames[(CORBA::ULong)len]=interfaceTxnNames[(CORBA::ULong)j];
						txnPolicies[(CORBA::ULong)len]=interfaceTxnPolicies[(CORBA::ULong)j];				
						
						len++;
					}										

					//
					//construct the txnRepids
					//
//					for(int listNum=0;listNum<operationTxnNames->length();listNum++)
					for(int listNum=0;listNum<methodNum;listNum++)
					{
						int txnRepidsLen = txnRepids.length();
						txnRepids.length(txnRepidsLen+1);
						CORBA::String_var txnRepid = CORBA::string_dup(interfaceRepid.transcode());
						txnRepids[(CORBA::ULong)txnRepidsLen] = txnRepid;
					}		
					
				}
				else if(portChildName.equals("emits")||
					portChildName.equals("publishes")||
					portChildName.equals("consumes"))
				{
					//
					//add event pocliy
					//
					
					DOM_Node eventpolicyNode=parser->findChild(portChild,"eventpolicy");
					if(eventpolicyNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: invalid ccd file: eventpolicy node missed.\n");						
						throw InvalidConfigureFile();						
					}
					
					DOMString eventPolicy=parser->getAttr(eventpolicyNode,"policy");
					if(eventPolicy==NULL)
					{
						TRACE0(LEVEL1,"Assembly: invalid ccd file: event policy attribute missed.\n");						
						throw InvalidConfigureFile();												
					}
					
					DOMString eventportName;
					if(portChildName.equals("emits"))
					{
						eventportName=parser->getAttr(portChild,"emitsname");	
					}
					else if(portChildName.equals("publishes"))
					{
						eventportName=parser->getAttr(portChild,"publishesname");	
					}
					else if(portChildName.equals("consumes"))
					{
						eventportName=parser->getAttr(portChild,"consumesname");	
					}
					
					if(eventportName==NULL)
					{
						TRACE0(LEVEL1,"Assembly: invalid ccd file: event port name attribute missed.\n");						
						throw InvalidConfigureFile();												
					}
					
					int len=eventPortNames.length();
					
					eventPortNames.length(len+1);
					eventPortPolicies.length(len+1);
					
					CORBA::String_var portName=CORBA::string_dup(eventportName.transcode());
					CORBA::String_var portPolicy=CORBA::string_dup(eventPolicy.transcode());
					
					eventPortNames[(CORBA::ULong)len]=portName;
					eventPortPolicies[(CORBA::ULong)len]=portPolicy;
					
				}
				
			}
			
		}
		
	}
	
}	


//*******************************************************************************
//parseInterface get the config value for install home from the interface
//only includeing transaction policy now, security policy in the future
//*******************************************************************************

void 
Assembly_impl::parseInterface(XMLParser *parser, DOM_Node root,const char * id, CORBA::StringSeq &txnNames, CORBA::StringSeq &txnPolicies)
	throw ( InvalidConfigureFile)
{
	DOM_Node interfaceNode=parser->findChildByAttr(root,"interface","repid",id);
	if(interfaceNode==NULL)
	{
		TRACE1(LEVEL1,"Assembly: invalid ccd file: interface node of %s missed.\n",id);
		throw InvalidConfigureFile();		
	}
	
	
	//
	//process the inherits interfaces
	//
 	DOM_NodeList interfaceChildren=interfaceNode.getChildNodes();
        unsigned int interfaceChildrenLen=interfaceChildren.getLength();	
	
	DOM_Node interfaceChild;
	DOMString interfaceChildName;
		
	for(unsigned int iterator=0;iterator<interfaceChildrenLen;iterator++)
	{
		interfaceChild=interfaceChildren.item(iterator);
		interfaceChildName=interfaceChild.getNodeName();	
		
		if(interfaceChildName.equals("inheritsinterface"))
		{
			DOMString inheritsinterfacerepid=parser->getAttr(interfaceChild,"repid");
			if(inheritsinterfacerepid==NULL)
			{
				TRACE0(LEVEL1,"Assembly: invalid ccd file: interfacerepid missed.\n");
				throw InvalidConfigureFile();					
			}
	
			try
			{
				parseInterface(parser,root,inheritsinterfacerepid.transcode(),txnNames,txnPolicies);
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE1(LEVEL1,"Assembly: invalid ccd file: parse interface %s failed.\n",inheritsinterfacerepid);
				throw InvalidConfigureFile();								
			}				
		}
		else if(interfaceChildName.equals("operationpolicies"))
		{
			try
			{
				parseOperationPolicies(parser,interfaceChild,txnNames,txnPolicies);
			}
			catch(const InvalidConfigureFile &)
			{

				TRACE0(LEVEL1,"Assembly: invalid ccd file: parse operationpolicies failed.\n");
				throw InvalidConfigureFile();
				
			}			
		}
	}	

}	

//**************************************************************************************
//parse operation policy of node ,node is "operationpolicies"
//get the transaction policy and security policy
//now only get the transaction policy
//**************************************************************************************

void 
Assembly_impl::parseOperationPolicies(XMLParser *parser,DOM_Node node,CORBA::StringSeq &txnNames, CORBA::StringSeq &txnPolicies)
	throw (InvalidConfigureFile)
{

	DOM_NodeList childrenNode=node.getChildNodes();
        unsigned int childrenLen=childrenNode.getLength();	
		
	DOM_Node child;
	DOMString childName;
			
	for(unsigned int iterator=0;iterator<childrenLen;iterator++)
	{
		child=childrenNode.item(iterator);
		childName=child.getNodeName();	
			
		if(childName.equals("operation"))
		{
			DOM_Node transactionNode=parser->findChild(child,"transaction");
			if(transactionNode!=NULL)
			{
				DOMString policy=parser->getAttr(transactionNode,"use");
				if(policy==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid ccd file: transaction node miss policy attrbute.\n");
					throw InvalidConfigureFile();							
				}
					
				DOMString operationName=parser->getAttr(child,"name");
				if(operationName==NULL)
				{
					TRACE0(LEVEL1,"Assembly: invalid ccd file: operation node miss name attrbute.\n");
					throw InvalidConfigureFile();							
					
				}
					
				CORBA::String_var txName=CORBA::string_dup(operationName.transcode());
				CORBA::String_var txPolicy=CORBA::string_dup(policy.transcode());
					
				int namesLen=txnNames.length();
				txnNames.length(namesLen+1);
				txnNames[(CORBA::ULong)namesLen]=txName;
					
				int policiesLen=txnPolicies.length();
				txnPolicies.length(policiesLen+1);
				txnPolicies[(CORBA::ULong)policiesLen]=txPolicy;					
			}
		}
	}	
}


//*****************************************************************************
//get the config values of home properties according to the home property file
//create its configurator according to the configvalue
//*****************************************************************************
void 
Assembly_impl::configHome(const char *ccdPropFile, const char *cadPropFile,Components::CCMObject_ptr obj)
	throw( InvalidConfigureFile,ConfigureFailure)
{
	
	ConfigValues_var ccdConfigValues;
	ConfigValues_var cadConfigValues;
	ConfigValues_var homeConfigValues;

    	//
    	//get the config value from the ccd and cad file of the home
    	//
    	
    	if(strcmp(ccdPropFile,"")!=0)
    	{
	    	try
	    	{
	    		ccdConfigValues=getProp(ccdPropFile);
	    	}
	    	catch(const InvalidConfigureFile &)
	    	{
	    		TRACE1(LEVEL1,"	Assembly: parse the home property file %s failed ,\n",ccdPropFile); 
	    		
	    		throw InvalidConfigureFile();
	    	}
	}    
    	else if(strcmp(cadPropFile,"")!=0)
    	{
	    	try
	    	{
	    		cadConfigValues=getProp(cadPropFile);
	    	}
	    	catch(const InvalidConfigureFile &)
	    	{
	    		TRACE1(LEVEL1,"	Assembly: parse the home property from cad file %s failed ,\n",cadPropFile); 
	    		
	    		throw InvalidConfigureFile();
	    	}
    	}
    	else
    	{
    		TRACE0(LEVEL1,"Assembly: get the home property failed in both cad and ccd file is empty. \n");
    		throw InvalidConfigureFile();
    	}

	
	//
	//merge the two config values, the confivalue in the cad file will cover the same one in the ccd file
	//
	if(strcmp(ccdPropFile,"")==0)
	{
		homeConfigValues=cadConfigValues;
	}
	else if(strcmp(ccdPropFile,"")==0)
	{
		homeConfigValues=ccdConfigValues;
	}
	else
	{
		homeConfigValues=ccdConfigValues;
		
		for(int i=0;i<cadConfigValues->length();i++)
		{			
			CORBA::String_var propName=cadConfigValues[(CORBA::ULong)i]->name();
			
			bool find=false;

			//modified by xiao heping 2004/02/15
			//for(int j=0;j<homeConfigValues->length();i++)
			for(int j=0;j<ccdConfigValues->length();j++)
			//end modified
			{
				CORBA::String_var name=homeConfigValues[(CORBA::ULong)i]->name();
				if(strcmp(name.in(),propName)==0)
				{
					homeConfigValues[(CORBA::ULong)i]->value(cadConfigValues[(CORBA::ULong)i]->value());
					find=true;
				}
			}
			
			//
			//specify the new attribute in the cad file
			//
			if(!find)
			{
				int len=homeConfigValues->length();
				homeConfigValues->length(len+1);
				homeConfigValues[(CORBA::ULong)len]=cadConfigValues[(CORBA::ULong)i];				
			}
			
		}
		
	}

    	
    	TRACE0(LEVEL5,"Assembly: create the stardard confiurator object for the home .\n");
    	
    	
    	//
    	//since the this configurator will only be used by the assembly locally, so we need not 
    	//invoke it remotelly, so we only need its impl
    	//
    	StandardConfigurator_impl* cf = new StandardConfigurator_impl(orb);    

    	cf->set_configuration(homeConfigValues.in());
    	
    	try
    	{
    		cf->configure(obj);
    	}
    	catch(const Components::WrongComponentType &)
    	{
    		TRACE0(LEVEL1,"Assembly: Configure home object failed.\n"); 
    		throw ConfigureFailure();
    	}
 	
}


//*****************************************************************************
//get the config values applyed for all the component created by this home
//according to the component property file 
//create its configurator according to the configvalue
//*****************************************************************************
void
Assembly_impl::configHomeComponent(const char * propFile,Components::CCMHome_ptr home,const char *id)
		throw( InvalidConfigureFile,ConfigureFailure)
{

	ConfigValues_var homeConfigValues;

    	//
    	//get the config value from the propFile
    	//
    	
	try
    {
    	homeConfigValues=getProp(propFile);
    }
	catch(const InvalidConfigureFile &)
	{
	 	TRACE1(LEVEL1,"	Assembly: parse the home Configuration property file %s failed ,\n",propFile); 
	    		
	 	throw InvalidConfigureFile();
	}

	//
	//create the configurator object 
	//
	
    TRACE0(LEVEL5,"Assembly: create the standard configurator object for the home .\n");
    
    StandardConfigurator_impl* cfImpl = new StandardConfigurator_impl(orb);    

    cfImpl->set_configuration(homeConfigValues.in());

   	PortableServer::ServantBase_var servant = cfImpl;
    	
    //activate the Configurator corba object  	
    CORBA::String_var str=CORBA::string_dup("StandardConfigurator");

#ifdef STARCCMSTRINGADD
	str = HelpFun::CORBA_string_add(str, CORBA::string_dup(id));
	str = HelpFun::CORBA_string_add(str, assemblyUUID);
#else    
    str+=CORBA::string_dup(id);
    str+=assemblyUUID;
#endif    
 	
    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId(str.in());

    poa->activate_object_with_id(oid,cfImpl);  
    CORBA::Object_var object=poa->id_to_reference(oid);
    StandardConfigurator_var cf=StandardConfigurator::_narrow(object);

#ifdef ORBacus        
	CORBA::Object_var obj = orb -> resolve_initial_references("BootManager");
	OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);
  
  	bootManager -> add_binding(oid, cf);   
#endif
    	
#ifdef StarBus        
	CORBA::Object_var obj = orb -> resolve_initial_references("AssistAdaptorManager");
	STAR::AssistAdaptorManager_var assistAdaptorManager = STAR::AssistAdaptorManager::_narrow(obj);

    assistAdaptorManager -> binding_objref(oid, cf);   
#endif

#ifdef TAO
    CORBA::Object_var table_object = orb -> resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
      
    IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

	CORBA::String_var iorStr = orb -> object_to_string(cf);	    
	
    adapter->bind (str.in(), iorStr.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
#endif  
    
    	
    //
    //set configurator for home configuration
    //
    try
    {	
    	Components::HomeConfiguration_var homeConfig=Components::HomeConfiguration::_narrow(home);    		
    	if(CORBA::is_nil(homeConfig))
    	{
   			TRACE0(LEVEL1,"Assembly: set home Configuration Configure object failed in narrow CCMHome to HomeConfiguration.\n"); 
    		throw ConfigureFailure();    			
    	}  		
    	
    	homeConfig->set_configurator(cf.in());
    	homeConfig->complete_component_configuration(true);
    }
    catch(const CORBA::SystemException &)
    {
    	TRACE0(LEVEL1,"Assembly: set home Configuration Configure object failed in CORBA System Exception.\n"); 
    	throw ConfigureFailure();
    }	
	
}


//*****************************************************************************
//get the config values of one component instantiation according to the component 
//property file declared in the component instantiation element
//create its configurator according to the configvalue
//*****************************************************************************
void 
Assembly_impl::configComponent(const char *ccdPropFile, const char *cadPropFile,Components::CCMObject_ptr obj)
		throw( InvalidConfigureFile,ConfigureFailure)
{
	ConfigValues_var ccdConfigValues;
	ConfigValues_var cadConfigValues;
	ConfigValues_var componentConfigValues;

    	//
    	//get the config value from the ccd property file and cad property file of the home
    	//
    	
    	if(strcmp(ccdPropFile,"")!=0)
    	{
	    	try
	    	{
	    		TRACE1(LEVEL6,"Assembly: the ccd config Values file is %s.\n",ccdPropFile);
	    		ccdConfigValues=getProp(ccdPropFile);
	    		
	    	}
	    	catch(const InvalidConfigureFile &)
	    	{
	    		TRACE1(LEVEL1,"	Assembly: parse the component property file %s failed ,\n",ccdPropFile); 	    		
	    		throw InvalidConfigureFile();
	    	}
	}    
	
    	if(strcmp(cadPropFile,"")!=0)
    	{
	    	try
	    	{
	    		TRACE1(LEVEL6,"Assembly: the ccd config Values file is %s.\n",cadPropFile);
	    		cadConfigValues=getProp(cadPropFile);
	    	}
	    	catch(const InvalidConfigureFile &)
	    	{
	    		TRACE1(LEVEL1,"	Assembly: parse the component property from cad file %s failed ,\n",cadPropFile); 
	    		
	    		throw InvalidConfigureFile();
	    	}
    	}
    	
    	
    	if((strcmp(ccdPropFile,"")==0)&&(strcmp(cadPropFile,"")==0))
    	{
    		TRACE0(LEVEL1,"Assembly: get the component property failed in both cad and ccd file is empty. \n");
    		throw InvalidConfigureFile();
    	}

	
	//
	//merge the two config values, the confivalue in the cad file will cover the same one in the ccd file
	//
	TRACE0(LEVEL5,"Assembly: merge the two config values property file .\n");
	
	if(strcmp(ccdPropFile,"")==0)
	{
		componentConfigValues=cadConfigValues;
	}
	else if(strcmp(cadPropFile,"")==0)
	{
		componentConfigValues=ccdConfigValues;
	}
	else
	{
		componentConfigValues=ccdConfigValues;
		
		for(int i=0;i<cadConfigValues->length();i++)
		{			
			CORBA::String_var propName=cadConfigValues[(CORBA::ULong)i]->name();
			
			bool find=false;
			for(int j=0;j<componentConfigValues->length();j++)
			{
				CORBA::String_var name=componentConfigValues[(CORBA::ULong)j]->name();
				if(strcmp(name.in(),propName)==0)
				{
					componentConfigValues[(CORBA::ULong)j]->value(cadConfigValues[(CORBA::ULong)i]->value());
					find=true;
				}
			}
			
			//
			//specify the new attribute in the cad file
			//
			if(!find)
			{
				int len=componentConfigValues->length();
				componentConfigValues->length(len+1);
				componentConfigValues[(CORBA::ULong)len]=cadConfigValues[(CORBA::ULong)i];				
			}
			
		}
		
	}

    	
    	TRACE0(LEVEL5,"Assembly: create the stardard confiurator object for the component .\n");
    	
    	
    	//
    	//since the this configurator will only be used by the assembly locally, so we need not 
    	//invoke it remotelly, so we only need its impl
    	//
    	StandardConfigurator_impl* cf = new StandardConfigurator_impl(orb);    

    	cf->set_configuration(componentConfigValues.in());
    	
    	try
    	{
    		cf->configure(obj);
    	}
    	catch(const Components::WrongComponentType &)
    	{
    		TRACE0(LEVEL1,"Assembly: Configure component object failed.\n"); 
    		throw ConfigureFailure();
    	}
    	
}

//****************************************************************************************
//get the primary key value for the component with type of entity or process according 
//to the component property file declared in the component instantiation element
//actually the process is the same as the common value type
//if every thing goes correctly ,return ConfigValue: attrName+attrValue
//if the propFIle have no primarykey node, then return NULL;
//if any error occured ,throw exception
//*****************************************************************************************
ConfigValue * 
Assembly_impl::getPrimaryKeyProp(const char * propFile)
	throw (InvalidConfigureFile)
{
		
	//XMLParser initialized
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(propFile);
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: XMLParser initialized failed for property file %s. \n",propFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();
	}    
	
	DOM_Node root = parser->getRootNode("properties");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: Invalid property file %s : have no componentassembly element. \n",propFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw InvalidConfigureFile();
	}	
	
	//
	//get the children of the root ,locate the primary key
	//and parse it through getNodeProp method to get the ConfigValue
	//
	
	
	ConfigValue_var pkValue;
		
 	DOM_Node pkNode=parser->findChild(root,"valuetype");
 	if(pkNode!=NULL)
 	{
		DOMString pktype=parser->getAttr(pkNode,"primarykey");
		if(pktype==NULL)
		{
			TRACE1(LEVEL1,"Assembly: invalid property file %s : primarykey attrbute missed.\n",propFile);
				
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}									
					
			throw InvalidConfigureFile();
		}
				
		if(pktype.equals(true))
		{
			try
			{
				pkValue=getNodeProp(parser,pkNode,false);
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE1(LEVEL1,"Assembly: get the primary key of property file %s failed. \n",propFile);
	
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}										
						
				throw InvalidConfigureFile();
			}
							
		}

 	}


	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}
	
	
	return pkValue._retn();
	
}


//****************************************************************************************
//get the configvalues according to the property file
//*****************************************************************************************
ConfigValues * 
Assembly_impl::getProp(const char * propFile)
	throw(InvalidConfigureFile)
{
	
	//XMLParser initialized
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(propFile);
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"Assembly: XMLParser initialized failed for property file %s. \n",propFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw InvalidConfigureFile();
	}    
	
	DOM_Node root = parser->getRootNode("properties");
	
	if(root==NULL)
	{
		TRACE1(LEVEL1,"Assembly: Invalid property file %s : have no componentassembly element. \n",propFile);		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw InvalidConfigureFile();
	}	
	
	
	//get the children of the root ,and process the child of the root one by one
	//if the node is the valuetype of primary key ,the do not process it in this process
	//because we have get the pk from the getPrimaryKeyProp
	
	ConfigValue_var valueElem;
	ConfigValues_var valueList=new ConfigValues;
	int len=valueList->length();
		
 	DOM_NodeList rootChildren=root.getChildNodes();
        unsigned int rootChildrenLen=rootChildren.getLength();	
	
	DOM_Node rootChild;
	DOMString rootChildName;
		
	for(unsigned int iterator=0;iterator<rootChildrenLen;iterator++)
	{
		rootChild=rootChildren.item(iterator);
		rootChildName=rootChild.getNodeName();	
		
		if((rootChildName.equals("simple"))||
		   (rootChildName.equals("sequence"))||
		   (rootChildName.equals("struct")))
		{
			
			try
			{
				if(rootChildName.equals("sequence"))
				{
					valueElem=getNodeProp(parser,rootChild,true);
				}
				else
				{
					valueElem=getNodeProp(parser,rootChild,false);
				}
			}
			catch(const InvalidConfigureFile &)
			{
				TRACE1(LEVEL1,"Assembly: invalid property file %s : primarykey attrbute missed.\n",propFile);
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}					
				
				throw InvalidConfigureFile();
			}
						
			valueList->length(len+1);
			valueList[(CORBA::ULong)len]=valueElem;
			len++;
		}
		else if(rootChildName.equals("valuetype"))
		{
			DOMString pktype=parser->getAttr(rootChild,"primarykey");
			if(pktype==NULL)
			{
				TRACE1(LEVEL1,"Assembly: invalid property file %s : primarykey attrbute missed.\n",propFile);
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}									
				
				throw InvalidConfigureFile();
			}
			
			if(pktype.equals(false))
			{
				try
				{
					valueElem=getNodeProp(parser,rootChild,false);
				}
				catch(const InvalidConfigureFile &)
				{
					TRACE1(LEVEL1,"Assembly: invalid property file %s : parse the valuetype failed.\n",propFile);

					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}										
					
					throw InvalidConfigureFile();
				}
							
				valueList->length(len+1);
				valueList[(CORBA::ULong)len]=valueElem;
				len++;							
			}
			
		}
	}

	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}

	return valueList._retn();	
	
}



//****************************************************************************************
//get the configvalue of the node in the property file 
//it is an nesting process
//****************************************************************************************
ConfigValue * 
Assembly_impl::getNodeProp(XMLParser *parser,DOM_Node node,bool seqMem)
	throw(InvalidConfigureFile)
{	
	
	DOMString nodeType=node.getNodeName();
	
	CORBA::Any_var anyValue=new CORBA::Any;	
	
	//
	//get the DynAnyFactory
	//
	CORBA::Object_var objFact;
	objFact = orb -> resolve_initial_references("DynAnyFactory");
	DynAnyFactory_var factory = DynAnyFactory::_narrow(objFact);	
	
	
	if(nodeType.equals("simple"))
	{
		
		CORBA::String_var value;
		try
		{
			value=getValue(parser,node);
		}
		catch(const InvalidConfigureFile &)
		{
			TRACE1(LEVEL1,"Assembly: invalid property file: invalid node %s.\n", nodeType.transcode());
			throw;
		}
		
		
		DOMString propType=parser->getAttr(node,"type");
		if(propType==NULL)
		{
			TRACE1(LEVEL1,"Assembly: invalid property file: node %s miss type.\n",nodeType.transcode());
			throw InvalidConfigureFile();			
		}
		
		
		if(propType.equals("boolean"))
		{
			CORBA::Boolean valueBool;
			if(strcmp(value.in(),"true")==0)
			{
				valueBool=true;				
			}
			else
			{
				valueBool=false;				
			}
					
			anyValue<<=CORBA::Any::from_boolean(valueBool);						
		}
		else if(propType.equals("char"))
		{			
			CORBA::Char valueChar=(value.in())[0];		
			
			anyValue<<=CORBA::Any::from_char(valueChar);
		}
		else if(propType.equals("double"))
		{
			CORBA::Double valueDouble=atof(value.in());
			
			anyValue<<=valueDouble;
		}		
		else if(propType.equals("float"))
		{
			CORBA::Float valueFloat=atof(value.in());
			
			anyValue<<=valueFloat;
		}		
		else if(propType.equals("short"))
		{
			CORBA::Short valueShort=atoi(value.in());
			
			anyValue<<=valueShort;
		}				
		else if(propType.equals("long"))
		{
			CORBA::Long valueLong=atol(value.in());
			
			anyValue<<=valueLong;
		}				
		else if(propType.equals("objref"))
		{
			CORBA::Object_var obj;
    			try
    			{
        			obj  = orb -> string_to_object(value.in());
    			}
    			catch(const CORBA::BAD_PARAM&)
    			{
    				TRACE0(LEVEL1,"Assembly: objectref string to object failed.\n");
    				throw InvalidConfigureFile();
    			}

    			if(CORBA::is_nil(obj))
		    	{
				TRACE0(LEVEL1,"Assembly: can not get the object ref from objectref.\n");				
				throw InvalidConfigureFile();
		    	}					
			
			anyValue<<=obj;
		}				
		else if(propType.equals("octet"))
		{			
			int val;
    			char hex[3];
			const char * str=value.in();

    			hex[2] = '\0';
			hex[0]=*str++;
			hex[1]=*str++;
		
			sscanf(hex, "%x", &val);			
			CORBA::Octet *oct =new CORBA::Octet[1];
			oct[0]=val;
						
			anyValue<<=CORBA::Any::from_octet(oct[0]);
		}				
		else if(propType.equals("string"))
		{
			const char *valueString=value.in();
			
			anyValue<<=valueString;
		}											
		else if(propType.equals("ulong"))
		{
			CORBA::ULong valueULong=atol(value.in());
			
			anyValue<<=valueULong;
		}											
		else if(propType.equals("ushort"))
		{
			CORBA::UShort valueUShort=atol(value.in());
			
			anyValue<<=valueUShort;
		}		
		
	}
	else if(nodeType.equals("sequence"))
	{

	 	DOM_NodeList sequenceChildren=node.getChildNodes();
	        unsigned int sequenceChildrenLen=sequenceChildren.getLength();	
		
		CORBA::TypeCode_var elemType;
		
		//CORBA::AnySeq_var members=new CORBA::AnySeq;
		DynamicAny::AnySeq_var members=new DynamicAny::AnySeq;
		
		int membersLen=members->length();
		
		DOM_Node sequenceChild;
		DOMString sequenceChildName;
			
		for(unsigned int iterator=0;iterator<sequenceChildrenLen;iterator++)
		{
			sequenceChild=sequenceChildren.item(iterator);
			sequenceChildName=sequenceChild.getNodeName();	
			

			
			if(sequenceChildName.equals("simple")||
			   sequenceChildName.equals("sequence")||
			   sequenceChildName.equals("struct")||
			   sequenceChildName.equals("valuetype"))
			{
				
				ConfigValue_var cfvalue;
				
				try
				{
					cfvalue=getNodeProp(parser,sequenceChild,true);
				}
				catch(const InvalidConfigureFile &)
				{
					TRACE0(LEVEL1,"Assembly: get the property failed in simple node .\n");
					throw InvalidConfigureFile();					
				}				
				
				elemType=(cfvalue->value()).type();
		
				members->length(membersLen+1);				
				members[(CORBA::ULong)membersLen]=cfvalue->value();
				membersLen++;
				
			}
			
		}
		
		//
		//create a type code for the struct
		//
		
		CORBA::TypeCode_var self_sequence_tc;
		self_sequence_tc=orb->create_sequence_tc(0,elemType.in());
		
		//
		//create the DynSequence and initialize it.
		//
		DynAny_var mydyn = factory -> create_dyn_any_from_type_code(self_sequence_tc);
		DynSequence_var ds = DynSequence::_narrow(mydyn);
		
		ds->set_elements(members);
		
		//
		//get the Any out of the DynStruct
		//
		anyValue=ds->to_any();
		
		//
		//clean up
		//	
		mydyn->destroy();
		ds->destroy();
		
	}
	else if(nodeType.equals("struct"))
	{
	 	DOM_NodeList structChildren=node.getChildNodes();
	        unsigned int structChildrenLen=structChildren.getLength();	
		
		DOM_Node structChild;
		DOMString structChildName;
		
		CORBA::StructMemberSeq_var mseq=new CORBA::StructMemberSeq;
		int mseqLen=mseq->length();
			
		DynamicAny::NameValuePairSeq_var members=new DynamicAny::NameValuePairSeq;
		int membersLen=members->length();		
			
		for(unsigned int iterator=0;iterator<structChildrenLen;iterator++)
		{
			structChild=structChildren.item(iterator);
			structChildName=structChild.getNodeName();	
			
			
			if(structChildName.equals("simple")||
			   structChildName.equals("sequence")||
			   structChildName.equals("struct")||
			   structChildName.equals("valuetype"))
			{
				
				ConfigValue_var cfvalue;
				
				try
				{
					cfvalue=getNodeProp(parser,structChild,false);
				}
				catch(const InvalidConfigureFile &)
				{
					TRACE0(LEVEL1,"Assembly: get the property failed in simple node .\n");
					throw InvalidConfigureFile();					
				}				
				
				mseq->length(mseqLen+1);
				mseq[(CORBA::ULong)mseqLen].name=cfvalue->name();
				mseq[(CORBA::ULong)mseqLen].type=(cfvalue->value()).type();
				mseq[(CORBA::ULong)mseqLen].type_def=CORBA::IDLType::_nil();
				mseqLen++;
				
				members->length(membersLen+1);
				members[(CORBA::ULong)membersLen].id=cfvalue->name();
				members[(CORBA::ULong)membersLen].value=cfvalue->value();
				membersLen++;
			}
			
		}
		
		//
		//create a type code for the struct
		//
		DOMString structType=parser->getAttr(node,"type");
		if(structType==NULL)
		{
			TRACE0(LEVEL1,"Assembly: get the property failed in type missed.\n");
			throw InvalidConfigureFile();					
		}		
		
		CORBA::TypeCode_var self_struct_tc;
		self_struct_tc=orb->create_struct_tc(structType.transcode(),"",mseq);
		
		//
		//create the DynStruct and initialize it.
		//
		DynAny_var mydyn = factory -> create_dyn_any_from_type_code(self_struct_tc);
		DynStruct_var ds = DynStruct::_narrow(mydyn);			
		
		ds->set_members(members);
		
		//
		//get the Any out of the DynStruct
		//
		anyValue=ds->to_any();
		
		//
		//clean up
		//
		mydyn->destroy();	
		ds->destroy();
						
	}
	else if(nodeType.equals("valuetype"))
	{
	 	DOM_NodeList valueChildren=node.getChildNodes();
	        unsigned int valueChildrenLen=valueChildren.getLength();	
		
		DOM_Node valueChild;
		DOMString valueChildName;

		CORBA::ValueMemberSeq_var mseq=new CORBA::ValueMemberSeq();

		int mseqLen=mseq->length();
			
		DynamicAny::NameValuePairSeq_var members=new DynamicAny::NameValuePairSeq;
		int membersLen=members->length();
			
		for(unsigned int iterator=0;iterator<valueChildrenLen;iterator++)
		{
			valueChild=valueChildren.item(iterator);
			valueChildName=valueChild.getNodeName();			
			
			
			if(valueChildName.equals("simple")||
			   valueChildName.equals("sequence")||
			   valueChildName.equals("struct")||
			   valueChildName.equals("valuetype"))
			{
				
				ConfigValue_var cfvalue;
				
				try
				{
					cfvalue=getNodeProp(parser,valueChild,false);
				}
				catch(const InvalidConfigureFile &)
				{
					TRACE0(LEVEL1,"Assembly: get the property failed in simple node .\n");
					throw InvalidConfigureFile();					
				}				
				
				mseq->length(mseqLen+1);
				mseq[(CORBA::ULong)mseqLen].name=cfvalue->name();
				mseq[(CORBA::ULong)mseqLen].type=(cfvalue->value()).type();
				mseq[(CORBA::ULong)mseqLen].access = CORBA::PUBLIC_MEMBER;
				mseqLen++;
				
				members->length(membersLen+1);
				members[(CORBA::ULong)membersLen].id=cfvalue->name();
				members[(CORBA::ULong)membersLen].value=cfvalue->value();
				membersLen++;
			}
			
		}
		
		//
		//create a type code for the struct
		//
		DOMString valueType=parser->getAttr(node,"type");
		if(valueType==NULL)
		{
			TRACE0(LEVEL1,"Assembly: get the property failed in type missed.\n");
			throw InvalidConfigureFile();					
		}		
		
		CORBA::TypeCode_var self_value_tc;
		self_value_tc=orb->create_value_tc(valueType.transcode(),"",CORBA::VM_NONE,CORBA::TypeCode::_nil(),mseq);
		
		//
		//create the DynStruct and initialize it.
		//	
		
		DynAny_var mydyn = factory -> create_dyn_any_from_type_code(self_value_tc);
		DynValue_var ds = DynValue::_narrow(mydyn);		
		
		ds->set_members(members);
		
		//
		//get the Any out of the DynStruct
		//
		anyValue=ds->to_any();
		
		//
		//clean up
		//	
		mydyn->destroy();
		ds->destroy();
						
	}

		
	
	//
	//set the name and create the ConfigValue to return
	//
		
	DOMString propName=parser->getAttr(node,"name");
	if(propName==NULL)
	{
		if(!seqMem)
		{
			TRACE1(LEVEL1,"Assembly: invalid property file: node %s miss name.\n",nodeType.transcode());
			throw InvalidConfigureFile();
		}		
			
		ConfigValue_var valueElem=new ConfigValue_impl("",anyValue.in());
		return valueElem._retn(); 
	}
	else
	{
		ConfigValue_var valueElem=new ConfigValue_impl(propName.transcode(),anyValue.in());
		return valueElem._retn(); 			
	}	
	
}

char *
Assembly_impl::getValue(XMLParser *parser,DOM_Node node)
	throw(InvalidConfigureFile)
{
	
	CORBA::String_var value;
	
	DOM_Node valueNode=parser->findChild(node,"value");
		
	if(valueNode!=NULL)
	{	
		DOM_Node valueText=valueNode.getFirstChild();
		if(valueText==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid property file: invalid node value.\n");
			throw InvalidConfigureFile();	
		}
			
		DOMString propValue=valueText.getNodeValue();
		
		//it is possibal the propValue is invalid 
		//however we will examin it in the configuration setconfiguration mehtod
		
		value=CORBA::string_dup(propValue.transcode());		
		return value._retn();
	}
	else		//default value
	{
		DOM_Node defaultValueNode=parser->findChild(node,"defaultvalue");
		if(defaultValueNode==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid property file: no value specified.\n");
			throw InvalidConfigureFile();				
		}
		
		DOM_Node valueText=defaultValueNode.getFirstChild();
		if(valueText==NULL)
		{
			TRACE0(LEVEL1,"Assembly: invalid property file: invalid defaultvalue node.\n");
			throw InvalidConfigureFile();	
		}
			
		DOMString propValue=valueText.getNodeValue();
		
		//it is possibal the propValue is invalid 
		//however we will examin it in the configuration setconfiguration mehtod
		
		value=CORBA::string_dup(propValue.transcode());		
		return value._retn();
	}
	
}

	
void
Assembly_impl::connectComponents(XMLParser *parser,DOM_Node root)
	throw (InvalidConfigureFile,ConnectFailure)
{
	//
	//locate connect
	//
	
	DOM_Node connectNode=parser->findChild(root,"connections");
	if(connectNode!=NULL)
	{

	 	DOM_NodeList connectChildren=connectNode.getChildNodes();
	        unsigned int connectChildrenLen=connectChildren.getLength();	
		
		DOM_Node connectChild;
		DOMString connectChildName;
		
		for(unsigned int iterator=0;iterator<connectChildrenLen;iterator++)
		{
			connectChild=connectChildren.item(iterator);
			connectChildName=connectChild.getNodeName();
			
			if(connectChildName.equals("connectinterface"))
			{
				//
				//get the connect interface id
				//
				DOMString connectId=parser->getAttr(connectChild,"id");
				if(connectId==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: connect node miss id attribute.\n");
					throw InvalidConfigureFile();					
				}
				
				//
				//locate the component that define the usesport
				//

				DOM_Node useNode=parser->findChild(connectChild,"usesport");
				if(useNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: usesport node missed.\n");
					throw InvalidConfigureFile();
				}
				
				DOM_Node  useIdentifierNode=parser->findChild(useNode,"usesidentifier");
				if(useIdentifierNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: usesidentifier node missed.\n");
					throw InvalidConfigureFile();					
				}
								
				
				
				DOM_Node  useIdentifierText=useIdentifierNode.getFirstChild();
				
				DOMString useIdentifier=useIdentifierText.getNodeValue();			
				
				TRACE1(LEVEL6,"Assembly: connect Component ,the uses port is %s.\n",useIdentifier.transcode());
				
				DOM_Node  componentinstantiationrefNode=parser->findChild(useNode,"componentinstantiationref");
				if(componentinstantiationrefNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
					throw InvalidConfigureFile();					
				}				
				DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
				if(idref==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
					throw InvalidConfigureFile();					
				}
				
				//modified by wsf 2004.7.21
				/*
				MapComponentInf::iterator iter=componentMap.find(string(idref.transcode()));
	
				if (iter == componentMap.end())	
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();										
				}		
				
				Components::CCMObject_var objUses=(* iter).second.ComponentRef;
				*/
				ArchComponent* acom = arch->getArchComponent(idref.transcode());
				if (acom == NULL)
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();	
				}

				CORBA::Object_var obj = acom->comref;
				Components::CCMObject_var objUses = Components::CCMObject::_narrow(obj);

				CORBA::String_var objUsesId = CORBA::string_dup(idref.transcode());

				delete acom;
				acom = NULL;

				//for count the configurationcomplete dependency
				MapComponentInf::iterator iter=componentMap.find(string(idref.transcode()));
				//end by wsf 2004.7.21			
				
				//
				//locate the object of provides,supports,homeinterface
				//
				CORBA::Object_var objUsed;				
				
				DOM_Node providesportNode=parser->findChild(connectChild,"providesport");
				if(providesportNode==NULL)
				{
					DOM_Node supportsNode=parser->findChild(connectChild,"componentsupportedinterface");
					if(supportsNode==NULL)
					{
						DOM_Node homeinterfaceNode=parser->findChild(connectChild,"homeinterface");
						if(homeinterfaceNode==NULL)
						{
							TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: invalid connect interface node.\n");
							throw InvalidConfigureFile();												
						}
						
						DOM_Node homeplacementrefNode=parser->findChild(homeinterfaceNode,"homeplacementref");
						if(homeplacementrefNode==NULL)
						{
							TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: homeplacementref node missed.\n");
							throw InvalidConfigureFile();																			
						}
						
						DOMString idref=parser->getAttr(homeplacementrefNode,"idref");
						if(idref==NULL)
						{
							TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: homeplacementref node miss idref attribute.\n");
							throw InvalidConfigureFile();					
						}
						
						//modifed by wsf 2004.7.21
						/*
						MapHomeInf::iterator theIterator=homeMap.find(string(idref.transcode()));
			
						if (theIterator == homeMap.end())	
						{
							TRACE0(LEVEL1,"Assembly: connect Components failed in idref does not refer to the valid home.\n");
							throw ConnectFailure();										
						}
						
						objUsed = CORBA::Object::_duplicate( ((* theIterator).second.HomeRef) );
						*/

						ArchHome* ahome = arch->getArchHome(idref.transcode());
						if (ahome == NULL)
						{
							TRACE0(LEVEL1,"Assembly: connect Components failed in idref does not refer to the valid home.\n");
							throw ConnectFailure();	
						}
						
						objUsed = ahome->homeref;

						delete ahome;
						ahome = NULL;
						//end by wsf 2004.7.21					

					}
					else
					{
						DOM_Node componentinstantiationrefNode=parser->findChild(supportsNode,"componentinstantiationref");
						if(componentinstantiationrefNode==NULL)
						{
							TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
							throw InvalidConfigureFile();												
						}
						
						DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
						if(idref==NULL)
						{
							TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
							throw InvalidConfigureFile();					
						}
						
						//modified by wsf 2004.7.21
						/*						
						MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
			
						if (theIterator == componentMap.end())	
						{
							TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
							throw ConnectFailure();										
						}
						
						objUsed = CORBA::Object::_duplicate( ((* theIterator).second.ComponentRef) );
						*/
						acom = arch->getArchComponent(idref.transcode());
						if (acom == NULL)
						{
							TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
							throw ConnectFailure();	
						}

						objUsed = acom->comref;
						
						//
						//add the depend information into the componentMap
						//		
						CORBA::String_var supportsComponentId=CORBA::string_dup(idref.transcode());
								
						CORBA::StringSeq_var dependList=(*iter).second.depend;
						int len=dependList->length();
						dependList->length(len+1);
						dependList[(CORBA::ULong)len]=supportsComponentId;
						(*iter).second.depend=dependList;																												

						delete acom;
						acom = NULL;
						//end by wsf 2004.7.21																													
						
					}
					
				}
				else
				{

					DOM_Node  providesIdentifierNode=parser->findChild(providesportNode,"providesidentifier");
					if(providesIdentifierNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: providesidentifier node missed.\n");
						throw InvalidConfigureFile();					
					}			
					
					DOM_Node  providesIdentifierText=providesIdentifierNode.getFirstChild();
					
					DOMString providesIdentifier=providesIdentifierText.getNodeValue();
					
					TRACE1(LEVEL6,"Assembly: connect Component ,the provides port is %s.\n",providesIdentifier.transcode());			
					
					DOM_Node  componentinstantiationrefNode=parser->findChild(providesportNode,"componentinstantiationref");
					if(componentinstantiationrefNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
						throw InvalidConfigureFile();					
					}				
					DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
						throw InvalidConfigureFile();					
					}
					
					//modified by wsf 2004.7.21
					/*
					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
					if (theIterator == componentMap.end())	
					{
						TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
						throw ConnectFailure();										
					}
					
					Components::CCMObject_var providesComponent=(* theIterator).second.ComponentRef;
					*/
					acom = arch->getArchComponent(idref.transcode());
					if (acom == NULL)
					{
						TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
						throw ConnectFailure();		
					}

					obj = acom->comref;
					Components::CCMObject_var providesComponent = Components::CCMObject::_narrow(obj);

					delete acom;
					acom = NULL;
					//end by wsf 2004.7.21				
					//
					//navigate to the facet specified by providesIdentifier
					//
					
					try
					{
						
						objUsed=providesComponent->provide_facet(providesIdentifier.transcode());
						TRACE1(LEVEL4,"Assembly: connectComponents: navigate to %s.\n",providesIdentifier.transcode());
					}
					catch(const CORBA::SystemException &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in navigate from the component to facet: SystemException happened.\n");
						throw ConnectFailure();																
					}
					catch(const Components::InvalidName &)
					{
						TRACE0(LEVEL6,"Assembly: connect Components failed in navigate from the component to facet:InvalidName.\n");
						throw ConnectFailure();																
					}					

					//
					//add the depend information into the componentMap
					//		
//					CORBA::String_var providesComponentId=CORBA::string_dup(((* theIterator).first).c_str());
					CORBA::String_var providesComponentId=CORBA::string_dup(idref.transcode());
							
					CORBA::StringSeq_var dependList=(*iter).second.depend;
					int len=dependList->length();
					dependList->length(len+1);
					dependList[(CORBA::ULong)len]=providesComponentId;
					(*iter).second.depend=dependList;						

				}
				
				//
				//connect the usesport and the usedport
				//
				Components::Cookie_var cookie_;
				try
				{
					cookie_=objUses->connect(useIdentifier.transcode(),objUsed);
					TRACE1(LEVEL4,"Assembly: connectComponents: connect the %s.\n",useIdentifier.transcode());
				}
				catch(const CORBA::SystemException &)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in CORBA SystemException .\n");
					throw ConnectFailure();														
				}
				catch(const InvalidName &)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in InvalidName.\n");
					throw ConnectFailure();									
				}
				catch(const InvalidConnection &)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in InvalidConnection.\n");
					throw ConnectFailure();									
				}
				catch(const AlreadyConnected &)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in AlreadyConnected.\n");
					throw ConnectFailure();									
				}								
				catch(const ExceededConnectionLimit &)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in ExceededConnectionLimit.\n");
					throw ConnectFailure();									
				}								
				
				Components::Cookie_impl* cookie = dynamic_cast<Components::Cookie_impl*>(cookie_.in());		
				CORBA::String_var connectCookie=cookie->toString();				
				TRACE1(LEVEL6,"Assembly: the connect cookie is %s.\n",connectCookie.in());
				//
				//add the connect information into the connect map
				//
				
				//deleted by wsf 2004.7.21
				//connectMap.insert(MapConnectInf::value_type(string(connectId.transcode()),connectCookie));
				//end by wsf  2004.7.21
								
				//add by wsf

				//
				//add the according archconnection info to the arch object
				//

				ArchComponent_var acom_ = arch->getArchComponent(objUsesId.in());
				ArchHome_var ahome = arch->getArchHome_By_UUID(acom_->homeuuid);
				obj = CORBA::Object::_duplicate(ahome->containerref);
				Container_var container = Components::Deployment::Container::_narrow(obj);
				
				ArchConnection_var aconn = container-> getArchConn();
				aconn->id = CORBA::string_dup(connectId.transcode());

				arch->addArchConnection(aconn);

				//end by wsf
			}
			else if(connectChildName.equals("connectevent"))
			{
				
				//
				//get the connect interface id
				//
				DOMString connectId=parser->getAttr(connectChild,"id");
				if(connectId==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: connect node miss id attribute.\n");
					throw InvalidConfigureFile();					
				}
				
				//
				//locate the component that define the consumesport
				//

				DOM_Node consumesNode=parser->findChild(connectChild,"consumesport");
				if(consumesNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: consumesport node missed.\n");
					throw InvalidConfigureFile();
				}
				
				DOM_Node  consumesIdentifierNode=parser->findChild(consumesNode,"consumesidentifier");
				if(consumesIdentifierNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: consumesidentifier node missed.\n");
					throw InvalidConfigureFile();					
				}								
				
				DOM_Node  consumesIdentifierText=consumesIdentifierNode.getFirstChild();
				
				DOMString consumesIdentifier=consumesIdentifierText.getNodeValue();			
				
				TRACE1(LEVEL6,"Assembly: connect Component ,the consume identifier is %s.\n",consumesIdentifier.transcode());
				
				DOM_Node  componentinstantiationrefNode=parser->findChild(consumesNode,"componentinstantiationref");
				if(componentinstantiationrefNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
					throw InvalidConfigureFile();					
				}				
				DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
				if(idref==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
					throw InvalidConfigureFile();					
				}
				
				TRACE1(LEVEL6,"Assembly: connectComponent : consumer is %s.\n",idref.transcode());
				
				//modified by wsf 2004.7.21
				/*				
				MapComponentInf::iterator iter=componentMap.find(string(idref.transcode()));
	
				if (iter == componentMap.end())	
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();										
				}		
				
				Components::CCMObject_var objConsumes=(* iter).second.ComponentRef;
				*/
				ArchComponent* acom = arch->getArchComponent(idref.transcode());
				if (acom == NULL)
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();		
				}

				CORBA::Object_var obj = acom->comref;
				Components::CCMObject_var objConsumes = Components::CCMObject::_narrow(obj);

				CORBA::String_var objConsumesId = CORBA::string_dup(idref.transcode());

				delete acom;
				acom = NULL;
				//end by wsf 2004.7.21
				
				Components::EventConsumerBase_var consumer=objConsumes->get_consumer(consumesIdentifier.transcode()); 
				
				
				//
				//locate the object to provides event: publishes or emits
				//
				Components::CCMObject_var objProvides;				
				
				DOM_Node publishesportNode=parser->findChild(connectChild,"publishesport");
				if(publishesportNode==NULL)
				{
					DOM_Node emitsportNode=parser->findChild(connectChild,"emitsport");
					if(emitsportNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: invalid connect interface node.\n");
						throw InvalidConfigureFile();												
					}						
					
					DOM_Node  emitsidentifierNode=parser->findChild(emitsportNode,"emitsidentifier");
					if(emitsidentifierNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: emitsidentifier node missed.\n");
						throw InvalidConfigureFile();					
					}				
					
					DOM_Node  emitsIdentifierText=emitsidentifierNode.getFirstChild();
					
					DOMString emitsIdentifier=emitsIdentifierText.getNodeValue();			
					
					TRACE1(LEVEL6,"Asembly: connect Component : the emits identifier is %s.\n",emitsIdentifier.transcode());
					
					DOM_Node  componentinstantiationrefNode=parser->findChild(emitsportNode,"componentinstantiationref");
					if(componentinstantiationrefNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
						throw InvalidConfigureFile();					
					}				
					DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
						throw InvalidConfigureFile();					
					}
					
					//modified by wsf 2004.7.21
					/*	
					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
					if (theIterator == componentMap.end())	
					{
						TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
						throw ConnectFailure();										
					}
					
					Components::CCMObject_var emitsComponent=(* theIterator).second.ComponentRef;
					*/
					acom = arch->getArchComponent(idref.transcode());
					if (acom == NULL)
					{
						TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
						throw ConnectFailure();		
					}
					
					CORBA::Object_var obj = acom->comref;
					Components::CCMObject_var emitsComponent =Components::CCMObject::_narrow(obj);

					delete acom;
					acom = NULL;

					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
					//end by wsf 2004.7.21
									
					//
					//connect to the emiter
					//
					try
					{
						emitsComponent->connect_consumer(emitsIdentifier.transcode(),consumer);
						TRACE2(LEVEL4,"Assembly: connectComponents: connect the consumer with emits identifier %s of component %s.\n",emitsIdentifier.transcode(),idref.transcode());
					}
					catch(const CORBA::SystemException &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in CORBA SystemException happened.\n");
						throw ConnectFailure();																
					}
					catch(const Components::InvalidName &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in InvalidName.\n");
						throw ConnectFailure();																
					} 	
					catch(const Components::InvalidConnection &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in InvalidConnection.\n");
						throw ConnectFailure();																
					}															
					catch(const Components::AlreadyConnected &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in AlreadyConnected.\n");
						throw ConnectFailure();																
					}	
					
					//
					//add the depend information into the componentMap
					//							
					CORBA::StringSeq_var dependList=(*theIterator).second.depend;										
					int len=dependList->length();
					dependList->length(len+1);
					dependList[(CORBA::ULong)len]=objConsumesId;
					(*theIterator).second.depend=dependList;
				
					//add by wsf

					//
					//add the according archconnection info to the arch object
					//

					ArchComponent_var acom_ = arch->getArchComponent(idref.transcode());
					ArchHome_var ahome = arch->getArchHome_By_UUID(acom_->homeuuid);
					obj = CORBA::Object::_duplicate(ahome->containerref);
					Container_var container = Components::Deployment::Container::_narrow(obj);
					
					ArchConnection_var aconn = container-> getArchConn();
					aconn->id = CORBA::string_dup(connectId.transcode());

					arch->addArchConnection(aconn);

					//end by wsf	
				}
				else
				{

					DOM_Node  publishesidentifierNode=parser->findChild(publishesportNode,"publishesidentifier");
					if(publishesidentifierNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: publishesidentifier node missed.\n");
						throw InvalidConfigureFile();					
					}				
					
					DOM_Node  publishesIdentifierText=publishesidentifierNode.getFirstChild();
					
					DOMString publishesIdentifier=publishesIdentifierText.getNodeValue();
					
					TRACE1(LEVEL6,"Assembly: connect Component : the publishes identifer is %s.\n",publishesIdentifier.transcode());		
					
					DOM_Node  componentinstantiationrefNode=parser->findChild(publishesportNode,"componentinstantiationref");
					if(componentinstantiationrefNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
						throw InvalidConfigureFile();					
					}				
					DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
						throw InvalidConfigureFile();					
					}
					
					TRACE1(LEVEL6,"Assembly: connectComponent: the publisher is %s.\n",idref.transcode());
					
					//modified by wsf 2004.7.21
					/*
					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
					if (theIterator == componentMap.end())	
					{
						TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
						throw ConnectFailure();										
					}
					
					Components::CCMObject_var providesComponent=(* theIterator).second.ComponentRef;
					*/
					acom = arch->getArchComponent(idref.transcode());
					if (acom == NULL)
					{
						TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
						throw ConnectFailure();		
					}
					
					CORBA::Object_var obj = acom->comref;
					Components::CCMObject_var providesComponent =Components::CCMObject::_narrow(obj);

					delete acom;
					acom = NULL;

					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
					//end by wsf 2004.7.21
									
					//
					//subscribe to the provider
					//
					Components::Cookie_var cookie_;
					try
					{
						cookie_=providesComponent->subscribe(publishesIdentifier.transcode(),consumer);
						TRACE2(LEVEL4,"Assembly: subscirbe the consumer with publisher %s of component %s.\n",publishesIdentifier.transcode(),idref.transcode());
					}
					catch(const CORBA::SystemException &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in CORBA SystemException happened.\n");
						throw ConnectFailure();																
					}
					catch(const Components::InvalidName &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in InvalidName.\n");
						throw ConnectFailure();																
					}	
					catch(const Components::InvalidConnection &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in InvalidConnection.\n");
						throw ConnectFailure();																
					}															
					catch(const Components::ExceededConnectionLimit &)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in ExceededConnectionLimit.\n");
						throw ConnectFailure();																
					}	
						
					Components::Cookie_impl* cookie = dynamic_cast<Components::Cookie_impl*>(cookie_.in());		
					CORBA::String_var connectCookie=cookie->toString();	
					TRACE1(LEVEL6,"Assembly: the connect cookie is %s.\n",connectCookie.in());
					
					//
					//add the connect information into the connect map
					//
					
					//deleted by wsf 2004.7.21
					//connectMap.insert(MapConnectInf::value_type(string(connectId.transcode()),connectCookie));
					//end by wsf 2004.7.21

					//
					//add the depend information into the componentMap
					//		
					CORBA::StringSeq_var dependList=(*theIterator).second.depend;										
					int len=dependList->length();
					dependList->length(len+1);
					dependList[(CORBA::ULong)len]=objConsumesId;
					(*theIterator).second.depend=dependList;
					
					//add by wsf

					//
					//add the according archconnection info to the arch object
					//

					ArchComponent_var acom_ = arch->getArchComponent(idref.transcode());
					ArchHome_var ahome = arch->getArchHome_By_UUID(acom_->homeuuid);
					obj = CORBA::Object::_duplicate(ahome->containerref);
					Container_var container = Components::Deployment::Container::_narrow(obj);
					
					ArchConnection_var aconn = container-> getArchConn();
					aconn->id = CORBA::string_dup(connectId.transcode());

					arch->addArchConnection(aconn);

					//end by wsf
				}				
			}
			else if(connectChildName.equals("connectcomponentinstantiation"))
			{
				setDynamicConfigurator(parser,connectChild);
			}
			
		}	

		
	}
	
	
}

//****************************************************************************
//set the dynamic configurator for the home according to the XML file
//****************************************************************************
void
Assembly_impl::setDynamicConfigurator(XMLParser *parser,DOM_Node node)
	throw (InvalidConfigureFile,ConnectFailure)
{

	//
	//locate home object
	//
	DOM_Node homeplacementrefNode=parser->findChild(node,"homeplacementref");
	if(homeplacementrefNode==NULL)
	{
		TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: homeplacementref node of connectcomponentinstantiation is missed.\n");
		throw InvalidConfigureFile();																			
	}
						
	DOMString idref=parser->getAttr(homeplacementrefNode,"idref");
	if(idref==NULL)
	{
		TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: homeplacementref node miss idref attribute.\n");
		throw InvalidConfigureFile();					
	}
						
	//modified by wsf 2004.7.21
	/*
	MapHomeInf::iterator theIterator=homeMap.find(string(idref.transcode()));
			
	if (theIterator == homeMap.end())	
	{
		TRACE0(LEVEL1,"Assembly: connect Components failed in idref does not refer to the valid home.\n");
		throw ConnectFailure();										
	}
						
	Components::CCMHome_var home = Components::CCMHome::_duplicate( ((* theIterator).second.HomeRef) );
	*/
	ArchHome* ahome = arch->getArchHome(idref.transcode());
	if (ahome == NULL)
	{
		TRACE0(LEVEL1,"Assembly: connect Components failed in idref does not refer to the valid home.\n");
		throw ConnectFailure();										
	}
	CORBA::Object_var obj = ahome->homeref;
	Components::CCMHome_var home = Components::CCMHome::_narrow(obj);
	delete ahome;
	ahome = NULL;
	//end by wsf 2004.7.21
    
    Components::HomeConfiguration_var homeConfig=Components::HomeConfiguration::_narrow(home); 
    if(CORBA::is_nil(homeConfig))
    {
   		TRACE0(LEVEL1,"Assembly: set dynamic configurator for home Configuration failed in narrow CCMHome to HomeConfiguration.\n"); 
    	throw ConfigureFailure();    			
    }
	
	DOMString componentInstantiationConnectId=parser->getAttr(node,"id");
	if(componentInstantiationConnectId==NULL)
	{
		TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: connect node miss id attribute.\n");
		throw InvalidConfigureFile();					
	}
	
	
	//
	//locate the connect to object and information, set the dynamic configurator
	//
	
	DynamicConnectInfos_var connectInfoList = new DynamicConnectInfos;	
	
	DOM_NodeList connectChildren=node.getChildNodes();
	unsigned int connectChildrenLen=connectChildren.getLength();	
		
	DOM_Node connectChild;
	DOMString connectChildName;
		
	for(unsigned int iterator=0;iterator<connectChildrenLen;iterator++)
	{
		connectChild=connectChildren.item(iterator);
		connectChildName=connectChild.getNodeName();
			
		if(connectChildName.equals("connectinterface"))
		{
			DynamicConnectInfo_var connectInformation = new DynamicConnectInfo;
			
			//
			//get the connect interface id
			//
			DOMString connectId=parser->getAttr(connectChild,"id");
			if(connectId==NULL)
			{
				TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: connect node miss id attribute.\n");
				throw InvalidConfigureFile();					
			}
				
			//
			//locate the component that define the usesport
			//

			DOM_Node useNode=parser->findChild(connectChild,"usesport");
			if(useNode==NULL)
			{
				TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: usesport node missed.\n");
				throw InvalidConfigureFile();
			}
				
			DOM_Node  useIdentifierNode=parser->findChild(useNode,"usesidentifier");
			if(useIdentifierNode==NULL)
			{
				TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: usesidentifier node missed.\n");
				throw InvalidConfigureFile();					
			}


			DOM_Node  useIdentifierText=useIdentifierNode.getFirstChild();
				
			DOMString useIdentifier=useIdentifierText.getNodeValue();			
				
			connectInformation->portIdentifier = CORBA::string_dup(useIdentifier.transcode());
			TRACE1(LEVEL6,"Assembly: connect Component ,the uses port is %s.\n",useIdentifier.transcode());			
			
				
			//
			//locate the object of provides,supports,homeinterface
			//
			CORBA::Object_var objUsed;				
				
			DOM_Node providesportNode=parser->findChild(connectChild,"providesport");
			if(providesportNode==NULL)
			{
				DOM_Node supportsNode=parser->findChild(connectChild,"componentsupportedinterface");
				if(supportsNode==NULL)
				{
					DOM_Node homeinterfaceNode=parser->findChild(connectChild,"homeinterface");
					if(homeinterfaceNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: invalid connect interface node.\n");
						throw InvalidConfigureFile();												
					}
					
					connectInformation->connectPortType = HOMEINTERFACE;
						
					DOM_Node homeplacementrefNode=parser->findChild(homeinterfaceNode,"homeplacementref");
					if(homeplacementrefNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: homeplacementref node missed.\n");
						throw InvalidConfigureFile();																			
					}
						
					DOMString idref=parser->getAttr(homeplacementrefNode,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: homeplacementref node miss idref attribute.\n");
						throw InvalidConfigureFile();					
					}
						
					//modified by wsf 2004.7.21
					/*
					MapHomeInf::iterator theIterator=homeMap.find(string(idref.transcode()));
			
					if (theIterator == homeMap.end())	
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in idref does not refer to the valid home.\n");
						throw ConnectFailure();										
					}
						
					objUsed = CORBA::Object::_duplicate( ((* theIterator).second.HomeRef) );	
					*/
					
					ahome = arch->getArchHome(idref.transcode());
					if (ahome == NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in idref does not refer to the valid home.\n");
						throw ConnectFailure();										
					}
					objUsed = ahome->homeref;

					delete ahome;
					ahome = NULL;		

					//end by wsf 2004.21
				}
				else
				{
					connectInformation->connectPortType = COMPONENTSUPPORTEDINTERFACE;
					
					DOM_Node componentinstantiationrefNode=parser->findChild(supportsNode,"componentinstantiationref");
					if(componentinstantiationrefNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
						throw InvalidConfigureFile();												
					}
						
					DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
						throw InvalidConfigureFile();					
					}

					//modified by wsf 2004.7.21
					/*						
					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
			
					if (theIterator == componentMap.end())	
					{
						TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
						throw ConnectFailure();										
					}
						
					objUsed = CORBA::Object::_duplicate( ((* theIterator).second.ComponentRef) );
					*/
					
					ArchComponent* acom = arch->getArchComponent(idref.transcode());
					if (acom == NULL)
					{
						TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
						throw ConnectFailure();										
					}
					objUsed = CORBA::Object::_duplicate(acom->comref);

					delete acom;
					acom = NULL;		

					//end by wsf 2004.21
				}					
			}
			else
			{
				connectInformation->connectPortType = PROVIDESTYPE;
				
				DOM_Node  providesIdentifierNode=parser->findChild(providesportNode,"providesidentifier");
				if(providesIdentifierNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: providesidentifier node missed.\n");
					throw InvalidConfigureFile();					
				}			
					
				DOM_Node  providesIdentifierText=providesIdentifierNode.getFirstChild();
					
				DOMString providesIdentifier=providesIdentifierText.getNodeValue();
				
				TRACE1(LEVEL6,"Assembly: connect Component ,the provides port is %s.\n",providesIdentifier.transcode());			
					
				DOM_Node  componentinstantiationrefNode=parser->findChild(providesportNode,"componentinstantiationref");
				if(componentinstantiationrefNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
					throw InvalidConfigureFile();					
				}				
				
				DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
				if(idref==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
					throw InvalidConfigureFile();					
				}

				//modified by wsf 2004.7.21
				/*					
				MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
				if (theIterator == componentMap.end())	
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();										
				}
					
				Components::CCMObject_var providesComponent=(* theIterator).second.ComponentRef;						
				*/
				
				ArchComponent* acom = arch->getArchComponent(idref.transcode());
				if (acom == NULL)
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();										
				}
				
				obj = CORBA::Object::_duplicate(acom->comref);
				Components::CCMObject_var providesComponent= Components::CCMObject::_narrow(obj);

				delete acom;
				acom = NULL;		

				//end by wsf 2004.21
										
				//
				//navigate to the facet specified by providesIdentifier
				//
					
				try
				{						
					objUsed=providesComponent->provide_facet(providesIdentifier.transcode());
					TRACE1(LEVEL4,"Assembly: connectComponents: navigate to %s.\n",providesIdentifier.transcode());
				}
				catch(const CORBA::SystemException &)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in navigate from the component to facet: SystemException happened.\n");
					throw ConnectFailure();																
				}
				catch(const Components::InvalidName &)
				{
					TRACE0(LEVEL6,"Assembly: connect Components failed in navigate from the component to facet:InvalidName.\n");
					throw ConnectFailure();																
				}					
			}
			
			connectInformation->connectToRef = objUsed;	

			//
			//add the connect information into the connect map left to be designed and implemented
			//
			
			//
			//add the connectInformation to the sequence
			//
			int len = connectInfoList->length();
			connectInfoList->length(len+1);
			connectInfoList[(CORBA::ULong)len]=connectInformation;				
		}
		else if(connectChildName.equals("connectevent"))
		{
			DynamicConnectInfo_var connectInformation = new DynamicConnectInfo;
			
			//
			//get the connect interface id
			//
			DOMString connectId=parser->getAttr(connectChild,"id");
			if(connectId==NULL)
			{
				TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: connect node miss id attribute.\n");
				throw InvalidConfigureFile();					
			}
				
			//
			//locate the component that define the consumesport
			//

			DOM_Node consumesNode=parser->findChild(connectChild,"consumesport");
			if(consumesNode==NULL)
			{
				TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: consumesport node missed.\n");
				throw InvalidConfigureFile();
			}
				
			DOM_Node  consumesIdentifierNode=parser->findChild(consumesNode,"consumesidentifier");
			if(consumesIdentifierNode==NULL)
			{
				TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: consumesidentifier node missed.\n");
				throw InvalidConfigureFile();					
			}								
				
			DOM_Node  consumesIdentifierText=consumesIdentifierNode.getFirstChild();
			
			DOMString consumesIdentifier=consumesIdentifierText.getNodeValue();			
			
			connectInformation->portIdentifier = CORBA::string_dup(consumesIdentifier.transcode());
			TRACE1(LEVEL6,"Assembly: connect Component ,the consume identifier is %s.\n",consumesIdentifier.transcode());
				
			//
			//locate the object to provides event: publishes or emits
			//
			Components::CCMObject_var objProvides;				
				
			DOM_Node publishesportNode=parser->findChild(connectChild,"publishesport");
			if(publishesportNode==NULL)
			{
				DOM_Node emitsportNode=parser->findChild(connectChild,"emitsport");
				if(emitsportNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: invalid connect interface node.\n");
					throw InvalidConfigureFile();												
				}
				
				connectInformation->connectPortType = EMITSTYPE;				
					
				DOM_Node  emitsidentifierNode=parser->findChild(emitsportNode,"emitsidentifier");
				if(emitsidentifierNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: emitsidentifier node missed.\n");
					throw InvalidConfigureFile();					
				}				
					
				DOM_Node  emitsIdentifierText=emitsidentifierNode.getFirstChild();
					
				DOMString emitsIdentifier=emitsIdentifierText.getNodeValue();			
					
				connectInformation->eventSourcePortIdentifier = CORBA::string_dup(emitsIdentifier.transcode());
				TRACE1(LEVEL6,"Asembly: connect Component : the emits identifier is %s.\n",emitsIdentifier.transcode());
					
				DOM_Node  componentinstantiationrefNode=parser->findChild(emitsportNode,"componentinstantiationref");
				if(componentinstantiationrefNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
					throw InvalidConfigureFile();					
				}				
				DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
				if(idref==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
					throw InvalidConfigureFile();					
				}
				//modified by wsf 2004.7.21
				/*					
				MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
				if (theIterator == componentMap.end())	
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();										
				}
				
				CORBA::Object_var emitsComponent=CORBA::Object::_duplicate((* theIterator).second.ComponentRef);
				*/
				
				ArchComponent* acom = arch->getArchComponent(idref.transcode());
				if (acom == NULL)
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();										
				}
				
				CORBA::Object_var emitsComponent=CORBA::Object::_duplicate(acom->comref);				

				delete acom;
				acom = NULL;		

				//end by wsf 2004.21
				
				connectInformation->connectToRef = emitsComponent;					
			}
			else
			{
				connectInformation->connectPortType = PUBLISHESTYPE;
				
				DOM_Node  publishesidentifierNode=parser->findChild(publishesportNode,"publishesidentifier");
				if(publishesidentifierNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: publishesidentifier node missed.\n");
					throw InvalidConfigureFile();					
				}				
					
				DOM_Node  publishesIdentifierText=publishesidentifierNode.getFirstChild();
				
				DOMString publishesIdentifier=publishesIdentifierText.getNodeValue();
					
				connectInformation->eventSourcePortIdentifier = CORBA::string_dup(publishesIdentifier.transcode());
				TRACE1(LEVEL6,"Assembly: connect Component : the publishes identifer is %s.\n",publishesIdentifier.transcode());		
					
				DOM_Node  componentinstantiationrefNode=parser->findChild(publishesportNode,"componentinstantiationref");
				if(componentinstantiationrefNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node missed.\n");
					throw InvalidConfigureFile();					
				}				
				DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
				if(idref==NULL)
				{
					TRACE0(LEVEL1,"Assembly: connect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
					throw InvalidConfigureFile();					
				}
					
				TRACE1(LEVEL6,"Assembly: connectComponent: the publisher is %s.\n",idref.transcode());
				//modified by wsf 2004.7.21
				/*						
				MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
				if (theIterator == componentMap.end())	
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();										
				}
					
				CORBA::Object_var providesComponent=CORBA::Object::_duplicate((* theIterator).second.ComponentRef);				
				*/
				
				ArchComponent* acom = arch->getArchComponent(idref.transcode());
				if (acom == NULL)
				{
					TRACE1(LEVEL1,"Assembly: connect Components failed in idref %s does not refer to the valid component instantiation.\n",idref.transcode());
					throw ConnectFailure();										
				}
				
				CORBA::Object_var providesComponent=CORBA::Object::_duplicate(acom->comref);				

				delete acom;
				acom = NULL;		

				//end by wsf 2004.21
				
				connectInformation->connectToRef = providesComponent;
					
				//
				//add the connect information into the connect map left to be designed and implemented
				//					
				
				//
				//add the connectInformation into the connectList
				//
				int len = connectInfoList->length();
				connectInfoList->length(len+1);
				connectInfoList[(CORBA::ULong)len]=connectInformation;					
			}				
		}
	}
	
	
	//
	//create the configurator object 
	//	
    TRACE0(LEVEL5,"Assembly: create the dynamic configurator object for the home .\n");
    
    DynamicConnectConfigurator_impl* cfImpl = new DynamicConnectConfigurator_impl();    

    cfImpl->set_connect_configuration(connectInfoList.in());

   	PortableServer::ServantBase_var servant = cfImpl;
    	
    //activate the Configurator corba object  	
    CORBA::String_var str=CORBA::string_dup("DynamicConfigurator");

#ifdef STARCCMSTRINGADD
	str = HelpFun::CORBA_string_add(str, CORBA::string_dup(componentInstantiationConnectId.transcode()));
	str = HelpFun::CORBA_string_add(str, assemblyUUID);
#else    
    str+=CORBA::string_dup(componentInstantiationConnectId.transcode());
    str+=assemblyUUID;
#endif    
 	
    PortableServer::ObjectId_var oid=PortableServer::string_to_ObjectId(str.in());

    poa->activate_object_with_id(oid,cfImpl);  
    CORBA::Object_var object=poa->id_to_reference(oid);
    DynamicConnectConfigurator_var cf=DynamicConnectConfigurator::_narrow(object);

#ifdef ORBacus        
	obj = orb -> resolve_initial_references("BootManager");
	OB::BootManager_var bootManager = OB::BootManager::_narrow(obj);
  
  	bootManager -> add_binding(oid, cf);   
#endif    

#ifdef StarBus       
	obj = orb -> resolve_initial_references("AssistAdaptorManager");
	STAR::AssistAdaptorManager_var assistAdaptorManager = STAR::AssistAdaptorManager::_narrow(obj);
        assistAdaptorManager -> binding_objref(oid, cf);   
#endif 	

#ifdef TAO
    CORBA::Object_var table_object = orb -> resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
      
    IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

	CORBA::String_var iorStr = orb -> object_to_string(cf);	
	
    adapter->bind (str.in(), iorStr.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
#endif  
    	
    //
    //set configurator for home configuration
    //	
    homeConfig->set_configurator(cf.in());
    homeConfig->complete_component_configuration(true);
}

//****************************************************************************
//disconnect the component according to the cad file
//****************************************************************************

void
Assembly_impl::disconnectComponents(XMLParser *parser,DOM_Node root,const char* nodename)
	throw (InvalidConfigureFile,DisConnectFailure)
{
	//
	//locate connect
	//
	
	DOM_Node connectNode;
	if (strcmp(nodename, "disconnections") == 0)
		connectNode = parser->findChild(root,"disconnections");
	else
		connectNode =parser->findChild(root,"connections");
		
	if(connectNode!=NULL)
	{

	 	DOM_NodeList connectChildren=connectNode.getChildNodes();
	        unsigned int connectChildrenLen=connectChildren.getLength();	
		
		DOM_Node connectChild;
		DOMString connectChildName;
		
		for(unsigned int iterator=0;iterator<connectChildrenLen;iterator++)
		{
			connectChild=connectChildren.item(iterator);
			connectChildName=connectChild.getNodeName();
			
			if(connectChildName.equals("connectinterface"))
			{
				//
				//get the connect interface id
				//
				DOMString connectId=parser->getAttr(connectChild,"id");
				if(connectId==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: connect node miss id attribute.\n");
					throw InvalidConfigureFile();					
				}
				
				//
				//locate the comonent that define the usesport
				//

				DOM_Node useNode=parser->findChild(connectChild,"usesport");
				if(useNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: usesport node missed.\n");
					throw InvalidConfigureFile();
				}
				
				DOM_Node  useIdentifierNode=parser->findChild(useNode,"usesidentifier");
				if(useIdentifierNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: usesidentifier node missed.\n");
					throw InvalidConfigureFile();					
				}				
				
				DOM_Node  useIdentifierText=useIdentifierNode.getFirstChild();
				
				DOMString useIdentifier=useIdentifierText.getNodeValue();
				
				TRACE1(LEVEL6,"Assembly: connect Component : uses identifier is %s.\n",	useIdentifier.transcode());		
				
				DOM_Node  componentinstantiationrefNode=parser->findChild(useNode,"componentinstantiationref");
				if(componentinstantiationrefNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node missed.\n");
					throw InvalidConfigureFile();					
				}				
				DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
				if(idref==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
					throw InvalidConfigureFile();					
				}
				
				//modified by wsf 2004.7.21
				/*
				MapComponentInf::iterator iter=componentMap.find(string(idref.transcode()));
	
				if (iter == componentMap.end())	
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
					throw DisConnectFailure();										
				}		
				
				Components::CCMObject_var objUses=(* iter).second.ComponentRef;
				*/
				ArchComponent* acom = arch->getArchComponent(idref.transcode());
				if (acom == NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
					throw DisConnectFailure();		
				}
				
				CORBA::Object_var obj = acom->comref;
				Components::CCMObject_var objUses =Components::CCMObject::_narrow(obj);

				CORBA::String_var objUsesId = acom->id;

				delete acom;
				acom = NULL;
				//end by wsf 2004.7.21
								
				//
				//locate the object of provides,supports,homeinterface
				//
				CORBA::Object_var objUsed;				
				
				DOM_Node providesportNode=parser->findChild(connectChild,"providesport");
				if(providesportNode==NULL)
				{
					DOM_Node supportsNode=parser->findChild(connectChild,"componentsupportedinterface");
					if(supportsNode==NULL)
					{
						DOM_Node homeinterfaceNode=parser->findChild(connectChild,"homeinterface");
						if(homeinterfaceNode==NULL)
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: invalid connect interface node.\n");
							throw InvalidConfigureFile();												
						}
						
						DOM_Node homeplacementrefNode=parser->findChild(homeinterfaceNode,"homeplacementref");
						if(homeplacementrefNode==NULL)
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: homeplacementref node missed.\n");
							throw InvalidConfigureFile();																			
						}
						
						DOMString idref=parser->getAttr(homeplacementrefNode,"idref");
						if(idref==NULL)
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: homeplacementref node miss idref attribute.\n");
							throw InvalidConfigureFile();					
						}
						
						//modified by wsf 2004.7.21
						/*						
						MapHomeInf::iterator theIterator=homeMap.find(string(idref.transcode()));
			
						if (theIterator == homeMap.end())	
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid home.\n");
							throw DisConnectFailure();										
						}
						
						objUsed=((* theIterator).second.HomeRef).in();	
						*/
						ArchHome* ahome = arch->getArchHome(idref.transcode());
						if (ahome == NULL)
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
							throw DisConnectFailure();		
						}

						objUsed = ahome->homeref;

						delete ahome;
						ahome = NULL;
						//end by wsf 2004.7.21					

					}
					else
					{
						DOM_Node componentinstantiationrefNode=parser->findChild(supportsNode,"componentinstantiationref");
						if(componentinstantiationrefNode==NULL)
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node missed.\n");
							throw InvalidConfigureFile();												
						}
						
						DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
						if(idref==NULL)
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
							throw InvalidConfigureFile();					
						}
						
						//modified by wsf 2004.7.21
						/*							
						MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
			
						if (theIterator == componentMap.end())	
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
							throw DisConnectFailure();										
						}
						
						objUsed=((* theIterator).second.ComponentRef).in();	
						*/
						acom = arch->getArchComponent(idref.transcode());
						if (acom == NULL)
						{
							TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
							throw DisConnectFailure();		
						}

						objUsed = acom->comref;

						delete acom;
						acom = NULL;
						//end by wsf 2004.7.21
						
						//add by wsf	//__deleted 2004.7.21																								
						//
						//delete the depend information into the componentMap
						//	
						/*
						CORBA::String_var supportsComponentId=CORBA::string_dup(idref.transcode());
								
						CORBA::StringSeq_var dependList=(*iter).second.depend;
						CORBA::StringSeq tmpseq;
						delElementInSeq(supportsComponentId.in(),dependList.in(),tmpseq);
						CORBA::StringSeq_var newDependList = new CORBA::StringSeq(tmpseq);						
						(*iter).second.depend = newDependList;	
						*/
						//end by wsf																							
						
					}
					
				}
				else
				{

					DOM_Node  providesIdentifierNode=parser->findChild(providesportNode,"providesidentifier");
					if(providesIdentifierNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: providesidentifier node missed.\n");
						throw InvalidConfigureFile();					
					}				
					
					DOM_Node  providesIdentifierText=providesIdentifierNode.getFirstChild();
					
					DOMString providesIdentifier=providesIdentifierText.getNodeValue();			
					
					DOM_Node  componentinstantiationrefNode=parser->findChild(providesportNode,"componentinstantiationref");
					if(componentinstantiationrefNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node missed.\n");
						throw InvalidConfigureFile();					
					}				
					DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
						throw InvalidConfigureFile();					
					}
					
					//modified by wsf 2004.7.21
					/*					
					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
					if (theIterator == componentMap.end())	
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
						throw DisConnectFailure();										
					}
					
					Components::CCMObject_var providesComponent=(* theIterator).second.ComponentRef;
					*/
					acom = arch->getArchComponent(idref.transcode());
					if (acom == NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
						throw DisConnectFailure();		
					}

					obj = acom->comref;
					Components::CCMObject_var providesComponent =Components::CCMObject::_narrow(obj);

					delete acom;
					acom = NULL;
					//end by wsf 2004.7.21
						
					//
					//navigate to the facet specified by providesIdentifier
					//
					
					try
					{
						objUsed=providesComponent->provide_facet(providesIdentifier.transcode());
					}
					catch(const CORBA::SystemException &)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in navigate from the component to facet: SystemException happened.\n");
						throw DisConnectFailure();																
					}
					catch(const Components::InvalidName &)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in navigate from the component to facet:InvalidName.\n");
						throw DisConnectFailure();																
					}
					
					//add by wsf //__deleted 2004.7.21
					//
					//delete the depend information into the componentMap
					//	
					/*
					CORBA::String_var providesComponentId=CORBA::string_dup(((* theIterator).first).c_str());
								
					CORBA::StringSeq_var dependList=(*iter).second.depend;
					CORBA::StringSeq tmpseq;
					delElementInSeq	(providesComponentId.in(),dependList.in(),tmpseq);
					CORBA::StringSeq_var newDependList = new CORBA::StringSeq(tmpseq);						
					(*iter).second.depend = newDependList;
					*/
					//end by wsf				

				}
				
				//
				//get the cookie of the coonect
				//
				//modified by wsf 2004.7.21
				/*
				MapConnectInf::iterator theIterator=connectMap.find(string(connectId.transcode()));
		
				if (theIterator == connectMap.end())	
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in Invalid connection id.\n");
					throw DisConnectFailure();														
				}
				
				CORBA::String_var connectCookie=(*theIterator).second;
					
				*/
				ArchConnection* aconn = arch->getArchConnection(connectId.transcode());
				if (aconn == NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in Invalid connection id.\n");
					throw DisConnectFailure();		
				}

				CORBA::String_var connectCookie = aconn->cookie;

				delete aconn;
				aconn = NULL;
				//end by wsf 2004.7.21
							
				Components::Cookie_var cookie = new Components::Cookie_impl(connectCookie.in());					
				
				//
				//disconnect the usesport and the usedport
				//				
				try
				{
					objUses->disconnect(useIdentifier.transcode(),cookie.in());
				}
				catch(const CORBA::SystemException &)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in CORBA SystemException .\n");
					throw DisConnectFailure();														
				}
				catch(const InvalidName &)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in InvalidName.\n");
					throw DisConnectFailure();									
				}
				catch(const InvalidConnection &)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in InvalidConnection.\n");
					throw DisConnectFailure();									
				}
				catch(const CookieRequired &)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in CookieRequired.\n");
					throw DisConnectFailure();									
				}								
				catch(const NoConnection &)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in NoConnection.\n");
					throw DisConnectFailure();									
				}								

				//add by wsf

				ArchConnection_var aconn_ = arch->getArchConnection(connectId.transcode());
				arch->delArchConnection(aconn_->cookie);
				
				//erase the connect information from the connect map //__deleted 2004.7.21
				//connectMap.erase(theIterator);
				//end by wsf				
				
			}
			else if(connectChildName.equals("connectevent"))
			{
				
				//
				//get the connect interface id
				//
				DOMString connectId=parser->getAttr(connectChild,"id");
				if(connectId==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: connect node miss id attribute.\n");
					throw InvalidConfigureFile();					
				}
				
				//
				//locate the component that define the consumesport
				//

				DOM_Node consumesNode=parser->findChild(connectChild,"consumesport");
				if(consumesNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: consumesport node missed.\n");
					throw InvalidConfigureFile();
				}
				
				DOM_Node  consumesIdentifierNode=parser->findChild(consumesNode,"consumesidentifier");
				if(consumesIdentifierNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: consumesidentifier node missed.\n");
					throw InvalidConfigureFile();					
				}				
				
				DOM_Node  consumesIdentifierText=consumesIdentifierNode.getFirstChild();
				
				DOMString consumesIdentifier=consumesIdentifierText.getNodeValue();			
				
				DOM_Node  componentinstantiationrefNode=parser->findChild(consumesNode,"componentinstantiationref");
				if(componentinstantiationrefNode==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node missed.\n");
					throw InvalidConfigureFile();					
				}				
				DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
				if(idref==NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
					throw InvalidConfigureFile();					
				}
				
				//modified by wsf 2004.7.21
				/*					
				MapComponentInf::iterator iter=componentMap.find(string(idref.transcode()));
	
				if (iter == componentMap.end())	
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
					throw DisConnectFailure();										
				}		
				
				Components::CCMObject_var objConsumes=(* iter).second.ComponentRef;
				*/
				ArchComponent* acom = arch->getArchComponent(idref.transcode());
				if (acom == NULL)
				{
					TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
					throw DisConnectFailure();		
				}

				CORBA::Object_var obj = acom->comref;
				Components::CCMObject_var objConsumes =Components::CCMObject::_narrow(obj);

				delete acom;
				acom = NULL;
				//end by wsf 2004.7.21
				
				Components::EventConsumerBase_var consumer=objConsumes->get_consumer(consumesIdentifier.transcode()); 
				
				
				//
				//locate the object to provides event: publishes or emits
				//
				Components::CCMObject_var objProvides;				
				
				DOM_Node publishesportNode=parser->findChild(connectChild,"publishesport");
				if(publishesportNode==NULL)
				{
					DOM_Node emitsportNode=parser->findChild(connectChild,"emitsport");
					if(emitsportNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: invalid connect interface node.\n");
						throw InvalidConfigureFile();												
					}						
					
					DOM_Node  emitsidentifierNode=parser->findChild(emitsportNode,"emitsidentifier");
					if(emitsidentifierNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: emitsidentifier node missed.\n");
						throw InvalidConfigureFile();					
					}				
					
					DOM_Node  emitsIdentifierText=emitsidentifierNode.getFirstChild();
					
					DOMString emitsIdentifier=emitsIdentifierText.getNodeValue();			
					
					DOM_Node  componentinstantiationrefNode=parser->findChild(emitsportNode,"componentinstantiationref");
					if(componentinstantiationrefNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node missed.\n");
						throw InvalidConfigureFile();					
					}				
					DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
						throw InvalidConfigureFile();					
					}
					
					//modified by wsf 2004.7.21
					/*					
					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
					if (theIterator == componentMap.end())	
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
						throw DisConnectFailure();										
					}
					
					Components::CCMObject_var emitsComponent=(* theIterator).second.ComponentRef;
					*/
					acom = arch->getArchComponent(idref.transcode());
					if (acom == NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
						throw DisConnectFailure();		
					}

					obj = acom->comref;
					Components::CCMObject_var emitsComponent =Components::CCMObject::_narrow(obj);

					delete acom;
					acom = NULL;
					//end by wsf 2004.7.21
									
					//
					//disconnect to the emiter
					//
					try
					{
						Components::EventConsumerBase_var consumer=emitsComponent->disconnect_consumer(emitsIdentifier.transcode());
					}
					catch(const CORBA::SystemException &)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in CORBA SystemException happened.\n");
						throw DisConnectFailure();																
					}
					catch(const Components::InvalidName &)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in InvalidName.\n");
						throw DisConnectFailure();																
					}	
					catch(const Components::NoConnection &)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in NoConnection.\n");
						throw DisConnectFailure();																
					}																				

					// add by wsf
					//
					//get the cookie of the coonect
					//
					//modified by wsf 2004.7.21
					/*
					MapConnectInf::iterator connectIter=connectMap.find(string(connectId.transcode()));
			
					if (connectIter == connectMap.end())	
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in Invalid connection id.\n");
						throw DisConnectFailure();														
					}	
					*/
					ArchConnection* aconn = arch->getArchConnection(connectId.transcode());
					if (aconn == NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in Invalid connection id.\n");
						throw DisConnectFailure();		
					}
										
					arch->delArchConnection(aconn->cookie);

					delete aconn;
					aconn = NULL;
					//end by wsf 2004.7.21
					//
					//erase the connect information into the connect map
					//	

					//add by wsf
					//__deleted 2004.7.21
					//connectMap.erase(connectIter);	
					//
					//delete the depend information into the componentMap
					//	
					/*
					CORBA::StringSeq_var dependList=(*theIterator).second.depend;					
					CORBA::String_var consumesComponentId=CORBA::string_dup(((* iter).first).c_str());					
					CORBA::StringSeq tmpseq;
					delElementInSeq	(consumesComponentId.in(),dependList.in(),tmpseq);
					CORBA::StringSeq_var newDependList = new CORBA::StringSeq(tmpseq);	
					(*iter).second.depend = newDependList;
					*/
					// end by wsf		
					
				}
				else
				{

					DOM_Node  publishesidentifierNode=parser->findChild(publishesportNode,"publishesidentifier");
					if(publishesidentifierNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: publishesidentifier node missed.\n");
						throw InvalidConfigureFile();					
					}				
					
					DOM_Node  publishesIdentifierText=publishesidentifierNode.getFirstChild();
					
					DOMString publishesIdentifier=publishesIdentifierText.getNodeValue();			
					
					DOM_Node  componentinstantiationrefNode=parser->findChild(publishesportNode,"componentinstantiationref");
					if(componentinstantiationrefNode==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node missed.\n");
						throw InvalidConfigureFile();					
					}				
					DOMString idref=parser->getAttr(componentinstantiationrefNode,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in invalid cad file: componentinstantiationref node miss idref attribute.\n");
						throw InvalidConfigureFile();					
					}
					
					//modified by wsf 2004.7.21
					/*					
					MapComponentInf::iterator theIterator=componentMap.find(string(idref.transcode()));
		
					if (theIterator == componentMap.end())	
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
						throw DisConnectFailure();										
					}
					
					Components::CCMObject_var providesComponent=(* theIterator).second.ComponentRef;
					*/
					acom = arch->getArchComponent(idref.transcode());
					if (acom == NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in idref does not refer to the valid component instantiation.\n");
						throw DisConnectFailure();		
					}

					obj = acom->comref;
					Components::CCMObject_var providesComponent =Components::CCMObject::_narrow(obj);

					delete acom;
					acom = NULL;
					//end by wsf 2004.7.21
						
					//
					//get the cookie of the coonect
					//
					//modified by wsf 2004.7.21
					/*
					MapConnectInf::iterator connectIter=connectMap.find(string(connectId.transcode()));
			
					if (connectIter == connectMap.end())	
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in Invalid connection id.\n");
						throw DisConnectFailure();														
					}
					
					CORBA::String_var connectCookie=(*connectIter).second;					
					*/
					ArchConnection* aconn = arch->getArchConnection(connectId.transcode());
					if (aconn == NULL)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in Invalid connection id.\n");
						throw DisConnectFailure();		
					}

					CORBA::String_var connectCookie = aconn->cookie;

					delete aconn;
					aconn = NULL;

					Components::Cookie_var cookie = new Components::Cookie_impl(connectCookie.in());
					//end by wsf 2004.7.21
								
					//
					//unsubscribe to the provider
					//					
					try
					{
						providesComponent->unsubscribe (publishesIdentifier.transcode(),cookie.in());
					}
					catch(const CORBA::SystemException &)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in CORBA SystemException happened.\n");
						throw DisConnectFailure();																
					}
					catch(const Components::InvalidName &)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in InvalidName.\n");
						throw DisConnectFailure();																
					}	
					catch(const Components::InvalidConnection &)
					{
						TRACE0(LEVEL1,"Assembly: disconnect Components failed in InvalidConnection.\n");
						throw DisConnectFailure();																
					}															

					// add by wsf

					ArchConnection_var aconn_ = arch->getArchConnection(connectId.transcode());
					arch->delArchConnection(aconn_->cookie);
					//
					//erase the connect information into the connect map
					//	

					//__deleted 2001/7/21
					//connectMap.erase(connectIter);										

					//
					//delete the depend information into the componentMap
					//
					/*
					CORBA::StringSeq_var dependList=(*theIterator).second.depend;					
					CORBA::String_var consumesComponentId=CORBA::string_dup(((* iter).first).c_str());					
					CORBA::StringSeq tmpseq;
					delElementInSeq	(consumesComponentId.in(),dependList.in(),tmpseq);
					CORBA::StringSeq_var newDependList = new CORBA::StringSeq(tmpseq);
					(*iter).second.depend = newDependList;
					*/
					// end by wsf				
				}

				
			}
			
		}	

		
	}
}



//*****************************************************************************************
//since the targetNode depend on comparedNode
//so we can check whether the compared Node will depend targetNode directly or indirectly
//if the circle exist ,retur true
//*****************************************************************************************
bool 
Assembly_impl::circleExist(MapComponentInf::iterator comparedNodeIter,MapComponentInf::iterator targetNodeIter)
	throw (setConfigurationCompleteFailure)
{
	CORBA::StringSeq_var dependList=(* comparedNodeIter).second.depend;
					
	bool circleExistFlag=false;					
	int i=0;
					
	while((i<dependList->length())&&(!circleExistFlag))
	{
		CORBA::String_var dependComponent=dependList[(CORBA::ULong)i];
						
		MapComponentInf::iterator dependIter=componentMap.find(string(dependComponent.in()));	
					
		if (dependIter == componentMap.end())
		{
			TRACE0(LEVEL1,"Assembly: set configuration complete failed in invalid ComponentMap.\n");
			throw setConfigurationCompleteFailure();
		}				
		
		bool dependNeedSet=(* dependIter).second.SetConfigurationComplete;	
		
		if((dependNeedSet)&&(dependIter == targetNodeIter))
		{
			TRACE0(LEVEL6,"Assembly: there is a depend circle .\n");
			circleExistFlag = true;
		}
		else
		{		
			circleExistFlag = circleExist(dependIter,targetNodeIter);		
		}

		i++;					
	}
	
	return circleExistFlag;
}

void
Assembly_impl::setConfigurationComplete()
	throw (setConfigurationCompleteFailure)
{
	int len=componentMap.size();
	
	MapComponentInf::iterator iter=componentMap.begin();
	
	MapComponentInf::iterator iter_end=componentMap.end();		
	
	//deleted by wsf
	//
	//get the component number that need set ConfigurationComplete false in the ccd file
	//
//	int notSetNum=0;
//	while(iter!=iter_end)
//	{
//		bool needSet=(* iter).second.SetConfigurationComplete;
//		
//		if(!needSet)
//		{
//			notSetNum++;
//		}
//		
//		iter++;
//	}
	
//	int setNum=0;
	//end by wsf
	
	//
	//remove the depend circle
	//
	iter=componentMap.begin();
		
	while(iter!=iter_end)
	{
		TRACE1(LEVEL6,"Assembly: check configuration complete depend circle now for component %s.\n",((* iter).first).c_str());
			
		bool needSet=(* iter).second.SetConfigurationComplete;
			
		if(needSet)
		{
			CORBA::StringSeq_var dependList=(* iter).second.depend;					
			CORBA::StringSeq_var newDependList = new CORBA::StringSeq;
			int i=0;
					
			while(i<dependList->length())
			{
				CORBA::String_var dependComponent=dependList[(CORBA::ULong)i];						
				MapComponentInf::iterator dependIter=componentMap.find(string(dependComponent.in()));	
					
				if (dependIter == componentMap.end())
				{
					TRACE0(LEVEL1,"Assembly: set configuration complete failed in invalid ComponentMap.\n");
					throw setConfigurationCompleteFailure();
				}				
					
				if(!circleExist(dependIter,iter))
				{
					int len = newDependList->length();
					newDependList->length(len+1);
					newDependList[(CORBA::ULong)len] = dependComponent;
				}
				else
				{
					TRACE2(LEVEL6,"Assembly: since depend circle exist ,so remove the depend relation between component %s and component %s.\n",((* iter).first).c_str(),((* dependIter).first).c_str());
				}
						
				i++;					
			}
			(* iter).second.depend = newDependList;
		}		
		++iter;		
	}

	
	//
	//set configuration complete
	//

	//modified by wsf
	iter=componentMap.begin();
	while(iter!=iter_end)
	{
		try
		{
			setHelper(iter);
		}
		catch (setHelperFailure &)
		{
			TRACE0(LEVEL1,"Assembly: set configuration complete failed .\n");
			throw setConfigurationCompleteFailure();
		}
		++iter;
	}
	
//	while(setNum<(len-notSetNum))
//	{
//		iter=componentMap.begin();
//		
//		while(iter!=iter_end)
//		{
//			TRACE1(LEVEL6,"Assembly: set configuration complete now for component %s.\n",((* iter).first).c_str());
//			
//			bool needSet=(* iter).second.SetConfigurationComplete;
//			
//			if(needSet)
//			{
//				bool set=(* iter).second.ConfigurationComplete;
//				
//				if(!set)
//				{
//					CORBA::StringSeq_var dependList=(* iter).second.depend;
//					
//					bool dependSetFinish=true;
//					
//					int i=0;
//					
//					while((i<dependList->length())&&dependSetFinish)
//					{
//						CORBA::String_var dependComponent=dependList[(CORBA::ULong)i];
//						
//						TRACE1(LEVEL6,"Assembly: set configuration complete : the depend id is %s.\n",dependComponent.in());
//						
//						MapComponentInf::iterator dependIter=componentMap.find(string(dependComponent.in()));	
//					
//						if (dependIter == componentMap.end())
//						{
//							TRACE0(LEVEL1,"Assembly: set configuration complete failed in invalid ComponentMap.\n");
//							throw setConfigurationCompleteFailure();
//						}				
//						
//						bool dependSet=	(* dependIter).second.ConfigurationComplete;	
//						bool dependNeedSet=(* dependIter).second.SetConfigurationComplete;			
//						
//						if((!dependSet)&&dependNeedSet)
//						{
//							dependSetFinish=false;
//						}
//						
//						i++;					
//					}
//					
//					if(dependSetFinish)
//					{
//						(* iter).second.ConfigurationComplete=true;
//						
//						try
//						{							
//							((* iter).second.ComponentRef)->configuration_complete();
//							TRACE1(LEVEL4,"Assembly: setConfigurationComplete: component %s has been set.\n",((* iter).first).c_str());
//						}
//						catch(Components::InvalidConfiguration)
//						{
//							TRACE1(LEVEL1,"Assembly: set configuration complete of component %s failed.\n",((* iter).first).c_str());
//							throw setConfigurationCompleteFailure();
//						}					
//						
//						setNum++;
//					}
//				}
//			}
//					
//			++iter;
//		}
//		
//	}	

	//end by wsf

}

//wsf
void
Assembly_impl::setHelper(MapComponentInf::iterator iter)
	throw (setHelperFailure)
{
	bool needSet=(* iter).second.SetConfigurationComplete;
			
	if(needSet)
	{
		bool set=(* iter).second.ConfigurationComplete;
		if (!set)
		{
			CORBA::StringSeq_var dependList=(* iter).second.depend;
			int i=0;
					
			while(i<dependList->length())
			{
				CORBA::String_var dependComponent=dependList[i];
				
				TRACE1(LEVEL6,"Assembly: set configuration complete : the depend id is %s.\n",dependComponent.in());
				
				MapComponentInf::iterator dependIter=componentMap.find(string(dependComponent.in()));
				
				if (dependIter == componentMap.end())
				{
					TRACE0(LEVEL1,"Assembly: set configuration complete failed in invalid ComponentMap.\n");
					throw setHelperFailure();
				}
				
				bool dependSet=	(* dependIter).second.ConfigurationComplete;	
				bool dependNeedSet=(* dependIter).second.SetConfigurationComplete;	
				
				if ((!dependSet)&&dependNeedSet)
				{
					try
					{
						setHelper(dependIter);
					}
					catch (setHelperFailure &)
					{
						throw;
					}
				}
				i++;
			}
			
			(* iter).second.ConfigurationComplete=true;
						
			try
			{							
				((* iter).second.ComponentRef)->configuration_complete();
				TRACE1(LEVEL4,"Assembly: setConfigurationComplete: component %s has been set.\n",((* iter).first).c_str());
			}
			catch(Components::InvalidConfiguration)
			{
				TRACE1(LEVEL1,"Assembly: set configuration complete of component %s failed.\n",((* iter).first).c_str());
				throw setHelperFailure();
			}
		}
	}
}

//delete by xiao heping 2004/05/27
	
//*************************************************************************
//locateFile locate the file specified by the fileName,
//if any error occured return NULL string, 
//else return the correct fileName not the full path
//*************************************************************************

//char *
//Assembly_impl::locateFile(const char *fileName)
//{
//	
//	TRACE1(LEVEL6,"the cadFilePath is %s \n",fileName);
//	
//#ifdef WIN32
//	
//	WIN32_FIND_DATA FindFileData;
//	HANDLE hFind;
//		
//	hFind = FindFirstFile(fileName, &FindFileData);
//	
//	if (hFind == INVALID_HANDLE_VALUE) 
//	{
//    		TRACE1(LEVEL2,"Assembly: can not find the file %s .\n",fileName);     		
//    		return NULL;
//  	} 
//  	
//  	CORBA::String_var findFileName=CORBA::string_dup(FindFileData.cFileName);  	
//  	
//    	FindClose(hFind);
//#endif	
//	
//	TRACE1(LEVEL6,"Assembly: locate the cad file %s failed .\n",findFileName);	
//	
//	return findFileName._retn();
//	
//}  



//char *
//Assembly_impl::uuidGenerator()
//{
//	return UuidGenerator::generateUuid();
//	/*
//	//
//	//create the UUID
//	//
//	uuidtype uid;
//	uuid_create(&uid);
//	
//	//
//	//transfer the UUID to the string type
//	//
//	
//	char *str=(char *)(malloc(37));
//	
//	int j=sprintf(str,"%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", uid.time_low, uid.time_mid,
//        	uid.time_hi_and_version, uid.clock_seq_hi_and_reserved,
//        	uid.clock_seq_low);
//	
//	for (int i = 0; i < 6; i++)
//	{
//        	j+=sprintf(str+j,"%2.2x", uid.node[(CORBA::ULong)i]);	
//        }
//	
//	CORBA::String_var objUUID=CORBA::string_dup(str);
//	
//	free(str);	
//					
//	return objUUID._retn();	
//	*/
//}
//end delete   	

//deleded 2004.7.21
//wsf
/*
void
Assembly_impl::delElementInSeq(const char* element,const CORBA::StringSeq & srcSeq,CORBA::StringSeq & newSeq)
{
	for(int i=0; i<srcSeq.length(); i++)	
	{
		if(!(strcmp(srcSeq[i],element) == 0))
		{
			int len = newSeq.length();
			newSeq.length(len+1);
			newSeq[len] = srcSeq[i];
		}			
	}
}
*/


