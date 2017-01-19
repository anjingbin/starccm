/*
 * user_locks.c --
 *
 * This loadable module provides support for user-level long-term
 * cooperative locks.
 *
 * Copyright (C) 1999, Massimo Dal Zotto <dz@cs.unitn.it>
 *
 * This software is distributed under the GNU General Public License
 * either version 2, or (at your option) any later version.
 */
#include "postgres.h"

#include "miscadmin.h"
#include "storage/lmgr.h"
#include "storage/proc.h"

#include "user_locks.h"


int
user_lock(uint32 id1, uint32 id2, LOCKMODE lockmode)
{
	LOCKTAG		tag;

	memset(&tag, 0, sizeof(LOCKTAG));
	tag.dbId = MyDatabaseId;
	tag.relId = 0;
	tag.objId.blkno = (BlockNumber) id2;
	tag.offnum = (OffsetNumber) (id1 & 0xffff);

	return LockAcquire(USER_LOCKMETHOD, &tag, InvalidTransactionId,
					   lockmode, true);
}

int
user_unlock(uint32 id1, uint32 id2, LOCKMODE lockmode)
{
	LOCKTAG		tag;

	memset(&tag, 0, sizeof(LOCKTAG));
	tag.dbId = MyDatabaseId;
	tag.relId = 0;
	tag.objId.blkno = (BlockNumber) id2;
	tag.offnum = (OffsetNumber) (id1 & 0xffff);

	return LockRelease(USER_LOCKMETHOD, &tag, InvalidTransactionId, lockmode);
}

int
user_write_lock(uint32 id1, uint32 id2)
{
	return user_lock(id1, id2, ExclusiveLock);
}


int
user_write_unlock(uint32 id1, uint32 id2)
{
	return user_unlock(id1, id2, ExclusiveLock);
}

int
user_write_lock_oid(Oid oid)
{
	return user_lock(0, oid, ExclusiveLock);
}

int
user_write_unlock_oid(Oid oid)
{
	return user_unlock(0, oid, ExclusiveLock);
}

int
user_unlock_all(void)
{
	return LockReleaseAll(USER_LOCKMETHOD, MyProc, false,
						  InvalidTransactionId);
}

/* end of file */

/*
 * Local Variables:
 *	tab-width: 4
 *	c-indent-level: 4
 *	c-basic-offset: 4
 * End:
 */
