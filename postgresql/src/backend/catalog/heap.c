/*-------------------------------------------------------------------------
 *
 * heap.c
 *	  code to create and destroy POSTGRES heap relations
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/catalog/heap.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 *
 * INTERFACE ROUTINES
 *		heap_create()			- Create an uncataloged heap relation
 *		heap_create_with_catalog() - Create a cataloged relation
 *		heap_drop_with_catalog() - Removes named relation from catalogs
 *
 * NOTES
 *	  this code taken from access/heap/create.c, which contains
 *	  the old heap_create_with_catalog, amcreate, and amdestroy.
 *	  those routines will soon call these routines using the function
 *	  manager,
 *	  just like the poorly named "NewXXX" routines do.	The
 *	  "New" routines are all going to die soon, once and for all!
 *		-cim 1/13/91
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/heapam.h"
#include "access/genam.h"
#include "catalog/catalog.h"
#include "catalog/catname.h"
#include "catalog/heap.h"
#include "catalog/index.h"
#include "catalog/indexing.h"
#include "catalog/pg_attrdef.h"
#include "catalog/pg_inherits.h"
#include "catalog/pg_index.h"
#include "catalog/pg_relcheck.h"
#include "catalog/pg_statistic.h"
#include "catalog/pg_type.h"
#include "commands/comment.h"
#include "commands/trigger.h"
#include "miscadmin.h"
#include "optimizer/clauses.h"
#include "optimizer/planmain.h"
#include "optimizer/prep.h"
#include "optimizer/var.h"
#include "parser/parse_expr.h"
#include "parser/parse_relation.h"
#include "parser/parse_target.h"
#include "parser/parse_type.h"
#include "rewrite/rewriteRemove.h"
#include "storage/smgr.h"
#include "utils/builtins.h"
#include "utils/fmgroids.h"
#include "utils/lsyscache.h"
#include "utils/relcache.h"
#include "utils/syscache.h"
#include "utils/temprel.h"


static void AddNewRelationTuple(Relation pg_class_desc,
					Relation new_rel_desc,
					Oid new_rel_oid, Oid new_type_oid,
					char relkind, bool relhasoids,
					char *temp_relname);
static void DeleteAttributeTuples(Relation rel);
static void DeleteRelationTuple(Relation rel);
static void DeleteTypeTuple(Relation rel);
static void RelationRemoveIndexes(Relation relation);
static void RelationRemoveInheritance(Relation relation);
static void AddNewRelationType(char *typeName, Oid new_rel_oid,
				   Oid new_type_oid);
static void StoreAttrDefault(Relation rel, AttrNumber attnum, char *adbin,
				 bool updatePgAttribute);
static void StoreRelCheck(Relation rel, char *ccname, char *ccbin);
static void StoreConstraints(Relation rel);
static void RemoveConstraints(Relation rel);
static void RemoveStatistics(Relation rel);


/* ----------------------------------------------------------------
 *				XXX UGLY HARD CODED BADNESS FOLLOWS XXX
 *
 *		these should all be moved to someplace in the lib/catalog
 *		module, if not obliterated first.
 * ----------------------------------------------------------------
 */


/*
 * Note:
 *		Should the system special case these attributes in the future?
 *		Advantage:	consume much less space in the ATTRIBUTE relation.
 *		Disadvantage:  special cases will be all over the place.
 */

static FormData_pg_attribute a1 = {
	0, {"ctid"}, TIDOID, 0, sizeof(ItemPointerData),
	SelfItemPointerAttributeNumber, 0, -1, -1,
	false, 'p', false, 'i', false, false
};

static FormData_pg_attribute a2 = {
	0, {"oid"}, OIDOID, 0, sizeof(Oid),
	ObjectIdAttributeNumber, 0, -1, -1,
	true, 'p', false, 'i', false, false
};

static FormData_pg_attribute a3 = {
	0, {"xmin"}, XIDOID, 0, sizeof(TransactionId),
	MinTransactionIdAttributeNumber, 0, -1, -1,
	true, 'p', false, 'i', false, false
};

static FormData_pg_attribute a4 = {
	0, {"cmin"}, CIDOID, 0, sizeof(CommandId),
	MinCommandIdAttributeNumber, 0, -1, -1,
	true, 'p', false, 'i', false, false
};

static FormData_pg_attribute a5 = {
	0, {"xmax"}, XIDOID, 0, sizeof(TransactionId),
	MaxTransactionIdAttributeNumber, 0, -1, -1,
	true, 'p', false, 'i', false, false
};

static FormData_pg_attribute a6 = {
	0, {"cmax"}, CIDOID, 0, sizeof(CommandId),
	MaxCommandIdAttributeNumber, 0, -1, -1,
	true, 'p', false, 'i', false, false
};

/*
 * We decided to call this attribute "tableoid" rather than say
 * "classoid" on the basis that in the future there may be more than one
 * table of a particular class/type. In any case table is still the word
 * used in SQL.
 */
static FormData_pg_attribute a7 = {
	0, {"tableoid"}, OIDOID, 0, sizeof(Oid),
	TableOidAttributeNumber, 0, -1, -1,
	true, 'p', false, 'i', false, false
};

static Form_pg_attribute SysAtt[] = {&a1, &a2, &a3, &a4, &a5, &a6, &a7};

/*
 * This function returns a Form_pg_attribute pointer for a system attribute.
 * Note that we elog if the presented attno is invalid.
 */
Form_pg_attribute
SystemAttributeDefinition(AttrNumber attno, bool relhasoids)
{
	if (attno >= 0 || attno < -(int) lengthof(SysAtt))
		elog(ERROR, "SystemAttributeDefinition: invalid attribute number %d",
			 attno);
	if (attno == ObjectIdAttributeNumber && !relhasoids)
		elog(ERROR, "SystemAttributeDefinition: invalid attribute number %d",
			 attno);
	return SysAtt[-attno - 1];
}

/*
 * If the given name is a system attribute name, return a Form_pg_attribute
 * pointer for a prototype definition.	If not, return NULL.
 */
Form_pg_attribute
SystemAttributeByName(const char *attname, bool relhasoids)
{
	int			j;

	for (j = 0; j < (int) lengthof(SysAtt); j++)
	{
		Form_pg_attribute att = SysAtt[j];

		if (relhasoids || att->attnum != ObjectIdAttributeNumber)
		{
			if (strcmp(NameStr(att->attname), attname) == 0)
				return att;
		}
	}

	return NULL;
}


/* ----------------------------------------------------------------
 *				XXX END OF UGLY HARD CODED BADNESS XXX
 * ---------------------------------------------------------------- */


/* ----------------------------------------------------------------
 *		heap_create		- Create an uncataloged heap relation
 *
 *		rel->rd_rel is initialized by RelationBuildLocalRelation,
 *		and is mostly zeroes at return.
 *
 *		Remove the system relation specific code to elsewhere eventually.
 *
 *		Eventually, must place information about this temporary relation
 *		into the transaction context block.
 *
 * NOTE: if istemp is TRUE then heap_create will overwrite relname with
 * the unique "real" name chosen for the temp relation.
 *
 * If storage_create is TRUE then heap_storage_create is called here,
 * else caller must call heap_storage_create later.
 * ----------------------------------------------------------------
 */
