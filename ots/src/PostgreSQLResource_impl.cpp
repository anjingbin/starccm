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
#include <PostgreSQLResource_impl.h>

#include <libpq-fe.h>
#include <fe-auth.h>  //renyi 5 17

#define _DEBUG_
#define _PG_

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

PG_Resource_impl::PG_Resource_impl(PortableServer::POA_var poa)
{
    // TODO: Add your constructor code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::constructor(poa) begin!]" << endl;
	#endif

	_poa_var = PortableServer::POA::_duplicate(poa);;
	
	_RC_var = CosTransactions::RecoveryCoordinator::_nil();
	_Coord_var = CosTransactions::Coordinator::_nil();
	_ResourceStatus = CosTransactions::StatusActive;
	_vote = CosTransactions::VoteRollback;
	
    _PGconn = NULL;
    _PGres = NULL;
        
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::constructor(poa) end!]" << endl;
	#endif
}

PG_Resource_impl::PG_Resource_impl(CosTransactions::Coordinator_ptr c)
{
    // TODO: Add your constructor code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::constructor(c) begin!]" << endl;
	#endif
	
	_RC_var = CosTransactions::RecoveryCoordinator::_nil();
	_Coord_var = CosTransactions::Coordinator::_duplicate(c);
	_ResourceStatus = CosTransactions::StatusActive;
	_vote = CosTransactions::VoteRollback;
	
    _PGconn = NULL;
    _PGres = NULL;
        
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::constructor(c) end!]" << endl;
	#endif
}

CosTransactions::Vote PG_Resource_impl::pg_prepare()
        	 throw (CORBA::SystemException, 
        	 CosTransactions::WRONG_TRANSACTION,
        	 CosTransactions::INVALID_TRANSACTION,
        	 CosTransactions::TRANSACTION_ROLLEDBACK,
        	 CosTransactions::TRANSACTION_REQUIRED,
        	 CosTransactions::HeuristicHazard,
        	 CosTransactions::HeuristicMixed)
{
    // TODO:Add your prepare implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::prepare() begin!]" <<endl;
	#endif

	switch (_ResourceStatus)
	{
		case CosTransactions::StatusActive:
		{
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'Active', "
					 << "so begin to prepare!]" 
					 << endl;
			#endif

			_ResourceStatus = CosTransactions::StatusPreparing;
			
			#ifdef _PG_
			
				#ifdef _DEBUG_
					cout << "\t[PQexec('xaprepare')]" << endl;
				#endif
		 		_PGres = PQexec( _PGconn,"xaprepare");
				int tmp_int = PQgetPrepare(_PGres);
				
				#ifdef _DEBUG_
					cout << "\t[PQclear]" << endl;
				#endif
				PQclear(_PGres);

				switch(tmp_int)
				{
				case -1 : //ROLLBACK
					#ifdef _DEBUG_
						cout << "\t[_TransactionId is: "
						     << _TransactionId
						     << "]" 
						     << endl
						     << "\t[Vote Rollback!]"
						     << endl;
					#endif
					_vote = CosTransactions::VoteRollback;
					_TransactionId = DisabledTransactionId;
					break;

				case 0 : //READONLY
					#ifdef _DEBUG_
						cout << "\t[_TransactionId is: "
						     << _TransactionId
						     << "]" 
						     << endl
						     << "\t[Vote ReadOnly]"
						     << endl;
					#endif
				 	_vote = CosTransactions::VoteReadOnly;
                                        	_TransactionId = DisabledTransactionId;
 					break;

				default : // COMMIT
					#ifdef _DEBUG_
						cout << "\t[_TransactionId is: "
						     << _TransactionId
						     << "]" 
						     << endl
						     << "\t[Vote Commit]"
						     << endl;
					#endif
					_vote = CosTransactions::VoteCommit;
                                         	_TransactionId = tmp_int;
                                       	break;
				}

			#endif	// _PG_
			
			_ResourceStatus = CosTransactions::StatusPrepared;
			break;

		}	// end of 'case CosTransactions::StatusActive'


		case CosTransactions::StatusRolledBack:
		{
			_vote = CosTransactions::VoteRollback;
			
			#ifdef _DEBUG_
				cout<< "\t[Current resource status is 'RolledBack', vote rollback!]"
					<< endl;
			#endif
			break;
		}
		
		case CosTransactions::StatusRollingBack:
		{
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'RollingBack', "
					 << "wait it chang to 'RolledBack'!]"
					 << endl
					 << "	[Vote Rollback!]"
					 << endl;
			#endif
			
			do {}
			while (_ResourceStatus != CosTransactions::StatusRolledBack);
			_vote = CosTransactions::VoteRollback;
			
			break;
		}	
			
		
		default: 		/*
				 CosTransactions::StatusMarkedRollback,
				 CosTransactions::StatusPrepared,
				 CosTransactions::StatusCommitted,
				 CosTransactions::StatusUnknown,
				 CosTransactions::StatusNoTransaction,
				 CosTransactions::StatusCommitting,
				 */
			_vote = CosTransactions::VoteRollback;

			#ifdef _DEBUG_
				cout << "\t[Current resource status is unknow, vote Rollback]"
					<<endl;
			#endif
			break;
	}

	#ifdef _DEBUG_
		cout << "	[PG_Resource::prepare() end!]" << endl;
	#endif

	return _vote;
}

