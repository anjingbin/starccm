/*-------------------------------------------------------------------------
 *
 * postgres.c
 *	  POSTGRES C Backend Interface
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/tcop/postgres.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 * NOTES
 *	  this is the "main" module of the postgres backend and
 *	  hence the main module of the "traffic cop".
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include "access/xlog.h"
#include "commands/async.h"
#include "commands/trigger.h"
#include "commands/variable.h"
#include "libpq/libpq.h"
#include "libpq/pqformat.h"
#include "libpq/pqsignal.h"
#include "miscadmin.h"
#include "nodes/print.h"
#include "optimizer/cost.h"
#include "optimizer/planner.h"
#include "parser/analyze.h"
#include "parser/parse.h"
#include "parser/parser.h"
#include "rewrite/rewriteHandler.h"
#include "tcop/fastpath.h"
#include "tcop/pquery.h"
#include "tcop/tcopprot.h"
#include "tcop/utility.h"
#include "storage/proc.h"
#include "utils/exc.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#ifdef MULTIBYTE
#include "mb/pg_wchar.h"
#endif

#include "pgstat.h"


/* ----------------
 *		global variables
 * ----------------
 */

extern int	optind;
extern char *optarg;

char	   *debug_query_string; /* used by pgmonitor */

/* Note: whereToSendOutput is initialized for the bootstrap/standalone case */
CommandDest whereToSendOutput = Debug;

static bool dontExecute = false;

/* note: these declarations had better match tcopprot.h */
sigjmp_buf	Warn_restart;

bool		Warn_restart_ready = false;
bool		InError = false;

static bool EchoQuery = false;	/* default don't echo */

/*
 * Flag to mark SIGHUP. Whenever the main loop comes around it
 * will reread the configuration file. (Better than doing the
 * reading in the signal handler, ey?)
 */
static volatile bool got_SIGHUP = false;

/* ----------------
 *		people who want to use EOF should #define DONTUSENEWLINE in
 *		tcop/tcopdebug.h
 * ----------------
 */
#ifndef TCOP_DONTUSENEWLINE
int			UseNewLine = 1;		/* Use newlines query delimiters (the
								 * default) */

#else
int			UseNewLine = 0;		/* Use EOF as query delimiters */
#endif   /* TCOP_DONTUSENEWLINE */

/*
** Flags for expensive function optimization -- JMH 3/9/92
*/
int			XfuncMode = 0;

/* ----------------------------------------------------------------
 *		decls for routines only used in this file
 * ----------------------------------------------------------------
 */
static int	InteractiveBackend(StringInfo inBuf);
static int	SocketBackend(StringInfo inBuf);
static int	ReadCommand(StringInfo inBuf);
static List *pg_parse_query(char *query_string, Oid *typev, int nargs);
static List *pg_analyze_and_rewrite(Node *parsetree);
static void start_xact_command(void);
static void finish_xact_command(void);
static void SigHupHandler(SIGNAL_ARGS);
static void FloatExceptionHandler(SIGNAL_ARGS);
static const char *CreateCommandTag(Node *parsetree);


/* ----------------------------------------------------------------
 *		routines to obtain user input
 * ----------------------------------------------------------------
 */

/* ----------------
 *	InteractiveBackend() is called for user interactive connections
 *	the string entered by the user is placed in its parameter inBuf.
 *
 *	EOF is returned if end-of-file input is seen; time to shut down.
 * ----------------
 */

static int
InteractiveBackend(StringInfo inBuf)
{
	int			c;				/* character read from getc() */
	bool		end = false;	/* end-of-input flag */
	bool		backslashSeen = false;	/* have we seen a \ ? */

	/*
	 * display a prompt and obtain input from the user
	 */
	printf("backend> ");
	fflush(stdout);

	/* Reset inBuf to empty */
	inBuf->len = 0;
	inBuf->data[0] = '\0';

	for (;;)
	{
		if (UseNewLine)
		{
			/*
			 * if we are using \n as a delimiter, then read characters
			 * until the \n.
			 */
			while ((c = getc(stdin)) != EOF)
			{
				if (c == '\n')
				{
					if (backslashSeen)
					{
						/* discard backslash from inBuf */
						inBuf->data[--inBuf->len] = '\0';
						backslashSeen = false;
						continue;
					}
					else
					{
						/* keep the newline character */
						appendStringInfoChar(inBuf, '\n');
						break;
					}
				}
				else if (c == '\\')
					backslashSeen = true;
				else
					backslashSeen = false;

				appendStringInfoChar(inBuf, (char) c);
			}

			if (c == EOF)
				end = true;
		}
		else
		{
			/*
			 * otherwise read characters until EOF.
			 */
			while ((c = getc(stdin)) != EOF)
				appendStringInfoChar(inBuf, (char) c);

			if (inBuf->len == 0)
				end = true;
		}

		if (end)
			return EOF;

		/*
		 * otherwise we have a user query so process it.
		 */
		break;
	}

	/*
	 * if the query echo flag was given, print the query..
	 */
	if (EchoQuery)
		printf("query: %s\n", inBuf->data);
	fflush(stdout);

	return 'Q';
}

/* ----------------
 *	SocketBackend()		Is called for frontend-backend connections
 *
 *	If the input is a query (case 'Q') then the string entered by
 *	the user is placed in its parameter inBuf.
 *
 *	If the input is a fastpath function call (case 'F') then
 *	the function call is processed in HandleFunctionRequest()
 *	(now called from PostgresMain()).
 *
 *	EOF is returned if the connection is lost.
 * ----------------
 */

static int
SocketBackend(StringInfo inBuf)
{
	int			qtype;

	/*
	 * get input from the frontend
	 */
	qtype = pq_getbyte();

	switch (qtype)
	{
		case EOF:
			/* frontend disconnected */
			break;

			/*
			 * 'Q': user entered a query
			 */
		case 'Q':
			if (pq_getstr(inBuf))
				return EOF;
			break;

			/*
			 * 'F':  calling user/system functions
			 */
		case 'F':
			if (pq_getstr(inBuf))
				return EOF;		/* ignore "string" at start of F message */
			break;

			/*
			 * 'X':  frontend is exiting
			 */
		case 'X':
			break;

			/*
			 * otherwise we got garbage from the frontend.
			 *
			 * XXX are we certain that we want to do an elog(FATAL) here?
			 * -cim 1/24/90
			 */
		default:
			elog(FATAL, "Socket command type %c unknown", qtype);
			break;
	}

	return qtype;
}

/* ----------------
 *		ReadCommand reads a command from either the frontend or
 *		standard input, places it in inBuf, and returns a char
 *		representing whether the string is a 'Q'uery or a 'F'astpath
 *		call.  EOF is returned if end of file.
 * ----------------
 */
static int
ReadCommand(StringInfo inBuf)
{
	int			result;

	if (IsUnderPostmaster)
		result = SocketBackend(inBuf);
	else
		result = InteractiveBackend(inBuf);
	return result;
}


/*
 * Parse a query string and pass it through the rewriter.
 *
 * A list of Query nodes is returned, since the string might contain
 * multiple queries and/or the rewriter might expand one query to several.
 *
 * NOTE: this routine is no longer used for processing interactive queries,
 * but it is still needed for parsing of SQL function bodies.
 */
List *
pg_parse_and_rewrite(char *query_string,		/* string to execute */
					 Oid *typev,	/* parameter types */
					 int nargs) /* number of parameters */
{
	List	   *raw_parsetree_list;
	List	   *querytree_list;
	List	   *list_item;

	/*
	 * (1) parse the request string into a list of raw parse trees.
	 */
	raw_parsetree_list = pg_parse_query(query_string, typev, nargs);

	/*
	 * (2) Do parse analysis and rule rewrite.
	 */
	querytree_list = NIL;
	foreach(list_item, raw_parsetree_list)
	{
		Node	   *parsetree = (Node *) lfirst(list_item);

		querytree_list = nconc(querytree_list,
							   pg_analyze_and_rewrite(parsetree));
	}

	return querytree_list;
}

