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
#include <Resource_impl.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

ResourceFactory_impl::ResourceFactory_impl(CORBA::ORB_var orb)
{
    // TODO: Add your constructor code here

   	_orb_var = CORBA::ORB::_duplicate(orb);
	
	// get global transcation identifier counter
	_global_id_counter = get_global_id();

	// initialize local transcation identifier counter 
	_local_id_counter = 0;
	
	#ifdef _DEBUG_
		cout << "[ResourceFactory::constructor is OK!]" << endl;
	#endif

}

Resource_ptr ResourceFactory_impl::create(CORBA::Short dbtype)
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your create implemention code here

	#ifdef _DEBUG_
		cout << "[ResourceFactory::create begin!]"
		     << endl;
	#endif	//_DEBUG_  
	     	     
	Resource_impl* Res1 = new Resource_impl(NULL, dbtype);
	Resource_var Res = Res1 ->_this();

	Resource::_duplicate(Res);


	#ifdef _DEBUG_
		cout << "\t[return Resource Object reference]"
		     << endl
		     << "\t[ResourceFactory::create end!]"
		     << endl;
	#endif
    
	return Resource::_duplicate(Res);
        
}

int ResourceFactory_impl::get_global_id()
{
	// get global transction identifier from identifier file

	struct RWFileErr{};

	int global_id;

	#ifdef _DEBUG_
		cout << "[ResourceFactory::get_global_id begin!]"
		     << endl;
	#endif	//_DEBUG_  

	char *filename = "global_id001.dat";
	FILE *idfile = fopen(filename, "r+");

	if(idfile == NULL)
	{
		#ifdef _DEBUG_
			cout << "\t[global identifier counter file does not exit!]"
				 << endl
				 << "\t[create a new and write global identifier counter to file!]"
				 << endl;
		#endif	//_DEBUG_  
		
		idfile = fopen(filename, "w");

		if(idfile == NULL)
		{
			#ifdef _DEBUG_
			    cout << "\t[Create global identifier counter file failed!]"
					 << endl
					 << "\t[Thorw RWFileErr!]" << endl;
			#endif	//_DEBUG_

			throw RWFileErr();
		}

		global_id = 1;
		if(fwrite(&global_id, sizeof(int), 1, idfile) != 1)
		{
			fclose(idfile);

			#ifdef _DEBUG_
				cout <<"\t[Write global identifier counter to file falied!]"
					 << endl
					 <<"\t[Thorw RWFileErr!]" << endl;
			#endif	//_DEBUG_

			throw RWFileErr();
		}

		fclose(idfile);

	}
	else
	{

		#ifdef _DEBUG_
			cout << "\t[Read global identifile from file!]"
				 << endl;
		#endif	//_DEBUG_  

		if(fread(&global_id, sizeof(int), 1, idfile) != 1)
		{
			fclose(idfile);

			#ifdef _DEBUG_
				cout <<"\t[Read global identifier counter from file falied!]"
					 << endl
					 <<"\t[Throw RWFileErr!]" << endl;
			#endif	//_DEBUG_

			throw RWFileErr();
		}

		#ifdef _DEBUG_
			cout << "\t[Write global identifier to file!]"
				 << endl;
		#endif	//_DEBUG_  

		global_id ++;

		rewind(idfile);
		if(fwrite(&global_id, sizeof(int), 1, idfile) != 1)
		{
			fclose(idfile);

			#ifdef _DEBUG_
				cout <<"\t[Write global identifier counter to file falied!]"
					 << endl
					 <<"\t[Thorw RWFileErr!]" << endl;
			#endif	//_DEBUG_

			throw RWFileErr();
		}

	}

	fclose(idfile);

	#ifdef _DEBUG_
		cout << "\t[ResourceFactory::get_global_id end!]"
		     << endl;
	#endif	//_DEBUG_  

	return global_id;

}

Resource_impl::Resource_impl(PortableServer::POA_var poa, 
							 CORBA::Short dbtype)
	:PG_Resource_impl(poa)
{
    // TODO: Add your constructor code here
	
	#ifdef _DEBUG_
		cout << "[Resource::constructor(poa, dbtype) begin!]" << endl;
	#endif
	
	_dbtype = dbtype;
	
	#ifdef _DEBUG_
		cout << "\t[Resource::constructor(poa, dbtype) end!]" << endl;
	#endif

}

Resource_impl::Resource_impl(CosTransactions::Coordinator_ptr c, 
							 CORBA::Short dbtype)
	:PG_Resource_impl(c)
{
    // TODO: Add your constructor code here
	
	#ifdef _DEBUG_
		cout << "[Resource::constructor(c, dbtype) begin!]" << endl;
	#endif
	
	_dbtype = dbtype;
	
	#ifdef _DEBUG_
		cout << "\t[Resource::constructor(c, dbtype) end!]" << endl;
	#endif

}

CosTransactions::Vote Resource_impl::prepare()
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
		cout << "[Resource::prepare() begin!]" << endl;
	#endif
	
		CosTransactions::Vote v;
	
	switch (_dbtype)
	{
		case Postgre:
			#ifdef _DEBUG_
				cout << "\t[pg_prepare()]" << endl;
			#endif
			v = pg_prepare();
			break;
			
	} // eof 'switch'
	
	#ifdef _DEBUG_
		cout << "\t[Resource::prepare() end!]" << endl;
	#endif
	
	return v;
}

void Resource_impl::rollback()
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
		cout << "[Resource::rollback() begin!]" << endl;
	#endif
	
	switch (_dbtype)
	{
		case Postgre:
			#ifdef _DEBUG_
				cout << "\t[pg_rollback()]" << endl;
			#endif
			pg_rollback();
			break;
			
	} // eof 'switch'
	
	#ifdef _DEBUG_
		cout << "\t[Resource::rollback() end!]" << endl;
	#endif
	
	return;
}

