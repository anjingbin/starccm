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
#include <CCMObject.h>
#include <ComponentServentBase.h>

using namespace std;
using namespace Components;

ComponentServentBase::ComponentServentBase()
	:nextPortID_(1)
{
}

ComponentServentBase::~ComponentServentBase()
{
}

int 
ComponentServentBase::nextPortID()
{
	return nextPortID_++;
}

//wsf
void
ComponentServentBase::setCIFHook(::Container::CIFHook_ptr cifHook)
{
    cifHook_ = OBJDUPLICATE(::Container::CIFHook_ptr, cifHook);
}

//wsf
::Container::CIFHook_ptr 
ComponentServentBase::getCIFHook()
{
	if (!CORBA::is_nil(cifHook_))
	{
		
		return OBJDUPLICATE(::Container::CIFHook_ptr, cifHook_);
	}
	else
	{
		return NULL;
	}
		
}

//wsf
void
ComponentServentBase::setComObjectID(const PortableServer::ObjectId& objID)
{
    comObjectId_ = new PortableServer::ObjectId(objID);
}

//wsf
PortableServer::ObjectId*
ComponentServentBase::getComObjetID()
{
	return new PortableServer::ObjectId(comObjectId_);
}

CORBA::Object_ptr
ComponentServentBase::provide_facet(const char* name)
	throw(Components::InvalidName)
{
	IterFacet iterFacet = facets_.find(string(name));

	if(iterFacet != facets_.end())
		return CORBA::Object::_duplicate(iterFacet -> second -> facet_ref()); 
	else
		throw Components::InvalidName();
}

Components::FacetDescriptions*
ComponentServentBase::get_all_facets()
	throw (CORBA::SystemException)

{
	int howMany = 0;

	Components::FacetDescriptions_var facetDescriptions 
		= new Components::FacetDescriptions(8);

	facetDescriptions -> length(0);
	
	IterFacet iterFacet;

	for(iterFacet=facets_.begin();iterFacet!=facets_.end();iterFacet++)
	{
		facetDescriptions -> length(howMany+1);

		//iterFacet -> second -> _add_ref();
		facetDescriptions -> operator[](howMany)
			= iterFacet -> second;

		howMany++;
	}

	return facetDescriptions._retn();
}

Components::FacetDescriptions*
ComponentServentBase::get_named_facets(const Components::NameList& nameList)
	throw(Components::InvalidName)
{
	int howMany = nameList.length();

	Components::FacetDescriptions_var facetDescriptions 
		= new Components::FacetDescriptions(howMany);

	facetDescriptions -> length(howMany);
	
	for(int i=0;i<howMany;i++)
	{
		IterFacet iterFacet = 
			facets_.find(string(static_cast<const char *>(nameList[i])));

		if(iterFacet != facets_.end())
		{
			//iterFacet -> second -> _add_ref();
			facetDescriptions -> operator[](i) 
				= iterFacet -> second;
		}
		else
			throw Components::InvalidName();		
	}

	return facetDescriptions._retn();
}

CORBA::Boolean 
ComponentServentBase::same_component(CORBA::Object_ptr object_ref)
	throw (CORBA::SystemException)

{
	if(_this() -> _is_equivalent (object_ref))
		return true;

	IterFacet iterFacet;

	for (iterFacet=facets_.begin();iterFacet!=facets_.end();iterFacet++)
	{
		if(iterFacet -> second -> 
			facet_ref() -> _is_equivalent(object_ref))
			return true;
	}

	return false;
}

