// **********************************************************************
//
// Generated by the CIDL Translator
//
// Copyright (c) 2001-2004
// StarMiddleware Group
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Huang Jie        huangjie@email.com
// Author: Chang Junsheng   cjs7908@163.com
// Author: Zhen Xianrong
//
// **********************************************************************

#include <CORBA.h>
#include <CCM_DelAccountComp_impl.h>
#include <CommonFunc.h>

#include <string>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::CCM_DelAccountComp_impl():database_( NULL )
{
}

Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::~CCM_DelAccountComp_impl()
{
}

void
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::configuration_complete()
	throw (Components::InvalidConfiguration, CORBA::SystemException)
{
}

void
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::enter_dyn_deploy()
	throw(CORBA::SystemException)
{
}

void
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::leave_dyn_deploy()
	throw(CORBA::SystemException)
{
}

void
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::set_session_context(Components::SessionContext* ctx)
    throw(Components::CCMException,
          CORBA::SystemException)
{
    #ifdef TAO
    ctx_ = Bank::CCM_DelAccountComp_Context::_duplicate(dynamic_cast<Bank::CCM_DelAccountComp_Context*>(ctx));
    #else
    ctx_ = Bank::CCM_DelAccountComp_Context::_narrow(ctx);
    #endif
}

void
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::ccm_activate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::ccm_passivate()
    throw(Components::CCMException,
          CORBA::SystemException)
{
}

void
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::ccm_remove()
    throw(Components::CCMException,
          CORBA::SystemException)
{
	if( database_ != NULL)
	{
		PQfinish(database_);
		database_ = NULL;
	}
}


void
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::DelAccount(const char* id)
    throw(CORBA::SystemException)
{
    if( database_ == NULL)
	{
		connect_database();
	}    
	
	string query = "delete from account where id = \'";
	query += id;
	query += "\'";
	
	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}
}

Bank::DelAccountCompComposition::CCM_DelAccountCompHome_impl::CCM_DelAccountCompHome_impl()
{
}

Bank::DelAccountCompComposition::CCM_DelAccountCompHome_impl::~CCM_DelAccountCompHome_impl()
{
}

Components::EnterpriseComponent_ptr
Bank::DelAccountCompComposition::CCM_DelAccountCompHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new Bank::DelAccountCompComposition::CCM_DelAccountComp_impl();
}

char*
Bank::DelAccountCompComposition::CCM_DelAccountCompHome_impl::get_storage_home_id()
{
    return 0;
}



CORBA::Boolean
Bank::DelAccountCompComposition::CCM_DelAccountComp_impl::connect_database()
    throw(CORBA::SystemException)
{
	// TODO: Implementation
	CORBA::Boolean _r = false;
	
//modified by xiao heping 2004/07/20		
//	string conninfo = "";
//	conninfo += "hostaddr = ";
//	conninfo += "172.26.13.88";
//	
//	conninfo += " dbname = \'";
//	conninfo += "bank";
//	conninfo += "\'";
//	
//	conninfo += " user = \'";
//	conninfo += "ccm";
//	conninfo += "\'";
//	
//	conninfo += " password = \'";
//	conninfo += "ccm613";
//	conninfo += "\'";

	string str = HelpFun::GetCCMProfileString("Bank","Bank_Database_Host","","starccm.properties");
	string conninfo = "";
	conninfo += "hostaddr = ";
	conninfo += str;
	
	str = HelpFun::GetCCMProfileString("Bank","Bank_Database_Name","","starccm.properties");	
	conninfo += " dbname = \'";
	conninfo += str;
	conninfo += "\'";
	
	str = HelpFun::GetCCMProfileString("Bank","Bank_Database_User","","starccm.properties");	
	conninfo += " user = \'";
	conninfo += str;
	conninfo += "\'";
	
	str = HelpFun::GetCCMProfileString("Bank","Bank_Database_Password","","starccm.properties");
	conninfo += " password = \'";
	conninfo += str;
	conninfo += "\'";
//end modify 
	
	PGconn * conn = PQconnectdb( conninfo.c_str() );
	
	if (PQstatus(conn) == CONNECTION_BAD)
	{
		PQfinish(conn);
		throw CORBA::PERSIST_STORE();
	}

	
	database_ = conn;    
	_r = true;
		
	return _r;
}
