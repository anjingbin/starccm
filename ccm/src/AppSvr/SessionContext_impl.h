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

#ifndef __SessionContext_impl_h__
#define __SessionContext_impl_h__

#include <CORBA.h>
#include <EnterpriseComponent.h>
#include <CCMContext_impl.h>


namespace Container
{

class SessionContext_impl : virtual public ::Components::SessionContext,
							virtual public CCMContext_impl
{

protected:
	

public:

	SessionContext_impl();
	virtual ~SessionContext_impl();

//	void setCompositionManager(CompositionManager_ptr current);

//	virtual ::Components::CCMHome_ptr get_CCM_home();

	CORBA::Object_ptr get_CCM_object();

};

}

#endif
