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
#include <CosTransactionsUser.h>

namespace CosTransactions
{

STAR::TypeCodeConst _tc_Synchronization(
"0134f5770e00000044000000010000002800000049444c3a436f735472616e73616374696f6e7"
"32f53796e6368726f6e697a6174696f6e3a312e30001000000053796e6368726f6e697a617469"
"6f6e00"
);

} // End of namespace CosTransactions

//
// Interface declare ::CosTransactions::Synchronization
//
const char* CosTransactions::Synchronization::ids_[] =
{
    "IDL:CosTransactions/Synchronization:1.0",
    "IDL:CosTransactions/TransactionalObject:1.0",
    0
};

void
STARDuplicate(CosTransactions::Synchronization_ptr p)
{
    if(p)
        p -> _STAR_incRef();
}

void
STARRelease(CosTransactions::Synchronization_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

CosTransactions::Synchronization_ptr
CosTransactions::Synchronization::_narrow(CORBA::Object_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        Synchronization_ptr v = dynamic_cast<Synchronization_ptr>(p);
        if(v)
            return _duplicate(v);

        if(p -> _is_a(ids_[0]))
        {
            CosTransactions::Synchronization_broker* val = new CosTransactions::Synchronization_broker;
            val -> _STAR_copyFrom(p);
            return val;
        }
    }

    return _nil();
}

CosTransactions::Synchronization_ptr
CosTransactions::Synchronization::_narrow(CORBA::AbstractBase_ptr p)
{
    if(!CORBA::is_nil(p))
    {
        CORBA::Object_var obj = p -> _to_object();
        return _narrow(obj);
    }
    return _nil();
}

const char**
CosTransactions::Synchronization::_STAR_staticIds()
{
    return ids_;
}

void
STARMarshal(CosTransactions::Synchronization_ptr _star_v, STAR::OutputStreamImpl* _star_out)
{
    _star_out -> write_Object(_star_v);
}

void
STARUnmarshal(CosTransactions::Synchronization_ptr& _star_v, STAR::InputStreamImpl* _star_in)
{
    CosTransactions::Synchronization_var old = _star_v;
    CORBA::Object_var p = _star_in -> read_Object();

    if(!CORBA::is_nil(p))
    {
        CosTransactions::Synchronization_broker* _star_obj = new CosTransactions::Synchronization_broker;
        _star_obj -> _STAR_copyFrom(p);
        _star_v = _star_obj;
    }
    else
        _star_v = CosTransactions::Synchronization::_nil();
}

void
operator<<=(CORBA::Any& any, CosTransactions::Synchronization_ptr* v)
{
    any.replace(CosTransactions::_tc_Synchronization, (CORBA::Object_ptr)*v, true);
}

void
operator<<=(CORBA::Any& any, CosTransactions::Synchronization_ptr v)
{
    CosTransactions::Synchronization_ptr val = CosTransactions::Synchronization::_duplicate(v);
    any <<= &val;
}

CORBA::Boolean
operator>>=(const CORBA::Any& any, CosTransactions::Synchronization_ptr& v)
{
    if(any.check_type(CosTransactions::_tc_Synchronization))
    {
        CORBA::Object_ptr val = (CORBA::Object_ptr)any.value();

        if(!CORBA::is_nil(val))
        {
            if(!(v = dynamic_cast<CosTransactions::Synchronization_ptr>(val)))
            {
                CosTransactions::Synchronization_broker* obj = new CosTransactions::Synchronization_broker;
                obj -> _STAR_copyFrom(val);
                v = obj;
                (CORBA::Any&)any <<= &v;
            }
        }
        else
            v = CosTransactions::Synchronization::_nil();

        return true;
    }
    else
        return false;
}

const char**
CosTransactions::Synchronization_broker::_STAR_ids() const
{
    return ids_;
}

//
// Operation declare CosTransactions::Synchronization_broker::before_completion
//
void
CosTransactions::Synchronization_broker::before_completion()
{
    bool _star_retry = true;
    while(true)
    {
        STAR::StubBase_var _star_stubBase = _STAR_getStub();
        try
        {
            STAR::MarshalStub_ptr _star_stub = dynamic_cast<STAR::MarshalStub_ptr>(_star_stubBase.in());
            if (_star_stub!=NULL)
            {
                for(;;)
                {
                    STAR::Downcall_var _star_down = _star_stub->_STAR_createDowncall("before_completion", true);
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
// Operation declare CosTransactions::Synchronization_broker::after_completion
//
void
CosTransactions::Synchronization_broker::after_completion(Status _star_a0)
{
    bool _star_retry = true;
    while(true)
    {
        STAR::StubBase_var _star_stubBase = _STAR_getStub();
        try
        {
            STAR::MarshalStub_ptr _star_stub = dynamic_cast<STAR::MarshalStub_ptr>(_star_stubBase.in());
            if (_star_stub!=NULL)
            {
                for(;;)
                {
                    STAR::Downcall_var _star_down = _star_stub->_STAR_createDowncall("after_completion", true);
                    try
                    {
                        STAR::OutputStreamImpl* _star_out = _star_stub->_STAR_prePrepareHeader(_star_down);
                        try
                        {
                            _star_out -> write_ulong((CORBA::ULong)_star_a0);
                        }
                        catch(const CORBA::SystemException& _star_ex)
                        {
                            _star_stub->_STAR_marshalEx(_star_down, _star_ex);
                        }
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
