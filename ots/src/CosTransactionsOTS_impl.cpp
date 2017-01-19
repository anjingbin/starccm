// **********************************************************************
//
// Copyright (c) 2002-2003 
// StarMiddleware.net	
// www.StarMiddleware.net
// 
// All Rights Reserved
//
// Author: Ren Yi	rstarots@sohu.com	
// **********************************************************************

// Version: 1.0.0

#include <OB/CORBA.h>

#include <CosTransactionsOTS_impl.h>
#include <Resource_impl.h>

#include <OTSHeader.h>

//#include <process.h>

#include <sys/types.h>
#include <signal.h>    

#include <time.h>
#include <stdio.h>
#include <errno.h>


#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif


//
// The declaration of "C" functions for multiple thread. 
//	

// Global variables serve for multiple thread routines.

CosTransactions::Control_var PTH_Ctl_var;
Resource_var PTH_Res_var;
CORBA::ULong PTH_time_out;

	// ----------------------------------------------------------------------
	// SynchronizationManager constructor/destructor
	// ----------------------------------------------------------------------

	SynchronizationManager::SynchronizationManager()
	{
	}

	SynchronizationManager::~SynchronizationManager()
	{
	}

	// ----------------------------------------------------------------------
	// SynchronizationManager public member implementation
	// ----------------------------------------------------------------------

	void
	SynchronizationManager::beforeCompletion()
		throw()
	{
		#ifdef _DEBUG_
			cout << "[SynchronizationManager::beforeCompletion() begin!]" << endl;
		#endif

	    //
	    // Next call before_completion on all registered synchronization
	    // objects
	    //
	    for(SynchronizationList::iterator iter = _syncs.begin() ;
		iter != _syncs.end() ;
		++iter)
	    {
		try
		{
		    CosTransactions::Synchronization_var sync = CosTransactions::Synchronization::_duplicate(*iter);

		    sync -> before_completion();
		}
		catch(const CORBA::SystemException& ex)
		{
			cout << "before_completion failed!" << endl;
		    cerr << ex << endl;
		}
	    }

		#ifdef _DEBUG_
			cout << "[SynchronizationManager::beforeCompletion() end!]" << endl;
		#endif

	}

	void
	SynchronizationManager::afterCompletion(CosTransactions::Status status)
	    throw()
	{
		#ifdef _DEBUG_
			cout << "[SynchronizationManager::afterCompletion() begin!]" << endl;
		#endif

	    //
	    // Next call after_completion on all registered synchronization
	    // objects
	    //
	    for(SynchronizationList::iterator iter = _syncs.begin() ;
		iter != _syncs.end() ;
		++iter)
	    {
		try
		{
		    CosTransactions::Synchronization_var sync = CosTransactions::Synchronization::_duplicate(*iter);
		 
		    sync -> after_completion(status);
		}
		catch(const CORBA::SystemException& ex)
		{
			cout << "after_completion failed" << endl;
		    cerr << ex << endl;
		}
	    }

		#ifdef _DEBUG_
			cout << "[SynchronizationManager::afterCompletion() end!]" << endl;
		#endif

	}

	void
	SynchronizationManager::registerSync(CosTransactions::Synchronization_ptr sync)
	    throw()
	{		
		#ifdef _DEBUG_
			cout << "[SynchronizationManager::registerSync begin!]" << endl;
		#endif

		CosTransactions::Synchronization_var s = CosTransactions::Synchronization::_duplicate(sync);

	    _syncs.push_back(s);

		#ifdef _DEBUG_
			cout << "[SynchronizationManager::registerSync end!]" << endl;
		#endif

	}

	void
	SynchronizationManager::clear()
	    throw()
	{
		#ifdef _DEBUG_
			cout << "[SynchronizationManager::clear() begin!]" << endl;
		#endif

	    _syncs.clear();

		#ifdef _DEBUG_
			cout << "[SynchronizationManager::clear() end!]" << endl;
		#endif
	}

CosTransactions::TransactionFactory_impl::TransactionFactory_impl(CORBA::ORB_var orb)
{
    // TODO: Add your constructor code here

	// Do following operations for delete 'zombie' process automotic.
        /*_sa.sa_sigaction = sigchld_handler;
        _sa.sa_flags = SA_NOCLDWAIT;
        
        sigaction(SIGCHLD, &_sa, 0);*/
        
	_orb_var = CORBA::ORB::_duplicate(orb);

	#ifdef _DEBUG_
		cout << "[TransactionFactory::constructor is OK!]" << endl;
	#endif
}


//=================================================================================
// If this function come across errors, the return value is set to ZERO.
// User should make sure wether return value is ZERO before transaction operations.
//=================================================================================
CosTransactions::Control_ptr CosTransactions::TransactionFactory_impl::create(CORBA::ULong time_out)
	throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException)
{
    // TODO:Add your create implemention code here
    
	#ifdef _DEBUG_
		cout << "[TransactionFactory::create begin!]"
		     << endl
		     << "\t[shmget: return the ID for a shared memory region]" 
		     << endl;
	#endif

    try
    {
		CosTransactions::Control_impl* Ctl1 = new CosTransactions::Control_impl(NULL, NULL);
	
		CosTransactions::Control_var Ctl = Ctl1 -> _this();

		CosTransactions::Control::_duplicate(Ctl);

	#ifdef _DEBUG_
		cout << "\t[return Control Object reference]"
		     << endl
		     << "\t[TransactionFactory::create end!]"
		     << endl;
	#endif
        
		return CosTransactions::Control::_duplicate(Ctl);
	
    } // eof try
    
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
		throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }

}


CosTransactions::Control_impl::Control_impl(CORBA::ORB_var orb, PortableServer::POA_var poa) 
{
    // TODO: Add your constructor code here

	#ifdef _DEBUG_
		cout << "[Control::constructor begin!]" << endl;
	#endif
	
    try
    {
    	_orb_var = CORBA::ORB::_duplicate(orb);
    	_poa_var = PortableServer::POA::_duplicate(poa);
		CosTransactions::Coordinator_impl* _Coord_var1 = new CosTransactions::Coordinator_impl(orb);
		_Coord_var = _Coord_var1 -> _this();
		CosTransactions::Terminator_impl* _Term_var1 = new CosTransactions::Terminator_impl(_Coord_var);
		_Term_var = _Term_var1 -> _this();

	#ifdef _DEBUG_
		cout << "\t[Control::constructor end!]" << endl;
	#endif
    }
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }

}

CosTransactions::Terminator_ptr CosTransactions::Control_impl::get_terminator()
	throw(CosTransactions::Unavailable,
              CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException)
{
    // TODO:Add your get_terminator implemention code here

	#ifdef _DEBUG_
		cout << "[Control::get_terminator() is OK!]" << endl;
	#endif

		return CosTransactions::Terminator::_duplicate(_Term_var);
}

CosTransactions::Coordinator_ptr CosTransactions::Control_impl::get_coordinator()
	throw(CosTransactions::Unavailable,
              CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException)
{
    // TODO:Add your get_coordinator implemention code here

	#ifdef _DEBUG_
		cout << "[Control::get_coordinator() is OK!]" << endl;
	#endif

		return CosTransactions::Coordinator::_duplicate(_Coord_var);
}


void CosTransactions::Control_impl::release ()
	throw(CORBA::SystemException)
{
    // TODO:Add your release implemention code here
	
	#ifdef _DEBUG_
		cout << "[Control::release begin!]" << endl;
	#endif
	
		CORBA::ULong RefCount = _OB_getRef ();

	#ifdef _DEBUG_
		cout << "\t[this Control object reference count is: "
		     << RefCount << "]" << endl;
	#endif
	
	_Term_var -> release();	// free private variables' spaces before
	_Coord_var -> release();	// set pointer to NULL.
	
	_Term_var -> _nil();	// set two T_var to NULL in order to make
	_Coord_var -> _nil();	// sure free their spaces allocated before.
	
	#ifdef _DEBUG_
		cout<<"\t[Deactivate Control Object process.]"<<endl;
	#endif
	
	//	_boa_var -> deactivate_impl(CORBA::ImplementationDef::_nil());


	#ifdef _DEBUG_
		cout << "\t[Control::release end!]" << endl;
	#endif
	
	return;
}


CosTransactions::Terminator_impl::Terminator_impl()
{
    // TODO: Add your constructor code here

	#ifdef _DEBUG_
		cout << "[Terminator::constructor begin!]" << endl;
	#endif
	
	_Coord_var = NULL;

	#ifdef _DEBUG_
		cout << "\t[Terminator::constructor is end!]" << endl;
	#endif
}

CosTransactions::Terminator_impl::Terminator_impl(CosTransactions::Coordinator_ptr c)
{
    // TODO: Add your constructor code here

	#ifdef _DEBUG_
		cout << "[Terminator::constructor(Coord) begin!]" << endl;
	#endif
	
		_Coord_var = CosTransactions::Coordinator::_duplicate(c);

	#ifdef _DEBUG_
		cout << "\t[Terminator::constructor(Coord) end!]" << endl;
	#endif
}

