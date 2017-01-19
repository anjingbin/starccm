// **********************************************************************
//
// Copyright (c) 2002-2003 
// StarMiddleware.net	
// www.StarMiddleware.net
// 
// All Rights Reserved
//
// Author: Ren Yi	starots@sohu.com	
// **********************************************************************

// Version: 1.0.0

#include <OB/CORBA.h>

#include <OTSPolicyFactory_impl.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

//renyi 6 17
#ifndef _DEBUG_
#define _DEBUG_
#endif

using namespace OB;

namespace CosTransactions
{

//
// The InvocationPolicy implementation
//
class InvocationPolicy_impl: public InvocationPolicy,
	                                 CORBA::LocalObject //renyi 12 2 : added
{
    // 
    // The InvocationPolicyValue
    // 
    InvocationPolicyValue ipv_;

public:

    InvocationPolicy_impl(InvocationPolicyValue);//renyi 6 4:remove
    virtual ~InvocationPolicy_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual InvocationPolicyValue ipv()
	throw(CORBA::SystemException);

    virtual CORBA::PolicyType policy_type()
	throw(CORBA::SystemException);

    virtual CORBA::Policy_ptr copy()
	throw(CORBA::SystemException);

    virtual void destroy()
	throw(CORBA::SystemException);
};


//
// The TransactionPolicy implementation
//
class OTSPolicy_impl: public OTSPolicy,
	                        CORBA::LocalObject //renyi 12 2 : added
{
    // 
    // The OTSPolicyValue
    // 
    OTSPolicyValue tpv_;

public:

    OTSPolicy_impl(OTSPolicyValue);
    virtual ~OTSPolicy_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual OTSPolicyValue tpv()
	throw(CORBA::SystemException);

    virtual CORBA::PolicyType policy_type()
	throw(CORBA::SystemException);

    virtual CORBA::Policy_ptr copy()
	throw(CORBA::SystemException);

    virtual void destroy()
	throw(CORBA::SystemException);
};

class NonTxTargetPolicy_impl : public NonTxTargetPolicy,
	                                     CORBA::LocalObject //renyi 12 2 : added
{
    // 
    // The OTSPolicyValue
    // 
    NonTxTargetPolicyValue tpv_;

public:

    NonTxTargetPolicy_impl(NonTxTargetPolicyValue);
    virtual ~NonTxTargetPolicy_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual NonTxTargetPolicyValue tpv()
	throw(CORBA::SystemException);

    virtual CORBA::PolicyType policy_type()
	throw(CORBA::SystemException);

    virtual CORBA::Policy_ptr copy()
	throw(CORBA::SystemException);

    virtual void destroy()
	throw(CORBA::SystemException);
};

} // end namespace CosTransactions

// ----------------------------------------------------------------------
// InvocationPolicy_impl constructor/destructor
// ----------------------------------------------------------------------

CosTransactions::InvocationPolicy_impl::InvocationPolicy_impl(
    CosTransactions::InvocationPolicyValue ipv)
    : ipv_(ipv)
{
 	#ifdef _DEBUG_
		cout << "[InvocationPolicy_impl::constructor is OK!]" << endl;
	#endif
}

