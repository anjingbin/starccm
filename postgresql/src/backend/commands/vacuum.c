/*-------------------------------------------------------------------------
 *
 * vacuum.c
 *	  The postgres vacuum cleaner.
 *
 * This file includes the "full" version of VACUUM, as well as control code
 * used by all three of full VACUUM, lazy VACUUM, and ANALYZE.	See
 * vacuumlazy.c and analyze.c for the rest of the code for the latter two.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/commands/vacuum.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <unistd.h>

#include "access/clog.h"
#include "access/genam.h"
#include "access/heapam.h"
#include "access/xlog.h"
#include "catalog/catalog.h"
#include "catalog/catname.h"
#include "catalog/pg_database.h"
#include "catalog/pg_index.h"
#include "commands/vacuum.h"
#include "executor/executor.h"
#include "miscadmin.h"
#include "storage/freespace.h"
#include "storage/sinval.h"
#include "storage/smgr.h"
#include "tcop/pquery.h"
#include "utils/acl.h"
#include "utils/builtins.h"
#include "utils/fmgroids.h"
#include "utils/inval.h"
#include "utils/relcache.h"
#include "utils/syscache.h"
#include "utils/temprel.h"

#include "pgstat.h"


typedef struct VRelListData
{
	Oid			vrl_relid;
	struct VRelListData *vrl_next;
} VRelListData;

typedef VRelListData *VRelList;

typedef struct VacPageData
{
	BlockNumber blkno;			/* BlockNumber of this Page */
	Size		free;			/* FreeSpace on this Page */
	uint16		offsets_used;	/* Number of OffNums used by vacuum */
	uint16		offsets_free;	/* Number of OffNums free or to be free */
	OffsetNumber offsets[1];	/* Array of free OffNums */
} VacPageData;

typedef VacPageData *VacPage;

typedef struct VacPageListData
{
	BlockNumber empty_end_pages;	/* Number of "empty" end-pages */
	int			num_pages;		/* Number of pages in pagedesc */
	int			num_allocated_pages;	/* Number of allocated pages in
										 * pagedesc */
	VacPage    *pagedesc;		/* Descriptions of pages */
} VacPageListData;

typedef VacPageListData *VacPageList;

typedef struct VTupleLinkData
{
	ItemPointerData new_tid;
	ItemPointerData this_tid;
} VTupleLinkData;

typedef VTupleLinkData *VTupleLink;

typedef struct VTupleMoveData
{
	ItemPointerData tid;		/* tuple ID */
	VacPage		vacpage;		/* where to move */
	bool		cleanVpd;		/* clean vacpage before using */
} VTupleMoveData;

typedef VTupleMoveData *VTupleMove;

typedef struct VRelStats
{
	BlockNumber rel_pages;
	double		rel_tuples;
	Size		min_tlen;
	Size		max_tlen;
	bool		hasindex;
	int			num_vtlinks;
	VTupleLink	vtlinks;
} VRelStats;


static MemoryContext vac_context = NULL;

static int	MESSAGE_LEVEL;		/* message level */

static TransactionId OldestXmin;
static TransactionId FreezeLimit;

static TransactionId initialOldestXmin;
static TransactionId initialFreezeLimit;


/* non-export function prototypes */
static void vacuum_init(VacuumStmt *vacstmt);
static void vacuum_shutdown(VacuumStmt *vacstmt);
static VRelList getrels(Name VacRelP, const char *stmttype);
static void vac_update_dbstats(Oid dbid,
				   TransactionId vacuumXID,
				   TransactionId frozenXID);
static void vac_truncate_clog(TransactionId vacuumXID,
				  TransactionId frozenXID);
static void vacuum_rel(Oid relid, VacuumStmt *vacstmt);
static void full_vacuum_rel(Relation onerel, VacuumStmt *vacstmt);
static void scan_heap(VRelStats *vacrelstats, Relation onerel,
		  VacPageList vacuum_pages, VacPageList fraged_pages);
static void repair_frag(VRelStats *vacrelstats, Relation onerel,
			VacPageList vacuum_pages, VacPageList fraged_pages,
			int nindexes, Relation *Irel);
static void vacuum_heap(VRelStats *vacrelstats, Relation onerel,
			VacPageList vacpagelist);
static void vacuum_page(Relation onerel, Buffer buffer, VacPage vacpage);
static void vacuum_index(VacPageList vacpagelist, Relation indrel,
			 double num_tuples, int keep_tuples);
static void scan_index(Relation indrel, double num_tuples);
static bool tid_reaped(ItemPointer itemptr, void *state);
static bool dummy_tid_reaped(ItemPointer itemptr, void *state);
static void vac_update_fsm(Relation onerel, VacPageList fraged_pages,
			   BlockNumber rel_pages);
static VacPage copy_vac_page(VacPage vacpage);
static void vpage_insert(VacPageList vacpagelist, VacPage vpnew);
static void *vac_bsearch(const void *key, const void *base,
			size_t nelem, size_t size,
			int (*compar) (const void *, const void *));
static int	vac_cmp_blk(const void *left, const void *right);
static int	vac_cmp_offno(const void *left, const void *right);
static int	vac_cmp_vtlinks(const void *left, const void *right);
static bool enough_space(VacPage vacpage, Size len);


/****************************************************************************
 *																			*
 *			Code common to all flavors of VACUUM and ANALYZE				*
 *																			*
 ****************************************************************************
 */


/*
 * Primary entry point for VACUUM and ANALYZE commands.
 */
void
vacuum(VacuumStmt *vacstmt)
{
	const char *stmttype = vacstmt->vacuum ? "VACUUM" : "ANALYZE";
	NameData	VacRel;
	Name		VacRelName;
	VRelList	vrl,
				cur;

	/*
	 * We cannot run VACUUM inside a user transaction block; if we were
	 * inside a transaction, then our commit- and
	 * start-transaction-command calls would not have the intended effect!
	 * Furthermore, the forced commit that occurs before truncating the
	 * relation's file would have the effect of committing the rest of the
	 * user's transaction too, which would certainly not be the desired
	 * behavior.
	 */
	if (IsTransactionBlock())
		elog(ERROR, "%s cannot run inside a BEGIN/END block", stmttype);

	/*
	 * Send info about dead objects to the statistics collector
	 */
	pgstat_vacuum_tabstat();

	if (vacstmt->verbose)
		MESSAGE_LEVEL = NOTICE;
	else
		MESSAGE_LEVEL = DEBUG;

	/*
	 * Create special memory context for cross-transaction storage.
	 *
	 * Since it is a child of QueryContext, it will go away eventually even
	 * if we suffer an error; there's no need for special abort cleanup
	 * logic.
	 */
	vac_context = AllocSetContextCreate(QueryContext,
										"Vacuum",
										ALLOCSET_DEFAULT_MINSIZE,
										ALLOCSET_DEFAULT_INITSIZE,
										ALLOCSET_DEFAULT_MAXSIZE);

	/* Convert vacrel, which is just a string, to a Name */
	if (vacstmt->vacrel)
	{
		namestrcpy(&VacRel, vacstmt->vacrel);
		VacRelName = &VacRel;
	}
	else
		VacRelName = NULL;

	/* Build list of relations to process (note this lives in vac_context) */
	vrl = getrels(VacRelName, stmttype);

	/*
	 * Start up the vacuum cleaner.
	 */
	vacuum_init(vacstmt);

	/*
	 * Process each selected relation.	We are careful to process each
	 * relation in a separate transaction in order to avoid holding too
	 * many locks at one time.	Also, if we are doing VACUUM ANALYZE, the
	 * ANALYZE part runs as a separate transaction from the VACUUM to
	 * further reduce locking.
	 */
	for (cur = vrl; cur != (VRelList) NULL; cur = cur->vrl_next)
	{
		if (vacstmt->vacuum)
			vacuum_rel(cur->vrl_relid, vacstmt);
		if (vacstmt->analyze)
			analyze_rel(cur->vrl_relid, vacstmt);
	}

	/* clean up */
	vacuum_shutdown(vacstmt);
}

/*
 *	vacuum_init(), vacuum_shutdown() -- start up and shut down the vacuum cleaner.
 *
 *		Formerly, there was code here to prevent more than one VACUUM from
 *		executing concurrently in the same database.  However, there's no
 *		good reason to prevent that, and manually removing lockfiles after
 *		a vacuum crash was a pain for dbadmins.  So, forget about lockfiles,
 *		and just rely on the locks we grab on each target table
 *		to ensure that there aren't two VACUUMs running on the same table
 *		at the same time.
 *
 *		The strangeness with committing and starting transactions in the
 *		init and shutdown routines is due to the fact that the vacuum cleaner
 *		is invoked via an SQL command, and so is already executing inside
 *		a transaction.	We need to leave ourselves in a predictable state
 *		on entry and exit to the vacuum cleaner.  We commit the transaction
 *		started in PostgresMain() inside vacuum_init(), and start one in
 *		vacuum_shutdown() to match the commit waiting for us back in
 *		PostgresMain().
 */
static void
vacuum_init(VacuumStmt *vacstmt)
{
	if (vacstmt->vacuum && vacstmt->vacrel == NULL)
	{
		/*
		 * Compute the initially applicable OldestXmin and FreezeLimit
		 * XIDs, so that we can record these values at the end of the
		 * VACUUM. Note that individual tables may well be processed with
		 * newer values, but we can guarantee that no (non-shared)
		 * relations are processed with older ones.
		 *
		 * It is okay to record non-shared values in pg_database, even though
		 * we may vacuum shared relations with older cutoffs, because only
		 * the minimum of the values present in pg_database matters.  We
		 * can be sure that shared relations have at some time been
		 * vacuumed with cutoffs no worse than the global minimum; for, if
		 * there is a backend in some other DB with xmin = OLDXMIN that's
		 * determining the cutoff with which we vacuum shared relations,
		 * it is not possible for that database to have a cutoff newer
		 * than OLDXMIN recorded in pg_database.
		 */
		vacuum_set_xid_limits(vacstmt, false,
							  &initialOldestXmin, &initialFreezeLimit);
	}

	/* matches the StartTransaction in PostgresMain() */
	CommitTransactionCommand();
}

static void
vacuum_shutdown(VacuumStmt *vacstmt)
{
	/* on entry, we are not in a transaction */

	/* matches the CommitTransaction in PostgresMain() */
	StartTransactionCommand();

	/*
	 * If we did a database-wide VACUUM, update the database's pg_database
	 * row with info about the transaction IDs used, and try to truncate
	 * pg_clog.
	 */
	if (vacstmt->vacuum && vacstmt->vacrel == NULL)
	{
		vac_update_dbstats(MyDatabaseId,
						   initialOldestXmin, initialFreezeLimit);
		vac_truncate_clog(initialOldestXmin, initialFreezeLimit);
	}

	/*
	 * If we did a complete vacuum or analyze, then flush the init file
	 * that relcache.c uses to save startup time. The next backend startup
	 * will rebuild the init file with up-to-date information from
	 * pg_class. This lets the optimizer see the stats that we've
	 * collected for certain critical system indexes.  See relcache.c for
	 * more details.
	 *
	 * Ignore any failure to unlink the file, since it might not be there if
	 * no backend has been started since the last vacuum.
	 */
	if (vacstmt->vacrel == NULL)
		unlink(RELCACHE_INIT_FILENAME);

	/*
	 * Clean up working storage --- note we must do this after
	 * StartTransactionCommand, else we might be trying to delete the
	 * active context!
	 */
	MemoryContextDelete(vac_context);
	vac_context = NULL;
}

/*
 * Build a list of VRelListData nodes for each relation to be processed
 *
 * The list is built in vac_context so that it will survive across our
 * per-relation transactions.
 */