void CosTransactions::Terminator_impl::commit(CORBA::Boolean report_heuristics)
	throw 	(CORBA::SystemException,
		CosTransactions::WRONG_TRANSACTION,
		CosTransactions::INVALID_TRANSACTION,
		CosTransactions::TRANSACTION_ROLLEDBACK,
		CosTransactions::TRANSACTION_REQUIRED,
		CosTransactions::HeuristicCommit,
		CosTransactions::HeuristicRollback,
		CosTransactions::HeuristicHazard,
		CosTransactions::HeuristicMixed)
{
    // TODO:Add your commit implemention code here

	#ifdef _DEBUG_
		cout << "[Terminator::commit begin!]" << endl;
	#endif

    try
    {
    
	_Coord_var->commit(report_heuristics);
	
    }
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }


	#ifdef _DEBUG_
		cout << "\t[Terminator::commit end!]" << endl;
	#endif
	
	return;
}

void CosTransactions::Terminator_impl::rollback()
	throw (CORBA::SystemException,
	CosTransactions::WRONG_TRANSACTION,
	CosTransactions::INVALID_TRANSACTION,
	CosTransactions::TRANSACTION_ROLLEDBACK,
	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your rollback implemention code here

	#ifdef _DEBUG_
		cout << "[Terminator::rollback begin!]" << endl;
	#endif

    try  
    {
	_Coord_var->rollback();

	#ifdef _DEBUG_
		cout << "\t[Terminator::rollback end!]" << endl;
	#endif

	return;
	
    }
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }

}

void CosTransactions::Terminator_impl::release ()
	throw (CORBA::SystemException)
{
    // TODO:Add your release implemention code here
	
	#ifdef _DEBUG_
		cout << "[Terminator::release begin!]" << endl;
	#endif
	
	CORBA::ULong RefCount = _OB_getRef ();

	#ifdef _DEBUG_
		cout << "\t[this Terminator object reference count is: "
		     << RefCount << "]" << endl;
	#endif
	
	_Coord_var -> _nil();

	#ifdef _DEBUG_
		cout << "\t[Terminator::release end!]" << endl;
	#endif
	
	return;
}


CosTransactions::Coordinator_impl::Coordinator_impl(CORBA::ORB_var orb)
{
    // TODO: Add your constructor code here

	#ifdef _DEBUG_
		cout << "[Coordinator::constructor begin!]" << endl;
	#endif

    try
    {
	_orb_var = CORBA::ORB::_duplicate(orb);
	_TransStatus = CosTransactions::StatusActive;
	_RCV_list_var = new CosTransactions::Coordinator::RCVs();//addded 3 21

	_sm = new SynchronizationManager();//renyi 12 25

	#ifdef _DEBUG_
		cout << "\t[Coordinator::constructor end!]" << endl;
	#endif
    }
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }

}

CosTransactions::Status CosTransactions::Coordinator_impl::get_status()
	throw (CORBA::SystemException,
	CosTransactions::WRONG_TRANSACTION,
	CosTransactions::INVALID_TRANSACTION,
	CosTransactions::TRANSACTION_ROLLEDBACK,
	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your get_status implemention code here

	#ifdef _DEBUG_
		cout << "[Coordinator::get_status is OK!]" << endl;
	#endif

	return _TransStatus;
}

CORBA::Boolean CosTransactions::Coordinator_impl::is_same_transaction(CosTransactions::Coordinator_ptr tc)
	throw (CORBA::SystemException,
	CosTransactions::WRONG_TRANSACTION,
	CosTransactions::INVALID_TRANSACTION,
	CosTransactions::TRANSACTION_ROLLEDBACK,
	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your is_same_transaction implemention code here

	#ifdef _DEBUG_
		cout << "[Coordinator::is_same_transaction is OK!]" << endl;
	#endif

	CosTransactions::Coordinator_var this_temp = this -> _this();

	if (this_temp == tc)
		return true;
	else
		return false;
}

CosTransactions::RecoveryCoordinator_ptr 
CosTransactions::Coordinator_impl::register_resource(Resource_ptr r)
	throw (CORBA::SystemException,
	CosTransactions::WRONG_TRANSACTION,
	CosTransactions::INVALID_TRANSACTION,
	CosTransactions::TRANSACTION_ROLLEDBACK,
	CosTransactions::TRANSACTION_REQUIRED,
	CosTransactions::Inactive)
{
    // TODO:Add your register_resource implemention code here

	#ifdef _DEBUG_
		cout << "[Coordinator::register_resource begin!]" << endl;
	#endif

    try
    {
	
	switch(_TransStatus)
	{
	case CosTransactions::StatusActive:
		{
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is Active!]" << endl;
			#endif
			
			//==================================================================
			//	Has this resource been registered in '_RCV_list_var' before?
			//==================================================================
			CosTransactions::Coordinator::RCV tmp_RCV;
			int tmp_len,i;
			Resource_var tmp_Res_var;
			tmp_len = _RCV_list_var->length();
			
			for(i=0;i<tmp_len;i++)
			{
				tmp_RCV = _RCV_list_var->operator[](i);
				tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
				if((Resource_ptr)tmp_Res_var == r)
				{
					cout << "\t[This resource has been registered before!]" << endl;

					#ifdef _DEBUG_
						cout << "\t[Coordinator::register_resource end!]" << endl;
					#endif
					
						return CosTransactions::RecoveryCoordinator::_duplicate(tmp_RCV.rc);
				}
				
			} //eof 'for'


			//======================================================
			//	This resource has not been registered before,
			// so register this  new resource as following.
			//======================================================
			#ifdef _DEBUG_
				cout << "\t[This resource hasn't been registered before!]" 
				     << endl
				     << "\t[Resource::forget(): initialize resource status]" 
				     << endl;
			#endif
		
			r -> forget();
	
			#ifdef _DEBUG_
				cout << "\t[Resource::BeginTrans]" << endl;
			#endif
		
			if (r -> BeginTrans() == false)
			{
				#ifdef _DEBUG_
					cout << "\t[BeginTrans return false]"
					     << endl
					     << "\t[throw exception: Inactive]"
					     << endl;
				#endif
		
					throw CosTransactions::Inactive();
			}
	
			CosTransactions::RecoveryCoordinator_impl* tmp_RC_var1 = new CosTransactions::RecoveryCoordinator_impl(_this());
			CosTransactions::RecoveryCoordinator_var tmp_RC_var = tmp_RC_var1 -> _this(); 
			tmp_RCV.r = Resource::_duplicate(r);
			tmp_RCV.r -> set_recoverycoordinator (tmp_RC_var.in());
			tmp_RCV.rc = CosTransactions::RecoveryCoordinator::_duplicate(tmp_RC_var);
			tmp_RCV.rv = CosTransactions::RV_NotPrepared;	
			CORBA::ULong tmp_lenth = _RCV_list_var->length();
			_RCV_list_var->length(tmp_lenth+1);
			_RCV_list_var[tmp_lenth] = tmp_RCV;

			#ifdef _DEBUG_
				cout << "\t[A new resource has been registered!]" << endl;
			#endif


			#ifdef _DEBUG_
				cout << "\t[Coordinator::register_resource end!]" << endl;
			#endif

				return CosTransactions::RecoveryCoordinator::_duplicate(tmp_RC_var);
			
			break;
			
		}	//end of case Active


	case CosTransactions::StatusMarkedRollback:
		{
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is MarkedRollback, "
				     << "can not register new resource!]"
				     << endl
				     << "\t[throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]"
				     << endl;
			#endif

				throw CosTransactions::TRANSACTION_ROLLEDBACK();

			break;
		} // end of 'case CosTransactions::StatusMarkedRollback'


		// When the resource has been prepared.
		// include commit or rolling back....
		default:
		{
			#ifdef _DEBUG_
			cout << "	[Current '_TransStatus' is above Prepared, "
				 << "can not register new resource!]"
				 << endl
				 << "	[throw exception: CosTransactions::Inactive]"
				 << endl;
			#endif

			throw CosTransactions::Inactive();
		    	
			break;
			
		}	//end of default
		
	}	//end of switch
	
    }
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }

	
	
}

//renyi 12 25
void
CosTransactions::Coordinator_impl::register_synchronization(
    CosTransactions::Synchronization_ptr sync)
     throw (CORBA::SystemException,
     CosTransactions::SynchronizationUnavailable,
     CosTransactions::Inactive)
{
	#ifdef _DEBUG_
		cout << "[Coordinator::register_synchronization begin!]" << endl;
	#endif

	if(_TransStatus != CosTransactions::StatusActive)
		throw CosTransactions::Inactive();

	_sm -> registerSync(sync);

	#ifdef _DEBUG_
		cout << "[Coordinator::register_synchronization end!]" << endl;
	#endif
}

