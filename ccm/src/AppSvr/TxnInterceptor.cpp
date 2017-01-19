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

#include <TxnInterceptor.h>

#include <EntityContext_impl.h>
#include <ContainerBase.h>
#include <ContainerRunTime.h>

#pragma warning(disable: 4503)

using namespace Container;
 

CORBA::Long TxnInterceptor::commitCount = 0; 
CORBA::Long TxnInterceptor::TotalRolledBack =0;

//SuLiang modified 2004.7.20
const char* 
TxnInterceptor::opName[] =
{
	//Navigation
	"provide_facet",
	"get_all_facets",
	"get_named_facets",
	"same_component",
	
	//Receptacles
	"connect",
	"disconnect",
	"get_connections",
	"get_all_receptacles",
	"get_named_receptacles",
	
	//EventConsumerBase
	"push_event",
	
	//Events
	"get_consumer",
	"subscribe",
	"unsubscribe",
	"connect_consumer",
	"disconnect_consumer"
	"get_all_consumers",
	"get_named_consumers",
	"get_all_emitters",
	"get_named_emitters",
	"get_all_publishers",
	"get_named_publishers",
	
	//CCMObject
	"get_component_def",
	"get_ccm_home",
	"get_primary_key",
	"configuration_complete",
	"remove",
	"get_all_ports",
	"enter_dyn_deploy",
	"leave_dyn_deploy",
	
	//Object
	"_duplicate",
	"_nil",
	"_get_component",
	"_get_interface",
	"_is_a",
	"_non_existent",
	"_is_equivalent",
	"_narrow",
	"_create_request",
	"_create_request",
	"_request",
	"_get_policy",
	"_set_policy_overrides",
	"_get_policy_overrides",
	"_get_client_policy",
	"_validate_connection",
	"_get_effective_policy_override",
	"_get_policy_bytypes",
	"_validate_policies",
	"_get_atf_connector_info",
	"_get_transportInfo",
	"_STAR_ids",
	"_STAR_staticIds",
	"_STAR_toAbstractBase",
	"_STAR_IOR",
	"_STAR_origIOR",
	"_STAR_marshalOrigIOR",
	"_add_ref",
	"_remove_ref",
	"_refcount_value",
	
	//ORB message
	"_locate",
	""
};
//SuLiang modified 2004.7.20


TxnInterceptor::TxnInterceptor(ContainerRunTime_ptr runtime)
{
	//modified by xiao heping 2004/03/30
	runtime_ = runtime;
	//end modified xiao heping 2004/03/30
}

TxnInterceptor::~TxnInterceptor()
{
}

void TxnInterceptor::notSupported(const PortableServer::ObjectId& oid,
								  //CosTransactions::Current_ptr current, 
								  Container::Cookie* cookie)
{
	CosTransactions::Current_ptr current = cookie->txnCurrent (); //added by xiao heping 
	CosTransactions::Control_var control;	
	
	switch (current -> get_status())
	{
	case CosTransactions::StatusActive :
	case CosTransactions::StatusMarkedRollback :
	case CosTransactions::StatusPrepared :
	case CosTransactions::StatusCommitted :
	case CosTransactions::StatusRolledBack :
	case CosTransactions::StatusPreparing :
	case CosTransactions::StatusCommitting :
	case CosTransactions::StatusUnknown :
	case CosTransactions::StatusRollingBack :
	{
		try
		{
			control = current -> suspend();

			cookie -> control(control.in());
			cookie -> txnAction(RESUME);
		}
		catch(CORBA::SystemException& /*e*/)
		{
			TRACE0(LEVEL1,"TxnInterceptor::notSupported raises SysytemException execption.\n");
		}
		break;
	}
	case CosTransactions::StatusNoTransaction :
		break;
	}	
}

