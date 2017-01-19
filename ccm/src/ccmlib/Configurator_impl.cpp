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

#include <CORBA.h>
#include <Configurator_impl.h>
#include <string>

using namespace Components;
	
ConfigValue_impl::ConfigValue_impl()
{	
}

ConfigValue_impl::ConfigValue_impl(const char * name, const CORBA::Any& value)
{
	
	this->name(name);
	this->value((CORBA::Any&)value);
}

ConfigValue_impl::~ConfigValue_impl()
{
}
	
CORBA::ValueBase*
ConfigValue_impl::_copy_value()
{
    ConfigValue_impl* result = new ConfigValue_impl;
    
    result -> name(name());
    result -> value(value());
        
    return result; 
}

ConfigValueFactory_impl::ConfigValueFactory_impl()
{
}

ConfigValueFactory_impl::~ConfigValueFactory_impl()
{
}			
	
CORBA::ValueBase*
ConfigValueFactory_impl::create_for_unmarshal()
{
	return new ConfigValue_impl();
}	

ConfigValue * 
ConfigValueFactory_impl::create()
{
	return new ConfigValue_impl();
}

ConfigValue * 
ConfigValueFactory_impl::create(const char * name, const CORBA::Any& value)
{
	return new ConfigValue_impl(name,value);
}
        
StandardConfigurator_impl::StandardConfigurator_impl (CORBA::ORB_ptr orb_)
	:orb(CORBA::ORB::_duplicate(orb_))
{
}

StandardConfigurator_impl::~StandardConfigurator_impl ()
{
}	
	
void 
StandardConfigurator_impl::set_configuration(const Components::ConfigValues& descr)
        throw(CORBA::SystemException)
{    	
	
//	cfValues = new Components::ConfigValues;	
//	cfValues->length(descr.length());
//	
//	for (int i=0;i<descr.length();i++)
//	{
//		cfValues[i]=descr[i];
//	}
	//Modifyied by Wang Kebo, for compilation on TAO
	cfValues = new Components::ConfigValues(descr);
	
}
	

void 
StandardConfigurator_impl::configure(Components::CCMObject_ptr comp)
        throw(Components::WrongComponentType,
              	CORBA::SystemException)
{  
	
	//
	//construct the DII request parameter
	//
	
	CORBA::Context_var ctx;
	orb ->get_default_context(ctx.out());
	
	CORBA::NamedValue_var result;
	orb ->create_named_value(result.out());
	
	CORBA::ExceptionList_var exceptions;
	orb ->create_exception_list(exceptions.out());
	
	CORBA::ContextList_var cl;
	orb ->create_context_list(cl.out());
	
	CORBA::NVList_var argList;
	
	CORBA::Request_var request;

	Components::CCMObject_var obj=Components::CCMObject::_duplicate(comp);
	
	for(int i=0;i<cfValues->length();i++)
	{	
		//
		//construct the operation name
		//		
		
		CORBA::String_var operationName=CORBA::string_dup("_set_");
		CORBA::String_var configName = CORBA::string_dup(cfValues[(CORBA::ULong)i]->name());

#ifdef STARCCMSTRINGADD
		operationName = HelpFun::CORBA_string_add(operationName, configName);
#else
		operationName += configName;
#endif
		//
		//construct the operation parameter
		//
		
		orb ->create_list(0,argList);
		
		CORBA::NamedValue_ptr arg;				
	
		arg = argList ->add(CORBA::ARG_IN);
		
		*(arg ->value()) = cfValues[(CORBA::ULong)i]->value();
		
		
		//
		//construct the request and invoke
		//
		
		obj->_create_request(ctx,operationName.in(),argList,result,exceptions,cl,request,0);
		
		request ->invoke();
		
		
		//
		//process the exception
		//
#ifdef TAO
		if( (request -> exceptions()) != 0)
#else
		if( (request -> env()->exception())!=0)
#endif
		{
			TRACE0(LEVEL1,"Configurator: DII request failed.\n");
			throw Components::WrongComponentType();
 		}
		
	}
}

