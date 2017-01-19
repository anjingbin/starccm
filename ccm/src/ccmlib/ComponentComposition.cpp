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

#include <ComponentComposition.h>

using namespace Container;

ComponentComposition::ComponentComposition()
{
	//modified by xiao heping 2004/02/14
	
	//status_ = FREE;
	setCompositionStatus(FREE);

	//end modified by xiao heping 2004/02/14

}

ComponentComposition::~ComponentComposition()
{
}

Components::EnterpriseComponent_ptr
ComponentComposition::getComponentExecutor()
{
//	Guard<FastRecursiveMutex> guard(executorLock_);
	
	Components::EnterpriseComponent_var executor = Components::EnterpriseComponent::_narrow(getExecutor());	
	return executor._retn();
}

void
ComponentComposition::registerComponentExecutor(Components::EnterpriseComponent_ptr executor)
{
//	Guard<FastRecursiveMutex> guard(executorLock_);
	
	CORBA::Object_var obj = Components::EnterpriseComponent::_duplicate(executor);

	setExecutor(obj.in());

}

//Components::HomeExecutorBase_ptr
//ComponentComposition::getHomeExecutor()
//{
//	return NULL;
//}

//PortableServer::Servant
//ComponentComposition::getHomeServant()
//{
//	return NULL;
//}

//PortableServer::ObjectId*
//ComponentComposition::getHomeObjectId()
//{
//	Guard<FastRecursiveMutex> guard(lock);
//	PortableServer::ObjectId_var oid = homeComposition_->getObjectId();
//	
//	return oid._retn();
//}

PortableServer::Servant
ComponentComposition::getComponentServant()
{
//	Guard<FastRecursiveMutex> guard(lock_);

	//modified by xiao heping 2004/02/14
			
	//return servant_;
	return CompositionManager::getServant();

	//end modified by xiao heping 2004/02/14
}

void
ComponentComposition::registerComponentServant(const PortableServer::ObjectId& oid,PortableServer::Servant servant)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	TRACE0(LEVEL4,"Registering component object ID and servant\n");
	PortableServer::ObjectId* objectId = new PortableServer::ObjectId(oid);
	setObjectId(objectId);
	setServant(servant);
	
	delete objectId;
}

PortableServer::Servant
ComponentComposition::getServant(PortableServer::ObjectId* oid)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	TRACE0(LEVEL4,"Finding servant from ComponentComposition\n");

	PortableServer::Servant servant;
	
	CORBA::String_var s = PortableServer::ObjectId_to_string(*oid);
	std::string key(s.in());
	
	//TRACE1(LEVEL6,"Target object ID : %s\n",key.c_str());
	
	CORBA::String_var mainString = PortableServer::ObjectId_to_string(objectId_.in());
	
	string mainServantKey(mainString.in());
	
	//TRACE1(LEVEL6,"Component servant base object id : %s\n",mainServantKey.c_str());
	
	if( strcmp(key.c_str(),mainServantKey.c_str()) == 0 )
	{
		TRACE0(LEVEL6,"the target servant is component servant\n");

		//modified by xiao heping 2004/02/14
		
		//servant = servant_;
		servant=CompositionManager::getServant();

		//end modified by xiao heping 2004/02/14
		
	}
	else
	{
		TRACE0(LEVEL6,"the target servant is NOT component servant\n");
		ServantMap::const_iterator result;
		//TRACE1(LEVEL6,"find port servant with id %s\n",key.c_str());
		result = portServants_.find(key.c_str());
		TRACE0(LEVEL6,"found object\n");
		
		if( result != portServants_.end() )
		{
			TRACE0(LEVEL5,"Servant found,returning\n");
			servant = (*result).second;
		}
		else
		{
			TRACE0(LEVEL5,"have NOT found any matched servant\n");
			servant = NULL;
		}
	}

	TRACE0(LEVEL4,"End of finding servant\n");
	return servant;
}

void
ComponentComposition::registerPortServant(const PortableServer::ObjectId& oid,PortableServer::Servant servant)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	TRACE0(LEVEL4,"Registering Port object ID and servant\n");
	CORBA::String_var s = PortableServer::ObjectId_to_string(oid);
	std::string key(s.in());
//	TRACE1(LEVEL6,"Inserting Object ID : %s\n",s);
//	TRACE1(LEVEL6,"Inserting servant %x\n",servant);
	
	pair<ServantMap::iterator, bool> result;
	result = portServants_.insert(ServantMap::value_type(key.c_str(),servant));
	
	if( result.second == false)
	{
		TRACE0(LEVEL4,"Can NOT insert port into composition\n");
	}
}

ObjectIdVector*
ComponentComposition::getPortObjectIdVector()
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	ObjectIdVector* ids = new ObjectIdVector;
	
	ServantMap::iterator pos = portServants_.begin();
	int len = portServants_.size();
	
	for( int i = 0 ; i < len ; pos++,i++ )
	{
		string key = (*pos).first;

		ids->push_back(key);
	}
	
	return ids;
}

int
ComponentComposition::isPort(PortableServer::ObjectId* oid)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	TRACE0(LEVEL4,"Conforming object ID\n");
	string key = PortableServer::ObjectId_to_string(*oid);
	ServantMap::const_iterator result;
	
	result = portServants_.find(key);
	
	if( result == portServants_.end() )
	{
		return 0;
		TRACE1(LEVEL6,"target object ID %s is not a port\n",key.c_str());
	}
	
	return 1;
	TRACE0(LEVEL4,"End of conforming object ID\n");
	
}

int
ComponentComposition::compositionType()
{
	return COMPONENT;
}


void
ComponentComposition::passivate()
{
	//modified by xiao heping 2004/02/14
	
	//status_ = PASSIVATED;
	setCompositionStatus(PASSIVATED);

	//end modified by xiao heping 2004/02/14
	
}

void
ComponentComposition::activate()
{
	
	//modified by xiao heping 2004/02/14
	
	//status_ = FREE;
	setCompositionStatus(FREE);

	//end modified by xiao heping 2004/02/14
}


//SuLiang add 2003.4.2
void
ComponentComposition::registerComponentRef(CORBA::Object_ptr comp)
{
	componentRef_ = CORBA::Object::_duplicate(comp);
}

CORBA::Object_ptr
ComponentComposition::getComponentRef()
{
	//modified by xiao heping 2004/03/12
	//return componentRef_._retn();
	return CORBA::Object::_duplicate(componentRef_);
	//end modify
}
