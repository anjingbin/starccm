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

#include <UuidGenerator.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#ifndef WIN32
	#include <sys/time.h>
#else 
	#include <time.h>
#endif

#ifdef WIN32

#define unsigned64_t unsigned __int64
#define I64(C) C

#else

#define unsigned64_t unsigned long long
#define I64(C) C##LL

#endif

/* set the following to the number of 100ns ticks of the actual
resolution of your system's clock */
#define UUIDS_PER_TICK 1

using namespace Components;

UuidGenerator::UuidGenerator()
{
	
}

UuidGenerator::~UuidGenerator()
{
	
}


char*
UuidGenerator::generateUuid()
{
	unsigned64_t time_now;
	
	
#ifdef WIN32

	LARGE_INTEGER time1;
	
	QueryPerformanceCounter((LARGE_INTEGER *)&time1);
	

//	time1.QuadPart +=
//		(unsigned __int64) (1000*1000*10)
//		* (unsigned __int64) (60 * 60 * 24)
//		* (unsigned __int64) (17+30+31+365*18+5);
	
	time_now = time1.QuadPart;
	
#else
	struct timeval tp;
	
	gettimeofday(&tp, (struct timezone *)0);
	
	/* Offset between UUID formatted times and Unix formatted times.
	UUID UTC base time is October 15, 1582.
	Unix base time is January 1, 1970.
	*/
	time_now = (tp.tv_sec * 10000000) + (tp.tv_usec * 10) +
		I64(0x01B21DD213814000);
	
#endif
	
	char b[255];
	
//	time2 = time2/UUIDS_PER_TICK;
	
	unsigned int i = (unsigned int)(((time_now >> 32) ^ time_now)&0xffffffff);
	
//	srand((unsigned int)(((time2 >> 32) ^ time2)&0xffffffff));
	
	sprintf((char*)&b,"%u",i);
	CORBA::String_var s = CORBA::string_dup(b);
	
	return s._retn();
}

