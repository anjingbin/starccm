package org.postgresql.jdbc1;

// IMPORTANT NOTE: This file implements the JDBC 1 version of the driver.
// If you make any modifications to this file, you must make sure that the
// changes are also made (if relevent) to the related JDBC 2 class in the
// org.postgresql.jdbc2 package.

import java.sql.*;

import org.postgresql.util.PSQLException;

/*
 * A Statement object is used for executing a static SQL statement and
 * obtaining the results produced by it.
 *
 * <p>Only one ResultSet per Statement can be open at any point in time.
 * Therefore, if the reading of one ResultSet is interleaved with the
 * reading of another, each must have been generated by different
 * Statements.	All statement execute methods implicitly close a
 * statement's current ResultSet if an open one exists.
 *
 * @see java.sql.Statement
 * @see ResultSet
 */
public class Statement extends org.postgresql.Statement implements java.sql.Statement
{
	private Connection connection;		// The connection who created us

	/*
	 * Constructor for a Statement.  It simply sets the connection
	 * that created us.
	 *
	 * @param c the Connection instantation that creates us
	 */
	public Statement (Connection c)
	{
		connection = c;
	}

	/*
	 * Execute a SQL statement that retruns a single ResultSet
	 *
	 * @param sql typically a static SQL SELECT statement
	 * @return a ResulSet that contains the data produced by the query
	 * @exception SQLException if a database access error occurs
	 */
	public java.sql.ResultSet executeQuery(String sql) throws SQLException
	{
		this.execute(sql);
		while (result != null && !((org.postgresql.ResultSet)result).reallyResultSet())
			result = ((org.postgresql.ResultSet)result).getNext();
		if (result == null)
			throw new PSQLException("postgresql.stat.noresult");
		return result;
	}

	/*
	 * Execute a SQL INSERT, UPDATE or DELETE statement.  In addition
	 * SQL statements that return nothing such as SQL DDL statements
	 * can be executed
	 *
	 * @param sql a SQL statement
	 * @return either a row count, or 0 for SQL commands
	 * @exception SQLException if a database access error occurs
	 */
	public int executeUpdate(String sql) throws SQLException
	{
		this.execute(sql);
		if (((org.postgresql.ResultSet)result).reallyResultSet())
			throw new PSQLException("postgresql.stat.result");
		return this.getUpdateCount();
	}

	/*
	 * setCursorName defines the SQL cursor name that will be used by
	 * subsequent execute methods.	This name can then be used in SQL
	 * positioned update/delete statements to identify the current row
	 * in the ResultSet generated by this statement.  If a database
	 * doesn't support positioned update/delete, this method is a
	 * no-op.
	 *
	 * <p><B>Note:</B> By definition, positioned update/delete execution
	 * must be done by a different Statement than the one which
	 * generated the ResultSet being used for positioning.	Also, cursor
	 * names must be unique within a Connection.
	 *
	 * <p>We throw an additional constriction.	There can only be one
	 * cursor active at any one time.
	 *
	 * @param name the new cursor name
	 * @exception SQLException if a database access error occurs
	 */
	public void setCursorName(String name) throws SQLException
	{
		connection.setCursorName(name);
	}

	/*
	 * Execute a SQL statement that may return multiple results. We
	 * don't have to worry about this since we do not support multiple
	 * ResultSets.	 You can use getResultSet or getUpdateCount to
	 * retrieve the result.
	 *
	 * @param sql any SQL statement
	 * @return true if the next result is a ResulSet, false if it is
	 *	an update count or there are no more results
	 * @exception SQLException if a database access error occurs
	 */
	public boolean execute(String sql) throws SQLException
	{
		if (escapeProcessing)
			sql = escapeSQL(sql);
		result = connection.ExecSQL(sql);
		return (result != null && ((org.postgresql.ResultSet)result).reallyResultSet());
	}

	/*
	 * getUpdateCount returns the current result as an update count,
	 * if the result is a ResultSet or there are no more results, -1
	 * is returned.  It should only be called once per result.
	 *
	 * @return the current result as an update count.
	 * @exception SQLException if a database access error occurs
	 */
	public int getUpdateCount() throws SQLException
	{
		if (result == null)
			return -1;
		if (((org.postgresql.ResultSet)result).reallyResultSet())
			return -1;
		return ((org.postgresql.ResultSet)result).getResultCount();
	}

	/*
	 * getMoreResults moves to a Statement's next result.  If it returns
	 * true, this result is a ResulSet.
	 *
	 * @return true if the next ResultSet is valid
	 * @exception SQLException if a database access error occurs
	 */
	public boolean getMoreResults() throws SQLException
	{
		result = ((org.postgresql.ResultSet)result).getNext();
		return (result != null && ((org.postgresql.ResultSet)result).reallyResultSet());
	}
}