// **********************************************************************
//
// Copyright (c) 2001-2002 
// StarMiddleware Group	
// www.StarMiddleware.org
// 
// All Rights Reserved
//
// Author: An Jingbin	anjb@sina.com	
// **********************************************************************


#ifndef _PostgreSQLSession_h_
#define _PostgreSQLSession_h_

#include <PSS.h>
#include <DatabaseSession.h>

// PostgreSQL client library header
#include <libpq-fe.h>

#include <string>
#include <map>

using namespace std;

namespace PSS
{

class PostgreSQLSession:
		virtual public PSS::DatabaseSession
{
private:
	PGconn* database_;
	
public:
	PostgreSQLSession(CosPersistentState::AccessMode ,CosPersistentState::Connector_ptr );

	virtual ~PostgreSQLSession();



	virtual void 
		write_object_state(const PersistentHome* , const CosPersistentState::ShortPid& , ObjectState* ) 
		throw (CORBA::SystemException );

	virtual ObjectState* 
		read_object_state(const PersistentHome*, const CosPersistentState::ShortPid&)
		throw (CORBA::SystemException);

	virtual char* 
		marshal_column_name( const char *);

	virtual char*
		marshal_table_name( const char *);

	virtual bool 
		table_exists( const char*) const
		throw(CORBA::SystemException);	


	virtual bool 
		object_exists( const char*, const CosPersistentState::ShortPid&) const 
		throw (CORBA::SystemException);

	virtual void 
		destroy_object( const char*, const CosPersistentState::ShortPid&) 
		throw (CORBA::SystemException);

	virtual CosPersistentState::ShortPid*
		find_by_key( const char*, KeyState*)
		throw(CosPersistentState::NotFound,
			CORBA::SystemException);

	virtual CosPersistentState::ShortPidSeq*
		find_all( const char * )
		throw(CORBA::SystemException);


	virtual void
		create_table(const PersistentHome*)
		throw(CORBA::SystemException);

	void 
		connect(const CosPersistentState::ParameterList& )
		throw (CORBA::SystemException);
};

}

#endif /* #ifndef _PostgreSQLSession_h_ */

