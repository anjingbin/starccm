/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * $Id: XMLUri.hpp,v 1.1 2003/11/12 01:57:56 AnJingBin Exp $
 * $Log: XMLUri.hpp,v $
 * Revision 1.1  2003/11/12 01:57:56  AnJingBin
 * *** empty log message ***
 *
 * Revision 1.1  2003/10/23 20:57:50  AnJingBin
 * *** empty log message ***
 *
 * Revision 1.2  2002/02/20 18:17:02  tng
 * [Bug 5977] Warnings on generating apiDocs.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:17  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/08/29 19:03:03  peiyongz
 * Bugzilla# 2816:on AIX 4.2, xlC 3 r ev.1, Compilation error on inline method
 *
 * Revision 1.2  2001/08/16 14:09:44  peiyongz
 * Removed unused ctors and methods
 *
 * Revision 1.1  2001/08/10 16:23:41  peiyongz
 * XMLUri: creation
 *
 *
 */

#if !defined(XMLURI_HPP)
#define XMLURI_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLString.hpp>

/*
 * This class is a direct port of Java's URI class, to distinguish
 * itself from the XMLURL, we use the name XMLUri instead of
 * XMLURI.
 *
 * TODO: how to relate XMLUri and XMLURL since URL is part of URI.
 *
 */

class XMLUTIL_EXPORT XMLUri
{
public:

    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    /**
     * Construct a new URI from a URI specification string.
     *
     * If the specification follows the "generic URI" syntax, (two slashes
     * following the first colon), the specification will be parsed
     * accordingly - setting the
     *                           scheme,
     *                           userinfo,
     *                           host,
     *                           port,
     *                           path,
     *                           querystring and
     *                           fragment
     * fields as necessary.
     *
     * If the specification does not follow the "generic URI" syntax,
     * the specification is parsed into a
     *                           scheme and
     *                           scheme-specific part (stored as the path) only.
     *
     * @param uriSpec the URI specification string (cannot be null or empty)
     *
     * ctor# 2
     *
     */
    XMLUri(const XMLCh* const    uriSpec);

    /**
     * Construct a new URI from a base URI and a URI specification string.
     * The URI specification string may be a relative URI.
     *
     * @param baseURI the base URI (cannot be null if uriSpec is null or
     *                empty)
     * @param uriSpec the URI specification string (cannot be null or
     *                empty if base is null)
     *
     * ctor# 7 relative ctor
     *
     */
    XMLUri(const XMLUri* const     baseURI
         , const XMLCh* const      uriSpec);

    virtual ~XMLUri();

    // -----------------------------------------------------------------------
    //  Operators
    // -----------------------------------------------------------------------


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------

    /**
     * Get the scheme for this URI.
     *
     * @return the scheme for this URI
     */
     const XMLCh* getScheme() const;

    /**
     * Get the userinfo for this URI.
     *
     * @return the userinfo for this URI (null if not specified).
     */
     const XMLCh* getUserInfo() const;


    /**
     * Get the host for this URI.
     *
     * @return the host for this URI (null if not specified).
     */
     const XMLCh* getHost() const;

    /**
     * Get the port for this URI.
     *
     * @return the port for this URI (-1 if not specified).
     */
     int getPort() const;

    /**
     * Get the path for this URI. Note that the value returned is the path
     * only and does not include the query string or fragment.
     *
     * @return the path for this URI.
     */
     const XMLCh* getPath() const;

    /**
     * Get the query string for this URI.
     *
     * @return the query string for this URI. Null is returned if there
     *         was no "?" in the URI spec, empty string if there was a
     *         "?" but no query string following it.
     */
     const XMLCh* getQueryString() const;

    /**
     * Get the fragment for this URI.
     *
     * @return the fragment for this URI. Null is returned if there
     *         was no "#" in the URI spec, empty string if there was a
     *         "#" but no fragment following it.
     */
     const XMLCh* getFragment() const;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    /**
     * Set the scheme for this URI. The scheme is converted to lowercase
     * before it is set.
     *
     * @param newScheme the scheme for this URI (cannot be null)
     *
     */
     void setScheme(const XMLCh* const newScheme);

