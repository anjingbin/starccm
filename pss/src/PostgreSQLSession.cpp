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

#include <PSS.h>
#include <PostgreSQLSession.h>


#define QUERY_BUFFER_LEN	(1024*2)

PSS::PostgreSQLSession::PostgreSQLSession(CosPersistentState::AccessMode access_mode,CosPersistentState::Connector_ptr connector):
	DatabaseSession(access_mode,connector),database_(NULL)
{
}

PSS::PostgreSQLSession::~PostgreSQLSession()
{
}

void
PSS::PostgreSQLSession::write_object_state(const PersistentHome* home, const CosPersistentState::ShortPid& short_pid, ObjectState* state) 
	throw (CORBA::SystemException )
{
	const char* table_name = home -> get_table();

	if( !table_exists(table_name) )
	{
		PSSTRACE1("PSS::PostgreSQLSession::write_object_state : database table \"%s\" does not exist!", table_name);
		throw CORBA::PERSIST_STORE();
	}

	char* escaped_spid = NULL;
	size_t length;

	unsigned char* spid = new unsigned char [short_pid.length()];

	memcpy( spid,short_pid.get_buffer(),short_pid.length());
	
	escaped_spid = (char*)PQescapeBytea(spid, short_pid.length(), &length);

 	string query = "select * from ";
	query += table_name;
	query += " where short_pid = \'";
	query += escaped_spid;
	query += "\'";


	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_TUPLES_OK)
	{
		PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}

	bool exist = PQntuples(result) >= 1;
	PQclear(result);
	query.empty();


	if( exist)
	{
		// update
		query = "update ";
		query += table_name;
		query += " set ";

		ObjectState::iterator it;
		for(it = state -> begin(); it != state -> end(); it++)
		{
			StateItem* item = &(*it);
			const unsigned char* data = item -> data();

			unsigned char* _data = new unsigned char[item -> length()];
			memcpy(_data,data,item -> length());

			size_t len;
			char* escaped_data = (char*)PQescapeBytea(_data, item -> length(), &len);

			delete[] _data;
			
			
			char* colname = marshal_column_name( item -> name());

			if( it != state -> begin())
				query += ",";

			query += colname;
			query += " = \'";
			query += escaped_data;
			query += "\'";

			free(escaped_data);
			delete[] colname;
		}
		
		query += " where ";
		query += "short_pid = \'";
		query += escaped_spid;
		query += "\'";

		result = PQexec(database_,query.c_str());

		if( PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
			PQclear(result);
			free(escaped_spid);
			
			throw CORBA::PERSIST_STORE();
		}

		PQclear(result);
	}
	else
	{
		// insert
		query = "insert into ";
		query += table_name;
		query += " values (";

		query += "\'";
		query += escaped_spid;
		query += "\'";

		ObjectState::iterator it;
		for(it = state -> begin(); it != state -> end(); it++)
		{
			StateItem* item = &(*it);
			const unsigned char* data = item -> data();
			unsigned char* _data = new unsigned char[item -> length()];
			memcpy(_data,data,item -> length());
			
			size_t len;
			char* escaped_data = (char*)PQescapeBytea(_data, item -> length(), &len);
			
			delete[] _data;

			query += ", \'";
			query += escaped_data;
			query += "\'";

			free(escaped_data);

		}
		
		query += ")";

		result = PQexec(database_,query.c_str());

		if( PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));

			PQclear(result);
			free(escaped_spid);
			
			throw CORBA::PERSIST_STORE();
		}

		PQclear(result);
	}

	free(escaped_spid);
}

