/*-------------------------------------------------------------------------
 *
 * nbtree.h
 *	  header file for postgres btree access method implementation.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nbtree.h,v 1.2 2004/07/05 08:55:58 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NBTREE_H
#define NBTREE_H

#include "access/itup.h"
#include "access/relscan.h"
#include "access/sdir.h"
#include "access/xlogutils.h"

/*
 *	BTPageOpaqueData -- At the end of every page, we store a pointer
 *	to both siblings in the tree.  This is used to do forward/backward
 *	index scans.  See Lehman and Yao's paper for more
 *	info.  In addition, we need to know what type of page this is
 *	(leaf or internal), and whether the page is available for reuse.
 *
 *	We also store a back-link to the parent page, but this cannot be trusted
 *	very far since it does not get updated when the parent is split.
 *	See backend/access/nbtree/README for details.
 */

typedef struct BTPageOpaqueData
{
	BlockNumber btpo_prev;		/* used for backward index scans */
	BlockNumber btpo_next;		/* used for forward index scans */
	BlockNumber btpo_parent;	/* pointer to parent, but not updated on
								 * parent split */
	uint16		btpo_flags;		/* LEAF?, ROOT?, FREE?, META?, REORDER? */

} BTPageOpaqueData;

typedef BTPageOpaqueData *BTPageOpaque;

/* Bits defined in btpo_flags */
#define BTP_LEAF		(1 << 0)	/* leaf page, if not internal page */
#define BTP_ROOT		(1 << 1)	/* root page (has no parent) */
#define BTP_FREE		(1 << 2)	/* page not in use */
#define BTP_META		(1 << 3)	/* meta-page */
#define BTP_REORDER		(1 << 4)	/* items need reordering */


/*
 * The Meta page is always the first page in the btree index.
 * Its primary purpose is to point to the location of the btree root page.
 */

typedef struct BTMetaPageData
{
	uint32		btm_magic;
	uint32		btm_version;
	BlockNumber btm_root;
	int32		btm_level;
} BTMetaPageData;

#define BTPageGetMeta(p) \
	((BTMetaPageData *) &((PageHeader) p)->pd_linp[0])

#define BTREE_METAPAGE	0		/* first page is meta */
#define BTREE_MAGIC		0x053162	/* magic number of btree pages */

#define BTreeInvalidParent(opaque)	\
	(opaque->btpo_parent == InvalidBlockNumber || \
		opaque->btpo_parent == BTREE_METAPAGE)

#define BTREE_VERSION	1

/*
 *	BTScanOpaqueData is used to remember which buffers we're currently
 *	examining in the scan.	We keep these buffers pinned (but not locked,
 *	see nbtree.c) and recorded in the opaque entry of the scan to avoid
 *	doing a ReadBuffer() for every tuple in the index.
 *
 *	And it's used to remember actual scankey info (we need it
 *	if some scankeys evaled at runtime).
 *
 *	curHeapIptr & mrkHeapIptr are heap iptr-s from current/marked
 *	index tuples: we don't adjust scans on insertions (and, if LLL
 *	is ON, don't hold locks on index pages between passes) - we
 *	use these pointers to restore index scan positions...
 *		- vadim 07/29/98
 */

typedef struct BTScanOpaqueData
{
	Buffer		btso_curbuf;
	Buffer		btso_mrkbuf;
	ItemPointerData curHeapIptr;
	ItemPointerData mrkHeapIptr;
	/* these fields are set by _bt_orderkeys(), which see for more info: */
	bool		qual_ok;		/* false if qual can never be satisfied */
	uint16		numberOfKeys;	/* number of scan keys */
	uint16		numberOfRequiredKeys;	/* number of keys that must be
										 * matched to continue the scan */
	ScanKey		keyData;		/* array of scan keys */
} BTScanOpaqueData;

typedef BTScanOpaqueData *BTScanOpaque;

