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

#ifndef __UuidGenerator_h__
#define __UuidGenerator_h__

#include <CORBA.h>
#include <stdio.h>
//#include "uuid/sysdep.h"
//#include "uuid/uuid.h"

//Comment is added by huangjie 2003.2.27
//The following four lines will be no use when compiling.
//But they are useful for Microsoft Visual Studio to recognise 
//some types which will be defined by us.
#if (!defined OBJVAR)&&(!defined OBJPTR)
#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;
#endif

namespace Components
{

class UuidGenerator;
OBJVAR(UuidGenerator)
OBJPTR(UuidGenerator)

class UuidGenerator : REF_COUNT_LOCAL_OBJECT
{

	//long counter;
public:
	UuidGenerator();
	virtual ~UuidGenerator();
	
	CCM_IMPORT static char* generateUuid();
	
};

}

#endif

