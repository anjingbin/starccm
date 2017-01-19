// **********************************************************************
//
// Copyright (c) 2002
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#include <STAR/CORBA.h>
#include <STAR/Codec.h>

#include "CosTSInteroperation.h"  
#include "CosTransactionsOTS.h"

#include <OTSClientInterceptor_impl.h>

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

using namespace STAR;

// ----------------------------------------------------------------------
// OTSClientInterceptor private member implementation
// ----------------------------------------------------------------------

CosTransactions::OTSPolicyValue
PortableInterceptor::OTSClientInterceptor_impl::getOTSPolicyValue(
    PortableInterceptor::ClientRequestInfo_ptr ri)
    throw(CORBA::SystemException)
{
 	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::getOTSPolicyValue begin!]" << endl;
	#endif

    //
    // This throws BAD_PARAM if there is no OTS_POLICY
    //
	
    IOP::TaggedComponent_var component = 
	ri -> get_effective_component(CosTSInteroperation::TAG_OTS_POLICY);//renyi 6 4:CosTSInteroperation
    
    CORBA::OctetSeq data(component -> component_data.length(),
			 component -> component_data.length(),
			 component -> component_data.get_buffer());
    CORBA::Any_var any = 
		codec_ -> decode_value(data, CosTransactions::_tc_OTSPolicyValue);

    CosTransactions::OTSPolicyValue value;
    bool rc = any >>= value;
    assert(rc);

	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::getOTSPolicyValue end!]" << endl;
	#endif

    return value;
}

CosTransactions::NonTxTargetPolicyValue
PortableInterceptor::OTSClientInterceptor_impl::getNonTxTargetPolicyValue(
    PortableInterceptor::ClientRequestInfo_ptr ri)
    throw(CORBA::SystemException)
{ 	
	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::getNonTxTargetPolicyValue begin!]" << endl;
	#endif

    try
    {
	CORBA::Policy_var p =
	    ri -> get_request_policy(
		CosTransactions::NON_TX_TARGET_POLICY_TYPE);
	CosTransactions::NonTxTargetPolicy_var targetPolicy =
	    CosTransactions::NonTxTargetPolicy::_narrow(p);

	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::getNonTxTargetPolicyValue end!]" << endl;
	#endif

	return targetPolicy -> tpv();
    }
    catch(const CORBA::INV_POLICY&)
    {
	//
	// The default is "PERMIT".
	//
	// TODO: Note that this is contrary to the draft
	// spec. However, hopefully (most likely in-fact) that will be
	// changed.
	//
	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::getNonTxTargetPolicyValue end!]" << endl;
	#endif

	return CosTransactions::PERMIT;
    }
}

// ----------------------------------------------------------------------
// OTSClientInterceptor_impl constructor and destructor
// ----------------------------------------------------------------------

PortableInterceptor::OTSClientInterceptor_impl::OTSClientInterceptor_impl(
    SlotId slotId,
    IOP::Codec_ptr codec)
    : slotId_(slotId),
      codec_(IOP::Codec::_duplicate(codec))
{
	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::constructor is OK!]" << endl;
	#endif
}

PortableInterceptor::OTSClientInterceptor_impl::~OTSClientInterceptor_impl()
{
	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::destructor is OK!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// OTSClientInterceptor_impl public member implementation
// ----------------------------------------------------------------------

char*
PortableInterceptor::OTSClientInterceptor_impl::name()
{
    return CORBA::string_dup("OTSClientInterceptor");
}

void 
PortableInterceptor::OTSClientInterceptor_impl::destroy()//renyi 12 2 : added
{
	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::destroy() to be implemented!]" << endl;
	#endif
}

