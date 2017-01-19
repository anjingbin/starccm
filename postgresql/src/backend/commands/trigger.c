/*-------------------------------------------------------------------------
 *
 * trigger.c
 *	  PostgreSQL TRIGGERs support code.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/commands/trigger.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/genam.h"
#include "access/heapam.h"
#include "catalog/catalog.h"
#include "catalog/catname.h"
#include "catalog/indexing.h"
#include "catalog/pg_language.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_trigger.h"
#include "commands/comment.h"
#include "commands/trigger.h"
#include "executor/executor.h"
#include "miscadmin.h"
#include "utils/acl.h"
#include "utils/builtins.h"
#include "utils/fmgroids.h"
#include "utils/inval.h"
#include "utils/syscache.h"


static void InsertTrigger(TriggerDesc *trigdesc, Trigger *trigger, int indx);
static HeapTuple GetTupleForTrigger(EState *estate,
				   ResultRelInfo *relinfo,
				   ItemPointer tid,
				   TupleTableSlot **newSlot);
static HeapTuple ExecCallTriggerFunc(TriggerData *trigdata,
					FmgrInfo *finfo,
					MemoryContext per_tuple_context);
static void DeferredTriggerSaveEvent(ResultRelInfo *relinfo, int event,
						 HeapTuple oldtup, HeapTuple newtup);
static void DeferredTriggerExecute(DeferredTriggerEvent event, int itemno,
					   Relation rel, FmgrInfo *finfo,
					   MemoryContext per_tuple_context);


void
CreateTrigger(CreateTrigStmt *stmt)
{
	int16		tgtype;
	int16		tgattr[FUNC_MAX_ARGS];
	Datum		values[Natts_pg_trigger];
	char		nulls[Natts_pg_trigger];
	Relation	rel;
	Relation	tgrel;
	HeapScanDesc tgscan;
	ScanKeyData key;
	Relation	pgrel;
	HeapTuple	tuple;
	Relation	idescs[Num_pg_trigger_indices];
	Relation	ridescs[Num_pg_class_indices];
	Oid			fargtypes[FUNC_MAX_ARGS];
	Oid			funcoid;
	Oid			funclang;
	int			found = 0;
	int			i;
	char		constrtrigname[NAMEDATALEN];
	char	   *constrname = "";
	Oid			constrrelid = InvalidOid;

	if (!allowSystemTableMods && IsSystemRelationName(stmt->relname))
		elog(ERROR, "CreateTrigger: can't create trigger for system relation %s", stmt->relname);

	if (pg_aclcheck(stmt->relname, GetUserId(),
					stmt->isconstraint ? ACL_REFERENCES : ACL_TRIGGER)
		!= ACLCHECK_OK)
		elog(ERROR, "permission denied");

	/*
	 * If trigger is a constraint, user trigger name as constraint name
	 * and build a unique trigger name instead.
	 */
	if (stmt->isconstraint)
	{
		constrname = stmt->trigname;
		stmt->trigname = constrtrigname;
		sprintf(constrtrigname, "RI_ConstraintTrigger_%u", newoid());

		if (strcmp(stmt->constrrelname, "") == 0)
			constrrelid = InvalidOid;
		else
		{
			/*
			 * NoLock is probably sufficient here, since we're only
			 * interested in getting the relation's OID...
			 */
			rel = heap_openr(stmt->constrrelname, NoLock);
			constrrelid = rel->rd_id;
			heap_close(rel, NoLock);
		}
	}

	rel = heap_openr(stmt->relname, AccessExclusiveLock);

	if (rel->rd_rel->relkind != RELKIND_RELATION)
		elog(ERROR, "CreateTrigger: relation \"%s\" is not a table",
			 stmt->relname);

	TRIGGER_CLEAR_TYPE(tgtype);
	if (stmt->before)
		TRIGGER_SETT_BEFORE(tgtype);
	if (stmt->row)
		TRIGGER_SETT_ROW(tgtype);
	else
		elog(ERROR, "CreateTrigger: STATEMENT triggers are unimplemented, yet");

	for (i = 0; i < 3 && stmt->actions[i]; i++)
	{
		switch (stmt->actions[i])
		{
			case 'i':
				if (TRIGGER_FOR_INSERT(tgtype))
					elog(ERROR, "CreateTrigger: double INSERT event specified");
				TRIGGER_SETT_INSERT(tgtype);
				break;
			case 'd':
				if (TRIGGER_FOR_DELETE(tgtype))
					elog(ERROR, "CreateTrigger: double DELETE event specified");
				TRIGGER_SETT_DELETE(tgtype);
				break;
			case 'u':
				if (TRIGGER_FOR_UPDATE(tgtype))
					elog(ERROR, "CreateTrigger: double UPDATE event specified");
				TRIGGER_SETT_UPDATE(tgtype);
				break;
			default:
				elog(ERROR, "CreateTrigger: unknown event specified");
				break;
		}
	}

	/*
	 * Scan pg_trigger for existing triggers on relation.  NOTE that this
	 * is cool only because we have AccessExclusiveLock on the relation,
	 * so the trigger set won't be changing underneath us.
	 */
	tgrel = heap_openr(TriggerRelationName, RowExclusiveLock);
	ScanKeyEntryInitialize(&key, 0, Anum_pg_trigger_tgrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));
	tgscan = heap_beginscan(tgrel, 0, SnapshotNow, 1, &key);
	while (HeapTupleIsValid(tuple = heap_getnext(tgscan, 0)))
	{
		Form_pg_trigger pg_trigger = (Form_pg_trigger) GETSTRUCT(tuple);

		if (namestrcmp(&(pg_trigger->tgname), stmt->trigname) == 0)
			elog(ERROR, "CreateTrigger: trigger %s already defined on relation %s",
				 stmt->trigname, stmt->relname);
		found++;
	}
	heap_endscan(tgscan);

	/*
	 * Find and validate the trigger function.
	 */
	MemSet(fargtypes, 0, FUNC_MAX_ARGS * sizeof(Oid));
	tuple = SearchSysCache(PROCNAME,
						   PointerGetDatum(stmt->funcname),
						   Int32GetDatum(0),
						   PointerGetDatum(fargtypes),
						   0);
	if (!HeapTupleIsValid(tuple))
		elog(ERROR, "CreateTrigger: function %s() does not exist",
			 stmt->funcname);
	if (((Form_pg_proc) GETSTRUCT(tuple))->prorettype != 0)
		elog(ERROR, "CreateTrigger: function %s() must return OPAQUE",
			 stmt->funcname);
	funcoid = tuple->t_data->t_oid;
	funclang = ((Form_pg_proc) GETSTRUCT(tuple))->prolang;
	ReleaseSysCache(tuple);

	if (funclang != ClanguageId && funclang != INTERNALlanguageId)
	{
		HeapTuple	langTup;

		langTup = SearchSysCache(LANGOID,
								 ObjectIdGetDatum(funclang),
								 0, 0, 0);
		if (!HeapTupleIsValid(langTup))
			elog(ERROR, "CreateTrigger: cache lookup for language %u failed",
				 funclang);
		if (((Form_pg_language) GETSTRUCT(langTup))->lanispl == false)
			elog(ERROR, "CreateTrigger: only internal, C and PL functions are supported");
		ReleaseSysCache(langTup);
	}

	/*
	 * Build the new pg_trigger tuple.
	 */
	MemSet(nulls, ' ', Natts_pg_trigger * sizeof(char));

	values[Anum_pg_trigger_tgrelid - 1] = ObjectIdGetDatum(RelationGetRelid(rel));
	values[Anum_pg_trigger_tgname - 1] = DirectFunctionCall1(namein,
										CStringGetDatum(stmt->trigname));
	values[Anum_pg_trigger_tgfoid - 1] = ObjectIdGetDatum(funcoid);
	values[Anum_pg_trigger_tgtype - 1] = Int16GetDatum(tgtype);
	values[Anum_pg_trigger_tgenabled - 1] = BoolGetDatum(true);
	values[Anum_pg_trigger_tgisconstraint - 1] = BoolGetDatum(stmt->isconstraint);
	values[Anum_pg_trigger_tgconstrname - 1] = PointerGetDatum(constrname);
	values[Anum_pg_trigger_tgconstrrelid - 1] = ObjectIdGetDatum(constrrelid);
	values[Anum_pg_trigger_tgdeferrable - 1] = BoolGetDatum(stmt->deferrable);
	values[Anum_pg_trigger_tginitdeferred - 1] = BoolGetDatum(stmt->initdeferred);

	if (stmt->args)
	{
		List	   *le;
		char	   *args;
		int16		nargs = length(stmt->args);
		int			len = 0;

		foreach(le, stmt->args)
		{
			char	   *ar = ((Value *) lfirst(le))->val.str;

			len += strlen(ar) + 4;
			for (; *ar; ar++)
			{
				if (*ar == '\\')
					len++;
			}
		}
		args = (char *) palloc(len + 1);
		args[0] = '\0';
		foreach(le, stmt->args)
		{
			char	   *s = ((Value *) lfirst(le))->val.str;
			char	   *d = args + strlen(args);

			while (*s)
			{
				if (*s == '\\')
					*d++ = '\\';
				*d++ = *s++;
			}
			strcpy(d, "\\000");
		}
		values[Anum_pg_trigger_tgnargs - 1] = Int16GetDatum(nargs);
		values[Anum_pg_trigger_tgargs - 1] = DirectFunctionCall1(byteain,
												  CStringGetDatum(args));
	}
	else
	{
		values[Anum_pg_trigger_tgnargs - 1] = Int16GetDatum(0);
		values[Anum_pg_trigger_tgargs - 1] = DirectFunctionCall1(byteain,
													CStringGetDatum(""));
	}
	MemSet(tgattr, 0, FUNC_MAX_ARGS * sizeof(int16));
	values[Anum_pg_trigger_tgattr - 1] = PointerGetDatum(tgattr);

	tuple = heap_formtuple(tgrel->rd_att, values, nulls);

	/*
	 * Insert tuple into pg_trigger.
	 */
	heap_insert(tgrel, tuple);
	CatalogOpenIndices(Num_pg_trigger_indices, Name_pg_trigger_indices, idescs);
	CatalogIndexInsert(idescs, Num_pg_trigger_indices, tgrel, tuple);
	CatalogCloseIndices(Num_pg_trigger_indices, idescs);
	heap_freetuple(tuple);
	heap_close(tgrel, RowExclusiveLock);

	pfree(DatumGetPointer(values[Anum_pg_trigger_tgname - 1]));
	pfree(DatumGetPointer(values[Anum_pg_trigger_tgargs - 1]));

	/*
	 * Update relation's pg_class entry.  Crucial side-effect: other
	 * backends (and this one too!) are sent SI message to make them
	 * rebuild relcache entries.
	 */
	pgrel = heap_openr(RelationRelationName, RowExclusiveLock);
	tuple = SearchSysCacheCopy(RELNAME,
							   PointerGetDatum(stmt->relname),
							   0, 0, 0);
	if (!HeapTupleIsValid(tuple))
		elog(ERROR, "CreateTrigger: relation %s not found in pg_class",
			 stmt->relname);

	((Form_pg_class) GETSTRUCT(tuple))->reltriggers = found + 1;
	simple_heap_update(pgrel, &tuple->t_self, tuple);
	CatalogOpenIndices(Num_pg_class_indices, Name_pg_class_indices, ridescs);
	CatalogIndexInsert(ridescs, Num_pg_class_indices, pgrel, tuple);
	CatalogCloseIndices(Num_pg_class_indices, ridescs);
	heap_freetuple(tuple);
	heap_close(pgrel, RowExclusiveLock);

	/*
	 * We used to try to update the rel's relcache entry here, but that's
	 * fairly pointless since it will happen as a byproduct of the
	 * upcoming CommandCounterIncrement...
	 */

	/* Keep lock on target rel until end of xact */
	heap_close(rel, NoLock);
}

