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

#include <Cookie.h>

using namespace Container;

Cookie::Cookie(CompositionManager* compositionManager)
:compositionManager_(compositionManager)
{
	txnAction_ = NOACTION;
	control_ = 0;
}

Cookie::~Cookie()
{
}

