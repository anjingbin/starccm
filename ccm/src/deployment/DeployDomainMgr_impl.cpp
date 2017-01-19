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

#include <DeployDomainMgr_impl.h>
#include <Configurator_impl.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>

#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#ifdef WIN32
#include <direct.h>
#include <Windows.h>
#else
#include<unistd.h>
#endif

using namespace std;
using namespace Components;
using namespace STARCCM::Deployment;



DeployDomainMgr_impl::DeployDomainMgr_impl(bool verbose_,const char *file)
	:verbose(verbose_)
{	
	
	if(verbose)
	{
		cout<<"DeployDomainMgr: DeployDomainMgr start."<<endl;
	}
	
	CORBA::String_var directory=CORBA::string_dup(file);
	DomainConfFile=directory;
#ifdef STARCCMSTRINGADD
	DomainConfFile = HelpFun::CORBA_string_add(DomainConfFile, PATH_DELILIMITOR);
	DomainConfFile = HelpFun::CORBA_string_add(DomainConfFile, CORBA::string_dup("DomainConf.xml"));
#else	
	DomainConfFile+=CORBA::string_dup(PATH_DELILIMITOR);
	DomainConfFile+=CORBA::string_dup("DomainConf.xml");
#endif
}

void
DeployDomainMgr_impl::initialize()
	throw(STARCCM::Deployment::InvalidDomainConfFile)
{
	

	if(verbose)
	{
		cout<<"DeployDomainMgr : initialize"<<endl;
	}	
	
	//initialize the XMLParser
	
	parser=new XMLParser();
		
	try
	{
		parser->initialize(DomainConfFile.in());
	}
	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE0(LEVEL1,"DeploydomainMgr: XMLParser initialized failed.\n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw STARCCM::Deployment::InvalidDomainConfFile();
	}
	
	
	root=parser->getRootNode("domainconf");
	
	if(root==NULL)
	{
		TRACE0(LEVEL1,"DeploydomainMgr: initialize failed: invalid DomainConf file .\n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw STARCCM::Deployment::InvalidDomainConfFile();
	}
	
	
	if(verbose)
	{
		cout<<"DeployDomainMgr: running"<<endl;
	}
		
}

DeployDomainMgr_impl::~DeployDomainMgr_impl ()
{
 
  	
    	if(parser!=NULL)
    	{    		
    		delete parser;
    	}
    	
    	if(verbose)
    	{
        	cout << "DeployDomainMgr: terminating." << endl;
        }
 
}


//***********************************************************************************************
//registerObject will register the object : 
//ServerActivator: id, loc
//ConsoleFlleAccessorFactory:  loc
//***********************************************************************************************

void 
DeployDomainMgr_impl::registerObject(STARCCM::Deployment::ObjectType type,                        
                        const char* id,
                        const char* loc)
        	throw(STARCCM::Deployment::UnknownObjectType,
              		STARCCM::Deployment::ObjectIDExist,
              		STARCCM::Deployment::InfoMissed,
              		STARCCM::Deployment::XMLParserError,
              		CORBA::SystemException)
{		
	
	switch (type)
	{
		
		case SERVERACTIVATOR:
			{
				if(verbose)
				{
					cout<<"DeployDomainMgr: register object ServerActivator of "<<id<<endl;
				}

				DOM_Document doc=root.getOwnerDocument();
				DOM_Element rootElem = doc.getDocumentElement();
				DOM_Element serveractivatorElem=doc.createElement("serveractivator");
				serveractivatorElem.setAttribute("id", id);
				serveractivatorElem.setAttribute("corbaloc", loc);				
				rootElem.appendChild(serveractivatorElem);
				if(parser->printToFile(DomainConfFile.in())==-1)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: register serveractivator, DOM print to file failed.\n");
					throw STARCCM::Deployment::XMLParserError();
				}				
				
				break;
			}
		
		case CONSOLEFILEACCESSORFACTORY:
			{
				if(verbose)
				{
					cout<<"DeployDomainMgr: register object ConsoleFileAccessorFactory "<<endl;
				}
				
				DOM_Document doc=root.getOwnerDocument();
				DOM_Element rootElem = doc.getDocumentElement();
				
				DOM_Element ConsoleFileAccessorElem=doc.createElement("consolefileaccessorfactory");				
				ConsoleFileAccessorElem.setAttribute("corbaloc", loc);				
				
				rootElem.appendChild(ConsoleFileAccessorElem);
				
				if(parser->printToFile(DomainConfFile.in())==-1)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: register ConsoleFileAccessorFactory, DOM print to file failed.\n");
					throw STARCCM::Deployment::XMLParserError();
				}				
				
				break;		
			}
		//add by jxh--04/03/17
			case COMPONENTMANAGER:
			{
				if(verbose)
				{
					cout<<"DeployDomainMgr: register object COMPONENTMANAGER "<<endl;
				}
				
				DOM_Document doc=root.getOwnerDocument();
				DOM_Element rootElem = doc.getDocumentElement();
				
				DOM_Element ComponentManagerElem=doc.createElement("componentmanager");				
				ComponentManagerElem.setAttribute("id", id);
				ComponentManagerElem.setAttribute("corbaloc", loc);
				//DOM_Element corbaElem=doc.createElement("corbaLocElem");				
				
				rootElem.appendChild(ComponentManagerElem);
				//ComponentManagerElem.appendChild(corbaElem);
				
				
				if(parser->printToFile(DomainConfFile.in())==-1)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: register COMPONENTMANAGER, DOM print to file failed.\n");
					throw STARCCM::Deployment::XMLParserError();
				}				
				TRACE0(LEVEL1,"DeployDomainMgr: register COMPONENTMANAGER successful\n");
				break;		
			}
			//06/21
			case TRANSACTIONMANAGER:
			{
				if(verbose)
				{
					cout<<"DeployDomainMgr: register object TRANSACTIONMANAGER "<<endl;
				}
				
				DOM_Document doc=root.getOwnerDocument();
				DOM_Element rootElem = doc.getDocumentElement();
				
				DOM_Element TransactionManagerElem=doc.createElement("transactionmanager");				
				TransactionManagerElem.setAttribute("id", id);
				TransactionManagerElem.setAttribute("corbaloc", loc);
				//DOM_Element corbaElem=doc.createElement("corbaLocElem");				
				
				rootElem.appendChild(TransactionManagerElem);
				//ComponentManagerElem.appendChild(corbaElem);
				
				
				if(parser->printToFile(DomainConfFile.in())==-1)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: register TRANSACTIONMANAGER, DOM print to file failed.\n");
					throw STARCCM::Deployment::XMLParserError();
				}				
				TRACE0(LEVEL1,"DeployDomainMgr: register TRANSACTIONMANAGER successful\n");
				break;		
			}
			case APPLICATIONS:
			{
				if(verbose)
				{
					cout<<"DeployDomainMgr: register object APPLICATIONS "<<endl;
				}
											
						
							
				DOM_Node node;
				DOM_Node temnode;
				node=parser->findChild(root,"Applications");				
				if(node!=NULL)
				{
					temnode=parser->findChildByAttr(node,"Application","name",id);
					if(temnode==NULL)
					{
						DOM_Document doc=root.getOwnerDocument();
						DOM_Element ApplicationElem=doc.createElement("Application");				
						ApplicationElem.setAttribute("name", id);
						ApplicationElem.setAttribute("id", loc);
						TRACE0(LEVEL1,loc);
						node.appendChild(ApplicationElem);
						if(parser->printToFile(DomainConfFile.in())==-1)
						{
							TRACE0(LEVEL1,"DeployDomainMgr: register APPLICATIONS, DOM print to file failed.\n");
							throw STARCCM::Deployment::XMLParserError();
						}
						
					}
					break;	
				}					
				else
				{
					DOM_Document doc=root.getOwnerDocument();
					DOM_Element rootElem = doc.getDocumentElement();
					
					DOM_Element ApplicationsElem = doc.createElement("Applications");
					DOM_Element ComponentManagerElem=doc.createElement("componentmanager");				
					TRACE0(LEVEL1,"DeployDomainMgr: register componentmanager test successful!!\n");
					
					DOM_Element ApplicationElem=doc.createElement("Application");				
					ApplicationElem.setAttribute("name", id);
					ApplicationElem.setAttribute("id", loc);
					rootElem.appendChild(ApplicationsElem);
					ApplicationsElem.appendChild(ApplicationElem);
					if(parser->printToFile(DomainConfFile.in())==-1)
					{
						TRACE0(LEVEL1,"DeployDomainMgr: register APPLICATIONS, DOM print to file failed.\n");
						throw STARCCM::Deployment::XMLParserError();
					}				
				
				break;			
				}
			}
			//jxh--end	
		default: 
			{
				TRACE0(LEVEL1,"DeployDomainMgr: register Object with unknown object type. \n");
				throw STARCCM::Deployment::UnknownObjectType();
				break;
			}
	}
	
	return;
}


