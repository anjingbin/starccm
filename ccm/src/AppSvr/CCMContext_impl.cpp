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

#include <CCMContext_impl.h>
#include <ContainerRunTime.h>
#include <ContainerBase.h>

#include <CosTransactions.h>

using namespace Container;

CCMContext_impl::CCMContext_impl()
{
	
}

CCMContext_impl::~CCMContext_impl()
{
}

//
// IDL:omg.org/Components/CCMContext/get_caller_principal:1.0
//
::Components::Principal_ptr
CCMContext_impl::get_caller_principal()
{
	return 0;//...?
}

//
// IDL:omg.org/Components/CCMContext/get_CCM_home:1.0
//
::Components::CCMHome_ptr
CCMContext_impl::get_CCM_home()
{
	
	return runtime_ -> getCcmHome();
}

//
// IDL:omg.org/Components/CCMContext/get_rollback_only:1.0
//
CORBA::Boolean 
CCMContext_impl::get_rollback_only()
{

	//resolve current
	//modified by xiao heping 2004/03/30
	//CORBA::ORB_var orb = runtime_ -> getContainer() -> getOrb();
	ContainerBase_var container = runtime_ -> getContainer();
	CORBA::ORB_var orb = container -> getOrb();
	//end modified
	
	CORBA::Object_var obj = orb -> resolve_initial_references("TransactionCurrent");

	assert(!CORBA::is_nil(obj));

	CosTransactions::Current_var current = CosTransactions::Current::_narrow(obj);

	assert(!CORBA::is_nil(current));
		
	//get CosTransactions::Coordinator object to query status
	
	CosTransactions::Coordinator_var coordinator = current -> get_control() -> get_coordinator();
	
	assert(!CORBA::is_nil(coordinator));
	
	if( (coordinator-> get_status()) ==  CosTransactions::StatusRolledBack )
	{
		return true;
	}
	else
	{
		return false;
	}

}

//
// IDL:omg.org/Components/CCMContext/get_user_transaction:1.0
//
::Components::Transaction::UserTransaction_ptr
CCMContext_impl::get_user_transaction()
{
	::Components::Transaction::UserTransaction_var tx;
	ContainerBase_var container = runtime_ -> getContainer();
	tx = new ::Components::Transaction::UserTransaction_impl(container);
	return tx._retn();
}

//
// IDL:omg.org/Components/CCMContext/is_caller_in_role:1.0
//
CORBA::Boolean
CCMContext_impl::is_caller_in_role(const char* role)
{
	return true;//...?
}

//
// IDL:omg.org/Components/CCMContext/set_rollback_only:1.0
//
void
CCMContext_impl::set_rollback_only()
{
	ContainerBase_var container = runtime_ -> getContainer();
	CORBA::ORB_var orb = container -> getOrb();
	
	CORBA::Object_var obj = orb -> resolve_initial_references("TransactionCurrent");

	assert(!CORBA::is_nil(obj));

	CosTransactions::Current_var current = CosTransactions::Current::_narrow(obj);

	assert(!CORBA::is_nil(current));
	
	current -> rollback_only();
	
}