static VRelList
getrels(Name VacRelP, const char *stmttype)
{
	Relation	rel;
	TupleDesc	tupdesc;
	HeapScanDesc scan;
	HeapTuple	tuple;
	VRelList	vrl,
				cur;
	Datum		d;
	char	   *rname;
	char		rkind;
	bool		n;
	ScanKeyData key;

	if (VacRelP)
	{
		/*
		 * we could use the cache here, but it is clearer to use scankeys
		 * for both vacuum cases, bjm 2000/01/19
		 */
		char	   *nontemp_relname;

		/* We must re-map temp table names bjm 2000-04-06 */
		nontemp_relname = get_temp_rel_by_username(NameStr(*VacRelP));
		if (nontemp_relname == NULL)
			nontemp_relname = NameStr(*VacRelP);

		ScanKeyEntryInitialize(&key, 0x0, Anum_pg_class_relname,
							   F_NAMEEQ,
							   PointerGetDatum(nontemp_relname));
	}
	else
	{
		/* find all plain relations listed in pg_class */
		ScanKeyEntryInitialize(&key, 0x0, Anum_pg_class_relkind,
							   F_CHAREQ, CharGetDatum(RELKIND_RELATION));
	}

	vrl = cur = (VRelList) NULL;

	rel = heap_openr(RelationRelationName, AccessShareLock);
	tupdesc = RelationGetDescr(rel);

	scan = heap_beginscan(rel, false, SnapshotNow, 1, &key);

	while (HeapTupleIsValid(tuple = heap_getnext(scan, 0)))
	{
		d = heap_getattr(tuple, Anum_pg_class_relname, tupdesc, &n);
		rname = (char *) DatumGetName(d);

		d = heap_getattr(tuple, Anum_pg_class_relkind, tupdesc, &n);
		rkind = DatumGetChar(d);

		if (rkind != RELKIND_RELATION)
		{
			elog(NOTICE, "%s: can not process indexes, views or special system tables",
				 stmttype);
			continue;
		}

		/* Make a relation list entry for this guy */
		if (vrl == (VRelList) NULL)
			vrl = cur = (VRelList)
				MemoryContextAlloc(vac_context, sizeof(VRelListData));
		else
		{
			cur->vrl_next = (VRelList)
				MemoryContextAlloc(vac_context, sizeof(VRelListData));
			cur = cur->vrl_next;
		}

		cur->vrl_relid = tuple->t_data->t_oid;
		cur->vrl_next = (VRelList) NULL;
	}

	heap_endscan(scan);
	heap_close(rel, AccessShareLock);

	if (vrl == NULL)
		elog(NOTICE, "%s: table not found", stmttype);

	return vrl;
}

/*
 * vacuum_set_xid_limits() -- compute oldest-Xmin and freeze cutoff points
 */
void
vacuum_set_xid_limits(VacuumStmt *vacstmt, bool sharedRel,
					  TransactionId *oldestXmin,
					  TransactionId *freezeLimit)
{
	TransactionId limit;

	*oldestXmin = GetOldestXmin(sharedRel);

	Assert(TransactionIdIsNormal(*oldestXmin));

	if (vacstmt->freeze)
	{
		/* FREEZE option: use oldest Xmin as freeze cutoff too */
		limit = *oldestXmin;
	}
	else
	{
		/*
		 * Normal case: freeze cutoff is well in the past, to wit, about
		 * halfway to the wrap horizon
		 */
		limit = GetCurrentTransactionId() - (MaxTransactionId >> 2);
	}

	/*
	 * Be careful not to generate a "permanent" XID
	 */
	if (!TransactionIdIsNormal(limit))
		limit = FirstNormalTransactionId;

	/*
	 * Ensure sane relationship of limits
	 */
	if (TransactionIdFollows(limit, *oldestXmin))
	{
		elog(NOTICE, "oldest Xmin is far in the past --- close open transactions soon to avoid wraparound problems");
		limit = *oldestXmin;
	}

	*freezeLimit = limit;
}


/*
 *	vac_update_relstats() -- update statistics for one relation
 *
 *		Update the whole-relation statistics that are kept in its pg_class
 *		row.  There are additional stats that will be updated if we are
 *		doing ANALYZE, but we always update these stats.  This routine works
 *		for both index and heap relation entries in pg_class.
 *
 *		We violate no-overwrite semantics here by storing new values for the
 *		statistics columns directly into the pg_class tuple that's already on
 *		the page.  The reason for this is that if we updated these tuples in
 *		the usual way, vacuuming pg_class itself wouldn't work very well ---
 *		by the time we got done with a vacuum cycle, most of the tuples in
 *		pg_class would've been obsoleted.  Of course, this only works for
 *		fixed-size never-null columns, but these are.
 *
 *		This routine is shared by full VACUUM, lazy VACUUM, and stand-alone
 *		ANALYZE.
 */
void
vac_update_relstats(Oid relid, BlockNumber num_pages, double num_tuples,
					bool hasindex)
{
	Relation	rd;
	HeapTupleData rtup;
	HeapTuple	ctup;
	Form_pg_class pgcform;
	Buffer		buffer;

	/*
	 * update number of tuples and number of pages in pg_class
	 */
	rd = heap_openr(RelationRelationName, RowExclusiveLock);

	ctup = SearchSysCache(RELOID,
						  ObjectIdGetDatum(relid),
						  0, 0, 0);
	if (!HeapTupleIsValid(ctup))
		elog(ERROR, "pg_class entry for relid %u vanished during vacuuming",
			 relid);

	/* get the buffer cache tuple */
	rtup.t_self = ctup->t_self;
	ReleaseSysCache(ctup);
	heap_fetch(rd, SnapshotNow, &rtup, &buffer, NULL);

	/* overwrite the existing statistics in the tuple */
	pgcform = (Form_pg_class) GETSTRUCT(&rtup);
	pgcform->relpages = (int32) num_pages;
	pgcform->reltuples = num_tuples;
	pgcform->relhasindex = hasindex;

	/*
	 * If we have discovered that there are no indexes, then there's no
	 * primary key either.	This could be done more thoroughly...
	 */
	if (!hasindex)
		pgcform->relhaspkey = false;

	/* invalidate the tuple in the cache and write the buffer */
	RelationInvalidateHeapTuple(rd, &rtup);
	WriteBuffer(buffer);

	heap_close(rd, RowExclusiveLock);
}


/*
 *	vac_update_dbstats() -- update statistics for one database
 *
 *		Update the whole-database statistics that are kept in its pg_database
 *		row.
 *
 *		We violate no-overwrite semantics here by storing new values for the
 *		statistics columns directly into the tuple that's already on the page.
 *		As with vac_update_relstats, this avoids leaving dead tuples behind
 *		after a VACUUM; which is good since GetRawDatabaseInfo
 *		can get confused by finding dead tuples in pg_database.
 *
 *		This routine is shared by full and lazy VACUUM.  Note that it is only
 *		applied after a database-wide VACUUM operation.
 */
static void
vac_update_dbstats(Oid dbid,
				   TransactionId vacuumXID,
				   TransactionId frozenXID)
{
	Relation	relation;
	ScanKeyData entry[1];
	HeapScanDesc scan;
	HeapTuple	tuple;
	Form_pg_database dbform;

	relation = heap_openr(DatabaseRelationName, RowExclusiveLock);

	/* Must use a heap scan, since there's no syscache for pg_database */
	ScanKeyEntryInitialize(&entry[0], 0x0,
						   ObjectIdAttributeNumber, F_OIDEQ,
						   ObjectIdGetDatum(dbid));

	scan = heap_beginscan(relation, 0, SnapshotNow, 1, entry);

	tuple = heap_getnext(scan, 0);

	if (!HeapTupleIsValid(tuple))
		elog(ERROR, "database %u does not exist", dbid);

	dbform = (Form_pg_database) GETSTRUCT(tuple);

	/* overwrite the existing statistics in the tuple */
	dbform->datvacuumxid = vacuumXID;
	dbform->datfrozenxid = frozenXID;

	/* invalidate the tuple in the cache and write the buffer */
	RelationInvalidateHeapTuple(relation, tuple);
	WriteNoReleaseBuffer(scan->rs_cbuf);

	heap_endscan(scan);

	heap_close(relation, RowExclusiveLock);
}


/*
 *	vac_truncate_clog() -- attempt to truncate the commit log
 *
 *		Scan pg_database to determine the system-wide oldest datvacuumxid,
 *		and use it to truncate the transaction commit log (pg_clog).
 *		Also generate a warning if the system-wide oldest datfrozenxid
 *		seems to be in danger of wrapping around.
 *
 *		The passed XIDs are simply the ones I just wrote into my pg_database
 *		entry.	They're used to initialize the "min" calculations.
 *
 *		This routine is shared by full and lazy VACUUM.  Note that it is only
 *		applied after a database-wide VACUUM operation.
 */
static void
vac_truncate_clog(TransactionId vacuumXID, TransactionId frozenXID)
{
	Relation	relation;
	HeapScanDesc scan;
	HeapTuple	tuple;
	int32		age;

	relation = heap_openr(DatabaseRelationName, AccessShareLock);

	scan = heap_beginscan(relation, 0, SnapshotNow, 0, NULL);

	while (HeapTupleIsValid(tuple = heap_getnext(scan, 0)))
	{
		Form_pg_database dbform = (Form_pg_database) GETSTRUCT(tuple);

		/* Ignore non-connectable databases (eg, template0) */
		/* It's assumed that these have been frozen correctly */
		if (!dbform->datallowconn)
			continue;

		if (TransactionIdIsNormal(dbform->datvacuumxid) &&
			TransactionIdPrecedes(dbform->datvacuumxid, vacuumXID))
			vacuumXID = dbform->datvacuumxid;
		if (TransactionIdIsNormal(dbform->datfrozenxid) &&
			TransactionIdPrecedes(dbform->datfrozenxid, frozenXID))
			frozenXID = dbform->datfrozenxid;
	}

	heap_endscan(scan);

	heap_close(relation, AccessShareLock);

	/* Truncate CLOG to the oldest vacuumxid */
	TruncateCLOG(vacuumXID);

	/* Give warning about impending wraparound problems */
	age = (int32) (GetCurrentTransactionId() - frozenXID);
	if (age > (int32) ((MaxTransactionId >> 3) * 3))
		elog(NOTICE, "Some databases have not been vacuumed in %d transactions."
			 "\n\tBetter vacuum them within %d transactions,"
			 "\n\tor you may have a wraparound failure.",
			 age, (int32) (MaxTransactionId >> 1) - age);
}


/****************************************************************************
 *																			*
 *			Code common to both flavors of VACUUM							*
 *																			*
 ****************************************************************************
 */


/*
 *	vacuum_rel() -- vacuum one heap relation
 *
 *		Doing one heap at a time incurs extra overhead, since we need to
 *		check that the heap exists again just before we vacuum it.	The
 *		reason that we do this is so that vacuuming can be spread across
 *		many small transactions.  Otherwise, two-phase locking would require
 *		us to lock the entire database during one pass of the vacuum cleaner.
 *
 *		At entry and exit, we are not inside a transaction.
 */
static void
vacuum_rel(Oid relid, VacuumStmt *vacstmt)
{
	LOCKMODE	lmode;
	Relation	onerel;
	LockRelId	onerelid;
	Oid			toast_relid;

	/* Begin a transaction for vacuuming this relation */
	StartTransactionCommand();

	/*
	 * Check for user-requested abort.	Note we want this to be inside a
	 * transaction, so xact.c doesn't issue useless NOTICE.
	 */
	CHECK_FOR_INTERRUPTS();

	/*
	 * Race condition -- if the pg_class tuple has gone away since the
	 * last time we saw it, we don't need to vacuum it.
	 */
	if (!SearchSysCacheExists(RELOID,
							  ObjectIdGetDatum(relid),
							  0, 0, 0))
	{
		CommitTransactionCommand();
		return;
	}

	/*
	 * Determine the type of lock we want --- hard exclusive lock for a
	 * FULL vacuum, but just ShareUpdateExclusiveLock for concurrent
	 * vacuum.	Either way, we can be sure that no other backend is
	 * vacuuming the same table.
	 */
	lmode = vacstmt->full ? AccessExclusiveLock : ShareUpdateExclusiveLock;

	/*
	 * Open the class, get an appropriate lock on it, and check
	 * permissions.
	 *
	 * We allow the user to vacuum a table if he is superuser, the table
	 * owner, or the database owner (but in the latter case, only if it's
	 * not a shared relation).	pg_ownercheck includes the superuser case.
	 *
	 * Note we choose to treat permissions failure as a NOTICE and keep
	 * trying to vacuum the rest of the DB --- is this appropriate?
	 */
	onerel = heap_open(relid, lmode);

	if (!(pg_ownercheck(GetUserId(), RelationGetRelationName(onerel),
						RELNAME) ||
		  (is_dbadmin(MyDatabaseId) && !onerel->rd_rel->relisshared)))
	{
		elog(NOTICE, "Skipping \"%s\" --- only table or database owner can VACUUM it",
			 RelationGetRelationName(onerel));
		heap_close(onerel, lmode);
		CommitTransactionCommand();
		return;
	}

	/*
	 * Get a session-level lock too. This will protect our access to the
	 * relation across multiple transactions, so that we can vacuum the
	 * relation's TOAST table (if any) secure in the knowledge that no one
	 * is deleting the parent relation.
	 *
	 * NOTE: this cannot block, even if someone else is waiting for access,
	 * because the lock manager knows that both lock requests are from the
	 * same process.
	 */
	onerelid = onerel->rd_lockInfo.lockRelId;
	LockRelationForSession(&onerelid, lmode);

	/*
	 * Remember the relation's TOAST relation for later
	 */
	toast_relid = onerel->rd_rel->reltoastrelid;

	/*
	 * Do the actual work --- either FULL or "lazy" vacuum
	 */
	if (vacstmt->full)
		full_vacuum_rel(onerel, vacstmt);
	else
		lazy_vacuum_rel(onerel, vacstmt);

	/* all done with this class, but hold lock until commit */
	heap_close(onerel, NoLock);

	/*
	 * Complete the transaction and free all temporary memory used.
	 */
	CommitTransactionCommand();

	/*
	 * If the relation has a secondary toast rel, vacuum that too while we
	 * still hold the session lock on the master table.  Note however that
	 * "analyze" will not get done on the toast table.	This is good,
	 * because the toaster always uses hardcoded index access and
	 * statistics are totally unimportant for toast relations.
	 */
	if (toast_relid != InvalidOid)
		vacuum_rel(toast_relid, vacstmt);

	/*
	 * Now release the session-level lock on the master table.
	 */
	UnlockRelationForSession(&onerelid, lmode);
}


