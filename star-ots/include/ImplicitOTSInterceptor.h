// **********************************************************************
//
// Copyright (c) 2002
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#ifndef STAR_IMPLICIT_H
#define STAR_IMPLICIT_H

#include "CosTransactions.h"//renyi 6 18:added

namespace STAR
{

//
// Initialize the OTS. This must be called once per-process.
//
void OTSInit(int&, char**);

} // End namespace STAR

#endif
