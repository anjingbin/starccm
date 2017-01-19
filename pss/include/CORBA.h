// **********************************************************************
//
// Copyright (c) 2001-2002 
// StarMiddleware Group	
// www.StarMiddleware.org
// 
// All Rights Reserved
//
// Author: An Jingbin	anjb@sina.com	
// **********************************************************************


#ifndef __CCM_CORBA_H__
#define __CCM_CORBA_H__

#ifndef WIN32
//#include <dlfcn.h>
#endif

#ifdef WIN32
	#define LOADLIBRARY ::LoadLibrary
	#define GETPROCADDRESS ::GetProcAddress
	#define HANDLEOFDLL HINSTANCE
	#define CLOSEDLL ::FreeLibrary
	#define PATH_DELILIMITOR "\\"
	#define PATH_DELILIMITOR_CHAR '\\'
#else
	#define LOADLIBRARY(filename) dlopen(filename,RTLD_NOW)
	#define GETPROCADDRESS dlsym
	#define HANDLEOFDLL void*
	#define CLOSEDLL dlclose
	#define PATH_DELILIMITOR "/"
	#define PATH_DELILIMITOR_CHAR '/'
#endif

#ifdef ORBacus

#include <OB/CORBA.h>
#include <OB/Hashtable.h>
#include <OB/BootManager.h>
#include <OB/ObjectIdHasher.h>
//#include <OB/CosTransactionsUser_skel.h>
#include <OB/IFR.h>


namespace AppServer
{
	typedef OB::SynchronizedHashtable< PortableServer::ObjectId,CORBA::Object_var,
    			   OBPortableServer::ObjectIdHasher> OBJECTHASHTABLE;
}

namespace CORBA
{
	typedef IRObject* IRObject_ptr;
};

#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;

#define REF_COUNT_LOCAL_OBJECT public OBCORBA::RefCountLocalObject
#define LOCAL_OBJECT_BASE virtual public OBCORBA::LocalObjectBase
#define OBJDUPLICATE(CLASSNAME,OBJ) dynamic_cast< CLASSNAME >(HelpFun::ObjHelper::_duplicate( OBJ ))
#define OBJNIL(CLASSNAME,OBJ) dynamic_cast< CLASSNAME >(HelpFun::ObjHelper::_nil( OBJ ))

namespace HelpFun
{
	class ObjHelper
	{
	public:
		static OBCORBA::RefCountLocalObject* 
		_duplicate(OBCORBA::RefCountLocalObject* p)
		{
			if(p) p -> _OB_incRef(); return p;
		}

		static OBCORBA::RefCountLocalObject* 
		_nil(OBCORBA::RefCountLocalObject* p)
		{
			return 0;
		}
 	};
}
#endif

#ifdef StarBus

#include <STAR/CORBA.h>
#include <STAR/ObjectIdHasher.h>
#include <STAR/AssistAdaptorManager.h>
#include <STAR/IFR.h>


namespace AppServer
{
}

namespace CORBA
{
	typedef IRObject* IRObject_ptr;
};

#define OBJVAR(CLASSNAME) typedef STAR::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;

#define REF_COUNT_LOCAL_OBJECT public CORBA::LocalObject
#define LOCAL_OBJECT_BASE virtual public STARCORBA::LocalObjectBase
#define OBJDUPLICATE(CLASSNAME,OBJ) dynamic_cast< CLASSNAME >(HelpFun::ObjHelper::_duplicate( OBJ ))
#define OBJNIL(CLASSNAME,OBJ) dynamic_cast< CLASSNAME >(HelpFun::ObjHelper::_nil( OBJ ))

namespace HelpFun
{
	class ObjHelper
	{
	public:
		static CORBA::LocalObject* 
		_duplicate(CORBA::LocalObject* p)
		{
			if(p) p -> _add_ref(); return p;
		}

		static CORBA::LocalObject* 
		_nil(CORBA::LocalObject* p)
		{
			return 0;
		}
 	};
}
#endif

#endif