/****************************************************************************
 *																			*
 *			Code for VACUUM FULL (only)										*
 *																			*
 ****************************************************************************
 */


/*
 *	full_vacuum_rel() -- perform FULL VACUUM for one heap relation
 *
 *		This routine vacuums a single heap, cleans out its indexes, and
 *		updates its num_pages and num_tuples statistics.
 *
 *		At entry, we have already established a transaction and opened
 *		and locked the relation.
 */
static void
full_vacuum_rel(Relation onerel, VacuumStmt *vacstmt)
{
	VacPageListData vacuum_pages;		/* List of pages to vacuum and/or
										 * clean indexes */
	VacPageListData fraged_pages;		/* List of pages with space enough
										 * for re-using */
	Relation   *Irel;
	int			nindexes,
				i;
	VRelStats  *vacrelstats;
	bool		reindex = false;

	if (IsIgnoringSystemIndexes() &&
		IsSystemRelationName(RelationGetRelationName(onerel)))
		reindex = true;

	vacuum_set_xid_limits(vacstmt, onerel->rd_rel->relisshared,
						  &OldestXmin, &FreezeLimit);

	/*
	 * Set up statistics-gathering machinery.
	 */
	vacrelstats = (VRelStats *) palloc(sizeof(VRelStats));
	vacrelstats->rel_pages = 0;
	vacrelstats->rel_tuples = 0;
	vacrelstats->hasindex = false;

	/* scan the heap */
	vacuum_pages.num_pages = fraged_pages.num_pages = 0;
	scan_heap(vacrelstats, onerel, &vacuum_pages, &fraged_pages);

	/* Now open all indexes of the relation */
	vac_open_indexes(onerel, &nindexes, &Irel);
	if (!Irel)
		reindex = false;
	else if (!RelationGetForm(onerel)->relhasindex)
		reindex = true;
	if (nindexes > 0)
		vacrelstats->hasindex = true;

#ifdef NOT_USED

	/*
	 * reindex in VACUUM is dangerous under WAL. ifdef out until it
	 * becomes safe.
	 */
	if (reindex)
	{
		vac_close_indexes(nindexes, Irel);
		Irel = (Relation *) NULL;
		activate_indexes_of_a_table(RelationGetRelid(onerel), false);
	}
#endif   /* NOT_USED */

	/* Clean/scan index relation(s) */
	if (Irel != (Relation *) NULL)
	{
		if (vacuum_pages.num_pages > 0)
		{
			for (i = 0; i < nindexes; i++)
				vacuum_index(&vacuum_pages, Irel[i],
							 vacrelstats->rel_tuples, 0);
		}
		else
		{
			/* just scan indexes to update statistic */
			for (i = 0; i < nindexes; i++)
				scan_index(Irel[i], vacrelstats->rel_tuples);
		}
	}

	if (fraged_pages.num_pages > 0)
	{
		/* Try to shrink heap */
		repair_frag(vacrelstats, onerel, &vacuum_pages, &fraged_pages,
					nindexes, Irel);
		vac_close_indexes(nindexes, Irel);
	}
	else
	{
		vac_close_indexes(nindexes, Irel);
		if (vacuum_pages.num_pages > 0)
		{
			/* Clean pages from vacuum_pages list */
			vacuum_heap(vacrelstats, onerel, &vacuum_pages);
		}
		else
		{
			/*
			 * Flush dirty pages out to disk.  We must do this even if we
			 * didn't do anything else, because we want to ensure that all
			 * tuples have correct on-row commit status on disk (see
			 * bufmgr.c's comments for FlushRelationBuffers()).
			 */
			i = FlushRelationBuffers(onerel, vacrelstats->rel_pages);
			if (i < 0)
				elog(ERROR, "VACUUM (full_vacuum_rel): FlushRelationBuffers returned %d",
					 i);
		}
	}

#ifdef NOT_USED
	if (reindex)
		activate_indexes_of_a_table(RelationGetRelid(onerel), true);
#endif   /* NOT_USED */

	/* update shared free space map with final free space info */
	vac_update_fsm(onerel, &fraged_pages, vacrelstats->rel_pages);

	/* update statistics in pg_class */
	vac_update_relstats(RelationGetRelid(onerel), vacrelstats->rel_pages,
						vacrelstats->rel_tuples, vacrelstats->hasindex);
}


/*
 *	scan_heap() -- scan an open heap relation
 *
 *		This routine sets commit status bits, constructs vacuum_pages (list
 *		of pages we need to compact free space on and/or clean indexes of
 *		deleted tuples), constructs fraged_pages (list of pages with free
 *		space that tuples could be moved into), and calculates statistics
 *		on the number of live tuples in the heap.
 */
static void
scan_heap(VRelStats *vacrelstats, Relation onerel,
		  VacPageList vacuum_pages, VacPageList fraged_pages)
{
	BlockNumber nblocks,
				blkno;
	ItemId		itemid;
	Buffer		buf;
	HeapTupleData tuple;
	OffsetNumber offnum,
				maxoff;
	bool		pgchanged,
				tupgone,
				notup;
	char	   *relname;
	VacPage		vacpage,
				vacpagecopy;
	BlockNumber empty_pages,
				new_pages,
				changed_pages,
				empty_end_pages;
	double		num_tuples,
				tups_vacuumed,
				nkeep,
				nunused;
	double		free_size,
				usable_free_size;
	Size		min_tlen = MaxTupleSize;
	Size		max_tlen = 0;
	int			i;
	bool		do_shrinking = true;
	VTupleLink	vtlinks = (VTupleLink) palloc(100 * sizeof(VTupleLinkData));
	int			num_vtlinks = 0;
	int			free_vtlinks = 100;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);

	relname = RelationGetRelationName(onerel);
	elog(MESSAGE_LEVEL, "--Relation %s--", relname);

	empty_pages = new_pages = changed_pages = empty_end_pages = 0;
	num_tuples = tups_vacuumed = nkeep = nunused = 0;
	free_size = 0;

	nblocks = RelationGetNumberOfBlocks(onerel);

	/*
	 * We initially create each VacPage item in a maximal-sized workspace,
	 * then copy the workspace into a just-large-enough copy.
	 */
	vacpage = (VacPage) palloc(sizeof(VacPageData) + MaxOffsetNumber * sizeof(OffsetNumber));

	for (blkno = 0; blkno < nblocks; blkno++)
	{
		Page		page,
					tempPage = NULL;
		bool		do_reap,
					do_frag;

		CHECK_FOR_INTERRUPTS();

		buf = ReadBuffer(onerel, blkno);
		page = BufferGetPage(buf);

		vacpage->blkno = blkno;
		vacpage->offsets_used = 0;
		vacpage->offsets_free = 0;

		if (PageIsNew(page))
		{
			elog(NOTICE, "Rel %s: Uninitialized page %u - fixing",
				 relname, blkno);
			PageInit(page, BufferGetPageSize(buf), 0);
			vacpage->free = ((PageHeader) page)->pd_upper - ((PageHeader) page)->pd_lower;
			free_size += (vacpage->free - sizeof(ItemIdData));
			new_pages++;
			empty_end_pages++;
			vacpagecopy = copy_vac_page(vacpage);
			vpage_insert(vacuum_pages, vacpagecopy);
			vpage_insert(fraged_pages, vacpagecopy);
			WriteBuffer(buf);
			continue;
		}

		if (PageIsEmpty(page))
		{
			vacpage->free = ((PageHeader) page)->pd_upper - ((PageHeader) page)->pd_lower;
			free_size += (vacpage->free - sizeof(ItemIdData));
			empty_pages++;
			empty_end_pages++;
			vacpagecopy = copy_vac_page(vacpage);
			vpage_insert(vacuum_pages, vacpagecopy);
			vpage_insert(fraged_pages, vacpagecopy);
			ReleaseBuffer(buf);
			continue;
		}

		pgchanged = false;
		notup = true;
		maxoff = PageGetMaxOffsetNumber(page);
		for (offnum = FirstOffsetNumber;
			 offnum <= maxoff;
			 offnum = OffsetNumberNext(offnum))
		{
			uint16		sv_infomask;

			itemid = PageGetItemId(page, offnum);

			/*
			 * Collect un-used items too - it's possible to have indexes
			 * pointing here after crash.
			 */
			if (!ItemIdIsUsed(itemid))
			{
				vacpage->offsets[vacpage->offsets_free++] = offnum;
				nunused += 1;
				continue;
			}

			tuple.t_datamcxt = NULL;
			tuple.t_data = (HeapTupleHeader) PageGetItem(page, itemid);
			tuple.t_len = ItemIdGetLength(itemid);
			ItemPointerSet(&(tuple.t_self), blkno, offnum);

			tupgone = false;
			sv_infomask = tuple.t_data->t_infomask;

			switch (HeapTupleSatisfiesVacuum(tuple.t_data, OldestXmin))
			{
				case HEAPTUPLE_DEAD:
					tupgone = true;		/* we can delete the tuple */
					break;
				case HEAPTUPLE_LIVE:

					/*
					 * Tuple is good.  Consider whether to replace its
					 * xmin value with FrozenTransactionId.
					 */
					if (TransactionIdIsNormal(tuple.t_data->t_xmin) &&
						TransactionIdPrecedes(tuple.t_data->t_xmin,
											  FreezeLimit))
					{
						tuple.t_data->t_xmin = FrozenTransactionId;
						/* infomask should be okay already */
						Assert(tuple.t_data->t_infomask & HEAP_XMIN_COMMITTED);
						pgchanged = true;
					}
					break;
				case HEAPTUPLE_RECENTLY_DEAD:

					/*
					 * If tuple is recently deleted then we must not
					 * remove it from relation.
					 */
					nkeep += 1;

					/*
					 * If we do shrinking and this tuple is updated one
					 * then remember it to construct updated tuple
					 * dependencies.
					 */
					if (do_shrinking &&
						!(ItemPointerEquals(&(tuple.t_self),
											&(tuple.t_data->t_ctid))))
					{
						if (free_vtlinks == 0)
						{
							free_vtlinks = 1000;
							vtlinks = (VTupleLink) repalloc(vtlinks,
										   (free_vtlinks + num_vtlinks) *
												 sizeof(VTupleLinkData));
						}
						vtlinks[num_vtlinks].new_tid = tuple.t_data->t_ctid;
						vtlinks[num_vtlinks].this_tid = tuple.t_self;
						free_vtlinks--;
						num_vtlinks++;
					}
					break;
				case HEAPTUPLE_INSERT_IN_PROGRESS:

					/*
					 * This should not happen, since we hold exclusive
					 * lock on the relation; shouldn't we raise an error?
					 */
					elog(NOTICE, "Rel %s: TID %u/%u: InsertTransactionInProgress %u - can't shrink relation",
						 relname, blkno, offnum, tuple.t_data->t_xmin);
					do_shrinking = false;
					break;
				case HEAPTUPLE_DELETE_IN_PROGRESS:

					/*
					 * This should not happen, since we hold exclusive
					 * lock on the relation; shouldn't we raise an error?
					 */
					elog(NOTICE, "Rel %s: TID %u/%u: DeleteTransactionInProgress %u - can't shrink relation",
						 relname, blkno, offnum, tuple.t_data->t_xmax);
					do_shrinking = false;
					break;
				default:
					elog(ERROR, "Unexpected HeapTupleSatisfiesVacuum result");
					break;
			}

			/* check for hint-bit update by HeapTupleSatisfiesVacuum */
			if (sv_infomask != tuple.t_data->t_infomask)
				pgchanged = true;

			/*
			 * Other checks...
			 */
			if (!OidIsValid(tuple.t_data->t_oid) &&
				onerel->rd_rel->relhasoids)
				elog(NOTICE, "Rel %s: TID %u/%u: OID IS INVALID. TUPGONE %d.",
					 relname, blkno, offnum, (int) tupgone);

			if (tupgone)
			{
				ItemId		lpp;

				/*
				 * Here we are building a temporary copy of the page with
				 * dead tuples removed.  Below we will apply
				 * PageRepairFragmentation to the copy, so that we can
				 * determine how much space will be available after
				 * removal of dead tuples.	But note we are NOT changing
				 * the real page yet...
				 */
				if (tempPage == (Page) NULL)
				{
					Size		pageSize;

					pageSize = PageGetPageSize(page);
					tempPage = (Page) palloc(pageSize);
					memcpy(tempPage, page, pageSize);
				}

				/* mark it unused on the temp page */
				lpp = PageGetItemId(tempPage, offnum);
				lpp->lp_flags &= ~LP_USED;

				vacpage->offsets[vacpage->offsets_free++] = offnum;
				tups_vacuumed += 1;
			}
			else
			{
				num_tuples += 1;
				notup = false;
				if (tuple.t_len < min_tlen)
					min_tlen = tuple.t_len;
				if (tuple.t_len > max_tlen)
					max_tlen = tuple.t_len;
			}
		}						/* scan along page */

		if (tempPage != (Page) NULL)
		{
			/* Some tuples are removable; figure free space after removal */
			PageRepairFragmentation(tempPage, NULL);
			vacpage->free = ((PageHeader) tempPage)->pd_upper - ((PageHeader) tempPage)->pd_lower;
			pfree(tempPage);
			do_reap = true;
		}
		else
		{
			/* Just use current available space */
			vacpage->free = ((PageHeader) page)->pd_upper - ((PageHeader) page)->pd_lower;
			/* Need to reap the page if it has ~LP_USED line pointers */
			do_reap = (vacpage->offsets_free > 0);
		}

		free_size += vacpage->free;

		/*
		 * Add the page to fraged_pages if it has a useful amount of free
		 * space.  "Useful" means enough for a minimal-sized tuple. But we
		 * don't know that accurately near the start of the relation, so
		 * add pages unconditionally if they have >= BLCKSZ/10 free space.
		 */
		do_frag = (vacpage->free >= min_tlen || vacpage->free >= BLCKSZ / 10);

		if (do_reap || do_frag)
		{
			vacpagecopy = copy_vac_page(vacpage);
			if (do_reap)
				vpage_insert(vacuum_pages, vacpagecopy);
			if (do_frag)
				vpage_insert(fraged_pages, vacpagecopy);
		}

		if (notup)
			empty_end_pages++;
		else
			empty_end_pages = 0;

		if (pgchanged)
		{
			WriteBuffer(buf);
			changed_pages++;
		}
		else
			ReleaseBuffer(buf);
	}

	pfree(vacpage);

	/* save stats in the rel list for use later */
	vacrelstats->rel_tuples = num_tuples;
	vacrelstats->rel_pages = nblocks;
	if (num_tuples == 0)
		min_tlen = max_tlen = 0;
	vacrelstats->min_tlen = min_tlen;
	vacrelstats->max_tlen = max_tlen;

	vacuum_pages->empty_end_pages = empty_end_pages;
	fraged_pages->empty_end_pages = empty_end_pages;

	/*
	 * Clear the fraged_pages list if we found we couldn't shrink. Else,
	 * remove any "empty" end-pages from the list, and compute usable free
	 * space = free space in remaining pages.
	 */
	if (do_shrinking)
	{
		Assert((BlockNumber) fraged_pages->num_pages >= empty_end_pages);
		fraged_pages->num_pages -= empty_end_pages;
		usable_free_size = 0;
		for (i = 0; i < fraged_pages->num_pages; i++)
			usable_free_size += fraged_pages->pagedesc[i]->free;
	}
	else
	{
		fraged_pages->num_pages = 0;
		usable_free_size = 0;
	}

	if (usable_free_size > 0 && num_vtlinks > 0)
	{
		qsort((char *) vtlinks, num_vtlinks, sizeof(VTupleLinkData),
			  vac_cmp_vtlinks);
		vacrelstats->vtlinks = vtlinks;
		vacrelstats->num_vtlinks = num_vtlinks;
	}
	else
	{
		vacrelstats->vtlinks = NULL;
		vacrelstats->num_vtlinks = 0;
		pfree(vtlinks);
	}

	elog(MESSAGE_LEVEL, "Pages %u: Changed %u, reaped %u, Empty %u, New %u; \
Tup %.0f: Vac %.0f, Keep/VTL %.0f/%u, UnUsed %.0f, MinLen %lu, MaxLen %lu; \
Re-using: Free/Avail. Space %.0f/%.0f; EndEmpty/Avail. Pages %u/%u.\n\t%s",
		 nblocks, changed_pages, vacuum_pages->num_pages, empty_pages,
		 new_pages, num_tuples, tups_vacuumed,
		 nkeep, vacrelstats->num_vtlinks,
		 nunused, (unsigned long) min_tlen, (unsigned long) max_tlen,
		 free_size, usable_free_size,
		 empty_end_pages, fraged_pages->num_pages,
		 vac_show_rusage(&ru0));

}


