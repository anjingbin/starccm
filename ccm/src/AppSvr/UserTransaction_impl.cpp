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

#include <UserTransaction_impl.h>

//
// IDL:omg.org/Components:1.0
//

//
// IDL:omg.org/Components/Transaction:1.0
//

//
// IDL:omg.org/Components/Transaction/UserTransaction:1.0
//
Components::Transaction::UserTransaction_impl::UserTransaction_impl(::Container::ContainerBase_ptr container)
{
	container_ = OBJDUPLICATE(ContainerBase_ptr,container);
	
	CORBA::ORB_var orb = container_ -> getOrb();
	CORBA::Object_var obj = orb -> resolve_initial_references("TransactionCurrent");

	if (CORBA::is_nil(obj))
	    TRACE0(LEVEL1,"TxnInterceptor can not get the initial reference of TransactionCurrent.\n"); 

	txnCurrent_ = CosTransactions::Current::_narrow(obj);

	if (CORBA::is_nil(txnCurrent_))
	    TRACE0(LEVEL1,"TxnInterceptor can not narrow the object to current.\n");
	
}

Components::Transaction::UserTransaction_impl::~UserTransaction_impl()
{
}

//
// IDL:omg.org/Components/Transaction/UserTransaction/begin:1.0
//
void
Components::Transaction::UserTransaction_impl::begin()
    throw(Components::Transaction::NotSupported,
          Components::Transaction::SystemError,
          CORBA::SystemException)
{
	try
	{
		switch (txnCurrent_ -> get_status())
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
			throw Components::Transaction::NotSupported();
			TRACE0(LEVEL6,"UserTransaction_impl::begin() raises Components::Transaction::NotSupported execption.\n");
		}
		case CosTransactions::StatusNoTransaction :
		{	
			try
				{
					txnCurrent_ -> begin();
				}
			catch (CosTransactions::SubtransactionsUnavailable& /*e*/)
				{
					throw Components::Transaction::SystemError();
					TRACE0(LEVEL1,"UserTransaction_impl::begin() raises CosTransactions::SubtransactionsUnavailable execption.\n");
				}
			break;								        
		}
		}
	}
	catch(CORBA::SystemException& /*e*/)
	{
		TRACE0(LEVEL1,"UserTransaction_impl::begin() raises CORBA::SystemException execption.\n");
	}
}

//
// IDL:omg.org/Components/Transaction/UserTransaction/commit:1.0
//
void
Components::Transaction::UserTransaction_impl::commit()
    throw(Components::Transaction::RollbackError,
          Components::Transaction::NoTransaction,
          Components::Transaction::HeuristicMixed,
          Components::Transaction::HeuristicRollback,
          Components::Transaction::Security,
          Components::Transaction::SystemError,
          CORBA::SystemException)
{
	try
	{
		txnCurrent_ -> commit(true);
	}
	
	catch (CosTransactions::NoTransaction& /*e*/)
	{
		throw Components::Transaction::NoTransaction();
		TRACE0(LEVEL6,"UserTransaction_impl::commit() has not a active trasaction.\n ");	 	
	}
	
	catch (CosTransactions::HeuristicMixed& /*e*/)
	{
		throw Components::Transaction::HeuristicMixed();
		TRACE0(LEVEL6,"UserTransaction_impl::commit() raises a Components::Transaction::HeuristicMixed() execption.\n");
	}
	
	catch (CosTransactions::HeuristicRollback& /*e*/)
	{
		throw Components::Transaction::HeuristicRollback();
		TRACE0(LEVEL6,"UserTransaction_impl::commit() raises a Components::Transaction::HeuristicRollback() execption.\n");
	}
	
	catch (CORBA::TRANSACTION_ROLLEDBACK& /*e*/)
	{
		throw Components::Transaction::RollbackError();
		TRACE0(LEVEL6,"UserTransaction_impl::commit() raises a Components::Transaction::RollbackError() execption.\n");
	}
	
	catch (CORBA::NO_PERMISSION& /*e*/)
	{
		throw Components::Transaction::Security();
		TRACE0(LEVEL6,"UserTransaction_impl::commit() raises a Components::Transaction::Security() execption.\n");
	}
	
	catch (Components::Transaction::SystemError& /*e*/)
	{
		throw Components::Transaction::SystemError();
		TRACE0(LEVEL6,"UserTransaction_impl::commit() raises a Components::Transaction::SystemError() execption.\n");
	}
	
	catch (...)
	{
		TRACE0(LEVEL6,"UserTransaction_impl::commit() raises a CORBA::SystemException() execption.\n");
	}
	
}