PSS::ObjectState*
PSS::PostgreSQLSession::read_object_state(const PersistentHome* home, const CosPersistentState::ShortPid& short_pid) 
	throw (CORBA::SystemException )
{
	const char* table_name = home -> get_table();
	if( !table_exists( table_name))
	{
		PSSTRACE1("PSS::PostgreSQLSession::read_object_state : database table \"%s\" does not exist!", table_name);
		throw CORBA::PERSIST_STORE();
	}

	ObjectState* state = NULL;

	StateItem item;

	char* escaped_spid = NULL;
	size_t length;

	unsigned char* spid = new unsigned char [short_pid.length()];

	memcpy( spid,short_pid.get_buffer(),short_pid.length());

	escaped_spid = (char*)PQescapeBytea(spid, short_pid.length(), &length);

 	string query = "select * from ";
	query += table_name;
	query += " where short_pid = \'";
	query += escaped_spid;
	query += "\'";

	free(escaped_spid);

	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_TUPLES_OK)
	{
		PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}

	int tuple_num = PQntuples(result);

	if( tuple_num == 0 )
	{
		// the target storage object must be exist!
		PSSTRACE("PSS::PostgreSQLSession::read_object_state : query by short_pid return 0 tuples!");
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}
	else if( tuple_num != 1)
	{
		// and there must be only one object exist,since the short pid is the primary key!
		PSSTRACE("PSS::PostgreSQLSession::read_object_state : query by short_pid return >1 tuples!");
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}

	// check the number of fields
	int col_num = home -> get_state_count();
	if( PQnfields(result) != (col_num +1))
	{
		PSSTRACE("PSS::PostgreSQLSession::read_object_state : count of fields mismatch!");
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}


	state = new ObjectState;

	int col;

	// Here we begin with the second column,since the first one is always the short pid!
	for( col = 1; col <= col_num; col++)
	{
		// now retrieve the state data
		//int data_len = PQgetlength(result,		//the result set
		//							0,			//retrieve the first tuple
		//							col);

		char*  orig_data = PQgetvalue(result, 0, col);
		unsigned int ue_len;
		unsigned char * ue_data = PQunescapeBytea((unsigned char*)orig_data,&ue_len);

		item.data( ue_data, ue_len);

		state -> push_back(item);
	}

	PQclear(result);
	
	return state;
}

void
PSS::PostgreSQLSession::create_table(const PersistentHome* home)
	throw(CORBA::SystemException)	
{
	const char* table_name = home -> get_table();
	string query = "create table ";
	
	query += table_name;
	query += " (";

	query += "short_pid bytea PRIMARY KEY";

	// HomeMembers: a vector of strings
	HomeMembers* members = home -> get_home_members();

	int num = members -> size();
	int i;
	for( i = 0; i < num; i++)
	{
		char * temp = marshal_column_name( (*members)[i].c_str());
		if (temp == NULL)
		{
			PSSTRACE("PSS::PostgreSQLSession::create_table : illegal table name!");
			throw CORBA::PERSIST_STORE();
		}

		string col_name = temp;

		query += ',';

		query += col_name;
		query += " bytea";
	}
	
	query += ")";

	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}

	PQclear(result);
	delete members;
}

char*
PSS::PostgreSQLSession::marshal_column_name( const char* col_name)
{
	if( col_name == NULL )
		return NULL;

	int orig_len = strlen(col_name);

	if( orig_len == 0)
		return NULL;

	char* fixed_name = new char[ orig_len * 2 + 2];
	memset( fixed_name , 0, orig_len * 2 + 2);

	int index = 0;
	fixed_name[index++] = '_';

	int i;
	for( i = 0; i < orig_len; i++)
	{
		if( col_name[i] == '_' )
		{
			fixed_name[index++] = '_';
			fixed_name[index++] = col_name[i];
		}
		else if( col_name[i] >= 'A' && col_name[i] <= 'Z' )
		{
			fixed_name[index++] = '_';
			fixed_name[index++] = col_name[i] - ('A' - 'a');
		}
		else
			fixed_name[index++] = col_name[i];
	}

	return fixed_name;
}

