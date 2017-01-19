// **********************************************************************
//
// Copyright (c) 2002-2003 
// StarMiddleware.net	
// www.StarMiddleware.net
// 
// All Rights Reserved
//
// Author: Ren Yi	starots@sohu.com	
// **********************************************************************

// Version: 1.0.0

#ifndef OB_IMPLICIT_H
#define OB_IMPLICIT_H

#include "CosTransactions.h"//renyi 6 18:added

namespace OB
{

//
// Initialize the OTS. This must be called once per-process.
//
void OTSInit(int&, char**);

} // End namespace OB

#endif
