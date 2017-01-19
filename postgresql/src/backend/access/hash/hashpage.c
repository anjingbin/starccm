/*-------------------------------------------------------------------------
 *
 * hashpage.c
 *	  Hash table page management code for the Postgres hash access method
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/access/hash/hashpage.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 * NOTES
 *	  Postgres hash pages look like ordinary relation pages.  The opaque
 *	  data at high addresses includes information about the page including
 *	  whether a page is an overflow page or a true bucket, the block
 *	  numbers of the preceding and following pages, and the overflow
 *	  address of the page if it is an overflow page.
 *
 *	  The first page in a hash relation, page zero, is special -- it stores
 *	  information describing the hash table; it is referred to as the
 *	  "meta page." Pages one and higher store the actual data.
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/genam.h"
#include "access/hash.h"
#include "miscadmin.h"
#include "storage/lmgr.h"


static void _hash_setpagelock(Relation rel, BlockNumber blkno, int access);
static void _hash_unsetpagelock(Relation rel, BlockNumber blkno, int access);
static void _hash_splitpage(Relation rel, Buffer metabuf, Bucket obucket, Bucket nbucket);

/*
 *	We use high-concurrency locking on hash indices.  There are two cases in
 *	which we don't do locking.  One is when we're building the index.
 *	Since the creating transaction has not committed, no one can see
 *	the index, and there's no reason to share locks.  The second case
 *	is when we're just starting up the database system.  We use some
 *	special-purpose initialization code in the relation cache manager
 *	(see utils/cache/relcache.c) to allow us to do indexed scans on
 *	the system catalogs before we'd normally be able to.  This happens
 *	before the lock table is fully initialized, so we can't use it.
 *	Strictly speaking, this violates 2pl, but we don't do 2pl on the
 *	system catalogs anyway.
 *
 *	Note that our page locks are actual lockmanager locks, not buffer
 *	locks (as are used by btree, for example).	This is a good idea because
 *	the algorithms are not deadlock-free, and we'd better be able to detect
 *	and recover from deadlocks.
 *
 *	Another important difference from btree is that a hash indexscan
 *	retains both a lock and a buffer pin on the current index page
 *	between hashgettuple() calls (btree keeps only a buffer pin).
 *	Because of this, it's safe to do item deletions with only a regular
 *	write lock on a hash page --- there cannot be an indexscan stopped on
 *	the page being deleted, other than an indexscan of our own backend,
 *	which will be taken care of by _hash_adjscans.
 */


#define USELOCKING		(!BuildingHash && !IsInitProcessingMode())


/*
 *	_hash_metapinit() -- Initialize the metadata page of a hash index,
 *				the two buckets that we begin with and the initial
 *				bitmap page.
 */
