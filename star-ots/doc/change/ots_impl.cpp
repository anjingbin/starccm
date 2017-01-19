// **********************************************************************
//
// Generated by the StarBus IDL-to-C++ Translator
//
// Copyright (c) 2002
// AI & IC Research Center, NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#include <STAR/CORBA.h>

#include <ots_impl.h>
#include <pg_res_impl.h>

#include <time.h>
#include <stdio.h>
//#include <pthread.h>  //renyi 5 16:remove all the codes about multithreads


#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

//renyi 5 16
#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

#define _DEBUG_
#define _PG_


//
// The declaration of "C" functions for multiple thread. 
//	

// Global variables serve for multiple thread routines.

//renyi 5 16
//CosTransactions::Control_var PTH_Ctl_var;  
//PG_Resource_var PTH_Res_var;
//CORBA::ULong PTH_time_out;
//pthread_t PTH_threads[2];
	

//extern "C" 
//{
         	          		
	/****************************************************************************
	    The 'thread_timeout' procedure is active when a new transaction begins. 
	After 'time_out' time if the transaction has not finished, this transaction
	will be marked rollback .
	*****************************************************************************/	
/*	static void *thread_timeout ()
	{
		#ifdef _DEBUG_
			cout << "[Time_out thread begin!]"
			     << endl;
		#endif
		
                	sleep ( PTH_time_out );
                	CosTransactions::Coordinator_var 
                	tmp_Coord_var = PTH_Ctl_var -> get_coordinator();
                	CosTransactions::Status 
                	tmp_TransStatus = tmp_Coord_var -> get_status();
               
       		switch ( tmp_TransStatus )
       		{
                	case CosTransactions::StatusNoTransaction:
	                case CosTransactions::StatusCommitted:
                        case CosTransactions::StatusRolledBack:
                        
                        	#ifdef _DEBUG_
					cout << "	[After time_out the transaction status is :]"
					     << endl
					     << "	[CosTransactions::StatusNoTransaction]"
					     << endl
					     << "	[CosTransactions::StatusCommitted]"
					     << endl
					     << "	[CosTransactions::StatusRolledBack]"
			     		     << endl
			     		     << "	[The transaction has finished!]"
			     		     << endl;
				#endif
	
	                       	break;
	                       	
	                case CosTransactions::StatusCommitting:
	                	do {}
	                	while ( tmp_TransStatus == CosTransactions::StatusCommitting );
                        	
                        	#ifdef _DEBUG_
					cout << "	[After time_out the transaction status is :]"
					     << endl
					     << "	[CosTransactions::StatusCommitting]"
			     		     << endl
			     		     << "	[The transaction can finish correctly!]"
			     		     << endl;
				#endif
		                	
	                	break;
	                	
		                	
	                case CosTransactions::StatusRollingBack:
	                	do {}
	                	while ( tmp_TransStatus == CosTransactions::StatusRollingBack );
                        	
                        	#ifdef _DEBUG_
					cout << "	[After time_out the transaction status is :]"
					     << endl
					     << "	[CosTransactions::StatusRollingBack]"
			     		     << endl
			     		     << "	[The transaction can finish correctly!]"
			     		     << endl;
				#endif
	                	
	                	break;
	                	
                     
     			default:*/	/*
       					  CosTransactions::StatusActive,
					  CosTransactions::StatusPrepared,
    					  CosTransactions::StatusUnknown,
    					  CosTransactions::StatusPreparing,
    					  CosTransactions::StatusMarkedRollback,
    					*/
	                       
/*	                       	#ifdef _DEBUG_
					cout << "	[After time_out the transaction status is :]"
					     << endl
					     << "	[CosTransactions::StatusActive]"
					     << endl
					     << "	[CosTransactions::StatusPrepared]"
					     << endl
					     << "	[CosTransactions::StatusUnknown]"
			     		     << endl
					     << "	[CosTransactions::StatusPreparing]"
					     << endl
					     << "	[CosTransactions::StatusMarkedRollback]"
					     << endl
					     << "	[The transaction has not finished!]"
			     		     << endl;
				#endif
		
    				/****		
    				#ifdef _DEBUG_
    					cout << "	[Throw 'TimeOut' exception!]"
    					     << endl;
    				#endif
    				*****/	
 /*   				tmp_Coord_var -> rollback_only();	//?????? TimeOut status
	                        break;
	                        
		}	// end of 'switch ( tmp_TransStatus )'
	                	                 
		#ifdef _DEBUG_
			cout << "	[Time_out thread end!]"
			     << endl;
		#endif
			
		pthread_exit (NULL);
		return (void *)0;		                 
	}
	
	static void *thread_resource ()
	{
		
               	PTH_Res_var -> heuristic_decision();
		pthread_exit (NULL);
		return (void *)0;		                 
	}
}
*/

