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
#include <CCMCommonType_impl.h>
#include <UuidGenerator.h>

CORBA::ULong Components::Cookie_impl::ckCount = 1;

Components::Cookie_impl::Cookie_impl()
{
	CORBA::String_var value = UuidGenerator::generateUuid();

	cookieValue().length(strlen(value.in()));

	CORBA::Octet* data = cookieValue().get_buffer(false);

	memcpy(data,value.in(),strlen(value.in()));
}

//CORBA::ULong Components::Cookie_impl::ckCount = 1;

Components::Cookie_impl::Cookie_impl(CORBA::Boolean dummy)
{
	cookieValue().length(sizeof(CORBA::ULong));

	CORBA::Octet* data = cookieValue().get_buffer(false);

	memcpy(data,&ckCount,sizeof(CORBA::ULong));

	ckCount++;
}

Components::Cookie_impl::Cookie_impl(CORBA::ULong ckValue)
{
	cookieValue().length(sizeof(CORBA::ULong));

	CORBA::Octet* data = cookieValue().get_buffer(false);

	memcpy(data,&ckValue,sizeof(CORBA::ULong));
}

Components::Cookie_impl::Cookie_impl(const char* value)
{
	if(!value) return;

	cookieValue().length(strlen(value));

	CORBA::Octet* data = cookieValue().get_buffer(false);

	memcpy(data,value,strlen(value));
}

Components::Cookie_impl::~Cookie_impl()
{
}

const CORBA::OctetSeq& 
Components::Cookie_impl::value() const
{
	return cookieValue();
}

CORBA::OctetSeq& 
Components::Cookie_impl::value()
{
	return cookieValue();
}

CORBA::Boolean 
Components::Cookie_impl::isEqual(Cookie_impl* cookieImpl)
{
	if((cookieImpl -> value()).length() != value().length())
		return false;

	if( memcmp(
	    value().get_buffer(false),
	    (cookieImpl->value()).get_buffer(false),
	    value().length()
		) == 0 )
		return true;
	else
		return false;
}

char*
Components::Cookie_impl::toString()
{
	unsigned char* pvalue = value().get_buffer(false);

	CORBA::String_var ret = CORBA::string_alloc(value().length()+1);
	memcpy((void *)ret.in(),pvalue,value().length());
	*(char*)(ret.in() + value().length()) = 0;

	return ret._retn();
}

CORBA::ValueBase*
Components::Cookie_impl::_copy_value()
{
	Cookie_impl* cp = new Cookie_impl();

	cp -> cookieValue(cookieValue());

	return cp;
}

Components::CookieFactory_impl::CookieFactory_impl()
{
}

Components::CookieFactory_impl::~CookieFactory_impl()
{
}

CORBA::ValueBase*
Components::CookieFactory_impl::create_for_unmarshal()
{
    return new Components::Cookie_impl((const char*)0);
}


Components::PortDescription_impl::PortDescription_impl()
{
}

Components::PortDescription_impl::~PortDescription_impl()
{
}

void 
Components::PortDescription_impl::_copy_base_value(PortDescription_impl* portDes)
{
	portDes -> name(name());
	portDes -> type_id(type_id());
}

CORBA::ValueBase*
Components::PortDescription_impl::_copy_value()
{
    PortDescription_impl* portDes = new PortDescription_impl();

	portDes -> name(name());
	portDes -> type_id(type_id());

    return portDes;
}

Components::PortDescriptionFactory_impl::PortDescriptionFactory_impl()
{
}

Components::PortDescriptionFactory_impl::~PortDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::PortDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::PortDescription_impl;
}

Components::FacetDescription_impl::FacetDescription_impl()
{
}

Components::FacetDescription_impl::~FacetDescription_impl()
{
}

CORBA::ValueBase*
Components::FacetDescription_impl::_copy_value()
{
	Components::FacetDescription_impl*  facetDesImpl 
		= new Components::FacetDescription_impl;
	
	facetDesImpl -> _copy_base_value(this);
	facetDesImpl -> facet_ref(facet_ref());

	return facetDesImpl;
}

Components::FacetDescriptionFactory_impl::FacetDescriptionFactory_impl()
{
}

Components::FacetDescriptionFactory_impl::~FacetDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::FacetDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::FacetDescription_impl;
}

Components::ConnectionDescription_impl::ConnectionDescription_impl()
{
}

Components::ConnectionDescription_impl::~ConnectionDescription_impl()
{
}

CORBA::ValueBase*
Components::ConnectionDescription_impl::_copy_value()
{
    ConnectionDescription_impl* conDesImpl = new ConnectionDescription_impl;

	conDesImpl -> ck(ck());
	conDesImpl -> objref(objref());

    return conDesImpl;
}

Components::ConnectionDescriptionFactory_impl::ConnectionDescriptionFactory_impl()
{
}

