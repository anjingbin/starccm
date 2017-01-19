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

#include <eating.xpm>
#include <thinking.xpm>
#include <sleeping.xpm>
#include <hungry.xpm>
#include <hungryleftfork.xpm>
#include <hungrybothfork.xpm>
#include <observer.xpm>
#include <dead.xpm> 

#include <X11/xpm.h>
#include <Xm/XmAll.h>
#define SLEEP(sec) sleep(sec)

#endif

#include <CORBA.h>
#include <CCM_Dinner_impl.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>

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

typedef map<HWND, Dinner::PhilosopherImpl::CCM_Philosopher_impl*> HWndPhMap;
HWndPhMap hWndPhMap;

volatile HWND hWndOb = NULL;

LRESULT CALLBACK PhWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ObWndProc(HWND, UINT, WPARAM, LPARAM);
#else

#define WM_EATING 100
#define WM_HUNGRY 101
#define WM_THINKING 102
#define WM_SLEEPING 103
#define WM_DEAD 104
#define WM_HUNGRY_LEFTFORK 105
#define WM_HUNGRY_BOTHFORK 106
#define WM_STATUSCHANGE 107

typedef map<Widget, Dinner::PhilosopherImpl::CCM_Philosopher_impl*> HWndPhMap;
HWndPhMap hWndPhMap;

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
	Arg args[8];
	int n = 0;
	unsigned int pixmap_error;

	Pixmap mask;
	XpmAttributes xpma;
	
	XtAppContext appContext;
	this_ -> hWnd_ = XtAppInitialize(&appContext,"PhilosopherApp", 0, 0, &n, 0,  0,  0,  0);

	XtVaSetValues(this_ -> hWnd_, XtNbackground, 0xffffff, XmNwidth,180,XmNheight,190,0);
	XtVaSetValues(this_ -> hWnd_, XtNtitle, this_ -> name(), 0);

	this_ -> dpy_ = XtDisplay(hWnd_);

	if ((pixmap_error = XCreatePixmapFromData(this_ -> dpy_, DefaultRootWindow(dpy), 
		eating, &(this_ -> hEating_), &mask, &xpma)) == PixmapSuccess) 
	{
		printf("ok\n");
	}

	XFreePixmap(this_ -> dpy_, mask);

	if ((pixmap_error = XCreatePixmapFromData(this_ -> dpy_, DefaultRootWindow(this_ -> dpy_), 
		thinking, &(this_ -> hThinking_), &mask, &xpma)) == PixmapSuccess) 
	{
		printf("ok\n");
	} 
	else
	{
		printf ("Couldn't load bitmap\n");
	}

	XFreePixmap(this_ -> dpy_, mask);

	if ((pixmap_error = XCreatePixmapFromData(this_ -> dpy_, DefaultRootWindow(this_ -> dpy_), 
		sleeping, &(this_ -> hSleeping_), &mask, &xpma)) == PixmapSuccess) 
	{
		printf("ok\n");
	} 
	else
	{
		printf ("Couldn't load bitmap\n");
	}

	XFreePixmap(this_ -> dpy_, mask);

	if ((pixmap_error = XCreatePixmapFromData(this_ -> dpy_, DefaultRootWindow(this_ -> dpy_), 
		hungry, &(this_ -> hHungry_), &mask, &xpma)) == PixmapSuccess) 
	{
		printf("ok\n");
	} 
	else
	{
		printf ("Couldn't load bitmap\n");
	}

	XFreePixmap(this_ -> dpy_, mask);

	if ((pixmap_error = XCreatePixmapFromData(this_ -> dpy_, DefaultRootWindow(this_ -> dpy_), 
		hungryleftfork, &(this_ -> hHungryLeftFork_), &mask, &xpma)) == PixmapSuccess) 
	{
		printf("ok\n");
	} 
	else
	{
		printf ("Couldn't load bitmap\n");
	}

	XFreePixmap(this_ -> dpy_, mask);

	if ((pixmap_error = XCreatePixmapFromData(this_ -> dpy_, DefaultRootWindow(this_ -> dpy_), 
		hungrybothfork, &(this_ -> hHungryBothFork_), &mask, &xpma)) == PixmapSuccess) 
	{
		printf("ok\n");
	} 
	else
	{
		printf ("Couldn't load bitmap\n");
	}

	XFreePixmap(this_ -> dpy_, mask);

	if ((pixmap_error = XCreatePixmapFromData(this_ -> dpy_, DefaultRootWindow(this_ -> dpy_), 
		dead, &(this_ -> hDead_), &mask, &xpma)) == PixmapSuccess) 
	{
		printf("ok\n");
	} 
	else
	{
		printf ("Couldn't load bitmap\n");
	}

	XFreePixmap(this_ -> dpy_, mask);

	Widget drawingW = XmCreateDrawingArea(hWnd_, "drawingW", 0, 0);

	XtVaSetValues(drawingW, XtNbackground, 0xffffff, XmNwidth, 180, XmNheight, 170,0);

	XtAddCallback(drawingW, XmNexposeCallback, exposeResizeProc, NULL);
	XtAddCallback(drawingW, XmNresizeCallback, exposeResizeProc, NULL);

	XtAddEventHandler(drawingW, NoEventMask, True, clientMsgHandler, NULL);
    
	gc_ = XtGetGC(drawingW, 0, 0);

	XSetBackground(dpy_ , gc_ , 0xffffff);

	XtManageChild(drawingW) ;

	this_ -> hWnd2_ = this_ -> hWnd_;
	this_ -> hWnd_ = drawingW;

	hWndPhMap.insert(HWndPhMap::value_type(drawingW ,this_));

	int ret = pthread_cond_signal(&(this_ -> hEvent2_));
	ASSERT(!ret);

	XtRealizeWidget(hWnd_);
	XtAppMainLoop (appContext);
	
	hWndPhMap.erase(drawingW);

	XFreePixmap(this_ -> dpy_, hHungry_);
	XFreePixmap(this_ -> dpy_, hEating_);
	XFreePixmap(this_ -> dpy_, hSleeping_);
	XFreePixmap(this_ -> dpy_, hThinking_);
	XFreePixmap(this_ -> dpy_, hDead_);
	XFreePixmap(this_ -> dpy_, hHungryLeftFork_);
	XFreePixmap(this_ -> dpy_, hHungryRightFork_);
	XFreePixmap(this_ -> dpy_, hHungryBothFork_);

	XtReleaseGC(gc_);

	pthread_exit(0);
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
		SendMessage(itemWnd, WM_SETTEXT, 0, (LPARAM)"");
		SendMessage(itemWnd, EM_SETSEL, 0, 0);
		SendMessage(itemWnd, EM_REPLACESEL, 0, lParam);
		SendMessage(itemWnd, EM_SETREADONLY, (WPARAM)TRUE, 0);

		return TRUE;
	}

	return FALSE;
}
#else