void PG_Resource_impl::pg_rollback()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard,
        	CosTransactions::HeuristicMixed,
        	CosTransactions::HeuristicCommit)
{
    // TODO:Add your rollback implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::rollback() begin!]" << endl;
	#endif

	switch (_ResourceStatus)
	{ 
		/************************************************
		While resource's heuristic dicision is on.
		*************************************************/
		case CosTransactions::StatusRollingBack:
			do {}
			while (_ResourceStatus != CosTransactions::StatusRolledBack);
//			_Coord_var->change_resourcevote (this, CosTransactions::RV_RolledBack);
			break;

		case CosTransactions::StatusRolledBack:
//			_Coord_var->change_resourcevote (this, CosTransactions::RV_RolledBack);
			break;

		/*************************************************************
			When Coord has recieved a vote rollback, it will not
		dispatch prepare() to other resources, so the other resource's 
		status is still Active.
			Another position is the Resource vote commit, its status
		is 'prepared'.
		**************************************************************/
		case CosTransactions::StatusActive:
		case CosTransactions::StatusPrepared:
		{
			_ResourceStatus = CosTransactions::StatusRollingBack;
			
			#ifdef _PG_
			//
			 _PGres= PQexec( _PGconn,"xarollback");
			PQclear( _PGres);

			#endif	// _PG_
				
			break;
		}


		default: /* 
				 CosTransactions::StatusMarkedRollback,
				 CosTransactions::StatusCommitted,
				 CosTransactions::StatusCommitting,
				 CosTransactions::StatusNoTransaction,
				 CosTransactions::StatusUnknown,
				 CosTransactions::StatusPreparing,
			  */
			  
			#ifdef _DEBUG_
				cout << "	[A wrong status!]" << endl;
			#endif
			
			break;
			
	}	// end of switch

	#ifdef _DEBUG_
		cout << "	[PG_Resource::rollback() end!]" << endl;
	#endif

	return;
}

