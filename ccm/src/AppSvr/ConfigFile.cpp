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

#include <ConfigFile.h>
#include <Trace.h>
#include <stdlib.h>

#ifndef WIN32
#include <unistd.h>
#endif

using namespace Container;
using namespace std;

ConfigFile::ConfigFile()
{
    char* path = getenv("CCM_HOME");
    
    if( path == NULL )
    {
//        TRACE0(LEVEL6,"Environment CCM_HOME must be set.\n");
        path = ".";
    }
    
    file_ = CORBA::string_dup(path);

#ifdef STARCCMSTRINGADD
    file_ = HelpFun::CORBA_string_add(file_, PATH_DELILIMITOR);
    file_ = HelpFun::CORBA_string_add(file_, "ApplicationServer.xml");
#else
    file_ += CORBA::string_dup(PATH_DELILIMITOR);
    
    file_ += CORBA::string_dup("ApplicationServer.xml");
#endif
    
    parser_ = new XMLParser();
    
    try
  	{
  		parser_->initialize(file_.in());
  	}
  	catch (const XMLInitializeFailed &)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in XMLParser initializd. \n");		
		
	}
}

ConfigFile::~ConfigFile()
{
    if( parser_ != NULL )
    {
        delete parser_;
    }
}

char*
ConfigFile::getRepositoryLocation()
{
    char* path = getenv("CCM_HOME");
    
    if( path == NULL )
    {
//        TRACE0(LEVEL6,"Environment CCM_HOME must be set.\n");
        path = ".";
    }
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode = parser_->findChild(root,"componentrepository");
	
	if(partitionNode == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOMString repository = parser_->getAttr(partitionNode,"location");
	
	if(repository == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository location attribute. \n");
		
		return NULL;
	}
	
	CORBA::String_var ret = CORBA::string_dup(path);
#ifdef STARCCMSTRINGADD
	ret = HelpFun::CORBA_string_add(ret.in(), PATH_DELILIMITOR);
	
	ret = HelpFun::CORBA_string_add(ret.in(), repository.transcode());
#else
	ret += CORBA::string_dup(PATH_DELILIMITOR);
	
	ret += CORBA::string_dup(repository.transcode());
#endif
	
	return ret._retn();
}

short
ConfigFile::getPort(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode = parser_->findChildByAttr(root, "componentserver", "uuid" ,uuid);
	
	if(partitionNode == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return NULL;
	}
	
	DOMString port = parser_->getAttr(partitionNode,"port");
	
	if(port == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver port attribute. \n");
		
		return NULL;
	}
	
	CORBA::String_var ret = CORBA::string_dup(port.transcode());
	
	return (short)atoi(ret.in());
}

CORBA::StringSeq*
ConfigFile::getContainerIds(const char* uuid)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq();
    
     DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return ret._retn();
	}
    
    DOM_NodeList children = partitionNode1.getChildNodes();
    unsigned int childrenLen = children.getLength();
    
    DOM_Node childNode;	
	DOMString id;	
	DOM_Node nodeNULL;
														
	for(int i=0;i<childrenLen;i++)					
	{
		childNode = children.item(i);
		
		if( stricmp(childNode.getNodeName().transcode(), "container") != 0 )
        {
            continue;
        }
        
		id = parser_->getAttr(childNode, "uuid");
		
		if( id == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in container element uuid attribute. \n");
		    
		    return ret._retn();
		}
		
		ret->length(ret->length() + 1);
		ret[ret->length() - 1] = CORBA::string_dup(id.transcode());
	}
    
    return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getComponentIds(const char* uuid)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq();

     DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return ret._retn();
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "container", "uuid", uuid);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in container element. \n");
		
		return ret._retn();
	}
	
    DOM_NodeList children = partitionNode1.getChildNodes();
    unsigned int childrenLen = children.getLength();
    
    DOM_Node childNode;
	DOMString id;
	DOM_Node nodeNULL;
	
	for(int i=0;i<childrenLen;i++)					
	{
		childNode = children.item(i);
		
		if( stricmp(childNode.getNodeName().transcode(), "component") != 0 )
        {
            continue;
        }
        
		id = parser_->getAttr(childNode, "uuid");
		
		if( id == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in componentid attribute. \n");
		
		    return ret._retn();
		}
		
		ret->length(ret->length() + 1);
		ret[ret->length() - 1] = CORBA::string_dup(id.transcode());
	}
    
    return ret._retn();
}

// What does this function do?
// Left it blank here.
//CORBA::StringSeq*
//ConfigFile::getComponentUuid(const char* uuid)
//{
//    CORBA::StringSeq_var ret = new CORBA::StringSeq();
//    
//    return ret._retn();
//}

char*
ConfigFile::getServantFile(const char* uuid)
{
    CORBA::String_var ret;
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"servantdll");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in servantdll element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode4 = parser_->findChild(partitionNode3,"fileinarchive");
	
	if(partitionNode4 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in fileinarchive element. \n");
		
		return NULL;
	}
	
	DOMString servantFile = parser_->getAttr(partitionNode4,"name");
	
	if(servantFile == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in fileinarchive name attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(servantFile.transcode());
	
    return ret._retn();
}

char*
ConfigFile::getServantEntry(const char* uuid)
{
    CORBA::String_var ret;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"servantdll");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in servantdll element. \n");
		
		return NULL;
	}
	
	DOMString servantEntry = parser_->getAttr(partitionNode3,"servantentrypoint");
	
	if(servantEntry == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in servantdll servantentrypoint attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(servantEntry.transcode());
	
    return ret._retn();
}

char*
ConfigFile::getContextEntry(const char* uuid)
{
    CORBA::String_var ret;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"servantdll");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in servantdll element. \n");
		
		return NULL;
	}
	
	DOMString contextEntry = parser_->getAttr(partitionNode3,"contextentrypoint");
	
	if(contextEntry == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in servantdll contextentrypoint attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(contextEntry.transcode());
	
    return ret._retn();
}

