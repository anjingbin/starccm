/*-------------------------------------------------------------------------
 *
 * heapam.h
 *	  POSTGRES heap access method definitions.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: heapam.h,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef HEAPAM_H
#define HEAPAM_H

#include "access/htup.h"
#include "access/relscan.h"
#include "access/tupmacs.h"
#include "access/xlogutils.h"
#include "storage/block.h"
#include "storage/lmgr.h"
#include "utils/rel.h"
#include "utils/tqual.h"

/* ----------------------------------------------------------------
 *				leftover cruft from old statistics code
 * ----------------------------------------------------------------
 */

#define IncrHeapAccessStat(x)	((void) 0)

/* ----------------
 *		fastgetattr
 *
 *		Fetch a user attribute's value as a Datum (might be either a
 *		value, or a pointer into the data area of the tuple).
 *
 *		This must not be used when a system attribute might be requested.
 *		Furthermore, the passed attnum MUST be valid.  Use heap_getattr()
 *		instead, if in doubt.
 *
 *		This gets called many times, so we macro the cacheable and NULL
 *		lookups, and call nocachegetattr() for the rest.
 * ----------------
 */

extern Datum nocachegetattr(HeapTuple tup, int attnum,
			   TupleDesc att, bool *isnull);

#if !defined(DISABLE_COMPLEX_MACRO)

#define fastgetattr(tup, attnum, tupleDesc, isnull)					\
(																	\
	AssertMacro((attnum) > 0),										\
	((isnull) ? (*(isnull) = false) : (dummyret)NULL),				\
	HeapTupleNoNulls(tup) ?											\
	(																\
		(tupleDesc)->attrs[(attnum)-1]->attcacheoff >= 0 ?			\
		(															\
			fetchatt((tupleDesc)->attrs[(attnum)-1],				\
				(char *) (tup)->t_data + (tup)->t_data->t_hoff +	\
					(tupleDesc)->attrs[(attnum)-1]->attcacheoff)	\
		)															\
		:															\
			nocachegetattr((tup), (attnum), (tupleDesc), (isnull))	\
	)																\
	:																\
	(																\
		att_isnull((attnum)-1, (tup)->t_data->t_bits) ?				\
		(															\
			((isnull) ? (*(isnull) = true) : (dummyret)NULL),		\
			(Datum)NULL												\
		)															\
		:															\
		(															\
			nocachegetattr((tup), (attnum), (tupleDesc), (isnull))	\
		)															\
	)																\
)

#else							/* defined(DISABLE_COMPLEX_MACRO) */

extern Datum fastgetattr(HeapTuple tup, int attnum, TupleDesc tupleDesc,
			bool *isnull);
#endif   /* defined(DISABLE_COMPLEX_MACRO) */


/* ----------------
 *		heap_getattr
 *
 *		Extract an attribute of a heap tuple and return it as a Datum.
 *		This works for either system or user attributes.  The given attnum
 *		is properly range-checked.
 *
 *		If the field in question has a NULL value, we return a zero Datum
 *		and set *isnull == true.  Otherwise, we set *isnull == false.
 *
 *		<tup> is the pointer to the heap tuple.  <attnum> is the attribute
 *		number of the column (field) caller wants.	<tupleDesc> is a
 *		pointer to the structure describing the row and all its fields.
 * ----------------
 */
#define heap_getattr(tup, attnum, tupleDesc, isnull) \
( \
	AssertMacro((tup) != NULL), \
	( \
		((attnum) > 0) ? \
		( \
			((attnum) > (int) (tup)->t_data->t_natts) ? \
			( \
				((isnull) ? (*(isnull) = true) : (dummyret)NULL), \
				(Datum)NULL \
			) \
			: \
				fastgetattr((tup), (attnum), (tupleDesc), (isnull)) \
		) \
		: \
			heap_getsysattr((tup), (attnum), (isnull)) \
	) \
)

extern Datum heap_getsysattr(HeapTuple tup, int attnum, bool *isnull);


/* ----------------
 *		function prototypes for heap access method
 *
 * heap_create, heap_create_with_catalog, and heap_drop_with_catalog
 * are declared in catalog/heap.h
 * ----------------
 */

/* heapam.c */

extern Relation relation_open(Oid relationId, LOCKMODE lockmode);
extern Relation relation_openr(const char *relationName, LOCKMODE lockmode);
extern void relation_close(Relation relation, LOCKMODE lockmode);

extern Relation heap_open(Oid relationId, LOCKMODE lockmode);
extern Relation heap_openr(const char *relationName, LOCKMODE lockmode);

#define heap_close(r,l)  relation_close(r,l)

extern HeapScanDesc heap_beginscan(Relation relation, int atend,
			   Snapshot snapshot, unsigned nkeys, ScanKey key);
extern void heap_rescan(HeapScanDesc scan, bool scanFromEnd, ScanKey key);
extern void heap_endscan(HeapScanDesc scan);
extern HeapTuple heap_getnext(HeapScanDesc scandesc, int backw);
extern void heap_fetch(Relation relation, Snapshot snapshot, HeapTuple tup, Buffer *userbuf, IndexScanDesc iscan);
extern ItemPointer heap_get_latest_tid(Relation relation, Snapshot snapshot, ItemPointer tid);
extern void setLastTid(const ItemPointer tid);
extern Oid	heap_insert(Relation relation, HeapTuple tup);
extern int	heap_delete(Relation relation, ItemPointer tid, ItemPointer ctid);
extern int heap_update(Relation relation, ItemPointer otid, HeapTuple tup,
			ItemPointer ctid);
extern int	heap_mark4update(Relation relation, HeapTuple tup, Buffer *userbuf);
extern void simple_heap_delete(Relation relation, ItemPointer tid);
extern void simple_heap_update(Relation relation, ItemPointer otid,
				   HeapTuple tup);
extern void heap_markpos(HeapScanDesc scan);
extern void heap_restrpos(HeapScanDesc scan);

extern void heap_redo(XLogRecPtr lsn, XLogRecord *rptr);
extern void heap_undo(XLogRecPtr lsn, XLogRecord *rptr);
extern void heap_desc(char *buf, uint8 xl_info, char *rec);
extern XLogRecPtr log_heap_clean(Relation reln, Buffer buffer,
			   char *unused, int unlen);
extern XLogRecPtr log_heap_move(Relation reln, Buffer oldbuf,
			  ItemPointerData from,
			  Buffer newbuf, HeapTuple newtup);

/* in common/heaptuple.c */
extern Size ComputeDataSize(TupleDesc tupleDesc, Datum *value, char *nulls);
extern void DataFill(char *data, TupleDesc tupleDesc,
		 Datum *value, char *nulls, uint16 *infomask,
		 bits8 *bit);
extern int	heap_attisnull(HeapTuple tup, int attnum);
extern Datum nocachegetattr(HeapTuple tup, int attnum,
			   TupleDesc att, bool *isnull);
extern HeapTuple heap_copytuple(HeapTuple tuple);
extern void heap_copytuple_with_tuple(HeapTuple src, HeapTuple dest);
extern HeapTuple heap_formtuple(TupleDesc tupleDescriptor,
			   Datum *value, char *nulls);
extern HeapTuple heap_modifytuple(HeapTuple tuple,
		Relation relation, Datum *replValue, char *replNull, char *repl);
extern void heap_freetuple(HeapTuple tuple);
extern HeapTuple heap_addheader(int natts, Size structlen, void *structure);

#endif   /* HEAPAM_H */
