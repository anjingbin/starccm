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

#include <OB/CORBA.h>
#include <OB/PICurrent.h>

#include <CosTransactionsCurrent_impl.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

//renyi 6 17
#ifndef _DEBUG_
#define _DEBUG_
#endif

using namespace OB;

//----------------------------------------------------------------------
//Current_impl get Coordinator
//----------------------------------------------------------------------

CosTransactions::Coordinator_ptr 
CosTransactions::Current_impl::get_Coordinator()
{
	#ifdef _DEBUG_
		cout << "[Current_impl::get_Coordinator() begin!]"
			 << endl;
	#endif

	CosTransactions::Control_ptr ctrl = get_control();
	
	if(CORBA::is_nil(ctrl))//renyi 6 12:changed
		throw CosTransactions::NoTransaction();

	#ifdef _DEBUG_
		cout << "\t[return Coordinator Object reference]"
		     << endl
			 << "\t[Current_impl::get_Coordinator() end!]"
			 << endl;
	#endif
	
	return ctrl -> get_coordinator();
}

//----------------------------------------------------------------------
//Current_impl get Coordinator
//----------------------------------------------------------------------

CosTransactions::Terminator_ptr 
CosTransactions::Current_impl::get_Terminator()
{
	#ifdef _DEBUG_
		cout << "[Current_impl::get_Terminator() begin!]"
			 << endl;
	#endif

	CosTransactions::Control_ptr ctrl = get_control();

	if(CORBA::is_nil(ctrl))//renyi 6 12:changed
		throw CosTransactions::NoTransaction();

	#ifdef _DEBUG_
		cout << "\t[return Terminator Object reference]"
		     << endl
			 << "\t[Current_impl::get_Coordinator() end!]"
			 << endl;
	#endif
	
	return ctrl -> get_terminator();
}


// ----------------------------------------------------------------------
// Current_impl constructor and destructor
// ----------------------------------------------------------------------

CosTransactions::Current_impl::Current_impl(
			   PortableInterceptor::Current_ptr piCurrent,
                           CORBA::ULong slotId,
                           CORBA::ULong defaultTimeout)
    : piCurrent_(PortableInterceptor::Current::_duplicate(piCurrent)),
      slotId_(slotId),
      defaultTimeout_(defaultTimeout)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::constructor is OK!]" << endl;
	#endif
}

CosTransactions::Current_impl::~Current_impl()
{
    //
    // Cleanup the TransactionService
    //

	#ifdef _DEBUG_
		cout << "[Current_impl::destructor is OK!]" << endl;
	#endif
}

// ----------------------------------------------------------------------
// Current_impl public member implementation
// ----------------------------------------------------------------------

//renyi 6 6:added
void 
CosTransactions::Current_impl::setTransactionFactory(CosTransactions::TransactionFactory_ptr txnFactory)
{
    //
    // This should be considered part of initialization, therefore no
    // sync
    //

	#ifdef _DEBUG_
		cout << "[Current_impl::setTransactionFactory begin!]" << endl;
	#endif

    txnFactory_ =CosTransactions::TransactionFactory::_duplicate(txnFactory);

	#ifdef _DEBUG_
		cout << "[Current_impl::setTransactionFactory end!]" << endl;
	#endif
}

void 
CosTransactions::Current_impl::begin()
    throw(CosTransactions::SubtransactionsUnavailable,
          CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::begin() begin!]" << endl;
	#endif

	begin_named("");

	#ifdef _DEBUG_
		cout << "[Current_impl::begin() end!]" << endl;
	#endif
}