//
// IDL:omg.org/Components/Transaction/UserTransaction/rollback:1.0
//
void
Components::Transaction::UserTransaction_impl::rollback()
    throw(Components::Transaction::NoTransaction,
          Components::Transaction::Security,
          Components::Transaction::SystemError,
          CORBA::SystemException)
{
	try 
	{
		txnCurrent_ -> rollback();
	}
	
	catch (CosTransactions::NoTransaction& /*e*/)
	{
		throw Components::Transaction::NoTransaction();
		TRACE0(LEVEL6,"UserTransaction_impl::rollback() has not a active trasaction.\n ");	 	
	}
	
	catch (CORBA::NO_PERMISSION& /*e*/)
	{
		throw Components::Transaction::Security();
		TRACE0(LEVEL6,"UserTransaction_impl::rollback() raises a Components::Transaction::Security() execption.\n");
	}
		
	catch (Components::Transaction::SystemError& /*e*/)
	{
		throw Components::Transaction::SystemError();
		TRACE0(LEVEL6,"UserTransaction_impl::rollback() raises a Components::Transaction::SystemError() execption.\n");
	}
	
	catch (...)
	{
		TRACE0(LEVEL6,"UserTransaction_impl::rollback() raises a CORBA::SystemException() execption.\n");
	}
	
}

//
// IDL:omg.org/Components/Transaction/UserTransaction/set_rollback_only:1.0
//
void
Components::Transaction::UserTransaction_impl::set_rollback_only()
    throw(Components::Transaction::NoTransaction,
          Components::Transaction::SystemError,
          CORBA::SystemException)
{
	try
	{
		txnCurrent_ -> rollback_only();
	}
	
	catch (CosTransactions::NoTransaction& /*e*/)
	{
		throw Components::Transaction::NoTransaction();
		TRACE0(LEVEL6,"UserTransaction_impl::set_rollback_only() has not a active trasaction.\n ");	 	
	}
			
	catch (Components::Transaction::SystemError& /*e*/)
	{
		throw Components::Transaction::SystemError();
		TRACE0(LEVEL6,"UserTransaction_impl::set_rollback_only() raises a Components::Transaction::SystemError() execption.\n");
	}
	
	catch (...)
	{
		TRACE0(LEVEL6,"UserTransaction_impl::set_rollback_only() raises a CORBA::SystemException() execption.\n");
	}
	
    
}

//
// IDL:omg.org/Components/Transaction/UserTransaction/get_status:1.0
//
Components::Transaction::Status
Components::Transaction::UserTransaction_impl::get_status()
    throw(Components::Transaction::SystemError,
          CORBA::SystemException)
{
      
      try
	{
		switch (txnCurrent_ -> get_status())
		{
    		case CosTransactions::StatusMarkedRollback :
    			{
    				return MARKED_ROLLBACK ;
    			}
    		case CosTransactions::StatusPrepared :
    			{
    				return PREPARED ;
    			}
    		case CosTransactions::StatusCommitted :
    			{
    				return COMMITTED ;
    			}
    		case CosTransactions::StatusRolledBack :
    			{
    				return ROLLED_BACK ;
    			}
    		case CosTransactions::StatusPreparing :
    			{
    				return PREPARING ;
    			}
    		case CosTransactions::StatusCommitting :
    			{
    				return COMMITTING ;
    			}
    		case CosTransactions::StatusRollingBack :
    			{
    				return ROLLING_BACK ;
    			}
    		case CosTransactions::StatusActive :
    			{
    				return ACTIVE ;
    			}
    		case CosTransactions::StatusNoTransaction :
    			{
    				return NO_TRANSACTION ;
    			}
    		case CosTransactions::StatusUnknown :
    			{
    				//because this status is transient.UserTransaction class has 
    				//not this status.
  				    //return txnCurrent_ -> get_status();
  				    return ROLLED_BACK;
    				
    			}
		}
	}
			
	catch (Components::Transaction::SystemError& /*e*/)
	{
		throw Components::Transaction::SystemError();
		TRACE0(LEVEL6,"UserTransaction_impl::get_status() raises a Components::Transaction::SystemError() execption.\n");
	}
	
	catch (...)
	{
		TRACE0(LEVEL6,"UserTransaction_impl::get_status() raises a CORBA::SystemException() execption.\n");
	}
	return ROLLED_BACK;
	
}

