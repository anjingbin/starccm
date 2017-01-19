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

#include <STAR/CORBAServer.h>
#include <counter_skel.h>

//
// ProtectedAttribute declare count
//
void
POA_Counter::_STAR_get_count(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_r ;

    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Counter * objImpl=dynamic_cast< POA_Counter*>( serv );
    if(objImpl!=NULL) 
    {
        _star_r = objImpl ->count();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare increment
//
void
POA_Counter::_STAR_op_increment(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Counter * objImpl=dynamic_cast< POA_Counter*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->increment();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare decrement
//
void
POA_Counter::_STAR_op_decrement(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Counter * objImpl=dynamic_cast< POA_Counter*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->decrement();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// Interface declare ::Counter
//
CORBA::Boolean
POA_Counter::_is_a(const char* type)
    throw(CORBA::SystemException)
{
    const char** _star_ids = Counter::_STAR_staticIds();
    for(CORBA::ULong _star_i = 0 ; _star_ids[_star_i] != 0 ; _star_i++)
        if(strcmp(type, _star_ids[_star_i]) == 0)
            return true;

    return false;
}

CORBA::RepositoryId
POA_Counter::_primary_interface(const PortableServer::ObjectId&,
                                PortableServer::POA_ptr)
{
    return CORBA::string_dup(Counter::_STAR_staticIds()[0]);
}

Counter_ptr
POA_Counter::_this()
{
    CORBA::Object_var obj = _STAR_buildReference();
    Counter_var result = Counter::_narrow(obj);
    return result._retn();
}

void
POA_Counter::_STAR_dispatch(STAR::Upcall_ptr _star_up)
{
    static const PortableServer::OperationTableEntry Counter_Optable_fixed[] =
    {
        {"_component",PortableServer::ServantBase::_STAR_op_get_component},
        {"_interface",PortableServer::ServantBase::_STAR_op_get_interface},
        {"_is_a",PortableServer::ServantBase::_STAR_op_is_a},
        {"_non_existent",PortableServer::ServantBase::_STAR_op_non_existent}
    };
    static const PortableServer::OperationTableEntry Counter_Optable_unfixed[] =
    {
        {"_get_count",POA_Counter::_STAR_get_count},
        {"decrement",POA_Counter::_STAR_op_decrement},
        {"increment",POA_Counter::_STAR_op_increment}
    };
    static const CORBA::ULong _star_methodCount = 3;

    PortableServer::operationSkel operation;
    if(_STAR_locateOperation(_star_up,Counter_Optable_unfixed,_star_methodCount, operation))
    {
        (*operation)(this,_star_up);
        return;
    }
    else
    if(_STAR_locateOperation(_star_up,Counter_Optable_fixed,4, operation))
    {
        (*operation)(this,_star_up);
        return;
    }
    throw CORBA::BAD_OPERATION(0, CORBA::COMPLETED_NO);

}