void CosTransactions::Coordinator_impl::rollback_only()
	 throw (CORBA::SystemException,
	 CosTransactions::WRONG_TRANSACTION,
	 CosTransactions::INVALID_TRANSACTION,
	 CosTransactions::TRANSACTION_ROLLEDBACK,
	 CosTransactions::TRANSACTION_REQUIRED,
	 CosTransactions::Inactive)
{
    // TODO:Add your rollback_only implemention code here

	#ifdef _DEBUG_
		cout << "[Coordinator::rollback_only begin!]" << endl;
	#endif
	
    try
    {

	switch(_TransStatus)
	{
	case CosTransactions::StatusActive:
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is Active, change it to MarkedRollback!]"
					 << endl;
			#endif
				_TransStatus = CosTransactions::StatusMarkedRollback;
			break;

	case CosTransactions::StatusMarkedRollback:
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is already MarkedRollback!]" << endl;
			#endif
			break;

		//all other status is above prepared.
		default:
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is above Prepared, "
					 << "throw Inactive exception!]" << endl;
			#endif
				throw CosTransactions::Inactive();
			break;
			
	}	//end of switch
	
    }
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }


	#ifdef _DEBUG_
		cout << "\t[Coordinator::rollback_only end!]" << endl;
	#endif

	return;
}

void CosTransactions::Coordinator_impl::commit(CORBA::Boolean report_heuristics)
	throw (CORBA::SystemException,
	CosTransactions::WRONG_TRANSACTION,
	CosTransactions::INVALID_TRANSACTION,
	CosTransactions::TRANSACTION_ROLLEDBACK,
	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your commit implemention code here
    
	//???? need to add heuristic decision.
	#ifdef _DEBUG_
		cout << "[Coordinator::commit begin!]" << endl;
	#endif

    int heuristics = 0;
	
	
#ifdef _RECOVERY_

    int tran_id;//the transaction ID of this transaction
    
#endif // _RECOVERY_
	

    try
    {

	switch ( _TransStatus )
	{
		//===============================================================
		//	When the transaction has not prepared, commit or roll
		// back the transaction according to the result of prepare.
		//===============================================================
	case CosTransactions::StatusActive:
		{
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is Active]" << endl;
			#endif

			CORBA::ULong tmp_len;
			tmp_len = _RCV_list_var->length();
			
			#ifdef _DEBUG_
				cout << "\t[The current length of _RCV_list_var is: "
					 << tmp_len << " !]" << endl;
			#endif


#ifdef _RECOVERY_
		
			#ifdef _DEBUG_
				cout << "    ====[write recovery file before prepare!]" << endl;
			#endif

			switch (tmp_len)
			{
				//===============================================
				//	When transaction has no resource....
				//===============================================
				case 0:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list is NULL, need not write recovery file!]" << endl;
					#endif

					break;
				}


				//===============================================
				//	When transaction has resources....
				//===============================================
				default:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list has resources, begin to write recovery file!]" 
							 << endl;
					#endif
					
					//open recovry file
					const char* path = "./recovery_file";
					const int ResNum=10,ResLen=1000;
					
					
					FILE *RecoveryFile = fopen(path,"r+");
					if (RecoveryFile == NULL)
					{
						#ifdef _DEBUG_
							cout << "    ====[can't open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
						
						return; //???? or throw exception
					}
					else
					{
						#ifdef _DEBUG_
							cout << "    ====[succeed open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
	
						int i,flag,len,tran_num,read_num;
						RCV tmp_RCV;
						Resource_var tmp_Res_var;
						fpos_t position;
						
						fread((int *)&tran_num,sizeof(int),1,RecoveryFile);
						#ifdef _DEBUG_
							cout << "    ====[transaction number is: "<<tran_num<< " !]"<<endl;
						#endif
						
						tran_num = tran_num+1;
						rewind(RecoveryFile);
						fwrite((int *)&tran_num,sizeof(int),1,RecoveryFile);
						//write the new transaction number to recovery file;
						
						flag = 1;
						read_num = 1;
						tran_id = 0;//transaction ID of this transaction
						while( !feof(RecoveryFile) && flag != 0 && read_num==1)
						{
							tran_id++;
							fgetpos(RecoveryFile,&position);
							read_num = fread((int *)&flag,sizeof(int),1,RecoveryFile);
							if (read_num == 1)
							{
								if (flag != 0)
								{
									#ifdef _DEBUG_
										cout << "    ====[read_num is 1, transaction flag in recovery file is not 0!]"<<endl;
									#endif
								
									fseek(RecoveryFile,(long int)sizeof(int)*2,SEEK_CUR);
									//the transaction ID and the length of transaction;
									fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
									//the resource list of transaction;
								}
								else
								{
									#ifdef _DEBUG_
										cout << "    ====[read_num is 1, transaction flag in recovery file is 0!]"<<endl;
									#endif
									fsetpos(RecoveryFile,&position);	
								}
							}
							else
							{
								#ifdef _DEBUG_
									cout << "    ====[read_num is not 1, end of file!]"<<endl;
								#endif
							}
							
						}  //end of while
						
												
						#ifdef _DEBUG_
							cout<<"    ====[transaction ID is: "<<tran_id<< " !]"<<endl;
						#endif
						
						flag = 1;
						// flag = 0 means transaction is over;
						// flag = 1 means transaction is preparing;
						// flag = 2 means transaction is committing;
						// flag = 3 means transaction is rollingback;
						len = tmp_len;
						
											
						fwrite((int *)&flag,sizeof(int),1,RecoveryFile);
						fwrite((int *)&tran_id,sizeof(int),1,RecoveryFile);
						fwrite((int *)&len,sizeof(int),1,RecoveryFile);
						
						for(i=0;i<len;i++)
						{
							tmp_RCV = _RCV_list_var->operator[](i);
							tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
							
							CORBA::String_var Res_string = _orb_var -> object_to_string(tmp_Res_var);
							
							fwrite(Res_string,sizeof(char)*ResLen,1,RecoveryFile);
							
						}
		
						
						
						fclose(RecoveryFile);
						
					}  //end of else
					
					break;
				
				}  //end of default
			
			}  //end of switch
			
			#ifdef _DEBUG_
				cout<<"    ====[end of write recovery file before prepare!]"<<endl;
			#endif		
			
			#ifdef _DEBUG_RECOVERY_
				cout<<"    ====[please press any key to continue!]"<<endl;
				getchar();
			#endif