void
DropTrigger(DropTrigStmt *stmt)
{
	Relation	rel;
	Relation	tgrel;
	HeapScanDesc tgscan;
	ScanKeyData key;
	Relation	pgrel;
	HeapTuple	tuple;
	Relation	ridescs[Num_pg_class_indices];
	int			found = 0;
	int			tgfound = 0;

	if (!allowSystemTableMods && IsSystemRelationName(stmt->relname))
		elog(ERROR, "DropTrigger: can't drop trigger for system relation %s",
			 stmt->relname);

	if (!pg_ownercheck(GetUserId(), stmt->relname, RELNAME))
		elog(ERROR, "%s: %s", stmt->relname,
			 aclcheck_error_strings[ACLCHECK_NOT_OWNER]);

	rel = heap_openr(stmt->relname, AccessExclusiveLock);

	if (rel->rd_rel->relkind != RELKIND_RELATION)
		elog(ERROR, "DropTrigger: relation \"%s\" is not a table",
			 stmt->relname);

	/*
	 * Search pg_trigger, delete target trigger, count remaining triggers
	 * for relation.  Note this is OK only because we have
	 * AccessExclusiveLock on the rel, so no one else is creating/deleting
	 * triggers on this rel at the same time.
	 */
	tgrel = heap_openr(TriggerRelationName, RowExclusiveLock);
	ScanKeyEntryInitialize(&key, 0, Anum_pg_trigger_tgrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));
	tgscan = heap_beginscan(tgrel, 0, SnapshotNow, 1, &key);
	while (HeapTupleIsValid(tuple = heap_getnext(tgscan, 0)))
	{
		Form_pg_trigger pg_trigger = (Form_pg_trigger) GETSTRUCT(tuple);

		if (namestrcmp(&(pg_trigger->tgname), stmt->trigname) == 0)
		{
			/* Delete any comments associated with this trigger */
			DeleteComments(tuple->t_data->t_oid, RelationGetRelid(tgrel));

			simple_heap_delete(tgrel, &tuple->t_self);
			tgfound++;
		}
		else
			found++;
	}
	if (tgfound == 0)
		elog(ERROR, "DropTrigger: there is no trigger %s on relation %s",
			 stmt->trigname, stmt->relname);
	if (tgfound > 1)
		elog(NOTICE, "DropTrigger: found (and deleted) %d triggers %s on relation %s",
			 tgfound, stmt->trigname, stmt->relname);
	heap_endscan(tgscan);
	heap_close(tgrel, RowExclusiveLock);

	/*
	 * Update relation's pg_class entry.  Crucial side-effect: other
	 * backends (and this one too!) are sent SI message to make them
	 * rebuild relcache entries.
	 */
	pgrel = heap_openr(RelationRelationName, RowExclusiveLock);
	tuple = SearchSysCacheCopy(RELNAME,
							   PointerGetDatum(stmt->relname),
							   0, 0, 0);
	if (!HeapTupleIsValid(tuple))
		elog(ERROR, "DropTrigger: relation %s not found in pg_class",
			 stmt->relname);

	((Form_pg_class) GETSTRUCT(tuple))->reltriggers = found;
	simple_heap_update(pgrel, &tuple->t_self, tuple);
	CatalogOpenIndices(Num_pg_class_indices, Name_pg_class_indices, ridescs);
	CatalogIndexInsert(ridescs, Num_pg_class_indices, pgrel, tuple);
	CatalogCloseIndices(Num_pg_class_indices, ridescs);
	heap_freetuple(tuple);
	heap_close(pgrel, RowExclusiveLock);

	/*
	 * We used to try to update the rel's relcache entry here, but that's
	 * fairly pointless since it will happen as a byproduct of the
	 * upcoming CommandCounterIncrement...
	 */

	/* Keep lock on target rel until end of xact */
	heap_close(rel, NoLock);
}

/*
 * Remove all triggers for a relation that's being deleted.
 */
void
RelationRemoveTriggers(Relation rel)
{
	Relation	tgrel;
	HeapScanDesc tgscan;
	ScanKeyData key;
	HeapTuple	tup;
	bool		found = false;

	tgrel = heap_openr(TriggerRelationName, RowExclusiveLock);
	ScanKeyEntryInitialize(&key, 0, Anum_pg_trigger_tgrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));

	tgscan = heap_beginscan(tgrel, 0, SnapshotNow, 1, &key);

	while (HeapTupleIsValid(tup = heap_getnext(tgscan, 0)))
	{
		/* Delete any comments associated with this trigger */
		DeleteComments(tup->t_data->t_oid, RelationGetRelid(tgrel));

		simple_heap_delete(tgrel, &tup->t_self);

		found = true;
	}

	heap_endscan(tgscan);

	/*
	 * If we deleted any triggers, must update pg_class entry and advance
	 * command counter to make the updated entry visible. This is fairly
	 * annoying, since we'e just going to drop the durn thing later, but
	 * it's necessary to have a consistent state in case we do
	 * CommandCounterIncrement() below --- if RelationBuildTriggers()
	 * runs, it will complain otherwise. Perhaps RelationBuildTriggers()
	 * shouldn't be so picky...
	 */
	if (found)
	{
		Relation	pgrel;
		Relation	ridescs[Num_pg_class_indices];

		pgrel = heap_openr(RelationRelationName, RowExclusiveLock);
		tup = SearchSysCacheCopy(RELOID,
								 RelationGetRelid(rel),
								 0, 0, 0);
		if (!HeapTupleIsValid(tup))
			elog(ERROR, "RelationRemoveTriggers: relation %u not found in pg_class",
				 RelationGetRelid(rel));

		((Form_pg_class) GETSTRUCT(tup))->reltriggers = 0;
		simple_heap_update(pgrel, &tup->t_self, tup);
		CatalogOpenIndices(Num_pg_class_indices, Name_pg_class_indices, ridescs);
		CatalogIndexInsert(ridescs, Num_pg_class_indices, pgrel, tup);
		CatalogCloseIndices(Num_pg_class_indices, ridescs);
		heap_freetuple(tup);
		heap_close(pgrel, RowExclusiveLock);
		CommandCounterIncrement();
	}

	/*
	 * Also drop all constraint triggers referencing this relation
	 */
	ScanKeyEntryInitialize(&key, 0, Anum_pg_trigger_tgconstrrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));

	tgscan = heap_beginscan(tgrel, 0, SnapshotNow, 1, &key);
	while (HeapTupleIsValid(tup = heap_getnext(tgscan, 0)))
	{
		Form_pg_trigger pg_trigger;
		Relation	refrel;
		DropTrigStmt stmt;

		pg_trigger = (Form_pg_trigger) GETSTRUCT(tup);

		stmt.trigname = pstrdup(NameStr(pg_trigger->tgname));

		/* May as well grab AccessExclusiveLock, since DropTrigger will. */
		refrel = heap_open(pg_trigger->tgrelid, AccessExclusiveLock);
		stmt.relname = pstrdup(RelationGetRelationName(refrel));
		heap_close(refrel, NoLock);

		elog(NOTICE, "DROP TABLE implicitly drops referential integrity trigger from table \"%s\"", stmt.relname);

		DropTrigger(&stmt);

		/*
		 * Need to do a command counter increment here to show up new
		 * pg_class.reltriggers in the next loop iteration (in case there
		 * are multiple referential integrity action triggers for the same
		 * FK table defined on the PK table).
		 */
		CommandCounterIncrement();

		pfree(stmt.relname);
		pfree(stmt.trigname);
	}
	heap_endscan(tgscan);

	heap_close(tgrel, RowExclusiveLock);
}

