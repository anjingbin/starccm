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

#ifndef OB_OTS_SERVER_INTERCEPTOR_H
#define OB_OTS_SERVER_INTERCEPTOR_H

#include <OB/PIServer.h>
#include "CosTransactionsPolicy.h"

namespace PortableInterceptor
{

//
// OTSServerInterceptor_impl gets the transaction context from the
// service context and updates the transaction context so that it
// knows what the current transaction is for the scope of the request
//
class OTSServerInterceptor_impl : public ServerRequestInterceptor,
	                                         CORBA::LocalObject //renyi 12 2 : added
{
    //
    // The slot id
    //
    SlotId slotId_;

    // 
    // Codec used to convert between service context and propagation
    // contexts
    // 
    IOP::Codec_var codec_;

    // 
    // Get the the OTSPolicyValue
    //
    CosTransactions::OTSPolicyValue getOTSPolicyValue(
        ServerRequestInfo_ptr);

public:

    OTSServerInterceptor_impl(SlotId,IOP::Codec_ptr);

    virtual ~OTSServerInterceptor_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual char* name();

	virtual void destroy();//renyi 12 2 : added

    virtual void receive_request_service_contexts(
        ServerRequestInfo_ptr)
	throw(ForwardRequest,
	      CORBA::SystemException);

    virtual void receive_request(
        ServerRequestInfo_ptr)
	throw(CORBA::SystemException);

    virtual void send_reply(
        ServerRequestInfo_ptr)
	throw(CORBA::SystemException);

    virtual void send_exception(
        ServerRequestInfo_ptr)
	throw(ForwardRequest,
	      CORBA::SystemException);

    virtual void send_other(
        ServerRequestInfo_ptr)
	throw(ForwardRequest,
	      CORBA::SystemException);
};

} // end namespace PortableInterceptor

#endif