Relation
heap_create(char *relname,
			TupleDesc tupDesc,
			bool istemp,
			bool storage_create,
			bool allow_system_table_mods)
{
	static unsigned int uniqueId = 0;

	Oid			relid;
	Oid			tblNode = MyDatabaseId;
	bool		nailme = false;
	Relation	rel;

	/*
	 * sanity checks
	 */
	if (relname && !allow_system_table_mods &&
		IsSystemRelationName(relname) && IsNormalProcessingMode())
		elog(ERROR, "invalid relation name \"%s\"; "
			 "the 'pg_' name prefix is reserved for system catalogs",
			 relname);

	/*
	 * Real ugly stuff to assign the proper relid in the relation
	 * descriptor follows.	Note that only "bootstrapped" relations whose
	 * OIDs are hard-coded in pg_class.h need be listed here.
	 */
	if (relname && IsSystemRelationName(relname))
	{
		if (strcmp(TypeRelationName, relname) == 0)
		{
			nailme = true;
			relid = RelOid_pg_type;
		}
		else if (strcmp(AttributeRelationName, relname) == 0)
		{
			nailme = true;
			relid = RelOid_pg_attribute;
		}
		else if (strcmp(ProcedureRelationName, relname) == 0)
		{
			nailme = true;
			relid = RelOid_pg_proc;
		}
		else if (strcmp(RelationRelationName, relname) == 0)
		{
			nailme = true;
			relid = RelOid_pg_class;
		}
		else if (strcmp(ShadowRelationName, relname) == 0)
		{
			tblNode = InvalidOid;
			relid = RelOid_pg_shadow;
		}
		else if (strcmp(GroupRelationName, relname) == 0)
		{
			tblNode = InvalidOid;
			relid = RelOid_pg_group;
		}
		else if (strcmp(DatabaseRelationName, relname) == 0)
		{
			tblNode = InvalidOid;
			relid = RelOid_pg_database;
		}
		else
		{
			relid = newoid();
			if (IsSharedSystemRelationName(relname))
				tblNode = InvalidOid;
		}
	}
	else
		relid = newoid();

	if (istemp)
	{
		/*
		 * replace relname of caller with a unique name for a temp
		 * relation
		 */
		snprintf(relname, NAMEDATALEN, "%s_%d_%u",
				 PG_TEMP_REL_PREFIX, (int) MyProcPid, uniqueId++);
	}

	/*
	 * build the relcache entry.
	 */
	rel = RelationBuildLocalRelation(relname, tupDesc,
									 relid, tblNode,
									 nailme);

	/*
	 * have the storage manager create the relation.
	 */
	if (storage_create)
		heap_storage_create(rel);

	return rel;
}

void
heap_storage_create(Relation rel)
{
	Assert(rel->rd_fd < 0);
	rel->rd_fd = smgrcreate(DEFAULT_SMGR, rel);
	Assert(rel->rd_fd >= 0);
}

/* ----------------------------------------------------------------
 *		heap_create_with_catalog		- Create a cataloged relation
 *
 *		this is done in 6 steps:
 *
 *		1) CheckAttributeNames() is used to make certain the tuple
 *		   descriptor contains a valid set of attribute names
 *
 *		2) pg_class is opened and RelationFindRelid()
 *		   performs a scan to ensure that no relation with the
 *		   same name already exists.
 *
 *		3) heap_create() is called to create the new relation on disk.
 *
 *		4) AddNewRelationTuple() is called to register the
 *		   relation in pg_class.
 *
 *		5) TypeCreate() is called to define a new type corresponding
 *		   to the new relation.
 *
 *		6) AddNewAttributeTuples() is called to register the
 *		   new relation's schema in pg_attribute.
 *
 *		7) StoreConstraints is called ()		- vadim 08/22/97
 *
 *		8) the relations are closed and the new relation's oid
 *		   is returned.
 *
 * ----------------------------------------------------------------
 */

/* --------------------------------
 *		CheckAttributeNames
 *
 *		this is used to make certain the tuple descriptor contains a
 *		valid set of attribute names.  a problem simply generates
 *		elog(ERROR) which aborts the current transaction.
 * --------------------------------
 */
static void
CheckAttributeNames(TupleDesc tupdesc, bool relhasoids)
{
	int			i;
	int			j;
	int			natts = tupdesc->natts;

	/*
	 * first check for collision with system attribute names
	 *
	 * also, warn user if attribute to be created has an unknown typid
	 * (usually as a result of a 'retrieve into' - jolly
	 */
	for (i = 0; i < natts; i++)
	{
		if (SystemAttributeByName(NameStr(tupdesc->attrs[i]->attname),
								  relhasoids) != NULL)
			elog(ERROR, "name of column \"%s\" conflicts with an existing system column",
				 NameStr(tupdesc->attrs[i]->attname));
		if (tupdesc->attrs[i]->atttypid == UNKNOWNOID)
			elog(NOTICE, "Attribute '%s' has an unknown type"
				 "\n\tProceeding with relation creation anyway",
				 NameStr(tupdesc->attrs[i]->attname));
	}

	/*
	 * next check for repeated attribute names
	 */
	for (i = 1; i < natts; i++)
	{
		for (j = 0; j < i; j++)
		{
			if (strcmp(NameStr(tupdesc->attrs[j]->attname),
					   NameStr(tupdesc->attrs[i]->attname)) == 0)
				elog(ERROR, "column name \"%s\" is duplicated",
					 NameStr(tupdesc->attrs[j]->attname));
		}
	}
}

/* --------------------------------
 *		RelnameFindRelid
 *
 *		Find any existing relation of the given name.
 * --------------------------------
 */
Oid
RelnameFindRelid(const char *relname)
{
	Oid			relid;

	/*
	 * If this is not bootstrap (initdb) time, use the catalog index on
	 * pg_class.
	 */
	if (!IsBootstrapProcessingMode())
	{
		relid = GetSysCacheOid(RELNAME,
							   PointerGetDatum(relname),
							   0, 0, 0);
	}
	else
	{
		Relation	pg_class_desc;
		ScanKeyData key;
		HeapScanDesc pg_class_scan;
		HeapTuple	tuple;

		pg_class_desc = heap_openr(RelationRelationName, AccessShareLock);

		/*
		 * At bootstrap time, we have to do this the hard way.	Form the
		 * scan key.
		 */
		ScanKeyEntryInitialize(&key,
							   0,
							   (AttrNumber) Anum_pg_class_relname,
							   (RegProcedure) F_NAMEEQ,
							   PointerGetDatum(relname));

		/*
		 * begin the scan
		 */
		pg_class_scan = heap_beginscan(pg_class_desc,
									   0,
									   SnapshotNow,
									   1,
									   &key);

		/*
		 * get a tuple.  if the tuple is NULL then it means we didn't find
		 * an existing relation.
		 */
		tuple = heap_getnext(pg_class_scan, 0);

		if (HeapTupleIsValid(tuple))
			relid = tuple->t_data->t_oid;
		else
			relid = InvalidOid;

		heap_endscan(pg_class_scan);

		heap_close(pg_class_desc, AccessShareLock);
	}
	return relid;
}

/* --------------------------------
 *		AddNewAttributeTuples
 *
 *		this registers the new relation's schema by adding
 *		tuples to pg_attribute.
 * --------------------------------
 */
static void
AddNewAttributeTuples(Oid new_rel_oid,
					  TupleDesc tupdesc,
					  bool relhasoids)
{
	Form_pg_attribute *dpp;
	int			i;
	HeapTuple	tup;
	Relation	rel;
	bool		hasindex;
	Relation	idescs[Num_pg_attr_indices];
	int			natts = tupdesc->natts;

	/*
	 * open pg_attribute
	 */
	rel = heap_openr(AttributeRelationName, RowExclusiveLock);

	/*
	 * Check if we have any indices defined on pg_attribute.
	 */
	hasindex = RelationGetForm(rel)->relhasindex;
	if (hasindex)
		CatalogOpenIndices(Num_pg_attr_indices, Name_pg_attr_indices, idescs);

	/*
	 * first we add the user attributes..
	 */
	dpp = tupdesc->attrs;
	for (i = 0; i < natts; i++)
	{
		/* Fill in the correct relation OID */
		(*dpp)->attrelid = new_rel_oid;
		/* Make sure these are OK, too */
		(*dpp)->attstattarget = DEFAULT_ATTSTATTARGET;
		(*dpp)->attcacheoff = -1;

		tup = heap_addheader(Natts_pg_attribute,
							 ATTRIBUTE_TUPLE_SIZE,
							 (void *) *dpp);

		heap_insert(rel, tup);

		if (hasindex)
			CatalogIndexInsert(idescs, Num_pg_attr_indices, rel, tup);

		heap_freetuple(tup);
		dpp++;
	}

	/*
	 * next we add the system attributes.  Skip OID if rel has no OIDs.
	 */
	dpp = SysAtt;
	for (i = 0; i < -1 - FirstLowInvalidHeapAttributeNumber; i++)
	{
		if (relhasoids || (*dpp)->attnum != ObjectIdAttributeNumber)
		{
			Form_pg_attribute attStruct;

			tup = heap_addheader(Natts_pg_attribute,
								 ATTRIBUTE_TUPLE_SIZE,
								 (void *) *dpp);

			/* Fill in the correct relation OID in the copied tuple */
			attStruct = (Form_pg_attribute) GETSTRUCT(tup);
			attStruct->attrelid = new_rel_oid;

			/*
			 * Unneeded since they should be OK in the constant data
			 * anyway
			 */
			/* attStruct->attstattarget = 0; */
			/* attStruct->attcacheoff = -1; */

			heap_insert(rel, tup);

			if (hasindex)
				CatalogIndexInsert(idescs, Num_pg_attr_indices, rel, tup);

			heap_freetuple(tup);
		}
		dpp++;
	}

	/*
	 * close pg_attribute indices
	 */
	if (hasindex)
		CatalogCloseIndices(Num_pg_attr_indices, idescs);

	heap_close(rel, RowExclusiveLock);
}

