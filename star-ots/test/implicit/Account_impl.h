#ifndef ___Account_impl_h__
#define ___Account_impl_h__

#include <Account_skel.h>

#include "CosTransactions.h"
#include "Resource.h"
#include <OTSPostgreSQL.h>

//
// Interface declare ::Account
//
class Account_impl : virtual public POA_Account,
                     virtual public PortableServer::RefCountServantBase
{
    Account_impl(const Account_impl&);
    void operator=(const Account_impl&);

    //
    // The ORB
    //
    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

	//
    // The Resource POA
    //
    PortableServer::POA_var resourcePOA_;

    //
    // Cached CosTransactions::Current
    //
    CosTransactions::Current_var current_;

	CosTransactions::TransactionFactory_var TF_var_;

	ResourceFactory_var RF_var_;

	Resource_var bank_A_;

	CosTransactions::Control_var Test_Ctl_;
	CosTransactions::Coordinator_var Test_Coord_;
	CosTransactions::RecoveryCoordinator_var Test_Rec_;

public:

    Account_impl(PortableServer::POA_ptr);
	Account_impl(CORBA::ORB_ptr,
		 PortableServer::POA_ptr,
		 CosTransactions::Current_ptr);
    ~Account_impl();

    virtual PortableServer::POA_ptr _default_POA();

    virtual void connect()
        throw(CORBA::SystemException);
	
	virtual void close()
        throw(CORBA::SystemException);
    
	virtual void sum()
        throw(CORBA::SystemException);

    //
    // Operation declare deposit
    //
    virtual void deposit()
        throw(CORBA::SystemException);

    //
    // Operation declare withdraw
    //
    virtual void withdraw()
        throw(CORBA::SystemException);

};

#endif
