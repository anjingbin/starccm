/*-------------------------------------------------------------------------
 *
 * tupdesc.c
 *	  POSTGRES tuple descriptor support code
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/access/common/tupdesc.c,v 1.1 2003/12/30 00:05:09 AnJingBin Exp $
 *
 * NOTES
 *	  some of the executor utility code such as "ExecTypeFromTL" should be
 *	  moved here.
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "catalog/pg_type.h"
#include "nodes/parsenodes.h"
#include "parser/parse_type.h"
#include "utils/builtins.h"
#include "utils/syscache.h"


/* ----------------------------------------------------------------
 *		CreateTemplateTupleDesc
 *
 *		This function allocates and zeros a tuple descriptor structure.
 * ----------------------------------------------------------------
 */
TupleDesc
CreateTemplateTupleDesc(int natts)
{
	uint32		size;
	TupleDesc	desc;

	/*
	 * sanity checks
	 */
	AssertArg(natts >= 1);

	/*
	 * allocate enough memory for the tuple descriptor and zero it as
	 * TupleDescInitEntry assumes that the descriptor is filled with NULL
	 * pointers.
	 */
	size = natts * sizeof(Form_pg_attribute);
	desc = (TupleDesc) palloc(sizeof(struct tupleDesc));
	desc->attrs = (Form_pg_attribute *) palloc(size);
	desc->constr = NULL;
	MemSet(desc->attrs, 0, size);

	desc->natts = natts;

	return desc;
}

/* ----------------------------------------------------------------
 *		CreateTupleDesc
 *
 *		This function allocates a new TupleDesc from Form_pg_attribute array
 * ----------------------------------------------------------------
 */
TupleDesc
CreateTupleDesc(int natts, Form_pg_attribute *attrs)
{
	TupleDesc	desc;

	/*
	 * sanity checks
	 */
	AssertArg(natts >= 1);

	desc = (TupleDesc) palloc(sizeof(struct tupleDesc));
	desc->attrs = attrs;
	desc->natts = natts;
	desc->constr = NULL;

	return desc;
}

/* ----------------------------------------------------------------
 *		CreateTupleDescCopy
 *
 *		This function creates a new TupleDesc by copying from an existing
 *		TupleDesc
 *
 *		!!! Constraints are not copied !!!
 * ----------------------------------------------------------------
 */
TupleDesc
CreateTupleDescCopy(TupleDesc tupdesc)
{
	TupleDesc	desc;
	int			i,
				size;

	desc = (TupleDesc) palloc(sizeof(struct tupleDesc));
	desc->natts = tupdesc->natts;
	size = desc->natts * sizeof(Form_pg_attribute);
	desc->attrs = (Form_pg_attribute *) palloc(size);
	for (i = 0; i < desc->natts; i++)
	{
		desc->attrs[i] = (Form_pg_attribute) palloc(ATTRIBUTE_TUPLE_SIZE);
		memmove(desc->attrs[i],
				tupdesc->attrs[i],
				ATTRIBUTE_TUPLE_SIZE);
		desc->attrs[i]->attnotnull = false;
		desc->attrs[i]->atthasdef = false;
	}
	desc->constr = NULL;

	return desc;
}

/* ----------------------------------------------------------------
 *		CreateTupleDescCopyConstr
 *
 *		This function creates a new TupleDesc by copying from an existing
 *		TupleDesc (with Constraints)
 *
 * ----------------------------------------------------------------
 */
