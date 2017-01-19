// **********************************************************************
//
// Copyright (c) 2002
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#ifndef STAR_OTS_IOR_INTERCEPTOR_IMPL_H
#define STAR_OTS_IOR_INTERCEPTOR_IMPL_H

#include <STAR/PIIOR.h>

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
    //SuLiang modify 2003-7-1
	virtual bool _STAR_isLC() const { return true; };
	//end modify. 2003-7-1



    OTSIORInterceptor_impl(IOP::Codec_ptr);

    ~OTSIORInterceptor_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual char* name()
	throw(CORBA::SystemException);

	//renyi 4 29 : removed
	//virtual void destroy();//renyi 12 2 : added

    virtual void establish_components(PortableInterceptor::IORInfo_ptr)
	throw(CORBA::SystemException);

	//renyi 12 2 : added

	//renyi 4 29 : removed
	/*
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
	*/
};

} // end namespace PortableInterceptor

#endif
