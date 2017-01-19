// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Huang Jie    huangjie@email.com
//         Wang Kebo    mep@263.net
//         Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#ifndef __CCM_CORBA_H__
#define __CCM_CORBA_H__

// Modified by WangKebo, for compilation on TAO
// Windows.h conflict with ACE/TAO, which included in Trace.h
// Move it to the end this file
//#include <Trace.h>


#ifndef WIN32
#include <dlfcn.h>
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

#ifdef DLL_LIB
#define CCM_IMPORT __declspec(dllimport)
#else
#define CCM_IMPORT
#endif

#ifdef ORBacus

#include <OB/CORBA.h>
#include <OB/Hashtable.h>
#include <OB/BootManager.h>
#include <OB/ObjectIdHasher.h>
//#include <OB/CosTransactionsUser_skel.h>
#include <OB/IFR.h>
#include <OB/CosNaming.h>
#include <JTC/JTC.h>


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

#if defined(STARCCM_MTL)
	#define STARCCM_COMMA_MONITOR				, public JTCMonitor
	#define STARCCM_MONITOR(monitorLock_)		JTCMonitor monitorLock_;
	#define STARCCM_COMMA_RECURSIVE_MUTEX		, virtual public JTCRecursiveMutex
	#define STARCCM_RECURSIVE_MUTEX(mutex)		JTCRecursiveMutex mutex;
	#define STARCCM_RECURSIVE_MUTEX_CLASS		JTCRecursiveMutex
	#define STARCCM_SYNCHRONIZED(sync_,mon_)	JTCSynchronized	sync_(mon_);
#else
	#define STARCCM_COMMA_MONITOR
	#define STARCCM_MONITOR(monitorLock_)
	#define STARCCM_COMMA_RECURSIVE_MUTEX
	#define STARCCM_RECURSIVE_MUTEX(mutex)
	#define STARCCM_RECURSIVE_MUTEX_CLASS
	#define STARCCM_SYNCHRONIZED(sync_,mon_)
#endif

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
#endif  //End of ORBacus

#ifdef StarBus

#include <STAR/CORBA.h>
#include <STAR/ObjectIdHasher.h>
#include <STAR/AssistAdaptorManager.h>
#include <STAR/IFR.h>
#include <STAR/CosNaming.h>
#include <MTL/MTL.h>

	
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
#define LOCAL_OBJECT_BASE public CORBA::LocalObject
#define OBJDUPLICATE(CLASSNAME,OBJ) dynamic_cast< CLASSNAME >(HelpFun::ObjHelper::_duplicate( OBJ ))
#define OBJNIL(CLASSNAME,OBJ) dynamic_cast< CLASSNAME >(HelpFun::ObjHelper::_nil( OBJ ))

#if defined(STARCCM_MTL)
	#define STARCCM_COMMA_MONITOR				, public MTLMonitor
	#define STARCCM_MONITOR(monitorLock_)		MTLMonitor monitorLock_;
	#define STARCCM_COMMA_RECURSIVE_MUTEX		,virtual public MTLRecursiveMutex
	#define STARCCM_RECURSIVE_MUTEX(mutex)		MTLRecursiveMutex mutex;
	#define STARCCM_RECURSIVE_MUTEX_CLASS		MTLRecursiveMutex
	#define STARCCM_SYNCHRONIZED(sync_,mon_)	MTLSynchronized	sync_(mon_);
#else
	#define STARCCM_COMMA_MONITOR
	#define STARCCM_MONITOR(monitorLock_)
	#define STARCCM_COMMA_RECURSIVE_MUTEX
	#define STARCCM_RECURSIVE_MUTEX(mutex)
	#define STARCCM_RECURSIVE_MUTEX_CLASS
	#define STARCCM_SYNCHRONIZED(sync_,mon_)
#endif

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
#endif  //End of Starbus


#ifdef TAO
#include <tao/corba.h>
#include <tao/PortableServer/PortableServer.h>
#include <tao/IORTable/IORTable.h>
#include <tao/DynamicC.h>
#include <tao/DynamicInterface/context.h>
#include <tao/DynamicInterface/Request.h>
#include <tao/DynamicInterface/ExceptionList.h>
#include <orbsvcs/CosNamingC.h>
#include <tao/DynamicAny/DynamicAny.h>
#include <tao/IFR_Client/IFR_BasicC.h>
#include <tao/IFR_Client/IFR_ExtendedC.h>
#include <tao/TypeCodeFactory/TypeCodeFactory_Adapter_Impl.h>
#include <string>

