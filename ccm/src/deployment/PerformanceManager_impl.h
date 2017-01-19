#ifndef PerformanceManager_IMPL_H
#define PerformanceManager_IMPL_H

//#include <DeployDomainMgr_impl.h>
//#include <Deployment_impl.h>

#include <Manager_skel.h>
#include <Tlhelp32.h>
#include <Psapi.h>

//
// Module declare ::ManagerDomain
//
namespace ManagerDomain
{
	//
// Interface declare ::ManagerDomain::PerformanceManager
//
class PerformanceManager_impl : virtual public POA_ManagerDomain::PerformanceManager,
                                virtual public PortableServer::RefCountServantBase
{
    PerformanceManager_impl(const PerformanceManager_impl&);
    void operator=(const PerformanceManager_impl&);

    PortableServer::POA_var poa_;
private:
	bool verbose;
#if defined(WIN32)
	MEMORYSTATUS state;    
	PROCESS_MEMORY_COUNTERS PMCounter;		
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32  ProcessEntry32;
#endif

public:
	PerformanceManager_impl(PortableServer::POA_ptr);
	//bool getCPuUsage();
    PerformanceManager_impl(bool verbose);
	//void test();
	bool getCPuUsage(long *plResult);
    ~PerformanceManager_impl();

    virtual PortableServer::POA_ptr _default_POA();

    //
    // Operation declare GetPerformanceInfor
    //
    virtual ManagerDomain::PerformanceInfor* GetPerformanceInfor()
        throw(CORBA::SystemException);
		


};

//
}
#endif