/*
 * Do raw parsing (only).
 *
 * A list of parsetrees is returned, since there might be multiple
 * commands in the given string.
 *
 * NOTE: for interactive queries, it is important to keep this routine
 * separate from the analysis & rewrite stages.  Analysis and rewriting
 * cannot be done in an aborted transaction, since they require access to
 * database tables.  So, we rely on the raw parser to determine whether
 * we've seen a COMMIT or ABORT command; when we are in abort state, other
 * commands are not processed any further than the raw parse stage.
 */
static List *
pg_parse_query(char *query_string, Oid *typev, int nargs)
{
	List	   *raw_parsetree_list;

	if (Debug_print_query)
		elog(DEBUG, "query: %s", query_string);

	if (Show_parser_stats)
		ResetUsage();

	raw_parsetree_list = parser(query_string, typev, nargs);

	if (Show_parser_stats)
		ShowUsage("PARSER STATISTICS");

	return raw_parsetree_list;
}

/*
 * Given a raw parsetree (gram.y output), perform parse analysis and
 * rule rewriting.
 *
 * A list of Query nodes is returned, since either the analyzer or the
 * rewriter might expand one query to several.
 *
 * NOTE: for reasons mentioned above, this must be separate from raw parsing.
 */
static List *
pg_analyze_and_rewrite(Node *parsetree)
{
	List	   *querytree_list;
	List	   *list_item;
	Query	   *querytree;
	List	   *new_list;

	/*
	 * (1) Perform parse analysis.
	 */
	if (Show_parser_stats)
		ResetUsage();

	querytree_list = parse_analyze(parsetree, NULL);

	if (Show_parser_stats)
	{
		ShowUsage("PARSE ANALYSIS STATISTICS");
		ResetUsage();
	}

	/*
	 * (2) Rewrite the queries, as necessary
	 *
	 * rewritten queries are collected in new_list.  Note there may be more
	 * or fewer than in the original list.
	 */
	new_list = NIL;
	foreach(list_item, querytree_list)
	{
		querytree = (Query *) lfirst(list_item);

		if (Debug_print_parse)
		{
			if (Debug_pretty_print)
			{
				elog(DEBUG, "parse tree:");
				nodeDisplay(querytree);
			}
			else
				elog(DEBUG, "parse tree: %s", nodeToString(querytree));
		}

		if (querytree->commandType == CMD_UTILITY)
		{
			/* don't rewrite utilities, just dump 'em into new_list */
			new_list = lappend(new_list, querytree);
		}
		else
		{
			/* rewrite regular queries */
			List	   *rewritten = QueryRewrite(querytree);

			new_list = nconc(new_list, rewritten);
		}
	}

	querytree_list = new_list;

	if (Show_parser_stats)
		ShowUsage("REWRITER STATISTICS");

#ifdef COPY_PARSE_PLAN_TREES

	/*
	 * Optional debugging check: pass querytree output through
	 * copyObject()
	 */
	new_list = (List *) copyObject(querytree_list);
	/* This checks both copyObject() and the equal() routines... */
	if (!equal(new_list, querytree_list))
		elog(NOTICE, "pg_analyze_and_rewrite: copyObject failed on parse tree");
	else
		querytree_list = new_list;
#endif

	if (Debug_print_rewritten)
	{
		if (Debug_pretty_print)
		{
			elog(DEBUG, "rewritten parse tree:");
			foreach(list_item, querytree_list)
			{
				querytree = (Query *) lfirst(list_item);
				nodeDisplay(querytree);
				printf("\n");
			}
		}
		else
		{
			elog(DEBUG, "rewritten parse tree:");
			foreach(list_item, querytree_list)
			{
				querytree = (Query *) lfirst(list_item);
				elog(DEBUG, "%s", nodeToString(querytree));
			}
		}
	}

	return querytree_list;
}


/* Generate a plan for a single query. */
Plan *
pg_plan_query(Query *querytree)
{
	Plan	   *plan;

	/* Utility commands have no plans. */
	if (querytree->commandType == CMD_UTILITY)
		return NULL;

	if (Show_planner_stats)
		ResetUsage();

	/* call that optimizer */
	plan = planner(querytree);

	if (Show_planner_stats)
		ShowUsage("PLANNER STATISTICS");

#ifdef COPY_PARSE_PLAN_TREES
	/* Optional debugging check: pass plan output through copyObject() */
	{
		Plan	   *new_plan = (Plan *) copyObject(plan);

		/*
		 * equal() currently does not have routines to compare Plan nodes,
		 * so don't try to test equality here.  Perhaps fix someday?
		 */
#ifdef NOT_USED
		/* This checks both copyObject() and the equal() routines... */
		if (!equal(new_plan, plan))
			elog(NOTICE, "pg_plan_query: copyObject failed on plan tree");
		else
#endif
			plan = new_plan;
	}
#endif

	/*
	 * Print plan if debugging.
	 */
	if (Debug_print_plan)
	{
		if (Debug_pretty_print)
		{
			elog(DEBUG, "plan:");
			nodeDisplay(plan);
		}
		else
			elog(DEBUG, "plan: %s", nodeToString(plan));
	}

	return plan;
}


/* ----------------------------------------------------------------
 *		pg_exec_query_string()
 *
 *		Takes a querystring, runs the parser/utilities or
 *		parser/planner/executor over it as necessary.
 *
 * Assumptions:
 *
 * At call, we are not inside a transaction command.
 *
 * The CurrentMemoryContext after starting a transaction command must be
 * appropriate for execution of individual queries (typically this will be
 * TransactionCommandContext).	Note that this routine resets that context
 * after each individual query, so don't store anything there that
 * must outlive the call!
 *
 * parse_context references a context suitable for holding the
 * parse/rewrite trees (typically this will be QueryContext).
 * This context *must* be longer-lived than the transaction context!
 * In fact, if the query string might contain BEGIN/COMMIT commands,
 * parse_context had better outlive TopTransactionContext!
 *
 * We could have hard-wired knowledge about QueryContext and
 * TransactionCommandContext into this routine, but it seems better
 * not to, in case callers from outside this module need to use some
 * other contexts.
 *
 * ----------------------------------------------------------------
 */

