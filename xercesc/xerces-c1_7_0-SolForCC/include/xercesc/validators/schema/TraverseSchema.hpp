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
 * $Id: TraverseSchema.hpp,v 1.1 2004/07/13 02:37:43 WangKeBo Exp $
 */

#if !defined(TRAVERSESCHEMA_HPP)
#define TRAVERSESCHEMA_HPP

/**
  * Instances of this class get delegated to Traverse the Schema and
  * to populate the SchemaGrammar internal representation.
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/idom/IDOM_Element.hpp>
#include <xercesc/idom/IDOM_Attr.hpp>
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/framework/XMLErrorCodes.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/util/RefHash2KeysTableOf.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/SchemaInfo.hpp>
#include <xercesc/validators/schema/GeneralAttributeCheck.hpp>

// ---------------------------------------------------------------------------
//  Forward Declarations
// ---------------------------------------------------------------------------
class GrammarResolver;
class EntityResolver;
class XMLValidator;
class XMLScanner;
class DatatypeValidator;
class DatatypeValidatorFactory;
class QName;
class ComplexTypeInfo;
class XMLAttDef;
class ContentSpecNode;
class NamespaceScope;
class SchemaAttDef;
class InputSource;
class ErrorHandler;
class XercesGroupInfo;
class XercesAttGroupInfo;
class IdentityConstraint;
class IDOMParser;


class VALIDATORS_EXPORT TraverseSchema
{
public:
    // -----------------------------------------------------------------------
    //  Public Constructors/Destructor
    // -----------------------------------------------------------------------
    TraverseSchema
    (
          IDOM_Element* const                schemaRoot
        , XMLStringPool* const               uriStringPool
        , SchemaGrammar* const               schemaGrammar
        , GrammarResolver* const             grammarResolver
        , XMLScanner* const                  xmlScanner
        , XMLValidator* const                xmlValidator
        , const XMLCh* const                 schemaURL
        , EntityResolver* const              entityResolver
        , ErrorHandler* const                errorHandler
    );

    ~TraverseSchema();

private:
   	// This enumeration is defined here for compatibility with the CodeWarrior
   	// compiler, which apparently doesn't like to accept default parameter
   	// arguments that it hasn't yet seen. The Not_All_Context argument is
   	// used in the declaration of checkMinMax, below.
   	//
    // Flags indicate any special restrictions on minOccurs and maxOccurs
    // relating to "all".
    //    Not_All_Context    - not processing an <all>
    //    All_Element        - processing an <element> in an <all>
    //    Group_Ref_With_All - processing <group> reference that contained <all>
    //    All_Group          - processing an <all> group itself
    enum
	{
        Not_All_Context = 0
        , All_Element = 1
        , Group_Ref_With_All = 2
        , All_Group = 4
    };

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    TraverseSchema(const TraverseSchema&);
    void operator=(const TraverseSchema&);

    // -----------------------------------------------------------------------
    //  Init/CleanUp methods
    // -----------------------------------------------------------------------
    void init();
    void cleanUp();

    // -----------------------------------------------------------------------
    //  Traversal methods
    // -----------------------------------------------------------------------
    /**
      * Traverse the Schema DOM tree
      */
    void                doTraverseSchema(const IDOM_Element* const schemaRoot);
    void                preprocessSchema(IDOM_Element* const schemaRoot,
                                         const XMLCh* const schemaURL);
    void                traverseSchemaHeader(const IDOM_Element* const schemaRoot);
    void                traverseAnnotationDecl(const IDOM_Element* const childElem);
    void                traverseInclude(const IDOM_Element* const childElem);
    void                traverseImport(const IDOM_Element* const childElem);
    void                traverseRedefine(const IDOM_Element* const childElem);
    void                traverseAttributeDecl(const IDOM_Element* const childElem,
                                              ComplexTypeInfo* const typeInfo);
    void                traverseSimpleContentDecl(const XMLCh* const typeName,
                                                  const IDOM_Element* const contentDecl,
                                                  ComplexTypeInfo* const typeInfo);
    void                traverseComplexContentDecl(const XMLCh* const typeName,
                                                  const IDOM_Element* const contentDecl,
                                                  ComplexTypeInfo* const typeInfo,
                                                  const bool isMixed);
    int                 traverseSimpleTypeDecl(const IDOM_Element* const childElem,
                                               int baseRefContext = SchemaSymbols::EMPTY_SET);
    int                 traverseComplexTypeDecl(const IDOM_Element* const childElem,
                                                const XMLCh* const recursingTypeName = 0);
    int                 traverseByList(const IDOM_Element* const rootElem,
                                       const IDOM_Element* const contentElem,
                                       const int typeNameIndex,
                                       const int finalSet);
    int                 traverseByRestriction(const IDOM_Element* const rootElem,
                                              const IDOM_Element* const contentElem,
                                              const int typeNameIndex,
                                              const int finalSet);
    int                 traverseByUnion(const IDOM_Element* const rootElem,
                                        const IDOM_Element* const contentElem,
                                        const int typeNameIndex,
                                        const int finalSet,
                                        int baseRefContext);
    QName*              traverseElementDecl(const IDOM_Element* const childElem, bool& toDelete);
    const XMLCh*        traverseNotationDecl(const IDOM_Element* const childElem);
    const XMLCh*        traverseNotationDecl(const XMLCh* const name,
                                             const XMLCh* const uriStr);
    ContentSpecNode*    traverseChoiceSequence(const IDOM_Element* const elemDecl,
                                               const int modelGroupType);
    ContentSpecNode*    traverseAny(const IDOM_Element* const anyDecl);
    ContentSpecNode*    traverseAll(const IDOM_Element* const allElem);
    XercesGroupInfo*    traverseGroupDecl(const IDOM_Element* const childElem,
                                          const bool circularAllowed = false);
    XercesAttGroupInfo* traverseAttributeGroupDecl(const IDOM_Element* const elem,
                                                   ComplexTypeInfo* const typeInfo);
    XercesAttGroupInfo* traverseAttributeGroupDeclNS(const XMLCh* const uriStr,
                                                     const XMLCh* const name);
    SchemaAttDef*       traverseAnyAttribute(const IDOM_Element* const elem);
    void                traverseKey(const IDOM_Element* const icElem,
                                    SchemaElementDecl* const elemDecl);
    void                traverseUnique(const IDOM_Element* const icElem,
                                       SchemaElementDecl* const elemDecl);
    void                traverseKeyRef(const IDOM_Element* const icElem,
                                       SchemaElementDecl* const elemDecl,
                                       const unsigned int namespaceDepth);
    bool                traverseIdentityConstraint(IdentityConstraint* const ic,
                                                   const IDOM_Element* const icElem);

    // -----------------------------------------------------------------------
    //  Error Reporting methods
    // -----------------------------------------------------------------------
    void reportSchemaError(const XMLCh* const msgDomain, const int errorCode);
    void reportSchemaError(const XMLCh* const msgDomain,
                           const int errorCode, 
                           const XMLCh* const text1,
                           const XMLCh* const text2 = 0,
                           const XMLCh* const text3 = 0,
                           const XMLCh* const text4 = 0);

    // -----------------------------------------------------------------------
    //  Private Helper methods
    // -----------------------------------------------------------------------
    /**
      * Retrived the Namespace mapping from the schema element
      */
    void retrieveNamespaceMapping(const IDOM_Element* const schemaRoot);

    /**
      * Loop through the children, and traverse the corresponding schema type
      * type declaration (simpleType, complexType, import, ....)
      */
    void processChildren(const IDOM_Element* const root);
    void preprocessChildren(const IDOM_Element* const root);

    void preprocessImport(const IDOM_Element* const elemNode);
    void preprocessInclude(const IDOM_Element* const elemNode);
    void preprocessRedefine(const IDOM_Element* const elemNode);

    /**
      * Parameters:
      *   rootElem - top element for a given type declaration
      *   contentElem - content must be annotation? or some other simple content
      *   isEmpty: - true if (annotation?, smth_else), false if (annotation?) 
      *
      * Check for Annotation if it is present, traverse it. If a sibling is
      * found and it is not an annotation return it, otherwise return 0.
      * Used by traverseSimpleTypeDecl.
      */
    IDOM_Element* checkContent(const IDOM_Element* const rootElem, 
                               IDOM_Element* const contentElem,
                               const bool isEmpty);

    /**
      * Parameters:
      *   contentElem - content element to check
      *
      * Check for identity constraints content.
      */
    const IDOM_Element* checkIdentityConstraintContent(const IDOM_Element* const contentElem);

    DatatypeValidator* getDatatypeValidator(const XMLCh* const uriStr,
                                            const XMLCh* const localPartStr);

    /**
      * Return qualified name of a given type name index from the string pool
      * If a target name space exist, it is returned as part of the name
      *
      * The allocation will be handled internally, and the caller should not
      * delete the returned pointer.
      */
    XMLCh* getQualifiedName(const int typeNameIndex);

    /**
      * Process simpleType content of a list|restriction|union
      * Return a dataype validator if valid type, otherwise 0.
      */
    DatatypeValidator* checkForSimpleTypeValidator(const IDOM_Element* const content,
                                                   int baseRefContext = SchemaSymbols::EMPTY_SET);

    /**
      * Process complexType content of an element
      * Return a ComplexTypeInfo if valid type, otherwise 0.
      */
    ComplexTypeInfo* checkForComplexTypeInfo(const IDOM_Element* const content);

    /**
      * Return DatatypeValidator available for the baseTypeStr.
      */
    DatatypeValidator* findDTValidator(const IDOM_Element* const rootElem,
                                       const XMLCh* const baseTypeStr,
                                       const int baseRefContext);

    const XMLCh* resolvePrefixToURI(const XMLCh* const prefix);
    const XMLCh* resolvePrefixToURI(const XMLCh* const prefix,
                                    const unsigned int namespaceDepth);

    /**
      * Return whether an element is defined as a top level schema component
      * or not.
      */
    bool isTopLevelComponent(const IDOM_Element* const elem);

    /**
      * Return the prefix for a given rawname string
      *
      * Function allocated, caller managed (facm) - pointer to be deleted by
      * caller.
      */
    const XMLCh* getPrefix(const XMLCh* const rawName);

    /**
      * Return the local for a given rawname string
      *
      * caller allocated, caller managed (cacm)
      */
    const XMLCh* getLocalPart(const XMLCh* const rawName);

    /**
      * Process a 'ref' of an Element declaration
      */
    QName* processElementDeclRef(const IDOM_Element* const elem,
                                 const XMLCh* const refName,
                                 bool& toDelete);

    /**
      * Process a 'ref' of an Attribute declaration
      */
    void processAttributeDeclRef(const IDOM_Element* const elem,
                                 ComplexTypeInfo* const typeInfo,
                                 const XMLCh* const refName,
                                 const XMLCh* const useVal,
                                 const XMLCh* const defaultVal,
                                 const XMLCh* const fixedVal);

    /**
      * Process a 'ref' on a group
      */
    XercesGroupInfo* processGroupRef(const IDOM_Element* const elem,
                                     const XMLCh* const refName,
                                     const bool circularAllowed);

    /**
      * Process a 'ref' on a attributeGroup
      */
    XercesAttGroupInfo* processAttributeGroupRef(const IDOM_Element* const elem,
                                                 const XMLCh* const refName,
                                                 ComplexTypeInfo* const typeInfo);

    /**
      * Parse block & final items
      */
    int parseBlockSet(const XMLCh* const blockStr, const int blockType);
    int parseFinalSet(const XMLCh* const finalStr, const int finalType);

    /**
      * Return true if a name is an identity constraint, otherwise false
      */
    bool isIdentityConstraintName(const XMLCh* const constraintName);

    /**
      * Check a 'ref' declaration representation constraint
      */
    bool isValidRefDeclaration(const IDOM_Element* const elem);

    /**
      * If 'typeStr' belongs to a different schema, return that schema URI,
      * otherwise return 0;
      */
    const XMLCh* checkTypeFromAnotherSchema(const XMLCh* const typeStr);

    /**
      * Return the datatype validator for a given element type attribute if
      * the type is a simple type
      */
    DatatypeValidator* getElementTypeValidator(const XMLCh* const typeStr,
                                               bool& noErrorDetected,
                                               const XMLCh* const otherSchemaURI);

    /**
      * Return the complexType info for a given element type attribute if
      * the type is a complex type
      */
    ComplexTypeInfo* getElementComplexTypeInfo(const XMLCh* const typeStr,
                                               bool& noErrorDetected,
                                               const XMLCh* const otherSchemaURI);

    /**
      * Return schema element declaration for a given substituteGroup element
      * name
      */
    SchemaElementDecl* getSubstituteGroupElemDecl(const XMLCh* const name,
                                                  bool& noErrorDetected);

    /**
      * Check validity constraint of a substitutionGroup attribute in
      * an element declaration
      */
    bool isSubstitutionGroupValid(const SchemaElementDecl* const elemDecl,
                                  const ComplexTypeInfo* const typeInfo,
                                  const DatatypeValidator* const validator,
                                  const XMLCh* const elemName,
                                  const bool toEmit = true);

    /**
      * Create a 'SchemaElementDecl' object and add it to SchemaGrammar
      */
    SchemaElementDecl* createSchemaElementDecl(const IDOM_Element* const elem,
                                               const bool topLevel,
                                               const unsigned short elemType,
                                               bool& isDuplicate,
                                               const bool isFixedValue);

    /**
      * Return the value of a given attribute name from an element node
      */
    const XMLCh* getElementAttValue(const IDOM_Element* const elem,
                                    const XMLCh* const attName,
                                    const bool toTrim = false);

    void checkMinMax(ContentSpecNode* const specNode,
                     const IDOM_Element* const elem,
                     const int allContext = Not_All_Context);

    /**
      * Process complex content for a complexType
      */
    void processComplexContent(const XMLCh* const typeName,
                               const IDOM_Element* const childElem,
                               ComplexTypeInfo* const typeInfo,
                               const XMLCh* const baseRawName,
                               const XMLCh* const baseLocalPart,
                               const XMLCh* const baseURI,
                               const bool isMixed,
                               const bool isBaseAnyType = false);

    /**
      * Process "base" information for a complexType
      */
    void processBaseTypeInfo(const XMLCh* const baseName,
                             const XMLCh* const localPart,
                             const XMLCh* const uriStr,
                             ComplexTypeInfo* const typeInfo);

    /**
      * Check if base is from another schema
      */
    bool isBaseFromAnotherSchema(const XMLCh* const baseURI);

    /**
      * Get complexType infp from another schema
      */
    ComplexTypeInfo* getTypeInfoFromNS(const XMLCh* const uriStr,
                                       const XMLCh* const localPart);

    /**
      * Returns true if 'name' is a valid facet for a given 'component',
      * otherwise false
      */
    bool isValidFacet(const XMLCh* const component, const XMLCh* const name);

    bool isAttrOrAttrGroup(const IDOM_Element* const elem);

    /**
      * Process attributes of a complex type
      */
    void processAttributes(const IDOM_Element* const elem,
                           const XMLCh* const baseRawName,
                           const XMLCh* const baseLocalPart,
                           const XMLCh* const baseURI,
                           ComplexTypeInfo* const typeInfo,
                           const bool isBaseAnyType = false);

    /**
      * Generate a name for an anonymous type
      */
    const XMLCh* genAnonTypeName(const XMLCh* const prefix);

    void defaultComplexTypeInfo(ComplexTypeInfo* const typeInfo);

    /**
      * Resolve a schema location attribute value to an input source.
      * Caller to delete the returned object.
      */
    InputSource* resolveSchemaLocation(const XMLCh* const loc);

    void restoreSchemaInfo(SchemaInfo* const toRestore,
                           SchemaInfo::ListType const aListType = SchemaInfo::INCLUDE);
    int  resetCurrentTypeNameStack(const int);

    /**
      * Check whether a mixed content is emptiable or not.
      * Needed to validate element constraint values (defualt, fixed)
      */
    bool emptiableParticle(const ContentSpecNode* const specNode);

    /**
      * Used by emptiableMixedContent to get the 'particle'
      * minimum/maximum total range. 
      */
    int getMinTotalRange(const ContentSpecNode* const specNode);
    int getMaxTotalRange(const ContentSpecNode* const specNode);

    void checkFixedFacet(const IDOM_Element* const, const XMLCh* const,
                         const DatatypeValidator* const, unsigned int&);
    void checkRefElementConsistency();
    void buildValidSubstitutionListF(SchemaElementDecl* const,
                                     SchemaElementDecl* const);
    void buildValidSubstitutionListB(SchemaElementDecl* const,
                                     SchemaElementDecl* const);

    void checkEnumerationRequiredNotation(const XMLCh* const name,
                                          const XMLCh* const typeStr);

    bool hasAllContent(const ContentSpecNode* const specNode);

    void processElements(ComplexTypeInfo* const baseTypeInfo,
                         ComplexTypeInfo* const newTypeInfo);

    void copyGroupElements(XercesGroupInfo* const fromGroup,
                           XercesGroupInfo* const toGroup,
                           ComplexTypeInfo* const typeInfo);

    void copyAttGroupAttributes(XercesAttGroupInfo* const fromAttGroup,
                                XercesAttGroupInfo* const toAttGroup,
                                ComplexTypeInfo* const typeInfo);

    const XMLCh* getTargetNamespaceString(const IDOM_Element* const elem);

    /**
      * Attribute wild card intersection.
      *
      * Note:
      *    The first parameter will be the result of the intersection, so 
      *    we need to make sure that first parameter is a copy of the
      *    actual attribute definition we need to intersect with.
      *
      *    What we need to wory about is: type, defaultType, namespace,
      *    and URI. All remaining data members should be the same.
      */
    void attWildCardIntersection(SchemaAttDef* const resultWildCart,
                                 const SchemaAttDef* const toCompareWildCard);

    /**
      * Attribute wild card union.
      *
      * Note:
      *    The first parameter will be the result of the union, so 
      *    we need to make sure that first parameter is a copy of the
      *    actual attribute definition we need to intersect with.
      *
      *    What we need to wory about is: type, defaultType, namespace,
      *    and URI. All remaining data members should be the same.
      */
    void attWildCardUnion(SchemaAttDef* const resultWildCart,
                          const SchemaAttDef* const toCompareWildCard);

    void copyWildCardData(const SchemaAttDef* const srcWildCard,
                          SchemaAttDef* const destWildCard);

    /**
      * Check that the attributes of a type derived by restriction satisfy
      * the constraints of derivation valid restriction
      */
    void checkAttDerivationOK(const ComplexTypeInfo* const baseTypeInfo,
                              const ComplexTypeInfo* const childTypeInfo);
    void checkAttDerivationOK(const XercesAttGroupInfo* const baseAttGrpInfo,
                              const XercesAttGroupInfo* const childAttGrpInfo);

    /**
      * Check whether a namespace value is valid with respect to wildcard
      * constraint
      */
    bool wildcardAllowsNamespace(const SchemaAttDef* const baseAttWildCard,
                                 const unsigned int nameURI);

    /**
      * Check whether a namespace constraint is an intensional subset of
      * another namespace constraint
      */
    bool isWildCardSubset(const SchemaAttDef* const baseAttWildCard,
                          const SchemaAttDef* const childAttWildCard);

    bool isAnyType(const XMLCh* const typeName);

    bool openRedefinedSchema(const IDOM_Element* const redefineElem);

    /**
      * The purpose of this method is twofold:
      * 1. To find and appropriately modify all information items
      * in redefinedSchema with names that are redefined by children of
      * redefineElem.
      * 2.  To make sure the redefine element represented by
      * redefineElem is valid as far as content goes and with regard to
      * properly referencing components to be redefined.
      *
      *	No traversing is done here!
      * This method also takes actions to find and, if necessary, modify
      * the names of elements in <redefine>'s in the schema that's being
      * redefined.
      */
    void renameRedefinedComponents(const IDOM_Element* const redefineElem,
                                   SchemaInfo* const redefiningSchemaInfo,
                                   SchemaInfo* const redefinedSchemaInfo);

    /**
      * This method returns true if the redefine component is valid, and if
      * it was possible to revise it correctly.
      */
    bool validateRedefineNameChange(const IDOM_Element* const redefineChildElem,
                                    const XMLCh* const redefineChildElemName,
                                    const XMLCh* const redefineChildDeclName,
                                    const int redefineNameCounter,
                                    SchemaInfo* const redefiningSchemaInfo);

	/**
      * This function looks among the children of 'redefineChildElem' for a 
      * component of type 'redefineChildComponentName'. If it finds one, it
      * evaluates whether its ref attribute contains a reference to
      * 'refChildTypeName'. If it does, it returns 1 + the value returned by
      * calls to itself on all other children.  In all other cases it returns
      * 0 plus the sum of the values returned by calls to itself on 
      * redefineChildElem's children. It also resets the value of ref so that
      * it will refer to the renamed type from the schema being redefined.
      */
    int changeRedefineGroup(const IDOM_Element* const redefineChildElem,
                            const XMLCh* const redefineChildComponentName,
                            const XMLCh* const redefineChildTypeName,
                            const int redefineNameCounter);

    /** This simple function looks for the first occurrence of a
      * 'redefineChildTypeName' item in the redefined schema and appropriately
      * changes the value of its name. If it turns out that what we're looking
      * for is in a <redefine> though, then we just rename it--and it's
      * reference--to be the same.
      */
    void fixRedefinedSchema(SchemaInfo* const redefinedSchemaInfo,
                            const XMLCh* const redefineChildComponentName,
                            const XMLCh* const redefineChildTypeName,
                            const int redefineNameCounter);

    void getRedefineNewTypeName(const XMLCh* const oldTypeName,
                                const int redefineCounter,
                                XMLBuffer& newTypeName);

    /**
      * This purpose of this method is threefold:
      * 1. To extract the schema information of included/redefined schema.
      * 2. Rename redefined components.
      * 3. Process components of included/redefined schemas
      */
    void preprocessRedefineInclude(SchemaInfo* const currSchemaInfo);

    /**
      * Update the list of valid substitution groups in the case of circular
      * import.
      */
    void updateCircularSubstitutionList(SchemaInfo* const aSchemaInfo);

    void processKeyRefFor(SchemaInfo* const aSchemaInfo,
                          ValueVectorOf<SchemaInfo*>* const infoList);

    // -----------------------------------------------------------------------
    //  Particle Derivation Checking methods
    // -----------------------------------------------------------------------
    void checkParticleDerivation();
    void checkParticleDerivationOk(ContentSpecNode* const curNode,
                                   const int derivedScope,
                                   ContentSpecNode* const baseNode,
                                   const int baseScope,
                                   const ComplexTypeInfo* const baseInfo = 0);
    ContentSpecNode* checkForPointlessOccurrences(ContentSpecNode* const specNode,
                                                  const ContentSpecNode::NodeTypes nodeType,
                                                  ValueVectorOf<ContentSpecNode*>* const nodes);
    void gatherChildren(const ContentSpecNode::NodeTypes parentNodeType,
                        ContentSpecNode* const specNode,
                        ValueVectorOf<ContentSpecNode*>* const nodes);
    bool isOccurrenceRangeOK(const int min1, const int max1, const int min2, const int max2);
    void checkNSCompat(const ContentSpecNode* const derivedSpecNode,
                       const ContentSpecNode* const baseSpecNode);
    bool wildcardEltAllowsNamespace(const ContentSpecNode* const baseSpecNode,
                                    const unsigned int derivedURI);
    void checkNameAndTypeOK(const ContentSpecNode* const derivedSpecNode,
                            const int derivedScope,
                            const ContentSpecNode* const baseSpecNode,
                            const int baseScope,
                            const ComplexTypeInfo* const baseInfo = 0);
    SchemaElementDecl* findElement(const int scope,
                                   const unsigned int uriIndex,
                                   const XMLCh* const name,
                                   SchemaGrammar* const grammar,
                                   const ComplexTypeInfo* const typeInfo = 0);
    void checkICRestriction(const SchemaElementDecl* const derivedElemDecl,
                            const SchemaElementDecl* const baseElemDecl,
                            const XMLCh* const derivedElemName,
                            const XMLCh* const baseElemName);
    void checkTypesOK(const SchemaElementDecl* const derivedElemDecl,
                      const SchemaElementDecl* const baseElemDecl,
                      const XMLCh* const derivedElemName);
    void checkRecurseAsIfGroup(ContentSpecNode* const derivedSpecNode,
                               const int derivedScope,
                               const ContentSpecNode* const baseSpecNode,
                               const int baseScope,
                               ValueVectorOf<ContentSpecNode*>* const nodes,
                               const ComplexTypeInfo* const baseInfo);
    void checkRecurse(const ContentSpecNode* const derivedSpecNode,
                      const int derivedScope,
                      ValueVectorOf<ContentSpecNode*>* const derivedNodes,
                      const ContentSpecNode* const baseSpecNode,
                      const int baseScope,
                      ValueVectorOf<ContentSpecNode*>* const baseNodes,
                      const ComplexTypeInfo* const baseInfo,
                      const bool toLax = false);
    void checkNSSubset(const ContentSpecNode* const derivedSpecNode,
                       const ContentSpecNode* const baseSpecNode);
    bool isWildCardEltSubset(const ContentSpecNode* const derivedSpecNode,
                             const ContentSpecNode* const baseSpecNode);
    void checkNSRecurseCheckCardinality(const ContentSpecNode* const derivedSpecNode,
                                        ValueVectorOf<ContentSpecNode*>* const derivedNodes,
                                        const int derivedScope,
                                        ContentSpecNode* const baseSpecNode);
    void checkRecurseUnordered(const ContentSpecNode* const derivedSpecNode,
                               ValueVectorOf<ContentSpecNode*>* const derivedNodes, 
                               const int derivedScope,
                               ContentSpecNode* const baseSpecNode, 
                               ValueVectorOf<ContentSpecNode*>* const baseNodes, 
                               const int baseScope,
                               const ComplexTypeInfo* const baseInfo);
    void checkMapAndSum(const ContentSpecNode* const derivedSpecNode,
                        ValueVectorOf<ContentSpecNode*>* const derivedNodes, 
                        const int derivedScope,
                        ContentSpecNode* const baseSpecNode, 
                        ValueVectorOf<ContentSpecNode*>* const baseNodes, 
                        const int baseScope,
                        const ComplexTypeInfo* const baseInfo);

    // -----------------------------------------------------------------------
    //  Private constants
    // -----------------------------------------------------------------------
    enum
    {
        ES_Block
        , C_Block
        , S_Final
        , EC_Final
        , ECS_Final
    };

    enum ExceptionCodes
    {
        NoException = 0,
        InvalidComplexTypeInfo = 1,
        RecursingElement = 2
    };

    enum
    {
        Elem_Def_Qualified = 1,
        Attr_Def_Qualified = 2
    };

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool                                          fFullConstraintChecking;
    unsigned short                                fElemAttrDefaultQualified;
    int                                           fTargetNSURI;
    int                                           fEmptyNamespaceURI;
    int                                           fCurrentScope;
    int                                           fFinalDefault;
    int                                           fBlockDefault;
    int                                           fScopeCount;
    int                                           fRecursingElemIndex;
    unsigned int                                  fAnonXSTypeCount;
    unsigned int                                  fCircularCheckIndex;
    const XMLCh*                                  fTargetNSURIString;
    DatatypeValidatorFactory*                     fDatatypeRegistry;
    GrammarResolver*                              fGrammarResolver;
    SchemaGrammar*                                fSchemaGrammar;
    EntityResolver*                               fEntityResolver;
    ErrorHandler*                                 fErrorHandler;
    XMLStringPool*                                fURIStringPool;
    XMLStringPool*                                fStringPool;
    XMLBuffer                                     fBuffer;
    XMLValidator*                                 fValidator;
    XMLScanner*                                   fScanner;
    NamespaceScope*                               fNamespaceScope;
    RefHashTableOf<XMLAttDef>*                    fAttributeDeclRegistry;
    RefHashTableOf<ComplexTypeInfo>*              fComplexTypeRegistry;
    RefHashTableOf<XercesGroupInfo>*              fGroupRegistry;
    RefHashTableOf<XercesAttGroupInfo>*           fAttGroupRegistry;
    RefHash2KeysTableOf<SchemaInfo>*              fSchemaInfoList;
    SchemaInfo*                                   fSchemaInfo;
    XercesGroupInfo*                              fCurrentGroupInfo;
    XercesAttGroupInfo*                           fCurrentAttGroupInfo;
    ComplexTypeInfo*                              fCurrentComplexType;
    ValueVectorOf<unsigned int>*                  fCurrentTypeNameStack;
    ValueVectorOf<unsigned int>*                  fCurrentGroupStack;
    ValueVectorOf<unsigned int>*                  fIC_NamespaceDepth;
    ValueVectorOf<SchemaElementDecl*>*            fIC_Elements;
    GeneralAttributeCheck                         fAttributeCheck;
    RefHash2KeysTableOf<XMLCh>*                   fGlobalDeclarations;
    RefHash2KeysTableOf<XMLCh>*                   fNotationRegistry;
    RefHash2KeysTableOf<XMLCh>*                   fRedefineComponents;
    RefHash2KeysTableOf<IdentityConstraint>*      fIdentityConstraintNames;
    RefHash2KeysTableOf<SchemaElementDecl>*       fSubstitutionGroups;
    RefHash2KeysTableOf<ElemVector>*              fValidSubstitutionGroups;
    RefVectorOf<QName>*                           fRefElements;
    ValueVectorOf<int>*                           fRefElemScope;
    RefHashTableOf<ValueVectorOf<IDOM_Element*> >*  fIC_NodeListNS;
    RefHashTableOf<ElemVector>*                   fIC_ElementsNS;
    RefHashTableOf<ValueVectorOf<unsigned int> >* fIC_NamespaceDepthNS;
    IDOMParser*                                   fParser;
    RefHashTableOf<SchemaInfo>*                   fPreprocessedNodes;
    ValueVectorOf<const IDOM_Element*>*           fRecursingAnonTypes;
    ValueVectorOf<const XMLCh*>*                  fRecursingTypeNames;

    friend class GeneralAttributeCheck;
};