Components::Cookie*
ComponentServentBase::connect(const char* name, CORBA::Object_ptr connection)
	throw(Components::InvalidName, 
		Components::InvalidConnection, 
		Components::AlreadyConnected, 
		Components::ExceededConnectionLimit)
{
	if(CORBA::is_nil(connection))
		throw Components::InvalidConnection();

	IterReceptacle iterReceptacle = receptacles_.find(string(name));

	if(iterReceptacle != receptacles_.end()) 
	{
		//iterReceptacle -> second -> _add_ref();
		Components::ReceptacleDescription_var recDesc = iterReceptacle -> second;

		if(!connection -> _is_a(recDesc -> type_id())) 
		{
			throw Components::InvalidConnection();
		}

		if(!recDesc -> is_multiple() 
			&& recDesc -> connections().length()!=0) 
				throw Components::AlreadyConnected();
		
		int len = recDesc -> connections().length();

		recDesc -> connections().length(len+1);

		recDesc -> connections()[len] = new ConnectionDescription_impl;

		Components::Cookie_var cookie = new Components::Cookie_impl();

		recDesc -> connections()[len] -> ck(cookie.in());
		recDesc -> connections()[len] -> objref(connection);

		//add by wsf
		STARCCM::ArchConnection acon;
		Components::Cookie_impl* ck = dynamic_cast<Components::Cookie_impl*>(cookie.in());
		acon.id = ck->toString();
		acon.cookie = ck->toString();
		acon.connectmode = CORBA::string_dup("connect");
		acon.srccomuuid = PortableServer::ObjectId_to_string(comObjectId_.in());
		//acon.srccomref = NULL;
		acon.srcport = CORBA::string_dup(name);
		//acon.targetuuid = NULL;
		if (connection->_is_a("IDL:omg.org/Components/CCMHome:1.0"))
		{
			acon.targettype = CORBA::string_dup("home");
		}
		else
		{
			if (connection->_is_a("IDL:omg.org/Components/CCMObject:1.0"))
			{
				acon.targettype = CORBA::string_dup("supports");
			}
			else
			{
				acon.targettype = CORBA::string_dup("facet");
			}
		}
		acon.targetref = CORBA::Object::_duplicate(connection);
		//acon.targetport = NULL;
		
		cifHook_->onConnect(acon);
		//end by wsf

		return cookie._retn();

	}
	else
		throw Components::InvalidName();
}

void 
ComponentServentBase::disconnect(const char* name,Components::Cookie* ck)
	throw(Components::InvalidName,
		Components::InvalidConnection,
		Components::CookieRequired,
		Components::NoConnection)
{
	IterReceptacle iterReceptacle = receptacles_.find(string(name));

	if(iterReceptacle != receptacles_.end()) 
	{
		//iterReceptacle -> second -> _add_ref();
		Components::ReceptacleDescription_var recDesc = iterReceptacle -> second;

		if(recDesc -> is_multiple())
		{
			Components::Cookie_impl* implCk = dynamic_cast<Components::Cookie_impl*>(ck);

			if(!implCk)
				throw Components::CookieRequired();

			int len = recDesc -> connections().length();
	
			if(len == 0)
			{
				throw Components::NoConnection();
			}
			else
			{
				int i;

				for(i=0;i<len;i++)
				{
					Components::Cookie* theCk =
						recDesc -> connections()[i] -> ck();

					Components::Cookie_impl* savedCk = dynamic_cast<Components::Cookie_impl*>(theCk);

					if(implCk -> isEqual(savedCk))
						break;
				}

				if(i != len)
				{
					//modified by xiao heping 2004/07/02
					//for(i=0;i<len;i++)
					for(int j=i+1;j<len;j++)
					{
//						if(i != len -1)
//						{
						Components::Cookie* theCk =	recDesc	-> connections()[j] -> ck();

						recDesc -> connections()[j-1] -> ck(theCk);

						CORBA::Object_ptr connection = recDesc -> connections()[j] -> objref();

						recDesc -> connections()[j-1] -> objref(connection);
//						}
					}
					//end modify

					recDesc -> connections().length(len-1);
				}
				else
				{
					throw Components::InvalidConnection();
				}
			}
		}
		else
		{
			recDesc -> connections().length(0);
		}
	}
	else
		throw Components::InvalidName();	
		
	//add by wsf
	PortableServer::ObjectId* oid = getComObjetID();
	Components::Cookie_impl* implCk = dynamic_cast<Components::Cookie_impl*>(ck);
	CORBA::String_var cookie = implCk->toString();
	cifHook_->onDisconnect(oid, name, cookie.in());
	delete oid;
	oid = NULL;
	//end by wsf
}

Components::ConnectionDescriptions* 
ComponentServentBase::get_connections(const char* name)
	throw(Components::InvalidName)
{
	IterReceptacle iterReceptacle = receptacles_.find(string(name));

	if(iterReceptacle != receptacles_.end()) 
	{
		//iterReceptacle -> second -> _add_ref();
		Components::ReceptacleDescription_var recDesc = iterReceptacle -> second;

		Components::ConnectionDescriptions& conn = recDesc -> connections();

		return new Components::ConnectionDescriptions(conn);
	}
	else
		throw Components::InvalidName();
}

