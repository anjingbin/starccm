// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Huang Jie    huangjie@email.com
// Author: Wang Kebo    mep@263.net
// Author: Su Liang     centuryfree@yahoo.com.cn
//
// **********************************************************************

// Version: 1.0.0

#ifndef __Trace_h__
#define __Trace_h__

#include <CORBA.h>

#ifdef _DEBUG

//LEVEL0：关闭所有输出；
//LEVEL1：发生致命错或异常（使得系统不可能继续运行下去）；
//LEVEL2：发生错误或异常，需要提示用户；
//LEVEL3：输出系统运行的提示信息（比如：系统启动）；
//LEVEL4：指明系统正在进行的比较具体的动作（比如：容器初始化）；
//LEVEL5：输出trace的具体的位置，比如：输出方法名；
//LEVEL6：输出具体的参数的值；

#define LEVEL0 0
#define LEVEL1 1
#define LEVEL2 2
#define LEVEL3 3
#define LEVEL4 4
#define LEVEL5 5
#define LEVEL6 6

#include <stdio.h>
#include <string.h>
#include <assert.h>

CCM_IMPORT extern int debugLevel;
CCM_IMPORT extern char* debugModule;
CCM_IMPORT extern char* noDebugModule;
CCM_IMPORT extern bool debugAllModule;
CCM_IMPORT extern bool debugLogFile;
CCM_IMPORT extern bool withFileName;
CCM_IMPORT extern bool withLineNumber;
CCM_IMPORT extern FILE* logFileName; 

CCM_IMPORT char* noPathFileName(char* file);
CCM_IMPORT bool checkDebugLevel(int lvl);
CCM_IMPORT bool checkModule(const char* file);

#ifdef WIN32

// Modified by WangKebo, for compilation on TAO
// Windows.h conflict with ACE/TAO
//#include <Windows.h>
#define _THREAD_ID GetCurrentThreadId()

#define _SPIN_LOCK_STRING \
		__asm{ \
			__asm L1: \
				__asm lock dec lock_ \
				__asm js L2 \
				__asm jmp L3 \
			__asm L2: \
				__asm cmp lock_ ,0x0  \
				__asm rep nop \
				__asm jle L2 \
				__asm jmp L1 \
			__asm L3: \
			}

#define _SPIN_UNLOCK_STRING __asm{ \
								__asm mov lock_, 0x1 \
								}

#else /*#ifdef WIN32*/

#include <pthread.h>
#define _THREAD_ID pthread_self()

#ifdef LINUX

#define _SPIN_LOCK_STRING \
	__asm__ __volatile__( \
	"\n1:\t" \
	"lock ; decb %0\n\t" \
	"js 2f\n" \
	".subsection 1\n" \
	"2:\t" \
	"cmpb $0,%0\n\t" \
	"rep;nop\n\t" \
	"jle 2b\n\t" \
	"jmp 1b\n" \
	".subsection 0\n" \
	:"=m" (lock_) : : "memory" \
	);

#define _SPIN_UNLOCK_STRING \
	__asm__ __volatile__( \
	"movb $1,%0" \
		:"=m" (lock->lock) : : "memory" \
	); 

#endif /*#ifdef LINUX*/

#define _SPIN_LOCK_STRING 
#define _SPIN_UNLOCK_STRING

#endif /*#ifdef WIN32*/

CCM_IMPORT extern volatile unsigned int lock_;

class SpinLock
{
public:
	SpinLock(){ _SPIN_LOCK_STRING }
	~SpinLock(){ _SPIN_UNLOCK_STRING }
};

#define _TRACE_BEGIN(lvl)	do{ \
							if(checkDebugLevel(lvl) && \
								checkModule(noPathFileName(__FILE__))) \
							{ \
								SpinLock lock;

#define _TRACE_END		} \
					} while(0)