//***********************************************************************************************
//removeObject will remove the object and all the information related: 
//ComponentServer: componentserver element and all the componentserverref element
//Container: container element and all the containerref element
//Assembly: assembly element and all the assemblyref element
//implementation: imiplementation element
//***********************************************************************************************
void 
DeployDomainMgr_impl::removeObject(STARCCM::Deployment::ObjectType type,
                              const char* id)
        throw(STARCCM::Deployment::UnknownObjectType,
              STARCCM::Deployment::ObjectNotExist,
              STARCCM::Deployment::InfoMissed,
              STARCCM::Deployment::XMLParserError,
              CORBA::SystemException)
{
	
	switch (type)
	{
	
	
		case ASSEMBLY:
			{
 				DOM_Node assemblyNode=parser->findChildByAttr(root,"assembly","id",id);
 				if(assemblyNode==NULL)
 				{
 					TRACE1(LEVEL1,"DeployDomainMgr: remove Assembly %s failed :can not locate it in XML file.\n",id);
 					throw STARCCM::Deployment::ObjectNotExist();
 				}
 				
 				DOM_NodeList assemblyChildren=assemblyNode.getChildNodes();
        			unsigned int assemblyChildrenLen=assemblyChildren.getLength();	
	
				DOM_Node assemblyChild;
				DOMString assemblyChildName;				
				
				//
				//remove the assemlbyref node in implementation node
				//
				for(unsigned int iterator=0;iterator<assemblyChildrenLen;iterator++)
				{
					assemblyChild=assemblyChildren.item(iterator);	
					assemblyChildName=assemblyChild.getNodeName();

					if(assemblyChildName.equals("implementationref"))
					{
						
						DOMString ref=parser->getAttr(assemblyChild,"idref");
						if(ref==NULL)
						{
							TRACE2(LEVEL1,"DeployDomainMgr: remove assembly %s failed: %s ref is NULL\n",id,assemblyChildName);
							throw STARCCM::Deployment::InfoMissed();
						}
						
						DOM_Node node=parser->findChildByAttr(root,"implementation","id",ref.transcode());
						if(node==NULL)
						{
							TRACE2(LEVEL1,"DeployDomainMgr: remove assembly %s failed: %s not exist\n",id,assemblyChildName);
							throw STARCCM::Deployment::InfoMissed();
						}
						
						DOM_Node assemblyRefNode=parser->findChildByAttr(node,"assemblyref","idref",id);
						if(assemblyRefNode==NULL)
						{
							TRACE2(LEVEL1,"DeployDomainMgr: remove assembly %s failed: %s have no assemblyref node exist\n",id,assemblyChildName);
							throw STARCCM::Deployment::InfoMissed();
						}
												
						node.removeChild(assemblyRefNode);						
					}
				
				}
				
				//
				//remove assembly node
				//
				root.removeChild(assemblyNode);
				
				//
				//print the DOM tree to the file
				//
				if(parser->printToFile(DomainConfFile.in())==-1)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: remove assembly, DOM print to file failed.\n");
					throw STARCCM::Deployment::XMLParserError();
				}				
				
				break;
			
			}
		
		case IMPLEMENTATION:
			{
				
				if(verbose)
				{
					cout<<"DeployDomainMgr: remove object Implementation. "<<endl;
				}
 				
 				DOM_Node ImplementationNode=parser->findChildByAttr(root,"implementation","id",id);
 				if(ImplementationNode==NULL)
 				{
 					TRACE1(LEVEL1,"DeployDomainMgr: remove Implementation %s failed :can not locate it in XML file.\n",id);
 					throw STARCCM::Deployment::ObjectNotExist();
 				}

				root.removeChild(ImplementationNode);	
				if(parser->printToFile(DomainConfFile.in())==-1)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: remove implementation, DOM print to file failed.\n");
					throw STARCCM::Deployment::XMLParserError();
				}				
 								
				break;
			}
			
		default:
			{
				if(verbose)
				{
					cout<<"DeployDomainMgr: remove object with unknown type. "<<endl;
				}
				
				TRACE0(LEVEL1,"DeployDomainMgr: remove Object with unknown object type. \n");
				throw STARCCM::Deployment::UnknownObjectType();				
				break;
			}
			
	}
	
}             

//***********************************************************************************************
//getObject will get the corbaloc of the object 
//according to the type directory or according to the type and id
//***********************************************************************************************

STARCCM::Deployment::ObjLoc 
DeployDomainMgr_impl::getObject(STARCCM::Deployment::ObjectType type,
                        const char* id)
        	throw(STARCCM::Deployment::UnknownObjectType,
              		STARCCM::Deployment::ObjectNotExist,
              		STARCCM::Deployment::InfoMissed,
              		STARCCM::Deployment::XMLParserError,
              		CORBA::SystemException)
{
	
	if(verbose)
	{
		cout<<"DeployDomainMgr: get object "<<type<<endl;		
	}
	
	CORBA::String_var corbalocRef;
		
	DOM_Node node;
	DOMString value;
	
	switch (type)
	{
		case DEPLOYDOMAINMANAGER:						
			{
				node=parser->findChild(root,"deploydomainmgr");
				break;
			}

		// add by wsf	
		case DYNDEPLOYMENTMGR:						
			{
				node=parser->findChild(root,"dyndeploymentmgr");
				break;
			}	
		// end by wsf
				
		case COMPONENTINSTALLATION:
			{
				node=parser->findChild(root,"componentinstallation");
				break;
			}
			
		case ASSEMBLYFACTORY:
			{
				node=parser->findChild(root,"assemblyfactory");			
				break;
			}
			
		case FILEACCESSORFACTORY:
			{
				node=parser->findChild(root,"fileaccessorfactory");			
				break;
			}
			
		case CONSOLEFILEACCESSORFACTORY:
			{
				node=parser->findChild(root,"consolefileaccessorfactory");			
				break;		
			}
			
		case SERVERACTIVATOR:
			{				
				node=parser->findChildByAttr(root,"serveractivator","id",id);		
				break;		
			}
			
		case COMPONENTSERVER:
			{
				node=parser->findChildByAttr(root,"componentserver","id",id);			
				break;		
			}
			
		case CONTAINER:
			{
				node=parser->findChildByAttr(root,"container","id",id);		
				break;		
			}
				
		case ASSEMBLY:
			{
				node=parser->findChildByAttr(root,"assembly","id",id);		
				break;		
			}		
			
		case IMPLEMENTATION:
			{
				node=parser->findChildByAttr(root,"implementation","id",id);
				break;		
			}
		//add by jianxinhong--04/03/17												
		case COMPONENTMANAGER:
			{
				node = parser->findChildByAttr(root,"componentmanager","id",id);
			if(node==NULL)
			{
				TRACE0(LEVEL2,"DeployDomainMgr: Invalid DomainConf file.Element missed.\n");	
					
				throw STARCCM::Deployment::ObjectNotExist();
				
			}
			value=parser->getAttr(node,"corbaloc");
				
			if(value==NULL)
			{
				TRACE0(LEVEL2,"DeployDomainMgr: Invalid DomainConf file: corbalocElement missed.\n");	
							
				throw STARCCM::Deployment::InfoMissed();
			}			
				
			
			
			corbalocRef=CORBA::string_dup(value.transcode());		
			
			return corbalocRef._retn();
								
			}	
				//add by jianxinhong--04/06/21												
		case TRANSACTIONMANAGER:
			{
				node = parser->findChildByAttr(root,"transactionmanager","id",id);
			if(node==NULL)
			{
				TRACE0(LEVEL2,"DeployDomainMgr: Invalid DomainConf file.Element missed.\n");	
					
				throw STARCCM::Deployment::ObjectNotExist();
				
			}
			value=parser->getAttr(node,"corbaloc");
				
			if(value==NULL)
			{
				TRACE0(LEVEL2,"DeployDomainMgr: Invalid DomainConf file: corbalocElement missed.\n");	
							
				throw STARCCM::Deployment::InfoMissed();
			}			
				
			
			
			corbalocRef=CORBA::string_dup(value.transcode());		
			
			return corbalocRef._retn();
								
			}
		default:
			{
				TRACE0(LEVEL2,"DeployDomainMgr: register object with unknown type.\n");				
				throw UnknownObjectType();				
				break;
			}
									
	}//end of switch
	
	if(node==NULL)
	{
		TRACE0(LEVEL2,"DeployDomainMgr: Invalid DomainConf file.Element missed.\n");	
			
		throw STARCCM::Deployment::ObjectNotExist();
	}						
		

	value=parser->getAttr(node,"corbaloc");
		
	if(value==NULL)
	{
		TRACE0(LEVEL2,"DeployDomainMgr: Invalid DomainConf file: corbalocElement missed.\n");	
					
		throw STARCCM::Deployment::InfoMissed();
	}			
		
	
	
	corbalocRef=CORBA::string_dup(value.transcode());		
	
	return corbalocRef._retn();


}