CosTransactions::TransactionFactory_impl::TransactionFactory_impl()
{
    // TODO: Add your constructor code here
	// Nothing to do.
	#ifdef _DEBUG_
		cout << "[TransactionFactory::constructor is OK!]" << endl;
	#endif
}

CosTransactions::Control_ptr CosTransactions::TransactionFactory_impl::create(CORBA::ULong time_out)
{
    // TODO:Add your create implemention code here

	#ifdef _DEBUG_
		cout << "[TransactionFactory::created begin!]" << endl;
	#endif

	CosTransactions::Control_impl* Ctl1 = new CosTransactions::Control_impl();

	CosTransactions::Control_var Ctl_var = Ctl1 -> _this();

    // Attension: duplicate 'Ctl_var' again for later use
	CosTransactions::Control::_duplicate(Ctl_var);

	/**************************************************
		Transaction has the reqest for time_out.
	***************************************************/
	if (time_out > 0)
	{
		// Add by Sun Jian: create pthread....
		
		#ifdef _PTHREAD_
		
			PTH_time_out = time_out;
			PTH_Ctl_var = CosTransactions::Control::_duplicate(Ctl_var);
			
			pthread_create ( &PTH_threads[0], NULL, thread_timeout, NULL );
			
		#endif	//_PTHREAD_
                 
		#ifdef _DEBUG_
			cout << "	[Create a new thread for time_out"
				 << " in TransactionFactory::create is OK!]"
				 << endl;
		#endif
	}

	#ifdef _DEBUG_
		cout << "	[TransactionFactory::created end!]" << endl;
	#endif

	return CosTransactions::Control::_duplicate(Ctl_var);
}


CosTransactions::Control_impl::Control_impl()
{
    // TODO: Add your constructor code here

	#ifdef _DEBUG_
		cout << "[Control::constructor begin!]" << endl;
	#endif

	CosTransactions::Coordinator_impl* _Coord_var1 = new CosTransactions::Coordinator_impl();
	_Coord_var = _Coord_var1 -> _this();
	//renyi 11 25 : changed
	//CosTransactions::Terminator_impl* _Term_var1 = new CosTransactions::Terminator_impl();
	CosTransactions::Terminator_impl* _Term_var1 = new CosTransactions::Terminator_impl(_Coord_var);
	_Term_var = _Term_var1 -> _this();

	#ifdef _DEBUG_
		cout << "	[Control::constructor end!]" << endl;
	#endif
}

CosTransactions::Terminator_ptr CosTransactions::Control_impl::get_terminator()
{
    // TODO:Add your get_terminator implemention code here

	#ifdef _DEBUG_
		cout << "[Control::get_terminator() is OK!]" << endl;
	#endif

	return CosTransactions::Terminator::_duplicate(_Term_var);
}

CosTransactions::Coordinator_ptr CosTransactions::Control_impl::get_coordinator()
{
    // TODO:Add your get_coordinator implemention code here

	#ifdef _DEBUG_
		cout << "[Control::get_coordinator() is OK!]" << endl;
	#endif

	return CosTransactions::Coordinator::_duplicate(_Coord_var);
}


void CosTransactions::Control_impl::release ()
{
    // TODO:Add your release implemention code here
	
	#ifdef _DEBUG_
		cout << "[Control::release begin!]" << endl;
	#endif
	
	CORBA::ULong RefCount = _STAR_getRef ();

	#ifdef _DEBUG_
		cout << "\t[this Control object reference count is: "
		     << RefCount << "]" << endl;
	#endif
	
	_Term_var -> release();	// free private variables' spaces before
	_Coord_var -> release();	// set pointer to NULL.
	
	_Term_var -> _nil();	// set two T_var to NULL in order to make
	_Coord_var -> _nil();	// sure free their spaces allocated before.
	
	//CORBA::release (this);	// release itself

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
		cout << "	[Terminator::constructor is end!]" << endl;
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
		cout << "	[Terminator::constructor(Coord) end!]" << endl;
	#endif
}

