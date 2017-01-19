/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * $Log: SAXParser.hpp,v $
 * Revision 1.1  2004/07/13 02:37:42  WangKeBo
 * Component implementation
 *
 * Revision 1.2  2002/02/20 18:17:01  tng
 * [Bug 5977] Warnings on generating apiDocs.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:07  peiyongz
 * sane_include
 *
 * Revision 1.22  2001/12/05 22:09:02  tng
 * Update documentation for setExternalSchemaLocation and setExternalNoNamespaceSchemaLocation.
 *
 * Revision 1.21  2001/11/20 18:51:44  tng
 * Schema: schemaLocation and noNamespaceSchemaLocation to be specified outside the instance document.  New methods setExternalSchemaLocation and setExternalNoNamespaceSchemaLocation are added (for SAX2, two new properties are added).
 *
 * Revision 1.20  2001/08/01 19:11:02  tng
 * Add full schema constraint checking flag to the samples and the parser.
 *
 * Revision 1.19  2001/07/27 20:24:21  tng
 * put getScanner() back as they were there before, not to break existing apps.
 *
 * Revision 1.18  2001/07/16 12:52:09  tng
 * APIDocs fix: default for schema processing in DOMParser, IDOMParser, and SAXParser should be false.
 *
 * Revision 1.17  2001/06/23 14:13:16  tng
 * Remove getScanner from the Parser headers as this is not needed and Scanner is not internal class.
 *
 * Revision 1.16  2001/06/03 19:26:20  jberry
 * Add support for querying error count following parse; enables simple parse without requiring error handler.
 *
 * Revision 1.15  2001/05/11 13:26:22  tng
 * Copyright update.
 *
 * Revision 1.14  2001/05/03 19:09:25  knoaman
 * Support Warning/Error/FatalError messaging.
 * Validity constraints errors are treated as errors, with the ability by user to set
 * validity constraints as fatal errors.
 *
 * Revision 1.13  2001/03/30 16:46:57  tng
 * Schema: Use setDoSchema instead of setSchemaValidation which makes more sense.
 *
 * Revision 1.12  2001/03/21 21:56:09  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.11  2001/02/15 15:56:29  tng
 * Schema: Add setSchemaValidation and getSchemaValidation for DOMParser and SAXParser.
 * Add feature "http://apache.org/xml/features/validation/schema" for SAX2XMLReader.
 * New data field  fSchemaValidation in XMLScanner as the flag.
 *
 * Revision 1.10  2001/01/12 21:23:41  tng
 * Documentation Enhancement: explain values of Val_Scheme
 *
 * Revision 1.9  2000/08/02 18:05:15  jpolast
 * changes required for sax2
 * (changed private members to protected)
 *
 * Revision 1.8  2000/04/12 22:58:30  roddey
 * Added support for 'auto validate' mode.
 *
 * Revision 1.7  2000/03/03 01:29:34  roddey
 * Added a scanReset()/parseReset() method to the scanner and
 * parsers, to allow for reset after early exit from a progressive parse.
 * Added calls to new Terminate() call to all of the samples. Improved
 * documentation in SAX and DOM parsers.
 *
 * Revision 1.6  2000/02/17 03:54:27  rahulj
 * Added some new getters to query the parser state and
 * clarified the documentation.
 *
 * Revision 1.5  2000/02/16 03:42:58  rahulj
 * Finished documenting the SAX Driver implementation.
 *
 * Revision 1.4  2000/02/15 04:47:37  rahulj
 * Documenting the SAXParser framework. Not done yet.
 *
 * Revision 1.3  2000/02/06 07:47:56  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/15 19:57:48  roddey
 * Got rid of redundant 'const' on boolean return value. Some compilers choke
 * on this and its useless.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:51  twl
 * Initial checkin
 *
 * Revision 1.6  1999/11/08 20:44:54  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(SAXPARSER_HPP)
#define SAXPARSER_HPP

#include <xercesc/sax/Parser.hpp>
#include <xercesc/internal/VecAttrListImpl.hpp>
#include <xercesc/framework/XMLDocumentHandler.hpp>
#include <xercesc/framework/XMLElementDecl.hpp>
#include <xercesc/framework/XMLEntityHandler.hpp>
#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/validators/DTD/DocTypeHandler.hpp>

class DocumentHandler;
class EntityResolver;
class XMLPScanToken;
class XMLScanner;
class XMLValidator;


/**
  * This class implements the SAX 'Parser' interface and should be
  * used by applications wishing to parse the XML files using SAX.
  * It allows the client program to install SAX handlers for event
  * callbacks.
  *
  * <p>It can be used to instantiate a validating or non-validating
  * parser, by setting a member flag.</p>
  */