TupleDesc
CreateTupleDescCopyConstr(TupleDesc tupdesc)
{
	TupleDesc	desc;
	TupleConstr *constr = tupdesc->constr;
	int			i,
				size;

	desc = (TupleDesc) palloc(sizeof(struct tupleDesc));
	desc->natts = tupdesc->natts;
	size = desc->natts * sizeof(Form_pg_attribute);
	desc->attrs = (Form_pg_attribute *) palloc(size);
	for (i = 0; i < desc->natts; i++)
	{
		desc->attrs[i] = (Form_pg_attribute) palloc(ATTRIBUTE_TUPLE_SIZE);
		memmove(desc->attrs[i],
				tupdesc->attrs[i],
				ATTRIBUTE_TUPLE_SIZE);
	}
	if (constr)
	{
		TupleConstr *cpy = (TupleConstr *) palloc(sizeof(TupleConstr));

		cpy->has_not_null = constr->has_not_null;

		if ((cpy->num_defval = constr->num_defval) > 0)
		{
			cpy->defval = (AttrDefault *) palloc(cpy->num_defval * sizeof(AttrDefault));
			memcpy(cpy->defval, constr->defval, cpy->num_defval * sizeof(AttrDefault));
			for (i = cpy->num_defval - 1; i >= 0; i--)
			{
				if (constr->defval[i].adbin)
					cpy->defval[i].adbin = pstrdup(constr->defval[i].adbin);
			}
		}

		if ((cpy->num_check = constr->num_check) > 0)
		{
			cpy->check = (ConstrCheck *) palloc(cpy->num_check * sizeof(ConstrCheck));
			memcpy(cpy->check, constr->check, cpy->num_check * sizeof(ConstrCheck));
			for (i = cpy->num_check - 1; i >= 0; i--)
			{
				if (constr->check[i].ccname)
					cpy->check[i].ccname = pstrdup(constr->check[i].ccname);
				if (constr->check[i].ccbin)
					cpy->check[i].ccbin = pstrdup(constr->check[i].ccbin);
			}
		}

		desc->constr = cpy;
	}
	else
		desc->constr = NULL;

	return desc;
}

void
FreeTupleDesc(TupleDesc tupdesc)
{
	int			i;

	for (i = 0; i < tupdesc->natts; i++)
		pfree(tupdesc->attrs[i]);
	pfree(tupdesc->attrs);
	if (tupdesc->constr)
	{
		if (tupdesc->constr->num_defval > 0)
		{
			AttrDefault *attrdef = tupdesc->constr->defval;

			for (i = tupdesc->constr->num_defval - 1; i >= 0; i--)
			{
				if (attrdef[i].adbin)
					pfree(attrdef[i].adbin);
			}
			pfree(attrdef);
		}
		if (tupdesc->constr->num_check > 0)
		{
			ConstrCheck *check = tupdesc->constr->check;

			for (i = tupdesc->constr->num_check - 1; i >= 0; i--)
			{
				if (check[i].ccname)
					pfree(check[i].ccname);
				if (check[i].ccbin)
					pfree(check[i].ccbin);
			}
			pfree(check);
		}
		pfree(tupdesc->constr);
	}

	pfree(tupdesc);

}

