// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Kebo    mep@263.net
// Author: Huang Jie    huangjie@email.com
// Author: Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#ifndef __ExecutorInvoker_h__
#define __ExecutorInvoker_h__

#include <CORBA.h>
#include <ExecutorInvoker_fwd.h>
#include <ContainerBase_fwd.h>

#include <TxnInterceptor_fwd.h>

namespace Container
{

class ExecutorInvoker 
	: virtual public PortableServer::ServantLocator , REF_COUNT_LOCAL_OBJECT
{
protected:
	//modified by xiao heping 2004/03/30
//	ContainerRunTime_var runtime_;
	ContainerRunTime_ptr runtime_;
	//end modify xiao heping 2004/03/30

	TxnInterceptor_var txnInterceptor_;	
//	::Container::Cookie* cookie_;
public:	
	ExecutorInvoker(ContainerRunTime_ptr runtime);
	virtual ~ExecutorInvoker();

    virtual PortableServer::Servant preinvoke(const PortableServer::ObjectId& oid,
												PortableServer::POA_ptr adapter,
												const char* operation,
												PortableServer::ServantLocator::Cookie& the_cookie);

    virtual void postinvoke(const PortableServer::ObjectId& oid,
							PortableServer::POA_ptr adapter,
                            const char* operation,
                            PortableServer::ServantLocator::Cookie the_cookie,
                            PortableServer::Servant the_servant);
                            
    //saving policies functions
    void setTxnPolicy(char* repId,char* operation,char* policyName);
};

}
#endif

