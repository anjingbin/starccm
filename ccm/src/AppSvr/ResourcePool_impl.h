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

#ifndef __ResourcePool_impl_H__
#define __ResourcePool_impl_H__

#include <CORBA.h>
#include <ResourcePool.h>
#include <CosTransactions.h>
#include <ContainerBase_fwd.h>
#include <ContainerRunTime.h>

#ifndef TAO
#include <OTSPostgreSQL.h>
#include "Resource.h"
#include <OTSHeader.h>
#endif



#include <string.h>
#include <fstream>
#include <iostream>

#include <list>
//
// IDL:STARCCM:1.0
//        
namespace STARCCM
{
	
	struct resInfor
	{
		Resource_var res; 
		bool isUsed;
	} ;
	typedef list <resInfor> ResList;		


class ResourcePool_impl: public virtual STARCCM::ResourcePool,
						 REF_COUNT_LOCAL_OBJECT
{
private :
//	CORBA::String_var name_;
//	CORBA::String_var password_;
	
	CORBA::ULong currentCon_;	
	CORBA::ULong initialCon_; 
	CORBA::ULong maxCon_;
	CORBA::ULong inc_;	
		
	ResList resList_;
	static ResourcePool_var resPool_;
	//ResourcePool_var resPool_;
	
#ifndef TAO
	::ResourceFactory_var resourceFactory_;
#endif
	//modified by xiao heping 2004/03/31
//	::Container::ContainerBase_var container_;
	::Container::ContainerBase_ptr container_;
	//end modify xiao heping 2004/03/31
    ResourcePool_impl::ResourcePool_impl(::Container::ContainerRunTime_ptr runtime, CORBA::ULong initialCon, CORBA::ULong maxCon, CORBA::ULong inc);    	         
    ResourcePool_impl::ResourcePool_impl();	
    
public :	
	
	static ResourcePool_ptr getResPoolInstance(::Container::ContainerRunTime_ptr runtime, CORBA::ULong initialCon, CORBA::ULong maxCon, CORBA::ULong inc);
	static ResourcePool_ptr getResPool();
	
	virtual ResourcePool_impl::~ResourcePool_impl();		        
	virtual ::Resource_ptr ResourcePool_impl::getResource()
		        	throw(STARCCM::NoResource);
	void ResourcePool_impl::removeResource(::Resource_ptr res)
		        	throw(STARCCM::RemoveResFailed); 
    
    
    	
        //add by jxh 04/06/14
        CORBA::ULong  ResourcePool_impl::getinc();
        CORBA::ULong  ResourcePool_impl::getmaxcon();
        CORBA::ULong ResourcePool_impl::getcurrentcon();
        CORBA::ULong ResourcePool_impl::getinitialcom();
        

};

} // End of namespace STARCCM

#endif