bool
equalTupleDescs(TupleDesc tupdesc1, TupleDesc tupdesc2)
{
	int			i,
				j,
				n;

	if (tupdesc1->natts != tupdesc2->natts)
		return false;
	for (i = 0; i < tupdesc1->natts; i++)
	{
		Form_pg_attribute attr1 = tupdesc1->attrs[i];
		Form_pg_attribute attr2 = tupdesc2->attrs[i];

		/*
		 * We do not need to check every single field here: we can
		 * disregard attrelid, attnum (it was used to place the row in the
		 * attrs array) and everything derived from the column datatype.
		 */
		if (strcmp(NameStr(attr1->attname), NameStr(attr2->attname)) != 0)
			return false;
		if (attr1->atttypid != attr2->atttypid)
			return false;
		if (attr1->attstattarget != attr2->attstattarget)
			return false;
		if (attr1->atttypmod != attr2->atttypmod)
			return false;
		if (attr1->attstorage != attr2->attstorage)
			return false;
		if (attr1->attnotnull != attr2->attnotnull)
			return false;
	}
	if (tupdesc1->constr != NULL)
	{
		TupleConstr *constr1 = tupdesc1->constr;
		TupleConstr *constr2 = tupdesc2->constr;

		if (constr2 == NULL)
			return false;
		if (constr1->has_not_null != constr2->has_not_null)
			return false;
		n = constr1->num_defval;
		if (n != (int) constr2->num_defval)
			return false;
		for (i = 0; i < n; i++)
		{
			AttrDefault *defval1 = constr1->defval + i;
			AttrDefault *defval2 = constr2->defval;

			/*
			 * We can't assume that the items are always read from the
			 * system catalogs in the same order; so use the adnum field
			 * to identify the matching item to compare.
			 */
			for (j = 0; j < n; defval2++, j++)
			{
				if (defval1->adnum == defval2->adnum)
					break;
			}
			if (j >= n)
				return false;
			if (strcmp(defval1->adbin, defval2->adbin) != 0)
				return false;
		}
		n = constr1->num_check;
		if (n != (int) constr2->num_check)
			return false;
		for (i = 0; i < n; i++)
		{
			ConstrCheck *check1 = constr1->check + i;
			ConstrCheck *check2 = constr2->check;

			/*
			 * Similarly, don't assume that the checks are always read in
			 * the same order; match them up by name and contents. (The
			 * name *should* be unique, but...)
			 */
			for (j = 0; j < n; check2++, j++)
			{
				if (strcmp(check1->ccname, check2->ccname) == 0 &&
					strcmp(check1->ccbin, check2->ccbin) == 0)
					break;
			}
			if (j >= n)
				return false;
		}
	}
	else if (tupdesc2->constr != NULL)
		return false;
	return true;
}

/* ----------------------------------------------------------------
 *		TupleDescInitEntry
 *
 *		This function initializes a single attribute structure in
 *		a preallocated tuple descriptor.
 * ----------------------------------------------------------------
 */
