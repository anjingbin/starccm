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

#include <ContainerBase.h>
#include <ContainerRunTime.h>
#include <CompositionManager.h>
#include <ExecutorInvoker.h>

#include <CosTransactions.h>
#include <TxnInterceptor.h>
#include <Cookie.h>

using namespace Container;

ExecutorInvoker::ExecutorInvoker(ContainerRunTime_ptr runtime)
{
	//modified by xiao heping 2004/03/30
//	runtime_ = OBJDUPLICATE(ContainerRunTime_ptr,runtime);
	runtime_ = runtime;
	//end modify xiao heping 2004/03/30 
	
//	ContainerBase_var container = runtime_ -> getContainer();

	//deleted by xiao heping 
//	cookie_ = new ::Container::Cookie();
#ifdef WITH_OTS

	txnInterceptor_ = new TxnInterceptor(runtime);
#endif
	
}

ExecutorInvoker::~ExecutorInvoker()
{
//	delete cookie_;
}

PortableServer::Servant 
ExecutorInvoker::preinvoke(const PortableServer::ObjectId& oid,
						PortableServer::POA_ptr adapter,
						const char* operation,
						PortableServer::ServantLocator::Cookie& the_cookie)
{
	TRACE0(LEVEL5,"ExecutorInvoker preinvoking\n");
	CORBA::String_var s = PortableServer::ObjectId_to_string(oid);
	TRACE1(LEVEL6,"Object id : %s \n",s);
	TRACE1(LEVEL6,"Operation name is %s\n",operation);

	PortableServer::Servant  tempServant = runtime_ -> preinvoke(oid,adapter,operation,the_cookie);
	
#ifdef WITH_OTS
	
	if (tempServant != NULL)
	{
		txnInterceptor_ -> preinvoke(oid,adapter,operation,static_cast<Container::Cookie*>(the_cookie),tempServant);
	}
#endif

	return  tempServant;
}

void 
ExecutorInvoker::postinvoke(const PortableServer::ObjectId& oid,
						PortableServer::POA_ptr adapter,
                        const char* operation,
                        PortableServer::ServantLocator::Cookie the_cookie,
                        PortableServer::Servant the_servant)
{
	TRACE0(LEVEL5,"ExecutorInvoker postinvoking\n");
	CORBA::String_var s = PortableServer::ObjectId_to_string(oid);
	//TRACE1(LEVEL6,"Object id : %s \n",s);
	TRACE1(LEVEL6,"Operation name is %s\n",operation);

#ifdef WITH_OTS

	txnInterceptor_ -> postinvoke(oid,adapter,operation,static_cast<Container::Cookie*>(the_cookie));
#endif

	runtime_ ->	postinvoke(oid,adapter,operation,the_cookie,the_servant);

}

void
ExecutorInvoker::setTxnPolicy(char* repId,char* operation,char* policyName)
{
	TxnPolicy policy;

	if(!strcmp(policyName,"self-managed"))
	{
		policy = SELF_MANAGED;
	}	
	if(!strcmp(policyName,"not-supported"))
	{
		policy = NOT_SUPPORTED;
	}
	else if(!strcmp(policyName,"required"))
	{
		policy = REQUIRED;
	}
	else if(!strcmp(policyName,"supports"))
	{
		policy = SUPPORTS;
	}
	else if(!strcmp(policyName,"requires-new"))
	{
		policy = REQUIRES_NEW;
	}
	else if(!strcmp(policyName,"mandatory"))
	{
		policy = MANDATORY;
	}
	else if(!strcmp(policyName,"never"))
	{
		policy = NEVER;
	}
	else if( strcmp(policyName,"*") == 0 )
	{
		policy = ALL;
	}
	else
	{
		TRACE0(LEVEL1,"Wrong transaction policy name");
	}
	txnInterceptor_ -> setTxnPolicy(repId,operation,policy);
}