//*********************************************************************************************
//getObjectValue get the value the object of specified type
//the object including :timeout,verbose,rootdir
//
//*********************************************************************************************
char *
DeployDomainMgr_impl::getObjectValue(STARCCM::Deployment::ObjectType type)
        	throw(STARCCM::Deployment::UnknownObjectType,
              		STARCCM::Deployment::ObjectNotExist,
              		STARCCM::Deployment::InfoMissed,
              		STARCCM::Deployment::XMLParserError,
              		CORBA::SystemException)
{
	CORBA::String_var value;	
		
	switch (type)
	{
		case DOMAINNAME:
			{				
				DOMString domainName=parser->getAttr(root,"name");
				if(domainName==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: domainname attribute is missed.\n");	
					throw STARCCM::Deployment::InfoMissed();					
				}
				
				value=CORBA::string_dup(domainName.transcode());
				
				return value._retn();
				
				break;
			}

		case TIMEOUT:
			{
				DOM_Node timeoutNode=parser->findChild(root,"timeout");
				if(timeoutNode==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: timeout element missed.\n");	
					throw STARCCM::Deployment::ObjectNotExist();
					
				}
				
				DOMString time=parser->getAttr(timeoutNode,"time");
				if(time==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: timeout have no time attr.\n");	
					throw STARCCM::Deployment::InfoMissed();					
					
				}
				
				value=CORBA::string_dup(time.transcode());
				
				return value._retn();
				
				break;
			}
		
		case VERBOSE:
			{
				DOM_Node verboseNode=parser->findChild(root,"verbose");
				if(verboseNode==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: verbose element missed.\n");	
					throw STARCCM::Deployment::ObjectNotExist();
					
				}
				
				DOMString open=parser->getAttr(verboseNode,"open");
				if(open==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: verbose have no open attr.\n");	
					throw STARCCM::Deployment::InfoMissed();					
					
				}
				
				value=CORBA::string_dup(open.transcode());
				
				return value._retn();				
				
				break;
			}

		case ROOTDIR:
			{

				DOM_Node rootdirNode=parser->findChild(root,"rootdir");
				if(rootdirNode==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: rootdir element missed.\n");	
					throw STARCCM::Deployment::ObjectNotExist();
					
				}
				
				DOMString directory=parser->getAttr(rootdirNode,"directory");
				if(directory==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: rootdir have no directory attr.\n");	
					throw STARCCM::Deployment::InfoMissed();					
					
				}
				
				value=CORBA::string_dup(directory.transcode());
				
				return value._retn();
								
				break;
			}
	//jxh---03/17
	case COMPONENTMANAGER:
			{

				DOM_Node rootdirNode=parser->findChild(root,"componentmanager");
				if(rootdirNode==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: rootdir element missed.\n");	
					throw STARCCM::Deployment::ObjectNotExist();
					
				}
				
				DOMString directory=parser->getAttr(rootdirNode,"id");
				if(directory==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: rootdir have no directory attr.\n");	
					throw STARCCM::Deployment::InfoMissed();					
					
				}
				
				value=CORBA::string_dup(directory.transcode());
				
				return value._retn();	
				break;	
			}						
				
	case DYNDEPLOYMENTMGR:
			{

				DOM_Node rootdirNode=parser->findChild(root,"dyndeploymentmgr");
				if(rootdirNode==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: rootdir element missed.\n");	
					throw STARCCM::Deployment::ObjectNotExist();
					
				}
				
				DOMString directory=parser->getAttr(rootdirNode,"corbaloc");
				if(directory==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: invalid DomainConf file: rootdir have no directory attr.\n");	
					throw STARCCM::Deployment::InfoMissed();					
					
				}
				
				value=CORBA::string_dup(directory.transcode());
				
				return value._retn();								
				
			}//jxh--end
		
		default:
			{
				TRACE0(LEVEL2,"DeployDomainMgr: register object with unknown type.\n");	
				throw UnknownObjectType();				
				break;
			}
	}//end of switch
					

}


//***********************************************************************************************
//get UUID list of object specified by the type
//including : Assembly, implementation ,ServerActivator, ComponentServer, Container
//***********************************************************************************************

STARCCM::Deployment::ObjIds* 
DeployDomainMgr_impl::getObjectUUIDs(STARCCM::Deployment::ObjectType type)
	throw(STARCCM::Deployment::UnknownObjectType,
		STARCCM::Deployment::InfoMissed, 
		CORBA::SystemException)
{
	
	CORBA::String_var nodeName;
	
	switch (type)
	{
		case ASSEMBLY:
			{
				nodeName=CORBA::string_dup("assembly");
				break;
			}

		case IMPLEMENTATION:
			{
				nodeName=CORBA::string_dup("implementation");
				break;
			}

		case SERVERACTIVATOR:
			{
				nodeName=CORBA::string_dup("serveractivator");
				break;
			}

		case COMPONENTSERVER:
			{
				nodeName=CORBA::string_dup("componentserver");
				break;
			}

		case CONTAINER:
			{
				nodeName=CORBA::string_dup("container");
				break;
			}
		//add by jianxinhong--04/03/17	
		case COMPONENTMANAGER:
			{
				nodeName=CORBA::string_dup("componentmanager");
				break;
		        }
		default:
			{
				TRACE0(LEVEL1,"DeployDomainMgr: getObjectUUIDs failed in invalid object type.\n");
				throw STARCCM::Deployment::UnknownObjectType();
				break;
			}
	}
	
	
	STARCCM::Deployment::ObjIds_var ids=new STARCCM::Deployment::ObjIds;
	
	DOM_NodeList rootChildren=root.getChildNodes();
        unsigned int rootChildrenLen=rootChildren.getLength();	
	
	DOM_Node rootChild;
	DOMString rootChildName;				
	
	for(unsigned int iterator=0;iterator<rootChildrenLen;iterator++)
	{
		rootChild=rootChildren.item(iterator);	
		rootChildName=rootChild.getNodeName();
		
		if(rootChildName.equals(nodeName.in()))
		{
			DOMString objectId=parser->getAttr(rootChild,"id");
			if(objectId==NULL)
			{
				TRACE0(LEVEL1,"DeployDomainMgr: getObjectUUIDs failed in invalid DomainConf file: node miss id attribute.\n");
				throw STARCCM::Deployment::InfoMissed();
			}
			
			int len=ids->length();
			ids->length(len+1);
			ids[(CORBA::ULong)len]=CORBA::string_dup(objectId.transcode());
		}
	}
	
	return ids._retn();	
												
}	

	        	
//***********************************************************************************************
//registerAssembly will add the Assembly element with id ,corbaloc and implref
//and according to the implids, to add assemblyref to the proper implementation element
//***********************************************************************************************

void 
DeployDomainMgr_impl::registerAssembly(const char* id,
                      const char* loc,
	              const STARCCM::Deployment::ObjIds& implids)
	        throw(STARCCM::Deployment::ObjectIDExist,
	              STARCCM::Deployment::InfoMissed,
	              STARCCM::Deployment::XMLParserError,
	              CORBA::SystemException)
{
	//
	//first check whether the id has been registered
	//
	DOM_Node node=parser->findChildByAttr(root,"ASSEMBLY","id",id);
	if(node!=NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: Assembly of id %s has existed.\n",id);
		throw STARCCM::Deployment::ObjectIDExist();
	}
	
	DOM_Document doc=root.getOwnerDocument();
	DOM_Element rootElem = doc.getDocumentElement();
				
	DOM_Element assemblyElem=doc.createElement("assembly");
	assemblyElem.setAttribute("id", id);
	assemblyElem.setAttribute("corbaloc", loc);				
	
	int len=implids.length();
	for(int i=0;i<len;i++)
	{	
		
		DOM_Node implementationNode =parser->findChildByAttr(root,"implementation","id",implids[(CORBA::ULong)i]);
		if(implementationNode==NULL)
		{
			TRACE1(LEVEL1,"DeployDomainMgr: register assembly locate implementation %s failed.\n",implids[(CORBA::ULong)i]);
			throw STARCCM::Deployment::InfoMissed();
		}
		//
		//judge whether the assembly have register the same implementation twice
		//
		DOM_Node assemblyRefNode=parser->findChildByAttr(implementationNode,"assemblyref","idref",id);
		if(assemblyRefNode==NULL)
		{
			DOM_Element assemblyRefElem=doc.createElement("assemblyref");
			assemblyRefElem.setAttribute("idref",id);		
			implementationNode.appendChild(assemblyRefElem);
		
			DOM_Element implementationRefElem=doc.createElement("implementationref");
			//string implidStr=string(implids[i].in());
			string implidStr=string(implids[i]);
			implementationRefElem.setAttribute("idref",implidStr.c_str());
			assemblyElem.appendChild(implementationRefElem);
		}
		
	}
				
	rootElem.appendChild(assemblyElem);
				
	if(parser->printToFile(DomainConfFile.in())==-1)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: register assembly DOM print to file failed.\n");
		throw STARCCM::Deployment::XMLParserError();
	}	
}



//***************************************************************************************
//register the assembly information
//***************************************************************************************

