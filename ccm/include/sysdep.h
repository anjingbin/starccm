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

#ifndef _SYSDEP_H
#define _SYSDEP_H

#include <sys/types.h>
#ifndef WIN32
	#include <sys/time.h>
#else 
	#include <time.h>
	#include <Winsock.h>
#endif
//#include <sys/sysinfo.h>

/* change to point to where MD5 .h's live */
/* get MD5 sample implementation from RFC 1321 */

#include <memory.h>
#include "generic.h"
#include "md5.h"


/* set the following to the number of 100ns ticks of the actual
resolution of your system's clock */

#define UUIDS_PER_TICK 1024

/* Set the following to a call to acquire a system wide global lock. */

#define LOCK
#define UNLOCK

typedef unsigned long   unsigned32;
typedef unsigned short  unsigned16;
typedef unsigned char   unsigned8;
typedef unsigned char   byte;


/* Set this to what your compiler uses for 64 bit data type */

#ifdef WININC

#define unsigned64_t unsigned __int64
#define I64(C) C

#else

#define unsigned64_t unsigned long
#define I64(C) C##LL

#endif


typedef unsigned64_t uuid_time_t;

typedef struct {
    char nodeID[6];
} uuid_node_t;

void get_ieee_node_identifier(uuid_node_t *node);
void get_system_time(uuid_time_t *uuid_time);
void get_random_info(char seed[16]);


#endif /* _SYSDEP_H */