void 
exposeResizeProc(Widget drawingW, XtPointer client_data, XtPointer call_data)
{
	HWndPhMap::iterator it;
	it = hWndPhMap.find(drawingW);

	Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_ = it -> second;

	XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) call_data;

	if(cbs -> reason == XmCR_EXPOSE) 
	{
		//XSetClipMask(XtDisplay(drawingW) , gc, mask);
        //XSetClipOrigin(XtDisplay(drawingW) , gc, 0, 0);
		XCopyArea(XtDisplay(drawingW), this_ -> hBitmap_, cbs -> window, this_ -> gc_, 0, 0, 180, 170, 0, 0);
		return;
	}
}

void clientMsgHandler(Widget drawingW, XtPointer client_data, XEvent* event)
{
	HWndPhMap::iterator it;
	it = hWndPhMap.find(hWnd);
	Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_ = it -> second;

	if(event -> type != ClientMessage)
		return;

	if(event -> xclient.message_type == WM_EATING)
	{
		this_ -> hBitmap_ = this_ -> hEating_;
	}
	else if(event -> xclient.message_type == WM_HUNGRY)
	{
		this_ -> hBitmap_ = this_ -> hHungry;
	}
	else if(event -> xclient.message_type == WM_THINKING)
	{
		this_ -> hBitmap_ = this_ -> hThinking;
	}
	else if(event -> xclient.message_type == WM_SLEEPING)
	{
		this_ -> hBitmap_ = this_ -> hSleeping;
	}
	else if(event -> xclient.message_type == WM_HUNGRY_LEFTFORK)
	{
		this_ -> hBitmap_ = this_ -> hHungryLeftFork;
	}
	else if(event -> xclient.message_type == WM_HUNGRY_BOTHFORK)
	{
		this_ -> hBitmap_ = this_ -> hHungryBothFork;
	}
	else if(event -> xclient.message_type == WM_DEAD)
	{
		this_ -> hBitmap_ = this_ -> hDead;
	}
	XmRedisplayWidget(drawingW);
}

