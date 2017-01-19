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

#ifndef XMLParser_H
#define XMLParser_H

#include <xercesc/dom/DOM_Node.hpp>
#include <xercesc/dom/DOMString.hpp>
#include <xercesc/parsers/DOMParser.hpp>

namespace STARCCM
{
	namespace Deployment
	{
		
		class XMLInitializeFailed
		{
		public:
		    XMLInitializeFailed(){};
		    ~XMLInitializeFailed(){};
		};
		
		class XMLParser
		{
		public:			
			XMLParser();
			~XMLParser();
			
			void initialize(const char *XMLfile) 
				throw (STARCCM::Deployment::XMLInitializeFailed);			
			
			DOM_Node getRootNode(const char *rootName);
			
			DOM_Node findChild(DOM_Node fatherNode,const char *chidName);
			
			DOM_Node findChildByAttr(DOM_Node fatherNode,const char *childName,const char *attrName,const char * attrValue);
			
			DOMString getAttr(DOM_Node node,const char *attrName);	
			
			DOMString parseFileInArchive(DOM_Node node);
			
			int printToFile(const char *fileName);
		
		private:
			
			DOMParser *parser;
			
			DOM_Node doc;
	
		};
	}
}

#endif