/*
 *	repair_frag() -- try to repair relation's fragmentation
 *
 *		This routine marks dead tuples as unused and tries re-use dead space
 *		by moving tuples (and inserting indexes if needed). It constructs
 *		Nvacpagelist list of free-ed pages (moved tuples) and clean indexes
 *		for them after committing (in hack-manner - without losing locks
 *		and freeing memory!) current transaction. It truncates relation
 *		if some end-blocks are gone away.
 */
static void
repair_frag(VRelStats *vacrelstats, Relation onerel,
			VacPageList vacuum_pages, VacPageList fraged_pages,
			int nindexes, Relation *Irel)
{
	TransactionId myXID;
	CommandId	myCID;
	Buffer		buf,
				cur_buffer;
	BlockNumber nblocks,
				blkno;
	BlockNumber last_move_dest_block = 0,
				last_vacuum_block;
	Page		page,
				ToPage = NULL;
	OffsetNumber offnum,
				maxoff,
				newoff,
				max_offset;
	ItemId		itemid,
				newitemid;
	HeapTupleData tuple,
				newtup;
	TupleDesc	tupdesc;
	ResultRelInfo *resultRelInfo;
	EState	   *estate;
	TupleTable	tupleTable;
	TupleTableSlot *slot;
	VacPageListData Nvacpagelist;
	VacPage		cur_page = NULL,
				last_vacuum_page,
				vacpage,
			   *curpage;
	int			cur_item = 0;
	int			i;
	Size		tuple_len;
	int			num_moved,
				num_fraged_pages,
				vacuumed_pages;
	int			checked_moved,
				num_tuples,
				keep_tuples = 0;
	bool		isempty,
				dowrite,
				chain_tuple_moved;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);

	myXID = GetCurrentTransactionId();
	myCID = GetCurrentCommandId();

	tupdesc = RelationGetDescr(onerel);

	/*
	 * We need a ResultRelInfo and an EState so we can use the regular
	 * executor's index-entry-making machinery.
	 */
	resultRelInfo = makeNode(ResultRelInfo);
	resultRelInfo->ri_RangeTableIndex = 1;		/* dummy */
	resultRelInfo->ri_RelationDesc = onerel;
	resultRelInfo->ri_TrigDesc = NULL;	/* we don't fire triggers */

	ExecOpenIndices(resultRelInfo);

	estate = CreateExecutorState();
	estate->es_result_relations = resultRelInfo;
	estate->es_num_result_relations = 1;
	estate->es_result_relation_info = resultRelInfo;

	/* Set up a dummy tuple table too */
	tupleTable = ExecCreateTupleTable(1);
	slot = ExecAllocTableSlot(tupleTable);
	ExecSetSlotDescriptor(slot, tupdesc, false);

	Nvacpagelist.num_pages = 0;
	num_fraged_pages = fraged_pages->num_pages;
	Assert((BlockNumber) vacuum_pages->num_pages >= vacuum_pages->empty_end_pages);
	vacuumed_pages = vacuum_pages->num_pages - vacuum_pages->empty_end_pages;
	if (vacuumed_pages > 0)
	{
		/* get last reaped page from vacuum_pages */
		last_vacuum_page = vacuum_pages->pagedesc[vacuumed_pages - 1];
		last_vacuum_block = last_vacuum_page->blkno;
	}
	else
	{
		last_vacuum_page = NULL;
		last_vacuum_block = InvalidBlockNumber;
	}
	cur_buffer = InvalidBuffer;
	num_moved = 0;

	vacpage = (VacPage) palloc(sizeof(VacPageData) + MaxOffsetNumber * sizeof(OffsetNumber));
	vacpage->offsets_used = vacpage->offsets_free = 0;

	/*
	 * Scan pages backwards from the last nonempty page, trying to move
	 * tuples down to lower pages.	Quit when we reach a page that we have
	 * moved any tuples onto, or the first page if we haven't moved
	 * anything, or when we find a page we cannot completely empty (this
	 * last condition is handled by "break" statements within the loop).
	 *
	 * NB: this code depends on the vacuum_pages and fraged_pages lists being
	 * in order by blkno.
	 */
	nblocks = vacrelstats->rel_pages;
	for (blkno = nblocks - vacuum_pages->empty_end_pages - 1;
		 blkno > last_move_dest_block;
		 blkno--)
	{
		CHECK_FOR_INTERRUPTS();

		/*
		 * Forget fraged_pages pages at or after this one; they're no
		 * longer useful as move targets, since we only want to move down.
		 * Note that since we stop the outer loop at last_move_dest_block,
		 * pages removed here cannot have had anything moved onto them
		 * already.
		 *
		 * Also note that we don't change the stored fraged_pages list, only
		 * our local variable num_fraged_pages; so the forgotten pages are
		 * still available to be loaded into the free space map later.
		 */
		while (num_fraged_pages > 0 &&
			fraged_pages->pagedesc[num_fraged_pages - 1]->blkno >= blkno)
		{
			Assert(fraged_pages->pagedesc[num_fraged_pages - 1]->offsets_used == 0);
			--num_fraged_pages;
		}

		/*
		 * Process this page of relation.
		 */
		buf = ReadBuffer(onerel, blkno);
		page = BufferGetPage(buf);

		vacpage->offsets_free = 0;

		isempty = PageIsEmpty(page);

		dowrite = false;

		/* Is the page in the vacuum_pages list? */
		if (blkno == last_vacuum_block)
		{
			if (last_vacuum_page->offsets_free > 0)
			{
				/* there are dead tuples on this page - clean them */
				Assert(!isempty);
				LockBuffer(buf, BUFFER_LOCK_EXCLUSIVE);
				vacuum_page(onerel, buf, last_vacuum_page);
				LockBuffer(buf, BUFFER_LOCK_UNLOCK);
				dowrite = true;
			}
			else
				Assert(isempty);
			--vacuumed_pages;
			if (vacuumed_pages > 0)
			{
				/* get prev reaped page from vacuum_pages */
				last_vacuum_page = vacuum_pages->pagedesc[vacuumed_pages - 1];
				last_vacuum_block = last_vacuum_page->blkno;
			}
			else
			{
				last_vacuum_page = NULL;
				last_vacuum_block = InvalidBlockNumber;
			}
			if (isempty)
			{
				ReleaseBuffer(buf);
				continue;
			}
		}
		else
			Assert(!isempty);

		chain_tuple_moved = false;		/* no one chain-tuple was moved
										 * off this page, yet */
		vacpage->blkno = blkno;
		maxoff = PageGetMaxOffsetNumber(page);
		for (offnum = FirstOffsetNumber;
			 offnum <= maxoff;
			 offnum = OffsetNumberNext(offnum))
		{
			itemid = PageGetItemId(page, offnum);

			if (!ItemIdIsUsed(itemid))
				continue;

			tuple.t_datamcxt = NULL;
			tuple.t_data = (HeapTupleHeader) PageGetItem(page, itemid);
			tuple_len = tuple.t_len = ItemIdGetLength(itemid);
			ItemPointerSet(&(tuple.t_self), blkno, offnum);

			if (!(tuple.t_data->t_infomask & HEAP_XMIN_COMMITTED))
			{
				if ((TransactionId) tuple.t_data->t_cmin != myXID)
					elog(ERROR, "Invalid XID in t_cmin");
				if (tuple.t_data->t_infomask & HEAP_MOVED_IN)
					elog(ERROR, "HEAP_MOVED_IN was not expected");

				/*
				 * If this (chain) tuple is moved by me already then I
				 * have to check is it in vacpage or not - i.e. is it
				 * moved while cleaning this page or some previous one.
				 */
				if (tuple.t_data->t_infomask & HEAP_MOVED_OFF)
				{
					if (keep_tuples == 0)
						continue;
					if (chain_tuple_moved)		/* some chains was moved
												 * while */
					{			/* cleaning this page */
						Assert(vacpage->offsets_free > 0);
						for (i = 0; i < vacpage->offsets_free; i++)
						{
							if (vacpage->offsets[i] == offnum)
								break;
						}
						if (i >= vacpage->offsets_free) /* not found */
						{
							vacpage->offsets[vacpage->offsets_free++] = offnum;
							keep_tuples--;
						}
					}
					else
					{
						vacpage->offsets[vacpage->offsets_free++] = offnum;
						keep_tuples--;
					}
					continue;
				}
				elog(ERROR, "HEAP_MOVED_OFF was expected");
			}

			/*
			 * If this tuple is in the chain of tuples created in updates
			 * by "recent" transactions then we have to move all chain of
			 * tuples to another places.
			 */
			if ((tuple.t_data->t_infomask & HEAP_UPDATED &&
			 !TransactionIdPrecedes(tuple.t_data->t_xmin, OldestXmin)) ||
				(!(tuple.t_data->t_infomask & HEAP_XMAX_INVALID) &&
				 !(ItemPointerEquals(&(tuple.t_self),
									 &(tuple.t_data->t_ctid)))))
			{
				Buffer		Cbuf = buf;
				Page		Cpage;
				ItemId		Citemid;
				ItemPointerData Ctid;
				HeapTupleData tp = tuple;
				Size		tlen = tuple_len;
				VTupleMove	vtmove = (VTupleMove)
				palloc(100 * sizeof(VTupleMoveData));
				int			num_vtmove = 0;
				int			free_vtmove = 100;
				VacPage		to_vacpage = NULL;
				int			to_item = 0;
				bool		freeCbuf = false;
				int			ti;

				if (vacrelstats->vtlinks == NULL)
					elog(ERROR, "No one parent tuple was found");
				if (cur_buffer != InvalidBuffer)
				{
					WriteBuffer(cur_buffer);
					cur_buffer = InvalidBuffer;
				}

				/*
				 * If this tuple is in the begin/middle of the chain then
				 * we have to move to the end of chain.
				 */
				while (!(tp.t_data->t_infomask & HEAP_XMAX_INVALID) &&
					   !(ItemPointerEquals(&(tp.t_self),
										   &(tp.t_data->t_ctid))))
				{
					Ctid = tp.t_data->t_ctid;
					if (freeCbuf)
						ReleaseBuffer(Cbuf);
					freeCbuf = true;
					Cbuf = ReadBuffer(onerel,
									  ItemPointerGetBlockNumber(&Ctid));
					Cpage = BufferGetPage(Cbuf);
					Citemid = PageGetItemId(Cpage,
									  ItemPointerGetOffsetNumber(&Ctid));
					if (!ItemIdIsUsed(Citemid))
					{
						/*
						 * This means that in the middle of chain there
						 * was tuple updated by older (than OldestXmin)
						 * xaction and this tuple is already deleted by
						 * me. Actually, upper part of chain should be
						 * removed and seems that this should be handled
						 * in scan_heap(), but it's not implemented at the
						 * moment and so we just stop shrinking here.
						 */
						ReleaseBuffer(Cbuf);
						pfree(vtmove);
						vtmove = NULL;
						elog(NOTICE, "Child itemid in update-chain marked as unused - can't continue repair_frag");
						break;
					}
					tp.t_datamcxt = NULL;
					tp.t_data = (HeapTupleHeader) PageGetItem(Cpage, Citemid);
					tp.t_self = Ctid;
					tlen = tp.t_len = ItemIdGetLength(Citemid);
				}
				if (vtmove == NULL)
					break;
				/* first, can chain be moved ? */
				for (;;)
				{
					if (to_vacpage == NULL ||
						!enough_space(to_vacpage, tlen))
					{
						for (i = 0; i < num_fraged_pages; i++)
						{
							if (enough_space(fraged_pages->pagedesc[i], tlen))
								break;
						}

						if (i == num_fraged_pages)
						{
							/* can't move item anywhere */
							for (i = 0; i < num_vtmove; i++)
							{
								Assert(vtmove[i].vacpage->offsets_used > 0);
								(vtmove[i].vacpage->offsets_used)--;
							}
							num_vtmove = 0;
							break;
						}
						to_item = i;
						to_vacpage = fraged_pages->pagedesc[to_item];
					}
					to_vacpage->free -= MAXALIGN(tlen);
					if (to_vacpage->offsets_used >= to_vacpage->offsets_free)
						to_vacpage->free -= MAXALIGN(sizeof(ItemIdData));
					(to_vacpage->offsets_used)++;
					if (free_vtmove == 0)
					{
						free_vtmove = 1000;
						vtmove = (VTupleMove) repalloc(vtmove,
											 (free_vtmove + num_vtmove) *
												 sizeof(VTupleMoveData));
					}
					vtmove[num_vtmove].tid = tp.t_self;
					vtmove[num_vtmove].vacpage = to_vacpage;
					if (to_vacpage->offsets_used == 1)
						vtmove[num_vtmove].cleanVpd = true;
					else
						vtmove[num_vtmove].cleanVpd = false;
					free_vtmove--;
					num_vtmove++;

					/* All done ? */
					if (!(tp.t_data->t_infomask & HEAP_UPDATED) ||
					TransactionIdPrecedes(tp.t_data->t_xmin, OldestXmin))
						break;

					/* Well, try to find tuple with old row version */
					for (;;)
					{
						Buffer		Pbuf;
						Page		Ppage;
						ItemId		Pitemid;
						HeapTupleData Ptp;
						VTupleLinkData vtld,
								   *vtlp;

						vtld.new_tid = tp.t_self;
						vtlp = (VTupleLink)
							vac_bsearch((void *) &vtld,
										(void *) (vacrelstats->vtlinks),
										vacrelstats->num_vtlinks,
										sizeof(VTupleLinkData),
										vac_cmp_vtlinks);
						if (vtlp == NULL)
							elog(ERROR, "Parent tuple was not found");
						tp.t_self = vtlp->this_tid;
						Pbuf = ReadBuffer(onerel,
								ItemPointerGetBlockNumber(&(tp.t_self)));
						Ppage = BufferGetPage(Pbuf);
						Pitemid = PageGetItemId(Ppage,
							   ItemPointerGetOffsetNumber(&(tp.t_self)));
						if (!ItemIdIsUsed(Pitemid))
							elog(ERROR, "Parent itemid marked as unused");
						Ptp.t_datamcxt = NULL;
						Ptp.t_data = (HeapTupleHeader) PageGetItem(Ppage, Pitemid);
						Assert(ItemPointerEquals(&(vtld.new_tid),
												 &(Ptp.t_data->t_ctid)));

						/*
						 * Read above about cases when
						 * !ItemIdIsUsed(Citemid) (child item is
						 * removed)... Due to the fact that at the moment
						 * we don't remove unuseful part of update-chain,
						 * it's possible to get too old parent row here.
						 * Like as in the case which caused this problem,
						 * we stop shrinking here. I could try to find
						 * real parent row but want not to do it because
						 * of real solution will be implemented anyway,
						 * latter, and we are too close to 6.5 release. -
						 * vadim 06/11/99
						 */
						if (!(TransactionIdEquals(Ptp.t_data->t_xmax,
												  tp.t_data->t_xmin)))
						{
							if (freeCbuf)
								ReleaseBuffer(Cbuf);
							freeCbuf = false;
							ReleaseBuffer(Pbuf);
							for (i = 0; i < num_vtmove; i++)
							{
								Assert(vtmove[i].vacpage->offsets_used > 0);
								(vtmove[i].vacpage->offsets_used)--;
							}
							num_vtmove = 0;
							elog(NOTICE, "Too old parent tuple found - can't continue repair_frag");
							break;
						}
#ifdef NOT_USED					/* I'm not sure that this will wotk
								 * properly... */

						/*
						 * If this tuple is updated version of row and it
						 * was created by the same transaction then no one
						 * is interested in this tuple - mark it as
						 * removed.
						 */
						if (Ptp.t_data->t_infomask & HEAP_UPDATED &&
							TransactionIdEquals(Ptp.t_data->t_xmin,
												Ptp.t_data->t_xmax))
						{
							TransactionIdStore(myXID,
								(TransactionId *) &(Ptp.t_data->t_cmin));
							Ptp.t_data->t_infomask &=
								~(HEAP_XMIN_COMMITTED | HEAP_XMIN_INVALID | HEAP_MOVED_IN);
							Ptp.t_data->t_infomask |= HEAP_MOVED_OFF;
							WriteBuffer(Pbuf);
							continue;
						}
#endif
						tp.t_datamcxt = Ptp.t_datamcxt;
						tp.t_data = Ptp.t_data;
						tlen = tp.t_len = ItemIdGetLength(Pitemid);
						if (freeCbuf)
							ReleaseBuffer(Cbuf);
						Cbuf = Pbuf;
						freeCbuf = true;
						break;
					}
					if (num_vtmove == 0)
						break;
				}
				if (freeCbuf)
					ReleaseBuffer(Cbuf);
				if (num_vtmove == 0)	/* chain can't be moved */
				{
					pfree(vtmove);
					break;
				}
				ItemPointerSetInvalid(&Ctid);
				for (ti = 0; ti < num_vtmove; ti++)
				{
					VacPage		destvacpage = vtmove[ti].vacpage;

					/* Get page to move from */
					tuple.t_self = vtmove[ti].tid;
					Cbuf = ReadBuffer(onerel,
							 ItemPointerGetBlockNumber(&(tuple.t_self)));

					/* Get page to move to */
					cur_buffer = ReadBuffer(onerel, destvacpage->blkno);

					LockBuffer(cur_buffer, BUFFER_LOCK_EXCLUSIVE);
					if (cur_buffer != Cbuf)
						LockBuffer(Cbuf, BUFFER_LOCK_EXCLUSIVE);

					ToPage = BufferGetPage(cur_buffer);
					Cpage = BufferGetPage(Cbuf);

					Citemid = PageGetItemId(Cpage,
							ItemPointerGetOffsetNumber(&(tuple.t_self)));
					tuple.t_datamcxt = NULL;
					tuple.t_data = (HeapTupleHeader) PageGetItem(Cpage, Citemid);
					tuple_len = tuple.t_len = ItemIdGetLength(Citemid);

					/*
					 * make a copy of the source tuple, and then mark the
					 * source tuple MOVED_OFF.
					 */
					heap_copytuple_with_tuple(&tuple, &newtup);

					RelationInvalidateHeapTuple(onerel, &tuple);

					/* NO ELOG(ERROR) TILL CHANGES ARE LOGGED */
					START_CRIT_SECTION();

					TransactionIdStore(myXID, (TransactionId *) &(tuple.t_data->t_cmin));
					tuple.t_data->t_infomask &=
						~(HEAP_XMIN_COMMITTED | HEAP_XMIN_INVALID | HEAP_MOVED_IN);
					tuple.t_data->t_infomask |= HEAP_MOVED_OFF;

					/*
					 * If this page was not used before - clean it.
					 *
					 * NOTE: a nasty bug used to lurk here.  It is possible
					 * for the source and destination pages to be the same
					 * (since this tuple-chain member can be on a page
					 * lower than the one we're currently processing in
					 * the outer loop).  If that's true, then after
					 * vacuum_page() the source tuple will have been
					 * moved, and tuple.t_data will be pointing at
					 * garbage.  Therefore we must do everything that uses
					 * tuple.t_data BEFORE this step!!
					 *
					 * This path is different from the other callers of
					 * vacuum_page, because we have already incremented
					 * the vacpage's offsets_used field to account for the
					 * tuple(s) we expect to move onto the page. Therefore
					 * vacuum_page's check for offsets_used == 0 is wrong.
					 * But since that's a good debugging check for all
					 * other callers, we work around it here rather than
					 * remove it.
					 */
					if (!PageIsEmpty(ToPage) && vtmove[ti].cleanVpd)
					{
						int			sv_offsets_used = destvacpage->offsets_used;

						destvacpage->offsets_used = 0;
						vacuum_page(onerel, cur_buffer, destvacpage);
						destvacpage->offsets_used = sv_offsets_used;
					}

					/*
					 * Update the state of the copied tuple, and store it
					 * on the destination page.
					 */
					TransactionIdStore(myXID, (TransactionId *) &(newtup.t_data->t_cmin));
					newtup.t_data->t_infomask &=
						~(HEAP_XMIN_COMMITTED | HEAP_XMIN_INVALID | HEAP_MOVED_OFF);
					newtup.t_data->t_infomask |= HEAP_MOVED_IN;
					newoff = PageAddItem(ToPage, (Item) newtup.t_data, tuple_len,
										 InvalidOffsetNumber, LP_USED);
					if (newoff == InvalidOffsetNumber)
					{
						elog(STOP, "moving chain: failed to add item with len = %lu to page %u",
						  (unsigned long) tuple_len, destvacpage->blkno);
					}
					newitemid = PageGetItemId(ToPage, newoff);
					pfree(newtup.t_data);
					newtup.t_datamcxt = NULL;
					newtup.t_data = (HeapTupleHeader) PageGetItem(ToPage, newitemid);
					ItemPointerSet(&(newtup.t_self), destvacpage->blkno, newoff);

					{
						XLogRecPtr	recptr =
						log_heap_move(onerel, Cbuf, tuple.t_self,
									  cur_buffer, &newtup);

						if (Cbuf != cur_buffer)
						{
							PageSetLSN(Cpage, recptr);
							PageSetSUI(Cpage, ThisStartUpID);
						}
						PageSetLSN(ToPage, recptr);
						PageSetSUI(ToPage, ThisStartUpID);
					}
					END_CRIT_SECTION();

					if (destvacpage->blkno > last_move_dest_block)
						last_move_dest_block = destvacpage->blkno;

					/*
					 * Set new tuple's t_ctid pointing to itself for last
					 * tuple in chain, and to next tuple in chain
					 * otherwise.
					 */
					if (!ItemPointerIsValid(&Ctid))
						newtup.t_data->t_ctid = newtup.t_self;
					else
						newtup.t_data->t_ctid = Ctid;
					Ctid = newtup.t_self;

					num_moved++;

					/*
					 * Remember that we moved tuple from the current page
					 * (corresponding index tuple will be cleaned).
					 */
					if (Cbuf == buf)
						vacpage->offsets[vacpage->offsets_free++] =
							ItemPointerGetOffsetNumber(&(tuple.t_self));
					else
						keep_tuples++;

					LockBuffer(cur_buffer, BUFFER_LOCK_UNLOCK);
					if (cur_buffer != Cbuf)
						LockBuffer(Cbuf, BUFFER_LOCK_UNLOCK);

					/* Create index entries for the moved tuple */
					if (resultRelInfo->ri_NumIndices > 0)
					{
						ExecStoreTuple(&newtup, slot, InvalidBuffer, false);
						ExecInsertIndexTuples(slot, &(newtup.t_self),
											  estate, true);
					}

					WriteBuffer(cur_buffer);
					WriteBuffer(Cbuf);
				}
				cur_buffer = InvalidBuffer;
				pfree(vtmove);
				chain_tuple_moved = true;
				continue;
			}

			/* try to find new page for this tuple */
			if (cur_buffer == InvalidBuffer ||
				!enough_space(cur_page, tuple_len))
			{
				if (cur_buffer != InvalidBuffer)
				{
					WriteBuffer(cur_buffer);
					cur_buffer = InvalidBuffer;
				}
				for (i = 0; i < num_fraged_pages; i++)
				{
					if (enough_space(fraged_pages->pagedesc[i], tuple_len))
						break;
				}
				if (i == num_fraged_pages)
					break;		/* can't move item anywhere */
				cur_item = i;
				cur_page = fraged_pages->pagedesc[cur_item];
				cur_buffer = ReadBuffer(onerel, cur_page->blkno);
				LockBuffer(cur_buffer, BUFFER_LOCK_EXCLUSIVE);
				ToPage = BufferGetPage(cur_buffer);
				/* if this page was not used before - clean it */
				if (!PageIsEmpty(ToPage) && cur_page->offsets_used == 0)
					vacuum_page(onerel, cur_buffer, cur_page);
			}
			else
				LockBuffer(cur_buffer, BUFFER_LOCK_EXCLUSIVE);

			LockBuffer(buf, BUFFER_LOCK_EXCLUSIVE);

			/* copy tuple */
			heap_copytuple_with_tuple(&tuple, &newtup);

			RelationInvalidateHeapTuple(onerel, &tuple);

			/* NO ELOG(ERROR) TILL CHANGES ARE LOGGED */
			START_CRIT_SECTION();

			/*
			 * Mark new tuple as moved_in by vacuum and store vacuum XID
			 * in t_cmin !!!
			 */
			TransactionIdStore(myXID, (TransactionId *) &(newtup.t_data->t_cmin));
			newtup.t_data->t_infomask &=
				~(HEAP_XMIN_COMMITTED | HEAP_XMIN_INVALID | HEAP_MOVED_OFF);
			newtup.t_data->t_infomask |= HEAP_MOVED_IN;

			/* add tuple to the page */
			newoff = PageAddItem(ToPage, (Item) newtup.t_data, tuple_len,
								 InvalidOffsetNumber, LP_USED);
			if (newoff == InvalidOffsetNumber)
			{
				elog(STOP, "failed to add item with len = %lu to page %u (free space %lu, nusd %u, noff %u)",
					 (unsigned long) tuple_len,
					 cur_page->blkno, (unsigned long) cur_page->free,
					 cur_page->offsets_used, cur_page->offsets_free);
			}
			newitemid = PageGetItemId(ToPage, newoff);
			pfree(newtup.t_data);
			newtup.t_datamcxt = NULL;
			newtup.t_data = (HeapTupleHeader) PageGetItem(ToPage, newitemid);
			ItemPointerSet(&(newtup.t_data->t_ctid), cur_page->blkno, newoff);
			newtup.t_self = newtup.t_data->t_ctid;

			/*
			 * Mark old tuple as moved_off by vacuum and store vacuum XID
			 * in t_cmin !!!
			 */
			TransactionIdStore(myXID, (TransactionId *) &(tuple.t_data->t_cmin));
			tuple.t_data->t_infomask &=
				~(HEAP_XMIN_COMMITTED | HEAP_XMIN_INVALID | HEAP_MOVED_IN);
			tuple.t_data->t_infomask |= HEAP_MOVED_OFF;

			{
				XLogRecPtr	recptr =
				log_heap_move(onerel, buf, tuple.t_self,
							  cur_buffer, &newtup);

				PageSetLSN(page, recptr);
				PageSetSUI(page, ThisStartUpID);
				PageSetLSN(ToPage, recptr);
				PageSetSUI(ToPage, ThisStartUpID);
			}
			END_CRIT_SECTION();

			cur_page->offsets_used++;
			num_moved++;
			cur_page->free = ((PageHeader) ToPage)->pd_upper - ((PageHeader) ToPage)->pd_lower;
			if (cur_page->blkno > last_move_dest_block)
				last_move_dest_block = cur_page->blkno;

			vacpage->offsets[vacpage->offsets_free++] = offnum;

			LockBuffer(cur_buffer, BUFFER_LOCK_UNLOCK);
			LockBuffer(buf, BUFFER_LOCK_UNLOCK);

			/* insert index' tuples if needed */
			if (resultRelInfo->ri_NumIndices > 0)
			{
				ExecStoreTuple(&newtup, slot, InvalidBuffer, false);
				ExecInsertIndexTuples(slot, &(newtup.t_self), estate, true);
			}
		}						/* walk along page */

		if (offnum < maxoff && keep_tuples > 0)
		{
			OffsetNumber off;

			for (off = OffsetNumberNext(offnum);
				 off <= maxoff;
				 off = OffsetNumberNext(off))
			{
				itemid = PageGetItemId(page, off);
				if (!ItemIdIsUsed(itemid))
					continue;
				tuple.t_datamcxt = NULL;
				tuple.t_data = (HeapTupleHeader) PageGetItem(page, itemid);
				if (tuple.t_data->t_infomask & HEAP_XMIN_COMMITTED)
					continue;
				if ((TransactionId) tuple.t_data->t_cmin != myXID)
					elog(ERROR, "Invalid XID in t_cmin (4)");
				if (tuple.t_data->t_infomask & HEAP_MOVED_IN)
					elog(ERROR, "HEAP_MOVED_IN was not expected (2)");
				if (tuple.t_data->t_infomask & HEAP_MOVED_OFF)
				{
					/* some chains was moved while */
					if (chain_tuple_moved)
					{			/* cleaning this page */
						Assert(vacpage->offsets_free > 0);
						for (i = 0; i < vacpage->offsets_free; i++)
						{
							if (vacpage->offsets[i] == off)
								break;
						}
						if (i >= vacpage->offsets_free) /* not found */
						{
							vacpage->offsets[vacpage->offsets_free++] = off;
							Assert(keep_tuples > 0);
							keep_tuples--;
						}
					}
					else
					{
						vacpage->offsets[vacpage->offsets_free++] = off;
						Assert(keep_tuples > 0);
						keep_tuples--;
					}
				}
			}
		}

		if (vacpage->offsets_free > 0)	/* some tuples were moved */
		{
			if (chain_tuple_moved)		/* else - they are ordered */
			{
				qsort((char *) (vacpage->offsets), vacpage->offsets_free,
					  sizeof(OffsetNumber), vac_cmp_offno);
			}
			vpage_insert(&Nvacpagelist, copy_vac_page(vacpage));
			WriteBuffer(buf);
		}
		else if (dowrite)
			WriteBuffer(buf);
		else
			ReleaseBuffer(buf);

		if (offnum <= maxoff)
			break;				/* some item(s) left */

	}							/* walk along relation */

	blkno++;					/* new number of blocks */

	if (cur_buffer != InvalidBuffer)
	{
		Assert(num_moved > 0);
		WriteBuffer(cur_buffer);
	}

	if (num_moved > 0)
	{
		/*
		 * We have to commit our tuple movings before we truncate the
		 * relation.  Ideally we should do Commit/StartTransactionCommand
		 * here, relying on the session-level table lock to protect our
		 * exclusive access to the relation.  However, that would require
		 * a lot of extra code to close and re-open the relation, indexes,
		 * etc.  For now, a quick hack: record status of current
		 * transaction as committed, and continue.
		 */
		RecordTransactionCommit();
	}

	/*
	 * We are not going to move any more tuples across pages, but we still
	 * need to apply vacuum_page to compact free space in the remaining
	 * pages in vacuum_pages list.	Note that some of these pages may also
	 * be in the fraged_pages list, and may have had tuples moved onto
	 * them; if so, we already did vacuum_page and needn't do it again.
	 */
	for (i = 0, curpage = vacuum_pages->pagedesc;
		 i < vacuumed_pages;
		 i++, curpage++)
	{
		CHECK_FOR_INTERRUPTS();
		Assert((*curpage)->blkno < blkno);
		if ((*curpage)->offsets_used == 0)
		{
			/* this page was not used as a move target, so must clean it */
			buf = ReadBuffer(onerel, (*curpage)->blkno);
			LockBuffer(buf, BUFFER_LOCK_EXCLUSIVE);
			page = BufferGetPage(buf);
			if (!PageIsEmpty(page))
				vacuum_page(onerel, buf, *curpage);
			LockBuffer(buf, BUFFER_LOCK_UNLOCK);
			WriteBuffer(buf);
		}
	}

	/*
	 * Now scan all the pages that we moved tuples onto and update tuple
	 * status bits.  This is not really necessary, but will save time for
	 * future transactions examining these tuples.
	 *
	 * XXX Notice that this code fails to clear HEAP_MOVED_OFF tuples from
	 * pages that were move source pages but not move dest pages.  One
	 * also wonders whether it wouldn't be better to skip this step and
	 * let the tuple status updates happen someplace that's not holding an
	 * exclusive lock on the relation.
	 */
	checked_moved = 0;
	for (i = 0, curpage = fraged_pages->pagedesc;
		 i < num_fraged_pages;
		 i++, curpage++)
	{
		CHECK_FOR_INTERRUPTS();
		Assert((*curpage)->blkno < blkno);
		if ((*curpage)->blkno > last_move_dest_block)
			break;				/* no need to scan any further */
		if ((*curpage)->offsets_used == 0)
			continue;			/* this page was never used as a move dest */
		buf = ReadBuffer(onerel, (*curpage)->blkno);
		LockBuffer(buf, BUFFER_LOCK_EXCLUSIVE);
		page = BufferGetPage(buf);
		num_tuples = 0;
		max_offset = PageGetMaxOffsetNumber(page);
		for (newoff = FirstOffsetNumber;
			 newoff <= max_offset;
			 newoff = OffsetNumberNext(newoff))
		{
			itemid = PageGetItemId(page, newoff);
			if (!ItemIdIsUsed(itemid))
				continue;
			tuple.t_datamcxt = NULL;
			tuple.t_data = (HeapTupleHeader) PageGetItem(page, itemid);
			if (!(tuple.t_data->t_infomask & HEAP_XMIN_COMMITTED))
			{
				if ((TransactionId) tuple.t_data->t_cmin != myXID)
					elog(ERROR, "Invalid XID in t_cmin (2)");
				if (tuple.t_data->t_infomask & HEAP_MOVED_IN)
				{
					tuple.t_data->t_infomask |= HEAP_XMIN_COMMITTED;
					num_tuples++;
				}
				else if (tuple.t_data->t_infomask & HEAP_MOVED_OFF)
					tuple.t_data->t_infomask |= HEAP_XMIN_INVALID;
				else
					elog(ERROR, "HEAP_MOVED_OFF/HEAP_MOVED_IN was expected");
			}
		}
		LockBuffer(buf, BUFFER_LOCK_UNLOCK);
		WriteBuffer(buf);
		Assert((*curpage)->offsets_used == num_tuples);
		checked_moved += num_tuples;
	}
	Assert(num_moved == checked_moved);

	elog(MESSAGE_LEVEL, "Rel %s: Pages: %u --> %u; Tuple(s) moved: %u.\n\t%s",
		 RelationGetRelationName(onerel),
		 nblocks, blkno, num_moved,
		 vac_show_rusage(&ru0));

	/*
	 * Reflect the motion of system tuples to catalog cache here.
	 */
	CommandCounterIncrement();

	if (Nvacpagelist.num_pages > 0)
	{
		/* vacuum indexes again if needed */
		if (Irel != (Relation *) NULL)
		{
			VacPage    *vpleft,
					   *vpright,
						vpsave;

			/* re-sort Nvacpagelist.pagedesc */
			for (vpleft = Nvacpagelist.pagedesc,
			vpright = Nvacpagelist.pagedesc + Nvacpagelist.num_pages - 1;
				 vpleft < vpright; vpleft++, vpright--)
			{
				vpsave = *vpleft;
				*vpleft = *vpright;
				*vpright = vpsave;
			}
			Assert(keep_tuples >= 0);
			for (i = 0; i < nindexes; i++)
				vacuum_index(&Nvacpagelist, Irel[i],
							 vacrelstats->rel_tuples, keep_tuples);
		}

		/* clean moved tuples from last page in Nvacpagelist list */
		if (vacpage->blkno == (blkno - 1) &&
			vacpage->offsets_free > 0)
		{
			OffsetNumber unbuf[BLCKSZ / sizeof(OffsetNumber)];
			OffsetNumber *unused = unbuf;
			int			uncnt;

			buf = ReadBuffer(onerel, vacpage->blkno);
			LockBuffer(buf, BUFFER_LOCK_EXCLUSIVE);
			page = BufferGetPage(buf);
			num_tuples = 0;
			maxoff = PageGetMaxOffsetNumber(page);
			for (offnum = FirstOffsetNumber;
				 offnum <= maxoff;
				 offnum = OffsetNumberNext(offnum))
			{
				itemid = PageGetItemId(page, offnum);
				if (!ItemIdIsUsed(itemid))
					continue;
				tuple.t_datamcxt = NULL;
				tuple.t_data = (HeapTupleHeader) PageGetItem(page, itemid);

				if (!(tuple.t_data->t_infomask & HEAP_XMIN_COMMITTED))
				{
					if ((TransactionId) tuple.t_data->t_cmin != myXID)
						elog(ERROR, "Invalid XID in t_cmin (3)");
					if (tuple.t_data->t_infomask & HEAP_MOVED_OFF)
					{
						itemid->lp_flags &= ~LP_USED;
						num_tuples++;
					}
					else
						elog(ERROR, "HEAP_MOVED_OFF was expected (2)");
				}

			}
			Assert(vacpage->offsets_free == num_tuples);
			START_CRIT_SECTION();
			uncnt = PageRepairFragmentation(page, unused);
			{
				XLogRecPtr	recptr;

				recptr = log_heap_clean(onerel, buf, (char *) unused,
						  (char *) (&(unused[uncnt])) - (char *) unused);
				PageSetLSN(page, recptr);
				PageSetSUI(page, ThisStartUpID);
			}
			END_CRIT_SECTION();
			LockBuffer(buf, BUFFER_LOCK_UNLOCK);
			WriteBuffer(buf);
		}

		/* now - free new list of reaped pages */
		curpage = Nvacpagelist.pagedesc;
		for (i = 0; i < Nvacpagelist.num_pages; i++, curpage++)
			pfree(*curpage);
		pfree(Nvacpagelist.pagedesc);
	}

	/*
	 * Flush dirty pages out to disk.  We do this unconditionally, even if
	 * we don't need to truncate, because we want to ensure that all
	 * tuples have correct on-row commit status on disk (see bufmgr.c's
	 * comments for FlushRelationBuffers()).
	 */
	i = FlushRelationBuffers(onerel, blkno);
	if (i < 0)
		elog(ERROR, "VACUUM (repair_frag): FlushRelationBuffers returned %d",
			 i);

	/* truncate relation, if needed */
	if (blkno < nblocks)
	{
		blkno = smgrtruncate(DEFAULT_SMGR, onerel, blkno);
		onerel->rd_nblocks = blkno;		/* update relcache immediately */
		onerel->rd_targblock = InvalidBlockNumber;
		vacrelstats->rel_pages = blkno; /* set new number of blocks */
	}

	/* clean up */
	pfree(vacpage);
	if (vacrelstats->vtlinks != NULL)
		pfree(vacrelstats->vtlinks);

	ExecDropTupleTable(tupleTable, true);

	ExecCloseIndices(resultRelInfo);
}

