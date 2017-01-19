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

#ifndef ___CCMCommon_Type_impl_h__
#define ___CCMCommon_Type_impl_h__

#include <CCMHome_skel.h>
#include <CCMObject_skel.h>

namespace Components
{
class Cookie_impl : virtual public OBV_Components::Cookie,
                    virtual public CORBA::DefaultValueRefCountBase
{
	Cookie_impl(const Cookie_impl&);
	void operator=(const Cookie_impl&);
public:
	CCM_IMPORT static CORBA::ULong ckCount;

	Cookie_impl();
	Cookie_impl(CORBA::Boolean dummy);
	Cookie_impl(CORBA::ULong);
	Cookie_impl(const char*);
	~Cookie_impl();

	virtual const CORBA::OctetSeq& value() const;
	virtual CORBA::OctetSeq& value();

	virtual CORBA::ValueBase* _copy_value();

	CORBA::Boolean isEqual(Cookie_impl*);
	char* toString();
};

class CookieFactory_impl : virtual public Cookie_init
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	CookieFactory_impl();
	virtual ~CookieFactory_impl();
};

class PortDescription_impl : virtual public OBV_Components::PortDescription,
                             virtual public CORBA::DefaultValueRefCountBase
{
	PortDescription_impl(const PortDescription_impl&);
	void operator=(const PortDescription_impl&);

protected:
	void _copy_base_value(PortDescription_impl* portDes);

public:

	PortDescription_impl();
	~PortDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class PortDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	PortDescriptionFactory_impl();
	virtual ~PortDescriptionFactory_impl();
};

class FacetDescription_impl : virtual public OBV_Components::FacetDescription,
                              virtual public Components::PortDescription_impl
{
	FacetDescription_impl(const FacetDescription_impl&);
	void operator=(const FacetDescription_impl&);
public:

	FacetDescription_impl();
	~FacetDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class FacetDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	FacetDescriptionFactory_impl();
	virtual ~FacetDescriptionFactory_impl();
};

class ConnectionDescription_impl : virtual public OBV_Components::ConnectionDescription,
                                   virtual public CORBA::DefaultValueRefCountBase
{
	ConnectionDescription_impl(const ConnectionDescription_impl&);
	void operator=(const ConnectionDescription_impl&);

public:

	ConnectionDescription_impl();
	~ConnectionDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class ConnectionDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	ConnectionDescriptionFactory_impl();
	virtual ~ConnectionDescriptionFactory_impl();
};

class ReceptacleDescription_impl : virtual public OBV_Components::ReceptacleDescription,
                                   virtual public Components::PortDescription_impl
{
	ReceptacleDescription_impl(const ReceptacleDescription_impl&);
	void operator=(const ReceptacleDescription_impl&);

protected:

	CORBA::Boolean multiple_;
	ConnectionDescriptions connections_;

public:

	ReceptacleDescription_impl();
	~ReceptacleDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class ReceptacleDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	ReceptacleDescriptionFactory_impl();
	virtual ~ReceptacleDescriptionFactory_impl();
};

class ConsumerDescription_impl : virtual public OBV_Components::ConsumerDescription,
                                 virtual public Components::PortDescription_impl
{
	ConsumerDescription_impl(const ConsumerDescription_impl&);
	void operator=(const ConsumerDescription_impl&);

public:

	ConsumerDescription_impl();
	~ConsumerDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class ConsumerDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	ConsumerDescriptionFactory_impl();
	virtual ~ConsumerDescriptionFactory_impl();
};

class EmitterDescription_impl : virtual public OBV_Components::EmitterDescription,
                                virtual public Components::PortDescription_impl
{
	EmitterDescription_impl(const EmitterDescription_impl&);
	void operator=(const EmitterDescription_impl&);

public:

	EmitterDescription_impl();
	~EmitterDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class EmitterDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	EmitterDescriptionFactory_impl();
	virtual ~EmitterDescriptionFactory_impl();
};

class SubscriberDescription_impl : virtual public OBV_Components::SubscriberDescription,
                                   virtual public CORBA::DefaultValueRefCountBase
{
	SubscriberDescription_impl(const SubscriberDescription_impl&);
	void operator=(const SubscriberDescription_impl&);

public:

	SubscriberDescription_impl();
	~SubscriberDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class SubscriberDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	SubscriberDescriptionFactory_impl();
	virtual ~SubscriberDescriptionFactory_impl();
};

class PublisherDescription_impl : virtual public OBV_Components::PublisherDescription,
                                  virtual public Components::PortDescription_impl
{
	PublisherDescription_impl(const PublisherDescription_impl&);
	void operator=(const PublisherDescription_impl&);

public:

	PublisherDescription_impl();
	~PublisherDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class PublisherDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	PublisherDescriptionFactory_impl();
	virtual ~PublisherDescriptionFactory_impl();
};

class ComponentPortDescription_impl : virtual public OBV_Components::ComponentPortDescription,
                                      virtual public CORBA::DefaultValueRefCountBase
{
	ComponentPortDescription_impl(const ComponentPortDescription_impl&);
	void operator=(const ComponentPortDescription_impl&);

public:

	ComponentPortDescription_impl();
	~ComponentPortDescription_impl();

	virtual CORBA::ValueBase* _copy_value();
};

class ComponentPortDescriptionFactory_impl : virtual public CORBA::ValueFactoryBase
{
	virtual CORBA::ValueBase* create_for_unmarshal();

public:
	ComponentPortDescriptionFactory_impl();
	virtual ~ComponentPortDescriptionFactory_impl();
};

} 

#endif
