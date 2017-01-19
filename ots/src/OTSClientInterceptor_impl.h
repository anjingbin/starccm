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

#ifndef OB_OTS_CLIENT_INTERCEPTOR_H
#define OB_OTS_CLIENT_INTERCEPTOR_H

#include <OB/PIClient.h>
#include "CosTransactionsPolicy.h"//renyi 6 7:???????

namespace PortableInterceptor
{

//
// Interceptor that gets the transaction context from the request
// context and updates the transaction context so that it knows what
// the current transaction is for the scope of the request
//
class OTSClientInterceptor_impl : public ClientRequestInterceptor,
	                                         CORBA::LocalObject //renyi 12 2 : added
{
    // 
    // The Transaction Service
    // 

    //
    // The slot id
    //
    SlotId slotId_;

    // 
    // CODEC used to convert between service context and propagation
    // contexts
    // 
    IOP::Codec_var codec_;

    // 
    // Get the the OTSPolicyValue
    //
    CosTransactions::OTSPolicyValue getOTSPolicyValue(
        ClientRequestInfo_ptr)
	throw(CORBA::SystemException);

    CosTransactions::NonTxTargetPolicyValue getNonTxTargetPolicyValue(
	ClientRequestInfo_ptr)
	throw(CORBA::SystemException);
    
public:

    OTSClientInterceptor_impl(SlotId, IOP::Codec_ptr);

    virtual ~OTSClientInterceptor_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual char* name();

	virtual void destroy();//renyi 12 2 : added
    
    virtual void send_request(
        ClientRequestInfo_ptr ri)
	throw(ForwardRequest, CORBA::SystemException);

    virtual void send_poll(ClientRequestInfo_ptr)
	throw(CORBA::SystemException);

    virtual void receive_reply(ClientRequestInfo_ptr)
	throw(CORBA::SystemException);

    virtual void receive_exception(ClientRequestInfo_ptr)
	throw(ForwardRequest, CORBA::SystemException);

    virtual void receive_other(ClientRequestInfo_ptr)
	throw(ForwardRequest, CORBA::SystemException);
};

} // end namespace PortableInterceptor

#endif