char*
PSS::PostgreSQLSession::marshal_table_name( const char* table_name)
{
	if( table_name == NULL )
		return NULL;

	int orig_len = strlen(table_name);

	if( orig_len == 0)
		return NULL;

	char* fixed_name = new char[ orig_len * 2 + 2];
	memset( fixed_name , 0, orig_len * 2 + 2);

	int index = 0;
	fixed_name[index++] = '_';

	int i;
	
	for( i = 0; i < orig_len; i++)
	{
		if( table_name[i] == ':' || 
			table_name[i] == '/' || 
			table_name[i] == '\\' || 
			table_name[i] == '.')
		{
			fixed_name[index++] = '_';
			fixed_name[index++] = '_';
			continue;
		}


		if( table_name[i] == '_' )
		{
			fixed_name[index++] = '_';
			fixed_name[index++] = table_name[i];
		}
		else if( table_name[i] >= 'A' && table_name[i] <= 'Z' )
		{
			fixed_name[index++] = '_';
			fixed_name[index++] = table_name[i] - ('A' - 'a');
		}
		else
			fixed_name[index++] = table_name[i];
	}

	return fixed_name;
}

bool 
PSS::PostgreSQLSession::table_exists( const char* table) const
	throw(CORBA::SystemException)	
{
	string query = "select tablename from pg_tables where tablename = \'";
	query += table;
	query +="\'";

	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_TUPLES_OK)
	{
		PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}

	bool _ret = false;

	if( PQntuples(result) > 0 )
		_ret = true;

	return _ret;
}

bool 
PSS::PostgreSQLSession::object_exists( const char* table, const CosPersistentState::ShortPid& short_pid) const
	throw (CORBA::SystemException)
{
	if( !table_exists( table) )
	{
		PSSTRACE1("PSS::PostgreSQLSession::destroy_object : database table \"%s\" does not exist!", table);
		throw CORBA::PERSIST_STORE();
	}

	char* escaped_spid = NULL;
	size_t length;

	unsigned char* spid = new unsigned char [short_pid.length()];

	memcpy( spid,short_pid.get_buffer(),short_pid.length());
	
	escaped_spid = (char*)PQescapeBytea(spid, short_pid.length(), &length);

 	string query = "select * from ";
	query += table;
	query += " where short_pid = \'";
	query += escaped_spid;
	query += "\'";

	free(escaped_spid);

	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_TUPLES_OK)
	{
		PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}

	bool exist = PQntuples(result) > 0;

	PQclear(result);

	return exist;
}

void 
PSS::PostgreSQLSession::destroy_object( const char* table, const CosPersistentState::ShortPid& short_pid)
	throw (CORBA::SystemException)
{
	if( !table_exists( table) )
	{
		PSSTRACE1("PSS::PostgreSQLSession::destroy_object : database table \"%s\" does not exist!", table);
		throw CORBA::PERSIST_STORE();
	}

	char* escaped_spid = NULL;
	size_t length;

	unsigned char* spid = new unsigned char [short_pid.length()];

	memcpy( spid,short_pid.get_buffer(),short_pid.length());
	
	escaped_spid = (char*)PQescapeBytea(spid, short_pid.length(), &length);

 	string query = "delete from ";
	query += table;
	query += " where short_pid = \'";
	query += escaped_spid;
	query += "\'";

	free(escaped_spid);

	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}

	PQclear(result);
}

void 
PSS::PostgreSQLSession::connect(const CosPersistentState::ParameterList& parameters)
	throw (CORBA::SystemException)
{
	const char* host = NULL;
	const char* port = NULL;
	const char* user = NULL;
	const char* database = NULL;
	const char* password = NULL;

	for( CORBA::ULong i = 0; i < parameters.length(); i++)
	{
		if( strcmp( parameters[i].name, "host") == 0 )
		{
			parameters[i].val >>= host;
		}
		else if( strcmp( parameters[i].name, "port" ) == 0)
		{
			parameters[i].val >>= port;
		}
		else if( strcmp( parameters[i].name, "user" ) == 0)
		{
			parameters[i].val >>= user;
		}
		else if( strcmp( parameters[i].name, "password") == 0)
		{
			parameters[i].val >>= password;
		}
		else if( strcmp( parameters[i].name, "database") == 0)
		{
			parameters[i].val >>= database;
		}
	}


	if( (host == NULL) ||
		(user == NULL) ||
		(database == NULL) ||
		(password == NULL))
	{
		PSSTRACE("Parameters for database connection not enough!");
		throw CORBA::PERSIST_STORE();
	}

	string conninfo = "";
	conninfo += "hostaddr = ";
	conninfo += host;

	conninfo += " dbname = \'";
	conninfo += database;
	conninfo += "\'";

	conninfo += " user = \'";
	conninfo += user;
	conninfo += "\'";

	conninfo += " password = \'";
	conninfo += password;
	conninfo += "\'";

	PGconn * conn = PQconnectdb( conninfo.c_str() );
	
	if (PQstatus(conn) == CONNECTION_BAD)
	{
		PSSTRACE1("PQconnectdb failed! \n\terror : %s",PQerrorMessage(conn));
		PQfinish(conn);
		throw CORBA::PERSIST_STORE();
	}
	
	database_ = conn;
}