#endif // _RECOVERY_

			_sm -> beforeCompletion(); //renyi 12 25

			#ifdef _DEBUG_
				cout<<"********[begin to send prepare to resource!]"<<endl;
			#endif
			
			
			switch (tmp_len)
			{
				//===============================================================
				//	When transaction has no resource....
				//===============================================================
				case 0:
				{
					#ifdef _DEBUG_
						cout << "	[Resource list is NULL, transaction has committed!]" << endl;
					#endif

						_TransStatus=CosTransactions::StatusNoTransaction;

					break;
				}


				//====================================================
				//	When transaction has only one resource....
				//====================================================
				case 1:
				{
					#ifdef _DEBUG_
						cout << "\t[Resource list has only one resource!]" << endl;
					#endif

					CosTransactions::Coordinator::RCV tmp_RCV;
					Resource_var tmp_Res_var;
					tmp_RCV = _RCV_list_var->operator[](0);
					tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
					
					#ifdef _DEBUG_
						cout << "\t[The only resource begin to 'commit_one_phase'!]"
							 << endl;
					#endif
				
					try
					{
						tmp_Res_var -> commit_one_phase();
					}
					catch (CORBA::OBJECT_NOT_EXIST)
					{
						#ifdef _DEBUG_
							cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
						#endif
						
						heuristics = 1;
	
					 }
					 catch (CORBA::COMM_FAILURE)
					 {
						#ifdef _DEBUG_
							cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
						#endif
	
						heuristics = 1;
					 }
					
					if (report_heuristics && heuristics == 1)
					{
						#ifdef _DEBUG_
							cout << "\t[Throw exception: CosTransactions::HeuristicHazard]" << endl;
						#endif
					
							throw CosTransactions::HeuristicHazard();
					}
#ifdef _RECOVERY_

			#ifdef _DEBUG_
				cout << "    ====[begin to delete transaction in recovery file after commit_one_phase!]" << endl;
				cout << "    ====[transaction ID is: "<<tran_id<< " !]" << endl;
			#endif
			
			switch (tmp_len)
			{
				//====================================================
				//	When transaction has no resource....
				//====================================================

				case 0:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list is NULL, need not updata recovery file!]" << endl;
					#endif

					break;
				}

				//====================================================
				//	When transaction has resources....
				//====================================================

				default:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list has resources, begin to updata recovery file!]" 
							 << endl;
					#endif
					
					//open recovry file
					const char* path = "./recovery_file";
					const int ResNum=10,ResLen=1000;
					
					
					FILE *RecoveryFile = fopen(path,"r+");
					if (RecoveryFile == NULL)
					{
						#ifdef _DEBUG_
							cout<<"    ====[can't open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
						
						return; //???? or throw exception
					}
					else
					{
						#ifdef _DEBUG_
							cout<<"    ====[succeed open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
	
						int flag,tran_num,read_num,tmp_tran_id;
						RCV tmp_RCV;
						Resource_var tmp_Res_var;
						fpos_t position;
						
						fgetpos(RecoveryFile,&position);
						fread((int *)&tran_num,sizeof(int),1,RecoveryFile);
						#ifdef _DEBUG_
							cout<<"    ====[transaction number is: "<<tran_num<< " !]"<<endl;
						#endif
						
						tran_num = tran_num - 1;
						fsetpos(RecoveryFile,&position);
						fwrite((int *)&tran_num,sizeof(int),1,RecoveryFile);
						// write the new transaction number to recovery file
						
						read_num = 1;
						tmp_tran_id = 0;
						
						while( !feof(RecoveryFile) && (tmp_tran_id != tran_id) && read_num==1)
						{
							fgetpos(RecoveryFile,&position);
							read_num = fread((int *)&flag,sizeof(int),1,RecoveryFile);
							if (read_num == 1)
							{
								if (flag != 1)
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is not 1!]"<<endl;
									#endif
								
									fseek(RecoveryFile,(long int)sizeof(int)*2,SEEK_CUR);
									//the transaction ID and the length of transaction;
									fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
									//the resource list of transaction;
								}
								else
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is 1!]"<<endl;
									#endif
									fread((int *)&tmp_tran_id,sizeof(int),1,RecoveryFile);
									
										if (tmp_tran_id != tran_id)
										{
											#ifdef _DEBUG_
												cout<<"    ====[not the same transaction!]"<<endl;
											#endif
								
											fseek(RecoveryFile,(long int)sizeof(int),SEEK_CUR);
											//the transaction ID and the length of transaction;
											fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
											//the resource list of transaction;
										}
										else
										{
											#ifdef _DEBUG_
												cout<<"    ====[find the same transaction!]"<<endl;
											#endif
											
											fsetpos(RecoveryFile,&position);	
											flag = 0;
											// flag = 0 means transaction is over;
											// flag = 1 means transaction is preparing;
											// flag = 2 means transaction is committing;
											// flag = 3 means transaction is rollingback;
											
											fwrite((int *)&flag,sizeof(int),1,RecoveryFile);
						
										}
									
									
								} // end of else (flag = 1)

							} // end of if (read_num == 1)

							else
							{
								#ifdef _DEBUG_
									cout<<"    ====[end of file, can not find the same transaction in recovery file!]"<<endl;
								#endif
							}
							
						}  //end of while
						
												
						fclose(RecoveryFile);
						
					}  //end of else (RecoveryFile != NULL)
					
					break;
				
				}  //end of default
			
			}  //end of switch
			
			#ifdef _DEBUG_
				cout<<"    ====[end of delete transaction in recovery file after commit_one_phase!]"<<endl;
			#endif
			
			#ifdef _DEBUG_
				cout<<"********[end of commit one phase!]"<<endl;
			#endif

			#endif // _RECOVERY_
					
					

					break;
				}


				//=======================================================
				//	When transaction has more than one resource....
				//=======================================================

				default:
				{
					#ifdef _DEBUG_
						cout << "\t[Resource list has more than one resources!]" << endl;
					#endif
	
					//=======================================================
					//	Begin to prepare all resources in list.
					//=======================================================
					#ifdef _DEBUG_
						cout << "\t[Preparing all resources in resource list!]" << endl;
					#endif

						_TransStatus=CosTransactions::StatusPreparing;
					CosTransactions::Coordinator::RCV tmp_RCV;
					CORBA::ULong i;
					Resource_var tmp_Res_var;
					CosTransactions::Vote tmp_vote;
					i = 0;
					while ((i<tmp_len) && 
						(_TransStatus != CosTransactions::StatusMarkedRollback))
					{
						i++;
						
						//tmp_RCV = _RCV_list_var->operator[](0);
						//Suliang add.
						tmp_RCV = _RCV_list_var[i-1];
						//Suliang add.
						tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
						
						try
						{
							tmp_vote = tmp_Res_var -> prepare();
						}
						catch (CORBA::OBJECT_NOT_EXIST)
						{
							#ifdef _DEBUG_
								cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]"
								     << endl
								     << "\t[This resource is supposed to vote rollback.]"
								     << endl;
							#endif
						
								tmp_vote = CosTransactions::VoteRollback;
								
						 }
						 catch (CORBA::COMM_FAILURE)
						{
							#ifdef _DEBUG_
								cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]"
								     << endl
								     << "\t[This resource is supposed to vote rollback.]"
								     << endl;
							#endif
	
								tmp_vote = CosTransactions::VoteRollback;
								
						 }
					
						switch(tmp_vote)
						{
								case CosTransactions::VoteCommit:
								tmp_RCV.rv = CosTransactions::RV_Vote_Commit;
							//	CosTransactions::Coordinator::remove(_RCV_list_var,0);
								
								//_RCV_list_var->length(_RCV_list_var->length()+1);
								//_RCV_list_var[_RCV_list_var->length()] = tmp_RCV;
																
								//SuLiang add.
								//int d = _RCV_list_var->length();								
//								_RCV_list_var->length(_RCV_list_var->length()+1);
						//		_RCV_list_var[_RCV_list_var->length()-1] = tmp_RCV;
								//SuLiang add.
								 _RCV_list_var[i-1].rv= tmp_RCV.rv;
								break;

								case CosTransactions::VoteRollback:
								tmp_RCV.rv = CosTransactions::RV_RolledBack;
			/*					CosTransactions::Coordinator::remove(_RCV_list_var,0);
								_RCV_list_var->length(_RCV_list_var->length()+1);
								_RCV_list_var[_RCV_list_var->length()] = tmp_RCV;
								*/
								_RCV_list_var[i-1].rv= tmp_RCV.rv;
								_TransStatus = CosTransactions::StatusMarkedRollback;
								break;

								case CosTransactions::VoteReadOnly:
								tmp_RCV.rv = CosTransactions::RV_Vote_ReadOnly;
				/*				CosTransactions::Coordinator::remove(_RCV_list_var,0);
								_RCV_list_var->length(_RCV_list_var->length()+1);
								_RCV_list_var[_RCV_list_var->length()] = tmp_RCV;
								*/
								_RCV_list_var[i-1].rv= tmp_RCV.rv;
								break;
								
						} // eof switch(tmp_vote)

					}	// end of while
	
					//=======================================================
					//  After prepare, Coordinator will make its decision.
					//=======================================================


					//=======================================================
					//	Because at least one resource, which is in
					// this transaction, has voted 'rollback', Coordinator 
					// decide to roll back all resources in list.
					//=======================================================
					if ( _TransStatus == CosTransactions::StatusMarkedRollback )
					{
						rollback();
						
						#ifdef _DEBUG_
							cout << "	[Transaction has rolled back!]"
								 << endl
								 << "	[throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]"
								 << endl;
						#endif

							throw CosTransactions::TRANSACTION_ROLLEDBACK();
					}
					//=======================================================
					//	For all resources in this transaction have
					// voted 'commit' or 'readonly', Coordinator decide
					// to commit all resources in list.
					//=======================================================
					else	
					{
					
					
						
#ifdef _RECOVERY_

			#ifdef _DEBUG_
				cout << "    ====[write recovery file before commit!]" << endl;
				cout << "    ====[transaction ID is: "<<tran_id<< " !]" << endl;
			#endif

			switch (tmp_len)
			{
				//==================================================
				//	When transaction has no resource....
				//==================================================

				case 0:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list is NULL, need not write recovery file!]" << endl;
					#endif

					break;
				}


				//==================================================
				//	When transaction has resources....
				//==================================================

				default:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list has resources, begin to write recovery file!]" 
							 << endl;
					#endif
					
					//open recovry file
					const char* path = "./recovery_file";
					const int ResNum=10,ResLen=1000;
					
					
					FILE *RecoveryFile = fopen(path,"r+");
					if (RecoveryFile == NULL)
					{
						#ifdef _DEBUG_
							cout<<"    ====[can't open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
						
						return; //???? or throw exception
					}
					else
					{
						#ifdef _DEBUG_
							cout<<"    ====[succeed open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
	
						int flag,tran_num,read_num,tmp_tran_id;
						RCV tmp_RCV;
						Resource_var tmp_Res_var;
						fpos_t position;
						
						fread((int *)&tran_num,sizeof(int),1,RecoveryFile);
						#ifdef _DEBUG_
							cout<<"    ====[transaction number is: "<<tran_num<< " !]"<<endl;
						#endif
						
						read_num = 1;
						tmp_tran_id = 0;
						
						while( !feof(RecoveryFile) && (tmp_tran_id != tran_id) && read_num==1)
						{
							fgetpos(RecoveryFile,&position);
							read_num = fread((int *)&flag,sizeof(int),1,RecoveryFile);
							if (read_num == 1)
							{
								if (flag != 1)
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is not 1!]"<<endl;
									#endif
								
									fseek(RecoveryFile,(long int)sizeof(int)*2,SEEK_CUR);
									//the transaction ID and the length of transaction;
									fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
									//the resource list of transaction;
								}
								else
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is 1!]"<<endl;
									#endif
									fread((int *)&tmp_tran_id,sizeof(int),1,RecoveryFile);
									
										if (tmp_tran_id != tran_id)
										{
											#ifdef _DEBUG_
												cout<<"    ====[not the same transaction!]"<<endl;
											#endif
								
											fseek(RecoveryFile,(long int)sizeof(int),SEEK_CUR);
											//the transaction ID and the length of transaction;
											fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
											//the resource list of transaction;
										}
										else
										{
											#ifdef _DEBUG_
												cout<<"    ====[find the same transaction!]"<<endl;
											#endif
											
											fsetpos(RecoveryFile,&position);	
											flag = 2;
											// flag = 0 means transaction is over;
											// flag = 1 means transaction is preparing;
											// flag = 2 means transaction is committing;
											// flag = 3 means transaction is rollingback;
											
											fwrite((int *)&flag,sizeof(int),1,RecoveryFile);
						
										}
									
									
								} // end of else (flag = 1)

							} // end of if (read_num == 1)

							else
							{
								#ifdef _DEBUG_
									cout<<"    ====[read_num is not 1, end of file!]"<<endl;
								#endif
							}
							
						}  //end of while
						
												
						fclose(RecoveryFile);
						
					}  //end of else (RecoveryFile != NULL)
					
					break;
				
				}  //end of default
			
			}  //end of switch
			
			#ifdef _DEBUG_
				cout<<"    ====[end of write recovery file before commit!]"<<endl;
			#endif
			
			#ifdef _DEBUG_
				cout<<"********[begin to send commit to resource!]"<<endl;
			#endif
			
			#ifdef _DEBUG_RECOVERY_
				cout<<"    ====[please press any key to continue!]"<<endl;
				getchar();
			#endif
			

