#ifndef _UUID_H
#define _UUID_H

#include "sysdep.h"
#ifdef __cplusplus
extern "C"
{

#endif 

typedef struct _uuid_t {
	unsigned32          time_low;
	unsigned16          time_mid;
	unsigned16          time_hi_and_version;
	unsigned8           clock_seq_hi_and_reserved;
	unsigned8           clock_seq_low;
	byte                node[6];
} uuid_t;

/* uuid_create -- generate a UUID */

int uuid_create(uuid_t * uuid);


/* uuid_create_from_name -- create a UUID using a "name" from a "name space" */

/*
 * uuid_t * uuid :  resulting UUID. 
 * uuid_t * nsid :  UUID to serve as context, so identical		
 *					names from different name spaces generate
 *					different UUIDs.
 * void * name	 :  The name from which to generate a UUID.
 * int namelen	 :  The length of the name.
 */

void uuid_create_from_name( uuid_t * uuid, uuid_t nsid, void * name, int namelen );

											
/* 
 * uuid_compare --  Compare two UUID's "lexically" and return
 *
 *  -1   u1 is lexically before u2
 *   0   u1 is equal to u2
 *   1   u1 is lexically after u2
 *
 *  Note:   lexical ordering is not temporal ordering!
 */

int uuid_compare(uuid_t *u1, uuid_t *u2);

#ifdef __cplusplus
}

#endif

#endif /* _UUID_H */