Components::ReceptacleDescriptions* 
ComponentServentBase::get_all_receptacles()
	throw (CORBA::SystemException)

{	
	int howMany = 0;

	Components::ReceptacleDescriptions_var receptacleDescriptions 
		= new Components::ReceptacleDescriptions(8);

	receptacleDescriptions -> length(0);
	
	IterReceptacle iterReceptacle;

	for(iterReceptacle=receptacles_.begin();iterReceptacle!=receptacles_.end();
		iterReceptacle++)
	{
		receptacleDescriptions -> length(howMany+1);

		//iterReceptacle -> second -> _add_ref();

		receptacleDescriptions -> operator[](howMany)
			= iterReceptacle -> second;
			
		//added by xiao heping 2004/06/03
		howMany++;
	}

	return receptacleDescriptions._retn();
}

Components::ReceptacleDescriptions*
ComponentServentBase::get_named_receptacles(const Components::NameList& nameList)
	throw(Components::InvalidName)
{
	int howMany = nameList.length();

	Components::ReceptacleDescriptions_var receptacleDescriptions 
		= new Components::ReceptacleDescriptions(howMany);

	receptacleDescriptions -> length(howMany);
	
	for(int i=0;i<howMany;i++)
	{
		IterReceptacle iterReceptacle = 
			receptacles_.find(string(static_cast<const char *>(nameList[i])));

		if(iterReceptacle != receptacles_.end())
		{
			//iterReceptacle -> second -> _add_ref();
			receptacleDescriptions -> operator[](i) 
				= iterReceptacle -> second;
		}
		else
			throw Components::InvalidName();		
	}

	return receptacleDescriptions._retn();
}

Components::EventConsumerBase_ptr
ComponentServentBase::get_consumer(const char* sink_name)
	throw(Components::InvalidName)
{
	IterConsumer iterConsumer = consumers_.find(string(sink_name));

	if(iterConsumer != consumers_.end())
		return Components::EventConsumerBase::_duplicate(iterConsumer -> second -> consumer()); 
	else
		throw Components::InvalidName();
}

Components::Cookie* 
ComponentServentBase::subscribe(const char* publisher_name,Components::EventConsumerBase_ptr subscriber)
	throw(Components::InvalidName,
		Components::InvalidConnection,
		Components::ExceededConnectionLimit)
{
	if(CORBA::is_nil(subscriber))
		throw Components::InvalidConnection();

	IterPublisher iterPublisher = publishers_.find(string(publisher_name));

	if(iterPublisher != publishers_.end()) 
	{
		//iterPublisher -> second ->_add_ref();
		Components::PublisherDescription_var publisherDesc = iterPublisher -> second;
		
		int len = publisherDesc -> consumers().length();
		publisherDesc -> consumers().length(len+1);

		publisherDesc -> consumers()[len] = new SubscriberDescription_impl;

		Components::Cookie_var cookie = new Components::Cookie_impl();

		publisherDesc -> consumers()[len] -> ck(cookie.in());
		publisherDesc -> consumers()[len] -> consumer(subscriber);

		//add by wsf
		STARCCM::ArchConnection acon;
		Components::Cookie_impl* ck = dynamic_cast<Components::Cookie_impl*>(cookie.in());
		acon.id = ck->toString();
		acon.cookie = ck->toString();
		acon.connectmode = CORBA::string_dup("subscribe");
		acon.srccomuuid = PortableServer::ObjectId_to_string(comObjectId_.in());
		//acon.srccomref = NULL;
		acon.srcport = CORBA::string_dup(publisher_name);
		//acon.targetuuid = NULL;
		acon.targettype = CORBA::string_dup("consumer");
		acon.targetref = CORBA::Object::_duplicate(subscriber);
		//acon.targetport = NULL;
		
		cifHook_->onConnect(acon);
		//end by wsf

		return cookie._retn();

	}
	else
		throw Components::InvalidName();
}