#endif // _RECOVERY_
					    
					    
					    
				_TransStatus = CosTransactions::StatusCommitting;
						
						#ifdef _DEBUG_
							cout << "\t[Current tmp_len is: " 
								 << tmp_len << " !]"
								 << endl;
						#endif

						for(i=0;i<tmp_len;i++)
						{
							tmp_RCV = _RCV_list_var->operator[](i);
							tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
							if (tmp_RCV.rv != CosTransactions::RV_Vote_Rollback)
							{	
								try
								{
									tmp_Res_var -> commit();
									change_resourcevote (tmp_Res_var.in(), CosTransactions::RV_Committed);
								}
								catch (CORBA::OBJECT_NOT_EXIST)
								{
									#ifdef _DEBUG_
										cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
									#endif
						
									heuristics = 1;
	
								 }
								 catch (CORBA::COMM_FAILURE)
								 {
									#ifdef _DEBUG_
										cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
									#endif
	
									heuristics = 1;
	
								 }
							
								
							}	//end of if
							
						}	//end of for

						_TransStatus = CosTransactions::StatusNoTransaction;
						


						
#ifdef _RECOVERY_

			#ifdef _DEBUG_
				cout << "    ====[begin to delete transaction in recovery file after commit!]" << endl;
				cout << "    ====[transaction ID is: "<<tran_id<< " !]" << endl;
			#endif

			switch (tmp_len)
			{
				//=================================================
				//	When transaction has no resource....
				//=================================================

				case 0:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list is NULL, need not updata recovery file!]" << endl;
					#endif

					break;
				}

				//=================================================
				//	When transaction has resources....
				//=================================================
				
				default:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list has resources, begin to updata recovery file!]" 
							 << endl;
					#endif
					
					//open recovry file
					const char* path = "./recovery_file";
					const int ResNum=10,ResLen=1000;
					
					
					FILE *RecoveryFile = fopen(path,"r+");
					if (RecoveryFile == NULL)
					{
						#ifdef _DEBUG_
							cout<<"    ====[can't open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
						
						return; //???? or throw exception
					}
					else
					{
						#ifdef _DEBUG_
							cout<<"    ====[succeed open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
	
						int flag,tran_num,read_num,tmp_tran_id;
						RCV tmp_RCV;
						Resource_var tmp_Res_var;
						fpos_t position;
						
						fgetpos(RecoveryFile,&position);
						fread((int *)&tran_num,sizeof(int),1,RecoveryFile);
						#ifdef _DEBUG_
							cout<<"    ====[transaction number is: "<<tran_num<< " !]"<<endl;
						#endif
						
						tran_num = tran_num - 1;
						fsetpos(RecoveryFile,&position);
						fwrite((int *)&tran_num,sizeof(int),1,RecoveryFile);
						// write the new transaction number to recovery file
						
						read_num = 1;
						tmp_tran_id = 0;
						
						while( !feof(RecoveryFile) && (tmp_tran_id != tran_id) && read_num==1)
						{
							fgetpos(RecoveryFile,&position);
							read_num = fread((int *)&flag,sizeof(int),1,RecoveryFile);
							if (read_num == 1)
							{
								if (flag != 2)
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is not 2!]"<<endl;
									#endif
								
									fseek(RecoveryFile,(long int)sizeof(int)*2,SEEK_CUR);
									//the transaction ID and the length of transaction;
									fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
									//the resource list of transaction;
								}
								else
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is 2!]"<<endl;
									#endif
									fread((int *)&tmp_tran_id,sizeof(int),1,RecoveryFile);
									
										if (tmp_tran_id != tran_id)
										{
											#ifdef _DEBUG_
												cout<<"    ====[not the same transaction!]"<<endl;
											#endif
								
											fseek(RecoveryFile,(long int)sizeof(int),SEEK_CUR);
											//the transaction ID and the length of transaction;
											fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
											//the resource list of transaction;
										}
										else
										{
											#ifdef _DEBUG_
												cout<<"    ====[find the same transaction!]"<<endl;
											#endif
											
											fsetpos(RecoveryFile,&position);	
											flag = 0;
											// flag = 0 means transaction is over;
											// flag = 1 means transaction is preparing;
											// flag = 2 means transaction is committing;
											// flag = 3 means transaction is rollingback;
											
											fwrite((int *)&flag,sizeof(int),1,RecoveryFile);
						
										}
									
									
								} // end of else (flag = 1)

							} // end of if (read_num == 1)

							else
							{
								#ifdef _DEBUG_
									cout<<"    ====[end of file, can not find the same transaction in recovery file!]"<<endl;
								#endif
							}
							
						}  //end of while
						
												
						fclose(RecoveryFile);
						
					}  //end of else (RecoveryFile != NULL)
					
					break;
				
				}  //end of default
			
			}  //end of switch
			
			#ifdef _DEBUG_
				cout<<"    ====[end of delete transaction in recovery file after commit!]"<<endl;
				cout<<"********[end of commit!]"<<endl;
			#endif
			
			#ifdef _DEBUG_RECOVERY_
				cout<<"    ====[please press any key to continue!]"<<endl;
				getchar();
			#endif
			