/*
 *	vacuum_heap() -- free dead tuples
 *
 *		This routine marks dead tuples as unused and truncates relation
 *		if there are "empty" end-blocks.
 */
static void
vacuum_heap(VRelStats *vacrelstats, Relation onerel, VacPageList vacuum_pages)
{
	Buffer		buf;
	VacPage    *vacpage;
	BlockNumber relblocks;
	int			nblocks;
	int			i;

	nblocks = vacuum_pages->num_pages;
	nblocks -= vacuum_pages->empty_end_pages;	/* nothing to do with them */

	for (i = 0, vacpage = vacuum_pages->pagedesc; i < nblocks; i++, vacpage++)
	{
		CHECK_FOR_INTERRUPTS();
		if ((*vacpage)->offsets_free > 0)
		{
			buf = ReadBuffer(onerel, (*vacpage)->blkno);
			LockBuffer(buf, BUFFER_LOCK_EXCLUSIVE);
			vacuum_page(onerel, buf, *vacpage);
			LockBuffer(buf, BUFFER_LOCK_UNLOCK);
			WriteBuffer(buf);
		}
	}

	/*
	 * Flush dirty pages out to disk.  We do this unconditionally, even if
	 * we don't need to truncate, because we want to ensure that all
	 * tuples have correct on-row commit status on disk (see bufmgr.c's
	 * comments for FlushRelationBuffers()).
	 */
	Assert(vacrelstats->rel_pages >= vacuum_pages->empty_end_pages);
	relblocks = vacrelstats->rel_pages - vacuum_pages->empty_end_pages;

	i = FlushRelationBuffers(onerel, relblocks);
	if (i < 0)
		elog(ERROR, "VACUUM (vacuum_heap): FlushRelationBuffers returned %d",
			 i);

	/* truncate relation if there are some empty end-pages */
	if (vacuum_pages->empty_end_pages > 0)
	{
		elog(MESSAGE_LEVEL, "Rel %s: Pages: %u --> %u.",
			 RelationGetRelationName(onerel),
			 vacrelstats->rel_pages, relblocks);
		relblocks = smgrtruncate(DEFAULT_SMGR, onerel, relblocks);
		onerel->rd_nblocks = relblocks; /* update relcache immediately */
		onerel->rd_targblock = InvalidBlockNumber;
		vacrelstats->rel_pages = relblocks;		/* set new number of
												 * blocks */
	}
}