void 
ComponentServentBase::unsubscribe(const char* publisher_name,Components::Cookie* ck)
	throw(Components::InvalidName, 
			Components::InvalidConnection)
{
	Components::Cookie_impl* implCk = dynamic_cast<Components::Cookie_impl*>(ck);

	if(!implCk)
		throw Components::CookieRequired();

	IterPublisher iterPublisher = publishers_.find(string(publisher_name));

	if(iterPublisher != publishers_.end()) 
	{
		//iterPublisher -> second -> _add_ref();
		Components::PublisherDescription_var publisherDesc = iterPublisher -> second;

		int len = publisherDesc -> consumers().length();
	
		if(len == 0)
		{
			throw Components::InvalidConnection();
		}
		else
		{
			int i;

			for(i=0;i<len;i++)
			{
				Components::Cookie* theCk =
					publisherDesc -> consumers()[i] -> ck();

				Components::Cookie_impl* savedCk = dynamic_cast<Components::Cookie_impl*>(theCk);

				if(implCk -> isEqual(savedCk))
					break;
			}

			if(i != len)
			{
				for(i=0;i<len;i++)
				{
					if(i != len -1)
					{
						Components::Cookie* theCk =
							publisherDesc -> consumers()[i+1] -> ck();

						publisherDesc -> consumers()[i] -> ck(theCk);

						Components::EventConsumerBase_ptr eventConsumer =
							publisherDesc -> consumers()[i+1] -> consumer();

						publisherDesc -> consumers()[i] -> consumer(eventConsumer);
					}
				}

				publisherDesc -> consumers().length(len-1);
			}
			else
			{
				throw Components::InvalidConnection();
			}
		}
	}
	else
		throw Components::InvalidName();
		
	//add by wsf
	PortableServer::ObjectId* oid = getComObjetID();
	CORBA::String_var cookie = implCk->toString();
	cifHook_->onDisconnect(oid, publisher_name, cookie.in());
	//end by wsf
}

void 
ComponentServentBase::connect_consumer(const char* emitter_name,Components::EventConsumerBase_ptr consumer)
	throw(Components::InvalidName, 
			Components::AlreadyConnected,
			Components::InvalidConnection)
{
	if(CORBA::is_nil(consumer))
		throw Components::InvalidConnection();

	IterEmitter iterEmitter = emitters_.find(string(emitter_name));

	if(iterEmitter != emitters_.end()) 
	{
		//iterEmitter -> second -> _add_ref();
		Components::EmitterDescription_var emitterDesc = iterEmitter -> second;
		
		if(CORBA::is_nil(emitterDesc -> consumer()))
		{
			emitterDesc -> consumer(consumer);
		}
		else
		{
			throw Components::AlreadyConnected();
		}
	}
	else
		throw Components::InvalidName();
		
	//add by wsf
	STARCCM::ArchConnection acon;
	Components::Cookie_impl* ck = new Components::Cookie_impl();
	acon.id = ck->toString();
	//acon.cookie = NULL;
	acon.connectmode = CORBA::string_dup("connect_consumer");
	acon.srccomuuid = PortableServer::ObjectId_to_string(comObjectId_.in());
	//acon.srccomref = NULL;
	acon.srcport = CORBA::string_dup(emitter_name);
	//acon.targetuuid = NULL;
	acon.targettype = CORBA::string_dup("consumer");
	acon.targetref = CORBA::Object::_duplicate(consumer);
	//acon.targetport = NULL;
	
	cifHook_->onConnect(acon);
	//end by wsf
}

Components::EventConsumerBase_ptr 
ComponentServentBase::disconnect_consumer(const char* source_name)
	throw(Components::InvalidName, 
			Components::NoConnection)
{
	IterEmitter iterEmitter = emitters_.find(string(source_name));

	if(iterEmitter != emitters_.end()) 
	{
		//iterEmitter -> second -> _add_ref();
		Components::EmitterDescription_var emitterDesc = iterEmitter -> second;
		
		if(CORBA::is_nil(emitterDesc -> consumer()))
		{
			throw Components::NoConnection();
		}
		else
		{
			Components::EventConsumerBase_ptr consumer = emitterDesc -> consumer();
			emitterDesc -> consumer(NULL);
			return Components::EventConsumerBase::_duplicate(consumer);
		}
	}
	else
		throw Components::InvalidName();
		
	//add by wsf
	PortableServer::ObjectId* oid = getComObjetID();
	cifHook_->onDisconnect(oid, source_name, NULL);
	//end by wsf
}