/* --------------------------------
 *		AddNewRelationTuple
 *
 *		this registers the new relation in the catalogs by
 *		adding a tuple to pg_class.
 * --------------------------------
 */
static void
AddNewRelationTuple(Relation pg_class_desc,
					Relation new_rel_desc,
					Oid new_rel_oid,
					Oid new_type_oid,
					char relkind,
					bool relhasoids,
					char *temp_relname)
{
	Form_pg_class new_rel_reltup;
	HeapTuple	tup;
	Relation	idescs[Num_pg_class_indices];

	/*
	 * first we update some of the information in our uncataloged
	 * relation's relation descriptor.
	 */
	new_rel_reltup = new_rel_desc->rd_rel;

	/*
	 * Here we insert bogus estimates of the size of the new relation. In
	 * reality, of course, the new relation has 0 tuples and pages, and if
	 * we were tracking these statistics accurately then we'd set the
	 * fields that way.  But at present the stats will be updated only by
	 * VACUUM or CREATE INDEX, and the user might insert a lot of tuples
	 * before he gets around to doing either of those.	So, instead of
	 * saying the relation is empty, we insert guesstimates.  The point is
	 * to keep the optimizer from making really stupid choices on
	 * never-yet-vacuumed tables; so the estimates need only be large
	 * enough to discourage the optimizer from using nested-loop plans.
	 * With this hack, nested-loop plans will be preferred only after the
	 * table has been proven to be small by VACUUM or CREATE INDEX.
	 * Maintaining the stats on-the-fly would solve the problem more
	 * cleanly, but the overhead of that would likely cost more than it'd
	 * save. (NOTE: CREATE INDEX inserts the same bogus estimates if it
	 * finds the relation has 0 rows and pages. See index.c.)
	 */
	switch (relkind)
	{
		case RELKIND_RELATION:
		case RELKIND_INDEX:
		case RELKIND_TOASTVALUE:
			new_rel_reltup->relpages = 10;		/* bogus estimates */
			new_rel_reltup->reltuples = 1000;
			break;
		case RELKIND_SEQUENCE:
			new_rel_reltup->relpages = 1;
			new_rel_reltup->reltuples = 1;
			break;
		default:				/* views, etc */
			new_rel_reltup->relpages = 0;
			new_rel_reltup->reltuples = 0;
			break;
	}

	new_rel_reltup->relowner = GetUserId();
	new_rel_reltup->reltype = new_type_oid;
	new_rel_reltup->relkind = relkind;
	new_rel_reltup->relhasoids = relhasoids;

	/* ----------------
	 *	now form a tuple to add to pg_class
	 *	XXX Natts_pg_class_fixed is a hack - see pg_class.h
	 * ----------------
	 */
	tup = heap_addheader(Natts_pg_class_fixed,
						 CLASS_TUPLE_SIZE,
						 (void *) new_rel_reltup);

	/* force tuple to have the desired OID */
	tup->t_data->t_oid = new_rel_oid;

	/*
	 * finally insert the new tuple and free it.
	 */
	heap_insert(pg_class_desc, tup);

	if (temp_relname)
		create_temp_relation(temp_relname, tup);

	if (!IsIgnoringSystemIndexes())
	{
		/*
		 * First, open the catalog indices and insert index tuples for the
		 * new relation.
		 */
		CatalogOpenIndices(Num_pg_class_indices, Name_pg_class_indices, idescs);
		CatalogIndexInsert(idescs, Num_pg_class_indices, pg_class_desc, tup);
		CatalogCloseIndices(Num_pg_class_indices, idescs);
	}

	heap_freetuple(tup);
}


/* --------------------------------
 *		AddNewRelationType -
 *
 *		define a complex type corresponding to the new relation
 * --------------------------------
 */
static void
AddNewRelationType(char *typeName, Oid new_rel_oid, Oid new_type_oid)
{
	/*
	 * The sizes are set to oid size because it makes implementing sets
	 * MUCH easier, and no one (we hope) uses these fields to figure out
	 * how much space to allocate for the type. An oid is the type used
	 * for a set definition.  When a user requests a set, what they
	 * actually get is the oid of a tuple in the pg_proc catalog, so the
	 * size of the "set" is the size of an oid. Similarly, byval being
	 * true makes sets much easier, and it isn't used by anything else.
	 */
	TypeCreate(typeName,		/* type name */
			   new_type_oid,	/* preassigned oid for type */
			   new_rel_oid,		/* relation oid */
			   sizeof(Oid),		/* internal size */
			   -1,				/* external size */
			   'c',				/* type-type (catalog) */
			   ',',				/* default array delimiter */
			   "oidin",			/* input procedure */
			   "oidout",		/* output procedure */
			   "oidin",			/* receive procedure */
			   "oidout",		/* send procedure */
			   NULL,			/* array element type - irrelevant */
			   NULL,			/* default type value - none */
			   true,			/* passed by value */
			   'i',				/* default alignment - same as for OID */
			   'p');			/* Not TOASTable */
}

/* --------------------------------
 *		heap_create_with_catalog
 *
 *		creates a new cataloged relation.  see comments above.
 * --------------------------------
 */
Oid
heap_create_with_catalog(char *relname,
						 TupleDesc tupdesc,
						 char relkind,
						 bool relhasoids,
						 bool istemp,
						 bool allow_system_table_mods)
{
	Relation	pg_class_desc;
	Relation	new_rel_desc;
	Oid			new_rel_oid;
	Oid			new_type_oid;
	char	   *temp_relname = NULL;

	/*
	 * sanity checks
	 */
	Assert(IsNormalProcessingMode() || IsBootstrapProcessingMode());
	if (tupdesc->natts <= 0 || tupdesc->natts > MaxHeapAttributeNumber)
		elog(ERROR, "Number of columns is out of range (1 to %d)",
			 MaxHeapAttributeNumber);

	CheckAttributeNames(tupdesc, relhasoids);

	/* temp tables can mask non-temp tables */
	if ((!istemp && RelnameFindRelid(relname)) ||
		(istemp && is_temp_rel_name(relname)))
		elog(ERROR, "Relation '%s' already exists", relname);

	if (istemp)
	{
		/* save user relation name because heap_create changes it */
		temp_relname = pstrdup(relname);		/* save original value */
		relname = palloc(NAMEDATALEN);
		strcpy(relname, temp_relname);	/* heap_create will change this */
	}

	/*
	 * Tell heap_create not to create a physical file; we'll do that below
	 * after all our catalog updates are done.	(This isn't really
	 * necessary anymore, but we may as well avoid the cycles of creating
	 * and deleting the file in case we fail.)
	 *
	 * Note: The call to heap_create() changes relname for temp tables; it
	 * becomes the true physical relname. The call to
	 * heap_storage_create() does all the "real" work of creating the disk
	 * file for the relation.
	 */
	new_rel_desc = heap_create(relname, tupdesc, istemp, false,
							   allow_system_table_mods);

	/* Fetch the relation OID assigned by heap_create */
	new_rel_oid = new_rel_desc->rd_att->attrs[0]->attrelid;

	/* Assign an OID for the relation's tuple type */
	new_type_oid = newoid();

	/*
	 * now create an entry in pg_class for the relation.
	 *
	 * NOTE: we could get a unique-index failure here, in case someone else
	 * is creating the same relation name in parallel but hadn't committed
	 * yet when we checked for a duplicate name above.
	 */
	pg_class_desc = heap_openr(RelationRelationName, RowExclusiveLock);

	AddNewRelationTuple(pg_class_desc,
						new_rel_desc,
						new_rel_oid,
						new_type_oid,
						relkind,
						relhasoids,
						temp_relname);

	/*
	 * since defining a relation also defines a complex type, we add a new
	 * system type corresponding to the new relation.
	 *
	 * NOTE: we could get a unique-index failure here, in case the same name
	 * has already been used for a type.
	 */
	AddNewRelationType(relname, new_rel_oid, new_type_oid);

	/*
	 * now add tuples to pg_attribute for the attributes in our new
	 * relation.
	 */
	AddNewAttributeTuples(new_rel_oid, tupdesc, relhasoids);

	StoreConstraints(new_rel_desc);

	/*
	 * We create the disk file for this relation here
	 */
	if (relkind != RELKIND_VIEW)
		heap_storage_create(new_rel_desc);

	/*
	 * ok, the relation has been cataloged, so close our relations and
	 * return the oid of the newly created relation.
	 */
	heap_close(new_rel_desc, NoLock);	/* do not unlock till end of xact */
	heap_close(pg_class_desc, RowExclusiveLock);

	if (istemp)
	{
		pfree(relname);
		pfree(temp_relname);
	}

	return new_rel_oid;
}


