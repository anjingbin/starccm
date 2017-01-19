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

#include <PassivationArbitor.h>

using namespace Container;

PassivationArbitor::PassivationArbitor()
{
	
}

PassivationArbitor::~PassivationArbitor()
{
	
}

int
PassivationArbitor::canPassiavte()
{
	switch(policy_)
	{
		case COMPONENTCOUNT:
			if( componentCount_ >= count_ )
			{
				return 1;
			}
			return 0;
		default:
			return 0;
	}
	
	return 0;
}

void
PassivationArbitor::setPassivatePolicy(PASSIVATEPOLICY policy)
{
	policy_ = policy;
}

void
PassivationArbitor::onCreate()
{
	componentCount_++;
}

void
PassivationArbitor::onRemove()
{
	componentCount_--;
}

void
PassivationArbitor::setComponentCount(int count)
{
	count_ = count;
}

