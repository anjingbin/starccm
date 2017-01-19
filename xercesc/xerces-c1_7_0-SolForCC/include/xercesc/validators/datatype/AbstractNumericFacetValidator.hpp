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
 * $Id: AbstractNumericFacetValidator.hpp,v 1.1 2004/07/13 02:37:43 WangKeBo Exp $
 * $Log: AbstractNumericFacetValidator.hpp,v $
 * Revision 1.1  2004/07/13 02:37:43  WangKeBo
 * Component implementation
 *
 * Revision 1.2  2002/02/14 15:17:31  peiyongz
 * getEnumString()
 *
 * Revision 1.1.1.1  2002/02/01 22:22:39  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/11/22 20:23:20  peiyongz
 * _declspec(dllimport) and inline warning C4273
 *
 * Revision 1.2  2001/11/12 20:37:57  peiyongz
 * SchemaDateTimeException defined
 *
 * Revision 1.1  2001/10/01 16:13:56  peiyongz
 * DTV Reorganization:new classes: AbstractNumericFactValidator/ AbstractNumericValidator
 *
 */

#if !defined(ABSTRACT_NUMERIC_FACET_VALIDATOR_HPP)
#define ABSTRACT_NUMERIC_FACET_VALIDATOR_HPP

#include <xercesc/validators/datatype/DatatypeValidator.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/util/XMLNumber.hpp>

class VALIDATORS_EXPORT AbstractNumericFacetValidator : public DatatypeValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public ctor/dtor
    // -----------------------------------------------------------------------
	/** @name Constructor. */
    //@{

    virtual ~AbstractNumericFacetValidator();

	//@}

	virtual const RefVectorOf<XMLCh>* getEnumString() const;

protected:

    AbstractNumericFacetValidator(DatatypeValidator*            const baseValidator
                                , RefHashTableOf<KVStringPair>* const facets
                                , const int                           finalSet
                                , const ValidatorType                 type);

    void init(RefVectorOf<XMLCh>*  const enums);

    //
    // Abstract interface
    //
    virtual void assignAdditionalFacet(const XMLCh* const key
                                     , const XMLCh* const value) = 0;

    virtual void inheritAdditionalFacet() = 0;

    virtual void checkAdditionalFacetConstraints() const = 0;

    virtual void checkAdditionalFacetConstraintsBase() const = 0;

    virtual int  compareValues(const XMLNumber* const lValue
                             , const XMLNumber* const rValue) = 0;

    virtual void checkContent(const XMLCh* const content
                            , bool               asBase) = 0;

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------

    virtual void  setMaxInclusive(const XMLCh* const) = 0;

    virtual void  setMaxExclusive(const XMLCh* const) = 0;

    virtual void  setMinInclusive(const XMLCh* const) = 0;

    virtual void  setMinExclusive(const XMLCh* const) = 0;

    virtual void  setEnumeration() = 0;

    static const int INDETERMINATE;

// -----------------------------------------------------------------------
// Getter methods
// -----------------------------------------------------------------------

    inline XMLNumber* const            getMaxInclusive() const;

    inline XMLNumber* const            getMaxExclusive() const;

    inline XMLNumber* const            getMinInclusive() const;

    inline XMLNumber* const            getMinExclusive() const;

    inline RefVectorOf<XMLNumber>*     getEnumeration() const;

    // -----------------------------------------------------------------------
    //  Protected data members
    //
    //      Allow access to derived class
    // 
    // -----------------------------------------------------------------------
    bool                     fMaxInclusiveInherited;
    bool                     fMaxExclusiveInherited;
    bool                     fMinInclusiveInherited;
    bool                     fMinExclusiveInherited;
    bool                     fEnumerationInherited;

    XMLNumber*               fMaxInclusive;
    XMLNumber*               fMaxExclusive;
    XMLNumber*               fMinInclusive;
    XMLNumber*               fMinExclusive;

    RefVectorOf<XMLNumber>*  fEnumeration;    // save the actual value
    RefVectorOf<XMLCh>*      fStrEnumeration;

private:

    void assignFacet();

    void inspectFacet();

    void inspectFacetBase();

    void inheritFacet();
    
};

// -----------------------------------------------------------------------
// Getter methods
// -----------------------------------------------------------------------

inline XMLNumber* const AbstractNumericFacetValidator::getMaxInclusive() const
{
    return fMaxInclusive;
}

inline XMLNumber* const AbstractNumericFacetValidator::getMaxExclusive() const
{
    return fMaxExclusive;
}

inline XMLNumber* const AbstractNumericFacetValidator::getMinInclusive() const
{
    return fMinInclusive;
}

inline XMLNumber* const AbstractNumericFacetValidator::getMinExclusive() const
{
    return fMinExclusive;
}

inline RefVectorOf<XMLNumber>* AbstractNumericFacetValidator::getEnumeration() const
{
    return fEnumeration;
}

/**
  * End of file AbstractNumericFacetValidator.hpp
  */
#endif
