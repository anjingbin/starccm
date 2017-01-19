/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/psql/common.c,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
 */
#include "postgres_fe.h"

#include "common.h"

#include <errno.h>
#include <stdarg.h>
#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif
#ifndef HAVE_STRDUP
#include <strdup.h>
#endif
#include <signal.h>
#ifndef WIN32
#include <unistd.h>				/* for write() */
#include <setjmp.h>
#else
#include <io.h>					/* for _write() */
#include <win32.h>
#endif

#include "libpq-fe.h"
#include "pqsignal.h"

#include "settings.h"
#include "variables.h"
#include "copy.h"
#include "prompt.h"
#include "print.h"
#include "mainloop.h"


/*
 * "Safe" wrapper around strdup()
 */
char *
xstrdup(const char *string)
{
	char	   *tmp;

	if (!string)
	{
		fprintf(stderr, gettext("%s: xstrdup: cannot duplicate null pointer (internal error)\n"),
				pset.progname);
		exit(EXIT_FAILURE);
	}
	tmp = strdup(string);
	if (!tmp)
	{
		psql_error("out of memory\n");
		exit(EXIT_FAILURE);
	}
	return tmp;
}



/*
 * setQFout
 * -- handler for -o command line option and \o command
 *
 * Tries to open file fname (or pipe if fname starts with '|')
 * and stores the file handle in pset)
 * Upon failure, sets stdout and returns false.
 */
bool
setQFout(const char *fname)
{
	bool		status = true;

	/* Close old file/pipe */
	if (pset.queryFout && pset.queryFout != stdout && pset.queryFout != stderr)
	{
		if (pset.queryFoutPipe)
			pclose(pset.queryFout);
		else
			fclose(pset.queryFout);
	}

	/* If no filename, set stdout */
	if (!fname || fname[0] == '\0')
	{
		pset.queryFout = stdout;
		pset.queryFoutPipe = false;
	}
	else if (*fname == '|')
	{
		pset.queryFout = popen(fname + 1, "w");
		pset.queryFoutPipe = true;
	}
	else
	{
		pset.queryFout = fopen(fname, "w");
		pset.queryFoutPipe = false;
	}

	if (!(pset.queryFout))
	{
		psql_error("%s: %s\n", fname, strerror(errno));
		pset.queryFout = stdout;
		pset.queryFoutPipe = false;
		status = false;
	}

	/* Direct signals */
#ifndef WIN32
	if (pset.queryFoutPipe)
		pqsignal(SIGPIPE, SIG_IGN);
	else
		pqsignal(SIGPIPE, SIG_DFL);
#endif

	return status;
}



/*
 * Error reporting for scripts. Errors should look like
 *	 psql:filename:lineno: message
 *
 */
void
psql_error(const char *fmt,...)
{
	va_list		ap;

	fflush(stdout);
	if (pset.queryFout != stdout)
		fflush(pset.queryFout);

	if (pset.inputfile)
		fprintf(stderr, "%s:%s:%u: ", pset.progname, pset.inputfile, pset.lineno);
	va_start(ap, fmt);
	vfprintf(stderr, gettext(fmt), ap);
	va_end(ap);
}



/*
 * for backend NOTICES
 */
void
NoticeProcessor(void *arg, const char *message)
{
	(void) arg;					/* not used */
	psql_error("%s", message);
}



/*
 * simple_prompt
 *
 * Generalized function especially intended for reading in usernames and
 * password interactively. Reads from /dev/tty or stdin/stderr.
 *
 * prompt:		The prompt to print
 * maxlen:		How many characters to accept
 * echo:		Set to false if you want to hide what is entered (for passwords)
 *
 * Returns a malloc()'ed string with the input (w/o trailing newline).
 */
static bool prompt_state = false;

char *
simple_prompt(const char *prompt, int maxlen, bool echo)
{
	int			length;
	char	   *destination;
	FILE	   *termin,
			   *termout;

#ifdef HAVE_TERMIOS_H
	struct termios t_orig,
				t;
#endif

	destination = (char *) malloc(maxlen + 2);
	if (!destination)
		return NULL;

	prompt_state = true;		/* disable SIGINT */

	/*
	 * Do not try to collapse these into one "w+" mode file. Doesn't work
	 * on some platforms (eg, HPUX 10.20).
	 */
	termin = fopen("/dev/tty", "r");
	termout = fopen("/dev/tty", "w");
	if (!termin || !termout)
	{
		if (termin)
			fclose(termin);
		if (termout)
			fclose(termout);
		termin = stdin;
		termout = stderr;
	}

#ifdef HAVE_TERMIOS_H
	if (!echo)
	{
		tcgetattr(fileno(termin), &t);
		t_orig = t;
		t.c_lflag &= ~ECHO;
		tcsetattr(fileno(termin), TCSAFLUSH, &t);
	}
#endif

	if (prompt)
	{
		fputs(gettext(prompt), termout);
		fflush(termout);
	}

	if (fgets(destination, maxlen, termin) == NULL)
		destination[0] = '\0';

	length = strlen(destination);
	if (length > 0 && destination[length - 1] != '\n')
	{
		/* eat rest of the line */
		char		buf[128];
		int			buflen;

		do
		{
			if (fgets(buf, sizeof(buf), termin) == NULL)
				break;
			buflen = strlen(buf);
		} while (buflen > 0 && buf[buflen - 1] != '\n');
	}

	if (length > 0 && destination[length - 1] == '\n')
		/* remove trailing newline */
		destination[length - 1] = '\0';

#ifdef HAVE_TERMIOS_H
	if (!echo)
	{
		tcsetattr(fileno(termin), TCSAFLUSH, &t_orig);
		fputs("\n", termout);
		fflush(termout);
	}
#endif

	if (termin != stdin)
	{
		fclose(termin);
		fclose(termout);
	}

	prompt_state = false;		/* SIGINT okay again */

	return destination;
}