void
pg_exec_query_string(char *query_string,		/* string to execute */
					 CommandDest dest,	/* where results should go */
					 MemoryContext parse_context)		/* context for
														 * parsetrees */
{
	bool		xact_started;
	MemoryContext oldcontext;
	List	   *parsetree_list,
			   *parsetree_item;

	debug_query_string = query_string;	/* used by pgmonitor */

	/*
	 * Start up a transaction command.	All queries generated by the
	 * query_string will be in this same command block, *unless* we find a
	 * BEGIN/COMMIT/ABORT statement; we have to force a new xact command
	 * after one of those, else bad things will happen in xact.c. (Note
	 * that this will possibly change current memory context.)
	 */
	start_xact_command();
	xact_started = true;

	/*
	 * parse_context *must* be different from the execution memory
	 * context, else the context reset at the bottom of the loop will
	 * destroy the parsetree list.	(We really ought to check that
	 * parse_context isn't a child of CurrentMemoryContext either, but
	 * that would take more cycles than it's likely to be worth.)
	 */
	Assert(parse_context != CurrentMemoryContext);

	/*
	 * Switch to appropriate context for constructing parsetrees.
	 */
	oldcontext = MemoryContextSwitchTo(parse_context);

	/*
	 * Do basic parsing of the query or queries (this should be safe even
	 * if we are in aborted transaction state!)
	 */
	parsetree_list = pg_parse_query(query_string, NULL, 0);

	/*
	 * Switch back to execution context to enter the loop.
	 */
	MemoryContextSwitchTo(oldcontext);

	/*
	 * Run through the parsetree(s) and process each one.
	 */
	foreach(parsetree_item, parsetree_list)
	{
		Node	   *parsetree = (Node *) lfirst(parsetree_item);
		bool		isTransactionStmt;
		const char *commandTag;
		char		completionTag[COMPLETION_TAG_BUFSIZE];
		List	   *querytree_list,
				   *querytree_item;

		/* Transaction control statements need some special handling */
		isTransactionStmt = IsA(parsetree, TransactionStmt);

		/*
		 * First we set the command-completion tag to the main query
		 * (as opposed to each of the others that may be generated by
		 * analyze and rewrite).  Also set ps_status to the main query tag.
		 */
		commandTag = CreateCommandTag(parsetree);

		set_ps_display(commandTag);

		/*
		 * If we are in an aborted transaction, ignore all commands except
		 * COMMIT/ABORT.  It is important that this test occur before we
		 * try to do parse analysis, rewrite, or planning, since all those
		 * phases try to do database accesses, which may fail in abort
		 * state. (It might be safe to allow some additional utility
		 * commands in this state, but not many...)
		 */
		if (IsAbortedTransactionBlockState())
		{
			bool		allowit = false;

			if (isTransactionStmt)
			{
				TransactionStmt *stmt = (TransactionStmt *) parsetree;

				switch (stmt->command)
				{
					case COMMIT:
					case ROLLBACK:
					//renyi 11 23 : added
					case XACOMMIT_TRANS:
					case XAROLLBACK_TRANS:
						allowit = true;
						break;
					default:
						break;
				}
			}

			if (!allowit)
			{
				elog(NOTICE, "current transaction is aborted, "
					 "queries ignored until end of transaction block");

				/*
				 * We need to emit a command-complete report to the client,
				 * even though we didn't process the query.
				 * - cim 6/1/90
				 */
				commandTag = "*ABORT STATE*";

				EndCommand(commandTag, dest);

				/*
				 * We continue in the loop, on the off chance that there
				 * is a COMMIT or ROLLBACK utility command later in the
				 * query string.
				 */
				continue;
			}
		}

		/* Make sure we are in a transaction command */
		if (!xact_started)
		{
			start_xact_command();
			xact_started = true;
		}

		/* If we got a cancel signal in parsing or prior command, quit */
		CHECK_FOR_INTERRUPTS();

		/*
		 * OK to analyze and rewrite this query.
		 *
		 * Switch to appropriate context for constructing querytrees (again,
		 * these must outlive the execution context).
		 */
		oldcontext = MemoryContextSwitchTo(parse_context);

		querytree_list = pg_analyze_and_rewrite(parsetree);

		/*
		 * Switch back to execution context for planning and execution.
		 */
		MemoryContextSwitchTo(oldcontext);

		/*
		 * Inner loop handles the individual queries generated from a
		 * single parsetree by analysis and rewrite.
		 */
		foreach(querytree_item, querytree_list)
		{
			Query	   *querytree = (Query *) lfirst(querytree_item);

			/* Make sure we are in a transaction command */
			if (!xact_started)
			{
				start_xact_command();
				xact_started = true;
			}

			/*
			 * If we got a cancel signal in analysis or prior command,
			 * quit
			 */
			CHECK_FOR_INTERRUPTS();

			if (querytree->commandType == CMD_UTILITY)
			{
				/*
				 * process utility functions (create, destroy, etc..)
				 */
				if (Debug_print_query)
					elog(DEBUG, "ProcessUtility: %s", query_string);
				else if (DebugLvl > 1)
					elog(DEBUG, "ProcessUtility");

				if (querytree->originalQuery)
				{
					/* utility statement can override default tag string */
					int i = 2;//renyi 11 24
					ProcessUtility(querytree->utilityStmt, dest,
								   completionTag,&i);//renyi 11 24
					if (completionTag[0])
						commandTag = completionTag;
					//renyi 11 24 : added
					switch (i) 
					{
						case -1 :
							commandTag = "ABORT";
							break;
						case 0:
							commandTag = "READONLY";
							break;
						case 1:
							commandTag = "COMMIT";
							break;
						default:
							break;
					}
				}
				else
				{
					/* utility added by rewrite cannot override tag */
					int i = 2;//renyi 11 24
					ProcessUtility(querytree->utilityStmt, dest, NULL,&i);//renyi 11 24
					//renyi 11 24: added
					switch (i) 
					{
						case -1 :
							commandTag = "ABORT";
							break;
						case 0:
							commandTag = "READONLY";
							break;
						case 1:
							commandTag = "COMMIT";
							break;
						default:
							break;
					}
				}
			}
			else
			{
				/*
				 * process a plannable query.
				 */
				Plan	   *plan;

				plan = pg_plan_query(querytree);

				/* if we got a cancel signal whilst planning, quit */
				CHECK_FOR_INTERRUPTS();

				/* Initialize snapshot state for query */
				SetQuerySnapshot();

				/*
				 * execute the plan
				 */
				if (Show_executor_stats)
					ResetUsage();

				if (dontExecute)
				{
					/* don't execute it, just show the query plan */
					print_plan(plan, querytree);
				}
				else
				{
					if (DebugLvl > 1)
						elog(DEBUG, "ProcessQuery");

					if (querytree->originalQuery)
					{
						/* original stmt can override default tag string */
						ProcessQuery(querytree, plan, dest, completionTag);
						commandTag = completionTag;
					}
					else
					{
						/* stmt added by rewrite cannot override tag */
						ProcessQuery(querytree, plan, dest, NULL);
					}
				}

				if (Show_executor_stats)
					ShowUsage("EXECUTOR STATISTICS");
			}

			/*
			 * In a query block, we want to increment the command counter
			 * between queries so that the effects of early queries are
			 * visible to subsequent ones.	In particular we'd better do
			 * so before checking constraints.
			 */
			if (!isTransactionStmt)
				CommandCounterIncrement();

			/*
			 * Clear the execution context to recover temporary memory
			 * used by the query.  NOTE: if query string contains
			 * BEGIN/COMMIT transaction commands, execution context may
			 * now be different from what we were originally passed; so be
			 * careful to clear current context not "oldcontext".
			 */
			Assert(parse_context != CurrentMemoryContext);

			MemoryContextResetAndDeleteChildren(CurrentMemoryContext);

			/*
			 * If this was a transaction control statement, commit it and
			 * arrange to start a new xact command for the next command
			 * (if any).
			 */
			if (isTransactionStmt)
			{
				finish_xact_command();
				xact_started = false;
			}

		}						/* end loop over queries generated from a
								 * parsetree */

		/*
		 * If this is the last parsetree of the query string, close down
		 * transaction statement before reporting command-complete.  This is
		 * so that any end-of-transaction errors are reported before the
		 * command-complete message is issued, to avoid confusing clients
		 * who will expect either a command-complete message or an error,
		 * not one and then the other.  But for compatibility with
		 * historical Postgres behavior, we do not force a transaction
		 * boundary between queries appearing in a single query string.
		 */
		if (lnext(parsetree_item) == NIL && xact_started)
		{
			finish_xact_command();
			xact_started = false;
		}

		/*
		 * It is possible that the original query was removed due to
		 * a DO INSTEAD rewrite rule.  In that case we will still have
		 * the default completion tag, which is fine for most purposes,
		 * but it may confuse clients if it's INSERT/UPDATE/DELETE.
		 * Clients expect those tags to have counts after them (cf.
		 * ProcessQuery).
		 */
		if (strcmp(commandTag, "INSERT") == 0)
			commandTag = "INSERT 0 0";
		else if (strcmp(commandTag, "UPDATE") == 0)
			commandTag = "UPDATE 0";
		else if (strcmp(commandTag, "DELETE") == 0)
			commandTag = "DELETE 0";

		/*
		 * Tell client that we're done with this query.  Note we emit
		 * exactly one EndCommand report for each raw parsetree, thus
		 * one for each SQL command the client sent, regardless of
		 * rewriting.  (But a command aborted by error will not send
		 * an EndCommand report at all.)
		 */
		EndCommand(commandTag, dest);
	}							/* end loop over parsetrees */

	/*
	 * Close down transaction statement, if one is open.
	 * (Note that this will only happen if the querystring was empty.)
	 */
	if (xact_started)
		finish_xact_command();

	debug_query_string = NULL;	/* used by pgmonitor */
}