/*
 *	BTItems are what we store in the btree.  Each item is an index tuple,
 *	including key and pointer values.  (In some cases either the key or the
 *	pointer may go unused, see backend/access/nbtree/README for details.)
 *
 *	Old comments:
 *	In addition, we must guarantee that all tuples in the index are unique,
 *	in order to satisfy some assumptions in Lehman and Yao.  The way that we
 *	do this is by generating a new OID for every insertion that we do in the
 *	tree.  This adds eight bytes to the size of btree index tuples.  Note
 *	that we do not use the OID as part of a composite key; the OID only
 *	serves as a unique identifier for a given index tuple (logical position
 *	within a page).
 *
 *	New comments:
 *	actually, we must guarantee that all tuples in A LEVEL
 *	are unique, not in ALL INDEX. So, we can use bti_itup->t_tid
 *	as unique identifier for a given index tuple (logical position
 *	within a level). - vadim 04/09/97
 */

typedef struct BTItemData
{
	IndexTupleData bti_itup;
} BTItemData;

typedef BTItemData *BTItem;

/*
 * For XLOG: size without alignement. Sizeof works as long as
 * IndexTupleData has exactly 8 bytes.
 */
#define SizeOfBTItem	sizeof(BTItemData)

/* Test whether items are the "same" per the above notes */
#define BTItemSame(i1, i2)	  ( (i1)->bti_itup.t_tid.ip_blkid.bi_hi == \
								(i2)->bti_itup.t_tid.ip_blkid.bi_hi && \
								(i1)->bti_itup.t_tid.ip_blkid.bi_lo == \
								(i2)->bti_itup.t_tid.ip_blkid.bi_lo && \
								(i1)->bti_itup.t_tid.ip_posid == \
								(i2)->bti_itup.t_tid.ip_posid )

/*
 *	BTStackData -- As we descend a tree, we push the (key, pointer)
 *	pairs from internal nodes onto a private stack.  If we split a
 *	leaf, we use this stack to walk back up the tree and insert data
 *	into parent nodes (and possibly to split them, too).  Lehman and
 *	Yao's update algorithm guarantees that under no circumstances can
 *	our private stack give us an irredeemably bad picture up the tree.
 *	Again, see the paper for details.
 */

typedef struct BTStackData
{
	BlockNumber bts_blkno;
	OffsetNumber bts_offset;
	BTItemData	bts_btitem;
	struct BTStackData *bts_parent;
} BTStackData;

typedef BTStackData *BTStack;

/*
 *	We need to be able to tell the difference between read and write
 *	requests for pages, in order to do locking correctly.
 */

#define BT_READ			BUFFER_LOCK_SHARE
#define BT_WRITE		BUFFER_LOCK_EXCLUSIVE

/*
 *	In general, the btree code tries to localize its knowledge about
 *	page layout to a couple of routines.  However, we need a special
 *	value to indicate "no page number" in those places where we expect
 *	page numbers.  We can use zero for this because we never need to
 *	make a pointer to the metadata page.
 */

#define P_NONE			0

/*
 * Macros to test whether a page is leftmost or rightmost on its tree level,
 * as well as other state info kept in the opaque data.
 */
#define P_LEFTMOST(opaque)		((opaque)->btpo_prev == P_NONE)
#define P_RIGHTMOST(opaque)		((opaque)->btpo_next == P_NONE)
#define P_ISLEAF(opaque)		((opaque)->btpo_flags & BTP_LEAF)
#define P_ISROOT(opaque)		((opaque)->btpo_flags & BTP_ROOT)

/*
 *	Lehman and Yao's algorithm requires a ``high key'' on every non-rightmost
 *	page.  The high key is not a data key, but gives info about what range of
 *	keys is supposed to be on this page.  The high key on a page is required
 *	to be greater than or equal to any data key that appears on the page.
 *	If we find ourselves trying to insert a key > high key, we know we need
 *	to move right (this should only happen if the page was split since we
 *	examined the parent page).
 *
 *	Our insertion algorithm guarantees that we can use the initial least key
 *	on our right sibling as the high key.  Once a page is created, its high
 *	key changes only if the page is split.
 *
 *	On a non-rightmost page, the high key lives in item 1 and data items
 *	start in item 2.  Rightmost pages have no high key, so we store data
 *	items beginning in item 1.
 */