void 
DeployDomainMgr_impl::registerAssemblyInfo(const char* id,
                      	              const STARCCM::Deployment::HomeInfos& homeInformations,
                               	      const STARCCM::Deployment::ConnectInfos& connectInformations)
		throw(STARCCM::Deployment::ObjectNotExist,
		      STARCCM::Deployment::InfoMissed,
		      STARCCM::Deployment::XMLParserError,
		      CORBA::SystemException)
{	
	DOM_Node assemblyNode=parser->findChildByAttr(root,"assembly","id",id);
	if(assemblyNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: registerAssemblyInfo failed in locate assembly %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}
	
	//
	//add home node
	//	
	DOM_Document doc=root.getOwnerDocument();
	DOM_Element rootElem = doc.getDocumentElement();	
	
	int i;	
	for(i=0;i<homeInformations.length();i++)
	{		
		//add by wsf
		DOM_Node tmpNode = parser->findChildByAttr(assemblyNode,"home","id",homeInformations[i].id.in());
		if (tmpNode != NULL)
		{
			continue;	
		}
		//end by wsf
		
		DOM_Element homeElem=doc.createElement("home");	
		homeElem.setAttribute("id", homeInformations[(CORBA::ULong)i].id.in());
		homeElem.setAttribute("homeref", homeInformations[(CORBA::ULong)i].ref.in());		
		
		DOM_Element serveractivatorrefElem=doc.createElement("serveractivatorref");	
		serveractivatorrefElem.setAttribute("idref", homeInformations[(CORBA::ULong)i].serveractivatorid.in());
		homeElem.appendChild(serveractivatorrefElem);		
		
		DOM_Element componentserverrefElem=doc.createElement("componentserverref");
		componentserverrefElem.setAttribute("idref", homeInformations[(CORBA::ULong)i].componentserverid.in());
		homeElem.appendChild(componentserverrefElem);
		
		DOM_Element containerrefrefElem=doc.createElement("containerref");
		containerrefrefElem.setAttribute("idref", homeInformations[(CORBA::ULong)i].containerid.in());
		homeElem.appendChild(containerrefrefElem);		
		
		assemblyNode.appendChild(homeElem);
		
		//
		//create assemblyRef elememt
		DOM_Element assemblyrefElem=doc.createElement("assemblyref");	
		assemblyrefElem.setAttribute("idref", id);			
		
		
		//
		//add the assemblyRef for the serveractivator node
		//		
		DOM_Node serveractivatorNode=parser->findChildByAttr(root,"serveractivator","id",homeInformations[(CORBA::ULong)i].serveractivatorid);
		if(serveractivatorNode==NULL)
		{
			TRACE1(LEVEL1,"DeployDomainMgr: registerAssemblyInfo failed in locate serveractivator %s failed.\n",homeInformations[(CORBA::ULong)i].serveractivatorid);
			throw STARCCM::Deployment::ObjectNotExist();			
		}	
			
		serveractivatorNode.appendChild(assemblyrefElem);

		//
		//add the assemblyRef for the componentserver node
		//		
		DOM_Node componentserverNode=parser->findChildByAttr(root,"componentserver","id",homeInformations[(CORBA::ULong)i].componentserverid.in());
		if(componentserverNode==NULL)
		{
			TRACE1(LEVEL1,"DeployDomainMgr: registerAssemblyInfo failed in locate componentserver %s failed.\n",homeInformations[(CORBA::ULong)i].componentserverid.in());
			throw STARCCM::Deployment::ObjectNotExist();			
		}	
			
		componentserverNode.appendChild(assemblyrefElem);		

		//
		//add the assemblyRef for the container node
		//		
		DOM_Node containerNode=parser->findChildByAttr(root,"container","id",homeInformations[(CORBA::ULong)i].containerid);
		if(containerNode==NULL)
		{
			TRACE1(LEVEL1,"DeployDomainMgr: registerAssemblyInfo failed in locate container %s failed.\n",homeInformations[(CORBA::ULong)i].containerid);
			throw STARCCM::Deployment::ObjectNotExist();			
		}	
			
		containerNode.appendChild(assemblyrefElem);	
		
	}
	
	//
	//add connect node
	//	
	for(i=0;i<connectInformations.length();i++)
	{
		//add by wsf
		DOM_Node tmpNode = parser->findChildByAttr(assemblyNode,"connect","id",connectInformations[i].id.in());
		if (tmpNode != NULL)
		{
			continue;	
		}
		//end by wsf
		
		DOM_Element connectElem=doc.createElement("connect");	
		connectElem.setAttribute("id", connectInformations[(CORBA::ULong)i].id.in());
		connectElem.setAttribute("cookie", connectInformations[(CORBA::ULong)i].cookie.in());
		
		assemblyNode.appendChild(connectElem);		
	}

	if(parser->printToFile(DomainConfFile.in())==-1)
	{
			TRACE0(LEVEL1,"DeployDomainMgr: register AssemblyInfo, DOM print to file failed.\n");
			throw STARCCM::Deployment::XMLParserError();
	}	
	
}	       



         

//***************************************************************************************
//get the implementation id list of the assembly specified by id
//***************************************************************************************
STARCCM::Deployment::ObjIds* 
DeployDomainMgr_impl::getAssemblyImplIds(const char* id)
         throw(STARCCM::Deployment::ObjectNotExist,
	              STARCCM::Deployment::InfoMissed,
	              STARCCM::Deployment::XMLParserError,
	              CORBA::SystemException)
{
	ObjIds_var list=new ObjIds();
	CORBA::ULong len=list->length();
	DOM_Node assemblyNode=parser->findChildByAttr(root,"assembly","id",id);
	if(assemblyNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: getAssemblyImplIds failed in locating the assembly %s .\n",id);
		throw STARCCM::Deployment::ObjectNotExist();		
	}
	
	DOM_NodeList assemblyChildren=assemblyNode.getChildNodes();
        unsigned int assemblyChildrenLen=assemblyChildren.getLength();	
	
	DOM_Node assemblyChild;
	DOMString assemblyChildName;				
	
	for(unsigned int iterator=0;iterator<assemblyChildrenLen;iterator++)
	{
		assemblyChild=assemblyChildren.item(iterator);	
		assemblyChildName=assemblyChild.getNodeName();
		
		if(assemblyChildName.equals("implementationref"))
		{
			DOMString idref=parser->getAttr(assemblyChild,"idref");
			if(idref==NULL)
			{
				TRACE1(LEVEL1,"DeployDomainMgr: getAssemblyImplIds failed in invalid DomainConf file .\n",id);
				throw STARCCM::Deployment::InfoMissed();
			}
			
			list->length(len + 1);            	
            		list[(CORBA::ULong)len]= CORBA::string_dup(idref.transcode());
            		len++;
		}
	}
	
	return list._retn();
}        

//************************************************************************************
//get assembly information
//************************************************************************************

void 
DeployDomainMgr_impl::getAssemblyInfo(const char* id,
                                 	STARCCM::Deployment::ObjLoc_out loc,
                                 	STARCCM::Deployment::HomeInfos_out homeInformations,
                                 	STARCCM::Deployment::ConnectInfos_out connectInformations)
        	throw(STARCCM::Deployment::ObjectNotExist,
              		STARCCM::Deployment::InfoMissed,
              		STARCCM::Deployment::XMLParserError,
              		CORBA::SystemException)              		
{
	return;
}		



//************************************************************************************
//corresponding to the tear_down operation to remove some information
//including: the container, ComponentServer, ServerActivator information relate to the assembly
//************************************************************************************
void 
DeployDomainMgr_impl::tearDownAssembly(const char* id)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException)
{
	DOM_Node assemblyNode=parser->findChildByAttr(root,"assembly","id",id);
	if(assemblyNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: tearDownAssembly failed in locate the assembly %s.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}

	DOM_NodeList assemblyChildren=assemblyNode.getChildNodes();
	unsigned int assemblyChildrenLen=assemblyChildren.getLength();	
	
	DOM_Node assemblyChild;
	DOMString assemblyChildName;				
	
	for(unsigned int iterator=0;iterator<assemblyChildrenLen;iterator++)
	{
		assemblyChild=assemblyChildren.item(iterator);	
		assemblyChildName=assemblyChild.getNodeName();

		if(assemblyChildName.equals("home"))
		{

			DOM_NodeList homeChildren=assemblyChild.getChildNodes();
			unsigned int homeChildrenLen=homeChildren.getLength();	
			
			DOM_Node homeChild;
			DOMString homeChildName;				

			//
			//remove the assemblyref in the ServerActivator,ComponentServer,Container
			//
			
			for(unsigned int i=0;i<homeChildrenLen;i++)
			{
				homeChild=homeChildren.item(iterator);	
				homeChildName=homeChild.getNodeName();

				if(homeChildName.equals("serveractivatorref")||
				  homeChildName.equals("componentserverref")||
				  homeChildName.equals("containerref"))
				{
					DOMString idref=parser->getAttr(homeChild,"idref");
					if(idref==NULL)
					{
						TRACE0(LEVEL1,"DeployDomainMgr: tearDownAssembly failed in invalid DomainConf file.\n");
						throw STARCCM::Deployment::InfoMissed();
					}

					CORBA::String_var nodeName;
					if(homeChildName.equals("serveractivatorref"))
					{
						nodeName=CORBA::string_dup("serveractivator");
					}
					else if(homeChildName.equals("componentserverref"))
					{
						nodeName=CORBA::string_dup("componentserver");
					}
					else if(homeChildName.equals("containerref"))
					{
						nodeName=CORBA::string_dup("container");
					}
					
					DOM_Node node=parser->findChildByAttr(root,nodeName.in(),"id",idref.transcode());
					if(node==NULL)
					{
						TRACE0(LEVEL1,"DeployDomainMgr: tearDownAssembly failed in invalid DomainConf file.\n");
						throw STARCCM::Deployment::InfoMissed();
					}

					DOM_Node assemblyRefNode=parser->findChildByAttr(node,"assemblyref","idref",id);
					if(assemblyRefNode==NULL)
					{
						TRACE0(LEVEL1,"DeployDomainMgr: tearDownAssembly failed in invalid DomainConf file.\n");
						throw STARCCM::Deployment::InfoMissed();
					}

					node.removeChild(assemblyRefNode);
				
				}
				
			}


			//
			//delete the home node
			//			
			
			assemblyNode.removeChild(assemblyChild);
			
		}
		else if(assemblyChildName.equals("connect"))
		{

			//
			//delete the connect node
			//
			assemblyNode.removeChild(assemblyChild);		
		
		}
	}
	
	//
	//output the DOM tree to the DomainConf file
	//
	if(parser->printToFile(DomainConfFile.in())==-1)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: tearDownAssembly, DOM print to file failed.\n");
		throw STARCCM::Deployment::XMLParserError();
	}		

}

