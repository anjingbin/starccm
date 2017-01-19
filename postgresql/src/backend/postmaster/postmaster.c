/*-------------------------------------------------------------------------
 *
 * postmaster.c
 *	  This program acts as a clearing house for requests to the
 *	  POSTGRES system.	Frontend programs send a startup message
 *	  to the Postmaster and the postmaster uses the info in the
 *	  message to setup a backend process.
 *
 *	  The postmaster also manages system-wide operations such as
 *	  startup, shutdown, and periodic checkpoints.	The postmaster
 *	  itself doesn't do those operations, mind you --- it just forks
 *	  off a subprocess to do them at the right times.  It also takes
 *	  care of resetting the system if a backend crashes.
 *
 *	  The postmaster process creates the shared memory and semaphore
 *	  pools during startup, but as a rule does not touch them itself.
 *	  In particular, it is not a member of the PROC array of backends
 *	  and so it cannot participate in lock-manager operations.	Keeping
 *	  the postmaster away from shared memory operations makes it simpler
 *	  and more reliable.  The postmaster is almost always able to recover
 *	  from crashes of individual backends by resetting shared memory;
 *	  if it did much with shared memory then it would be prone to crashing
 *	  along with the backends.
 *
 *	  When a request message is received, we now fork() immediately.
 *	  The child process performs authentication of the request, and
 *	  then becomes a backend if successful.  This allows the auth code
 *	  to be written in a simple single-threaded style (as opposed to the
 *	  crufty "poor man's multitasking" code that used to be needed).
 *	  More importantly, it ensures that blockages in non-multithreaded
 *	  libraries like SSL or PAM cannot cause denial of service to other
 *	  clients.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/postmaster/postmaster.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 * NOTES
 *
 * Initialization:
 *		The Postmaster sets up a few shared memory data structures
 *		for the backends.  It should at the very least initialize the
 *		lock manager.
 *
 * Synchronization:
 *		The Postmaster shares memory with the backends but should avoid
 *		touching shared memory, so as not to become stuck if a crashing
 *		backend screws up locks or shared memory.  Likewise, the Postmaster
 *		should never block on messages from frontend clients.
 *
 * Garbage Collection:
 *		The Postmaster cleans up after backends if they have an emergency
 *		exit and/or core dump.
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <limits.h>

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include "catalog/pg_database.h"
#include "commands/async.h"
#include "lib/dllist.h"
#include "libpq/auth.h"
#include "libpq/crypt.h"
#include "libpq/libpq.h"
#include "libpq/pqcomm.h"
#include "libpq/pqsignal.h"
#include "miscadmin.h"
#include "nodes/nodes.h"
#include "storage/fd.h"
#include "storage/ipc.h"
#include "storage/pmsignal.h"
#include "storage/proc.h"
#include "access/xlog.h"
#include "tcop/tcopprot.h"
#include "utils/exc.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "bootstrap/bootstrap.h"

#include "pgstat.h"

#define INVALID_SOCK	(-1)
#define ARGV_SIZE	64

#ifdef HAVE_SIGPROCMASK
sigset_t	UnBlockSig,
			BlockSig,
			AuthBlockSig;

#else
int			UnBlockSig,
			BlockSig,
			AuthBlockSig;
#endif

/*
 * List of active backends (or child processes anyway; we don't actually
 * know whether a given child has become a backend or is still in the
 * authorization phase).  This is used mainly to keep track of how many
 * children we have and send them appropriate signals when necessary.
 */
typedef struct bkend
{
	pid_t		pid;			/* process id of backend */
	long		cancel_key;		/* cancel key for cancels for this backend */
} Backend;

static Dllist *BackendList;

/* The socket number we are listening for connections on */
int			PostPortNumber;
char	   *UnixSocketDir;
char	   *VirtualHost;

/*
 * MaxBackends is the limit on the number of backends we can start.
 * The default is established by configure, but it can be altered at
 * postmaster start with the postmaster's -N switch.  Note
 * that a larger MaxBackends value will increase the size of the shared
 * memory area as well as cause the postmaster to grab more kernel
 * semaphores, even if you never actually use that many backends.
 */
int			MaxBackends = DEF_MAXBACKENDS;


static char *progname = (char *) NULL;

/*
 * Default Values
 */
static int	ServerSock_INET = INVALID_SOCK;		/* stream socket server */

#ifdef HAVE_UNIX_SOCKETS
static int	ServerSock_UNIX = INVALID_SOCK;		/* stream socket server */
#endif

#ifdef USE_SSL
static SSL_CTX *SSL_context = NULL;		/* Global SSL context */
#endif

/*
 * Set by the -o option
 */
static char ExtraOptions[MAXPGPATH];

/*
 * These globals control the behavior of the postmaster in case some
 * backend dumps core.	Normally, it kills all peers of the dead backend
 * and reinitializes shared memory.  By specifying -s or -n, we can have
 * the postmaster stop (rather than kill) peers and not reinitialize
 * shared data structures.
 */
static bool Reinit = true;
static int	SendStop = false;

/* still more option variables */
bool		NetServer = false;	/* listen on TCP/IP */
bool		EnableSSL = false;
bool		SilentMode = false; /* silent mode (-S) */

int			PreAuthDelay = 0;
int			AuthenticationTimeout = 60;
int			CheckPointTimeout = 300;

bool		HostnameLookup;		/* for ps display */
bool		ShowPortNumber;
bool		Log_connections = false;

/* Startup/shutdown state */
static pid_t StartupPID = 0,
			ShutdownPID = 0,
			CheckPointPID = 0;
static time_t checkpointed = 0;

#define			NoShutdown		0
#define			SmartShutdown	1
#define			FastShutdown	2

static int	Shutdown = NoShutdown;

static bool FatalError = false; /* T if recovering from backend crash */

/*
 * State for assigning random salts and cancel keys.
 * Also, the global MyCancelKey passes the cancel key assigned to a given
 * backend from the postmaster to that backend (via fork).
 */

static unsigned int random_seed = 0;

extern char *optarg;
extern int	optind,
			opterr;

#ifdef HAVE_INT_OPTRESET
extern int	optreset;
#endif

/*
 * postmaster.c - function prototypes
 */
static void pmdaemonize(int argc, char *argv[]);
static Port *ConnCreate(int serverFd);
static void ConnFree(Port *port);
static void reset_shared(unsigned short port);
static void SIGHUP_handler(SIGNAL_ARGS);
static void pmdie(SIGNAL_ARGS);
static void reaper(SIGNAL_ARGS);
static void sigusr1_handler(SIGNAL_ARGS);
static void dummy_handler(SIGNAL_ARGS);
static void CleanupProc(int pid, int exitstatus);
static void LogChildExit(const char *procname, int pid, int exitstatus);
static int	DoBackend(Port *port);
static void ExitPostmaster(int status);
static void usage(const char *);
static int	ServerLoop(void);
static int	BackendStartup(Port *port);
static int	ProcessStartupPacket(Port *port, bool SSLdone);
static void processCancelRequest(Port *port, void *pkt);
static int	initMasks(fd_set *rmask, fd_set *wmask);
static void report_fork_failure_to_client(Port *port, int errnum);
enum CAC_state
{
	CAC_OK, CAC_STARTUP, CAC_SHUTDOWN, CAC_RECOVERY, CAC_TOOMANY
};
static enum CAC_state canAcceptConnections(void);
static long PostmasterRandom(void);
static void RandomSalt(char *cryptSalt, char *md5Salt);
static void SignalChildren(int signal);
static int	CountChildren(void);
static bool CreateOptsFile(int argc, char *argv[]);
static pid_t SSDataBase(int xlop);
static void
postmaster_error(const char *fmt,...)
/* This lets gcc check the format string for consistency. */
__attribute__((format(printf, 1, 2)));

#define StartupDataBase()		SSDataBase(BS_XLOG_STARTUP)
#define CheckPointDataBase()	SSDataBase(BS_XLOG_CHECKPOINT)
#define ShutdownDataBase()		SSDataBase(BS_XLOG_SHUTDOWN)

#ifdef USE_SSL
static void InitSSL(void);
static const char *SSLerrmessage(void);
#endif


static void
checkDataDir(const char *checkdir)
{
	char		path[MAXPGPATH];
	FILE	   *fp;
#ifndef __CYGWIN__
	struct stat stat_buf;
#endif

	if (checkdir == NULL)
	{
		fprintf(stderr, gettext(
			 "%s does not know where to find the database system data.\n"
								"You must specify the directory that contains the database system\n"
								"either by specifying the -D invocation option or by setting the\n"
								"PGDATA environment variable.\n\n"),
				progname);
		ExitPostmaster(2);
	}

	/*
	 * Check if the directory has group or world access.  If so, reject.
	 *
	 * XXX temporarily suppress check when on Windows, because there may
	 * not be proper support for Unix-y file permissions.  Need to think
	 * of a reasonable check to apply on Windows.
	 */
#ifndef __CYGWIN__

	if (stat(checkdir, &stat_buf) == -1)
	{
		if (errno == ENOENT)
			elog(FATAL, "data directory %s was not found", checkdir);
		else
			elog(FATAL, "could not read permissions of directory %s: %m",
				 checkdir);
	}

	if (stat_buf.st_mode & (S_IRWXG | S_IRWXO))
		elog(FATAL, "data directory %s has group or world access; permissions should be u=rwx (0700)",
			 checkdir);

#endif /* !__CYGWIN__ */

	/* Look for PG_VERSION before looking for pg_control */
	ValidatePgVersion(checkdir);

	snprintf(path, sizeof(path), "%s/global/pg_control", checkdir);

	fp = AllocateFile(path, PG_BINARY_R);
	if (fp == NULL)
	{
		fprintf(stderr, gettext(
								"%s does not find the database system.\n"
				  "Expected to find it in the PGDATA directory \"%s\",\n"
								"but unable to open file \"%s\": %s\n\n"),
				progname, checkdir, path, strerror(errno));
		ExitPostmaster(2);
	}
	FreeFile(fp);
}


