#include <CustomThreadPool.h>
#include <iostream>


using namespace GreenThread;
using namespace std;

#if defined(STARCCM_MTL)

const CORBA::ULong  CustomThreadPool::DEFAULT_MAXIMUMPOOLSIZE=1024;

const CORBA::ULong  CustomThreadPool::DEFAULT_MINIMUMPOOLSIZE = 8;

const CORBA::Long CustomThreadPool::DEFAULT_KEEPALIVETIME = 60000;


//
//Create a new pool that uses the supplied Channel for queuing, 
//and with all default parameter settings except for maximum pool size.
//
CustomThreadPool::CustomThreadPool(CORBA::ULong maxPoolSize)
{
    maximumPoolSize_ = maxPoolSize;
    
	handOff_ = new LinkedRequestQueue();

	minimumPoolSize_ = DEFAULT_MINIMUMPOOLSIZE;
	poolSize_ = 0;
	keepAliveTime_ = DEFAULT_KEEPALIVETIME;
	destroy_ = false;
	poolName_ = CORBA::string_dup("threadpool_");
	group_ = new STARCCM_THREAD_GROUP_CLASS((const char*)(poolName_.in()));
}

char* CustomThreadPool::getThreadPoolName()
{
	return CORBA::string_dup(poolName_.in());
}
void CustomThreadPool::setThreadPoolName(const char* name)
{
	poolName_ = CORBA::string_dup(name);
}

// 
//Return the maximum number of threads to simultaneously execute
//New unqueued requests will be handled according to the current
//blocking policy once this limit is exceeded.
//
CORBA::ULong CustomThreadPool::getMaximumPoolSize() 
{ 
//	MTLSynchronized sync(*this);
    return maximumPoolSize_; 
}

//
//Set the maximum number of threads to use. 
//Decreasing the pool size will not immediately kill existing threads, 
//but they may later die when idle.
//
void CustomThreadPool::setMaximumPoolSize(CORBA::ULong newMaximum) 
{ 
	STARCCM_SYNCHRONIZED(sync,*this)
    if (newMaximum >=minimumPoolSize_) 
		maximumPoolSize_ = newMaximum; 
}

//
//Return the minimum number of threads to simultaneously execute.
// (Default value is 1).  If fewer than the mininum number are
//running upon reception of a new request, a new thread is started
//handle this request.
//
CORBA::ULong CustomThreadPool::getMinimumPoolSize() 
{ 
//	MTLSynchronized sync(*this);
    return minimumPoolSize_; 
}

//
//Set the minimum number of threads to use. 
//the minimum not changed if less than zero or 
//greater than the maximum. 
//
void CustomThreadPool::setMinimumPoolSize(CORBA::ULong newMinimum) 
{ 
	STARCCM_SYNCHRONIZED(sync,*this)
    if (newMinimum > 0 && newMinimum<=maximumPoolSize_) 
		minimumPoolSize_ = newMinimum; 
}
  
//
//Return the current number of active threads in the pool.  
//This number is just a snaphot, and may change immediately upon returning
//
CORBA::ULong CustomThreadPool::getPoolSize() 
{ 
//	MTLSynchronized sync(*this);
    return poolSize_; 
}

//
//Return the number of milliseconds to keep threads alive waiting
//for new commands. A negative value means to wait forever. A zero
//value means not to wait at all.
//
CORBA::Long CustomThreadPool::getKeepAliveTime() 
{ 
	STARCCM_SYNCHRONIZED(sync,*this)
    return keepAliveTime_; 
}

//
//Set the number of milliseconds to keep threads alive waiting for
//new commands. A negative value means to wait forever. A zero
//value means not to wait at all.
//
void CustomThreadPool::setKeepAliveTime(CORBA::Long msecs) 
{ 
	STARCCM_SYNCHRONIZED(sync,*this)
    keepAliveTime_ = msecs; 
}

//
//Cleanup method called upon termination of worker thread.
//
void CustomThreadPool::workerDone(ThreadPoolDispatcher* w) 
{
	STARCCM_SYNCHRONIZED(sync,*this)

    threads_.remove(STARCCM_THREAD_HANDLE_CLASS(w));

	/*
    if (--poolSize_ == 0 && destroy_) 
	{ 
		maximumPoolSize_ = minimumPoolSize_ = 0; // disable new threads
		notifyAll(); // notify awaitTerminationAfterShutdown
    }

    // Create a replacement if needed
    if (poolSize_ == 0 || poolSize_ < minimumPoolSize_) 
	{
		try 
		{
			STAR::RequestLevelDispatch_ptr req = handOff_->pollRequest(0);
			if (!CORBA::is_nil(req) && !destroy_) // just consume task if shut down
				addThread(req);
		} 
		catch(...) 
		{
		}
    }
	*/
}