void 
TxnInterceptor::required(const PortableServer::ObjectId& oid,
						 //CosTransactions::Current_ptr current, 
						 Container::Cookie* cookie)
{
	CosTransactions::Current_ptr current = cookie->txnCurrent (); //added by xiao heping 
	CosTransactions::Control_var control;
	
	try
	{
		switch (current -> get_status())
		{
		case CosTransactions::StatusActive :
		case CosTransactions::StatusMarkedRollback :
		case CosTransactions::StatusPrepared :
		case CosTransactions::StatusCommitted :
		case CosTransactions::StatusRolledBack :
		case CosTransactions::StatusUnknown :
		case CosTransactions::StatusPreparing :
		case CosTransactions::StatusCommitting :
		case CosTransactions::StatusRollingBack :
			break;
		case CosTransactions::StatusNoTransaction :
		{
			try
			{
				current -> begin();
				control = current -> get_control();
				cookie -> control(control.in());
				cookie -> txnAction(COMMIT);			
			}
			catch(CosTransactions::SubtransactionsUnavailable& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::required raises CosTransactions::SubtransactionsUnavailable execption.\n");	
				current -> rollback();
				// add by jxh 06/22
				TotalRolledBack++;
			}
			break;
		}
		}
	}
	catch(CORBA::SystemException& /*e*/)
	{
		TRACE0(LEVEL1,"TxnInterceptor::required raises SysytemException execption.\n");
	}
}

void 
TxnInterceptor::supports(const PortableServer::ObjectId& oid,
						 //CosTransactions::Current_ptr current, 
						 Container::Cookie* cookie)
{
	CosTransactions::Current_ptr current = cookie->txnCurrent (); //added by xiao heping 
	try
	{
		switch (current -> get_status())
		{
		case CosTransactions::StatusActive :
		case CosTransactions::StatusMarkedRollback :
		case CosTransactions::StatusPrepared :
		case CosTransactions::StatusCommitted :
		case CosTransactions::StatusRolledBack :
		case CosTransactions::StatusUnknown :
		case CosTransactions::StatusPreparing :
		case CosTransactions::StatusCommitting :
		case CosTransactions::StatusRollingBack :
		case CosTransactions::StatusNoTransaction :
			break;//do nothing	
		}
	}
	catch(CORBA::SystemException& /*e*/)
	{
		TRACE0(LEVEL1,"TxnInterceptor::supports raises SysytemException execption.\n");
	}
}

void 
TxnInterceptor::requiresNew(const PortableServer::ObjectId& oid,
							//CosTransactions::Current_ptr current, 
							Container::Cookie* cookie)
{
	CosTransactions::Current_ptr current = cookie->txnCurrent (); //added by xiao heping 
	CosTransactions::Control_var control;
	
	try
	{
		switch (current -> get_status())
		{
		case CosTransactions::StatusActive :
		case CosTransactions::StatusMarkedRollback :
		case CosTransactions::StatusPrepared :
		case CosTransactions::StatusCommitted :
		case CosTransactions::StatusRolledBack :
		case CosTransactions::StatusUnknown :
		case CosTransactions::StatusPreparing :
		case CosTransactions::StatusCommitting :
		case CosTransactions::StatusRollingBack :
		{
			try
			{
				control = current -> suspend();
				
				current -> begin();				
				cookie -> control(control.in());
				cookie -> txnAction(COMMITRESUME);	
			}
			catch(CosTransactions::SubtransactionsUnavailable& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::requiresNew raises CosTransactions::SubtransactionsUnavailable execption."
				               "Current transaction's status is not CosTransactions::StatusNoTransaction. \n");	
				current -> rollback();
				// add by jxh 06/22
				TotalRolledBack++;
			}
			break;
		}
		case CosTransactions::StatusNoTransaction :
		{
			try
			{
				current -> begin();
				control = current -> get_control();
				cookie -> control(control.in());
				cookie -> txnAction(COMMIT);			
			}
			catch(CosTransactions::SubtransactionsUnavailable& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::requiresNew raises CosTransactions::SubtransactionsUnavailable execption. Current transaction's status is CosTransactions::StatusNoTransaction. \n");	
				current -> rollback();
				// add by jxh 06/22
				TotalRolledBack++;
			}
			break;
		}
		}
	}
	catch(CORBA::SystemException& /*e*/)
	{
		TRACE0(LEVEL1,"TxnInterceptor::requiresNew raises SysytemException execption.\n");
	}
}

