/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log: DOM_DOMException.hpp,v $
 * Revision 1.1  2003/11/12 01:57:59  AnJingBin
 * *** empty log message ***
 *
 * Revision 1.1  2003/10/23 20:57:50  AnJingBin
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2002/02/01 22:21:45  peiyongz
 * sane_include
 *
 * Revision 1.7  2000/03/02 19:53:54  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.6  2000/02/24 20:11:27  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.5  2000/02/10 19:52:08  abagchi
 * Added docs for enum
 *
 * Revision 1.4  2000/02/06 07:47:28  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/02/04 05:46:31  andyh
 * Change offsets and lengths form signed to unsigned
 *
 * Revision 1.2  2000/01/05 01:16:06  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:53  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:14  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#ifndef DOMException_HEADER_GUARD_
#define DOMException_HEADER_GUARD_

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOMString.hpp>

/**
  * Encapsulate a general DOM error or warning.
  *
  * <p> The DOM will create and throw an instance of DOMException
  * when an error condition is detected.  Exceptions can occur
  * when an application directly manipulates the DOM document
  * tree that is produced by the parser, or when a document tree
  * is created from scratch using the DOM API.  DOM exceptions will
  * not be generated by the parser while constructing a document
  * tree from an XML source document.
  *
  * <p>Unlike the other classes in the C++ DOM API, DOM_DOMException
  * is NOT a reference to an underlying implementation class, and
  * does not provide automatic memory management.  Code that catches
  * a DOM exception is responsible for deleting it, or otherwise
  * arranging for its disposal.
  *
  */
class CDOM_EXPORT DOM_DOMException  {
public:
    /** @name Enumerators for DOM Exceptions */
    //@{
        enum ExceptionCode {
                INDEX_SIZE_ERR       = 1,
                DOMSTRING_SIZE_ERR   = 2,
                HIERARCHY_REQUEST_ERR = 3,
                WRONG_DOCUMENT_ERR   = 4,
                INVALID_CHARACTER_ERR = 5,
                NO_DATA_ALLOWED_ERR  = 6,
                NO_MODIFICATION_ALLOWED_ERR = 7,
                NOT_FOUND_ERR        = 8,
                NOT_SUPPORTED_ERR    = 9,
                INUSE_ATTRIBUTE_ERR  = 10,
                INVALID_STATE_ERR    = 11,
	       	SYNTAX_ERR	     = 12,
        	INVALID_MODIFICATION_ERR    = 13,
        	NAMESPACE_ERR	     = 14,
        	INVALID_ACCESS_ERR   = 15
        };
    //@}
public:
    /** @name Constructors and assignment operator */
    //@{
    /**
      * Default constructor for DOM_DOMException.
      *
      */
    DOM_DOMException();

    /**
      * Constructor which takes an error code and a message.
      *
      * @param code The error code which indicates the exception
      * @param message The string containing the error message
      */
    DOM_DOMException(short code, const DOMString &message);

    /**
      * Copy constructor.
      *
      * @param other The object to be copied.
      */
    DOM_DOMException(const DOM_DOMException &other);

    //@}
    /** @name Destructor. */
    //@{
	 /**
	  * Destructor for DOM_DOMException.  Applications are responsible
      * for deleting DOM_Exception objects that they catch after they
      * have completed their exception processing.
	  *
	  */
    virtual ~DOM_DOMException();
    //@}

    /** @name Public variables. */
     //@{
	 /**
	  * A code value, from the set defined by the ExceptionCode enum,
      * indicating the type of error that occured.
	  */
   ExceptionCode   code;

	 /**
	  * A string value.  Applications may use this field to hold an error
      *  message.  The field value is not set by the DOM implementation,
      *  meaning that the string will be empty when an exception is first
      *  thrown.
	  */
    DOMString msg;
    //@}

};

#endif

