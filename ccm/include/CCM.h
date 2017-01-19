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

#ifndef __CCM_h__
#define __CCM_h__

#include <CORBA.h>
#include <ComponentsClient.h>
#include <CCMCommonType_impl.h>

//Comment is added by huangjie 2003.2.27
//The following four lines will be no use when compiling.
//But they are useful for Microsoft Visual Studio to recognise 
//some types which will be defined by us.
#if (!defined OBJVAR)&&(!defined OBJPTR)
#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;
#endif

class CCM;
OBJVAR(CCM)
OBJPTR(CCM)

class CCM : REF_COUNT_LOCAL_OBJECT
{
public:
        CCM_IMPORT static void init(CORBA::ORB_ptr orb);
};

#endif

