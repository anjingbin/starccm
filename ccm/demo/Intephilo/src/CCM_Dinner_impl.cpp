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

#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <resource.h>
typedef unsigned int (__stdcall *THREADROUTING)(void *);
#define SLEEP(sec) Sleep(sec*1000)
#else
#include <unistd.h>
#define SLEEP(sec) sleep(sec)
#endif

#include <CORBA.h>
#include <CCM_Dinner_impl.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <CosNaming.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif


#ifdef WIN32
extern TCHAR* szPhWindowClass;
extern TCHAR* szObWindowClass;

extern volatile HBITMAP hHungry;
extern volatile HBITMAP hEating;
extern volatile HBITMAP hSleeping;
extern volatile HBITMAP hThinking;
extern volatile HBITMAP hDead;
extern volatile HBITMAP hHungryLeftFork;
extern volatile HBITMAP hHungryRightFork;
extern volatile HBITMAP hHungryBothFork;

extern volatile HINSTANCE hInst;

#define WM_EATING WM_USER+100
#define WM_HUNGRY WM_USER+101
#define WM_THINKING WM_USER+102
#define WM_SLEEPING WM_USER+103
#define WM_DEAD WM_USER+104
#define WM_HUNGRY_LEFTFORK WM_USER+105
#define WM_HUNGRY_BOTHFORK WM_USER+106

#define WM_STATUSCHANGE WM_USER+107

typedef map<HWND,Dinner::PhilosopherImpl::CCM_Philosopher_impl*> HWndPhMap;
HWndPhMap hWndPhMap;

volatile HWND hWndOb = NULL;

LRESULT CALLBACK PhWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ObWndProc(HWND, UINT, WPARAM, LPARAM);
#else
#endif

LOCK_T

unsigned int 
PhGUIThreadProc(Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_)
{
#ifdef WIN32
	MSG msg;
	WNDCLASSEX wcex;
	HWND hWnd;

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)PhWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, (LPCTSTR)IDI_PH);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szPhWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	RegisterClassEx(&wcex);

	this_ -> hBitmap_ = hThinking;

	hWnd = CreateWindow(szPhWindowClass, this_ -> name_ , WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ,
		CW_USEDEFAULT, 0, 180, 220, NULL, NULL, hInst, NULL);

	this_ -> hWnd_ = hWnd;

	hWndPhMap.insert(HWndPhMap::value_type(hWnd,this_));

	BOOL ret = SetEvent(this_ -> hEvent2_);
	ASSERT(ret);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	hWndPhMap.erase(hWnd);

	_endthread();
#else
#endif

	return 0;
}

