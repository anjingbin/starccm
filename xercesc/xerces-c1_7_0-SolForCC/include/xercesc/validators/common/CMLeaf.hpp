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
 * $Log: CMLeaf.hpp,v $
 * Revision 1.1  2004/07/13 02:37:43  WangKeBo
 * Component implementation
 *
 * Revision 1.1.1.1  2002/02/01 22:22:37  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/12/06 17:52:17  tng
 * Performance Enhancement.  The QName that was passed to the CMLeaf
 * constructor was always being copied, even though the CMLeaf objects
 * only existed during construction of a DFA.  In most cases the original
 * QName that was passed into the CMLeaf constructor continued to exist long
 * after the CMLeaf was destroyed; in some cases the QName was constructed
 * specifically to be passed to the CMLeaf constructor.  Added a second CMLeaf constructor that indicated the QName passed in was to be adopted; otherwise the CMLeaf constructor just sets a reference to the QName passed in.
 * By Henry Zongaro.
 *
 * Revision 1.5  2001/10/03 15:08:45  tng
 * typo fix: remove the extra space which may confuse some compilers while constructing the qname.
 *
 * Revision 1.4  2001/05/11 13:27:16  tng
 * Copyright update.
 *
 * Revision 1.3  2001/04/19 18:17:27  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.2  2001/02/16 14:58:57  tng
 * Schema: Update Makefile, configure files, project files, and include path in
 * certain cpp files because of the move of the common Content Model files.  By Pei Yong Zhang.
 *
 * Revision 1.1  2001/02/16 14:17:29  tng
 * Schema: Move the common Content Model files that are shared by DTD
 * and schema from 'DTD' folder to 'common' folder.  By Pei Yong Zhang.
 *
 * Revision 1.5  2000/03/28 19:43:25  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
 * Revision 1.4  2000/03/02 19:55:37  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:16:47  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:36  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:04  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:36  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(CMLEAF_HPP)
#define CMLEAF_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/common/CMNode.hpp>
#include <xercesc/validators/common/CMStateSet.hpp>

//
//  This class represents a leaf in the content spec node tree of an
//  element's content model. It just has an element qname and a position value,
//  the latter of which is used during the building of a DFA.
//
class CMLeaf : public CMNode
{
public :
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    CMLeaf
    (
          QName* const          element
        , const unsigned int    position = ~0
    );
    CMLeaf
    (
          QName* const          element
        , const unsigned int    position
        , const bool            adopt
    );
    ~CMLeaf();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    QName* getElement();
    const QName* getElement() const;
    unsigned int getPosition() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setPosition(const unsigned int newPosition);


    // -----------------------------------------------------------------------
    //  Implementation of public CMNode virtual interface
    // -----------------------------------------------------------------------
    bool isNullable() const;


protected :
    // -----------------------------------------------------------------------
    //  Implementation of protected CMNode virtual interface
    // -----------------------------------------------------------------------
    void calcFirstPos(CMStateSet& toSet) const;
    void calcLastPos(CMStateSet& toSet) const;


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fElement
    //      This is the element that this leaf represents.
    //
    //  fPosition
    //      Part of the algorithm to convert a regex directly to a DFA
    //      numbers each leaf sequentially. If its -1, that means its an
    //      epsilon node. All others are non-epsilon positions.
    //
    //  fAdopt
    //      This node is responsible for the storage of the fElement QName.
    // -----------------------------------------------------------------------
    QName*          fElement;
    unsigned int    fPosition;
    bool            fAdopt;
};


// -----------------------------------------------------------------------
//  Constructors
// -----------------------------------------------------------------------
inline CMLeaf::CMLeaf(    QName* const          element
                        , const unsigned int    position) :
    CMNode(ContentSpecNode::Leaf)
    , fElement(0)
    , fPosition(position)
    , fAdopt(false)
{
    if (!element)
    {
        fElement = new QName(XMLUni::fgZeroLenString
                           , XMLUni::fgZeroLenString
                           , XMLElementDecl::fgInvalidElemId);
        // We have to be responsible for this QName - override default fAdopt
        fAdopt = true;
    }
    else
    {
        fElement = element;
    }
}

inline CMLeaf::CMLeaf(    QName* const          element
                        , const unsigned int    position
                        , const bool            adopt) :
    CMNode(ContentSpecNode::Leaf)
    , fElement(0)
    , fPosition(position)
    , fAdopt(adopt)
{
    if (!element)
    {
        fElement = new QName(XMLUni::fgZeroLenString
                           , XMLUni::fgZeroLenString
                           , XMLElementDecl::fgInvalidElemId);
        // We have to be responsible for this QName - override adopt parameter
        fAdopt = true;
    }
    else
    {
        fElement = element;
    }
}

inline CMLeaf::~CMLeaf()
{
    if (fAdopt)
        delete fElement;
}


// ---------------------------------------------------------------------------
//  Getter methods
// ---------------------------------------------------------------------------
inline QName* CMLeaf::getElement()
{
    return fElement;
}

inline const QName* CMLeaf::getElement() const
{
    return fElement;
}

inline unsigned int CMLeaf::getPosition() const
{
    return fPosition;
}


// ---------------------------------------------------------------------------
//  Setter methods
// ---------------------------------------------------------------------------
inline void CMLeaf::setPosition(const unsigned int newPosition)
{
    fPosition = newPosition;
}


// ---------------------------------------------------------------------------
//  Implementation of public CMNode virtual interface
// ---------------------------------------------------------------------------
inline bool CMLeaf::isNullable() const
{
    // Leaf nodes are never nullable unless its an epsilon node
    return (fPosition == -1);
}


// ---------------------------------------------------------------------------
//  Implementation of protected CMNode virtual interface
// ---------------------------------------------------------------------------
inline void CMLeaf::calcFirstPos(CMStateSet& toSet) const
{
    // If we are an epsilon node, then the first pos is an empty set
    if (fPosition == -1)
    {
        toSet.zeroBits();
        return;
    }

    // Otherwise, its just the one bit of our position
    toSet.setBit(fPosition);
}

inline void CMLeaf::calcLastPos(CMStateSet& toSet) const
{
    // If we are an epsilon node, then the last pos is an empty set
    if (fPosition == -1)
    {
        toSet.zeroBits();
        return;
    }

    // Otherwise, its just the one bit of our position
    toSet.setBit(fPosition);
}

#endif