//************************************************************************************
//register implementation information
//************************************************************************************

void 
DeployDomainMgr_impl::registerImpl(const char* id,
                  const char* source,
                  const char* locallocation)
	        throw(STARCCM::Deployment::ObjectIDExist,
	              STARCCM::Deployment::InfoMissed,
	              STARCCM::Deployment::XMLParserError,
	              CORBA::SystemException)
{

	TRACE1(LEVEL5,"DeployDomainMgr: register Implementation of %s. \n",id);
	TRACE1(LEVEL5,"DeployDomainMgr: source %s. \n",source);
	TRACE1(LEVEL5,"DeployDomainMgr: locallocation %s. \n",locallocation);
	
	if(verbose)
	{
		cout<<"register the implementation of "<<id<<endl;
		
	}
	

	DOM_Document doc=root.getOwnerDocument();
	DOM_Element rootElem = doc.getDocumentElement();
	
	DOM_Element implementationElem=doc.createElement("implementation");	
	implementationElem.setAttribute("id", id);
	implementationElem.setAttribute("source", source);
	implementationElem.setAttribute("locallocation", locallocation);		
					
	rootElem.appendChild(implementationElem);	
	
	if(parser->printToFile(DomainConfFile.in())==-1)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: register implementation, DOM print to file failed.\n");
		throw STARCCM::Deployment::XMLParserError();
	}	
	
	return;
}	              
			

		              
//************************************************************************************
//get the information of the implementation specified by id
//************************************************************************************
void 
DeployDomainMgr_impl::getImplInfo(const char* id,
                CORBA::String_out source,
                CORBA::String_out locallocation,
                STARCCM::Deployment::ObjIds_out assemblyids)
        throw(STARCCM::Deployment::ObjectNotExist,
              STARCCM::Deployment::InfoMissed,
              STARCCM::Deployment::XMLParserError,
              CORBA::SystemException)
{
	
	DOM_Node implementationNode=parser->findChildByAttr(root,"implementation","id",id);
	if(implementationNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: getImplInfo failed to locate the implementation %s .\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}
	
	DOMString implsource=parser->getAttr(implementationNode,"source");
	if(implsource==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: getImplInfo failed to locate the source attr of implementation %s .\n",id);
		throw STARCCM::Deployment::InfoMissed();
	}	
	source=CORBA::string_dup(implsource.transcode());
	
	DOMString impllocallocation=parser->getAttr(implementationNode,"locallocation");
	if(impllocallocation==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: getImplInfo failed to locate the locallocation attr of implementation %s .\n",id);
		throw STARCCM::Deployment::InfoMissed();
	}	
	locallocation=CORBA::string_dup(impllocallocation.transcode());	
		
	assemblyids=new ObjIds;
	int len=assemblyids->length();
	
	DOM_NodeList implementationChildren=implementationNode.getChildNodes();
        unsigned int implementationChildrenLen=implementationChildren.getLength();	
	
	DOM_Node implementationChild;
	DOMString implementationChildName;				
	
	for(unsigned int iterator=0;iterator<implementationChildrenLen;iterator++)
	{
		implementationChild=implementationChildren.item(iterator);	
		implementationChildName=implementationChild.getNodeName();
		
		if(implementationChildName.equals("assemblyref"))
		{
			DOMString id=parser->getAttr(implementationChild,"idref");
			if(id==NULL)
			{
				TRACE0(LEVEL1,"DeployDomainMgr: getImplInfo failed to get the idref attr of asemblyref.\n");
				throw STARCCM::Deployment::InfoMissed();
			}
			assemblyids->length(len+1);
			assemblyids[(CORBA::ULong)len]=CORBA::string_dup(id.transcode());
			len++;
		}
		
	}

	return;
}              


        

