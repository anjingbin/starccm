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
#include <Account.h>

#ifndef OB_INTEGER_VERSION
#   error No ORBacus version defined! Is <OB/CORBA.h> included?
#endif

#ifndef OB_NO_VERSION_CHECK
#   if (OB_INTEGER_VERSION != 4010000L)
#       error ORBacus version mismatch!
#   endif
#endif

::OB::TypeCodeConst _tc_Account(
"01a87c000e00000030000000010000001900000049444c3a696d706c696369742f4163636f756"
"e743a312e3000000280080000004163636f756e7400"
);

//
// IDL:implicit/Account:1.0
//
const char* Account::ids_[] =
{
    "IDL:implicit/Account:1.0",
    0
};

void
OBDuplicate(Account_ptr p)
{
    if(p)
        p -> _add_ref();
}

void
OBRelease(Account_ptr p)
{
    if(p)
        p -> _remove_ref();
}

Account_ptr
Account::_narrow(CORBA::Object_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        Account_ptr v = dynamic_cast<Account_ptr>(p);
        if(v)
            return _duplicate(v);

        if(p -> _is_a(ids_[0]))
        {
            OBProxy_Account* val = new OBProxy_Account;
            val -> _OB_copyFrom(p);
            return val;
        }
    }

    return _nil();
}

Account_ptr
Account::_narrow(CORBA::AbstractBase_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        Account_ptr v = dynamic_cast<Account_ptr>(p);
        if(v)
            return _duplicate(v);

        CORBA::Object_var obj = p -> _to_object();
        return _narrow(obj);
    }
    return _nil();
}

Account_ptr
Account::_unchecked_narrow(CORBA::Object_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        Account_ptr v = dynamic_cast<Account_ptr>(p);
        if(v)
            return _duplicate(v);

        OBProxy_Account* val = new OBProxy_Account;
        val -> _OB_copyFrom(p);
        return val;
    }

    return _nil();
}

Account_ptr
Account::_unchecked_narrow(CORBA::AbstractBase_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        CORBA::Object_var obj = p -> _to_object();
        return _unchecked_narrow(obj);
    }
    return _nil();
}

const char**
Account::_OB_staticIds()
{
    return ids_;
}

void
OBMarshal(Account_ptr _ob_v, OB::OutputStreamImpl* _ob_out)
{
    _ob_out -> write_Object(_ob_v);
}

void
OBUnmarshal(Account_ptr& _ob_v, OB::InputStreamImpl* _ob_in)
{
    Account_var old = _ob_v;
    CORBA::Object_var p = _ob_in -> read_Object();

    if(!CORBA::is_nil(p))
    {
        OBProxy_Account* _ob_obj = new OBProxy_Account;
        _ob_obj -> _OB_copyFrom(p);
        _ob_v = _ob_obj;
    }
    else
        _ob_v = Account::_nil();
}

void
operator<<=(CORBA::Any& any, Account_ptr* v)
{
    any.replace(_tc_Account, (CORBA::Object_ptr)*v, true);
}

void
operator<<=(CORBA::Any& any, Account_ptr v)
{
    Account_ptr val = Account::_duplicate(v);
    any <<= &val;
}

CORBA::Boolean
operator>>=(const CORBA::Any& any, Account_ptr& v)
{
    if(any.check_type(_tc_Account))
    {
        CORBA::Object_ptr val = (CORBA::Object_ptr)any.value();

        if(!CORBA::is_nil(val))
        {
            if(!(v = dynamic_cast<Account_ptr>(val)))
            {
                OBProxy_Account* obj = new OBProxy_Account;
                obj -> _OB_copyFrom(val);
                v = obj;
                (CORBA::Any&)any <<= &v;
            }
        }
        else
            v = Account::_nil();

        return true;
    }
    else
        return false;
}

//
// IDL:implicit/Account:1.0
//
OB::MarshalStubImpl_ptr
OBProxy_Account::_OB_createMarshalStubImpl()
{
    return new OBMarshalStubImpl_Account;
}

const char**
OBProxy_Account::_OB_ids() const
{
    return ::Account::ids_;
}

//
// IDL:implicit/Account/connect:1.0
//
void
OBProxy_Account::connect()
{
    CORBA::ULong _ob_retry = 0, _ob_hop = 0;
    while(true)
    {
        try
        {
            OB::StubImplBase_var _ob_stubImplBase = _OB_getStubImpl();
            OBStubImpl_Account_ptr _ob_stubImpl = dynamic_cast< OBStubImpl_Account_ptr>(_ob_stubImplBase.in());
            _ob_stubImpl -> connect();
#if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
            volatile int i = 1;
            i++;
#endif
            return;
        }
        catch(const OB::ExceptionBase& _ob_ex)
        {
            _OB_handleException(_ob_ex, _ob_retry, _ob_hop);
        }
    }
}

//
// IDL:implicit/Account/close:1.0
//
void
OBProxy_Account::close()
{
    CORBA::ULong _ob_retry = 0, _ob_hop = 0;
    while(true)
    {
        try
        {
            OB::StubImplBase_var _ob_stubImplBase = _OB_getStubImpl();
            OBStubImpl_Account_ptr _ob_stubImpl = dynamic_cast< OBStubImpl_Account_ptr>(_ob_stubImplBase.in());
            _ob_stubImpl -> close();
#if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
            volatile int i = 1;
            i++;
#endif
            return;
        }
        catch(const OB::ExceptionBase& _ob_ex)
        {
            _OB_handleException(_ob_ex, _ob_retry, _ob_hop);
        }
    }
}

