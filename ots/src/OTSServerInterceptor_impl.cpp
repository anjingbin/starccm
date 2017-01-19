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

#include <CosTransactionsOTS.h>
#include <OTSServerInterceptor_impl.h>

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
// OTSServerInterceptor_impl private member implementation
// ----------------------------------------------------------------------

CosTransactions::OTSPolicyValue
PortableInterceptor::OTSServerInterceptor_impl::getOTSPolicyValue(
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
	#ifdef _DEBUG_
		cout << "[OTSServerInterceptor_impl::getOTSPolicyValue begin!]" << endl;
	#endif

    CosTransactions::OTSPolicyValue value;
    try
    {
        CORBA::Policy_var policy = 
            ri -> get_server_policy(CosTransactions::OTS_POLICY_TYPE);
        CosTransactions::OTSPolicy_var otsPolicy = 
	    CosTransactions::OTSPolicy::_narrow(policy);
        value = otsPolicy -> tpv();
    }
    catch(const CORBA::INV_POLICY&)
    {
	//
	// The default is FORBIDS. TODO: We should this allow to be a
	// configuration parameter.
	//
	value = CosTransactions::FORBIDS;
    }

	#ifdef _DEBUG_
		cout << "[OTSServerInterceptor_impl::getOTSPolicyValue end!]" << endl;
	#endif

    return value;
}

// ----------------------------------------------------------------------
// OTSServerInterceptor_impl constructor and destructor
// ----------------------------------------------------------------------

PortableInterceptor::OTSServerInterceptor_impl::OTSServerInterceptor_impl(
    PortableInterceptor::SlotId slotId,
    IOP::Codec_ptr codec)
    : slotId_(slotId),
      codec_(IOP::Codec::_duplicate(codec))
{
	#ifdef _DEBUG_
		cout << "[OTSServerInterceptor_impl::constructor is OK!]" << endl;
	#endif
}

PortableInterceptor::OTSServerInterceptor_impl::~OTSServerInterceptor_impl()
{
	#ifdef _DEBUG_
		cout << "[OTSServerInterceptor_impl::destructor is OK!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// ServerInterceptor public member implementation
// ----------------------------------------------------------------------

char *
PortableInterceptor::OTSServerInterceptor_impl::name()
{
    return CORBA::string_dup("OTSServerInterceptor");
}

void 
PortableInterceptor::OTSServerInterceptor_impl::destroy()//renyi 12 2 : added
{
	#ifdef _DEBUG_
		cout << "[OTSServerInterceptor_impl::destroy() to be implemented!]" << endl;
	#endif
}

void
PortableInterceptor::OTSServerInterceptor_impl::
receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr ri)
    throw(PortableInterceptor::ForwardRequest,
	  CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[OTSServerInterceptor_impl::receive_request_service_contexts begin!]" << endl;
	#endif

    //
    // Retrieve the OTSPolicyValue
    //
    CosTransactions::OTSPolicyValue value = getOTSPolicyValue(ri);

    //
    // If the OTSPolicy is internal we're done
    //
    if(value == CosTransactions::INTERNAL)
		return;

    IOP::ServiceContext_var context;
    bool haveContext;
    try
    {
        context = ri -> get_request_service_context(IOP::TransactionService);
		haveContext = true;
    }
    catch(const CORBA::BAD_PARAM&)
    {
		haveContext = false;
    }

    switch(value)
    {
	    case CosTransactions::REQUIRES:
		//
		// A transaction service context is required.
		//
		if(!haveContext)
		    throw CORBA::TRANSACTION_REQUIRED();
		break;
		
	    case CosTransactions::FORBIDS:
		//
		// A transaction service context is forbidden. 
		//
		if(haveContext)
		    throw CORBA::INVALID_TRANSACTION();
		break;
		
	    case CosTransactions::ADAPTS:
		//
		// Either/or is good
		//
		break;

    }

    if(haveContext)
    {
		// 
		// Create propagation context from the service context.
		// 
		CORBA::OctetSeq octetSeq(context -> context_data.length(),
					 context -> context_data.length(),
					 context -> context_data.get_buffer());

		CORBA::Any_var contextAny;

		try//renyi 6 12:added
		{
			contextAny =
			    codec_ -> decode_value(
				octetSeq,
				CosTransactions::_tc_Control);  //renyi 6 8:changed
		}
		catch(const IOP::Codec::TypeMismatch&)
		{
			cout << "Exception: IOP::Codec::TypeMismatch" << endl;
		}
		catch(const IOP::Codec::FormatMismatch&)
		{
			cout << "Exception: IOP::Codec::FormatMismatch" << endl;
		}
		
		CosTransactions::Control* ctrl;//renyi 6 21:changed
		bool rc = contextAny >>= ctrl;
		assert(rc);		
		
		ri -> set_slot(slotId_, contextAny);

		#ifdef _DEBUG_
			cout << "[OTSServerInterceptor_impl::receive_request_service_contexts end!]" << endl;
		#endif
    }
}

