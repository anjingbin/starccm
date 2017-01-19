// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Huang Jie    huangjie@email.com
// Author: Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

#ifndef ___CCM_Dinner_impl_h__
#define ___CCM_Dinner_impl_h__

#include <CCM.h>
#include <CCM_Dinner_skel.h>
#include <Dinner_ValueType_impl.h>

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
namespace Dinner
{
	namespace PhilosopherImpl
	{
		class CCM_Philosopher_impl;
	}

}

unsigned int philosopherAction(Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_);
unsigned int PhGUIThreadProc(Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_);
//unsigned int ObGUIThreadProc(Dinner::ObserverImpl::CCM_Observer_impl* this_);

namespace Dinner
{

#define LOCK AutoLock dummy(mutex_);

#ifdef WIN32
#define LOCK_T HANDLE mutex_; 
#else
#define LOCK_T 	pthread_mutex_t mutex_;
#endif

class AutoLock
{
#ifdef WIN32
	HANDLE& hMutex_; 
#else
	pthread_mutex_t& mutex_;
#endif

public:
	
#ifdef WIN32
	AutoLock(HANDLE& hMutex)
		:hMutex_(hMutex)
	{
		WaitForSingleObject(hMutex_,INFINITE); 
#else
AutoLock(pthread_mutex_t& mutex)
		:mutex_(mutex)
	{
		pthread_mutex_lock(mutex_);
#endif
	}
	~AutoLock()
	{
#ifdef WIN32
		ReleaseMutex(hMutex_); 
#else
		pthread_mutex_unlock(mutex_);
#endif
	}
};

namespace PhilosopherImpl
{
class CCM_Philosopher_impl : virtual public CCM_Philosopher_skel, 
			     public Components::SessionComponent
{
    CCM_Philosopher_impl(const CCM_Philosopher_impl&);
    void operator=(const CCM_Philosopher_impl&);
    
    CORBA::String_var name_;
    CORBA::ULong thinking_seconds_,eating_seconds_,sleeping_seconds_;
    
    CCM_Philosopher_Context_var ctx_;

	LOCK_T
	
	volatile bool dead_;

public:
#ifdef WIN32
	unsigned long hThread_;
	HANDLE hEvent_;
	HANDLE hEvent2_;
	unsigned long hGUIThread_;
	unsigned int threadGUIId_;
	HWND hWnd_;
	HBITMAP hBitmap_;
#else
	pthread_t hThread_;
	pthread_cond_t hEvent_;
	pthread_attr_t attr_;
#endif

    CCM_Philosopher_impl();
    ~CCM_Philosopher_impl();

    virtual char* name()
        throw(CORBA::SystemException);
    virtual void name(const char*)
        throw(CORBA::SystemException);

    virtual CORBA::ULong thinking_seconds()
        throw(CORBA::SystemException);
    virtual void thinking_seconds(CORBA::ULong)
        throw(CORBA::SystemException);


    virtual CORBA::ULong eating_seconds()
        throw(CORBA::SystemException);
    virtual void eating_seconds(CORBA::ULong)
        throw(CORBA::SystemException);

    virtual CORBA::ULong sleeping_seconds()
        throw(CORBA::SystemException);
    virtual void sleeping_seconds(CORBA::ULong)
        throw(CORBA::SystemException);
        

    virtual void set_session_context(::Components::SessionContext_ptr ctx)
    {
    	ctx_ = CCM_Philosopher_Context::_narrow(ctx);
    }

    virtual void ccm_activate(){}

    virtual void ccm_passivate(){}

    virtual void ccm_remove(){}

	virtual void configuration_complete();

	friend unsigned int ::philosopherAction(Dinner::PhilosopherImpl::CCM_Philosopher_impl*  this_);
	friend unsigned int ::PhGUIThreadProc(Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_);
};

class CCM_PhilosopherHome_impl : virtual public CCM_PhilosopherHome_skel
{
    CCM_PhilosopherHome_impl(const CCM_PhilosopherHome_impl&);
    void operator=(const CCM_PhilosopherHome_impl&);

public:

    CCM_PhilosopherHome_impl();
    ~CCM_PhilosopherHome_impl();

    virtual Components::EnterpriseComponent_ptr create()
        throw(Components::CCMException,
              CORBA::SystemException);

	virtual char* get_storage_home_id();
              
};
}

namespace CutleryImpl
{
class CCM_Cutlery_impl : virtual public CCM_Cutlery_skel, 
			 public Components::SessionComponent
{
    CCM_Cutlery_impl(const CCM_Cutlery_impl&);
    void operator=(const CCM_Cutlery_impl&);

    bool inUse_;
    Components::Cookie_var cookie_;
    
    CCM_Cutlery_Context_var ctx_;
    
    static int idBase_;
    
    int id_;

	LOCK_T

public:

    CCM_Cutlery_impl();
    ~CCM_Cutlery_impl();

    virtual Components::Cookie* obtain_fork()
        throw(Dinner::ForkNotAvailable,
              CORBA::SystemException);

    virtual void release_fork(Components::Cookie* ck)
        throw(Dinner::NotTheEater,
              CORBA::SystemException);
              
    virtual void set_session_context(::Components::SessionContext_ptr ctx)
    {
    	ctx_ = CCM_Cutlery_Context::_narrow(ctx);
    }

    virtual void ccm_activate(){}

    virtual void ccm_passivate(){}

    virtual void ccm_remove(){}
};

class CCM_CutleryHome_impl : virtual public CCM_CutleryHome_skel
{
    CCM_CutleryHome_impl(const CCM_CutleryHome_impl&);
    void operator=(const CCM_CutleryHome_impl&);

public:

    CCM_CutleryHome_impl();
    ~CCM_CutleryHome_impl();

    virtual Components::EnterpriseComponent_ptr create()
        throw(Components::CCMException,
              CORBA::SystemException);

	virtual char* get_storage_home_id();
};
}


}

#endif