void
_hash_metapinit(Relation rel)
{
	HashMetaPage metap;
	HashPageOpaque pageopaque;
	Buffer		metabuf;
	Buffer		buf;
	Page		pg;
	int			nbuckets;
	uint32		nelem;			/* number elements */
	uint32		lg2nelem;		/* _hash_log2(nelem)   */
	uint16		i;

	/* can't be sharing this with anyone, now... */
	if (USELOCKING)
		LockRelation(rel, AccessExclusiveLock);

	if (RelationGetNumberOfBlocks(rel) != 0)
		elog(ERROR, "Cannot initialize non-empty hash table %s",
			 RelationGetRelationName(rel));

	metabuf = _hash_getbuf(rel, HASH_METAPAGE, HASH_WRITE);
	pg = BufferGetPage(metabuf);
	metap = (HashMetaPage) pg;
	_hash_pageinit(pg, BufferGetPageSize(metabuf));

	metap->hashm_magic = HASH_MAGIC;
	metap->hashm_version = HASH_VERSION;
	metap->hashm_nkeys = 0;
	metap->hashm_nmaps = 0;
	metap->hashm_ffactor = DEFAULT_FFACTOR;
	metap->hashm_bsize = BufferGetPageSize(metabuf);
	metap->hashm_bshift = _hash_log2(metap->hashm_bsize);
	for (i = metap->hashm_bshift; i > 0; --i)
	{
		if ((1 << i) < (metap->hashm_bsize -
						(MAXALIGN(sizeof(PageHeaderData)) +
						 MAXALIGN(sizeof(HashPageOpaqueData)))))
			break;
	}
	Assert(i);
	metap->hashm_bmsize = 1 << i;
	metap->hashm_procid = index_getprocid(rel, 1, HASHPROC);

	/*
	 * Make nelem = 2 rather than 0 so that we end up allocating space for
	 * the next greater power of two number of buckets.
	 */
	nelem = 2;
	lg2nelem = 1;				/* _hash_log2(MAX(nelem, 2)) */
	nbuckets = 2;				/* 1 << lg2nelem */

	MemSet((char *) metap->hashm_spares, 0, sizeof(metap->hashm_spares));
	MemSet((char *) metap->hashm_mapp, 0, sizeof(metap->hashm_mapp));

	metap->hashm_spares[lg2nelem] = 2;	/* lg2nelem + 1 */
	metap->hashm_spares[lg2nelem + 1] = 2;		/* lg2nelem + 1 */
	metap->hashm_ovflpoint = 1; /* lg2nelem */
	metap->hashm_lastfreed = 2;

	metap->hashm_maxbucket = metap->hashm_lowmask = 1;	/* nbuckets - 1 */
	metap->hashm_highmask = 3;	/* (nbuckets << 1) - 1 */

	pageopaque = (HashPageOpaque) PageGetSpecialPointer(pg);
	pageopaque->hasho_oaddr = InvalidOvflAddress;
	pageopaque->hasho_prevblkno = InvalidBlockNumber;
	pageopaque->hasho_nextblkno = InvalidBlockNumber;
	pageopaque->hasho_flag = LH_META_PAGE;
	pageopaque->hasho_bucket = -1;

	/*
	 * First bitmap page is at: splitpoint lg2nelem page offset 1 which
	 * turns out to be page 3. Couldn't initialize page 3  until we
	 * created the first two buckets above.
	 */
	if (_hash_initbitmap(rel, metap, OADDR_OF(lg2nelem, 1), lg2nelem + 1, 0))
		elog(ERROR, "Problem with _hash_initbitmap.");

	/* all done */
	_hash_wrtnorelbuf(rel, metabuf);

	/*
	 * initialize the first two buckets
	 */
	for (i = 0; i <= 1; i++)
	{
		buf = _hash_getbuf(rel, BUCKET_TO_BLKNO(i), HASH_WRITE);
		pg = BufferGetPage(buf);
		_hash_pageinit(pg, BufferGetPageSize(buf));
		pageopaque = (HashPageOpaque) PageGetSpecialPointer(pg);
		pageopaque->hasho_oaddr = InvalidOvflAddress;
		pageopaque->hasho_prevblkno = InvalidBlockNumber;
		pageopaque->hasho_nextblkno = InvalidBlockNumber;
		pageopaque->hasho_flag = LH_BUCKET_PAGE;
		pageopaque->hasho_bucket = i;
		_hash_wrtbuf(rel, buf);
	}

	_hash_relbuf(rel, metabuf, HASH_WRITE);

	if (USELOCKING)
		UnlockRelation(rel, AccessExclusiveLock);
}

/*
 *	_hash_getbuf() -- Get a buffer by block number for read or write.
 *
 *		When this routine returns, the appropriate lock is set on the
 *		requested buffer its reference count is correct.
 *
 *		XXX P_NEW is not used because, unlike the tree structures, we
 *		need the bucket blocks to be at certain block numbers.	we must
 *		depend on the caller to call _hash_pageinit on the block if it
 *		knows that this is a new block.
 */
Buffer
_hash_getbuf(Relation rel, BlockNumber blkno, int access)
{
	Buffer		buf;

	if (blkno == P_NEW)
		elog(ERROR, "_hash_getbuf: internal error: hash AM does not use P_NEW");
	switch (access)
	{
		case HASH_WRITE:
		case HASH_READ:
			_hash_setpagelock(rel, blkno, access);
			break;
		default:
			elog(ERROR, "_hash_getbuf: invalid access (%d) on new blk: %s",
				 access, RelationGetRelationName(rel));
			break;
	}
	buf = ReadBuffer(rel, blkno);

	/* ref count and lock type are correct */
	return buf;
}

/*
 *	_hash_relbuf() -- release a locked buffer.
 */