#ifdef WIN32
LRESULT CALLBACK PhWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hDC;
	HDC hMemDC;

	HWndPhMap::iterator it;
	it = hWndPhMap.find(hWnd);
	Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_ = it -> second;

	switch (message) 
	{
//		case WM_COMMAND:
//			wmId    = LOWORD(wParam); 
//			wmEvent = HIWORD(wParam); 
//			switch (wmId)
//			{
//				case IDM_EXIT:
//				   DestroyWindow(hWnd);
//				   break;
//				default:
//				   return DefWindowProc(hWnd, message, wParam, lParam);
//			}
//			break;
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			hMemDC = CreateCompatibleDC(hDC);
			
			BITMAP bitmap;
			GetObject(this_ -> hBitmap_,sizeof(bitmap),&bitmap);
			SelectObject(hMemDC, this_ -> hBitmap_);
			StretchBlt(hDC, 0, 0, 180, 180, hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
			//BitBlt(hDC, 0, 0, 180, 180, hMemDC, 0, 0, SRCCOPY);

			DeleteDC(hMemDC);
			EndPaint(hWnd, &ps);

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_EATING:
			this_ -> hBitmap_ = hEating;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_HUNGRY:
			this_ -> hBitmap_ = hHungry;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_THINKING:
			this_ -> hBitmap_ = hThinking;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_SLEEPING:
			this_ -> hBitmap_ = hSleeping;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_DEAD:
			this_ -> hBitmap_ = hDead;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_HUNGRY_LEFTFORK:
			this_ -> hBitmap_ = hHungryLeftFork;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_HUNGRY_BOTHFORK:
			this_ -> hBitmap_ = hHungryBothFork;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

LRESULT CALLBACK OberserDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch(message)
	{
	case WM_INITDIALOG:
		hWndOb = hDlg;
		return TRUE;
	case WM_COMMAND:
		if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		EndDialog(hDlg, TRUE);
		break;
	case WM_STATUSCHANGE:
		HWND itemWnd = GetDlgItem(hDlg,IDC_EDIT);
//		SendMessage(itemWnd, EM_REPLACESEL, 0, lParam);
//		SendMessage(itemWnd, EM_REPLACESEL, 0, (LPARAM)"\r\n");
//		SendMessage(itemWnd, EM_SETREADONLY, (WPARAM)TRUE, 0);
		SendMessage(itemWnd, WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(itemWnd, EM_SETSEL, 0, 0);
		SendMessage(itemWnd, EM_REPLACESEL, 0, lParam);
		SendMessage(itemWnd, EM_SETREADONLY, (WPARAM)TRUE, 0);

		return TRUE;
	}

	return FALSE;
}
#endif

//unsigned int 
//ObGUIThreadProc(Dinner::ObserverImpl::CCM_Observer_impl* this_)
//{
//#ifdef WIN32
//	DialogBox(hInst,MAKEINTRESOURCE(IDD_OBSERVER),NULL,(DLGPROC)OberserDialogProc);
//
//	_endthread();
//#else
//#endif
//	return 0;
//}

unsigned int 
philosopherAction(::Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_)
{
	TRACE1(LEVEL1,"PHILOSOPHER %s : i'm born\n", this_ -> name_);
	
	::Dinner::CCM_Philosopher_Context_var ctx = this_ -> ctx_;
	
//	::Dinner::PhilosopherState_var thinking = new ::Dinner::PhilosopherState_impl(::Dinner::THINKING,this_ -> name(),NULL);
//	::Dinner::PhilosopherState_var hungry = new ::Dinner::PhilosopherState_impl(::Dinner::HUNGRY,this_ -> name(),NULL);
//	::Dinner::PhilosopherState_var eating = new ::Dinner::PhilosopherState_impl(::Dinner::EATING,this_ -> name(),NULL);
//	::Dinner::PhilosopherState_var sleeping = new ::Dinner::PhilosopherState_impl(::Dinner::SLEEPING,this_ -> name(),NULL);
//	::Dinner::PhilosopherState_var dead = new ::Dinner::PhilosopherState_impl(::Dinner::DEAD,this_ -> name(),NULL);


	#ifdef WIN32
	//avoid race condition.
	//be care of the `PostMessage' goes faster than Philosogher's HWND mapping to their executors. 
	WaitForSingleObject(this_ -> hEvent2_,INFINITE);
	CloseHandle(this_ -> hEvent2_);
	#else
	#endif
	
	::Dinner::Observing_var observer = ctx -> get_connection_observer();
	ASSERT(!CORBA::is_nil(observer));
	
	wchar_t buffer[1000];
	
	if( MultiByteToWideChar(CP_ACP,
	                        0,
	                        this_ -> name_.in(),
	                        strlen(this_ -> name_.in())+1,
	                        buffer,
	                        sizeof(buffer)/sizeof(buffer[0]) ) )
	{
		//Error
	}
	
	CORBA::WString_var name = (const wchar_t*)buffer;
	
	CORBA::WStringValue_var para = new CORBA::WStringValue(name);
	
	while(!this_ -> dead_)
	{
		TRACE1(LEVEL1,"PHILOSOPHER %s : i'm thinking\n", this_ -> name_);
		
		#ifdef WIN32
		PostMessage(this_ -> hWnd_, WM_THINKING, 0, 0);
		#else
		#endif

//		ctx -> push_philosopher_state(thinking.in());
		observer -> change_status(1/*THINKING*/, para.in());
		
		SLEEP(this_ -> thinking_seconds());
		
		TRACE1(LEVEL1,"PHILOSOPHER %s : i'm hungry\n", this_ -> name_);
		
		#ifdef WIN32
		PostMessage(this_ -> hWnd_, WM_HUNGRY, 0, 0);
		#else
		#endif

//		ctx -> push_philosopher_state(hungry.in());
		observer -> change_status(3/*HUNGRY*/, para.in());
		
		SLEEP(1);

		Components::Cookie_var leftCookie;
		Components::Cookie_var rightCookie;
		
		::Dinner::Fork_var leftHand = ctx -> get_connection_left_hand();
		::Dinner::Fork_var rightHand = ctx -> get_connection_right_hand();
		
		
		ASSERT(!CORBA::is_nil(leftHand));
		ASSERT(!CORBA::is_nil(rightHand));

		bool waitingFork = false;
		bool firstTringToGetFork = true;

		int haveTryedTimes = 0;

		while(true)
		{
			

			if(waitingFork && firstTringToGetFork)
			{
				TRACE1(LEVEL0,"PHILOSOPHER %s : i'm waiting for forks\n", this_ -> name_);
				#ifndef _DEBUG
				cout<<"Philosopher "<<this_ -> name()<<" : i'm waiting forks"<<endl;
				#endif
				firstTringToGetFork = false;
			}
RetryGetLeft:
			try
			{
				TRACE1(LEVEL2,"PHILOSOPHER %s : i'm trying to obtain left fork\n", this_ -> name_);
				leftCookie = leftHand -> obtain_fork();
				
				#ifdef WIN32
				PostMessage(this_ -> hWnd_, WM_HUNGRY_LEFTFORK, 0, 0);
				#else
				#endif
				
				TRACE1(LEVEL2,"PHILOSOPHER %s : i have obtained left fork\n", this_ -> name_);
				if(haveTryedTimes < 5) SLEEP(1);
			}
			catch(::Dinner::ForkNotAvailable&)
			{
				waitingFork = true;
				TRACE1(LEVEL2,"PHILOSOPHER %s : i have failed to obtain left fork\n", this_ -> name_);
				haveTryedTimes ++;
				continue;
			}
			catch(...)
			{
				goto RetryGetLeft;
			}
RetryGetRight:
			try
			{
				TRACE1(LEVEL2,"PHILOSOPHER %s : i'm trying to obtain right fork\n", this_ -> name_);
				rightCookie = rightHand -> obtain_fork();
				
				#ifdef WIN32
				PostMessage(this_ -> hWnd_, WM_HUNGRY_BOTHFORK, 0, 0);
				#else
				#endif
				
				TRACE1(LEVEL2,"PHILOSOPHER %s : i have obtained right fork\n", this_ -> name_);
				if(haveTryedTimes < 5) SLEEP(1);
			}
			catch(::Dinner::ForkNotAvailable&)
			{
				waitingFork = true;
				TRACE1(LEVEL2,"PHILOSOPHER %s : i have failed to obtain right fork\n", this_ -> name_);

RetryReleaseLeft:

				try
				{
					TRACE1(LEVEL2,"PHILOSOPHER %s : i'm trying to release left fork\n", this_ -> name_);
					leftHand -> release_fork(leftCookie.in());
					
					#ifdef WIN32
					PostMessage(this_ -> hWnd_, WM_HUNGRY, 0, 0);
					#else
					#endif
					
					TRACE1(LEVEL2,"PHILOSOPHER %s : i have released left fork\n", this_ -> name_);
					if(haveTryedTimes < 5)  SLEEP(1);
				}
				catch(::Dinner::NotTheEater&)
				{
					ASSERT(false);
				}
				catch(...)
				{
					goto RetryReleaseLeft;
				}
				haveTryedTimes ++;
				continue;
			}
			catch(...)
			{
				goto RetryGetRight;
			}

			break;
		}
		
		haveTryedTimes = 0;

		TRACE1(LEVEL1,"PHILOSOPHER %s : i'm eating\n", this_ -> name_);
		
		#ifdef WIN32
		PostMessage(this_ -> hWnd_, WM_EATING, 0, 0);
		#else
		#endif

//		ctx -> push_philosopher_state(eating.in());
		observer -> change_status(0/*EATING*/, para.in());
		
		SLEEP(this_ -> eating_seconds());
		
		try
		{
			TRACE1(LEVEL2,"PHILOSOPHER %s : i'm trying to release both forks\n", this_ -> name_);
			leftHand -> release_fork(leftCookie.in());
			rightHand -> release_fork(rightCookie.in());
			TRACE1(LEVEL2,"PHILOSOPHER %s : i have released both forks\n", this_ -> name_);
		}
		catch(::Dinner::NotTheEater&)
		{
			ASSERT(false);
		}
		
		TRACE1(LEVEL1,"PHILOSOPHER %s : i'm sleeping\n", this_ -> name_);
		
		#ifdef WIN32
		PostMessage(this_ -> hWnd_, WM_SLEEPING, 0, 0);
		#else
		#endif

//		ctx -> push_philosopher_state(sleeping.in());
		observer -> change_status(2/*SLEEPING*/, para.in());
		
		SLEEP(this_ -> sleeping_seconds());

		waitingFork = false;
		firstTringToGetFork = true;
	}
	
	TRACE1(LEVEL1,"PHILOSOPHER %s : i'm dead", this_ -> name_);

	#ifdef WIN32
	PostMessage(this_ -> hWnd_, WM_DEAD, 0, 0);
	#else
	#endif

//	ctx -> push_philosopher_state(dead.in());
	observer -> change_status(4/*DEAD*/, para.in());
	SLEEP(1);

#ifdef WIN32
	BOOL ret = SetEvent(this_ -> hEvent_);
	ASSERT(ret);
	_endthread();
#else
	int ret = pthread_cond_signal(&(this_ -> hEvent_));
	ASSERT(!ret);
#endif

	return 0;
}

Dinner::PhilosopherImpl::CCM_Philosopher_impl::CCM_Philosopher_impl()
	:dead_(false),thinking_seconds_(1),eating_seconds_(1),sleeping_seconds_(1)
{
#ifdef WIN32
	hThread_ = NULL;
	hGUIThread_ = NULL;
	hWnd_ = NULL;
	threadGUIId_ = 0;
	hEvent_ = CreateEvent(NULL,FALSE,FALSE,NULL);
	hEvent2_ = CreateEvent(NULL,FALSE,FALSE,NULL);
#else
	hThread = 0;
	pthread_cond_init(&hEvent_,NULL);
#endif
}

Dinner::PhilosopherImpl::CCM_Philosopher_impl::~CCM_Philosopher_impl()
{
	dead_ = true;
#ifdef WIN32
	WaitForSingleObject(hEvent_,INFINITE);
	CloseHandle(hEvent_);
	//if(hThread_) CloseHandle(hThread_);
	//if(hGUIThread_) CloseHandle(hGUIThread_);
#else
	pthread_cond_wait(&hEvent_,NULL);
	pthread_cond_destroy(&hEvent_);
	pthread_attr_destroy(&attr_);
#endif
}

void
Dinner::PhilosopherImpl::CCM_Philosopher_impl::configuration_complete()
{
	//	SECURITY_ATTRIBUTES secAttr;
	//	secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	//	secAttr.lpSecurityDescriptor = NULL;
	//	secAttr.bInheritHandle = TRUE;	
	TRACE0(LEVEL6,"Configuration begin\n");
	
	TRACE0(LEVEL6,"Resolving naming service\n");
	//CCM_Philosopher_Context_impl* ctx = dynamic_cast<CCM_Philosopher_Context_impl*>(ctx_.in());
	CORBA::ORB_var orb = ctx_ -> getORB();
	
	CORBA::Object_var namingObject;
	CORBA::Object_var namingObject1;
	
	try
	{
//		namingObject = orb -> string_to_object("corbaloc:iiop:localhost:7001/NameService");
//		namingObject1 = orb -> string_to_object("corbaloc:iiop:localhost:9991/NameService");
	
		namingObject = orb -> resolve_initial_references("JNDI");
		namingObject1 = orb -> resolve_initial_references("NameService");
		assert(!CORBA::is_nil(namingObject.in()));
		assert(!CORBA::is_nil(namingObject1.in()));
	}
	catch(const CORBA::ORB::InvalidName&)
	{
		TRACE0(LEVEL6, "Resolve Initial References ERROR!\n");
		assert(false);
	}
	
	TRACE0(LEVEL6,"Narrowing...\n");
	
	CosNaming::NamingContext_var naming = CosNaming::NamingContext::_narrow(namingObject);
	CosNaming::NamingContext_var naming1 = CosNaming::NamingContext::_narrow(namingObject1);
	
	if(CORBA::is_nil(naming.in()))
    {
    	printf("Configuration ERROR!\n");
    }
    if(CORBA::is_nil(naming1.in()))
    {
    	printf("Configuration ERROR!\n");
    }
    
    TRACE0(LEVEL6,"Check EJB observer\n");
	
	int flag = 0;
	CORBA::Object_var aObj;
	CORBA::Object_var aObj1;
	try
	{
	    CosNaming::Name aName;
	    aName.length(1);
	    aName[0].id = CORBA::string_dup("Observer");
	    aName[0].kind = CORBA::string_dup("");
	    aObj = naming1 -> resolve(aName);
	    assert(!CORBA::is_nil(aObj.in()));
    }
    catch(const CosNaming::NamingContext::NotFound&)
    {
    	flag = 1;
    	TRACE0(LEVEL6,"Configuration Naming NotFound!\n");
    }
    catch(const CosNaming::NamingContext::CannotProceed&)
    {
    	flag = 1;
    	TRACE0(LEVEL6,"Configuration Naming CannotProceed!\n");
    }
    catch(const CosNaming::NamingContext::InvalidName&)
    {
    	flag = 1;
    	TRACE0(LEVEL6,"Configuration Naming InvalidName!\n");
    }
    catch(const CosNaming::NamingContext::AlreadyBound&)
    {
    	TRACE0(LEVEL6,"Configuration Naming AlreadyBound!\n");
    }
    catch(const CosNaming::NamingContext::NotEmpty&)
    {
    	TRACE0(LEVEL6,"Configuration Naming NotEmpty!\n");
    }
    
    TRACE0(LEVEL6,"Observer resolved\n");
    
    if( flag == 0 )
    {
    	TRACE0(LEVEL6,"Observer exist\n");
    	//not nil
    	Components::ComponentServentBase* servant = ctx_ -> getServant();
    	
    	TRACE0(LEVEL6,"Connnect observer\n");
    	try
    	{
    		servant -> connect("observer", aObj);
    	}
    	catch(Components::InvalidName&)
    	{
    		TRACE0(LEVEL6,"Configuration InvalidName!\n");
    	}
    	catch(Components::InvalidConnection)
    	{
    		TRACE0(LEVEL6,"Configuration InvalidConnection!\n");
    	}
    	catch(Components::AlreadyConnected)
    	{
    		TRACE0(LEVEL6,"Configuration AlreadyConnected!\n");
    	}
    	catch(Components::ExceededConnectionLimit)
    	{
    		TRACE0(LEVEL6,"Configuration ExceededConnectionLimit!\n");
    	}
    	TRACE0(LEVEL6,"Observer connected\n");
    }
    else
    {
    	TRACE0(LEVEL6,"Observer NOT exist, resolve ObservingHome\n");
    	//nil, create one then register into naming
    	
    	try
    	{
	    	CosNaming::Name aName1;
		    aName1.length(1);
		    aName1[0].id = CORBA::string_dup("Dinner-ObservingHome");
		    aName1[0].kind = CORBA::string_dup("");
		    aObj1 = naming -> resolve(aName1);
    	    assert(!CORBA::is_nil(aObj1.in()));
		}
		catch(const CosNaming::NamingContext::NotFound&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming NotFound!\n");
	    }
	    catch(const CosNaming::NamingContext::CannotProceed&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming CannotProceed!\n");
	    }
	    catch(const CosNaming::NamingContext::InvalidName&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming InvalidName!\n");
	    }
	    catch(const CosNaming::NamingContext::AlreadyBound&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming AlreadyBound!\n");
	    }
	    catch(const CosNaming::NamingContext::NotEmpty&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming NotEmpty!\n");
	    }
	    
	    
	    TRACE0(LEVEL6,"ObservingHome resolving ends, narrowing...\n");
	    
	    Dinner::ObservingHome_var home = Dinner::ObservingHome::_narrow(aObj1);
	    assert(!CORBA::is_nil(home.in()));
	    
	    TRACE0(LEVEL6, "Before EJB Home Create\n");
	    CORBA::Object_var observer = home -> create();
	    TRACE0(LEVEL6, "After EJB Home Create\n");
	    assert(!CORBA::is_nil(observer.in()));
	    
	    Components::ComponentServentBase* servant = ctx_ -> getServant();
	    
    	TRACE0(LEVEL6,"Connnect observer\n");
    	
    	try
    	{
    		servant -> connect("observer", observer);
    	}
    	catch(Components::InvalidName&)
    	{
    		TRACE0(LEVEL6,"Configuration InvalidName!\n");
    	}
    	catch(Components::InvalidConnection)
    	{
    		TRACE0(LEVEL6,"Configuration InvalidConnection!\n");
    	}
    	catch(Components::AlreadyConnected)
    	{
    		TRACE0(LEVEL6,"Configuration AlreadyConnected!\n");
    	}
    	catch(Components::ExceededConnectionLimit)
    	{
    		TRACE0(LEVEL6,"Configuration ExceededConnectionLimit!\n");
    	}
    	TRACE0(LEVEL6,"Observer connected\n");
    	
    	TRACE0(LEVEL6,"Register Observer into Naming\n");
    	//register into naming
    	
    	try
    	{
//	    	CosNaming::Name nc1Name;
//	        nc1Name.length(1);
//		    nc1Name[0].id = CORBA::string_dup("Observing");
//		    nc1Name[0].kind = CORBA::string_dup("");
//		    TRACE0(LEVEL6, "Before context Binding\n");
//		    CosNaming::NamingContext_var nc2 = naming -> bind_new_context(nc1Name);
//		    TRACE0(LEVEL6, "After context Binding\n");
		    
		    CosNaming::Name nc2Name;
		    nc2Name.length(1);
		    nc2Name[0].id = CORBA::string_dup("Observer");
		    nc2Name[0].kind = CORBA::string_dup("");
        
	        TRACE0(LEVEL6, "Before Observer Binding\n");
	        naming1 -> rebind(nc2Name, observer.in());
	        TRACE0(LEVEL6, "After Observer Binding\n");
	    }
	    catch(const CosNaming::NamingContext::NotFound&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming NotFound!\n");
	    }
	    catch(const CosNaming::NamingContext::CannotProceed&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming CannotProceed!\n");
	    }
	    catch(const CosNaming::NamingContext::InvalidName&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming InvalidName!\n");
	    }
	    catch(const CosNaming::NamingContext::AlreadyBound&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming AlreadyBound!\n");
	    }
	    catch(const CosNaming::NamingContext::NotEmpty&)
	    {
	    	TRACE0(LEVEL6,"Configuration Naming NotEmpty!\n");
	    }
    }
    
    TRACE0(LEVEL6,"Configuration end\n");
    
#ifdef WIN32
	hGUIThread_ = _beginthreadex(NULL,0,(THREADROUTING)PhGUIThreadProc,this,0,&threadGUIId_); 
	ASSERT(hGUIThread_);

	unsigned int threadId;
	hThread_ = _beginthreadex(NULL,0,(THREADROUTING)philosopherAction,this,0,&threadId); 
	ASSERT(hThread_ != NULL);
#else
	pthread_attr_init(&attr_);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);

	int ret = pthread_create(&hThread_,&attr,philosopherAction,this);
	ASSERT(ret == 0);
#endif

}

char* 
Dinner::PhilosopherImpl::CCM_Philosopher_impl::name()
	throw(CORBA::SystemException)
{
	return CORBA::string_dup(name_);
}

void
Dinner::PhilosopherImpl::CCM_Philosopher_impl::name(const char* nm)
	throw(CORBA::SystemException)
{
	name_ = CORBA::string_dup(nm);
}

CORBA::ULong 
Dinner::PhilosopherImpl::CCM_Philosopher_impl::thinking_seconds()
	throw(CORBA::SystemException)
{
	return thinking_seconds_;
}

void 
Dinner::PhilosopherImpl::CCM_Philosopher_impl::thinking_seconds(CORBA::ULong thinking_seconds)
	throw(CORBA::SystemException)
{
	thinking_seconds_ = thinking_seconds;
}

CORBA::ULong 
Dinner::PhilosopherImpl::CCM_Philosopher_impl::eating_seconds()
	throw(CORBA::SystemException)
{
	return eating_seconds_;
}

void 
Dinner::PhilosopherImpl::CCM_Philosopher_impl::eating_seconds(CORBA::ULong eating_seconds_)
	throw(CORBA::SystemException)
{
	eating_seconds_ = eating_seconds;
}

CORBA::ULong 
Dinner::PhilosopherImpl::CCM_Philosopher_impl::sleeping_seconds()
	throw(CORBA::SystemException)
{
	return sleeping_seconds_;
}

void Dinner::PhilosopherImpl::CCM_Philosopher_impl::sleeping_seconds(CORBA::ULong sleeping_seconds_)
	throw(CORBA::SystemException)
{
	sleeping_seconds_ = sleeping_seconds;
}

int Dinner::CutleryImpl::CCM_Cutlery_impl::idBase_ = 0;

Dinner::CutleryImpl::CCM_Cutlery_impl::CCM_Cutlery_impl()
	:inUse_(false),id_(idBase_++)
{
}

Dinner::CutleryImpl::CCM_Cutlery_impl::~CCM_Cutlery_impl()
{
}

Components::Cookie* 
Dinner::CutleryImpl::CCM_Cutlery_impl::obtain_fork()
	throw(Dinner::ForkNotAvailable,
		  CORBA::SystemException)
{
	LOCK
	 
	TRACE0(LEVEL6,"EXECUTOR:begin obtain fork\n");
	TRACE1(LEVEL6,"EXECUTOR:fork id is %d\n",id_);
	if( inUse_ )
	{
		TRACE0(LEVEL6,"EXECUTOR:fork is in use\n");
		throw Dinner::ForkNotAvailable();
	}
	
	inUse_ = true;
	
	cookie_ = new ::Components::Cookie_impl;

	cookie_ -> _add_ref();
	
	TRACE0(LEVEL6,"EXECUTOR:after obtain fork\n");
	
	return cookie_.in();
}

void 
Dinner::CutleryImpl::CCM_Cutlery_impl::release_fork(Components::Cookie* ck)
	throw(Dinner::ForkNotAvailable,
		  CORBA::SystemException)
{
	LOCK

	TRACE0(LEVEL6,"EXECUTOR:begin release fork\n");
	TRACE1(LEVEL6,"EXECUTOR:fork id is %d\n",id_);
	
	::Components::Cookie_impl* cookie1 = dynamic_cast<::Components::Cookie_impl*>(cookie_.in());
	::Components::Cookie_impl* cookie2 = dynamic_cast<::Components::Cookie_impl*>(ck);
	
	ASSERT(cookie1);
	ASSERT(cookie2);
	
	if(cookie1 -> isEqual(cookie2))
	{
		TRACE0(LEVEL6,"EXECUTOR:releasing fork\n");
		inUse_ = false;

		cookie_ = NULL;
		return;
	}
	
	TRACE0(LEVEL6,"EXECUTOR:fork is not yours\n");
	throw Dinner::ForkNotAvailable();
}

Dinner::PhilosopherImpl::CCM_PhilosopherHome_impl::CCM_PhilosopherHome_impl()
{
}

Dinner::PhilosopherImpl::CCM_PhilosopherHome_impl::~CCM_PhilosopherHome_impl()
{
}

Components::EnterpriseComponent_ptr
Dinner::PhilosopherImpl::CCM_PhilosopherHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new ::Dinner::PhilosopherImpl::CCM_Philosopher_impl;
}

char*
Dinner::PhilosopherImpl::CCM_PhilosopherHome_impl::get_storage_home_id()
{
    return 0;
}

Dinner::CutleryImpl::CCM_CutleryHome_impl::CCM_CutleryHome_impl()
{
}

Dinner::CutleryImpl::CCM_CutleryHome_impl::~CCM_CutleryHome_impl()
{
}

Components::EnterpriseComponent_ptr
Dinner::CutleryImpl::CCM_CutleryHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new ::Dinner::CutleryImpl::CCM_Cutlery_impl;
}

char*
Dinner::CutleryImpl::CCM_CutleryHome_impl::get_storage_home_id()
{
    return 0;
}