/*
 * Code to support query cancellation
 *
 * Before we start a query, we enable a SIGINT signal catcher that sends a
 * cancel request to the backend. Note that sending the cancel directly from
 * the signal handler is safe because PQrequestCancel() is written to make it
 * so. We use write() to print to stdout because it's better to use simple
 * facilities in a signal handler.
 */

PGconn	   *cancelConn;
volatile bool cancel_pressed;

#ifndef WIN32

#define write_stderr(String) write(fileno(stderr), String, strlen(String))

void
handle_sigint(SIGNAL_ARGS)
{
	int			save_errno = errno;

	/* Don't muck around if copying in or prompting for a password. */
	if ((copy_in_state && pset.cur_cmd_interactive) || prompt_state)
		return;

	if (cancelConn == NULL)
		siglongjmp(main_loop_jmp, 1);

	cancel_pressed = true;

	if (PQrequestCancel(cancelConn))
		write_stderr("Cancel request sent\n");
	else
	{
		write_stderr("Could not send cancel request: ");
		write_stderr(PQerrorMessage(cancelConn));
	}
	errno = save_errno;			/* just in case the write changed it */
}
#endif   /* not WIN32 */


/*
 * PSQLexec
 *
 * This is the way to send "backdoor" queries (those not directly entered
 * by the user). It is subject to -E but not -e.
 */
PGresult *
PSQLexec(const char *query)
{
	PGresult   *res;
	const char *var;

	if (!pset.db)
	{
		psql_error("You are currently not connected to a database.\n");
		return NULL;
	}

	var = GetVariable(pset.vars, "ECHO_HIDDEN");
	if (var)
	{
		printf("********* QUERY **********\n"
			   "%s\n"
			   "**************************\n\n", query);
		fflush(stdout);
	}

	if (var && strcmp(var, "noexec") == 0)
		return NULL;

	cancelConn = pset.db;
	res = PQexec(pset.db, query);
	if (PQresultStatus(res) == PGRES_COPY_IN)
		copy_in_state = true;
	/* keep cancel connection for copy out state */
	if (PQresultStatus(res) != PGRES_COPY_OUT)
		cancelConn = NULL;

	if (res && (PQresultStatus(res) == PGRES_COMMAND_OK ||
				PQresultStatus(res) == PGRES_TUPLES_OK ||
				PQresultStatus(res) == PGRES_COPY_IN ||
				PQresultStatus(res) == PGRES_COPY_OUT)
		)
		return res;
	else
	{
		psql_error("%s", PQerrorMessage(pset.db));
		PQclear(res);

		if (PQstatus(pset.db) == CONNECTION_BAD)
		{
			if (!pset.cur_cmd_interactive)
			{
				psql_error("connection to server was lost\n");
				exit(EXIT_BADCONN);
			}
			fputs(gettext("The connection to the server was lost. Attempting reset: "), stderr);
			PQreset(pset.db);
			if (PQstatus(pset.db) == CONNECTION_BAD)
			{
				fputs(gettext("Failed.\n"), stderr);
				PQfinish(pset.db);
				pset.db = NULL;
				SetVariable(pset.vars, "DBNAME", NULL);
				SetVariable(pset.vars, "HOST", NULL);
				SetVariable(pset.vars, "PORT", NULL);
				SetVariable(pset.vars, "USER", NULL);
				SetVariable(pset.vars, "ENCODING", NULL);
			}
			else
				fputs(gettext("Succeeded.\n"), stderr);
		}

		return NULL;
	}
}



/*
 * SendQuery: send the query string to the backend
 * (and print out results)
 *
 * Note: This is the "front door" way to send a query. That is, use it to
 * send queries actually entered by the user. These queries will be subject to
 * single step mode.
 * To send "back door" queries (generated by slash commands, etc.) in a
 * controlled way, use PSQLexec().
 *
 * Returns true if the query executed successfully, false otherwise.
 */