char*
ConfigFile::getExecutorFile(const char* uuid)
{
    CORBA::String_var ret;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"executordll");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode4 = parser_->findChild(partitionNode3,"fileinarchive");
	
	if(partitionNode4 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in fileinarchive element. \n");
		
		return NULL;
	}
	
	DOMString executorFile = parser_->getAttr(partitionNode4,"name");
	
	if(executorFile == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in fileinarchive name attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(executorFile.transcode());
	
    return ret._retn();
}


char*
ConfigFile::getExecutorRepId(const char* uuid)
{
    CORBA::String_var ret;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"executordll");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in executordll element. \n");
		
		return NULL;
	}
	
	DOMString repId = parser_->getAttr(partitionNode3,"componentrepid");
	
	if(repId == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in executordll componentrepid attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(repId.transcode());
	
    return ret._retn();
}

char*
ConfigFile::getExecutorEntry(const char* uuid)
{
    CORBA::String_var ret;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"executordll");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in executordll element. \n");
		
		return NULL;
	}
	
	DOMString executorEntry = parser_->getAttr(partitionNode3,"entrypoint");
	
	if(executorEntry == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in executordll entrypoint attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(executorEntry.transcode());
	
    return ret._retn();
}

char*
ConfigFile::getHomeRepId(const char* uuid)
{
    CORBA::String_var ret;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"executordll");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in executordll element. \n");
		
		return NULL;
	}
	
	DOMString repId = parser_->getAttr(partitionNode3,"homerepid");
	
	if(repId == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in executordll homerepid attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(repId.transcode());
	
    return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getValueTypeFile(const char* uuid)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}

	
	//modified by xiao heping 2004/02/15
	/*
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"valuetype");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in valuetype element. \n");
		
		return NULL;
	}

    DOM_NodeList children = partitionNode3.getChildNodes();
	*/
	DOM_NodeList children = partitionNode2.getChildNodes();
	//end modified

    unsigned int childrenLen = children.getLength();
    
    DOM_Node childNode;	
	DOMString valueTypeFile;	
	DOM_Node nodeNULL;
	DOM_Node partitionNode4;
														
	for(int i=0;i<childrenLen;i++)
	{
		childNode = children.item(i);
		
		if( stricmp(childNode.getNodeName().transcode(), "valuetype") != 0 )
        {
            continue;
        }
		
		partitionNode4 = parser_->findChild(childNode,"fileinarchive");
	
    	if(partitionNode4 == NULL)
    	{
    		TRACE0(LEVEL1,"Application Server config file read failed in fileinarchive element. \n");
    		
    		return NULL;
    	}
	        
		valueTypeFile = parser_->getAttr(partitionNode4, "name");
		
		if( valueTypeFile == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in container element name attribute. \n");
		    
		    return NULL;
		}

		ret->length(ret->length() + 1);
		ret[ret->length() - 1] = CORBA::string_dup(valueTypeFile.transcode());
	}
	
    return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getValueTypeFactoryRepId(const char* uuid)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}

	//modified by xiao heping 2004/02/15
	/*

	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"valuetype");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in valuetype element. \n");
		
		return NULL;
	}
	
	DOM_NodeList children = partitionNode3.getChildNodes();
	*/
	DOM_NodeList children = partitionNode2.getChildNodes();
	//end modified
    unsigned int childrenLen = children.getLength();
    
    DOM_Node childNode;	
	DOMString repId;	
	DOM_Node nodeNULL;
	
	for(int i=0;i<childrenLen;i++)
	{
		childNode = children.item(i);
		
		if( stricmp(childNode.getNodeName().transcode(), "valuetype") != 0 )
        {
            continue;
        }
        
        
    	repId = parser_->getAttr(childNode,"repid");
    	
    	if(repId == NULL)
    	{
    		TRACE0(LEVEL1,"Application Server config file read failed in valuetype repid attribute. \n");
    		
    		return NULL;
    	}
    	
    	ret->length(ret->length() + 1);
		ret[ret->length() - 1] = CORBA::string_dup(repId.transcode());
		
	}
	
    return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getValueTypeFactoryEntry(const char* uuid)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	//modified by xiao heping 2004/02/15
	/*
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"valuetype");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in valuetype element. \n");
		
		return NULL;
	}
	
	DOM_NodeList children = partitionNode3.getChildNodes();
	*/
	DOM_NodeList children = partitionNode2.getChildNodes();
	//end modified
    unsigned int childrenLen = children.getLength();
    
    DOM_Node childNode;	
	DOMString entry;	
	DOM_Node nodeNULL;
	
	for(int i=0;i<childrenLen;i++)
	{
		childNode = children.item(i);
		
		if( stricmp(childNode.getNodeName().transcode(), "valuetype") != 0 )
        {
            continue;
        }
        
        
    	entry = parser_->getAttr(childNode,"factoryentrypoint");
    	
    	if(entry == NULL)
    	{
    		TRACE0(LEVEL1,"Application Server config file read failed in valuetype factoryentrypoint attribute. \n");
    		
    		return NULL;
    	}
    	
    	ret->length(ret->length() + 1);
		ret[ret->length() - 1] = CORBA::string_dup(entry.transcode());
		
	}
	
    return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getValueTypeEntry(const char* uuid)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq();

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}

	//modified by xiao heping 2004/02/15
	/*
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"valuetype");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in valuetype element. \n");
		
		return NULL;
	}
	
	DOM_NodeList children = partitionNode3.getChildNodes();
	*/
	DOM_NodeList children = partitionNode2.getChildNodes();
	//end modified
    unsigned int childrenLen = children.getLength();
    
    DOM_Node childNode;	
	DOMString entry;	
	DOM_Node nodeNULL;
	
	for(int i=0;i<childrenLen;i++)
	{
		childNode = children.item(i);
		
		if( stricmp(childNode.getNodeName().transcode(), "valuetype") != 0 )
        {
            continue;
        }
        
        
    	entry = parser_->getAttr(childNode,"valueentrypoint");
    	
    	if(entry == NULL)
    	{
    		TRACE0(LEVEL1,"Application Server config file read failed in valuetype factoryentrypoint attribute. \n");
    		
    		return NULL;
    	}
    	
    	ret->length(ret->length() + 1);
		ret[ret->length() - 1] = CORBA::string_dup(entry.transcode());
		
	}
	
    return ret._retn();
}

