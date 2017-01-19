/*-------------------------------------------------------------------------
 *
 * postinit.c
 *	  postgres initialization utilities
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/init/postinit.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h>

#include "catalog/catalog.h"
#include "access/heapam.h"
#include "catalog/catname.h"
#include "catalog/pg_database.h"
#include "catalog/pg_shadow.h"
#include "commands/trigger.h"
#include "commands/variable.h"	/* for set_default_client_encoding() */
#include "mb/pg_wchar.h"
#include "miscadmin.h"
#include "storage/backendid.h"
#include "storage/proc.h"
#include "storage/sinval.h"
#include "storage/smgr.h"
#include "utils/fmgroids.h"
#include "utils/portal.h"
#include "utils/relcache.h"
#include "utils/syscache.h"
#include "utils/temprel.h"


static void ReverifyMyDatabase(const char *name);
static void InitCommunication(void);
static void ShutdownPostgres(void);
static bool ThereIsAtLeastOneUser(void);


/*** InitPostgres support ***/


/* --------------------------------
 *		ReverifyMyDatabase
 *
 * Since we are forced to fetch the database OID out of pg_database without
 * benefit of locking or transaction ID checking (see utils/misc/database.c),
 * we might have gotten a wrong answer.  Or, we might have attached to a
 * database that's in process of being destroyed by destroydb().  This
 * routine is called after we have all the locking and other infrastructure
 * running --- now we can check that we are really attached to a valid
 * database.
 *
 * In reality, if destroydb() is running in parallel with our startup,
 * it's pretty likely that we will have failed before now, due to being
 * unable to read some of the system tables within the doomed database.
 * This routine just exists to make *sure* we have not started up in an
 * invalid database.  If we quit now, we should have managed to avoid
 * creating any serious problems.
 *
 * This is also a handy place to fetch the database encoding info out
 * of pg_database, if we are in MULTIBYTE mode.
 * --------------------------------
 */
static void
ReverifyMyDatabase(const char *name)
{
	Relation	pgdbrel;
	HeapScanDesc pgdbscan;
	ScanKeyData key;
	HeapTuple	tup;
	Form_pg_database dbform;

	/*
	 * Because we grab AccessShareLock here, we can be sure that destroydb
	 * is not running in parallel with us (any more).
	 */
	pgdbrel = heap_openr(DatabaseRelationName, AccessShareLock);

	ScanKeyEntryInitialize(&key, 0, Anum_pg_database_datname,
						   F_NAMEEQ, NameGetDatum(name));

	pgdbscan = heap_beginscan(pgdbrel, 0, SnapshotNow, 1, &key);

	tup = heap_getnext(pgdbscan, 0);
	if (!HeapTupleIsValid(tup) ||
		tup->t_data->t_oid != MyDatabaseId)
	{
		/* OOPS */
		heap_close(pgdbrel, AccessShareLock);

		/*
		 * The only real problem I could have created is to load dirty
		 * buffers for the dead database into shared buffer cache; if I
		 * did, some other backend will eventually try to write them and
		 * die in mdblindwrt.  Flush any such pages to forestall trouble.
		 */
		DropBuffers(MyDatabaseId);
		/* Now I can commit hara-kiri with a clear conscience... */
		elog(FATAL, "Database \"%s\", OID %u, has disappeared from pg_database",
			 name, MyDatabaseId);
	}

	/*
	 * Also check that the database is currently allowing connections.
	 */
	dbform = (Form_pg_database) GETSTRUCT(tup);
	if (!dbform->datallowconn)
		elog(FATAL, "Database \"%s\" is not currently accepting connections",
			 name);

	/*
	 * OK, we're golden.  Only other to-do item is to save the MULTIBYTE
	 * encoding info out of the pg_database tuple --- or complain, if we
	 * can't support it.
	 */
#ifdef MULTIBYTE
	SetDatabaseEncoding(dbform->encoding);
#else
	if (dbform->encoding != PG_SQL_ASCII)
		elog(FATAL, "database was initialized with MULTIBYTE encoding %d,\n\tbut the backend was compiled without multibyte support.\n\tlooks like you need to initdb or recompile.",
			 dbform->encoding);
#endif

	heap_endscan(pgdbscan);
	heap_close(pgdbrel, AccessShareLock);
}



