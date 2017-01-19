/*
 * The Apache Software License, Version 1.1
 *
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
 *    permission, please contact apache@apache.org.
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
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.apache.org.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/**
 * $Id: GrammarResolver.hpp,v 1.1 2003/11/12 01:58:42 AnJingBin Exp $
 */

#if !defined(GRAMMARRESOLVER_HPP)
#define GRAMMARRESOLVER_HPP

#include <xercesc/util/RefHashTableOf.hpp>
#include <xercesc/util/StringPool.hpp>
#include <xercesc/validators/common/Grammar.hpp>


class DatatypeValidator;
class DatatypeValidatorFactory;


/**
 * This class embodies the representation of a Grammar pool Resolver.
 * This class is called from the validator.
 *
 */

class GrammarResolver {
public:

    /** @name Constructor and Destructor */
    //@{
    /**
     *
     * Default Constructor
     */
    GrammarResolver();

    /**
      * Destructor
      */
    ~GrammarResolver();

    //@}

    /** @name Getter methods */
    //@{
    /**
     * Retrieve the DatatypeValidatorFactory Registry
     * @return the DatatypeValidatorFactory Registry
     */
     DatatypeValidatorFactory* getDatatypeRegistry();

    /**
     * Retrieve the grammar that is associated with the specified namespace key
     *
     * @param  nameSpaceKey   Namespace key into Grammar pool
     * @return Grammar abstraction associated with the NameSpace key.
     */
    Grammar* getGrammar( const XMLCh* const nameSpaceKey ) ;

    /**
     * Get an enumeration of Grammar in the Grammar pool
     *
     * @return enumeration of Grammar in Grammar pool
     */
    RefHashTableOfEnumerator<Grammar> getGrammarEnumerator() const;


    /**
     * Get a string pool of schema grammar element/attribute names/prefixes
     * (used by TraverseSchema)
     *
     * @return a string pool of schema grammar element/attribute names/prefixes
     */
    XMLStringPool* getStringPool();

    /**
     * Is the specified Namespace key in Grammar pool?
     *
     * @param  nameSpaceKey    Namespace key
     * @return True if Namespace key association is in the Grammar pool.
     */
    bool containsNameSpace( const XMLCh* const nameSpaceKey );

    //@}

    /** @name GrammarResolver methods */
    //@{
    /**
     * Add the Grammar with Namespace Key associated to the Grammar Pool.
     * The Grammar will be owned by the Grammar Pool.
     *
     * @param  nameSpaceKey    Key to associate with Grammar abstraction
     * @param  grammarToAdopt  Grammar abstraction used by validator.
     */
    void putGrammar(const XMLCh* const nameSpaceKey, Grammar* const grammarToAdopt );

    /**
     * Remove the Grammar with Namespace Key associated from the Grammar Pool
     *
     * @param  nameSpaceKey    Key to associate with Grammar abstraction
     */
    void removeGrammar( const XMLCh* const nameSpaceKey );

    /**
     * Reset internal Namespace/Grammar registry.
     */
    void reset();

    //@}

private:
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fStringPool          The string pool used by TraverseSchema to store
    //                       element/attribute names and prefixes.
    //
    //  fGrammarRegistry     The Grammar Pool.  It represents a mapping
    //                       between Namespace and a Grammar
    //  fDataTypeReg         DatatypeValidatorFactory register
    //
    // -----------------------------------------------------------------------
    XMLStringPool                fStringPool;
    RefHashTableOf<Grammar>*     fGrammarRegistry;
    DatatypeValidatorFactory*    fDataTypeReg;
};

inline XMLStringPool* GrammarResolver::getStringPool() {

    return &fStringPool;
}

#endif
