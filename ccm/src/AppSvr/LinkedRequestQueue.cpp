#if defined(STARCCM_MTL)

#include <LinkedRequestQueue.h>

#ifdef WIN32
#include <Time.h>
#else
	#include <sys/time.h>
#endif

#include <iostream>
using namespace std;

using namespace GreenThread;

const CORBA::ULong LinkedRequestQueue::INITIAL_DEFAULT_CAPACITY=1024;

//
//Create a queue with the given capacity
//if capacity less or equal to zero,
//create a queue with the current default capacity
//
LinkedRequestQueue::LinkedRequestQueue(CORBA::ULong capacity)
{
    if (capacity <= 0) 
		capacity_=INITIAL_DEFAULT_CAPACITY;
	else
		capacity_ = capacity;

	waitingForTake_ = 0;
	queueSize_=0;
    head_ =  new RequestRecord; 
	head_->request=CCM_REQUEST_LEVEL_DISPATCH::_nil();
	head_->next=0;
    last_ = head_;
	destroy_ = false;
}

//
//Return the current capacity of this queue
//
CORBA::ULong LinkedRequestQueue::capacity() 
{ 
	return capacity_; 
}

//
//Return the number of elements in the queue.
//This is only a snapshot value, that may be in the midst of changing. 
//The returned value will be unreliable in the presence of active puts and takes, 
//and should only be used as a heuristic estimate, 
//for example for resource monitoring purposes.
//
CORBA::ULong LinkedRequestQueue::size() 
{
	//
    //This should ideally synch on putGuard_, but
    //doing so would cause it to block waiting for an in-progress
    //put, which might be stuck. So we instead use whatever
    //value of putSidePutPermits_ that we happen to read.
    //
    return queueSize_;
}

//
//Reset the capacity of this queue.
//If the new capacity is less than the old capacity,
//existing elements are NOT removed, but
//incoming puts will not proceed until the number of elements
//is less than the new capacity.
//The capacity of this queue not changed if capacity less or equal to zero
//
void LinkedRequestQueue::setCapacity(CORBA::ULong newCapacity) 
{
   if (newCapacity <= 0) 
		return;
   else
	   capacity_ = newCapacity;
}

//
//Main mechanics for take/poll
//
CCM_REQUEST_LEVEL_DISPATCH_PTR LinkedRequestQueue::extract() 
{
	CCM_REQUEST_LEVEL_DISPATCH_PTR request=CCM_REQUEST_LEVEL_DISPATCH::_nil();

	STARCCM_SYNCHRONIZED(sync_,headMutex_)
	RequestRecord* first = head_->next;
	if (first) 
	{
		request = first->request._retn();
		delete head_;
		head_ = first; 
		--queueSize_;
	}
	return request;
}

CCM_REQUEST_LEVEL_DISPATCH_PTR LinkedRequestQueue::takeRequest()
	throw(CCM_MTL_INTERRUPTED_EXCEPTION)
{
    CCM_REQUEST_LEVEL_DISPATCH_PTR request = extract();

    if (!CORBA::is_nil(request)) 
	{
		return request;
	}
    else 
	{
		STARCCM_SYNCHRONIZED(sync,putLock_)
		try 
		{
			++waitingForTake_;
			while(true)
			{
				request = extract();
				if (!CORBA::is_nil(request)) 
				{
					--waitingForTake_;
					return request;
				}
				else 
				{
					if(destroy_)
						return	CCM_REQUEST_LEVEL_DISPATCH::_nil();
					putLock_.wait(); 
				}
			}//for while(true)
        }        
		catch(CCM_MTL_INTERRUPTED_EXCEPTION& ex) 
		{
			--waitingForTake_;
			putLock_.notify();
			throw ex; 
        }
	}
}

CCM_REQUEST_LEVEL_DISPATCH_PTR LinkedRequestQueue::pollRequest(CORBA::Long msecs) 
	throw(CCM_MTL_INTERRUPTED_EXCEPTION)
{
    CCM_REQUEST_LEVEL_DISPATCH_PTR request = extract();
    if (!CORBA::is_nil(request)) 
		return request;
    else 
	{
		STARCCM_SYNCHRONIZED(sync,putLock_)
		try 
		{
			time_t ltime;
			CORBA::Long waitTime = msecs;
			CORBA::Long start = (msecs <= 0)? 0: time( &ltime );
			++waitingForTake_;
			while(true) 
			{
				request = extract();
				if (request != 0 || waitTime <= 0) 
				{
					--waitingForTake_;
					return request;
				}
				else 
				{
					if(destroy_)
					{
						--waitingForTake_;
						putLock_.notify();
						return request;
					}
					putLock_.wait(waitTime); 
					waitTime = msecs - (time( &ltime ) - start);
				}
			}
        }
        catch(CCM_MTL_INTERRUPTED_EXCEPTION* ex) 
		{
			--waitingForTake_; 
			putLock_.notify();
			throw ex; 
        }
	}
}

bool LinkedRequestQueue::addRequest(CCM_REQUEST_LEVEL_DISPATCH_PTR request) 
{
	//?????????????
//	STARCCM_SYNCHRONIZED(syncMonitor,putLock_)

    RequestRecord* p = new RequestRecord;
	p->request=CCM_REQUEST_LEVEL_DISPATCH::_duplicate(request);
	p->next=0;
	{
		STARCCM_SYNCHRONIZED(sync_,lastMutex_)
		last_->next = p;
		last_ = p;
	}
	++queueSize_;
	if (waitingForTake_ > 0)
	{
		//????????????
		STARCCM_SYNCHRONIZED(syncMonitor,putLock_)
        putLock_.notify();
	}

	if(queueSize_>capacity_)
		return true;
	else
		return false;
}

bool LinkedRequestQueue::isEmpty() 
{
	STARCCM_SYNCHRONIZED(sync,headMutex_)
	return head_->next == 0;
}  

void LinkedRequestQueue::destroy()
{
	STARCCM_SYNCHRONIZED(sync,putLock_)
	destroy_ = true;
	if (waitingForTake_ > 0)
        putLock_.notifyAll();
}

#endif //for  STARCCM_MTL

