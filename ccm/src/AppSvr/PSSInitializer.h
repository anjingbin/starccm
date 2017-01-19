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

#ifndef __PSSInitializer_h__
#define __PSSInitializer_h__

#include <CORBA.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ContainerBase_fwd.h>
#include <Loader.h>
#include <CosPersistentState.h>

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

class PSSInitializer;
OBJPTR(PSSInitializer)
OBJVAR(PSSInitializer)

class PSSInitializer : REF_COUNT_LOCAL_OBJECT
{
    private:
        CORBA::String_var connectorId_;
        short accessMode_;
        CosPersistentState::ParameterList* parameters_;
        
    public:
        PSSInitializer(const char* connectId, short accessMode, CosPersistentState::ParameterList* parameters);
        virtual ~PSSInitializer();
    
        char* getConnectorId();
        short getAccessMode();
        CosPersistentState::ParameterList* getPatemeters();
    
};

}

#endif