void 
DeployDomainMgr_impl::getServerActivatorInfo(const char* id,
			STARCCM::Deployment::ObjLoc_out loc,
                        STARCCM::Deployment::ObjIds_out componentserverids,
                        STARCCM::Deployment::ObjIds_out assemblyids)
        	throw(STARCCM::Deployment::ObjectNotExist,
        		STARCCM::Deployment::InfoMissed,
        		STARCCM::Deployment::XMLParserError,
        		CORBA::SystemException)
{	
	componentserverids=new STARCCM::Deployment::ObjIds;
	assemblyids=new STARCCM::Deployment::ObjIds;

	DOM_Node serverActivatorNode=parser->findChildByAttr(root,"serveractivator","id",id);
	if(serverActivatorNode==NULL)
	{		
		TRACE1(LEVEL1,"DeployDomainMgr: locate ServerActivator %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}	
	
	DOMString corbaLocAttr = parser->getAttr(serverActivatorNode,"corbaloc");
	loc = CORBA::string_dup(corbaLocAttr.transcode());
		
	DOM_NodeList serverActivatorChildren=serverActivatorNode.getChildNodes();
	unsigned int serverActivatorChildrenLen=serverActivatorChildren.getLength();	
	
	DOM_Node serverActivatorChild;
	DOMString serverActivatorChildName;				
	
	for(unsigned int iterator=0;iterator<serverActivatorChildrenLen;iterator++)
	{
		serverActivatorChild=serverActivatorChildren.item(iterator);	
		serverActivatorChildName=serverActivatorChild.getNodeName();
			
		if(serverActivatorChildName.equals("assemblyref"))
		{
			DOMString assemblyId=parser->getAttr(serverActivatorChild,"idref");
			int len=assemblyids->length();
			assemblyids->length(len+1);
			assemblyids[(CORBA::ULong)len]=CORBA::string_dup(assemblyId.transcode());
		}
		if(serverActivatorChildName.equals("componentserverref"))
		{
			DOMString componentServerId=parser->getAttr(serverActivatorChild,"idref");
			int len=componentserverids->length();
			componentserverids->length(len+1);
			componentserverids[(CORBA::ULong)len]=CORBA::string_dup(componentServerId.transcode());
		}				
	}	
	
	return;
}	        	


//*****************************************************************************
//register the ComponentServer information:
//id: the id of this ComponentServer
//fatherid : the id of ServerActivator who create the ComponentServer
//loc: the corbaloc of this ComponentServer
//conf: the conf of this ComponentServer while set at create_component_server
//*****************************************************************************

void 
DeployDomainMgr_impl::registerComponentServer(const char* id,
	                                         const char* fatherid,	                                         
	                                         const char* loc,
	                                         const Components::ConfigValues& config)
			        throw(STARCCM::Deployment::ObjectIDExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException)
{

	DOM_Document doc=root.getOwnerDocument();
	DOM_Element rootElem = doc.getDocumentElement();
	
	//
	//create componentserver element
	//			
	DOM_Element componentserverElem=doc.createElement("componentserver");
	componentserverElem.setAttribute("id", id);
	componentserverElem.setAttribute("corbaloc", loc);				
	rootElem.appendChild(componentserverElem);
	
	//
	//create serveractivatorref element
	//							
	DOM_Element ServerActivatorRefELem=doc.createElement("serveractivatorref");
	ServerActivatorRefELem.setAttribute("idref", fatherid);
	componentserverElem.appendChild(ServerActivatorRefELem);
				
	//
	//create comoponentserverref element
	//			
	DOM_Node ServerActivatorElem=parser->findChildByAttr(root,"serveractivator","id",fatherid);
	if(ServerActivatorElem==NULL)
	{
		TRACE2(LEVEL1,"DeployDomainMgr: register componentserver %s failed in finding its ServerActivator %s.\n",id,fatherid);
		throw STARCCM::Deployment::ObjectIDExist();
	}
				
	DOM_Element ComponentServerRefElem=doc.createElement("componentserverref");
	ComponentServerRefElem.setAttribute("idref", id);
	ServerActivatorElem.appendChild(ComponentServerRefElem);

	//
	//create configvalues element
	//
	DOM_Element configvaluesElem=doc.createElement("configvalues");				
	componentserverElem.appendChild(configvaluesElem);
	
	for(int i=0;i<config.length();i++)
	{	
		const char * value;
		(config[(CORBA::ULong)i]->value())>>=value;
		CORBA::String_var valueStr=CORBA::string_dup(value);
		CORBA::String_var nameStr=config[(CORBA::ULong)i]->name();
		
		DOM_Element configvalueElem=doc.createElement("configvalue");
		configvalueElem.setAttribute("name", nameStr.in());
		configvalueElem.setAttribute("value", valueStr.in());				
		configvaluesElem.appendChild(configvalueElem);	
	}

	//
	//print the DOM tree to the DomainConf file
	//								
	if(parser->printToFile(DomainConfFile.in())==-1)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: register ComponentServer, DOM print to file failed.\n");
		throw STARCCM::Deployment::XMLParserError();
	}

}


//*****************************************************************************
//get assembly UUIDs that deployed on the component server specified by id
//*****************************************************************************

STARCCM::Deployment::ObjIds* 
DeployDomainMgr_impl::getComponentServerAssemlbyIds(const char* id)
		throw(STARCCM::Deployment::ObjectNotExist,
			STARCCM::Deployment::InfoMissed,
		        STARCCM::Deployment::XMLParserError,
		        CORBA::SystemException)
{
	STARCCM::Deployment::ObjIds_var assemblyIds=new STARCCM::Deployment::ObjIds;
	
	DOM_Node componentserverNode=parser->findChildByAttr(root,"componentserver","id",id);
	if(componentserverNode==NULL)
	{		
		
		TRACE1(LEVEL1,"DeployDomainMgr: locate componentserver %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}	
		
	DOM_NodeList componentserverChildren=componentserverNode.getChildNodes();
        unsigned int componentserverChildrenLen=componentserverChildren.getLength();	
	
	DOM_Node componentserverChild;
	DOMString componentserverChildName;				
	
	for(unsigned int iterator=0;iterator<componentserverChildrenLen;iterator++)
	{
		componentserverChild=componentserverChildren.item(iterator);	
		componentserverChildName=componentserverChild.getNodeName();
			
		if(componentserverChildName.equals("assemblyref"))
		{
			DOMString assemblyId=parser->getAttr(componentserverChild,"idref");
			int len=assemblyIds->length();
			assemblyIds->length(len+1);
			assemblyIds[(CORBA::ULong)len]=CORBA::string_dup(assemblyId.transcode());
		}
	}
	
	return 	assemblyIds._retn();	
	
}		        


void 
DeployDomainMgr_impl::getComponentServerInfo(const char* id,
                        STARCCM::Deployment::ObjLoc_out loc,
                        STARCCM::Deployment::ObjId_out serveractivatorid,
                        STARCCM::Deployment::ObjIds_out containerids,
                        STARCCM::Deployment::ObjIds_out assemblyids)
        	throw(STARCCM::Deployment::ObjectNotExist,
        		STARCCM::Deployment::InfoMissed,
        		STARCCM::Deployment::XMLParserError,
        		CORBA::SystemException)
{	
	containerids=new STARCCM::Deployment::ObjIds;
	assemblyids=new STARCCM::Deployment::ObjIds;

	DOM_Node componentserverNode=parser->findChildByAttr(root,"componentserver","id",id);
	if(componentserverNode==NULL)
	{		
		TRACE1(LEVEL1,"DeployDomainMgr: locate componentserver %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}	
	
	DOMString corbaLocAttr = parser->getAttr(componentserverNode,"corbaloc");
	loc = CORBA::string_dup(corbaLocAttr.transcode());
		
	DOM_NodeList componentserverChildren=componentserverNode.getChildNodes();
	unsigned int componentserverChildrenLen=componentserverChildren.getLength();	
	
	DOM_Node componentserverChild;
	DOMString componentserverChildName;				
	
	for(unsigned int iterator=0;iterator<componentserverChildrenLen;iterator++)
	{
		componentserverChild=componentserverChildren.item(iterator);	
		componentserverChildName=componentserverChild.getNodeName();
			
		if(componentserverChildName.equals("assemblyref"))
		{
			DOMString assemblyId=parser->getAttr(componentserverChild,"idref");
			int len=assemblyids->length();
			assemblyids->length(len+1);
			assemblyids[(CORBA::ULong)len]=CORBA::string_dup(assemblyId.transcode());
		}
		if(componentserverChildName.equals("serveractivatorref"))
		{
			DOMString id=parser->getAttr(componentserverChild,"idref");		
			serveractivatorid=CORBA::string_dup(id.transcode());
		}
		if(componentserverChildName.equals("containerref"))
		{
			DOMString containerId=parser->getAttr(componentserverChild,"idref");
			int len=containerids->length();
			containerids->length(len+1);
			containerids[(CORBA::ULong)len]=CORBA::string_dup(containerId.transcode());
		}				
	}	
	
	return;
}	        	



//*****************************************************************************
//get the configvalues of the ComponentServer while set at create_component_server.
//*****************************************************************************
Components::ConfigValues* 
DeployDomainMgr_impl::getComponentServerConf(const char* id)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException)
{
	//
	//locate the ComponentServer
	//
	DOM_Node componentserverNode=parser->findChildByAttr(root,"componentserver","id",id);
	if(componentserverNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: getComponentServerConf failed in locate componentserver %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();		
	}
	
	DOM_Node configValuesNode=parser->findChild(componentserverNode,"configvalues");
	if(configValuesNode==NULL)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: getComponentServerConf failed in locate configvalues node.\n");
		throw STARCCM::Deployment::InfoMissed();				
	}
	
	
	//
	//construct the configvalues
	//
	DOM_NodeList configValuesChildren=configValuesNode.getChildNodes();
        unsigned int configValuesChildrenLen=configValuesChildren.getLength();	
	
	DOM_Node configValueNode;
	DOMString configValueNodeName;				
	
	Components::ConfigValues_var cfValues=new  Components::ConfigValues;
	int len=cfValues->length();
	
	for(unsigned int iterator=0;iterator<configValuesChildrenLen;iterator++)
	{
		configValueNode=configValuesChildren.item(iterator);	
		configValueNodeName=configValueNode.getNodeName();
		
		if(configValueNodeName.equals("configvalue"))
		{
			DOMString name=parser->getAttr(configValueNode,"name");
			if(name==NULL)
			{
				TRACE0(LEVEL1,"DeployDomainMgr: getComponentServerConf failed in confivalue node miss name attr.\n");
				throw STARCCM::Deployment::InfoMissed();				
			}

			DOMString value=parser->getAttr(configValueNode,"value");
			if(name==NULL)
			{
				TRACE0(LEVEL1,"DeployDomainMgr: getComponentServerConf failed in confivalue node miss value attr.\n");
				throw STARCCM::Deployment::InfoMissed();				
			}	
			
			CORBA::String_var valueStr=CORBA::string_dup(value.transcode());
			
			CORBA::Any_var anyValue;
			anyValue<<=valueStr;
			Components::ConfigValue_var cfValue=new ConfigValue_impl(name.transcode(),anyValue.in());
			
			cfValues->length(len+1);					
			cfValues[(CORBA::ULong)len]=cfValue;
			len++;
		}	
	}
	
	return cfValues._retn();	
}

//*****************************************************************************
//register the Container information:
//id: the id of this container
//fatherid : the id of ComponentServer who create the container
//loc: the corbaloc of this container
//conf: the conf of this Container while set at create_container
//*****************************************************************************
void 
DeployDomainMgr_impl::registerContainer(const char* id,
	                                   const char* fatherid,	                                   
	                                   const char* loc,
	                                   const Components::ConfigValues& config)
			        throw(STARCCM::Deployment::ObjectIDExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException)
{
		
	DOM_Document doc=root.getOwnerDocument();
	DOM_Element rootElem = doc.getDocumentElement();
	
	
	//
	//create container element
	//				
	DOM_Element ContainerElem=doc.createElement("container");
	ContainerElem.setAttribute("id", id);
	ContainerElem.setAttribute("corbaloc", loc);				
	rootElem.appendChild(ContainerElem);
	
	
	//
	//create componentserverref element
	//						
	DOM_Element ComponentServerRefELem=doc.createElement("componentserverref");
	ComponentServerRefELem.setAttribute("idref", fatherid);
	ContainerElem.appendChild(ComponentServerRefELem);
				
	
	//
	//create containerref element for ComponentServer
	//			
				
	DOM_Node ComponentServerElem=parser->findChildByAttr(root,"componentserver","id",fatherid);
	if(ComponentServerElem==NULL)
	{
		TRACE2(LEVEL1,"DeployDomainMgr: register container %s failed in finding its ComponentServer %s.\n",id,fatherid);
		throw STARCCM::Deployment::ObjectIDExist();
	}
				
	DOM_Element ContainerRefElem=doc.createElement("containerref");
	ContainerRefElem.setAttribute("idref", id);
	ComponentServerElem.appendChild(ContainerRefElem);
	
	//
	//create configvalues element
	//
	DOM_Element configvaluesElem=doc.createElement("configvalues");				
	ContainerElem.appendChild(configvaluesElem);
	
	for(int i=0;i<config.length();i++)
	{	
		const char * value;
		(config[(CORBA::ULong)i]->value())>>=value;
		CORBA::String_var valueStr=CORBA::string_dup(value);	
		CORBA::String_var nameStr=config[(CORBA::ULong)i]->name();
		
		DOM_Element configvalueElem=doc.createElement("configvalue");
		configvalueElem.setAttribute("name", nameStr.in());
		configvalueElem.setAttribute("value", valueStr.in());				
		configvaluesElem.appendChild(configvalueElem);	
	}
	
	//
	//print the DOM tree to DomainConf file
	//							
	if(parser->printToFile(DomainConfFile.in())==-1)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: register Container, DOM print to file failed.\n");
		throw STARCCM::Deployment::XMLParserError();
	}	

}


//*****************************************************************************
//register the information of home installed in the container :
//containerId: the UUID of the Container who install the home
//id: the UUID of home
//configValues: the configValues while set at install_home 
//*****************************************************************************
void 
DeployDomainMgr_impl::registerInstallHome(const char* containerId,
					const char* id,
					const Components::ConfigValues& config)
		throw(STARCCM::Deployment::ObjectIDExist,
		       STARCCM::Deployment::InfoMissed,
		       STARCCM::Deployment::XMLParserError,
		      CORBA::SystemException)
{
	
	//
	//locate the container node
	//
	DOM_Node containerNode=parser->findChildByAttr(root,"container","id",containerId);
	if(containerNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: registerInstallHome failed in locate conainer %s failed.\n",containerId);
		throw STARCCM::Deployment::ObjectIDExist();
	}
	
	//
	//create the installhome node
	//
	DOM_Document doc=root.getOwnerDocument();
	DOM_Element rootElem = doc.getDocumentElement();	
			
	DOM_Element installhomeElem=doc.createElement("installhome");
	installhomeElem.setAttribute("id", id);			
	containerNode.appendChild(installhomeElem);
	
		
	//
	//create the configvalues node
	//
	DOM_Element configvaluesElem=doc.createElement("configvalues");				
	installhomeElem.appendChild(configvaluesElem);
	
	int i=0;
	while(i<config.length())
	{
		CORBA::String_var cfValueName=config[(CORBA::ULong)i]->name(); 
		
		if(strcmp(cfValueName.in(),"ValueFactoryDllNames")==0)
		{
			const CORBA::StringSeq * ValueFactoryDllNames;
			const CORBA::StringSeq * ValueFactoryEntryPoints;
			const CORBA::StringSeq * ValueEntryPoints;
			const CORBA::StringSeq * ValueFactoryRepIds;
			
			(config[(CORBA::ULong)i]->value())>>=ValueFactoryDllNames;
			(config[(CORBA::ULong)(i+1)]->value())>>=ValueFactoryEntryPoints;
			(config[(CORBA::ULong)(i+2)]->value())>>=ValueEntryPoints;
			(config[(CORBA::ULong)(i+3)]->value())>>=ValueFactoryRepIds;
			
			for(int j=0;j<ValueFactoryDllNames->length();j++)
			{						
				DOM_Element valuefactoryElem=doc.createElement("valuefactory");		
							
				valuefactoryElem.setAttribute("name", (*ValueFactoryDllNames)[(CORBA::ULong)j]);							
				valuefactoryElem.setAttribute("factoryentrypoint", (*ValueFactoryEntryPoints)[(CORBA::ULong)j]);			
				valuefactoryElem.setAttribute("valueentrypoint", (*ValueEntryPoints)[(CORBA::ULong)j]);				
				valuefactoryElem.setAttribute("repid", (*ValueFactoryRepIds)[(CORBA::ULong)j]);		
							
				configvaluesElem.appendChild(valuefactoryElem);	
			}	
			
			i=i+4;		
			
		}
		else if((strcmp(cfValueName.in(),"HomeMethodTxnNames")==0)||
			(strcmp(cfValueName.in(),"ComponentMethodTxnNames")==0))
		{
			const CORBA::StringSeq * MethodTxnNames;
			const CORBA::StringSeq * MethodTxnPolicies;
			
			(config[(CORBA::ULong)i]->value())>>=MethodTxnNames;
			(config[(CORBA::ULong)(i+1)]->value())>>=MethodTxnPolicies;

			for(int j=0;j<MethodTxnNames->length();j++)
			{
				DOM_Element transactionpolicyElem=doc.createElement("transactionpolicy");
								
				transactionpolicyElem.setAttribute("methodname", (*MethodTxnNames)[(CORBA::ULong)j]);	
				transactionpolicyElem.setAttribute("policy", (*MethodTxnPolicies)[(CORBA::ULong)j]);
									
				configvaluesElem.appendChild(transactionpolicyElem);	
			}
			
			i=i+2;
		}	
		else if(strcmp(cfValueName.in(),"ComponentEventPortNames")==0)
		{
			const CORBA::StringSeq * ComponentEventPortNames;
			const CORBA::StringSeq * ComponentEventPortPolicies;
			
			(config[(CORBA::ULong)i]->value())>>=ComponentEventPortNames;
			(config[(CORBA::ULong)(i+1)]->value())>>=ComponentEventPortPolicies;

			for(int j=0;j<ComponentEventPortNames->length();j++)
			{
				DOM_Element eventpolicyElem=doc.createElement("eventpolicy");
				eventpolicyElem.setAttribute("methodname", (*ComponentEventPortNames)[(CORBA::ULong)j]);	
				eventpolicyElem.setAttribute("policy", (*ComponentEventPortPolicies)[(CORBA::ULong)j]);
									
				configvaluesElem.appendChild(eventpolicyElem);	
			}
			
			i=i+2;			
		}	
		else if((strcmp(cfValueName.in(),"HomeRegisterWithHomeFinders")==0)||
			(strcmp(cfValueName.in(),"HomeRegisterWithNamings")==0)||
			(strcmp(cfValueName.in(),"RegisterComponents")==0))
		{
			const CORBA::StringSeq * valueSeq;;
			(config[(CORBA::ULong)i]->value())>>=valueSeq;						
			
			for(int j=0;j<valueSeq->length();j++)
			{				
				DOM_Element configvalueElem=doc.createElement("configvalue");
				configvalueElem.setAttribute("name", cfValueName.in());
				configvalueElem.setAttribute("value", (*valueSeq)[(CORBA::ULong)j]);				
				configvaluesElem.appendChild(configvalueElem);		
			}
			
			i++;
		}				
		else
		{
			const char * value;
			(config[(CORBA::ULong)i]->value())>>=value;
			CORBA::String_var valueStr=CORBA::string_dup(value);				
			
			DOM_Element configvalueElem=doc.createElement("configvalue");
			configvalueElem.setAttribute("name", cfValueName.in());
			configvalueElem.setAttribute("value", valueStr.in());				
			configvaluesElem.appendChild(configvalueElem);		
			
			i++;
		}
		
	}	
	
	
	//
	//print the DOM tree to the file
	//
	if(parser->printToFile(DomainConfFile.in())==-1)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: registerInstallHome, DOM print to file failed.\n");
		throw STARCCM::Deployment::XMLParserError();
	}
			
}		      

//*****************************************************************************
//remove the information of home specified by id in the container,
//corresponding to the operation of remove_home:
//containerId: the UUID of the container who install the home
//id: the UUID of home
//*****************************************************************************
void 
DeployDomainMgr_impl::removeInstallHome(const char* containerId, const char* id)
		throw(STARCCM::Deployment::ObjectNotExist,
              		STARCCM::Deployment::InfoMissed,
              		STARCCM::Deployment::XMLParserError,
              		CORBA::SystemException)
{
	//
	//locate the Container node
	//
	DOM_Node containerNode=parser->findChildByAttr(root,"container","id",containerId);
	if(containerNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: removeInstallHome failed in locate conainer %s failed.\n",containerId);
		throw STARCCM::Deployment::ObjectIDExist();
	}	
	
	//
	//locate the installhome node
	//
	DOM_Node installhomeNode=parser->findChildByAttr(containerNode,"installhome","id",id);
	if(installhomeNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: removeInstallHome failed in locate installhome %s failed.\n",id);
		throw STARCCM::Deployment::ObjectIDExist();
	}
	
	//
	//remove the installhome
	//
	containerNode.removeChild(installhomeNode);
		
	//
	//print the DOM tree to the file
	//
	if(parser->printToFile(DomainConfFile.in())==-1)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: removeInstallHome , DOM print to file failed.\n");
		throw STARCCM::Deployment::XMLParserError();
	}	
	
}   

           		
//*****************************************************************************
//get assembly UUIDs that deployed on the component server specified by id
//*****************************************************************************

