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
#include <PostgreSQLResource_skel.h>

//
// ProtectedOperation declare pg_prepare
//
void
POA_PG_Resource::_STAR_op_pg_prepare(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CosTransactions::Vote _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_prepare();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_ulong((CORBA::ULong)_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_rollback
//
void
POA_PG_Resource::_STAR_op_pg_rollback(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_rollback();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_commit
//
void
POA_PG_Resource::_STAR_op_pg_commit(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_commit();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_commit_one_phase
//
void
POA_PG_Resource::_STAR_op_pg_commit_one_phase(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_commit_one_phase();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_forget
//
void
POA_PG_Resource::_STAR_op_pg_forget(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_forget();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_release
//
void
POA_PG_Resource::_STAR_op_pg_release(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_release();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_BeginTrans
//
void
POA_PG_Resource::_STAR_op_pg_BeginTrans(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Boolean _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_BeginTrans();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_boolean(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_set_recoverycoordinator
//
void
POA_PG_Resource::_STAR_op_pg_set_recoverycoordinator(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CosTransactions::RecoveryCoordinator_var _star_a0;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    STARUnmarshal(_star_a0.inout(), _star_in);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_set_recoverycoordinator(_star_a0);
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_heuristic_decision
//
void
POA_PG_Resource::_STAR_op_pg_heuristic_decision(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_heuristic_decision();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQsetdbLogin
//
void
POA_PG_Resource::_STAR_op_pg_PQsetdbLogin(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    STAR::StrForStruct _star_a0;
    STAR::StrForStruct _star_a1;
    STAR::StrForStruct _star_a2;
    STAR::StrForStruct _star_a3;
    STAR::StrForStruct _star_a4;
    STAR::StrForStruct _star_a5;
    STAR::StrForStruct _star_a6;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_string();
    _star_a1 = _star_in -> read_string();
    _star_a2 = _star_in -> read_string();
    _star_a3 = _star_in -> read_string();
    _star_a4 = _star_in -> read_string();
    _star_a5 = _star_in -> read_string();
    _star_a6 = _star_in -> read_string();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_PQsetdbLogin(_star_a0, _star_a1, _star_a2, _star_a3, _star_a4, _star_a5, _star_a6);
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQdb
//
void
POA_PG_Resource::_STAR_op_pg_PQdb(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::String_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQdb();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQhost
//
void
POA_PG_Resource::_STAR_op_pg_PQhost(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::String_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQhost();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQoptions
//
void
POA_PG_Resource::_STAR_op_pg_PQoptions(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::String_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQoptions();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQport
//
void
POA_PG_Resource::_STAR_op_pg_PQport(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::String_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQport();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQtty
//
void
POA_PG_Resource::_STAR_op_pg_PQtty(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::String_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQtty();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQstatus
//
void
POA_PG_Resource::_STAR_op_pg_PQstatus(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    pg_ConnStatusType _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQstatus();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_short(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQerrorMessage
//
void
POA_PG_Resource::_STAR_op_pg_PQerrorMessage(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::String_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQerrorMessage();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQfinish
//
void
POA_PG_Resource::_STAR_op_pg_PQfinish(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_PQfinish();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQreset
//
void
POA_PG_Resource::_STAR_op_pg_PQreset(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_PQreset();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQuntrace
//
void
POA_PG_Resource::_STAR_op_pg_PQuntrace(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_PQuntrace();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQexec
//
void
POA_PG_Resource::_STAR_op_pg_PQexec(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    STAR::StrForStruct _star_a0;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_string();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_PQexec(_star_a0);
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQresultStatus
//
void
POA_PG_Resource::_STAR_op_pg_PQresultStatus(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    pg_ExecStatusType _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQresultStatus();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_short(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQntuples
//
void
POA_PG_Resource::_STAR_op_pg_PQntuples(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQntuples();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQnfields
//
void
POA_PG_Resource::_STAR_op_pg_PQnfields(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQnfields();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQfname
//
void
POA_PG_Resource::_STAR_op_pg_PQfname(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_a0;
    CORBA::String_var _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_long();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQfname(_star_a0);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQfnumber
//
void
POA_PG_Resource::_STAR_op_pg_PQfnumber(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    STAR::StrForStruct _star_a0;
    CORBA::Long _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_string();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQfnumber(_star_a0);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQftype
//
void
POA_PG_Resource::_STAR_op_pg_PQftype(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_a0;
    CORBA::Long _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_long();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQftype(_star_a0);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQfsize
//
void
POA_PG_Resource::_STAR_op_pg_PQfsize(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_a0;
    CORBA::Long _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_long();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQfsize(_star_a0);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQgetvalue
//
void
POA_PG_Resource::_STAR_op_pg_PQgetvalue(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_a0;
    CORBA::Long _star_a1;
    CORBA::String_var _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_long();
    _star_a1 = _star_in -> read_long();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQgetvalue(_star_a0, _star_a1);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQgetlength
//
void
POA_PG_Resource::_STAR_op_pg_PQgetlength(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_a0;
    CORBA::Long _star_a1;
    CORBA::Long _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_long();
    _star_a1 = _star_in -> read_long();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQgetlength(_star_a0, _star_a1);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQcmdStatus
//
void
POA_PG_Resource::_STAR_op_pg_PQcmdStatus(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::String_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQcmdStatus();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQoidStatus
//
void
POA_PG_Resource::_STAR_op_pg_PQoidStatus(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::String_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQoidStatus();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQclear
//
void
POA_PG_Resource::_STAR_op_pg_PQclear(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_PQclear();
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQnotifies
//
void
POA_PG_Resource::_STAR_op_pg_PQnotifies(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    pg_PGnotify_var _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQnotifies();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_r.in()._STAR_marshal(_star_out);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQgetline
//
void
POA_PG_Resource::_STAR_op_pg_PQgetline(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    STAR::StrForStruct _star_a0;
    CORBA::Long _star_a1;
    CORBA::Long _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_string();
    _star_a1 = _star_in -> read_long();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQgetline(_star_a0, _star_a1);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQputline
//
void
POA_PG_Resource::_STAR_op_pg_PQputline(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    STAR::StrForStruct _star_a0;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_string();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_PQputline(_star_a0);
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_PQendcopy
//
void
POA_PG_Resource::_STAR_op_pg_PQendcopy(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    CORBA::Long _star_r;
    _STAR_preRemoveHeader(_star_up);
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_PQendcopy();
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_long(_star_r);
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_fe_getauthname
//
void
POA_PG_Resource::_STAR_op_pg_fe_getauthname(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    STAR::StrForStruct _star_a0;
    CORBA::String_var _star_r;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_string();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         _star_r =  objImpl ->pg_fe_getauthname(_star_a0);
    }
    STAR::OutputStreamImpl* _star_out = _STAR_prePrepareHeader(_star_up);
    _star_out -> write_string(_star_r.in());
    _STAR_postPrepareHeader(_star_up);
}

//
// ProtectedOperation declare pg_fe_setauthsvc
//
void
POA_PG_Resource::_STAR_op_pg_fe_setauthsvc(PortableServer::ServantBase * serv,STAR::Upcall_ptr _star_up)
{
    STAR::StrForStruct _star_a0;
    STAR::StrForStruct _star_a1;
    STAR::InputStreamImpl* _star_in = _STAR_preRemoveHeader(_star_up);
    _star_a0 = _star_in -> read_string();
    _star_a1 = _star_in -> read_string();
    _STAR_postRemoveHeader(_star_up);
    POA_PG_Resource * objImpl=dynamic_cast< POA_PG_Resource*>( serv );
    if(objImpl!=NULL) 
    {
         objImpl ->pg_fe_setauthsvc(_star_a0, _star_a1);
    }
    _STAR_prePrepareHeader(_star_up);
    _STAR_postPrepareHeader(_star_up);
}

//
// Interface declare ::PG_Resource
//
CORBA::Boolean
POA_PG_Resource::_is_a(const char* type)
    throw(CORBA::SystemException)
{
    const char** _star_ids = PG_Resource::_STAR_staticIds();
    for(CORBA::ULong _star_i = 0 ; _star_ids[_star_i] != 0 ; _star_i++)
        if(strcmp(type, _star_ids[_star_i]) == 0)
            return true;

    return false;
}

CORBA::RepositoryId
POA_PG_Resource::_primary_interface(const PortableServer::ObjectId&,
                                    PortableServer::POA_ptr)
{
    return CORBA::string_dup(PG_Resource::_STAR_staticIds()[0]);
}

PG_Resource_ptr
POA_PG_Resource::_this()
{
    CORBA::Object_var obj = _STAR_buildReference();
    PG_Resource_var result = PG_Resource::_narrow(obj);
    return result._retn();
}

void
POA_PG_Resource::_STAR_dispatch(STAR::Upcall_ptr _star_up)
{
    static const PortableServer::OperationTableEntry PG_Resource_Optable_fixed[] =
    {
        {"_component",PortableServer::ServantBase::_STAR_op_get_component},
        {"_interface",PortableServer::ServantBase::_STAR_op_get_interface},
        {"_is_a",PortableServer::ServantBase::_STAR_op_is_a},
        {"_non_existent",PortableServer::ServantBase::_STAR_op_non_existent}
    };
    static const PortableServer::OperationTableEntry PG_Resource_Optable_unfixed[] =
    {
        {"pg_BeginTrans",POA_PG_Resource::_STAR_op_pg_BeginTrans},
        {"pg_PQclear",POA_PG_Resource::_STAR_op_pg_PQclear},
        {"pg_PQcmdStatus",POA_PG_Resource::_STAR_op_pg_PQcmdStatus},
        {"pg_PQdb",POA_PG_Resource::_STAR_op_pg_PQdb},
        {"pg_PQendcopy",POA_PG_Resource::_STAR_op_pg_PQendcopy},
        {"pg_PQerrorMessage",POA_PG_Resource::_STAR_op_pg_PQerrorMessage},
        {"pg_PQexec",POA_PG_Resource::_STAR_op_pg_PQexec},
        {"pg_PQfinish",POA_PG_Resource::_STAR_op_pg_PQfinish},
        {"pg_PQfname",POA_PG_Resource::_STAR_op_pg_PQfname},
        {"pg_PQfnumber",POA_PG_Resource::_STAR_op_pg_PQfnumber},
        {"pg_PQfsize",POA_PG_Resource::_STAR_op_pg_PQfsize},
        {"pg_PQftype",POA_PG_Resource::_STAR_op_pg_PQftype},
        {"pg_PQgetlength",POA_PG_Resource::_STAR_op_pg_PQgetlength},
        {"pg_PQgetline",POA_PG_Resource::_STAR_op_pg_PQgetline},
        {"pg_PQgetvalue",POA_PG_Resource::_STAR_op_pg_PQgetvalue},
        {"pg_PQhost",POA_PG_Resource::_STAR_op_pg_PQhost},
        {"pg_PQnfields",POA_PG_Resource::_STAR_op_pg_PQnfields},
        {"pg_PQnotifies",POA_PG_Resource::_STAR_op_pg_PQnotifies},
        {"pg_PQntuples",POA_PG_Resource::_STAR_op_pg_PQntuples},
        {"pg_PQoidStatus",POA_PG_Resource::_STAR_op_pg_PQoidStatus},
        {"pg_PQoptions",POA_PG_Resource::_STAR_op_pg_PQoptions},
        {"pg_PQport",POA_PG_Resource::_STAR_op_pg_PQport},
        {"pg_PQputline",POA_PG_Resource::_STAR_op_pg_PQputline},
        {"pg_PQreset",POA_PG_Resource::_STAR_op_pg_PQreset},
        {"pg_PQresultStatus",POA_PG_Resource::_STAR_op_pg_PQresultStatus},
        {"pg_PQsetdbLogin",POA_PG_Resource::_STAR_op_pg_PQsetdbLogin},
        {"pg_PQstatus",POA_PG_Resource::_STAR_op_pg_PQstatus},
        {"pg_PQtty",POA_PG_Resource::_STAR_op_pg_PQtty},
        {"pg_PQuntrace",POA_PG_Resource::_STAR_op_pg_PQuntrace},
        {"pg_commit",POA_PG_Resource::_STAR_op_pg_commit},
        {"pg_commit_one_phase",POA_PG_Resource::_STAR_op_pg_commit_one_phase},
        {"pg_fe_getauthname",POA_PG_Resource::_STAR_op_pg_fe_getauthname},
        {"pg_fe_setauthsvc",POA_PG_Resource::_STAR_op_pg_fe_setauthsvc},
        {"pg_forget",POA_PG_Resource::_STAR_op_pg_forget},
        {"pg_heuristic_decision",POA_PG_Resource::_STAR_op_pg_heuristic_decision},
        {"pg_prepare",POA_PG_Resource::_STAR_op_pg_prepare},
        {"pg_release",POA_PG_Resource::_STAR_op_pg_release},
        {"pg_rollback",POA_PG_Resource::_STAR_op_pg_rollback},
        {"pg_set_recoverycoordinator",POA_PG_Resource::_STAR_op_pg_set_recoverycoordinator}
    };
    static const CORBA::ULong _star_methodCount = 39;

    PortableServer::operationSkel operation;
    if(_STAR_locateOperation(_star_up,PG_Resource_Optable_unfixed,_star_methodCount, operation))
    {
        (*operation)(this,_star_up);
        return;
    }
    else
    if(_STAR_locateOperation(_star_up,PG_Resource_Optable_fixed,4, operation))
    {
        (*operation)(this,_star_up);
        return;
    }
    throw CORBA::BAD_OPERATION(0, CORBA::COMPLETED_NO);

}