int
PostmasterMain(int argc, char *argv[])
{
	int			opt;
	int			status;
	char		original_extraoptions[MAXPGPATH];
	char	   *potential_DataDir = NULL;

	*original_extraoptions = '\0';

	progname = argv[0];

	/*
	 * Catch standard options before doing much else.  This even works on
	 * systems without getopt_long.
	 */
	if (argc > 1)
	{
		if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0)
		{
			usage(progname);
			ExitPostmaster(0);
		}
		if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0)
		{
			puts("postmaster (PostgreSQL) " PG_VERSION);
			ExitPostmaster(0);
		}
	}

	/*
	 * for security, no dir or file created can be group or other
	 * accessible
	 */
	umask((mode_t) 0077);

	MyProcPid = getpid();

	/*
	 * Fire up essential subsystems: error and memory management
	 */
	EnableExceptionHandling(true);
	MemoryContextInit();

	/*
	 * By default, palloc() requests in the postmaster will be allocated
	 * in the PostmasterContext, which is space that can be recycled by
	 * backends.  Allocated data that needs to be available to backends
	 * should be allocated in TopMemoryContext.
	 */
	PostmasterContext = AllocSetContextCreate(TopMemoryContext,
											  "Postmaster",
											  ALLOCSET_DEFAULT_MINSIZE,
											  ALLOCSET_DEFAULT_INITSIZE,
											  ALLOCSET_DEFAULT_MAXSIZE);
	MemoryContextSwitchTo(PostmasterContext);

	/*
	 * Options setup
	 */
	ResetAllOptions(true);

	/* PGPORT environment variable, if set, overrides GUC setting */
	if (getenv("PGPORT"))
		SetConfigOption("port", getenv("PGPORT"), PGC_POSTMASTER, true);

	potential_DataDir = getenv("PGDATA");		/* default value */

	/*
	 * First we must scan for a -D argument to get the data dir. Then read
	 * the config file. Finally, scan all the other arguments. (Command
	 * line switches override config file.)
	 *
	 * Note: The two lists of options must be exactly the same, even though
	 * perhaps the first one would only have to be "D:" with opterr turned
	 * off. But some versions of getopt (notably GNU) are going to
	 * arbitrarily permute some "non-options" (according to the local
	 * world view) which will result in some switches being associated
	 * with the wrong argument. Death and destruction will occur.
	 */
	opterr = 1;
	while ((opt = getopt(argc, argv, "A:a:B:b:c:D:d:Fh:ik:lm:MN:no:p:Ss-:")) != -1)
	{
		switch (opt)
		{
			case 'D':
				potential_DataDir = optarg;
				break;

			case '?':
				fprintf(stderr, gettext("Try '%s --help' for more information.\n"), progname);
				ExitPostmaster(1);
		}
	}

	/*
	 * Postmaster accepts no non-option switch arguments.
	 */
	if (optind < argc)
	{
		postmaster_error("invalid argument -- %s", argv[optind]);
		fprintf(stderr, gettext("Try '%s --help' for more information.\n"),
				progname);
		ExitPostmaster(1);
	}

	checkDataDir(potential_DataDir);	/* issues error messages */
	SetDataDir(potential_DataDir);

	ProcessConfigFile(PGC_POSTMASTER);

	IgnoreSystemIndexes(false);

	/* reset getopt(3) to rescan arguments */
	optind = 1;
#ifdef HAVE_INT_OPTRESET
	optreset = 1;				/* some systems need this too */
#endif

	while ((opt = getopt(argc, argv, "A:a:B:b:c:D:d:Fh:ik:lm:MN:no:p:Ss-:")) != -1)
	{
		switch (opt)
		{
			case 'A':
#ifdef USE_ASSERT_CHECKING
				SetConfigOption("debug_assertions", optarg, PGC_POSTMASTER, true);
#else
				postmaster_error("Assert checking is not compiled in.");
#endif
				break;
			case 'a':
				/* Can no longer set authentication method. */
				break;
			case 'B':
				SetConfigOption("shared_buffers", optarg, PGC_POSTMASTER, true);
				break;
			case 'b':
				/* Can no longer set the backend executable file to use. */
				break;
			case 'D':
				/* already done above */
				break;
			case 'd':

				/*
				 * Turn on debugging for the postmaster and the backend
				 * servers descended from it.
				 */
				SetConfigOption("debug_level", optarg, PGC_POSTMASTER, true);
				break;
			case 'F':
				SetConfigOption("fsync", "false", PGC_POSTMASTER, true);
				break;
			case 'h':
				SetConfigOption("virtual_host", optarg, PGC_POSTMASTER, true);
				break;
			case 'i':
				SetConfigOption("tcpip_socket", "true", PGC_POSTMASTER, true);
				break;
			case 'k':
				SetConfigOption("unix_socket_directory", optarg, PGC_POSTMASTER, true);
				break;
#ifdef USE_SSL
			case 'l':
				SetConfigOption("ssl", "true", PGC_POSTMASTER, true);
				break;
#endif
			case 'm':
				/* Multiplexed backends no longer supported. */
				break;
			case 'M':

				/*
				 * ignore this flag.  This may be passed in because the
				 * program was run as 'postgres -M' instead of
				 * 'postmaster'
				 */
				break;
			case 'N':
				/* The max number of backends to start. */
				SetConfigOption("max_connections", optarg, PGC_POSTMASTER, true);
				break;
			case 'n':
				/* Don't reinit shared mem after abnormal exit */
				Reinit = false;
				break;
			case 'o':

				/*
				 * Other options to pass to the backend on the command
				 * line -- useful only for debugging.
				 */
				strcat(ExtraOptions, " ");
				strcat(ExtraOptions, optarg);
				strcpy(original_extraoptions, optarg);
				break;
			case 'p':
				SetConfigOption("port", optarg, PGC_POSTMASTER, true);
				break;
			case 'S':

				/*
				 * Start in 'S'ilent mode (disassociate from controlling
				 * tty). You may also think of this as 'S'ysV mode since
				 * it's most badly needed on SysV-derived systems like
				 * SVR4 and HP-UX.
				 */
				SetConfigOption("silent_mode", "true", PGC_POSTMASTER, true);
				break;
			case 's':

				/*
				 * In the event that some backend dumps core, send
				 * SIGSTOP, rather than SIGQUIT, to all its peers.	This
				 * lets the wily post_hacker collect core dumps from
				 * everyone.
				 */
				SendStop = true;
				break;
			case 'c':
			case '-':
				{
					char	   *name,
							   *value;

					ParseLongOption(optarg, &name, &value);
					if (!value)
					{
						if (opt == '-')
							elog(ERROR, "--%s requires argument", optarg);
						else
							elog(ERROR, "-c %s requires argument", optarg);
					}

					SetConfigOption(name, value, PGC_POSTMASTER, true);
					free(name);
					if (value)
						free(value);
					break;
				}

			default:
				/* shouldn't get here */
				fprintf(stderr, gettext("Try '%s --help' for more information.\n"), progname);
				ExitPostmaster(1);
		}
	}

	/*
	 * Check for invalid combinations of switches
	 */
	if (NBuffers < 2 * MaxBackends || NBuffers < 16)
	{
		/*
		 * Do not accept -B so small that backends are likely to starve
		 * for lack of buffers.  The specific choices here are somewhat
		 * arbitrary.
		 */
		postmaster_error("The number of buffers (-B) must be at least twice the number of allowed connections (-N) and at least 16.");
		ExitPostmaster(1);
	}

	/*
	 * Now that we are done processing the postmaster arguments, reset
	 * getopt(3) library so that it will work correctly in subprocesses.
	 */
	optind = 1;
#ifdef HAVE_INT_OPTRESET
	optreset = 1;				/* some systems need this too */
#endif

	/* For debugging: display postmaster environment */
	if (DebugLvl > 2)
	{
		extern char **environ;
		char	  **p;

		fprintf(stderr, "%s: PostmasterMain: initial environ dump:\n",
				progname);
		fprintf(stderr, "-----------------------------------------\n");
		for (p = environ; *p; ++p)
			fprintf(stderr, "\t%s\n", *p);
		fprintf(stderr, "-----------------------------------------\n");
	}

	/*
	 * On some systems our dynloader code needs the executable's pathname.
	 */
	if (FindExec(pg_pathname, progname, "postgres") < 0)
		elog(FATAL, "%s: could not locate executable, bailing out...",
			 progname);

	/*
	 * Initialize SSL library, if specified.
	 */
#ifdef USE_SSL
	if (EnableSSL && !NetServer)
	{
		postmaster_error("For SSL, TCP/IP connections must be enabled.");
		fprintf(stderr, gettext("Try '%s --help' for more information.\n"), progname);
		ExitPostmaster(1);
	}
	if (EnableSSL)
		InitSSL();