void PG_Resource_impl::pg_commit()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard,
        	CosTransactions::HeuristicMixed,
        	CosTransactions::HeuristicRollback,
        	CosTransactions::NotPrepared)
{
    // TODO:Add your commit implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::commit() begin!]" << endl;
	#endif
	
	switch (_ResourceStatus)
	{
		case CosTransactions::StatusCommitted:
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'Committed'!]" << endl;
			#endif

			break;

		case CosTransactions::StatusCommitting:
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'Committing'!]" << endl;
			#endif

			do {}
			while (_ResourceStatus != CosTransactions::StatusCommitting);
			break;

		case CosTransactions::StatusNoTransaction:
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'NoTransaction'!]" << endl;
			#endif

			break;

		case CosTransactions::StatusActive:
		case CosTransactions::StatusPreparing:
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'Active' or 'Preparing'!" 
					 << endl;
			#endif

			#ifdef _DEBUG_
				cout << "	[Throw NotPrepared exception!]" << endl;
			#endif

			throw CosTransactions::NotPrepared();
			break;

		case CosTransactions::StatusRollingBack:
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'RollingBack'!]" << endl;
			#endif

			do {}
			while (_ResourceStatus != CosTransactions::StatusRolledBack);

			#ifdef _DEBUG_
				cout << "	[Throw HeuristicRollback exception!]" << endl;
			#endif

			throw CosTransactions::HeuristicRollback();
			break;


		case CosTransactions::StatusRolledBack:
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'RolledBack'!]" << endl;
			#endif

			#ifdef _DEBUG_
				cout << "	[Throw HeuristicRollback exception!]" << endl;
			#endif

			throw CosTransactions::HeuristicRollback();
			break;

			
		case CosTransactions::StatusPrepared:
		{
			#ifdef _DEBUG_
				cout << "	[Current resource status is 'Prepared'!]" << endl;
			#endif
			
			_ResourceStatus = CosTransactions::StatusCommitting;
			
			#ifdef _PG_
			
				#ifdef _DEBUG_
					cout << "\t[PQexec('xacommit')]" << endl;
				#endif
			 	_PGres= PQexec( _PGconn,"xacommit");
				if (PQresultStatus(_PGres) != PGRES_COMMAND_OK)
					throw CosTransactions::HeuristicRollback();
				else
				{
					#ifdef _DEBUG_
						cout << "\t[PQexec('xacommit') OK]" << endl;
					#endif
				}
	
				PQclear(_PGres);
			#endif	// _PG_
			
			break;
			
		}	// end of 'case CosTransactions::StatusPrepared'


		default:
			break;
			
	}	// end of switch

	#ifdef _DEBUG_
		cout << "	[PG_Reource::commit() end!]" << endl;
	#endif

	return;
}

void PG_Resource_impl::pg_commit_one_phase()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard)
{
    // TODO:Add your commit_one_phase implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::commit_one_phase() begin !]"
			 << endl;
	#endif

	switch (pg_prepare())
	{
		case CosTransactions::VoteCommit:
			pg_commit();
			//_Coord_var->change_resourcevote(this, CosTransactions::RV_Committed);
			break;

		case CosTransactions::VoteReadOnly:
			//_Coord_var->change_resourcevote(this, CosTransactions::RV_Committed);
			break;

		case CosTransactions::VoteRollback:
			pg_rollback();

			#ifdef _DEBUG_
				cout << "	[throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]" << endl;
			#endif

			throw CosTransactions::TRANSACTION_ROLLEDBACK();
			//_Coord_var->change_resourcevote(this, CosTransactions::RV_RolledBack);
			break;
	} // end of switch
	
	#ifdef _DEBUG_
		cout << "	[PG_Resource::commit_one_phase() end!]" << endl;
	#endif
	return;
}

void PG_Resource_impl::pg_forget()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your forget implemention code here
	// Nothing to do.
}

void PG_Resource_impl::pg_release()
        	throw (CORBA::SystemException)
{
    // TODO:Add your release implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::release begin!]" << endl;
	#endif
	
	CORBA::ULong RefCount = _OB_getRef ();

	#ifdef _DEBUG_
		cout << "\t[this PG_Resource object reference count is: "
		     << RefCount << "]" << endl;
	#endif
	
	// Free private variables' spaces first.
	
	_RC_var = CosTransactions::RecoveryCoordinator::_nil();
	_Coord_var = CosTransactions::Coordinator::_nil();
	//?????
	
	//CORBA::release (this);     //!!!!!!!!!renyi 5 16
		
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::release end!]" << endl;
	#endif

	return;
}

