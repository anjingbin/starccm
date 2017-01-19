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

#ifndef __StorageObjectLoader_h__
#define __StorageObjectLoader_h__

#include <CORBA.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ContainerBase_fwd.h>
#include <Loader.h>
#include <EnterpriseComponent.h>
#include <PSSInitializer.h>

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

//Comment is added by huangjie 2003.2.27
//The following four lines will be no use when compiling.
//But it is useful for Microsoft Visual Studio to recognise 
//some type which we will define.
#if (!defined OBJVAR)&&(!defined OBJPTR)
#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;
#endif

namespace Container
{

class StorageObjectLoader;

OBJVAR(StorageObjectLoader)
OBJPTR(StorageObjectLoader)

class StorageObjectLoader : virtual public Loader
{
private:
	PSSInitializer* pssInitializer_;
	CosPersistentState::Connector_var connector_;
public:
	StorageObjectLoader(char * dllUrl,ContainerRunTime_ptr runtime);
	virtual ~StorageObjectLoader();
	
	void loadPSS();
	
//SuLiang add 2003.3.31
	CosPersistentState::Session_ptr getSession();

};	//End of class Loader

}	//End of namespace Container

#endif