#endif // _RECOVERY_

            _sm -> afterCompletion(_TransStatus); //renyi 12 25		
						
						if (report_heuristics && heuristics == 1)
						{
							#ifdef _DEBUG_
								cout << "\t[Throw exception: CosTransactions::HeuristicHazard]" << endl;
							#endif
						
								throw CosTransactions::HeuristicHazard();
						}

					}	//end of else(do committing)

					break;

				}	//end of default: resource list is more than one

			}	// end of switch (tmp_len)

			break;
		}	// end of case Active


		//================================================================
		//	There are two situations as following.
		// 1.	Before 'prepare' there is an error occured such
		//	as operation failure in Recoverable Object service.
		//	The service function marked the transaction.
		// 2.	The 'pthread' for time out marked the transaction
		//	befor the sending of 'commit' operation, because 
		//	the transaction has not finished after time_out.
		//================================================================
		case CosTransactions::StatusMarkedRollback:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is MarkedRollback!]" << endl;
			#endif

			//rollback the transaction
			rollback();
			
			#ifdef _DEBUG_
				cout << "\t[Throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]" << endl;
			#endif
			
				throw CosTransactions::TRANSACTION_ROLLEDBACK();
			break;


		case CosTransactions::StatusCommitted:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is Commited!]" << endl;
			#endif
			break;

		case CosTransactions::StatusNoTransaction:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is NoTransaction!]" << endl;
			#endif
			break;


		case CosTransactions::StatusRolledBack:
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is RolledBack!]"
				     << endl
				     << "\t[Throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]" 
				     << endl;
			#endif
			
				throw CosTransactions::TRANSACTION_ROLLEDBACK();
			break;


		case CosTransactions::StatusPreparing:
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is Preparing!]" << endl;
			#endif
			break;


		//==============================================================
		//	There are two satuations same as following:
		// 1.	There is a 'COMM_FAILURE' exception after Coord send
		//	the first 'commit' command and before Coord receive the 
		//	return value. So Coord send 'commit' for more time after 
		//	a period of waiting while the first 'commit' has not 
		//	finished.
		// 2.	While the 'commit' operation hasn't finished, the computer
		// 	containing 'Coordinator' shut down. When the 'Coordinator'
		//	is recoveried, it will commit again in this 'case'.
		//==============================================================
		
		case CosTransactions::StatusCommitting:
		{
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is Commiting!]" << endl;
			#endif

			CosTransactions::Coordinator::RCV tmp_RCV;
			CORBA::ULong tmp_len,i;
			Resource_var tmp_Res_var;
			tmp_len = _RCV_list_var->length();
			i = 0;
			for(i=0; i<tmp_len; i++)
			{
				tmp_RCV = _RCV_list_var->operator[](i);
				tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
				switch (tmp_RCV.rv)
				{
					case CosTransactions::RV_Vote_Commit:
						tmp_Res_var -> commit();
						change_resourcevote (tmp_Res_var.in(), CosTransactions::RV_Committed);
						break;

					default:
						break;

				}	//end of switch (tmp_RCV.rv)

			}	//end of for

			_TransStatus = CosTransactions::StatusNoTransaction;

			break;
			
		}	// end of case: Committing


		case CosTransactions::StatusRollingBack:
		{
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is RollingBack!]" 
					 << endl
					 << "\t[throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]"
					 << endl;
			#endif

			do {}
			while (_TransStatus != CosTransactions::StatusRolledBack);
			
			throw CosTransactions::TRANSACTION_ROLLEDBACK();
			break;
		}


		default:
		{
			#ifdef _DEBUG_
				cout << "\t[current '_TransStatus' is Unknown]" << endl;
			#endif
			break;
		}
		
	}	// end of switch (_TransStatus)
	
    }
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }


	#ifdef _DEBUG_
		cout << "	[Coordinator::commit end!]"
			 << endl;
	#endif

	return;
}	// end of commit()



void CosTransactions::Coordinator_impl::rollback()
	throw(CosTransactions::TRANSACTION_REQUIRED,
              CosTransactions::TRANSACTION_ROLLEDBACK,
              CosTransactions::INVALID_TRANSACTION,
              CosTransactions::WRONG_TRANSACTION,
              CORBA::SystemException)
{
    // TODO:Add your rollback implemention code here
    
	#ifdef _DEBUG_
		cout << "[Coordinator::rollback begin!]"
			 << endl;
	#endif

	int heuristics = 0;
	
	
#ifdef _RECOVERY_
	int tran_id;//the transaction ID of this transaction
#endif // _RECOVERY_


    try
    {

	switch(_TransStatus)
	{
		case CosTransactions::StatusActive:
		case CosTransactions::StatusMarkedRollback:
		{
			#ifdef _DEBUG_
				cout << "\t[current status is Active or MarkedRollback, begin rollback.]"
					 << endl;
			#endif

			//==================================================
			//	Begin to rollback this transaction.....
			//==================================================
			
			_TransStatus = CosTransactions::StatusRollingBack;
			CosTransactions::Coordinator::RCV tmp_RCV;
			CORBA::ULong tmp_len,i;
			Resource_var tmp_Res_var;
			tmp_len = _RCV_list_var->length();



#ifdef _RECOVERY_

			#ifdef _DEBUG_
				cout << "    ====[write recovery file before rollback!]" << endl;
			#endif

			switch (tmp_len)
			{
				//==================================================
				//	When transaction has no resource....
				//==================================================

				case 0:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list is NULL, need not write recovery file!]" << endl;
					#endif

					break;
				}


				//==================================================
				//	When transaction has resources....
				//==================================================

				default:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list has resources, begin to write recovery file!]" 
							 << endl;
					#endif
					
					//open recovry file
					const char* path = "./recovery_file";
					const int ResNum=10,ResLen=1000;
					
					
					FILE *RecoveryFile = fopen(path,"r+");
					if (RecoveryFile == NULL)
					{
						#ifdef _DEBUG_
							cout<<"    ====[can't open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
						
						return; 
					}
					else
					{
						#ifdef _DEBUG_
							cout<<"    ====[succeed open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
	
						int i,flag,len,tran_num,read_num;
						RCV tmp_RCV;
						Resource_var tmp_Res_var;
						fpos_t position;
						
						fread((int *)&tran_num,sizeof(int),1,RecoveryFile);
						#ifdef _DEBUG_
							cout<<"    ====[transaction number is: "<<tran_num<< " !]"<<endl;
						#endif
						
						tran_num = tran_num+1;
						rewind(RecoveryFile);
						fwrite((int *)&tran_num,sizeof(int),1,RecoveryFile);
						//write the new transaction number to recovery file;
						
						flag = 1;
						read_num = 1;
						while( !feof(RecoveryFile) && flag != 0 && read_num==1)
						{
							fgetpos(RecoveryFile,&position);
							read_num = fread((int *)&flag,sizeof(int),1,RecoveryFile);
							if (read_num == 1)
							{
								if (flag != 0)
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is not 0!]"<<endl;
									#endif
								
									fseek(RecoveryFile,(long int)sizeof(int)*2,SEEK_CUR);
									//the transaction ID and the length of transaction;
									fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
									//the resource list of transaction;
								}
								else
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is 0!]"<<endl;
									#endif
									fsetpos(RecoveryFile,&position);	
								}
							}
							else
							{
								#ifdef _DEBUG_
									cout<<"    ====[read_num is not 1, end of file!]"<<endl;
								#endif
							}
							
						}  //end of while
						
												
						tran_id = tran_num;//transaction ID of this transaction
						#ifdef _DEBUG_
							cout<<"    ====[transaction ID is: "<<tran_id<< " !]"<<endl;
						#endif
						
						flag = 3;
						// flag = 0 means transaction is over;
						// flag = 1 means transaction is preparing;
						// flag = 2 means transaction is committing;
						// flag = 3 means transaction is rollingback;
						len = tmp_len;
						
											
						fwrite((int *)&flag,sizeof(int),1,RecoveryFile);
						fwrite((int *)&tran_id,sizeof(int),1,RecoveryFile);
						fwrite((int *)&len,sizeof(int),1,RecoveryFile);
						
						for(i=0;i<len;i++)
						{
							tmp_RCV = _RCV_list_var->operator[](i);
							tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
							
							CORBA::String_var Res_string = _orb_var -> object_to_string(tmp_Res_var);
							
							fwrite(Res_string,sizeof(char)*ResLen,1,RecoveryFile);
							
						}
		
						
						
						fclose(RecoveryFile);
						
					}  //end of else
					
					break;
				
				}  //end of default
			
			}  //end of switch
			
			#ifdef _DEBUG_
				cout<<"    ====[end of write recovery file before rollback!]"<<endl;
			#endif
			
#endif // _RECOVERY_



			for(i=0;i<tmp_len;i++)
			{
				tmp_RCV = _RCV_list_var->operator[](i);
				tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
				switch(tmp_RCV.rv)
				{
					case CosTransactions::RV_Vote_Commit:
					case CosTransactions::RV_NotPrepared:
					   {
						try
						{
							tmp_Res_var -> rollback();
							change_resourcevote (tmp_Res_var.in(), CosTransactions::RV_RolledBack);
						}
						catch (CORBA::OBJECT_NOT_EXIST)
						{
							#ifdef _DEBUG_
								cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
							#endif
						
							heuristics = 1;
	
						 }
						 catch (CORBA::COMM_FAILURE)
						 {
							#ifdef _DEBUG_
								cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
							#endif
	
							heuristics = 1;
						 }
						
						break;
						
					    }//end of  case

					default:
						break;
						
				} //eof 'switch'

			} //eof 'for'

			_TransStatus = CosTransactions::StatusNoTransaction;