#define P_HIKEY				((OffsetNumber) 1)
#define P_FIRSTKEY			((OffsetNumber) 2)
#define P_FIRSTDATAKEY(opaque)	(P_RIGHTMOST(opaque) ? P_HIKEY : P_FIRSTKEY)

/*
 * XLOG allows to store some information in high 4 bits of log
 * record xl_info field
 */
#define XLOG_BTREE_DELETE	0x00	/* delete btitem */
#define XLOG_BTREE_INSERT	0x10	/* add btitem without split */
#define XLOG_BTREE_SPLIT	0x20	/* add btitem with split */
#define XLOG_BTREE_SPLEFT	0x30	/* as above + flag that new btitem */
 /* goes to the left sibling */
#define XLOG_BTREE_NEWROOT	0x40	/* new root page */

#define XLOG_BTREE_LEAF		0x80	/* leaf/internal page was changed */

/*
 * All what we need to find changed index tuple
 */
typedef struct xl_btreetid
{
	RelFileNode node;
	ItemPointerData tid;		/* changed tuple id */
} xl_btreetid;

/*
 * This is what we need to know about delete
 */
typedef struct xl_btree_delete
{
	xl_btreetid target;			/* deleted tuple id */
} xl_btree_delete;

#define SizeOfBtreeDelete	(offsetof(xl_btreetid, tid) + SizeOfIptrData)

/*
 * This is what we need to know about pure (without split) insert
 */
typedef struct xl_btree_insert
{
	xl_btreetid target;			/* inserted tuple id */
	/* BTITEM FOLLOWS AT END OF STRUCT */
} xl_btree_insert;

#define SizeOfBtreeInsert	(offsetof(xl_btreetid, tid) + SizeOfIptrData)

/*
 * On insert with split we save items of both left and right siblings
 * and restore content of both pages from log record
 */
typedef struct xl_btree_split
{
	xl_btreetid target;			/* inserted tuple id */
	BlockIdData otherblk;		/* second block participated in split: */
	/* first one is stored in target' tid */
	BlockIdData parentblk;		/* parent block */
	BlockIdData leftblk;		/* prev left block */
	BlockIdData rightblk;		/* next right block */
	uint16		leftlen;		/* len of left page items below */
	/* LEFT AND RIGHT PAGES ITEMS FOLLOW AT THE END */
} xl_btree_split;

#define SizeOfBtreeSplit	(offsetof(xl_btree_split, leftlen) + sizeof(uint16))

/*
 * New root log record.
 */
typedef struct xl_btree_newroot
{
	RelFileNode node;
	int32		level;
	BlockIdData rootblk;
	/* 0 or 2 BTITEMS FOLLOW AT END OF STRUCT */
} xl_btree_newroot;

#define SizeOfBtreeNewroot	(offsetof(xl_btree_newroot, rootblk) + sizeof(BlockIdData))

/*
 *	Operator strategy numbers -- ordering of these is <, <=, =, >=, >
 */

#define BTLessStrategyNumber			1
#define BTLessEqualStrategyNumber		2
#define BTEqualStrategyNumber			3
#define BTGreaterEqualStrategyNumber	4
#define BTGreaterStrategyNumber			5
#define BTMaxStrategyNumber				5

/*
 *	When a new operator class is declared, we require that the user
 *	supply us with an amproc procedure for determining whether, for
 *	two keys a and b, a < b, a = b, or a > b.  This routine must
 *	return < 0, 0, > 0, respectively, in these three cases.  Since we
 *	only have one such proc in amproc, it's number 1.
 */

#define BTORDER_PROC	1

/*
 * prototypes for functions in nbtree.c (external entry points for btree)
 */
extern bool BuildingBtree;		/* in nbtree.c */