#endif

	/*
	 * Fork away from controlling terminal, if -S specified.
	 *
	 * Must do this before we grab any interlock files, else the interlocks
	 * will show the wrong PID.
	 */
	if (SilentMode)
		pmdaemonize(argc, argv);

	/*
	 * Create lockfile for data directory.
	 *
	 * We want to do this before we try to grab the input sockets, because
	 * the data directory interlock is more reliable than the socket-file
	 * interlock (thanks to whoever decided to put socket files in /tmp
	 * :-(). For the same reason, it's best to grab the TCP socket before
	 * the Unix socket.
	 */
	if (!CreateDataDirLockFile(DataDir, true))
		ExitPostmaster(1);

	/*
	 * Remove old temporary files.	At this point there can be no other
	 * Postgres processes running in this directory, so this should be
	 * safe.
	 */
	RemovePgTempFiles();

	/*
	 * Establish input sockets.
	 */
	if (NetServer)
	{
		status = StreamServerPort(AF_INET, VirtualHost,
								  (unsigned short) PostPortNumber,
								  UnixSocketDir,
								  &ServerSock_INET);
		if (status != STATUS_OK)
		{
			postmaster_error("cannot create INET stream port");
			ExitPostmaster(1);
		}
	}

#ifdef HAVE_UNIX_SOCKETS
	status = StreamServerPort(AF_UNIX, VirtualHost,
							  (unsigned short) PostPortNumber,
							  UnixSocketDir,
							  &ServerSock_UNIX);
	if (status != STATUS_OK)
	{
		postmaster_error("cannot create UNIX stream port");
		ExitPostmaster(1);
	}
#endif

	XLOGPathInit();

	/*
	 * Set up shared memory and semaphores.
	 */
	reset_shared(PostPortNumber);

	/*
	 * Initialize the list of active backends.
	 */
	BackendList = DLNewList();

	/*
	 * Record postmaster options.  We delay this till now to avoid
	 * recording bogus options (eg, NBuffers too high for available
	 * memory).
	 */
	if (!CreateOptsFile(argc, argv))
		ExitPostmaster(1);

	/*
	 * Set up signal handlers for the postmaster process.
	 */
	pqinitmask();
	PG_SETMASK(&BlockSig);

	pqsignal(SIGHUP, SIGHUP_handler);	/* reread config file and have
										 * children do same */
	pqsignal(SIGINT, pmdie);	/* send SIGTERM and ShutdownDataBase */
	pqsignal(SIGQUIT, pmdie);	/* send SIGQUIT and die */
	pqsignal(SIGTERM, pmdie);	/* wait for children and ShutdownDataBase */
	pqsignal(SIGALRM, SIG_IGN); /* ignored */
	pqsignal(SIGPIPE, SIG_IGN); /* ignored */
	pqsignal(SIGUSR1, sigusr1_handler); /* message from child process */
	pqsignal(SIGUSR2, dummy_handler);	/* unused, reserve for children */
	pqsignal(SIGCHLD, reaper);	/* handle child termination */
	pqsignal(SIGTTIN, SIG_IGN); /* ignored */
	pqsignal(SIGTTOU, SIG_IGN); /* ignored */

	/*
	 * Reset whereToSendOutput from Debug (its starting state) to None.
	 * This prevents elog from sending messages to stderr unless the
	 * syslog/stderr switch permits.  We don't do this until the
	 * postmaster is fully launched, since startup failures may as well be
	 * reported to stderr.
	 */
	whereToSendOutput = None;

	/*
	 * Initialize and startup the statistics collector process
	 */
	if (pgstat_init() < 0)
		ExitPostmaster(1);
	if (pgstat_start() < 0)
		ExitPostmaster(1);

	/*
	 * Load cached files for client authentication.
	 */
	load_hba_and_ident();
	load_password_cache();

	/*
	 * We're ready to rock and roll...
	 */
	StartupPID = StartupDataBase();

	status = ServerLoop();

	/*
	 * ServerLoop probably shouldn't ever return, but if it does, close
	 * down.
	 */
	ExitPostmaster(status != STATUS_OK);

	return 0;					/* not reached */
}

static void
pmdaemonize(int argc, char *argv[])
{
	int			i;
	pid_t		pid;

	pid = fork();
	if (pid == (pid_t) -1)
	{
		postmaster_error("fork failed: %s", strerror(errno));
		ExitPostmaster(1);
		return;					/* not reached */
	}
	else if (pid)
	{							/* parent */
		/* Parent should just exit, without doing any atexit cleanup */
		_exit(0);
	}

	MyProcPid = getpid();		/* reset MyProcPid to child */

/* GH: If there's no setsid(), we hopefully don't need silent mode.
 * Until there's a better solution.
 */
#ifdef HAVE_SETSID
	if (setsid() < 0)
	{
		postmaster_error("cannot disassociate from controlling TTY: %s",
						 strerror(errno));
		ExitPostmaster(1);
	}
#endif
	i = open(NULL_DEV, O_RDWR | PG_BINARY);
	dup2(i, 0);
	dup2(i, 1);
	dup2(i, 2);
	close(i);
}



/*
 * Print out help message
 */
static void
usage(const char *progname)
{
	printf(gettext("%s is the PostgreSQL server.\n\n"), progname);
	printf(gettext("Usage:\n  %s [options...]\n\n"), progname);
	printf(gettext("Options:\n"));
#ifdef USE_ASSERT_CHECKING
	printf(gettext("  -A 1|0          enable/disable run-time assert checking\n"));
#endif
	printf(gettext("  -B NBUFFERS     number of shared buffers (default %d)\n"), DEF_NBUFFERS);
	printf(gettext("  -c NAME=VALUE   set run-time parameter\n"));
	printf(gettext("  -d 1-5          debugging level\n"));
	printf(gettext("  -D DATADIR      database directory\n"));
	printf(gettext("  -F              turn fsync off\n"));
	printf(gettext("  -h HOSTNAME     host name or IP address to listen on\n"));
	printf(gettext("  -i              enable TCP/IP connections\n"));
	printf(gettext("  -k DIRECTORY    Unix-domain socket location\n"));
#ifdef USE_SSL
	printf(gettext("  -l              enable SSL connections\n"));
#endif
	printf(gettext("  -N MAX-CONNECT  maximum number of allowed connections (default %d)\n"),
		   DEF_MAXBACKENDS);
	printf(gettext("  -o OPTIONS      pass 'OPTIONS' to each backend server\n"));
	printf(gettext("  -p PORT         port number to listen on (default %d)\n"), DEF_PGPORT);
	printf(gettext("  -S              silent mode (start in background without logging output)\n"));

	printf(gettext("\nDeveloper options:\n"));
	printf(gettext("  -n              do not reinitialize shared memory after abnormal exit\n"));
	printf(gettext("  -s              send SIGSTOP to all backend servers if one dies\n"));

	printf(gettext("\nPlease read the documentation for the complete list of run-time\n"
				   "configuration settings and how to set them on the command line or in\n"
				   "the configuration file.\n\n"
				   "Report bugs to <pgsql-bugs@postgresql.org>.\n"));
}

static int
ServerLoop(void)
{
	fd_set		readmask,
				writemask;
	int			nSockets;
	struct timeval now,
				later;
	struct timezone tz;

	gettimeofday(&now, &tz);

	nSockets = initMasks(&readmask, &writemask);

	for (;;)
	{
		Port	   *port;
		fd_set		rmask,
					wmask;
		struct timeval timeout;

		/*
		 * The timeout for the select() below is normally set on the basis
		 * of the time to the next checkpoint.	However, if for some
		 * reason we don't have a next-checkpoint time, time out after 60
		 * seconds. This keeps checkpoint scheduling from locking up when
		 * we get new connection requests infrequently (since we are
		 * likely to detect checkpoint completion just after enabling
		 * signals below, after we've already made the decision about how
		 * long to wait this time).
		 */
		timeout.tv_sec = 60;
		timeout.tv_usec = 0;

		if (CheckPointPID == 0 && checkpointed &&
			Shutdown == NoShutdown && !FatalError && random_seed != 0)
		{
			time_t		now = time(NULL);

			if (CheckPointTimeout + checkpointed > now)
			{
				/*
				 * Not time for checkpoint yet, so set select timeout
				 */
				timeout.tv_sec = CheckPointTimeout + checkpointed - now;
			}
			else
			{
				/* Time to make the checkpoint... */
				CheckPointPID = CheckPointDataBase();

				/*
				 * if fork failed, schedule another try at 0.1 normal
				 * delay
				 */
				if (CheckPointPID == 0)
				{
					timeout.tv_sec = CheckPointTimeout / 10;
					checkpointed = now + timeout.tv_sec - CheckPointTimeout;
				}
			}
		}

		/*
		 * Wait for something to happen.
		 */
		memcpy((char *) &rmask, (char *) &readmask, sizeof(fd_set));
		memcpy((char *) &wmask, (char *) &writemask, sizeof(fd_set));

		PG_SETMASK(&UnBlockSig);

		if (select(nSockets, &rmask, &wmask, (fd_set *) NULL, &timeout) < 0)
		{
			PG_SETMASK(&BlockSig);
			if (errno == EINTR || errno == EWOULDBLOCK)
				continue;
			elog(DEBUG, "ServerLoop: select failed: %m");
			return STATUS_ERROR;
		}

		/*
		 * Block all signals until we wait again.  (This makes it safe for
		 * our signal handlers to do nontrivial work.)
		 */
		PG_SETMASK(&BlockSig);

		/*
		 * Select a random seed at the time of first receiving a request.
		 */
		while (random_seed == 0)
		{
			gettimeofday(&later, &tz);

			/*
			 * We are not sure how much precision is in tv_usec, so we
			 * swap the nibbles of 'later' and XOR them with 'now'. On the
			 * off chance that the result is 0, we loop until it isn't.
			 */
			random_seed = now.tv_usec ^
				((later.tv_usec << 16) |
				 ((later.tv_usec >> 16) & 0xffff));
		}

		/*
		 * New connection pending on our well-known port's socket? If so,
		 * fork a child process to deal with it.
		 */

#ifdef HAVE_UNIX_SOCKETS
		if (ServerSock_UNIX != INVALID_SOCK
			&& FD_ISSET(ServerSock_UNIX, &rmask))
		{
			port = ConnCreate(ServerSock_UNIX);
			if (port)
			{
				BackendStartup(port);

				/*
				 * We no longer need the open socket or port structure in
				 * this process
				 */
				StreamClose(port->sock);
				ConnFree(port);
			}
		}
#endif

		if (ServerSock_INET != INVALID_SOCK
			&& FD_ISSET(ServerSock_INET, &rmask))
		{
			port = ConnCreate(ServerSock_INET);
			if (port)
			{
				BackendStartup(port);

				/*
				 * We no longer need the open socket or port structure in
				 * this process
				 */
				StreamClose(port->sock);
				ConnFree(port);
			}
		}
	}
}