#endif

unsigned int 
ObGUIThreadProc(Dinner::ObserverImpl::CCM_Observer_impl* this_)
{
#ifdef WIN32
	DialogBox(hInst,MAKEINTRESOURCE(IDD_OBSERVER),NULL,(DLGPROC)OberserDialogProc);
	_endthread();
#else
	Arg args[8];
	int n = 0;
	unsigned int pixmap_error;

	Pixmap mask;
	XpmAttributes xpma;
	
	XtAppContext appContext;
	this_ -> hWnd_ = XtAppInitialize(&appContext,"ObserverApp", 0, 0, &n, 0,  0,  0,  0);

	XtVaSetValues(hWnd_, XtNtitle, "Observer", 0);

	this_ -> dpy_ = XtDisplay(hWnd_);

	if ((pixmap_error = XCreatePixmapFromData(this_ -> dpy_, DefaultRootWindow(dpy), 
		eating, &(this_ -> hObserver_), &mask, &xpma)) == PixmapSuccess) 
	{
		printf("ok\n");
	}

	XFreePixmap(this_ -> dpy_, mask);
	
	Widget form = XmCreateForm(this_ -> hWnd_, "form", 0, 0);

	n = 0;
	XtSetArg(args[n],XmNlabelType, XmPIXMAP); n++;
	XtSetArg(args[n],XmNlabelPixmap, this_ -> hObserver_); n++;
	XtSetArg(args[n],XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n],XmNtopOffset, 10); n++;
	XtSetArg(args[n],XmNleftAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n],XmNleftOffset, 10); n++;
	Widget label = XmCreateLabel(form, "label", args, n);
	
	n = 0;
	XtSetArg(args[n],XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n],XmNtopOffset, 10); n++;
	XtSetArg(args[n],XmNleftWidget, label); n++;
	XtSetArg(args[n],XmNleftAttachment, XmATTACH_WIDGET); n++;
	XtSetArg(args[n],XmNleftOffset, 10); n++;
	XtSetArg(args[n],XmNeditMode, XmMULTI_LINE_EDIT); n++;
	
	XtSetArg(args[n],XmNwidth, 200); n++;
	XtSetArg(args[n],XmNheight, 200); n++;

	XtSetArg(args[n],XmNeditable, False); n++;

	XtSetArg(args[n],XmNwordWrap, True); n++;

	XtSetArg(args[n],XmNcursorPositionVisible , True); n++;
	
	Widget edit =  XmCreateText(form, "text", args, n);
	
	this_ -> hEdit_ = edit;

	XtManageChild(label);
	XtManageChild(edit);
	XtManageChild(form);

	//XmTextSetString(edit, hello);
	//XtVaSetValues(edit, XmNcursorPosition, strlen(hello), 0);

	XtRealizeWidget(this_ -> hWnd_);
	XtAppMainLoop (appContext);

	XFreePixmap(this_ -> dpy_, this_ -> hObserver_);