//renyi 6 6:changed
void
CosTransactions::Current_impl::begin_named(const char* name)
    throw(CosTransactions::SubtransactionsUnavailable,//renyi 6 6:????????
          CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::begin_named begin!]" << endl;
	#endif

	if(name == 0)
	throw CORBA::BAD_PARAM();

    if(CORBA::is_nil(txnFactory_))
	// 
	// Collocated Coordinators not supported at this time
	// 
	throw CORBA::NO_IMPLEMENT();
    
	//
    //todo:renyi 6 6:decide if there is another active transation
    //and throw exceptions if it is true:???????
    //

	CORBA::Any_var any = piCurrent_ -> get_slot(slotId_);
	CORBA::TypeCode_var tc = any -> type();
	
	if(tc -> kind() == CORBA::tk_null)//renyi 6 12
	{
	    //renyi 6 21:added for debug
		//CosTransactions::Control_var ctrl;
		//any >>= ctrl;
		CosTransactions::Control_var ctrl = 
		txnFactory_ -> create(defaultTimeout_);

		CORBA::Any newContent;
		newContent <<= ctrl;

		setPropagationContext(newContent);
	}
	else 
	{
		cout << "[Throw CosTransactions::SubtransactionsUnavailable exception!]" << endl;
		throw CosTransactions::SubtransactionsUnavailable();
	}

	#ifdef _DEBUG_
		cout << "[Current_impl::begin_named end!]" << endl;
	#endif
}

//renyi 6 6:changed
void 
CosTransactions::Current_impl::commit(CORBA::Boolean report_heuristics)
    throw(CosTransactions::NoTransaction,
	  CosTransactions::HeuristicMixed,
	  CosTransactions::HeuristicHazard,
	  CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::commit begin!]" << endl;
	#endif

	try
	{
		CosTransactions::Terminator_var term = get_Terminator();
		
		term -> commit(report_heuristics);

		//renyi 7 23
		CORBA::Any_var any = piCurrent_ -> get_slot(slotId_);
		CORBA::TypeCode_var tc = any -> type();

		if(tc -> kind() != CORBA::tk_null)
		{
			CORBA::Any newContent;

			setPropagationContext(newContent);
		}

		#ifdef _DEBUG_
			cout << "[Current_impl::commit end!]" << endl;
		#endif
	}
	catch(const CosTransactions::Unavailable&) //renyi 6 12:???????
	{
		throw CORBA::NO_PERMISSION();	
	}
	catch(const CosTransactions::NoTransaction&)//renyi 6 12:added
    {
		throw;
    }
}

//renyi 6 6:changed
void 
CosTransactions::Current_impl::rollback()
    throw(CosTransactions::NoTransaction,
           CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::rollback() begin!]" << endl;
	#endif

	try
	{
		CosTransactions::Terminator_var term = get_Terminator();
		term -> rollback();

		//renyi 7 23
		CORBA::Any_var any = piCurrent_ -> get_slot(slotId_);
		CORBA::TypeCode_var tc = any -> type();

		if(tc -> kind() != CORBA::tk_null)
		{
			CORBA::Any newContent;

			setPropagationContext(newContent);
		}

		#ifdef _DEBUG_
			cout << "[Current_impl::rollback() end!]" << endl;
		#endif
	}
	catch(const CosTransactions::Unavailable&) //renyi 6 12:???????
	{
		throw CORBA::NO_PERMISSION();	
	}
    catch(const CosTransactions::NoTransaction&)//renyi 6 12:added
    {
		throw;
    }
}

//renyi 6 6:changed
void 
CosTransactions::Current_impl::rollback_only()
    throw(CosTransactions::NoTransaction,
           CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::rollback_only() begin!]" << endl;
	#endif

    try
    {
		CosTransactions::Coordinator_var coord = get_Coordinator();
		coord -> rollback_only();

		#ifdef _DEBUG_
			cout << "[Current_impl::rollback_only() end!]" << endl;
		#endif
    }
    catch(const CosTransactions::Unavailable&) //renyi 6 6:???????
    {
        throw CORBA::NO_PERMISSION();
    }
    catch(const CosTransactions::NoTransaction&)//renyi 6 12:added
    {
		throw;
    }
}

//renyi 6 6:changed
CosTransactions::Status 
CosTransactions::Current_impl::get_status()
    throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::get_status() begin!]" << endl;
	#endif

