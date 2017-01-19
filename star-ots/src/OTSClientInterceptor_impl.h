// **********************************************************************
//
// Copyright (c) 2002
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#ifndef STAR_OTS_CLIENT_INTERCEPTOR_H
#define STAR_OTS_CLIENT_INTERCEPTOR_H

#include <STAR/PIClient.h>
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
    //STAR::TransactionService* service_;//renyi 6 4:remove

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

    //SuLiang modify 2003-7-1
	virtual bool _STAR_isLC() const { return true; };
	//end modify. 2003-7-1


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
