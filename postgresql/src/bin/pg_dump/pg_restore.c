/*-------------------------------------------------------------------------
 *
 * pg_restore.c
 *	pg_restore is an utility extracting postgres database definitions
 *	from a backup archive created by pg_dump using the archiver
 *	interface.
 *
 *	pg_restore will read the backup archive and
 *	dump out a script that reproduces
 *	the schema of the database in terms of
 *		  user-defined types
 *		  user-defined functions
 *		  tables
 *		  indexes
 *		  aggregates
 *		  operators
 *		  ACL - grant/revoke
 *
 * the output script is SQL that is understood by PostgreSQL
 *
 * Basic process in a restore operation is:
 *
 *	Open the Archive and read the TOC.
 *	Set flags in TOC entries, and *maybe* reorder them.
 *	Generate script to stdout
 *	Exit
 *
 * Copyright (c) 2000, Philip Warner
 *		Rights are granted to use this software in any way so long
 *		as this notice is not removed.
 *
 *	The author is not responsible for loss or damages that may
 *	result from it's use.
 *
 *
 * IDENTIFICATION
 *		$Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/pg_dump/pg_restore.c,v 1.2 2004/07/05 08:55:50 SuLiang Exp $
 *
 * Modifications - 28-Jun-2000 - pjw@rhyme.com.au
 *
 *		Initial version. Command processing taken from original pg_dump.
 *
 * Modifications - 28-Jul-2000 - pjw@rhyme.com.au (1.45)
 *
 *		Added --create, --no-owner, --superuser, --no-reconnect (pg_dump & pg_restore)
 *		Added code to dump 'Create Schema' statement (pg_dump)
 *		Don't bother to disable/enable triggers if we don't have a superuser (pg_restore)
 *		Cleaned up code for reconnecting to database.
 *		Force a reconnect as superuser before enabling/disabling triggers.
 *
 * Modifications - 6-Mar-2001 - pjw@rhyme.com.au
 *		Change -U option to -L to allow -U to specify username in future.
 *
 *-------------------------------------------------------------------------
 */

#include "pg_backup.h"
#include "pg_backup_archiver.h"

#include <ctype.h>

#ifndef HAVE_STRDUP
#include "strdup.h"
#endif

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif

#include <unistd.h>

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#ifdef ENABLE_NLS
#include <locale.h>
#endif

/* Forward decls */
static void usage(const char *progname);
static char *_cleanupName(char *name);

typedef struct option optType;


int
main(int argc, char **argv)
{
	RestoreOptions *opts;
	int			c;
	Archive    *AH;
	char	   *fileSpec = NULL;
	extern int	optind;
	extern char *optarg;
	static int	use_setsessauth = 0;

#ifdef HAVE_GETOPT_LONG
	struct option cmdopts[] = {
		{"clean", 0, NULL, 'c'},
		{"create", 0, NULL, 'C'},
		{"data-only", 0, NULL, 'a'},
		{"dbname", 1, NULL, 'd'},
		{"file", 1, NULL, 'f'},
		{"format", 1, NULL, 'F'},
		{"function", 1, NULL, 'P'},
		{"host", 1, NULL, 'h'},
		{"ignore-version", 0, NULL, 'i'},
		{"index", 1, NULL, 'I'},
		{"list", 0, NULL, 'l'},
		{"no-privileges", 0, NULL, 'x'},
		{"no-acl", 0, NULL, 'x'},
		{"no-owner", 0, NULL, 'O'},
		{"no-reconnect", 0, NULL, 'R'},
		{"port", 1, NULL, 'p'},
		{"oid-order", 0, NULL, 'o'},
		{"orig-order", 0, NULL, 'N'},
		{"password", 0, NULL, 'W'},
		{"rearrange", 0, NULL, 'r'},
		{"schema-only", 0, NULL, 's'},
		{"superuser", 1, NULL, 'S'},
		{"table", 1, NULL, 't'},
		{"trigger", 1, NULL, 'T'},
		{"use-list", 1, NULL, 'L'},
		{"username", 1, NULL, 'U'},
		{"verbose", 0, NULL, 'v'},

		/*
		 * the following options don't have an equivalent short option
		 * letter, but are available as '-X long-name'
		 */
		{"use-set-session-authorization", no_argument, &use_setsessauth, 1},
		{NULL, 0, NULL, 0}
	};
#endif   /* HAVE_GETOPT_LONG */


#ifdef ENABLE_NLS
	setlocale(LC_ALL, "");
	bindtextdomain("pg_dump", LOCALEDIR);
	textdomain("pg_dump");
#endif

	opts = NewRestoreOptions();

	if (!strrchr(argv[0], '/'))
		progname = argv[0];
	else
		progname = strrchr(argv[0], '/') + 1;

	if (argc > 1)
	{
		if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0)
		{
			usage(progname);
			exit(0);
		}
		if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0)
		{
			puts("pg_restore (PostgreSQL) " PG_VERSION);
			exit(0);
		}
	}

