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
// *********************************************************************

// Version: 1.0.0

#include <ComponentsPool.h>
#include <HomeComposition.h>
#include <ComponentComposition.h>
#include <Delegation.h>

#include <Cookie.h>  //added by xiao heping 

//added by xiao heping 2004/03/28
#include <ComponentServentBase.h>
//end add xiao heping 2004/03/28

using namespace std;
using namespace Container;

ComponentsPool::ComponentsPool()
{
}

ComponentsPool::~ComponentsPool()
{
	TRACE0(LEVEL6,"ComponentsPool::~ComponentsPool()\n");
}

ComponentComposition_ptr
ComponentsPool::getComposition(PortableServer::ObjectId* oid)
{
	TRACE0(LEVEL5,"Search compositionManager from composition pool\n");
	CORBA::String_var s = PortableServer::ObjectId_to_string(*oid);
	std::string key(s.in());
	TRACE1(LEVEL6,"Object ID is %s\n",key.c_str());

	//added by xiao heping 2004/03/25
	STARCCM_SYNCHRONIZED(sync_,*this)
	//end add xiao heping 2004/03/25

	//modified by xiao heping 2004/03/18
	ComponentComposition_var compComposition;
	ActiveObjectMap::iterator iter;

	iter = activeObjectMap_.find(key);	
	TRACE0(LEVEL6,"Search...\n");	
	if( iter == activeObjectMap_.end())
	{
		TRACE0(LEVEL6,"Have NOT find any matched composition\n");
		return NULL;
	}
	else
	{
		TRACE0(LEVEL6,"Matched composition found\n");
		compComposition = iter -> second;

		/*
		PortableServer::ObjectId_var mainId = component->getObjectId();
		CORBA::String_var mainKey = PortableServer::ObjectId_to_string(mainId);
		
		std::string stringKey(mainKey.in());
		lru_.remove(stringKey);
		lru_.push_front(stringKey);
		*/
		
		return compComposition._retn();
	}
	//end modify xiao heping 2004/03/18
}

//deleted by xiao heping 2004/03/18
/*
CORBA::Object_ptr
ComponentsPool::getExecutor(PortableServer::ObjectId* oid)
{
//	Guard<FastRecursiveMutex> guard(lock_);
	
	CORBA::Object_var executor;
	
	ComponentComposition_var component = getComposition(oid);
	
	executor = component -> getExecutor();
	
	assert( !CORBA::is_nil(executor) );
	
	return executor._retn();
}
*/
//end delete xiao heping 2004/03/18

PortableServer::Servant
ComponentsPool::getServant(PortableServer::ObjectId* oid,
						   PortableServer::ServantLocator::Cookie& the_cookie,
						   const char* operation)
{
	TRACE0(LEVEL5,"Searching servant from composition pool\n");

	//added by xiao heping 2004/03/25
	STARCCM_SYNCHRONIZED(sync_,*this)
	//end add xiao heping 2004/03/25
	ComponentComposition_var compComposition = this->getComposition(oid);
	if( CORBA::is_nil (compComposition))
	{
		TRACE0(LEVEL3,"Do not find any matched Composition Manager\n");
		return NULL;
	}

	
	//PortableServer::Servant servant = compComposition -> getServant(oid);

	//deleted by xiao heping 2004/03/17
	/*	
	TRACE0(LEVEL6,"Set delegation\n");
	
	CORBA::Object_var executor = compComposition -> getComponentExecutor();
	
	if( executor.in() == NULL )
	{
		TRACE0(LEVEL2,"ERROR:NULL executor pointer\n");
	}

	Components::Delegation* delegation = dynamic_cast<Components::Delegation*>(servant);
	
	assert( delegation != NULL );
	
	delegation -> setExecutor(executor.in());
	
	TRACE0(LEVEL6,"End of set delegation\n");
	*/
	//end delete xiao heping 2004/03/17

	
	/* check if executor and storage objects having been passivated */
	
	/* if passivated, then activated it */
	
	TRACE0(LEVEL5,"End of searching servant from composition pool\n");

	//added by xiao heping 2004/03/25
//	the_cookie = (void*)dynamic_cast<CompositionManager*>(compComposition.in());
	::Container::Cookie* cookie = new ::Container::Cookie(compComposition.in());
	the_cookie = (void*)cookie;

	PortableServer::Servant servant = compComposition -> getServant(oid);
	servant ->_add_ref();
	compComposition->_add_ref();
	//end add xiao heping 2004/03/25

	//added by xiao heping 2004/03/28
	try
	{
		int isPort = compComposition->isPort(oid) ;
		if(!isPort && (strcmp(operation,"remove") == 0))
			removeComponent(oid);
	}
	catch(...)
	{
	}
	//end add xiao heping 2004/03/28


//	return compComposition -> getServant(oid);
	return servant;
}

