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

#ifndef ___CosTransactionsUser_h__
#define ___CosTransactionsUser_h__

#include <CosTransactionsOTS.h>

namespace CosTransactions
{

class Synchronization;
typedef Synchronization* Synchronization_ptr;
typedef Synchronization* SynchronizationRef;

extern STAR::TypeCodeConst _tc_Synchronization;

} // End of namespace CosTransactions

void STARDuplicate(CosTransactions::Synchronization_ptr);
void STARRelease(CosTransactions::Synchronization_ptr);

void STARMarshal(CosTransactions::Synchronization_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(CosTransactions::Synchronization_ptr&, STAR::InputStreamImpl*);

namespace CosTransactions
{

typedef STAR::ObjVar< Synchronization > Synchronization_var;
typedef STAR::ObjOut< Synchronization > Synchronization_out;

} // End of namespace CosTransactions

//
// Module declare ::CosTransactions
//
namespace CosTransactions
{

//
// Interface declare ::CosTransactions::Synchronization
//
class Synchronization : virtual public CosTransactions::TransactionalObject
{
    Synchronization(const Synchronization&);
    void operator=(const Synchronization&);

protected:

    static const char* ids_[];

public:

    Synchronization() { }
    virtual ~Synchronization() { }

    typedef Synchronization_ptr _ptr_type;
    typedef Synchronization_var _var_type;

    static inline Synchronization_ptr
    _duplicate(Synchronization_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline Synchronization_ptr
    _nil()
    {
        return 0;
    }

    static Synchronization_ptr _narrow(CORBA::Object_ptr);
    static Synchronization_ptr _narrow(CORBA::AbstractBase_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::CosTransactions::Synchronization::before_completion
    //
    virtual void before_completion() = 0;

    //
    // Operation declare ::CosTransactions::Synchronization::after_completion
    //
    virtual void after_completion(Status s) = 0;
};


class Synchronization_broker : virtual public CosTransactions::Synchronization,
                               virtual public CosTransactions::TransactionalObject_broker
{
    Synchronization_broker(const Synchronization_broker&);
    void operator=(const Synchronization_broker&);

public:

    Synchronization_broker() { }
    virtual ~Synchronization_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::CosTransactions::Synchronization_broker::before_completion
    //
    void before_completion();

    //
    // Operation declare ::CosTransactions::Synchronization_broker::after_completion
    //
    void after_completion(CosTransactions::Status s);
};

} // End of namespace CosTransactions

//
// Module declare ::CosTransactions
//
namespace OBV_CosTransactions
{

} // End of namespace OBV_CosTransactions

//
// Interface declare ::CosTransactions::Synchronization
//
namespace CORBA
{

inline void
release(CosTransactions::Synchronization_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(CosTransactions::Synchronization_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, CosTransactions::Synchronization_ptr*);
void operator<<=(CORBA::Any&, CosTransactions::Synchronization_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, CosTransactions::Synchronization_ptr&);

inline void
operator<<=(CORBA::Any_var& any, CosTransactions::Synchronization_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, CosTransactions::Synchronization_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, CosTransactions::Synchronization_ptr& val)
{
    return any.in() >>= val;
}

#endif