void
_hash_relbuf(Relation rel, Buffer buf, int access)
{
	BlockNumber blkno;

	blkno = BufferGetBlockNumber(buf);

	switch (access)
	{
		case HASH_WRITE:
		case HASH_READ:
			_hash_unsetpagelock(rel, blkno, access);
			break;
		default:
			elog(ERROR, "_hash_relbuf: invalid access (%d) on blk %x: %s",
				 access, blkno, RelationGetRelationName(rel));
	}

	ReleaseBuffer(buf);
}

/*
 *	_hash_wrtbuf() -- write a hash page to disk.
 *
 *		This routine releases the lock held on the buffer and our reference
 *		to it.	It is an error to call _hash_wrtbuf() without a write lock
 *		or a reference to the buffer.
 */
void
_hash_wrtbuf(Relation rel, Buffer buf)
{
	BlockNumber blkno;

	blkno = BufferGetBlockNumber(buf);
	WriteBuffer(buf);
	_hash_unsetpagelock(rel, blkno, HASH_WRITE);
}

/*
 *	_hash_wrtnorelbuf() -- write a hash page to disk, but do not release
 *						 our reference or lock.
 *
 *		It is an error to call _hash_wrtnorelbuf() without a write lock
 *		or a reference to the buffer.
 */
void
_hash_wrtnorelbuf(Relation rel, Buffer buf)
{
	BlockNumber blkno;

	blkno = BufferGetBlockNumber(buf);
	WriteNoReleaseBuffer(buf);
}

Page
_hash_chgbufaccess(Relation rel,
				   Buffer *bufp,
				   int from_access,
				   int to_access)
{
	BlockNumber blkno;

	blkno = BufferGetBlockNumber(*bufp);

	switch (from_access)
	{
		case HASH_WRITE:
			_hash_wrtbuf(rel, *bufp);
			break;
		case HASH_READ:
			_hash_relbuf(rel, *bufp, from_access);
			break;
		default:
			elog(ERROR, "_hash_chgbufaccess: invalid access (%d) on blk %x: %s",
				 from_access, blkno, RelationGetRelationName(rel));
			break;
	}
	*bufp = _hash_getbuf(rel, blkno, to_access);
	return BufferGetPage(*bufp);
}

/*
 *	_hash_pageinit() -- Initialize a new page.
 */
void
_hash_pageinit(Page page, Size size)
{
	Assert(PageIsNew(page));
	PageInit(page, size, sizeof(HashPageOpaqueData));
}

static void
_hash_setpagelock(Relation rel,
				  BlockNumber blkno,
				  int access)
{

	if (USELOCKING)
	{
		switch (access)
		{
			case HASH_WRITE:
				LockPage(rel, blkno, ExclusiveLock);
				break;
			case HASH_READ:
				LockPage(rel, blkno, ShareLock);
				break;
			default:
				elog(ERROR, "_hash_setpagelock: invalid access (%d) on blk %x: %s",
					 access, blkno, RelationGetRelationName(rel));
				break;
		}
	}
}

static void
_hash_unsetpagelock(Relation rel,
					BlockNumber blkno,
					int access)
{

	if (USELOCKING)
	{
		switch (access)
		{
			case HASH_WRITE:
				UnlockPage(rel, blkno, ExclusiveLock);
				break;
			case HASH_READ:
				UnlockPage(rel, blkno, ShareLock);
				break;
			default:
				elog(ERROR, "_hash_unsetpagelock: invalid access (%d) on blk %x: %s",
					 access, blkno, RelationGetRelationName(rel));
				break;
		}
	}
}

/*
 * Delete a hash index item.
 *
 * It is safe to delete an item after acquiring a regular WRITE lock on
 * the page, because no other backend can hold a READ lock on the page,
 * and that means no other backend currently has an indexscan stopped on
 * any item of the item being deleted.	Our own backend might have such
 * an indexscan (in fact *will*, since that's how VACUUM found the item
 * in the first place), but _hash_adjscans will fix the scan position.
 */