/* --------------------------------
 *		RelationRemoveInheritance
 *
 *		Note: for now, we cause an exception if relation is a
 *		superclass.  Someday, we may want to allow this and merge
 *		the type info into subclass procedures....	this seems like
 *		lots of work.
 * --------------------------------
 */
static void
RelationRemoveInheritance(Relation relation)
{
	Relation	catalogRelation;
	HeapTuple	tuple;
	HeapScanDesc scan;
	ScanKeyData entry;
	bool		found = false;

	/*
	 * open pg_inherits
	 */
	catalogRelation = heap_openr(InheritsRelationName, RowExclusiveLock);

	/*
	 * form a scan key for the subclasses of this class and begin scanning
	 */
	ScanKeyEntryInitialize(&entry, 0x0, Anum_pg_inherits_inhparent,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(relation)));

	scan = heap_beginscan(catalogRelation,
						  false,
						  SnapshotNow,
						  1,
						  &entry);

	/*
	 * if any subclasses exist, then we disallow the deletion.
	 */
	tuple = heap_getnext(scan, 0);
	if (HeapTupleIsValid(tuple))
	{
		Oid			subclass = ((Form_pg_inherits) GETSTRUCT(tuple))->inhrelid;
		char	   *subclassname;

		subclassname = get_rel_name(subclass);
		/* Just in case get_rel_name fails... */
		if (subclassname)
			elog(ERROR, "Relation \"%s\" inherits from \"%s\"",
				 subclassname, RelationGetRelationName(relation));
		else
			elog(ERROR, "Relation %u inherits from \"%s\"",
				 subclass, RelationGetRelationName(relation));
	}
	heap_endscan(scan);

	/*
	 * If we get here, it means the relation has no subclasses so we can
	 * trash it.  First we remove dead INHERITS tuples.
	 */
	entry.sk_attno = Anum_pg_inherits_inhrelid;

	scan = heap_beginscan(catalogRelation,
						  false,
						  SnapshotNow,
						  1,
						  &entry);

	while (HeapTupleIsValid(tuple = heap_getnext(scan, 0)))
	{
		simple_heap_delete(catalogRelation, &tuple->t_self);
		found = true;
	}

	heap_endscan(scan);
	heap_close(catalogRelation, RowExclusiveLock);
}

/* --------------------------------
 *		RelationRemoveIndexes
 *
 * --------------------------------
 */
static void
RelationRemoveIndexes(Relation relation)
{
	Relation	indexRelation;
	HeapTuple	tuple;
	HeapScanDesc scan;
	ScanKeyData entry;

	indexRelation = heap_openr(IndexRelationName, RowExclusiveLock);

	ScanKeyEntryInitialize(&entry, 0x0, Anum_pg_index_indrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(relation)));

	scan = heap_beginscan(indexRelation,
						  false,
						  SnapshotNow,
						  1,
						  &entry);

	while (HeapTupleIsValid(tuple = heap_getnext(scan, 0)))
	{
		index_drop(((Form_pg_index) GETSTRUCT(tuple))->indexrelid);
		/* advance cmd counter to make catalog changes visible */
		CommandCounterIncrement();
	}

	heap_endscan(scan);
	heap_close(indexRelation, RowExclusiveLock);
}

/* --------------------------------
 *		DeleteRelationTuple
 *
 * --------------------------------
 */
static void
DeleteRelationTuple(Relation rel)
{
	Relation	pg_class_desc;
	HeapTuple	tup;

	/*
	 * open pg_class
	 */
	pg_class_desc = heap_openr(RelationRelationName, RowExclusiveLock);

	tup = SearchSysCacheCopy(RELOID,
							 ObjectIdGetDatum(rel->rd_id),
							 0, 0, 0);
	if (!HeapTupleIsValid(tup))
		elog(ERROR, "Relation \"%s\" does not exist",
			 RelationGetRelationName(rel));

	/*
	 * delete the relation tuple from pg_class, and finish up.
	 */
	simple_heap_delete(pg_class_desc, &tup->t_self);
	heap_freetuple(tup);

	heap_close(pg_class_desc, RowExclusiveLock);
}

/* --------------------------------
 * RelationTruncateIndexes - This routine is used to truncate all
 * indices associated with the heap relation to zero tuples.
 * The routine will truncate and then reconstruct the indices on
 * the relation specified by the heapId parameter.
 * --------------------------------
 */
static void
RelationTruncateIndexes(Oid heapId)
{
	Relation	indexRelation;
	ScanKeyData entry;
	HeapScanDesc scan;
	HeapTuple	indexTuple;

	/* Scan pg_index to find indexes on specified heap */
	indexRelation = heap_openr(IndexRelationName, AccessShareLock);
	ScanKeyEntryInitialize(&entry, 0, Anum_pg_index_indrelid, F_OIDEQ,
						   ObjectIdGetDatum(heapId));
	scan = heap_beginscan(indexRelation, false, SnapshotNow, 1, &entry);

	while (HeapTupleIsValid(indexTuple = heap_getnext(scan, 0)))
	{
		Oid			indexId;
		IndexInfo  *indexInfo;
		Relation	heapRelation,
					currentIndex;

		/*
		 * For each index, fetch info needed for index_build
		 */
		indexId = ((Form_pg_index) GETSTRUCT(indexTuple))->indexrelid;
		indexInfo = BuildIndexInfo(indexTuple);

		/*
		 * We have to re-open the heap rel each time through this loop
		 * because index_build will close it again.  We need grab no lock,
		 * however, because we assume heap_truncate is holding an
		 * exclusive lock on the heap rel.
		 */
		heapRelation = heap_open(heapId, NoLock);

		/* Open the index relation */
		currentIndex = index_open(indexId);

		/* Obtain exclusive lock on it, just to be sure */
		LockRelation(currentIndex, AccessExclusiveLock);

		/*
		 * Drop any buffers associated with this index. If they're dirty,
		 * they're just dropped without bothering to flush to disk.
		 */
		DropRelationBuffers(currentIndex);

		/* Now truncate the actual data and set blocks to zero */
		smgrtruncate(DEFAULT_SMGR, currentIndex, 0);
		currentIndex->rd_nblocks = 0;
		currentIndex->rd_targblock = InvalidBlockNumber;

		/* Initialize the index and rebuild */
		index_build(heapRelation, currentIndex, indexInfo);

		/*
		 * index_build will close both the heap and index relations (but
		 * not give up the locks we hold on them).
		 */
	}

	/* Complete the scan and close pg_index */
	heap_endscan(scan);
	heap_close(indexRelation, AccessShareLock);
}

/* ----------------------------
 *	 heap_truncate
 *
 *	 This routine is used to truncate the data from the
 *	 storage manager of any data within the relation handed
 *	 to this routine.
 * ----------------------------
 */