/*
 * Build trigger data to attach to the given relcache entry.
 *
 * Note that trigger data must be allocated in CacheMemoryContext
 * to ensure it survives as long as the relcache entry.  But we
 * are probably running in a less long-lived working context.
 */
void
RelationBuildTriggers(Relation relation)
{
	TriggerDesc *trigdesc;
	int			ntrigs = relation->rd_rel->reltriggers;
	Trigger    *triggers = NULL;
	Trigger    *build;
	Relation	tgrel;
	Form_pg_trigger pg_trigger;
	Relation	irel = (Relation) NULL;
	ScanKeyData skey;
	HeapTupleData tuple;
	IndexScanDesc sd = (IndexScanDesc) NULL;
	HeapScanDesc tgscan = (HeapScanDesc) NULL;
	HeapTuple	htup;
	RetrieveIndexResult indexRes;
	Buffer		buffer;
	struct varlena *val;
	bool		isnull;
	int			found;
	bool		hasindex;

	trigdesc = (TriggerDesc *) MemoryContextAlloc(CacheMemoryContext,
												  sizeof(TriggerDesc));
	MemSet(trigdesc, 0, sizeof(TriggerDesc));

	ScanKeyEntryInitialize(&skey,
						   (bits16) 0x0,
						   (AttrNumber) 1,
						   (RegProcedure) F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(relation)));

	tgrel = heap_openr(TriggerRelationName, AccessShareLock);
	hasindex = (tgrel->rd_rel->relhasindex && !IsIgnoringSystemIndexes());
	if (hasindex)
	{
		irel = index_openr(TriggerRelidIndex);
		sd = index_beginscan(irel, false, 1, &skey);
	}
	else
		tgscan = heap_beginscan(tgrel, 0, SnapshotNow, 1, &skey);

	for (found = 0;;)
	{
		if (hasindex)
		{
			indexRes = index_getnext(sd, ForwardScanDirection);
			if (!indexRes)
				break;

			tuple.t_self = indexRes->heap_iptr;
			heap_fetch(tgrel, SnapshotNow, &tuple, &buffer, sd);
			pfree(indexRes);
			if (!tuple.t_data)
				continue;
			htup = &tuple;
		}
		else
		{
			htup = heap_getnext(tgscan, 0);
			if (!HeapTupleIsValid(htup))
				break;
		}
		if (found == ntrigs)
			elog(ERROR, "RelationBuildTriggers: unexpected record found for rel %s",
				 RelationGetRelationName(relation));

		pg_trigger = (Form_pg_trigger) GETSTRUCT(htup);

		if (triggers == NULL)
			triggers = (Trigger *) MemoryContextAlloc(CacheMemoryContext,
													  sizeof(Trigger));
		else
			triggers = (Trigger *) repalloc(triggers,
										  (found + 1) * sizeof(Trigger));
		build = &(triggers[found]);

		build->tgoid = htup->t_data->t_oid;
		build->tgname = MemoryContextStrdup(CacheMemoryContext,
							 DatumGetCString(DirectFunctionCall1(nameout,
									NameGetDatum(&pg_trigger->tgname))));
		build->tgfoid = pg_trigger->tgfoid;
		build->tgtype = pg_trigger->tgtype;
		build->tgenabled = pg_trigger->tgenabled;
		build->tgisconstraint = pg_trigger->tgisconstraint;
		build->tgdeferrable = pg_trigger->tgdeferrable;
		build->tginitdeferred = pg_trigger->tginitdeferred;
		build->tgnargs = pg_trigger->tgnargs;
		memcpy(build->tgattr, &(pg_trigger->tgattr),
			   FUNC_MAX_ARGS * sizeof(int16));
		if (build->tgnargs > 0)
		{
			char	   *p;
			int			i;

			val = (struct varlena *) fastgetattr(htup,
												 Anum_pg_trigger_tgargs,
												 tgrel->rd_att, &isnull);
			if (isnull)
				elog(ERROR, "RelationBuildTriggers: tgargs IS NULL for rel %s",
					 RelationGetRelationName(relation));
			p = (char *) VARDATA(val);
			build->tgargs = (char **)
				MemoryContextAlloc(CacheMemoryContext,
								   build->tgnargs * sizeof(char *));
			for (i = 0; i < build->tgnargs; i++)
			{
				build->tgargs[i] = MemoryContextStrdup(CacheMemoryContext,
													   p);
				p += strlen(p) + 1;
			}
		}
		else
			build->tgargs = NULL;

		found++;
		if (hasindex)
			ReleaseBuffer(buffer);
	}

	if (found < ntrigs)
		elog(ERROR, "RelationBuildTriggers: %d record(s) not found for rel %s",
			 ntrigs - found,
			 RelationGetRelationName(relation));

	if (hasindex)
	{
		index_endscan(sd);
		index_close(irel);
	}
	else
		heap_endscan(tgscan);
	heap_close(tgrel, AccessShareLock);

	/* Build trigdesc */
	trigdesc->triggers = triggers;
	trigdesc->numtriggers = ntrigs;
	for (found = 0; found < ntrigs; found++)
		InsertTrigger(trigdesc, &(triggers[found]), found);

	relation->trigdesc = trigdesc;
}

/* Insert the given trigger into the appropriate index list(s) for it */
static void
InsertTrigger(TriggerDesc *trigdesc, Trigger *trigger, int indx)
{
	uint16	   *n;
	int		  **t,
			  **tp;

	if (TRIGGER_FOR_ROW(trigger->tgtype))
	{
		/* ROW trigger */
		if (TRIGGER_FOR_BEFORE(trigger->tgtype))
		{
			n = trigdesc->n_before_row;
			t = trigdesc->tg_before_row;
		}
		else
		{
			n = trigdesc->n_after_row;
			t = trigdesc->tg_after_row;
		}
	}
	else
	{
		/* STATEMENT trigger */
		if (TRIGGER_FOR_BEFORE(trigger->tgtype))
		{
			n = trigdesc->n_before_statement;
			t = trigdesc->tg_before_statement;
		}
		else
		{
			n = trigdesc->n_after_statement;
			t = trigdesc->tg_after_statement;
		}
	}

	if (TRIGGER_FOR_INSERT(trigger->tgtype))
	{
		tp = &(t[TRIGGER_EVENT_INSERT]);
		if (*tp == NULL)
			*tp = (int *) MemoryContextAlloc(CacheMemoryContext,
											 sizeof(int));
		else
			*tp = (int *) repalloc(*tp, (n[TRIGGER_EVENT_INSERT] + 1) *
								   sizeof(int));
		(*tp)[n[TRIGGER_EVENT_INSERT]] = indx;
		(n[TRIGGER_EVENT_INSERT])++;
	}

	if (TRIGGER_FOR_DELETE(trigger->tgtype))
	{
		tp = &(t[TRIGGER_EVENT_DELETE]);
		if (*tp == NULL)
			*tp = (int *) MemoryContextAlloc(CacheMemoryContext,
											 sizeof(int));
		else
			*tp = (int *) repalloc(*tp, (n[TRIGGER_EVENT_DELETE] + 1) *
								   sizeof(int));
		(*tp)[n[TRIGGER_EVENT_DELETE]] = indx;
		(n[TRIGGER_EVENT_DELETE])++;
	}

	if (TRIGGER_FOR_UPDATE(trigger->tgtype))
	{
		tp = &(t[TRIGGER_EVENT_UPDATE]);
		if (*tp == NULL)
			*tp = (int *) MemoryContextAlloc(CacheMemoryContext,
											 sizeof(int));
		else
			*tp = (int *) repalloc(*tp, (n[TRIGGER_EVENT_UPDATE] + 1) *
								   sizeof(int));
		(*tp)[n[TRIGGER_EVENT_UPDATE]] = indx;
		(n[TRIGGER_EVENT_UPDATE])++;
	}
}

