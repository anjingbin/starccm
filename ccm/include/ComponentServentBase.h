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

#ifndef __ComponentServentBase_h__
#define __ComponentServentBase_h__

#include <CORBA.h>
#include <CCMCommon.h>
#include <CCMObject_skel.h>
#include <CCMCommonType_impl.h>
//next 2 lines commented out by An Jingbin 2003-11-06
//#include <ComponentsPSS.h>
//#include <ComponentsPSS_psdl.h> 

//add by wsf
#include <CIFHook.h>
#include <ArchObject.h>
//end by wsf

#include <string> 
#include <map> 

using namespace std;

namespace Components
{

typedef map<string, Components::FacetDescription_var> FacetsMap;
typedef map<string, Components::ReceptacleDescription_var> ReceptaclesMap;
typedef map<string, Components::ConsumerDescription_var> ConsumersMap;
typedef map<string, Components::EmitterDescription_var> EmittersMap;
typedef map<string, Components::PublisherDescription_var> PublishersMap;

typedef FacetsMap::iterator IterFacet;
typedef ReceptaclesMap::iterator IterReceptacle;
typedef ConsumersMap::iterator IterConsumer;
typedef EmittersMap::iterator IterEmitter;
typedef PublishersMap::iterator IterPublisher;

class ComponentServentBase : virtual public POA_Components::CCMObject
{
public:
	FacetsMap facets_;
	ReceptaclesMap receptacles_;
	ConsumersMap consumers_;
	EmittersMap emitters_;
	PublishersMap publishers_;
private:
	int nextPortID_;
protected:
	int nextPortID();
	
	//add by wsf
	::Container::CIFHook_var cifHook_;
       	PortableServer::ObjectId_var comObjectId_;
public:	
       	void setCIFHook(::Container::CIFHook_ptr cifHook);
      	 ::Container::CIFHook_ptr getCIFHook();
       
    	void setComObjectID(const PortableServer::ObjectId& objID);
    	PortableServer::ObjectId* getComObjetID();
	//end by wsf
public:
	virtual CORBA::Object_ptr provide_facet(const char* name)
		throw(Components::InvalidName);

	virtual Components::FacetDescriptions* get_all_facets()
		throw(CORBA::SystemException);
	virtual Components::FacetDescriptions* get_named_facets(const Components::NameList& nameList)
		throw(Components::InvalidName);

	virtual CORBA::Boolean same_component(CORBA::Object_ptr object_ref)
		throw(CORBA::SystemException);


	virtual Components::Cookie* connect(const char* name, CORBA::Object_ptr connection)
		throw(Components::InvalidName, 
			Components::InvalidConnection, 
			Components::AlreadyConnected, 
			Components::ExceededConnectionLimit);

	virtual void disconnect(const char* name,Components::Cookie* ck)
		throw(Components::InvalidName,
			Components::InvalidConnection,
			Components::CookieRequired,
			Components::NoConnection);

	virtual Components::ConnectionDescriptions* get_connections(const char* name)
		throw(Components::InvalidName);

	virtual Components::ReceptacleDescriptions* get_all_receptacles()
		throw(CORBA::SystemException);
	virtual Components::ReceptacleDescriptions* get_named_receptacles(const Components::NameList& nameList)
		throw(Components::InvalidName);


	virtual Components::EventConsumerBase_ptr get_consumer(const char* sink_name)
		throw(Components::InvalidName);

	virtual Components::Cookie* subscribe(const char* publisher_name,Components::EventConsumerBase_ptr subscriber)
		throw(Components::InvalidName,
			Components::InvalidConnection,
			Components::ExceededConnectionLimit);

	virtual void unsubscribe(const char* publisher_name,Components::Cookie* ck)
		throw(Components::InvalidName, 
			Components::InvalidConnection);

	virtual void connect_consumer(const char* emitter_name,Components::EventConsumerBase_ptr consumer)
		throw(Components::InvalidName, 
			Components::AlreadyConnected,
			Components::InvalidConnection);

	virtual Components::EventConsumerBase_ptr disconnect_consumer(const char* source_name)
		throw(Components::InvalidName, 
			Components::NoConnection);

	virtual Components::ConsumerDescriptions* get_all_consumers()
		throw(CORBA::SystemException);
	virtual Components::ConsumerDescriptions* get_named_consumers(const Components::NameList& nameList)
		throw(Components::InvalidName);

	virtual Components::EmitterDescriptions* get_all_emitters()
		throw(CORBA::SystemException);
	virtual Components::EmitterDescriptions* get_named_emitters(const Components::NameList& nameList)
		throw(Components::InvalidName);

	virtual Components::PublisherDescriptions* get_all_publishers()
		throw(CORBA::SystemException);
	virtual Components::PublisherDescriptions* get_named_publishers(const Components::NameList& nameList)
		throw(Components::InvalidName);

	virtual CORBA::IRObject_ptr get_component_def()
		throw(CORBA::SystemException);
	virtual Components::CCMHome_ptr get_ccm_home()
		throw(CORBA::SystemException);
	virtual Components::PrimaryKeyBase* get_primary_key()
		throw(Components::NoKeyAvailable);

	virtual void configuration_complete()
		throw(CORBA::SystemException, Components::InvalidConfiguration);

//add by wsf 2004.6.29
	virtual void run_complete()
		throw(CORBA::SystemException);

	virtual void passivate_component()
		throw(CORBA::SystemException);

	virtual void activate_component()
		throw(CORBA::SystemException);

	virtual ConfigValues* externalize()
		throw(CORBA::SystemException);

	virtual void initialize(const ConfigValues& states)
		throw(CORBA::SystemException);
//end by wsf 2004.6.29

	virtual void remove()
		throw(Components::RemoveFailure);

	virtual Components::ComponentPortDescription* get_all_ports()
		throw(CORBA::SystemException);
public:
	ComponentServentBase();
	virtual  ~ComponentServentBase();

	//additional function ,added by huangjie 2002.3.21
	virtual  CORBA::Object_ptr unsubscribeEx(const char* publisher_name,Components::Cookie* ck)
		throw(Components::InvalidName, 
			Components::InvalidConnection);

	virtual void setExecutor(CORBA::Object_ptr obj) = 0;
	virtual CORBA::Object_ptr getExecutor() = 0;

	//additional three function for passivating and activating 2003.4.3
// next 4 lines commented out by An Jingbin 2003-11-06
//	CORBA::Object_ptr activate(ContainerPSS::ASComponentHome_ptr storageHome, const char* uuid);
//	void passivate(ContainerPSS::ASComponentHome_ptr storageHome
//		, const char* uuid, CORBA::Object_ptr componentRef);
//	static CORBA::Long isActive(ContainerPSS::ASComponentHome_ptr storageHome, const char* uuid);
};

}

#endif