#define TRACE0(lvl,s)	_TRACE_BEGIN(lvl) \
						if(logFileName != NULL) \
						{ \
							if(withFileName) \
							{ \
								fprintf(logFileName,"%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								fprintf(logFileName," %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								fprintf(logFileName,"\n"); \
							fprintf(logFileName,"T@%d ",_THREAD_ID); \
							fprintf(logFileName,s); \
							fflush(logFileName); \
						} \
						else \
						{ \
							if(withFileName) \
							{ \
								printf("%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								printf(" %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								printf("\n"); \
							printf("T@%d ",_THREAD_ID); \
							printf(s); \
						} \
						_TRACE_END

#define TRACE1(lvl,s,v1)	_TRACE_BEGIN(lvl) \
						if(logFileName != NULL) \
						{ \
							if(withFileName) \
							{ \
								fprintf(logFileName,"%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								fprintf(logFileName," %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								fprintf(logFileName,"\n"); \
							fprintf(logFileName,"T@%d ",_THREAD_ID); \
							fprintf(logFileName,s,v1); \
							fflush(logFileName); \
						} \
						else \
						{ \
							if(withFileName) \
							{ \
								printf("%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								printf(" %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								printf("\n"); \
							printf("T@%d ",_THREAD_ID); \
							printf(s,v1); \
						} \
						_TRACE_END

#define TRACE2(lvl,s,v1,v2)	_TRACE_BEGIN(lvl) \
						if(logFileName != NULL) \
						{ \
							if(withFileName) \
							{ \
								fprintf(logFileName,"%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								fprintf(logFileName," %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								fprintf(logFileName,"\n"); \
							fprintf(logFileName,"T@%d ",_THREAD_ID); \
							fprintf(logFileName,s,v1,v2); \
							fflush(logFileName); \
						} \
						else \
						{ \
							if(withFileName) \
							{ \
								printf("%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								printf(" %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								printf("\n"); \
							printf("T@%d ",_THREAD_ID); \
							printf(s,v1,v2); \
						} \
						_TRACE_END

#define TRACE3(lvl,s,v1,v2,v3)	_TRACE_BEGIN(lvl) \
						if(logFileName != NULL) \
						{ \
							if(withFileName) \
							{ \
								fprintf(logFileName,"%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								fprintf(logFileName," %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								fprintf(logFileName,"\n"); \
							fprintf(logFileName,"T@%d ",_THREAD_ID); \
							fprintf(logFileName,s,v1,v2,v3); \
							fflush(logFileName); \
						} \
						else \
						{ \
							if(withFileName) \
							{ \
								printf("%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								printf(" %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								printf("\n"); \
							printf("T@%d ",_THREAD_ID); \
							printf(s,v1,v2,v3); \
						} \
						_TRACE_END

#define TRACE4(lvl,s,v1,v2,v3,v4)	_TRACE_BEGIN(lvl) \
						if(logFileName != NULL) \
						{ \
							if(withFileName) \
							{ \
								fprintf(logFileName,"%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								fprintf(logFileName," %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								fprintf(logFileName,"\n"); \
							fprintf(logFileName,"T@%d ",_THREAD_ID); \
							fprintf(logFileName,s,v1,v2,v3,v4); \
							fflush(logFileName); \
						} \
						else \
						{ \
							if(withFileName) \
							{ \
								printf("%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								printf(" %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								printf("\n"); \
							printf("T@%d ",_THREAD_ID); \
							printf(s,v1,v2,v3,v4); \
						} \
						_TRACE_END

#define TRACE5(lvl,s,v1,v2,v3,v4,v5)	_TRACE_BEGIN(lvl) \
						if(logFileName != NULL) \
						{ \
							if(withFileName) \
							{ \
								fprintf(logFileName,"%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								fprintf(logFileName," %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								fprintf(logFileName,"\n"); \
							fprintf(logFileName,"T@%d ",_THREAD_ID); \
							fprintf(logFileName,s,v1,v2,v3,v4,v5); \
							fflush(logFileName); \
						} \
						else \
						{ \
							if(withFileName) \
							{ \
								printf("%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								printf(" %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								printf("\n"); \
							printf("T@%d ",_THREAD_ID); \
							printf(s,v1,v2,v3,v4,v5); \
						} \
						_TRACE_END

#define TRACE6(lvl,s,v1,v2,v3,v4,v5,v6)	_TRACE_BEGIN(lvl) \
						if(logFileName != NULL) \
						{ \
							if(withFileName) \
							{ \
								fprintf(logFileName,"%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								fprintf(logFileName," %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								fprintf(logFileName,"\n"); \
							fprintf(logFileName,"T@%d ",_THREAD_ID); \
							fprintf(logFileName,s,v1,v2,v3,v4,v5,v6); \
							fflush(logFileName); \
						} \
						else \
						{ \
							if(withFileName) \
							{ \
								printf("%s",__FILE__); \
							} \
							if(withLineNumber) \
							{ \
								printf(" %d",__LINE__); \
							} \
							if(withFileName||withLineNumber) \
								printf("\n"); \
							printf("T@%d ",_THREAD_ID); \
							printf(s,v1,v2,v3,v4,v5,v6); \
						} \
						_TRACE_END

#define ASSERT(exp) assert(exp)

class InitHelper
{
	void LoadProfile();
public:
	InitHelper();
};

#else /*#ifdef _DEBUG*/

#define TRACE0(lvl,s)
#define TRACE1(lvl,s,v1)
#define TRACE2(lvl,s,v1,v2)
#define TRACE3(lvl,s,v1,v2,v3)
#define TRACE4(lvl,s,v1,v2,v3,v4)
#define TRACE5(lvl,s,v1,v2,v3,v4,v5)
#define TRACE6(lvl,s,v1,v2,v3,v4,v5,v6)

//#define TRACE0(lvl,s) printf(s)
//#define TRACE1(lvl,s,v1) printf(s,v1)
//#define TRACE2(lvl,s,v1,v2) printf(s,v1,v2)
//#define TRACE3(lvl,s,v1,v2,v3) printf(s,v1,v2,v3)
//#define TRACE4(lvl,s,v1,v2,v3,v4) printf(s,v1,v2,v3,v4)
//#define TRACE5(lvl,s,v1,v2,v3,v4,v5) printf(s,v1,v2,v3,v4,v5)
//#define TRACE6(lvl,s,v1,v2,v3,v4,v5,v6) printf(s,v1,v2,v3,v4,v5,v6)

#define ASSERT(exp)

#endif /*#ifdef _DEBUG*/

#endif /*#ifndef __Trace_h__*/