//==============================================================================
// This function is used when a resource join a new transaction, the resource
// should do something for 2PC.
//==============================================================================

CORBA::Boolean
PG_Resource_impl::pg_BeginTrans()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_BeginTrans implemention code here
	
	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_BeginTrans begin!]"
		     << endl
		     << "\t[pg_PQexec('BEGIN')]" << endl;
	#endif
	
	CORBA::Boolean ret = true;
		
	pg_PQexec("BEGIN");
		
	#ifdef _DEBUG_
		cout << "\t[pg_PQresultStatus())]" << endl;
	#endif

	if (pg_PQresultStatus() != PGRES_COMMAND_OK)
	{
		#ifdef _DEBUG_
			cout << "\t[BEGIN command failed.]"
			     << endl
			     << "\t[pg_PQclear]" 
			     << endl;
		#endif
		
		pg_PQclear();
		
		#ifdef _DEBUG_
			cout << "\t[return false]" << endl;
		#endif
			
		ret = false;
	}
	else
	{
		// PQclear PGresult whenever it is no longer needed to avoid memory leaks
		#ifdef _DEBUG_
			cout << "\t[pg_PQclear]" << endl;
		#endif
	
		pg_PQclear();
	
		#ifdef _DEBUG_
			cout << "\t[return true]" << endl;
		#endif _DEBUG_
		
		ret = true;
	}
			
	#ifdef _DEBUG_
		cout << "\t[YH_Resource::pg_BeginTrans end!]"
		     << endl;
	#endif _DEBUG_
    	
    	return ret;
}


void PG_Resource_impl::pg_set_recoverycoordinator(CosTransactions::RecoveryCoordinator_ptr rc)
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your set_recoverycoordinator implemention code here

	_RC_var = CosTransactions::RecoveryCoordinator::_duplicate(rc);

	#ifdef _DEBUG_
		cout << "[PG_Resource::set_recoverycoordinator is OK!]" << endl;
	#endif
}

void PG_Resource_impl::pg_heuristic_decision()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your heuristic_decision implemention code here
    	
    	//??? delete this function?
	return;
}

void PG_Resource_impl::pg_PQsetdb(const char* pghost,
                                  const char* pgport,
                                  const char* pgoptions,
                                  const char* pgtty,
                                  const char* dbName)
	throw(PG_Resource::PG_CONNECTTION_FAILED)
{
    // TODO:Add your pg_PQsetdb implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQsetdb begin!]" 
		     << endl
		     << "\t[PQsetdb(pghost, pgport, pgoprions, pgtty, dbName)]"
		     << endl;
	#endif
	
	// IN 'string' parameter can not passed by NULL, so user should change
	// NULL to "" when set parameters.
	char *tmp_pghost, *tmp_pgport, *tmp_pgoptions, *tmp_pgtty, *tmp_dbName;
		
	tmp_pghost = (char*)pghost;
	tmp_pgport = (char*)pgport;
	tmp_pgoptions = (char*)pgoptions;
	tmp_pgtty = (char*)pgtty;
	tmp_dbName = (char*)dbName;
	
	#ifdef _DEBUG_
		cout << "\t[tmp_pghost -- " << tmp_pghost << "]"
		     << endl
		     << "\t[tmp_dbName -- " << tmp_dbName << "]"
		     << endl;
	#endif
	
	if ( !strcmp(pghost, "") )
	{
		#ifdef _DEBUG_
			cout << "\t[pghost is NULL]" << endl;
		#endif
		tmp_pghost = NULL;
	}
	
	if ( !strcmp(pgport, "") )
	{
		#ifdef _DEBUG_
			cout << "\t[pgport is NULL]" << endl;
		#endif
		tmp_pgport = NULL;
	}
	
	if ( !strcmp(pgoptions, "") )
	{
		#ifdef _DEBUG_
			cout << "\t[pgoptions is NULL]" << endl;
		#endif
		tmp_pgoptions = NULL;
	}
	
	if ( !strcmp(pgtty, "") )
	{
		#ifdef _DEBUG_
			cout << "\t[pgtty is NULL]" << endl;
		#endif
		tmp_pgtty = NULL;
	}
	
	if ( !strcmp(dbName, "") )
	{
		#ifdef _DEBUG_
			cout << "\t[dbName is NULL]" << endl;
		#endif
		tmp_dbName = NULL;
	}
	
	_PGconn = PQsetdb( (char*)tmp_pghost, (char*)tmp_pgport, 
			(char*)tmp_pgoptions, (char*)tmp_pgtty, 
			(char*)tmp_dbName );
	
	if (PQstatus(_PGconn) == CONNECTION_BAD)
	{
		#ifdef _DEBUG_
			cout << "\t[Connection to database postgres failed.]"
			     << endl
			     << "\t[Error: " 
			     << PQerrorMessage(_PGconn)
			     << "]" 
			     << endl;
		#endif
		
		throw PG_Resource::PG_CONNECTTION_FAILED();
	}
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQsetdb end!]" << endl;
	#endif
	
	return;
}

