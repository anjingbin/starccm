/*-------------------------------------------------------------------------
 *
 * bufpage.c
 *	  POSTGRES standard buffer page code.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/storage/page/bufpage.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <sys/types.h>
#include <sys/file.h>

#include "storage/bufpage.h"


/* ----------------------------------------------------------------
 *						Page support functions
 * ----------------------------------------------------------------
 */

/*
 * PageInit
 *		Initializes the contents of a page.
 */
void
PageInit(Page page, Size pageSize, Size specialSize)
{
	PageHeader	p = (PageHeader) page;

	specialSize = MAXALIGN(specialSize);

	Assert(pageSize == BLCKSZ);
	Assert(pageSize >
		   specialSize + sizeof(PageHeaderData) - sizeof(ItemIdData));

	/* Make sure all fields of page are zero, as well as unused space */
	MemSet(p, 0, pageSize);

	p->pd_lower = sizeof(PageHeaderData) - sizeof(ItemIdData);
	p->pd_upper = pageSize - specialSize;
	p->pd_special = pageSize - specialSize;
	PageSetPageSize(page, pageSize);
}


/* ----------------
 *		PageAddItem
 *
 *		Add an item to a page.	Return value is offset at which it was
 *		inserted, or InvalidOffsetNumber if there's not room to insert.
 *
 *		If offsetNumber is valid and <= current max offset in the page,
 *		insert item into the array at that position by shuffling ItemId's
 *		down to make room.
 *		If offsetNumber is not valid, then assign one by finding the first
 *		one that is both unused and deallocated.
 *
 *	 !!! ELOG(ERROR) IS DISALLOWED HERE !!!
 *
 * ----------------
 */
OffsetNumber
PageAddItem(Page page,
			Item item,
			Size size,
			OffsetNumber offsetNumber,
			ItemIdFlags flags)
{
	PageHeader	phdr = (PageHeader) page;
	int			i;
	Size		alignedSize;
	int			lower;
	int			upper;
	ItemId		itemId;
	OffsetNumber limit;
	bool		needshuffle = false;
	bool		overwritemode = (flags & OverwritePageMode) != 0;

	flags &= ~OverwritePageMode;

	/*
	 * Be wary about corrupted page pointers
	 */
	if (phdr->pd_lower < (sizeof(PageHeaderData) - sizeof(ItemIdData)) ||
		phdr->pd_lower > phdr->pd_upper ||
		phdr->pd_upper > phdr->pd_special ||
		phdr->pd_special > BLCKSZ)
		elog(ERROR, "PageAddItem: corrupted page pointers: lower = %u, upper = %u, special = %u",
			 phdr->pd_lower, phdr->pd_upper, phdr->pd_special);

	/*
	 * Find first unallocated offsetNumber
	 */
	limit = OffsetNumberNext(PageGetMaxOffsetNumber(page));

	/* was offsetNumber passed in? */
	if (OffsetNumberIsValid(offsetNumber))
	{
		if (overwritemode)
		{
			if (offsetNumber > limit)
			{
				elog(NOTICE, "PageAddItem: tried overwrite after maxoff");
				return InvalidOffsetNumber;
			}
			if (offsetNumber < limit)
			{
				itemId = &phdr->pd_linp[offsetNumber - 1];
				if (((*itemId).lp_flags & LP_USED) ||
					((*itemId).lp_len != 0))
				{
					elog(NOTICE, "PageAddItem: tried overwrite of used ItemId");
					return InvalidOffsetNumber;
				}
			}
		}
		else
		{
			/*
			 * Don't actually do the shuffle till we've checked free
			 * space!
			 */
			needshuffle = true; /* need to increase "lower" */
		}
	}
	else
	{
		/* offsetNumber was not passed in, so find one */
		/* look for "recyclable" (unused & deallocated) ItemId */
		for (offsetNumber = 1; offsetNumber < limit; offsetNumber++)
		{
			itemId = &phdr->pd_linp[offsetNumber - 1];
			if ((((*itemId).lp_flags & LP_USED) == 0) &&
				((*itemId).lp_len == 0))
				break;
		}
	}

	/*
	 * Compute new lower and upper pointers for page, see if it'll fit.
	 *
	 * Note: do arithmetic as signed ints, to avoid mistakes if, say,
	 * alignedSize > pd_upper.
	 */
	if (offsetNumber > limit)
		lower = (char *) (&phdr->pd_linp[offsetNumber]) - (char *) page;
	else if (offsetNumber == limit || needshuffle)
		lower = phdr->pd_lower + sizeof(ItemIdData);
	else
		lower = phdr->pd_lower;

	alignedSize = MAXALIGN(size);

	upper = (int) phdr->pd_upper - (int) alignedSize;

	if (lower > upper)
		return InvalidOffsetNumber;

	/*
	 * OK to insert the item.  First, shuffle the existing pointers if
	 * needed.
	 */
	if (needshuffle)
	{
		/* shuffle ItemId's (Do the PageManager Shuffle...) */
		for (i = (int) limit - 1; i >= (int) offsetNumber; i--)
		{
			ItemId		fromitemId,
						toitemId;

			fromitemId = &phdr->pd_linp[i - 1];
			toitemId = &phdr->pd_linp[i];
			*toitemId = *fromitemId;
		}
	}

	itemId = &phdr->pd_linp[offsetNumber - 1];
	(*itemId).lp_off = upper;
	(*itemId).lp_len = size;
	(*itemId).lp_flags = flags;

	memmove((char *) page + upper, item, size);

	phdr->pd_lower = (LocationIndex) lower;
	phdr->pd_upper = (LocationIndex) upper;

	return offsetNumber;
}

