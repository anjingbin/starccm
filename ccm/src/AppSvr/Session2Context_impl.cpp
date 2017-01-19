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

#include <Session2Context_impl.h>
#include <ContainerRunTime.h>
#include <ContainerBase.h>

using namespace Container;

Session2Context_impl::Session2Context_impl()
{
}

Session2Context_impl::~Session2Context_impl()
{
}

//
// IDL:omg.org/Components/Session2Context/create_ref:1.0
//
CORBA::Object_ptr
Session2Context_impl::create_ref(const char* repid)
{
	CORBA::ORB_var orb = runtime_ -> getContainer() -> getOrb();
	PortableServer::POA_var poa = runtime_ -> getComponentPoa();
	
	PortableServer::Current_var poaCurrent;
	
	CORBA::Object_var obj = orb -> resolve_initial_references("POACurrent");

	poaCurrent = PortableServer::Current::_narrow(obj.in());
	
	PortableServer::ObjectId_var oid;
	try
	{
		oid = poaCurrent->get_object_id();
	}
	catch(PortableServer::Current::NoContext&)
	{
		TRACE0(LEVEL3,"No Context Exception\n");
	}
	
	CORBA::Object_var ref;
	
	ref = poa -> create_reference_with_id(oid,repid);
	
	return ref._retn();
}

//
// IDL:omg.org/Components/Session2Context/create_ref_from_oid:1.0
//
CORBA::Object_ptr
Session2Context_impl::create_ref_from_oid(const ::CORBA::OctetSeq& oid,
                                              const char* repid)
{
	CORBA::Object_var obj;
	//CORBA::String_var strOctSeq_ = CORBA::string_dup(oid.get_buffer(false));
	::CORBA::OctetSeq* seqOctet = new ::CORBA::OctetSeq(oid);
	
	//for release
	::CORBA::OctetSeq_var seq = seqOctet;
	
	PortableServer::ObjectId_var objectId 
		= reinterpret_cast<PortableServer::ObjectId*>(seqOctet);
		
    PortableServer::POA_var poa = runtime_ -> getComponentPoa();
    
	obj = poa -> create_reference_with_id(objectId,repid);
	
	return obj._retn();
}

//
// IDL:omg.org/Components/Session2Context/get_oid_from_ref:1.0
//
::CORBA::OctetSeq*
Session2Context_impl::get_oid_from_ref(CORBA::Object_ptr objref)
{
	PortableServer::ObjectId* objectId;
	
	PortableServer::POA_var poa = runtime_ -> getComponentPoa();
	
	try
	{
		objectId = poa->reference_to_id(objref);
	}
	catch(PortableServer::POA::WrongAdapter&)
	{
		TRACE0(LEVEL3,"WrongAdapter Exception in context\n");
	}

	return reinterpret_cast<CORBA::OctetSeq*>(objectId);
//	return objectId._retn();
}