int
ConfigFile::getComponentKind(const char* uuid)
{
    CORBA::String_var ret;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOMString kind = parser_->getAttr(partitionNode2,"componentkind");
	
	if(kind == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in valuetype valueentrypoint attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(kind.transcode());
	
	if( stricmp(ret.in(), "service") == 0 )
	{
	    return 0;
	}
	else if( stricmp(ret.in(), "session") == 0 )
	{
	    return 1;
	}
	else if( stricmp(ret.in(), "entity") == 0 )
	{
	    return 2;
	}
	else if( stricmp(ret.in(), "process") == 0)
	{
	    return 3;
	}
	else
	{
	    TRACE1(LEVEL1,"Application Server config file read Bad Component Kind %s. \n", ret.in());
	    return 0;
	}

}

int
ConfigFile::getComponentThreading(const char* uuid)
{
    CORBA::String_var ret;
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOMString threaded = parser_->getAttr(partitionNode2,"threading");
	
	if(threaded == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in valuetype valueentrypoint attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(threaded.transcode());
	
	if( stricmp(ret.in(), "serializable") == 0 )
	{
	    return 0;
	}
	else if( stricmp(ret.in(), "multithreaded") == 0 )
	{
	    return 1;
	}
	else
	{
	    TRACE1(LEVEL1,"Application Server config file read Bad Component thread policy %s. \n", ret.in());
	    return 0;
	}
	
}

int
ConfigFile::getConfigurationComplete(const char* uuid)
{
    CORBA::String_var ret;

    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOMString completed = parser_->getAttr(partitionNode2,"configurationcomplete");
	
	if(completed == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in valuetype valueentrypoint attribute. \n");
		
		return NULL;
	}
	
	ret = CORBA::string_dup(completed.transcode());
	
	if( stricmp(ret.in(), "false") == 0 )
	{
	    return 0;
	}
	else if( stricmp(ret.in(), "true") == 0 )
	{
	    return 1;
	}
	else
	{
	    TRACE1(LEVEL1,"Application Server config file read Bad Component ConfigurationComplete value %s. \n", ret.in());
	    return 0;
	}
	
    return 0;
}

CORBA::StringSeq*
ConfigFile::getResourcePools(const char* uuid)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq;
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in resourcepool element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChild(partitionNode1,"resourcepool");
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in resourcepool element. \n");
		
		return NULL;
	}
	
	DOM_NodeList children = partitionNode2.getChildNodes();
    unsigned int childrenLen = children.getLength();
    
    DOM_Node childNode;	
	DOMString name;	
	DOM_Node nodeNULL;
	
	for( int i = 0; i < childrenLen ; i++ )
	{
		childNode = children.item(i);

        if( stricmp(childNode.getNodeName().transcode(), "pool") != 0 )
        {
            continue;
        }
		
		name = parser_->getAttr(childNode, "name");

		if( name == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in resourcepool name attribute. \n");
		
		    return NULL;
		}
		
		int len = ret->length() + 1;
		ret->length(len);
		ret[(CORBA::ULong)(len - 1)] = CORBA::string_dup(name.transcode());
	}
	
	return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getResourcePoolProperties(const char* uuid, const char* name)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq;
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChild(partitionNode1,"resourcepool");
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in resourcepool element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChildByAttr(partitionNode2, "pool", "name", name);
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in pool element. \n");
		
		return NULL;
	}
	
	ret->length(7);
	
	DOMString host;
	DOMString database;
	DOMString user;
	DOMString password;
	DOMString max;
	DOMString initial;
	DOMString inc;
	
	host = parser_->getAttr(partitionNode3, "host");
	
	if(host == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s host attribute. \n", name);
		
		return NULL;
	}
	
	database = parser_->getAttr(partitionNode3, "database");
	
	if(database == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s host attribute. \n", name);
		
		return NULL;
	}
	
	user = parser_->getAttr(partitionNode3, "user");
	
	if(user == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s user attribute. \n", name);
		
		return NULL;
	}
	
	password = parser_->getAttr(partitionNode3, "password");
	
	if(password == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s password attribute. \n", name);
		
		return NULL;
	}
	
	max = parser_->getAttr(partitionNode3, "maxconnections");
	
	if(max == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s maxconnection attribute. \n", name);
		
		return NULL;
	}
	
	initial = parser_->getAttr(partitionNode3, "initialconnections");
	
	if(initial == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s initialconnection attribute. \n", name);
		
		return NULL;
	}
	
	inc = parser_->getAttr(partitionNode3, "increment");
	
	if(inc == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s increment attribute. \n", name);
		
		return NULL;
	}
	
	ret[(CORBA::ULong)0] = CORBA::string_dup(host.transcode());
	ret[(CORBA::ULong)1] = CORBA::string_dup(database.transcode());
	ret[(CORBA::ULong)2] = CORBA::string_dup(user.transcode());
	ret[(CORBA::ULong)3] = CORBA::string_dup(password.transcode());
	ret[(CORBA::ULong)4] = CORBA::string_dup(initial.transcode());
	ret[(CORBA::ULong)5] = CORBA::string_dup(max.transcode());
	ret[(CORBA::ULong)6] = CORBA::string_dup(inc.transcode());
	
	return ret._retn();
	
}