class PARSERS_EXPORT SAXParser :

    public Parser
    , public XMLDocumentHandler
    , public XMLErrorReporter
    , public XMLEntityHandler
    , public DocTypeHandler
{
public :
    // -----------------------------------------------------------------------
    //  Class types
    // -----------------------------------------------------------------------
    enum ValSchemes
    {
        Val_Never
        , Val_Always
        , Val_Auto
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    /** @name Constructors and Destructor */
    //@{
    /** Constructor with an instance of validator class to use for
      * validation.
      * @param valToAdopt Pointer to the validator instance to use. The
      *                   parser is responsible for freeing the memory.
      */
    SAXParser(XMLValidator* const valToAdopt = 0);

    /**
      * Destructor
      */
    ~SAXParser();
    //@}


    /** @name Getter methods */
    //@{
    /**
      * This method returns the installed document handler. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed document handler object.
      */
    DocumentHandler* getDocumentHandler();

    /**
      * This method returns the installed document handler. Suitable
      * only for 'rvalue' usages.
      *
      * @return A const pointer to the installed document handler object.
      */
    const DocumentHandler* getDocumentHandler() const;

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed entity resolver object.
      */
    EntityResolver* getEntityResolver();

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'rvalue' usages.
      *
      * @return A const pointer to the installed entity resolver object.
      */
    const EntityResolver* getEntityResolver() const;

    /**
      * This method returns the installed error handler. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed error handler object.
      */
    ErrorHandler* getErrorHandler();

    /**
      * This method returns the installed error handler. Suitable
      * for 'rvalue' usages.
      *
      * @return A const pointer to the installed error handler object.
      */
    const ErrorHandler* getErrorHandler() const;

    /**
      * This method returns a reference to the underlying scanner object.
      * It allows read only access to data maintained in the scanner.
      *
      * @return A const reference to the underlying scanner object.
      */
    const XMLScanner& getScanner() const;

    /**
      * This method returns a reference to the parser's installed
      * validator.
      *
      * @return A const reference to the installed validator object.
      */
    const XMLValidator& getValidator() const;

    /**
      * This method returns an enumerated value that indicates the current
      * validation scheme set on this parser.
      *
      * @return The ValSchemes value current set on this parser.
      * @see #setValidationScheme
      */
    ValSchemes getValidationScheme() const;

    /** Get the 'do schema' flag
      *
      * This method returns the state of the parser's schema processing
      * flag.
      *
      * @return true, if the parser is currently configured to
      *         understand schema, false otherwise.
      *
      * @see #setDoSchema
      */
    bool getDoSchema() const;

    /** Get the 'full schema constraint checking' flag
      *
      * This method returns the state of the parser's full schema constraint
      * checking flag.
      *
      * @return true, if the parser is currently configured to
      *         have full schema constraint checking, false otherwise.
      *
      * @see #setValidationSchemaFullChecking
      */
    bool getValidationSchemaFullChecking() const;

    /** Get error count from the last parse operation.
      *
      * This method returns the error count from the last parse
      * operation. Note that this count is actually stored in the
      * scanner, so this method simply returns what the
      * scanner reports.
      *
      * @return number of errors encountered during the latest
      *			parse operation.
      */
    int getErrorCount() const;

    /**
      * This method returns the state of the parser's namespace
      * handling capability.
      *
      * @return true, if the parser is currently configured to
      *         understand namespaces, false otherwise.
      *
      * @see #setDoNamespaces
      */
    bool getDoNamespaces() const;

    /**
      * This method returns the state of the parser's
      * exit-on-First-Fatal-Error flag.
      *
      * @return true, if the parser is currently configured to
      *         exit on the first fatal error, false otherwise.
      *
      * @see #setExitOnFirstFatalError
      */
    bool getExitOnFirstFatalError() const;

    /**
      * This method returns the state of the parser's
      * validation-constraint-fatal flag.
      *
      * @return true, if the parser is currently configured to
      *         set validation constraint errors as fatal, false
      *         otherwise.
      *
      * @see #setValidationContraintFatal
      */
    bool getValidationConstraintFatal() const;

   /** Get the set of Namespace/SchemaLocation that is specified externaly.
      *
      * This method returns the list of Namespace/SchemaLocation that was
      * specified using setExternalSchemaLocation.
      *
      * The parser owns the returned string, and the memory allocated for
      * the returned string will be destroyed when the parser is deleted.
      *
      * To ensure assessiblity of the returned information after the parser
      * is deleted, callers need to copy and store the returned information
      * somewhere else.
      *
      * @return a pointer to the list of Namespace/SchemaLocation that was
      *         specified externally.  The pointer spans the same life-time as
      *         the parser.  A null pointer is returned if nothing
      *         was specified externally.
      *
      * @see #setExternalSchemaLocation(const XMLCh* const)
      */
    XMLCh* getExternalSchemaLocation() const;

   /** Get the noNamespace SchemaLocation that is specified externaly.
      *
      * This method returns the no target namespace XML Schema Location
      * that was specified using setExternalNoNamespaceSchemaLocation.
      *
      * The parser owns the returned string, and the memory allocated for
      * the returned string will be destroyed when the parser is deleted.
      *
      * To ensure assessiblity of the returned information after the parser
      * is deleted, callers need to copy and store the returned information
      * somewhere else.
      *
      * @return a pointer to the no target namespace Schema Location that was
      *         specified externally.  The pointer spans the same life-time as
      *         the parser.  A null pointer is returned if nothing
      *         was specified externally.
      *
      * @see #setExternalNoNamespaceSchemaLocation(const XMLCh* const)
      */
    XMLCh* getExternalNoNamespaceSchemaLocation() const;

    //@}


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //@{
    /**
      * This method allows users to enable or disable the parser's
      * namespace processing. When set to true, parser starts enforcing
      * all the constraints / rules specified by the NameSpace
      * specification.
      *
      * <p>The parser's default state is: false.</p>
      *
      * <p>This flag is ignored by the underlying scanner if the installed
      * validator indicates that namespace constraints should be
      * enforced.</p>
      *
      * @param newState The value specifying whether NameSpace rules should
      *                 be enforced or not.
      *
      * @see #getDoNamespaces
      */
    void setDoNamespaces(const bool newState);

    /**
      * This method allows users to set the validation scheme to be used
      * by this parser. The value is one of the ValSchemes enumerated values
      * defined by this class:
      *
      * <br>  Val_Never  - turn off validation
      * <br>  Val_Always - turn on validation
      * <br>  Val_Auto   - turn on validation if any internal/external
      *                  DTD subset have been seen
      *
      * <p>The parser's default state is: Val_Auto.</p>
      *
      * @param newScheme The new validation scheme to use.
      *
      * @see #getValidationScheme
      */
    void setValidationScheme(const ValSchemes newScheme);

    /** Set the 'schema support' flag
      *
      * This method allows users to enable or disable the parser's
      * schema processing. When set to false, parser will not process
      * any schema found.
      *
      * The parser's default state is: false.
      *
      * @param newState The value specifying whether schema support should
      *                 be enforced or not.
      *
      * @see #getDoSchema
      */
    void setDoSchema(const bool newState);

    /**
      * This method allows the user to turn full Schema constraint checking on/off.
      * Only takes effect if Schema validation is enabled.
      * If turned off, partial constraint checking is done.
      *
      * Full schema constraint checking includes those checking that may
      * be time-consuming or memory intensive. Currently, particle unique
      * attribution constraint checking and particle derivation resriction checking
      * are controlled by this option.
      *
      * The parser's default state is: false.
      *
      * @param schemaFullChecking True to turn on full schema constraint checking.
      *
      * @see #getValidationSchemaFullChecking
      */
    void setValidationSchemaFullChecking(const bool schemaFullChecking);

    /**
      * This method allows users to set the parser's behaviour when it
      * encounters the first fatal error. If set to true, the parser
      * will exit at the first fatal error. If false, then it will
      * report the error and continue processing.
      *
      * <p>The default value is 'true' and the parser exits on the
      * first fatal error.</p>
      *
      * @param newState The value specifying whether the parser should
      *                 continue or exit when it encounters the first
      *                 fatal error.
      *
      * @see #getExitOnFirstFatalError
      */
    void setExitOnFirstFatalError(const bool newState);

    /**
      * This method allows users to set the parser's behaviour when it
      * encounters a validtion constraint error. If set to true, and the
      * the parser is set to exit when it encounter the first fatal error,
      * the parser will exit at the first encounter. If false, then it will
      * report the error and continue processing.
      *
      * <p>The default value is 'false'.</p>
      *
      * @param newState The value specifying whether the parser should
      *                 continue or exit when it encounters a validation
      *                 constraint error.
      *
      * @see #getValidationConstraintFatal
      */
    void setValidationConstraintFatal(const bool newState);

    /**
      * This method allows the user to specify a list of schemas to use.
      * If the targetNamespace of a schema specified using this method matches
      * the targetNamespace of a schema occuring in the instance document in
      * the schemaLocation attribute, or if the targetNamespace matches the
      * namespace attribute of the "import" element, the schema specified by the
      * user using this method will be used (i.e., the schemaLocation attribute
      * in the instance document or on the "import" element will be effectively ignored).
      *
      * If this method is called more than once, only the last one takes effect.
      *
      * The syntax is the same as for schemaLocation attributes in instance
      * documents: e.g, "http://www.example.com file_name.xsd". The user can
      * specify more than one XML Schema in the list.
      *
      * @param schemaLocation the list of schemas to use
      *
      * @see #getExternalSchemaLocation
      */

    void setExternalSchemaLocation(const XMLCh* const schemaLocation);

    /**
      * This method is same as setExternalSchemaLocation(const XMLCh* const).
      * It takes native char string as parameter
      *
      * @param schemaLocation the list of schemas to use
      *
      * @see #setExternalSchemaLocation(const XMLCh* const)
      */
    void setExternalSchemaLocation(const char* const schemaLocation);

    /**
      * This method allows the user to specify the no target namespace XML
      * Schema Location externally.  If specified, the instance document's
      * noNamespaceSchemaLocation attribute will be effectively ignored.
      *
      * If this method is called more than once, only the last one takes effect.
      *
      * The syntax is the same as for the noNamespaceSchemaLocation attribute
      * that may occur in an instance document: e.g."file_name.xsd".
      *
      * @param noNamespaceSchemaLocation the XML Schema Location with no target namespace
      *
      * @see #getExternalNoNamespaceSchemaLocation
      */
    void setExternalNoNamespaceSchemaLocation(const XMLCh* const noNamespaceSchemaLocation);

    /**
      * This method is same as setExternalNoNamespaceSchemaLocation(const XMLCh* const).
      * It takes native char string as parameter
      *
      * @param noNamespaceSchemaLocation the XML Schema Location with no target namespace
      *
      * @see #setExternalNoNamespaceSchemaLocation(const XMLCh* const)
      */
    void setExternalNoNamespaceSchemaLocation(const char* const noNamespaceSchemaLocation);

    //@}


    // -----------------------------------------------------------------------
    //  Advanced document handler list maintenance methods
    // -----------------------------------------------------------------------

    /** @name Advanced document handler list maintenance methods */
    //@{
    /**
      * This method installs the specified 'advanced' document callback
      * handler, thereby allowing the user to customize the processing,
      * if they choose to do so. Any number of advanced callback handlers
      * maybe installed.
      *
      * <p>The methods in the advanced callback interface represent
      * Xerces-C extensions. There is no specification for this interface.</p>
      *
      * @param toInstall A pointer to the users advanced callback handler.
      *
      * @see #removeAdvDocHandler
      */
    void installAdvDocHandler(XMLDocumentHandler* const toInstall);

    /**
      * This method removes the 'advanced' document handler callback from
      * the underlying parser scanner. If no handler is installed, advanced
      * callbacks are not invoked by the scanner.
      * @param toRemove A pointer to the advanced callback handler which
      *                 should be removed.
      *
      * @see #installAdvDocHandler
      */
    bool removeAdvDocHandler(XMLDocumentHandler* const toRemove);
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the SAXParser interface
    // -----------------------------------------------------------------------

    /** @name Implementation of SAX 1.0 interface's. */
    //@{
    /**
      * This method invokes the parsing process on the XML file specified
      * by the InputSource parameter.
      *
      * @param source A const reference to the InputSource object which
      *               points to the XML file to be parsed.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      *
      * @see Parser#parse(InputSource)
      */
    virtual void parse(const InputSource& source, const bool reuseGrammar = false);

    /**
      * This method invokes the parsing process on the XML file specified by
      * the Unicode string parameter 'systemId'.
      *
      * @param systemId A const XMLCh pointer to the Unicode string which
      *                 contains the path to the XML file to be parsed.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      *
      * @see Parser#parse(XMLCh*)
      */
    virtual void parse(const XMLCh* const systemId, const bool reuseGrammar = false);

    /**
      * This method invokes the parsing process on the XML file specified by
      * the native char* string parameter 'systemId'.
      *
      * @param systemId A const char pointer to a native string which
      *                 contains the path to the XML file to be parsed.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      */
    virtual void parse(const char* const systemId, const bool reuseGrammar = false);

    /**
      * This method installs the user specified SAX Document Handler
      * callback function on parser.
      *
      * @param handler A pointer to the document handler to be called
      *                when the parser comes across 'document' events
      *                as per the SAX specification.
      *
      * @see Parser#parse(char*)
      */
    virtual void setDocumentHandler(DocumentHandler* const handler);

    /**
      * This method installs the user specified DTD handler on the parser.
      *
      * @param handler A pointer to the DTD handler to be called
      *                when the parser comes across 'DTD' events
      *                as per the SAX specification.
      *
      * @see Parser#setDTDHandler
      */
    virtual void setDTDHandler(DTDHandler* const handler);

    /**
      * This method installs the user specified error handler on
      * the parser.
      *
      * @param handler A pointer to the error handler to be called
      *                when the parser comes across 'error' events
      *                as per the SAX specification.
      *
      * @see Parser#setErrorHandler
      */
    virtual void setErrorHandler(ErrorHandler* const handler);

    /**
      * This method installs the user specified entity resolver on the
      * parser. It allows applications to trap and redirect calls to
      * external entities.
      *
      * @param resolver A pointer to the entity resolver to be called
      *                 when the parser comes across references to
      *                 entities in the XML file.
      *
      * @see Parser#setEntityResolver
      */
    virtual void setEntityResolver(EntityResolver* const resolver);
    //@}


    // -----------------------------------------------------------------------
    //  Progressive scan methods
    // -----------------------------------------------------------------------

    /** @name Progressive scan methods */
    //@{

    /** Begin a progressive parse operation
      *
      * This method is used to start a progressive parse on a XML file.
      * To continue parsing, subsequent calls must be to the parseNext
      * method.
      *
      * It scans through the prolog and returns a token to be used on
      * subsequent scanNext() calls. If the return value is true, then the
      * token is legal and ready for further use. If it returns false, then
      * the scan of the prolog failed and the token is not going to work on
      * subsequent scanNext() calls.
      *
      * @param systemId A pointer to a Unicode string represting the path
      *                 to the XML file to be parsed.
      * @param toFill   A token maintaing state information to maintain
      *                 internal consistency between invocation of 'parseNext'
      *                 calls.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      *
      * @return 'true', if successful in parsing the prolog. It indicates the
      *         user can go ahead with parsing the rest of the file. It
      *         returns 'false' to indicate that the parser could parse the
      *         prolog (which means the token will not be valid.)
      *
      * @see #parseNext
      * @see #parseFirst(char*,...)
      * @see #parseFirst(InputSource&,...)
      */
    bool parseFirst
    (
        const   XMLCh* const    systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseGrammar = false
    );

    /** Begin a progressive parse operation
      *
      * This method is used to start a progressive parse on a XML file.
      * To continue parsing, subsequent calls must be to the parseNext
      * method.
      *
      * It scans through the prolog and returns a token to be used on
      * subsequent scanNext() calls. If the return value is true, then the
      * token is legal and ready for further use. If it returns false, then
      * the scan of the prolog failed and the token is not going to work on
      * subsequent scanNext() calls.
      *
      * @param systemId A pointer to a regular native string represting
      *                 the path to the XML file to be parsed.
      * @param toFill   A token maintaing state information to maintain
      *                 internal consIstency between invocation of 'parseNext'
      *                 calls.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      *
      * @return 'true', if successful in parsing the prolog. It indicates the
      *         user can go ahead with parsing the rest of the file. It
      *         returns 'false' to indicate that the parser could not parse
      *         the prolog.
      *
      * @see #parseNext
      * @see #parseFirst(XMLCh*,...)
      * @see #parseFirst(InputSource&,...)
      */
    bool parseFirst
    (
        const   char* const     systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseGrammar = false
    );

    /** Begin a progressive parse operation
      *
      * This method is used to start a progressive parse on a XML file.
      * To continue parsing, subsequent calls must be to the parseNext
      * method.
      *
      * It scans through the prolog and returns a token to be used on
      * subsequent scanNext() calls. If the return value is true, then the
      * token is legal and ready for further use. If it returns false, then
      * the scan of the prolog failed and the token is not going to work on
      * subsequent scanNext() calls.
      *
      * @param source   A const reference to the InputSource object which
      *                 points to the XML file to be parsed.
      * @param toFill   A token maintaing state information to maintain
      *                 internal consistency between invocation of 'parseNext'
      *                 calls.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      *
      * @return 'true', if successful in parsing the prolog. It indicates the
      *         user can go ahead with parsing the rest of the file. It
      *         returns 'false' to indicate that the parser could not parse
      *         the prolog.
      *
      * @see #parseNext
      * @see #parseFirst(XMLCh*,...)
      * @see #parseFirst(char*,...)
      */
    bool parseFirst
    (
        const   InputSource&    source
        ,       XMLPScanToken&  toFill
        , const bool            reuseGrammar = false
    );

    /** Continue a progressive parse operation
      *
      * This method is used to continue with progressive parsing of
      * XML files started by a call to 'parseFirst' method.
      *
      * It parses the XML file and stops as soon as it comes across
      * a XML token (as defined in the XML specification). Relevant
      * callback handlers are invoked as required by the SAX
      * specification.
      *
      * @param token A token maintaing state information to maintain
      *              internal consistency between invocation of 'parseNext'
      *              calls.
      *
      * @return 'true', if successful in parsing the next XML token.
      *         It indicates the user can go ahead with parsing the rest
      *         of the file. It returns 'false' to indicate that the parser
      *         could not find next token as per the XML specification
      *         production rule.
      *
      * @see #parseFirst(XMLCh*,...)
      * @see #parseFirst(char*,...)
      * @see #parseFirst(InputSource&,...)
      */
    bool parseNext(XMLPScanToken& token);

    /** Reset the parser after a progressive parse
      *
      * If a progressive parse loop exits before the end of the document
      * is reached, the parser has no way of knowing this. So it will leave
      * open any files or sockets or memory buffers that were in use at
      * the time that the parse loop exited.
      *
      * The next parse operation will cause these open files and such to
      * be closed, but the next parse operation might occur at some unknown
      * future point. To avoid this problem, you should reset the parser if
      * you exit the loop early.
      *
      * If you exited because of an error, then this cleanup will be done
      * for you. Its only when you exit the file prematurely of your own
      * accord, because you've found what you wanted in the file most
      * likely.
      *
      * @param token A token maintaing state information to maintain
      *              internal consistency between invocation of 'parseNext'
      *              calls.
      */
    void parseReset(XMLPScanToken& token);

    //@}



    // -----------------------------------------------------------------------
    //  Implementation of the DocTypeHandler Interface
    // -----------------------------------------------------------------------

    /** @name Implementation of the DocTypeHandler Interface */
    //@{
    /**
      * This method is used to report an attribute definition.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX
      * driver implementation.</font></b>
      *
      * @param elemDecl A const reference to the object containing information
      *                 about the element whose attribute definition was just
      *                 parsed.
      * @param attDef   A const reference to the object containing information
      *                 attribute definition.
      * @param ignore   The flag indicating whether this attribute definition
      *                 was ignored by the parser or not.
      */
    virtual void attDef
    (
        const   DTDElementDecl& elemDecl
        , const DTDAttDef&      attDef
        , const bool            ignore
    );

    /**
      * This method is used to report a comment occurring within the DTD.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param comment  A const pointer to a Unicode string representing the
      *                 text of the comment just parsed.
      */
    virtual void doctypeComment
    (
        const   XMLCh* const    comment
    );

    /**
      * This method is used to report the DOCTYPE declaration.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param elemDecl A const reference to the object containing information
      *                 about the root element definition declaration of the
      *                 XML document being parsed.
      * @param publicId A const pointer to a Unicode string representing the
      *                 public id of the DTD file.
      * @param systemId A const pointer to a Unicode string representing the
      *                 system id of the DTD file.
      * @param hasIntSubset A flag indicating if this XML file contains any
      *                     internal subset.
      */
    virtual void doctypeDecl
    (
        const   DTDElementDecl& elemDecl
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
        , const bool            hasIntSubset
    );

    /**
      * This method is used to report any PI declarations
      * occurring inside the DTD definition block.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param target A const pointer to a Unicode string representing the
      *               target of the PI declaration.
      * @param data   A const pointer to a Unicode string representing the
      *               data of the PI declaration. See the PI production rule
      *               in the XML specification for details.
      */
    virtual void doctypePI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    /**
      * This method is used to report any whitespaces
      * occurring inside the DTD definition block.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param chars  A const pointer to a Unicode string representing the
      *               whitespace characters.
      * @param length The length of the whitespace Unicode string.
      */
    virtual void doctypeWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    );

    /**
      * This method is used to report an element declarations
      * successfully scanned by the parser.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param decl   A const reference to the object containing element
      *               declaration information.
      * @param isIgnored The flag indicating whether this definition was
      *                  ignored by the parser or not.
      */
    virtual void elementDecl
    (
        const   DTDElementDecl& decl
        , const bool            isIgnored
    );

    /**
      * This method is used to report the end of an attribute
      * list declaration for an element.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param elemDecl A const reference to the object containing element
      *                 declaration information.
      */
    virtual void endAttList
    (
        const   DTDElementDecl& elemDecl
    );

    /**
      * This method is used to report the end of the internal subset.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void endIntSubset();

    /**
      * This method is used to report the end of the external subset.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void endExtSubset();

    /**
      * This method is used to report any entity declarations.
      * For unparsed entities, this driver will invoke the
      * SAX DTDHandler::unparsedEntityDecl callback.
      *
      * @param entityDecl A const reference to the object containing
      *                   the entity declaration information.
      * @param isPEDecl  The flag indicating whether this was a
      *                  parameter entity declaration or not.
      * @param isIgnored The flag indicating whether this definition
      *                  was ignored by the parser or not.
      *
      * @see DTDHandler#unparsedEntityDecl
      */
    virtual void entityDecl
    (
        const   DTDEntityDecl&  entityDecl
        , const bool            isPEDecl
        , const bool            isIgnored
    );

    /**
      * This method allows the user installed DTD handler to
      * reset itself.
      */
    virtual void resetDocType();

    /**
      * This method is used to report any notation declarations.
      * If there is a user installed DTDHandler, then the driver will
      * invoke the SAX DTDHandler::notationDecl callback.
      *
      * @param notDecl A const reference to the object containing the notation
      *                declaration information.
      * @param isIgnored The flag indicating whether this definition was ignored
      *                  by the parser or not.
      *
      * @see DTDHandler#notationDecl
      */
    virtual void notationDecl
    (
        const   XMLNotationDecl&    notDecl
        , const bool                isIgnored
    );

    /**
      * This method is used to indicate the start of an element's attribute
      * list declaration.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param elemDecl A const reference to the object containing element
      *                 declaration information.
      */
    virtual void startAttList
    (
        const   DTDElementDecl& elemDecl
    );

    /**
      * This method is used indicate the start of the internal subset.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void startIntSubset();

    /**
      * This method is used indicate the start of the external subset.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void startExtSubset();

    /**
      * This method is used to report the TextDecl. Refer to the XML
      * specification for the syntax of a TextDecl.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param versionStr A const pointer to a Unicode string representing
      *                   the version number of the 'version' clause.
      * @param encodingStr A const pointer to a Unicode string representing
      *                    the encoding name of the 'encoding' clause.
      */
    virtual void TextDecl
    (
        const   XMLCh* const    versionStr
        , const XMLCh* const    encodingStr
    );
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the XMLDocumentHandler interface
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLDocumentHandler Interface. */
    //@{
    /**
      * This method is used to report all the characters scanned
      * by the parser. The driver will invoke the 'characters'
      * method of the user installed SAX Document Handler.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'docCharacters' method will also be invoked.</p>
      *
      * @param chars   A const pointer to a Unicode string representing the
      *                character data.
      * @param length  The length of the Unicode string returned in 'chars'.
      * @param cdataSection  A flag indicating if the characters represent
      *                      content from the CDATA section.
      * @see DocumentHandler#characters
      */
    virtual void docCharacters
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    /**
      * This method is used to report any comments scanned by the parser.
      * This method is a no-op unless, unless an advanced callback handler
      * is installed, in which case the corresponding 'docComment' method
      * is invoked.
      *
      * @param comment A const pointer to a null terminated Unicode
      *                string representing the comment text.
      */
    virtual void docComment
    (
        const   XMLCh* const    comment
    );

    /**
      * This method is used to report any PI scanned by the parser.
      *
      * <p>Any PI's occurring before any 'content' are not reported
      * to any SAX handler as per the specification. However, all
      * PI's within content are reported via the SAX Document Handler's
      * 'processingInstruction' method.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'docPI' method will be invoked.</p>
      *
      * @param target A const pointer to a Unicode string representing the
      *               target of the PI declaration.
      * @param data   A const pointer to a Unicode string representing the
      *               data of the PI declaration. See the PI production rule
      *               in the XML specification for details.
      *
      * @see DocumentHandler#processingInstruction
      */
    virtual void docPI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    /**
      * This method is used to indicate the end of root element
      * was just scanned by the parser. Corresponding 'endDocument'
      * method of the user installed SAX Document Handler will also
      * be invoked.
      *
      * <p>In addition, if any advanced callback handlers are installed,
      * the corresponding 'endDocument' method is invoked.</p>
      *
      * @see DocumentHandler#endDocument
      */
    virtual void endDocument();

    /**
      * This method is used to indicate the end tag of an element.
      * The driver will invoke the corresponding 'endElement' method of
      * the SAX Document Handler interface.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'endElement' method is also invoked.</p>
      *
      * @param elemDecl A const reference to the object containing element
      *                 declaration information.
      * @param urlId    An id referring to the namespace prefix, if
      *                 namespaces setting is switched on.
      * @param isRoot   A flag indicating whether this element was the
      *                 root element.
      * @see DocumentHandler#endElement
      */
    virtual void endElement
    (
        const   XMLElementDecl& elemDecl
        , const unsigned int    urlId
        , const bool            isRoot
    );

    /**
      * This method is used to indicate that an end of an entity reference
      * was just scanned.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'endEnityReference' method is invoked.</p>
      *
      * @param entDecl A const reference to the object containing the
      *                entity declaration information.
      */
    virtual void endEntityReference
    (
        const   XMLEntityDecl&  entDecl
    );

    /**
      * This method is used to report all the whitespace characters,
      * which are determined to be 'ignorable'. This distinction
      * between characters is only made, if validation is enabled.
      * Corresponding 'ignorableWhitespace' method of the user installed
      * SAX Document Handler interface is called.
      *
      * <p>Any whitespace before content is not reported to the SAX
      * Document Handler method, as per the SAX specification.
      * However, if any advanced callback handlers are installed, the
      * corresponding 'ignorableWhitespace' method is invoked.</p>
      *
      * @param chars   A const pointer to a Unicode string representing the
      *                ignorable whitespace character data.
      * @param length  The length of the Unicode string 'chars'.
      * @param cdataSection  A flag indicating if the characters represent
      *                      content from the CDATA section.
      * @see DocumentHandler#ignorableWhitespace
      */
    virtual void ignorableWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    /**
      * This method allows the user installed Document Handler and
      * any advanced callback handlers to 'reset' themselves.
      */
    virtual void resetDocument();

    /**
      * This method is used to report the start of the parsing process.
      * The corresponding user installed SAX Document Handler's method
      * 'startDocument' is invoked.
      *
      * <p>If any advanced callback handlers are installed, then the
      * corresponding 'startDocument' method is also called.</p>
      *
      * @see DocumentHandler#startDocument
      */
    virtual void startDocument();

    /**
      * This method is used to report the start of an element. It is
      * called at the end of the element, by which time all attributes
      * specified are also parsed. The corresponding user installed
      * SAX Document Handler's method 'startElement' is invoked.
      *
      * <p>If any advanced callback handlers are installed, then the
      * corresponding 'startElement' method is also called.</p>
      *
      * @param elemDecl A const reference to the object containing element
      *                 declaration information.
      * @param urlId    An id referring to the namespace prefix, if
      *                 namespaces setting is switched on.
      * @param elemPrefix A const pointer to a Unicode string containing
      *                   the namespace prefix for this element. Applicable
      *                   only when namespace processing is enabled.
      * @param attrList  A const reference to the object containing the
      *                  list of attributes just scanned for this element.
      * @param attrCount A count of number of attributes in the list
      *                  specified by the parameter 'attrList'.
      * @param isEmpty  A flag indicating whether this is an empty element
      *                 or not.
      * @param isRoot   A flag indicating whether this element was the
      *                 root element.
      * @see DocumentHandler#startElement
      */
    virtual void startElement
    (
        const   XMLElementDecl&         elemDecl
        , const unsigned int            urlId
        , const XMLCh* const            elemPrefix
        , const RefVectorOf<XMLAttr>&   attrList
        , const unsigned int            attrCount
        , const bool                    isEmpty
        , const bool                    isRoot
    );

    /**
      * This method is used to indicate the start of an entity reference.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'endEnityReference' method is invoked.</p>
      *
      * @param entDecl A const reference to the object containing the
      *                entity declaration information.
      */
    virtual void startEntityReference
    (
        const   XMLEntityDecl&  entDecl
    );

    /**
      * This method is used to report the XML decl scanned by the parser.
      * Refer to the XML specification to see the meaning of parameters.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param versionStr A const pointer to a Unicode string representing
      *                   version string value.
      * @param encodingStr A const pointer to a Unicode string representing
      *                    the encoding string value.
      * @param standaloneStr A const pointer to a Unicode string
      *                      representing the standalone string value.
      * @param actualEncodingStr A const pointer to a Unicode string
      *                          representing the actual encoding string
      *                          value.
      */
    virtual void XMLDecl
    (
        const   XMLCh* const    versionStr
        , const XMLCh* const    encodingStr
        , const XMLCh* const    standaloneStr
        , const XMLCh* const    actualEncodingStr
    );
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLErrorReporter Interface. */
    //@{
    /**
      * This method is used to report back errors found while parsing the
      * XML file. The driver will call the corresponding user installed
      * SAX Error Handler methods: 'fatal', 'error', 'warning' depending
      * on the severity of the error. This classification is defined by
      * the XML specification.
      *
      * @param errCode An integer code for the error.
      * @param msgDomain A const pointer to an Unicode string representing
      *                  the message domain to use.
      * @param errType An enumeration classifying the severity of the error.
      * @param errorText A const pointer to an Unicode string representing
      *                  the text of the error message.
      * @param systemId  A const pointer to an Unicode string representing
      *                  the system id of the XML file where this error
      *                  was discovered.
      * @param publicId  A const pointer to an Unicode string representing
      *                  the public id of the XML file where this error
      *                  was discovered.
      * @param lineNum   The line number where the error occurred.
      * @param colNum    The column number where the error occurred.
      * @see ErrorHandler
      */
    virtual void error
    (
        const   unsigned int                errCode
        , const XMLCh* const                msgDomain
        , const XMLErrorReporter::ErrTypes  errType
        , const XMLCh* const                errorText
        , const XMLCh* const                systemId
        , const XMLCh* const                publicId
        , const unsigned int                lineNum
        , const unsigned int                colNum
    );

    /**
      * This method allows the user installed Error Handler
      * callback to 'reset' itself.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      */
    virtual void resetErrors();
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the XMLEntityHandler interface
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLEntityHandler Interface. */
    //@{
    /**
      * This method is used to indicate the end of parsing of an external
      * entity file.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param inputSource A const reference to the InputSource object
      *                    which points to the XML file being parsed.
      * @see InputSource
      */
    virtual void endInputSource(const InputSource& inputSource);

    /**
      * This method allows an installed XMLEntityHandler to further
      * process any system id's of enternal entities encountered in
      * the XML file being parsed, such as redirection etc.
      *
      * <b><font color="#FF0000">This method always returns 'false'
      * for this SAX driver implementation.</font></b>
      *
      * @param systemId  A const pointer to an Unicode string representing
      *                  the system id scanned by the parser.
      * @param toFill    A pointer to a buffer in which the application
      *                  processed system id is stored.
      * @return 'true', if any processing is done, 'false' otherwise.
      */
    virtual bool expandSystemId
    (
        const   XMLCh* const    systemId
        ,       XMLBuffer&      toFill
    );

    /**
      * This method allows the installed XMLEntityHandler to reset
      * itself.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void resetEntities();

    /**
      * This method allows a user installed entity handler to further
      * process any pointers to external entities. The applications
      * can implement 'redirection' via this callback. The driver
      * should call the SAX EntityHandler 'resolveEntity' method.
      *
      * @param publicId A const pointer to a Unicode string representing the
      *                 public id of the entity just parsed.
      * @param systemId A const pointer to a Unicode string representing the
      *                 system id of the entity just parsed.
      * @return The value returned by the SAX resolveEntity method or
      *         NULL otherwise to indicate no processing was done.
      * @see EntityResolver
      */
    virtual InputSource* resolveEntity
    (
        const   XMLCh* const    publicId
        , const XMLCh* const    systemId
    );

    /**
      * This method is used to indicate the start of parsing an
      * external entity file.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param inputSource A const reference to the InputSource object
      *                    which points to the external entity
      *                    being parsed.
      */
    virtual void startInputSource(const InputSource& inputSource);
    //@}


    /** @name Deprecated Methods */
    //@{
    /**
      * This method returns the state of the parser's validation
      * handling flag which controls whether validation checks
      * are enforced or not.
      *
      * @return true, if the parser is currently configured to
      *         do validation, false otherwise.
      *
      * @see #setDoValidation
      */
    bool getDoValidation() const;

    /**
      * This method allows users to enable or disable the parser's validation
      * checks.
      *
      * <p>By default, the parser does not to any validation. The default
      * value is false.</p>
      *
      * @param newState The value specifying whether the parser should
      *                 do validity checks or not against the DTD in the
      *                 input XML document.
      *
      * @see #getDoValidation
      */
    void setDoValidation(const bool newState);
    //@}


protected :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    SAXParser(const SAXParser&);
    void operator=(const SAXParser&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAttrList
    //      A temporary implementation of the basic SAX attribute list
    //      interface. We use this one over and over on each startElement
    //      event to allow SAX-like access to the element attributes.
    //
    //  fDocHandler
    //      The installed SAX doc handler, if any. Null if none.
    //
    //  fDTDHandler
    //      The installed SAX DTD handler, if any. Null if none.
    //
    //  fElemDepth
    //      This is used to track the element nesting depth, so that we can
    //      know when we are inside content. This is so we can ignore char
    //      data outside of content.
    //
    //  fEntityResolver
    //      The installed SAX entity handler, if any. Null if none.
    //
    //  fErrorHandler
    //      The installed SAX error handler, if any. Null if none.
    //
    //  fAdvDHCount
    //  fAdvDHList
    //  fAdvDHListSize
    //      This is an array of pointers to XMLDocumentHandlers, which is
    //      how we see installed advanced document handlers. There will
    //      usually not be very many at all, so a simple array is used
    //      instead of a collection, for performance. It will grow if needed,
    //      but that is unlikely.
    //
    //      The count is how many handlers are currently installed. The size
    //      is how big the array itself is (for expansion purposes.) When
    //      count == size, is time to expand.
    //
    //  fParseInProgress
    //      This flag is set once a parse starts. It is used to prevent
    //      multiple entrance or reentrance of the parser.
    //
    //  fScanner
    //      The scanner being used by this parser. It is created internally
    //      during construction.
    //
    // -----------------------------------------------------------------------
    VecAttrListImpl         fAttrList;
    DocumentHandler*        fDocHandler;
    DTDHandler*             fDTDHandler;
    unsigned int            fElemDepth;
    EntityResolver*         fEntityResolver;
    ErrorHandler*           fErrorHandler;
    unsigned int            fAdvDHCount;
    XMLDocumentHandler**    fAdvDHList;
    unsigned int            fAdvDHListSize;
    bool                    fParseInProgress;
    XMLScanner*             fScanner;
};


// ---------------------------------------------------------------------------
//  SAXParser: Getter methods
// ---------------------------------------------------------------------------
inline DocumentHandler* SAXParser::getDocumentHandler()
{
    return fDocHandler;
}

inline const DocumentHandler* SAXParser::getDocumentHandler() const
{
    return fDocHandler;
}

inline EntityResolver* SAXParser::getEntityResolver()
{
    return fEntityResolver;
}

inline const EntityResolver* SAXParser::getEntityResolver() const
{
    return fEntityResolver;
}

inline ErrorHandler* SAXParser::getErrorHandler()
{
    return fErrorHandler;
}

inline const ErrorHandler* SAXParser::getErrorHandler() const
{
    return fErrorHandler;
}

inline const XMLScanner& SAXParser::getScanner() const
{
    return *fScanner;
}

#endif