Components::ConsumerDescriptions*
ComponentServentBase::get_all_consumers()
	throw (CORBA::SystemException)

{
	int howMany = 0;

	Components::ConsumerDescriptions_var consumerDescs 
		= new Components::ConsumerDescriptions(8);

	consumerDescs -> length(0);
	
	IterConsumer iterConsumer;

	for(iterConsumer=consumers_.begin();iterConsumer!=consumers_.end();iterConsumer++)
	{
		consumerDescs -> length(howMany+1);

		//iterConsumer -> second -> _add_ref();
		consumerDescs -> operator[](howMany)
			= iterConsumer -> second;
			
		//added by xiao heping 2004/06/03
		howMany++;
	}

	return consumerDescs._retn();
}

Components::ConsumerDescriptions* 
ComponentServentBase::get_named_consumers(const Components::NameList& nameList)
	throw(Components::InvalidName)
{
	int howMany = nameList.length();

	Components::ConsumerDescriptions_var consumerDescs 
		= new Components::ConsumerDescriptions(howMany);

	consumerDescs -> length(howMany);
	
	for(int i=0;i<howMany;i++)
	{
		IterConsumer iterConsumer = 
			consumers_.find(string(static_cast<const char *>(nameList[i])));

		if(iterConsumer != consumers_.end())
		{
			//iterConsumer -> second -> _add_ref();
			consumerDescs -> operator[](i) 
				= iterConsumer -> second;
		}
		else
			throw Components::InvalidName();		
	}

	return consumerDescs._retn();
}

Components::EmitterDescriptions* 
ComponentServentBase::get_all_emitters()
	throw (CORBA::SystemException)

{
	int howMany = 0;

	Components::EmitterDescriptions_var emitterDescs 
		= new Components::EmitterDescriptions(8);

	emitterDescs -> length(0);
	
	IterEmitter iterEmitter;

	for(iterEmitter=emitters_.begin();iterEmitter!=emitters_.end();iterEmitter++)
	{
		emitterDescs -> length(howMany+1);

		//iterEmitter -> second -> _add_ref();
		emitterDescs -> operator[](howMany)
			= iterEmitter -> second;
			
		//added by xiao heping 2004/06/03
		howMany++;
	}

	return emitterDescs._retn();
}

Components::EmitterDescriptions* 
ComponentServentBase::get_named_emitters(const Components::NameList& nameList)
	throw(Components::InvalidName)
{
	int howMany = nameList.length();

	Components::EmitterDescriptions_var emitterDescs 
		= new Components::EmitterDescriptions(howMany);

	emitterDescs -> length(howMany);
	
	for(int i=0;i<howMany;i++)
	{
		IterEmitter iterEmitter = 
			emitters_.find(string(static_cast<const char *>(nameList[i])));

		if(iterEmitter != emitters_.end())
		{
			//iterEmitter -> second -> _add_ref();
			emitterDescs -> operator[](i) 
				= iterEmitter -> second;
		}
		else
			throw Components::InvalidName();		
	}

	return emitterDescs._retn();
}

Components::PublisherDescriptions* 
ComponentServentBase::get_all_publishers()
	throw (CORBA::SystemException)

{
	int howMany = 0;

	Components::PublisherDescriptions_var publisherDescs
		= new Components::PublisherDescriptions(8);

	publisherDescs -> length(0);
	
	IterPublisher iterPublisher;

	for(iterPublisher=publishers_.begin();iterPublisher!=publishers_.end();iterPublisher++)
	{
		publisherDescs -> length(howMany+1);

		//iterPublisher -> second -> _add_ref();
		publisherDescs -> operator[](howMany)
			= iterPublisher -> second;
			
		//added by xiao heping 2004/06/03
		howMany++;
	}

	return publisherDescs._retn();
}