char*
ConfigFile::getCosRef(const char* uuid, const char* name)
{
    CORBA::String_var ret;
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChild(partitionNode1,"cos");
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in cos element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,name);
	
	if(partitionNode3 == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s element. \n", name);
		
		return NULL;
	}
	
	DOMString ref = parser_->getAttr(partitionNode3,"initref");
	
	if(ref == NULL)
	{
		TRACE1(LEVEL1,"Application Server config file read failed in %s initref attribute. \n", name);
		
		return NULL;
	}
	
	ret = CORBA::string_dup(ref.transcode());
	
	return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getHomeMethodPolicies(const char* uuid, const char* repId)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq();
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "transactionpolicies");
	
	if(partitionNode3 == NULL)
	{
//		TRACE0(LEVEL1,"Application Server config file read failed in transactionpolicies element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode4 = parser_->findChildByAttr(partitionNode3, "homemethodpolicies", "repid", repId);
	
	if(partitionNode4 == NULL)
	{
//		TRACE0(LEVEL1,"Application Server config file read failed in homemethodpolicies element. \n");
		
		return NULL;
	}
	
	DOM_NodeList children = partitionNode4.getChildNodes();
    unsigned int childrenLen = children.getLength();
    
	DOM_Node childNode;
	DOMString name;
	DOMString policy;
	DOM_Node nodeNULL;
	
	for( int i = 0; i < childrenLen ; i++ )
	{
		childNode = children.item(i);

        if( stricmp(childNode.getNodeName().transcode(), "operation") != 0 )
        {
            continue;
        }
		
		name = parser_->getAttr(childNode, "name");

		if( name == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in operation name attribute. \n");
		
		    return NULL;
		}
		
		policy = parser_->getAttr(childNode, "policy");

		if( policy == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in operation policy attribute. \n");
		
		    return NULL;
		}
		
		int len = ret->length() + 2;
		ret->length(len);
		ret[(CORBA::ULong)(len - 2)] = CORBA::string_dup(name.transcode());
		ret[(CORBA::ULong)(len - 1)] = CORBA::string_dup(policy.transcode());
	}
	
	
    return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getComponentRepIds(const char* uuid)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq();
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "transactionpolicies");
	
	if(partitionNode3 == NULL)
	{
//		TRACE0(LEVEL1,"Application Server config file read failed in transactionpolicies element. \n");
		
		return NULL;
	}
		
	DOM_NodeList children = partitionNode3.getChildNodes();
    unsigned int childrenLen = children.getLength();
    
	DOM_Node childNode;
	DOMString repid;
	DOM_Node nodeNULL;
	
	for( int i = 0; i < childrenLen ; i++ )
	{
		childNode = children.item(i);

        if( stricmp(childNode.getNodeName().transcode(), "componentmethodpolicies") != 0 )
        {
            continue;
        }
		
		repid = parser_->getAttr(childNode, "repid");

		if( repid == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in componentmethodpolicies repid attribute. \n");
		
		    return NULL;
		}
				
		int len = ret->length() + 1;
		ret->length(len);
		ret[(CORBA::ULong)(len - 1)] = CORBA::string_dup(repid.transcode());
	}
	
    return ret._retn();
}

CORBA::StringSeq*
ConfigFile::getComponentMethodPolicies(const char* uuid, const char* repId)
{
    CORBA::StringSeq_var ret = new CORBA::StringSeq();
    
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "transactionpolicies");
	
	if(partitionNode3 == NULL)
	{
//		TRACE0(LEVEL1,"Application Server config file read failed in transactionpolicies element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode4 = parser_->findChildByAttr(partitionNode3, "componentmethodpolicies", "repid", repId);
	
	if(partitionNode4 == NULL)
	{
//		TRACE0(LEVEL1,"Application Server config file read failed in componentmethodpolicies element. \n");
		
		return NULL;
	}
	
	DOM_NodeList children = partitionNode4.getChildNodes();
    unsigned int childrenLen = children.getLength();
    
	DOM_Node childNode;
	DOMString name;
	DOMString policy;
	DOM_Node nodeNULL;
	
	for( int i = 0; i < childrenLen ; i++ )
	{
		childNode = children.item(i);

        if( stricmp(childNode.getNodeName().transcode(), "operation") != 0 )
        {
            continue;
        }
		
		name = parser_->getAttr(childNode, "name");

		if( name == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in operation name attribute. \n");
		
		    return NULL;
		}
		
		policy = parser_->getAttr(childNode, "policy");

		if( policy == NULL )
		{
		    TRACE0(LEVEL1,"Application Server config file read failed in operation policy attribute. \n");
		
		    return NULL;
		}
		
		int len = ret->length() + 2;
		ret->length(len);
		ret[(CORBA::ULong)(len - 2)] = CORBA::string_dup(name.transcode());
		ret[(CORBA::ULong)(len - 1)] = CORBA::string_dup(policy.transcode());
	}
	
    return ret._retn();
}
//modified by xiao heping 
//void ConfigFile::addContainer(const char* uuid)
void
ConfigFile::addContainer(const char* ComponentServerUuid,const char* uuid)
//end modified
{
    //find the component server first
    DOM_Node root = parser_->getRootNode("ccm");

    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "container", "uuid", uuid);
	
	if(partitionNode1 != NULL)
	{
		TRACE0(LEVEL1,"Application Server config file container element existed. \n");
		
		return;
	}
	
	//modified by xiao heping 2004/02/15
	//DOM_Node partitionNode2 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid_.in());
	DOM_Node partitionNode2 = parser_->findChildByAttr(root, "componentserver", "uuid", ComponentServerUuid);
	//end modified
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return;
	}

	DOM_Node newNode1 = doc.createElement("container");

	DOM_Element newContainer1 = (DOM_Element&)newNode1;

	newContainer1.setAttribute("uuid", uuid);
	
	DOM_Node newNode2 = doc.createElement("container");

	DOM_Element newContainer2 = (DOM_Element&)newNode2;

	newContainer2.setAttribute("uuid", uuid);
	
	root.appendChild(newContainer1);
	partitionNode2.appendChild(newContainer2);

	parser_->printToFile(file_.in());

	
}

