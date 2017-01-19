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


#include <CORBA.h>
#include <CCM_Dinner_impl.h>
#include <Dinner_ValueType_impl.h>

#ifdef WIN32
#ifdef ORBacus
JTCInitialize* pJTCInitialize;
#endif

#ifdef StarBus
MTLInitialize* pMTLInitialize;
#endif


#ifdef WIN32

#include <windows.h>
#include <resource.h>

TCHAR* szPhWindowClass = "Philosopher";
TCHAR* szObWindowClass = "Observer";

volatile HBITMAP hHungry = NULL;
volatile HBITMAP hEating = NULL;
volatile HBITMAP hSleeping = NULL;
volatile HBITMAP hThinking = NULL;
volatile HBITMAP hDead = NULL;
volatile HBITMAP hHungryLeftFork = NULL;
volatile HBITMAP hHungryRightFork = NULL;
volatile HBITMAP hHungryBothFork = NULL;

volatile HINSTANCE hInst = NULL;

LRESULT CALLBACK	PhWndProc(HWND, UINT, WPARAM, LPARAM);

#else
#endif

BOOL APIENTRY DllMain( HANDLE hModule, 
         DWORD  reason, 
         LPVOID lpReserved)
{
     switch (reason)
     {
           case DLL_PROCESS_ATTACH:
				#ifdef ORBacus
				pJTCInitialize = new JTCInitialize();
				#endif
				
				#ifdef StarBus
				pMTLInitialize = new MTLInitialize();
				#endif
				
				#ifdef WIN32

				hInst = (HINSTANCE)hModule;		
				hHungry = LoadBitmap(hInst, "HUNGRY");
				ASSERT(hHungry);
				hEating = LoadBitmap(hInst, "EATING");
				ASSERT(hEating);
				hSleeping = LoadBitmap(hInst, "SLEEPING");
				ASSERT(hSleeping);
				hThinking = LoadBitmap(hInst, "THINKING");
				ASSERT(hThinking);
				hDead = LoadBitmap(hInst, "DEAD");
				ASSERT(hDead);
				hHungryLeftFork = LoadBitmap(hInst, "HUNGRYLEFTFORK");
				ASSERT(hHungryLeftFork);
				hHungryBothFork = LoadBitmap(hInst, "HUNGRYBOTHFORK");
				ASSERT(hHungryBothFork);
				#else
				#endif

                break;
           case DLL_THREAD_ATTACH:
                break;
           case DLL_THREAD_DETACH:
                break;
           case DLL_PROCESS_DETACH:
                {
                	#ifdef ORBacus
                	delete pJTCInitialize;
                	#endif
                	
                	#ifdef StarBus
                	delete pMTLInitialize;
                	#endif                	               	
                }

				#ifdef WIN32

				DeleteObject(hHungry);
				DeleteObject(hEating);
				DeleteObject(hSleeping);
				DeleteObject(hThinking);

				#else
				#endif
                break;
     }
     return TRUE;
}


extern "C"
{
__declspec(dllexport) Dinner::PhilosopherState*
createDinnerPhilosopherState();

__declspec(dllexport) CORBA::ValueFactoryBase*
createDinnerPhilosopherStateFactory();

};

#else

extern "C"
{
Dinner::PhilosopherState*
createDinnerPhilosopherState();

CORBA::ValueFactoryBase*
createDinnerPhilosopherStateFactory();

};
#endif

Dinner::PhilosopherState*
createDinnerPhilosopherState()
{
    return new Dinner::PhilosopherState_impl();
}

CORBA::ValueFactoryBase*
createDinnerPhilosopherStateFactory()
{
    return new Dinner::PhilosopherStateFactory_impl();
}


#ifdef WIN32

extern "C"
{
__declspec(dllexport) Components::HomeExecutorBase* 
createDinnerPhilosopherImplCCM_PhilosopherHome_impl();

__declspec(dllexport) Components::HomeExecutorBase* 
createDinnerCutleryImplCCM_CutleryHome_impl();

__declspec(dllexport) Components::HomeExecutorBase* 
createDinnerObserverImplCCM_ObserverHome_impl();

};

#else

extern "C"
{
Components::HomeExecutorBase* 
createDinnerPhilosopherImplCCM_PhilosopherHome_impl();

Components::HomeExecutorBase* 
createDinnerCutleryImplCCM_CutleryHome_impl();

Components::HomeExecutorBase* 
createDinnerObserverImplCCM_ObserverHome_impl();

};
#endif






Components::HomeExecutorBase* 
createDinnerPhilosopherImplCCM_PhilosopherHome_impl()
{
    return new Dinner::PhilosopherImpl::CCM_PhilosopherHome_impl();
}

Components::HomeExecutorBase* 
createDinnerCutleryImplCCM_CutleryHome_impl()
{
    return new Dinner::CutleryImpl::CCM_CutleryHome_impl();
}

Components::HomeExecutorBase* 
createDinnerObserverImplCCM_ObserverHome_impl()
{
    return new Dinner::ObserverImpl::CCM_ObserverHome_impl();
}