/*
 * Convenience routines for starting/committing a single command.
 */
static void
start_xact_command(void)
{
	if (DebugLvl >= 1)
		elog(DEBUG, "StartTransactionCommand");
	StartTransactionCommand();
}

static void
finish_xact_command(void)
{
	/* Invoke IMMEDIATE constraint triggers */
	DeferredTriggerEndQuery();

	/* Now commit the command */
	if (DebugLvl >= 1)
		elog(DEBUG, "CommitTransactionCommand");

	CommitTransactionCommand();

#ifdef SHOW_MEMORY_STATS
	/* Print mem stats at each commit for leak tracking */
	if (ShowStats)
		MemoryContextStats(TopMemoryContext);
#endif
}


/* --------------------------------
 *		signal handler routines used in PostgresMain()
 * --------------------------------
 */

/*
 * quickdie() occurs when signalled SIGQUIT by the postmaster.
 *
 * Some backend has bought the farm,
 * so we need to stop what we're doing and exit.
 */
void
quickdie(SIGNAL_ARGS)
{
	PG_SETMASK(&BlockSig);
	elog(NOTICE, "Message from PostgreSQL backend:"
		 "\n\tThe Postmaster has informed me that some other backend"
		 "\n\tdied abnormally and possibly corrupted shared memory."
		 "\n\tI have rolled back the current transaction and am"
	   "\n\tgoing to terminate your database system connection and exit."
	"\n\tPlease reconnect to the database system and repeat your query.");

	/*
	 * DO NOT proc_exit() -- we're here because shared memory may be
	 * corrupted, so we don't want to try to clean up our transaction.
	 * Just nail the windows shut and get out of town.
	 *
	 * Note we do exit(1) not exit(0).	This is to force the postmaster into
	 * a system reset cycle if some idiot DBA sends a manual SIGQUIT to a
	 * random backend.	This is necessary precisely because we don't clean
	 * up our shared memory state.
	 */

	exit(1);
}

/*
 * Shutdown signal from postmaster: abort transaction and exit
 * at soonest convenient time
 */
void
die(SIGNAL_ARGS)
{
	int			save_errno = errno;

	/* Don't joggle the elbow of proc_exit */
	if (!proc_exit_inprogress)
	{
		InterruptPending = true;
		ProcDiePending = true;

		/*
		 * If it's safe to interrupt, and we're waiting for input or a
		 * lock, service the interrupt immediately
		 */
		if (ImmediateInterruptOK && InterruptHoldoffCount == 0 &&
			CritSectionCount == 0)
		{
			/* bump holdoff count to make ProcessInterrupts() a no-op */
			/* until we are done getting ready for it */
			InterruptHoldoffCount++;
			DisableNotifyInterrupt();
			/* Make sure HandleDeadLock won't run while shutting down... */
			LockWaitCancel();
			InterruptHoldoffCount--;
			ProcessInterrupts();
		}
	}

	errno = save_errno;
}

/*
 * Timeout or shutdown signal from postmaster during client authentication.
 * Simply exit(0).
 *
 * XXX: possible future improvement: try to send a message indicating
 * why we are disconnecting.  Problem is to be sure we don't block while
 * doing so, nor mess up the authentication message exchange.
 */
void
authdie(SIGNAL_ARGS)
{
	exit(0);
}

/*
 * Query-cancel signal from postmaster: abort current transaction
 * at soonest convenient time
 */
static void
QueryCancelHandler(SIGNAL_ARGS)
{
	int			save_errno = errno;

	/*
	 * Don't joggle the elbow of proc_exit, nor an already-in-progress
	 * abort
	 */
	if (!proc_exit_inprogress && !InError)
	{
		InterruptPending = true;
		QueryCancelPending = true;

		/*
		 * If it's safe to interrupt, and we're waiting for a lock,
		 * service the interrupt immediately.  No point in interrupting if
		 * we're waiting for input, however.
		 */
		if (ImmediateInterruptOK && InterruptHoldoffCount == 0 &&
			CritSectionCount == 0)
		{
			/* bump holdoff count to make ProcessInterrupts() a no-op */
			/* until we are done getting ready for it */
			InterruptHoldoffCount++;
			if (LockWaitCancel())
			{
				DisableNotifyInterrupt();
				InterruptHoldoffCount--;
				ProcessInterrupts();
			}
			else
				InterruptHoldoffCount--;
		}
	}

	errno = save_errno;
}

/* signal handler for floating point exception */
static void
FloatExceptionHandler(SIGNAL_ARGS)
{
	elog(ERROR, "floating point exception!"
		 " The last floating point operation either exceeded legal ranges"
		 " or was a divide by zero");
}

/* SIGHUP: set flag to re-read config file at next convenient time */
static void
SigHupHandler(SIGNAL_ARGS)
{
	got_SIGHUP = true;
}


/*
 * ProcessInterrupts: out-of-line portion of CHECK_FOR_INTERRUPTS() macro
 *
 * If an interrupt condition is pending, and it's safe to service it,
 * then clear the flag and accept the interrupt.  Called only when
 * InterruptPending is true.
 */
void
ProcessInterrupts(void)
{
	/* OK to accept interrupt now? */
	if (InterruptHoldoffCount != 0 || CritSectionCount != 0)
		return;
	InterruptPending = false;
	if (ProcDiePending)
	{
		ProcDiePending = false;
		QueryCancelPending = false;		/* ProcDie trumps QueryCancel */
		ImmediateInterruptOK = false;	/* not idle anymore */
		elog(FATAL, "This connection has been terminated by the administrator.");
	}
	if (QueryCancelPending)
	{
		QueryCancelPending = false;
		ImmediateInterruptOK = false;	/* not idle anymore */
		elog(ERROR, "Query was cancelled.");
	}
	/* If we get here, do nothing (probably, QueryCancelPending was reset) */
}


static void
usage(char *progname)
{
	printf("%s is the PostgreSQL stand-alone backend.  It is not\nintended to be used by normal users.\n\n", progname);

	printf("Usage:\n  %s [options...] [dbname]\n\n", progname);
	printf("Options:\n");
#ifdef USE_ASSERT_CHECKING
	printf("  -A 1|0          enable/disable run-time assert checking\n");
#endif
	printf("  -B NBUFFERS     number of shared buffers (default %d)\n", DEF_NBUFFERS);
	printf("  -c NAME=VALUE   set run-time parameter\n");
	printf("  -d 1-5          debugging level\n");
	printf("  -D DATADIR      database directory\n");
	printf("  -e              use European date format\n");
	printf("  -E              echo query before execution\n");
	printf("  -F              turn fsync off\n");
	printf("  -N              do not use newline as interactive query delimiter\n");
	printf("  -o FILENAME     send stdout and stderr to given file\n");
	printf("  -P              disable system indexes\n");
	printf("  -s              show statistics after each query\n");
	printf("  -S SORT-MEM     set amount of memory for sorts (in kbytes)\n");
	printf("Developer options:\n");
	printf("  -f [s|i|n|m|h]  forbid use of some plan types\n");
	printf("  -i              do not execute queries\n");
	printf("  -O              allow system table structure changes\n");
	printf("  -t [pa|pl|ex]   show timings after each query\n");
	printf("  -W NUM          wait NUM seconds to allow attach from a debugger\n");
	printf("\nReport bugs to <pgsql-bugs@postgresql.org>.\n");
}



/* ----------------------------------------------------------------
 * PostgresMain
 *	   postgres main loop -- all backends, interactive or otherwise start here
 *
 * argc/argv are the command line arguments to be used.  (When being forked
 * by the postmaster, these are not the original argv array of the process.)
 * username is the (possibly authenticated) PostgreSQL user name to be used
 * for the session.
 * ----------------------------------------------------------------
 */