/*
 * Initialise the read and write masks for select() for the well-known ports
 * we are listening on.  Return the number of sockets to listen on.
 */

static int
initMasks(fd_set *rmask, fd_set *wmask)
{
	int			nsocks = -1;

	FD_ZERO(rmask);
	FD_ZERO(wmask);

#ifdef HAVE_UNIX_SOCKETS
	if (ServerSock_UNIX != INVALID_SOCK)
	{
		FD_SET(ServerSock_UNIX, rmask);

		if (ServerSock_UNIX > nsocks)
			nsocks = ServerSock_UNIX;
	}
#endif

	if (ServerSock_INET != INVALID_SOCK)
	{
		FD_SET(ServerSock_INET, rmask);
		if (ServerSock_INET > nsocks)
			nsocks = ServerSock_INET;
	}

	return nsocks + 1;
}


/*
 * Read the startup packet and do something according to it.
 *
 * Returns STATUS_OK or STATUS_ERROR, or might call elog(FATAL) and
 * not return at all.
 *
 * (Note that elog(FATAL) stuff is sent to the client, so only use it
 * if that's what you want.  Return STATUS_ERROR if you don't want to
 * send anything to the client, which would typically be appropriate
 * if we detect a communications failure.)
 */
static int
ProcessStartupPacket(Port *port, bool SSLdone)
{
	StartupPacket *packet;
	enum CAC_state cac;
	int32		len;
	void	   *buf;

	if (pq_getbytes((char *) &len, 4) == EOF)
	{
		elog(DEBUG, "incomplete startup packet");
		return STATUS_ERROR;
	}

	len = ntohl(len);
	len -= 4;

	if (len < sizeof(len) || len > sizeof(len) + sizeof(StartupPacket))
		elog(FATAL, "invalid length of startup packet");

	buf = palloc(len);

	if (pq_getbytes(buf, len) == EOF)
	{
		elog(DEBUG, "incomplete startup packet");
		return STATUS_ERROR;
	}

	packet = buf;

	/*
	 * The first field is either a protocol version number or a special
	 * request code.
	 */
	port->proto = ntohl(packet->protoVersion);

	if (port->proto == CANCEL_REQUEST_CODE)
	{
		processCancelRequest(port, packet);
		return 127;				/* XXX */
	}

	if (port->proto == NEGOTIATE_SSL_CODE && !SSLdone)
	{
		char		SSLok;

#ifdef USE_SSL
		/* No SSL when disabled or on Unix sockets */
		if (!EnableSSL || port->laddr.sa.sa_family != AF_INET)
			SSLok = 'N';
		else
			SSLok = 'S';		/* Support for SSL */
#else
		SSLok = 'N';			/* No support for SSL */
#endif
		if (send(port->sock, &SSLok, 1, 0) != 1)
		{
			elog(DEBUG, "failed to send SSL negotiation response: %s",
				 strerror(errno));
			return STATUS_ERROR;	/* close the connection */
		}

#ifdef USE_SSL
		if (SSLok == 'S')
		{
			if (!(port->ssl = SSL_new(SSL_context)) ||
				!SSL_set_fd(port->ssl, port->sock) ||
				SSL_accept(port->ssl) <= 0)
			{
				elog(DEBUG, "failed to initialize SSL connection: %s (%m)",
					 SSLerrmessage());
				return STATUS_ERROR;
			}
		}
#endif
		/* regular startup packet, cancel, etc packet should follow... */
		/* but not another SSL negotiation request */
		return ProcessStartupPacket(port, true);
	}

	/* Could add additional special packet types here */


	/* Check we can handle the protocol the frontend is using. */

	if (PG_PROTOCOL_MAJOR(port->proto) < PG_PROTOCOL_MAJOR(PG_PROTOCOL_EARLIEST) ||
		PG_PROTOCOL_MAJOR(port->proto) > PG_PROTOCOL_MAJOR(PG_PROTOCOL_LATEST) ||
		(PG_PROTOCOL_MAJOR(port->proto) == PG_PROTOCOL_MAJOR(PG_PROTOCOL_LATEST) &&
		 PG_PROTOCOL_MINOR(port->proto) > PG_PROTOCOL_MINOR(PG_PROTOCOL_LATEST)))
		elog(FATAL, "unsupported frontend protocol");

	/*
	 * Get the parameters from the startup packet as C strings.  The
	 * packet destination was cleared first so a short packet has zeros
	 * silently added and a long packet is silently truncated.
	 */
	StrNCpy(port->database, packet->database, sizeof(port->database));
	StrNCpy(port->user, packet->user, sizeof(port->user));
	StrNCpy(port->options, packet->options, sizeof(port->options));
	StrNCpy(port->tty, packet->tty, sizeof(port->tty));

	/* The database defaults to the user name. */
	if (port->database[0] == '\0')
		StrNCpy(port->database, packet->user, sizeof(port->database));

	/*
	 * Truncate given database and user names to length of a Postgres
	 * name.  This avoids lookup failures when overlength names are given.
	 */
	if ((int) sizeof(port->database) >= NAMEDATALEN)
		port->database[NAMEDATALEN - 1] = '\0';
	if ((int) sizeof(port->user) >= NAMEDATALEN)
		port->user[NAMEDATALEN - 1] = '\0';

	/* Check a user name was given. */
	if (port->user[0] == '\0')
		elog(FATAL, "no PostgreSQL user name specified in startup packet");

	/*
	 * If we're going to reject the connection due to database state, say
	 * so now instead of wasting cycles on an authentication exchange.
	 * (This also allows a pg_ping utility to be written.)
	 */
	cac = canAcceptConnections();

	switch (cac)
	{
		case CAC_STARTUP:
			elog(FATAL, "The database system is starting up");
			break;
		case CAC_SHUTDOWN:
			elog(FATAL, "The database system is shutting down");
			break;
		case CAC_RECOVERY:
			elog(FATAL, "The database system is in recovery mode");
			break;
		case CAC_TOOMANY:
			elog(FATAL, "Sorry, too many clients already");
			break;
		case CAC_OK:
		default:
			;
	}

	return STATUS_OK;
}


/*
 * The client has sent a cancel request packet, not a normal
 * start-a-new-connection packet.  Perform the necessary processing.
 * Nothing is sent back to the client.
 */
static void
processCancelRequest(Port *port, void *pkt)
{
	CancelRequestPacket *canc = (CancelRequestPacket *) pkt;
	int			backendPID;
	long		cancelAuthCode;
	Dlelem	   *curr;
	Backend    *bp;

	backendPID = (int) ntohl(canc->backendPID);
	cancelAuthCode = (long) ntohl(canc->cancelAuthCode);

	if (backendPID == CheckPointPID)
	{
		if (DebugLvl)
			elog(DEBUG, "processCancelRequest: CheckPointPID in cancel request for process %d", backendPID);
		return;
	}

	/* See if we have a matching backend */

	for (curr = DLGetHead(BackendList); curr; curr = DLGetSucc(curr))
	{
		bp = (Backend *) DLE_VAL(curr);
		if (bp->pid == backendPID)
		{
			if (bp->cancel_key == cancelAuthCode)
			{
				/* Found a match; signal that backend to cancel current op */
				if (DebugLvl)
					elog(DEBUG, "processing cancel request: sending SIGINT to process %d",
						 backendPID);
				kill(bp->pid, SIGINT);
			}
			else
			{
				/* Right PID, wrong key: no way, Jose */
				if (DebugLvl)
					elog(DEBUG, "bad key in cancel request for process %d",
						 backendPID);
			}
			return;
		}
	}

	/* No matching backend */
	if (DebugLvl)
		elog(DEBUG, "bad pid in cancel request for process %d", backendPID);
}

/*
 * canAcceptConnections --- check to see if database state allows connections.
 */