bool
TupleDescInitEntry(TupleDesc desc,
				   AttrNumber attributeNumber,
				   char *attributeName,
				   Oid oidtypeid,
				   int32 typmod,
				   int attdim,
				   bool attisset)
{
	HeapTuple	tuple;
	Form_pg_type typeForm;
	Form_pg_attribute att;

	/*
	 * sanity checks
	 */
	AssertArg(PointerIsValid(desc));
	AssertArg(attributeNumber >= 1);

	/*
	 * attributeName's are sometimes NULL, from resdom's.  I don't know
	 * why that is, though -- Jolly
	 */
/*	  AssertArg(NameIsValid(attributeName));*/

	AssertArg(!PointerIsValid(desc->attrs[attributeNumber - 1]));

	/*
	 * allocate storage for this attribute
	 */

	att = (Form_pg_attribute) palloc(ATTRIBUTE_TUPLE_SIZE);
	desc->attrs[attributeNumber - 1] = att;

	/*
	 * initialize the attribute fields
	 */
	att->attrelid = 0;			/* dummy value */

	if (attributeName != NULL)
		namestrcpy(&(att->attname), attributeName);
	else
		MemSet(NameStr(att->attname), 0, NAMEDATALEN);

	att->attstattarget = 0;
	att->attcacheoff = -1;
	att->atttypmod = typmod;

	att->attnum = attributeNumber;
	att->attndims = attdim;
	att->attisset = attisset;

	att->attnotnull = false;
	att->atthasdef = false;

	/* ----------------
	 *	search the system cache for the type tuple of the attribute
	 *	we are creating so that we can get the typeid and some other
	 *	stuff.
	 *
	 *	Note: in the special case of
	 *
	 *		create EMP (name = text, manager = EMP)
	 *
	 *	RelationNameCreateHeapRelation() calls BuildDesc() which
	 *	calls this routine and since EMP does not exist yet, the
	 *	system cache lookup below fails.  That's fine, but rather
	 *	then doing a elog(ERROR) we just leave that information
	 *	uninitialized, return false, then fix things up later.
	 *	-cim 6/14/90
	 * ----------------
	 */
	tuple = SearchSysCache(TYPEOID,
						   ObjectIdGetDatum(oidtypeid),
						   0, 0, 0);
	if (!HeapTupleIsValid(tuple))
	{
		/*
		 * here type info does not exist yet so we just fill the attribute
		 * with dummy information and return false.
		 */
		att->atttypid = InvalidOid;
		att->attlen = (int16) 0;
		att->attbyval = (bool) 0;
		att->attalign = 'i';
		att->attstorage = 'p';
		return false;
	}

	/*
	 * type info exists so we initialize our attribute information from
	 * the type tuple we found..
	 */
	typeForm = (Form_pg_type) GETSTRUCT(tuple);

	att->atttypid = tuple->t_data->t_oid;

	/*
	 * There are a couple of cases where we must override the information
	 * stored in pg_type.
	 *
	 * First: if this attribute is a set, what is really stored in the
	 * attribute is the OID of a tuple in the pg_proc catalog. The pg_proc
	 * tuple contains the query string which defines this set - i.e., the
	 * query to run to get the set. So the atttypid (just assigned above)
	 * refers to the type returned by this query, but the actual length of
	 * this attribute is the length (size) of an OID.
	 *
	 * (Why not just make the atttypid point to the OID type, instead of the
	 * type the query returns?	Because the executor uses the atttypid to
	 * tell the front end what type will be returned (in BeginCommand),
	 * and in the end the type returned will be the result of the query,
	 * not an OID.)
	 *
	 * (Why not wait until the return type of the set is known (i.e., the
	 * recursive call to the executor to execute the set has returned)
	 * before telling the front end what the return type will be?  Because
	 * the executor is a delicate thing, and making sure that the correct
	 * order of front-end commands is maintained is messy, especially
	 * considering that target lists may change as inherited attributes
	 * are considered, etc.  Ugh.)
	 *
	 * Second: if we are dealing with a complex type (a tuple type), then
	 * pg_type will say that the representation is the same as Oid.  But
	 * if typmod is sizeof(Pointer) then the internal representation is
	 * actually a pointer to a TupleTableSlot, and we have to substitute
	 * that information.
	 *
	 * A set of complex type is first and foremost a set, so its
	 * representation is Oid not pointer.  So, test that case first.
	 */
	if (attisset)
	{
		att->attlen = sizeof(Oid);
		att->attbyval = true;
		att->attalign = 'i';
		att->attstorage = 'p';
	}
	else if (typeForm->typtype == 'c' && typmod == sizeof(Pointer))
	{
		att->attlen = sizeof(Pointer);
		att->attbyval = true;
		att->attalign = 'd';	/* kluge to work with 8-byte pointers */
		/* XXX ought to have a separate attalign value for pointers ... */
		att->attstorage = 'p';
	}
	else
	{
		att->attlen = typeForm->typlen;
		att->attbyval = typeForm->typbyval;
		att->attalign = typeForm->typalign;
		att->attstorage = typeForm->typstorage;
	}

	ReleaseSysCache(tuple);

	return true;
}


/* ----------------------------------------------------------------
 *		TupleDescMakeSelfReference
 *
 *		This function initializes a "self-referential" attribute like
 *		manager in "create EMP (name=text, manager = EMP)".
 *		It calls TypeShellMake() which inserts a "shell" type
 *		tuple into pg_type.  A self-reference is one kind of set, so
 *		its size and byval are the same as for a set.  See the comments
 *		above in TupleDescInitEntry.
 * ----------------------------------------------------------------
 */
static void
TupleDescMakeSelfReference(TupleDesc desc,
						   AttrNumber attnum,
						   char *relname)
{
	Form_pg_attribute att;

	att = desc->attrs[attnum - 1];
	att->atttypid = TypeShellMake(relname);
	att->attlen = sizeof(Oid);
	att->attbyval = true;
	att->attalign = 'i';
	att->attstorage = 'p';
	att->attndims = 0;
}

