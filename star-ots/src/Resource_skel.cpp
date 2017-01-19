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
#include <Resource_skel.h>

//
// ProtectedOperation declare create
//
void
POA_ResourceFactory::_STAR_op_create(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Short _star_a0;
    Resource_var _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_short();
    _STAR_postRemoveHeader(_star_up);
    POA_ResourceFactory * objImpl=dynamic_cast< POA_ResourceFactory*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->create(_star_a0);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    STARMarshal(_star_r.in(), _star_out);
    _STAR_postPrepareHeader(_star_up);
}

//
// Interface declare ::ResourceFactory
//
CORBA::Boolean
POA_ResourceFactory::_is_a(const char* type)
    throw(CORBA::SystemException)
{
    const char** _star_ids = ResourceFactory::_STAR_staticIds();
    for(CORBA::ULong _star_i = 0 ; _star_ids[_star_i] != 0 ; _star_i++)
        if(strcmp(type, _star_ids[_star_i]) == 0)
            return true;

    return false;
}

CORBA::RepositoryId
POA_ResourceFactory::_primary_interface(const PortableServer::ObjectId&,
                                        PortableServer::POA_ptr)
{
    return CORBA::string_dup(ResourceFactory::_STAR_staticIds()[0]);
}

ResourceFactory_ptr
POA_ResourceFactory::_this()
{
    CORBA::Object_var obj = _STAR_buildReference();
    ResourceFactory_var result = ResourceFactory::_narrow(obj);
    return result._retn();
}

void
POA_ResourceFactory::_STAR_dispatch(STAR::Upcall_ptr _star_up)
{
    static const PortableServer::OperationTableEntry ResourceFactory_Optable_fixed[] =
    {
        {"_component",PortableServer::ServantBase::_STAR_op_get_component},
        {"_interface",PortableServer::ServantBase::_STAR_op_get_interface},
        {"_is_a",PortableServer::ServantBase::_STAR_op_is_a},
        {"_non_existent",PortableServer::ServantBase::_STAR_op_non_existent}
    };
    static const PortableServer::OperationTableEntry ResourceFactory_Optable_unfixed[] =
    {
        {"create",POA_ResourceFactory::_STAR_op_create}
    };
    static const CORBA::ULong _star_methodCount = 1;

    PortableServer::operationSkel operation;
    if(_STAR_locateOperation(_star_up,ResourceFactory_Optable_unfixed,_star_methodCount, operation))
    {
        (*operation)(this,_star_up);
        return;
    }
    else
    if(_STAR_locateOperation(_star_up,ResourceFactory_Optable_fixed,4, operation))
    {
        (*operation)(this,_star_up);
        return;
    }
    throw CORBA::BAD_OPERATION(0, CORBA::COMPLETED_NO);

}

