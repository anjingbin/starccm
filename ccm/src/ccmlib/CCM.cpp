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

#include <CCM.h>
#include <Configurator_impl.h>


void
CCM::init(CORBA::ORB_ptr orb)
{
	CORBA::ValueFactoryBase_var factory;

	factory = new Components::CookieFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/Cookie:1.0", factory.in());

	factory = new Components::PortDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/PortDescription:1.0", factory.in());

	factory = new Components::FacetDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/FacetDescription:1.0", factory.in());

	factory = new Components::ConnectionDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/ConnectionDescription:1.0", factory.in());

	factory = new Components::ReceptacleDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/ReceptacleDescription:1.0", factory.in());

	factory = new Components::ConsumerDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/ConsumerDescription:1.0", factory.in());

	factory = new Components::EmitterDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/EmitterDescription:1.0", factory.in());

	factory = new Components::SubscriberDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/SubscriberDescription:1.0", factory.in());

	factory = new Components::PublisherDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/PublisherDescription:1.0", factory.in());

	factory = new Components::ComponentPortDescriptionFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/ComponentPortDescription:1.0", factory.in());
    
    factory = new Components::ConfigValueFactory_impl();
    orb -> register_value_factory("IDL:omg.org/Components/ConfigValue:1.0", factory.in());
}

