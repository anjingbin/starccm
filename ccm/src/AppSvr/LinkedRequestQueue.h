#ifndef __LinkedRequestQueue_h__
#define __LinkedRequestQueue_h__

#include <CORBA.h>

#if defined(ORBacus) 
	#define CCM_MTL_INTERRUPTED_EXCEPTION			JTCInterruptedException
	#define STARCCM_ILLEGAL_THREAD_STATE_EXCEPTION	JTCIllegalThreadStateException
	#define CCM_REQUEST_LEVEL_DISPATCH_VAR			OB::DispatchRequest_var
	#define CCM_REQUEST_LEVEL_DISPATCH_PTR			OB::DispatchRequest_ptr
	#define CCM_REQUEST_LEVEL_DISPATCH				OB::DispatchRequest
	#define STARCCM_COMMA_DISPATCH_STRATEGY_ID		OB::DispatchStrategyId
	#define STARCCM_COMMA_DISPATCH_STRATEGY			, virtual public OB::DispatchStrategy
	#define STARCCM_COLON_THREAD					: public JTCThread
	#define STARCCM_THREAD_HANDLE(handle)			JTCThreadHandle handle;
	#define STARCCM_THREAD_HANDLE_CLASS				JTCThreadHandle
	#define STARCCM_THREAD_GROUP_HANDLE(handle)		JTCThreadGroupHandle handle;
	#define STARCCM_THREAD_GROUP_HANDLE_CLASS		JTCThreadGroupHandle	
	#define STARCCM_THREAD_GROUP_CLASS				JTCThreadGroup
	#define STARCCM_THREAD_CLASS					JTCThread
#endif
#if defined(StarBus)
	#define CCM_MTL_INTERRUPTED_EXCEPTION			MTLInterruptedException
	#define STARCCM_ILLEGAL_THREAD_STATE_EXCEPTION	MTLIllegalThreadStateException
	#define CCM_REQUEST_LEVEL_DISPATCH_VAR			STAR::RequestLevelDispatch_var
	#define CCM_REQUEST_LEVEL_DISPATCH_PTR			STAR::RequestLevelDispatch_ptr
	#define CCM_REQUEST_LEVEL_DISPATCH				STAR::RequestLevelDispatch
	#define STARCCM_COMMA_DISPATCH_STRATEGY_ID		STAR::DispatchRequestLevelStrategyId
	#define STARCCM_COMMA_DISPATCH_STRATEGY			, virtual public STAR::DispatchRequestLevelStrategy
	#define STARCCM_COLON_THREAD					: public MTLThread
	#define STARCCM_THREAD_HANDLE(handle)			MTLThreadHandle handle;
	#define STARCCM_THREAD_HANDLE_CLASS				MTLThreadHandle
	#define STARCCM_THREAD_GROUP_HANDLE(handle)		MTLThreadGroupHandle handle;
	#define STARCCM_THREAD_GROUP_HANDLE_CLASS		MTLThreadGroupHandle	
	#define STARCCM_THREAD_GROUP_CLASS				MTLThreadGroup
	#define STARCCM_THREAD_CLASS					MTLThread
#endif
namespace GreenThread
{
#if defined(STARCCM_MTL)

class LinkedRequestQueue;
OBJVAR(LinkedRequestQueue)
OBJPTR(LinkedRequestQueue)

class  LinkedRequestQueue:REF_COUNT_LOCAL_OBJECT
{

	//
	//Main mechanics for takeRequest/pollRequest
	//
	CCM_REQUEST_LEVEL_DISPATCH_PTR extract(); 

public:
	//
	//Initial default capacity is 1024
	//
	const static CORBA::ULong INITIAL_DEFAULT_CAPACITY;

	static inline LinkedRequestQueue_ptr _duplicate(LinkedRequestQueue_ptr p)
    { 
		#ifdef StarBus
			if(p) 
		 		p -> _STAR_incRef(); 
		#endif

		#ifdef ORBacus
			if(p) 
		 		p -> _OB_incRef(); 
		#endif

		return p; 
	}

    static inline LinkedRequestQueue_ptr _nil()
    { 
		return 0; 
	}