int
PostgresMain(int argc, char *argv[], const char *username)
{
	int			flag;

	const char *DBName = NULL;
	bool		secure;
	int			errs = 0;
	GucContext	ctx;
	char	   *tmp;

	int			firstchar;
	StringInfo	parser_input;

	char	   *potential_DataDir = NULL;

	/*
	 * Catch standard options before doing much else.  This even works on
	 * systems without getopt_long.
	 */
	if (!IsUnderPostmaster && argc > 1)
	{
		if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0)
		{
			usage(argv[0]);
			exit(0);
		}
		if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0)
		{
			puts("postgres (PostgreSQL) " PG_VERSION);
			exit(0);
		}
	}

	/*
	 * Fire up essential subsystems: error and memory management
	 *
	 * If we are running under the postmaster, this is done already.
	 */
	if (!IsUnderPostmaster)
	{
		EnableExceptionHandling(true);
		MemoryContextInit();
	}

	set_ps_display("startup");

	SetProcessingMode(InitProcessing);

	/*
	 * Set default values for command-line options.
	 */
	Noversion = false;
	EchoQuery = false;

	if (!IsUnderPostmaster)
	{
		ResetAllOptions(true);
		potential_DataDir = getenv("PGDATA");
	}

	/* Check for PGDATESTYLE environment variable */
	set_default_datestyle();

	/* ----------------
	 *	parse command line arguments
	 *
	 *	There are now two styles of command line layout for the backend:
	 *
	 *	For interactive use (not started from postmaster) the format is
	 *		postgres [switches] [databasename]
	 *	If the databasename is omitted it is taken to be the user name.
	 *
	 *	When started from the postmaster, the format is
	 *		postgres [secure switches] -p databasename [insecure switches]
	 *	Switches appearing after -p came from the client (via "options"
	 *	field of connection request).  For security reasons we restrict
	 *	what these switches can do.
	 * ----------------
	 */

	/* all options are allowed until '-p' */
	secure = true;
	ctx = PGC_POSTMASTER;

	while ((flag = getopt(argc, argv, "A:B:c:CD:d:Eef:FiNOPo:p:S:st:v:W:x:-:")) != -1)
		switch (flag)
		{
			case 'A':
#ifdef USE_ASSERT_CHECKING
				SetConfigOption("debug_assertions", optarg, ctx, true);
#else
				elog(NOTICE, "Assert checking is not compiled in");
#endif
				break;

			case 'B':

				/*
				 * specify the size of buffer pool
				 */
				SetConfigOption("shared_buffers", optarg, ctx, true);
				break;

			case 'C':

				/*
				 * don't print version string
				 */
				Noversion = true;
				break;

			case 'D':			/* PGDATA directory */
				if (secure)
					potential_DataDir = optarg;
				break;

			case 'd':			/* debug level */
				SetConfigOption("debug_level", optarg, ctx, true);
				if (DebugLvl >= 1)
					SetConfigOption("log_connections", "true", ctx, true);
				if (DebugLvl >= 2)
					SetConfigOption("debug_print_query", "true", ctx, true);
				if (DebugLvl >= 3)
					SetConfigOption("debug_print_parse", "true", ctx, true);
				if (DebugLvl >= 4)
					SetConfigOption("debug_print_plan", "true", ctx, true);
				if (DebugLvl >= 5)
					SetConfigOption("debug_print_rewritten", "true", ctx, true);
				break;

			case 'E':

				/*
				 * E - echo the query the user entered
				 */
				EchoQuery = true;
				break;

			case 'e':

				/*
				 * Use european date formats.
				 */
				EuroDates = true;
				break;

			case 'F':

				/*
				 * turn off fsync
				 */
				SetConfigOption("fsync", "false", ctx, true);
				break;

			case 'f':

				/*
				 * f - forbid generation of certain plans
				 */
				tmp = NULL;
				switch (optarg[0])
				{
					case 's':	/* seqscan */
						tmp = "enable_seqscan";
						break;
					case 'i':	/* indexscan */
						tmp = "enable_indexscan";
						break;
					case 't':	/* tidscan */
						tmp = "enable_tidscan";
						break;
					case 'n':	/* nestloop */
						tmp = "enable_nestloop";
						break;
					case 'm':	/* mergejoin */
						tmp = "enable_mergejoin";
						break;
					case 'h':	/* hashjoin */
						tmp = "enable_hashjoin";
						break;
					default:
						errs++;
				}
				if (tmp)
					SetConfigOption(tmp, "false", ctx, true);
				break;

			case 'i':
				dontExecute = true;
				break;

			case 'N':

				/*
				 * N - Don't use newline as a query delimiter
				 */
				UseNewLine = 0;
				break;

			case 'O':

				/*
				 * allow system table structure modifications
				 */
				if (secure)		/* XXX safe to allow from client??? */
					allowSystemTableMods = true;
				break;

			case 'P':

				/*
				 * ignore system indexes
				 */
				if (secure)		/* XXX safe to allow from client??? */
					IgnoreSystemIndexes(true);
				break;

			case 'o':

				/*
				 * o - send output (stdout and stderr) to the given file
				 */
				if (secure)
					StrNCpy(OutputFileName, optarg, MAXPGPATH);
				break;

			case 'p':

				/*
				 * p - special flag passed if backend was forked by a
				 * postmaster.
				 */
				if (secure)
				{
					DBName = strdup(optarg);
					secure = false;		/* subsequent switches are NOT
										 * secure */
					ctx = PGC_BACKEND;
				}
				break;

			case 'S':

				/*
				 * S - amount of sort memory to use in 1k bytes
				 */
				SetConfigOption("sort_mem", optarg, ctx, true);
				break;

			case 's':

				/*
				 * s - report usage statistics (timings) after each query
				 */
				SetConfigOption("show_query_stats", "true", ctx, true);
				break;

			case 't':
				/* ---------------
				 *	tell postgres to report usage statistics (timings) for
				 *	each query
				 *
				 *	-tpa[rser] = print stats for parser time of each query
				 *	-tpl[anner] = print stats for planner time of each query
				 *	-te[xecutor] = print stats for executor time of each query
				 *	caution: -s can not be used together with -t.
				 * ----------------
				 */
				tmp = NULL;
				switch (optarg[0])
				{
					case 'p':
						if (optarg[1] == 'a')
							tmp = "show_parser_stats";
						else if (optarg[1] == 'l')
							tmp = "show_planner_stats";
						else
							errs++;
						break;
					case 'e':
						tmp = "show_executor_stats";
						break;
					default:
						errs++;
						break;
				}
				if (tmp)
					SetConfigOption(tmp, "true", ctx, true);
				break;

			case 'v':
				if (secure)
					FrontendProtocol = (ProtocolVersion) atoi(optarg);
				break;

			case 'W':

				/*
				 * wait N seconds to allow attach from a debugger
				 */
				sleep(atoi(optarg));
				break;

			case 'x':
#ifdef NOT_USED					/* planner/xfunc.h */

				/*
				 * control joey hellerstein's expensive function
				 * optimization
				 */
				if (XfuncMode != 0)
				{
					elog(NOTICE, "only one -x flag is allowed");
					errs++;
					break;
				}
				if (strcmp(optarg, "off") == 0)
					XfuncMode = XFUNC_OFF;
				else if (strcmp(optarg, "nor") == 0)
					XfuncMode = XFUNC_NOR;
				else if (strcmp(optarg, "nopull") == 0)
					XfuncMode = XFUNC_NOPULL;
				else if (strcmp(optarg, "nopm") == 0)
					XfuncMode = XFUNC_NOPM;
				else if (strcmp(optarg, "pullall") == 0)
					XfuncMode = XFUNC_PULLALL;
				else if (strcmp(optarg, "wait") == 0)
					XfuncMode = XFUNC_WAIT;
				else
				{
					elog(NOTICE, "use -x {off,nor,nopull,nopm,pullall,wait}");
					errs++;
				}
#endif
				break;

			case 'c':
			case '-':
				{
					char	   *name,
							   *value;

					ParseLongOption(optarg, &name, &value);
					if (!value)
					{
						if (flag == '-')
							elog(ERROR, "--%s requires argument", optarg);
						else
							elog(ERROR, "-c %s requires argument", optarg);
					}

					SetConfigOption(name, value, ctx, true);
					free(name);
					if (value)
						free(value);
					break;
				}

			default:
				errs++;
				break;
		}

	/*
	 * Post-processing for command line options.
	 */
	if (Show_query_stats &&
		(Show_parser_stats || Show_planner_stats || Show_executor_stats))
	{
		elog(NOTICE, "Query statistics are disabled because parser, planner, or executor statistics are on.");
		SetConfigOption("show_query_stats", "false", ctx, true);
	}

	if (!IsUnderPostmaster)
	{
		if (!potential_DataDir)
		{
			fprintf(stderr, "%s does not know where to find the database system "
			   "data.  You must specify the directory that contains the "
				"database system either by specifying the -D invocation "
			 "option or by setting the PGDATA environment variable.\n\n",
					argv[0]);
			proc_exit(1);
		}
		SetDataDir(potential_DataDir);
	}
	Assert(DataDir);

	/*
	 * Set up signal handlers and masks.
	 *
	 * Note that postmaster blocked all signals before forking child process,
	 * so there is no race condition whereby we might receive a signal
	 * before we have set up the handler.
	 *
	 * Also note: it's best not to use any signals that are SIG_IGNored in
	 * the postmaster.	If such a signal arrives before we are able to
	 * change the handler to non-SIG_IGN, it'll get dropped.  If
	 * necessary, make a dummy handler in the postmaster to reserve the
	 * signal.
	 */

	pqsignal(SIGHUP, SigHupHandler);	/* set flag to read config file */
	pqsignal(SIGINT, QueryCancelHandler);		/* cancel current query */
	pqsignal(SIGTERM, die);		/* cancel current query and exit */
	pqsignal(SIGQUIT, quickdie);	/* hard crash time */
	pqsignal(SIGALRM, HandleDeadLock);	/* check for deadlock after
										 * timeout */

	/*
	 * Ignore failure to write to frontend. Note: if frontend closes
	 * connection, we will notice it and exit cleanly when control next
	 * returns to outer loop.  This seems safer than forcing exit in the
	 * midst of output during who-knows-what operation...
	 */
	pqsignal(SIGPIPE, SIG_IGN);
	pqsignal(SIGUSR1, SIG_IGN); /* this signal available for use */

	pqsignal(SIGUSR2, Async_NotifyHandler);		/* flush also sinval cache */
	pqsignal(SIGFPE, FloatExceptionHandler);

	/*
	 * Reset some signals that are accepted by postmaster but not by
	 * backend
	 */
	pqsignal(SIGCHLD, SIG_DFL); /* system() requires this on some
								 * platforms */
	pqsignal(SIGTTIN, SIG_DFL);
	pqsignal(SIGTTOU, SIG_DFL);
	pqsignal(SIGCONT, SIG_DFL);
	pqsignal(SIGWINCH, SIG_DFL);

	pqinitmask();

	/* We allow SIGQUIT (quickdie) at all times */
