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

/*
 * 	testlibpq1.cpp
 *
 * This test programs shows to use LIBPQ to make multiple backend
 * connections
 *
 */

#include <OB/CORBA.h>

#include <CosTransactionsOTS.h>
#include <CosTransactions.h>
#include <Resource.h>
#include <OTS.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "libpq-fe.h"
#include "OTSHeader.h"

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

static void
check_conn(Resource_var res)
{
	// check to see that the backend connection was successfully made
	if (res -> pg_PQstatus() == CONNECTION_BAD)
	{
		cout << "\t[Connection to DB postgres failed.]\n";
		cout << res -> pg_PQerrorMessage() << endl;
		exit(-1);
	}
}


int main_loop (ResourceFactory_var RF_var,
			   int MAXTIMES,CORBA::ORB_ptr orb)
{
	
	long full;
	if (MAXTIMES == 0)
		full = (long)(pow(2,32)) - 1;
	else	full = MAXTIMES;
	
	CosTransactions::Control_var Test_Ctl;
	CosTransactions::Coordinator_var Test_Coord;
	CosTransactions::Terminator_var Test_Term;
	Resource_var bank_A;
	CosTransactions::RecoveryCoordinator_var Test_Rec;
	
	// These are the variables used to store the returning data.
	char	*pghost, *pgport, *pgoptions, *pgtty;
	CORBA::Long	nFields, i, j;
	pghost = "";	// host name of the backend server
	pgport = "";	// port of the backend server
	pgoptions = "";	// special options to start up the backend server
	pgtty = "";	// debugging tty for the backend server	

	//
    // Get the CosTransactions::Current
    //
    CosTransactions::Current_var current =
	    CosTransactions::Current::_narrow(
	        CORBA::Object_var(
		    orb -> resolve_initial_references("TransactionCurrent")));

    cout << "	[get current object is ok!]" << endl;
	
	//
	// main loop:
	//
	
	for (int times=1; times<=full; times++)
	{	
	//
	// Create a new transaction
	//
	
	//Test_Ctl = TF_var -> create(0);
	//Test_Coord = Test_Ctl -> get_coordinator();
	//Test_Term = Test_Ctl -> get_terminator();
	current -> begin();
	Test_Ctl = current -> get_control();
	Test_Coord = Test_Ctl -> get_coordinator();

	//
	// Create resource:
	//

	bank_A = RF_var -> create(Postgre);

	// Connection phase
	bank_A -> pg_PQsetdb(pghost, pgport, pgoptions, pgtty, "bank");
	check_conn(bank_A);

	
	// ========== begin to example1 ==========
	
	Test_Rec = Test_Coord -> register_resource(bank_A.in());
	
	// Command phase
	bank_A -> pg_PQexec("update account set money=money+1;");
	if (bank_A -> pg_PQresultStatus() != PGRES_COMMAND_OK)
	{
		cout << "\t[update money command failed]\n";
		bank_A -> pg_PQclear();
	}
	bank_A -> pg_PQclear();

	bank_A -> pg_PQexec("select * from account;");
	if (bank_A -> pg_PQresultStatus() != PGRES_TUPLES_OK)
	{
		bank_A -> pg_PQclear();
	}

	/* first, print out the attribute names */
	nFields = bank_A -> pg_PQnfields();
	for (i = 0; i < nFields; i++)
		cout << "\t\t" << (char*)bank_A -> pg_PQfname(i);
	cout << endl;

	cout << "-----------------------------------------\n\n";
	
	// Print out instances.
	for (i = 0; i < bank_A -> pg_PQntuples(); i++)
	{
		for (j = 0; j < nFields; j++)
			cout << "\t" 
			     << (char*)bank_A -> pg_PQgetvalue(i, j);
		cout << endl;
	}

	bank_A -> pg_PQclear();

	try
	{
		// Terminate this transaction
		current -> commit(false);
	}
	catch (CosTransactions::HeuristicRollback& ex)
	{
		cout << "	[rollback!=====" << times << endl;
		bank_A -> pg_PQfinish();
		return 1;
	}
	
	// close the connection to the database and cleanup
	bank_A -> pg_PQfinish();
	
	bank_A -> release ();
	Test_Ctl -> release ();
	
	cout << "\n\t[times = " << times << "]\n";
	
	} // end of 'for(times)'
	
	cout << "	[OK!Test end!  @_@ ]" << endl;
	getchar();
	
	return 0;
	
}; // end of 'main_loop'



int main(int argc, char* argv[])
{

    try //try 1: begin
    {
	OB::OTSInit(argc, argv);
	//
	// Create ORB
	//
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
	
	
	//
	// Get  objects of OTS: CosTransactions::TransactionFactory
	//
		
	/*const char* ots_refFile = "ots.ref";
	ifstream ots_in(ots_refFile);
	
	if(ots_in.fail())
	{
	    extern int errno;
	    cerr << argv[0] << ": can't open `" << ots_refFile << "': "
		 << strerror(errno) << endl;
	    return 1;
	}

	char S_TF[1000];
	
	ots_in >> S_TF;
	ots_in.close();
		
	CORBA::Object_var TF = orb -> string_to_object(S_TF);
	assert(!CORBA::is_nil(TF));
	
	CosTransactions::TransactionFactory_var TF_var
			= CosTransactions::TransactionFactory::_narrow(TF);
	assert(!CORBA::is_nil(TF_var));
	*/

	
	//
	// Get  objects of PG_Resource: PG_ResourceFactory
	//
	const char* res_refFile = "rf.ref";
	ifstream res_in(res_refFile);
	
	if(res_in.fail())
	{
	    extern int errno;
	    cerr << argv[0] << ": can't open `" << res_refFile << "': "
		 << strerror(errno) << endl;
	    return 1;
	}

	char S_RF[1000];
	
	res_in >> S_RF;
	res_in.close();
		
	CORBA::Object_var RF = orb -> string_to_object(S_RF);
	assert(!CORBA::is_nil(RF));
	
	ResourceFactory_var RF_var
			= ResourceFactory::_narrow(RF);
	assert(!CORBA::is_nil(RF_var));
		
	//
	// main loop:
	//
	cout << "\t[This example will output all columns in table 'sysobjects']\n"
	     << endl;
	char timesstr[30];
	int times;
	cout << "\t[Please input the loop times]\n"
	     << "\t\t[0-----infinite]\n"
	     << "\t\t[other integer is the exact times to run]\n"
	     << "\tinput = ";
	cin >> timesstr;
	times = atoi(timesstr);
	
	//main_loop(RF_var, TF_var, times);
	main_loop(RF_var,times,orb);
	
	
	//delete []timesstr;
	getchar ();
	return 0;
	
    } //try 1: end
    
    
#ifdef __GNUG__
    catch(CORBA::COMM_FAILURE& ex) //catch 1: begin
#else
    catch(CORBA::SystemException& ex) //catch 1: begin
#endif
    {
		cerr << ex << endl;
		return 1;
    } //catch 1: end
    
    return 0;

}; //main: end