#ifdef _RECOVERY_

			#ifdef _DEBUG_
				cout << "    ====[begin to delete transaction in recovery file after rollback!]" << endl;
			#endif

			switch (tmp_len)
			{
				//==================================================
				//	When transaction has no resource....
				//==================================================

				case 0:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list is NULL, need not updata recovery file!]" << endl;
					#endif

					break;
				}


				//==================================================
				//	When transaction has resources....
				//==================================================

				default:
				{
					#ifdef _DEBUG_
						cout << "    ====[Resource list has resources, begin to updata recovery file!]" 
							 << endl;
					#endif
					
					//open recovry file
					const char* path = "./recovery_file";
					const int ResNum=10,ResLen=1000;
					
					
					FILE *RecoveryFile = fopen(path,"r+");
					if (RecoveryFile == NULL)
					{
						#ifdef _DEBUG_
							cout<<"    ====[can't open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
						
						return; //???? or throw exception
					}
					else
					{
						#ifdef _DEBUG_
							cout<<"    ====[succeed open file:  "<<path <<"  with mode of write!]"<<endl;
						#endif
	
						int flag,tran_num,read_num,tmp_tran_id;
						RCV tmp_RCV;
						Resource_var tmp_Res_var;
						fpos_t position;
						
						fgetpos(RecoveryFile,&position);
						fread((int *)&tran_num,sizeof(int),1,RecoveryFile);
						#ifdef _DEBUG_
							cout<<"    ====[transaction number is: "<<tran_num<< " !]"<<endl;
						#endif
						
						tran_num = tran_num - 1;
						fsetpos(RecoveryFile,&position);
						fwrite((int *)&tran_num,sizeof(int),1,RecoveryFile);
						// write the new transaction number to recovery file
						
						read_num = 1;
						tmp_tran_id = 0;
						
						while( !feof(RecoveryFile) && (tmp_tran_id != tran_id) && read_num==1)
						{
							fgetpos(RecoveryFile,&position);
							read_num = fread((int *)&flag,sizeof(int),1,RecoveryFile);
							if (read_num == 1)
							{
								if (flag != 3)
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is not 3!]"<<endl;
									#endif
								
									fseek(RecoveryFile,(long int)sizeof(int)*2,SEEK_CUR);
									//the transaction ID and the length of transaction;
									fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
									//the resource list of transaction;
								}
								else
								{
									#ifdef _DEBUG_
										cout<<"    ====[read_num is 1, transaction flag in recovery file is 3!]"<<endl;
									#endif
									fread((int *)&tmp_tran_id,sizeof(int),1,RecoveryFile);
									
										if (tmp_tran_id != tran_id)
										{
											#ifdef _DEBUG_
												cout<<"    ====[not the same transaction!]"<<endl;
											#endif
								
											fseek(RecoveryFile,(long int)sizeof(int),SEEK_CUR);
											//the transaction ID and the length of transaction;
											fseek(RecoveryFile,(long int)ResLen*ResNum*sizeof(char),SEEK_CUR);
											//the resource list of transaction;
										}
										else
										{
											#ifdef _DEBUG_
												cout<<"    ====[find the same transaction!]"<<endl;
											#endif
											
											fsetpos(RecoveryFile,&position);	
											flag = 0;
											// flag = 0 means transaction is over;
											// flag = 1 means transaction is preparing;
											// flag = 2 means transaction is committing;
											// flag = 3 means transaction is rollingback;
											
											fwrite((int *)&flag,sizeof(int),1,RecoveryFile);
						
										}
									
									
								} // end of else (flag = 1)

							} // end of if (read_num == 1)

							else
							{
								#ifdef _DEBUG_
									cout<<"    ====[end of file, can not find the same transaction in recovery file!]"<<endl;
								#endif
							}
							
						}  //end of while
						
												
						fclose(RecoveryFile);
						
					}  //end of else (RecoveryFile != NULL)
					
					break;
				
				}  //end of default
			
			}  //end of switch
			
			#ifdef _DEBUG_
				cout<<"    ====[end of delete transaction in recovery file after rollback!]"<<endl;
			#endif

#endif // _RECOVERY_


			
			if ( heuristics == 1)
			{
				#ifdef _DEBUG_
					cout << "\t[Throw exception: CosTransactions::HeuristicHazard]" << endl;
				#endif
				
					throw CosTransactions::HeuristicHazard();
			}
			
			break;
			
		} // eof 'case: Active and MarkedRollback'


		case CosTransactions::StatusCommitted:
		{
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is Commited, can not rollback!]"
				     << endl
				     << "\t[Throw exception: CosTransactions::WRONG_TRANSACTION]"
				     << endl;
			#endif
			
				throw CosTransactions::WRONG_TRANSACTION();
			break;
		}

		case CosTransactions::StatusNoTransaction:
		{
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is NoTransaction, can not rollback!]"
				     << endl
				     << "\t[Throw exception: CosTransactions::INVALID_TRANSACTION]"
				     << endl;
			#endif
			
				throw CosTransactions::INVALID_TRANSACTION();
			break;
		}


		//=================================================================
		//	There are two satuations same as following:
		// 1.	There is a 'COMM_FAILURE' exception after Coord send
		//	the first 'rollback' command and before Coord receive the 
		//	return value. So Coord send 'rollback' for more time after 
		//	a period of waiting while the first 'rollback' has not 
		//	finished.
		// 2.	While the 'rollback' operation hasn't finished, the computer
		//	containing 'Coordinator' shut down. When the 'Coordinator'
		//	is recoveried, it will rollback again in this 'case'.
		//=================================================================
		
		case CosTransactions::StatusRollingBack:
		{
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is RollingBack, roll back resources left!]" 
					 << endl;
			#endif
			
			CosTransactions::Coordinator::RCV tmp_RCV;
			CORBA::ULong tmp_len,i;
			Resource_var tmp_Res_var;
			tmp_len = _RCV_list_var->length();

			for ( i=0; i<tmp_len; i++ )
			{
				tmp_RCV = _RCV_list_var->operator[](i);
				tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
				switch(tmp_RCV.rv)
				{
					case CosTransactions::RV_Vote_Commit:
					case CosTransactions::RV_NotPrepared:
						tmp_Res_var -> rollback();
						change_resourcevote (tmp_Res_var.in(), CosTransactions::RV_RolledBack);
						break;

					default: break;
						
				} // eof switch (tmp_RCV.rv)

			}	//end of for

			_TransStatus = CosTransactions::StatusNoTransaction;
			
			break;
			
		} // eof 'case RollingBack'
	

		case CosTransactions::StatusRolledBack:
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is RolledBack!]" << endl;
			#endif
			return;
			break;

		default:
			#ifdef _DEBUG_
				cout << "\t[Current '_TransStatus' is Unknown!]" << endl;
			#endif

			return;
			break;
			
	} // eof 'switch'
	

	#ifdef _DEBUG_
		cout << "\t[Coordinator::rollback end!]" << endl;
	#endif

	_sm -> afterCompletion(_TransStatus); //renyi 12 25
	
	return;
    }
    
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }

}

void CosTransactions::Coordinator_impl::change_resourcevote(Resource_ptr r, CosTransactions::ResourceVote rv)
	throw (CORBA::SystemException,
	CosTransactions::WRONG_TRANSACTION,
	CosTransactions::INVALID_TRANSACTION,
	CosTransactions::TRANSACTION_ROLLEDBACK,
	CosTransactions::TRANSACTION_REQUIRED)	
{
    // TODO:Add your change_resourcevote implemention code here
    
	#ifdef _DEBUG_
		cout << "[Coordinator::change_resourcevote begin!]" << endl;
	#endif

    try
    {
	CosTransactions::Coordinator::RCV tmp_RCV;
	//CORBA::ULong tmp_len,i;
	Resource_var tmp_Res_var;
	CORBA::ULong tmp_len = (CORBA::ULong) _RCV_list_var->length();

	for ( int i=0; i<tmp_len; i++ )
	{
		tmp_RCV = _RCV_list_var->operator[](i);
		tmp_Res_var = Resource::_duplicate(tmp_RCV.r);
		if((Resource_ptr)tmp_Res_var == r)
		{
			tmp_RCV.rv = rv;
			_RCV_list_var[i] = tmp_RCV;

			#ifdef _DEBUG_
				cout << "\t[This resource vote in _RVC_list has been changed!]" << endl;
			#endif

			#ifdef _DEBUG_
				cout << "\t[Coordinator::change_resourcevote end!]" << endl;
			#endif

			return;
		}
		
	} //eof 'for'
	

	#ifdef _DEBUG_
		cout << "\t[This resource does not exit in _RVC_list!]"
		     << endl
		     << "\t[Coordinator::change_resourcevote end!]"
		     << endl;
	#endif

	return;
    }
    
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }

}

void CosTransactions::Coordinator_impl::release ()
	 throw (CORBA::SystemException)
{
    // TODO:Add your release implemention code here
	
	#ifdef _DEBUG_
		cout << "[Coordinator::release begin!]" << endl;
	#endif
	
	CORBA::ULong RefCount = _OB_getRef ();

	#ifdef _DEBUG_
		cout << "\t[this Coordinator object reference count is: "
		     << RefCount << "]" << endl;
	#endif
	
	int len = _RCV_list_var -> length();
	
	#ifdef _DEBUG_
		cout << "\t[length of _RCV_list_var is: "
		     << len << "]" << endl;
	#endif
	
	for (int idx=0; idx<len; idx++)
	{
		_RCV_list_var[0].r -> _nil();
		_RCV_list_var[0].rc -> release();
		_RCV_list_var[0].rc -> _nil();
		CosTransactions::Coordinator::remove(_RCV_list_var,0);
		
	} // eof 'for idx'
	
	_RCV_list_var = NULL;

	_sm -> clear();

	delete _sm;//renyi 12 25
	
	#ifdef _DEBUG_
		cout << "\t[Coordinator::release end!]" << endl;
	#endif
	
	return;
}