void
heap_truncate(char *relname)
{
	Relation	rel;
	Oid			rid;

	/* Open relation for processing, and grab exclusive access on it. */

	rel = heap_openr(relname, AccessExclusiveLock);
	rid = RelationGetRelid(rel);

	/*
	 * TRUNCATE TABLE within a transaction block is dangerous, because if
	 * the transaction is later rolled back we have no way to undo
	 * truncation of the relation's physical file.  Disallow it except for
	 * a rel created in the current xact (which would be deleted on abort,
	 * anyway).
	 */
	if (IsTransactionBlock() && !rel->rd_myxactonly)
		elog(ERROR, "TRUNCATE TABLE cannot run inside a transaction block");

	/*
	 * Release any buffers associated with this relation.  If they're
	 * dirty, they're just dropped without bothering to flush to disk.
	 */
	DropRelationBuffers(rel);

	/* Now truncate the actual data and set blocks to zero */
	smgrtruncate(DEFAULT_SMGR, rel, 0);
	rel->rd_nblocks = 0;
	rel->rd_targblock = InvalidBlockNumber;

	/* If this relation has indexes, truncate the indexes too */
	RelationTruncateIndexes(rid);

	/*
	 * Close the relation, but keep exclusive lock on it until commit.
	 */
	heap_close(rel, NoLock);
}


/* --------------------------------
 *		DeleteAttributeTuples
 *
 * --------------------------------
 */
static void
DeleteAttributeTuples(Relation rel)
{
	Relation	pg_attribute_desc;
	HeapTuple	tup;
	int2		attnum;

	/*
	 * open pg_attribute
	 */
	pg_attribute_desc = heap_openr(AttributeRelationName, RowExclusiveLock);

	for (attnum = FirstLowInvalidHeapAttributeNumber + 1;
		 attnum <= rel->rd_att->natts;
		 attnum++)
	{
		tup = SearchSysCacheCopy(ATTNUM,
								 ObjectIdGetDatum(RelationGetRelid(rel)),
								 Int16GetDatum(attnum),
								 0, 0);
		if (HeapTupleIsValid(tup))
		{
			simple_heap_delete(pg_attribute_desc, &tup->t_self);
			heap_freetuple(tup);
		}
	}

	heap_close(pg_attribute_desc, RowExclusiveLock);
}

/* --------------------------------
 *		DeleteTypeTuple
 *
 *		If the user attempts to destroy a relation and there
 *		exists attributes in other relations of type
 *		"relation we are deleting", then we have to do something
 *		special.  presently we disallow the destroy.
 * --------------------------------
 */
static void
DeleteTypeTuple(Relation rel)
{
	Relation	pg_type_desc;
	HeapScanDesc pg_type_scan;
	Relation	pg_attribute_desc;
	HeapScanDesc pg_attribute_scan;
	ScanKeyData key;
	ScanKeyData attkey;
	HeapTuple	tup;
	HeapTuple	atttup;
	Oid			typoid;

	/*
	 * open pg_type
	 */
	pg_type_desc = heap_openr(TypeRelationName, RowExclusiveLock);

	/*
	 * create a scan key to locate the type tuple corresponding to this
	 * relation.
	 */
	ScanKeyEntryInitialize(&key, 0,
						   Anum_pg_type_typrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));

	pg_type_scan = heap_beginscan(pg_type_desc,
								  0,
								  SnapshotNow,
								  1,
								  &key);

	/*
	 * use heap_getnext() to fetch the pg_type tuple.  If this tuple is
	 * not valid then something's wrong.
	 */
	tup = heap_getnext(pg_type_scan, 0);

	if (!HeapTupleIsValid(tup))
	{
		heap_endscan(pg_type_scan);
		heap_close(pg_type_desc, RowExclusiveLock);
		elog(ERROR, "DeleteTypeTuple: type \"%s\" does not exist",
			 RelationGetRelationName(rel));
	}

	/*
	 * now scan pg_attribute.  if any other relations have attributes of
	 * the type of the relation we are deleteing then we have to disallow
	 * the deletion.  should talk to stonebraker about this.  -cim 6/19/90
	 */
	typoid = tup->t_data->t_oid;

	pg_attribute_desc = heap_openr(AttributeRelationName, RowExclusiveLock);

	ScanKeyEntryInitialize(&attkey,
						   0,
						   Anum_pg_attribute_atttypid,
						   F_OIDEQ,
						   ObjectIdGetDatum(typoid));

	pg_attribute_scan = heap_beginscan(pg_attribute_desc,
									   0,
									   SnapshotNow,
									   1,
									   &attkey);

	/*
	 * try and get a pg_attribute tuple.  if we succeed it means we can't
	 * delete the relation because something depends on the schema.
	 */
	atttup = heap_getnext(pg_attribute_scan, 0);

	if (HeapTupleIsValid(atttup))
	{
		Oid			relid = ((Form_pg_attribute) GETSTRUCT(atttup))->attrelid;

		heap_endscan(pg_attribute_scan);
		heap_close(pg_attribute_desc, RowExclusiveLock);
		heap_endscan(pg_type_scan);
		heap_close(pg_type_desc, RowExclusiveLock);

		elog(ERROR, "DeleteTypeTuple: column of type %s exists in relation %u",
			 RelationGetRelationName(rel), relid);
	}
	heap_endscan(pg_attribute_scan);
	heap_close(pg_attribute_desc, RowExclusiveLock);

	/*
	 * Ok, it's safe so we delete the relation tuple from pg_type and
	 * finish up.
	 */
	simple_heap_delete(pg_type_desc, &tup->t_self);

	heap_endscan(pg_type_scan);
	heap_close(pg_type_desc, RowExclusiveLock);
}

/* ----------------------------------------------------------------
 *		heap_drop_with_catalog	- removes all record of named relation from catalogs
 *
 *		1)	open relation, check for existence, etc.
 *		2)	remove inheritance information
 *		3)	remove indexes
 *		4)	remove pg_class tuple
 *		5)	remove pg_attribute tuples and related descriptions
 *		6)	remove pg_description tuples
 *		7)	remove pg_type tuples
 *		8)	RemoveConstraints ()
 *		9)	unlink relation
 *
 * old comments
 *		Except for vital relations, removes relation from
 *		relation catalog, and related attributes from
 *		attribute catalog (needed?).  (Anything else?)
 *
 *		get proper relation from relation catalog (if not arg)
 *		scan attribute catalog deleting attributes of reldesc
 *				(necessary?)
 *		delete relation from relation catalog
 *		(How are the tuples of the relation discarded?)
 *
 *		XXX Must fix to work with indexes.
 *		There may be a better order for doing things.
 *		Problems with destroying a deleted database--cannot create
 *		a struct reldesc without having an open file descriptor.
 * ----------------------------------------------------------------
 */
void
heap_drop_with_catalog(const char *relname,
					   bool allow_system_table_mods)
{
	Relation	rel;
	Oid			rid;
	bool		has_toasttable;
	bool		istemp = is_temp_rel_name(relname);
	int			i;

	/*
	 * Open and lock the relation.
	 */
	rel = heap_openr(relname, AccessExclusiveLock);
	rid = RelationGetRelid(rel);
	has_toasttable = rel->rd_rel->reltoastrelid != InvalidOid;

	/*
	 * prevent deletion of system relations
	 */
	/* allow temp of pg_class? Guess so. */
	if (!istemp &&
		!allow_system_table_mods &&
		IsSystemRelationName(RelationGetRelationName(rel)) &&
		!is_temp_relname(RelationGetRelationName(rel)))
		elog(ERROR, "System relation \"%s\" may not be dropped",
			 RelationGetRelationName(rel));

	/*
	 * Release all buffers that belong to this relation, after writing any
	 * that are dirty
	 */
	i = FlushRelationBuffers(rel, (BlockNumber) 0);
	if (i < 0)
		elog(ERROR, "heap_drop_with_catalog: FlushRelationBuffers returned %d",
			 i);

	/*
	 * remove rules if necessary
	 */
	if (rel->rd_rules != NULL)
		RelationRemoveRules(rid);

	/* triggers */
	RelationRemoveTriggers(rel);

	/*
	 * remove inheritance information
	 */
	RelationRemoveInheritance(rel);

	/*
	 * remove indexes if necessary
	 */
	RelationRemoveIndexes(rel);

	/*
	 * delete attribute tuples
	 */
	DeleteAttributeTuples(rel);

	/*
	 * delete comments, statistics, and constraints
	 */
	DeleteComments(rid, RelOid_pg_class);

	RemoveStatistics(rel);

	RemoveConstraints(rel);

	/*
	 * delete type tuple
	 */
	DeleteTypeTuple(rel);

	/*
	 * delete relation tuple
	 */
	DeleteRelationTuple(rel);

	/*
	 * unlink the relation's physical file and finish up.
	 */
	if (rel->rd_rel->relkind != RELKIND_VIEW)
		smgrunlink(DEFAULT_SMGR, rel);

	/*
	 * Close relcache entry, but *keep* AccessExclusiveLock on the
	 * relation until transaction commit.  This ensures no one else will
	 * try to do something with the doomed relation.
	 */
	heap_close(rel, NoLock);

	/*
	 * flush the relation from the relcache
	 */
	RelationForgetRelation(rid);

	/* and from the temp-table map */
	if (istemp)
		remove_temp_rel_by_relid(rid);

	if (has_toasttable)
	{
		char		toast_relname[NAMEDATALEN];

		sprintf(toast_relname, "pg_toast_%u", rid);
		heap_drop_with_catalog(toast_relname, true);
	}
}