    /**
     * Set the userinfo for this URI. If a non-null value is passed in and
     * the host value is null, then an exception is thrown.
     *
     * @param newUserInfo the userinfo for this URI
     *
     */
     void setUserInfo(const XMLCh* const newUserInfo);

    /**
     * Set the host for this URI. If null is passed in, the userinfo
     * field is also set to null and the port is set to -1.
     *
     * @param newHost the host for this URI
     *
     */
     void setHost(const XMLCh* const newHost);

    /**
     * Set the port for this URI. -1 is used to indicate that the port is
     * not specified, otherwise valid port numbers are  between 0 and 65535.
     * If a valid port number is passed in and the host field is null,
     * an exception is thrown.
     *
     * @param newPort the port number for this URI
     *
     */
     void setPort(int newPort);

    /**
     * Set the path for this URI.
     *
     * If the supplied path is null, then the
     * query string and fragment are set to null as well.
     *
     * If the supplied path includes a query string and/or fragment,
     * these fields will be parsed and set as well.
     *
     * Note:
     *
     * For URIs following the "generic URI" syntax, the path
     * specified should start with a slash.
     *
     * For URIs that do not follow the generic URI syntax, this method
     * sets the scheme-specific part.
     *
     * @param newPath the path for this URI (may be null)
     *
     */
     void setPath(const XMLCh* const newPath);

    /**
     * Set the query string for this URI. A non-null value is valid only
     * if this is an URI conforming to the generic URI syntax and
     * the path value is not null.
     *
     * @param newQueryString the query string for this URI
     *
     */
     void setQueryString(const XMLCh* const newQueryString);

    /**
     * Set the fragment for this URI. A non-null value is valid only
     * if this is a URI conforming to the generic URI syntax and
     * the path value is not null.
     *
     * @param newFragment the fragment for this URI
     *
     */
     void setFragment(const XMLCh* const newFragment);

     // -----------------------------------------------------------------------
    //  Miscellaneous methods
    // -----------------------------------------------------------------------

private:

    static const XMLCh RESERVED_CHARACTERS[];
    static const XMLCh MARK_CHARACTERS[];
    static const XMLCh SCHEME_CHARACTERS[];
    static const XMLCh USERINFO_CHARACTERS[];

    /**
     * Unimplemented copy ctor
     */
    XMLUri(const XMLUri& toCopy);
    XMLUri& operator=(const XMLUri& toAssign);

    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------

    /**
     * Determine whether a character is a reserved character:
     *
     * @return true if the string contains any reserved characters
     */
    static bool isReservedCharacter(const XMLCh theChar);

    /**
     * Determine whether a char is an unreserved character.
     *
     * @return true if the char is unreserved, false otherwise
     */
    static bool isUnreservedCharacter(const XMLCh theChar);

    /**
     * Determine whether a scheme conforms to the rules for a scheme name.
     * A scheme is conformant if it starts with an alphanumeric, and
     * contains only alphanumerics, '+','-' and '.'.
     *
     * @return true if the scheme is conformant, false otherwise
     */
    static bool isConformantSchemeName(const XMLCh* const scheme);

    /**
     * Determine whether a userInfo conforms to the rules for a userinfo.
     *
     * @return true if the scheme is conformant, false otherwise
     */
    static void isConformantUserInfo(const XMLCh* const userInfo);

    /**
     * Determine whether a given string contains only URI characters (also
     * called "uric" in RFC 2396). uric consist of all reserved
     * characters, unreserved characters and escaped characters.
     *
     * @return true if the string is comprised of uric, false otherwise
     */
    static bool isURIString(const XMLCh* const uric);

    /**
     * Determine whether a string is syntactically capable of representing
     * a valid IPv4 address or the domain name of a network host.
     *
     * A valid IPv4 address consists of four decimal digit groups
     * separated by a '.'.
     *
     * A hostname consists of domain labels (each of which must begin and
     * end with an alphanumeric but may contain '-') separated by a '.'.
     * See RFC 2396 Section 3.2.2.
     *
     * @return true if the string is a syntactically valid IPv4 address
     *              or hostname
     */
     static bool isWellFormedAddress(const XMLCh* const addr);

