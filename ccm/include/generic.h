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

#ifndef _GENERIC_H
#define _GENERIC_H


#ifndef PROTOTYPES

#define PROTOTYPES 0

#endif

/* POINTER defines a generic pointer type */

typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */

typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */

typedef unsigned long int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.

  If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
  returns an empty list. 
*/

#if PROTOTYPES

#define PROTO_LIST(list) list

#else

#define PROTO_LIST(list) ()

#endif


#endif /* _GENERIC_H */