//
// ProtectedOperation declare prepare
//
void
POA_Resource::_STAR_op_prepare(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CosTransactions::Vote _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->prepare();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_ulong((CORBA::ULong)_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare rollback
//
void
POA_Resource::_STAR_op_rollback(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->rollback();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare commit
//
void
POA_Resource::_STAR_op_commit(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->commit();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare commit_one_phase
//
void
POA_Resource::_STAR_op_commit_one_phase(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->commit_one_phase();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare forget
//
void
POA_Resource::_STAR_op_forget(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->forget();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare release
//
void
POA_Resource::_STAR_op_release(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->release();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare BeginTrans
//
void
POA_Resource::_STAR_op_BeginTrans(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Boolean _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->BeginTrans();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_boolean(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare set_recoverycoordinator
//
void
POA_Resource::_STAR_op_set_recoverycoordinator(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CosTransactions::RecoveryCoordinator_var _star_a0;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    STARUnmarshal(_star_a0.inout(), _star_in);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->set_recoverycoordinator(_star_a0);
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare heuristic_decision
//
void
POA_Resource::_STAR_op_heuristic_decision(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_Resource * objImpl=dynamic_cast< POA_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->heuristic_decision();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// Interface declare ::Resource
//
CORBA::Boolean
POA_Resource::_is_a(const char* type)
    throw(CORBA::SystemException)
{
    const char** _star_ids = Resource::_STAR_staticIds();
    for(CORBA::ULong _star_i = 0 ; _star_ids[_star_i] != 0 ; _star_i++)
        if(strcmp(type, _star_ids[_star_i]) == 0)
            return true;

    return false;
}

CORBA::RepositoryId
POA_Resource::_primary_interface(const PortableServer::ObjectId&,
                                 PortableServer::POA_ptr)
{
    return CORBA::string_dup(Resource::_STAR_staticIds()[0]);
}

Resource_ptr
POA_Resource::_this()
{
    CORBA::Object_var obj = _STAR_buildReference();
    Resource_var result = Resource::_narrow(obj);
    return result._retn();
}

void
POA_Resource::_STAR_dispatch(STAR::Upcall_ptr _star_up)
{
    static const PortableServer::OperationTableEntry Resource_Optable_fixed[] =
    {
        {"_component",PortableServer::ServantBase::_STAR_op_get_component},
        {"_interface",PortableServer::ServantBase::_STAR_op_get_interface},
        {"_is_a",PortableServer::ServantBase::_STAR_op_is_a},
        {"_non_existent",PortableServer::ServantBase::_STAR_op_non_existent}
    };
    static const PortableServer::OperationTableEntry Resource_Optable_unfixed[] =
    {
        {"BeginTrans",POA_Resource::_STAR_op_BeginTrans},
        {"commit",POA_Resource::_STAR_op_commit},
        {"commit_one_phase",POA_Resource::_STAR_op_commit_one_phase},
        {"forget",POA_Resource::_STAR_op_forget},
        {"heuristic_decision",POA_Resource::_STAR_op_heuristic_decision},
        {"pg_BeginTrans",POA_Resource::_STAR_op_pg_BeginTrans},
        {"pg_PQclear",POA_Resource::_STAR_op_pg_PQclear},
        {"pg_PQcmdStatus",POA_Resource::_STAR_op_pg_PQcmdStatus},
        {"pg_PQdb",POA_Resource::_STAR_op_pg_PQdb},
        {"pg_PQendcopy",POA_Resource::_STAR_op_pg_PQendcopy},
        {"pg_PQerrorMessage",POA_Resource::_STAR_op_pg_PQerrorMessage},
        {"pg_PQexec",POA_Resource::_STAR_op_pg_PQexec},
        {"pg_PQfinish",POA_Resource::_STAR_op_pg_PQfinish},
        {"pg_PQfname",POA_Resource::_STAR_op_pg_PQfname},
        {"pg_PQfnumber",POA_Resource::_STAR_op_pg_PQfnumber},
        {"pg_PQfsize",POA_Resource::_STAR_op_pg_PQfsize},
        {"pg_PQftype",POA_Resource::_STAR_op_pg_PQftype},
        {"pg_PQgetlength",POA_Resource::_STAR_op_pg_PQgetlength},
        {"pg_PQgetline",POA_Resource::_STAR_op_pg_PQgetline},
        {"pg_PQgetvalue",POA_Resource::_STAR_op_pg_PQgetvalue},
        {"pg_PQhost",POA_Resource::_STAR_op_pg_PQhost},
        {"pg_PQnfields",POA_Resource::_STAR_op_pg_PQnfields},
        {"pg_PQnotifies",POA_Resource::_STAR_op_pg_PQnotifies},
        {"pg_PQntuples",POA_Resource::_STAR_op_pg_PQntuples},
        {"pg_PQoidStatus",POA_Resource::_STAR_op_pg_PQoidStatus},
        {"pg_PQoptions",POA_Resource::_STAR_op_pg_PQoptions},
        {"pg_PQport",POA_Resource::_STAR_op_pg_PQport},
        {"pg_PQputline",POA_Resource::_STAR_op_pg_PQputline},
        {"pg_PQreset",POA_Resource::_STAR_op_pg_PQreset},
        {"pg_PQresultStatus",POA_Resource::_STAR_op_pg_PQresultStatus},
        {"pg_PQsetdbLogin",POA_Resource::_STAR_op_pg_PQsetdbLogin},
        {"pg_PQstatus",POA_Resource::_STAR_op_pg_PQstatus},
        {"pg_PQtty",POA_Resource::_STAR_op_pg_PQtty},
        {"pg_PQuntrace",POA_Resource::_STAR_op_pg_PQuntrace},
        {"pg_commit",POA_Resource::_STAR_op_pg_commit},
        {"pg_commit_one_phase",POA_Resource::_STAR_op_pg_commit_one_phase},
        {"pg_fe_getauthname",POA_Resource::_STAR_op_pg_fe_getauthname},
        {"pg_fe_setauthsvc",POA_Resource::_STAR_op_pg_fe_setauthsvc},
        {"pg_forget",POA_Resource::_STAR_op_pg_forget},
        {"pg_heuristic_decision",POA_Resource::_STAR_op_pg_heuristic_decision},
        {"pg_prepare",POA_Resource::_STAR_op_pg_prepare},
        {"pg_release",POA_Resource::_STAR_op_pg_release},
        {"pg_rollback",POA_Resource::_STAR_op_pg_rollback},
        {"pg_set_recoverycoordinator",POA_Resource::_STAR_op_pg_set_recoverycoordinator},
        {"prepare",POA_Resource::_STAR_op_prepare},
        {"release",POA_Resource::_STAR_op_release},
        {"rollback",POA_Resource::_STAR_op_rollback},
        {"set_recoverycoordinator",POA_Resource::_STAR_op_set_recoverycoordinator}
    };
    static const CORBA::ULong _star_methodCount = 48;

    PortableServer::operationSkel operation;
    if(_STAR_locateOperation(_star_up,Resource_Optable_unfixed,_star_methodCount, operation))
    {
        (*operation)(this,_star_up);
        return;
    }
    else
    if(_STAR_locateOperation(_star_up,Resource_Optable_fixed,4, operation))
    {
        (*operation)(this,_star_up);
        return;
    }
    throw CORBA::BAD_OPERATION(0, CORBA::COMPLETED_NO);

}