/*
 * PageGetTempPage
 *		Get a temporary page in local memory for special processing
 */
Page
PageGetTempPage(Page page, Size specialSize)
{
	Size		pageSize;
	Size		size;
	Page		temp;
	PageHeader	thdr;

	pageSize = PageGetPageSize(page);
	temp = (Page) palloc(pageSize);
	thdr = (PageHeader) temp;

	/* copy old page in */
	memcpy(temp, page, pageSize);

	/* clear out the middle */
	size = (pageSize - sizeof(PageHeaderData)) + sizeof(ItemIdData);
	size -= MAXALIGN(specialSize);
	MemSet((char *) &(thdr->pd_linp[0]), 0, size);

	/* set high, low water marks */
	thdr->pd_lower = sizeof(PageHeaderData) - sizeof(ItemIdData);
	thdr->pd_upper = pageSize - MAXALIGN(specialSize);

	return temp;
}

/*
 * PageRestoreTempPage
 *		Copy temporary page back to permanent page after special processing
 *		and release the temporary page.
 */
void
PageRestoreTempPage(Page tempPage, Page oldPage)
{
	Size		pageSize;

	pageSize = PageGetPageSize(tempPage);
	memmove((char *) oldPage, (char *) tempPage, pageSize);

	pfree(tempPage);
}

/*
 * sorting support for PageRepairFragmentation
 */
struct itemIdSortData
{
	int			offsetindex;	/* linp array index */
	int			itemoff;		/* page offset of item data */
	Size		alignedlen;		/* MAXALIGN(item data len) */
};

static int
itemoffcompare(const void *itemidp1, const void *itemidp2)
{
	/* Sort in decreasing itemoff order */
	return ((struct itemIdSortData *) itemidp2)->itemoff -
		((struct itemIdSortData *) itemidp1)->itemoff;
}

/*
 * PageRepairFragmentation
 *
 * Frees fragmented space on a page.
 * It doesn't remove unused line pointers! Please don't change this.
 * This routine is usable for heap pages only.
 *
 * Returns number of unused line pointers on page.	If "unused" is not NULL
 * then the unused[] array is filled with indexes of unused line pointers.
 */