CosPersistentState::ShortPid*
PSS::PostgreSQLSession::find_by_key( const char* table,  KeyState* key_state)
	throw(CosPersistentState::NotFound,
		CORBA::SystemException)
{
	CosPersistentState::ShortPid_var _ret = new CosPersistentState::ShortPid;

	if( !table_exists(table) )
	{
		PSSTRACE1("PSS::PostgreSQLSession::find_by_key : database table \"%s\" does not exist!", table);
		throw CORBA::PERSIST_STORE();
	}

 	string query = "select short_pid from ";
	query += table;
	query += " where ";

	KeyState::iterator it;
	for(it = key_state -> begin(); it != key_state -> end(); it++)
	{
		StateItem* item = &(*it);

		const unsigned char* data = item -> data();

		unsigned char* _data = new unsigned char[item -> length()];
		memcpy(_data,data,item -> length());

		size_t len;
		char* escaped_data = (char*)PQescapeBytea(_data, item -> length(), &len);

		delete[] _data;
		
		char* colname = marshal_column_name( item -> name());

		if( it != key_state -> begin())
			query += " and ";

		query += colname;
		query += " = \'";
		query += escaped_data;
		query += "\'";

		free(escaped_data);
		delete[] colname;
	}
		
	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_TUPLES_OK)
	{
		PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}


	int ret_count = PQntuples(result);
	if( ret_count == 0)
	{
		PQclear(result);
		throw CosPersistentState::NotFound();
	}

	//int len = PQgetlength(result, 0, 0);
	char*  data = PQgetvalue(result, 0, 0);
	unsigned int ue_len;
	unsigned char * ue_data = PQunescapeBytea((unsigned char*)data,&ue_len);

	_ret -> length(ue_len);

	for( unsigned int i = 0; i < ue_len; i++)
	{
		_ret[i] = ue_data[i];
	}

	PQclear(result);
	free(ue_data);
	return _ret._retn();
}
CosPersistentState::ShortPidSeq*
PSS::PostgreSQLSession::find_all( const char * table)
	throw(CORBA::SystemException)
{
	CosPersistentState::ShortPidSeq_var _ret = new CosPersistentState::ShortPidSeq;

	if( !table_exists(table) )
	{
		PSSTRACE1("PSS::PostgreSQLSession::find_all : database table \"%s\" does not exist!", table);
		throw CORBA::PERSIST_STORE();
	}

 	string query = "select short_pid from ";
	query += table;
		
	PGresult * result = PQexec(database_,query.c_str());

	if( PQresultStatus(result) != PGRES_TUPLES_OK)
	{
		PSSTRACE2("SQL: \"%s\"\n\terror:%s",query,PQresultErrorMessage(result));
		PQclear(result);
		throw CORBA::PERSIST_STORE();
	}


	int ret_count = PQntuples(result);
	
	_ret -> length(ret_count);

	for( int i = 0; i < ret_count; i++)
	{
		//int len = PQgetlength(result, i, 0);
		char*  data = PQgetvalue(result, i, 0);
		unsigned int ue_len;
		unsigned char * ue_data = PQunescapeBytea((unsigned char*)data,&ue_len);

		_ret[i].length(ue_len);

		for( unsigned int j = 0; j < ue_len; j++)
		{
			_ret[i][j] = ue_data[j];
		}

		free(ue_data);
	}

	PQclear(result);

	return _ret._retn();
}