static enum CAC_state
canAcceptConnections(void)
{
	/* Can't start backends when in startup/shutdown/recovery state. */
	if (Shutdown > NoShutdown)
		return CAC_SHUTDOWN;
	if (StartupPID)
		return CAC_STARTUP;
	if (FatalError)
		return CAC_RECOVERY;

	/*
	 * Don't start too many children.
	 *
	 * We allow more connections than we can have backends here because some
	 * might still be authenticating; they might fail auth, or some
	 * existing backend might exit before the auth cycle is completed. The
	 * exact MaxBackends limit is enforced when a new backend tries to
	 * join the shared-inval backend array.
	 */
	if (CountChildren() >= 2 * MaxBackends)
		return CAC_TOOMANY;

	return CAC_OK;
}


/*
 * ConnCreate -- create a local connection data structure
 */
static Port *
ConnCreate(int serverFd)
{
	Port	   *port;

	if (!(port = (Port *) calloc(1, sizeof(Port))))
	{
		elog(DEBUG, "ConnCreate: malloc failed");
		SignalChildren(SIGQUIT);
		ExitPostmaster(1);
	}

	if (StreamConnection(serverFd, port) != STATUS_OK)
	{
		StreamClose(port->sock);
		ConnFree(port);
		port = NULL;
	}
	else
	{
		/*
		 * Precompute password salt values to use for this connection.
		 * It's slightly annoying to do this long in advance of knowing
		 * whether we'll need 'em or not, but we must do the random()
		 * calls before we fork, not after.  Else the postmaster's random
		 * sequence won't get advanced, and all backends would end up
		 * using the same salt...
		 */
		RandomSalt(port->cryptSalt, port->md5Salt);
	}

	return port;
}


/*
 * ConnFree -- free a local connection data structure
 */
static void
ConnFree(Port *conn)
{
#ifdef USE_SSL
	if (conn->ssl)
		SSL_free(conn->ssl);
#endif
	free(conn);
}


/*
 * ClosePostmasterPorts -- close all the postmaster's open sockets
 *
 * This is called during child process startup to release file descriptors
 * that are not needed by that child process.  The postmaster still has
 * them open, of course.
 */
void
ClosePostmasterPorts(bool pgstat_too)
{
	/* Close the listen sockets */
	if (NetServer)
		StreamClose(ServerSock_INET);
	ServerSock_INET = INVALID_SOCK;
#ifdef HAVE_UNIX_SOCKETS
	StreamClose(ServerSock_UNIX);
	ServerSock_UNIX = INVALID_SOCK;
#endif
	/* Close pgstat control sockets, unless we're starting pgstat itself */
	if (pgstat_too)
		pgstat_close_sockets();
}


/*
 * reset_shared -- reset shared memory and semaphores
 */
static void
reset_shared(unsigned short port)
{
	/*
	 * Reset assignment of shared mem and semaphore IPC keys. Doing this
	 * means that in normal cases we'll assign the same keys on each
	 * "cycle of life", and thereby avoid leaving dead IPC objects
	 * floating around if the postmaster crashes and is restarted.
	 */
	IpcInitKeyAssignment(port);

	/*
	 * Create or re-create shared memory and semaphores.
	 */
	CreateSharedMemoryAndSemaphores(false, MaxBackends);
}


/*
 * SIGHUP -- reread config files, and tell children to do same
 */
static void
SIGHUP_handler(SIGNAL_ARGS)
{
	int			save_errno = errno;

	PG_SETMASK(&BlockSig);

	if (Shutdown <= SmartShutdown)
	{
		SignalChildren(SIGHUP);
		ProcessConfigFile(PGC_SIGHUP);
		load_hba_and_ident();
	}

	PG_SETMASK(&UnBlockSig);

	errno = save_errno;
}



/*
 * pmdie -- signal handler for processing various postmaster signals.
 */
static void
pmdie(SIGNAL_ARGS)
{
	int			save_errno = errno;

	PG_SETMASK(&BlockSig);

	if (DebugLvl >= 1)
		elog(DEBUG, "pmdie %d", postgres_signal_arg);

	switch (postgres_signal_arg)
	{
		case SIGTERM:

			/*
			 * Smart Shutdown:
			 *
			 * Wait for children to end their work and ShutdownDataBase.
			 */
			if (Shutdown >= SmartShutdown)
				break;
			Shutdown = SmartShutdown;
			elog(DEBUG, "smart shutdown request");
			if (DLGetHead(BackendList)) /* let reaper() handle this */
				break;

			/*
			 * No children left. Shutdown data base system.
			 */
			if (StartupPID > 0 || FatalError)	/* let reaper() handle
												 * this */
				break;
			if (ShutdownPID > 0)
			{
				elog(REALLYFATAL, "shutdown process %d already running",
					 ShutdownPID);
				abort();
			}

			ShutdownPID = ShutdownDataBase();
			break;

		case SIGINT:

			/*
			 * Fast Shutdown:
			 *
			 * abort all children with SIGTERM (rollback active transactions
			 * and exit) and ShutdownDataBase when they are gone.
			 */
			if (Shutdown >= FastShutdown)
				break;
			elog(DEBUG, "fast shutdown request");
			if (DLGetHead(BackendList)) /* let reaper() handle this */
			{
				Shutdown = FastShutdown;
				if (!FatalError)
				{
					elog(DEBUG, "aborting any active transactions");
					SignalChildren(SIGTERM);
				}
				break;
			}
			if (Shutdown > NoShutdown)
			{
				Shutdown = FastShutdown;
				break;
			}
			Shutdown = FastShutdown;

			/*
			 * No children left. Shutdown data base system.
			 */
			if (StartupPID > 0 || FatalError)	/* let reaper() handle
												 * this */
				break;
			if (ShutdownPID > 0)
			{
				elog(REALLYFATAL, "shutdown process %d already running",
					 ShutdownPID);
				abort();
			}

			ShutdownPID = ShutdownDataBase();
			break;

		case SIGQUIT:

			/*
			 * Immediate Shutdown:
			 *
			 * abort all children with SIGQUIT and exit without attempt to
			 * properly shutdown data base system.
			 */
			elog(DEBUG, "immediate shutdown request");
			if (ShutdownPID > 0)
				kill(ShutdownPID, SIGQUIT);
			if (StartupPID > 0)
				kill(StartupPID, SIGQUIT);
			if (DLGetHead(BackendList))
				SignalChildren(SIGQUIT);
			ExitPostmaster(0);
			break;
	}

	PG_SETMASK(&UnBlockSig);

	errno = save_errno;
}

/*
 * Reaper -- signal handler to cleanup after a backend (child) dies.
 */
static void
reaper(SIGNAL_ARGS)
{
	int			save_errno = errno;

#ifdef HAVE_WAITPID
	int			status;			/* backend exit status */

#else
	union wait	status;			/* backend exit status */
#endif
	int			exitstatus;
	int			pid;			/* process id of dead backend */

	PG_SETMASK(&BlockSig);

	if (DebugLvl)
		elog(DEBUG, "reaping dead processes");
#ifdef HAVE_WAITPID
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		exitstatus = status;
#else
	while ((pid = wait3(&status, WNOHANG, NULL)) > 0)
	{
		exitstatus = status.w_status;
#endif

		/*
		 * Check if this child was the statistics collector. If so, start
		 * a new one.
		 */
		if (pgstat_ispgstat(pid))
		{
			LogChildExit(gettext("statistics collector process"),
						 pid, exitstatus);
			pgstat_start();
			continue;
		}

		/*
		 * Check if this child was a shutdown or startup process.
		 */
		if (ShutdownPID > 0 && pid == ShutdownPID)
		{
			if (exitstatus != 0)
			{
				LogChildExit(gettext("shutdown process"),
							 pid, exitstatus);
				ExitPostmaster(1);
			}
			ExitPostmaster(0);
		}

		if (StartupPID > 0 && pid == StartupPID)
		{
			if (exitstatus != 0)
			{
				LogChildExit(gettext("startup process"),
							 pid, exitstatus);
				elog(DEBUG, "aborting startup due to startup process failure");
				ExitPostmaster(1);
			}
			StartupPID = 0;
			FatalError = false; /* done with recovery */
			if (Shutdown > NoShutdown)
			{
				if (ShutdownPID > 0)
				{
					elog(STOP, "startup process %d died while shutdown process %d already running",
						 pid, ShutdownPID);
					abort();
				}
				ShutdownPID = ShutdownDataBase();
			}

			/*
			 * Startup succeeded - remember its ID and RedoRecPtr
			 */
			SetThisStartUpID();

			/*
			 * Arrange for first checkpoint to occur after standard delay.
			 */
			CheckPointPID = 0;
			checkpointed = time(NULL);

			goto reaper_done;
		}

		CleanupProc(pid, exitstatus);
	}

	if (FatalError)
	{
		/*
		 * Wait for all children exit, then reset shmem and
		 * StartupDataBase.
		 */
		if (DLGetHead(BackendList) || StartupPID > 0 || ShutdownPID > 0)
			goto reaper_done;
		elog(DEBUG, "all server processes terminated; reinitializing shared memory and semaphores");

		shmem_exit(0);
		reset_shared(PostPortNumber);

		StartupPID = StartupDataBase();

		goto reaper_done;
	}

	if (Shutdown > NoShutdown)
	{
		if (DLGetHead(BackendList))
			goto reaper_done;
		if (StartupPID > 0 || ShutdownPID > 0)
			goto reaper_done;
		ShutdownPID = ShutdownDataBase();
	}

reaper_done:
	PG_SETMASK(&UnBlockSig);

	errno = save_errno;
}

/*
 * CleanupProc -- cleanup after terminated backend.
 *
 * Remove all local state associated with backend.
 */