#endif
	return 0;
}

unsigned int 
philosopherAction(::Dinner::PhilosopherImpl::CCM_Philosopher_impl* this_)
{
	TRACE1(LEVEL1,"PHILOSOPHER %s : i'm born\n", this_ -> name_);
	
	::Dinner::CCM_Philosopher_Context_var ctx = this_ -> ctx_;
	
	::Dinner::PhilosopherState_var thinking = new ::Dinner::PhilosopherState_impl(::Dinner::THINKING,this_ -> name(),NULL);
	::Dinner::PhilosopherState_var hungry = new ::Dinner::PhilosopherState_impl(::Dinner::HUNGRY,this_ -> name(),NULL);
	::Dinner::PhilosopherState_var eating = new ::Dinner::PhilosopherState_impl(::Dinner::EATING,this_ -> name(),NULL);
	::Dinner::PhilosopherState_var sleeping = new ::Dinner::PhilosopherState_impl(::Dinner::SLEEPING,this_ -> name(),NULL);
	::Dinner::PhilosopherState_var dead = new ::Dinner::PhilosopherState_impl(::Dinner::DEAD,this_ -> name(),NULL);


	#ifdef WIN32
	//avoid race condition.
	//be care of the `PostMessage' goes faster than Philosogher's HWND mapping to their executors. 
	WaitForSingleObject(this_ -> hEvent2_,INFINITE);
	CloseHandle(this_ -> hEvent2_);
	#else
	pthread_cond_wait(&(this_ -> hEvent2_),NULL);
	pthread_cond_destroy(&(this_ -> hEvent2_));
	#endif

	#ifdef WIN32
	#else
	XClientMessageEvent event;
	event.display = XDisplay(this_ -> hWnd_);
	event.send_event = True;
	event.window = XWindow(this_ -> hWnd_);
	event.format = 8;
	#endif

	while(!this_ -> dead_)
	{
		while(!this_ -> dynState_)
		{
			TRACE1(LEVEL1,"PHILOSOPHER %s : i'm thinking\n", this_ -> name_);
			
			#ifdef WIN32
			PostMessage(this_ -> hWnd_, WM_THINKING, 0, 0);
			#else
			event.message_type = WM_THINKING;
			XSendEvent(event.display, event.window, True, NoEventMask, &event);
			#endif
	
			ctx -> push_philosopher_state(thinking.in());
			
			SLEEP(this_ -> thinking_seconds());
			
			TRACE1(LEVEL1,"PHILOSOPHER %s : i'm hungry\n", this_ -> name_);
			
			#ifdef WIN32
			PostMessage(this_ -> hWnd_, WM_HUNGRY, 0, 0);
			#else
			event.message_type = WM_HUNGRY;
			XSendEvent(event.display, event.window, True, NoEventMask, &event);
			#endif
	
			ctx -> push_philosopher_state(hungry.in());
			
			SLEEP(1);
	
			Components::Cookie_var leftCookie;
			Components::Cookie_var rightCookie;
			
			::Dinner::Fork_var leftHand = ctx -> get_connection_left_hand();
			::Dinner::Fork_var rightHand = ctx -> get_connection_right_hand();
			
			ASSERT(!CORBA::is_nil(leftHand));
			ASSERT(!CORBA::is_nil(rightHand));
	
			bool waitingFork = false;
			bool firstTringToGetFork = true;

			//deleted by xiao heping 2004/06/03 
			//int haveTryedTimes = 0;
	
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
					TRACE1(LEVEL6,"PHILOSOPHER %s : i'm trying to obtain left fork\n", this_ -> name_);
					leftCookie = leftHand -> obtain_fork();
					
					#ifdef WIN32
					PostMessage(this_ -> hWnd_, WM_HUNGRY_LEFTFORK, 0, 0);
					#else
					event.message_type = WM_HUNGRY_LEFTFORK;
					XSendEvent(event.display, event.window, True, NoEventMask, &event);
					#endif
					
					TRACE1(LEVEL6,"PHILOSOPHER %s : i have obtained left fork\n", this_ -> name_);

					//deleted by xiao heping 2004/06/03 for avoid deadlock 
					//if(haveTryedTimes < 5) 
					SLEEP(1);
				}
				catch(::Dinner::ForkNotAvailable&)
				{
					waitingFork = true;
					TRACE1(LEVEL6,"PHILOSOPHER %s : i have failed to obtain left fork\n", this_ -> name_);

					//deleted by xiao heping 2004/06/03
					//haveTryedTimes ++;
					continue;
				}
				catch(...)
				{
					goto RetryGetLeft;
				}
	RetryGetRight:
				try
				{
					TRACE1(LEVEL6,"PHILOSOPHER %s : i'm trying to obtain right fork\n", this_ -> name_);
					rightCookie = rightHand -> obtain_fork();
					
					#ifdef WIN32
					PostMessage(this_ -> hWnd_, WM_HUNGRY_BOTHFORK, 0, 0);
					#else
					event.message_type = WM_HUNGRY_BOTHFORK;
					XSendEvent(event.display, event.window, True, NoEventMask, &event);
					#endif
					
					TRACE1(LEVEL6,"PHILOSOPHER %s : i have obtained right fork\n", this_ -> name_);
					//deleted by xiao heping 2004/06/03 for avoid deadlock 
					//if(haveTryedTimes < 5) 
					SLEEP(1);
				}
				catch(::Dinner::ForkNotAvailable&)
				{
					waitingFork = true;
					TRACE1(LEVEL6,"PHILOSOPHER %s : i have failed to obtain right fork\n", this_ -> name_);
	
	RetryReleaseLeft:
	
					try
					{
						TRACE1(LEVEL2,"PHILOSOPHER %s : i'm trying to release left fork\n", this_ -> name_);
						leftHand -> release_fork(leftCookie.in());
						
						#ifdef WIN32
						PostMessage(this_ -> hWnd_, WM_HUNGRY, 0, 0);
						#else
						event.message_type = WM_HUNGRY;
						XSendEvent(event.display, event.window, True, NoEventMask, &event);
						#endif
						
						TRACE1(LEVEL2,"PHILOSOPHER %s : i have released left fork\n", this_ -> name_);

						//deleted by xiao heping 2004/06/03 for avoid deadlock 
						//if(haveTryedTimes < 5)  
						SLEEP(1);
					}
					catch(::Dinner::NotTheEater&)
					{
						ASSERT(false);
					}
					catch(...)
					{
						goto RetryReleaseLeft;
					}
					//deleted by xiao heping 2004/06/03 for avoid deadlock 
					//haveTryedTimes ++;
					continue;
				}
				catch(...)
				{
					goto RetryGetRight;
				}
	
				break;
			}
			//deleted by xiao heping 2004/06/03
			//haveTryedTimes = 0;
	
			TRACE1(LEVEL1,"PHILOSOPHER %s : i'm eating\n", this_ -> name_);
			
			#ifdef WIN32
			PostMessage(this_ -> hWnd_, WM_EATING, 0, 0);
			#else
			event.message_type = WM_EATING;
			XSendEvent(event.display, event.window, True, NoEventMask, &event);
			#endif
	
			ctx -> push_philosopher_state(eating.in());
	
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
			event.message_type = WM_SLEEPING;
			XSendEvent(event.display, event.window, True, NoEventMask, &event);
			#endif
	
			ctx -> push_philosopher_state(sleeping.in());
	
			SLEEP(this_ -> sleeping_seconds());
	
			waitingFork = false;
			firstTringToGetFork = true;
			
			
			if(this_ -> dynState_)
			{
			#ifdef WIN32
				BOOL ret = SetEvent(this_ -> hDynEvent_);
				ASSERT(ret);
			#else
				int ret = pthread_cond_signal(&(this_ -> hDynEvent_));
				ASSERT(!ret);
			#endif
			}
		}
		
		if(this_ -> dynState_)
		{
			SLEEP(1);
		}
	}
	
	TRACE1(LEVEL1,"PHILOSOPHER %s : i'm dead", this_ -> name_);

	#ifdef WIN32
	PostMessage(this_ -> hWnd_, WM_DEAD, 0, 0);
	PostMessage(this_ -> hWnd_, WM_DESTROY, 0, 0);
	#else
	event.message_type = WM_DEAD;
	XSendEvent(event.display, event.window, True, NoEventMask, &event);
	#endif

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