/* --------------------------------
 *		InitCommunication
 *
 *		This routine initializes stuff needed for ipc, locking, etc.
 *		it should be called something more informative.
 * --------------------------------
 */
static void
InitCommunication(void)
{
	/*
	 * initialize shared memory and semaphores appropriately.
	 */
	if (!IsUnderPostmaster)		/* postmaster already did this */
	{
		/*
		 * we're running a postgres backend by itself with no front end or
		 * postmaster.	Create private "shmem" and semaphores.	Setting
		 * MaxBackends = 16 is arbitrary.
		 */
		CreateSharedMemoryAndSemaphores(true, 16);
	}
}


/*
 * Early initialization of a backend (either standalone or under postmaster).
 * This happens even before InitPostgres.
 *
 * If you're wondering why this is separate from InitPostgres at all:
 * the critical distinction is that this stuff has to happen before we can
 * run XLOG-related initialization, which is done before InitPostgres --- in
 * fact, for cases such as checkpoint creation processes, InitPostgres may
 * never be done at all.
 */
void
BaseInit(void)
{
	/*
	 * Attach to shared memory and semaphores, and initialize our
	 * input/output/debugging file descriptors.
	 */
	InitCommunication();
	DebugFileOpen();

	/* Do local initialization of storage and buffer managers */
	smgrinit();
	InitBufferPoolAccess();
	InitLocalBuffer();
}


/* --------------------------------
 * InitPostgres
 *		Initialize POSTGRES.
 *
 * Note:
 *		Be very careful with the order of calls in the InitPostgres function.
 * --------------------------------
 */
void
InitPostgres(const char *dbname, const char *username)
{
	bool		bootstrap = IsBootstrapProcessingMode();

	/*
	 * Set up the global variables holding database name, id, and path.
	 *
	 * We take a shortcut in the bootstrap case, otherwise we have to look up
	 * the db name in pg_database.
	 */
	SetDatabaseName(dbname);

	if (bootstrap)
	{
		MyDatabaseId = TemplateDbOid;
		SetDatabasePath(GetDatabasePath(MyDatabaseId));
	}
	else
	{
		char	   *fullpath,
					datpath[MAXPGPATH];

		/*
		 * Formerly we validated DataDir here, but now that's done
		 * earlier.
		 */

		/*
		 * Find oid and path of the database we're about to open. Since
		 * we're not yet up and running we have to use the hackish
		 * GetRawDatabaseInfo.
		 */
		GetRawDatabaseInfo(dbname, &MyDatabaseId, datpath);

		if (!OidIsValid(MyDatabaseId))
			elog(FATAL,
				 "Database \"%s\" does not exist in the system catalog.",
				 dbname);

		fullpath = GetDatabasePath(MyDatabaseId);

		/* Verify the database path */

		if (access(fullpath, F_OK) == -1)
			elog(FATAL, "Database \"%s\" does not exist.\n\t"
				 "The database subdirectory '%s' is missing.",
				 dbname, fullpath);

		ValidatePgVersion(fullpath);

		if (chdir(fullpath) == -1)
			elog(FATAL, "Unable to change directory to '%s': %m", fullpath);

		SetDatabasePath(fullpath);
	}

	/*
	 * Code after this point assumes we are in the proper directory!
	 */

	/*
	 * Set up my per-backend PROC struct in shared memory.	(We need to
	 * know MyDatabaseId before we can do this, since it's entered into
	 * the PROC struct.)
	 */
	InitProcess();

	/*
	 * Initialize my entry in the shared-invalidation manager's array of
	 * per-backend data.  (Formerly this came before InitProcess, but now
	 * it must happen after, because it uses MyProc.)  Once I have done
	 * this, I am visible to other backends!
	 *
	 * Sets up MyBackendId, a unique backend identifier.
	 */
	MyBackendId = InvalidBackendId;

	InitBackendSharedInvalidationState();

	if (MyBackendId > MaxBackends || MyBackendId <= 0)
		elog(FATAL, "InitPostgres: bad backend id %d", MyBackendId);

	/*
	 * Initialize the transaction system override state.
	 */
	AmiTransactionOverride(bootstrap);

	/*
	 * Initialize the relation descriptor cache. The pre-allocated
	 * reldescs are created here.
	 */
	RelationCacheInitialize();

	/*
	 * Initialize all the system catalog caches.
	 *
	 * Does not touch files since all routines are builtins (?) - thomas
	 * 1997-11-01
	 */
	InitCatalogCache();

	/* Initialize portal manager */
	EnablePortalManager();

	/*
	 * Initialize the deferred trigger manager --- must happen before
	 * first transaction start.
	 */
	DeferredTriggerInit();

	/* start a new transaction here before access to db */
	if (!bootstrap)
		StartTransactionCommand();

	/* replace faked-up relcache entries with the real info */
	RelationCacheInitializePhase2();

	/*
	 * Figure out our postgres user id.  In standalone mode we use a fixed
	 * id, otherwise we figure it out from the authenticated user name.
	 */
	if (bootstrap)
		InitializeSessionUserIdStandalone();
	else if (!IsUnderPostmaster)
	{
		InitializeSessionUserIdStandalone();
		if (!ThereIsAtLeastOneUser())
		{
			elog(NOTICE, "There are currently no users defined in this database system.");
			elog(NOTICE, "You should immediately run 'CREATE USER \"%s\" WITH SYSID %d CREATEUSER;'.",
				 username, BOOTSTRAP_USESYSID);
		}
	}
	else
		/* normal multiuser case */
		InitializeSessionUserId(username);

	/*
	 * Unless we are bootstrapping, double-check that InitMyDatabaseInfo()
	 * got a correct result.  We can't do this until all the
	 * database-access infrastructure is up.
	 */
	if (!bootstrap)
		ReverifyMyDatabase(dbname);

#ifdef MULTIBYTE
	/* set default client encoding --- uses info from ReverifyMyDatabase */
	set_default_client_encoding();
#endif

	/*
	 * Set up process-exit callbacks to remove temp relations and then do
	 * pre-shutdown cleanup.  This should be last because we want
	 * shmem_exit to call these routines before the exit callbacks that
	 * are registered by buffer manager, lock manager, etc.  We need to
	 * run this code before we close down database access!
	 */
	on_shmem_exit(ShutdownPostgres, 0);

	/*
	 * because callbacks are called in reverse order, this gets done
	 * first:
	 */
	on_shmem_exit(remove_all_temp_relations, 0);

	/* close the transaction we started above */
	if (!bootstrap)
		CommitTransactionCommand();
}