Components::ConnectionDescriptionFactory_impl::~ConnectionDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::ConnectionDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::ConnectionDescription_impl;
}

Components::ReceptacleDescription_impl::ReceptacleDescription_impl()
{
}

Components::ReceptacleDescription_impl::~ReceptacleDescription_impl()
{
}

CORBA::ValueBase*
Components::ReceptacleDescription_impl::_copy_value()
{
    ReceptacleDescription_impl* recpDesImpl = new ReceptacleDescription_impl;

	recpDesImpl -> _copy_base_value(this);
	recpDesImpl -> is_multiple(is_multiple());
	recpDesImpl -> connections(connections());

    return recpDesImpl;
}

Components::ReceptacleDescriptionFactory_impl::ReceptacleDescriptionFactory_impl()
{
}

Components::ReceptacleDescriptionFactory_impl::~ReceptacleDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::ReceptacleDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::ReceptacleDescription_impl;
}

Components::ConsumerDescription_impl::ConsumerDescription_impl()
{
}

Components::ConsumerDescription_impl::~ConsumerDescription_impl()
{
}

CORBA::ValueBase*
Components::ConsumerDescription_impl::_copy_value()
{
    ConsumerDescription_impl* consDesImpl = new ConsumerDescription_impl;

	consDesImpl -> _copy_base_value(this);
	consDesImpl -> consumer(consumer());

    return consDesImpl;
}

Components::ConsumerDescriptionFactory_impl::ConsumerDescriptionFactory_impl()
{
}

Components::ConsumerDescriptionFactory_impl::~ConsumerDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::ConsumerDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::ConsumerDescription_impl;
}

Components::EmitterDescription_impl::EmitterDescription_impl()
{
}

Components::EmitterDescription_impl::~EmitterDescription_impl()
{
}

CORBA::ValueBase*
Components::EmitterDescription_impl::_copy_value()
{
    EmitterDescription_impl* emitDesImpl = new EmitterDescription_impl;

	emitDesImpl -> _copy_base_value(this);
	emitDesImpl -> consumer(consumer());

    return emitDesImpl;
}

Components::EmitterDescriptionFactory_impl::EmitterDescriptionFactory_impl()
{
}

Components::EmitterDescriptionFactory_impl::~EmitterDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::EmitterDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::EmitterDescription_impl;
}

Components::SubscriberDescription_impl::SubscriberDescription_impl()
{
}

Components::SubscriberDescription_impl::~SubscriberDescription_impl()
{
}

CORBA::ValueBase*
Components::SubscriberDescription_impl::_copy_value()
{
    SubscriberDescription_impl* subDesImpl = new SubscriberDescription_impl;

	subDesImpl -> ck(ck());
	subDesImpl -> consumer(consumer());

    return subDesImpl;
}

Components::SubscriberDescriptionFactory_impl::SubscriberDescriptionFactory_impl()
{
}

Components::SubscriberDescriptionFactory_impl::~SubscriberDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::SubscriberDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::SubscriberDescription_impl;
}

Components::PublisherDescription_impl::PublisherDescription_impl()
{
}

Components::PublisherDescription_impl::~PublisherDescription_impl()
{
}

CORBA::ValueBase*
Components::PublisherDescription_impl::_copy_value()
{
    PublisherDescription_impl* pubDesImpl = new PublisherDescription_impl;

	pubDesImpl -> _copy_base_value(this);
	pubDesImpl -> consumers(consumers()); 

    return pubDesImpl;
}

Components::PublisherDescriptionFactory_impl::PublisherDescriptionFactory_impl()
{
}

Components::PublisherDescriptionFactory_impl::~PublisherDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::PublisherDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::PublisherDescription_impl;
}

Components::ComponentPortDescription_impl::ComponentPortDescription_impl()
{
}

Components::ComponentPortDescription_impl::~ComponentPortDescription_impl()
{
}

CORBA::ValueBase*
Components::ComponentPortDescription_impl::_copy_value()
{
    ComponentPortDescription_impl* comPortDesImpl = new ComponentPortDescription_impl;

	comPortDesImpl -> facets(facets());
	comPortDesImpl -> receptacles(receptacles());
	comPortDesImpl -> consumers(consumers());
	comPortDesImpl -> emitters(emitters());
	comPortDesImpl -> publishers(publishers());
	
    return comPortDesImpl;
}

Components::ComponentPortDescriptionFactory_impl::ComponentPortDescriptionFactory_impl()
{
}

Components::ComponentPortDescriptionFactory_impl::~ComponentPortDescriptionFactory_impl()
{
}

CORBA::ValueBase*
Components::ComponentPortDescriptionFactory_impl::create_for_unmarshal()
{
    return new Components::ComponentPortDescription_impl;
}