/*
 * Store a default expression for column attnum of relation rel.
 * The expression must be presented as a nodeToString() string.
 * If updatePgAttribute is true, update the pg_attribute entry
 * for the column to show that a default exists.
 */
static void
StoreAttrDefault(Relation rel, AttrNumber attnum, char *adbin,
				 bool updatePgAttribute)
{
	Node	   *expr;
	char	   *adsrc;
	Relation	adrel;
	Relation	idescs[Num_pg_attrdef_indices];
	HeapTuple	tuple;
	Datum		values[4];
	static char nulls[4] = {' ', ' ', ' ', ' '};
	Relation	attrrel;
	Relation	attridescs[Num_pg_attr_indices];
	HeapTuple	atttup;
	Form_pg_attribute attStruct;

	/*
	 * Need to construct source equivalent of given node-string.
	 */
	expr = stringToNode(adbin);

	/*
	 * deparse it
	 */
	adsrc = deparse_expression(expr,
						deparse_context_for(RelationGetRelationName(rel),
											RelationGetRelid(rel)),
							   false);

	values[Anum_pg_attrdef_adrelid - 1] = RelationGetRelid(rel);
	values[Anum_pg_attrdef_adnum - 1] = attnum;
	values[Anum_pg_attrdef_adbin - 1] = DirectFunctionCall1(textin,
												 CStringGetDatum(adbin));
	values[Anum_pg_attrdef_adsrc - 1] = DirectFunctionCall1(textin,
												 CStringGetDatum(adsrc));
	adrel = heap_openr(AttrDefaultRelationName, RowExclusiveLock);
	tuple = heap_formtuple(adrel->rd_att, values, nulls);
	heap_insert(adrel, tuple);
	CatalogOpenIndices(Num_pg_attrdef_indices, Name_pg_attrdef_indices,
					   idescs);
	CatalogIndexInsert(idescs, Num_pg_attrdef_indices, adrel, tuple);
	CatalogCloseIndices(Num_pg_attrdef_indices, idescs);
	heap_close(adrel, RowExclusiveLock);

	pfree(DatumGetPointer(values[Anum_pg_attrdef_adbin - 1]));
	pfree(DatumGetPointer(values[Anum_pg_attrdef_adsrc - 1]));
	heap_freetuple(tuple);
	pfree(adsrc);

	if (!updatePgAttribute)
		return;					/* done if pg_attribute is OK */

	attrrel = heap_openr(AttributeRelationName, RowExclusiveLock);
	atttup = SearchSysCacheCopy(ATTNUM,
								ObjectIdGetDatum(RelationGetRelid(rel)),
								Int16GetDatum(attnum),
								0, 0);
	if (!HeapTupleIsValid(atttup))
		elog(ERROR, "cache lookup of attribute %d in relation %u failed",
			 attnum, RelationGetRelid(rel));
	attStruct = (Form_pg_attribute) GETSTRUCT(atttup);
	if (!attStruct->atthasdef)
	{
		attStruct->atthasdef = true;
		simple_heap_update(attrrel, &atttup->t_self, atttup);
		/* keep catalog indices current */
		CatalogOpenIndices(Num_pg_attr_indices, Name_pg_attr_indices,
						   attridescs);
		CatalogIndexInsert(attridescs, Num_pg_attr_indices, attrrel, atttup);
		CatalogCloseIndices(Num_pg_attr_indices, attridescs);
	}
	heap_close(attrrel, RowExclusiveLock);
	heap_freetuple(atttup);
}

/*
 * Store a constraint expression for the given relation.
 * The expression must be presented as a nodeToString() string.
 *
 * Caller is responsible for updating the count of constraints
 * in the pg_class entry for the relation.
 */
static void
StoreRelCheck(Relation rel, char *ccname, char *ccbin)
{
	Node	   *expr;
	char	   *ccsrc;
	Relation	rcrel;
	Relation	idescs[Num_pg_relcheck_indices];
	HeapTuple	tuple;
	Datum		values[4];
	static char nulls[4] = {' ', ' ', ' ', ' '};

	/*
	 * Convert condition to a normal boolean expression tree.
	 */
	expr = stringToNode(ccbin);
	expr = (Node *) make_ands_explicit((List *) expr);

	/*
	 * deparse it
	 */
	ccsrc = deparse_expression(expr,
						deparse_context_for(RelationGetRelationName(rel),
											RelationGetRelid(rel)),
							   false);

	values[Anum_pg_relcheck_rcrelid - 1] = RelationGetRelid(rel);
	values[Anum_pg_relcheck_rcname - 1] = DirectFunctionCall1(namein,
												CStringGetDatum(ccname));
	values[Anum_pg_relcheck_rcbin - 1] = DirectFunctionCall1(textin,
												 CStringGetDatum(ccbin));
	values[Anum_pg_relcheck_rcsrc - 1] = DirectFunctionCall1(textin,
												 CStringGetDatum(ccsrc));
	rcrel = heap_openr(RelCheckRelationName, RowExclusiveLock);
	tuple = heap_formtuple(rcrel->rd_att, values, nulls);
	heap_insert(rcrel, tuple);
	CatalogOpenIndices(Num_pg_relcheck_indices, Name_pg_relcheck_indices,
					   idescs);
	CatalogIndexInsert(idescs, Num_pg_relcheck_indices, rcrel, tuple);
	CatalogCloseIndices(Num_pg_relcheck_indices, idescs);
	heap_close(rcrel, RowExclusiveLock);

	pfree(DatumGetPointer(values[Anum_pg_relcheck_rcname - 1]));
	pfree(DatumGetPointer(values[Anum_pg_relcheck_rcbin - 1]));
	pfree(DatumGetPointer(values[Anum_pg_relcheck_rcsrc - 1]));
	heap_freetuple(tuple);
	pfree(ccsrc);
}

/*
 * Store defaults and constraints passed in via the tuple constraint struct.
 *
 * NOTE: only pre-cooked expressions will be passed this way, which is to
 * say expressions inherited from an existing relation.  Newly parsed
 * expressions can be added later, by direct calls to StoreAttrDefault
 * and StoreRelCheck (see AddRelationRawConstraints()).  We assume that
 * pg_attribute and pg_class entries for the relation were already set
 * to reflect the existence of these defaults/constraints.
 */
static void
StoreConstraints(Relation rel)
{
	TupleConstr *constr = rel->rd_att->constr;
	int			i;

	if (!constr)
		return;

	/*
	 * deparsing of constraint expressions will fail unless the
	 * just-created pg_attribute tuples for this relation are made
	 * visible.  So, bump the command counter.
	 */
	CommandCounterIncrement();

	for (i = 0; i < constr->num_defval; i++)
		StoreAttrDefault(rel, constr->defval[i].adnum,
						 constr->defval[i].adbin, false);

	for (i = 0; i < constr->num_check; i++)
		StoreRelCheck(rel, constr->check[i].ccname,
					  constr->check[i].ccbin);
}