//
// IDL:implicit/Account/sum:1.0
//
void
OBProxy_Account::sum()
{
    CORBA::ULong _ob_retry = 0, _ob_hop = 0;
    while(true)
    {
        try
        {
            OB::StubImplBase_var _ob_stubImplBase = _OB_getStubImpl();
            OBStubImpl_Account_ptr _ob_stubImpl = dynamic_cast< OBStubImpl_Account_ptr>(_ob_stubImplBase.in());
            _ob_stubImpl -> sum();
#if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
            volatile int i = 1;
            i++;
#endif
            return;
        }
        catch(const OB::ExceptionBase& _ob_ex)
        {
            _OB_handleException(_ob_ex, _ob_retry, _ob_hop);
        }
    }
}

//
// IDL:implicit/Account/deposit:1.0
//
void
OBProxy_Account::deposit()
{
    CORBA::ULong _ob_retry = 0, _ob_hop = 0;
    while(true)
    {
        try
        {
            OB::StubImplBase_var _ob_stubImplBase = _OB_getStubImpl();
            OBStubImpl_Account_ptr _ob_stubImpl = dynamic_cast< OBStubImpl_Account_ptr>(_ob_stubImplBase.in());
            _ob_stubImpl -> deposit();
#if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
            volatile int i = 1;
            i++;
#endif
            return;
        }
        catch(const OB::ExceptionBase& _ob_ex)
        {
            _OB_handleException(_ob_ex, _ob_retry, _ob_hop);
        }
    }
}

//
// IDL:implicit/Account/withdraw:1.0
//
void
OBProxy_Account::withdraw()
{
    CORBA::ULong _ob_retry = 0, _ob_hop = 0;
    while(true)
    {
        try
        {
            OB::StubImplBase_var _ob_stubImplBase = _OB_getStubImpl();
            OBStubImpl_Account_ptr _ob_stubImpl = dynamic_cast< OBStubImpl_Account_ptr>(_ob_stubImplBase.in());
            _ob_stubImpl -> withdraw();
#if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
            volatile int i = 1;
            i++;
#endif
            return;
        }
        catch(const OB::ExceptionBase& _ob_ex)
        {
            _OB_handleException(_ob_ex, _ob_retry, _ob_hop);
        }
    }
}

//
// IDL:implicit/Account:1.0
//
void
OBDuplicate(OBStubImpl_Account_ptr p)
{
    if(p)
        p -> _OB_incRef();
}

void
OBRelease(OBStubImpl_Account_ptr p)
{
    if(p)
        p -> _OB_decRef();
}

//
// IDL:implicit/Account/connect:1.0
//
void
OBMarshalStubImpl_Account::connect()
{
    while(true)
    {
        OB::Downcall_var _ob_down = _OB_createDowncall("connect", true);
        try
        {
            _OB_preMarshal(_ob_down);
            _OB_postMarshal(_ob_down);
            _OB_request(_ob_down);
            _OB_preUnmarshal(_ob_down);
            _OB_postUnmarshal(_ob_down);
            return;
        }
        catch(const OB::FailureException& _ob_ex)
        {
            _OB_handleFailureException(_ob_down, _ob_ex);
        }
    }
}

//
// IDL:implicit/Account/close:1.0
//
void
OBMarshalStubImpl_Account::close()
{
    while(true)
    {
        OB::Downcall_var _ob_down = _OB_createDowncall("close", true);
        try
        {
            _OB_preMarshal(_ob_down);
            _OB_postMarshal(_ob_down);
            _OB_request(_ob_down);
            _OB_preUnmarshal(_ob_down);
            _OB_postUnmarshal(_ob_down);
            return;
        }
        catch(const OB::FailureException& _ob_ex)
        {
            _OB_handleFailureException(_ob_down, _ob_ex);
        }
    }
}

//
// IDL:implicit/Account/sum:1.0
//
void
OBMarshalStubImpl_Account::sum()
{
    while(true)
    {
        OB::Downcall_var _ob_down = _OB_createDowncall("sum", true);
        try
        {
            _OB_preMarshal(_ob_down);
            _OB_postMarshal(_ob_down);
            _OB_request(_ob_down);
            _OB_preUnmarshal(_ob_down);
            _OB_postUnmarshal(_ob_down);
            return;
        }
        catch(const OB::FailureException& _ob_ex)
        {
            _OB_handleFailureException(_ob_down, _ob_ex);
        }
    }
}

//
// IDL:implicit/Account/deposit:1.0
//
void
OBMarshalStubImpl_Account::deposit()
{
    while(true)
    {
        OB::Downcall_var _ob_down = _OB_createDowncall("deposit", true);
        try
        {
            _OB_preMarshal(_ob_down);
            _OB_postMarshal(_ob_down);
            _OB_request(_ob_down);
            _OB_preUnmarshal(_ob_down);
            _OB_postUnmarshal(_ob_down);
            return;
        }
        catch(const OB::FailureException& _ob_ex)
        {
            _OB_handleFailureException(_ob_down, _ob_ex);
        }
    }
}

//
// IDL:implicit/Account/withdraw:1.0
//
void
OBMarshalStubImpl_Account::withdraw()
{
    while(true)
    {
        OB::Downcall_var _ob_down = _OB_createDowncall("withdraw", true);
        try
        {
            _OB_preMarshal(_ob_down);
            _OB_postMarshal(_ob_down);
            _OB_request(_ob_down);
            _OB_preUnmarshal(_ob_down);
            _OB_postUnmarshal(_ob_down);
            return;
        }
        catch(const OB::FailureException& _ob_ex)
        {
            _OB_handleFailureException(_ob_down, _ob_ex);
        }
    }
}
