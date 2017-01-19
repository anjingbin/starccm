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

#include <CosTransactions.h>
#include <CosTransactionsOTS.h>
#include <OTS.h>

#include <Account.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

void
help()
{
    cout << "Enter Command:\n";
    cout << "  B\tBegin new Transaction\n";
    cout << "  R\tRollback current transaction\n";
    cout << "  C\tCommit current transaction\n";
    cout << "  I\tDeposit current account\n";
    cout << "  D\tWithdraw current account\n";
    cout << "  P\tPrint current account value\n";
    cout << "  X\tExit\n";
    cout << flush;
}

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
    cout << endl;
	cout << "********************************************************************" << endl;
	cout << "     This program is to test implicit transaction propagation......     " << endl;
	cout << "********************************************************************" << endl;
    //
    // Get "Account" object
    //
    CORBA::Object_var obj = orb -> string_to_object("relfile:/Account.ref");
    if(CORBA::is_nil(obj))
    {
		cerr << argv[0] << ": cannot read IOR from Account.ref" << endl;
		return EXIT_FAILURE;
    }
    
    Account_var account = Account::_narrow(obj);
    assert(!CORBA::is_nil(account));

    //
    // Get the CosTransactions::Current
    //
    CosTransactions::Current_var current =
	CosTransactions::Current::_narrow(
	    CORBA::Object_var(
		orb -> resolve_initial_references("TransactionCurrent")));
    cout << "	[get current object is ok!]" << endl;
    
    //
    // To ensure transactions aren't left dangling set a timeout of 5
    // minutes. After this time the transaction is automatically rolled
    // back.
    //
    current -> set_timeout(300);

    //
    // Main loop
    //
    help();
    char c;
    do
    {
	try
	{
	    cout << "> " << flush;
	    cin >> c;
	    switch(c)
	    {
	    case 'b':
		case 'B':
            {
                CosTransactions::Control_var currentCtrl = 
                    current -> get_control();
                cout << "	[get control object from current object is ok!]" << endl;
                if(!CORBA::is_nil(currentCtrl))
                    cout << "Transaction active" << endl;
                else
                {
                    current -> begin();
                    cout << "	[current begin is ok!]" << endl;
					account -> connect();
                }
            }
            break;

	    case 'r':
	    case 'R':
	    {
		cout << "	[now is rollbacking...]" << endl;
		current -> rollback();
		cout << "	[current rollback is ok!]" << endl;
		
		cout << "	[After rollback of the transaction,]" << endl;
		cout << "	[close connection to database and cleanup objects instances...]" << endl;
		cout << "	[Account::close() begin without in a transaction context!]" << endl;
		cout << "	[If TRANSACTION_REQUIRED EXCEPTION is reported, it's just all right!:)]" << endl;
		account -> close();
		cout << "	[connection to the database is closed and objects is cleaned up!]" << endl;

		break;
	    }

	    case 'c':
	    case 'C':
	    {
		cout << "	[now is committing...]" << endl;
		current -> commit(false);
		cout << "	[current commit is ok!]" << endl;
		
		cout << "	[After commit of the transaction,]" << endl;
		cout << "	[close connection to database and cleanup objects instances...]" << endl;
		cout << "	[Account::close() begin without in a transaction context!]" << endl;
		cout << "	[If TRANSACTION_REQUIRED EXCEPTION is reported, it's just all right!:)]" << endl;

		account -> close();
		cout << "	[connection to the database is closed and objects is cleaned up!]" << endl;
		
		break;
	    }

	    case 'i':
	    case 'I':
	    cout << "	[transaction is implicit propagating to server side]" << endl;
		account -> deposit();
		cout << "	[transactional operation is ok!]" << endl;
		break;

	    case 'd':
	    case 'D':
	    cout << "	[transaction is implicit propagating to server side]" << endl;
		account -> withdraw();
		cout << "	[transactional operation is ok!]" << endl;
		break;

	    case 'p':
	    case 'P':
		account -> sum();
		cout << "Print the account on the server side...... " << endl;
		break;

	    case 'x':
	    case 'X':
		break;

	    default:
		help();
	    }
	}
	catch(const CORBA::Exception& ex)
	{
	    cerr << ex << endl;
	}
    }
    while(cin.good() && c != 'x' && c != 'X');

	cout << "Implicit transaction propogation is ok!" << endl;
    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;

    try
    {
	OB::OTSInit(argc, argv);
	orb = CORBA::ORB_init(argc, argv);
	status = run(orb, argc, argv);
    }
    catch(const CORBA::Exception& ex)
    {
	cerr << ex << endl;
	status = EXIT_FAILURE;
    }

    if(!CORBA::is_nil(orb))
    {
	try
	{
	    orb -> destroy();
	}
	catch(const CORBA::Exception& ex)
	{
	    cerr << ex << endl;
	    status = EXIT_FAILURE;
	}
    }
    
    return status;
}
