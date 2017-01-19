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
 
#ifdef WIN32
#	include <direct.h>
#	include <Windows.h>
#else
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#endif

#include <ComponentInstallation_impl.h>
#include <XMLParser.h>

#include <iostream>
#include <errno.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>

#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <CommonFunc.h>


using namespace Components::Deployment;
using namespace STARCCM::Deployment;
using namespace std;



ComponentInstallation_impl::ComponentInstallation_impl(DeployDomainMgr_ptr manager,const char *directory,bool _verbose)
	: verbose(_verbose)
{
	
	rootDir=directory;	
	DomainMgr=DeployDomainMgr::_duplicate(manager);

	if (verbose) {
		cout << "ComponentInstallation: running." << endl<<endl;
	}
	
	
}

ComponentInstallation_impl::~ComponentInstallation_impl ()
{	
  
  	if (verbose) 
  	{
    		cout << "ComponentInstallation: terminating." << endl;
  	}
}

 
void
ComponentInstallation_impl::install (const char * implUUID, const char * component_loc)				     
		throw(  Components::Deployment::InvalidLocation,
			Components::Deployment::InstallationFailure,
			CORBA::SystemException)
{
	
	string impluuid; 
	MapImplInf::iterator theIterator;
	ImplInf impl;
	
		
	if(verbose)
	{
		cout<<"ComponentInstallation: install component impl "<<implUUID<<endl<<endl;
	}
	

	//
	//first check whether the implementation has been installed
	//

	impluuid=string(implUUID);		

	theIterator=implMap.find(impluuid);
	
	if (theIterator != implMap.end())
	{		
		TRACE1(LEVEL2,"ComponentInstallation: implementation %s has already been installed.\n",implUUID);		
		return;
	}
		
	impl.location=component_loc;		
	impl.localLocation=getLocalLocation(implUUID,rootDir.in());

	if((impl.localLocation).in()==NULL)
	{
		TRACE0(LEVEL1,"ComponentInstallation: the target location of the implementation created failure.\n");
		throw Components::Deployment::InstallationFailure();
	}	
	
	//
	//download the component implementation using XMLParser
	//
	try
	{
		download(implUUID,component_loc,(impl.localLocation).in());
	}
	catch(const InvalidLocation &)
	{
		throw;
	}
	catch(const InstallationFailure &)
	{
		throw;
	}
	catch(const CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw CORBA::UNKNOWN();
	}
	   
		
	//Add the component info into the map
	implMap.insert(MapImplInf::value_type(impluuid,impl));	
	
	//
	//register the component implementation into the DomainConf
	//
	
	try
	{	
		TRACE3(LEVEL6,"ComponentInstallation: registerimpl impl %s from %s to %s. \n",implUUID,component_loc,impl.localLocation);		
		DomainMgr->registerImpl(implUUID,component_loc,impl.localLocation);
	}
	catch(const ObjectIDExist &)
	{
		TRACE1(LEVEL1,"ComponentInstallation: register implementation %s failed. implUUID has existed\n",implUUID);
		throw Components::Deployment::InstallationFailure();
	} 
	catch(const InfoMissed &)
	{
		TRACE1(LEVEL1,"ComponentInstallation: register implementation failed.info missed\n",implUUID);
		throw Components::Deployment::InstallationFailure();
	}
	catch(const XMLParserError &)
	{
		TRACE1(LEVEL1,"ComponentInstallation: register implementation failed.XMLParser error\n",implUUID);
		throw Components::Deployment::InstallationFailure();
	}
	catch(...)
	{
		throw CORBA::UNKNOWN();
	}
	
	//
	//output the installed succeed implementation information
	//

	theIterator=implMap.find(impluuid);
	
	
	
	
	if(verbose)
	{
		cout<<"ComponentInstallation: succeed install component "
		    <<impluuid
		    <<" from the "
		    <<(*theIterator).second.location 
		    <<" to local "
		    <<(*theIterator).second.localLocation 		   
		    <<endl<<endl;
	}
}


