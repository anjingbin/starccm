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

#ifndef OB_OTS_POLICYFACTORY_IMPL_H
#define OB_OTS_POLICYFACTORY_IMPL_H

#include <OB/PIPolicy.h>

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