#ifdef HAVE_SIGPROCMASK
	sigdelset(&BlockSig, SIGQUIT);
#else
	BlockSig &= ~(sigmask(SIGQUIT));
#endif

	PG_SETMASK(&BlockSig);		/* block everything except SIGQUIT */


	if (IsUnderPostmaster)
	{
		/* noninteractive case: nothing should be left after switches */
		if (errs || argc != optind || DBName == NULL)
		{
			elog(NOTICE, "%s: invalid command line arguments\nTry -? for help.",
				 argv[0]);
			proc_exit(0);		/* not 1, that causes system-wide
								 * restart... */
		}
		BaseInit();
	}
	else
	{
		/* interactive case: database name can be last arg on command line */
		if (errs || argc - optind > 1)
		{
			elog(NOTICE, "%s: invalid command line arguments\nTry -? for help.",
				 argv[0]);
			proc_exit(1);
		}
		else if (argc - optind == 1)
			DBName = argv[optind];
		else if ((DBName = username) == NULL)
		{
			elog(NOTICE, "%s: user name undefined and no database specified\n",
				 argv[0]);
			proc_exit(1);
		}

		/*
		 * On some systems our dynloader code needs the executable's
		 * pathname.  (If under postmaster, this was done already.)
		 */
		if (FindExec(pg_pathname, argv[0], "postgres") < 0)
			elog(FATAL, "%s: could not locate executable, bailing out...",
				 argv[0]);

		/*
		 * Validate we have been given a reasonable-looking DataDir (if
		 * under postmaster, assume postmaster did this already).
		 */
		ValidatePgVersion(DataDir);

		/*
		 * Create lockfile for data directory.
		 */
		if (!CreateDataDirLockFile(DataDir, false))
			proc_exit(1);

		XLOGPathInit();
		BaseInit();

		/*
		 * Start up xlog for standalone backend, and register to have it
		 * closed down at exit.
		 */
		StartupXLOG();
		on_shmem_exit(ShutdownXLOG, 0);
	}

	/*
	 * Set up additional info.
	 */

#ifdef CYR_RECODE
	SetCharSet();