void 
TxnInterceptor::mandatory(const PortableServer::ObjectId& oid,
						  //CosTransactions::Current_ptr current, 
						  Container::Cookie* cookie)
	throw ( CosTransactions::TRANSACTION_REQUIRED )
{
	CosTransactions::Current_ptr current = cookie->txnCurrent (); //added by xiao heping 
	try
	{
		switch (current -> get_status())
		{
		case CosTransactions::StatusActive :
		case CosTransactions::StatusMarkedRollback :
		case CosTransactions::StatusPrepared :
		case CosTransactions::StatusCommitted :
		case CosTransactions::StatusRolledBack :
		case CosTransactions::StatusPreparing :
		case CosTransactions::StatusCommitting :
		case CosTransactions::StatusRollingBack :
		case CosTransactions::StatusUnknown :
			break;
		
		case CosTransactions::StatusNoTransaction :
		{
			throw new CosTransactions::TRANSACTION_REQUIRED();
			break;
		}
		}
	}
	catch(CORBA::SystemException& /*e*/)
	{
		TRACE0(LEVEL1,"TxnInterceptor::mandatory raises SysytemException execption.\n");
	}
}

void 
TxnInterceptor::never(const PortableServer::ObjectId& oid,
					  //CosTransactions::Current_ptr current, 
					  Container::Cookie* cookie)
	throw ( CosTransactions::INVALID_TRANSACTION )
{
	CosTransactions::Current_ptr current = cookie->txnCurrent (); //added by xiao heping 
	try
	{
		switch (current -> get_status())
		{
		case CosTransactions::StatusActive :
		case CosTransactions::StatusMarkedRollback :
		case CosTransactions::StatusPrepared :
		case CosTransactions::StatusCommitted :
		case CosTransactions::StatusRolledBack :
		case CosTransactions::StatusPreparing :
		case CosTransactions::StatusCommitting :
		case CosTransactions::StatusRollingBack :
		case CosTransactions::StatusUnknown :
		{
			throw new CosTransactions::INVALID_TRANSACTION();
			break;
		}
		
		case CosTransactions::StatusNoTransaction :
			break;
		}
	}
	catch(CORBA::SystemException& /*e*/)
	{
		TRACE0(LEVEL1,"TxnInterceptor::mandatory raises SysytemException execption.\n");
	}
}

CosTransactions::Current_ptr TxnInterceptor::txnCurrent()
{
	ContainerBase_var container = runtime_ -> getContainer();
	CORBA::ORB_var orb = container -> getOrb();

	CORBA::Object_var obj = orb -> resolve_initial_references("TransactionCurrent");

	if (CORBA::is_nil(obj))
	    TRACE0(LEVEL1,"TxnInterceptor can not get the initial reference of TransactionCurrent.\n"); 

	CosTransactions::Current_var current = CosTransactions::Current::_narrow(obj);

	if (CORBA::is_nil(current))
	    TRACE0(LEVEL1,"TxnInterceptor can not narrow the object to current.\n");
	
	return current._retn();
}

//deleted by xiao heping 
/*
char*
TxnInterceptor::getRepositoryId(const PortableServer::ObjectId& oid,
								PortableServer::POA_ptr adapter,
								const char* operation,
								PortableServer::Servant the_servant)
{
	//modified by xiao heping 
//	PortableServer::Servant servant;
//
//	PortableServer::ObjectId* objectId = new PortableServer::ObjectId(oid);
//	PortableServer::ServantLocator::Cookie tempCookie = 0;
//	servant = runtime_ -> getServant(objectId,tempCookie,operation);
//        
//  if( servant == NULL )
//  {
//      delete objectId;
//		return NULL;
//  }

//	CORBA::String_var repId = servant->_primary_interface(oid, adapter);	
//	delete objectId;
	CORBA::String_var repId = the_servant->_primary_interface(oid, adapter);
	return repId._retn();
}
*/
//end modify