#ifdef HAVE_GETOPT_LONG
	while ((c = getopt_long(argc, argv, "acCd:f:F:h:i:lL:NoOp:P:rRsS:t:T:uU:vWxX:", cmdopts, NULL)) != -1)
#else
	while ((c = getopt(argc, argv, "acCd:f:F:h:i:lL:NoOp:P:rRsS:t:T:uU:vWxX:")) != -1)
#endif
	{
		switch (c)
		{
			case 'a':			/* Dump data only */
				opts->dataOnly = 1;
				break;
			case 'c':			/* clean (i.e., drop) schema prior to
								 * create */
				opts->dropSchema = 1;
				break;
			case 'C':
				opts->create = 1;
				break;
			case 'd':
				if (strlen(optarg) != 0)
				{
					opts->dbname = strdup(optarg);
					opts->useDB = 1;
				}
				break;
			case 'f':			/* output file name */
				opts->filename = strdup(optarg);
				break;
			case 'F':
				if (strlen(optarg) != 0)
					opts->formatName = strdup(optarg);
				break;
			case 'h':
				if (strlen(optarg) != 0)
					opts->pghost = strdup(optarg);
				break;
			case 'i':
				opts->ignoreVersion = 1;
				break;

			case 'l':			/* Dump the TOC summary */
				opts->tocSummary = 1;
				break;

			case 'L':			/* input TOC summary file name */
				opts->tocFile = strdup(optarg);
				break;

			case 'N':
				opts->origOrder = 1;
				break;
			case 'o':
				opts->oidOrder = 1;
				break;
			case 'O':
				opts->noOwner = 1;
				break;
			case 'p':
				if (strlen(optarg) != 0)
					opts->pgport = strdup(optarg);
				break;
			case 'r':
				opts->rearrange = 1;
				break;
			case 'R':
				opts->noReconnect = 1;
				break;
			case 'P':			/* Function */
				opts->selTypes = 1;
				opts->selFunction = 1;
				opts->functionNames = optarg ? strdup(optarg) : NULL;
				break;
			case 'I':			/* Index */
				opts->selTypes = 1;
				opts->selIndex = 1;
				opts->indexNames = _cleanupName(optarg);
				break;
			case 'T':			/* Trigger */
				opts->selTypes = 1;
				opts->selTrigger = 1;
				opts->triggerNames = _cleanupName(optarg);
				break;
			case 's':			/* dump schema only */
				opts->schemaOnly = 1;
				break;
			case 'S':			/* Superuser username */
				if (strlen(optarg) != 0)
					opts->superuser = strdup(optarg);
				break;
			case 't':			/* Dump data for this table only */
				opts->selTypes = 1;
				opts->selTable = 1;
				opts->tableNames = _cleanupName(optarg);
				break;

			case 'u':
				opts->requirePassword = true;
				opts->username = simple_prompt("User name: ", 100, true);
				break;

			case 'U':
				opts->username = optarg;
				break;

			case 'v':			/* verbose */
				opts->verbose = 1;
				break;

			case 'W':
				opts->requirePassword = true;
				break;

			case 'x':			/* skip ACL dump */
				opts->aclsSkip = 1;
				break;

			case 'X':
				if (strcmp(optarg, "use-set-session-authorization") == 0)
					use_setsessauth = 1;
				else
				{
					fprintf(stderr,
							gettext("%s: invalid -X option -- %s\n"),
							progname, optarg);
					fprintf(stderr, gettext("Try '%s --help' for more information.\n"), progname);
					exit(1);
				}
				break;

#ifdef HAVE_GETOPT_LONG
				/* This covers the long options equivalent to -X xxx. */
			case 0:
				break;
#endif

			default:
				fprintf(stderr, gettext("Try '%s --help' for more information.\n"), progname);
				exit(1);
		}
	}

	if (optind < argc)
		fileSpec = argv[optind];
	else
		fileSpec = NULL;

	opts->use_setsessauth = use_setsessauth;

	if (opts->formatName)
	{

		switch (opts->formatName[0])
		{

			case 'c':
			case 'C':
				opts->format = archCustom;
				break;

			case 'f':
			case 'F':
				opts->format = archFiles;
				break;

			case 't':
			case 'T':
				opts->format = archTar;
				break;

			default:
				write_msg("unrecognized archive format '%s'; please specify 't' or 'c'\n",
						  opts->formatName);
				exit(1);
		}
	}

	AH = OpenArchive(fileSpec, opts->format);

	/* Let the archiver know how noisy to be */
	AH->verbose = opts->verbose;

	if (opts->tocFile)
		SortTocFromFile(AH, opts);

	if (opts->oidOrder)
		SortTocByOID(AH);
	else if (opts->origOrder)
		SortTocByID(AH);

	if (opts->rearrange)
	{
		MoveToStart(AH, "<Init>");
		MoveToEnd(AH, "TABLE DATA");
		MoveToEnd(AH, "BLOBS");
		MoveToEnd(AH, "INDEX");
		MoveToEnd(AH, "TRIGGER");
		MoveToEnd(AH, "RULE");
		MoveToEnd(AH, "SEQUENCE SET");
	}

	/* Database MUST be at start */
	MoveToStart(AH, "DATABASE");

	if (opts->tocSummary)
		PrintTOCSummary(AH, opts);
	else
		RestoreArchive(AH, opts);

	CloseArchive(AH);

	return 0;
}