/* ----------------------------------------------------------------
 *		BuildDescForRelation
 *
 *		This is a general purpose function identical to BuildDesc
 *		but is used by the DefineRelation() code to catch the
 *		special case where you
 *
 *				create FOO ( ..., x = FOO )
 *
 *		here, the initial type lookup for "x = FOO" will fail
 *		because FOO isn't in the catalogs yet.  But since we
 *		are creating FOO, instead of doing an elog() we add
 *		a shell type tuple to pg_type and fix things later
 *		in amcreate().
 * ----------------------------------------------------------------
 */
TupleDesc
BuildDescForRelation(List *schema, char *relname)
{
	int			natts;
	AttrNumber	attnum;
	List	   *p;
	TupleDesc	desc;
	AttrDefault *attrdef = NULL;
	TupleConstr *constr = (TupleConstr *) palloc(sizeof(TupleConstr));
	char	   *attname;
	char		typename[NAMEDATALEN];
	int32		atttypmod;
	int			attdim;
	int			ndef = 0;
	bool		attisset;

	/*
	 * allocate a new tuple descriptor
	 */
	natts = length(schema);
	desc = CreateTemplateTupleDesc(natts);
	constr->has_not_null = false;

	attnum = 0;

	foreach(p, schema)
	{
		ColumnDef  *entry = lfirst(p);
		List	   *arry;

		/*
		 * for each entry in the list, get the name and type information
		 * from the list and have TupleDescInitEntry fill in the attribute
		 * information we need.
		 */
		attnum++;

		attname = entry->colname;
		arry = entry->typename->arrayBounds;
		attisset = entry->typename->setof;
		atttypmod = entry->typename->typmod;

		if (arry != NIL)
		{
			/* array of XXX is _XXX */
			snprintf(typename, NAMEDATALEN,
					 "_%.*s", NAMEDATALEN - 2, entry->typename->name);
			attdim = length(arry);
		}
		else
		{
			StrNCpy(typename, entry->typename->name, NAMEDATALEN);
			attdim = 0;
		}

		if (!TupleDescInitEntry(desc, attnum, attname,
								typenameTypeId(typename),
								atttypmod, attdim, attisset))
		{
			/*
			 * if TupleDescInitEntry() fails, it means there is no type in
			 * the system catalogs.  So now we check if the type name
			 * equals the relation name.  If so we have a self reference,
			 * otherwise it's an error.
			 */
			if (strcmp(typename, relname) == 0)
				TupleDescMakeSelfReference(desc, attnum, relname);
			else
				elog(ERROR, "DefineRelation: no such type %s",
					 typename);
		}

		/* This is for constraints */
		if (entry->is_not_null)
			constr->has_not_null = true;
		desc->attrs[attnum - 1]->attnotnull = entry->is_not_null;

		/*
		 * Note we copy only pre-cooked default expressions. Digestion of
		 * raw ones is someone else's problem.
		 */
		if (entry->cooked_default != NULL)
		{
			if (attrdef == NULL)
				attrdef = (AttrDefault *) palloc(natts * sizeof(AttrDefault));
			attrdef[ndef].adnum = attnum;
			attrdef[ndef].adbin = pstrdup(entry->cooked_default);
			ndef++;
			desc->attrs[attnum - 1]->atthasdef = true;
		}

	}
	if (constr->has_not_null || ndef > 0)
	{
		desc->constr = constr;

		if (ndef > 0)			/* DEFAULTs */
		{
			if (ndef < natts)
				constr->defval = (AttrDefault *)
					repalloc(attrdef, ndef * sizeof(AttrDefault));
			else
				constr->defval = attrdef;
			constr->num_defval = ndef;
		}
		else
		{
			constr->defval = NULL;
			constr->num_defval = 0;
		}
		constr->check = NULL;
		constr->num_check = 0;
	}
	else
	{
		pfree(constr);
		desc->constr = NULL;
	}
	return desc;
}