void
TxnInterceptor::preinvoke(const PortableServer::ObjectId& oid,
						  PortableServer::POA_ptr adapter,
						  const char* operation,
						  Container::Cookie* cookie,
						  PortableServer::Servant the_servant)
{
	TxnPolicies tempPolicies;
	TxnPolicies::iterator theIterator,defaultIterator;
	TxnPolicy policy;
	AllTxnPolicies::iterator allIterator;

//SuLiang modified 2004.7.20
	int i = 0;
	while(strcmp(opName[i],"") != 0)
	{
		if(strcmp(operation,opName[i]) == 0)
			return;
		i++;	
	}
//SuLiang modified 2004.7.20

	//modified by xiao heping 
	CORBA::String_var repId = the_servant->_primary_interface(oid, adapter);
//	CORBA::String_var repId = getRepositoryId(oid,adapter,operation);
	//end modify

	std::string keyRepId(repId.in());

	allIterator = allPolicies_.find(keyRepId);
	
	if( allIterator != allPolicies_.end() )
	{
		tempPolicies = (*allIterator).second;
	}
	else
	{
		return ;
	}	
	
	theIterator = tempPolicies.find(operation);
	defaultIterator = tempPolicies.find("*");
	
	if( theIterator != tempPolicies.end() )
    {
    	policy = (*theIterator).second;
	}
	else 
	{
		if( defaultIterator != tempPolicies.end())
		{
			policy = (*defaultIterator).second;
		}
		else
		{
			return;
		}
	}
	TRACE2(LEVEL6,"---$$$$$$----The Operation %s is %d ---$$$$$$----\n",operation,policy);

	//modified by xiao heping 
//	current_ = txnCurrent();
	cookie->txnCurrent (txnCurrent());
	//end modify
	
	switch(policy)
	{
	case SELF_MANAGED:
		TRACE1(LEVEL1,"TxnInterceptor::preinvoke %s is SELF_MANAGED. \n",operation);
		break;		
	case NOT_SUPPORTED:
//		notSupported(oid,current_.in(),cookie);
		notSupported(oid,cookie);//modified by xiao heping 
		break;
	case SUPPORTS:
//		supports(oid,current_.in(),cookie);
		supports(oid,cookie);//modified by xiao heping 
		break;
	case REQUIRED:
//		required(oid,current_.in(),cookie);
		required(oid,cookie);//modified by xiao heping 
		break;
	case REQUIRES_NEW:
//		requiresNew(oid,current_.in(),cookie);
		requiresNew(oid,cookie);//modified by xiao heping 
		break;
	case MANDATORY:
//		mandatory(oid,current_.in(),cookie);
		mandatory(oid,cookie);//modified by xiao heping 
		break;
	case NEVER:
//		never(oid,current_.in(),cookie);
		never(oid,cookie);//modified by xiao heping 
		break;		
	default:
		TRACE0(LEVEL1,"TxnInterceptor::preinvoke has no transaction policy. \n");
		break;
	}
}