char* PG_Resource_impl::pg_PQdb()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQdb implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQdb begin!]" 
		     << endl
		     << "\t[PQdb(_PGconn)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = CORBA::string_dup( (char*)PQdb(_PGconn) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQdb end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

char* PG_Resource_impl::pg_PQhost()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQhost implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQhost begin!]" 
		     << endl
		     << "\t[PQhost(_PGconn)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = CORBA::string_dup( (char*)PQhost(_PGconn) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQhost end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

char* PG_Resource_impl::pg_PQoptions()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQoptions implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQoptions begin!]" 
		     << endl
		     << "\t[PQoptions(_PGconn)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = CORBA::string_dup( (char*)PQoptions(_PGconn) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQoptions end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

char* PG_Resource_impl::pg_PQport()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQport implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQport begin!]" 
		     << endl
		     << "\t[PQport(_PGconn)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = CORBA::string_dup( (char*)PQport(_PGconn) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQport end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

char* PG_Resource_impl::pg_PQtty()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQtty implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQtty begin!]" 
		     << endl
		     << "\t[PQtty(_PGconn)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = CORBA::string_dup( (char*)PQtty(_PGconn) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQtty end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

pg_ConnStatusType PG_Resource_impl::pg_PQstatus()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQstatus implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQstatus begin!]" 
		     << endl
		     << "\t[PQstatus(_PGconn)]"
		     << endl;
	#endif
	
	pg_ConnStatusType tmp_pg_status;
	ConnStatusType tmp_status;
	tmp_status = PQstatus(_PGconn);
	
	switch (tmp_status)
	{
	case CONNECTION_OK:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_CONNECTION_OK]" << endl;
		#endif
	
		tmp_pg_status = pg_CONNECTION_OK;
		break;
		
	case CONNECTION_BAD:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_CONNECTION_BAD]" << endl;
		#endif
	
		tmp_pg_status = pg_CONNECTION_BAD;
		break;
		
	default:
		#ifdef _DEBUG_
			cout << "\t[WARNING: return value is wrong! return pg_CONNECTION_BAD]" << endl;
		#endif
		
		tmp_pg_status = pg_CONNECTION_BAD;
		break;
	}	
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQstatus end!]" << endl;
	#endif
	
	return tmp_pg_status;
}