static void
CleanupProc(int pid,
			int exitstatus)		/* child's exit status. */
{
	Dlelem	   *curr,
			   *next;
	Backend    *bp;

	if (DebugLvl)
		LogChildExit(gettext("child process"), pid, exitstatus);

	/*
	 * If a backend dies in an ugly way (i.e. exit status not 0) then we
	 * must signal all other backends to quickdie.	If exit status is zero
	 * we assume everything is hunky dory and simply remove the backend
	 * from the active backend list.
	 */
	if (exitstatus == 0)
	{
		curr = DLGetHead(BackendList);
		while (curr)
		{
			bp = (Backend *) DLE_VAL(curr);
			if (bp->pid == pid)
			{
				DLRemove(curr);
				free(bp);
				DLFreeElem(curr);
				break;
			}
			curr = DLGetSucc(curr);
		}

		if (pid == CheckPointPID)
		{
			CheckPointPID = 0;
			if (!FatalError)
			{
				checkpointed = time(NULL);
				/* Update RedoRecPtr for future child backends */
				GetSavedRedoRecPtr();
			}
		}
		else
			pgstat_beterm(pid);

		return;
	}

	/* below here we're dealing with a non-normal exit */

	/* Make log entry unless we did so already */
	if (!FatalError)
	{
		LogChildExit(gettext("server process"), pid, exitstatus);
		elog(DEBUG, "terminating any other active server processes");
	}

	curr = DLGetHead(BackendList);
	while (curr)
	{
		next = DLGetSucc(curr);
		bp = (Backend *) DLE_VAL(curr);
		if (bp->pid != pid)
		{
			/*
			 * This backend is still alive.  Unless we did so already,
			 * tell it to commit hara-kiri.
			 *
			 * SIGQUIT is the special signal that says exit without proc_exit
			 * and let the user know what's going on. But if SendStop is
			 * set (-s on command line), then we send SIGSTOP instead, so
			 * that we can get core dumps from all backends by hand.
			 */
			if (!FatalError)
			{
				if (DebugLvl)
					elog(DEBUG, "CleanupProc: sending %s to process %d",
						 (SendStop ? "SIGSTOP" : "SIGQUIT"),
						 bp->pid);
				kill(bp->pid, (SendStop ? SIGSTOP : SIGQUIT));
			}
		}
		else
		{
			/*
			 * Found entry for freshly-dead backend, so remove it.
			 */
			DLRemove(curr);
			free(bp);
			DLFreeElem(curr);
		}
		curr = next;
	}

	if (pid == CheckPointPID)
	{
		CheckPointPID = 0;
		checkpointed = 0;
	}
	else
	{
		/*
		 * Tell the collector about backend termination
		 */
		pgstat_beterm(pid);
	}

	FatalError = true;
}

/*
 * Log the death of a child process.
 */
static void
LogChildExit(const char *procname, int pid, int exitstatus)
{
	/*
	 * translator: the first %s in these messages is a noun phrase
	 * describing a child process, such as "server process"
	 */
	if (WIFEXITED(exitstatus))
		elog(DEBUG, "%s (pid %d) exited with exit code %d",
			 procname, pid, WEXITSTATUS(exitstatus));
	else if (WIFSIGNALED(exitstatus))
		elog(DEBUG, "%s (pid %d) was terminated by signal %d",
			 procname, pid, WTERMSIG(exitstatus));
	else
		elog(DEBUG, "%s (pid %d) exited with unexpected status %d",
			 procname, pid, exitstatus);
}

/*
 * Send a signal to all backend children.
 */
static void
SignalChildren(int signal)
{
	Dlelem	   *curr,
			   *next;
	Backend    *bp;
	int			mypid = getpid();

	curr = DLGetHead(BackendList);
	while (curr)
	{
		next = DLGetSucc(curr);
		bp = (Backend *) DLE_VAL(curr);

		if (bp->pid != mypid)
		{
			if (DebugLvl >= 1)
				elog(DEBUG, "SignalChildren: sending signal %d to process %d",
					 signal, bp->pid);

			kill(bp->pid, signal);
		}

		curr = next;
	}
}

/*
 * BackendStartup -- start backend process
 *
 * returns: STATUS_ERROR if the fork/exec failed, STATUS_OK otherwise.
 */
static int
BackendStartup(Port *port)
{
	Backend    *bn;				/* for backend cleanup */
	pid_t		pid;

	/*
	 * Compute the cancel key that will be assigned to this backend. The
	 * backend will have its own copy in the forked-off process' value of
	 * MyCancelKey, so that it can transmit the key to the frontend.
	 */
	MyCancelKey = PostmasterRandom();

	/*
	 * Make room for backend data structure.  Better before the fork() so
	 * we can handle failure cleanly.
	 */
	bn = (Backend *) malloc(sizeof(Backend));
	if (!bn)
	{
		elog(DEBUG, "out of memory; connection startup aborted");
		return STATUS_ERROR;
	}

	/*
	 * Flush stdio channels just before fork, to avoid double-output
	 * problems. Ideally we'd use fflush(NULL) here, but there are still a
	 * few non-ANSI stdio libraries out there (like SunOS 4.1.x) that
	 * coredump if we do. Presently stdout and stderr are the only stdio
	 * output channels used by the postmaster, so fflush'ing them should
	 * be sufficient.
	 */
	fflush(stdout);
	fflush(stderr);

#ifdef __BEOS__
	/* Specific beos actions before backend startup */
	beos_before_backend_startup();
#endif

	pid = fork();

	if (pid == 0)				/* child */
	{
		int			status;

#ifdef __BEOS__
		/* Specific beos backend startup actions */
		beos_backend_startup();
#endif
		free(bn);

		status = DoBackend(port);
		if (status != 0)
		{
			elog(DEBUG, "connection startup failed");
			proc_exit(status);
		}
		else
			proc_exit(0);
	}

	/* in parent, error */
	if (pid < 0)
	{
		int			save_errno = errno;

#ifdef __BEOS__
		/* Specific beos backend startup actions */
		beos_backend_startup_failed();
#endif
		free(bn);
		elog(DEBUG, "connection startup failed (fork failure): %s",
			 strerror(save_errno));
		report_fork_failure_to_client(port, save_errno);
		return STATUS_ERROR;
	}

	/* in parent, normal */
	if (DebugLvl >= 1)
		elog(DEBUG, "BackendStartup: forked pid=%d socket=%d",
			 pid, port->sock);

	/*
	 * Everything's been successful, it's safe to add this backend to our
	 * list of backends.
	 */
	bn->pid = pid;
	bn->cancel_key = MyCancelKey;
	DLAddHead(BackendList, DLNewElem(bn));

	return STATUS_OK;
}


/*
 * Try to report backend fork() failure to client before we close the
 * connection.  Since we do not care to risk blocking the postmaster on
 * this connection, we set the connection to non-blocking and try only once.
 *
 * This is grungy special-purpose code; we cannot use backend libpq since
 * it's not up and running.
 */
static void
report_fork_failure_to_client(Port *port, int errnum)
{
	char		buffer[1000];
#ifdef __BEOS__
	int			on = 1;
#endif

	/* Format the error message packet */
	snprintf(buffer, sizeof(buffer), "E%s%s\n",
			 gettext("Server process fork() failed: "),
			 strerror(errnum));

	/* Set port to non-blocking.  Don't do send() if this fails */
#ifdef __BEOS__
	if (ioctl(port->sock, FIONBIO, &on) != 0)
		return;
#else
	if (fcntl(port->sock, F_SETFL, O_NONBLOCK) < 0)
		return;
#endif

	send(port->sock, buffer, strlen(buffer)+1, 0);
}


/*
 * split_opts -- split a string of options and append it to an argv array
 *
 * NB: the string is destructively modified!
 *
 * Since no current POSTGRES arguments require any quoting characters,
 * we can use the simple-minded tactic of assuming each set of space-
 * delimited characters is a separate argv element.
 *
 * If you don't like that, well, we *used* to pass the whole option string
 * as ONE argument to execl(), which was even less intelligent...
 */
static void
split_opts(char **argv, int *argcp, char *s)
{
	while (s && *s)
	{
		while (isspace((unsigned char) *s))
			++s;
		if (*s == '\0')
			break;
		argv[(*argcp)++] = s;
		while (*s && !isspace((unsigned char) *s))
			++s;
		if (*s)
			*s++ = '\0';
	}
}

/*
 * DoBackend -- perform authentication, and if successful, set up the
 *		backend's argument list and invoke backend main().
 *
 * This used to perform an execv() but we no longer exec the backend;
 * it's the same executable as the postmaster.
 *
 * returns:
 *		Shouldn't return at all.
 *		If PostgresMain() fails, return status.
 */