/*
 * AddRelationRawConstraints
 *
 * Add raw (not-yet-transformed) column default expressions and/or constraint
 * check expressions to an existing relation.  This is defined to do both
 * for efficiency in DefineRelation, but of course you can do just one or
 * the other by passing empty lists.
 *
 * rel: relation to be modified
 * rawColDefaults: list of RawColumnDefault structures
 * rawConstraints: list of Constraint nodes
 *
 * All entries in rawColDefaults will be processed.  Entries in rawConstraints
 * will be processed only if they are CONSTR_CHECK type and contain a "raw"
 * expression.
 *
 * NB: caller should have opened rel with AccessExclusiveLock, and should
 * hold that lock till end of transaction.	Also, we assume the caller has
 * done a CommandCounterIncrement if necessary to make the relation's catalog
 * tuples visible.
 */
void
AddRelationRawConstraints(Relation rel,
						  List *rawColDefaults,
						  List *rawConstraints)
{
	char	   *relname = RelationGetRelationName(rel);
	TupleDesc	tupleDesc;
	TupleConstr *oldconstr;
	int			numoldchecks;
	ConstrCheck *oldchecks;
	ParseState *pstate;
	RangeTblEntry *rte;
	int			numchecks;
	List	   *listptr;
	Relation	relrel;
	Relation	relidescs[Num_pg_class_indices];
	HeapTuple	reltup;
	Form_pg_class relStruct;

	/*
	 * Get info about existing constraints.
	 */
	tupleDesc = RelationGetDescr(rel);
	oldconstr = tupleDesc->constr;
	if (oldconstr)
	{
		numoldchecks = oldconstr->num_check;
		oldchecks = oldconstr->check;
	}
	else
	{
		numoldchecks = 0;
		oldchecks = NULL;
	}

	/*
	 * Create a dummy ParseState and insert the target relation as its
	 * sole rangetable entry.  We need a ParseState for transformExpr.
	 */
	pstate = make_parsestate(NULL);
	rte = addRangeTableEntry(pstate, relname, NULL, false, true);
	addRTEtoQuery(pstate, rte, true, true);

	/*
	 * Process column default expressions.
	 */
	foreach(listptr, rawColDefaults)
	{
		RawColumnDefault *colDef = (RawColumnDefault *) lfirst(listptr);
		Node	   *expr;
		Oid			type_id;

		Assert(colDef->raw_default != NULL);

		/*
		 * Transform raw parsetree to executable expression.
		 */
		expr = transformExpr(pstate, colDef->raw_default, EXPR_COLUMN_FIRST);

		/*
		 * Make sure default expr does not refer to any vars.
		 */
		if (contain_var_clause(expr))
			elog(ERROR, "cannot use column references in DEFAULT clause");

		/*
		 * No subplans or aggregates, either...
		 */
		if (contain_subplans(expr))
			elog(ERROR, "cannot use subselects in DEFAULT clause");
		if (contain_agg_clause(expr))
			elog(ERROR, "cannot use aggregate functions in DEFAULT clause");

		/*
		 * Check that it will be possible to coerce the expression to the
		 * column's type.  We store the expression without coercion,
		 * however, to avoid premature coercion in cases like
		 *
		 * CREATE TABLE tbl (fld datetime DEFAULT 'now'::text);
		 *
		 * NB: this should match the code in optimizer/prep/preptlist.c that
		 * will actually do the coercion, to ensure we don't accept an
		 * unusable default expression.
		 */
		type_id = exprType(expr);
		if (type_id != InvalidOid)
		{
			Form_pg_attribute atp = rel->rd_att->attrs[colDef->attnum - 1];

			if (type_id != atp->atttypid)
			{
				if (CoerceTargetExpr(NULL, expr, type_id,
								  atp->atttypid, atp->atttypmod) == NULL)
					elog(ERROR, "Column \"%s\" is of type %s"
						 " but default expression is of type %s"
					"\n\tYou will need to rewrite or cast the expression",
						 NameStr(atp->attname),
						 format_type_be(atp->atttypid),
						 format_type_be(type_id));
			}
		}

		/*
		 * Might as well try to reduce any constant expressions.
		 */
		expr = eval_const_expressions(expr);

		/*
		 * Must fix opids, in case any operators remain...
		 */
		fix_opids(expr);

		/*
		 * OK, store it.
		 */
		StoreAttrDefault(rel, colDef->attnum, nodeToString(expr), true);
	}

	/*
	 * Process constraint expressions.
	 */
	numchecks = numoldchecks;
	foreach(listptr, rawConstraints)
	{
		Constraint *cdef = (Constraint *) lfirst(listptr);
		char	   *ccname;
		Node	   *expr;

		if (cdef->contype != CONSTR_CHECK || cdef->raw_expr == NULL)
			continue;
		Assert(cdef->cooked_expr == NULL);

		/* Check name uniqueness, or generate a new name */
		if (cdef->name != NULL)
		{
			int			i;
			List	   *listptr2;

			ccname = cdef->name;
			/* Check against old constraints */
			for (i = 0; i < numoldchecks; i++)
			{
				if (strcmp(oldchecks[i].ccname, ccname) == 0)
					elog(ERROR, "Duplicate CHECK constraint name: '%s'",
						 ccname);
			}
			/* Check against other new constraints */
			foreach(listptr2, rawConstraints)
			{
				Constraint *cdef2 = (Constraint *) lfirst(listptr2);

				if (cdef2 == cdef ||
					cdef2->contype != CONSTR_CHECK ||
					cdef2->raw_expr == NULL ||
					cdef2->name == NULL)
					continue;
				if (strcmp(cdef2->name, ccname) == 0)
					elog(ERROR, "Duplicate CHECK constraint name: '%s'",
						 ccname);
			}
		}
		else
		{
			int			i;
			int			j;
			bool		success;
			List	   *listptr2;

			ccname = (char *) palloc(NAMEDATALEN);

			/* Loop until we find a non-conflicting constraint name */
			/* What happens if this loops forever? */
			j = numchecks + 1;
			do
			{
				success = true;
				snprintf(ccname, NAMEDATALEN, "$%d", j);

				/* Check against old constraints */
				for (i = 0; i < numoldchecks; i++)
				{
					if (strcmp(oldchecks[i].ccname, ccname) == 0)
					{
						success = false;
						break;
					}
				}

				/*
				 * Check against other new constraints, if the check
				 * hasn't already failed
				 */
				if (success)
				{
					foreach(listptr2, rawConstraints)
					{
						Constraint *cdef2 = (Constraint *) lfirst(listptr2);

						if (cdef2 == cdef ||
							cdef2->contype != CONSTR_CHECK ||
							cdef2->raw_expr == NULL ||
							cdef2->name == NULL)
							continue;
						if (strcmp(cdef2->name, ccname) == 0)
						{
							success = false;
							break;
						}
					}
				}

				++j;
			} while (!success);
		}

		/*
		 * Transform raw parsetree to executable expression.
		 */
		expr = transformExpr(pstate, cdef->raw_expr, EXPR_COLUMN_FIRST);

		/*
		 * Make sure it yields a boolean result.
		 */
		if (exprType(expr) != BOOLOID)
			elog(ERROR, "CHECK constraint expression '%s' does not yield boolean result",
				 ccname);

		/*
		 * Make sure no outside relations are referred to.
		 */
		if (length(pstate->p_rtable) != 1)
			elog(ERROR, "Only relation \"%s\" can be referenced in CHECK constraint expression",
				 relname);

		/*
		 * No subplans or aggregates, either...
		 */
		if (contain_subplans(expr))
			elog(ERROR, "cannot use subselect in CHECK constraint expression");
		if (contain_agg_clause(expr))
			elog(ERROR, "cannot use aggregate function in CHECK constraint expression");

		/*
		 * Might as well try to reduce any constant expressions.
		 */
		expr = eval_const_expressions(expr);

		/*
		 * Constraints are evaluated with execQual, which expects an
		 * implicit-AND list, so convert expression to implicit-AND form.
		 * (We could go so far as to convert to CNF, but that's probably
		 * overkill...)
		 */
		expr = (Node *) make_ands_implicit((Expr *) expr);

		/*
		 * Must fix opids in operator clauses.
		 */
		fix_opids(expr);

		/*
		 * OK, store it.
		 */
		StoreRelCheck(rel, ccname, nodeToString(expr));

		numchecks++;
	}

	/*
	 * Update the count of constraints in the relation's pg_class tuple.
	 * We do this even if there was no change, in order to ensure that an
	 * SI update message is sent out for the pg_class tuple, which will
	 * force other backends to rebuild their relcache entries for the rel.
	 * (Of course, for a newly created rel there is no need for an SI
	 * message, but for ALTER TABLE ADD ATTRIBUTE this'd be important.)
	 */
	relrel = heap_openr(RelationRelationName, RowExclusiveLock);
	reltup = SearchSysCacheCopy(RELOID,
								ObjectIdGetDatum(RelationGetRelid(rel)),
								0, 0, 0);
	if (!HeapTupleIsValid(reltup))
		elog(ERROR, "cache lookup of relation %u failed",
			 RelationGetRelid(rel));
	relStruct = (Form_pg_class) GETSTRUCT(reltup);

	relStruct->relchecks = numchecks;

	simple_heap_update(relrel, &reltup->t_self, reltup);

	/* keep catalog indices current */
	CatalogOpenIndices(Num_pg_class_indices, Name_pg_class_indices,
					   relidescs);
	CatalogIndexInsert(relidescs, Num_pg_class_indices, relrel, reltup);
	CatalogCloseIndices(Num_pg_class_indices, relidescs);

	heap_freetuple(reltup);
	heap_close(relrel, RowExclusiveLock);
}