void 
ComponentInstallation_impl::replace(const char* implUUID, const char* component_loc)
		throw(  Components::Deployment::InvalidLocation,
              		Components::Deployment::InstallationFailure,
             		CORBA::SystemException)
{

	//it will be implemented in dynamic deployment in the future
	
	if(verbose)
	{
		cout<<"ComponentInstallation: in replace implementation "<<implUUID<<endl<<endl;
	}
	
		
	MapImplInf::iterator theIterator;
	
	string impluuid;
	impluuid=string(implUUID);		

	theIterator=implMap.find(impluuid);
	
	if (theIterator == implMap.end())
	{
		TRACE1(LEVEL1,"ComponentInstallation: replace implementation %s failed for it not exist.\n",implUUID);		
		
		throw Components::Deployment::InstallationFailure();
		
	}
	
	try
	{
		remove (implUUID);
		
		install (implUUID, component_loc);
		
	}
	catch(const Components::Deployment::UnknownImplId &)                            
	{
		TRACE0(LEVEL1,"ComponentInstallation : remove for replace failed.\n");		
		throw Components::Deployment::InstallationFailure();
	}
	catch(const Components::RemoveFailure &)
	{
		TRACE0(LEVEL1,"ComponentInstallation : remove for replace failed.\n");		
		throw Components::Deployment::InstallationFailure();		
	}	
	catch(const Components::Deployment::InvalidLocation &)	      
	{
		TRACE0(LEVEL1,"ComponentInstallation : install for replace failed.\n");		;
		throw;
	}
	catch(const Components::Deployment::InstallationFailure &)
	{
		TRACE0(LEVEL1,"ComponentInstallation : install for replace failed.\n");		;
		throw;
	}
	
	
	if (verbose)
		cout << "ComponentInstallation: succeed replace implementation "
		     << implUUID
		     <<" to "
		     <<component_loc
		     << endl<<endl;
	
	return;	
}


void 
ComponentInstallation_impl::remove(const char* implUUID)
		throw(  Components::Deployment::UnknownImplId,
              		Components::RemoveFailure,
              		CORBA::SystemException)
{
	
	if(verbose)
	{
		cout<<"ComponentInstallation: in remove implementation "<<implUUID<<endl<<endl;
	}
	
	MapImplInf::iterator theIterator;

	string impluuid;
	impluuid=string(implUUID);	
	
	theIterator=implMap.find(impluuid);	
	
	if (theIterator == implMap.end())
	{
		TRACE1(LEVEL1,"ComponentInstallation: remove unknown implementation %s.\n",implUUID);		
		throw Components::Deployment::UnknownImplId ();
	}
	
	CORBA::String_var path=(*theIterator).second.localLocation;
	
	//deleted by wsf
//	char c;
//	cout<<"ComponentInstallation:Do you really want to remove "<<path<<endl;
//	cout << "Enter 'y' for ok, 'n' for refuse:\n";
//	cout << "> ";
//	cin >> c;
//	
//	if((c=='n')||(c=='N'))
//	{
//		cout<<"ComponentInstallation: cancel remove implementation "<<implUUID<<endl<<endl;
//		return;
//	}
	//end by wsf

#ifdef WIN32	
	if(_rmdir(path.in())==-1)
#else
	if(rmdir(path.in()) == -1)
#endif
	{
		if(errno!=ENOTEMPTY)
		{
			TRACE1(LEVEL5,"ComponentInstallation: remove path %s error.\n",path.in());			
			throw Components::RemoveFailure();
		}

		if(HelpFun::deleteDir(path)==-1)
		{
			TRACE1(LEVEL5,"ComponentInstallation: delete implementation directory %s failed.\n",path.in());			
			throw Components::RemoveFailure();
		}
#ifdef WIN32
		if(_rmdir(path.in()) == -1)
#else
		if(rmdir(path.in()) == -1)
#endif
		{
			TRACE1(LEVEL5,"ComponentInstallation: delete implementation directory %s failed.\n",path.in());				
			throw Components::RemoveFailure();
		}
	}
	
	implMap.erase(theIterator);
	
	//
	//remove the implementation object from the DomainConf file
	//
	try
	{
		DomainMgr->removeObject(IMPLEMENTATION,implUUID);
	}
	catch(const ObjectNotExist &)
	{
		TRACE1(LEVEL1,"ComponentInstallation: remove implementation %s from DomainConf file failed for it not exist.\n",implUUID);
		throw Components::RemoveFailure();
	}
	catch(const InfoMissed &)
	{
		TRACE1(LEVEL1,"ComponentInstallation: remove implementation %s from DomainConf file failed for info missed.\n",implUUID);
		throw Components::RemoveFailure();		
	}
	catch(const XMLParserError &)
	{
		TRACE1(LEVEL1,"ComponentInstallation: remove implementation %s from DomainConf file failed for XMLParser error.\n",implUUID);
		throw Components::RemoveFailure();		
	}
	catch(...)
	{
		throw CORBA::UNKNOWN();
	}
	
		
	if (verbose)
	{
		cout << "ComponentInstallation: succeed remove implementation "<< implUUID <<endl<<endl;
	}
}