void CosTransactions::Terminator_impl::commit(CORBA::Boolean report_heuristics)
{
    // TODO:Add your commit implemention code here

	#ifdef _DEBUG_
		cout << "[Terminator::commit begin!]" << endl;
	#endif

	_Coord_var->commit(report_heuristics);

	#ifdef _DEBUG_
		cout << "	[Terminator::commit end!]" << endl;
	#endif
	
	return;
}

void CosTransactions::Terminator_impl::rollback()
{
    // TODO:Add your rollback implemention code here

	#ifdef _DEBUG_
		cout << "[Terminator::rollback begin!]" << endl;
	#endif

	_Coord_var->rollback();

	#ifdef _DEBUG_
		cout << "	[Terminator::rollback end!]" << endl;
	#endif

	return;
}

void CosTransactions::Terminator_impl::release ()
{
    // TODO:Add your release implemention code here
	
	#ifdef _DEBUG_
		cout << "[Terminator::release begin!]" << endl;
	#endif
	
	CORBA::ULong RefCount = _STAR_getRef ();

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


CosTransactions::Coordinator_impl::Coordinator_impl()
{
    // TODO: Add your constructor code here

	#ifdef _DEBUG_
		cout << "[Coordinator::constructor begin!]" << endl;
	#endif

	_TransStatus = CosTransactions::StatusActive;
	_RCV_list_var = new CosTransactions::Coordinator::RCVs();

	#ifdef _DEBUG_
		cout << "	[Coordinator::constructor end!]" << endl;
	#endif
}

CosTransactions::Status CosTransactions::Coordinator_impl::get_status()
{
    // TODO:Add your get_status implemention code here

	#ifdef _DEBUG_
		cout << "[Coordinator::get_status is OK!]" << endl;
	#endif

	return _TransStatus;
}

CORBA::Boolean CosTransactions::Coordinator_impl::is_same_transaction(CosTransactions::Coordinator_ptr tc)
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
	CosTransactions::Coordinator_impl::register_resource(PG_Resource_ptr r)
{
    // TODO:Add your register_resource implemention code here

	#ifdef _DEBUG_
		cout << "[Coordinator::register_resource begin!]" << endl;
	#endif

	// Begin Transaction.
	#ifdef _PG_
		
		#ifdef _DEBUG_
			cout << "\t[pg_PQexec('BEGIN')]" << endl;
		#endif
		r -> pg_PQexec("BEGIN");
		
		if (r -> pg_PQresultStatus() != PGRES_COMMAND_OK)
		{
			#ifdef _DEBUG_
				cout << "\t[????stderr????BEGIN command failed]"
				     << endl
				     << "\t[pg_PQclear]" 
				     << endl;
			#endif
			
			r -> pg_PQclear();
			
			#ifdef _DEBUG_
				cout << "\t[Throw exception: INVALID_TRANSACTION]"
				     << endl;
			#endif
			
			throw CosTransactions::INVALID_TRANSACTION();
		}

		// PQclear PGresult whenever it is no longer needed to avoid memory leaks
		#ifdef _DEBUG_
			cout << "\t[pg_PQclear]" << endl;
		#endif
		r -> pg_PQclear();
	
	#endif // _PG_


	switch(_TransStatus)
	{
		case CosTransactions::StatusActive:
		{
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is Active!]"
					 << endl;
			#endif

			/******************************************************************
				Has this resource been registered in '_RCV_list_var' before?
			*******************************************************************/
			CosTransactions::Coordinator::RCV tmp_RCV;
			int tmp_len,i;
			PG_Resource_var tmp_Res_var;
			tmp_len = _RCV_list_var->length();
			for(i=0;i<tmp_len;i++)
			{
				tmp_RCV = _RCV_list_var->operator[](i);
				tmp_Res_var = PG_Resource::_duplicate(tmp_RCV.r);
				if(tmp_Res_var == r)
				{
					cout << "	[This resource has been registered before!]" 
						 << endl;

					#ifdef _DEBUG_
						cout << "	[Coordinator::register_resource end!]" << endl;
					#endif
					
					return CosTransactions::RecoveryCoordinator::_duplicate(tmp_RCV.rc);
				}
			}	//end of for

			/*************************************************
				This resource has not been registered before,
			so register this  new resource as following.
			**************************************************/

			CosTransactions::RecoveryCoordinator_impl* tmp_RC_var1 = new CosTransactions::RecoveryCoordinator_impl(_this());
			CosTransactions::RecoveryCoordinator_var tmp_RC_var = tmp_RC_var1 -> _this(); 
			tmp_RCV.r = PG_Resource::_duplicate(r);
			tmp_RCV.r -> set_recoverycoordinator (tmp_RC_var.in());
			tmp_RCV.rc = CosTransactions::RecoveryCoordinator::_duplicate(tmp_RC_var);
			tmp_RCV.rv = CosTransactions::RV_NotPrepared;	
			CORBA::ULong tmp_lenth = _RCV_list_var->length();
			_RCV_list_var->length(tmp_lenth+1);
			_RCV_list_var[tmp_lenth] = tmp_RCV;

			#ifdef _DEBUG_
				cout << "	[A new resource has been registered!]"  
					 << endl;
			#endif

			// Add by Sun Jian: start heuristic dicision of this resource.

	/*????		#ifdef _PTHREAD_
			
				PTH_Res_var = PG_Resource::_duplicate(r);
                        	pthread_create ( &PTH_threads[1], NULL, thread_resource, NULL );
                        	                                                	
  			#endif	//  _PTHREAD_                  
	*/	
			#ifdef _DEBUG_
				cout << "	[Coordinator::register_resource end!]" << endl;
			#endif

			return CosTransactions::RecoveryCoordinator::_duplicate(tmp_RC_var);
			
			break;
			
		}	//end of case Active


		case CosTransactions::StatusMarkedRollback:
		{
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is MarkedRollback, "
				     << "can not register new resource!]"
				     << endl
				     << "	[throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]"
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

void CosTransactions::Coordinator_impl::rollback_only()
{
    // TODO:Add your rollback_only implemention code here

	#ifdef _DEBUG_
		cout << "[Coordinator::rollback_only begin!]" << endl;
	#endif

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
				cout << "	[Current '_TransStatus' is already MarkedRollback!]";
			#endif

			break;

		//all other status is above prepared.
		default:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is above Prepared, "
					 << "throw Inactive exception!]"
					 << endl;
			#endif

			throw CosTransactions::Inactive();
			break;
	}	//end of switch

	#ifdef _DEBUG_
		cout << "	[Coordinator::rollback_only end!]" << endl;
	#endif

	return;
}

void CosTransactions::Coordinator_impl::commit(CORBA::Boolean report_heuristics)
{
    // TODO:Add your commit implemention code here
	//???? need to add heuristic decision.
	#ifdef _DEBUG_
		cout << "[Coordinator::commit begin!]"
			 << endl;
	#endif

	switch ( _TransStatus )
	{
		/*********************************************************
			When the transaction has not prepared, commit or roll
		back the transaction according to the result of prepare.
		**********************************************************/
		case CosTransactions::StatusActive:
		{
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is Active]" << endl;
			#endif

			CORBA::ULong tmp_len;
			tmp_len = _RCV_list_var->length();
			
			#ifdef _DEBUG_
				cout << "	[The current length of _RCV_list_var is: "
					 << tmp_len << " !]"
					 << endl;
			#endif

			switch (tmp_len)
			{
				/**************************************************
					When transaction has no resource....
				***************************************************/
				case 0:
				{
					#ifdef _DEBUG_
						cout << "	[Resource list is NULL, transaction has committed!]" << endl;
					#endif

					_TransStatus=CosTransactions::StatusNoTransaction;

					break;
				}


				/**************************************************
					When transaction has only one resource....
				***************************************************/
				case 1:
				{
					#ifdef _DEBUG_
						cout << "	[Resource list has only one resource!]"
							 << endl;
					#endif

					CosTransactions::Coordinator::RCV tmp_RCV;
					PG_Resource_var tmp_Res_var;
					tmp_RCV = _RCV_list_var->operator[](0);
					tmp_Res_var = PG_Resource::_duplicate(tmp_RCV.r);
					
					#ifdef _DEBUG_
						cout << "	[The only resource begin to 'commit_one_phase'!]"
							 << endl;
					#endif
				
					tmp_Res_var->commit_one_phase();
					break;
				}


				/**************************************************
					When transaction has more than one resource....
				***************************************************/
				default:
				{
					#ifdef _DEBUG_
						cout << "	[Resource list has more than one resources!]" 
							 << endl;
					#endif
	
					/********************************************
						Begin to prepare all resources in list.
					*********************************************/
					#ifdef _DEBUG_
						cout << "	[Preparing all resources in resource list!]" 
							 << endl;
					#endif

					_TransStatus=CosTransactions::StatusPreparing;
					CosTransactions::Coordinator::RCV tmp_RCV;
					CORBA::ULong i;
					PG_Resource_var tmp_Res_var;
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
						tmp_Res_var = PG_Resource::_duplicate(tmp_RCV.r);
						tmp_vote = tmp_Res_var->prepare();
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
						}	// end of switch(tmp_vote)

					}	// end of while
	
					/*************************************************
					  After prepare, Coordinator will make its decision.
					**************************************************/

					/*************************************************
						Because at least one resource, which is in
					this transaction, has voted 'rollback', Coordinator 
					decide to roll back all resources in list.
					**************************************************/
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

					/************************************************
						For all resources in this transaction have
					voted 'commit' or 'readonly', Coordinator decide
					to commit all resources in list.
					*************************************************/
					else	
					{
						_TransStatus = CosTransactions::StatusCommitting;
						
						#ifdef _DEBUG_
							cout << "	[Current tmp_len is: " 
								 << tmp_len << " !]"
								 << endl;
						#endif

						for(i=0;i<tmp_len;i++)
						{
							tmp_RCV = _RCV_list_var->operator[](i);
							tmp_Res_var = PG_Resource::_duplicate(tmp_RCV.r);
							if (tmp_RCV.rv == CosTransactions::RV_Vote_Commit)
							{	
								tmp_Res_var->commit();
								change_resourcevote (tmp_Res_var.in(), CosTransactions::RV_Committed);
							}	//end of if
						}	//end of for

						_TransStatus = CosTransactions::StatusNoTransaction;

					}	//end of else(do committing)

					break;

				}	//end of default: resource list is more than one

			}	// end of switch (tmp_len)

			break;
		}	// end of case Active


		/*****************************************************
		//spring
			There are two situations as following.
		1.	Before 'prepare' there is an error occured such
			as operation failure in Recoverable Object service.
			The service function marked the transaction.
		2.	The 'pthread' for time out marked the transaction
			befor the sending of 'commit' operation, because 
			the transaction has not finished after time_out.
		******************************************************/
		case CosTransactions::StatusMarkedRollback:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is MarkedRollback!]" << endl;
			#endif

			//rollback the transaction
			rollback();
			throw CosTransactions::TRANSACTION_ROLLEDBACK();
			break;


		case CosTransactions::StatusCommitted:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is Commited!]" << endl;
			#endif
			break;

		case CosTransactions::StatusNoTransaction:	//spring
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is NoTransaction!]" << endl;
			#endif
			break;


		case CosTransactions::StatusRolledBack:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is RolledBack!]" << endl;
			#endif

			throw CosTransactions::TRANSACTION_ROLLEDBACK();
			break;


		case CosTransactions::StatusPreparing:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is Preparing!]" << endl;
			#endif
			break;


		/**********************************************************
		//add by spring
			There are two satuations same as following:
		1.	There is a 'COMM_FAILURE' exception after Coord send
			the first 'commit' command and before Coord receive the 
			return value. So Coord send 'commit' for more time after 
			a period of waiting while the first 'commit' has not 
			finished.
		2.	While the 'commit' operation hasn't finished, the computer
			containing 'Coordinator' shut down. When the 'Coordinator'
			is recoveried, it will commit again in this 'case'.
		***********************************************************/
		case CosTransactions::StatusCommitting:
		{
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is Commiting!]" << endl;
			#endif

			CosTransactions::Coordinator::RCV tmp_RCV;
			CORBA::ULong tmp_len,i;
			PG_Resource_var tmp_Res_var;
			tmp_len = _RCV_list_var->length();
			i = 0;
			for(i=0; i<tmp_len; i++)
			{
				tmp_RCV = _RCV_list_var->operator[](i);
				tmp_Res_var = PG_Resource::_duplicate(tmp_RCV.r);
				switch (tmp_RCV.rv)
				{
					case CosTransactions::RV_Vote_Commit:
						tmp_Res_var->commit();
						change_resourcevote (tmp_Res_var.in(), CosTransactions::RV_Committed);
						break;

					default: 		/*
							 * CosTransactions::StatusCommitted,
							 * CosTransactions::StatusCommitting,
							 * CosTransactions::RV_Vote_ReadOnly
							 */
						break;

				}	//end of switch (tmp_RCV.rv)

			}	//end of for

			_TransStatus = CosTransactions::StatusNoTransaction;