void
FreeTriggerDesc(TriggerDesc *trigdesc)
{
	int		  **t;
	Trigger    *trigger;
	int			i;

	if (trigdesc == NULL)
		return;

	t = trigdesc->tg_before_statement;
	for (i = 0; i < TRIGGER_NUM_EVENT_CLASSES; i++)
		if (t[i] != NULL)
			pfree(t[i]);
	t = trigdesc->tg_before_row;
	for (i = 0; i < TRIGGER_NUM_EVENT_CLASSES; i++)
		if (t[i] != NULL)
			pfree(t[i]);
	t = trigdesc->tg_after_row;
	for (i = 0; i < TRIGGER_NUM_EVENT_CLASSES; i++)
		if (t[i] != NULL)
			pfree(t[i]);
	t = trigdesc->tg_after_statement;
	for (i = 0; i < TRIGGER_NUM_EVENT_CLASSES; i++)
		if (t[i] != NULL)
			pfree(t[i]);

	trigger = trigdesc->triggers;
	for (i = 0; i < trigdesc->numtriggers; i++)
	{
		pfree(trigger->tgname);
		if (trigger->tgnargs > 0)
		{
			while (--(trigger->tgnargs) >= 0)
				pfree(trigger->tgargs[trigger->tgnargs]);
			pfree(trigger->tgargs);
		}
		trigger++;
	}
	pfree(trigdesc->triggers);
	pfree(trigdesc);
}

bool
equalTriggerDescs(TriggerDesc *trigdesc1, TriggerDesc *trigdesc2)
{
	int			i,
				j;

	/*
	 * We need not examine the "index" data, just the trigger array
	 * itself; if we have the same triggers with the same types, the
	 * derived index data should match.
	 */
	if (trigdesc1 != NULL)
	{
		if (trigdesc2 == NULL)
			return false;
		if (trigdesc1->numtriggers != trigdesc2->numtriggers)
			return false;
		for (i = 0; i < trigdesc1->numtriggers; i++)
		{
			Trigger    *trig1 = trigdesc1->triggers + i;
			Trigger    *trig2 = NULL;

			/*
			 * We can't assume that the triggers are always read from
			 * pg_trigger in the same order; so use the trigger OIDs to
			 * identify the triggers to compare.  (We assume here that the
			 * same OID won't appear twice in either trigger set.)
			 */
			for (j = 0; j < trigdesc2->numtriggers; j++)
			{
				trig2 = trigdesc2->triggers + j;
				if (trig1->tgoid == trig2->tgoid)
					break;
			}
			if (j >= trigdesc2->numtriggers)
				return false;
			if (strcmp(trig1->tgname, trig2->tgname) != 0)
				return false;
			if (trig1->tgfoid != trig2->tgfoid)
				return false;
			if (trig1->tgtype != trig2->tgtype)
				return false;
			if (trig1->tgenabled != trig2->tgenabled)
				return false;
			if (trig1->tgisconstraint != trig2->tgisconstraint)
				return false;
			if (trig1->tgdeferrable != trig2->tgdeferrable)
				return false;
			if (trig1->tginitdeferred != trig2->tginitdeferred)
				return false;
			if (trig1->tgnargs != trig2->tgnargs)
				return false;
			if (memcmp(trig1->tgattr, trig2->tgattr,
					   sizeof(trig1->tgattr)) != 0)
				return false;
			for (j = 0; j < trig1->tgnargs; j++)
				if (strcmp(trig1->tgargs[j], trig2->tgargs[j]) != 0)
					return false;
		}
	}
	else if (trigdesc2 != NULL)
		return false;
	return true;
}

/*
 * Call a trigger function.
 *
 *		trigdata: trigger descriptor.
 *		finfo: possibly-cached call info for the function.
 *		per_tuple_context: memory context to execute the function in.
 *
 * Returns the tuple (or NULL) as returned by the function.
 */
static HeapTuple
ExecCallTriggerFunc(TriggerData *trigdata,
					FmgrInfo *finfo,
					MemoryContext per_tuple_context)
{
	FunctionCallInfoData fcinfo;
	Datum		result;
	MemoryContext oldContext;

	/*
	 * We cache fmgr lookup info, to avoid making the lookup again on each
	 * call.
	 */
	if (finfo->fn_oid == InvalidOid)
		fmgr_info(trigdata->tg_trigger->tgfoid, finfo);

	Assert(finfo->fn_oid == trigdata->tg_trigger->tgfoid);

	/*
	 * Do the function evaluation in the per-tuple memory context, so that
	 * leaked memory will be reclaimed once per tuple. Note in particular
	 * that any new tuple created by the trigger function will live till
	 * the end of the tuple cycle.
	 */
	oldContext = MemoryContextSwitchTo(per_tuple_context);

	/*
	 * Call the function, passing no arguments but setting a context.
	 */
	MemSet(&fcinfo, 0, sizeof(fcinfo));

	fcinfo.flinfo = finfo;
	fcinfo.context = (Node *) trigdata;

	result = FunctionCallInvoke(&fcinfo);

	MemoryContextSwitchTo(oldContext);

	/*
	 * Trigger protocol allows function to return a null pointer, but NOT
	 * to set the isnull result flag.
	 */
	if (fcinfo.isnull)
		elog(ERROR, "ExecCallTriggerFunc: function %u returned NULL",
			 fcinfo.flinfo->fn_oid);

	return (HeapTuple) DatumGetPointer(result);
}

HeapTuple
ExecBRInsertTriggers(EState *estate, ResultRelInfo *relinfo,
					 HeapTuple trigtuple)
{
	TriggerDesc *trigdesc = relinfo->ri_TrigDesc;
	int			ntrigs = trigdesc->n_before_row[TRIGGER_EVENT_INSERT];
	int		   *tgindx = trigdesc->tg_before_row[TRIGGER_EVENT_INSERT];
	HeapTuple	newtuple = trigtuple;
	HeapTuple	oldtuple;
	TriggerData LocTriggerData;
	int			i;

	/* Allocate cache space for fmgr lookup info, if not done yet */
	if (relinfo->ri_TrigFunctions == NULL)
	{
		relinfo->ri_TrigFunctions = (FmgrInfo *)
			palloc(trigdesc->numtriggers * sizeof(FmgrInfo));
		MemSet(relinfo->ri_TrigFunctions, 0,
			   trigdesc->numtriggers * sizeof(FmgrInfo));
	}

	LocTriggerData.type = T_TriggerData;
	LocTriggerData.tg_event = TRIGGER_EVENT_INSERT | TRIGGER_EVENT_ROW | TRIGGER_EVENT_BEFORE;
	LocTriggerData.tg_relation = relinfo->ri_RelationDesc;
	LocTriggerData.tg_newtuple = NULL;
	for (i = 0; i < ntrigs; i++)
	{
		Trigger    *trigger = &trigdesc->triggers[tgindx[i]];

		if (!trigger->tgenabled)
			continue;
		LocTriggerData.tg_trigtuple = oldtuple = newtuple;
		LocTriggerData.tg_trigger = trigger;
		newtuple = ExecCallTriggerFunc(&LocTriggerData,
								   relinfo->ri_TrigFunctions + tgindx[i],
									   GetPerTupleMemoryContext(estate));
		if (oldtuple != newtuple && oldtuple != trigtuple)
			heap_freetuple(oldtuple);
		if (newtuple == NULL)
			break;
	}
	return newtuple;
}

void
ExecARInsertTriggers(EState *estate, ResultRelInfo *relinfo,
					 HeapTuple trigtuple)
{
	TriggerDesc *trigdesc = relinfo->ri_TrigDesc;

	if (trigdesc->n_after_row[TRIGGER_EVENT_INSERT] > 0)
		DeferredTriggerSaveEvent(relinfo, TRIGGER_EVENT_INSERT,
								 NULL, trigtuple);
}

