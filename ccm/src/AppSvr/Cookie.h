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

#ifndef __Cookie_h__
#define __Cookie_h__

#include <CORBA.h>
#include <CosTransactions.h>

#include <CompositionManager.h> //added by xiao heping 

namespace Container
{

enum TxnAction
{
	RESUME,
	COMMIT,
	COMMITRESUME,
	NOACTION
};

class Cookie
{
private:
	CosTransactions::Control_var control_;
	CosTransactions::Current_var current_;//added by xiao heping 
	CompositionManager* compositionManager_;//added by xiao heping 
	TxnAction txnAction_;
public:
	Cookie::Cookie(CompositionManager* compositionManager);
	Cookie::~Cookie();

	void control(CosTransactions::Control_ptr control) 
	{ control_ = CosTransactions::Control::_duplicate(control); }

	void txnAction(TxnAction txnAction) 
	{ txnAction_ = txnAction; }

	CosTransactions::Control_ptr control() 
	{ return CosTransactions::Control::_duplicate(control_); }

	TxnAction txnAction() 
	{ return txnAction_; }

	
	CosTransactions::Current_ptr txnCurrent() //added by xiao heping 
	{  
		return CosTransactions::Current::_duplicate(current_);  
	}

	void txnCurrent(CosTransactions::Current_ptr current) //added by xiao heping 
	{  
		current_ = current;  
	}

	CompositionManager_ptr getCompositionManager() //added by xiao heping 
	{  
		return compositionManager_;  
	}
};

}

#endif