//
// IDL:omg.org/Components/Transaction/UserTransaction/set_timeout:1.0
//
void
Components::Transaction::UserTransaction_impl::set_timeout(CORBA::Long to)
//    throw(Components::Transaction::SystemError,
//          CORBA::SystemException)
{
	try
	{
		txnCurrent_ -> set_timeout(to);
	}
			
	catch (Components::Transaction::SystemError& /*e*/)
	{
		throw Components::Transaction::SystemError();
		TRACE0(LEVEL6,"UserTransaction_impl::set_timeout() raises a Components::Transaction::SystemError() execption.\n");
	}
	
	catch (...)
	{
		TRACE0(LEVEL6,"UserTransaction_impl::set_timeout() raises a CORBA::SystemException() execption.\n");
	}
    
}

//
// IDL:omg.org/Components/Transaction/UserTransaction/suspend:1.0
//
Components::Transaction::TranToken*
Components::Transaction::UserTransaction_impl::suspend()
    throw(Components::Transaction::NoTransaction,
          Components::Transaction::SystemError,
          CORBA::SystemException)
{
    Components::Transaction::TranToken* token= new Components::Transaction::TranToken;
    
    CORBA::ORB_var orb = container_->getOrb();
    
    try
    {
    	   control_ = txnCurrent_ -> suspend(); 
    	   	
//    	   CosTransactions::Control_ptr tempControl = control_.retn();
//    	   stack_control_.push(tempControl);    
    }
    
    catch (CosTransactions::NoTransaction& /*e*/)
	{
		throw Components::Transaction::NoTransaction();
		TRACE0(LEVEL6,"UserTransaction_impl::suspend() has not a active trasaction.\n ");	 	
	}
			
	catch (Components::Transaction::SystemError& /*e*/)
	{
		throw Components::Transaction::SystemError();
		TRACE0(LEVEL6,"UserTransaction_impl::suspend() raises a Components::Transaction::SystemError() execption.\n");
	}
	
	catch (...)
	{
		TRACE0(LEVEL6,"UserTransaction_impl::suspend() raises a CORBA::SystemException() execption.\n");
	}
     	   
     	   
    CORBA::String_var str_control = orb -> object_to_string(control_);
     	   //CORBA::OctetSeq temptoken;
//	   Container::Cookie cookieValue;
//	    cookieValue.length(strlen(value));
//
//	   CORBA::Octet* data = token->get_buffer(FALSE);
//           memcpy(data,str_control,strlen(value));

    token->length( strlen(str_control) );
    int counter;
    
    for (counter = 0; counter < strlen(str_control); counter++)
    {
        (*token)[(CORBA::ULong)counter] = CORBA::Octet(str_control[(CORBA::ULong)counter]);
    }
    
    return token;
}

//
// IDL:omg.org/Components/Transaction/UserTransaction/resume:1.0
//
void
Components::Transaction::UserTransaction_impl::resume(const Components::Transaction::TranToken& txtoken)
    throw(Components::Transaction::InvalidToken,
          Components::Transaction::SystemError,
          CORBA::SystemException)
{
    CORBA::ORB_var orb = container_ -> getOrb();
//	unsigned char* pvalue = cookieValue.get_buffer(FALSE);
//
//	char* result = CORBA::string_alloc(txtoken.length()+1);
//	memcpy(ret,txtoken,cookieValue.length());
//	*(char*)(result + txtoken.length()) = 0;
	CORBA::String_var result = CORBA::string_dup("");
	char tempchar;

    int counter;
    for (counter = 0; counter < txtoken.length(); counter++)
    {
        tempchar = int(txtoken[counter]);
        result += tempchar;
    }
    	   
    CORBA::Object_var obj = orb -> string_to_object(result);
    CosTransactions::Control_var control_ = CosTransactions::Control::_narrow(obj);
	try
	{
//		control_ = stack_control_.top();
//		stack_control_.pop();
		txnCurrent_ -> resume(control_);
	}
	
	catch (CosTransactions::InvalidControl& /*e*/)
	{
		throw Components::Transaction::InvalidToken();
		TRACE0(LEVEL6,"UserTransaction_impl::resume() has a invalid Token.\n");
	}
			
	catch (Components::Transaction::SystemError& /*e*/)
	{
		throw Components::Transaction::SystemError();
		TRACE0(LEVEL6,"UserTransaction_impl::resume() raises a Components::Transaction::SystemError() execption.\n");
	}
	
	catch (...)
	{
		TRACE0(LEVEL6,"UserTransaction_impl::resume() raises a CORBA::SystemException() execption.\n");
	}
    
}