extern void AtEOXact_nbtree(void);

extern Datum btbuild(PG_FUNCTION_ARGS);
extern Datum btinsert(PG_FUNCTION_ARGS);
extern Datum btgettuple(PG_FUNCTION_ARGS);
extern Datum btbeginscan(PG_FUNCTION_ARGS);
extern Datum btrescan(PG_FUNCTION_ARGS);
extern void btmovescan(IndexScanDesc scan, Datum v);
extern Datum btendscan(PG_FUNCTION_ARGS);
extern Datum btmarkpos(PG_FUNCTION_ARGS);
extern Datum btrestrpos(PG_FUNCTION_ARGS);
extern Datum btbulkdelete(PG_FUNCTION_ARGS);

extern void btree_redo(XLogRecPtr lsn, XLogRecord *record);
extern void btree_undo(XLogRecPtr lsn, XLogRecord *record);
extern void btree_desc(char *buf, uint8 xl_info, char *rec);

/*
 * prototypes for functions in nbtinsert.c
 */
extern InsertIndexResult _bt_doinsert(Relation rel, BTItem btitem,
			 bool index_is_unique, Relation heapRel);

/*
 * prototypes for functions in nbtpage.c
 */
extern void _bt_metapinit(Relation rel);
extern Buffer _bt_getroot(Relation rel, int access);
extern Buffer _bt_getbuf(Relation rel, BlockNumber blkno, int access);
extern void _bt_relbuf(Relation rel, Buffer buf);
extern void _bt_wrtbuf(Relation rel, Buffer buf);
extern void _bt_wrtnorelbuf(Relation rel, Buffer buf);
extern void _bt_pageinit(Page page, Size size);
extern void _bt_metaproot(Relation rel, BlockNumber rootbknum, int level);
extern void _bt_itemdel(Relation rel, Buffer buf, ItemPointer tid);

/*
 * prototypes for functions in nbtsearch.c
 */
extern BTStack _bt_search(Relation rel, int keysz, ScanKey scankey,
		   Buffer *bufP, int access);
extern Buffer _bt_moveright(Relation rel, Buffer buf, int keysz,
			  ScanKey scankey, int access);
extern OffsetNumber _bt_binsrch(Relation rel, Buffer buf, int keysz,
			ScanKey scankey);
extern int32 _bt_compare(Relation rel, int keysz, ScanKey scankey,
			Page page, OffsetNumber offnum);
extern RetrieveIndexResult _bt_next(IndexScanDesc scan, ScanDirection dir);
extern RetrieveIndexResult _bt_first(IndexScanDesc scan, ScanDirection dir);
extern bool _bt_step(IndexScanDesc scan, Buffer *bufP, ScanDirection dir);

/*
 * prototypes for functions in nbtstrat.c
 */
extern StrategyNumber _bt_getstrat(Relation rel, AttrNumber attno,
			 RegProcedure proc);

/*
 * prototypes for functions in nbtutils.c
 */
extern ScanKey _bt_mkscankey(Relation rel, IndexTuple itup);
extern ScanKey _bt_mkscankey_nodata(Relation rel);
extern void _bt_freeskey(ScanKey skey);
extern void _bt_freestack(BTStack stack);
extern void _bt_orderkeys(Relation relation, BTScanOpaque so);
extern bool _bt_checkkeys(IndexScanDesc scan, IndexTuple tuple,
			  ScanDirection dir, bool *continuescan);
extern BTItem _bt_formitem(IndexTuple itup);

/*
 * prototypes for functions in nbtsort.c
 */

typedef struct BTSpool BTSpool; /* opaque type known only within nbtsort.c */

extern BTSpool *_bt_spoolinit(Relation index, bool isunique);
extern void _bt_spooldestroy(BTSpool *btspool);
extern void _bt_spool(BTItem btitem, BTSpool *btspool);
extern void _bt_leafbuild(BTSpool *btspool, BTSpool *spool2);

#endif   /* NBTREE_H */