//modified by xiao heping 2004/02/15
//void ConfigFile::removeContainer(const char* uuid)
void
ConfigFile::removeContainer(const char* ComponentServerUuid,const char* uuid)
//end modified
{
    DOM_Node root = parser_->getRootNode("ccm");

    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	//modified by xiao heping 2004/02/15
	//DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid_.in());
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", ComponentServerUuid);
	//end modified
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "container", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file  read failed in container element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChildByAttr(root,"container","uuid",uuid);
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in container element. \n");
		
		return;
	}
	
	root.removeChild(partitionNode3);
	partitionNode1.removeChild(partitionNode2);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::addCosRef(const char* uuid, const char* name, const char* ref)
{
    if( strcmp(ref, "") == 0 )
    {
        return;
    }
    
    DOM_Node root = parser_->getRootNode("ccm");

    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChild(partitionNode1,"cos");
	
	if(partitionNode2 == NULL)
	{
		//create cos node if not existed.
		partitionNode2 = doc.createElement("cos");
		partitionNode1.appendChild(partitionNode2);
		
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,name);
	
	if(partitionNode3 != NULL)
	{
		partitionNode2.removeChild(partitionNode3);
	}

    //update or add new reference of service
	DOM_Node newNode2 = doc.createElement(name);
	
	DOM_Element newCos = (DOM_Element&)newNode2;

	newCos.setAttribute("initref", ref);
	
	partitionNode2.appendChild(newCos);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::addInstalledComponent(const char* container, const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");

    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root,"container", "uuid", container);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in container element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1,"component","uuid",uuid);
	
	if(partitionNode2 != NULL)
	{
	    TRACE0(LEVEL1,"Application Server config file component element existed. \n");
	    
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node newNode1 = doc.createElement("component");
	
	DOM_Element newComp1 = (DOM_Element&)newNode1;

	newComp1.setAttribute("uuid", uuid);
	
	DOM_Node newNode2 = doc.createElement("component");
	
	DOM_Element newComp2 = (DOM_Element&)newNode2;

	newComp2.setAttribute("uuid", uuid);
	
	partitionNode1.appendChild(newComp1);
    partitionNode3.appendChild(newComp2);
    
	parser_->printToFile(file_.in());
}

void
ConfigFile::removeInstalledComponent(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");

    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentserver");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChild(partitionNode1,"container");
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in container element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChildByAttr(partitionNode2,"component","uuid",uuid);
	
	if(partitionNode3 == NULL)
	{
	    TRACE0(LEVEL1,"Application Server config file component id element non-existed. \n");
	    
		return;
	}
	
	DOM_Node partitionNode4 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode4 == NULL)
	{
	    TRACE0(LEVEL1,"Application Server config file component id element non-existed. \n");
	    
		return;
	}
	
	DOM_Node partitionNode5 = parser_->findChildByAttr(root, "component", "uuid", uuid);
	
	if(partitionNode5 == NULL)
	{
	    TRACE0(LEVEL1,"Application Server config file component id element non-existed. \n");
	    
		return;
	}
	
	partitionNode2.removeChild(partitionNode3);
	partitionNode4.removeChild(partitionNode5);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::addServantFile(const char* uuid, 
                    const char* servantEntryPoint, 
                    const char* contextEntryPoint,
                    const char* name)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"servantdll");
	
	if(partitionNode3 != NULL)
	{
		TRACE0(LEVEL1,"Application Server config file servantdll element existed. \n");
		
		partitionNode2.removeChild(partitionNode3);
		
		return;
	}
	
	DOM_Document doc = root.getOwnerDocument();
	
	DOM_Node newNode = doc.createElement("servantdll");
	
	DOM_Element newServantDll = (DOM_Element&)newNode;
	
	newServantDll.setAttribute("contextentrypoint", contextEntryPoint);
	newServantDll.setAttribute("servantentrypoint", servantEntryPoint);
	
	DOM_Node newNode1 = doc.createElement("fileinarchive");
	
	DOM_Element newFile = (DOM_Element&)newNode1;
	
	newFile.setAttribute("name", name);
	
	newServantDll.appendChild(newFile);
	
	partitionNode2.appendChild(newServantDll);
	
	parser_->printToFile(file_.in());
	
}

