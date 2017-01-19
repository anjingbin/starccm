// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Kebo    mep@263.net
// Author: Huang Jie    huangjie@email.com
// Author: Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#ifndef _UUID_H
#define _UUID_H

#include "sysdep.h"

typedef struct _uuid_t {
	unsigned32          time_low;
	unsigned16          time_mid;
	unsigned16          time_hi_and_version;
	unsigned8           clock_seq_hi_and_reserved;
	unsigned8           clock_seq_low;
	byte                node[6];
} uuidtype;

/* uuid_create -- generate a UUID */

int uuid_create(uuidtype * uuid);


/* uuid_create_from_name -- create a UUID using a "name" from a "name space" */

/*
 * uuid_t * uuid :  resulting UUID. 
 * uuid_t * nsid :  UUID to serve as context, so identical		
 *					names from different name spaces generate
 *					different UUIDs.
 * void * name	 :  The name from which to generate a UUID.
 * int namelen	 :  The length of the name.
 */

void uuid_create_from_name( uuidtype * uuid, uuidtype nsid, void * name, int namelen );

											
/* 
 * uuid_compare --  Compare two UUID's "lexically" and return
 *
 *  -1   u1 is lexically before u2
 *   0   u1 is equal to u2
 *   1   u1 is lexically after u2
 *
 *  Note:   lexical ordering is not temporal ordering!
 */

int uuid_compare(uuidtype *u1, uuidtype *u2);


#endif /* _UUID_H */