bool
ExecBRDeleteTriggers(EState *estate, ResultRelInfo *relinfo,
					 ItemPointer tupleid)
{
	TriggerDesc *trigdesc = relinfo->ri_TrigDesc;
	int			ntrigs = trigdesc->n_before_row[TRIGGER_EVENT_DELETE];
	int		   *tgindx = trigdesc->tg_before_row[TRIGGER_EVENT_DELETE];
	TriggerData LocTriggerData;
	HeapTuple	trigtuple;
	HeapTuple	newtuple = NULL;
	TupleTableSlot *newSlot;
	int			i;

	trigtuple = GetTupleForTrigger(estate, relinfo, tupleid, &newSlot);
	if (trigtuple == NULL)
		return false;

	/* Allocate cache space for fmgr lookup info, if not done yet */
	if (relinfo->ri_TrigFunctions == NULL)
	{
		relinfo->ri_TrigFunctions = (FmgrInfo *)
			palloc(trigdesc->numtriggers * sizeof(FmgrInfo));
		MemSet(relinfo->ri_TrigFunctions, 0,
			   trigdesc->numtriggers * sizeof(FmgrInfo));
	}

	LocTriggerData.type = T_TriggerData;
	LocTriggerData.tg_event = TRIGGER_EVENT_DELETE | TRIGGER_EVENT_ROW | TRIGGER_EVENT_BEFORE;
	LocTriggerData.tg_relation = relinfo->ri_RelationDesc;
	LocTriggerData.tg_newtuple = NULL;
	for (i = 0; i < ntrigs; i++)
	{
		Trigger    *trigger = &trigdesc->triggers[tgindx[i]];

		if (!trigger->tgenabled)
			continue;
		LocTriggerData.tg_trigtuple = trigtuple;
		LocTriggerData.tg_trigger = trigger;
		newtuple = ExecCallTriggerFunc(&LocTriggerData,
								   relinfo->ri_TrigFunctions + tgindx[i],
									   GetPerTupleMemoryContext(estate));
		if (newtuple == NULL)
			break;
		if (newtuple != trigtuple)
			heap_freetuple(newtuple);
	}
	heap_freetuple(trigtuple);

	return (newtuple == NULL) ? false : true;
}

void
ExecARDeleteTriggers(EState *estate, ResultRelInfo *relinfo,
					 ItemPointer tupleid)
{
	TriggerDesc *trigdesc = relinfo->ri_TrigDesc;

	if (trigdesc->n_after_row[TRIGGER_EVENT_DELETE] > 0)
	{
		HeapTuple	trigtuple = GetTupleForTrigger(estate, relinfo,
												   tupleid, NULL);

		DeferredTriggerSaveEvent(relinfo, TRIGGER_EVENT_DELETE,
								 trigtuple, NULL);
		heap_freetuple(trigtuple);
	}
}

HeapTuple
ExecBRUpdateTriggers(EState *estate, ResultRelInfo *relinfo,
					 ItemPointer tupleid, HeapTuple newtuple)
{
	TriggerDesc *trigdesc = relinfo->ri_TrigDesc;
	int			ntrigs = trigdesc->n_before_row[TRIGGER_EVENT_UPDATE];
	int		   *tgindx = trigdesc->tg_before_row[TRIGGER_EVENT_UPDATE];
	TriggerData LocTriggerData;
	HeapTuple	trigtuple;
	HeapTuple	oldtuple;
	HeapTuple	intuple = newtuple;
	TupleTableSlot *newSlot;
	int			i;

	trigtuple = GetTupleForTrigger(estate, relinfo, tupleid, &newSlot);
	if (trigtuple == NULL)
		return NULL;

	/*
	 * In READ COMMITTED isolevel it's possible that newtuple was changed
	 * due to concurrent update.
	 */
	if (newSlot != NULL)
		intuple = newtuple = ExecRemoveJunk(estate->es_junkFilter, newSlot);

	/* Allocate cache space for fmgr lookup info, if not done yet */
	if (relinfo->ri_TrigFunctions == NULL)
	{
		relinfo->ri_TrigFunctions = (FmgrInfo *)
			palloc(trigdesc->numtriggers * sizeof(FmgrInfo));
		MemSet(relinfo->ri_TrigFunctions, 0,
			   trigdesc->numtriggers * sizeof(FmgrInfo));
	}

	LocTriggerData.type = T_TriggerData;
	LocTriggerData.tg_event = TRIGGER_EVENT_UPDATE | TRIGGER_EVENT_ROW | TRIGGER_EVENT_BEFORE;
	LocTriggerData.tg_relation = relinfo->ri_RelationDesc;
	for (i = 0; i < ntrigs; i++)
	{
		Trigger    *trigger = &trigdesc->triggers[tgindx[i]];

		if (!trigger->tgenabled)
			continue;
		LocTriggerData.tg_trigtuple = trigtuple;
		LocTriggerData.tg_newtuple = oldtuple = newtuple;
		LocTriggerData.tg_trigger = trigger;
		newtuple = ExecCallTriggerFunc(&LocTriggerData,
								   relinfo->ri_TrigFunctions + tgindx[i],
									   GetPerTupleMemoryContext(estate));
		if (oldtuple != newtuple && oldtuple != intuple)
			heap_freetuple(oldtuple);
		if (newtuple == NULL)
			break;
	}
	heap_freetuple(trigtuple);
	return newtuple;
}

void
ExecARUpdateTriggers(EState *estate, ResultRelInfo *relinfo,
					 ItemPointer tupleid, HeapTuple newtuple)
{
	TriggerDesc *trigdesc = relinfo->ri_TrigDesc;

	if (trigdesc->n_after_row[TRIGGER_EVENT_UPDATE] > 0)
	{
		HeapTuple	trigtuple = GetTupleForTrigger(estate, relinfo,
												   tupleid, NULL);

		DeferredTriggerSaveEvent(relinfo, TRIGGER_EVENT_UPDATE,
								 trigtuple, newtuple);
		heap_freetuple(trigtuple);
	}
}


static HeapTuple
GetTupleForTrigger(EState *estate, ResultRelInfo *relinfo,
				   ItemPointer tid, TupleTableSlot **newSlot)
{
	Relation	relation = relinfo->ri_RelationDesc;
	HeapTupleData tuple;
	HeapTuple	result;
	Buffer		buffer;

	if (newSlot != NULL)
	{
		int			test;

		/*
		 * mark tuple for update
		 */
		*newSlot = NULL;
		tuple.t_self = *tid;
ltrmark:;
		test = heap_mark4update(relation, &tuple, &buffer);
		switch (test)
		{
			case HeapTupleSelfUpdated:
				ReleaseBuffer(buffer);
				return (NULL);

			case HeapTupleMayBeUpdated:
				break;

			case HeapTupleUpdated:
				ReleaseBuffer(buffer);
				if (XactIsoLevel == XACT_SERIALIZABLE)
					elog(ERROR, "Can't serialize access due to concurrent update");
				else if (!(ItemPointerEquals(&(tuple.t_self), tid)))
				{
					TupleTableSlot *epqslot = EvalPlanQual(estate,
											 relinfo->ri_RangeTableIndex,
														&(tuple.t_self));

					if (!(TupIsNull(epqslot)))
					{
						*tid = tuple.t_self;
						*newSlot = epqslot;
						goto ltrmark;
					}
				}

				/*
				 * if tuple was deleted or PlanQual failed for updated
				 * tuple - we have not process this tuple!
				 */
				return (NULL);

			default:
				ReleaseBuffer(buffer);
				elog(ERROR, "Unknown status %u from heap_mark4update", test);
				return (NULL);
		}
	}
	else
	{
		PageHeader	dp;
		ItemId		lp;

		buffer = ReadBuffer(relation, ItemPointerGetBlockNumber(tid));

		if (!BufferIsValid(buffer))
			elog(ERROR, "GetTupleForTrigger: failed ReadBuffer");

		dp = (PageHeader) BufferGetPage(buffer);
		lp = PageGetItemId(dp, ItemPointerGetOffsetNumber(tid));

		Assert(ItemIdIsUsed(lp));

		tuple.t_datamcxt = NULL;
		tuple.t_data = (HeapTupleHeader) PageGetItem((Page) dp, lp);
		tuple.t_len = ItemIdGetLength(lp);
		tuple.t_self = *tid;
	}

	result = heap_copytuple(&tuple);
	ReleaseBuffer(buffer);

	return result;
}


/* ----------
 * Deferred trigger stuff
 * ----------
 */


/* ----------
 * Internal data to the deferred trigger mechanism is held
 * during entire session in a global context created at startup and
 * over statements/commands in a separate context which
 * is created at transaction start and destroyed at transaction end.
 * ----------
 */
static MemoryContext deftrig_gcxt = NULL;
static MemoryContext deftrig_cxt = NULL;

/* ----------
 * Global data that tells which triggers are actually in
 * state IMMEDIATE or DEFERRED.
 * ----------
 */
static bool deftrig_dfl_all_isset = false;
static bool deftrig_dfl_all_isdeferred = false;
static List *deftrig_dfl_trigstates = NIL;

static bool deftrig_all_isset;
static bool deftrig_all_isdeferred;
static List *deftrig_trigstates;

/* ----------
 * The list of pending deferred trigger events during the current transaction.
 *
 * deftrig_events is the head, deftrig_event_tail is the last entry.
 * Because this can grow pretty large, we don't use separate List nodes,
 * but instead thread the list through the dte_next fields of the member
 * nodes.  Saves just a few bytes per entry, but that adds up.
 *
 * XXX Need to be able to shove this data out to a file if it grows too
 *	   large...
 * ----------
 */