void 
PortableInterceptor::OTSClientInterceptor_impl::send_request(
    PortableInterceptor::ClientRequestInfo_ptr ri)
    throw(PortableInterceptor::ForwardRequest,
	  CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::send_request begin!]" << endl;
	#endif

    CosTransactions::OTSPolicyValue value;
    CosTransactions::NonTxTargetPolicyValue targetPolicyValue =
	getNonTxTargetPolicyValue(ri);

    try
    {
		value = getOTSPolicyValue(ri);
    }
    catch(const CORBA::BAD_PARAM&) // No OTSPolicyValue
    {
		if(targetPolicyValue == CosTransactions::PREVENT)
		{
		    //
		    // If this request is in the context of a transaction then
		    // throw a INVALID_TRANSACTION exception.
		    //
		    CORBA::Any_var any = ri -> get_slot(slotId_);
		    CORBA::TypeCode_var type = any -> type();
		    if(!type -> equivalent(CosTransactions::_tc_Control))
			throw CORBA::INVALID_TRANSACTION();
		}

		//
		// No OTSPolicyValue -- we're done.
		//
		// TODO: Strictly speaking to be interoperable with pre OTS
		// 1.2 the code call _is_a here to determine if the object
		// inherits from TransactionalObject. If so, then propagate.
		//
		return;
    }

	//renyi 6 20:temporaly added for debug
	//value = 1;

    //
    // Short circuit -- there will be nothing to do if OTSPolicy is
    // FORBIDS and NonTxTargetPolicy is PERMIT
    //
    if(targetPolicyValue == CosTransactions::PERMIT &&
       value == CosTransactions::FORBIDS)
	{
		#ifdef _DEBUG_
			cout << "[OTSClientInterceptor_impl::send_request:]"
				 << endl
				 << "\t[OTSPolicy is FORBIDS and NonTxTargetPolicy is PERMIT,return]" 
				 << endl;
		#endif

        return;
	}

    //
    // TODO: Currently the code ignores the InvocationPolicy. The code
    // will also only interop with OTS 1.2 since it doesn't check
    // TransactionalObject inheritance and pass PropogationContext
    // accordingly.
    //

    try //renyi 6 12:added
    {
	    CORBA::Any_var any = ri -> get_slot(slotId_);

		//renyi 6 12:added
		bool ctrl_is_nil;
		ctrl_is_nil = false;
		CORBA::TypeCode_var tc = any -> type();
		if(tc -> kind() == CORBA::tk_null)//renyi 6 12
			ctrl_is_nil = true;
	    
		CosTransactions::Control* ctrl;
	    bool rc = any >>= ctrl;
	    
		//Renyi add.2004.5.26
//		if (!ctrl_is_nil) 
//			assert(rc);
		//assert(rc); 2004.6.20
		//Renyi add.2004.5.26
		
	    switch(value)
	    {
	    case CosTransactions::REQUIRES:
	        //
	        // A transaction is required
	        //
	        if(ctrl_is_nil)//renyi 6 8:ctrl
	            throw CORBA::TRANSACTION_REQUIRED();
	        break;
	        
	    case CosTransactions::FORBIDS:
			//
			// A transaction is forbidden
			//
			if(!ctrl_is_nil)//renyi 6 8:ctrl
			    throw CORBA::INVALID_TRANSACTION();
		        break;
	        
	    case CosTransactions::ADAPTS:
	        //
	        // Either/or is good
	        //
	        break;

	    case CosTransactions::INTERNAL:
	        break;
	    }

	    //
	    // If there is transaction information then add an OTS service
	    // context
	    //
	    if(!ctrl_is_nil && value != CosTransactions::INTERNAL)//renyi 6 4:transaction-->ctrl
	    {    			
			// 
			// Create a service context from the propagation context.
			// 
		    
			IOP::ServiceContext context;
			context.context_id = IOP::TransactionService;
		     
			//renyi 6 21:removed
			//CORBA::Any any;
			//any <<= ctrl;//renyi 6 14:????????
			CORBA::OctetSeq_var octetSeq = 
			    codec_ -> encode_value(any);
		
			context.context_data.length(octetSeq -> length());
			memcpy(context.context_data.get_buffer(), 
			       octetSeq -> get_buffer(), 
			       octetSeq -> length());
			
			ri -> add_request_service_context(context, false);
    	
			#ifdef _DEBUG_
				cout << "[OTSClientInterceptor_impl::send_request end!]" << endl;
			#endif
   	    }
    }
    catch(const PortableInterceptor::InvalidSlot&)
	{
		// Ignore failure in this demo
		cout << "Exception: PortableInterceptor::InvalidSlot" << endl;
	}
	catch(const IOP::Codec::InvalidTypeForEncoding&)
	{
		// Ignore failure in this demo
		cout << "Exception: IOP::Codec::InvalidTypeForEncoding" << endl;
	}
}

