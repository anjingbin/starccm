/*-------------------------------------------------------------------------
 *
 * parse_type.c
 *		handle type operations for parser
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/parser/parse_type.c,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "catalog/pg_type.h"
#include "nodes/parsenodes.h"
#include "parser/parser.h"
#include "parser/parse_expr.h"
#include "parser/parse_type.h"
#include "utils/syscache.h"


/* check to see if a type id is valid,
 * returns true if it is. By using this call before calling
 * typeidType or typeidTypeName, more meaningful error messages
 * can be produced because the caller typically has more context of
 *	what's going on                 - jolly
 */
bool
typeidIsValid(Oid id)
{
	return SearchSysCacheExists(TYPEOID,
								ObjectIdGetDatum(id),
								0, 0, 0);
}

/* return a Type structure, given a type id */
/* NB: caller must ReleaseSysCache the type tuple when done with it */
Type
typeidType(Oid id)
{
	HeapTuple	tup;

	tup = SearchSysCache(TYPEOID,
						 ObjectIdGetDatum(id),
						 0, 0, 0);
	if (!HeapTupleIsValid(tup))
		elog(ERROR, "Unable to locate type oid %u in catalog", id);
	return (Type) tup;
}

/* return a Type structure, given type name */
/* NB: caller must ReleaseSysCache the type tuple when done with it */
Type
typenameType(char *s)
{
	HeapTuple	tup;

	if (s == NULL)
		elog(ERROR, "typenameType: Null typename");

	tup = SearchSysCache(TYPENAME,
						 PointerGetDatum(s),
						 0, 0, 0);
	if (!HeapTupleIsValid(tup))
		elog(ERROR, "Unable to locate type name '%s' in catalog", s);
	return (Type) tup;
}

/* given type (as type struct), return the type OID */
Oid
typeTypeId(Type tp)
{
	if (tp == NULL)
		elog(ERROR, "typeTypeId() called with NULL type struct");
	return tp->t_data->t_oid;
}

/* given type (as type struct), return the length of type */
int16
typeLen(Type t)
{
	Form_pg_type typ;

	typ = (Form_pg_type) GETSTRUCT(t);
	return typ->typlen;
}

/* given type (as type struct), return the value of its 'byval' attribute.*/
bool
typeByVal(Type t)
{
	Form_pg_type typ;

	typ = (Form_pg_type) GETSTRUCT(t);
	return typ->typbyval;
}

/* given type (as type struct), return the name of type */
char *
typeTypeName(Type t)
{
	Form_pg_type typ;

	typ = (Form_pg_type) GETSTRUCT(t);
	/* pstrdup here because result may need to outlive the syscache entry */
	return pstrdup(NameStr(typ->typname));
}

/* given a type, return its typetype ('c' for 'c'atalog types) */
char
typeTypeFlag(Type t)
{
	Form_pg_type typ;

	typ = (Form_pg_type) GETSTRUCT(t);
	return typ->typtype;
}

Oid
typeTypeRelid(Type typ)
{
	Form_pg_type typtup;

	typtup = (Form_pg_type) GETSTRUCT(typ);

	return typtup->typrelid;
}

#ifdef NOT_USED
Oid
typeTypElem(Type typ)
{
	Form_pg_type typtup;

	typtup = (Form_pg_type) GETSTRUCT(typ);

	return typtup->typelem;
}
#endif

#ifdef NOT_USED
/* Given a type structure, return the in-conversion function of the type */
Oid
typeInfunc(Type typ)
{
	Form_pg_type typtup;

	typtup = (Form_pg_type) GETSTRUCT(typ);

	return typtup->typinput;
}
#endif

#ifdef NOT_USED
/* Given a type structure, return the out-conversion function of the type */
Oid
typeOutfunc(Type typ)
{
	Form_pg_type typtup;

	typtup = (Form_pg_type) GETSTRUCT(typ);

	return typtup->typoutput;
}
#endif

/* Given a type structure and a string, returns the internal form of
   that string */
Datum
stringTypeDatum(Type tp, char *string, int32 atttypmod)
{
	Oid			op;
	Oid			typelem;

	op = ((Form_pg_type) GETSTRUCT(tp))->typinput;
	typelem = ((Form_pg_type) GETSTRUCT(tp))->typelem;	/* XXX - used for
														 * array_in */
	return OidFunctionCall3(op,
							CStringGetDatum(string),
							ObjectIdGetDatum(typelem),
							Int32GetDatum(atttypmod));
}