/*
 *	vacuum_page() -- free dead tuples on a page
 *					 and repair its fragmentation.
 */
static void
vacuum_page(Relation onerel, Buffer buffer, VacPage vacpage)
{
	OffsetNumber unbuf[BLCKSZ / sizeof(OffsetNumber)];
	OffsetNumber *unused = unbuf;
	int			uncnt;
	Page		page = BufferGetPage(buffer);
	ItemId		itemid;
	int			i;

	/* There shouldn't be any tuples moved onto the page yet! */
	Assert(vacpage->offsets_used == 0);

	START_CRIT_SECTION();
	for (i = 0; i < vacpage->offsets_free; i++)
	{
		itemid = PageGetItemId(page, vacpage->offsets[i]);
		itemid->lp_flags &= ~LP_USED;
	}
	uncnt = PageRepairFragmentation(page, unused);
	{
		XLogRecPtr	recptr;

		recptr = log_heap_clean(onerel, buffer, (char *) unused,
						  (char *) (&(unused[uncnt])) - (char *) unused);
		PageSetLSN(page, recptr);
		PageSetSUI(page, ThisStartUpID);
	}
	END_CRIT_SECTION();
}

/*
 *	scan_index() -- scan one index relation to update statistic.
 *
 * We use this when we have no deletions to do.
 */