//namespace CORBA
//{
//	typedef IRObject* IRObject_ptr;
//};

template<class T>
class ObjVar : public TAO_Base_var
{
	T* ptr_;
	ObjVar(const TAO_Base_var &rhs);
	ObjVar &operator=(const TAO_Base_var &rhs);
public:
	ObjVar(): ptr_(0)
	{
	}
	
	ObjVar(T* p) : ptr_(p) 
	{
	}
	
//	ObjVar(const ObjVar<T>& p) 
//		:TAO_Base_var (), ptr_ (T::_duplicate(p.ptr ()))
//	{
//	};

	~ObjVar() 
	{
//		CORBA::release(ptr_);
		ptr_ -> _remove_ref();
	}

	ObjVar& operator=(T*p)
	{
		CORBA::release(ptr_);
		ptr_ = p;
		return *this;
	}

	ObjVar& operator=(const ObjVar<T>& p)
	{
		if (ptr_ != p.ptr_)
		{
			CORBA::release(ptr_);
			p -> _add_ref();
			ptr_ = p.ptr_;
		}
		return *this;
	}

	T* operator->() const
	{
		return ptr_;
	}

	operator const T*&() const
	{
		return ptr_;
	}

	operator T*&()
	{
		return ptr_;
	}

	// in, inout, out, _retn 
	T* in() const
	{
		return ptr_;
	}

	T* &inout()
	{
		return ptr_;
	}

	T* &out()
	{
		CORBA::release(ptr_);
		ptr_ = 0;
		return ptr_;
	}

	T* _retn()
	{
		T* val = ptr_;
		ptr_ = 0;
		return val;
	}

	T* ptr() const
	{
		return ptr_;
	}
/*
	static T* tao_duplicate(T* p)
	{
		return T::_duplicate (p);
	}

	static void tao_release(T* p)
	{
		CORBA::release (p);
	}

	static T* tao_nil()
	{
		return T::_nil ();
	}

	static T* tao_narrow(CORBA::Object* p, CORBA::Environment& ACE_TRY_ENV)
	{
		return T::_narrow (p, ACE_TRY_ENV);
	}

	static CORBA::Object* tao_upcast(void* src)
	{
		T** tmp = ACE_static_cast (T**, src);
		return *tmp;
	} 
*/
};

template <class T>
class VarOut
{
	T*& ptr_;
public:
	VarOut(T*& p)  
		: ptr_(p)
	{
		ptr_ = 0;
	}

	VarOut(ObjVar<T>& p)  
		: ptr_(p.out ())
	{
		CORBA::release (ptr_);
		ptr_ = 0;
	}

	VarOut(const VarOut<T>& p)
		: ptr_(ACE_const_cast(VarOut<T>&, p).ptr_)
	{
	}

	VarOut& operator= (const VarOut<T>& p)
	{
		ptr_ = ACE_const_cast(VarOut<T>&, p).ptr_;
		return *this;
	}

	VarOut& operator= (const ObjVar<T>& p)
	{
		p -> _add_ref();
		ptr_ = p.ptr_;
		return *this;
	}

	VarOut& operator=(T* p)
	{
		ptr_ = p;
		return *this;
	}

	operator T*&()
	{
		return ptr_;
	}

	T*& ptr ()
	{
		return ptr_;
	}

	T* operator->()
	{
		return ptr_;
	}
};

#define OBJVAR(CLASSNAME) typedef ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;

#define REF_COUNT_LOCAL_OBJECT public TAO_Local_RefCounted_Object
#define LOCAL_OBJECT_BASE public CORBA::LocalObject
#define OBJDUPLICATE(CLASSNAME,OBJ) dynamic_cast< CLASSNAME >(HelpFun::ObjHelper::_duplicate( OBJ ))
#define OBJNIL(CLASSNAME,OBJ) dynamic_cast< CLASSNAME >(HelpFun::ObjHelper::_nil( OBJ ))

namespace CORBA
{
	typedef CORBA_ValueFactoryBase_var ValueFactoryBase_var;
}

#define STARCCMSTRINGADD 1

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
 	
	static char*
	CORBA_string_add(const char* s1, const char* s2)
	{
		std::string ss1(s1);
		std::string ss2(s2);
		
		ss1 = ss1 + ss2;
		CORBA::String_var result = ss1.c_str();
		
		return result._retn();
	}
}

#endif  //End of TAO

#include <Trace.h>

#endif
