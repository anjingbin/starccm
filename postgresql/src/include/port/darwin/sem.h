/*-------------------------------------------------------------------------
 *
 * sem.h
 *	  System V Semaphore Emulation
 *
 * Copyright (c) 1999, repas AEG Automation GmbH
 *
 *
 * IDENTIFICATION
 *		 $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/include/port/darwin/sem.h,v 1.2 2004/07/05 08:56:04 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef _SYS_SEM_H
#define _SYS_SEM_H

/* #define DEBUG_IPC here if you want to see the shim in action */

#include <sys/ipc.h>

#ifdef __cplusplus
extern		"C"
{
#endif

/*
 *	Semctl Command Definitions.
 */

#define GETNCNT 3				/* get semncnt */
#define GETPID	4				/* get sempid */
#define GETVAL	5				/* get semval */
#define GETALL	6				/* get all semval's */
#define GETZCNT 7				/* get semzcnt */
#define SETVAL	8				/* set semval */
#define SETALL	9				/* set all semval's */

#ifndef ushort_t
#define ushort_t unsigned int
#endif

/*
 *	There is one semaphore structure for each semaphore in the system.
 */

struct sem
{
	ushort_t	semval;			/* semaphore text map address	*/
	pid_t		sempid;			/* pid of last operation	*/
	ushort_t	semncnt;		/* # awaiting semval > cval */
	ushort_t	semzcnt;		/* # awaiting semval = 0	*/
};

/*
 * User semaphore template for semop system calls.
 */

struct sembuf
{
	ushort_t	sem_num;		/* semaphore #			*/
	short		sem_op;			/* semaphore operation		*/
	short		sem_flg;		/* operation flags		*/
};

extern int	semctl(int semid, int semnum, int cmd, /* ... */ union semun arg);
extern int	semget(key_t key, int nsems, int semflg);
extern int	semop(int semid, struct sembuf * sops, size_t nsops);

#ifdef __cplusplus
}
#endif

#endif   /* _SYS_SEM_H */
