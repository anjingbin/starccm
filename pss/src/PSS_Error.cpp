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

#ifdef _WIN32
	#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory.h>
#include <PSS_Error.h>

using namespace std;

void 
PSS::pss_trace(const char* format, ...)
{
    va_list ap;
	char buffer[1024];
	// Clear buffer
	memset(buffer,0x00,sizeof(buffer));

	va_start(ap, format);
	
#if defined _WIN32
	_vsnprintf(buffer, sizeof(buffer), format, ap);
    strcat(buffer, "\n\r");
    OutputDebugString(buffer);
#else
	vsnprintf(buffer, sizeof(buffer), format, ap);

#endif

	cerr << buffer << endl;

    va_end(ap);
}		