int
PageRepairFragmentation(Page page, OffsetNumber *unused)
{
	Offset		pd_lower = ((PageHeader) page)->pd_lower;
	Offset		pd_upper = ((PageHeader) page)->pd_upper;
	Offset		pd_special = ((PageHeader) page)->pd_special;
	struct itemIdSortData *itemidbase,
			   *itemidptr;
	ItemId		lp;
	int			nline,
				nused;
	int			i;
	Size		totallen;
	Offset		upper;

	/*
	 * It's worth the trouble to be more paranoid here than in most
	 * places, because we are about to reshuffle data in (what is usually)
	 * a shared disk buffer.  If we aren't careful then corrupted
	 * pointers, lengths, etc could cause us to clobber adjacent disk
	 * buffers, spreading the data loss further.  So, check everything.
	 */
	if (pd_lower < (sizeof(PageHeaderData) - sizeof(ItemIdData)) ||
		pd_lower > pd_upper ||
		pd_upper > pd_special ||
		pd_special > BLCKSZ ||
		pd_special != MAXALIGN(pd_special))
		elog(ERROR, "PageRepairFragmentation: corrupted page pointers: lower = %u, upper = %u, special = %u",
			 pd_lower, pd_upper, pd_special);

	nline = PageGetMaxOffsetNumber(page);
	nused = 0;
	for (i = 0; i < nline; i++)
	{
		lp = ((PageHeader) page)->pd_linp + i;
		if ((*lp).lp_flags & LP_DELETE) /* marked for deletion */
			(*lp).lp_flags &= ~(LP_USED | LP_DELETE);
		if ((*lp).lp_flags & LP_USED)
			nused++;
		else if (unused)
			unused[i - nused] = (OffsetNumber) i;
	}

	if (nused == 0)
	{
		/* Page is completely empty, so just reset it quickly */
		for (i = 0; i < nline; i++)
		{
			lp = ((PageHeader) page)->pd_linp + i;
			(*lp).lp_len = 0;	/* indicate unused & deallocated */
		}
		((PageHeader) page)->pd_upper = pd_special;
	}
	else
	{							/* nused != 0 */
		/* Need to compact the page the hard way */
		itemidbase = (struct itemIdSortData *)
			palloc(sizeof(struct itemIdSortData) * nused);
		itemidptr = itemidbase;
		totallen = 0;
		for (i = 0; i < nline; i++)
		{
			lp = ((PageHeader) page)->pd_linp + i;
			if ((*lp).lp_flags & LP_USED)
			{
				itemidptr->offsetindex = i;
				itemidptr->itemoff = (*lp).lp_off;
				if (itemidptr->itemoff < (int) pd_upper ||
					itemidptr->itemoff >= (int) pd_special)
					elog(ERROR, "PageRepairFragmentation: corrupted item pointer %u",
						 itemidptr->itemoff);
				itemidptr->alignedlen = MAXALIGN((*lp).lp_len);
				totallen += itemidptr->alignedlen;
				itemidptr++;
			}
			else
			{
				(*lp).lp_len = 0;		/* indicate unused & deallocated */
			}
		}

		if (totallen > (Size) (pd_special - pd_lower))
			elog(ERROR, "PageRepairFragmentation: corrupted item lengths, total %u, avail %u",
				 (unsigned int) totallen, pd_special - pd_lower);

		/* sort itemIdSortData array into decreasing itemoff order */
		qsort((char *) itemidbase, nused, sizeof(struct itemIdSortData),
			  itemoffcompare);

		/* compactify page */
		upper = pd_special;

		for (i = 0, itemidptr = itemidbase; i < nused; i++, itemidptr++)
		{
			lp = ((PageHeader) page)->pd_linp + itemidptr->offsetindex;
			upper -= itemidptr->alignedlen;
			memmove((char *) page + upper,
					(char *) page + itemidptr->itemoff,
					itemidptr->alignedlen);
			(*lp).lp_off = upper;
		}

		((PageHeader) page)->pd_upper = upper;

		pfree(itemidbase);
	}

	return (nline - nused);
}

