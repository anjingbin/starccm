#include <STAR/CORBA.h>
#include <Account_impl.h>

#include <math.h>
#include <OTSHeader.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

//
// Interface declare ::Account
//
Account_impl::Account_impl(PortableServer::POA_ptr poa)
    : poa_(PortableServer::POA::_duplicate(poa))
{

}

Account_impl::Account_impl(CORBA::ORB_ptr orb,
			   PortableServer::POA_ptr poa, 
               CosTransactions::Current_ptr current)
    : orb_(CORBA::ORB::_duplicate(orb)),
      poa_(PortableServer::POA::_duplicate(poa)), 
      current_(CosTransactions::Current::_duplicate(current))
{
		resourcePOA_ = PortableServer::POA::_narrow(
		CORBA::Object_var(orb_ -> resolve_initial_references("RootPOA")));
}

Account_impl::~Account_impl()
{
}

PortableServer::POA_ptr
Account_impl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}

void 
Account_impl::connect()
        throw(CORBA::SystemException)
{
		#ifdef _DEBUG_
			cout << "    [Account_impl::connect() begin!]"
			     << endl;
		#endif	//_DEBUG_  

		//
		// Get  objects of ResourceFactory
		//
			
		const char* res_refFile = "rf.ref";
		ifstream res_in(res_refFile);
		
		if(res_in.fail())
		{
			extern int errno;
			cerr << ": can't open `" << res_refFile << "': "
			 << strerror(errno) << endl;
			//return 1;
		}

		char S_RF[1000];
		
		res_in >> S_RF;
		res_in.close();
			
		CORBA::Object_var RF = orb_ -> string_to_object(S_RF);
		assert(!CORBA::is_nil(RF));
		
		RF_var_	= ResourceFactory::_narrow(RF);
		assert(!CORBA::is_nil(RF_var_));


		//
		// Create resource:
		//

		bank_A_ = RF_var_ -> create(Postgre);

		cout << "\n\t[Create resource succeed!]" << endl;

		// These are the variables used to connect to the PostgreSQL DB.
		char	*pghost, *pgport, *pgoptions, *pgtty,*login,*pwd;
		pghost = "";	// host name of the backend server
		pgport = "";	// port of the backend server
		pgoptions = "";	// special options to start up the backend server
		pgtty = "";	// debugging tty for the backend server
		//renyi andded 6 30 2003
		login = "";	// the login account 
		pwd = "";	// the password	

		// Connect to the database -- bank
		//renyi changed 6 30 2003
		//bank_A_ -> pg_PQsetdbLogin(pghost, pgport, pgoptions, pgtty, "bank");
		bank_A_ -> pg_PQsetdbLogin("172.26.13.91", "5678", pgoptions, pgtty, "bank","renyi","renxiaoyi");

		// check to see that the backend connection was successfully made
		if (bank_A_ -> pg_PQstatus() == pg_CONNECTION_BAD)
		{
			cout << "\t[Connection to DB postgres failed.]\n";
			cout << bank_A_ -> pg_PQerrorMessage() << endl;
			exit(-1);
		}

		Test_Ctl_ = current_ -> get_control();
		cout << "\t[Current get_control is OK!]\n";

		Test_Coord_ = Test_Ctl_ -> get_coordinator();
		cout << "\t[Control get_coordinator is OK!]\n";

		Test_Rec_ = Test_Coord_ -> register_resource(bank_A_.in());
		cout << "\t[The Resource is registered!]\n";
		
		#ifdef _DEBUG_
			cout << "    [Account_impl::connect() end!]"
			     << endl;
		#endif

}

void
Account_impl::close()
    throw(CORBA::SystemException)
{
		#ifdef _DEBUG_
			cout << "    [Account_impl::close() begin!]"
			     << endl;
		#endif	//_DEBUG_  

		// close the connection to the database and cleanup
		bank_A_ -> pg_PQfinish();
	
		try
		{
			bank_A_ -> release ();
		}
		catch (CORBA::COMM_FAILURE)
		{
			cout << "\t[close connection end!]" << endl;
		}			

		try
		{
			Test_Ctl_ -> release ();
		}
		catch (CORBA::COMM_FAILURE)
		{
			cout << "\t[close connection end!]" << endl;
		}
		
		#ifdef _DEBUG_
			cout << "    [Account_impl::close() end!]"
			     << endl;
		#endif	//_DEBUG_  

}

void
Account_impl::sum()
    throw(CORBA::SystemException)
{
		#ifdef _DEBUG_
			cout << "    [Account_impl::sum() begin!]"
			     << endl;
		#endif	//_DEBUG_  

		// These are the variables used to store the returning data.
		CORBA::Long	nFields, i, j;

		bank_A_ -> pg_PQexec("select * from account;");
		if (bank_A_ -> pg_PQresultStatus() != pg_PGRES_TUPLES_OK)
		{
			bank_A_ -> pg_PQclear();
		}

		/* first, print out the attribute names */
		nFields = bank_A_ -> pg_PQnfields();
		for (i = 0; i < nFields; i++)
			cout << "\t\t" << (char*)bank_A_ -> pg_PQfname(i);
		cout << endl;

		cout << "-----------------------------------------\n\n";
		
		// Print out instances.
		for (i = 0; i < bank_A_ -> pg_PQntuples(); i++)
		{
			for (j = 0; j < nFields; j++)
				cout << "\t" 
				     << (char*)bank_A_ -> pg_PQgetvalue(i, j);
			cout << endl;
		}

		bank_A_ -> pg_PQclear();

		#ifdef _DEBUG_
			cout << "    [Account_impl::sum() end!]"
			     << endl;
		#endif	//_DEBUG_  

}

//
// Operation declare deposit
//
void
Account_impl::deposit()
    throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "    [Account_impl::deposit() begin!]"
			 << endl;
	#endif	//_DEBUG_  

	// Command phase
	bank_A_ -> pg_PQexec("update account set money=money+100;");
	if (bank_A_ -> pg_PQresultStatus() != pg_PGRES_COMMAND_OK)
	{
		cout << "\t[update money command failed]\n";
		bank_A_ -> pg_PQclear();
	}
	bank_A_ -> pg_PQclear();

	#ifdef _DEBUG_
		cout << "    [Account_impl::deposit() end!]"
			 << endl;
	#endif	//_DEBUG_  

}

//
// Operation declare withdraw
//
void
Account_impl::withdraw()
    throw(CORBA::SystemException)
{
	#ifdef _DEBUG_
		cout << "    [Account_impl::withdraw() begin!]"
			 << endl;
	#endif	//_DEBUG_  

	// Command phase
	bank_A_ -> pg_PQexec("update account set money=money-100;");
	if (bank_A_ -> pg_PQresultStatus() != pg_PGRES_COMMAND_OK)
	{
		cout << "\t[update money command failed]\n";
		bank_A_ -> pg_PQclear();
	}
	bank_A_ -> pg_PQclear();
	
	#ifdef _DEBUG_
		cout << "    [Account_impl::withdraw() end!]"
			 << endl;
	#endif	//_DEBUG_  
}