// ---------------------------------------------------------------------------
//  TraverseSchema: Helper methods
// ---------------------------------------------------------------------------
inline const XMLCh* TraverseSchema::getPrefix(const XMLCh* const rawName) {

    int colonIndex = XMLString::indexOf(rawName, chColon);

    if (colonIndex == -1 || colonIndex == 0) {
        return XMLUni::fgZeroLenString;
    }

    fBuffer.set(rawName, colonIndex);

    return fStringPool->getValueForId(fStringPool->addOrFind(fBuffer.getRawBuffer()));
}

inline const XMLCh* TraverseSchema::getLocalPart(const XMLCh* const rawName) {

    int    colonIndex = XMLString::indexOf(rawName, chColon);
    int    rawNameLen = XMLString::stringLen(rawName);

    if (colonIndex + 1 == rawNameLen) {
        return XMLUni::fgZeroLenString;
    }

    if (colonIndex == -1) {
        fBuffer.set(rawName, rawNameLen);
    }
    else {

        fBuffer.set(rawName + colonIndex + 1, rawNameLen - colonIndex - 1);
    }

    return fStringPool->getValueForId(fStringPool->addOrFind(fBuffer.getRawBuffer()));
}

inline bool
TraverseSchema::isValidRefDeclaration(const IDOM_Element* const elem) {

    return !(XMLString::stringLen(elem->getAttribute(SchemaSymbols::fgATT_ABSTRACT)) != 0
             || XMLString::stringLen(elem->getAttribute(SchemaSymbols::fgATT_NILLABLE)) != 0
             || XMLString::stringLen(elem->getAttribute(SchemaSymbols::fgATT_BLOCK)) != 0
             || XMLString::stringLen(elem->getAttribute(SchemaSymbols::fgATT_FINAL)) != 0
             || XMLString::stringLen(elem->getAttribute(SchemaSymbols::fgATT_TYPE)) != 0
             || XMLString::stringLen(elem->getAttribute(SchemaSymbols::fgATT_DEFAULT)) != 0 
             || XMLString::stringLen(elem->getAttribute(SchemaSymbols::fgATT_FIXED)) != 0
             || XMLString::stringLen(elem->getAttribute(SchemaSymbols::fgATT_SUBSTITUTIONGROUP)) != 0);
}