char* PG_Resource_impl::pg_PQerrorMessage()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQerrorMessage implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQerrorMessage begin!]" 
		     << endl
		     << "\t[PQerrorMessage(_PGconn)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = 
		CORBA::string_dup( (char*)PQerrorMessage(_PGconn) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQerrorMessage end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

void PG_Resource_impl::pg_PQfinish()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQfinish implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQfinish begin!]" 
		     << endl
		     << "\t[PQfinish(_PGconn)]"
		     << endl;
	#endif
	
	PQfinish(_PGconn);
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQfinish end!]" << endl;
	#endif
	
	return;
}

void PG_Resource_impl::pg_PQreset()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQreset implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQreset begin!]" 
		     << endl
		     << "\t[PQreset(_PGconn)]"
		     << endl;
	#endif
	
	PQreset(_PGconn);
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQreset end!]" << endl;
	#endif
	
	return;
}

void PG_Resource_impl::pg_PQuntrace()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQuntrace implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQuntrace begin!]" 
		     << endl
		     << "\t[PQuntrace(_PGconn)]"
		     << endl;
	#endif
	
	PQuntrace(_PGconn);
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQuntrace end!]" << endl;
	#endif
	
	return;
}

pg_ExecStatusType PG_Resource_impl::pg_PQresultStatus()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PGresultStatus implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQresultStatus begin!]" 
		     << endl
		     << "\t[PGresultStatus(_PGres)]"
		     << endl;
	#endif
	
	pg_ExecStatusType tmp_result;
	ExecStatusType tmp_statustype = PQresultStatus(_PGres);
	
	switch (tmp_statustype)
	{
	
	case PGRES_EMPTY_QUERY:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_PGRES_EMPTY_QUERY]" << endl;
		#endif
	
		tmp_result = pg_PGRES_EMPTY_QUERY;
		break;
		
	case PGRES_COMMAND_OK:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_PGRES_COMMAND_OK]" << endl;
		#endif
	
		tmp_result = pg_PGRES_COMMAND_OK;
		break;
		
	case PGRES_TUPLES_OK:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_PGRES_TUPLES_OK]" << endl;
		#endif
	
		tmp_result = pg_PGRES_TUPLES_OK;
		break;
		
	case PGRES_COPY_OUT:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_PGRES_COPY_OUT]" << endl;
		#endif
	
		tmp_result = pg_PGRES_COPY_OUT;
		break;
		
	case PGRES_COPY_IN:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_PGRES_COPY_IN]" << endl;
		#endif
	
		tmp_result = pg_PGRES_COPY_IN;
		break;
		
	case PGRES_BAD_RESPONSE:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_PGRES_BAD_RESPONSE]" << endl;
		#endif
	
		tmp_result = pg_PGRES_BAD_RESPONSE;
		break;
		
	case PGRES_NONFATAL_ERROR:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_PGRES_NONFATAL_ERROR]" << endl;
		#endif
	
		tmp_result = pg_PGRES_NONFATAL_ERROR;
		break;
		
	case PGRES_FATAL_ERROR:
		#ifdef _DEBUG_
			cout << "\t[return value is: pg_PGRES_FATAL_ERROR]" << endl;
		#endif
	
		tmp_result = pg_PGRES_FATAL_ERROR;
		break;
		
	default:
		#ifdef _DEBUG_
			cout << "\t[WARNING: return value is wrong! return pg_PGRES_FATAL_ERROR]" << endl;
		#endif
		
		tmp_result = pg_PGRES_FATAL_ERROR;
		break;
	}
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQresultStatus end!]" 
		     << endl;
	#endif
	
	return tmp_result;	
	
}

void PG_Resource_impl::pg_PQexec(const char* query)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQexec implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQexec begin!]" 
		     << endl
		     << "\t[PQexec(_PGconn, query)]"
		     << endl;
	#endif
	
	_PGres = PQexec(_PGconn, (char*)query);
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQexec end!]" << endl;
	#endif
	
	return;
}

