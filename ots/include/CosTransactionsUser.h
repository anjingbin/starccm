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

#ifndef ___CosTransactionsUser_h__
#define ___CosTransactionsUser_h__

#include <CosTransactionsOTS.h>

#ifndef OB_INTEGER_VERSION
#   error No ORBacus version defined! Is <OB/CORBA.h> included?
#endif

#ifndef OB_NO_VERSION_CHECK
#   if (OB_INTEGER_VERSION != 4010000L)
#       error ORBacus version mismatch!
#   endif
#endif

namespace CosTransactions
{

class Synchronization;
typedef Synchronization* Synchronization_ptr;
typedef Synchronization* SynchronizationRef;

extern OB::TypeCodeConst _tc_Synchronization;

} // End of namespace CosTransactions

void OBDuplicate(CosTransactions::Synchronization_ptr);
void OBRelease(CosTransactions::Synchronization_ptr);

void OBMarshal(CosTransactions::Synchronization_ptr, OB::OutputStreamImpl*);
void OBUnmarshal(CosTransactions::Synchronization_ptr&, OB::InputStreamImpl*);

namespace CosTransactions
{

typedef OB::ObjVar< Synchronization > Synchronization_var;
typedef OB::ObjOut< Synchronization > Synchronization_out;

} // End of namespace CosTransactions

namespace OBStubImpl_CosTransactions
{

class Synchronization;
typedef Synchronization* Synchronization_ptr;

} // End of namespace OBStubImpl_CosTransactions

void OBDuplicate(OBStubImpl_CosTransactions::Synchronization_ptr);
void OBRelease(OBStubImpl_CosTransactions::Synchronization_ptr);

namespace OBStubImpl_CosTransactions
{

typedef OB::ObjVar< Synchronization > Synchronization_var;

} // End of namespace OBStubImpl_CosTransactions

//
// IDL:CosTransactions:1.0
//
namespace CosTransactions
{

//
// IDL:CosTransactions/Synchronization:1.0
//
class Synchronization : virtual public ::CosTransactions::TransactionalObject
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
            p -> _add_ref();
        return p;
    }

    static inline Synchronization_ptr
    _nil()
    {
        return 0;
    }

    static Synchronization_ptr _narrow(CORBA::Object_ptr);
    static Synchronization_ptr _unchecked_narrow(CORBA::Object_ptr);

    static Synchronization_ptr _narrow(CORBA::AbstractBase_ptr);
    static Synchronization_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);

    static const char** _OB_staticIds();

    //
    // IDL:CosTransactions/Synchronization/before_completion:1.0
    //
    virtual void before_completion() = 0;

    //
    // IDL:CosTransactions/Synchronization/after_completion:1.0
    //
    virtual void after_completion(Status s) = 0;
};

} // End of namespace CosTransactions

//
// IDL:CosTransactions:1.0
//
namespace OBProxy_CosTransactions
{

//
// IDL:CosTransactions/Synchronization:1.0
//
class Synchronization : virtual public ::CosTransactions::Synchronization,
                        virtual public OBProxy_CosTransactions::TransactionalObject
{
    Synchronization(const Synchronization&);
    void operator=(const Synchronization&);

protected:

    virtual OB::MarshalStubImpl_ptr _OB_createMarshalStubImpl();

public:

    Synchronization() { }
    virtual ~Synchronization() { }

    virtual const char** _OB_ids() const;

    //
    // IDL:CosTransactions/Synchronization/before_completion:1.0
    //
    void before_completion();

    //
    // IDL:CosTransactions/Synchronization/after_completion:1.0
    //
    void after_completion(::CosTransactions::Status s);
};

} // End of namespace OBProxy_CosTransactions

//
// IDL:CosTransactions:1.0
//
namespace OBStubImpl_CosTransactions
{

//
// IDL:CosTransactions/Synchronization:1.0
//
class Synchronization : virtual public OBStubImpl_CosTransactions::TransactionalObject
{
    Synchronization(const Synchronization&);
    void operator=(const Synchronization&);

protected:

    Synchronization() { }

public:

    static inline Synchronization_ptr
    _duplicate(Synchronization_ptr p)
    {
        if(p)
            p -> _OB_incRef();
        return p;
    }

    static inline Synchronization_ptr
    _nil()
    {
        return 0;
    }

    //
    // IDL:CosTransactions/Synchronization/before_completion:1.0
    //
    virtual void before_completion() = 0;

    //
    // IDL:CosTransactions/Synchronization/after_completion:1.0
    //
    virtual void after_completion(::CosTransactions::Status s) = 0;
};

} // End of namespace OBStubImpl_CosTransactions

//
// IDL:CosTransactions:1.0
//
namespace OBMarshalStubImpl_CosTransactions
{

//
// IDL:CosTransactions/Synchronization:1.0
//
class Synchronization : virtual public OBStubImpl_CosTransactions::Synchronization,
                        virtual public OBMarshalStubImpl_CosTransactions::TransactionalObject
{
    Synchronization(const Synchronization&);
    void operator=(const Synchronization&);

protected:

    Synchronization() { }
    friend class OBProxy_CosTransactions::Synchronization;

public:

    //
    // IDL:CosTransactions/Synchronization/before_completion:1.0
    //
    virtual void before_completion();

    //
    // IDL:CosTransactions/Synchronization/after_completion:1.0
    //
    virtual void after_completion(::CosTransactions::Status s);
};

} // End of namespace OBMarshalStubImpl_CosTransactions

//
// IDL:CosTransactions:1.0
//
namespace OBV_CosTransactions
{

} // End of namespace OBV_CosTransactions

//
// IDL:CosTransactions/Synchronization:1.0
//
namespace CORBA
{

inline void
release(::CosTransactions::Synchronization_ptr p)
{
    if(p)
        p -> _remove_ref();
}

inline Boolean
is_nil(::CosTransactions::Synchronization_ptr p)
{
    return p == 0;
}

inline void
release(OBStubImpl_CosTransactions::Synchronization_ptr p)
{
    if(p)
        p -> _OB_decRef();
}

inline Boolean
is_nil(OBStubImpl_CosTransactions::Synchronization_ptr p)
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
