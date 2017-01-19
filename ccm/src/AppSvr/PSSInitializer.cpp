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

#include <PSSInitializer.h>


using namespace Container;

   
PSSInitializer::PSSInitializer(const char* connectId, short accessMode,CosPersistentState::ParameterList* parameters)
{
	connectorId_ = CORBA::string_dup(connectId);
	accessMode_ = accessMode;
	parameters_ = new CosPersistentState::ParameterList(*parameters);
	
}

PSSInitializer::~PSSInitializer()
{
    
}

char* 
PSSInitializer::getConnectorId()
{
	return CORBA::string_dup(connectorId_);
}

short
PSSInitializer::getAccessMode()
{
	return accessMode_;
}

CosPersistentState::ParameterList*
PSSInitializer::getPatemeters()
{
	return new CosPersistentState::ParameterList(*parameters_);
}

