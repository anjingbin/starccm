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

#ifndef __PassivationArbitor_h__
#define __PassivationArbitor_h__

#include <CORBA.h>

#if (!defined OBJVAR)&&(!defined OBJPTR)
#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;
#endif

namespace Container
{

class PassivationArbitor;

OBJVAR(PassivationArbitor)
OBJPTR(PassivationArbitor)

enum PASSIVATEPOLICY
{
	COMPONENTCOUNT
};

class PassivationArbitor : REF_COUNT_LOCAL_OBJECT
{
protected:
	PASSIVATEPOLICY policy_;
	int componentCount_;
	int count_;
	
public:
	PassivationArbitor();
	virtual ~PassivationArbitor();
	
	int canPassiavte();
	void setPassivatePolicy(PASSIVATEPOLICY policy);
	void setComponentCount(int count);
	void onCreate();
	void onRemove();
};

}
#endif

