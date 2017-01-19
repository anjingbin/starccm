#ifndef __CUSTOMTHREADPOOL_H__
#define __CUSTOMTHREADPOOL_H__

#include <CORBA.h>
#include <list>
#include <LinkedRequestQueue.h>
#include <iostream>

using namespace std;

namespace GreenThread
{


class CustomThreadPool;
OBJVAR(CustomThreadPool)
OBJPTR(CustomThreadPool)

class ThreadPoolDispatcher;

class CustomThreadPool:REF_COUNT_LOCAL_OBJECT STARCCM_COMMA_MONITOR
{
#if defined(STARCCM_MTL)

	CustomThreadPool(const CustomThreadPool&);
    void operator=(const CustomThreadPool&);

	CORBA::String_var poolName_;

	//
	// True if destroy was called
	//
	bool destroy_; 

	//
	// Thread group for the threads in the pool
	//
	STARCCM_THREAD_GROUP_HANDLE(group_)
	

	//
	//The maximum number of threads allowed in pool.
	//
	CORBA::ULong maximumPoolSize_;
	
	//
	//The minumum number of threads to maintain in pool. 
	//
	CORBA::ULong minimumPoolSize_;

	//
	//Current pool size.  
	//
	CORBA::ULong poolSize_;

	//
	//The maximum time for an idle thread to wait for new task. 
	//
	CORBA::Long keepAliveTime_;

	//
	//Shutdown flag - latches true when a shutdown method is called 
	//in order to disable queuing/handoffs of new tasks.
	//
//	boolean shutdown_ ;

	//
	//The LinkedRequestQueue used to hand off the command to a thread in the pool.
	//
	LinkedRequestQueue_var handOff_;

	//
	//The set of active threads, declared as a map from workers to
	//their threads.  This is needed by the interruptAll method.  It
	//may also be useful in subclasses that need to perform other
	//thread management chores.
	//
	std::list<STARCCM_THREAD_HANDLE_CLASS> threads_;

  /** The current handler for unserviceable requests. **/
  //protected BlockedExecutionHandler blockedExecutionHandler_;

public:
	//
	//The maximum pool size; used if not otherwise specified.  
	//Default value is 1024.
	//
	static const CORBA::ULong  DEFAULT_MAXIMUMPOOLSIZE;

	//
	//The minimum pool size; used if not otherwise specified.  
	//Default value is 1.
	//
	static const CORBA::ULong  DEFAULT_MINIMUMPOOLSIZE;

  
	//
	//The maximum time to keep worker threads alive waiting for new tasks; 
	//used if not otherwise specified. Default value is one minute (60000 milliseconds).
	//
	static const CORBA::Long DEFAULT_KEEPALIVETIME;

	static inline CustomThreadPool_ptr _duplicate(CustomThreadPool_ptr p)
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
    static inline CustomThreadPool_ptr _nil()
    { 
		return 0; 
	}

	//
	//Create a new pool that uses the supplied Channel for queuing, 
	//and with all default parameter settings except for maximum pool size.
	//
	CustomThreadPool(CORBA::ULong maxPoolSize=DEFAULT_MAXIMUMPOOLSIZE);

	~CustomThreadPool(){TRACE0(LEVEL6,"CustomThreadPool::~CustomThreadPool()\n");}

	char* getThreadPoolName();
	void setThreadPoolName(const char* name);

	// 
	//Return the maximum number of threads to simultaneously execute
	//New unqueued requests will be handled according to the current
	//blocking policy once this limit is exceeded.
	//
	CORBA::ULong getMaximumPoolSize(); 

	//
	//Set the maximum number of threads to use. 
	//Decreasing the pool size will not immediately kill existing threads, 
	//but they may later die when idle.
	//@exception IllegalArgumentException if less than the minimum.
	//
	void setMaximumPoolSize(CORBA::ULong newMaximum) ;

	//
	//Return the minimum number of threads to simultaneously execute.
	// (Default value is 1).  If fewer than the mininum number are
	//running upon reception of a new request, a new thread is started
	//handle this request.
	//
	CORBA::ULong getMinimumPoolSize(); 

	//
	//Set the minimum number of threads to use. 
	//@exception IllegalArgumentException if less than zero or 
	//greater than the maximum. 
	//
	void setMinimumPoolSize(CORBA::ULong newMinimum);

	//
	//Return the current number of active threads in the pool.  
	//This number is just a snaphot, and may change immediately upon returning
	//
	CORBA::ULong getPoolSize() ;