/* Given a type id, returns the out-conversion function of the type */
#ifdef NOT_USED
Oid
typeidOutfunc(Oid type_id)
{
	HeapTuple	typeTuple;
	Form_pg_type type;
	Oid			outfunc;

	typeTuple = SearchSysCache(TYPEOID,
							   ObjectIdGetDatum(type_id),
							   0, 0, 0);
	if (!HeapTupleIsValid(typeTuple))
		elog(ERROR, "typeidOutfunc: Invalid type - oid = %u", type_id);

	type = (Form_pg_type) GETSTRUCT(typeTuple);
	outfunc = type->typoutput;
	ReleaseSysCache(typeTuple);
	return outfunc;
}
#endif

/* return a type name, given a typeid */
char *
typeidTypeName(Oid id)
{
	HeapTuple	tup;
	Form_pg_type typetuple;
	char	   *result;

	tup = SearchSysCache(TYPEOID,
						 ObjectIdGetDatum(id),
						 0, 0, 0);
	if (!HeapTupleIsValid(tup))
		elog(ERROR, "Unable to locate type oid %u in catalog", id);
	typetuple = (Form_pg_type) GETSTRUCT(tup);

	/*
	 * pstrdup here because result may need to outlive the syscache entry
	 * (eg, it might end up as part of a parse tree that will outlive the
	 * current transaction...)
	 */
	result = pstrdup(NameStr(typetuple->typname));
	ReleaseSysCache(tup);
	return result;
}

/* given a typeid, return the type's typrelid (associated relation, if any) */
Oid
typeidTypeRelid(Oid type_id)
{
	HeapTuple	typeTuple;
	Form_pg_type type;
	Oid			result;

	typeTuple = SearchSysCache(TYPEOID,
							   ObjectIdGetDatum(type_id),
							   0, 0, 0);
	if (!HeapTupleIsValid(typeTuple))
		elog(ERROR, "typeidTypeRelid: Invalid type - oid = %u", type_id);

	type = (Form_pg_type) GETSTRUCT(typeTuple);
	result = type->typrelid;
	ReleaseSysCache(typeTuple);
	return result;
}

/* given a type name, return the type's typeid */
Oid
typenameTypeId(char *s)
{
	Type		typ = typenameType(s);
	Oid			result;

	result = typ->t_data->t_oid;
	ReleaseSysCache(typ);
	return result;
}

/*
 * Given a string that is supposed to be a SQL-compatible type declaration,
 * such as "int4" or "integer" or "character varying(32)", parse
 * the string and convert it to a type OID and type modifier.
 *
 * This routine is not currently used by the main backend, but it is
 * exported for use by add-on modules such as plpgsql, in hopes of
 * centralizing parsing knowledge about SQL type declarations.
 */
void
parseTypeString(const char *str, Oid *type_id, int32 *typmod)
{
	char	   *buf;
	List	   *raw_parsetree_list;
	SelectStmt *stmt;
	ResTarget  *restarget;
	A_Const    *aconst;
	TypeName   *typename;

	buf = (char *) palloc(strlen(str) + 16);
	sprintf(buf, "SELECT (NULL::%s)", str);

	raw_parsetree_list = parser(buf, NULL, 0);

	/*
	 * Make sure we got back exactly what we expected and no more;
	 * paranoia is justified since the string might contain anything.
	 */
	if (length(raw_parsetree_list) != 1)
		elog(ERROR, "Invalid type name '%s'", str);
	stmt = (SelectStmt *) lfirst(raw_parsetree_list);
	if (stmt == NULL ||
		!IsA(stmt, SelectStmt) ||
		stmt->distinctClause != NIL ||
		stmt->into != NULL ||
		stmt->fromClause != NIL ||
		stmt->whereClause != NULL ||
		stmt->groupClause != NIL ||
		stmt->havingClause != NULL ||
		stmt->sortClause != NIL ||
		stmt->portalname != NULL ||
		stmt->limitOffset != NULL ||
		stmt->limitCount != NULL ||
		stmt->forUpdate != NIL ||
		stmt->op != SETOP_NONE)
		elog(ERROR, "Invalid type name '%s'", str);
	if (length(stmt->targetList) != 1)
		elog(ERROR, "Invalid type name '%s'", str);
	restarget = (ResTarget *) lfirst(stmt->targetList);
	if (restarget == NULL ||
		!IsA(restarget, ResTarget) ||
		restarget->name != NULL ||
		restarget->indirection != NIL)
		elog(ERROR, "Invalid type name '%s'", str);
	aconst = (A_Const *) restarget->val;
	if (aconst == NULL ||
		!IsA(aconst, A_Const) ||
		aconst->val.type != T_Null)
		elog(ERROR, "Invalid type name '%s'", str);
	typename = aconst->typename;
	if (typename == NULL ||
		!IsA(typename, TypeName))
		elog(ERROR, "Invalid type name '%s'", str);

	*type_id = typenameTypeId(TypeNameToInternalName(typename));
	*typmod = typename->typmod;

	pfree(buf);
}