//add by spring: end

			break;
		}	// end of case: Committing


		case CosTransactions::StatusRollingBack:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is RollingBack!]" 
					 << endl
					 << "	[throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]"
					 << endl;
			#endif

			do {}
			while (_TransStatus != CosTransactions::StatusRolledBack);
			
			throw CosTransactions::TRANSACTION_ROLLEDBACK();
			break;


		default:
			#ifdef _DEBUG_
				cout << "	[current '_TransStatus' is Unknown]" << endl;
			#endif
			break;
	}	// end of switch (_TransStatus)

	#ifdef _DEBUG_
		cout << "	[Coordinator::commit end!]"
			 << endl;
	#endif

	return;
}	// end of commit()



void CosTransactions::Coordinator_impl::rollback()
{
    // TODO:Add your rollback implemention code here
	#ifdef _DEBUG_
		cout << "[Coordinator::rollback begin!]"
			 << endl;
	#endif

	switch(_TransStatus)
	{
		case CosTransactions::StatusActive:
		case CosTransactions::StatusMarkedRollback:
		{
			#ifdef _DEBUG_
				cout << "	[current status is Active or MarkedRollback,begin rollback]"
					 << endl;
			#endif

			/**************************************************
				Begin to rollback this transaction.....
			***************************************************/
			_TransStatus = CosTransactions::StatusRollingBack;
			CosTransactions::Coordinator::RCV tmp_RCV;
			CORBA::ULong tmp_len,i;
			PG_Resource_var tmp_Res_var;
			tmp_len = _RCV_list_var->length();

			for(i=0;i<tmp_len;i++)
			{
				tmp_RCV = _RCV_list_var->operator[](i);
				tmp_Res_var = PG_Resource::_duplicate(tmp_RCV.r);
				switch(tmp_RCV.rv)
				{
					case CosTransactions::RV_Vote_Commit:
					case CosTransactions::RV_NotPrepared:
						tmp_Res_var->rollback();
						change_resourcevote (tmp_Res_var.in(), CosTransactions::RV_RolledBack);
						break;

					default:	/*
							 CosTransactions::RV_Vote_Rollback
							 CosTransactions::RV_ReadOnly
							 CosTransactions::RV_RolledBackCosTransactions::RV_Committed
						 */
					break;
				}//end of switch

			}	//end of for

			_TransStatus = CosTransactions::StatusNoTransaction;	//spring: rolledback before
			return;
			break;
		}	//end of case: Active and MarkedRollback


		case CosTransactions::StatusCommitted:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is Commited,can not rollback!]"
					 << endl;
			#endif
			return;
			break;

		case CosTransactions::StatusNoTransaction:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is NoTransaction, can not rollback!]"
					 << endl;
			#endif
			return;
			break;

		//add by spring: begin
		/**********************************************************
			There are two satuations same as following:
		1.	There is a 'COMM_FAILURE' exception after Coord send
			the first 'rollback' command and before Coord receive the 
			return value. So Coord send 'rollback' for more time after 
			a period of waiting while the first 'rollback' has not 
			finished.
		2.	While the 'rollback' operation hasn't finished, the computer
			containing 'Coordinator' shut down. When the 'Coordinator'
			is recoveried, it will rollback again in this 'case'.
		***********************************************************/
		case CosTransactions::StatusRollingBack:
		{
			CosTransactions::Coordinator::RCV tmp_RCV;
			CORBA::ULong tmp_len,i;
			PG_Resource_var tmp_Res_var;
			tmp_len = _RCV_list_var->length();

			for ( i=0; i<tmp_len; i++ )
			{
				tmp_RCV = _RCV_list_var->operator[](i);
				tmp_Res_var = PG_Resource::_duplicate(tmp_RCV.r);
				switch(tmp_RCV.rv)
				{
					case CosTransactions::RV_Vote_Commit:
					case CosTransactions::RV_NotPrepared:
						tmp_Res_var->rollback();
						change_resourcevote (tmp_Res_var.in(), CosTransactions::RV_RolledBack);
						break;

					default: /*
							    CosTransactions::RV_Vote_Rollback,
								CosTransactions::RV_Vote_ReadOnly,
								CosTransactions::RV_RolledBack,
							 */
						break;
				}	//end of switch (tmp_RCV.rv)

			}	//end of for

			_TransStatus = CosTransactions::StatusNoTransaction;	//spring
			break;
		}	// end of case RollingBack
	

		case CosTransactions::StatusRolledBack:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is RolledBack,need not rollback!]" 
					 << endl;
			#endif
			return;
			break;

		default:
			#ifdef _DEBUG_
				cout << "	[Current '_TransStatus' is Unknown!]" << endl;
			#endif

			return;
			break;
	}	// end of switch

	#ifdef _DEBUG_
		cout << "	[Coordinator::rollback end!]"
			 << endl;
	#endif

	return;
}