//CosTransactions::Coordinator_var coord = get_Coordinator();
//SuLiang modify. 2003-7-7
	CosTransactions::Coordinator_var coord = 0;
	try
	{
		coord = get_Coordinator();
	}
	catch(CosTransactions::NoTransaction& )
	{
		return CosTransactions::StatusNoTransaction;
	}
//end of modify .2003-7-7

	#ifdef _DEBUG_
		cout << "\t[return Transactions Status]"
		     << endl
			 << "\t[Current_impl::get_status() end!]"
			 << endl;
	#endif

	return coord -> get_status();
}

//renyi 6 6:to be implemented
char* 
CosTransactions::Current_impl::get_transaction_name()
    throw(CORBA::SystemException)
{
	//renyi 6 6:remove
	#ifdef _DEBUG_
		cout << "[Current_impl::get_transaction_name() to be implemented!]" << endl;
	#endif

    return CORBA::string_dup("default");
}


void
CosTransactions::Current_impl::set_timeout(CORBA::ULong seconds)
    throw(CORBA::SystemException)
{	
	#ifdef _DEBUG_
		cout << "[Current_impl::set_timeout begin!]" << endl;
	#endif

    defaultTimeout_ = seconds;

	#ifdef _DEBUG_
		cout << "[Current_impl::set_timeout end!]" << endl;
	#endif
}

CORBA::ULong 
CosTransactions::Current_impl::get_timeout()
    throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::get_timeout begin!]" << endl;
	#endif

    return defaultTimeout_;

	#ifdef _DEBUG_
		cout << "[Current_impl::get_timeout end!]" << endl;
	#endif
}

//renyi 6 6:changed
CosTransactions::Control_ptr 
CosTransactions::Current_impl::get_control()
    throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::get_control() begin!]" << endl;
	#endif

	CORBA::Any_var any = piCurrent_ -> get_slot(slotId_);
	CORBA::TypeCode_var tc = any -> type();
	
	if(tc -> kind() == CORBA::tk_null)//renyi 6 12
	return CosTransactions::Control::_nil();

	CosTransactions::Control* ctrl;
	bool rc = any >>= ctrl;
	assert(rc);

	#ifdef _DEBUG_
		cout << "[Current_impl::get_control() end!]" << endl;
	#endif

	return CosTransactions::Control::_duplicate(ctrl);
}

//renyi 6 6:changed
CosTransactions::Control_ptr 
CosTransactions::Current_impl::suspend()
    throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[Current_impl::suspend() begin!]" << endl;
	#endif

    CosTransactions::Control_var control = get_control();

	#ifdef _DEBUG_
		cout << "[Current_impl::suspend() end!]" << endl;
	#endif
	CORBA::Any newContent;
	setPropagationContext(newContent);	
	return control._retn();
}

//renyi 12 3
void 
CosTransactions::Current_impl::resume(CosTransactions::Control_ptr control)
    throw(CosTransactions::InvalidControl, CORBA::SystemException)
{
    //Transaction_var transaction;

	#ifdef _DEBUG_
		cout << "[Current_impl::resume begin!]" << endl;
	#endif

    if(!CORBA::is_nil(control))
    {
		CORBA::Any newContent;
		newContent <<= control;

		setPropagationContext(newContent);
    }
    else
    	cout << "[ERROR : The Control is NULL!]" << endl;

	#ifdef _DEBUG_
		cout << "[Current_impl::resume end!]" << endl;
	#endif
}

//renyi 6 6:added
void
CosTransactions::Current_impl::setPropagationContext(const CORBA::Any OTSPropogationContext) 
{
	#ifdef _DEBUG_
		cout << "[Current_impl::setPropagationContext() begin!]" << endl;
	#endif

	piCurrent_ -> set_slot(slotId_, OTSPropogationContext);

	#ifdef _DEBUG_
		cout << "[Current_impl::setPropagationContext() end!]" << endl;
	#endif
}
