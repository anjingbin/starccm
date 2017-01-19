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

#ifndef ___CCMHome_impl_h__
#define ___CCMHome_impl_h__

#include <CORBA.h>
//#include <CCMHome_skel.h>
#include <CCMObject_skel.h>

namespace Components
{
	class CCMHome_impl:
			virtual public POA_Components::CCMHome,
			virtual public PortableServer::RefCountServantBase
	{
	public:
		CCMHome_impl ();
		~CCMHome_impl ();		
	
	};	

};

#endif