inline
const XMLCh* TraverseSchema::getElementAttValue(const IDOM_Element* const elem,
                                                const XMLCh* const attName,
                                                const bool toTrim) {

    IDOM_Attr* attNode = elem->getAttributeNode(attName);

    if (attNode == 0) {
        return 0;
    }

    const XMLCh* attValue = attNode->getValue();

    if (toTrim) {

        fBuffer.set(attValue);
        XMLCh* bufValue = fBuffer.getRawBuffer();
        XMLString::trim(bufValue);

        if (!XMLString::stringLen(bufValue)) {
            return XMLUni::fgZeroLenString;
        }

        return fStringPool->getValueForId(fStringPool->addOrFind(bufValue));
    }

    return attValue;
}

inline const XMLCh* 
TraverseSchema::getTargetNamespaceString(const IDOM_Element* const elem) {

    const XMLCh* targetNS = getElementAttValue(elem, SchemaSymbols::fgATT_TARGETNAMESPACE);

    if (targetNS && XMLString::stringLen(targetNS) == 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidTargetNSValue);
    }

    return targetNS;
}

inline bool TraverseSchema::isBaseFromAnotherSchema(const XMLCh* const baseURI)
{
    if (XMLString::compareString(baseURI,fTargetNSURIString) != 0
        && XMLString::compareString(baseURI, SchemaSymbols::fgURI_SCHEMAFORSCHEMA) != 0
        && XMLString::stringLen(baseURI) != 0) {
        //REVISIT, !!!! a hack: for schema that has no 
        //target namespace, e.g. personal-schema.xml
        return true;
    }

    return false;
}