Components::Deployment::Location 
ComponentInstallation_impl::get_implementation(const char* implUUID)
        	throw(  Components::Deployment::UnknownImplId,
              		Components::Deployment::InstallationFailure,
              		CORBA::SystemException)
{

	string impluuid;
	MapImplInf::iterator theIterator;	
	
	if(verbose)
	{
		cout<<"ComponentInstallation: get_implementation of "<<implUUID<<endl<<endl;
	}
	
	impluuid=CORBA::string_dup(implUUID);		
			
	theIterator = implMap.find (impluuid);
	
	if (theIterator == implMap.end())
	{
		TRACE1(LEVEL1,"ComponentInstallation:get implementation can not find the implUUID %s.",implUUID);		
		throw Components::Deployment::UnknownImplId ();
	}
	
	CORBA::String_var location=(*theIterator).second.localLocation;
	
	if(verbose)
	{
		cout<<"ComponentInstallation: get_implementation finished\n"<<endl<<endl;
	}
	
	return location._retn();	
	
}

void 
ComponentInstallation_impl::recover()
       	throw(Components::Deployment::RecoverFailure,
              	CORBA::SystemException)
{
	return;
}              


//******************************************************************************
//localLocation will make directory according to the rootdir and the implUUID
//in this implementation the target directory is directory is "rootdir\implUUID"
//if create succeed , return the directory created
//if any error happend including the directory already exist, return  NULL 
//******************************************************************************

char *
ComponentInstallation_impl::getLocalLocation(const char *implUUID,const char *dir)	
{
	
	//
	//according to different rules to create different target directory
	//now we assume the target directory is "rootdir\implUUID"
	//
		
	CORBA::String_var targetDir=dir;	

#ifdef STARCCMSTRINGADD
	targetDir = HelpFun::CORBA_string_add(targetDir, PATH_DELILIMITOR);
	targetDir = HelpFun::CORBA_string_add(targetDir, CORBA::string_dup(implUUID));
#else	
	targetDir+=CORBA::string_dup(PATH_DELILIMITOR);
	targetDir+=CORBA::string_dup(implUUID);
#endif	
	
	TRACE1(LEVEL6,"ComponentInstallation: localLocation: the target dir is %s.\n",targetDir.in());		
	
	//
	//create target directory	
	//

#ifdef WIN32	
	if(_mkdir(targetDir.in()) == 0)
#else
	if(mkdir(targetDir.in(),0775) == 0)
#endif
	{			
		return 	targetDir._retn();
	}
	else if(errno==EEXIST)
	{		
		TRACE1(LEVEL1,"ComponentInstallation: localLocation: the target %s exist.\n",targetDir.in());		
		return NULL;		
	}
	else		
	{
		TRACE1(LEVEL1,"ComponentInstallation: localLocation: create the target %s failed.\n",targetDir.in());		
		return NULL;
	}
	
}

