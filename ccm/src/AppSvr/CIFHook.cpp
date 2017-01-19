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
#include <CIFHook.h>
#include <ContainerBase.h>
#include <ContainerRunTime.h>
#include <Session2Context_impl.h>
#include <Entity2Context_impl.h>


using namespace Container;

CIFHook::CIFHook(ContainerRunTime_ptr runtime)
{
	//modified by xiao heping 2004/03/29
//	runtime_ = OBJDUPLICATE(ContainerRunTime_ptr,runtime);
	runtime_ = runtime;
	//end modify xiao heping 2004/03/29
}

CIFHook::~CIFHook()
{
}

//ContainerHook_ptr
//ContainerHook::hook()
//{
//	return OBJDUPLICATE(ContainerHook_ptr,hook_);
//}

void
CIFHook::onCreateHome()
{
	
}

ContainerRunTime_ptr
CIFHook::getContainerRunTime()
{
	return OBJDUPLICATE(ContainerRunTime_ptr,runtime_);
}

void
CIFHook::onCreateComponent(::Container::ComponentComposition_ptr comp)
{
	runtime_ -> onCreateComponent(comp);
}

void
CIFHook::onRemoveComponent(const PortableServer::ObjectId& oid)
{
	runtime_ -> onRemoveComponent(&oid);
}

CORBA::Object_ptr
CIFHook::createReference(const PortableServer::ObjectId& oid,const char* repId)
{
//	TRACE0(LEVEL6,"Using CIF Hook create reference\n");
	
	PortableServer::POA_var poa = runtime_->getComponentPoa();
	
//	CORBA::String_var s = PortableServer::ObjectId_to_string(oid);
//	TRACE1(LEVEL6,"create reference with object id %s ",s);
//	TRACE1(LEVEL6,"Repository id %s\n",repId);

	CORBA::Object_var ref = poa->create_reference_with_id(oid,repId);
	
	return ref._retn();
}

PortableServer::POA_ptr
CIFHook::getHomePOA()
{
	return runtime_ -> getHomePoa();
}

PortableServer::POA_ptr
CIFHook::getComponentPOA()
{
	return runtime_ -> getComponentPoa();
}

Components::HomeExecutorBase_ptr
CIFHook::getHomeExecutorBase()
{
	return runtime_ -> getHomeExecutor();
}

//Components::EnterpriseComponent_ptr
//CIFHook::getExecutor()
//{
//	return runtime_ -> getExecutor();
//}

CORBA::Object_ptr
CIFHook::createReference2(const PortableServer::ObjectId& oid,
                          int facetId,
                          CORBA::OctetSeq* pid,
                          const char* repId)
{
    TRACE0(LEVEL6, "enter CIFHook createReference2\n");
    
    CORBA::Object_var ref;
    PortableServer::POA_var poa;
    
    poa = runtime_->getComponentPoa();
    
//    switch( runtime_->getContainerType() )
    if( pid == NULL )
    {
//        case SERVICECONTAINER:
//        case SESSIONCONTAINER:
		ref = poa->create_reference_with_id(oid, repId);
		return ref._retn();
     }
     else
     {
//        case ENTITYCONTAINER:
//        case PROCESSCONTAINER:
        char buf[5];
        sprintf((char*)&buf, "%4d" ,facetId);
        CORBA::OctetSeq_var id = new CORBA::OctetSeq;
        id->length(pid->length() + 4);
        
        id[0] = buf[0];
        id[1] = buf[1];
        id[2] = buf[2];
        id[3] = buf[3];
        
        for( int i = 0; i < pid->length(); ++i )
        {
            id[i + 4] = (*pid)[i];
        }
        
//      poa = runtime_->getComponentPoa();
        
        PortableServer::ObjectId_var myId = new PortableServer::ObjectId();
        
        // assign object id.Is there other way?
        
        myId->length(id->length());
        for(int j = 0; j < id->length(); j++ )
        {
            if( id[j] != 0 )
            {
                myId[j] = id[j];
            }
            else
            {
                myId[j] = 30;
            }
        }
        ref = poa->create_reference_with_id(myId, repId);
        TRACE0(LEVEL6, "CIFHook createReference2 successfully return\n");
        return ref._retn();
//        default:
//            //error
//            break;
    }
    TRACE0(LEVEL6, "CIFHook createReference2 NULL return\n");
    return NULL;
}

