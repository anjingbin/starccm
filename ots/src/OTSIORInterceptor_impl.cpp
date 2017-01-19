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
#include <OB/Codec.h>

#include "CosTransactionsPolicy.h"
#include "CosTSInteroperation.h"

#include "OTSIORInterceptor_impl.h"

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

// ----------------------------------------------------------------------
// PortableInterceptor::OTSIORInterceptor_impl constructor and destructor
// ----------------------------------------------------------------------

PortableInterceptor::OTSIORInterceptor_impl::OTSIORInterceptor_impl(
    IOP::Codec_ptr codec)
    : codec_(IOP::Codec::_duplicate(codec))
{
	#ifdef _DEBUG_
		cout << "[OTSIORInterceptor_impl::constructor is OK!]" << endl;
	#endif
}

PortableInterceptor::OTSIORInterceptor_impl::~OTSIORInterceptor_impl()
{
	#ifdef _DEBUG_
		cout << "[OTSIORInterceptor_impl::destructor is OK!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// PortableInterceptor::OTSIORInterceptor_impl public member implementation
// ----------------------------------------------------------------------

char* 
PortableInterceptor::OTSIORInterceptor_impl::name()
    throw(CORBA::SystemException)
{
    return CORBA::string_dup("nis.ots.ior.interceptor");
}

void 
PortableInterceptor::OTSIORInterceptor_impl::destroy()//renyi 12 2 : added
{
	#ifdef _DEBUG_
		cout << "[OTSIORInterceptor_impl::destroy() to be implemented!]" << endl;
	#endif
}

void 
PortableInterceptor::OTSIORInterceptor_impl::establish_components(
    PortableInterceptor::IORInfo_ptr info)
    throw(CORBA::SystemException)
{
 	#ifdef _DEBUG_
		cout << "[OTSIORInterceptor_impl::establish_components begin!]" << endl;
	#endif

    //
    // Write the TAG_OTS_POLICY component into the IOR.
    //
    CosTransactions::OTSPolicyValue value;

    try
    {
	CORBA::Policy_var policy =
	    info -> get_effective_policy(CosTransactions::OTS_POLICY_TYPE);
	
	CosTransactions::OTSPolicy_var otsPolicy =
	    CosTransactions::OTSPolicy::_narrow(policy);
	value = otsPolicy -> tpv();//renyi 6 4:tpv()?
    }
    catch(const CORBA::INV_POLICY&)
    {
	//
	// The default is FORBIDS. TODO: We should this allow to be a
	// configuration parameter.
	//
	value = CosTransactions::FORBIDS;
    }

    CORBA::Any any;
    any <<= value;
    CORBA::OctetSeq_var octetSeq = 
	codec_ -> encode_value(any);
     
    IOP::TaggedComponent component;
    component.tag = CosTSInteroperation::TAG_OTS_POLICY;
    component.component_data.length(octetSeq -> length());
    memcpy(component.component_data.get_buffer(),
	   octetSeq -> get_buffer(), octetSeq -> length());
    
    info -> add_ior_component(component);

 	#ifdef _DEBUG_
		cout << "[OTSIORInterceptor_impl::establish_components end!]" 
			 << endl;
	#endif

/*
 *
 * TODO: At present we don't propogate the InvocationPolicy (no good
 * reason except that we don't support unshared.
 *
    try
    {
	CORBA::Policy_var policy =
	    info -> get_effective_policy(
		CosPolicyDef::InvocationPolicyType);
	
	CosPolicyDef::InvocationPolicy_var invPolicy =
	    CosPolicyDef::InvocationPolicy::_narrow(policy);

	//
	// Don't propogate the EITHER policy to the IOR -- this is the
	// default.
	//
	if(invPolicy -> ipv() != CosPolicyDef::EITHER)
	{
	    CORBA::Any any;
	    any <<= invPolicy -> ipv();
	    CORBA::OctetSeq_var octetSeq = 
		codec_ -> encode_value(any);
	    
	    IOP::TaggedComponent component;
	    component.tag = CosPolicyDef::TAG_INV_POLICY;
	    component.component_data.length(octetSeq -> length());
	    memcpy(component.component_data.get_buffer(),
		   octetSeq -> get_buffer(), octetSeq -> length());
	    
	    info -> add_ior_component(component);
	}
    }
    catch(const CORBA::INV_POLICY&)
    {
    }
    *
    **/
}

//renyi 12 2 : added

void
PortableInterceptor::OTSIORInterceptor_impl::components_established(PortableInterceptor::IORInfo_ptr)
	throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[OTSIORInterceptor_impl::components_established to be implemented!]" << endl;
	#endif
}

void
PortableInterceptor::OTSIORInterceptor_impl::adapter_state_change(const PortableInterceptor::ObjectReferenceTemplateSeq&,
					PortableInterceptor::AdapterState)
	throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[OTSIORInterceptor_impl::adapter_state_change to be implemented!]" << endl;
	#endif
}

void 
PortableInterceptor::OTSIORInterceptor_impl::adapter_manager_state_change(PortableInterceptor::AdapterManagerId,
							PortableInterceptor::AdapterState)
	throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[OTSIORInterceptor_impl::adapter_manager_state_change to be implemented!]" << endl;
	#endif
}



