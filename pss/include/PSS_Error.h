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


#ifndef _PSS_Error_h_
#define _PSS_Error_h_

namespace PSS
{

void pss_trace(const char*, ...);

}

#ifdef _DEBUG


#define PSSTRACE(x)					pss_trace(x) 
#define PSSTRACE1(x,y)				pss_trace(x,y) 
#define PSSTRACE2(x,y,z)			pss_trace(x,y,z) 
#define PSSTRACE3(x,y,z,w)			pss_trace(x,y,z,w) 

#else

#define PSSTRACE(x)
#define PSSTRACE1(x,y)			
#define PSSTRACE2(x,y,z)
#define PSSTRACE3(x,y,z,w)

#endif

#endif /* #ifndef _PSS_Error_h_ */