bool
SendQuery(const char *query)
{
	bool		success = false;
	PGresult   *results;
	PGnotify   *notify;

	if (!pset.db)
	{
		psql_error("You are currently not connected to a database.\n");
		return false;
	}

	if (GetVariableBool(pset.vars, "SINGLESTEP"))
	{
		char		buf[3];

		printf(gettext("***(Single step mode: Verify query)*********************************************\n"
					   "%s\n"
					   "***(press return to proceed or enter x and return to cancel)********************\n"),
			   query);
		fflush(stdout);
		if (fgets(buf, sizeof(buf), stdin) != NULL)
			if (buf[0] == 'x')
				return false;
	}
	else
	{
		const char *var = GetVariable(pset.vars, "ECHO");

		if (var && strncmp(var, "queries", strlen(var)) == 0)
			puts(query);
	}

	cancelConn = pset.db;
	results = PQexec(pset.db, query);
	if (PQresultStatus(results) == PGRES_COPY_IN)
		copy_in_state = true;
	/* keep cancel connection for copy out state */
	if (PQresultStatus(results) != PGRES_COPY_OUT)
		cancelConn = NULL;

	if (results == NULL)
	{
		fputs(PQerrorMessage(pset.db), pset.queryFout);
		success = false;
	}
	else
	{
		switch (PQresultStatus(results))
		{
			case PGRES_TUPLES_OK:
				/* write output to \g argument, if any */
				if (pset.gfname)
				{
					FILE	   *queryFout_copy = pset.queryFout;
					bool		queryFoutPipe_copy = pset.queryFoutPipe;

					pset.queryFout = stdout;	/* so it doesn't get
												 * closed */

					/* open file/pipe */
					if (!setQFout(pset.gfname))
					{
						pset.queryFout = queryFout_copy;
						pset.queryFoutPipe = queryFoutPipe_copy;
						success = false;
						break;
					}

					printQuery(results, &pset.popt, pset.queryFout);

					/* close file/pipe, restore old setting */
					setQFout(NULL);

					pset.queryFout = queryFout_copy;
					pset.queryFoutPipe = queryFoutPipe_copy;

					free(pset.gfname);
					pset.gfname = NULL;

					success = true;
				}
				else
				{
					printQuery(results, &pset.popt, pset.queryFout);
					success = true;
				}
				break;
			case PGRES_EMPTY_QUERY:
				success = true;
				break;
			case PGRES_COMMAND_OK:
				{
					char		buf[10];

					success = true;
					sprintf(buf, "%u", (unsigned int) PQoidValue(results));
					if (!QUIET())
						fprintf(pset.queryFout, "%s\n", PQcmdStatus(results));
					SetVariable(pset.vars, "LASTOID", buf);
					break;
				}
			case PGRES_COPY_OUT:
				success = handleCopyOut(pset.db, pset.queryFout);
				break;

			case PGRES_COPY_IN:
				if (pset.cur_cmd_interactive && !QUIET())
					puts(gettext("Enter data to be copied followed by a newline.\n"
								 "End with a backslash and a period on a line by itself."));

				success = handleCopyIn(pset.db, pset.cur_cmd_source,
									   pset.cur_cmd_interactive ? get_prompt(PROMPT_COPY) : NULL);
				break;

			case PGRES_NONFATAL_ERROR:
			case PGRES_FATAL_ERROR:
			case PGRES_BAD_RESPONSE:
				success = false;
				psql_error("%s", PQerrorMessage(pset.db));
				break;
		}

		fflush(pset.queryFout);

		if (PQstatus(pset.db) == CONNECTION_BAD)
		{
			if (!pset.cur_cmd_interactive)
			{
				psql_error("connection to server was lost\n");
				exit(EXIT_BADCONN);
			}
			fputs(gettext("The connection to the server was lost. Attempting reset: "), stderr);
			PQreset(pset.db);
			if (PQstatus(pset.db) == CONNECTION_BAD)
			{
				fputs(gettext("Failed.\n"), stderr);
				PQfinish(pset.db);
				PQclear(results);
				pset.db = NULL;
				SetVariable(pset.vars, "DBNAME", NULL);
				SetVariable(pset.vars, "HOST", NULL);
				SetVariable(pset.vars, "PORT", NULL);
				SetVariable(pset.vars, "USER", NULL);
				SetVariable(pset.vars, "ENCODING", NULL);
				return false;
			}
			else
				fputs(gettext("Succeeded.\n"), stderr);
		}

		/* check for asynchronous notification returns */
		while ((notify = PQnotifies(pset.db)) != NULL)
		{
			fprintf(pset.queryFout, gettext("Asynchronous NOTIFY '%s' from backend with pid %d received.\n"),
					notify->relname, notify->be_pid);
			free(notify);
			fflush(pset.queryFout);
		}

		if (results)
			PQclear(results);
	}

	return success;
}
