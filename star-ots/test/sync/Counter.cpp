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

#include <STAR/CORBAClient.h>
#include <STAR/CORBAServer.h>
#include <counter.h>

STAR::TypeCodeConst _tc_Counter(
"01a97d000e0000002400000001f0fd7f1000000049444c3a436f756e7465723a312e300008000"
"000436f756e74657200"
);

//
// Interface declare ::Counter
//
const char* Counter::ids_[] =
{
    "IDL:Counter:1.0",
    0
};

void
STARDuplicate(Counter_ptr p)
{
    if(p)
        p -> _STAR_incRef();
}

void
STARRelease(Counter_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

Counter_ptr
Counter::_narrow(CORBA::Object_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        Counter_ptr v = dynamic_cast<Counter_ptr>(p);
        if(v)
            return _duplicate(v);

        if(p -> _is_a(ids_[0]))
        {
            Counter_broker* val = new Counter_broker;
            val -> _STAR_copyFrom(p);
            return val;
        }
    }

    return _nil();
}

Counter_ptr
Counter::_narrow(CORBA::AbstractBase_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        CORBA::Object_var obj = p -> _to_object();
        return _narrow(obj);
    }
    return _nil();
}

Counter_ptr
Counter::_unchecked_narrow(CORBA::Object_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        Counter_ptr v = dynamic_cast<Counter_ptr>(p);
        if(v)
            return _duplicate(v);

        Counter_broker* val = new Counter_broker;
        val -> _STAR_copyFrom(p);
        return val;
    }

    return _nil();
}

Counter_ptr
Counter::_unchecked_narrow(CORBA::AbstractBase_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        CORBA::Object_var obj = p -> _to_object();
        return _unchecked_narrow(obj);
    }
    return _nil();
}

const char**
Counter::_STAR_staticIds()
{
    return ids_;
}

void
STARMarshal(Counter_ptr _star_v, STAR::OutputStreamImpl* _star_out)
{
    _star_out -> write_Object(_star_v);
}

void
STARUnmarshal(Counter_ptr& _star_v, STAR::InputStreamImpl* _star_in)
{
    Counter_var old = _star_v;
    CORBA::Object_var p = _star_in -> read_Object();

    if(!CORBA::is_nil(p))
    {
        Counter_broker* _star_obj = new Counter_broker;
        _star_obj -> _STAR_copyFrom(p);
        _star_v = _star_obj;
    }
    else
        _star_v = Counter::_nil();
}

void
operator<<=(CORBA::Any& any, Counter_ptr* v)
{
    any.replace(_tc_Counter, (CORBA::Object_ptr)*v, true);
}

void
operator<<=(CORBA::Any& any, Counter_ptr v)
{
    Counter_ptr val = Counter::_duplicate(v);
    any <<= &val;
}

CORBA::Boolean
operator>>=(const CORBA::Any& any, Counter_ptr& v)
{
    if(any.check_type(_tc_Counter))
    {
        CORBA::Object_ptr val = (CORBA::Object_ptr)any.value();

        if(!CORBA::is_nil(val))
        {
            if(!(v = dynamic_cast<Counter_ptr>(val)))
            {
                Counter_broker* obj = new Counter_broker;
                obj -> _STAR_copyFrom(val);
                v = obj;
                (CORBA::Any&)any <<= &v;
            }
        }
        else
            v = Counter::_nil();

        return true;
    }
    else
        return false;
}

const char**
Counter_broker::_STAR_ids() const
{
    return ids_;
}

