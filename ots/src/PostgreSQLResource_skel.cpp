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

#include <OB/CORBAServer.h>
#include <PostgreSQLResource_skel.h>

#ifndef OB_INTEGER_VERSION
#   error No ORBacus version defined! Is <OB/CORBA.h> included?
#endif

#ifndef OB_NO_VERSION_CHECK
#   if (OB_INTEGER_VERSION != 4010000L)
#       error ORBacus version mismatch!
#   endif
#endif

//
// IDL:PG_Resource/pg_prepare:1.0
//
void
POA_PG_Resource::_OB_op_pg_prepare(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CosTransactions::Vote _ob_r = pg_prepare();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_ulong((CORBA::ULong)_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_rollback:1.0
//
void
POA_PG_Resource::_OB_op_pg_rollback(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_rollback();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_commit:1.0
//
void
POA_PG_Resource::_OB_op_pg_commit(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_commit();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_commit_one_phase:1.0
//
void
POA_PG_Resource::_OB_op_pg_commit_one_phase(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_commit_one_phase();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_forget:1.0
//
void
POA_PG_Resource::_OB_op_pg_forget(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_forget();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_release:1.0
//
void
POA_PG_Resource::_OB_op_pg_release(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_release();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_BeginTrans:1.0
//
void
POA_PG_Resource::_OB_op_pg_BeginTrans(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::Boolean _ob_r = pg_BeginTrans();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_boolean(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_set_recoverycoordinator:1.0
//
void
POA_PG_Resource::_OB_op_pg_set_recoverycoordinator(OB::Upcall_ptr _ob_up)
{
    CosTransactions::RecoveryCoordinator_var _ob_a0;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    OBUnmarshal(_ob_a0.inout(), _ob_in);
    _OB_postUnmarshal(_ob_up);
    pg_set_recoverycoordinator(_ob_a0);
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_heuristic_decision:1.0
//
void
POA_PG_Resource::_OB_op_pg_heuristic_decision(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_heuristic_decision();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQsetdb:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQsetdb(OB::Upcall_ptr _ob_up)
{
    OB::StrForStruct _ob_a0;
    OB::StrForStruct _ob_a1;
    OB::StrForStruct _ob_a2;
    OB::StrForStruct _ob_a3;
    OB::StrForStruct _ob_a4;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_string();
    _ob_a1 = _ob_in -> read_string();
    _ob_a2 = _ob_in -> read_string();
    _ob_a3 = _ob_in -> read_string();
    _ob_a4 = _ob_in -> read_string();
    _OB_postUnmarshal(_ob_up);
    pg_PQsetdb(_ob_a0, _ob_a1, _ob_a2, _ob_a3, _ob_a4);
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQdb:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQdb(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQdb();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQhost:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQhost(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQhost();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQoptions:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQoptions(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQoptions();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQport:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQport(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQport();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQtty:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQtty(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQtty();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQstatus:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQstatus(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_ConnStatusType _ob_r = pg_PQstatus();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_short(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQerrorMessage:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQerrorMessage(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQerrorMessage();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQfinish:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQfinish(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_PQfinish();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQreset:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQreset(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_PQreset();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQuntrace:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQuntrace(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_PQuntrace();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQexec:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQexec(OB::Upcall_ptr _ob_up)
{
    OB::StrForStruct _ob_a0;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_string();
    _OB_postUnmarshal(_ob_up);
    pg_PQexec(_ob_a0);
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQresultStatus:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQresultStatus(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_ExecStatusType _ob_r = pg_PQresultStatus();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_short(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQntuples:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQntuples(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::Long _ob_r = pg_PQntuples();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_long(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQnfields:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQnfields(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::Long _ob_r = pg_PQnfields();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_long(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQfname:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQfname(OB::Upcall_ptr _ob_up)
{
    CORBA::Long _ob_a0;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_long();
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQfname(_ob_a0);
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQfnumber:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQfnumber(OB::Upcall_ptr _ob_up)
{
    OB::StrForStruct _ob_a0;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_string();
    _OB_postUnmarshal(_ob_up);
    CORBA::Long _ob_r = pg_PQfnumber(_ob_a0);
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_long(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQftype:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQftype(OB::Upcall_ptr _ob_up)
{
    CORBA::Long _ob_a0;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_long();
    _OB_postUnmarshal(_ob_up);
    CORBA::Long _ob_r = pg_PQftype(_ob_a0);
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_long(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQfsize:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQfsize(OB::Upcall_ptr _ob_up)
{
    CORBA::Long _ob_a0;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_long();
    _OB_postUnmarshal(_ob_up);
    CORBA::Long _ob_r = pg_PQfsize(_ob_a0);
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_long(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQgetvalue:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQgetvalue(OB::Upcall_ptr _ob_up)
{
    CORBA::Long _ob_a0;
    CORBA::Long _ob_a1;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_long();
    _ob_a1 = _ob_in -> read_long();
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQgetvalue(_ob_a0, _ob_a1);
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQgetlength:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQgetlength(OB::Upcall_ptr _ob_up)
{
    CORBA::Long _ob_a0;
    CORBA::Long _ob_a1;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_long();
    _ob_a1 = _ob_in -> read_long();
    _OB_postUnmarshal(_ob_up);
    CORBA::Long _ob_r = pg_PQgetlength(_ob_a0, _ob_a1);
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_long(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQcmdStatus:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQcmdStatus(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQcmdStatus();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQoidStatus:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQoidStatus(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_PQoidStatus();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQclear:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQclear(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_PQclear();
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQnotifies:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQnotifies(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    pg_PGnotify_var _ob_r = pg_PQnotifies();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_r.in()._OB_marshal(_ob_out);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQgetline:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQgetline(OB::Upcall_ptr _ob_up)
{
    OB::StrForStruct _ob_a0;
    CORBA::Long _ob_a1;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_string();
    _ob_a1 = _ob_in -> read_long();
    _OB_postUnmarshal(_ob_up);
    CORBA::Long _ob_r = pg_PQgetline(_ob_a0, _ob_a1);
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_long(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQputline:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQputline(OB::Upcall_ptr _ob_up)
{
    OB::StrForStruct _ob_a0;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_string();
    _OB_postUnmarshal(_ob_up);
    pg_PQputline(_ob_a0);
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_PQendcopy:1.0
//
void
POA_PG_Resource::_OB_op_pg_PQendcopy(OB::Upcall_ptr _ob_up)
{
    _OB_preUnmarshal(_ob_up);
    _OB_postUnmarshal(_ob_up);
    CORBA::Long _ob_r = pg_PQendcopy();
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_long(_ob_r);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_fe_getauthname:1.0
//
void
POA_PG_Resource::_OB_op_pg_fe_getauthname(OB::Upcall_ptr _ob_up)
{
    OB::StrForStruct _ob_a0;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_string();
    _OB_postUnmarshal(_ob_up);
    CORBA::String_var _ob_r = pg_fe_getauthname(_ob_a0);
    OB::OutputStreamImpl* _ob_out = _OB_preMarshal(_ob_up);
    _ob_out -> write_string(_ob_r.in());
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource/pg_fe_setauthsvc:1.0
//
void
POA_PG_Resource::_OB_op_pg_fe_setauthsvc(OB::Upcall_ptr _ob_up)
{
    OB::StrForStruct _ob_a0;
    OB::StrForStruct _ob_a1;
    OB::InputStreamImpl* _ob_in = _OB_preUnmarshal(_ob_up);
    _ob_a0 = _ob_in -> read_string();
    _ob_a1 = _ob_in -> read_string();
    _OB_postUnmarshal(_ob_up);
    pg_fe_setauthsvc(_ob_a0, _ob_a1);
    _OB_preMarshal(_ob_up);
    _OB_postMarshal(_ob_up);
}

//
// IDL:PG_Resource:1.0
//
CORBA::Boolean
POA_PG_Resource::_is_a(const char* type)
    throw(CORBA::SystemException)
{
    const char** _ob_ids = PG_Resource::_OB_staticIds();
    for(CORBA::ULong _ob_i = 0 ; _ob_ids[_ob_i] != 0 ; _ob_i++)
        if(strcmp(type, _ob_ids[_ob_i]) == 0)
            return true;

    return false;
}

CORBA::RepositoryId
POA_PG_Resource::_primary_interface(const PortableServer::ObjectId&,
                                    PortableServer::POA_ptr)
{
    return CORBA::string_dup(PG_Resource::_OB_staticIds()[0]);
}

PG_Resource_ptr
POA_PG_Resource::_this()
{
    CORBA::Object_var obj = _OB_createReference();
    PG_Resource_var result = PG_Resource::_narrow(obj);
    return result._retn();
}

OB::DirectStubImpl_ptr
POA_PG_Resource::_OB_createDirectStubImpl(PortableServer::POA_ptr poa,
                                          const PortableServer::ObjectId& oid)
{
    return new OBDirectStubImpl_PG_Resource(poa, oid, this);
}

void
POA_PG_Resource::_OB_dispatch(OB::Upcall_ptr _ob_up)
{
    static const char* _ob_names[] =
    {
        "pg_BeginTrans",
        "pg_PQclear",
        "pg_PQcmdStatus",
        "pg_PQdb",
        "pg_PQendcopy",
        "pg_PQerrorMessage",
        "pg_PQexec",
        "pg_PQfinish",
        "pg_PQfname",
        "pg_PQfnumber",
        "pg_PQfsize",
        "pg_PQftype",
        "pg_PQgetlength",
        "pg_PQgetline",
        "pg_PQgetvalue",
        "pg_PQhost",
        "pg_PQnfields",
        "pg_PQnotifies",
        "pg_PQntuples",
        "pg_PQoidStatus",
        "pg_PQoptions",
        "pg_PQport",
        "pg_PQputline",
        "pg_PQreset",
        "pg_PQresultStatus",
        "pg_PQsetdb",
        "pg_PQstatus",
        "pg_PQtty",
        "pg_PQuntrace",
        "pg_commit",
        "pg_commit_one_phase",
        "pg_fe_getauthname",
        "pg_fe_setauthsvc",
        "pg_forget",
        "pg_heuristic_decision",
        "pg_prepare",
        "pg_release",
        "pg_rollback",
        "pg_set_recoverycoordinator"
    };
    static const CORBA::ULong _ob_numNames = 39;

    switch(_OB_findOperation(_ob_up, _ob_names, _ob_numNames))
    {
    case 0: // pg_BeginTrans
        _OB_op_pg_BeginTrans(_ob_up);
        return;

    case 1: // pg_PQclear
        _OB_op_pg_PQclear(_ob_up);
        return;

    case 2: // pg_PQcmdStatus
        _OB_op_pg_PQcmdStatus(_ob_up);
        return;

    case 3: // pg_PQdb
        _OB_op_pg_PQdb(_ob_up);
        return;

    case 4: // pg_PQendcopy
        _OB_op_pg_PQendcopy(_ob_up);
        return;

    case 5: // pg_PQerrorMessage
        _OB_op_pg_PQerrorMessage(_ob_up);
        return;

    case 6: // pg_PQexec
        _OB_op_pg_PQexec(_ob_up);
        return;

    case 7: // pg_PQfinish
        _OB_op_pg_PQfinish(_ob_up);
        return;

    case 8: // pg_PQfname
        _OB_op_pg_PQfname(_ob_up);
        return;

    case 9: // pg_PQfnumber
        _OB_op_pg_PQfnumber(_ob_up);
        return;

    case 10: // pg_PQfsize
        _OB_op_pg_PQfsize(_ob_up);
        return;

    case 11: // pg_PQftype
        _OB_op_pg_PQftype(_ob_up);
        return;

    case 12: // pg_PQgetlength
        _OB_op_pg_PQgetlength(_ob_up);
        return;

    case 13: // pg_PQgetline
        _OB_op_pg_PQgetline(_ob_up);
        return;

    case 14: // pg_PQgetvalue
        _OB_op_pg_PQgetvalue(_ob_up);
        return;

    case 15: // pg_PQhost
        _OB_op_pg_PQhost(_ob_up);
        return;

    case 16: // pg_PQnfields
        _OB_op_pg_PQnfields(_ob_up);
        return;

    case 17: // pg_PQnotifies
        _OB_op_pg_PQnotifies(_ob_up);
        return;

    case 18: // pg_PQntuples
        _OB_op_pg_PQntuples(_ob_up);
        return;

    case 19: // pg_PQoidStatus
        _OB_op_pg_PQoidStatus(_ob_up);
        return;

    case 20: // pg_PQoptions
        _OB_op_pg_PQoptions(_ob_up);
        return;

    case 21: // pg_PQport
        _OB_op_pg_PQport(_ob_up);
        return;

    case 22: // pg_PQputline
        _OB_op_pg_PQputline(_ob_up);
        return;

    case 23: // pg_PQreset
        _OB_op_pg_PQreset(_ob_up);
        return;

    case 24: // pg_PQresultStatus
        _OB_op_pg_PQresultStatus(_ob_up);
        return;

    case 25: // pg_PQsetdb
        _OB_op_pg_PQsetdb(_ob_up);
        return;

    case 26: // pg_PQstatus
        _OB_op_pg_PQstatus(_ob_up);
        return;

    case 27: // pg_PQtty
        _OB_op_pg_PQtty(_ob_up);
        return;

    case 28: // pg_PQuntrace
        _OB_op_pg_PQuntrace(_ob_up);
        return;

    case 29: // pg_commit
        _OB_op_pg_commit(_ob_up);
        return;

    case 30: // pg_commit_one_phase
        _OB_op_pg_commit_one_phase(_ob_up);
        return;

    case 31: // pg_fe_getauthname
        _OB_op_pg_fe_getauthname(_ob_up);
        return;

    case 32: // pg_fe_setauthsvc
        _OB_op_pg_fe_setauthsvc(_ob_up);
        return;

    case 33: // pg_forget
        _OB_op_pg_forget(_ob_up);
        return;

    case 34: // pg_heuristic_decision
        _OB_op_pg_heuristic_decision(_ob_up);
        return;

    case 35: // pg_prepare
        _OB_op_pg_prepare(_ob_up);
        return;

    case 36: // pg_release
        _OB_op_pg_release(_ob_up);
        return;

    case 37: // pg_rollback
        _OB_op_pg_rollback(_ob_up);
        return;

    case 38: // pg_set_recoverycoordinator
        _OB_op_pg_set_recoverycoordinator(_ob_up);
        return;
    }

    _OB_dispatchBase(_ob_up);
}

//
// IDL:PG_Resource:1.0
//
OBDirectStubImpl_PG_Resource::OBDirectStubImpl_PG_Resource(PortableServer::POA_ptr poa,
                                                           const PortableServer::ObjectId& oid,
                                                           PortableServer::ServantBase* servant)
#ifdef HAVE_VCPLUSPLUS_BUGS
{
    _ob_initialize(poa, oid, servant);
}
#else
    : OB::DirectStubImpl(poa, oid, servant)
{
}
#endif

//
// IDL:PG_Resource/pg_prepare:1.0
//
CosTransactions::Vote
OBDirectStubImpl_PG_Resource::pg_prepare()
{
    OB::InvocationHandler _ob_handler(this, "pg_prepare");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_prepare();
}

//
// IDL:PG_Resource/pg_rollback:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_rollback()
{
    OB::InvocationHandler _ob_handler(this, "pg_rollback");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_rollback();
}

//
// IDL:PG_Resource/pg_commit:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_commit()
{
    OB::InvocationHandler _ob_handler(this, "pg_commit");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_commit();
}

//
// IDL:PG_Resource/pg_commit_one_phase:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_commit_one_phase()
{
    OB::InvocationHandler _ob_handler(this, "pg_commit_one_phase");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_commit_one_phase();
}

//
// IDL:PG_Resource/pg_forget:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_forget()
{
    OB::InvocationHandler _ob_handler(this, "pg_forget");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_forget();
}

//
// IDL:PG_Resource/pg_release:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_release()
{
    OB::InvocationHandler _ob_handler(this, "pg_release");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_release();
}

//
// IDL:PG_Resource/pg_BeginTrans:1.0
//
CORBA::Boolean
OBDirectStubImpl_PG_Resource::pg_BeginTrans()
{
    OB::InvocationHandler _ob_handler(this, "pg_BeginTrans");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_BeginTrans();
}

//
// IDL:PG_Resource/pg_set_recoverycoordinator:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_set_recoverycoordinator(CosTransactions::RecoveryCoordinator_ptr _ob_a0)
{
    OB::InvocationHandler _ob_handler(this, "pg_set_recoverycoordinator");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_set_recoverycoordinator(_ob_a0);
}

//
// IDL:PG_Resource/pg_heuristic_decision:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_heuristic_decision()
{
    OB::InvocationHandler _ob_handler(this, "pg_heuristic_decision");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_heuristic_decision();
}

//
// IDL:PG_Resource/pg_PQsetdb:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_PQsetdb(const char* _ob_a0,
                                         const char* _ob_a1,
                                         const char* _ob_a2,
                                         const char* _ob_a3,
                                         const char* _ob_a4)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQsetdb");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQsetdb(_ob_a0, _ob_a1, _ob_a2, _ob_a3, _ob_a4);
}

//
// IDL:PG_Resource/pg_PQdb:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQdb()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQdb");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQdb();
}

//
// IDL:PG_Resource/pg_PQhost:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQhost()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQhost");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQhost();
}

//
// IDL:PG_Resource/pg_PQoptions:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQoptions()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQoptions");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQoptions();
}

//
// IDL:PG_Resource/pg_PQport:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQport()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQport");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQport();
}

//
// IDL:PG_Resource/pg_PQtty:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQtty()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQtty");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQtty();
}

//
// IDL:PG_Resource/pg_PQstatus:1.0
//
pg_ConnStatusType
OBDirectStubImpl_PG_Resource::pg_PQstatus()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQstatus");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQstatus();
}

//
// IDL:PG_Resource/pg_PQerrorMessage:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQerrorMessage()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQerrorMessage");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQerrorMessage();
}

//
// IDL:PG_Resource/pg_PQfinish:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_PQfinish()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQfinish");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQfinish();
}

//
// IDL:PG_Resource/pg_PQreset:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_PQreset()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQreset");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQreset();
}

//
// IDL:PG_Resource/pg_PQuntrace:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_PQuntrace()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQuntrace");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQuntrace();
}

//
// IDL:PG_Resource/pg_PQexec:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_PQexec(const char* _ob_a0)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQexec");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQexec(_ob_a0);
}

//
// IDL:PG_Resource/pg_PQresultStatus:1.0
//
pg_ExecStatusType
OBDirectStubImpl_PG_Resource::pg_PQresultStatus()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQresultStatus");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQresultStatus();
}

//
// IDL:PG_Resource/pg_PQntuples:1.0
//
CORBA::Long
OBDirectStubImpl_PG_Resource::pg_PQntuples()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQntuples");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQntuples();
}

//
// IDL:PG_Resource/pg_PQnfields:1.0
//
CORBA::Long
OBDirectStubImpl_PG_Resource::pg_PQnfields()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQnfields");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQnfields();
}

//
// IDL:PG_Resource/pg_PQfname:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQfname(CORBA::Long _ob_a0)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQfname");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQfname(_ob_a0);
}

//
// IDL:PG_Resource/pg_PQfnumber:1.0
//
CORBA::Long
OBDirectStubImpl_PG_Resource::pg_PQfnumber(const char* _ob_a0)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQfnumber");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQfnumber(_ob_a0);
}

//
// IDL:PG_Resource/pg_PQftype:1.0
//
CORBA::Long
OBDirectStubImpl_PG_Resource::pg_PQftype(CORBA::Long _ob_a0)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQftype");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQftype(_ob_a0);
}

//
// IDL:PG_Resource/pg_PQfsize:1.0
//
CORBA::Long
OBDirectStubImpl_PG_Resource::pg_PQfsize(CORBA::Long _ob_a0)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQfsize");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQfsize(_ob_a0);
}

//
// IDL:PG_Resource/pg_PQgetvalue:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQgetvalue(CORBA::Long _ob_a0,
                                            CORBA::Long _ob_a1)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQgetvalue");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQgetvalue(_ob_a0, _ob_a1);
}

//
// IDL:PG_Resource/pg_PQgetlength:1.0
//
CORBA::Long
OBDirectStubImpl_PG_Resource::pg_PQgetlength(CORBA::Long _ob_a0,
                                             CORBA::Long _ob_a1)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQgetlength");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQgetlength(_ob_a0, _ob_a1);
}

//
// IDL:PG_Resource/pg_PQcmdStatus:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQcmdStatus()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQcmdStatus");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQcmdStatus();
}

//
// IDL:PG_Resource/pg_PQoidStatus:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_PQoidStatus()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQoidStatus");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQoidStatus();
}

//
// IDL:PG_Resource/pg_PQclear:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_PQclear()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQclear");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQclear();
}

//
// IDL:PG_Resource/pg_PQnotifies:1.0
//
pg_PGnotify*
OBDirectStubImpl_PG_Resource::pg_PQnotifies()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQnotifies");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQnotifies();
}

//
// IDL:PG_Resource/pg_PQgetline:1.0
//
CORBA::Long
OBDirectStubImpl_PG_Resource::pg_PQgetline(const char* _ob_a0,
                                           CORBA::Long _ob_a1)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQgetline");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQgetline(_ob_a0, _ob_a1);
}

//
// IDL:PG_Resource/pg_PQputline:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_PQputline(const char* _ob_a0)
{
    OB::InvocationHandler _ob_handler(this, "pg_PQputline");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQputline(_ob_a0);
}

//
// IDL:PG_Resource/pg_PQendcopy:1.0
//
CORBA::Long
OBDirectStubImpl_PG_Resource::pg_PQendcopy()
{
    OB::InvocationHandler _ob_handler(this, "pg_PQendcopy");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_PQendcopy();
}

//
// IDL:PG_Resource/pg_fe_getauthname:1.0
//
char*
OBDirectStubImpl_PG_Resource::pg_fe_getauthname(const char* _ob_a0)
{
    OB::InvocationHandler _ob_handler(this, "pg_fe_getauthname");
    return dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_fe_getauthname(_ob_a0);
}

//
// IDL:PG_Resource/pg_fe_setauthsvc:1.0
//
void
OBDirectStubImpl_PG_Resource::pg_fe_setauthsvc(const char* _ob_a0,
                                               const char* _ob_a1)
{
    OB::InvocationHandler _ob_handler(this, "pg_fe_setauthsvc");
    dynamic_cast<POA_PG_Resource*>(_ob_servant_) -> pg_fe_setauthsvc(_ob_a0, _ob_a1);
}
