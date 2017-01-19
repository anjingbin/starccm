// **********************************************************************
//
// Generated by the ORBacus IDL-to-C++ Translator
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

// Version: 4.1.0

#ifndef ___CosTransactionsUser_skel_h__
#define ___CosTransactionsUser_skel_h__

#include <CosTransactionsUser.h>

#include <CosTransactionsOTS_skel.h>

#ifndef OB_INTEGER_VERSION
#   error No ORBacus version defined! Is <OB/CORBA.h> included?
#endif

#ifndef OB_NO_VERSION_CHECK
#   if (OB_INTEGER_VERSION != 4010000L)
#       error ORBacus version mismatch!
#   endif
#endif

//
// IDL:CosTransactions:1.0
//
namespace POA_CosTransactions
{

//
// IDL:CosTransactions/Synchronization:1.0
//
class Synchronization : virtual public POA_CosTransactions::TransactionalObject
{
    Synchronization(const Synchronization&);
    void operator=(const Synchronization&);

protected:

    //
    // IDL:CosTransactions/Synchronization/before_completion:1.0
    //
    void _OB_op_before_completion(OB::Upcall_ptr);

    //
    // IDL:CosTransactions/Synchronization/after_completion:1.0
    //
    void _OB_op_after_completion(OB::Upcall_ptr);

public:

    Synchronization() { }

    virtual CORBA::Boolean _is_a(const char*)
        throw(CORBA::SystemException);

    virtual CORBA::RepositoryId _primary_interface(const PortableServer::ObjectId&,
                                                   PortableServer::POA_ptr);

    CosTransactions::Synchronization_ptr _this();

    virtual OB::DirectStubImpl_ptr _OB_createDirectStubImpl(PortableServer::POA_ptr,
                                                            const PortableServer::ObjectId&);
    virtual void _OB_dispatch(OB::Upcall_ptr);

    //
    // IDL:CosTransactions/Synchronization/before_completion:1.0
    //
    virtual void before_completion()
        throw(CORBA::SystemException) = 0;

    //
    // IDL:CosTransactions/Synchronization/after_completion:1.0
    //
    virtual void after_completion(CosTransactions::Status s)
        throw(CORBA::SystemException) = 0;
};

} // End of namespace POA_CosTransactions

//
// IDL:CosTransactions:1.0
//
namespace OBDirectStubImpl_CosTransactions
{

//
// IDL:CosTransactions/Synchronization:1.0
//
class Synchronization : virtual public OBStubImpl_CosTransactions::Synchronization,
                        virtual public OBDirectStubImpl_CosTransactions::TransactionalObject
{
    Synchronization(const Synchronization&);
    void operator=(const Synchronization&);

protected:

#ifdef HAVE_VCPLUSPLUS_BUGS

    Synchronization() { }

#endif

    Synchronization(PortableServer::POA_ptr,
                    const PortableServer::ObjectId&,
                    PortableServer::ServantBase*);

    friend class POA_CosTransactions::Synchronization;

public:

    //
    // IDL:CosTransactions/Synchronization/before_completion:1.0
    //
    virtual void before_completion();

    //
    // IDL:CosTransactions/Synchronization/after_completion:1.0
    //
    virtual void after_completion(CosTransactions::Status s);
};

} // End of namespace OBDirectStubImpl_CosTransactions

#endif