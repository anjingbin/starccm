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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id: Token.hpp,v 1.1 2003/11/12 01:57:57 AnJingBin Exp $
 */

#if !defined(TOKEN_HPP)
#define TOKEN_HPP

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/RuntimeException.hpp>

// ---------------------------------------------------------------------------
//  Forward Declaration
// ---------------------------------------------------------------------------
class RangeToken;
class TokenFactory;


class XMLUTIL_EXPORT Token {
public:
	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	Token(const unsigned short tokType);
	virtual ~Token();

	// -----------------------------------------------------------------------
    //  Public Constants
    // -----------------------------------------------------------------------
	// Token types
	enum {
		CHAR = 0,
		CONCAT = 1,
		UNION = 2,
		CLOSURE = 3,
		RANGE = 4,
		NRANGE = 5,
		PAREN = 6,
		EMPTY = 7,
		ANCHOR = 8,
		NONGREEDYCLOSURE = 9,
		STRING = 10,
		DOT = 11,
		BACKREFERENCE = 12,
		LOOKAHEAD = 20,
		NEGATIVELOOKAHEAD = 21,
		LOOKBEHIND = 22,
		NEGATIVELOOKBEHIND = 23,
		INDEPENDENT = 24,
		MODIFIERGROUP = 25,
		CONDITION = 26
	};

	static const XMLInt32		UTF16_MAX;
	static const unsigned short FC_CONTINUE;
	static const unsigned short FC_TERMINAL;
	static const unsigned short FC_ANY;

	// -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
	unsigned short       getTokenType() const;
	int                  getMinLength() const;
    int                  getMaxLength() const;
	virtual Token*       getChild(const int index) const;
	virtual int          size() const;
    virtual int          getMin() const;
    virtual int          getMax() const;
    virtual int          getNoParen() const;
	virtual int          getReferenceNo() const;
    virtual const XMLCh* getString() const;
	virtual XMLInt32     getChar() const;

	// -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
	void setTokenType(const unsigned short tokType);
	virtual void setMin(const int minVal);
	virtual void setMax(const int maxVal);

    // -----------------------------------------------------------------------
    //  Range manipulation methods
    // -----------------------------------------------------------------------
	virtual void addRange(const XMLInt32 start, const XMLInt32 end);
	virtual void mergeRanges(const Token *const tok);
	virtual void sortRanges();
	virtual void compactRanges();
	virtual void subtractRanges(RangeToken* const tok);
	virtual void intersectRanges(RangeToken* const tok);

	// -----------------------------------------------------------------------
    //  Putter methods
    // -----------------------------------------------------------------------
	virtual void addChild(Token* const child, TokenFactory* const tokFactory);

	// -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
	int analyzeFirstCharacter(RangeToken* const rangeTok, const int options,
                              TokenFactory* const tokFactory);
    Token* findFixedString(int options, int& outOptions);

private:
	// -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Token(const Token&);
    void operator=(const Token&);

    // -----------------------------------------------------------------------
    //  Private Helper methods
    // -----------------------------------------------------------------------
	bool isSet(const int options, const unsigned int flag);
    bool isShorterThan(Token* const tok);

	// -----------------------------------------------------------------------
    //  Private data members
	// -----------------------------------------------------------------------
	unsigned short fTokenType;
};


// ---------------------------------------------------------------------------
//  Token: getter methods
// ---------------------------------------------------------------------------
inline unsigned short Token::getTokenType() const {

	return fTokenType;
}

inline int Token::size() const {

	return 0;
}

inline Token* Token::getChild(const int index) const {

	return 0;
}

inline int Token::getMin() const {

    return -1;
}

inline int Token::getMax() const {

    return -1;
}

inline int Token::getReferenceNo() const {

    return 0;
}

inline int Token::getNoParen() const {

    return 0;
}

inline const XMLCh* Token::getString() const {

    return 0;
}

inline XMLInt32 Token::getChar() const {

    return -1;
}

// ---------------------------------------------------------------------------
//  Token: setter methods
// ---------------------------------------------------------------------------
inline void Token::setTokenType(const unsigned short tokType) {
	
	fTokenType = tokType;
}

inline void Token::setMax(const int maxVal) {
	// ClosureToken
}

inline void Token::setMin(const int minVal) {
	// ClosureToken
}

inline bool Token::isSet(const int options, const unsigned int flag) {

	return (options & flag) == flag;
}

// ---------------------------------------------------------------------------
//  Token: setter methods
// ---------------------------------------------------------------------------
inline void Token::addChild(Token* const child, TokenFactory* const tokFactory) {

    ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

// ---------------------------------------------------------------------------
//  Token: Range manipulation methods
// ---------------------------------------------------------------------------
inline void Token::addRange(const XMLInt32 start, const XMLInt32 end) {

    ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline void Token::mergeRanges(const Token *const tok) {

    ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline void Token::sortRanges() {

    ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline void Token::compactRanges() {

    ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline void Token::subtractRanges(RangeToken* const tok) {

    ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline void Token::intersectRanges(RangeToken* const tok) {

    ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

#endif

/**
  * End of file Token.hpp
  */