void
ComponentInstallation_impl::download(const char *implUUID, const char *component_loc,const char *localLocation)
		throw(  Components::Deployment::InvalidLocation,
			Components::Deployment::InstallationFailure,
			CORBA::SystemException)
{
	//download the component from the component_loc to local
	//to be implemented using XML parser
	
	if(verbose)
	{
		cout<<"ComponentInstallation: download component "
		    <<implUUID\
		    <<" from the "
		    <<component_loc
		    <<" to "
		    <<localLocation		    
		    <<endl;		    
	}
	
	
	//
	//actually ComponentInstallation should judge which host the component lies in ,and which file 
	//transfer protocol is used .this informantion should be got from the component_loc parameter.
	//at last select the proper fileaccessor object to get the file.since we just implement the 
	//basic scenario, so we assume the ComponentInstallation always download the file from the
	// host ComponentInstalltion object lies in.
	//

	//
	//locate the csd file in the cmoponent_loc directory
	//
	CORBA::String_var filePath=component_loc;
	
#ifdef STARCCMSTRINGADD
	filePath = HelpFun::CORBA_string_add(filePath, PATH_DELILIMITOR);
	filePath = HelpFun::CORBA_string_add(filePath, CORBA::string_dup("*.csd"));
#else	
	filePath+=CORBA::string_dup(PATH_DELILIMITOR);
	filePath+=CORBA::string_dup("*.csd");
#endif	

  	CORBA::String_var fileName=component_loc;

	CORBA::String_var realFileName = HelpFun::locateFile(filePath.in());
	if(realFileName == NULL)
	{
		TRACE1(LEVEL1,"ComponentInstallation: download failed for no csd file in the path %s.\n",component_loc);
		throw Components::Deployment::InvalidLocation();
	}

#ifdef STARCCMSTRINGADD
	fileName = HelpFun::CORBA_string_add(fileName, PATH_DELILIMITOR);
	fileName = HelpFun::CORBA_string_add(fileName, realFileName);
#else  	
  	fileName += CORBA::string_dup(PATH_DELILIMITOR);
	fileName += CORBA::string_dup(realFileName);  	
#endif  	
  	
	TRACE1(LEVEL6,"the csd file is %s. \n",fileName.in());	
	//
	//parse the csd file to get the information of the implUUID and transfer them to the target directory
	//
	
	//
	//construct the DOM tree using the Xerces-c XML parser from apache
	//
	
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser->initialize(fileName.in());
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE0(LEVEL1,"ComponentInstallation: XMLParser initialized failed. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw Components::Deployment::InstallationFailure();
	}    
  	
	
	//
	//parse the component impl information
	//
	
	DOM_Node root = parser->getRootNode("softpkg");
	
	if(root==NULL)
	{
		TRACE0(LEVEL1,"ComponentInstallation: Invalid sofpkg file: have no sofpkg element. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::Deployment::InstallationFailure();
	}
	 

        DOM_Node implNode=parser->findChildByAttr(root,"implementation","id",implUUID);

	if(implNode==NULL)
	{
		TRACE0(LEVEL1,"ComponentInstallation: Invalid sofpkg file:  have no implementation element. \n");				
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw Components::Deployment::InstallationFailure();
	}        
	
        
        //
        //according to the csd , we need to download all the file relate with the implUUI
        //include :dependency,code,servantcode,psscode,ccd 
        //and the home/component property files denoted by the ccd
        //
	                
 	DOM_NodeList implChildren=implNode.getChildNodes();
        unsigned int implChildrenLen=implChildren.getLength();

	CORBA::String_var sourceLocation;	
	CORBA::String_var targetLocation;	
	
	DOM_Node implChild;
	DOMString implChildName;
				        
	//
	//process the dependency code
	//
	
	for(unsigned int iterator=0;iterator<implChildrenLen;iterator++)
	{
		//
		//initialize the directory info	
		//
		sourceLocation=component_loc;	
		targetLocation=localLocation;	

#ifdef STARCCMSTRINGADD
                sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
		targetLocation = HelpFun::CORBA_string_add(targetLocation, PATH_DELILIMITOR);		
		
#else		
                sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
		targetLocation+=CORBA::string_dup(PATH_DELILIMITOR);
				
#endif		
		
		implChild=implChildren.item(iterator);
		implChildName=implChild.getNodeName();	
					
		if(implChildName.equals("dependency"))
		{
			DOM_Node valuefactoryNode=parser->findChild(implChild,"valuetypefactory");
			if(valuefactoryNode!=NULL)			
			{
				DOMString location=parser->parseFileInArchive(valuefactoryNode);			
								
				if(location==NULL)
				{									
					TRACE0(LEVEL1,"ComponentInstallation: valuetypefactory have no valid fileinarchive element.\n");					
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}
					
					throw Components::Deployment::InstallationFailure();
				}

#ifdef STARCCMSTRINGADD
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, implUUID);
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
				targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else
				sourceLocation+=implUUID;
				sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
				sourceLocation+=CORBA::string_dup(location.transcode());
				targetLocation+=CORBA::string_dup(location.transcode());
#endif
				
				if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
				{		
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}
					
					throw Components::Deployment::InstallationFailure();
				}
				
			}				
			else
			{
				TRACE0(LEVEL5,"ComponentInstallation:sofpkg descriptor file have no valuefactory dependency element.\n");				
			}
		
		}
		else if(implChildName.equals("code")||
			implChildName.equals("servantcode")||
			implChildName.equals("psscode"))
		{
				
			DOMString location=parser->parseFileInArchive(implChild);
								
			if(location==NULL)
			{									
				TRACE0(LEVEL1,"ComponentInstallation : Invalid softpkg file: invalid fileinarchive element. \n");				
					
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
					
				throw Components::Deployment::InstallationFailure();
			}

#ifdef STARCCMSTRINGADD
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, implUUID);
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
			targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else
			sourceLocation+=implUUID;
			sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
			sourceLocation+=CORBA::string_dup(location.transcode());
			targetLocation+=CORBA::string_dup(location.transcode());