#endif

	/*
	 * General initialization.
	 *
	 * NOTE: if you are tempted to add code in this vicinity, consider
	 * putting it inside InitPostgres() instead.  In particular, anything
	 * that involves database access should be there, not here.
	 */
	if (DebugLvl > 1)
		elog(DEBUG, "InitPostgres");
	InitPostgres(DBName, username);

	SetProcessingMode(NormalProcessing);

	/*
	 * Send this backend's cancellation info to the frontend.
	 */
	if (whereToSendOutput == Remote &&
		PG_PROTOCOL_MAJOR(FrontendProtocol) >= 2)
	{
		StringInfoData buf;

		pq_beginmessage(&buf);
		pq_sendbyte(&buf, 'K');
		pq_sendint(&buf, (int32) MyProcPid, sizeof(int32));
		pq_sendint(&buf, (int32) MyCancelKey, sizeof(int32));
		pq_endmessage(&buf);
		/* Need not flush since ReadyForQuery will do it. */
	}

	if (!IsUnderPostmaster)
	{
		puts("\nPOSTGRES backend interactive interface ");
		puts("$Revision: 1.2 $ $Date: 2004/07/05 08:55:39 $\n");
	}

	/*
	 * Create the memory context we will use in the main loop.
	 *
	 * QueryContext is reset once per iteration of the main loop, ie, upon
	 * completion of processing of each supplied query string. It can
	 * therefore be used for any data that should live just as long as the
	 * query string --- parse trees, for example.
	 */
	QueryContext = AllocSetContextCreate(TopMemoryContext,
										 "QueryContext",
										 ALLOCSET_DEFAULT_MINSIZE,
										 ALLOCSET_DEFAULT_INITSIZE,
										 ALLOCSET_DEFAULT_MAXSIZE);

	/* ----------
	 * Tell the statistics collector that we're alive and
	 * to which database we belong.
	 * ----------
	 */
	pgstat_bestart();

	/*
	 * POSTGRES main processing loop begins here
	 *
	 * If an exception is encountered, processing resumes here so we abort
	 * the current transaction and start a new one.
	 */

	if (sigsetjmp(Warn_restart, 1) != 0)
	{
		/*
		 * NOTE: if you are tempted to add more code in this if-block,
		 * consider the probability that it should be in
		 * AbortTransaction() instead.
		 *
		 * Make sure we're not interrupted while cleaning up.  Also forget
		 * any pending QueryCancel request, since we're aborting anyway.
		 * Force InterruptHoldoffCount to a known state in case we elog'd
		 * from inside a holdoff section.
		 */
		ImmediateInterruptOK = false;
		QueryCancelPending = false;
		InterruptHoldoffCount = 1;
		CritSectionCount = 0;	/* should be unnecessary, but... */
		debug_query_string = NULL;		/* used by pgmonitor */

		/*
		 * Make sure we are in a valid memory context during recovery.
		 *
		 * We use ErrorContext in hopes that it will have some free space
		 * even if we're otherwise up against it...
		 */
		MemoryContextSwitchTo(ErrorContext);

		/* Do the recovery */
		if (DebugLvl >= 1)
			elog(DEBUG, "AbortCurrentTransaction");
		AbortCurrentTransaction();

		/*
		 * Now return to normal top-level context and clear ErrorContext
		 * for next time.
		 */
		MemoryContextSwitchTo(TopMemoryContext);
		MemoryContextResetAndDeleteChildren(ErrorContext);

		/*
		 * Clear flag to indicate that we got out of error recovery mode
		 * successfully.  (Flag was set in elog.c before longjmp().)
		 */
		InError = false;

		/*
		 * Exit interrupt holdoff section we implicitly established above.
		 */
		RESUME_INTERRUPTS();
	}

	Warn_restart_ready = true;	/* we can now handle elog(ERROR) */

	PG_SETMASK(&UnBlockSig);

	/*
	 * Non-error queries loop here.
	 */

	for (;;)
	{
		/*
		 * Release storage left over from prior query cycle, and create a
		 * new query input buffer in the cleared QueryContext.
		 */
		MemoryContextSwitchTo(QueryContext);
		MemoryContextResetAndDeleteChildren(QueryContext);

		parser_input = makeStringInfo();

		/*
		 * (1) tell the frontend we're ready for a new query.
		 *
		 * Note: this includes fflush()'ing the last of the prior output.
		 */
		ReadyForQuery(whereToSendOutput);

		/* ----------
		 * Tell the statistics collector what we've collected
		 * so far.
		 * ----------
		 */
		pgstat_report_tabstat();

		if (IsTransactionBlock())
		{
			set_ps_display("idle in transaction");
			pgstat_report_activity("<IDLE> in transaction");
		}
		else
		{
			set_ps_display("idle");
			pgstat_report_activity("<IDLE>");
		}

		/*
		 * (2) deal with pending asynchronous NOTIFY from other backends,
		 * and enable async.c's signal handler to execute NOTIFY directly.
		 * Then set up other stuff needed before blocking for input.
		 */
		QueryCancelPending = false;		/* forget any earlier CANCEL
										 * signal */

		EnableNotifyInterrupt();

		/* Allow "die" interrupt to be processed while waiting */
		ImmediateInterruptOK = true;
		/* and don't forget to detect one that already arrived */
		QueryCancelPending = false;
		CHECK_FOR_INTERRUPTS();

		/*
		 * (3) read a command (loop blocks here)
		 */
		firstchar = ReadCommand(parser_input);

		/*
		 * (4) disable async signal conditions again.
		 */
		ImmediateInterruptOK = false;
		QueryCancelPending = false;		/* forget any CANCEL signal */

		DisableNotifyInterrupt();

		/*
		 * (5) check for any other interesting events that happened while
		 * we slept.
		 */
		if (got_SIGHUP)
		{
			got_SIGHUP = false;
			ProcessConfigFile(PGC_SIGHUP);
		}

		/*
		 * (6) process the command.
		 */
		switch (firstchar)
		{
				/*
				 * 'F' indicates a fastpath call.
				 */
			case 'F':
				/* ----------
				 * Tell the collector what we're doing
				 * ----------
				 */
				pgstat_report_activity("<FASTPATH> function call");

				/* start an xact for this function invocation */
				start_xact_command();

				if (HandleFunctionRequest() == EOF)
				{
					/* lost frontend connection during F message input */
					proc_exit(0);
				}

				/* commit the function-invocation transaction */
				finish_xact_command();
				break;

				/*
				 * 'Q' indicates a user query
				 */
			case 'Q':
				if (strspn(parser_input->data, " \t\r\n") == parser_input->len)
				{
					/*
					 * if there is nothing in the input buffer, don't
					 * bother trying to parse and execute anything; just
					 * send back a quick NullCommand response.
					 */
					if (IsUnderPostmaster)
						NullCommand(Remote);
				}
				else
				{
					/*
					 * otherwise, process the input string.
					 *
					 * Note: transaction command start/end is now done within
					 * pg_exec_query_string(), not here.
					 */
					if (Show_query_stats)
						ResetUsage();

					pgstat_report_activity(parser_input->data);

					pg_exec_query_string(parser_input->data,
										 whereToSendOutput,
										 QueryContext);

					if (Show_query_stats)
						ShowUsage("QUERY STATISTICS");
				}
				break;

				/* ----------------
				 *	'X' means that the frontend is closing down the socket.
				 *	EOF means unexpected loss of frontend connection.
				 *	Either way, perform normal shutdown.
				 * ----------------
				 */
			case 'X':
			case EOF:

				/*
				 * NOTE: if you are tempted to add more code here, DON'T!
				 * Whatever you had in mind to do should be set up as an
				 * on_proc_exit or on_shmem_exit callback, instead.
				 * Otherwise it will fail to be called during other
				 * backend-shutdown scenarios.
				 */
				proc_exit(0);

			default:
				elog(ERROR, "unknown frontend message was received");
		}

#ifdef MEMORY_CONTEXT_CHECKING

		/*
		 * Check all memory after each backend loop.  This is a rather
		 * weird place to do it, perhaps.
		 */
		MemoryContextCheck(TopMemoryContext);
#endif
	}							/* end of input-reading loop */

	/* can't get here because the above loop never exits */
	Assert(false);

	return 1;					/* keep compiler quiet */
}

#ifndef HAVE_GETRUSAGE
#include "rusagestub.h"
#else
#include <sys/resource.h>
#endif   /* HAVE_GETRUSAGE */

struct rusage Save_r;
struct timeval Save_t;

void
ResetUsage(void)
{
	struct timezone tz;

	getrusage(RUSAGE_SELF, &Save_r);
	gettimeofday(&Save_t, &tz);
	ResetBufferUsage();
/*	  ResetTupleCount(); */
}

void
ShowUsage(const char *title)
{
	StringInfoData str;
	struct timeval user,
				sys;
	struct timeval elapse_t;
	struct timezone tz;
	struct rusage r;
	char *bufusage;

	getrusage(RUSAGE_SELF, &r);
	gettimeofday(&elapse_t, &tz);
	memcpy((char *) &user, (char *) &r.ru_utime, sizeof(user));
	memcpy((char *) &sys, (char *) &r.ru_stime, sizeof(sys));
	if (elapse_t.tv_usec < Save_t.tv_usec)
	{
		elapse_t.tv_sec--;
		elapse_t.tv_usec += 1000000;
	}
	if (r.ru_utime.tv_usec < Save_r.ru_utime.tv_usec)
	{
		r.ru_utime.tv_sec--;
		r.ru_utime.tv_usec += 1000000;
	}
	if (r.ru_stime.tv_usec < Save_r.ru_stime.tv_usec)
	{
		r.ru_stime.tv_sec--;
		r.ru_stime.tv_usec += 1000000;
	}

	/*
	 * the only stats we don't show here are for memory usage -- i can't
	 * figure out how to interpret the relevant fields in the rusage
	 * struct, and they change names across o/s platforms, anyway. if you
	 * can figure out what the entries mean, you can somehow extract
	 * resident set size, shared text size, and unshared data and stack
	 * sizes.
	 */
	initStringInfo(&str);

	appendStringInfo(&str, "! system usage stats:\n");
	appendStringInfo(&str,
			"!\t%ld.%06ld elapsed %ld.%06ld user %ld.%06ld system sec\n",
			(long int) elapse_t.tv_sec - Save_t.tv_sec,
			(long int) elapse_t.tv_usec - Save_t.tv_usec,
			(long int) r.ru_utime.tv_sec - Save_r.ru_utime.tv_sec,
			(long int) r.ru_utime.tv_usec - Save_r.ru_utime.tv_usec,
			(long int) r.ru_stime.tv_sec - Save_r.ru_stime.tv_sec,
			(long int) r.ru_stime.tv_usec - Save_r.ru_stime.tv_usec);
	appendStringInfo(&str,
			"!\t[%ld.%06ld user %ld.%06ld sys total]\n",
			(long int) user.tv_sec,
			(long int) user.tv_usec,
			(long int) sys.tv_sec,
			(long int) sys.tv_usec);
/* BeOS has rusage but only has some fields, and not these... */
#if defined(HAVE_GETRUSAGE)
	appendStringInfo(&str,
			"!\t%ld/%ld [%ld/%ld] filesystem blocks in/out\n",
			r.ru_inblock - Save_r.ru_inblock,
	/* they only drink coffee at dec */
			r.ru_oublock - Save_r.ru_oublock,
			r.ru_inblock, r.ru_oublock);
	appendStringInfo(&str,
		  "!\t%ld/%ld [%ld/%ld] page faults/reclaims, %ld [%ld] swaps\n",
			r.ru_majflt - Save_r.ru_majflt,
			r.ru_minflt - Save_r.ru_minflt,
			r.ru_majflt, r.ru_minflt,
			r.ru_nswap - Save_r.ru_nswap,
			r.ru_nswap);
	appendStringInfo(&str,
	 "!\t%ld [%ld] signals rcvd, %ld/%ld [%ld/%ld] messages rcvd/sent\n",
			r.ru_nsignals - Save_r.ru_nsignals,
			r.ru_nsignals,
			r.ru_msgrcv - Save_r.ru_msgrcv,
			r.ru_msgsnd - Save_r.ru_msgsnd,
			r.ru_msgrcv, r.ru_msgsnd);
	appendStringInfo(&str,
		 "!\t%ld/%ld [%ld/%ld] voluntary/involuntary context switches\n",
			r.ru_nvcsw - Save_r.ru_nvcsw,
			r.ru_nivcsw - Save_r.ru_nivcsw,
			r.ru_nvcsw, r.ru_nivcsw);
#endif   /* HAVE_GETRUSAGE */

	bufusage = ShowBufferUsage();
	appendStringInfo(&str, "! postgres usage stats:\n%s", bufusage);
	pfree(bufusage);

	/* remove trailing newline */
	if (str.data[str.len-1] == '\n')
		str.data[--str.len] = '\0';

	elog(DEBUG, "%s\n%s", title, str.data);

	pfree(str.data);
}

