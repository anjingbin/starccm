// *************************************************************************
//
// This File Is Automatically Generated by the StarBusIDL-to-C++ Compiler !
//
// Copyright (c) 2003
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// *************************************************************************

// Version: 5.0.0

#ifndef ___counter_skel_h__
#define ___counter_skel_h__

#include <counter.h>
#include <STAR/SkelForServerRequest.h>

//
// Interface declare ::Counter
//
class POA_Counter : virtual public PortableServer::ServantBase,
                    virtual public STAR::SkelForServerRequest
{
    POA_Counter(const POA_Counter&);
    void operator=(const POA_Counter&);

protected:

    //
    // ProtectedAttribute declare count
    //
    static void _STAR_get_count(PortableServer::ServantBase *,STAR::Upcall_ptr);

    //
    // ProtectedOperation declare increment
    //
    static void _STAR_op_increment(PortableServer::ServantBase *,STAR::Upcall_ptr);

    //
    // ProtectedOperation declare decrement
    //
    static void _STAR_op_decrement(PortableServer::ServantBase *,STAR::Upcall_ptr);

public:

    POA_Counter() { }

    virtual CORBA::Boolean _is_a(const char*)
        throw(CORBA::SystemException);

    virtual CORBA::RepositoryId _primary_interface(const PortableServer::ObjectId&,
                                                   PortableServer::POA_ptr);

    Counter_ptr _this();

    virtual void _STAR_dispatch(STAR::Upcall_ptr);

    //
    // Attribute declare count
    //
    virtual CORBA::Long count()
        throw(CORBA::SystemException) = 0;

    //
    // Operation declare increment
    //
    virtual void increment()
        throw(CORBA::SystemException) = 0;

    //
    // Operation declare decrement
    //
    virtual void decrement()
        throw(CORBA::SystemException) = 0;
};

#endif