Components::PublisherDescriptions* 
ComponentServentBase::get_named_publishers(const Components::NameList& nameList)
	throw(Components::InvalidName)
{
	int howMany = nameList.length();

	Components::PublisherDescriptions_var publisherDescs 
		= new Components::PublisherDescriptions(howMany);

	publisherDescs -> length(howMany);
	
	for(int i=0;i<howMany;i++)
	{
		IterPublisher iterPublisher = 
			publishers_.find(string(static_cast<const char *>(nameList[i])));

		if(iterPublisher != publishers_.end())
		{
			//iterPublisher -> second -> _add_ref();
			publisherDescs -> operator[](i) 
				= iterPublisher -> second;
		}
		else
			throw Components::InvalidName();		
	}

	return publisherDescs._retn();
}

CORBA::IRObject_ptr 
ComponentServentBase::get_component_def()
	throw (CORBA::SystemException)

{
	return CORBA::IRObject::_nil();
}

Components::CCMHome_ptr 
ComponentServentBase::get_ccm_home()
	throw (CORBA::SystemException)
{
	return Components::CCMHome::_nil();
}

Components::PrimaryKeyBase*
ComponentServentBase::get_primary_key()
	throw(Components::NoKeyAvailable)
{
	throw Components::NoKeyAvailable();
}

void 
ComponentServentBase::configuration_complete()
	throw(CORBA::SystemException, Components::InvalidConfiguration)
{
}

//add by wsf 2004.6.29
void 
ComponentServentBase::passivate_component()
	throw(CORBA::SystemException)
{
}

void 
ComponentServentBase::activate_component()
	throw(CORBA::SystemException)
{
}

void 
ComponentServentBase::run_complete()
	throw(CORBA::SystemException)
{
}

ConfigValues* 
ComponentServentBase::externalize()
	throw(CORBA::SystemException)
{
	return NULL;
}

void 
ComponentServentBase::initialize(const ConfigValues& states)
	throw(CORBA::SystemException)
{
}
//end by wsf 2004.6.29
void 
ComponentServentBase::remove()
	throw(Components::RemoveFailure)
{
	//This method is override by specific servant remove implement
	facets_.clear();
	receptacles_.clear();
	consumers_.clear();
	emitters_.clear();
	publishers_.clear();

	//_remove_ref() is called within container.
	//Container remove all servants(include port servants and main servant)
	//which related to this component.
}

Components::ComponentPortDescription*
ComponentServentBase::get_all_ports()
	throw (CORBA::SystemException)
{
	Components::ComponentPortDescription_var comPortDescs 
		= new Components::ComponentPortDescription_impl();

	Components::FacetDescriptions_var facetDecs = get_all_facets();
	comPortDescs -> facets(*(facetDecs._retn()));

	Components::ReceptacleDescriptions_var receptacleDes = get_all_receptacles();
	comPortDescs -> receptacles(*(receptacleDes._retn()));

	Components::ConsumerDescriptions_var consumerDes = get_all_consumers();
	comPortDescs -> consumers(*(consumerDes._retn()));

	Components::EmitterDescriptions_var emitterDes = get_all_emitters();
	comPortDescs -> emitters(*(emitterDes._retn()));

	Components::PublisherDescriptions_var publisherDes = get_all_publishers();
	comPortDescs -> publishers(*(publisherDes._retn()));

	return comPortDescs._retn();
}

//additional function ,added by huangjie 2002.3.21
CORBA::Object_ptr
ComponentServentBase::unsubscribeEx(const char* publisher_name,Components::Cookie* ck)
	throw(Components::InvalidName, 
			Components::InvalidConnection)
{
	Components::Cookie_impl* implCk = dynamic_cast<Components::Cookie_impl*>(ck);

	if(!implCk)
		throw Components::CookieRequired();

	IterPublisher iterPublisher = publishers_.find(string(publisher_name));

	if(iterPublisher != publishers_.end()) 
	{
		//iterPublisher -> second -> _add_ref();
		Components::PublisherDescription_var publisherDesc = iterPublisher -> second;

		int len = publisherDesc -> consumers().length();
	
		if(len == 0)
		{
			throw Components::InvalidConnection();
		}
		else
		{
			int i;

			for(i=0;i<len;i++)
			{
				Components::Cookie* theCk =
					publisherDesc -> consumers()[i] -> ck();

				Components::Cookie_impl* savedCk = dynamic_cast<Components::Cookie_impl*>(theCk);

				if(implCk -> isEqual(savedCk))
					break;
			}

			if(i != len)
			{
				CORBA::Object_ptr obj =  publisherDesc -> consumers()[i] -> consumer();

				for(i=0;i<len;i++)
				{
					if(i != len -1)
					{
						Components::Cookie* theCk =
							publisherDesc -> consumers()[i+1] -> ck();

						publisherDesc -> consumers()[i] -> ck(theCk);

						Components::EventConsumerBase_ptr eventConsumer =
							publisherDesc -> consumers()[i+1] -> consumer();

						publisherDesc -> consumers()[i] -> consumer(eventConsumer);
					}
				}

				publisherDesc -> consumers().length(len-1);

				return CORBA::Object::_duplicate(obj);
			}
			else
			{
				throw Components::InvalidConnection();
			}
		}
	}
	else
		throw Components::InvalidName();
}