#ifdef NOT_USED
static int
assertEnable(int val)
{
	assert_enabled = val;
	return val;
}

#ifdef ASSERT_CHECKING_TEST
int
assertTest(int val)
{
	Assert(val == 0);

	if (assert_enabled)
	{
		/* val != 0 should be trapped by previous Assert */
		elog(NOTICE, "Assert test successfull (val = %d)", val);
	}
	else
		elog(NOTICE, "Assert checking is disabled (val = %d)", val);

	return val;
}
#endif

#endif


/* ----------------------------------------------------------------
 *		CreateCommandTag
 *
 *		utility to get a string representation of the
 *		command operation.
 * ----------------------------------------------------------------
 */
static const char *
CreateCommandTag(Node *parsetree)
{
	const char   *tag;

	switch (nodeTag(parsetree))
	{
		case T_InsertStmt:
			tag = "INSERT";
			break;

		case T_DeleteStmt:
			tag = "DELETE";
			break;

		case T_UpdateStmt:
			tag = "UPDATE";
			break;

		case T_SelectStmt:
			tag = "SELECT";
			break;

		case T_TransactionStmt:
			{
				TransactionStmt *stmt = (TransactionStmt *) parsetree;

				switch (stmt->command)
				{
					case BEGIN_TRANS:
						tag = "BEGIN";
						break;

					case COMMIT:
						tag = "COMMIT";
						break;

					case ROLLBACK:
						tag = "ROLLBACK";
						break;

				    //renyi 11 13:added
				    case XAPREPARE_TRANS:
						tag = "XAPREPARE";
						break;

					case XACOMMIT_TRANS:
						tag = "XACOMMIT";
						break;

					case XAROLLBACK_TRANS:
						tag = "XAROLLBACK";
						break;

					default:
						tag = "???";
						break;
				}
			}
			break;

		case T_ClosePortalStmt:
			tag = "CLOSE";
			break;

		case T_FetchStmt:
			{
				FetchStmt  *stmt = (FetchStmt *) parsetree;
				tag = (stmt->ismove) ? "MOVE" : "FETCH";
			}
			break;

		case T_CreateStmt:
			tag = "CREATE";
			break;

		case T_DropStmt:
			tag = "DROP";
			break;

		case T_TruncateStmt:
			tag = "TRUNCATE";
			break;

		case T_CommentStmt:
			tag = "COMMENT";
			break;

		case T_CopyStmt:
			tag = "COPY";
			break;

		case T_RenameStmt:
			tag = "ALTER";
			break;

		case T_AlterTableStmt:
			tag = "ALTER";
			break;

		case T_GrantStmt:
			{
				GrantStmt  *stmt = (GrantStmt *) parsetree;
				tag = (stmt->is_grant) ? "GRANT" : "REVOKE";
			}
			break;

		case T_DefineStmt:
			tag = "CREATE";
			break;

		case T_ViewStmt:		/* CREATE VIEW */
			tag = "CREATE";
			break;

		case T_ProcedureStmt:	/* CREATE FUNCTION */
			tag = "CREATE";
			break;

		case T_IndexStmt:		/* CREATE INDEX */
			tag = "CREATE";
			break;

		case T_RuleStmt:		/* CREATE RULE */
			tag = "CREATE";
			break;

		case T_CreateSeqStmt:
			tag = "CREATE";
			break;

		case T_RemoveAggrStmt:
			tag = "DROP";
			break;

		case T_RemoveFuncStmt:
			tag = "DROP";
			break;

		case T_RemoveOperStmt:
			tag = "DROP";
			break;

		case T_VersionStmt:
			tag = "CREATE VERSION";
			break;

		case T_CreatedbStmt:
			tag = "CREATE DATABASE";
			break;

		case T_DropdbStmt:
			tag = "DROP DATABASE";
			break;

		case T_NotifyStmt:
			tag = "NOTIFY";
			break;

		case T_ListenStmt:
			tag = "LISTEN";
			break;

		case T_UnlistenStmt:
			tag = "UNLISTEN";
			break;

		case T_LoadStmt:
			tag = "LOAD";
			break;

		case T_ClusterStmt:
			tag = "CLUSTER";
			break;

		case T_VacuumStmt:
			if (((VacuumStmt *) parsetree)->vacuum)
				tag = "VACUUM";
			else
				tag = "ANALYZE";
			break;

		case T_ExplainStmt:
			tag = "EXPLAIN";
			break;

#ifdef NOT_USED
		case T_RecipeStmt:
			tag = "EXECUTE RECIPE";
			break;
#endif

		case T_VariableSetStmt:
			tag = "SET VARIABLE";
			break;

		case T_VariableShowStmt:
			tag = "SHOW VARIABLE";
			break;

		case T_VariableResetStmt:
			tag = "RESET VARIABLE";
			break;

		case T_CreateTrigStmt:
			tag = "CREATE";
			break;

		case T_DropTrigStmt:
			tag = "DROP";
			break;

		case T_CreatePLangStmt:
			tag = "CREATE";
			break;

		case T_DropPLangStmt:
			tag = "DROP";
			break;

		case T_CreateUserStmt:
			tag = "CREATE USER";
			break;

		case T_AlterUserStmt:
			tag = "ALTER USER";
			break;

		case T_DropUserStmt:
			tag = "DROP USER";
			break;

		case T_LockStmt:
			tag = "LOCK TABLE";
			break;

		case T_ConstraintsSetStmt:
			tag = "SET CONSTRAINTS";
			break;

		case T_CreateGroupStmt:
			tag = "CREATE GROUP";
			break;

		case T_AlterGroupStmt:
			tag = "ALTER GROUP";
			break;

		case T_DropGroupStmt:
			tag = "DROP GROUP";
			break;

		case T_CheckPointStmt:
			tag = "CHECKPOINT";
			break;

		case T_ReindexStmt:
			tag = "REINDEX";
			break;

		default:
			elog(DEBUG, "CreateCommandTag: unknown parse node type %d",
				 nodeTag(parsetree));
			tag = "???";
			break;
	}

	return tag;
}
