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
#ifndef STAR_OTS_IOR_INTERCEPTOR_IMPL_H
#define STAR_OTS_IOR_INTERCEPTOR_IMPL_H

#include <OB/PIIOR.h>

namespace PortableInterceptor
{

// 
// 
// TxnIORInterceptor specifies the transactional policies for objects
// created on a particular POA instance.
// 
class OTSIORInterceptor_impl : public IORInterceptor,CORBA::LocalObject
{
    // 
    // The codec used to construct IOP::TaggedComponents from OTS
    // policies.
    // 
    IOP::Codec_var codec_;

public:
    
    OTSIORInterceptor_impl(IOP::Codec_ptr);

    ~OTSIORInterceptor_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual char* name()
	throw(CORBA::SystemException);

	virtual void destroy();//renyi 12 2 : added

    virtual void establish_components(PortableInterceptor::IORInfo_ptr)
	throw(CORBA::SystemException);

	//renyi 12 2 : added

	virtual void
    components_established(PortableInterceptor::IORInfo_ptr)
	throw(CORBA::SystemException);

    virtual void
    adapter_state_change(
        const PortableInterceptor::ObjectReferenceTemplateSeq&,
    	PortableInterceptor::AdapterState)
	throw(CORBA::SystemException);

    virtual void adapter_manager_state_change(
	PortableInterceptor::AdapterManagerId,
	PortableInterceptor::AdapterState)
	throw(CORBA::SystemException);
};

} // end namespace PortableInterceptor

#endif
