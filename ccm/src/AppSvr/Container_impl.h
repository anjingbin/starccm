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

#ifndef __Container_impl_h__
#define __Container_impl_h__

#include <CORBA.h>
#include <ContainerBase_fwd.h>
#include <Deployment_skel.h>

using namespace Container;

class Container_impl : public virtual POA_Components::Deployment::Container,
						virtual public PortableServer::RefCountServantBase
{
protected:
	//modified by xiao heping 2004/03/31
//	ContainerBase_var container_;
	ContainerBase_ptr container_;
	//end modify xiao heping 2004/03/31

	PortableServer::POA_var poa_;

public:
	Container_impl(ContainerBase_ptr container,PortableServer::POA_ptr poa);
	virtual ~Container_impl();

	virtual PortableServer::POA_ptr _default_POA();
	
    //
    // IDL:omg.org/Components/Deployment/Container/configuration:1.0
    //
    virtual Components::ConfigValues* configuration()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/Deployment/Container/get_component_server:1.0
    //
    virtual Components::Deployment::ComponentServer_ptr get_component_server()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/Deployment/Container/install_home:1.0
    //
    virtual Components::CCMHome_ptr install_home(const char* id,
                                                 const char* entrypt,
                                                 const Components::ConfigValues& config)
        throw(Components::Deployment::UnknownImplId,
              Components::Deployment::ImplEntryPointNotFound,
              Components::Deployment::InstallationFailure,
              Components::Deployment::InvalidConfiguration,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Deployment/Container/remove_home:1.0
    //
    virtual void remove_home(Components::CCMHome_ptr href)
        throw(Components::RemoveFailure,
              CORBA::SystemException);

    //
    // IDL:omg.org/Components/Deployment/Container/get_homes:1.0
    //
    virtual Components::CCMHomes* get_homes()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/Components/Deployment/Container/remove:1.0
    //
    virtual void remove()
        throw(Components::RemoveFailure,
              CORBA::SystemException);

    //add by wsf
    virtual STARCCM::ArchHome* getArchHome()
    	throw(CORBA::SystemException);
    virtual STARCCM::ArchComponent* getArchCom()
    	throw(CORBA::SystemException);
    virtual STARCCM::ArchConnection* getArchConn()
    	throw(CORBA::SystemException);
    virtual void updateArchitecture(const STARCCM::UUIDs& homeUUIDs,
    						STARCCM::ArchComponents_out addedcoms, 
    						STARCCM::DeletedComs_out dcomslist, 
						STARCCM::ArchConnections_out addedconns, 
						STARCCM::ArchConnections_out dconnslist)
    	throw(CORBA::SystemException);
    virtual void cleanArchObjects(const STARCCM::UUIDs& homeUUIDs)
    	throw(CORBA::SystemException);

    //end by wsf

    //add by wsf 2004.6.29
	virtual void setRedirectReqInfo(const char* homeUUID,
								const CORBA::StringSeq& oidList, 
								const Components::ObjectSeq& newObjList,
								CORBA::Long timeout)
		throw(CORBA::SystemException);

	virtual void passivateClient(const char* homeUUID,
							const CORBA::StringSeq& oidList, 
							const Components::Deployment::PassivateClientTypeSeq& typeList)
		throw(CORBA::SystemException);
	
	virtual void activateClient(const char* homeUUID,
							const CORBA::StringSeq& oidList)
		throw(CORBA::SystemException);
    //end by wsf 2004.6.29
                      
};

#endif

