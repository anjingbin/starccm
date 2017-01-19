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

#include <Container_impl.h>
//#include <ContainerBase_fwd.h>
#include <ContainerBase.h>

using namespace Container;

Container_impl::Container_impl(ContainerBase_ptr container,PortableServer::POA_ptr poa)
	:container_(container),
	poa_ (PortableServer::POA::_duplicate(poa))
{
	//modified by xiao heping 2004/03/31
//	container_ = OBJDUPLICATE(ContainerBase_ptr,container);
//	container_ = container;
	//end modify xiao heping 2004/03/31
//	poa_ = PortableServer::POA::_duplicate(poa);
}
Container_impl::~Container_impl()
{
}

PortableServer::POA_ptr
Container_impl::_default_POA()
{
	return PortableServer::POA::_duplicate(poa_);
}

//
//Accessing the configuration attribute returns the sequence of ConfigValue objects
//passed in to the create_container operation at the Container creation.
//
Components::ConfigValues*
Container_impl::configuration()
    throw(CORBA::SystemException)
{
	return container_ -> configuration();
}

//
//The get_component_server operation returns a reference to the
//ComponentServer object that created this Container.
//
Components::Deployment::ComponentServer_ptr
Container_impl::get_component_server()
    throw(CORBA::SystemException)
{
	return container_ -> get_component_server();
}

//
//The install_home operation installs and returns a new CCMHome object. The id and
//entrypt parameters are used by the container to locate an implementation file and
//instantiate a new home object. The config parameter represents a sequence of
//ConfigValue objects that provide name value pairs used to configure the installation
//of the new home instance; for example, provide persistency source, transaction, and
//security policies that must be applied to the home and its components. The operation
//raises an UnknownImplId exception if the id parameter does not correspond to any
//component packages installed using the ComponentInstallation object. The
//operation raises an ImplEntryPointNotFound exception if the entrypt parameter
//cannot be found in the implementation returned from ComponentInstallation. The
//operation raises an InstallationFailure exception if the home could not be installed
//in the container for internal reasons such as insufficient resources or inadequate
//implementation for this container; for example, installing a C++ home implementation
//in a Java container. The operation raises an InvalidConfiguration exception if the
//config parameter does not contain valid configuration name value pairs.
//
Components::CCMHome_ptr
Container_impl::install_home(const char* id,
             const char* entrypt,
             const Components::ConfigValues& config)
    throw(Components::Deployment::UnknownImplId,
          Components::Deployment::ImplEntryPointNotFound,
          Components::Deployment::InstallationFailure,
         Components::Deployment::InvalidConfiguration,
          CORBA::SystemException)
{
	return container_ -> install_home(id,entrypt,config);
}

//
//The remove_home operation removes a home from the container and all components
//that were created by this home. The operation raises a BAD_PARAM system
//exception if the href parameter was not created by this container. 
//The operation raises a RemoveFailure exception if the home or any of its components 
//could not be removed from the container for any internal reasons.
//
void
Container_impl::remove_home(Components::CCMHome_ptr href)
    throw(Components::RemoveFailure,CORBA::SystemException)
{
	container_ -> remove_home(href);
}

Components::CCMHomes*
Container_impl::get_homes()
    throw(CORBA::SystemException)
{
	return container_ -> get_homes();
}

//
//The remove operation removes the container and all homes and components created
//in the container. The operation raises a RemoveFailure exception if the container or
//any of its homes could not be removed for any internal reasons.
//
void
Container_impl::remove()
    throw(Components::RemoveFailure,
          CORBA::SystemException)
{
	//modified by xiao heping 2004/03/12
	Components::Deployment::ComponentServer_var server = container_ ->get_component_server();
	Components::Deployment::Container_var containerRef = container_ ->getReference();
	try
	{
		server -> remove_container(containerRef.in());
	}
	catch(...)
	{
		throw new Components::RemoveFailure();
	}
	//end modify xiao heping 2004/03/12
}

//wsf
STARCCM::ArchHome* 
Container_impl::getArchHome()
	throw(CORBA::SystemException)
{
	return container_ -> getArchHome();
}

//wsf
STARCCM::ArchComponent* 
Container_impl::getArchCom()
	throw(CORBA::SystemException)
{
	return container_ -> getArchCom();
}

//wsf
STARCCM::ArchConnection* 
Container_impl::getArchConn()
	throw(CORBA::SystemException)
{
	return container_ -> getArchConn();
}

//wsf
void 
Container_impl::updateArchitecture(const STARCCM::UUIDs& homeUUIDs,
							STARCCM::ArchComponents_out addedcoms, 
							STARCCM::DeletedComs_out dcomslist, 
    							STARCCM::ArchConnections_out addedconns, 
    							STARCCM::ArchConnections_out dconnslist)
	throw(CORBA::SystemException)
{
	container_ -> updateArchitecture(homeUUIDs, addedcoms, dcomslist, addedconns, dconnslist);
}

//wsf
void 
Container_impl::cleanArchObjects(const STARCCM::UUIDs& homeUUIDs)
	throw(CORBA::SystemException)
{
	container_ -> cleanArchObjects(homeUUIDs);
}

//add by wsf 2004.6.29
void 
Container_impl::setRedirectReqInfo(const char* homeUUID,
								const CORBA::StringSeq& oidList, 
								const Components::ObjectSeq& newObjList,
								CORBA::Long timeout)
	throw(CORBA::SystemException)
{
	container_->setRedirectReqInfo(homeUUID,oidList, newObjList, timeout);
}

void 
Container_impl::passivateClient(const char* homeUUID,
							const CORBA::StringSeq& oidList, 
							const Components::Deployment::PassivateClientTypeSeq& typeList)
	throw(CORBA::SystemException)
{
	container_->passivateClient(homeUUID,oidList, typeList);
}

void 
Container_impl::activateClient(const char* homeUUID,
							const CORBA::StringSeq& oidList)
	throw(CORBA::SystemException)
{
	container_->activateClient(homeUUID,oidList);
}
//end by wsf 2004.6.29