void 
PortableInterceptor::OTSClientInterceptor_impl::send_poll(
    PortableInterceptor::ClientRequestInfo_ptr ri)
    throw(CORBA::SystemException)
{
}

void
PortableInterceptor::OTSClientInterceptor_impl::receive_reply(
    PortableInterceptor::ClientRequestInfo_ptr ri)
    throw(CORBA::SystemException)
{
 	#ifdef _DEBUG_
		cout << "[OTSClientInterceptor_impl::receive_reply begin!]" << endl;
	#endif

		CosTransactions::OTSPolicyValue value;
    try
    {
		value = getOTSPolicyValue(ri);
    }
    catch(const CORBA::BAD_PARAM&)
    {
		//
		// No value, then we're done. See the TODO: note in
		// send_request
		//
		return;
    }

    //
    // If the OTSPolicy value is INTERNAL then we're done
    //
    if(value == CosTransactions::INTERNAL)
	return;
	
    PortableInterceptor::ReplyStatus status = ri -> reply_status();

	CosTransactions::Control* ctrl;

    if(status == PortableInterceptor::SYSTEM_EXCEPTION)//renyi 6 4:remove checked
    {
        try
        {
	        IOP::ServiceContext_var context =
			ri -> get_reply_service_context(IOP::TransactionService);

		    // 
		    // Create propagation context from the service context.
		    // 
		    CORBA::OctetSeq octetSeq(context -> context_data.length(),
					     context -> context_data.length(),
					     context -> context_data.get_buffer());
		    
		    CORBA::Any_var contextAny =
			codec_ -> decode_value(
			    octetSeq,
			    CosTransactions::_tc_Control);//renyi 6 12:changed
			
		    bool rc = contextAny >>= ctrl;
		    assert(rc);

            // 
            // TODO: Revisit after beta for messaging: This assertion
            // may no longer be valid with asynchronous messaging,
            // where it may be required to import a transaction when a
            // reply is being handled by a different process than the
            // sender
            // 
            
            assert(!CORBA::is_nil(ctrl));//renyi 6 8:added
        }
        catch(const CORBA::BAD_PARAM&)
        {
            // 
            // Reply didn't have service context, get context from the
            // original request.
		    //
		    // TODO: BUG Is this really correct? If you send out a
		    // request for txn1 and get a reply with txn2 in the svc
		    // context won't this cause txn1 to have an outstanding
		    // request?
		    //
		    CORBA::Any_var any = ri -> get_slot(slotId_);
		    bool rc = any >>= ctrl;
			
			//SuLiang add.2004.7.24			
			//assert(rc);
			if(!rc)
				throw new CORBA::UNKNOWN();
			//SuLiang add.2004.7.24
			
		    //
		    // If there is no transaction, or there is a transaction
		    // but the NonTxTarget Policy is PERMIT and OTS Policy is
		    // FORBIDS, then do nothing
		    //

		    //renyi 6 12:added
		    bool ctrl_is_nil;
			ctrl_is_nil = false;
			CORBA::TypeCode_var tc = any -> type();
			if(tc -> kind() == CORBA::tk_null)//renyi 6 12
			ctrl_is_nil = true;
	        if((ctrl_is_nil) ||//renyi 6 4:change transaction-->ctrl
		       (value == CosTransactions::FORBIDS && getNonTxTargetPolicyValue(ri) == CosTransactions::PERMIT))
			return;

        }

		//
		// If the result is a SystemException and the OTSPolicyValue
		// is not ALLOWS then the transaction is marked rollback.
		//
        if(status == PortableInterceptor::SYSTEM_EXCEPTION)
        {
            try
            {                
				//renyi 6 8:added
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

    }
}

void
PortableInterceptor::OTSClientInterceptor_impl::receive_exception(
    PortableInterceptor::ClientRequestInfo_ptr ri)
    throw(PortableInterceptor::ForwardRequest,
	  CORBA::SystemException)
{
    receive_reply(ri);
}

void
PortableInterceptor::OTSClientInterceptor_impl::receive_other(
    PortableInterceptor::ClientRequestInfo_ptr ri)
    throw(PortableInterceptor::ForwardRequest,
	  CORBA::SystemException)
{
    receive_reply(ri);
}
