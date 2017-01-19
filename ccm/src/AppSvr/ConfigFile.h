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

#ifndef __ConfigFile_h__
#define __ConfigFile_h__

#include <CORBA.h>
#include <TxnInterceptor_fwd.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>

#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>

#include <iostream>

#ifdef WIN32
#include <direct.h>
#endif

#include <XMLParser.h>

using namespace ::STARCCM::Deployment;

//Comment is added by huangjie 2003.2.27
//The following four lines will be no use when compiling.
//But it is useful for Microsoft Visual Studio to recognise 
//some type which we will define.
#if (!defined OBJVAR)&&(!defined OBJPTR)
#define OBJVAR(CLASSNAME) typedef OB::ObjVar< CLASSNAME > CLASSNAME##_var;
#define OBJPTR(CLASSNAME) typedef CLASSNAME * CLASSNAME##_ptr;
#endif

namespace Container
{

class ConfigFile;
OBJPTR(ConfigFile)
OBJVAR(ConfigFile)

class ConfigFile : REF_COUNT_LOCAL_OBJECT
{
    CORBA::String_var file_;
    XMLParser* parser_;
    
	//deleted by xiao heping 2004/02/15
	//CORBA::String_var uuid_;
    
    public:
       ConfigFile();
       virtual ~ConfigFile();
       
	   //deleted by xiao heping 2004/02/15
       //void setUuid(const char* uuid);
       //char* getUuid();
	   //end deleted

       //for ComponentServer
       CORBA::StringSeq* getContainerIds(const char* uuid);
       CORBA::StringSeq* getComponentIds(const char* uuid);
       char* getRepositoryLocation();
       short getPort(const char* uuid);
       
       //for component implementation
//       CORBA::StringSeq* getComponentUuid(const char* uuid);
       
       //for servant
       char* getServantFile(const char* uuid);
       char* getServantEntry(const char* uuid);
       char* getContextEntry(const char* uuid);
       
       //for executor
       char* getExecutorFile(const char* uuid);
       char* getExecutorRepId(const char* uuid);
       char* getExecutorEntry(const char* uuid);
       char* getHomeRepId(const char* uuid);
       
       //for valuetype
       CORBA::StringSeq* getValueTypeFile(const char* uuid);
       CORBA::StringSeq* getValueTypeFactoryRepId(const char* uuid);
       CORBA::StringSeq* getValueTypeFactoryEntry(const char* uuid);
       CORBA::StringSeq* getValueTypeEntry(const char* uuid);
       
       //other component attributes
       int getComponentKind(const char* uuid);
       int getComponentThreading(const char* uuid);
       int getConfigurationComplete(const char* uuid);
       
       //transaction policies
       CORBA::StringSeq* getHomeMethodPolicies(const char* uuid, const char* repId);
       CORBA::StringSeq* getComponentRepIds(const char* uuid);
       CORBA::StringSeq* getComponentMethodPolicies(const char* uuid, const char* repId);
       
       //for persistent state
       // return value 0: self-management persistence
       // return value 1: container management persistence
       // SMP default
       int getPersistentPolicy(const char* uuid);
       // return value 0: transactional persistence
       // return value 1: non-transactional persistence
       // non-transactional default
       int getPersistentTxnPolicy(const char* uuid);
       // return value 0: read only
       // return value 1: read write
       // read-write default
       int getAccessMode(const char* uuid);
       // reutrn value 0: read uncommitted
       // reutrn value 1: read committed
       // reutrn value 2: repeatable read
       // reutrn value 3: serializable
       // serializable default
       int getisolationLevel(const char* uuid);
       char* getStorageHomeId(const char* uuid);
       char* getPersistentFile(const char* uuid);
       
       //ResourcePool
       CORBA::StringSeq* getResourcePools(const char* uuid);
       CORBA::StringSeq* getResourcePoolProperties(const char* uuid, const char* name);
       
       //Common CORBA Service
       char* getCosRef(const char* uuid, const char* name);
       
       //add & remove entry functions from config file
       void addComponentServer(const char* uuid);
       void removeCompnentServer(const char* uuid);
       
	   //modified by xiao heping 2004/02/15
       //void addContainer(const char* uuid);
       //void removeContainer(const char* uuid);
	   void addContainer(const char* ComponentServerUuid,const char* uuid);
       void removeContainer(const char* ComponentServerUuid,const char* uuid);
	   //end modified
	   
       
       //dont need removeXXX here,update instead
       //addXXX functions as updateXXX, too
       void addCosRef(const char* uuid, const char* name, const char* ref);
       
       void addInstalledComponent(const char* container, const char* uuid);
       void removeInstalledComponent(const char* uuid);
       
       //for servant dll
       //addXXX functions as updateXXX, too
       void addServantFile(const char* uuid, 
                           const char* servantEntryPoint, 
                           const char* contextEntryPoint,
                           const char* name);
       
       //for executor dll
       //addXXX functions as updateXXX, too
       void addExecutorFile(const char* uuid, 
                            const char* entryPoint, 
                            const char* repId,
                            const char* compId,
                            const char* name);
       
       //for valuetype
       //addXXX functions as updateXXX, too
       void addValueTypeFile(const char* uuid,
                              const char* valueEntryPoint, 
                              const char* factoryEntryPoint, 
                              const char* repId,
                              const char* name);
       
       //addXXX functions as updateXXX, too
       void addComponentKind(const char* uuid, int kind);
       
       //addXXX functions as updateXXX, too
       void addComponentThreading(const char* uuid, int isMultiThreaded);
       
       //addXXX functions as updateXXX, too
       void addConfigurationComplete(const char* uuid, int configurationCompleted);
       
       //addXXX functions as updateXXX, too
       void addHomeTransactionPolicy(const char* uuid, 
                                     const char* opName, 
                                     const char* repId, 
                                     const char* policy);
       
       void addHomeTransactionPolicy(const char* uuid, 
                                     const char* opName, 
                                     const char* repId, 
                                     TxnPolicy policy);
       
       void addCompTransactionPolicy(const char* uuid, 
                                     const char* opName, 
                                     const char* repId, 
                                     const char* policy);
       
       void addCompTransactionPolicy(const char* uuid, 
                                     const char* opName, 
                                     const char* repId, 
                                     TxnPolicy policy);
       
       void addReourcePool(const char* uuid, CORBA::StringSeq* ss);
       
       void updatePort(const char* uuid, short port);
       
       // return value 0: self-management persistence
       // return value 1: container management persistence
       void setPersistentPolicy(const char* uuid, int isCmp);
       // return value 0: transactional persistence
       // return value 1: non-transactional persistence
       void setPersistentTxnPolicy(const char* uuid, int isTransactional);
       // return value 0: read only
       // return value 1: read write
       void setAccessMode(const char* uuid, int mode);
       // reutrn value 0: read uncommitted
       // reutrn value 1: read committed
       // reutrn value 2: repeatable read
       // reutrn value 3: serializable
       void setIsolationLevel(const char* uuid, int level);
       void setStorageHomeId(const char* uuid, const char* id);
       void setPersistentFile(const char* uuid, const char* name);
       
};

}

#endif