void 
TxnInterceptor::postinvoke(const PortableServer::ObjectId& oid,
						   PortableServer::POA_ptr adapter,
						   const char* operation,
						   Container::Cookie* cookie)
{	  
	CosTransactions::Current_ptr current = cookie->txnCurrent (); //added by xiao heping 
	CosTransactions::Control_var control;

	CosTransactions::Terminator_var terminator;

//SuLiang modified 2004.7.20
	int i = 0;
	while(strcmp(opName[i],"") != 0)
	{
		if(strcmp(operation,opName[i]) == 0)
			return;
		i++;	
	}
//SuLiang modified 2004.7.20
	
	switch(cookie -> txnAction())
	{
		case RESUME:
		{
			try
			{
				current -> resume(cookie -> control());
			}
			catch(CosTransactions::InvalidControl& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke RESUME can not get the valid control object and can not been resume.\n");
			}
			catch(CORBA::SystemException& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke RESUME raises CORBA::SystemException.\n");
			}
			break;
		}
		case COMMIT:
		{
			try
			{
				//current -> commit(true);
				control = cookie -> control();
				terminator = control -> get_terminator();
				terminator -> commit(false);

				//JXH 2004.6.21
				commitCount++;
				//JXH 2004.6.21

			}
			catch(CosTransactions::NoTransaction& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMIT raises CosTransactions::NoTransaction.\n");
				terminator -> rollback();
			}
			catch(CosTransactions::HeuristicHazard& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMIT raises CosTransactions::HeuristicHazard.\n");
				terminator -> rollback();
			}
			catch(CosTransactions::HeuristicMixed& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMIT raises CosTransactions::HeuristicMixed.\n");
				terminator -> rollback();
			}
			catch(CORBA::SystemException& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMIT raises CORBA::SystemException.\n");
				terminator -> rollback();
			}
			break;
		}
		case COMMITRESUME:
		{
			try
			{
				//current -> commit(true);
				control = cookie -> control();
				terminator = control -> get_terminator();
				terminator -> commit(false);

				//JXH 2004.6.21
				commitCount++;
				//JXH 2004.6.21

			}
			catch(CosTransactions::NoTransaction& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMITRESUME raises CosTransactions::NoTransaction.\n");
				terminator -> rollback();
			}
			catch(CosTransactions::HeuristicHazard& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMITRESUME raises CosTransactions::HeuristicHazard.\n");
				terminator -> rollback();
			}
			catch(CosTransactions::HeuristicMixed& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMITRESUME raises CosTransactions::HeuristicMixed.\n");
				terminator -> rollback();
			}
			catch(CORBA::SystemException& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMITRESUME raises CORBA::SystemException.\n");
				terminator -> rollback();
			}
			
			try
			{
				current -> resume(cookie -> control());
			}
			catch(CosTransactions::InvalidControl& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMITRESUME can not get the valid control object and can not been resume.\n");
				current -> rollback();
			}
			catch(CORBA::SystemException& /*e*/)
			{
				TRACE0(LEVEL1,"TxnInterceptor::postinvoke COMMITRESUME raises CORBA::SystemException.\n");
				current -> rollback();
			}
			break;
		}
		default:
		{
			TRACE0(LEVEL1,"TxnInterceptor::postinvoke has no transaction policy. \n");
			break;
		}
	}
	cookie -> txnAction(NOACTION);

}

void
TxnInterceptor::setTxnPolicy(const char* repId,const char* operation,TxnPolicy policy)
{	
	TxnPolicies tempPolicies;
	
	std::string keyOperation(operation);
	
	std::string keyRepId(repId);
	
	AllTxnPolicies::iterator allIterator = allPolicies_.find(keyRepId); 
 
    if( allIterator != allPolicies_.end() )
    {
        tempPolicies = (*allIterator).second;
        TxnPolicies::iterator theIterator = tempPolicies.find(keyOperation);

        if ( theIterator != tempPolicies.end() )
        {
            throw new Components::Deployment::InvalidConfiguration();
        }
        else
        {
            tempPolicies.insert(TxnPolicies::value_type(keyOperation, policy));
            allPolicies_.insert(AllTxnPolicies::value_type(keyRepId, tempPolicies));
        }
    }
    else
    {
        tempPolicies.insert(TxnPolicies::value_type(keyOperation, policy));
        allPolicies_.insert(AllTxnPolicies::value_type(keyRepId, tempPolicies));
    }  
}

CORBA::Long 
TxnInterceptor::getCommitCount()
{
	return commitCount;
}
CORBA::Long  TxnInterceptor::getTotalRolledBack()
{
	return TotalRolledBack;
}