#endif
				
			if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
			{
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
					
				throw Components::Deployment::InstallationFailure();
			}
			
		}
		else if(implChildName.equals("descriptor"))		
		{
			//
			//get the ccd file name
			//

			DOMString location=parser->parseFileInArchive(implChild);								
								
			if(location==NULL)
			{									
				
				TRACE0(LEVEL1,"ComponentInstallation : Invalid softpkg file: invalid fileinarchive element. \n");
					
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
					
				throw Components::Deployment::InstallationFailure();
			}
			
			CORBA::String_var ccdFileName=component_loc;

#ifdef STARCCMSTRINGADD
			ccdFileName = HelpFun::CORBA_string_add(ccdFileName, PATH_DELILIMITOR);
			ccdFileName = HelpFun::CORBA_string_add(ccdFileName, implUUID);
			ccdFileName = HelpFun::CORBA_string_add(ccdFileName, PATH_DELILIMITOR);
			ccdFileName = HelpFun::CORBA_string_add(ccdFileName, CORBA::string_dup(location.transcode()));
#else			
			ccdFileName+=CORBA::string_dup(PATH_DELILIMITOR);
			ccdFileName+=implUUID;
			ccdFileName+=CORBA::string_dup(PATH_DELILIMITOR);
  			ccdFileName+=CORBA::string_dup(location.transcode());  				
#endif
			
			//
			//copy ccd file to the target dir
			//

#ifdef STARCCMSTRINGADD
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, implUUID);
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
			sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
			targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else							
			sourceLocation+=implUUID;
			sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
			sourceLocation+=CORBA::string_dup(location.transcode());									
			targetLocation+=CORBA::string_dup(location.transcode());
