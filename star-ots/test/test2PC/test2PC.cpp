/*
 * testlibpqforlyc.c
 * This test programs shows to use LIBPQ to make multiple backend
 * connections
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libpq-fe.h"

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif


static void
exit_nicely(PGconn *conn)
{
	if (conn)
		PQfinish(conn);
	
	exit(1);
}

static void
check_conn(PGconn *conn, const char *dbName)
{
	/* check to see that the backend connection was successfully made */
	if (PQstatus(conn) == CONNECTION_BAD)
	{
		fprintf(stderr, "Connection to database '%s' failed.\n", dbName);
		fprintf(stderr, "%s", PQerrorMessage(conn));
		exit(1);
	}
}

int
main(int argc, char **argv)
{
	char	   *pghost,
			   *pgport,
			   *pgoptions,
			   *pgtty;
	char	   *dbName;
		
	int			nFields;
	int			i,
				j;

	PGconn	   *conn;


	/*
	 * PGresult   *res1, *res2;
	 */
	PGresult   *res;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s  dbName\n", argv[0]);
		exit(1);
	}

	dbName = argv[1];



	/*
	 * begin, by setting the parameters for a backend connection if the
	 * parameters are null, then the system will try to use reasonable
	 * defaults by looking up environment variables or, failing that,
	 * using hardwired constants
	 */
	pghost = "172.26.13.91";				/* host name of the backend server */
	pgport = "5678";				/* port of the backend server */
	pgoptions = NULL;			/* special options to start up the backend
								 * server */
	pgtty = NULL;				/* debugging tty for the backend server */

	/* make a connection to the database */
	conn = PQsetdb(pghost, pgport, pgoptions, pgtty, "bank");
	check_conn(conn, dbName);

	cout << "after check_conn" << endl; 

	/* start a transaction block */
	res = PQexec(conn, "BEGIN");
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		fprintf(stderr, "BEGIN command failed\n");
		PQclear(res);
		exit_nicely(conn);
	}

	/*
	 * should PQclear PGresult whenever it is no longer needed to avoid
	 * memory leaks
	 */
	PQclear(res);

	
	res = PQexec(conn, "update account set money=money+1;");
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		fprintf(stderr, "update money command failed\n");
		PQclear(res);
		exit_nicely(conn);
	}
	PQclear(res);

	res = PQexec(conn, "select * from account;");
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{

	PQclear(res);
		exit_nicely(conn);
	}

	/* first, print out the attribute names */
	nFields = PQnfields(res);
	for (i = 0; i < nFields; i++)
		printf("%-15s", PQfname(res, i));
	printf("\n");

	for (i = 0; i < nFields; i++)
		printf("---------     ");
	printf("\n\n");
	/* next, print out the instances */
	for (i = 0; i < PQntuples(res); i++)
	{
		for (j = 0; j < nFields; j++)
			printf("%-15s", PQgetvalue(res, i, j));
		printf("\n");
	}

	PQclear(res);

	res = PQexec(conn, "XAPREPARE");
	printf("the result is %d .\n",PQgetPrepare(res));
	PQclear(res);
	/* end the transaction */
	res = PQexec(conn, "XACOMMIT");
	PQclear(res);

	/* close the connection to the database and cleanup */
	PQfinish(conn);

	getchar();

/*	 fclose(debug); */
	return 0;					/* Though PQfinish(conn1) has called
								 * exit(1) */
}