static void
scan_index(Relation indrel, double num_tuples)
{
	IndexBulkDeleteResult *stats;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);

	/*
	 * Even though we're not planning to delete anything, use the
	 * ambulkdelete call, so that the scan happens within the index AM for
	 * more speed.
	 */
	stats = index_bulk_delete(indrel, dummy_tid_reaped, NULL);

	if (!stats)
		return;

	/* now update statistics in pg_class */
	vac_update_relstats(RelationGetRelid(indrel),
						stats->num_pages, stats->num_index_tuples,
						false);

	elog(MESSAGE_LEVEL, "Index %s: Pages %u; Tuples %.0f.\n\t%s",
		 RelationGetRelationName(indrel),
		 stats->num_pages, stats->num_index_tuples,
		 vac_show_rusage(&ru0));

	/*
	 * Check for tuple count mismatch.	If the index is partial, then it's
	 * OK for it to have fewer tuples than the heap; else we got trouble.
	 */
	if (stats->num_index_tuples != num_tuples)
	{
		if (stats->num_index_tuples > num_tuples ||
			!vac_is_partial_index(indrel))
			elog(NOTICE, "Index %s: NUMBER OF INDEX' TUPLES (%.0f) IS NOT THE SAME AS HEAP' (%.0f).\
\n\tRecreate the index.",
				 RelationGetRelationName(indrel),
				 stats->num_index_tuples, num_tuples);
	}

	pfree(stats);
}

/*
 *	vacuum_index() -- vacuum one index relation.
 *
 *		Vpl is the VacPageList of the heap we're currently vacuuming.
 *		It's locked. Indrel is an index relation on the vacuumed heap.
 *
 *		We don't bother to set locks on the index relation here, since
 *		the parent table is exclusive-locked already.
 *
 *		Finally, we arrange to update the index relation's statistics in
 *		pg_class.
 */