static void
RemoveAttrDefault(Relation rel)
{
	Relation	adrel;
	HeapScanDesc adscan;
	ScanKeyData key;
	HeapTuple	tup;

	adrel = heap_openr(AttrDefaultRelationName, RowExclusiveLock);

	ScanKeyEntryInitialize(&key, 0, Anum_pg_attrdef_adrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));

	adscan = heap_beginscan(adrel, 0, SnapshotNow, 1, &key);

	while (HeapTupleIsValid(tup = heap_getnext(adscan, 0)))
		simple_heap_delete(adrel, &tup->t_self);

	heap_endscan(adscan);
	heap_close(adrel, RowExclusiveLock);
}

static void
RemoveRelCheck(Relation rel)
{
	Relation	rcrel;
	HeapScanDesc rcscan;
	ScanKeyData key;
	HeapTuple	tup;

	rcrel = heap_openr(RelCheckRelationName, RowExclusiveLock);

	ScanKeyEntryInitialize(&key, 0, Anum_pg_relcheck_rcrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));

	rcscan = heap_beginscan(rcrel, 0, SnapshotNow, 1, &key);

	while (HeapTupleIsValid(tup = heap_getnext(rcscan, 0)))
		simple_heap_delete(rcrel, &tup->t_self);

	heap_endscan(rcscan);
	heap_close(rcrel, RowExclusiveLock);

}

/*
 * Removes all CHECK constraints on a relation that match the given name.
 * It is the responsibility of the calling function to acquire a lock on
 * the relation.
 * Returns: The number of CHECK constraints removed.
 */
int
RemoveCheckConstraint(Relation rel, const char *constrName, bool inh)
{
	Oid			relid;
	Relation	rcrel;
	Relation	relrel;
	Relation	inhrel;
	Relation	relidescs[Num_pg_class_indices];
	TupleDesc	tupleDesc;
	TupleConstr *oldconstr;
	int			numoldchecks;
	int			numchecks;
	HeapScanDesc rcscan;
	ScanKeyData key[2];
	HeapTuple	rctup;
	HeapTuple	reltup;
	Form_pg_class relStruct;
	int			rel_deleted = 0;
	int			all_deleted = 0;

	/* Find id of the relation */
	relid = RelationGetRelid(rel);

	/*
	 * Process child tables and remove constraints of the same name.
	 */
	if (inh)
	{
		List	   *child,
				   *children;

		/* This routine is actually in the planner */
		children = find_all_inheritors(relid);

		/*
		 * find_all_inheritors does the recursive search of the
		 * inheritance hierarchy, so all we have to do is process all of
		 * the relids in the list that it returns.
		 */
		foreach(child, children)
		{
			Oid			childrelid = lfirsti(child);

			if (childrelid == relid)
				continue;
			inhrel = heap_open(childrelid, AccessExclusiveLock);
			all_deleted += RemoveCheckConstraint(inhrel, constrName, false);
			heap_close(inhrel, NoLock);
		}
	}

	/* Grab an exclusive lock on the pg_relcheck relation */
	rcrel = heap_openr(RelCheckRelationName, RowExclusiveLock);

	/*
	 * Create two scan keys.  We need to match on the oid of the table the
	 * CHECK is in and also we need to match the name of the CHECK
	 * constraint.
	 */
	ScanKeyEntryInitialize(&key[0], 0, Anum_pg_relcheck_rcrelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));

	ScanKeyEntryInitialize(&key[1], 0, Anum_pg_relcheck_rcname,
						   F_NAMEEQ,
						   PointerGetDatum(constrName));

	/* Begin scanning the heap */
	rcscan = heap_beginscan(rcrel, 0, SnapshotNow, 2, key);

	/*
	 * Scan over the result set, removing any matching entries.  Note that
	 * this has the side-effect of removing ALL CHECK constraints that
	 * share the specified constraint name.
	 */
	while (HeapTupleIsValid(rctup = heap_getnext(rcscan, 0)))
	{
		simple_heap_delete(rcrel, &rctup->t_self);
		++rel_deleted;
		++all_deleted;
	}

	/* Clean up after the scan */
	heap_endscan(rcscan);

	/*
	 * Update the count of constraints in the relation's pg_class tuple.
	 * We do this even if there was no change, in order to ensure that an
	 * SI update message is sent out for the pg_class tuple, which will
	 * force other backends to rebuild their relcache entries for the rel.
	 * (Of course, for a newly created rel there is no need for an SI
	 * message, but for ALTER TABLE ADD ATTRIBUTE this'd be important.)
	 */

	/*
	 * Get number of existing constraints.
	 */

	tupleDesc = RelationGetDescr(rel);
	oldconstr = tupleDesc->constr;
	if (oldconstr)
		numoldchecks = oldconstr->num_check;
	else
		numoldchecks = 0;

	/* Calculate the new number of checks in the table, fail if negative */
	numchecks = numoldchecks - rel_deleted;

	if (numchecks < 0)
		elog(ERROR, "check count became negative");

	relrel = heap_openr(RelationRelationName, RowExclusiveLock);
	reltup = SearchSysCacheCopy(RELOID,
					   ObjectIdGetDatum(RelationGetRelid(rel)), 0, 0, 0);

	if (!HeapTupleIsValid(reltup))
		elog(ERROR, "cache lookup of relation %u failed",
			 RelationGetRelid(rel));
	relStruct = (Form_pg_class) GETSTRUCT(reltup);

	relStruct->relchecks = numchecks;

	simple_heap_update(relrel, &reltup->t_self, reltup);

	/* Keep catalog indices current */
	CatalogOpenIndices(Num_pg_class_indices, Name_pg_class_indices,
					   relidescs);
	CatalogIndexInsert(relidescs, Num_pg_class_indices, relrel, reltup);
	CatalogCloseIndices(Num_pg_class_indices, relidescs);

	/* Clean up after the scan */
	heap_freetuple(reltup);
	heap_close(relrel, RowExclusiveLock);

	/* Close the heap relation */
	heap_close(rcrel, RowExclusiveLock);

	/* Return the number of tuples deleted */
	return all_deleted;
}

static void
RemoveConstraints(Relation rel)
{
	TupleConstr *constr = rel->rd_att->constr;

	if (!constr)
		return;

	if (constr->num_defval > 0)
		RemoveAttrDefault(rel);

	if (constr->num_check > 0)
		RemoveRelCheck(rel);
}

static void
RemoveStatistics(Relation rel)
{
	Relation	pgstatistic;
	HeapScanDesc scan;
	ScanKeyData key;
	HeapTuple	tuple;

	pgstatistic = heap_openr(StatisticRelationName, RowExclusiveLock);

	ScanKeyEntryInitialize(&key, 0x0, Anum_pg_statistic_starelid,
						   F_OIDEQ,
						   ObjectIdGetDatum(RelationGetRelid(rel)));
	scan = heap_beginscan(pgstatistic, false, SnapshotNow, 1, &key);

	while (HeapTupleIsValid(tuple = heap_getnext(scan, 0)))
		simple_heap_delete(pgstatistic, &tuple->t_self);

	heap_endscan(scan);
	heap_close(pgstatistic, RowExclusiveLock);
}