CORBA::Object_ptr
CIFHook::onFind(CORBA::OctetSeq* pid)
{
    TRACE0(LEVEL6, "enter CIFHook onFind\n");
    
    CORBA::Object_var ref;
    
    char buf[5];
    sprintf((char*)&buf, "%4d" ,0);
    CORBA::OctetSeq_var id = new CORBA::OctetSeq;
    id->length(pid->length() + 4);
    
    id[0] = buf[0];
    id[1] = buf[1];
    id[2] = buf[2];
    id[3] = buf[3];
    
    for( int i = 0; i < pid->length(); ++i )
    {
        id[i + 4] = (*pid)[i];
    }
    
    //check if component composition is in memory
    //omitted here
    
    
    PortableServer::POA_var poa = runtime_->getComponentPoa();
    
    PortableServer::ObjectId* idPtr = new PortableServer::ObjectId();
	        
    // assign object id.Is there other way?
    
    idPtr->length(id->length());
    for(int j = 0; j < id->length(); j++ )
    {
        if( id[j] != 0 )
        {
            idPtr->operator[](j) = id[j];
        }
        else
        {
            idPtr->operator[](j) = 30;
        }
    }
    
 	//modified by xiao heping 2004/03/24
    //PortableServer::Servant servant = runtime_->getServant(idPtr);
	PortableServer::Servant servant = runtime_->getComponentServant(idPtr);
	//end modify
    
    if( servant == NULL )
    {
        TRACE0(LEVEL6, "CIFHook onFind servant not FOUND\n");
        return NULL;
    }
    
    PortableServer::ObjectId_var myId = idPtr;
    
    ref = poa->create_reference_with_id(myId, runtime_->getCompRepId());
    
    TRACE0(LEVEL6, "CIFHook onFind successful return\n");
    
    return ref._retn();
}

void
CIFHook::hookContext(Components::CCMContext_ptr ctx)
{
    
}

CORBA::ORB_ptr
CIFHook::getOrb()
{
    ContainerBase_var container = runtime_ -> getContainer();
//  CORBA::ORB_var orb = container -> getOrb();
//  return orb._retn();
    return container -> getOrb();
}

PortableServer::ObjectId*
CIFHook::createObjectId(const PortableServer::ObjectId& oid,int facetId,CORBA::OctetSeq* pid)
{
    if( pid != NULL )
    {
        char buf[5];
        sprintf((char*)&buf, "%4d" ,facetId);
        CORBA::OctetSeq_var id = new CORBA::OctetSeq;
        id->length(pid->length() + 4);
        
        id[0] = buf[0];
        id[1] = buf[1];
        id[2] = buf[2];
        id[3] = buf[3];
        
        for( int i = 0; i < pid->length(); ++i )
        {
            id[i + 4] = (*pid)[i];
        }
        
        PortableServer::ObjectId_var myId = new PortableServer::ObjectId();
        
        myId->length( id->length() );
        for(int j = 0; j < id->length(); j++ )
        {
            if( id[j] != 0 )
            {
                myId[j] = id[j];
            }
            else
            {
                myId[j] = 30;
            }
        }
        
        return myId._retn();
    }
    else
    {
        PortableServer::ObjectId_var myId = new PortableServer::ObjectId(oid);
        
        return myId._retn();
    }
    
}

::Components::Session2Context_ptr 
CIFHook::getSession2ctx()
{
	::Container::Session2Context_impl* impl = new ::Container::Session2Context_impl();
	impl -> setContainerRunTime(runtime_);
//	ContainerBase_var container = runtime_ -> getContainer();
	::STARCCM::ResourcePool_var resPool = runtime_ -> getResourcePool();
	
//modified by xiao heping 2004/06/30
#ifdef WITH_OTS
	impl -> setResourcePool(resPool);
#endif
//end modify

	::Components::Session2Context_var session2ctx = impl;
	return session2ctx._retn();
}



::Components::Entity2Context_ptr 
CIFHook::getEntity2ctx()
{
	::Container::Entity2Context_impl* impl = new ::Container::Entity2Context_impl();
	impl -> setContainerRunTime(runtime_);
//	ContainerBase_var container = runtime_ -> getContainer();
	::STARCCM::ResourcePool_var resPool = runtime_ -> getResourcePool();
	
//modified by xiao heping 2004/06/30
#ifdef WITH_OTS
	impl -> setResourcePool(resPool);
#endif
//end modify

	::Components::Entity2Context_var entity2ctx = impl;
	return entity2ctx._retn();
}

//wsf
void
CIFHook::setArchComp(Container::ComponentComposition_ptr comp)
{
	runtime_ -> setArchComp(comp);
}

//wsf
void 
CIFHook::onConnect(STARCCM::ArchConnection& acon)
{
	runtime_ -> onConnect(acon);
}

//wsf
void 
CIFHook::onDisconnect(PortableServer::ObjectId* srcoid, 
						const char* featurename, 
						const char* ck)
{
	runtime_ -> onDisconnect(srcoid, featurename, ck);
}