static void
vacuum_index(VacPageList vacpagelist, Relation indrel,
			 double num_tuples, int keep_tuples)
{
	IndexBulkDeleteResult *stats;
	VacRUsage	ru0;

	vac_init_rusage(&ru0);

	/* Do bulk deletion */
	stats = index_bulk_delete(indrel, tid_reaped, (void *) vacpagelist);

	if (!stats)
		return;

	/* now update statistics in pg_class */
	vac_update_relstats(RelationGetRelid(indrel),
						stats->num_pages, stats->num_index_tuples,
						false);

	elog(MESSAGE_LEVEL, "Index %s: Pages %u; Tuples %.0f: Deleted %.0f.\n\t%s",
		 RelationGetRelationName(indrel), stats->num_pages,
		 stats->num_index_tuples - keep_tuples, stats->tuples_removed,
		 vac_show_rusage(&ru0));

	/*
	 * Check for tuple count mismatch.	If the index is partial, then it's
	 * OK for it to have fewer tuples than the heap; else we got trouble.
	 */
	if (stats->num_index_tuples != num_tuples + keep_tuples)
	{
		if (stats->num_index_tuples > num_tuples + keep_tuples ||
			!vac_is_partial_index(indrel))
			elog(NOTICE, "Index %s: NUMBER OF INDEX' TUPLES (%.0f) IS NOT THE SAME AS HEAP' (%.0f).\
\n\tRecreate the index.",
				 RelationGetRelationName(indrel),
				 stats->num_index_tuples, num_tuples);
	}

	pfree(stats);
}

/*
 *	tid_reaped() -- is a particular tid reaped?
 *
 *		This has the right signature to be an IndexBulkDeleteCallback.
 *
 *		vacpagelist->VacPage_array is sorted in right order.
 */
static bool
tid_reaped(ItemPointer itemptr, void *state)
{
	VacPageList vacpagelist = (VacPageList) state;
	OffsetNumber ioffno;
	OffsetNumber *voff;
	VacPage		vp,
			   *vpp;
	VacPageData vacpage;

	vacpage.blkno = ItemPointerGetBlockNumber(itemptr);
	ioffno = ItemPointerGetOffsetNumber(itemptr);

	vp = &vacpage;
	vpp = (VacPage *) vac_bsearch((void *) &vp,
								  (void *) (vacpagelist->pagedesc),
								  vacpagelist->num_pages,
								  sizeof(VacPage),
								  vac_cmp_blk);

	if (vpp == NULL)
		return false;

	/* ok - we are on a partially or fully reaped page */
	vp = *vpp;

	if (vp->offsets_free == 0)
	{
		/* this is EmptyPage, so claim all tuples on it are reaped!!! */
		return true;
	}

	voff = (OffsetNumber *) vac_bsearch((void *) &ioffno,
										(void *) (vp->offsets),
										vp->offsets_free,
										sizeof(OffsetNumber),
										vac_cmp_offno);

	if (voff == NULL)
		return false;

	/* tid is reaped */
	return true;
}

/*
 * Dummy version for scan_index.
 */
static bool
dummy_tid_reaped(ItemPointer itemptr, void *state)
{
	return false;
}

/*
 * Update the shared Free Space Map with the info we now have about
 * free space in the relation, discarding any old info the map may have.
 */
static void
vac_update_fsm(Relation onerel, VacPageList fraged_pages,
			   BlockNumber rel_pages)
{
	int			nPages = fraged_pages->num_pages;
	int			i;
	BlockNumber *pages;
	Size	   *spaceAvail;

	/* +1 to avoid palloc(0) */
	pages = (BlockNumber *) palloc((nPages + 1) * sizeof(BlockNumber));
	spaceAvail = (Size *) palloc((nPages + 1) * sizeof(Size));

	for (i = 0; i < nPages; i++)
	{
		pages[i] = fraged_pages->pagedesc[i]->blkno;
		spaceAvail[i] = fraged_pages->pagedesc[i]->free;

		/*
		 * fraged_pages may contain entries for pages that we later
		 * decided to truncate from the relation; don't enter them into
		 * the map!
		 */
		if (pages[i] >= rel_pages)
		{
			nPages = i;
			break;
		}
	}

	MultiRecordFreeSpace(&onerel->rd_node,
						 0, MaxBlockNumber,
						 nPages, pages, spaceAvail);
	pfree(pages);
	pfree(spaceAvail);
}

/* Copy a VacPage structure */
static VacPage
copy_vac_page(VacPage vacpage)
{
	VacPage		newvacpage;

	/* allocate a VacPageData entry */
	newvacpage = (VacPage) palloc(sizeof(VacPageData) +
						   vacpage->offsets_free * sizeof(OffsetNumber));

	/* fill it in */
	if (vacpage->offsets_free > 0)
		memcpy(newvacpage->offsets, vacpage->offsets,
			   vacpage->offsets_free * sizeof(OffsetNumber));
	newvacpage->blkno = vacpage->blkno;
	newvacpage->free = vacpage->free;
	newvacpage->offsets_used = vacpage->offsets_used;
	newvacpage->offsets_free = vacpage->offsets_free;

	return newvacpage;
}

/*
 * Add a VacPage pointer to a VacPageList.
 *
 *		As a side effect of the way that scan_heap works,
 *		higher pages come after lower pages in the array
 *		(and highest tid on a page is last).
 */
static void
vpage_insert(VacPageList vacpagelist, VacPage vpnew)
{
#define PG_NPAGEDESC 1024

	/* allocate a VacPage entry if needed */
	if (vacpagelist->num_pages == 0)
	{
		vacpagelist->pagedesc = (VacPage *) palloc(PG_NPAGEDESC * sizeof(VacPage));
		vacpagelist->num_allocated_pages = PG_NPAGEDESC;
	}
	else if (vacpagelist->num_pages >= vacpagelist->num_allocated_pages)
	{
		vacpagelist->num_allocated_pages *= 2;
		vacpagelist->pagedesc = (VacPage *) repalloc(vacpagelist->pagedesc, vacpagelist->num_allocated_pages * sizeof(VacPage));
	}
	vacpagelist->pagedesc[vacpagelist->num_pages] = vpnew;
	(vacpagelist->num_pages)++;
}

/*
 * vac_bsearch: just like standard C library routine bsearch(),
 * except that we first test to see whether the target key is outside
 * the range of the table entries.	This case is handled relatively slowly
 * by the normal binary search algorithm (ie, no faster than any other key)
 * but it occurs often enough in VACUUM to be worth optimizing.
 */
static void *
vac_bsearch(const void *key, const void *base,
			size_t nelem, size_t size,
			int (*compar) (const void *, const void *))
{
	int			res;
	const void *last;

	if (nelem == 0)
		return NULL;
	res = compar(key, base);
	if (res < 0)
		return NULL;
	if (res == 0)
		return (void *) base;
	if (nelem > 1)
	{
		last = (const void *) ((const char *) base + (nelem - 1) * size);
		res = compar(key, last);
		if (res > 0)
			return NULL;
		if (res == 0)
			return (void *) last;
	}
	if (nelem <= 2)
		return NULL;			/* already checked 'em all */
	return bsearch(key, base, nelem, size, compar);
}

/*
 * Comparator routines for use with qsort() and bsearch().
 */
static int
vac_cmp_blk(const void *left, const void *right)
{
	BlockNumber lblk,
				rblk;

	lblk = (*((VacPage *) left))->blkno;
	rblk = (*((VacPage *) right))->blkno;

	if (lblk < rblk)
		return -1;
	if (lblk == rblk)
		return 0;
	return 1;
}

static int
vac_cmp_offno(const void *left, const void *right)
{
	if (*(OffsetNumber *) left < *(OffsetNumber *) right)
		return -1;
	if (*(OffsetNumber *) left == *(OffsetNumber *) right)
		return 0;
	return 1;
}

static int
vac_cmp_vtlinks(const void *left, const void *right)
{
	if (((VTupleLink) left)->new_tid.ip_blkid.bi_hi <
		((VTupleLink) right)->new_tid.ip_blkid.bi_hi)
		return -1;
	if (((VTupleLink) left)->new_tid.ip_blkid.bi_hi >
		((VTupleLink) right)->new_tid.ip_blkid.bi_hi)
		return 1;
	/* bi_hi-es are equal */
	if (((VTupleLink) left)->new_tid.ip_blkid.bi_lo <
		((VTupleLink) right)->new_tid.ip_blkid.bi_lo)
		return -1;
	if (((VTupleLink) left)->new_tid.ip_blkid.bi_lo >
		((VTupleLink) right)->new_tid.ip_blkid.bi_lo)
		return 1;
	/* bi_lo-es are equal */
	if (((VTupleLink) left)->new_tid.ip_posid <
		((VTupleLink) right)->new_tid.ip_posid)
		return -1;
	if (((VTupleLink) left)->new_tid.ip_posid >
		((VTupleLink) right)->new_tid.ip_posid)
		return 1;
	return 0;
}


void
vac_open_indexes(Relation relation, int *nindexes, Relation **Irel)
{
	List	   *indexoidlist,
			   *indexoidscan;
	int			i;

	indexoidlist = RelationGetIndexList(relation);

	*nindexes = length(indexoidlist);

	if (*nindexes > 0)
		*Irel = (Relation *) palloc(*nindexes * sizeof(Relation));
	else
		*Irel = NULL;

	i = 0;
	foreach(indexoidscan, indexoidlist)
	{
		Oid			indexoid = lfirsti(indexoidscan);

		(*Irel)[i] = index_open(indexoid);
		i++;
	}

	freeList(indexoidlist);
}


void
vac_close_indexes(int nindexes, Relation *Irel)
{
	if (Irel == (Relation *) NULL)
		return;

	while (nindexes--)
		index_close(Irel[nindexes]);
	pfree(Irel);
}


/*
 * Is an index partial (ie, could it contain fewer tuples than the heap?)
 */
bool
vac_is_partial_index(Relation indrel)
{
	bool		result;
	HeapTuple	cachetuple;
	Form_pg_index indexStruct;

	/*
	 * If the index's AM doesn't support nulls, it's partial for our
	 * purposes
	 */
	if (!indrel->rd_am->amindexnulls)
		return true;

	/* Otherwise, look to see if there's a partial-index predicate */
	cachetuple = SearchSysCache(INDEXRELID,
							  ObjectIdGetDatum(RelationGetRelid(indrel)),
								0, 0, 0);
	if (!HeapTupleIsValid(cachetuple))
		elog(ERROR, "vac_is_partial_index: index %u not found",
			 RelationGetRelid(indrel));
	indexStruct = (Form_pg_index) GETSTRUCT(cachetuple);

	result = (VARSIZE(&indexStruct->indpred) > VARHDRSZ);

	ReleaseSysCache(cachetuple);
	return result;
}


static bool
enough_space(VacPage vacpage, Size len)
{
	len = MAXALIGN(len);

	if (len > vacpage->free)
		return false;

	/* if there are free itemid(s) and len <= free_space... */
	if (vacpage->offsets_used < vacpage->offsets_free)
		return true;

	/* noff_used >= noff_free and so we'll have to allocate new itemid */
	if (len + sizeof(ItemIdData) <= vacpage->free)
		return true;

	return false;
}


/*
 * Initialize usage snapshot.
 */
void
vac_init_rusage(VacRUsage *ru0)
{
	struct timezone tz;

	getrusage(RUSAGE_SELF, &ru0->ru);
	gettimeofday(&ru0->tv, &tz);
}

/*
 * Compute elapsed time since ru0 usage snapshot, and format into
 * a displayable string.  Result is in a static string, which is
 * tacky, but no one ever claimed that the Postgres backend is
 * threadable...
 */
const char *
vac_show_rusage(VacRUsage *ru0)
{
	static char result[100];
	VacRUsage	ru1;

	vac_init_rusage(&ru1);

	if (ru1.tv.tv_usec < ru0->tv.tv_usec)
	{
		ru1.tv.tv_sec--;
		ru1.tv.tv_usec += 1000000;
	}
	if (ru1.ru.ru_stime.tv_usec < ru0->ru.ru_stime.tv_usec)
	{
		ru1.ru.ru_stime.tv_sec--;
		ru1.ru.ru_stime.tv_usec += 1000000;
	}
	if (ru1.ru.ru_utime.tv_usec < ru0->ru.ru_utime.tv_usec)
	{
		ru1.ru.ru_utime.tv_sec--;
		ru1.ru.ru_utime.tv_usec += 1000000;
	}

	snprintf(result, sizeof(result),
			 "CPU %d.%02ds/%d.%02du sec elapsed %d.%02d sec.",
			 (int) (ru1.ru.ru_stime.tv_sec - ru0->ru.ru_stime.tv_sec),
	  (int) (ru1.ru.ru_stime.tv_usec - ru0->ru.ru_stime.tv_usec) / 10000,
			 (int) (ru1.ru.ru_utime.tv_sec - ru0->ru.ru_utime.tv_sec),
	  (int) (ru1.ru.ru_utime.tv_usec - ru0->ru.ru_utime.tv_usec) / 10000,
			 (int) (ru1.tv.tv_sec - ru0->tv.tv_sec),
			 (int) (ru1.tv.tv_usec - ru0->tv.tv_usec) / 10000);

	return result;
}
