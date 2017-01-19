// **********************************************************************
//
// Copyright (c) 2002
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#ifndef STAR_OTS_POLICYFACTORY_IMPL_H
#define STAR_OTS_POLICYFACTORY_IMPL_H

#include <STAR/PIPolicy.h>

#include "CosTransactionsPolicy.h"

namespace PortableInterceptor
{

// 
// TxnPolicyFactory creates transaction policies.
// 
class OTSPolicyFactory_impl : public PolicyFactory,
	                                   CORBA::LocalObject //renyi 12 2 : added
{
public:

	//SuLiang modify 2003-7-1
	virtual bool _STAR_isLC() const { return true; };
	//end modify. 2003-7-1
    



    OTSPolicyFactory_impl();
    virtual ~OTSPolicyFactory_impl();

    //
    // Standard IDL to C++ Mapping
    //
    virtual CORBA::Policy_ptr create_policy(CORBA::PolicyType,
                                            const CORBA::Any&)
	throw(CORBA::PolicyError);
};

} // end namespace PortableInterceptor

#endif
