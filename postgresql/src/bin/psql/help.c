/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/psql/help.c,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
 */
#include "postgres_fe.h"
#include "help.h"

#include <signal.h>
#include <errno.h>

#ifndef WIN32
#include <sys/ioctl.h>			/* for ioctl() */
#ifdef HAVE_PWD_H
#include <pwd.h>				/* for getpwuid() */
#endif
#include <sys/types.h>			/* (ditto) */
#include <unistd.h>				/* for getuid() */
#else
#include <win32.h>
#endif

#include "pqsignal.h"
#include "libpq-fe.h"

#include "settings.h"
#include "common.h"
#include "sql_help.h"

#define _(x) gettext((x))

/*
 * PLEASE:
 * If you change something in this file, also make the same changes
 * in the DocBook documentation, file ref/psql-ref.sgml. If you don't
 * know how to do it, please find someone who can help you.
 */


/*
 * usage
 *
 * print out command line arguments
 */
#define ON(var) (var ? _("on") : _("off"))

void
usage(void)
{
	const char *env;
	const char *user;

#ifndef WIN32
	struct passwd *pw = NULL;
#endif

	/* Find default user, in case we need it. */
	user = getenv("USER");
	if (!user)
	{
#ifndef WIN32
		pw = getpwuid(geteuid());
		if (pw)
			user = pw->pw_name;
		else
		{
			psql_error("could not get current user name: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
#else
		user = "?";
#endif
	}

/* If this " is the start of the string then it ought to end there to fit in 80 columns >> " */
	puts(_("This is psql, the PostgreSQL interactive terminal.\n"));
	puts(_("Usage:"));
	puts(_("  psql [options] [dbname [username]]\n"));
	puts(_("Options:"));
	puts(_("  -a              Echo all input from script"));
	puts(_("  -A              Unaligned table output mode (-P format=unaligned)"));
	puts(_("  -c COMMAND      Run only single command (SQL or internal) and exit"));

	/* Display default database */
	env = getenv("PGDATABASE");
	if (!env)
		env = user;
	printf(_("  -d DBNAME       Specify database name to connect to (default: %s)\n"), env);

	puts(_("  -e              Echo commands sent to server"));
	puts(_("  -E              Display queries that internal commands generate"));
	puts(_("  -f FILENAME     Execute commands from file, then exit"));
	printf(_("  -F STRING       Set field separator (default: \"%s\") (-P fieldsep=)\n"),
		   DEFAULT_FIELD_SEP);

	/* Display default host */
	env = getenv("PGHOST");
	printf(_("  -h HOSTNAME     Specify database server host (default: %s)\n"),
		   env ? env : _("local socket"));

	puts(_("  -H              HTML table output mode (-P format=html)"));
	puts(_("  -l              List available databases, then exit"));
	puts(_("  -n              Disable enhanced command line editing (readline)"));
	puts(_("  -o FILENAME     Send query results to file (or |pipe)"));

	/* Display default port */
	env = getenv("PGPORT");
	printf(_("  -p PORT         Specify database server port (default: %s)\n"),
		   env ? env : DEF_PGPORT_STR);

	puts(_("  -P VAR[=ARG]    Set printing option 'VAR' to 'ARG' (see \\pset command)"));
	puts(_("  -q              Run quietly (no messages, only query output)"));
	puts(_("  -R STRING       Set record separator (default: newline) (-P recordsep=)"));
	puts(_("  -s              Single step mode (confirm each query)"));
	puts(_("  -S              Single line mode (end of line terminates SQL command)"));
	puts(_("  -t              Print rows only (-P tuples_only)"));
	puts(_("  -T TEXT         Set HTML table tag attributes (width, border) (-P tableattr=)"));

	/* Display default user */
	env = getenv("PGUSER");
	if (!env)
		env = user;
	printf(_("  -U NAME         Specify database user name (default: %s)\n"), env);

	puts(_("  -v NAME=VALUE   Set psql variable 'NAME' to 'VALUE'"));
	puts(_("  -V              Show version information and exit"));
	puts(_("  -W              Prompt for password (should happen automatically)"));
	puts(_("  -x              Turn on expanded table output (-P expanded)"));
	puts(_("  -X              Do not read startup file (~/.psqlrc)"));

	puts(_(
		   "\nFor more information, type \"\\?\" (for internal commands) or \"\\help\"\n"
		   "(for SQL commands) from within psql, or consult the psql section in\n"
		   "the PostgreSQL documentation.\n\n"
		   "Report bugs to <pgsql-bugs@postgresql.org>."));
}



/*
 * slashUsage
 *
 * print out help for the backslash commands
 */

#ifndef TIOCGWINSZ
struct winsize
{
	int			ws_row;
	int			ws_col;
};
#endif

void
slashUsage(void)
{
	bool		usePipe = false;
	const char *pagerenv;
	FILE	   *fout;
	struct winsize screen_size;

#ifdef TIOCGWINSZ
	if (pset.notty == 0 &&
		(ioctl(fileno(stdout), TIOCGWINSZ, &screen_size) == -1 ||
		 screen_size.ws_col == 0 ||
		 screen_size.ws_row == 0))
	{
#endif
		screen_size.ws_row = 24;
		screen_size.ws_col = 80;
#ifdef TIOCGWINSZ
	}
#endif

	if (pset.notty == 0 &&
		(pagerenv = getenv("PAGER")) &&
		(pagerenv[0] != '\0') &&
		screen_size.ws_row <= 39 &&
		(fout = popen(pagerenv, "w")))
	{
		usePipe = true;
#ifndef WIN32
		pqsignal(SIGPIPE, SIG_IGN);
#endif
	}
	else
		fout = stdout;

	/* if you add/remove a line here, change the row test above */
	fprintf(fout, _(" \\a             toggle between unaligned and aligned output mode\n"));
	fprintf(fout, _(" \\c[onnect] [DBNAME|- [USER]]\n"
		 "                connect to new database (currently \"%s\")\n"),
			PQdb(pset.db));
	fprintf(fout, _(" \\C TITLE       set table title\n"));
	fprintf(fout, _(" \\cd [DIRNAME]  change the current working directory\n"));
	fprintf(fout, _(" \\copy ...      perform SQL COPY with data stream to the client host\n"));
	fprintf(fout, _(" \\copyright     show PostgreSQL usage and distribution terms\n"));
	fprintf(fout, _(" \\d TABLE       describe table (or view, index, sequence)\n"));
	fprintf(fout, _(" \\d{t|i|s|v}... list tables/indexes/sequences/views\n"));
	fprintf(fout, _(" \\d{p|S|l}      list access privileges, system tables, or large objects\n"));
	fprintf(fout, _(" \\da            list aggregate functions\n"));
	fprintf(fout, _(" \\dd NAME       show comment for table, type, function, or operator\n"));
	fprintf(fout, _(" \\df            list functions\n"));
	fprintf(fout, _(" \\do            list operators\n"));
	fprintf(fout, _(" \\dT            list data types\n"));
	fprintf(fout, _(" \\e FILENAME    edit the current query buffer or file with external editor\n"));
	fprintf(fout, _(" \\echo TEXT     write text to standard output\n"));
	fprintf(fout, _(" \\encoding ENCODING  set client encoding\n"));
	fprintf(fout, _(" \\f STRING      set field separator\n"));
	fprintf(fout, _(" \\g FILENAME    send SQL command to server (and write results to file or |pipe)\n"));
	fprintf(fout, _(" \\h NAME        help on syntax of SQL commands, * for all commands\n"));
	fprintf(fout, _(" \\H             toggle HTML output mode (currently %s)\n"),
			ON(pset.popt.topt.format == PRINT_HTML));
	fprintf(fout, _(" \\i FILENAME    execute commands from file\n"));
	fprintf(fout, _(" \\l             list all databases\n"));
	fprintf(fout, _(" \\lo_export, \\lo_import, \\lo_list, \\lo_unlink\n"
					"                large object operations\n"));
	fprintf(fout, _(" \\o FILENAME    send all query results to file or |pipe\n"));
	fprintf(fout, _(" \\p             show the content of the current query buffer\n"));
	fprintf(fout, _(" \\pset VAR      set table output option (VAR := {format|border|expanded|\n"
					"                fieldsep|null|recordsep|tuples_only|title|tableattr|pager})\n"));
	fprintf(fout, _(" \\q             quit psql\n"));
	fprintf(fout, _(" \\qecho TEXT    write text to query output stream (see \\o)\n"));
	fprintf(fout, _(" \\r             reset (clear) the query buffer\n"));
	fprintf(fout, _(" \\s FILENAME    print history or save it to file\n"));
	fprintf(fout, _(" \\set NAME VALUE  set internal variable\n"));
	fprintf(fout, _(" \\t             show only rows (currently %s)\n"),
			ON(pset.popt.topt.tuples_only));
	fprintf(fout, _(" \\T TEXT        set HTML table tag attributes\n"));
	fprintf(fout, _(" \\unset NAME    unset (delete) internal variable\n"));
	fprintf(fout, _(" \\w FILENAME    write current query buffer to file\n"));
	fprintf(fout, _(" \\x             toggle expanded output (currently %s)\n"),
			ON(pset.popt.topt.expanded));
	fprintf(fout, _(" \\z             list table access privileges\n"));
	fprintf(fout, _(" \\! [COMMAND]   execute command in shell or start interactive shell\n"));

	if (usePipe)
	{
		pclose(fout);
#ifndef WIN32
		pqsignal(SIGPIPE, SIG_DFL);
#endif
	}
}



/*
 * helpSQL -- help with SQL commands
 *
 */
void
helpSQL(const char *topic)
{
#define VALUE_OR_NULL(a) ((a) ? (a) : "")

	if (!topic || strlen(topic) == 0)
	{
		int			i;
		int			items_per_column = (QL_HELP_COUNT + 2) / 3;

		puts(_("Available help:"));

		for (i = 0; i < items_per_column; i++)
		{
			printf("  %-26s%-26s",
				   VALUE_OR_NULL(QL_HELP[i].cmd),
				   VALUE_OR_NULL(QL_HELP[i + items_per_column].cmd));
			if (i + 2 * items_per_column < QL_HELP_COUNT)
				printf("%-26s",
				   VALUE_OR_NULL(QL_HELP[i + 2 * items_per_column].cmd));
			fputc('\n', stdout);
		}
	}

	else
	{
		int			i;
		bool		help_found = false;
		size_t		len;

		/* don't care about trailing spaces */
		len = strlen(topic);
		while (topic[len - 1] == ' ')
			len--;

		for (i = 0; QL_HELP[i].cmd; i++)
		{
			if (strncasecmp(topic, QL_HELP[i].cmd, len) == 0 ||
				strcmp(topic, "*") == 0)
			{
				help_found = true;
				printf(_("Command:     %s\n"
						 "Description: %s\n"
						 "Syntax:\n%s\n\n"),
					 QL_HELP[i].cmd, QL_HELP[i].help, QL_HELP[i].syntax);
				/* If we have an exact match, exit.  Fixes \h SELECT */
				if (strcasecmp(topic, QL_HELP[i].cmd) == 0)
					break;
			}
		}

		if (!help_found)
			printf(_("No help available for '%-.*s'.\nTry \\h with no arguments to see available help.\n"), (int) len, topic);
	}
}



void
print_copyright(void)
{
	puts(
		 "PostgreSQL Data Base Management System\n\n"
		 "Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group\n\n"
		 "This software is based on Postgres95, formerly known as Postgres, which\n"
		 "contains the following notice:\n\n"
		 "Portions Copyright(c) 1994 - 7 Regents of the University of California\n\n"
		 "Permission to use, copy, modify, and distribute this software and its\n"
		 "documentation for any purpose, without fee, and without a written agreement\n"
		 "is hereby granted, provided that the above copyright notice and this paragraph\n"
		 "and the following two paragraphs appear in all copies.\n\n"
		 "IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR\n"
		 "DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST\n"
		 "PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF\n"
		 "THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH\n"
		 "DAMAGE.\n\n"
		 "THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,\n"
		 "BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A\n"
		 "PARTICULAR PURPOSE.THE SOFTWARE PROVIDED HEREUNDER IS ON AN \"AS IS\" BASIS,\n"
		 "AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE,\n"
		 "SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
		);
}