void
_hash_pagedel(Relation rel, ItemPointer tid)
{
	Buffer		buf;
	Buffer		metabuf;
	Page		page;
	BlockNumber blkno;
	OffsetNumber offno;
	HashMetaPage metap;
	HashPageOpaque opaque;

	blkno = ItemPointerGetBlockNumber(tid);
	offno = ItemPointerGetOffsetNumber(tid);

	buf = _hash_getbuf(rel, blkno, HASH_WRITE);
	page = BufferGetPage(buf);
	_hash_checkpage(page, LH_BUCKET_PAGE | LH_OVERFLOW_PAGE);
	opaque = (HashPageOpaque) PageGetSpecialPointer(page);

	PageIndexTupleDelete(page, offno);
	_hash_wrtnorelbuf(rel, buf);

	if (PageIsEmpty(page) && (opaque->hasho_flag & LH_OVERFLOW_PAGE))
	{
		buf = _hash_freeovflpage(rel, buf);
		if (BufferIsValid(buf))
			_hash_relbuf(rel, buf, HASH_WRITE);
	}
	else
		_hash_relbuf(rel, buf, HASH_WRITE);

	metabuf = _hash_getbuf(rel, HASH_METAPAGE, HASH_WRITE);
	metap = (HashMetaPage) BufferGetPage(metabuf);
	_hash_checkpage((Page) metap, LH_META_PAGE);
	metap->hashm_nkeys--;
	_hash_wrtbuf(rel, metabuf);
}

void
_hash_expandtable(Relation rel, Buffer metabuf)
{
	HashMetaPage metap;
	Bucket		old_bucket;
	Bucket		new_bucket;
	uint32		spare_ndx;

/*	  elog(DEBUG, "_hash_expandtable: expanding..."); */

	metap = (HashMetaPage) BufferGetPage(metabuf);
	_hash_checkpage((Page) metap, LH_META_PAGE);

	metap = (HashMetaPage) _hash_chgbufaccess(rel, &metabuf, HASH_READ, HASH_WRITE);
	new_bucket = ++metap->hashm_maxbucket;
	metap = (HashMetaPage) _hash_chgbufaccess(rel, &metabuf, HASH_WRITE, HASH_READ);
	old_bucket = (metap->hashm_maxbucket & metap->hashm_lowmask);

	/*
	 * If the split point is increasing (hashm_maxbucket's log base 2 *
	 * increases), we need to copy the current contents of the spare split
	 * bucket to the next bucket.
	 */
	spare_ndx = _hash_log2(metap->hashm_maxbucket + 1);
	if (spare_ndx > metap->hashm_ovflpoint)
	{

		metap = (HashMetaPage) _hash_chgbufaccess(rel, &metabuf, HASH_READ, HASH_WRITE);
		metap->hashm_spares[spare_ndx] = metap->hashm_spares[metap->hashm_ovflpoint];
		metap->hashm_ovflpoint = spare_ndx;
		metap = (HashMetaPage) _hash_chgbufaccess(rel, &metabuf, HASH_WRITE, HASH_READ);
	}

	if (new_bucket > metap->hashm_highmask)
	{

		/* Starting a new doubling */
		metap = (HashMetaPage) _hash_chgbufaccess(rel, &metabuf, HASH_READ, HASH_WRITE);
		metap->hashm_lowmask = metap->hashm_highmask;
		metap->hashm_highmask = new_bucket | metap->hashm_lowmask;
		metap = (HashMetaPage) _hash_chgbufaccess(rel, &metabuf, HASH_WRITE, HASH_READ);

	}
	/* Relocate records to the new bucket */
	_hash_splitpage(rel, metabuf, old_bucket, new_bucket);
}


/*
 * _hash_splitpage -- split 'obucket' into 'obucket' and 'nbucket'
 *
 * this routine is actually misnamed -- we are splitting a bucket that
 * consists of a base bucket page and zero or more overflow (bucket
 * chain) pages.
 */