void
ConfigFile::addExecutorFile(const char* uuid, 
                     const char* entryPoint, 
                     const char* repId,
                     const char* compId,
                     const char* name)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"executordll");
	
	if(partitionNode3 != NULL)
	{
		TRACE0(LEVEL1,"Application Server config file executordll element existed. \n");
		
		partitionNode2.removeChild(partitionNode3);
		
		return;
	}
	
	DOM_Document doc = root.getOwnerDocument();
	
	DOM_Node newNode = doc.createElement("executordll");
	
	DOM_Element newExecutorDll = (DOM_Element&)newNode;
	
	newExecutorDll.setAttribute("homerepid", repId);
	newExecutorDll.setAttribute("entrypoint", entryPoint);
	newExecutorDll.setAttribute("componentrepid", compId);
	
	DOM_Node newNode1 = doc.createElement("fileinarchive");
	
	DOM_Element newFile = (DOM_Element&)newNode1;
	
	newFile.setAttribute("name", name);
	
	newExecutorDll.appendChild(newFile);
	
	partitionNode2.appendChild(newExecutorDll);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::addValueTypeFile(const char* uuid, 
                      const char* valueEntryPoint, 
                      const char* factoryEntryPoint, 
                      const char* repId,
                      const char* name)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2,"valuetype");
	
	if(partitionNode3 != NULL)
	{
		TRACE0(LEVEL1,"Application Server config file valuetype element existed. \n");
		
		partitionNode2.removeChild(partitionNode3);
		
		return;
	}
	
	DOM_Document doc = root.getOwnerDocument();
	
	DOM_Node newNode = doc.createElement("valuetype");
	
	DOM_Element newValueDll = (DOM_Element&)newNode;
	
	newValueDll.setAttribute("repid", repId);
	newValueDll.setAttribute("valueentrypoint", valueEntryPoint);
	newValueDll.setAttribute("factoryentrypoint", factoryEntryPoint);
	
	DOM_Node newNode1 = doc.createElement("fileinarchive");
	
	DOM_Element newFile = (DOM_Element&)newNode1;
	
	newFile.setAttribute("name", name);
	
	newValueDll.appendChild(newFile);
	
	partitionNode2.appendChild(newValueDll);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::addComponentKind(const char* uuid, int kind)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Element element = (DOM_Element&)partitionNode2;
	
	char* value;
	
	switch(kind)
	{
	    case 0:
	        value = "service";
	        break;
	    case 1:
	        value = "session";
	        break;
	    case 2:
	        value = "entity";
	        break;
	    case 3:
	        value = "process";
	        break;
	    default:
	        break;
	}
	
	element.setAttribute("componentkind", value);
	
    parser_->printToFile(file_.in());
}

void
ConfigFile::addComponentThreading(const char* uuid, int isMultiThreaded)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Element element = (DOM_Element&)partitionNode2;
	
	char* value;
	
	switch(isMultiThreaded)
	{
	    case 0:
	        value = "serializable";
	        break;
	    case 1:
	        value = "multithreaded";
	        break;
	    default:
	        break;
	}
	
	element.setAttribute("threading", value);
	
    parser_->printToFile(file_.in());
}

void
ConfigFile::addConfigurationComplete(const char* uuid, int configurationCompleted)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root,"componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Element element = (DOM_Element&)partitionNode2;
	
	char* value;
	
	switch(configurationCompleted)
	{
	    case 0:
	        value = "false";
	        break;
	    case 1:
	        value = "true";
	        break;
	    default:
	        break;
	}
	
	element.setAttribute("configurationcomplete", value);
	
    parser_->printToFile(file_.in());
}

void
ConfigFile::addHomeTransactionPolicy(const char* uuid, 
                                     const char* opName, 
                                     const char* repId, 
                                     TxnPolicy policy)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();

    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "transactionpolicies");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("transactionpolicies");
		
		partitionNode2.appendChild(partitionNode3);
	}
    
    DOM_Element policyNode;
    
    DOM_Node partitionNode4 = parser_->findChildByAttr(partitionNode3, "homemethodpolicies", "repid", repId);
	
	if(partitionNode4 == NULL)
	{

    	DOM_Node newNode1 = doc.createElement("homemethodpolicies");
    	policyNode = (DOM_Element&)newNode1;
    	
    	policyNode.setAttribute("repid", repId);
    	
    	partitionNode3.appendChild(policyNode);
    	
	}
	else
	{
	    policyNode = (DOM_Element&)partitionNode4;
	}
    
    char* policyString;
    
    switch(policy)
    {
        case NOT_SUPPORTED:
            policyString="NotSupported";
            break;
        case REQUIRED:
            policyString="Required";
            break;
        case SUPPORTS:
            policyString="Supported";
            break;
        case REQUIRES_NEW:
            policyString="RequiresNew";
            break;
        case MANDATORY:
            policyString="Mandatory";
            break;
        case NEVER:
            policyString="Never";
            break;
        default:
            
            break;
    }

	
	DOM_Node newNode2 = doc.createElement("operation");
	DOM_Element operationNode = (DOM_Element&)newNode2;
	
	operationNode.setAttribute("name", opName);
	operationNode.setAttribute("policy", policyString);
	
    policyNode.appendChild(operationNode);
    
    parser_->printToFile(file_.in());
}

void
ConfigFile::addHomeTransactionPolicy(const char* uuid, 
                                     const char* opName, 
                                     const char* repId, 
                                     const char* policy)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();

    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "transactionpolicies");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("transactionpolicies");
		
		partitionNode2.appendChild(partitionNode3);
	}
    
    DOM_Element policyNode;
    
    DOM_Node partitionNode4 = parser_->findChildByAttr(partitionNode3, "homemethodpolicies", "repid", repId);
	
	if(partitionNode4 == NULL)
	{

    	DOM_Node newNode1 = doc.createElement("homemethodpolicies");
    	policyNode = (DOM_Element&)newNode1;
    	
    	policyNode.setAttribute("repid", repId);
    	
    	partitionNode3.appendChild(policyNode);
    	
	}
	else
	{
	    policyNode = (DOM_Element&)partitionNode4;
	}
    
    DOM_Node newNode2 = doc.createElement("operation");
	DOM_Element operationNode = (DOM_Element&)newNode2;
	
	operationNode.setAttribute("name", opName);
	operationNode.setAttribute("policy", policy);
	
    policyNode.appendChild(operationNode);
    
    parser_->printToFile(file_.in());
}