//
//Get a task from the handoff queue, or null if shutting down.
//
CCM_REQUEST_LEVEL_DISPATCH_PTR CustomThreadPool::getTask(CORBA::Long waitTime)
{
    CORBA::Long lWaitTime;
	{
		STARCCM_SYNCHRONIZED(sync,*this)

		if(destroy_)
			return CCM_REQUEST_LEVEL_DISPATCH::_nil();

		//
		// Cause to die if too many threads
		//
		if (poolSize_ > maximumPoolSize_) 
			return CCM_REQUEST_LEVEL_DISPATCH::_nil();

		lWaitTime = (destroy_)? 0 : waitTime;
    }
    if (lWaitTime >= 0) 
		return handOff_->pollRequest(lWaitTime);
    else 
		return handOff_->takeRequest();
}
  


//
//Create and start a thread to handle a new command.  
//Call only when holding lock.
//
void CustomThreadPool::addThread(CCM_REQUEST_LEVEL_DISPATCH_PTR command,CORBA::Long waitTime) 
{
    STARCCM_THREAD_HANDLE(thread)
	thread = new ThreadPoolDispatcher(group_,this,command,waitTime);
    threads_.push_back(thread);
	++poolSize_;
    thread->start();
}

//
//Arrange for the given command to be executed by a thread in this pool.  
//The method normally returns when the command has been
//handed off for (possibly later) execution.
//
void CustomThreadPool::execute(CCM_REQUEST_LEVEL_DISPATCH_PTR command) 
	throw(CCM_MTL_INTERRUPTED_EXCEPTION)
{
	STARCCM_SYNCHRONIZED(sync,*this)
	if(destroy_)
		throw CORBA::OBJECT_NOT_EXIST();
	
	int size = poolSize_;

	//
	// Ensure minimum number of threads
	//
	if (size < minimumPoolSize_) 
	{
		addThread(command,-1);
		return;
	}

	//
	// Try to give to existing thread
	//
	if (handOff_->addRequest(command)) 
	{ 
		if (size < maximumPoolSize_) 
		{
			addThread(0,keepAliveTime_);
			return;
		}
	}
}

//
//Create and start up to numberOfThreads threads in the pool.
//Return the number created. This may be less than the number
//requested if creating more would exceed maximum pool size bound.
//
CORBA::ULong CustomThreadPool::createThreads(CORBA::ULong numberOfThreads) 
{
    CORBA::ULong ncreated = 0;
    for (CORBA::ULong i = 0; i < numberOfThreads; ++i) 
	{
		STARCCM_SYNCHRONIZED(sync,*this)
	    if (poolSize_ < maximumPoolSize_)
		{
			addThread(CCM_REQUEST_LEVEL_DISPATCH::_nil(),-1);
			++ncreated;
        }
        else 
			break;
    }
    return ncreated;
}

CORBA::ULong CustomThreadPool::getQueueSize()
{
	return handOff_->size();
}

void CustomThreadPool::destroy()
{
    {
        STARCCM_SYNCHRONIZED(sync,*this)
		if(destroy_)
			return;
        destroy_ = true;
		notifyAll();
    }

	handOff_->destroy();

    //
    // Wait for all the threads in the pool to end
    //
	STARCCM_SYNCHRONIZED(groupSync,*group_)
    while(group_ -> activeCount() > 0)
    {
		try
		{
			group_ -> wait();
		}
		catch(const CCM_MTL_INTERRUPTED_EXCEPTION&)
		{
		}
	}

    //
    // Destroy the group
    //
    try
    {
		group_ -> destroy();
    }
    catch(const STARCCM_ILLEGAL_THREAD_STATE_EXCEPTION&)
    {
		assert(false);
    }

    //
    // Clean out all pending requests
    //
/*    while(head_)
    {
        RequestRecord* next = head_ -> next;
        delete head_;
        head_ = next;
    }
*/
}

#endif //for STARCCM_MTL