    /**
     * Get the indicator as to whether this URI uses the "generic URI"
     * syntax.
     *
     * @return true if this URI uses the "generic URI" syntax, false
     *         otherwise
     */
     bool isGenericURI();

    // -----------------------------------------------------------------------
    //  Miscellaneous methods
    // -----------------------------------------------------------------------

    /**
     * Initialize all fields of this URI from another URI.
     *
     * @param toCopy the URI to copy (cannot be null)
     */
     void initialize(const XMLUri& toCopy);

    /**
     * Initializes this URI from a base URI and a URI specification string.
     * See RFC 2396 Section 4 and Appendix B for specifications on parsing
     * the URI and Section 5 for specifications on resolving relative URIs
     * and relative paths.
     *
     * @param baseURI the base URI (may be null if uriSpec is an absolute
     *               URI)
     *
     * @param uriSpec the URI spec string which may be an absolute or
     *                  relative URI (can only be null/empty if base
     *                  is not null)
     *
     */
     void initialize(const XMLUri* const baseURI
                   , const XMLCh*  const uriSpec);

    /**
     * Initialize the scheme for this URI from a URI string spec.
     *
     * @param uriSpec the URI specification (cannot be null)
     *
     */
     void initializeScheme(const XMLCh* const uriSpec);

    /**
     * Initialize the authority (userinfo, host and port) for this
     * URI from a URI string spec.
     *
     * @param uriSpec the URI specification (cannot be null)
     *
     */
     void initializeAuthority(const XMLCh* const uriSpec);

    /**
     * Initialize the path for this URI from a URI string spec.
     *
     * @param uriSpec the URI specification (cannot be null)
     *
     */
     void initializePath(const XMLCh* const uriSpec);

    // -----------------------------------------------------------------------
    //  Data members
    //
    //  for all the data member, we own it,
    //  responsible for the creation and/or deletion for
    //  the memory allocated.
    //
    // -----------------------------------------------------------------------

    XMLCh*          fScheme;
    XMLCh*          fUserInfo;
    XMLCh*          fHost;
    int             fPort;
    XMLCh*          fPath;
    XMLCh*          fQueryString;
    XMLCh*          fFragment;

};

// ---------------------------------------------------------------------------
//  XMLUri: Public operators
// ---------------------------------------------------------------------------

// ctor# 2
inline XMLUri::XMLUri(const XMLCh* const uriSpec)
:fScheme(0)
,fUserInfo(0)
,fHost(0)
,fPort(-1)
,fPath(0)
,fQueryString(0)
,fFragment(0)
{
    initialize((XMLUri *)0, uriSpec);
}

// ctor# 7 relative ctor
inline XMLUri::XMLUri(const XMLUri* const      baseURI
                    , const XMLCh* const       uriSpec)
:fScheme(0)
,fUserInfo(0)
,fHost(0)
,fPort(-1)
,fPath(0)
,fQueryString(0)
,fFragment(0)
{
    initialize(baseURI, uriSpec);
}

// ---------------------------------------------------------------------------
//  XMLUri: Getter methods
// ---------------------------------------------------------------------------
inline const XMLCh* XMLUri::getScheme() const
{
    return fScheme;
}

inline const XMLCh* XMLUri::getUserInfo() const
{
	return fUserInfo;
}

inline const XMLCh* XMLUri::getHost() const
{
	return fHost;
}

inline int XMLUri::getPort() const
{
	return fPort;
}

inline const XMLCh* XMLUri::getPath() const
{
	return fPath;
}

inline const XMLCh* XMLUri::getQueryString() const
{
	return fQueryString;
}

inline const XMLCh* XMLUri::getFragment() const
{
	return fFragment;
}

// ---------------------------------------------------------------------------
//  XMLUri: Helper methods
// ---------------------------------------------------------------------------
inline bool XMLUri::isReservedCharacter(const XMLCh theChar)
{
    return (XMLString::indexOf(RESERVED_CHARACTERS, theChar) != -1);
}

inline bool XMLUri::isUnreservedCharacter(const XMLCh theChar)
{
    return (XMLString::isAlphaNum(theChar) ||
            XMLString::indexOf(MARK_CHARACTERS, theChar) != -1);
}

#endif
