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

#ifndef __TxnInterceptor_h__
#define __TxnInterceptor_h__

#include <CORBA.h>
#include <TxnInterceptor_fwd.h>
#include <ContainerBase_fwd.h>
#include <ContainerBase.h>
#include <CosTransactions.h>
#include <Cookie.h>
#include <string>
#include <map>

//SuLiang modified 2004.7.20
#include <string.h>
//SuLiang modified 2004.7.20

using namespace std;

namespace Container
{
typedef std::string operationName;
typedef map<operationName,TxnPolicy> TxnPolicies;
typedef std::string RepositoryId;
typedef map<RepositoryId,TxnPolicies> AllTxnPolicies;

class TxnInterceptor : REF_COUNT_LOCAL_OBJECT
{

//SuLiang modified 2004.7.20
	static const char* opName[];
//SuLiang modified 2004.7.20


	ContainerBase_var container_;

	AllTxnPolicies allPolicies_;
	
	//modified by xiao heping 2004/03/30
//	ContainerRunTime_var runtime_;
	ContainerRunTime_ptr runtime_;
	//end modify xiao heping 2004/03/30
	 
//	CosTransactions::Current_var current_;//deleted by xiao heping

	void notSupported(const PortableServer::ObjectId& oid,/*CosTransactions::Current_ptr current, */Cookie* cookie);
	void required(const PortableServer::ObjectId& oid,/*CosTransactions::Current_ptr current,*/Cookie* cookie);
	void supports(const PortableServer::ObjectId& oid,/*CosTransactions::Current_ptr current,*/Cookie* cookie);
	void requiresNew(const PortableServer::ObjectId& oid,/*CosTransactions::Current_ptr current,*/Cookie* cookie);
	void mandatory(const PortableServer::ObjectId& oid,/*CosTransactions::Current_ptr current,*/Cookie* cookie)
		throw ( CosTransactions::TRANSACTION_REQUIRED );
	void never(const PortableServer::ObjectId& oid,/*CosTransactions::Current_ptr current,*/Cookie* cookie)
			throw ( CosTransactions::INVALID_TRANSACTION );
	CosTransactions::Current_ptr txnCurrent();

	//deleted by xiao heping 
//	char* getRepositoryId(const PortableServer::ObjectId& oid,PortableServer::POA_ptr adapter,const char* operation);
	//end delete




	//JXH 2004.6.21	
	static CORBA::Long commitCount;
	static CORBA::Long TotalRolledBack;
	
	
	
public:
	TxnInterceptor::TxnInterceptor(ContainerRunTime_ptr container);
	TxnInterceptor::~TxnInterceptor();


	//modified by xiao heping 
	void preinvoke(const PortableServer::ObjectId& oid,
					PortableServer::POA_ptr adapter,
					const char* operation,Cookie* cookie,
					PortableServer::Servant the_servant);
//	void preinvoke(const PortableServer::ObjectId& oid,PortableServer::POA_ptr adapter,const char* operation,Cookie* cookie);
	//end modify

    void postinvoke(const PortableServer::ObjectId& oid,
					PortableServer::POA_ptr adapter,
					const char* operation,Cookie* cookie);

    void setTxnPolicy(const char* repId,const char* operartion,TxnPolicy policy);
    //add by jxh
    static CORBA::Long getCommitCount();
    static CORBA::Long getTotalRolledBack();
	//JXH 2004.6.21
    
};

}
#endif

