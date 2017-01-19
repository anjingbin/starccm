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
 * $Id: ComplexTypeInfo.hpp,v 1.1 2004/07/13 02:37:43 WangKeBo Exp $
 */

#if !defined(COMPLEXTYPEINFO_HPP)
#define COMPLEXTYPEINFO_HPP


/**
  * The class act as a place holder to store complex type information.
  *
  * The class is intended for internal use.
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/RefHash2KeysTableOf.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/framework/XMLElementDecl.hpp>
#include <xercesc/framework/XMLContentModel.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>


// ---------------------------------------------------------------------------
//  Forward Declarations
// ---------------------------------------------------------------------------
class DatatypeValidator;
class ContentSpecNode;
class SchemaAttDefList;
class SchemaElementDecl;


class VALIDATORS_EXPORT ComplexTypeInfo
{
public:
    // -----------------------------------------------------------------------
    //  Public Constructors/Destructor
    // -----------------------------------------------------------------------
    ComplexTypeInfo();
    ~ComplexTypeInfo();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool                     getAbstract() const;
    bool                     getAdoptContentSpec() const;
    bool                     containsAttWithTypeId() const;
    bool                     getPreprocessed() const;
    int                      getDerivedBy() const;
    int                      getBlockSet() const;
    int                      getFinalSet() const;
    int                      getScopeDefined() const;
    unsigned int             getElementId() const;
    int                      getContentType() const;
    unsigned int             elementCount() const;
    XMLCh*                   getTypeName() const;
    DatatypeValidator*       getBaseDatatypeValidator() const;
    DatatypeValidator*       getDatatypeValidator() const;
    ComplexTypeInfo*         getBaseComplexTypeInfo() const;
    ContentSpecNode*         getContentSpec() const;
    const SchemaAttDef*      getAttWildCard() const;
    SchemaAttDef*            getAttWildCard();
    const SchemaAttDef*      getAttDef(const XMLCh* const baseName,
                                       const int uriId) const;
    SchemaAttDef*            getAttDef(const XMLCh* const baseName,
                                       const int uriId);
    XMLAttDefList&           getAttDefList() const;
    const SchemaElementDecl* elementAt(const unsigned int index) const;
    SchemaElementDecl*       elementAt(const unsigned int index);
    XMLContentModel*         getContentModel(const bool checkUPA = false);
    const XMLCh*             getFormattedContentModel ()   const;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setAbstract(const bool isAbstract);
    void setAdoptContentSpec(const bool toAdopt);
    void setAttWithTypeId(const bool value);
    void setPreprocessed(const bool aValue = true);
    void setDerivedBy(const int derivedBy);
    void setBlockSet(const int blockSet);
    void setFinalSet(const int finalSet);
    void setScopeDefined(const int scopeDefined);
    void setElementId(const unsigned int elemId);
    void setTypeName(const XMLCh* const typeName);
    void setContentType(const int contentType);
    void setBaseDatatypeValidator(DatatypeValidator* const baseValidator);
    void setDatatypeValidator(DatatypeValidator* const validator);
    void setBaseComplexTypeInfo(ComplexTypeInfo* const typeInfo);
    void setContentSpec(ContentSpecNode* const toAdopt);
    void setAttWildCard(SchemaAttDef* const toAdopt);
    void addAttDef(SchemaAttDef* const toAdd);
    void addElement(SchemaElementDecl* const toAdd);
    void setContentModel(XMLContentModel* const newModelToAdopt);

    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
    bool hasAttDefs() const;
    bool contains(const XMLCh* const attName);
    XMLAttDef* findAttr
    (
        const   XMLCh* const    qName
        , const unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefix
        , const XMLElementDecl::LookupOpts      options
        ,       bool&           wasAdded
    )   const;
    bool resetDefs();
    void checkUniqueParticleAttribution
    (
        SchemaGrammar*    const pGrammar
      , GrammarResolver*  const pGrammarResolver
      , XMLStringPool*    const pStringPool
      , XMLValidator*     const pValidator
    ) ;

private:
    // -----------------------------------------------------------------------
    //  Unimplemented contstructors and operators
    // -----------------------------------------------------------------------
    ComplexTypeInfo(const ComplexTypeInfo& elemInfo);
    ComplexTypeInfo& operator= (const ComplexTypeInfo& other);

    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void faultInAttDefList() const;
    XMLContentModel* createChildModel(ContentSpecNode* specNode, const bool isMixed);
    XMLContentModel* makeContentModel(const bool checkUPA = false, ContentSpecNode* const specNode = 0);
    XMLCh* formatContentModel () const ;
    ContentSpecNode* expandContentModel(ContentSpecNode* const curNode, const int minOccurs, const int maxOccurs);
    ContentSpecNode* convertContentSpecTree(ContentSpecNode* const curNode, const bool checkUPA = false);
    void resizeContentSpecOrgURI();

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool                               fAbstract;
    bool                               fAdoptContentSpec;
    bool                               fAttWithTypeId;
    bool                               fPreprocessed;
    int                                fDerivedBy;
    int                                fBlockSet;
    int                                fFinalSet;
    int                                fScopeDefined;
    unsigned int                       fElementId;
    int                                fContentType;
    XMLCh*                             fTypeName;
    DatatypeValidator*                 fBaseDatatypeValidator;
    DatatypeValidator*                 fDatatypeValidator;
    ComplexTypeInfo*                   fBaseComplexTypeInfo;
    ContentSpecNode*                   fContentSpec;
    SchemaAttDef*                      fAttWildCard;
    RefHash2KeysTableOf<SchemaAttDef>* fAttDefs;
    SchemaAttDefList*                  fAttList;
    RefVectorOf<SchemaElementDecl>*    fElements;
    XMLContentModel*                   fContentModel;
    XMLCh*                             fFormattedModel;
    unsigned int*                      fContentSpecOrgURI;
    unsigned int                       fUniqueURI;
    unsigned int                       fContentSpecOrgURISize;
    RefVectorOf<ContentSpecNode>*      fSpecNodesToDelete;
};

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Getter methods
// ---------------------------------------------------------------------------
inline bool ComplexTypeInfo::getAbstract() const {

    return fAbstract;
}

inline bool ComplexTypeInfo::getAdoptContentSpec() const {

    return fAdoptContentSpec;
}

inline bool ComplexTypeInfo::containsAttWithTypeId() const {

    return fAttWithTypeId;
}

inline bool ComplexTypeInfo::getPreprocessed() const {

    return fPreprocessed;
}

inline int ComplexTypeInfo::getDerivedBy() const {

    return fDerivedBy;
}

inline int ComplexTypeInfo::getBlockSet() const {

    return fBlockSet;
}

inline int ComplexTypeInfo::getFinalSet() const {

    return fFinalSet;
}

inline int ComplexTypeInfo::getScopeDefined() const {

    return fScopeDefined;
}

inline unsigned int ComplexTypeInfo::getElementId() const {

    return fElementId;
}

inline int ComplexTypeInfo::getContentType() const {

    return fContentType;
}

inline unsigned int ComplexTypeInfo::elementCount() const {

    if (fElements) {
        return fElements->size();
    }

    return 0;
}

inline XMLCh* ComplexTypeInfo::getTypeName() const {

    return fTypeName;
}

inline DatatypeValidator* ComplexTypeInfo::getBaseDatatypeValidator() const {

    return fBaseDatatypeValidator;
}

inline DatatypeValidator* ComplexTypeInfo::getDatatypeValidator() const {

    return fDatatypeValidator;
}

inline ComplexTypeInfo* ComplexTypeInfo::getBaseComplexTypeInfo() const {

    return fBaseComplexTypeInfo;
}

inline ContentSpecNode* ComplexTypeInfo::getContentSpec() const {

    return fContentSpec;
}

inline const SchemaAttDef* ComplexTypeInfo::getAttWildCard() const {

    return fAttWildCard;
}

inline SchemaAttDef* ComplexTypeInfo::getAttWildCard() {

    return fAttWildCard;
}

inline const SchemaAttDef* ComplexTypeInfo::getAttDef(const XMLCh* const baseName,
                                                      const int uriId) const {

    // If no list, then return a null
    if (!fAttDefs)
        return 0;

    return fAttDefs->get(baseName, uriId);
}

inline SchemaAttDef* ComplexTypeInfo::getAttDef(const XMLCh* const baseName,
                                                const int uriId)
{
    // If no list, then return a null
    if (!fAttDefs)
        return 0;

    return fAttDefs->get(baseName, uriId);
}

inline SchemaElementDecl*
ComplexTypeInfo::elementAt(const unsigned int index) {

    if (!fElements) {
        return 0; // REVISIT - need to throw an exception
    }

    return fElements->elementAt(index);
}

inline const SchemaElementDecl*
ComplexTypeInfo::elementAt(const unsigned int index) const {

    if (!fElements) {
        return 0; // REVISIT - need to throw an exception
    }

    return fElements->elementAt(index);
}

inline XMLContentModel* ComplexTypeInfo::getContentModel(const bool checkUPA)
{
    if (!fContentModel)
        fContentModel = makeContentModel(checkUPA);

    return fContentModel;
}


// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Setter methods
// ---------------------------------------------------------------------------
inline void ComplexTypeInfo::setAbstract(const bool isAbstract) {

    fAbstract = isAbstract;
}

inline void ComplexTypeInfo::setAdoptContentSpec(const bool toAdopt) {

    fAdoptContentSpec = toAdopt;
}

inline void ComplexTypeInfo::setAttWithTypeId(const bool value) {

    fAttWithTypeId = value;
}

inline void ComplexTypeInfo::setPreprocessed(const bool aValue) {

    fPreprocessed = aValue;
}

inline void ComplexTypeInfo::setDerivedBy(const int derivedBy) {

    fDerivedBy = derivedBy;
}

inline void ComplexTypeInfo::setBlockSet(const int blockSet) {

    fBlockSet = blockSet;
}

inline void ComplexTypeInfo::setFinalSet(const int finalSet) {

    fFinalSet = finalSet;
}

inline void ComplexTypeInfo::setScopeDefined(const int scopeDefined) {

    fScopeDefined = scopeDefined;
}

inline void ComplexTypeInfo::setElementId(const unsigned int elemId) {

    fElementId = elemId;
}

inline void
ComplexTypeInfo::setContentType(const int contentType) {

    fContentType = contentType;
}

inline void ComplexTypeInfo::setTypeName(const XMLCh* const typeName) {

    if (fTypeName != 0) {
        delete [] fTypeName;
    }

    fTypeName = XMLString::replicate(typeName);
}

inline void
ComplexTypeInfo::setBaseDatatypeValidator(DatatypeValidator* const validator) {

    fBaseDatatypeValidator = validator;
}

inline void
ComplexTypeInfo::setDatatypeValidator(DatatypeValidator* const validator) {

    fDatatypeValidator = validator;
}

inline void
ComplexTypeInfo::setBaseComplexTypeInfo(ComplexTypeInfo* const typeInfo) {

    fBaseComplexTypeInfo = typeInfo;
}

inline void ComplexTypeInfo::addElement(SchemaElementDecl* const elem) {

    if (!fElements) {
        fElements = new RefVectorOf<SchemaElementDecl>(8, false);
    }

    fElements->addElement(elem);
}

inline void ComplexTypeInfo::setAttWildCard(SchemaAttDef* const toAdopt) {

    if (fAttWildCard) {
       delete fAttWildCard;
    }

    fAttWildCard = toAdopt;
}

inline void
ComplexTypeInfo::setContentModel(XMLContentModel* const newModelToAdopt)
{
    delete fContentModel;
    fContentModel = newModelToAdopt;
}


// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Helper methods
// ---------------------------------------------------------------------------
inline bool ComplexTypeInfo::hasAttDefs() const
{
    // If the collection hasn't been faulted in, then no att defs
    if (!fAttDefs)
        return false;

    return !fAttDefs->isEmpty();
}

inline bool ComplexTypeInfo::contains(const XMLCh* const attName) {

    if (!fAttDefs) {
        return false;
    }

    RefHash2KeysTableOfEnumerator<SchemaAttDef> enumDefs(fAttDefs);

    while (enumDefs.hasMoreElements()) {

        if (XMLString::compareString(attName,
                enumDefs.nextElement().getAttName()->getLocalPart()) == 0) {
            return true;
        }
    }

    return false;
}

#endif

/**
  * End of file ComplexTypeInfo.hpp
  */