//
// Attribute declare Counter_broker::count
//
CORBA::Long
Counter_broker::count()
{
    bool _star_retry = true;
    while(true)
    {
        try
        {
            STAR::StubBase_var _star_stubBase = _STAR_getStub();
            STAR::MarshalStub_ptr _star_stub = dynamic_cast<STAR::MarshalStub_ptr>(_star_stubBase.in());
            if (_star_stub!=NULL)
            {
                for(;;)
                {
                    STAR::Downcall_var _star_down = _star_stub->_STAR_createDowncall("_get_count", true);
                    try
                    {
                        _star_stub->_STAR_prePrepareHeader(_star_down);
                        _star_stub->_STAR_postPrepareHeader(_star_down);
                        _star_stub->_STAR_request(_star_down);
                        STAR::InputStreamImpl* _star_in = _star_stub->_STAR_preRemoveHeader(_star_down);
                        CORBA::Long _star_r;
                        try
                        {
                            _star_r = _star_in -> read_long();
                        }
                        catch(const CORBA::SystemException& _star_ex)
                        {
                            _star_stub->_STAR_unmarshalEx(_star_down, _star_ex);
                        }
                        _star_stub->_STAR_postRemoveHeader(_star_down);
                        return _star_r;
                    }
                    catch(const STAR::FailureException& _star_ex)
                    {
                        _star_stub->_STAR_handleNetworkFailureException(_star_down, _star_ex);
                    }
                }
            }
        }
        catch(const STAR::ExceptionBase& _star_ex)
        {
            _STAR_handleException(_star_ex, _star_retry);
        }
    }
}

//
// Operation declare Counter_broker::increment
//
void
Counter_broker::increment()
{
    bool _star_retry = true;
    while(true)
    {
        try
        {
            STAR::StubBase_var _star_stubBase = _STAR_getStub();
            STAR::MarshalStub_ptr _star_stub = dynamic_cast<STAR::MarshalStub_ptr>(_star_stubBase.in());
            if (_star_stub!=NULL)
            {
                for(;;)
                {
                    STAR::Downcall_var _star_down = _star_stub->_STAR_createDowncall("increment", true);
                    try
                    {
                        _star_stub->_STAR_prePrepareHeader(_star_down);
                        _star_stub->_STAR_postPrepareHeader(_star_down);
                        _star_stub->_STAR_request(_star_down);
                        _star_stub->_STAR_preRemoveHeader(_star_down);
                        _star_stub->_STAR_postRemoveHeader(_star_down);
                        break;
                    }
                    catch(const STAR::FailureException& _star_ex)
                    {
                        _star_stub->_STAR_handleNetworkFailureException(_star_down, _star_ex);
                    }
                }
                #if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
                                    volatile int i = 1;
                                    i++;
                #endif
                                return;
            }
            else
            {
                    throw CORBA::INV_OBJREF();
            }
        }
        catch(const STAR::ExceptionBase& _star_ex)
        {
            _STAR_handleException(_star_ex, _star_retry);
        }
    }
}

//
// Operation declare Counter_broker::decrement
//
void
Counter_broker::decrement()
{
    bool _star_retry = true;
    while(true)
    {
        try
        {
            STAR::StubBase_var _star_stubBase = _STAR_getStub();
            STAR::MarshalStub_ptr _star_stub = dynamic_cast<STAR::MarshalStub_ptr>(_star_stubBase.in());
            if (_star_stub!=NULL)
            {
                for(;;)
                {
                    STAR::Downcall_var _star_down = _star_stub->_STAR_createDowncall("decrement", true);
                    try
                    {
                        _star_stub->_STAR_prePrepareHeader(_star_down);
                        _star_stub->_STAR_postPrepareHeader(_star_down);
                        _star_stub->_STAR_request(_star_down);
                        _star_stub->_STAR_preRemoveHeader(_star_down);
                        _star_stub->_STAR_postRemoveHeader(_star_down);
                        break;
                    }
                    catch(const STAR::FailureException& _star_ex)
                    {
                        _star_stub->_STAR_handleNetworkFailureException(_star_down, _star_ex);
                    }
                }
                #if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
                                    volatile int i = 1;
                                    i++;
                #endif
                                return;
            }
            else
            {
                    throw CORBA::INV_OBJREF();
            }
        }
        catch(const STAR::ExceptionBase& _star_ex)
        {
            _STAR_handleException(_star_ex, _star_retry);
        }
    }
}