static int
DoBackend(Port *port)
{
	char	   *remote_host;
	char	   *av[ARGV_SIZE * 2];
	int			ac = 0;
	char		debugbuf[ARGV_SIZE];
	char		protobuf[ARGV_SIZE];
	char		dbbuf[ARGV_SIZE];
	char		optbuf[ARGV_SIZE];
	char		ttybuf[ARGV_SIZE];
	int			i;
	int			status;
	struct timeval now;
	struct timezone tz;

	/*
	 * Let's clean up ourselves as the postmaster child
	 */

	IsUnderPostmaster = true;	/* we are a postmaster subprocess now */

	/* We don't want the postmaster's proc_exit() handlers */
	on_exit_reset();

	/*
	 * Signal handlers setting is moved to tcop/postgres...
	 */

	/* Close the postmaster's other sockets */
	ClosePostmasterPorts(true);

	/* Save port etc. for ps status */
	MyProcPort = port;

	/* Reset MyProcPid to new backend's pid */
	MyProcPid = getpid();

	/*
	 * Initialize libpq and enable reporting of elog errors to the client.
	 * Must do this now because authentication uses libpq to send
	 * messages.
	 */
	pq_init();					/* initialize libpq to talk to client */
	whereToSendOutput = Remote; /* now safe to elog to client */

	/*
	 * We arrange for a simple exit(0) if we receive SIGTERM or SIGQUIT
	 * during any client authentication related communication. Otherwise
	 * the postmaster cannot shutdown the database FAST or IMMED cleanly
	 * if a buggy client blocks a backend during authentication.  We also
	 * will exit(0) after a time delay, so that a broken client can't hog
	 * a connection indefinitely.
	 *
	 * PreAuthDelay is a debugging aid for investigating problems in the
	 * authentication cycle: it can be set in postgresql.conf to allow
	 * time to attach to the newly-forked backend with a debugger. (See
	 * also the -W backend switch, which we allow clients to pass through
	 * PGOPTIONS, but it is not honored until after authentication.)
	 */
	pqsignal(SIGTERM, authdie);
	pqsignal(SIGQUIT, authdie);
	pqsignal(SIGALRM, authdie);
	PG_SETMASK(&AuthBlockSig);

	if (PreAuthDelay > 0)
		sleep(PreAuthDelay);

	if (!enable_sigalrm_interrupt(AuthenticationTimeout * 1000))
		elog(FATAL, "DoBackend: Unable to set timer for auth timeout");

	/*
	 * Receive the startup packet (which might turn out to be a cancel
	 * request packet).
	 */
	status = ProcessStartupPacket(port, false);

	if (status != STATUS_OK)
		return 0;				/* cancel request processed, or error */

	/*
	 * Now that we have the user and database name, we can set the process
	 * title for ps.  It's good to do this as early as possible in
	 * startup.
	 *
	 * But first, we need the remote host name.
	 */
	if (port->raddr.sa.sa_family == AF_INET)
	{
		unsigned short remote_port;
		char	   *host_addr;

		remote_port = ntohs(port->raddr.in.sin_port);
		host_addr = inet_ntoa(port->raddr.in.sin_addr);

		remote_host = NULL;

		if (HostnameLookup)
		{
			struct hostent *host_ent;

			host_ent = gethostbyaddr((char *) &port->raddr.in.sin_addr,
									 sizeof(port->raddr.in.sin_addr),
									 AF_INET);

			if (host_ent)
			{
				remote_host = palloc(strlen(host_addr) + strlen(host_ent->h_name) + 3);
				sprintf(remote_host, "%s[%s]", host_ent->h_name, host_addr);
			}
		}

		if (remote_host == NULL)
			remote_host = pstrdup(host_addr);

		if (ShowPortNumber)
		{
			char	   *str = palloc(strlen(remote_host) + 7);

			sprintf(str, "%s:%hu", remote_host, remote_port);
			pfree(remote_host);
			remote_host = str;
		}
	}
	else
	{
		/* not AF_INET */
		remote_host = "[local]";
	}

	/*
	 * Set process parameters for ps display.
	 */
	init_ps_display(port->user, port->database, remote_host);
	set_ps_display("authentication");

	/*
	 * Now perform authentication exchange.
	 */
	ClientAuthentication(port); /* might not return, if failure */

	/*
	 * Done with authentication.  Disable timeout, and prevent
	 * SIGTERM/SIGQUIT again until backend startup is complete.
	 */
	if (!disable_sigalrm_interrupt())
		elog(FATAL, "DoBackend: Unable to disable timer for auth timeout");
	PG_SETMASK(&BlockSig);

	if (Log_connections)
		elog(DEBUG, "connection: host=%s user=%s database=%s",
			 remote_host, port->user, port->database);

	/*
	 * Don't want backend to be able to see the postmaster random number
	 * generator state.  We have to clobber the static random_seed *and*
	 * start a new random sequence in the random() library function.
	 */
	random_seed = 0;
	gettimeofday(&now, &tz);
	srandom((unsigned int) now.tv_usec);

	/* ----------------
	 * Now, build the argv vector that will be given to PostgresMain.
	 *
	 * The layout of the command line is
	 *		postgres [secure switches] -p databasename [insecure switches]
	 * where the switches after -p come from the client request.
	 * ----------------
	 */

	av[ac++] = "postgres";

	/*
	 * Pass the requested debugging level along to the backend. Level one
	 * debugging in the postmaster traces postmaster connection activity,
	 * and levels two and higher are passed along to the backend.  This
	 * allows us to watch only the postmaster or the postmaster and the
	 * backend.
	 */
	if (DebugLvl > 1)
	{
		sprintf(debugbuf, "-d%d", DebugLvl);
		av[ac++] = debugbuf;
	}

	/*
	 * Pass any backend switches specified with -o in the postmaster's own
	 * command line.  We assume these are secure. (It's OK to mangle
	 * ExtraOptions since we are now in the child process; this won't
	 * change the postmaster's copy.)
	 */
	split_opts(av, &ac, ExtraOptions);

	/* Tell the backend what protocol the frontend is using. */
	sprintf(protobuf, "-v%u", port->proto);
	av[ac++] = protobuf;

	/*
	 * Tell the backend it is being called from the postmaster, and which
	 * database to use.  -p marks the end of secure switches.
	 */
	av[ac++] = "-p";

	StrNCpy(dbbuf, port->database, ARGV_SIZE);
	av[ac++] = dbbuf;

	/*
	 * Pass the (insecure) option switches from the connection request.
	 */
	StrNCpy(optbuf, port->options, ARGV_SIZE);
	split_opts(av, &ac, optbuf);

	/*
	 * Pass the (insecure) debug output file request.
	 *
	 * NOTE: currently, this is useless code, since the backend will not
	 * honor an insecure -o switch.  I left it here since the backend
	 * could be modified to allow insecure -o, given adequate checking
	 * that the specified filename is something safe to write on.
	 */
	if (port->tty[0])
	{
		StrNCpy(ttybuf, port->tty, ARGV_SIZE);
		av[ac++] = "-o";
		av[ac++] = ttybuf;
	}

	av[ac] = (char *) NULL;

	/*
	 * Release postmaster's working memory context so that backend can
	 * recycle the space.  Note this does not trash *MyProcPort, because
	 * ConnCreate() allocated that space with malloc() ... else we'd need
	 * to copy the Port data here.
	 */
	MemoryContextSwitchTo(TopMemoryContext);
	MemoryContextDelete(PostmasterContext);
	PostmasterContext = NULL;

	/*
	 * Debug: print arguments being passed to backend
	 */
	if (DebugLvl > 1)
	{
		fprintf(stderr, "%s child[%d]: starting with (",
				progname, MyProcPid);
		for (i = 0; i < ac; ++i)
			fprintf(stderr, "%s ", av[i]);
		fprintf(stderr, ")\n");
	}

	return (PostgresMain(ac, av, port->user));
}

/*
 * ExitPostmaster -- cleanup
 *
 * Do NOT call exit() directly --- always go through here!
 */
static void
ExitPostmaster(int status)
{
	/* should cleanup shared memory and kill all backends */

	/*
	 * Not sure of the semantics here.	When the Postmaster dies, should
	 * the backends all be killed? probably not.
	 *
	 * MUST		-- vadim 05-10-1999
	 */
	if (ServerSock_INET != INVALID_SOCK)
		StreamClose(ServerSock_INET);
	ServerSock_INET = INVALID_SOCK;
#ifdef HAVE_UNIX_SOCKETS
	if (ServerSock_UNIX != INVALID_SOCK)
		StreamClose(ServerSock_UNIX);
	ServerSock_UNIX = INVALID_SOCK;
#endif

	proc_exit(status);
}

/*
 * sigusr1_handler - handle signal conditions from child processes
 */
static void
sigusr1_handler(SIGNAL_ARGS)
{
	int			save_errno = errno;

	PG_SETMASK(&BlockSig);

	if (CheckPostmasterSignal(PMSIGNAL_DO_CHECKPOINT))
	{
		/*
		 * Request to schedule a checkpoint
		 *
		 * Ignore request if checkpoint is already running or checkpointing
		 * is currently disabled
		 */
		if (CheckPointPID == 0 && checkpointed &&
			Shutdown == NoShutdown && !FatalError && random_seed != 0)
		{
			CheckPointPID = CheckPointDataBase();
			/* note: if fork fails, CheckPointPID stays 0; nothing happens */
		}
	}

	if (CheckPostmasterSignal(PMSIGNAL_PASSWORD_CHANGE))
	{
		/*
		 * Password file has changed.
		 */
		load_password_cache();
	}

	if (CheckPostmasterSignal(PMSIGNAL_WAKEN_CHILDREN))
	{
		/*
		 * Send SIGUSR2 to all children (triggers AsyncNotifyHandler). See
		 * storage/ipc/sinvaladt.c for the use of this.
		 */
		if (Shutdown == NoShutdown)
			SignalChildren(SIGUSR2);
	}

	PG_SETMASK(&UnBlockSig);

	errno = save_errno;
}