	//
	//Return the number of milliseconds to keep threads alive waiting
	//for new commands. A negative value means to wait forever. A zero
	//value means not to wait at all.
	//
	CORBA::Long getKeepAliveTime();
	
	//
	//Set the number of milliseconds to keep threads alive waiting for
	//new commands. A negative value means to wait forever. A zero
	//value means not to wait at all.
	//
	void setKeepAliveTime(CORBA::Long msecs); 

	//
	//Get a task from the handoff queue, or null if shutting down.
	//
	CCM_REQUEST_LEVEL_DISPATCH_PTR getTask(CORBA::Long waitTime);

	//
	//Cleanup method called upon termination of worker thread.
	//
	void workerDone(ThreadPoolDispatcher* w); 

	//
	//Create and start a thread to handle a new command.  
	//Call only when holding lock.
	//
	void addThread(CCM_REQUEST_LEVEL_DISPATCH_PTR command,CORBA::Long waitTime); 

	//
	//Create and start up to numberOfThreads threads in the pool.
	//Return the number created. This may be less than the number
	//requested if creating more would exceed maximum pool size bound.
	//
	CORBA::ULong createThreads(CORBA::ULong numberOfThreads); 

	//
	//Arrange for the given command to be executed by a thread in this pool.  
	//The method normally returns when the command has been
	//handed off for (possibly later) execution.
	//
	void execute(CCM_REQUEST_LEVEL_DISPATCH_PTR command) 
		throw(CCM_MTL_INTERRUPTED_EXCEPTION);

	CORBA::ULong getQueueSize();

	void destroy();
#endif
};

#if defined(STARCCM_MTL)

//Class defining the basic run loop for pooled threads.
class ThreadPoolDispatcher STARCCM_COLON_THREAD
{
	CustomThreadPool_var threadPool_;
	
	CCM_REQUEST_LEVEL_DISPATCH_VAR firstTask_;
	//
	//The maximum time for an idle thread to wait for new task. 
	//
	CORBA::Long keepAliveTime_;

public:
	ThreadPoolDispatcher(STARCCM_THREAD_GROUP_HANDLE_CLASS& group,
						CustomThreadPool_ptr pool,
						CCM_REQUEST_LEVEL_DISPATCH_PTR firstTask,
						CORBA::Long keepAliveTime)
	:STARCCM_THREAD_CLASS(group),
	threadPool_(CustomThreadPool::_duplicate(pool)),
	firstTask_(CCM_REQUEST_LEVEL_DISPATCH::_duplicate(firstTask)),
	keepAliveTime_(keepAliveTime)
	{
	}
	virtual void run()
	{
		try
		{
			if(!CORBA::is_nil(firstTask_))
			{
				firstTask_->invoke();

				firstTask_=CCM_REQUEST_LEVEL_DISPATCH::_nil();
			}

			while(true)
			{
				CCM_REQUEST_LEVEL_DISPATCH_VAR req=threadPool_->getTask(keepAliveTime_);

				//
				// CustomThreadPool has terminated
				//
				if(CORBA::is_nil(req))
					break;
				req -> invoke();

			}//for while
		}//for try
		catch(...)
		{
		}
		threadPool_->workerDone(this);

//		STARCCM_SYNCHRONIZED(groupSync,*group_)


		//
		// Release the reference
		//
		threadPool_ = CustomThreadPool::_nil(); 
	}//for run
};//for class ThreadPoolDispatcher

#endif //for STARCCM_MTL

class CustomDispatchThreadPool: REF_COUNT_LOCAL_OBJECT 
								STARCCM_COMMA_DISPATCH_STRATEGY
{
#if defined(STARCCM_MTL)

    CustomThreadPool_ptr threadPool_;
public:
	CustomDispatchThreadPool(CustomThreadPool_ptr pool)
		: threadPool_(pool)
    {
    }

    virtual ~CustomDispatchThreadPool() { }

    //
    // IDL:omg.org/PortableServer/DispatchRequestLevelStrategy/dispatch:1.0
    //
    virtual STARCCM_COMMA_DISPATCH_STRATEGY_ID
    id()
    {
		return 1001;
    }
    
    virtual CORBA::Any*
    info()
    {
		CORBA::Any_var any = new CORBA::Any();
		return any._retn();
    }

    virtual void
	dispatch(CCM_REQUEST_LEVEL_DISPATCH_PTR request)
    {
		threadPool_ -> execute(request);
    }
#endif //for STARCCM_MTL
};//for class CustomDispatchThreadPool

}//for namespace 


#endif