static DeferredTriggerEvent deftrig_events;
static DeferredTriggerEvent deftrig_event_tail;


/* ----------
 * deferredTriggerCheckState()
 *
 *	Returns true if the trigger identified by tgoid is actually
 *	in state DEFERRED.
 * ----------
 */
static bool
deferredTriggerCheckState(Oid tgoid, int32 itemstate)
{
	MemoryContext oldcxt;
	List	   *sl;
	DeferredTriggerStatus trigstate;

	/*
	 * Not deferrable triggers (i.e. normal AFTER ROW triggers and
	 * constraints declared NOT DEFERRABLE, the state is allways false.
	 */
	if ((itemstate & TRIGGER_DEFERRED_DEFERRABLE) == 0)
		return false;

	/*
	 * Lookup if we know an individual state for this trigger
	 */
	foreach(sl, deftrig_trigstates)
	{
		trigstate = (DeferredTriggerStatus) lfirst(sl);
		if (trigstate->dts_tgoid == tgoid)
			return trigstate->dts_tgisdeferred;
	}

	/*
	 * No individual state known - so if the user issued a SET CONSTRAINT
	 * ALL ..., we return that instead of the triggers default state.
	 */
	if (deftrig_all_isset)
		return deftrig_all_isdeferred;

	/*
	 * No ALL state known either, remember the default state as the
	 * current and return that.
	 */
	oldcxt = MemoryContextSwitchTo(deftrig_cxt);

	trigstate = (DeferredTriggerStatus)
		palloc(sizeof(DeferredTriggerStatusData));
	trigstate->dts_tgoid = tgoid;
	trigstate->dts_tgisdeferred =
		((itemstate & TRIGGER_DEFERRED_INITDEFERRED) != 0);
	deftrig_trigstates = lappend(deftrig_trigstates, trigstate);

	MemoryContextSwitchTo(oldcxt);

	return trigstate->dts_tgisdeferred;
}


/* ----------
 * deferredTriggerAddEvent()
 *
 *	Add a new trigger event to the queue.
 * ----------
 */
static void
deferredTriggerAddEvent(DeferredTriggerEvent event)
{
	/*
	 * Since the event list could grow quite long, we keep track of the
	 * list tail and append there, rather than just doing a stupid
	 * "lappend". This avoids O(N^2) behavior for large numbers of events.
	 */
	event->dte_next = NULL;
	if (deftrig_event_tail == NULL)
	{
		/* first list entry */
		deftrig_events = event;
		deftrig_event_tail = event;
	}
	else
	{
		deftrig_event_tail->dte_next = event;
		deftrig_event_tail = event;
	}
}


/* ----------
 * DeferredTriggerExecute()
 *
 *	Fetch the required tuples back from the heap and fire one
 *	single trigger function.
 *
 *	Frequently, this will be fired many times in a row for triggers of
 *	a single relation.	Therefore, we cache the open relation and provide
 *	fmgr lookup cache space at the caller level.
 *
 *	event: event currently being fired.
 *	itemno: item within event currently being fired.
 *	rel: open relation for event.
 *	finfo: array of fmgr lookup cache entries (one per trigger of relation).
 *	per_tuple_context: memory context to call trigger function in.
 * ----------
 */
static void
DeferredTriggerExecute(DeferredTriggerEvent event, int itemno,
					   Relation rel, FmgrInfo *finfo,
					   MemoryContext per_tuple_context)
{
	Oid			tgoid = event->dte_item[itemno].dti_tgoid;
	TriggerDesc *trigdesc = rel->trigdesc;
	TriggerData LocTriggerData;
	HeapTupleData oldtuple;
	HeapTupleData newtuple;
	HeapTuple	rettuple;
	Buffer		oldbuffer;
	Buffer		newbuffer;
	int			tgindx;

	/*
	 * Fetch the required OLD and NEW tuples.
	 */
	if (ItemPointerIsValid(&(event->dte_oldctid)))
	{
		ItemPointerCopy(&(event->dte_oldctid), &(oldtuple.t_self));
		heap_fetch(rel, SnapshotAny, &oldtuple, &oldbuffer, NULL);
		if (!oldtuple.t_data)
			elog(ERROR, "DeferredTriggerExecute: failed to fetch old tuple");
	}

	if (ItemPointerIsValid(&(event->dte_newctid)))
	{
		ItemPointerCopy(&(event->dte_newctid), &(newtuple.t_self));
		heap_fetch(rel, SnapshotAny, &newtuple, &newbuffer, NULL);
		if (!newtuple.t_data)
			elog(ERROR, "DeferredTriggerExecute: failed to fetch new tuple");
	}

	/*
	 * Setup the trigger information
	 */
	LocTriggerData.type = T_TriggerData;
	LocTriggerData.tg_event = (event->dte_event & TRIGGER_EVENT_OPMASK) |
		TRIGGER_EVENT_ROW;
	LocTriggerData.tg_relation = rel;

	LocTriggerData.tg_trigger = NULL;
	for (tgindx = 0; tgindx < trigdesc->numtriggers; tgindx++)
	{
		if (trigdesc->triggers[tgindx].tgoid == tgoid)
		{
			LocTriggerData.tg_trigger = &(trigdesc->triggers[tgindx]);
			break;
		}
	}
	if (LocTriggerData.tg_trigger == NULL)
		elog(ERROR, "DeferredTriggerExecute: can't find trigger %u", tgoid);

	switch (event->dte_event & TRIGGER_EVENT_OPMASK)
	{
		case TRIGGER_EVENT_INSERT:
			LocTriggerData.tg_trigtuple = &newtuple;
			LocTriggerData.tg_newtuple = NULL;
			break;

		case TRIGGER_EVENT_UPDATE:
			LocTriggerData.tg_trigtuple = &oldtuple;
			LocTriggerData.tg_newtuple = &newtuple;
			break;

		case TRIGGER_EVENT_DELETE:
			LocTriggerData.tg_trigtuple = &oldtuple;
			LocTriggerData.tg_newtuple = NULL;
			break;
	}

	/*
	 * Call the trigger and throw away an eventually returned updated
	 * tuple.
	 */
	rettuple = ExecCallTriggerFunc(&LocTriggerData,
								   finfo + tgindx,
								   per_tuple_context);
	if (rettuple != NULL && rettuple != &oldtuple && rettuple != &newtuple)
		heap_freetuple(rettuple);

	/*
	 * Might have been a referential integrity constraint trigger. Reset
	 * the snapshot overriding flag.
	 */
	ReferentialIntegritySnapshotOverride = false;

	/*
	 * Release buffers
	 */
	if (ItemPointerIsValid(&(event->dte_oldctid)))
		ReleaseBuffer(oldbuffer);
	if (ItemPointerIsValid(&(event->dte_newctid)))
		ReleaseBuffer(newbuffer);
}


/* ----------
 * deferredTriggerInvokeEvents()
 *
 *	Scan the event queue for not yet invoked triggers. Check if they
 *	should be invoked now and do so.
 * ----------
 */
