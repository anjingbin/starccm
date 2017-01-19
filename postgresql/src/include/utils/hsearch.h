/*-------------------------------------------------------------------------
 *
 * hsearch.h
 *	  for hash tables, particularly hash tables in shared memory
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: hsearch.h,v 1.2 2004/07/05 08:56:08 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef HSEARCH_H
#define HSEARCH_H


/*
 * Constants
 *
 * A hash table has a top-level "directory", each of whose entries points
 * to a "segment" of ssize bucket headers.	The maximum number of hash
 * buckets is thus dsize * ssize (but dsize may be expansible).  Of course,
 * the number of records in the table can be larger, but we don't want a
 * whole lot of records per bucket or performance goes down.
 *
 * In a hash table allocated in shared memory, the directory cannot be
 * expanded because it must stay at a fixed address.  The directory size
 * should be selected using hash_select_dirsize (and you'd better have
 * a good idea of the maximum number of entries!).	For non-shared hash
 * tables, the initial directory size can be left at the default.
 */
#define DEF_SEGSIZE			   256
#define DEF_SEGSIZE_SHIFT	   8	/* must be log2(DEF_SEGSIZE) */
#define DEF_DIRSIZE			   256
#define DEF_FFACTOR			   1	/* default fill factor */

#define PRIME1				   37		/* for the hash function */
#define PRIME2				   1048583


/*
 * HASHELEMENT is the private part of a hashtable entry.  The caller's data
 * follows the HASHELEMENT structure (on a MAXALIGN'd boundary).  The hash key
 * is expected to be at the start of the caller's hash entry data structure.
 */
typedef struct HASHELEMENT
{
	struct HASHELEMENT *link;	/* link to next entry in same bucket */
} HASHELEMENT;

/* A hash bucket is a linked list of HASHELEMENTs */
typedef HASHELEMENT *HASHBUCKET;

/* A hash segment is an array of bucket headers */
typedef HASHBUCKET *HASHSEGMENT;

/* Header structure for a hash table --- contains all changeable info */
typedef struct HASHHDR
{
	long		dsize;			/* Directory Size */
	long		ssize;			/* Segment Size --- must be power of 2 */
	long		sshift;			/* Segment shift */
	long		max_bucket;		/* ID of Maximum bucket in use */
	long		high_mask;		/* Mask to modulo into entire table */
	long		low_mask;		/* Mask to modulo into lower half of table */
	long		ffactor;		/* Fill factor */
	long		nentries;		/* Number of entries in hash table */
	long		nsegs;			/* Number of allocated segments */
	long		keysize;		/* hash key length in bytes */
	long		entrysize;		/* total user element size in bytes */
	long		max_dsize;		/* 'dsize' limit if directory is fixed
								 * size */
	HASHELEMENT *freeList;		/* linked list of free elements */
#ifdef HASH_STATISTICS
	long		accesses;
	long		collisions;
#endif
} HASHHDR;

/*
 * Top control structure for a hashtable --- need not be shared, since
 * no fields change at runtime
 */
typedef struct HTAB
{
	HASHHDR    *hctl;			/* shared control information */
	HASHSEGMENT *dir;			/* directory of segment starts */
	long		(*hash) (void *key, int keysize);		/* Hash Function */
	void	   *(*alloc) (Size);	/* memory allocator */
	MemoryContext hcxt;			/* memory context if default allocator
								 * used */
	char	   *tabname;		/* table name (for error messages) */
	bool		isshared;		/* true if table is in shared memory */
} HTAB;

/* Parameter data structure for hash_create */
/* Only those fields indicated by hash_flags need be set */
typedef struct HASHCTL
{
	long		ssize;			/* Segment Size */
	long		dsize;			/* (initial) Directory Size */
	long		ffactor;		/* Fill factor */
	long		(*hash) (void *key, int keysize);		/* Hash Function */
	long		keysize;		/* hash key length in bytes */
	long		entrysize;		/* total user element size in bytes */
	long		max_dsize;		/* limit to dsize if directory size is
								 * limited */
	void	   *(*alloc) (Size);	/* memory allocation function */
	HASHSEGMENT *dir;			/* directory of segment starts */
	HASHHDR    *hctl;			/* location of header in shared mem */
	MemoryContext hcxt;			/* memory context to use for allocations */
} HASHCTL;

/* Flags to indicate which parameters are supplied */
#define HASH_SEGMENT	0x002	/* Setting segment size */
#define HASH_DIRSIZE	0x004	/* Setting directory size */
#define HASH_FFACTOR	0x008	/* Setting fill factor */
#define HASH_FUNCTION	0x010	/* Set user defined hash function */
#define HASH_ELEM		0x020	/* Setting key/entry size */
#define HASH_SHARED_MEM 0x040	/* Setting shared mem const */
#define HASH_ATTACH		0x080	/* Do not initialize hctl */
#define HASH_ALLOC		0x100	/* Setting memory allocator */
#define HASH_CONTEXT	0x200	/* Setting explicit memory context */


/* max_dsize value to indicate expansible directory */
#define NO_MAX_DSIZE			(-1)
/* number of hash elements allocated at once */
#define HASHELEMENT_ALLOC_INCR	(32)

/* hash_search operations */
typedef enum
{
	HASH_FIND,
	HASH_ENTER,
	HASH_REMOVE,
	HASH_FIND_SAVE,
	HASH_REMOVE_SAVED
} HASHACTION;

/* hash_seq status (should be considered an opaque type by callers) */
typedef struct
{
	HTAB	   *hashp;
	long		curBucket;		/* index of current bucket */
	HASHELEMENT *curEntry;		/* current entry in bucket */
} HASH_SEQ_STATUS;

/*
 * prototypes for functions in dynahash.c
 */
extern HTAB *hash_create(const char *tabname, long nelem,
			HASHCTL *info, int flags);
extern void hash_destroy(HTAB *hashp);
extern void hash_stats(const char *where, HTAB *hashp);
extern void *hash_search(HTAB *hashp, void *keyPtr, HASHACTION action,
			bool *foundPtr);
extern void hash_seq_init(HASH_SEQ_STATUS *status, HTAB *hashp);
extern void *hash_seq_search(HASH_SEQ_STATUS *status);
extern long hash_estimate_size(long num_entries, long entrysize);
extern long hash_select_dirsize(long num_entries);

/*
 * prototypes for functions in hashfn.c
 */
extern long string_hash(void *key, int keysize);
extern long tag_hash(void *key, int keysize);

#endif   /* HSEARCH_H */