inline bool TraverseSchema::isAttrOrAttrGroup(const IDOM_Element* const elem) {

    const XMLCh* elementName = elem->getLocalName();

    if (!XMLString::compareString(elementName, SchemaSymbols::fgELT_ATTRIBUTE) ||
        !XMLString::compareString(elementName, SchemaSymbols::fgELT_ATTRIBUTEGROUP) ||
        !XMLString::compareString(elementName, SchemaSymbols::fgELT_ANYATTRIBUTE)) {
        return true;
    }

    return false;
}

inline const XMLCh* TraverseSchema::genAnonTypeName(const XMLCh* const prefix) {

    XMLCh anonCountStr[16]; // a count of 15 digits should be enough

    XMLString::binToText(fAnonXSTypeCount++, anonCountStr, 15, 10);
    fBuffer.set(prefix);
    fBuffer.append(anonCountStr);

    return fStringPool->getValueForId(fStringPool->addOrFind(fBuffer.getRawBuffer()));
}

inline int TraverseSchema::resetCurrentTypeNameStack(const int value) {

    unsigned int stackSize = fCurrentTypeNameStack->size();

    if (stackSize != 0) {
        fCurrentTypeNameStack->removeElementAt(stackSize - 1);
    }

    return value;
}

inline void 
TraverseSchema::copyWildCardData(const SchemaAttDef* const srcWildCard,
                                 SchemaAttDef* const destWildCard) {

    destWildCard->getAttName()->setURI(srcWildCard->getAttName()->getURI());
    destWildCard->setType(srcWildCard->getType());
    destWildCard->setDefaultType(srcWildCard->getDefaultType());
}