CosTransactions::InvocationPolicy_impl::~InvocationPolicy_impl()
{
 	#ifdef _DEBUG_
		cout << "[InvocationPolicy_impl::destructor is OK!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// InvocationPolicy_impl public member implementation
// ----------------------------------------------------------------------

CosTransactions::InvocationPolicyValue
CosTransactions::InvocationPolicy_impl::ipv()
    throw(CORBA::SystemException)
{
    return ipv_;
}

CORBA::PolicyType
CosTransactions::InvocationPolicy_impl::policy_type()
    throw(CORBA::SystemException)
{
    return CosTransactions::INVOCATION_POLICY_TYPE;
}

CORBA::Policy_ptr
CosTransactions::InvocationPolicy_impl::copy()
    throw(CORBA::SystemException)
{
    return CORBA::Policy::_duplicate(this);
}

void
CosTransactions::InvocationPolicy_impl::destroy()
    throw(CORBA::SystemException)
{
}


// ----------------------------------------------------------------------
// OTSPolicy_impl constructor/destructor
// ----------------------------------------------------------------------

CosTransactions::OTSPolicy_impl::OTSPolicy_impl(
    CosTransactions::OTSPolicyValue tpv)
    : tpv_(tpv)
{
 	#ifdef _DEBUG_
		cout << "[OTSPolicy_impl::constructor is OK!]" << endl;
	#endif
}

CosTransactions::OTSPolicy_impl::~OTSPolicy_impl()
{
 	#ifdef _DEBUG_
		cout << "[OTSPolicy_impl::destructor is OK!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// OTSPolicy_impl public member implementation
// ----------------------------------------------------------------------

CosTransactions::OTSPolicyValue
CosTransactions::OTSPolicy_impl::tpv()
    throw(CORBA::SystemException)
{
    return tpv_;
}

CORBA::PolicyType
CosTransactions::OTSPolicy_impl::policy_type()
    throw(CORBA::SystemException)
{
    return CosTransactions::OTS_POLICY_TYPE;
}

CORBA::Policy_ptr
CosTransactions::OTSPolicy_impl::copy()
    throw(CORBA::SystemException)
{
    return CORBA::Policy::_duplicate(this);
}

void
CosTransactions::OTSPolicy_impl::destroy()
    throw(CORBA::SystemException)
{
}

// ----------------------------------------------------------------------
// NonTxTargetPolicy_impl constructor/destructor
// ----------------------------------------------------------------------

CosTransactions::NonTxTargetPolicy_impl::NonTxTargetPolicy_impl(
    CosTransactions::NonTxTargetPolicyValue tpv)
    : tpv_(tpv)
{
 	#ifdef _DEBUG_
		cout << "[NonTxTargetPolicy_impl::constructor is OK!]" << endl;
	#endif
}

CosTransactions::NonTxTargetPolicy_impl::~NonTxTargetPolicy_impl()
{
 	#ifdef _DEBUG_
		cout << "[NonTxTargetPolicy_impl::destructor is OK!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// NonTxTargetPolicy_impl public member implementation
// ----------------------------------------------------------------------

CosTransactions::NonTxTargetPolicyValue
CosTransactions::NonTxTargetPolicy_impl::tpv()
    throw(CORBA::SystemException)
{
    return tpv_;
}

CORBA::PolicyType
CosTransactions::NonTxTargetPolicy_impl::policy_type()
    throw(CORBA::SystemException)
{
    return CosTransactions::NON_TX_TARGET_POLICY_TYPE;
}

CORBA::Policy_ptr
CosTransactions::NonTxTargetPolicy_impl::copy()
    throw(CORBA::SystemException)
{
    return CORBA::Policy::_duplicate(this);
}

void
CosTransactions::NonTxTargetPolicy_impl::destroy()
    throw(CORBA::SystemException)
{
}

// ----------------------------------------------------------------------
// OTSPolicyFactory_impl constructor/destructor
// ----------------------------------------------------------------------

PortableInterceptor::OTSPolicyFactory_impl::OTSPolicyFactory_impl()
{
 	#ifdef _DEBUG_
		cout << "[OTSPolicyFactory_impl::constructor is OK!]" << endl;
	#endif
}

PortableInterceptor::OTSPolicyFactory_impl::~OTSPolicyFactory_impl()
{
 	#ifdef _DEBUG_
		cout << "[OTSPolicyFactory_impl::destructor is OK!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// OTSPolicyFactory_impl public member implementation
// ----------------------------------------------------------------------

CORBA::Policy_ptr
PortableInterceptor::OTSPolicyFactory_impl::create_policy(
    CORBA::PolicyType type,
    const CORBA::Any& value)
    throw(CORBA::PolicyError)
{
 	#ifdef _DEBUG_
		cout << "[OTSPolicyFactory_impl::create_policy begin!]" << endl;
	#endif

	if(type == CosTransactions::INVOCATION_POLICY_TYPE) //renyi 6 4:remove
    {
	CosTransactions::InvocationPolicyValue v;
	if(value >>= v)
	    return new CosTransactions::InvocationPolicy_impl(v);
        throw CORBA::PolicyError(CORBA::BAD_POLICY_TYPE);
    }
	

    if(type == CosTransactions::OTS_POLICY_TYPE)
    {
	CosTransactions::OTSPolicyValue v;
	if(value >>= v)
	    return new CosTransactions::OTSPolicy_impl(v);
        throw CORBA::PolicyError(CORBA::BAD_POLICY_TYPE);
    }

    if(type == CosTransactions::NON_TX_TARGET_POLICY_TYPE)
    {
	CosTransactions::NonTxTargetPolicyValue v;
	if(value >>= v)
	    return new CosTransactions::NonTxTargetPolicy_impl(v);
	throw CORBA::PolicyError(CORBA::BAD_POLICY_TYPE);
    }

    throw CORBA::PolicyError(CORBA::BAD_POLICY);
    return CORBA::Policy::_nil(); // Some compilers need this
}