static void
usage(const char *progname)
{
	printf(gettext(
				   "%s restores a PostgreSQL database from an archive created by pg_dump.\n\n"
				   "Usage:\n  %s [options] [file]\n\n"
				   "Options:\n"),
		   progname, progname);
#ifdef HAVE_GETOPT_LONG
	puts(gettext(
		  "  -a, --data-only          restore only the data, no schema\n"
	   "  -c, --clean              clean (drop) schema prior to create\n"
	 "  -C, --create             issue commands to create the database\n"
				 "  -d, --dbname=NAME        output database name\n"
				 "  -f, --file=FILENAME      output file name\n"
				 "  -F, --format={c|t}       specify backup file format\n"
				 "  -h, --host=HOSTNAME      server host name\n"
				 "  -i, --index=NAME         restore named index\n"
	   "  -l, --list               print summarized TOC of the archive\n"
				 "  -L, --use-list=FILENAME  use specified table of contents for ordering\n"
				 "                           output from this file\n"
			"  -N, --orig-order         restore in original dump order\n"
				 "  -o, --oid-order          restore in OID order\n"
	 "  -O, --no-owner           do not reconnect to database to match\n"
				 "                           object owner\n"
				 "  -p, --port=PORT          server port number\n"
				 "  -P, --function=NAME      restore named function\n"
				 "  -r, --rearrange          rearrange output to put indexes etc. at end\n"
				 "  -R, --no-reconnect       disallow ALL reconnections to the database\n"
		  "  -s, --schema-only        restore only the schema, no data\n"
				 "  -S, --superuser=NAME     specify the superuser user name to use for\n"
				 "                           disabling triggers\n"
				 "  -t, --table=NAME         restore named table\n"
				 "  -T, --trigger=NAME       restore named trigger\n"
		"  -U, --username=NAME      connect as specified database user\n"
				 "  -v, --verbose            verbose mode\n"
				 "  -W, --password           force password prompt (should happen automatically)\n"
				 "  -x, --no-privileges      skip restoration of access privileges (grant/revoke)\n"
				 "  -X use-set-session-authorization, --use-set-session-authorization\n"
				 "                           use SET SESSION AUTHORIZATION commands instead\n"
			  "                           of reconnecting, if possible\n"
				 ));

#else							/* not HAVE_GETOPT_LONG */

	puts(gettext(
		  "  -a                       restore only the data, no schema\n"
	   "  -c                       clean (drop) schema prior to create\n"
	 "  -C                       issue commands to create the database\n"
				 "  -d NAME                  output database name\n"
				 "  -f FILENAME              output file name\n"
				 "  -F {c|t}                 specify backup file format\n"
				 "  -h HOSTNAME              server host name\n"
				 "  -i NAME                  restore named index\n"
	   "  -l                       print summarized TOC of the archive\n"
				 "  -L FILENAME              use specified table of contents for ordering\n"
				 "                           output from this file\n"
			"  -N                       restore in original dump order\n"
				 "  -o                       restore in OID order\n"
	 "  -O                       do not reconnect to database to match\n"
				 "                           object owner\n"
				 "  -p PORT                  server port number\n"
				 "  -P NAME                  restore named function\n"
				 "  -r                       rearrange output to put indexes etc. at end\n"
				 "  -R                       disallow ALL reconnections to the database\n"
		  "  -s                       restore only the schema, no data\n"
				 "  -S NAME                  specify the superuser user name to use for\n"
				 "                           disabling triggers\n"
				 "  -t NAME                  restore named table\n"
				 "  -T NAME                  restore named trigger\n"
		"  -U NAME                  connect as specified database user\n"
				 "  -v                       verbose mode\n"
				 "  -W                       force password prompt (should happen automatically)\n"
				 "  -x                       skip restoration of access privileges (grant/revoke)\n"
				 "  -X use-set-session-authorization\n"
				 "                           use SET SESSION AUTHORIZATION commands instead\n"
			  "                           of reconnecting, if possible\n"
				 ));
#endif
	puts(gettext("If no input file name is supplied, then standard input is used.\n"));
	puts(gettext("Report bugs to <pgsql-bugs@postgresql.org>."));
}


static char *
_cleanupName(char *name)
{
	int			i;

	if (!name || !name[0])
		return NULL;

	name = strdup(name);

	if (name[0] == '"')
	{
		strcpy(name, &name[1]);
		if (name[0] && *(name + strlen(name) - 1) == '"')
			*(name + strlen(name) - 1) = '\0';
	}
	/* otherwise, convert table name to lowercase... */
	else
	{
		for (i = 0; name[i]; i++)
			if (isupper((unsigned char) name[i]))
				name[i] = tolower((unsigned char) name[i]);
	}
	return name;
}