void CosTransactions::Coordinator_impl::change_resourcevote(PG_Resource_ptr r,
                                                           CosTransactions::ResourceVote rv)
{
    // TODO:Add your change_resourcevote implemention code here
	#ifdef _DEBUG_
		cout << "[Coordinator::change_resourcevote begin!]"
			 << endl;
	#endif

	CosTransactions::Coordinator::RCV tmp_RCV;
	CORBA::ULong tmp_len,i;
	PG_Resource_var tmp_Res_var;
	tmp_len = _RCV_list_var->length();

	for ( i=0; i<tmp_len; i++ )
	{
		tmp_RCV = _RCV_list_var->operator[](i);
		tmp_Res_var = PG_Resource::_duplicate(tmp_RCV.r);
		if(tmp_Res_var == r)
		{
			tmp_RCV.rv = rv;
			_RCV_list_var[i] = tmp_RCV;

			#ifdef _DEBUG_
				cout << "	[This resource vote in _RVC_list has been changed!]"
					 << endl;
			#endif

			#ifdef _DEBUG_
				cout << "	[Coordinator::change_resourcevote end!]"
					 << endl;
			#endif

			return;
		}
	}//end of for

	#ifdef _DEBUG_
		cout << "	[This resource does not exit in _RVC_list!]" << endl;
	#endif

	#ifdef _DEBUG_
		cout << "	[Coordinator::change_resourcevote end!]"
			 << endl;
	#endif

	return;
}