void
ConfigFile::addCompTransactionPolicy(const char* uuid, 
                                     const char* opName, 
                                     const char* repId, 
                                     TxnPolicy policy)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();

    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "transactionpolicies");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("transactionpolicies");
		
		partitionNode2.appendChild(partitionNode3);
	}
    
    DOM_Element policyNode;
    
    DOM_Node partitionNode4 = parser_->findChildByAttr(partitionNode3, "componentmethodpolicies", "repid", repId);
	
	if(partitionNode4 == NULL)
	{
    	
    	DOM_Node newNode1 = doc.createElement("componentmethodpolicies");
    	policyNode = (DOM_Element&)newNode1;
    	
    	policyNode.setAttribute("repid", repId);
    	
    	partitionNode3.appendChild(policyNode);

	}
	else
	{
	    policyNode = (DOM_Element&)partitionNode4;
	}
    
    char* policyString;
    
    switch(policy)
    {
        case NOT_SUPPORTED:
            policyString="NotSupported";
            break;
        case REQUIRED:
            policyString="Required";
            break;
        case SUPPORTS:
            policyString="Supported";
            break;
        case REQUIRES_NEW:
            policyString="RequiresNew";
            break;
        case MANDATORY:
            policyString="Mandatory";
            break;
        case NEVER:
            policyString="Never";
            break;
        default:
            
            break;
    }

	
	DOM_Node newNode2 = doc.createElement("operation");
	DOM_Element operationNode = (DOM_Element&)newNode2;
	
	operationNode.setAttribute("name", opName);
	operationNode.setAttribute("policy", policyString);
	
    policyNode.appendChild(operationNode);
    
    parser_->printToFile(file_.in());
}

void
ConfigFile::addCompTransactionPolicy(const char* uuid, 
                                     const char* opName, 
                                     const char* repId, 
                                     const char* policy)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();

    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "transactionpolicies");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("transactionpolicies");
		
		partitionNode2.appendChild(partitionNode3);
	}
    
    DOM_Element policyNode;
    
    DOM_Node partitionNode4 = parser_->findChildByAttr(partitionNode3, "componentmethodpolicies", "repid", repId);
	
	if(partitionNode4 == NULL)
	{
    	
    	DOM_Node newNode1 = doc.createElement("componentmethodpolicies");
    	policyNode = (DOM_Element&)newNode1;
    	
    	policyNode.setAttribute("repid", repId);
    	
    	partitionNode3.appendChild(policyNode);

	}
	else
	{
	    policyNode = (DOM_Element&)partitionNode4;
	}
    
	DOM_Node newNode2 = doc.createElement("operation");
	DOM_Element operationNode = (DOM_Element&)newNode2;
	
	operationNode.setAttribute("name", opName);
	operationNode.setAttribute("policy", policy);
	
    policyNode.appendChild(operationNode);
    
    parser_->printToFile(file_.in());
}

void
ConfigFile::updatePort(const char* uuid, short port)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return;
	}
	
    DOM_Element serverNode = (DOM_Element&)partitionNode;
    
    char buffer[10];
#ifdef WIN32
    itoa(port, buffer, 10);
#endif
    serverNode.setAttribute("port", buffer);
	
    parser_->printToFile(file_.in());
}

void
ConfigFile::addReourcePool(const char* uuid, CORBA::StringSeq* ss)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChild(partitionNode1,"resourcepool");
	
	if(partitionNode2 == NULL)
	{
		partitionNode2 = doc.createElement("resourcepool");
		
		partitionNode1.appendChild(partitionNode2);
	}
    
    DOM_Node partitionNode3 = parser_->findChildByAttr(partitionNode2, "pool", "name", ((*ss)[0]).in());
	
	if(partitionNode3 != NULL)
	{
		return;
	}
    
    DOM_Node newNode2 = doc.createElement("pool");
    
    DOM_Element node = (DOM_Element&)newNode2;
    
    node.setAttribute("name", ((*ss)[0]).in());
    node.setAttribute("host", ((*ss)[1]).in());
    node.setAttribute("database", ((*ss)[2]).in());
    node.setAttribute("user", ((*ss)[3]).in());
    node.setAttribute("password", ((*ss)[4]).in());
    node.setAttribute("initialconnections", ((*ss)[5]).in());
    node.setAttribute("maxconnections", ((*ss)[6]).in());
    node.setAttribute("increment", ((*ss)[7]).in());
	
	partitionNode2.appendChild(node);
	
    parser_->printToFile(file_.in());
}

//deleted by xiao heping 2004/02/15
/*
void
ConfigFile::setUuid(const char* uuid)
{
    uuid_ = CORBA::string_dup(uuid);
}

char*
ConfigFile::getUuid()
{
    return CORBA::string_dup(uuid_);
}
*/
//end deleted

void
ConfigFile::addComponentServer(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode1 != NULL)
	{
//		TRACE0(LEVEL1,"Application Server config file componentserver existed. \n");
		
		return;
	}
	
	DOM_Node newNode = doc.createElement("componentserver");
	
	DOM_Element serverNode = (DOM_Element&)newNode;
	
	serverNode.setAttribute("uuid", uuid);
	
	root.appendChild(serverNode);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::removeCompnentServer(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChildByAttr(root, "componentserver", "uuid", uuid);
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentserver element. \n");
		
		return;
	}
	
	root.removeChild(partitionNode1);
	
	parser_->printToFile(file_.in());
}

int
ConfigFile::getPersistentPolicy(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return 0;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return 0;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return 0;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy element. \n");
		
		return 0;
	}
	
	DOMString manager = parser_->getAttr(partitionNode3, "manager");
	
	if(manager == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy element. \n");
		
		return 0;
	}
	
	CORBA::String_var ret = CORBA::string_dup(manager.transcode());
	
	if( strcmp(ret.in(), "container") == 0 )
	{
	    return 1;
	}
	else
	{
	    return 0;
	}
}

int
ConfigFile::getPersistentTxnPolicy(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return 1;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return 1;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return 1;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy element. \n");
		
		return 1;
	}
	
	DOMString policy = parser_->getAttr(partitionNode3, "transactionpolicy");
	
	if(policy == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy transactionpolicy attribute. \n");
		
		return 1;
	}
	
	CORBA::String_var ret = CORBA::string_dup(policy.transcode());
	
	if( strcmp(ret.in(), "transactional") == 0 )
	{
	    return 0;
	}
	else
	{
	    return 1;
	}
}