DynamicConnectConfigurator_impl::DynamicConnectConfigurator_impl ()
{
}

DynamicConnectConfigurator_impl::~DynamicConnectConfigurator_impl ()
{
}	
	
void 
DynamicConnectConfigurator_impl::set_connect_configuration(const Components::DynamicConnectInfos& descr)
        throw(CORBA::SystemException)
{	
//	connectInfoList = new Components::DynamicConnectInfos;
//	connectInfoList->length(descr.length());
//	
//	for (int i=0;i<descr.length();i++)
//	{
//		connectInfoList[i]=descr[i];
//	}	
	//Modified by Wang Kebo, for compilation on TAO
	connectInfoList = new Components::DynamicConnectInfos(descr);
}
	

void 
DynamicConnectConfigurator_impl::configure(Components::CCMObject_ptr comp)
        throw(Components::WrongComponentType,
              	CORBA::SystemException)
{
	
	try
	{
		for(int i=0;i<connectInfoList->length();i++)
		{
			DynamicConnectInfo connectionInfo = connectInfoList[(CORBA::ULong)i];
			ConnectType connectionType = connectionInfo.connectPortType;
			CORBA::String_var connectPortIdentifier = connectionInfo.portIdentifier;
			CORBA::Object_var connectToObject = connectionInfo.connectToRef;					

			Components::Cookie_var cookie_;
			
			if((connectionType == PROVIDESTYPE)||
				(connectionType == COMPONENTSUPPORTEDINTERFACE)||
				(connectionType == HOMEINTERFACE))
			{						
				
				cookie_=comp->connect(connectPortIdentifier.in(),connectToObject.in());				
				TRACE1(LEVEL4,"Configurator: connectComponents: dynamic connect the %s.\n",connectPortIdentifier.in());				
			}
			else if(connectionType == EMITSTYPE)
			{				
				CORBA::String_var emitsIdentifier = connectionInfo.eventSourcePortIdentifier;
				
				Components::EventConsumerBase_var consumer=comp->get_consumer(connectPortIdentifier.in());
				Components::CCMObject_var emitsComponent = Components::CCMObject::_narrow(connectToObject);
				
				emitsComponent->connect_consumer(emitsIdentifier.in(),consumer);
				TRACE2(LEVEL4,"Configurator: connectComponents: connect the consumes identifier %s with emits identifier %s.\n",connectPortIdentifier.in(),emitsIdentifier.in());
			}
			else if(connectionType == PUBLISHESTYPE)
			{
				CORBA::String_var publishesIdentifier = connectionInfo.eventSourcePortIdentifier;
				
				Components::EventConsumerBase_var consumer=comp->get_consumer(connectPortIdentifier.in());
				Components::CCMObject_var providesComponent = Components::CCMObject::_narrow(connectToObject);
				
				cookie_=providesComponent->subscribe(publishesIdentifier.in(),consumer);
				TRACE2(LEVEL4,"Configurator: subscirbe the consumes identifier %s with publisher %s.\n",connectPortIdentifier.in(),publishesIdentifier.in());
			}		
		}
	}
	catch(const CORBA::SystemException &)
	{
		TRACE0(LEVEL1,"Configurator: connect Components failed in CORBA SystemException .\n");
		throw Components::WrongComponentType();														
	}
	catch(const InvalidName &)
	{
		TRACE0(LEVEL1,"Configurator: connect Components failed in InvalidName.\n");
		throw Components::WrongComponentType();									
	}
	catch(const InvalidConnection &)
	{
		TRACE0(LEVEL1,"Configurator: connect Components failed in InvalidConnection.\n");
		throw Components::WrongComponentType();									
	}
	catch(const AlreadyConnected &)
	{
		TRACE0(LEVEL1,"Configurator: connect Components failed in AlreadyConnected.\n");
		throw Components::WrongComponentType();									
	}								
	catch(const ExceededConnectionLimit &)
	{
		TRACE0(LEVEL1,"Configurator: connect Components failed in ExceededConnectionLimit.\n");
		throw Components::WrongComponentType();									
	}

}