#endif			
			
			if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
			{
					
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
					
				throw Components::Deployment::InstallationFailure();
			}	
			
			
			//
			// XMLParser to process ccd file initialize
			//
			
			XMLParser *ccdParser=new XMLParser();
			
			try
			{
				ccdParser->initialize(ccdFileName.in());
			}
			catch(const STARCCM::Deployment::XMLInitializeFailed &)
			{
				TRACE0(LEVEL1,"ComponentInstallation: XMLParser initialized failed. \n");				
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}				
				
				if(ccdParser!=NULL)
				{
					delete ccdParser;
					ccdParser=NULL;
				}	
				
				throw Components::Deployment::InstallationFailure();
			}    

			DOM_Node ccdRoot = ccdParser->getRootNode("corbacomponent");
			
			if(ccdRoot==NULL)
			{
				TRACE0(LEVEL1,"ComponentInstallation: Invalid component descriptor file: have no corbacomponent element. \n");				
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				
				if(ccdParser!=NULL)
				{
					delete ccdParser;
					ccdParser=NULL;
				}
				
				throw Components::Deployment::InstallationFailure();
			}
			
			//
			//process the componentproperties file specified in the ccd file
			//
			
			DOM_Node componentPropertiesNode=ccdParser->findChild(ccdRoot,"componentproperties");
										       
			if(componentPropertiesNode!=NULL)
			{
				
				DOMString location=ccdParser->parseFileInArchive(componentPropertiesNode);								
								
				if(location==NULL)
				{									
					TRACE0(LEVEL1,"ComponentInstallation : Invalid ccd file: invalid fileinarchive element. \n");
				
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}					
					
					if(ccdParser!=NULL)
					{
						delete ccdParser;
						ccdParser=NULL;
					}
					
					throw Components::Deployment::InstallationFailure();
				}
				
				
				sourceLocation=component_loc;	
				targetLocation=localLocation;

#ifdef STARCCMSTRINGADD
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, implUUID);
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
				
				targetLocation = HelpFun::CORBA_string_add(targetLocation, PATH_DELILIMITOR);
				targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else					
				sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
				sourceLocation+=implUUID;
				sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
				sourceLocation+=CORBA::string_dup(location.transcode());
				
				targetLocation+=CORBA::string_dup(PATH_DELILIMITOR);
				targetLocation+=CORBA::string_dup(location.transcode());
#endif				
				
				if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
				{
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}
					
					if(ccdParser!=NULL)
					{
						delete ccdParser;
						ccdParser=NULL;
					}
					
					throw Components::Deployment::InstallationFailure();
				}		
				
			}				
			else
			{
				TRACE0(LEVEL5,"ComponentInstallation: ccd file have no componentProperties element.\n");
				
			}
			
			//
			//process the homeproperties file specified in the ccd file
			//
			
			DOM_Node homePropertiesNode=ccdParser->findChild(ccdRoot,"homeproperties");
			
			if(homePropertiesNode!=NULL)
			{
				
				DOMString location=ccdParser->parseFileInArchive(homePropertiesNode);								
								
				if(location==NULL)
				{									
					TRACE0(LEVEL1,"ComponentInstallation: Invalid ccd file: invalid fileinarchive element. \n");
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}
					
					if(ccdParser!=NULL)
					{
						delete ccdParser;
						ccdParser=NULL;
					}
					
					throw Components::Deployment::InstallationFailure();
				}
					
				sourceLocation=component_loc;	
				targetLocation=localLocation;