STARCCM::Deployment::ObjIds* 
DeployDomainMgr_impl::getContainerAssemlbyIds(const char* id)
        throw(STARCCM::Deployment::ObjectNotExist,
	      STARCCM::Deployment::InfoMissed,
	      STARCCM::Deployment::XMLParserError,
	     CORBA::SystemException)
{
	
	STARCCM::Deployment::ObjIds_var assemblyIds=new STARCCM::Deployment::ObjIds;
	
	DOM_Node containerNode=parser->findChildByAttr(root,"container","id",id);
	if(containerNode==NULL)
	{		
		TRACE1(LEVEL1,"DeployDomainMgr: locate container %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}	
		
	DOM_NodeList containerChildren=containerNode.getChildNodes();
        unsigned int containerChildrenLen=containerChildren.getLength();	
	
	DOM_Node containerChild;
	DOMString containerChildName;				
	
	for(unsigned int iterator=0;iterator<containerChildrenLen;iterator++)
	{
		containerChild=containerChildren.item(iterator);	
		containerChildName=containerChild.getNodeName();
			
		if(containerChildName.equals("assemblyref"))
		{
			DOMString assemblyId=parser->getAttr(containerChild,"idref");
			int len=assemblyIds->length();
			assemblyIds->length(len+1);
			assemblyIds[(CORBA::ULong)len]=CORBA::string_dup(assemblyId.transcode());
		}
	}
	
	return 	assemblyIds._retn();
}	     



void 
DeployDomainMgr_impl::getContainerInfo(const char* id,
                        STARCCM::Deployment::ObjLoc_out loc,
                        STARCCM::Deployment::ObjId_out componentserverid,
                        STARCCM::Deployment::ObjIds_out assemblyids)
        	throw(STARCCM::Deployment::ObjectNotExist,
        		STARCCM::Deployment::InfoMissed,
        		STARCCM::Deployment::XMLParserError,
        		CORBA::SystemException)
{		
	assemblyids=new STARCCM::Deployment::ObjIds;

	DOM_Node containerNode=parser->findChildByAttr(root,"container","id",id);
	if(containerNode==NULL)
	{		
		TRACE1(LEVEL1,"DeployDomainMgr: locate container %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}	
	
	DOMString corbaLocAttr = parser->getAttr(containerNode,"corbaloc");
	loc = CORBA::string_dup(corbaLocAttr.transcode());
		
	DOM_NodeList containerChildren=containerNode.getChildNodes();
	unsigned int containerChildrenLen=containerChildren.getLength();	
	
	DOM_Node containerChild;
	DOMString containerChildName;				
	
	for(unsigned int iterator=0;iterator<containerChildrenLen;iterator++)
	{
		containerChild=containerChildren.item(iterator);	
		containerChildName=containerChild.getNodeName();
			
		if(containerChildName.equals("assemblyref"))
		{
			DOMString assemblyId=parser->getAttr(containerChild,"idref");
			int len=assemblyids->length();
			assemblyids->length(len+1);
			assemblyids[(CORBA::ULong)len]=CORBA::string_dup(assemblyId.transcode());
		}
		if(containerChildName.equals("componentserverref"))
		{
			DOMString id=parser->getAttr(containerChild,"idref");		
			componentserverid=CORBA::string_dup(id.transcode());
		}	
	}	
	
	return;
}	        	


//*****************************************************************************
//get the configvalues of the container while set at create_container.
//*****************************************************************************
Components::ConfigValues* 
DeployDomainMgr_impl::getContainerConf(const char* id)
			        throw(STARCCM::Deployment::ObjectNotExist,
			              STARCCM::Deployment::InfoMissed,
			              STARCCM::Deployment::XMLParserError,
			              CORBA::SystemException)
{
	//
	//locate the container
	//
	DOM_Node containerNode=parser->findChildByAttr(root,"container","id",id);
	if(containerNode==NULL)
	{
		TRACE1(LEVEL1,"DeployDomainMgr: getContainerConf failed in locate container %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();		
	}
	
	DOM_Node configValuesNode=parser->findChild(containerNode,"configvalues");
	if(configValuesNode==NULL)
	{
		TRACE0(LEVEL1,"DeployDomainMgr: getContainerConf failed in locate configvalues node.\n");
		throw STARCCM::Deployment::InfoMissed();				
	}
	
	
	//
	//construct the configvalues
	//
	DOM_NodeList configValuesChildren=configValuesNode.getChildNodes();
        unsigned int configValuesChildrenLen=configValuesChildren.getLength();	
	
	DOM_Node configValueNode;
	DOMString configValueNodeName;				
	
	Components::ConfigValues_var cfValues=new  Components::ConfigValues;
	int len=cfValues->length();
	
	for(unsigned int iterator=0;iterator<configValuesChildrenLen;iterator++)
	{
		configValueNode=configValuesChildren.item(iterator);	
		configValueNodeName=configValueNode.getNodeName();
		
		if(configValueNodeName.equals("configvalue"))
		{
			DOMString name=parser->getAttr(configValueNode,"name");
			if(name==NULL)
			{
				TRACE0(LEVEL1,"DeployDomainMgr: getContainerConf failed in confivalue node miss name attr.\n");
				throw STARCCM::Deployment::InfoMissed();				
			}

			DOMString value=parser->getAttr(configValueNode,"value");
			if(name==NULL)
			{
				TRACE0(LEVEL1,"DeployDomainMgr: getContainerConf failed in confivalue node miss value attr.\n");
				throw STARCCM::Deployment::InfoMissed();				
			}	
			
			CORBA::String_var valueStr=CORBA::string_dup(value.transcode());
			
			CORBA::Any_var anyValue;
			anyValue<<=valueStr;
			ConfigValue_var cfValue=new ConfigValue_impl(name.transcode(),anyValue.in());
			
			cfValues->length(len+1);					
			cfValues[(CORBA::ULong)len]=cfValue;
			len++;
		}	
	}
	
	return cfValues._retn();
	
}
//***********************************************************************************************
//get applications list of object specified by the type
//Added by jxh 2004/03//23
//***********************************************************************************************
STARCCM::Deployment::AppInforSeq* DeployDomainMgr_impl::getApplications(STARCCM::Deployment::ObjectType type)
					  throw(STARCCM::Deployment::UnknownObjectType,
					           STARCCM::Deployment::InfoMissed,
					           CORBA::SystemException)	
{
	    
	//DOM_Node node=parser->findChild(root,"Applications");	
	CORBA::String_var nodeName;
	STARCCM::Deployment::AppInforSeq_var appinfor = new STARCCM::Deployment::AppInforSeq(0);
	switch (type)
		{
		case APPLICATIONS:
			{
					nodeName=CORBA::string_dup("Applications");					
					
				break;
			}
		default:
			{
				TRACE0(LEVEL1,"DeployDomainMgr: getApplication failed in invalid object type.\n");
				throw STARCCM::Deployment::UnknownObjectType();
				break;
			}
		}
	

	//STARCCM::Deployment::ObjIds_var ids=new STARCCM::Deployment::ObjIds;		  
	
	DOM_NodeList rootChildren=root.getChildNodes();
	unsigned int rootChildrenLen=rootChildren.getLength();	
	
	DOM_Node rootChild;	
	DOMString rootChildName;	
	//cout<<rootChildName.transcode()<<endl;
	
	for(unsigned int iterator=0;iterator<rootChildrenLen;iterator++)
	{
		rootChild=rootChildren.item(iterator);
	
		rootChildName=rootChild.getNodeName();
		//TRACE0(LEVEL1,"hehe1-------------\n");
		if(rootChildName.equals(nodeName.in()))
		{
			//unsigned int nodelen = rootChild.getLength();
			//TRACE0(LEVEL1,"hehe2-------------\n");
			DOM_NodeList appChildren=rootChild.getChildNodes();
		    unsigned int appChildrenLen=appChildren.getLength();
		   
		    appinfor->length(appChildrenLen);
			for(unsigned int j=0;j<appChildrenLen;j++)
			{
				//DOM_Node temnde=parser->findChild(rootChild,"Application");
				
				DOM_Node temChild=appChildren.item(j);
				TRACE0(LEVEL1,temChild.getNodeName().transcode());
				DOMString id = parser->getAttr(temChild,"id");	
				DOMString objectId=parser->getAttr(temChild,"name");
				TRACE0(LEVEL1,objectId.transcode());	
				TRACE0(LEVEL1,id.transcode());	
				appinfor[j].AssemblyId = CORBA::string_dup(id.transcode());
				appinfor[j].AppName = CORBA::string_dup(objectId.transcode());
				//cout<<objectId.transcode()<<endl;
				TRACE0(LEVEL1,"get appinfor pass-------------\n");	
							
				if(objectId==NULL)
				{
					TRACE0(LEVEL1,"DeployDomainMgr: getOApplications failed in invalid DomainConf file: node miss id attribute.\n");
					throw STARCCM::Deployment::InfoMissed();
				}
				
					
				//	int len=ids->length();
				//	ids->length(len+1);
				//ids[(CORBA::ULong)len]=CORBA::string_dup(objectId.transcode());
			}
			return appinfor._retn();		
		}	
	}
	TRACE0(LEVEL1,"test-------------\n");
	appinfor->length(0);
	return appinfor._retn();			
			
	
}		

//wsf
void 
DeployDomainMgr_impl::getHomeInfo(const char* id, STARCCM::Deployment::HomeInfo_out homeInformation)
			throw(STARCCM::Deployment::ObjectNotExist,
		              STARCCM::Deployment::InfoMissed,
		              STARCCM::Deployment::XMLParserError,
		              CORBA::SystemException)
{

	DOM_Node homeNode=parser->findChildByAttr(root,"home","id",id);
	if(homeNode==NULL)
	{		
		TRACE1(LEVEL1,"DeployDomainMgr: locate home %s failed.\n",id);
		throw STARCCM::Deployment::ObjectNotExist();
	}	
	
	DOMString homeref = parser->getAttr(homeNode,"homeref");
	homeInformation->ref = CORBA::string_dup(homeref.transcode());
		
	DOM_NodeList homeChildren=homeNode.getChildNodes();
	unsigned int homeChildrenLen=homeChildren.getLength();	
	
	DOM_Node homeChild;
	DOMString homeChildName;				
	
	for(unsigned int iterator=0;iterator<homeChildrenLen;iterator++)
	{
		homeChild=homeChildren.item(iterator);	
		homeChildName=homeChild.getNodeName();
			
		if(homeChildName.equals("serveractivatorref"))
		{
			DOMString serveractivatorref=parser->getAttr(homeChild,"idref");
			homeInformation->serveractivatorid = CORBA::string_dup(serveractivatorref.transcode());
		}

		if(homeChildName.equals("componentserverid"))
		{
			DOMString componentserverref=parser->getAttr(homeChild,"idref");
			homeInformation->componentserverid = CORBA::string_dup(componentserverref.transcode());
		}

		if(homeChildName.equals("containerid"))
		{
			DOMString containerref=parser->getAttr(homeChild,"idref");
			homeInformation->containerid = CORBA::string_dup(containerref.transcode());
		}		
	}
}