/*
 * Backend-shutdown callback.  Do cleanup that we want to be sure happens
 * before all the supporting modules begin to nail their doors shut via
 * their own callbacks.  Note that because this has to be registered very
 * late in startup, it will not get called if we suffer a failure *during*
 * startup.
 *
 * User-level cleanup, such as temp-relation removal and UNLISTEN, happens
 * via separate callbacks that execute before this one.  We don't combine the
 * callbacks because we still want this one to happen if the user-level
 * cleanup fails.
 */
static void
ShutdownPostgres(void)
{
	/*
	 * These operations are really just a minimal subset of
	 * AbortTransaction(). We don't want to do any inessential cleanup,
	 * since that just raises the odds of failure --- but there's some
	 * stuff we need to do.
	 *
	 * Release any LW locks and buffer context locks we might be holding.
	 * This is a kluge to improve the odds that we won't get into a
	 * self-made stuck-lock scenario while trying to shut down.
	 */
	LWLockReleaseAll();
	AbortBufferIO();
	UnlockBuffers();

	/*
	 * In case a transaction is open, delete any files it created.	This
	 * has to happen before bufmgr shutdown, so having smgr register a
	 * callback for it wouldn't work.
	 */
	smgrDoPendingDeletes(false);	/* delete as though aborting xact */
}



/*
 * Returns true if at least one user is defined in this database cluster.
 */
static bool
ThereIsAtLeastOneUser(void)
{
	Relation	pg_shadow_rel;
	TupleDesc	pg_shadow_dsc;
	HeapScanDesc scan;
	bool		result;

	pg_shadow_rel = heap_openr(ShadowRelationName, AccessExclusiveLock);
	pg_shadow_dsc = RelationGetDescr(pg_shadow_rel);

	scan = heap_beginscan(pg_shadow_rel, false, SnapshotNow, 0, 0);
	result = HeapTupleIsValid(heap_getnext(scan, 0));

	heap_endscan(scan);
	heap_close(pg_shadow_rel, AccessExclusiveLock);

	return result;
}