void Resource_impl::commit()
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
		cout << "[Resource::commit() begin!]" << endl;
	#endif
	
    try
    {
		switch (_dbtype)
		{
			case Postgre:
				#ifdef _DEBUG_
					cout << "\t[pg_commit()]" << endl;
				#endif
				pg_commit();
				break;
			
		} // eof 'switch'
	
    } // eof 'try'
    
    catch (CosTransactions::NotPrepared)
    {
 		#ifdef _DEBUG_
			cout << "\t[throw exception: CosTransactions::NotPrepared]" << endl;
		#endif
		
			throw CosTransactions::NotPrepared();  
    }
    
    catch (CosTransactions::HeuristicRollback)
    {
 		#ifdef _DEBUG_
			cout << "\t[throw exception: CosTransactions::HeuristicRollback]" << endl;
		#endif
		
			throw CosTransactions::HeuristicRollback();  
    }
    
	
	#ifdef _DEBUG_
		cout << "\t[Resource::commit() end!]" << endl;
	#endif
	
	return;

}

void Resource_impl::commit_one_phase()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED,
        	CosTransactions::HeuristicHazard)
{
    // TODO:Add your commit_one_phase implemention code here

	#ifdef _DEBUG_
		cout << "[Resource::commit_one_phase() begin!]" << endl;
	#endif
	
    try
    {
		switch (_dbtype)
		{
			case Postgre:
				#ifdef _DEBUG_
					cout << "\t[pg_commit_one_phase()]" << endl;
				#endif
				pg_commit_one_phase();
				break;
			
		} // eof 'switch'
	
    } // eof 'try'
    
    catch (CosTransactions::TRANSACTION_ROLLEDBACK)
    {
 		#ifdef _DEBUG_
			cout << "\t[throw exception: CosTransactions::TRANSACTION_ROLLEDBACK]" << endl;
		#endif
		
			throw CosTransactions::TRANSACTION_ROLLEDBACK();  
    }    
	
	#ifdef _DEBUG_
		cout << "\t[Resource::commit_one_phase() end!]" << endl;
	#endif
	
	return;

}

void Resource_impl::forget()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your forget implemention code here

	#ifdef _DEBUG_
		cout << "[Resource::forget() begin!]" << endl;
	#endif
	
	switch (_dbtype)
	{
		case Postgre:
			#ifdef _DEBUG_
				cout << "\t[pg_forget()]" << endl;
			#endif
			pg_forget();
			break;
			
	} // eof 'switch'
		
	#ifdef _DEBUG_
		cout << "\t[Resource::forget() end!]" << endl;
	#endif
	
	return;

}

void Resource_impl::release()
        	throw (CORBA::SystemException)
{
    // TODO:Add your release implemention code here

	#ifdef _DEBUG_
		cout << "[Resource::release() begin!]" << endl;
	#endif
	
	switch (_dbtype)
	{
		case Postgre:
			#ifdef _DEBUG_
				cout << "\t[pg_release()]" << endl;
			#endif
			pg_release();
			break;
			
	} // eof 'switch'
		
	#ifdef _DEBUG_
		cout << "\t[Resource::release() end!]" << endl;
	#endif
	
	return;
}

CORBA::Boolean Resource_impl::BeginTrans()
        	throw (CORBA::SystemException)
{
    // TODO:Add your BeginTrans implemention code here

	#ifdef _DEBUG_
		cout << "[Resource::BeginTrans() begin!]" << endl;
	#endif
	
	CORBA::Boolean ret;
	
	switch (_dbtype)
	{
		case Postgre:
			#ifdef _DEBUG_
				cout << "\t[pg_BeginTrans()]" << endl;
			#endif
			ret = pg_BeginTrans();
			break;
			
	} // eof 'switch'
		
	#ifdef _DEBUG_
		cout << "\t[Resource::BeginTrans() end!]" << endl;
	#endif
	
	return ret;
}

void Resource_impl::set_recoverycoordinator(CosTransactions::RecoveryCoordinator_ptr rc)
        	throw (CORBA::SystemException,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your set_recoverycoordinator implemention code here

	#ifdef _DEBUG_
		cout << "[Resource::set_recoverycoordinator(rc) begin!]" << endl;
	#endif
	
	switch (_dbtype)
	{
	case Postgre:
		#ifdef _DEBUG_
			cout << "\t[pg_set_recoverycoordinator(rc)]" << endl;
		#endif
		pg_set_recoverycoordinator(rc);
		break;
			
	} // eof 'switch'
		
	#ifdef _DEBUG_
		cout << "\t[Resource::set_recoverycoordinator(rc) end!]" << endl;
	#endif
	
	return;
}

void Resource_impl::heuristic_decision()
        	throw (CORBA::SystemException,
        	CosTransactions::WRONG_TRANSACTION,
        	CosTransactions::INVALID_TRANSACTION,
        	CosTransactions::TRANSACTION_ROLLEDBACK,
        	CosTransactions::TRANSACTION_REQUIRED)
{
    // TODO:Add your heuristic_decision implemention code here

	#ifdef _DEBUG_
		cout << "[Resource::heuristic_decision() begin!]" << endl;
	#endif
	
	switch (_dbtype)
	{
	case Postgre:
		#ifdef _DEBUG_
			cout << "\t[pg_heuristic_decision()]" << endl;
		#endif
		pg_heuristic_decision();
		break;
			
	} // eof 'switch'
		
	#ifdef _DEBUG_
		cout << "\t[Resource::heuristic_decision() end!]" << endl;
	#endif
	
	return;
}
