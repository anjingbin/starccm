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

#ifndef ___Configurator_impl_h__
#define ___Configurator_impl_h__

#include <CORBA.h>
#include <Configurator_skel.h>
#include <CCMHome_impl.h>
#include <ComponentServentBase.h>

using namespace Components;

namespace Components
{

	class ConfigValue_impl:
			virtual public OBV_Components::ConfigValue,
                    	virtual public CORBA::DefaultValueRefCountBase
        {
	public:

		
		ConfigValue_impl();
		
		ConfigValue_impl(const char * name,const CORBA::Any& value);
				
		~ConfigValue_impl();
	
		CORBA::ValueBase* _copy_value();		
        };

	class ConfigValueFactory_impl:
			virtual public CORBA::ValueFactoryBase                    	
        {
		virtual CORBA::ValueBase* create_for_unmarshal();
	
	public:
		ConfigValueFactory_impl();
		~ConfigValueFactory_impl();
		
		ConfigValue * create();
		ConfigValue * create(const char * name, const CORBA::Any& value);
		
	};
        
	class StandardConfigurator_impl:
		virtual public POA_Components::StandardConfigurator,
		virtual public PortableServer::RefCountServantBase			
	{
	public:
		StandardConfigurator_impl (CORBA::ORB_ptr orb_);
		~StandardConfigurator_impl ();	
	
    		void set_configuration(const Components::ConfigValues& descr)
        		throw(CORBA::SystemException);
        		
		void configure(Components::CCMObject_ptr comp)
        		throw(Components::WrongComponentType,
              			CORBA::SystemException);
              			
              			        		
        private:
        
        	ConfigValues_var cfValues;	
        	
        	CORBA::ORB_var orb;
        	
	};

	class DynamicConnectConfigurator_impl:
		virtual public POA_Components::DynamicConnectConfigurator,
		virtual public PortableServer::RefCountServantBase			
	{
	public:
		DynamicConnectConfigurator_impl ();
		~DynamicConnectConfigurator_impl ();	
	
		virtual void set_connect_configuration(const Components::DynamicConnectInfos& descr)
			throw(CORBA::SystemException);
        		
		void configure(Components::CCMObject_ptr comp)
			throw(Components::WrongComponentType,
					CORBA::SystemException);
					
	private:
		DynamicConnectInfos_var connectInfoList;
	};	
	

}
#endif