void 
Dinner::PhilosopherImpl::CCM_Philosopher_impl::passivate_component()
	throw(CORBA::SystemException)
{
	dynState_ = true;
#ifdef WIN32	
	WaitForSingleObject(hDynEvent_,50000);
#else
	//how to set timeout??? huangjie 2004.5.24
	pthread_cond_wait((char*)this + (int)&hDynEvent_,NULL); 
#endif

}

void 
Dinner::PhilosopherImpl::CCM_Philosopher_impl::activate_component()
	throw(CORBA::SystemException)
{
	dynState_ = false;
}

Dinner::PhilosopherImpl::CCM_Philosopher_impl::CCM_Philosopher_impl()
	:dead_(false),dynState_(false),thinking_seconds_(1),eating_seconds_(1),sleeping_seconds_(1)
{
#ifdef WIN32
	hThread_ = NULL;
	hGUIThread_ = NULL;
	hWnd_ = NULL;
	threadGUIId_ = 0;
	hEvent_ = CreateEvent(NULL,FALSE,FALSE,NULL);
	hEvent2_ = CreateEvent(NULL,FALSE,FALSE,NULL);
	hDynEvent_ = CreateEvent(NULL,FALSE,FALSE,NULL);
#else
	hThread_ = 0;
	hGUIThread_ =0;
	pthread_cond_init((char*)this + (int)&hEvent_,NULL);
	pthread_cond_init((char*)this + (int)&hEvent2_,NULL);
	pthread_cond_init((char*)this + (int)&hDynEvent_,NULL);
#endif
}