void
ComponentsPool::addComponent(ComponentComposition_ptr comp)
{
//	modified by xiao heping 2004/03/18

	TRACE0(LEVEL5,"Add a component to pool\n");
	
	//here is component
	ComponentComposition_var componentComposition = OBJDUPLICATE(ComponentComposition_ptr,comp);

	//deleted  by xiao heping 2004/03/17
//	componentComposition -> _add_ref();	
//	CORBA::Object_var component_ = componentComposition -> getComponentRef();
//	component_ -> _add_ref(); 
	//end delete xiao heping 2004/03/17
	
	PortableServer::ObjectId_var objectId = componentComposition -> getObjectId();
	CORBA::String_var s = PortableServer::ObjectId_to_string(objectId);
	TRACE1(LEVEL6,"Component object id is %s\n",s.in());
	string key(s.in());

	//get all ports
	ObjectIdVector* objectIdVector = componentComposition -> getPortObjectIdVector();
	int loopLength = objectIdVector -> size();

	STARCCM_SYNCHRONIZED(sync_,*this)

	activeObjectMap_.insert(ActiveObjectMap::value_type(key,componentComposition));
	
	//add by wkb for passivate 2003/04/02
	//lru_.push_front(key);
	
	for( int i = 0;i < loopLength;i++ )
	{
		string portKey = (*objectIdVector)[i];
		TRACE1(LEVEL6,"Port object id is %s\n",portKey.c_str());
		activeObjectMap_.insert(ActiveObjectMap::value_type(portKey,componentComposition));
	}
//	end modify xiao heping 2004/03/18
		
	delete objectIdVector;
	TRACE0(LEVEL5,"End of adding composition to pool\n");
}

//modified by xiao heping 2004/03/28

void
ComponentsPool::removeComponent(const PortableServer::ObjectId* oid)
{
//	modified by xiao heping 2004/03/12
	
	TRACE0(LEVEL5,"Remove composition from pool\n");

	//
	//get the ComponentComposition object from activeObjectMap_
	//	
	std::string key = PortableServer::ObjectId_to_string(*oid);
	ComponentComposition_var composition_;

//	STARCCM_SYNCHRONIZED(sync_,*this)

	ActiveObjectMap::iterator theIterator = activeObjectMap_.find(key);
	if (theIterator != activeObjectMap_.end())
	{
		composition_ = theIterator->second;
	}
	else
	{
		TRACE0(LEVEL1,"Fatal Error : the component doesn't exit and component pool currepted\n");
		throw new Components::RemoveFailure();
	}
	
		
	//
	//get all ports object of composition from activeObjectMap_
	//	
	ObjectIdVector* objectIdVector = composition_ -> getPortObjectIdVector();
	int loopLength = objectIdVector -> size();
	for( int i = 0;i < loopLength;i++ )
	{
		std::string portKey = (*objectIdVector)[i];
		activeObjectMap_.erase(portKey);
	}
	delete objectIdVector;
		
	//
	//earse the ComponentComposition from the activeObjectMap_.
	//
	activeObjectMap_.erase(key);

	//add by wkb for passivate 2003/04/02
	//lru_.remove(key);
	//composition_ -> _remove_ref();

	TRACE0(LEVEL5,"End of Remove composition from pool\n");
//	end modify xiao heping 2004/03/13
}

void
ComponentsPool::cleanup()
{
//	modified by xiao heping 2004/03/12
	STARCCM_SYNCHRONIZED(sync_,*this)

	ActiveObjectMap::iterator iter = activeObjectMap_.begin();
	
	//
	//remove all components in the componentpool!
	//
	while(iter != activeObjectMap_.end() )
	{
		ComponentComposition_var componentComposition = iter -> second;
		PortableServer::ObjectId_var oid =	componentComposition -> getObjectId();
		

		//added by xiao heping 2004/03/28
		if(componentComposition ->isPort(oid))
		{
			iter++;
			continue;
		}
		//end add xiao heping 2004/03/28

		
		//
		//remove the component through remove method of servant
		//
		PortableServer::Servant servant = componentComposition -> getComponentServant();
//		Components::CCMObject_var componentRef = componentComposition -> getComponentRef();
		try
		{
			//
			//this operation may be throw Components::RemoveFailure exception
			//
			//componentRef -> remove();
			this->removeComponent(oid);
			Components::ComponentServentBase* componentServant=
				dynamic_cast<Components::ComponentServentBase*>(servant);
			componentServant ->remove ();
			//servant -> remove();
		}
		catch(Components::RemoveFailure & ex)
		{
			throw ex;
		}
		
		//
		//must be careful!cannot use "iter++"
		//when invoke remove method of compoentRef,
		//we have erased some item from activeObjectMap_!
		//
		iter = activeObjectMap_.begin();
	}
//	end modify xiao heping 2004/03/18

	if(!activeObjectMap_.empty())
		activeObjectMap_.clear();
}

void
ComponentsPool::passivate()
{
    
}

void
ComponentsPool::activate()
{
    
}

void
ComponentsPool::partPassivate()
{
	//how many component to passivate
}