void
PortableInterceptor::OTSServerInterceptor_impl::receive_request(
    PortableInterceptor::ServerRequestInfo_ptr ri)
    throw(CORBA::SystemException)
{
}

void
PortableInterceptor::OTSServerInterceptor_impl::send_reply(
    PortableInterceptor::ServerRequestInfo_ptr ri)
    throw(CORBA::SystemException)
{
 	#ifdef _DEBUG_
		cout << "[OTSServerInterceptor_impl::send_reply begin!]" << endl;
	#endif

	//
    // Retrieve the OTSPolicyValue
    //
    CosTransactions::OTSPolicyValue value = getOTSPolicyValue(ri);

    //
    // If the value is FORBIDS or INTERNAL we're done.
    //
    if(value == CosTransactions::FORBIDS || value == CosTransactions::INTERNAL)
		return;

    //
    // Retrieve the transaction from the slot data
    //
    //Transaction_var transaction;//renyi 6 4:remove

    CORBA::Any_var any = ri -> get_slot(slotId_);

	//renyi 6 12:added
	bool ctrl_is_nil;
	ctrl_is_nil = false;
	CORBA::TypeCode_var tc = any -> type();
	if(tc -> kind() == CORBA::tk_null)//renyi 6 12
		ctrl_is_nil = true;
    
//SuLiang 2004.7.15
	////  CosTransactions::Control* ctrl;
	////	bool rc = any >>= ctrl;
	////	assert(rc);
	//
	CosTransactions::Control* ctrl;
	if(!ctrl_is_nil)
        {		
		bool rc = any >>= ctrl;
		assert(rc);
	}
	//SuLiang 2004.7.15
	
    //
    // If there is no transaction then return immediately
    //
    if(ctrl_is_nil)//renyi 6 8
    {
		assert(value == CosTransactions::ADAPTS);
        return;
    }

    //
    // If the result is a SystemException then mark the transaction as
    // rolledback.
    //
    if(ri -> reply_status() == PortableInterceptor::SYSTEM_EXCEPTION)
    {
        try
        {
            //renyi 6 8:aded
            CosTransactions::Coordinator_ptr coord = ctrl -> get_coordinator();
            coord -> rollback_only();
        }
        catch(const CORBA::Exception&)
        {
            // 
            // Exceptions such as OBJECT_NOT_EXIST are normal here
            // 
        }
    }
            
    IOP::ServiceContext context;
    context.context_id = IOP::TransactionService;
        
    CORBA::Any contextAny;
    //contextAny <<= pcontext;
    contextAny <<= ctrl;
    CORBA::OctetSeq_var octetSeq = codec_ -> encode_value(contextAny);

    context.context_data.length(octetSeq -> length());
    memcpy(context.context_data.get_buffer(), 
	   octetSeq -> get_buffer(), 
	   octetSeq -> length());

    ri -> add_reply_service_context(context, false);

 	#ifdef _DEBUG_
		cout << "[OTSServerInterceptor_impl::send_reply end!]" << endl;
	#endif
}

void
PortableInterceptor::OTSServerInterceptor_impl::send_exception(
    PortableInterceptor::ServerRequestInfo_ptr ri)
    throw(PortableInterceptor::ForwardRequest,
	  CORBA::SystemException)
{
    send_reply(ri);
}

void
PortableInterceptor::OTSServerInterceptor_impl::send_other(
    PortableInterceptor::ServerRequestInfo_ptr ri)
    throw(PortableInterceptor::ForwardRequest,
	  CORBA::SystemException)
{
    send_reply(ri);
}