inline bool
TraverseSchema::isAnyType(const XMLCh* const typeName) {

    const XMLCh* const localPart = getLocalPart(typeName);
    const XMLCh* const uri = resolvePrefixToURI(getPrefix(typeName));

    return (!XMLString::compareString(uri, SchemaSymbols::fgURI_SCHEMAFORSCHEMA)
            && !XMLString::compareString(localPart, SchemaSymbols::fgATTVAL_ANYTYPE));
}

inline void TraverseSchema::getRedefineNewTypeName(const XMLCh* const oldTypeName,
                                                   const int redefineCounter,
                                                   XMLBuffer& newTypeName) {

    newTypeName.set(oldTypeName);

    for (int i=0; i < redefineCounter; i++) {
        newTypeName.append(SchemaSymbols::fgRedefIdentifier);
    }
}

inline bool
TraverseSchema::isOccurrenceRangeOK(const int min1, const int max1,
                                    const int min2, const int max2) {

    if (min1 >= min2 &&
        (max2 == SchemaSymbols::UNBOUNDED || 
         (max1 != SchemaSymbols::UNBOUNDED && max1 <= max2))) {
        return true;
    }
    return false;
}

inline bool
TraverseSchema::isTopLevelComponent(const IDOM_Element* const elem) {

    const XMLCh* parentName = elem->getParentNode()->getLocalName();

    return (XMLString::endsWith(parentName, SchemaSymbols::fgELT_SCHEMA))
            || (XMLString::endsWith(parentName, SchemaSymbols::fgELT_REDEFINE));
}

#endif

/**
  * End of file TraverseSchema.hpp
  */