static void
deferredTriggerInvokeEvents(bool immediate_only)
{
	DeferredTriggerEvent event,
				prev_event = NULL;
	MemoryContext per_tuple_context;
	Relation	rel = NULL;
	FmgrInfo   *finfo = NULL;

	/*
	 * If immediate_only is true, we remove fully-processed events from
	 * the event queue to recycle space.  If immediate_only is false,
	 * we are going to discard the whole event queue on return anyway,
	 * so no need to bother with "retail" pfree's.
	 *
	 * In a scenario with many commands in a transaction and many
	 * deferred-to-end-of-transaction triggers, it could get annoying
	 * to rescan all the deferred triggers at each command end.
	 * To speed this up, we could remember the actual end of the queue at
	 * EndQuery and examine only events that are newer. On state changes 
	 * we simply reset the saved position to the beginning of the queue 
	 * and process all events once with the new states.
	 */

	/* Make a per-tuple memory context for trigger function calls */
	per_tuple_context =
		AllocSetContextCreate(CurrentMemoryContext,
							  "DeferredTriggerTupleContext",
							  ALLOCSET_DEFAULT_MINSIZE,
							  ALLOCSET_DEFAULT_INITSIZE,
							  ALLOCSET_DEFAULT_MAXSIZE);

	event = deftrig_events;
	while (event != NULL)
	{
		bool		still_deferred_ones = false;
		DeferredTriggerEvent next_event;
		int			i;

		/*
		 * Check if event is already completely done.
		 */
		if (! (event->dte_event & (TRIGGER_DEFERRED_DONE |
								   TRIGGER_DEFERRED_CANCELED)))
		{
			MemoryContextReset(per_tuple_context);

			/*
			 * Check each trigger item in the event.
			 */
			for (i = 0; i < event->dte_n_items; i++)
			{
				if (event->dte_item[i].dti_state & TRIGGER_DEFERRED_DONE)
					continue;

				/*
				 * This trigger item hasn't been called yet. Check if we
				 * should call it now.
				 */
				if (immediate_only &&
					deferredTriggerCheckState(event->dte_item[i].dti_tgoid,
											  event->dte_item[i].dti_state))
				{
					still_deferred_ones = true;
					continue;
				}

				/*
				 * So let's fire it... but first, open the correct relation
				 * if this is not the same relation as before.
				 */
				if (rel == NULL || rel->rd_id != event->dte_relid)
				{
					if (rel)
						heap_close(rel, NoLock);
					if (finfo)
						pfree(finfo);

					/*
					 * We assume that an appropriate lock is still held by the
					 * executor, so grab no new lock here.
					 */
					rel = heap_open(event->dte_relid, NoLock);

					/*
					 * Allocate space to cache fmgr lookup info for triggers
					 * of this relation.
					 */
					finfo = (FmgrInfo *)
						palloc(rel->trigdesc->numtriggers * sizeof(FmgrInfo));
					MemSet(finfo, 0,
						   rel->trigdesc->numtriggers * sizeof(FmgrInfo));
				}

				DeferredTriggerExecute(event, i, rel, finfo,
									   per_tuple_context);

				event->dte_item[i].dti_state |= TRIGGER_DEFERRED_DONE;
			} /* end loop over items within event */
		}

		/*
		 * If it's now completely done, throw it away.
		 *
		 * NB: it's possible the trigger calls above added more events to the
		 * queue, or that calls we will do later will want to add more,
		 * so we have to be careful about maintaining list validity here.
		 */
		next_event = event->dte_next;

		if (still_deferred_ones)
		{
			/* Not done, keep in list */
			prev_event = event;
		}
		else
		{
			/* Done */
			if (immediate_only)
			{
				/* delink it from list and free it */
				if (prev_event)
					prev_event->dte_next = next_event;
				else
					deftrig_events = next_event;
				pfree(event);
			}
			else
			{
				/*
				 * We will clean up later, but just for paranoia's sake,
				 * mark the event done.
				 */
				event->dte_event |= TRIGGER_DEFERRED_DONE;
			}
		}

		event = next_event;
	}

	/* Update list tail pointer in case we just deleted tail event */
	deftrig_event_tail = prev_event;

	/* Release working resources */
	if (rel)
		heap_close(rel, NoLock);
	if (finfo)
		pfree(finfo);
	MemoryContextDelete(per_tuple_context);
}


/* ----------
 * DeferredTriggerInit()
 *
 *	Initialize the deferred trigger mechanism. This is called during
 *	backend startup and is guaranteed to be before the first of all
 *	transactions.
 * ----------
 */
void
DeferredTriggerInit(void)
{
	/*
	 * Since this context will never be reset, give it a minsize of 0.
	 * This avoids using any memory if the session never stores anything.
	 */
	deftrig_gcxt = AllocSetContextCreate(TopMemoryContext,
										 "DeferredTriggerSession",
										 0,
										 ALLOCSET_DEFAULT_INITSIZE,
										 ALLOCSET_DEFAULT_MAXSIZE);
}


/* ----------
 * DeferredTriggerBeginXact()
 *
 *	Called at transaction start (either BEGIN or implicit for single
 *	statement outside of transaction block).
 * ----------
 */
void
DeferredTriggerBeginXact(void)
{
	MemoryContext oldcxt;
	List	   *l;
	DeferredTriggerStatus dflstat;
	DeferredTriggerStatus stat;

	if (deftrig_cxt != NULL)
		elog(ERROR,
		   "DeferredTriggerBeginXact() called while inside transaction");

	/*
	 * Create the per transaction memory context and copy all states from
	 * the per session context to here.  Set the minsize to 0 to avoid
	 * wasting memory if there is no deferred trigger data.
	 */
	deftrig_cxt = AllocSetContextCreate(TopTransactionContext,
										"DeferredTriggerXact",
										0,
										ALLOCSET_DEFAULT_INITSIZE,
										ALLOCSET_DEFAULT_MAXSIZE);
	oldcxt = MemoryContextSwitchTo(deftrig_cxt);

	deftrig_all_isset = deftrig_dfl_all_isset;
	deftrig_all_isdeferred = deftrig_dfl_all_isdeferred;

	deftrig_trigstates = NIL;
	foreach(l, deftrig_dfl_trigstates)
	{
		dflstat = (DeferredTriggerStatus) lfirst(l);
		stat = (DeferredTriggerStatus)
			palloc(sizeof(DeferredTriggerStatusData));

		stat->dts_tgoid = dflstat->dts_tgoid;
		stat->dts_tgisdeferred = dflstat->dts_tgisdeferred;

		deftrig_trigstates = lappend(deftrig_trigstates, stat);
	}

	MemoryContextSwitchTo(oldcxt);

	deftrig_events = NULL;
	deftrig_event_tail = NULL;
}


/* ----------
 * DeferredTriggerEndQuery()
 *
 *	Called after one query sent down by the user has completely been
 *	processed. At this time we invoke all outstanding IMMEDIATE triggers.
 * ----------
 */
void
DeferredTriggerEndQuery(void)
{
	/*
	 * Ignore call if we aren't in a transaction.
	 */
	if (deftrig_cxt == NULL)
		return;

	deferredTriggerInvokeEvents(true);
}


/* ----------
 * DeferredTriggerEndXact()
 *
 *	Called just before the current transaction is committed. At this
 *	time we invoke all DEFERRED triggers and tidy up.
 * ----------
 */
void
DeferredTriggerEndXact(void)
{
	/*
	 * Ignore call if we aren't in a transaction.
	 */
	if (deftrig_cxt == NULL)
		return;

	deferredTriggerInvokeEvents(false);

	MemoryContextDelete(deftrig_cxt);
	deftrig_cxt = NULL;
}


/* ----------
 * DeferredTriggerAbortXact()
 *
 *	The current transaction has entered the abort state.
 *	All outstanding triggers are canceled so we simply throw
 *	away anything we know.
 * ----------
 */
void
DeferredTriggerAbortXact(void)
{
	/*
	 * Ignore call if we aren't in a transaction.
	 */
	if (deftrig_cxt == NULL)
		return;

	MemoryContextDelete(deftrig_cxt);
	deftrig_cxt = NULL;
}


/* ----------
 * DeferredTriggerSetState()
 *
 *	Called for the users SET CONSTRAINTS ... utility command.
 * ----------
 */