/*
 * Dummy signal handler
 *
 * We use this for signals that we don't actually use in the postmaster,
 * but we do use in backends.  If we SIG_IGN such signals in the postmaster,
 * then a newly started backend might drop a signal that arrives before it's
 * able to reconfigure its signal processing.  (See notes in postgres.c.)
 */
static void
dummy_handler(SIGNAL_ARGS)
{
}


/*
 * CharRemap: given an int in range 0..61, produce textual encoding of it
 * per crypt(3) conventions.
 */
static char
CharRemap(long ch)
{
	if (ch < 0)
		ch = -ch;
	ch = ch % 62;

	if (ch < 26)
		return 'A' + ch;

	ch -= 26;
	if (ch < 26)
		return 'a' + ch;

	ch -= 26;
	return '0' + ch;
}

/*
 * RandomSalt
 */
static void
RandomSalt(char *cryptSalt, char *md5Salt)
{
	long		rand = PostmasterRandom();

	cryptSalt[0] = CharRemap(rand % 62);
	cryptSalt[1] = CharRemap(rand / 62);

	/*
	 * It's okay to reuse the first random value for one of the MD5 salt
	 * bytes, since only one of the two salts will be sent to the client.
	 * After that we need to compute more random bits.
	 *
	 * We use % 255, sacrificing one possible byte value, so as to ensure
	 * that all bits of the random() value participate in the result.
	 * While at it, add one to avoid generating any null bytes.
	 */
	md5Salt[0] = (rand % 255) + 1;
	rand = PostmasterRandom();
	md5Salt[1] = (rand % 255) + 1;
	rand = PostmasterRandom();
	md5Salt[2] = (rand % 255) + 1;
	rand = PostmasterRandom();
	md5Salt[3] = (rand % 255) + 1;
}

/*
 * PostmasterRandom
 */
static long
PostmasterRandom(void)
{
	static bool initialized = false;

	if (!initialized)
	{
		Assert(random_seed != 0);
		srandom(random_seed);
		initialized = true;
	}

	return random();
}

/*
 * Count up number of child processes.
 */
static int
CountChildren(void)
{
	Dlelem	   *curr;
	Backend    *bp;
	int			mypid = getpid();
	int			cnt = 0;

	for (curr = DLGetHead(BackendList); curr; curr = DLGetSucc(curr))
	{
		bp = (Backend *) DLE_VAL(curr);
		if (bp->pid != mypid)
			cnt++;
	}
	if (CheckPointPID != 0)
		cnt--;
	return cnt;
}

#ifdef USE_SSL

/*
 * Initialize SSL library and structures
 */
static void
InitSSL(void)
{
	char		fnbuf[2048];

	SSL_load_error_strings();
	SSL_library_init();
	SSL_context = SSL_CTX_new(SSLv23_method());
	if (!SSL_context)
	{
		postmaster_error("failed to create SSL context: %s",
						 SSLerrmessage());
		ExitPostmaster(1);
	}
	snprintf(fnbuf, sizeof(fnbuf), "%s/server.crt", DataDir);
	if (!SSL_CTX_use_certificate_file(SSL_context, fnbuf, SSL_FILETYPE_PEM))
	{
		postmaster_error("failed to load server certificate (%s): %s",
						 fnbuf, SSLerrmessage());
		ExitPostmaster(1);
	}
	snprintf(fnbuf, sizeof(fnbuf), "%s/server.key", DataDir);
	if (!SSL_CTX_use_PrivateKey_file(SSL_context, fnbuf, SSL_FILETYPE_PEM))
	{
		postmaster_error("failed to load private key file (%s): %s",
						 fnbuf, SSLerrmessage());
		ExitPostmaster(1);
	}
	if (!SSL_CTX_check_private_key(SSL_context))
	{
		postmaster_error("check of private key failed: %s",
						 SSLerrmessage());
		ExitPostmaster(1);
	}
}

/*
 * Obtain reason string for last SSL error
 *
 * Some caution is needed here since ERR_reason_error_string will
 * return NULL if it doesn't recognize the error code.  We don't
 * want to return NULL ever.
 */
static const char *
SSLerrmessage(void)
{
	unsigned long	errcode;
	const char	   *errreason;
	static char		errbuf[32];

	errcode = ERR_get_error();
	if (errcode == 0)
		return "No SSL error reported";
	errreason = ERR_reason_error_string(errcode);
	if (errreason != NULL)
		return errreason;
	snprintf(errbuf, sizeof(errbuf), "SSL error code %lu", errcode);
	return errbuf;
}

#endif /* USE_SSL */

/*
 * Fire off a subprocess for startup/shutdown/checkpoint.
 *
 * Return value is subprocess' PID, or 0 if failed to start subprocess
 * (0 is returned only for checkpoint case).
 */
static pid_t
SSDataBase(int xlop)
{
	pid_t		pid;
	Backend    *bn;

	fflush(stdout);
	fflush(stderr);

#ifdef __BEOS__
	/* Specific beos actions before backend startup */
	beos_before_backend_startup();
#endif

	if ((pid = fork()) == 0)	/* child */
	{
		const char *statmsg;
		char	   *av[ARGV_SIZE * 2];
		int			ac = 0;
		char		nbbuf[ARGV_SIZE];
		char		dbbuf[ARGV_SIZE];
		char		xlbuf[ARGV_SIZE];

#ifdef __BEOS__
		/* Specific beos actions after backend startup */
		beos_backend_startup();
#endif

		IsUnderPostmaster = true;		/* we are a postmaster subprocess
										 * now */

		/* Lose the postmaster's on-exit routines and port connections */
		on_exit_reset();

		/* Close the postmaster's sockets */
		ClosePostmasterPorts(true);

		/*
		 * Identify myself via ps
		 */
		switch (xlop)
		{
			case BS_XLOG_STARTUP:
				statmsg = "startup subprocess";
				break;
			case BS_XLOG_CHECKPOINT:
				statmsg = "checkpoint subprocess";
				break;
			case BS_XLOG_SHUTDOWN:
				statmsg = "shutdown subprocess";
				break;
			default:
				statmsg = "??? subprocess";
				break;
		}
		init_ps_display(statmsg, "", "");
		set_ps_display("");

		/* Set up command-line arguments for subprocess */
		av[ac++] = "postgres";

		av[ac++] = "-d";

		sprintf(nbbuf, "-B%d", NBuffers);
		av[ac++] = nbbuf;

		sprintf(xlbuf, "-x%d", xlop);
		av[ac++] = xlbuf;

		av[ac++] = "-p";

		StrNCpy(dbbuf, "template1", ARGV_SIZE);
		av[ac++] = dbbuf;

		av[ac] = (char *) NULL;

		BootstrapMain(ac, av);
		ExitPostmaster(0);
	}

	/* in parent */
	if (pid < 0)
	{
#ifdef __BEOS__
		/* Specific beos actions before backend startup */
		beos_backend_startup_failed();
#endif

		switch (xlop)
		{
			case BS_XLOG_STARTUP:
				elog(DEBUG, "could not launch startup process (fork failure): %s",
					 strerror(errno));
				break;
			case BS_XLOG_CHECKPOINT:
				elog(DEBUG, "could not launch checkpoint process (fork failure): %s",
					 strerror(errno));
				break;
			case BS_XLOG_SHUTDOWN:
			default:
				elog(DEBUG, "could not launch shutdown process (fork failure): %s",
					 strerror(errno));
				break;
		}

		/*
		 * fork failure is fatal during startup/shutdown, but there's no
		 * need to choke if a routine checkpoint fails.
		 */
		if (xlop == BS_XLOG_CHECKPOINT)
			return 0;
		ExitPostmaster(1);
	}

	/*
	 * The startup and shutdown processes are not considered normal
	 * backends, but the checkpoint process is.  Checkpoint must be added
	 * to the list of backends.
	 */
	if (xlop == BS_XLOG_CHECKPOINT)
	{
		if (!(bn = (Backend *) calloc(1, sizeof(Backend))))
		{
			elog(DEBUG, "CheckPointDataBase: malloc failed");
			ExitPostmaster(1);
		}

		bn->pid = pid;
		bn->cancel_key = PostmasterRandom();
		DLAddHead(BackendList, DLNewElem(bn));

		/*
		 * Since this code is executed periodically, it's a fine place to
		 * do other actions that should happen every now and then on no
		 * particular schedule.  Such as...
		 */
		TouchSocketLockFile();
	}

	return pid;
}


/*
 * Create the opts file
 */
static bool
CreateOptsFile(int argc, char *argv[])
{
	char		fullprogname[MAXPGPATH];
	char	   *filename;
	FILE	   *fp;
	unsigned	i;

	if (FindExec(fullprogname, argv[0], "postmaster") < 0)
		return false;

	filename = palloc(strlen(DataDir) + 20);
	sprintf(filename, "%s/postmaster.opts", DataDir);

	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		postmaster_error("cannot create file %s: %s",
						 filename, strerror(errno));
		return false;
	}

	fprintf(fp, "%s", fullprogname);
	for (i = 1; i < argc; i++)
		fprintf(fp, " '%s'", argv[i]);
	fputs("\n", fp);

	if (ferror(fp))
	{
		postmaster_error("writing file %s failed", filename);
		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}


static void
postmaster_error(const char *fmt,...)
{
	va_list		ap;

	fprintf(stderr, "%s: ", progname);
	va_start(ap, fmt);
	vfprintf(stderr, gettext(fmt), ap);
	va_end(ap);
	fprintf(stderr, "\n");
}