void CosTransactions::Coordinator_impl::release ()
{
    // TODO:Add your release implemention code here
	
	#ifdef _DEBUG_
		cout << "[Coordinator::release begin!]" << endl;
	#endif
	
	CORBA::ULong RefCount = _STAR_getRef ();

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
CosTransactions::RecoveryCoordinator_impl::replay_completion(PG_Resource_ptr r)
{
    // TODO:Add your replay_completion implemention code here
	#ifdef _DEBUG_
		cout << "[RecoveryCoordinator::replay_completion() begin!]"
			 << endl;
	#endif

	CosTransactions::Status tmp_status;
	tmp_status = _Coord_var->get_status();
	if (tmp_status == CosTransactions::StatusActive ||
	    tmp_status == CosTransactions::StatusPreparing) 
	{
		#ifdef _DEBUG_
			cout << "	[throw exception: CosTransactions::NotPrepared]"
				 << endl;
		#endif

		throw CosTransactions::NotPrepared();
	}

	#ifdef _DEBUG_
		cout << "	[RecoveryCoordinator::replay_completion() end!]"
			 << endl;
	#endif

	return tmp_status;
}

void CosTransactions::RecoveryCoordinator_impl::release ()
{
    // TODO:Add your release implemention code here
	
	#ifdef _DEBUG_
		cout << "[RecoveryCoordinator::release begin!]" << endl;
	#endif
	
	CORBA::ULong RefCount = _STAR_getRef ();

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