#ifdef STARCCMSTRINGADD
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, implUUID);
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, PATH_DELILIMITOR);
				sourceLocation = HelpFun::CORBA_string_add(sourceLocation, CORBA::string_dup(location.transcode()));
				
				targetLocation = HelpFun::CORBA_string_add(targetLocation, PATH_DELILIMITOR);
				targetLocation = HelpFun::CORBA_string_add(targetLocation, CORBA::string_dup(location.transcode()));
#else				
				sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
				sourceLocation+=implUUID;
				sourceLocation+=CORBA::string_dup(PATH_DELILIMITOR);
				sourceLocation+=CORBA::string_dup(location.transcode());
				
				targetLocation+=CORBA::string_dup(PATH_DELILIMITOR);
				targetLocation+=CORBA::string_dup(location.transcode());
#endif
				
				if(HelpFun::copyFile(sourceLocation.in(),targetLocation.in())==-1)
				{
					
					if(parser!=NULL)
					{
						delete parser;
						parser=NULL;
					}
					
					if(ccdParser!=NULL)
					{
						delete ccdParser;
						ccdParser=NULL;
					}
					
					throw Components::Deployment::InstallationFailure();
				}		
				
			}				
			else
			{
				TRACE0(LEVEL5,"ComponentInstallation: ccd file have no homeProperties element.\n");				
			}	
			
			if(ccdParser!=NULL)
			{
				delete ccdParser;
				ccdParser=NULL;
			}		
			
		}//end of if
	
	}//end of for
		
	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}
 	
}
			
//deleted by xiao heping 2004/05/27

//int 
//ComponentInstallation_impl::deleteDir(const char *dir)
//{
//	
//	string directory="";
//	directory=directory+dir+"\\*.*";
//	
//	WIN32_FIND_DATA FindFileData;
//	HANDLE hFind;
//
//	hFind = FindFirstFile(directory.c_str(), &FindFileData);
//
//	while(FindNextFile(hFind,&FindFileData)!=0)
//	{
//		TRACE1(LEVEL6,"deleteDir: the file name is %s. \n",FindFileData.cFileName);		
//	
//		string fileName=FindFileData.cFileName;
//
//		if((fileName!=".")&&(fileName!=".."))
//		{
//			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//			{
//				string subDir="";
//				subDir=subDir+dir+"\\"+fileName;
//				
//				if(deleteDir(subDir.c_str())==-1)
//				{					
//					TRACE1(LEVEL1,"deleteDir %s failed. \n",FindFileData.cFileName);					
//					FindClose(hFind);
//					return -1;
//				}
//					
//				if(_rmdir(subDir.c_str())==-1)
//				{
//					TRACE1(LEVEL1,"deleteDir %s failed. \n",FindFileData.cFileName);
//					FindClose(hFind);
//					return -1;
//				}
//			}
//			else
//			{
//				string subFile="";
//				subFile=subFile+dir+"\\"+fileName;
//				if(DeleteFile(subFile.c_str())==0)
//				{
//					TRACE1(LEVEL1,"deleteDir: delete file %s failed.",subFile.c_str());
//					
//					FindClose(hFind);
//					return -1;
//				}
//			}
//		}//end of if to skip . and ..
//	} //end of while
//  
//	 FindClose(hFind);
//	 return 0;
//}
//
//
//int 
//ComponentInstallation_impl::copyFile(const char *source, const char *target)
//{
//
//	TRACE1(LEVEL6,"copyFile: the source file is %s .\n",source);
//	TRACE1(LEVEL6,"copyFile: the target file is %s .\n",target);
//
//#ifdef WIN32	
//
//	if(CopyFile(source,target,false)==0)
//	{
//		TRACE0(LEVEL1,"copyFile: Copy file into the directory specified failed..\n");
//		
//		return -1;
//	}
//#endif
//	return 0;
//}

//end delete


	