CORBA::Long PG_Resource_impl::pg_PQntuples()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQntuples implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQntuples begin!]" 
		     << endl
		     << "\t[PQntuples(_PGres)]"
		     << endl;
	#endif
	
	CORBA::Long tmp_int = PQntuples(_PGres);
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_int << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQntuples end!]" 
		     << endl;
	#endif
	
	return tmp_int;
}

CORBA::Long PG_Resource_impl::pg_PQnfields()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQnfields implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQnfields begin!]" 
		     << endl
		     << "\t[PQnfields(_PGres)]"
		     << endl;
	#endif
	
	CORBA::Long tmp_int = PQnfields(_PGres);
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_int << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQnfields end!]" 
		     << endl;
	#endif
	
	return tmp_int;
}

char* PG_Resource_impl::pg_PQfname(CORBA::Long field_index)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQfname implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQfname begin!]" 
		     << endl
		     << "\t[PQfname(_PGres, field_index)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = 
		CORBA::string_dup( (char*)PQfname (_PGres, (int)field_index) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQfname end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

CORBA::Long PG_Resource_impl::pg_PQfnumber(const char* field_name)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQfnumber implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQfnumber begin!]" 
		     << endl
		     << "\t[PQfnumber(_PGres, field_name)]"
		     << endl;
	#endif
	
	CORBA::Long tmp_int = PQfnumber(_PGres, (char*)field_name);
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_int << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQfnumber end!]" 
		     << endl;
	#endif
	
	return tmp_int;
}

CORBA::Long PG_Resource_impl::pg_PQftype(CORBA::Long field_num)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQftype implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQftype begin!]" 
		     << endl
		     << "\t[PQftype(_PGres, field_num)]"
		     << endl;
	#endif
	
	CORBA::Long tmp_Oid = (CORBA::Long)PQftype(_PGres, (int)field_num);
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_Oid << "]"
		     << endl 
		     << "\t[PG_Resource::pg_PQftype end!]" 
		     << endl;
	#endif
	
	return tmp_Oid;
}

CORBA::Long PG_Resource_impl::pg_PQfsize(CORBA::Long field_index)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQfsize implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQfsize begin!]" 
		     << endl
		     << "\t[PQfsize(_PGres, field_index)]"
		     << endl;
	#endif
	
	int tmp_int2 = (int)PQfsize(_PGres, (int)field_index);
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_int2 << "]"
		     << endl 
		     << "\t[PG_Resource::pg_PQfsize end!]" 
		     << endl;
	#endif
	
	return (CORBA::Long)tmp_int2;
}

char* PG_Resource_impl::pg_PQgetvalue(CORBA::Long tup_num,
                                      CORBA::Long field_num)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQgetvalue implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQgetvalue begin!]" 
		     << endl
		     << "\t[PQgetvalue(_PGres, tup_num, field_num)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = CORBA::string_dup
		( (char*)PQgetvalue(_PGres, (int)tup_num, (int)field_num) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQgetvalue end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

CORBA::Long PG_Resource_impl::pg_PQgetlength(CORBA::Long tup_num,
                                            CORBA::Long field_num)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQgetlength implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQgetlength begin!]" 
		     << endl
		     << "\t[PQgetlength(_PGres, tup_num, field_num)]"
		     << endl;
	#endif
	
	CORBA::Long tmp_int = (CORBA::Long)PQgetlength
		(_PGres, (int)tup_num, (int)field_num);
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_int << "]"
		     << endl 
		     << "\t[PG_Resource::pg_PQgetlength end!]" 
		     << endl;
	#endif
	
	return tmp_int;
}

