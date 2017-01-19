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

#include <EntityContext_impl.h>
#include <ContainerBase.h>
#include <ContainerRunTime.h>

using namespace Container;

//
// IDL:omg.org/Components/EntityContext:1.0
//
EntityContext_impl::EntityContext_impl()
{
}

EntityContext_impl::~EntityContext_impl()
{
}

//
// IDL:omg.org/Components/EntityContext/get_CCM_object:1.0
//
CORBA::Object_ptr
EntityContext_impl::get_CCM_object()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation
    ContainerBase_var container = runtime_ -> getContainer();
	CORBA::ORB_var orb = container -> getOrb();
	PortableServer::Current_var poaCurrent;
	
	CORBA::Object_var obj = orb -> resolve_initial_references("POACurrent");

	poaCurrent = PortableServer::Current::_narrow(obj.in());
	
	CORBA::Object_var ref;

	try
	{
		//NOT implemented in Orbacus4.1
		//ref = poaCurrent->get_reference();

		//So we use RepositoryID and ObjectID to create references

		PortableServer::POA_var poa = poaCurrent->get_POA();

		PortableServer::ObjectId_var oid = poaCurrent->get_object_id();
		
		PortableServer::ObjectId* myId = new PortableServer::ObjectId(oid.in());
		PortableServer::Servant servant = runtime_->getComponentServant(myId);
		delete myId;
        
        if( servant == NULL )
        {
            return NULL;
        }
        
		CORBA::RepositoryId_var repId;
#ifndef TAO
		repId = 
			servant->_primary_interface(oid.in(), poa);
#else
		//IMPORTANT!!NOT imlemantation here!
//		repId = servant->_interface_repository_id();
#endif
		ref = poa->create_reference_with_id(oid.in() ,repId.in());
	}
	catch(PortableServer::Current::NoContext)
	{
		TRACE0(LEVEL3,"No Context Exception : PortableServer::Current::NoContext\n");
	}
	
	return ref._retn();
}

//
// IDL:omg.org/Components/EntityContext/get_primary_key:1.0
//
Components::PrimaryKeyBase*
EntityContext_impl::get_primary_key()
    throw(Components::IllegalState,
          CORBA::SystemException)
{
    // TODO: Implementation
    Components::PrimaryKeyBase* _r = 0;
    return _r;
}