Dinner::PhilosopherImpl::CCM_Philosopher_impl::~CCM_Philosopher_impl()
{
	dead_ = true;
	
	//conflict with dynamic deploy
	//ctx -> push_philosopher_state(dead.in());
	//SLEEP(1);
	
#ifdef WIN32
	WaitForSingleObject(hEvent_, INFINITE);
	CloseHandle(hEvent_);
	CloseHandle(hEvent2_);
	CloseHandle(hDynEvent_);
	//if(hThread_) CloseHandle(hThread_);
	//if(hGUIThread_) CloseHandle(hGUIThread_);
#else
	pthread_cond_wait((char*)this + (int)&hEvent_, NULL);
	pthread_cond_destroy((char*)this + (int)&hEvent_);
	pthread_cond_destroy((char*)this + (int)&hEvent2_);
	pthread_cond_destroy((char*)this + (int)&hDynEvent_);
	pthread_attr_destroy((char*)this + (int)&attr_);
#endif
}

void
Dinner::PhilosopherImpl::CCM_Philosopher_impl::configuration_complete()
	throw(CORBA::SystemException, Components::InvalidConfiguration)
{
	//	SECURITY_ATTRIBUTES secAttr;
	//	secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	//	secAttr.lpSecurityDescriptor = NULL;
	//	secAttr.bInheritHandle = TRUE;	

#ifdef WIN32
	hGUIThread_ = _beginthreadex(NULL,0,(THREADROUTING)PhGUIThreadProc, this, 0, &threadGUIId_); 
	ASSERT(hGUIThread_);

	unsigned int threadId;
	hThread_ = _beginthreadex(NULL,0,(THREADROUTING)philosopherAction, this,0, &threadId); 
	ASSERT(hThread_ != NULL);
#else
	pthread_attr_init((char*)this + (int)&attr_);
	pthread_attr_setdetachstate((char*)this + (int)&attr_ ,PTHREAD_CREATE_DETACHED);
	pthread_attr_setscope((char*)this + (int)&attr_, PTHREAD_SCOPE_SYSTEM);

	int ret = pthread_create((char*)this + (int)&hGUIThread_, (char*)this + (int)&attr, PhGUIThreadProc, this);
	ASSERT(ret == 0);

	int ret = pthread_create((char*)this + (int)&hThread_, (char*)this + (int)&attr, philosopherAction, this);
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

void Dinner::PhilosopherImpl::CCM_Philosopher_impl::ccm_remove()
{
//	#ifdef WIN32
//	PostMessage(this -> hWnd_, WM_DESTROY, 0, 0);
//	TerminateThread((void *)hThread_, 1);
//	#endif
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

Dinner::ObserverImpl::CCM_Observer_impl::CCM_Observer_impl()
{
#ifdef WIN32
	hGUIThread_ = NULL;
	threadGUIId_ = 0;
	hGUIThread_ = _beginthreadex(NULL,0,(THREADROUTING)ObGUIThreadProc,this,0,&threadGUIId_); 
	ASSERT(hGUIThread_);
#else
#endif
}

Dinner::ObserverImpl::CCM_Observer_impl::~CCM_Observer_impl()
{
	#ifdef WIN32
	SendMessage(hWndOb, WM_COMMAND, (WPARAM)IDOK, 0);
	#else
	DestroyWidget(hWnd_);
	#endif
	//if(hGUIThread_) 
		//CloseHandle(hGUIThread_);
}

void
Dinner::ObserverImpl::CCM_Observer_impl::push_philosopher_state(Dinner::PhilosopherState* event)
    throw(CORBA::SystemException)
{
	static char* buf = new char[2048];
	static int msgOffset = 0;
	static int tailOffset = 0;

	LOCK

	CORBA::String_var s = (const char *)"Philosopher ";	
	s += event -> name();
	
	switch( event -> status() )
	{
		case EATING:
			s += " is eating";
			break;
		case THINKING:
			s += " is thinking";
			break;
		case SLEEPING:
			s += " is sleeping";
			break;
		case HUNGRY:
			s += " is hungry";
			break;
		case DEAD:
			s += " is dead";
			break;
		break;
	}
	
	TRACE1(LEVEL0,"Observor : %s\n", s.in());

	s += "\r\n";

	if((int)(tailOffset + strlen(s.in()) + 1) > 2048)
	{
		memcpy(buf, buf + 1024, 1024);
		tailOffset -= 1024;
	}

	memcpy((char*)(buf + tailOffset), s.in(), strlen(s.in())+1);
	tailOffset += strlen(s.in());
	msgOffset = (int)(tailOffset - 1024);
	if(msgOffset < 0) msgOffset = 0;
	
	#ifdef WIN32
	SendMessage(hWndOb, WM_STATUSCHANGE, 0, (LPARAM)(buf + msgOffset));
	#else
	XmTextSetString(hEdit_, s.in());
	XtVaSetValues(hEdit_, XmNcursorPosition, strlen(s.in()), 0);
	#endif

#ifndef _DEBUG
	cout<<"Observor : "<<s<<endl;
#endif
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

Dinner::ObserverImpl::CCM_ObserverHome_impl::CCM_ObserverHome_impl()
{
}

Dinner::ObserverImpl::CCM_ObserverHome_impl::~CCM_ObserverHome_impl()
{
}

Components::EnterpriseComponent_ptr
Dinner::ObserverImpl::CCM_ObserverHome_impl::create()
    throw(Components::CCMException,
          CORBA::SystemException)
{
    return new ::Dinner::ObserverImpl::CCM_Observer_impl;
}

char*
Dinner::ObserverImpl::CCM_ObserverHome_impl::get_storage_home_id()
{
    return 0;
}