int
ConfigFile::getAccessMode(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return 1;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return 1;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return 1;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy element. \n");
		
		return 1;
	}
	
	DOMString mode = parser_->getAttr(partitionNode3, "accessmode");
	
	if(mode == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy accessmode attribute. \n");
		
		return 1;
	}
	
	CORBA::String_var ret = CORBA::string_dup(mode.transcode());
	
	if( strcmp(ret.in(), "readonly") == 0 )
	{
	    return 0;
	}
	else 
	{
	    return 1;
	}
}

int
ConfigFile::getisolationLevel(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return 3;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return 3;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return 3;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy element. \n");
		
		return 3;
	}
	
	DOMString level = parser_->getAttr(partitionNode3, "isolationlevel");
	
	if(level == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy isolationlevel attribute. \n");
		
		return 3;
	}
	
	CORBA::String_var ret = CORBA::string_dup(level.transcode());
	
	if( strcmp(ret.in(), "readuncommitted") == 0 )
	{
	    return 0;
	}
	else if( strcmp(ret.in(), "readcommitted") == 0 )
	{
	    return 1;
	}
	else if( strcmp(ret.in(), "repeatableread") == 0 )
	{
	    return 2;
	}
	else if( strcmp(ret.in(), "serializable") == 0 )
	{
	    return 3;
	}
	else
	{
	    return 3;
	}
}

char*
ConfigFile::getStorageHomeId(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy element. \n");
		
		return NULL;
	}
	
	DOMString homeId = parser_->getAttr(partitionNode3, "storagehomeid");
	
	if(homeId == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy storagehomeid attribute. \n");
		
		return NULL;
	}
	
	CORBA::String_var ret = CORBA::string_dup(homeId.transcode());
	
	return ret._retn();
}

char*
ConfigFile::getPersistentFile(const char* uuid)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return NULL;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy element. \n");
		
		return NULL;
	}
	
	DOMString name = parser_->getAttr(partitionNode3, "filename");
	
	if(name == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in persistentpolicy filename attribute. \n");
		
		return NULL;
	}
	
	CORBA::String_var ret = CORBA::string_dup(name.transcode());
	
	return ret._retn();
}

void
ConfigFile::setPersistentPolicy(const char* uuid, int isCmp)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("persistentpolicy");
		partitionNode2.appendChild(partitionNode3);
		
	}
	
	char* manager;
	
	switch(isCmp)
	{
	    case 0:
	        manager = "self";
	        break;
	    case 1:
	        manager = "container";
	        break;
	    default:
	        break;
	}
	
	DOM_Element node = (DOM_Element&)partitionNode3;
	
	node.setAttribute("manager", manager);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::setPersistentTxnPolicy(const char* uuid, int isTransactional)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("persistentpolicy");
		partitionNode2.appendChild(partitionNode3);
		
	}
	
	char* policy;
	
	switch(isTransactional)
	{
	    case 0:
	        policy = "transactional";
	        break;
	    case 1:
	        policy = "nontransactional";
	        break;
	    default:
	        break;
	}
	
	DOM_Element node = (DOM_Element&)partitionNode3;
	
	node.setAttribute("transactionpolicy", policy);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::setAccessMode(const char* uuid, int mode)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("persistentpolicy");
		partitionNode2.appendChild(partitionNode3);
		
	}
	
	char* accessMode;
	
	switch(mode)
	{
	    case 0:
	        accessMode = "readonly";
	        break;
	    case 1:
	        accessMode = "readwrite";
	        break;
	    default:
	        break;
	}
	
	DOM_Element node = (DOM_Element&)partitionNode3;
	
	node.setAttribute("accessmode", accessMode);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::setIsolationLevel(const char* uuid, int level)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("persistentpolicy");
		partitionNode2.appendChild(partitionNode3);
		
	}
	
	char* isolationLevel;
	
	switch(level)
	{
	    case 0:
	        isolationLevel = "readuncommitted";
	        break;
	    case 1:
	        isolationLevel = "readcommitted";
	        break;
	    case 2:
	        isolationLevel = "repeatableread";
	        break;
	    case 3:
	        isolationLevel = "serializable";
	        break;
	    default:
	        break;
	}
	
	DOM_Element node = (DOM_Element&)partitionNode3;
	
	node.setAttribute("isolationlevel", isolationLevel);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::setStorageHomeId(const char* uuid, const char* id)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("persistentpolicy");
		partitionNode2.appendChild(partitionNode3);
		
	}
	
	DOM_Element node = (DOM_Element&)partitionNode3;
	
	node.setAttribute("storagehomeid", id);
	
	parser_->printToFile(file_.in());
}

void
ConfigFile::setPersistentFile(const char* uuid, const char* name)
{
    DOM_Node root = parser_->getRootNode("ccm");
    
    DOM_Document doc = root.getOwnerDocument();
    
    if(root == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in root element. \n");
		
		return;
	}
	
	DOM_Node partitionNode1 = parser_->findChild(root, "componentrepository");
	
	if(partitionNode1 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in componentrepository element. \n");
		
		return;
	}
	
	DOM_Node partitionNode2 = parser_->findChildByAttr(partitionNode1, "component", "uuid", uuid);
	
	if(partitionNode2 == NULL)
	{
		TRACE0(LEVEL1,"Application Server config file read failed in component element. \n");
		
		return;
	}
	
	DOM_Node partitionNode3 = parser_->findChild(partitionNode2, "persistentpolicy");
	
	if(partitionNode3 == NULL)
	{
		partitionNode3 = doc.createElement("persistentpolicy");
		partitionNode2.appendChild(partitionNode3);
		
	}
	
	DOM_Element node = (DOM_Element&)partitionNode3;
	
	node.setAttribute("filename", name);
	
	parser_->printToFile(file_.in());
}

