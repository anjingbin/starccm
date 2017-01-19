/*-------------------------------------------------------------------------
 *
 * parse_type.h
 *
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: parse_type.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSE_TYPE_H
#define PARSE_TYPE_H

#include "access/htup.h"

typedef HeapTuple Type;

extern bool typeidIsValid(Oid id);
extern Type typeidType(Oid id);
extern Type typenameType(char *s);

extern Oid	typeTypeId(Type tp);
extern int16 typeLen(Type t);
extern bool typeByVal(Type t);
extern char *typeTypeName(Type t);
extern char typeTypeFlag(Type t);
extern Oid	typeTypeRelid(Type typ);
extern Datum stringTypeDatum(Type tp, char *string, int32 atttypmod);

extern char *typeidTypeName(Oid id);
extern Oid	typeidTypeRelid(Oid type_id);
extern Oid	typenameTypeId(char *s);

extern void parseTypeString(const char *str, Oid *type_id, int32 *typmod);

#define ISCOMPLEX(typeid) (typeidTypeRelid(typeid) != InvalidOid)

#endif   /* PARSE_TYPE_H */
