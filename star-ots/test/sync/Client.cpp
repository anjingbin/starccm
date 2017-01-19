// **********************************************************************
//
// Copyright (c) 2000
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#include <STAR/CORBA.h>

#include <ImplicitOTSInterceptor.h>

#include <Counter.h>

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
    cout << "  I\tIncrement current counter\n";
    cout << "  D\tDecrement current counter\n";
    cout << "  X\tExit\n";
    cout << flush;
}

int
run(CORBA::ORB_ptr orb, int argc, char* argv[])
{
    cout << endl;
	cout << "********************************************************************" << endl;
	cout << "     This program is to test synchronization resource......     " << endl;
	cout << "********************************************************************" << endl;
    //
    // Get "Counter" object
    //
    CORBA::Object_var obj = orb -> string_to_object("relfile:/Counter.ref");
    if(CORBA::is_nil(obj))
    {
	cerr << argv[0] << ": cannot read IOR from Counter.ref" << endl;
	return EXIT_FAILURE;
    }
    
    Counter_var counter = Counter::_narrow(obj);
    assert(!CORBA::is_nil(counter));

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
                }
            }
            break;

	    case 'r':
	    case 'R':
	    {
		cout << "	[now is rollbacking...]" << endl;
		current -> rollback();
		cout << "	[current rollback is ok!]" << endl;
		break;
	    }

	    case 'c':
	    case 'C':
	    {
		cout << "	[now is committing...]" << endl;
		current -> commit(true);
		cout << "	[current commit is ok!]" << endl;
		break;
	    }

	    case 'i':
	    case 'I':
	    cout << "	[transaction is implicit propagating to server side]" << endl;
		counter -> increment();
		cout << "	[transactional operation is ok!]" << endl;
		break;

	    case 'd':
	    case 'D':
	    cout << "	[transaction is implicit propagating to server side]" << endl;
		counter -> decrement();
		cout << "	[transactional operation is ok!]" << endl;
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

	cout << "Synchronization resource test is ok!" << endl;
    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;

    try
    {
	STAR::OTSInit(argc, argv);
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