/*
 * PageGetFreeSpace
 *		Returns the size of the free (allocatable) space on a page.
 */
Size
PageGetFreeSpace(Page page)
{
	int			space;

	/*
	 * Use signed arithmetic here so that we behave sensibly if
	 * pd_lower > pd_upper.
	 */
	space = (int) ((PageHeader) page)->pd_upper -
		(int) ((PageHeader) page)->pd_lower;

	if (space < (int) sizeof(ItemIdData))
		return 0;
	space -= sizeof(ItemIdData);	/* XXX not always appropriate */

	return (Size) space;
}


/*
 * PageIndexTupleDelete
 *
 * This routine does the work of removing a tuple from an index page.
 *
 * Unlike heap pages, we compact out the line pointer for the removed tuple.
 */
void
PageIndexTupleDelete(Page page, OffsetNumber offnum)
{
	PageHeader	phdr = (PageHeader) page;
	char	   *addr;
	ItemId		tup;
	Size		size;
	unsigned	offset;
	int			nbytes;
	int			offidx;
	int			nline,
				i;

	/*
	 * As with PageRepairFragmentation, paranoia seems justified.
	 */
	if (phdr->pd_lower < (sizeof(PageHeaderData) - sizeof(ItemIdData)) ||
		phdr->pd_lower > phdr->pd_upper ||
		phdr->pd_upper > phdr->pd_special ||
		phdr->pd_special > BLCKSZ)
		elog(ERROR, "PageIndexTupleDelete: corrupted page pointers: lower = %u, upper = %u, special = %u",
			 phdr->pd_lower, phdr->pd_upper, phdr->pd_special);

	nline = PageGetMaxOffsetNumber(page);
	if ((int) offnum <= 0 || (int) offnum > nline)
		elog(ERROR, "PageIndexTupleDelete: bad offnum %u", offnum);

	/* change offset number to offset index */
	offidx = offnum - 1;

	tup = PageGetItemId(page, offnum);
	size = ItemIdGetLength(tup);
	offset = ItemIdGetOffset(tup);

	if (offset < phdr->pd_upper || (offset + size) > phdr->pd_special ||
		offset != MAXALIGN(offset) || size != MAXALIGN(size))
		elog(ERROR, "PageIndexTupleDelete: corrupted item pointer: offset = %u size = %u",
			 offset, (unsigned int) size);

	/*
	 * First, we want to get rid of the pd_linp entry for the index tuple.
	 * We copy all subsequent linp's back one slot in the array.
	 */
	nbytes = phdr->pd_lower -
		((char *) &phdr->pd_linp[offidx + 1] - (char *) phdr);

	memmove((char *) &(phdr->pd_linp[offidx]),
			(char *) &(phdr->pd_linp[offidx + 1]),
			nbytes);

	/*
	 * Now move everything between the old upper bound (beginning of tuple
	 * space) and the beginning of the deleted tuple forward, so that
	 * space in the middle of the page is left free.  If we've just
	 * deleted the tuple at the beginning of tuple space, then there's no
	 * need to do the copy (and bcopy on some architectures SEGV's if
	 * asked to move zero bytes).
	 */

	/* beginning of tuple space */
	addr = (char *) page + phdr->pd_upper;

	if (offset > phdr->pd_upper)
		memmove(addr + size, addr, (int) (offset - phdr->pd_upper));

	/* adjust free space boundary pointers */
	phdr->pd_upper += size;
	phdr->pd_lower -= sizeof(ItemIdData);

	/*
	 * Finally, we need to adjust the linp entries that remain.
	 *
	 * Anything that used to be before the deleted tuple's data was moved
	 * forward by the size of the deleted tuple.
	 */
	if (!PageIsEmpty(page))
	{
		nline--;				/* there's one less than when we started */
		for (i = nline; --i >= 0; )
		{
			if (phdr->pd_linp[i].lp_off <= offset)
				phdr->pd_linp[i].lp_off += size;
		}
	}
}