void
DeferredTriggerSetState(ConstraintsSetStmt *stmt)
{
	Relation	tgrel;
	Relation	irel = (Relation) NULL;
	List	   *l;
	List	   *ls;
	List	   *loid = NIL;
	MemoryContext oldcxt;
	bool		found;
	DeferredTriggerStatus state;
	bool		hasindex;

	/*
	 * Handle SET CONSTRAINTS ALL ...
	 */
	if (stmt->constraints == NIL)
	{
		if (!IsTransactionBlock())
		{
			/*
			 * ... outside of a transaction block
			 *
			 * Drop all information about individual trigger states per
			 * session.
			 */
			l = deftrig_dfl_trigstates;
			while (l != NIL)
			{
				List	   *next = lnext(l);

				pfree(lfirst(l));
				pfree(l);
				l = next;
			}
			deftrig_dfl_trigstates = NIL;

			/*
			 * Set the session ALL state to known.
			 */
			deftrig_dfl_all_isset = true;
			deftrig_dfl_all_isdeferred = stmt->deferred;

			return;
		}
		else
		{
			/*
			 * ... inside of a transaction block
			 *
			 * Drop all information about individual trigger states per
			 * transaction.
			 */
			l = deftrig_trigstates;
			while (l != NIL)
			{
				List	   *next = lnext(l);

				pfree(lfirst(l));
				pfree(l);
				l = next;
			}
			deftrig_trigstates = NIL;

			/*
			 * Set the per transaction ALL state to known.
			 */
			deftrig_all_isset = true;
			deftrig_all_isdeferred = stmt->deferred;

			return;
		}
	}

	/* ----------
	 * Handle SET CONSTRAINTS constraint-name [, ...]
	 * First lookup all trigger Oid's for the constraint names.
	 * ----------
	 */
	tgrel = heap_openr(TriggerRelationName, AccessShareLock);
	hasindex = (tgrel->rd_rel->relhasindex && !IsIgnoringSystemIndexes());
	if (hasindex)
		irel = index_openr(TriggerConstrNameIndex);

	foreach(l, stmt->constraints)
	{
		ScanKeyData skey;
		HeapTupleData tuple;
		IndexScanDesc sd = (IndexScanDesc) NULL;
		HeapScanDesc tgscan = (HeapScanDesc) NULL;
		HeapTuple	htup;
		RetrieveIndexResult indexRes;
		Buffer		buffer;
		Form_pg_trigger pg_trigger;
		Oid			constr_oid;

		/*
		 * Check that only named constraints are set explicitly
		 */
		if (strcmp((char *) lfirst(l), "") == 0)
			elog(ERROR, "unnamed constraints cannot be set explicitly");

		/*
		 * Setup to scan pg_trigger by tgconstrname ...
		 */
		ScanKeyEntryInitialize(&skey,
							   (bits16) 0x0,
							   (AttrNumber) 1,
							   (RegProcedure) F_NAMEEQ,
							   PointerGetDatum((char *) lfirst(l)));

		if (hasindex)
			sd = index_beginscan(irel, false, 1, &skey);
		else
			tgscan = heap_beginscan(tgrel, 0, SnapshotNow, 1, &skey);

		/*
		 * ... and search for the constraint trigger row
		 */
		found = false;
		for (;;)
		{
			if (hasindex)
			{
				indexRes = index_getnext(sd, ForwardScanDirection);
				if (!indexRes)
					break;

				tuple.t_self = indexRes->heap_iptr;
				heap_fetch(tgrel, SnapshotNow, &tuple, &buffer, sd);
				pfree(indexRes);
				if (!tuple.t_data)
					continue;
				htup = &tuple;
			}
			else
			{
				htup = heap_getnext(tgscan, 0);
				if (!HeapTupleIsValid(htup))
					break;
			}

			/*
			 * If we found some, check that they fit the deferrability but
			 * skip ON <event> RESTRICT ones, since they are silently
			 * never deferrable.
			 */
			pg_trigger = (Form_pg_trigger) GETSTRUCT(htup);
			if (stmt->deferred && !pg_trigger->tgdeferrable &&
				pg_trigger->tgfoid != F_RI_FKEY_RESTRICT_UPD &&
				pg_trigger->tgfoid != F_RI_FKEY_RESTRICT_DEL)
				elog(ERROR, "Constraint '%s' is not deferrable",
					 (char *) lfirst(l));

			constr_oid = htup->t_data->t_oid;
			loid = lappendi(loid, constr_oid);
			found = true;

			if (hasindex)
				ReleaseBuffer(buffer);
		}

		/*
		 * Not found ?
		 */
		if (!found)
			elog(ERROR, "Constraint '%s' does not exist", (char *) lfirst(l));

		if (hasindex)
			index_endscan(sd);
		else
			heap_endscan(tgscan);
	}
	if (hasindex)
		index_close(irel);
	heap_close(tgrel, AccessShareLock);

	if (!IsTransactionBlock())
	{
		/*
		 * Outside of a transaction block set the trigger states of
		 * individual triggers on session level.
		 */
		oldcxt = MemoryContextSwitchTo(deftrig_gcxt);

		foreach(l, loid)
		{
			found = false;
			foreach(ls, deftrig_dfl_trigstates)
			{
				state = (DeferredTriggerStatus) lfirst(ls);
				if (state->dts_tgoid == (Oid) lfirsti(l))
				{
					state->dts_tgisdeferred = stmt->deferred;
					found = true;
					break;
				}
			}
			if (!found)
			{
				state = (DeferredTriggerStatus)
					palloc(sizeof(DeferredTriggerStatusData));
				state->dts_tgoid = (Oid) lfirsti(l);
				state->dts_tgisdeferred = stmt->deferred;

				deftrig_dfl_trigstates =
					lappend(deftrig_dfl_trigstates, state);
			}
		}

		MemoryContextSwitchTo(oldcxt);

		return;
	}
	else
	{
		/*
		 * Inside of a transaction block set the trigger states of
		 * individual triggers on transaction level.
		 */
		oldcxt = MemoryContextSwitchTo(deftrig_cxt);

		foreach(l, loid)
		{
			found = false;
			foreach(ls, deftrig_trigstates)
			{
				state = (DeferredTriggerStatus) lfirst(ls);
				if (state->dts_tgoid == (Oid) lfirsti(l))
				{
					state->dts_tgisdeferred = stmt->deferred;
					found = true;
					break;
				}
			}
			if (!found)
			{
				state = (DeferredTriggerStatus)
					palloc(sizeof(DeferredTriggerStatusData));
				state->dts_tgoid = (Oid) lfirsti(l);
				state->dts_tgisdeferred = stmt->deferred;

				deftrig_trigstates =
					lappend(deftrig_trigstates, state);
			}
		}

		MemoryContextSwitchTo(oldcxt);

		return;
	}
}


/* ----------
 * DeferredTriggerSaveEvent()
 *
 *	Called by ExecAR...Triggers() to add the event to the queue.
 *
 *	NOTE: should be called only if we've determined that an event must
 *	be added to the queue.
 * ----------
 */
static void
DeferredTriggerSaveEvent(ResultRelInfo *relinfo, int event,
						 HeapTuple oldtup, HeapTuple newtup)
{
	Relation	rel = relinfo->ri_RelationDesc;
	TriggerDesc *trigdesc = relinfo->ri_TrigDesc;
	MemoryContext oldcxt;
	DeferredTriggerEvent new_event;
	int			new_size;
	int			i;
	int			ntriggers;
	int		   *tgindx;
	ItemPointerData oldctid;
	ItemPointerData newctid;
	TriggerData LocTriggerData;

	if (deftrig_cxt == NULL)
		elog(ERROR,
			 "DeferredTriggerSaveEvent() called outside of transaction");

	/*
	 * Get the CTID's of OLD and NEW
	 */
	if (oldtup != NULL)
		ItemPointerCopy(&(oldtup->t_self), &(oldctid));
	else
		ItemPointerSetInvalid(&(oldctid));
	if (newtup != NULL)
		ItemPointerCopy(&(newtup->t_self), &(newctid));
	else
		ItemPointerSetInvalid(&(newctid));

	/*
	 * Create a new event
	 */
	oldcxt = MemoryContextSwitchTo(deftrig_cxt);

	ntriggers = trigdesc->n_after_row[event];
	tgindx = trigdesc->tg_after_row[event];
	new_size = offsetof(DeferredTriggerEventData, dte_item[0]) +
		ntriggers * sizeof(DeferredTriggerEventItem);

	new_event = (DeferredTriggerEvent) palloc(new_size);
	new_event->dte_next = NULL;
	new_event->dte_event = event & TRIGGER_EVENT_OPMASK;
	new_event->dte_relid = rel->rd_id;
	ItemPointerCopy(&oldctid, &(new_event->dte_oldctid));
	ItemPointerCopy(&newctid, &(new_event->dte_newctid));
	new_event->dte_n_items = ntriggers;
	for (i = 0; i < ntriggers; i++)
	{
		Trigger    *trigger = &trigdesc->triggers[tgindx[i]];

		new_event->dte_item[i].dti_tgoid = trigger->tgoid;
		new_event->dte_item[i].dti_state =
			((trigger->tgdeferrable) ?
			 TRIGGER_DEFERRED_DEFERRABLE : 0) |
			((trigger->tginitdeferred) ?
			 TRIGGER_DEFERRED_INITDEFERRED : 0) |
			((trigdesc->n_before_row[event] > 0) ?
			 TRIGGER_DEFERRED_HAS_BEFORE : 0);
	}

	MemoryContextSwitchTo(oldcxt);

	switch (event & TRIGGER_EVENT_OPMASK)
	{
		case TRIGGER_EVENT_INSERT:
			/* nothing to do */
			break;

		case TRIGGER_EVENT_UPDATE:
			/*
			 * Check if one of the referenced keys is changed.
			 */
			for (i = 0; i < ntriggers; i++)
			{
				Trigger    *trigger = &trigdesc->triggers[tgindx[i]];
				bool		is_ri_trigger;
				bool		key_unchanged;

				/*
				 * We are interested in RI_FKEY triggers only.
				 */
				switch (trigger->tgfoid)
				{
					case F_RI_FKEY_NOACTION_UPD:
					case F_RI_FKEY_CASCADE_UPD:
					case F_RI_FKEY_RESTRICT_UPD:
					case F_RI_FKEY_SETNULL_UPD:
					case F_RI_FKEY_SETDEFAULT_UPD:
						is_ri_trigger = true;
						break;

					default:
						is_ri_trigger = false;
						break;
				}
				if (!is_ri_trigger)
					continue;

				LocTriggerData.type = T_TriggerData;
				LocTriggerData.tg_event = TRIGGER_EVENT_UPDATE;
				LocTriggerData.tg_relation = rel;
				LocTriggerData.tg_trigtuple = oldtup;
				LocTriggerData.tg_newtuple = newtup;
				LocTriggerData.tg_trigger = trigger;

				key_unchanged = RI_FKey_keyequal_upd(&LocTriggerData);

				if (key_unchanged)
				{
					/*
					 * The key hasn't changed, so no need later to invoke
					 * the trigger at all.
					 */
					new_event->dte_item[i].dti_state |= TRIGGER_DEFERRED_DONE;
				}
			}

			break;

		case TRIGGER_EVENT_DELETE:
			/* nothing to do */
			break;
	}

	/*
	 * Add the new event to the queue.
	 */
	deferredTriggerAddEvent(new_event);
}
