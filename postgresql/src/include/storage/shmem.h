/*-------------------------------------------------------------------------
 *
 * shmem.h
 *	  shared memory management structures
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: shmem.h,v 1.2 2004/07/05 08:56:08 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef SHMEM_H
#define SHMEM_H

#include "utils/hsearch.h"


/*
 * The shared memory region can start at a different address
 * in every process.  Shared memory "pointers" are actually
 * offsets relative to the start of the shared memory region(s).
 *
 * In current usage, this is not actually a problem, but we keep
 * the code that used to handle it...
 */
typedef unsigned long SHMEM_OFFSET;

#define INVALID_OFFSET	(-1)
#define BAD_LOCATION	(-1)

/*
 * Start of the primary shared memory region, in this process' address space.
 * The macros in this header file can only cope with offsets into this
 * shared memory region!
 */
extern DLLIMPORT SHMEM_OFFSET ShmemBase;


/* coerce an offset into a pointer in this process's address space */
#define MAKE_PTR(xx_offs)\
  (ShmemBase+((unsigned long)(xx_offs)))

/* coerce a pointer into a shmem offset */
#define MAKE_OFFSET(xx_ptr)\
  ((SHMEM_OFFSET) (((unsigned long)(xx_ptr))-ShmemBase))

#define SHM_PTR_VALID(xx_ptr)\
  (((unsigned long)(xx_ptr)) > ShmemBase)

/* cannot have an offset to ShmemFreeStart (offset 0) */
#define SHM_OFFSET_VALID(xx_offs)\
  (((xx_offs) != 0) && ((xx_offs) != INVALID_OFFSET))


/* shmemqueue.c */
typedef struct SHM_QUEUE
{
	SHMEM_OFFSET prev;
	SHMEM_OFFSET next;
} SHM_QUEUE;

/* shmem.c */
extern void InitShmemAllocation(void *seghdr);
extern void *ShmemAlloc(Size size);
extern bool ShmemIsValid(unsigned long addr);
extern void InitShmemIndex(void);
extern HTAB *ShmemInitHash(const char *name, long init_size, long max_size,
			  HASHCTL *infoP, int hash_flags);
extern void *ShmemInitStruct(const char *name, Size size, bool *foundPtr);


/* size constants for the shmem index table */
 /* max size of data structure string name */
#define SHMEM_INDEX_KEYSIZE		 (48)
 /* maximum size of the shmem index table */
#define SHMEM_INDEX_SIZE		 (100)

/* this is a hash bucket in the shmem index table */
typedef struct
{
	char		key[SHMEM_INDEX_KEYSIZE];		/* string name */
	unsigned long location;		/* location in shared mem */
	unsigned long size;			/* numbytes allocated for the structure */
} ShmemIndexEnt;

/*
 * prototypes for functions in shmqueue.c
 */
extern void SHMQueueInit(SHM_QUEUE *queue);
extern void SHMQueueElemInit(SHM_QUEUE *queue);
extern void SHMQueueDelete(SHM_QUEUE *queue);
extern void SHMQueueInsertBefore(SHM_QUEUE *queue, SHM_QUEUE *elem);
extern Pointer SHMQueueNext(SHM_QUEUE *queue, SHM_QUEUE *curElem,
			 Size linkOffset);
extern bool SHMQueueEmpty(SHM_QUEUE *queue);

#endif   /* SHMEM_H */