static void
_hash_splitpage(Relation rel,
				Buffer metabuf,
				Bucket obucket,
				Bucket nbucket)
{
	Bucket		bucket;
	Buffer		obuf;
	Buffer		nbuf;
	Buffer		ovflbuf;
	BlockNumber oblkno;
	BlockNumber nblkno;
	bool		null;
	Datum		datum;
	HashItem	hitem;
	HashPageOpaque oopaque;
	HashPageOpaque nopaque;
	HashMetaPage metap;
	IndexTuple	itup;
	Size		itemsz;
	OffsetNumber ooffnum;
	OffsetNumber noffnum;
	OffsetNumber omaxoffnum;
	Page		opage;
	Page		npage;
	TupleDesc	itupdesc;

/*	  elog(DEBUG, "_hash_splitpage: splitting %d into %d,%d",
		 obucket, obucket, nbucket);
*/
	metap = (HashMetaPage) BufferGetPage(metabuf);
	_hash_checkpage((Page) metap, LH_META_PAGE);

	/* get the buffers & pages */
	oblkno = BUCKET_TO_BLKNO(obucket);
	nblkno = BUCKET_TO_BLKNO(nbucket);
	obuf = _hash_getbuf(rel, oblkno, HASH_WRITE);
	nbuf = _hash_getbuf(rel, nblkno, HASH_WRITE);
	opage = BufferGetPage(obuf);
	npage = BufferGetPage(nbuf);

	/* initialize the new bucket */
	_hash_pageinit(npage, BufferGetPageSize(nbuf));
	nopaque = (HashPageOpaque) PageGetSpecialPointer(npage);
	nopaque->hasho_prevblkno = InvalidBlockNumber;
	nopaque->hasho_nextblkno = InvalidBlockNumber;
	nopaque->hasho_flag = LH_BUCKET_PAGE;
	nopaque->hasho_oaddr = InvalidOvflAddress;
	nopaque->hasho_bucket = nbucket;
	_hash_wrtnorelbuf(rel, nbuf);

	/*
	 * make sure the old bucket isn't empty.  advance 'opage' and friends
	 * through the overflow bucket chain until we find a non-empty page.
	 *
	 * XXX we should only need this once, if we are careful to preserve the
	 * invariant that overflow pages are never empty.
	 */
	_hash_checkpage(opage, LH_BUCKET_PAGE);
	oopaque = (HashPageOpaque) PageGetSpecialPointer(opage);
	if (PageIsEmpty(opage))
	{
		oblkno = oopaque->hasho_nextblkno;
		_hash_relbuf(rel, obuf, HASH_WRITE);
		if (!BlockNumberIsValid(oblkno))
		{
			/*
			 * the old bucket is completely empty; of course, the new
			 * bucket will be as well, but since it's a base bucket page
			 * we don't care.
			 */
			_hash_relbuf(rel, nbuf, HASH_WRITE);
			return;
		}
		obuf = _hash_getbuf(rel, oblkno, HASH_WRITE);
		opage = BufferGetPage(obuf);
		_hash_checkpage(opage, LH_OVERFLOW_PAGE);
		if (PageIsEmpty(opage))
			elog(ERROR, "_hash_splitpage: empty overflow page %d", oblkno);
		oopaque = (HashPageOpaque) PageGetSpecialPointer(opage);
	}

	/*
	 * we are now guaranteed that 'opage' is not empty.  partition the
	 * tuples in the old bucket between the old bucket and the new bucket,
	 * advancing along their respective overflow bucket chains and adding
	 * overflow pages as needed.
	 */
	ooffnum = FirstOffsetNumber;
	omaxoffnum = PageGetMaxOffsetNumber(opage);
	for (;;)
	{
		/*
		 * at each iteration through this loop, each of these variables
		 * should be up-to-date: obuf opage oopaque ooffnum omaxoffnum
		 */

		/* check if we're at the end of the page */
		if (ooffnum > omaxoffnum)
		{
			/* at end of page, but check for overflow page */
			oblkno = oopaque->hasho_nextblkno;
			if (BlockNumberIsValid(oblkno))
			{
				/*
				 * we ran out of tuples on this particular page, but we
				 * have more overflow pages; re-init values.
				 */
				_hash_wrtbuf(rel, obuf);
				obuf = _hash_getbuf(rel, oblkno, HASH_WRITE);
				opage = BufferGetPage(obuf);
				_hash_checkpage(opage, LH_OVERFLOW_PAGE);
				oopaque = (HashPageOpaque) PageGetSpecialPointer(opage);

				/* we're guaranteed that an ovfl page has at least 1 tuple */
				if (PageIsEmpty(opage))
				{
					elog(ERROR, "_hash_splitpage: empty ovfl page %d!",
						 oblkno);
				}
				ooffnum = FirstOffsetNumber;
				omaxoffnum = PageGetMaxOffsetNumber(opage);
			}
			else
			{
				/*
				 * we're at the end of the bucket chain, so now we're
				 * really done with everything.  before quitting, call
				 * _hash_squeezebucket to ensure the tuples in the bucket
				 * (including the overflow pages) are packed as tightly as
				 * possible.
				 */
				_hash_wrtbuf(rel, obuf);
				_hash_wrtbuf(rel, nbuf);
				_hash_squeezebucket(rel, metap, obucket);
				return;
			}
		}

		/* hash on the tuple */
		hitem = (HashItem) PageGetItem(opage, PageGetItemId(opage, ooffnum));
		itup = &(hitem->hash_itup);
		itupdesc = RelationGetDescr(rel);
		datum = index_getattr(itup, 1, itupdesc, &null);
		bucket = _hash_call(rel, metap, datum);

		if (bucket == nbucket)
		{
			/*
			 * insert the tuple into the new bucket.  if it doesn't fit on
			 * the current page in the new bucket, we must allocate a new
			 * overflow page and place the tuple on that page instead.
			 */
			itemsz = IndexTupleDSize(hitem->hash_itup)
				+ (sizeof(HashItemData) - sizeof(IndexTupleData));

			itemsz = MAXALIGN(itemsz);

			if (PageGetFreeSpace(npage) < itemsz)
			{
				ovflbuf = _hash_addovflpage(rel, &metabuf, nbuf);
				_hash_wrtbuf(rel, nbuf);
				nbuf = ovflbuf;
				npage = BufferGetPage(nbuf);
				_hash_checkpage(npage, LH_BUCKET_PAGE | LH_OVERFLOW_PAGE);
			}

			noffnum = OffsetNumberNext(PageGetMaxOffsetNumber(npage));
			if (PageAddItem(npage, (Item) hitem, itemsz, noffnum, LP_USED)
				== InvalidOffsetNumber)
				elog(ERROR, "_hash_splitpage: failed to add index item to %s",
					 RelationGetRelationName(rel));
			_hash_wrtnorelbuf(rel, nbuf);

			/*
			 * now delete the tuple from the old bucket.  after this
			 * section of code, 'ooffnum' will actually point to the
			 * ItemId to which we would point if we had advanced it before
			 * the deletion (PageIndexTupleDelete repacks the ItemId
			 * array).	this also means that 'omaxoffnum' is exactly one
			 * less than it used to be, so we really can just decrement it
			 * instead of calling PageGetMaxOffsetNumber.
			 */
			PageIndexTupleDelete(opage, ooffnum);
			_hash_wrtnorelbuf(rel, obuf);
			omaxoffnum = OffsetNumberPrev(omaxoffnum);

			/*
			 * tidy up.  if the old page was an overflow page and it is
			 * now empty, we must free it (we want to preserve the
			 * invariant that overflow pages cannot be empty).
			 */
			if (PageIsEmpty(opage) &&
				(oopaque->hasho_flag & LH_OVERFLOW_PAGE))
			{
				obuf = _hash_freeovflpage(rel, obuf);

				/* check that we're not through the bucket chain */
				if (BufferIsInvalid(obuf))
				{
					_hash_wrtbuf(rel, nbuf);
					_hash_squeezebucket(rel, metap, obucket);
					return;
				}

				/*
				 * re-init. again, we're guaranteed that an ovfl page has
				 * at least one tuple.
				 */
				opage = BufferGetPage(obuf);
				_hash_checkpage(opage, LH_OVERFLOW_PAGE);
				oblkno = BufferGetBlockNumber(obuf);
				oopaque = (HashPageOpaque) PageGetSpecialPointer(opage);
				if (PageIsEmpty(opage))
				{
					elog(ERROR, "_hash_splitpage: empty overflow page %d",
						 oblkno);
				}
				ooffnum = FirstOffsetNumber;
				omaxoffnum = PageGetMaxOffsetNumber(opage);
			}
		}
		else
		{
			/*
			 * the tuple stays on this page.  we didn't move anything, so
			 * we didn't delete anything and therefore we don't have to
			 * change 'omaxoffnum'.
			 *
			 * XXX any hash value from [0, nbucket-1] will map to this
			 * bucket, which doesn't make sense to me.
			 */
			ooffnum = OffsetNumberNext(ooffnum);
		}
	}
	/* NOTREACHED */
}
