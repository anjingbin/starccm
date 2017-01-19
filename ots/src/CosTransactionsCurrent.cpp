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

#include <OB/CORBAClient.h>
#include <CosTransactionsCurrent.h>

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

} // End of namespace CosTransactions

//
// IDL:CosTransactions/Current:1.0
//
void
OBDuplicate(CosTransactions::Current_ptr p)
{
    if(p)
        p -> _add_ref();
}

void
OBRelease(CosTransactions::Current_ptr p)
{
    if(p)
        p -> _remove_ref();
}

CosTransactions::Current_ptr
CosTransactions::Current::_narrow(CORBA::Object_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        Current_ptr v = dynamic_cast<Current_ptr>(p);
        if(v)
            return _duplicate(v);
    }

    return _nil();
}

CosTransactions::Current_ptr
CosTransactions::Current::_narrow(CORBA::AbstractBase_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        Current_ptr v = dynamic_cast<Current_ptr>(p);
        if(v)
            return _duplicate(v);

        CORBA::Object_var obj = p -> _to_object();
        return _narrow(obj);
    }
    return _nil();
}

void
OBMarshal(CosTransactions::Current_ptr, OB::OutputStreamImpl*)
{
    throw CORBA::MARSHAL();
}

void
OBUnmarshal(CosTransactions::Current_ptr&, OB::InputStreamImpl*)
{
    throw CORBA::MARSHAL();
}