/*
CORBA::Object_ptr 
ComponentServentBase::activate(ContainerPSS::ASComponentHome_ptr storageHome, const char* uuid)
{
	ContainerPSS::ASComponent_var storage;
	ContainerPSS::ComponentPSS_var comp;
	int i;

	try
	{
		storage = storageHome -> find_by_uuid(uuid);
		comp = storage -> comp();

		FacetDescriptions& facets = comp -> facets;

		for(i = 0; i < facets.length(); i++)
		{
			FacetDescription_var facet = facets[i];
			facets_.insert(
				FacetsMap::value_type(static_cast<const char*>(facet -> name()), facet));
		}

		ReceptacleDescriptions& receptacles = comp -> receptacles;

		for(i = 0; i < receptacles.length(); i++)
		{
			ReceptacleDescription_var receptacle = receptacles[i];
			receptacles_.insert(
				ReceptaclesMap::value_type(static_cast<const char*>(receptacle -> name()), receptacle));
		}

		ConsumerDescriptions& consumers = comp -> consumers;

		for(i = 0; i < consumers.length(); i++)
		{
			ConsumerDescription_var consumer = consumers[i];
			consumers_.insert(
				ConsumersMap::value_type(static_cast<const char*>(consumer -> name()), consumer));
		}

		PublisherDescriptions& publishers = comp -> publishers;

		for(i = 0; i < receptacles.length(); i++)
		{
			PublisherDescription_var publisher = publishers[i];
            publishers_.insert(
				PublishersMap::value_type(static_cast<const char*>(publisher -> name()), publisher));
		}

		EmitterDescriptions& emitters = comp -> emitters;

		for(i = 0; i < emitters.length(); i++)
		{
			EmitterDescription_var emitter = emitters[i];
			emitters_.insert(
				EmittersMap::value_type(static_cast<const char*>(emitter -> name()), emitter));
		}

		return comp -> componentRef;
	}
	catch(CosPersistentState::NotFound)
	{
	}

	return 0;
}

void 
ComponentServentBase::passivate(ContainerPSS::ASComponentHome_ptr storageHome, const char* uuid, CORBA::Object_ptr componentRef)
{
	ContainerPSS::ASComponent_var storage;
	ContainerPSS::ComponentPSS_var comp = new ContainerPSS::ComponentPSS();

	try
	{
		storage = storageHome -> find_by_uuid(uuid);
	}
	catch(CosPersistentState::NotFound)
	{
		storage = storageHome -> create(uuid);
	}

	FacetDescriptions_var facets = get_all_facets();
	ReceptacleDescriptions_var receptacles = get_all_receptacles();
	ConsumerDescriptions_var consumers = get_all_consumers();
	PublisherDescriptions_var publishers = get_all_publishers();
	EmitterDescriptions_var emitters = get_all_emitters();

	comp -> facets = facets.in();
	comp -> receptacles = receptacles.in();
	comp -> consumers = consumers.in();
	comp -> publishers = publishers.in();
	comp -> emitters = emitters.in();

	comp -> componentRef = CORBA::Object::_duplicate(componentRef);

	storage -> comp(comp.in());
	storage -> status(0);
}

CORBA::Long
ComponentServentBase::isActive(ContainerPSS::ASComponentHome_ptr storageHome, const char* uuid)
{
	ContainerPSS::ASComponent_var storage;

	try
	{
		storage = storageHome -> find_by_uuid(uuid);
		return storage -> status();
	}
	catch(CosPersistentState::NotFound)
	{
	}

	return 0;
}

*/

