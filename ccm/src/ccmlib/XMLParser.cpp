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

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/util/TranscodingException.hpp>

#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/SAXException.hpp>

#include <iostream>
#include <malloc.h>
#include <strstream>
#include <string>

//#ifdef HAVE_FSTREAM
#   include <fstream>
//#else
//#   include <fstream.h>
//#endif

#include <XMLParser.h>
#include <CORBA.h>

#ifdef WIN32
#	include<io.h>
#	include<windows.h>
#endif

using namespace std;
using namespace STARCCM::Deployment;
 
class RedirectHandlers : public EntityResolver
{
public:
	RedirectHandlers(const char * path)
	{
		rootPath = string (path);
		rootPath += PATH_DELILIMITOR;
		rootPath += "temp.dtd";
		TRACE1(LEVEL6,"XMLParser: the redirect dtd file in resolver is %s",rootPath.c_str());
	};
	
    ~RedirectHandlers(){};
    
    InputSource* resolveEntity(const   XMLCh* const    publicId
        						, const XMLCh* const    systemId)
    {    	
    	LocalFileInputSource * dtdfile=new LocalFileInputSource(XMLString::transcode(rootPath.c_str()));
    	 
    	return dtdfile;
    };
    
private:
	string  rootPath;    
};

// ---------------------------------------------------------------------------
//  Local const data
//
//  Note: This is the 'safe' way to do these strings. If you compiler supports
//        L"" style strings, and portability is not a concern, you can use
//        those types constants directly.
// ---------------------------------------------------------------------------
static const XMLCh  gEndElement[] = { chOpenAngle, chForwardSlash, chNull };
static const XMLCh  gEndPI[] = { chQuestion, chCloseAngle, chNull};
static const XMLCh  gStartPI[] = { chOpenAngle, chQuestion, chNull };
static const XMLCh  gXMLDecl1[] =
{
        chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l
    ,   chSpace, chLatin_v, chLatin_e, chLatin_r, chLatin_s, chLatin_i
    ,   chLatin_o, chLatin_n, chEqual, chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl2[] =
{
        chDoubleQuote, chSpace, chLatin_e, chLatin_n, chLatin_c
    ,   chLatin_o, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chEqual
    ,   chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl3[] =
{
        chDoubleQuote, chSpace, chLatin_s, chLatin_t, chLatin_a
    ,   chLatin_n, chLatin_d, chLatin_a, chLatin_l, chLatin_o
    ,   chLatin_n, chLatin_e, chEqual, chDoubleQuote, chNull
};
static const XMLCh  gXMLDecl4[] =
{
        chDoubleQuote, chQuestion, chCloseAngle
    ,   chLF, chNull
};

static const XMLCh  gStartCDATA[] =
{
        chOpenAngle, chBang, chOpenSquare, chLatin_C, chLatin_D,
        chLatin_A, chLatin_T, chLatin_A, chOpenSquare, chNull
};

static const XMLCh  gEndCDATA[] =
{
    chCloseSquare, chCloseSquare, chCloseAngle, chNull
};
static const XMLCh  gStartComment[] =
{
    chOpenAngle, chBang, chDash, chDash, chNull
};

static const XMLCh  gEndComment[] =
{
    chDash, chDash, chCloseAngle, chNull
};

static const XMLCh  gStartDoctype[] =
{
    chOpenAngle, chBang, chLatin_D, chLatin_O, chLatin_C, chLatin_T,
    chLatin_Y, chLatin_P, chLatin_E, chSpace, chNull
};
static const XMLCh  gPublic[] =
{
    chLatin_P, chLatin_U, chLatin_B, chLatin_L, chLatin_I,
    chLatin_C, chSpace, chDoubleQuote, chNull
};
static const XMLCh  gSystem[] =
{
    chLatin_S, chLatin_Y, chLatin_S, chLatin_T, chLatin_E,
    chLatin_M, chSpace, chDoubleQuote, chNull
};
static const XMLCh  gStartEntity[] =
{
    chOpenAngle, chBang, chLatin_E, chLatin_N, chLatin_T, chLatin_I,
    chLatin_T, chLatin_Y, chSpace, chNull
};
static const XMLCh  gNotation[] =
{
    chLatin_N, chLatin_D, chLatin_A, chLatin_T, chLatin_A,
    chSpace, chDoubleQuote, chNull
};


// ---------------------------------------------------------------------------
//  Local classes
// ---------------------------------------------------------------------------

class DOMPrintFormatTarget : public XMLFormatTarget
{
public:
    DOMPrintFormatTarget(strstreambuf* buf)  { this->buf = buf; };
    ~DOMPrintFormatTarget() {};

    // -----------------------------------------------------------------------
    //  Implementations of the format target interface
    // -----------------------------------------------------------------------

    void writeChars(const   XMLByte* const  toWrite,
                    const   unsigned int    count,
                            XMLFormatter * const formatter)
    {
        // Surprisingly, Solaris was the only platform on which
        // required the char* cast to print out the string correctly.
        // Without the cast, it was printing the pointer value in hex.
        // Quite annoying, considering every other platform printed
        // the string with the explicit cast to char* below.
        
        //original code
        //out.write((char *) toWrite, (int) count);

        //Dou's code
//			ofstream out;
//	    	out.open(outPutFile,ios::app|ios_base::out);
//
//            out.write((char *) toWrite, (int) count);
//			out.close();
        
        //My code
        ostream out(buf);
        out.write((char *) toWrite, (int) count);
        
    };

private:
    // -----------------------------------------------------------------------
    //  Unimplemented methods.
    // -----------------------------------------------------------------------
    DOMPrintFormatTarget(const DOMPrintFormatTarget& other);
    void operator=(const DOMPrintFormatTarget& rhs);
    strstreambuf* buf;
};

// ---------------------------------------------------------------------------
//  Local data
//
//  gEncodingName
//      The encoding we are to output in. If not set on the command line,
//      then it is defaults to the encoding of the input XML file.
//
// ---------------------------------------------------------------------------

static XMLCh*                   gEncodingName          = 0;
static XMLFormatter::UnRepFlags gUnRepFlags            = XMLFormatter::UnRep_CharRef;
static XMLFormatter*            gFormatter             = 0;

// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
ostream& operator<<(ostream& target, const DOMString& toWrite);
ostream& operator<<(ostream& target, DOM_Node& toWrite);
XMLFormatter& operator<< (XMLFormatter& strm, const DOMString& s);


XMLParser::XMLParser()	
{
}

XMLParser::~XMLParser()	
{
	if(parser!=NULL)
	{
		delete parser;
	}
}

void 
XMLParser::initialize(const char *XMLfile) 
	throw (STARCCM::Deployment::XMLInitializeFailed)
{
	
	//XMLPlatform initialize
  	try
    	{
        	XMLPlatformUtils::Initialize();
    	}
    	catch(const XMLException& toCatch)
    	{
        	TRACE1(LEVEL1,"XMLParser: Error during Xerces-c Initialization. Exception message: %s \n",DOMString(toCatch.getMessage()).transcode());
        	        	
        	throw STARCCM::Deployment::XMLInitializeFailed();
    	} 	

	//
	//get the path of the XMLfile
	//
	TRACE1(LEVEL6,"XMLparser: The  XML file is  %s \n",XMLfile);
#ifdef SOLARIS
	const char * fileName = strrchr(XMLfile,PATH_DELILIMITOR_CHAR);
#else
	char * fileName = strrchr(XMLfile,PATH_DELILIMITOR_CHAR);
#endif
	if(fileName == NULL)
	{
		TRACE1(LEVEL1,"XMLparser: invalid XML file of  %s \n",XMLfile);
		throw STARCCM::Deployment::XMLInitializeFailed(); 
	}
	
	int fileLen = strlen(XMLfile);
	int fileNameLen = strlen(fileName);	
	int filePathLen = fileLen-fileNameLen;
	
	//
	//construct the temp dtd file name
	//
	char *filePathTemp=(char *)malloc(filePathLen+1);
	strncpy(filePathTemp,XMLfile,filePathLen);
	filePathTemp[filePathLen]=NULL;			
	string  tempDTD = string(filePathTemp);
	tempDTD+=PATH_DELILIMITOR;
	tempDTD+="temp.dtd";
	TRACE1(LEVEL6,"XMLParser, the tempDTD is %s \n",tempDTD.c_str());
	
	//
	//create the temp dtd file for the entity resolver
	//
	FILE *stream = fopen(tempDTD.c_str(),"w+");
	if(stream == NULL)
	{
		TRACE1(LEVEL1,"XMLparser: can not open the temp dtd file of  %s \n",tempDTD.c_str());
		free(filePathTemp);
		throw STARCCM::Deployment::XMLInitializeFailed(); 
	}        
	fclose(stream);

	//
	//create the parser
	//
	parser = new DOMParser;	
    RedirectHandlers *myResolver = new RedirectHandlers(filePathTemp);
	free(filePathTemp);  
    parser->setEntityResolver(myResolver);	
	parser->setToCreateXMLDeclTypeNode(true);
		
	int errorCount;
	bool errorOccured=false;

	try
	{
	    //XMLfile need to be full path 
		parser->parse(XMLfile);
		
		#ifdef WIN32	
			if(!DeleteFile(tempDTD.c_str()))
			{
				TRACE0(LEVEL1,"XMLParser: delete the temp dtd file failed.");
				
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				
				throw STARCCM::Deployment::XMLInitializeFailed();				
			}
		#endif  		
		
	     errorCount = parser->getErrorCount();
	     if (errorCount > 0)
            	errorOccured = true;
            
            TRACE1(LEVEL6,"XMLParser: the errorcount is %d . \n",errorCount);	     
            
	     
	}
	catch (const XMLException& e)
	{
	     TRACE1(LEVEL1,"XMLparser: An error occured during parsing \n Message: %s \n",DOMString(e.getMessage()).transcode());
	     
	     errorOccured = true;	     
	}
	catch (const DOM_DOMException& e)
	{
		TRACE1(LEVEL1,"XMLParser: A DOM error occured during parsing\n   DOMException code: %s .\n",e.code);
	
	    	errorOccured = true;
	}
	catch (...)
	{
	        TRACE0(LEVEL1,"XMLParser: An error occured during parsing\n ");
	
		errorOccured = true;
	}
	
	if(errorOccured)
	{		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw STARCCM::Deployment::XMLInitializeFailed();
	}	
	
}
 
DOM_Node 
XMLParser::getRootNode(const char *rootName)
{
	
	doc = parser->getDocument();	
	DOM_Node nodeNULL;
	
	if(doc==NULL)	
	{
		TRACE0(LEVEL1,"XMLParser:Invalid xml file : document node is NULL. \n");		
		return nodeNULL;		
	}
	
	DOM_Node root = findChild(doc,rootName); 
	if(root==NULL)	
	{
		TRACE1(LEVEL1,"XMLParser:Invalid xml file: root %s node is NULL. \n",rootName);
		
		return nodeNULL;		
	}
	
	//
	//if the node is the DOCTYPE node with the same name with the root
	//then get its sibling ,which should be the root node
	//
	
	if((root.getNodeType())==10)
	{
		do
		{
			root=root.getNextSibling();
	
		}
		while((root!=NULL)&&!(((root.getNodeName()).equals(rootName))));
		
		if( root == NULL )
		{
			TRACE1(LEVEL1,"XMLParser:Invalid xml file: root %s node is NULL. \n",rootName);
		
			return nodeNULL;		
		}	
		
	}
	
	return root;
}

DOM_Node 
XMLParser::findChild(DOM_Node fatherNode,const char *childName)
{
	
	DOM_NodeList children=fatherNode.getChildNodes();
	unsigned int childrenLen=children.getLength();	
			
	DOM_Node childNode;	
	DOMString name;	
	DOM_Node nodeNULL;
														
	for(int i=0;i<childrenLen;i++)					
	{
		childNode=children.item(i);
		name=childNode.getNodeName();		

		if(name.equals(childName))
		{		
			return childNode;		
		}
	}
	
	return nodeNULL;
		
}

DOM_Node 
XMLParser::findChildByAttr(DOM_Node fatherNode,const char *childName,const char *attrName,const char * attrValue)
{
	
	DOM_NodeList children=fatherNode.getChildNodes();
	unsigned int childrenLen=children.getLength();					
					
	DOM_Node childNode;	
	DOMString name;
	DOM_Node nodeNULL;	
														
	for(int i=0;i<childrenLen;i++)					
	{
		childNode=children.item(i);
		name=childNode.getNodeName();
		
		if(name.equals(childName))
		{			
			DOMString value=getAttr(childNode,attrName);
			if((value!=NULL)&&(value.equals(attrValue)))
			{
				return childNode;
			}
		}
	}
	
	return nodeNULL;
		
}

DOMString 
XMLParser::getAttr(DOM_Node node,const char *attrName)
{
	DOM_NamedNodeMap attr=node.getAttributes();										
	DOM_Node attrNode=attr.getNamedItem(attrName);	
										
	if(attrNode!=NULL)
	{				
		DOMString attrValue=attrNode.getNodeValue();
		return attrValue;
	}
	
	return NULL;
	
}

DOMString 
XMLParser::parseFileInArchive(DOM_Node node)
{
		
	DOMString nodeName=node.getNodeName();	
	
	DOM_Node fileinarchiveNode=findChild(node,"fileinarchive");
	
	if(fileinarchiveNode==NULL)		
	{									
		TRACE1(LEVEL1,"XMLParser: %s element have no fileinarchive element\n",nodeName.transcode());								
				
		return NULL;	
					
	}
				
	DOMString location=getAttr(fileinarchiveNode,"name");								
								
	if(location==NULL)
	{									
		TRACE1(LEVEL1,"XMLParser: fileinarchive of %s have no name attribute\n",nodeName.transcode());
			
		return NULL;
	}
	
	return location;
}

int 
XMLParser::printToFile(const char * fileName)
{
	if (gEncodingName == 0)
        {
            DOMString encNameStr("UTF-8");
            DOM_Node aNode = doc.getFirstChild();
            if (aNode.getNodeType() == DOM_Node::XML_DECL_NODE)
            {
                DOMString aStr = ((DOM_XMLDecl &)aNode).getEncoding();
                if (aStr != "")
                {
                    encNameStr = aStr;
                }
            }
            unsigned int lent = encNameStr.length();
            gEncodingName = new XMLCh[lent + 1];
            XMLString::copyNString(gEncodingName, encNameStr.rawBuffer(), lent);
            gEncodingName[lent] = 0;
        }
    
    strstreambuf* buf = new strstreambuf();
	ostream out(buf);

	DOMPrintFormatTarget* formatTarget = new DOMPrintFormatTarget(buf);
	XMLFormatter::UnRepFlags gUnRepFlags  = XMLFormatter::UnRep_CharRef;
	gFormatter = new XMLFormatter(gEncodingName, formatTarget,
                                          XMLFormatter::NoEscapes, gUnRepFlags);
   	
   	out << doc;
   	
   	*gFormatter << chLF;
	
   	/* Flush stdout stream buffer so it goes to correct file */
   	out.flush();
    buf->freeze();
        
    FILE *outputfile;
    
   	if( ( outputfile = fopen( fileName, "w" ) ) == NULL )
   	{
      		TRACE0(LEVEL1,"XMLParser: Can't open the output file\n");      		
      		return -1;
   	}
   	
   	fwrite(buf->str(), 1, buf->pcount(), outputfile);
    
    fclose(outputfile);
    
	delete formatTarget;
	delete gFormatter;
	delete buf;
	return 0;
}

//int 
//XMLParser::printToFile1(const char * fileName)
//{
//	
//	if (gEncodingName == 0)
//        {
//            DOMString encNameStr("UTF-8");
//            DOM_Node aNode = doc.getFirstChild();
//            if (aNode.getNodeType() == DOM_Node::XML_DECL_NODE)
//            {
//                DOMString aStr = ((DOM_XMLDecl &)aNode).getEncoding();
//                if (aStr != "")
//                {
//                    encNameStr = aStr;
//                }
//            }
//            unsigned int lent = encNameStr.length();
//            gEncodingName = new XMLCh[lent + 1];
//            XMLString::copyNString(gEncodingName, encNameStr.rawBuffer(), lent);
//            gEncodingName[lent] = 0;
//        }
//
//
//	DOMPrintFormatTarget* formatTarget = new DOMPrintFormatTarget();
//	XMLFormatter::UnRepFlags gUnRepFlags  = XMLFormatter::UnRep_CharRef;
//	gFormatter = new XMLFormatter(gEncodingName, formatTarget,
//                                          XMLFormatter::NoEscapes, gUnRepFlags);
// 
//   	int old;
//   	FILE *outputfile;
//
//   	old = _dup( 1 );   /* "old" now refers to "stdout" */
//        	           /* Note:  file handle 1 == "stdout" */
//	if( old == -1 )
//	{
//		TRACE0(LEVEL1,"XMLParser: can not dup stdout\n");
//	     	return -1;
//	}
//   
//   	if( ( outputfile = fopen( fileName, "w" ) ) == NULL )
//   	{
//      		TRACE0(LEVEL1,"XMLParser: Can't open the output file\n");      		
//      		return -1;
//   	}
//	
//   	/* stdout now refers to file "data" */
//   	if( -1 == _dup2( _fileno( outputfile ), 1 ) )
//   	{
//      		TRACE0(LEVEL1,"XMLParser: Can't _dup2 stdout.\n");          		
//      		return -1;
//   	}     
// 
//   	cout << doc;
//    
//	*gFormatter << chLF;
//	
//   	/* Flush stdout stream buffer so it goes to correct file */
//   	fflush( stdout );
//   	fclose( outputfile );
//
//   	/* Restore original stdout */
//   	_dup2( old, 1 );	   	
//
//    	delete formatTarget;
//    	delete gFormatter;
//    	return 0;
//}

ostream& operator<<(ostream& target, DOM_Node& toWrite)
{
    // Get the name and value out for convenience
    DOMString   nodeName = toWrite.getNodeName();
    DOMString   nodeValue = toWrite.getNodeValue();
    unsigned long lent = nodeValue.length();

    switch (toWrite.getNodeType())
    {
        case DOM_Node::TEXT_NODE:
        {
            gFormatter->formatBuf(nodeValue.rawBuffer(),
                                  lent, XMLFormatter::CharEscapes);
            break;
        }


        case DOM_Node::PROCESSING_INSTRUCTION_NODE :
        {
            *gFormatter << XMLFormatter::NoEscapes << gStartPI  << nodeName;
            if (lent > 0)
            {
                *gFormatter << chSpace << nodeValue;
            }
            *gFormatter << XMLFormatter::NoEscapes << gEndPI;
            break;
        }


        case DOM_Node::DOCUMENT_NODE :
        {

            DOM_Node child = toWrite.getFirstChild();
            while( child != 0)
            {
                target << child;
                // add linefeed in requested output encoding
                *gFormatter << chLF;
                target << flush;
                child = child.getNextSibling();
            }
            break;
        }


        case DOM_Node::ELEMENT_NODE :
        {
            // The name has to be representable without any escapes
            *gFormatter  << XMLFormatter::NoEscapes
                         << chOpenAngle << nodeName;

            // Output the element start tag.

            // Output any attributes on this element
            DOM_NamedNodeMap attributes = toWrite.getAttributes();
            int attrCount = attributes.getLength();
            for (int i = 0; i < attrCount; i++)
            {
                DOM_Node  attribute = attributes.item(i);

                //
                //  Again the name has to be completely representable. But the
                //  attribute can have refs and requires the attribute style
                //  escaping.
                //
                *gFormatter  << XMLFormatter::NoEscapes
                             << chSpace << attribute.getNodeName()
                             << chEqual << chDoubleQuote
                             << XMLFormatter::AttrEscapes
                             << attribute.getNodeValue()
                             << XMLFormatter::NoEscapes
                             << chDoubleQuote;
            }

            //
            //  Test for the presence of children, which includes both
            //  text content and nested elements.
            //
            DOM_Node child = toWrite.getFirstChild();
            if (child != 0)
            {
                // There are children. Close start-tag, and output children.
                // No escapes are legal here
                *gFormatter << XMLFormatter::NoEscapes << chCloseAngle;

                while( child != 0)
                {
                    target << child;
                    child = child.getNextSibling();
                }

                //
                // Done with children.  Output the end tag.
                //
                *gFormatter << XMLFormatter::NoEscapes << gEndElement
                            << nodeName << chCloseAngle;
            }
            else
            {
                //
                //  There were no children. Output the short form close of
                //  the element start tag, making it an empty-element tag.
                //
                *gFormatter << XMLFormatter::NoEscapes << chForwardSlash << chCloseAngle;
            }
            break;
        }


        case DOM_Node::ENTITY_REFERENCE_NODE:
            {
                DOM_Node child;
#if 0
                for (child = toWrite.getFirstChild();
                child != 0;
                child = child.getNextSibling())
                {
                    target << child;
                }
#else
                //
                // Instead of printing the refernece tree
                // we'd output the actual text as it appeared in the xml file.
                // This would be the case when -e option was chosen
                //
                    *gFormatter << XMLFormatter::NoEscapes << chAmpersand
                        << nodeName << chSemiColon;
#endif
                break;
            }


        case DOM_Node::CDATA_SECTION_NODE:
            {
            *gFormatter << XMLFormatter::NoEscapes << gStartCDATA
                        << nodeValue << gEndCDATA;
            break;
        }


        case DOM_Node::COMMENT_NODE:
        {
            *gFormatter << XMLFormatter::NoEscapes << gStartComment
                        << nodeValue << gEndComment;
            break;
        }

 
        case DOM_Node::DOCUMENT_TYPE_NODE:
        {
            DOM_DocumentType doctype = (DOM_DocumentType &)toWrite;;

            *gFormatter << XMLFormatter::NoEscapes  << gStartDoctype
                        << nodeName;

            DOMString id = doctype.getPublicId();
            if (id != 0)
            {
                *gFormatter << XMLFormatter::NoEscapes << chSpace << gPublic
                    << id << chDoubleQuote;
                id = doctype.getSystemId();
                if (id != 0)
                {
                    *gFormatter << XMLFormatter::NoEscapes << chSpace
                       << chDoubleQuote << id << chDoubleQuote;
                }
            }
            else
            {
                id = doctype.getSystemId();
                if (id != 0)
                {
                    *gFormatter << XMLFormatter::NoEscapes << chSpace << gSystem
                        << id << chDoubleQuote;
                }
            }

            id = doctype.getInternalSubset();
            if (id !=0)
                *gFormatter << XMLFormatter::NoEscapes << chOpenSquare
                            << id << chCloseSquare;

            *gFormatter << XMLFormatter::NoEscapes << chCloseAngle;
            break;
        }


        case DOM_Node::ENTITY_NODE:
        {
            *gFormatter << XMLFormatter::NoEscapes << gStartEntity
                        << nodeName;

            DOMString id = ((DOM_Entity &)toWrite).getPublicId();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gPublic
                            << id << chDoubleQuote;

            id = ((DOM_Entity &)toWrite).getSystemId();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gSystem
                            << id << chDoubleQuote;

            id = ((DOM_Entity &)toWrite).getNotationName();
            if (id != 0)
                *gFormatter << XMLFormatter::NoEscapes << gNotation
                            << id << chDoubleQuote;

            *gFormatter << XMLFormatter::NoEscapes << chCloseAngle << chLF;

            break;
        }


        case DOM_Node::XML_DECL_NODE:
        {
            DOMString  str;

            *gFormatter << gXMLDecl1 << ((DOM_XMLDecl &)toWrite).getVersion();

            *gFormatter << gXMLDecl2 << gEncodingName;

            str = ((DOM_XMLDecl &)toWrite).getStandalone();
            if (str != 0)
                *gFormatter << gXMLDecl3 << str;

            *gFormatter << gXMLDecl4;

            break;
        }


        default:
            cerr << "Unrecognized node type = "
                 << (long)toWrite.getNodeType() << endl;
    }
    return target;
}



// ---------------------------------------------------------------------------
//  ostream << DOMString
//
//  Stream out a DOM string. Doing this requires that we first transcode
//  to char * form in the default code page for the system
// ---------------------------------------------------------------------------
ostream& operator<< (ostream& target, const DOMString& s)
{
    char *p = s.transcode();
    target << p;
    delete [] p;
    return target;
}


XMLFormatter& operator<< (XMLFormatter& strm, const DOMString& s)
{
    unsigned int lent = s.length();

	if (lent <= 0)
		return strm;

    XMLCh*  buf = new XMLCh[lent + 1];
    XMLString::copyNString(buf, s.rawBuffer(), lent);
    buf[lent] = 0;
    strm << buf;
    delete [] buf;
    return strm;
}