char* PG_Resource_impl::pg_PQcmdStatus()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQcmdStatus implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQcmdStatus begin!]" 
		     << endl
		     << "\t[PQcmdStatus(_PGres)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = 
		CORBA::string_dup( (char*)PQcmdStatus(_PGres) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQcmdStatus end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

char* PG_Resource_impl::pg_PQoidStatus()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQoidStatus implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQoidStatus begin!]" 
		     << endl
		     << "\t[PQoidStatus(_PGres)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = 
		CORBA::string_dup( (char*)PQoidStatus(_PGres) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_PQoidStatus end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

void PG_Resource_impl::pg_PQclear()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQclear implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQclear begin!]" 
		     << endl
		     << "\t[PQclear(_PGres)]"
		     << endl;
	#endif
	
	PQclear(_PGres);
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQclear end!]" << endl;
	#endif
	
	return;
}

pg_PGnotify* PG_Resource_impl::pg_PQnotifies()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQnotifies implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQnotifies begin!]" 
		     << endl
		     << "\t[PQnotifies()]"
		     << endl;
	#endif
	
	pg_PGnotify* tmp_notify = NULL;
	PGnotify* tmp_pg = PQnotifies(_PGconn);
	tmp_notify -> relname = CORBA::string_dup( (char*)tmp_pg -> relname );
	tmp_notify -> be_pid = (CORBA::Long)(tmp_pg -> be_pid);
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQnotifies end!]" << endl;
	#endif
	
	return tmp_notify;
}

CORBA::Long PG_Resource_impl::pg_PQgetline(const char* str,
                                          CORBA::Long length)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQgetline implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQgetline begin!]" 
		     << endl
		     << "\t[PQgetline(_PGconn, str, length)]"
		     << endl;
	#endif
	
	CORBA::Long tmp_int = (CORBA::Long)PQgetline
		(_PGconn, (char*)str, (int)length);
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_int << "]"
		     << endl 
		     << "\t[PG_Resource::pg_PQgetline end!]" 
		     << endl;
	#endif
	
	return tmp_int;
}

void PG_Resource_impl::pg_PQputline(const char* str)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQputline implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQputline begin!]" 
		     << endl
		     << "\t[PQputline(_PGconn, str)]"
		     << endl;
	#endif
	
	PQputline(_PGconn, (char*)str);
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_PQputline end!]" << endl;
	#endif
	
	return;
}

CORBA::Long PG_Resource_impl::pg_PQendcopy()
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_PQendcopy implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_PQendcopy begin!]" 
		     << endl
		     << "\t[PQendcopy(_PGconn)]"
		     << endl;
	#endif
	
	CORBA::Long tmp_int = (CORBA::Long)PQendcopy(_PGconn);
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_int << "]"
		     << endl 
		     << "\t[PG_Resource::pg_PQendcopy end!]" 
		     << endl;
	#endif
	
	return tmp_int;
}

char* PG_Resource_impl::pg_fe_getauthname(const char* errorMessage)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_fe_getauthname implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_fe_getauthname begin!]" 
		     << endl
		     << "\t[fe_getauthname(errorMessage)]"
		     << endl;
	#endif
	
	CORBA::String_var tmp_string = 
		CORBA::string_dup( (char*)fe_getauthname( (char*)errorMessage) );
	
	#ifdef _DEBUG_
		cout << "\t[return value is: " << tmp_string << "]"
		     << endl
		     << "\t[PG_Resource::pg_fe_getauthname end!]" 
		     << endl;
	#endif
	
	return CORBA::string_dup(tmp_string);
}

void PG_Resource_impl::pg_fe_setauthsvc(const char* name,
                                        const char* errorMessage)
        	throw (CORBA::SystemException)
{
    // TODO:Add your pg_fe_setauthsvc implemention code here

	#ifdef _DEBUG_
		cout << "[PG_Resource::pg_fe_setauthsvc begin!]" 
		     << endl
		     << "\t[fe_setauthsvc(name, errorMessage)]"
		     << endl;
	#endif
	
	fe_setauthsvc( (char*)name, (char*)errorMessage );
	
	#ifdef _DEBUG_
		cout << "\t[PG_Resource::pg_fe_setauthsvc end!]" << endl;
	#endif
	
	return;
}