CosTransactions::RecoveryCoordinator_impl::RecoveryCoordinator_impl()
{
    // TODO: Add your constructor code here
    
	_Coord_var = CosTransactions::Coordinator::_nil();
	
	#ifdef _DEBUG_
		cout << "[RecoveryCoordinator::constructor() is OK!]" << endl;
	#endif
}


CosTransactions::RecoveryCoordinator_impl::RecoveryCoordinator_impl(CosTransactions::Coordinator_ptr c)
{
    // TODO: Add your constructor code here
    
	_Coord_var = CosTransactions::Coordinator::_duplicate(c);
	
	#ifdef _DEBUG_
		cout << "[RecoveryCoordinator::constructor(Coordinator) is OK!]" 
			 << endl;
	#endif
}

CosTransactions::Status 
CosTransactions::RecoveryCoordinator_impl::replay_completion(Resource_ptr r)
	throw (CORBA::SystemException,
	CosTransactions::WRONG_TRANSACTION,
	CosTransactions::INVALID_TRANSACTION,
	CosTransactions::TRANSACTION_ROLLEDBACK,
	CosTransactions::TRANSACTION_REQUIRED,
	CosTransactions::NotPrepared)
{
    // TODO:Add your replay_completion implemention code here
    
	#ifdef _DEBUG_
		cout << "[RecoveryCoordinator::replay_completion() begin!]"
			 << endl;
	#endif
	
		CosTransactions::Status tmp_status;
	tmp_status = _Coord_var->get_status();

    try
    {    
	
	if (tmp_status == CosTransactions::StatusActive ||
		tmp_status == CosTransactions::StatusPreparing) 
	{
		#ifdef _DEBUG_
			cout << "\t[throw exception: CosTransactions::NotPrepared]" << endl;
		#endif

			throw CosTransactions::NotPrepared();
	}
	
	#ifdef _DEBUG_
		cout << "\t[RecoveryCoordinator::replay_completion() end!]" << endl;
	#endif

	return tmp_status;
    }
    
    catch (CORBA::UNKNOWN)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::UNKNOW]" << endl;
	#endif
	
	throw CORBA::UNKNOWN();
	
    }
    catch (CORBA::BAD_PARAM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_PARAM]" << endl;
	#endif
	
	throw CORBA::BAD_PARAM();
	
    }
    catch (CORBA::NO_MEMORY)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_MEMORY]" << endl;
	#endif
	
	throw CORBA::NO_MEMORY();
	
    }
    catch (CORBA::IMP_LIMIT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::IMP_LIMIT]" << endl;
	#endif
	
	throw CORBA::IMP_LIMIT();
	
    }
    catch (CORBA::COMM_FAILURE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::COMM_FAILURE]" << endl;
	#endif
	
	throw CORBA::COMM_FAILURE();
	
    }
    catch (CORBA::INV_OBJREF)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_OBJREF]" << endl;
	#endif
	
	throw CORBA::INV_OBJREF();
	
    }
    catch (CORBA::NO_PERMISSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_PERMISSION]" << endl;
	#endif
	
	throw CORBA::NO_PERMISSION();
	
    }
    catch (CORBA::INTERNAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTERNAL]" << endl;
	#endif
	
	throw CORBA::INTERNAL();
	
    }
    catch (CORBA::MARSHAL)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::MARSHAL]" << endl;
	#endif
	
	throw CORBA::MARSHAL();
	
    }
    catch (CORBA::NO_IMPLEMENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_IMPLEMENT]" << endl;
	#endif
	
	throw CORBA::NO_IMPLEMENT();
	
    }
    catch (CORBA::BAD_TYPECODE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_TYPECODE]" << endl;
	#endif
	
	throw CORBA::BAD_TYPECODE();
	
    }
    catch (CORBA::BAD_OPERATION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_OPERATION]" << endl;
	#endif
	
	throw CORBA::BAD_OPERATION();
	
    }
    catch (CORBA::NO_RESOURCES)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESOURCES]" << endl;
	#endif
	
	throw CORBA::NO_RESOURCES();
	
    }
    catch (CORBA::NO_RESPONSE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::NO_RESPONSE]" << endl;
	#endif
	
	throw CORBA::NO_RESPONSE();
	
    }
    catch (CORBA::PERSIST_STORE)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::PERSIST_STORE]" << endl;
	#endif
	
	throw CORBA::PERSIST_STORE();
	
    }
    catch (CORBA::BAD_INV_ORDER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_INV_ORDER]" << endl;
	#endif
	
	throw CORBA::BAD_INV_ORDER();
	
    }
    catch (CORBA::TRANSIENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::TRANSIENT]" << endl;
	#endif
	
	throw CORBA::TRANSIENT();
	
    }
    catch (CORBA::FREE_MEM)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::FREE_MEM]" << endl;
	#endif
	
	throw CORBA::FREE_MEM();
	
    }
    catch (CORBA::INV_IDENT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_IDENT]" << endl;
	#endif
	
	throw CORBA::INV_IDENT();
	
    }
    catch (CORBA::INV_FLAG)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INV_FLAG]" << endl;
	#endif
	
	throw CORBA::INV_FLAG();
	
    }
    catch (CORBA::INTF_REPOS)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::INTF_REPOS]" << endl;
	#endif
	
	throw CORBA::INTF_REPOS();
	
    }
    catch (CORBA::BAD_CONTEXT)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::BAD_CONTEXT]" << endl;
	#endif
	
	throw CORBA::BAD_CONTEXT();
	
    }
    catch (CORBA::OBJ_ADAPTER)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJ_ADAPTER]" << endl;
	#endif
	
	throw CORBA::OBJ_ADAPTER();
	
    }
    catch (CORBA::DATA_CONVERSION)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::DATA_CONVERSION]" << endl;
	#endif
	
	throw CORBA::DATA_CONVERSION();
	
    }
    catch (CORBA::OBJECT_NOT_EXIST)
    {
	#ifdef _DEBUG_
		cout << "\t[Catch CORBA Exception:CORBA::OBJECT_NOT_EXIST]" << endl;
	#endif
	
	throw CORBA::OBJECT_NOT_EXIST();
	
    }

}

void CosTransactions::RecoveryCoordinator_impl::release ()
	throw (CORBA::SystemException)
{
    // TODO:Add your release implemention code here
	
	#ifdef _DEBUG_
		cout << "[RecoveryCoordinator::release begin!]" << endl;
	#endif
	
	CORBA::ULong RefCount = _OB_getRef ();

	#ifdef _DEBUG_
		cout << "\t[this RecoveryCoordinator object reference count is: "
		     << RefCount << "]" << endl;
	#endif
	
	_Coord_var -> _nil();

	#ifdef _DEBUG_
		cout << "\t[RecoveryCoordinator::release end!]" << endl;
	#endif
	
	return;
}


CosTransactions::TransactionalObject_impl::TransactionalObject_impl()
{
    // TODO: Add your constructor code here
	// Nothing to do.
}

//renyi 12 26
// ----------------------------------------------------------------------
// Synchronization_impl constructor/destructor
// ----------------------------------------------------------------------


CosTransactions::Synchronization_impl::Synchronization_impl(
    CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa)
    : _poa(PortableServer::POA::_duplicate(poa))
{
    //
    // Resolve the POACurrent
    //
    _poaCurrent = PortableServer::Current::_narrow(
	CORBA::Object_var(
	    orb -> resolve_initial_references("POACurrent")));
    
}

CosTransactions::Synchronization_impl::~Synchronization_impl()
{
}

// ----------------------------------------------------------------------
// Synchronization_impl public member implementation
// ----------------------------------------------------------------------

void
CosTransactions::Synchronization_impl::before_completion()
	throw (CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[CosTransactions::Synchronization_impl::before_completion() is OK!]" << endl;
	#endif
}

void
CosTransactions::Synchronization_impl::after_completion(CosTransactions::Status s)
	throw (CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "[CosTransactions::Synchronization_impl::after_completion() is OK!]" << endl;
	#endif
}

PortableServer::POA_ptr
CosTransactions::Synchronization_impl::_default_POA()
{
    return PortableServer::POA::_duplicate(_poa);
}


	
void signal_handler(int sig)
{
	// Do nothing.
	#ifdef _DEBUG_
		cout << "[signal_handler is OK!]"
		     << endl;
	#endif //_DEBUG_
	
	return;	
}  