	//
	//Create a queue with the given capacity
	//if capacity less or equal to zero,
	//create a queue with the current default capacity
	//
	LinkedRequestQueue(CORBA::ULong capacity=INITIAL_DEFAULT_CAPACITY);

	~LinkedRequestQueue(){TRACE0(LEVEL6,"invoked ~LinkedRequestQueue()\n");};
	
	//
	//Return the number of elements in the queue.
	//This is only a snapshot value, that may be in the midst of changing. 
	//The returned value will be unreliable in the presence of active puts and takes, 
	//and should only be used as a heuristic estimate, 
	//for example for resource monitoring purposes.
	//
	CORBA::ULong size();

	//
	//Reset the capacity of this queue.
	//If the new capacity is less than the old capacity,
	//existing elements are NOT removed, but
	//incoming puts will not proceed until the number of elements
	//is less than the new capacity.
	//The capacity of this queue not changed if capacity less or equal to zero
	//
	void setCapacity(CORBA::ULong newCapacity) ;

	//
	//Place item in LinkedRequestQueue only if it can be accepted within msecs milliseconds. 
	//The time bound is interpreted in a coarse-grained, best-effort fashion. 
	//@param request:	the element to be inserted. Should be non-null.
	//@param msecs:		the number of milliseconds to wait. If less than or equal to zero, 
	//					the method does not perform any timed waits, but might still require
	//					access to a synchronization lock, which can impose unbounded
	//					delay if there is a lot of contention for the channel.
	//@return value:	return true if accepted, else false.
	//
   	bool addRequest(CCM_REQUEST_LEVEL_DISPATCH_PTR request);

	//
	//Return and remove an request from LinkedRequestQueue, 
	//possibly waiting indefinitely until such an item exists.
	//@return value:	return some item from the LinkedRequestQueue.
	//
	CCM_REQUEST_LEVEL_DISPATCH_PTR takeRequest() throw(CCM_MTL_INTERRUPTED_EXCEPTION);

	//
	//Return and remove an item from LinkedRequestQueue only if one is available within msecs milliseconds. 
	//The time bound is interpreted in a coarse grained, best-effort fashion.
	//@param msecs:		the number of milliseconds to wait. If less than or equal to zero,
	//					the operation does not perform any timed waits,
	//					but might still require access to a synchronization lock, 
	//					which can impose unbounded delay if there is a lot of contention for the LinkedRequestQueue.
	//@return value:	return some item, or null if the LinkedRequestQueue is empty.
	//
	CCM_REQUEST_LEVEL_DISPATCH_PTR pollRequest(CORBA::Long msecs) throw(CCM_MTL_INTERRUPTED_EXCEPTION);


	//
	//Return the maximum number of elements that can be held.
	//@return the capacity of this LinkedRequestQueue.
	//
	CORBA::ULong capacity();

	bool isEmpty();
	void destroy();

protected:
	struct RequestRecord
    {
		CCM_REQUEST_LEVEL_DISPATCH_VAR request;
		RequestRecord* next;
    };

	//
	//Dummy header node of list. The first actual node, if it exists, is always 
	//at head_.next. After each take, the old first node becomes the head.
	//
	RequestRecord* head_;

	// 
	// The last node of list. Put() appends to list, so modifies last_
	//
	RequestRecord* last_;

	//
	//The number of threads waiting for a take.
	//Notifications are provided in put only if greater than zero.
	//The bookkeeping is worth it here since in reasonably balanced
	//usages, the notifications will hardly ever be necessary, so
	//the call overhead to notify can be eliminated.
	//
	CORBA::ULong waitingForTake_;  

	STARCCM_RECURSIVE_MUTEX(headMutex_)
	STARCCM_RECURSIVE_MUTEX(lastMutex_)

	//
	//Helper monitor. Ensures that only one put at a time executes.
	//
	//CCM_MTL_MONITOR			putLock_;
	STARCCM_MONITOR(putLock_)

	//
	//Number of elements allowed
	CORBA::ULong capacity_;

	CORBA::ULong queueSize_;

	int destroy_ ;
};

#endif //for STARCCM_MTL
}

#endif

