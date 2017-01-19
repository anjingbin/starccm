/*-------------------------------------------------------------------------
 *
 * shm.h
 *	  System V Shared Memory Emulation
 *
 * Copyright (c) 1999, repas AEG Automation GmbH
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/qnx4/shm.h,v 1.2 2004/07/05 09:13:24 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef _SYS_SHM_H
#define _SYS_SHM_H

#include <sys/ipc.h>

#ifdef	__cplusplus
extern		"C"
{
#endif

#define SHM_R	0400			/* read permission */
#define SHM_W	0200			/* write permission */

struct shmid_ds
{
	int			dummy;
	int			shm_nattch;
};

extern void *shmat(int shmid, const void *shmaddr, int shmflg);
extern int	shmdt(const void *addr);
extern int	shmctl(int shmid, int cmd, struct shmid_ds * buf);
extern int	shmget(key_t key, size_t size, int flags);

#ifdef	__cplusplus
}
#endif

#endif   /* _SYS_SHM_H */
