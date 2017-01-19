#include <CORBA.h>
#include <PerformanceManager_impl.h>
#if defined(WIN32)
#include <Windows.h>
#endif
#include <iostream>
//#include <afx.h>

//
// Module declare ::ManagerDomain
using namespace std;
using namespace Components;
using namespace STARCCM::Deployment;
using namespace CORBA;

//
//
// Use to change the divisor from Kb to Mb.
#define DIV 1024
//Use to get cpu usage 
#define SystemBasicInformation       0
#define SystemPerformanceInformation 2
#define SystemTimeInformation        3
//
//used to computer cpu usage 
//
#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))
//
//use the struct get system memory infromation
//
typedef struct
	{
		DWORD   dwUnknown1;
		ULONG   uKeMaximumIncrement;
		ULONG   uPageSize;
		ULONG   uMmNumberOfPhysicalPages;
		ULONG   uMmLowestPhysicalPage;
		ULONG   uMmHighestPhysicalPage;
		ULONG   uAllocationGranularity;
		PVOID   pLowestUserAddress;
		PVOID   pMmHighestUserAddress;
		ULONG   uKeActiveProcessors;
		BYTE    bKeNumberProcessors;
		BYTE    bUnknown2;
		WORD    wUnknown3;
	} SYSTEM_BASIC_INFORMATION;
	//used to get cpu infromation
	typedef struct
	{
		LARGE_INTEGER   liIdleTime;
		DWORD           dwSpare[76];
	} SYSTEM_PERFORMANCE_INFORMATION;
    //used to get cpu information
	typedef struct
	{
		LARGE_INTEGER liKeBootTime;
		LARGE_INTEGER liKeSystemTime;
		LARGE_INTEGER liExpTimeZoneBias;
		ULONG         uCurrentTimeZoneId;
		DWORD         dwReserved;
	} SYSTEM_TIME_INFORMATION;
	//def function point
	typedef LONG (WINAPI *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);

	
    PROCNTQSI NtQuerySystemInformation;
    SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo;
    SYSTEM_TIME_INFORMATION        SysTimeInfo;
    SYSTEM_BASIC_INFORMATION       SysBaseInfo;
    double                         dbIdleTime;
    double                         dbSystemTime;
    LONG                           status;
    LARGE_INTEGER                  liOldIdleTime = {0,0};
    LARGE_INTEGER                  liOldSystemTime = {0,0};





 
// Interface declare ::ManagerDomain::PerformanceManager
//
ManagerDomain::PerformanceManager_impl::PerformanceManager_impl(PortableServer::POA_ptr poa)
    : poa_(PortableServer::POA::_duplicate(poa))
{
}

ManagerDomain::PerformanceManager_impl::PerformanceManager_impl(bool verbose)
    
{
	verbose = verbose;
}

ManagerDomain::PerformanceManager_impl::~PerformanceManager_impl()
{
}

PortableServer::POA_ptr
ManagerDomain::PerformanceManager_impl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}

//
// Operation declare GetPerformanceInfor
//
ManagerDomain::PerformanceInfor*
ManagerDomain::PerformanceManager_impl::GetPerformanceInfor()
    throw(CORBA::SystemException)
{
    //
	//def the return value variable
	//
#if defined(WIN32)
	PerformanceInfor_var serverPerformance = new PerformanceInfor;
	ProcessInfor_var processInfor = new ProcessInfor;
	CORBA::ULong MemorySize;
	serverPerformance->ProcessInformation.length(12);

	//get system memory information
	GlobalMemoryStatus (&state);
    
	//serverPerformance->IdleThreads = 10;

	//the system's available memory

	serverPerformance->FreeMemory = state.dwAvailPhys ;
	//
	//teh system's total memory
	//
	serverPerformance->TotalMemory = state.dwTotalPhys;   
	
	//
	//PROCESSENTRY32 Describes an entry from a list that enumerates the processes 
	//residing in the system address space when a snapshot was taken
	//
	ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);
	//
	//Takes a snapshot of the processes and the heaps, modules, and threads used by the processes.
	//
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot for Process Error: %d\n",GetLastError());
	}
	if(Process32First(hProcessSnap,&ProcessEntry32))
	{
		do
		{
			if(ProcessEntry32.th32ProcessID == 0)
			{
				continue;
			}
			else
			{			
				printf("%-20s",ProcessEntry32.szExeFile);					
			
			}
			//The OpenProcess function opens an existing process object
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,ProcessEntry32.th32ProcessID);
			if(!GetProcessMemoryInfo(hProcess,&PMCounter,sizeof(PMCounter)))
			{
				printf("GetProcessMemoryInfo Error in ManagerDomain::PerformanceManager_impl::GetPerformanceInfor(): %d\n",GetLastError());
				
			}
			else
			{
				//get HomeFinder process's memory information
				CORBA::String_var HomeFinderName = string_dup("HomeFinder.exe");
				
				if (!strcmp(ProcessEntry32.szExeFile,HomeFinderName))
				{
					
					MemorySize = PMCounter.WorkingSetSize/1024	;
					processInfor->ProcessMemory= MemorySize;
					processInfor->ProcessName = string_dup(HomeFinderName);			
				
					serverPerformance->ProcessInformation[0] = processInfor;		
                   	
					//cout<<ProcessEntry32.szExeFile<<PMCounter.WorkingSetSize/1024<<endl;
					printf("  %7d",PMCounter.WorkingSetSize/1024);
					
				} 
				//get Ots process's memory information
				CORBA::String_var OtsName = string_dup("OTS.exe");
				if (!strcmp(ProcessEntry32.szExeFile,OtsName))
				{
					MemorySize = PMCounter.WorkingSetSize/1024;
				
					processInfor->ProcessMemory= MemorySize;
					processInfor->ProcessName = string_dup(OtsName);
					serverPerformance->ProcessInformation[1] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
				//get Assembly process's memory information
				CORBA::String_var AssemblyName = string_dup("Assembly.exe");
				if (!strcmp(ProcessEntry32.szExeFile,AssemblyName))
				{
					
				
					MemorySize = PMCounter.WorkingSetSize/1024;
					processInfor->ProcessMemory= MemorySize;
					processInfor->ProcessName = string_dup(AssemblyName);
					serverPerformance->ProcessInformation[2] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
				//get AssemblyFactory process's memory information
				CORBA::String_var AssemblyFactoryName = string_dup("AssemblyFactory");
				if (!strcmp(ProcessEntry32.szExeFile,AssemblyFactoryName))
				{
					
					
					MemorySize = PMCounter.WorkingSetSize/1024;
					processInfor->ProcessMemory= MemorySize;
					processInfor->ProcessName = string_dup(AssemblyFactoryName);
					serverPerformance->ProcessInformation[3] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
				//get ComponentInstallation process's memory information
				CORBA::String_var ComponentInstallationName = string_dup("ComponentInstal");
				if (!strcmp(ProcessEntry32.szExeFile,ComponentInstallationName))
				{
					
					processInfor->ProcessMemory= PMCounter.WorkingSetSize/1024;
					processInfor->ProcessName = string_dup(ComponentInstallationName);
					serverPerformance->ProcessInformation[4] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
		    	//get DeployDomainMgr process's memory information
				CORBA::String_var DeployDomainMgrName = string_dup("DeployDomainMgr");
				if (!strcmp(ProcessEntry32.szExeFile,DeployDomainMgrName))
				{
					
					processInfor->ProcessMemory= PMCounter.WorkingSetSize/1024;
					processInfor->ProcessName = string_dup(DeployDomainMgrName);
					serverPerformance->ProcessInformation[5] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
				//
				//get FileAccessorFac process's memory information
				//
		    	CORBA::String_var FileAccessorFactoryName = string_dup("FileAccessorFac");
				if (!strcmp(ProcessEntry32.szExeFile,FileAccessorFactoryName))
				{
					
					processInfor->ProcessMemory= PMCounter.WorkingSetSize/1024;
					processInfor->ProcessName = string_dup(FileAccessorFactoryName);
					serverPerformance->ProcessInformation[6] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
				//
				//get nameserv process's memory information
				//
				CORBA::String_var nameservName = string_dup("nameserv.exe");
				if (!strcmp(ProcessEntry32.szExeFile,nameservName))
				{
					
					processInfor->ProcessMemory= PMCounter.WorkingSetSize/1024;
					processInfor->ProcessName = string_dup(nameservName);
					serverPerformance->ProcessInformation[7] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
				//get RES process's memory information
				CORBA::String_var ResName = string_dup("RES.exe");
				if (!strcmp(ProcessEntry32.szExeFile,ResName))
				{
					
					processInfor->ProcessMemory= PMCounter.WorkingSetSize/1024;
					processInfor->ProcessName = string_dup(ResName);
					serverPerformance->ProcessInformation[8] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
				//
				//get ServerActivator process's memory information
				//
				CORBA::String_var ServerActivatorName = string_dup("ServerActivator");
				if (!strcmp(ProcessEntry32.szExeFile,ServerActivatorName))
				{
					
					processInfor->ProcessMemory= PMCounter.WorkingSetSize/1024;
					processInfor->ProcessName = string_dup(ServerActivatorName);
					serverPerformance->ProcessInformation[9] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
				//get ComponentServer process's memory information
				CORBA::String_var ComponentServerName = string_dup("ComponentServer");
				if (!strcmp(ProcessEntry32.szExeFile,ComponentServerName))
				{
					
					processInfor->ProcessMemory= PMCounter.WorkingSetSize/1024;
					processInfor->ProcessName = string_dup(ComponentServerName);
					serverPerformance->ProcessInformation[10] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}
			    //get MainHostInstall process's memory information
				CORBA::String_var MainhostInstallName = string_dup("MainHostInstall");
				if (!strcmp(ProcessEntry32.szExeFile,MainhostInstallName))
				{
					
					processInfor->ProcessMemory= PMCounter.WorkingSetSize/1024;
					processInfor->ProcessName = string_dup(MainhostInstallName);
					serverPerformance->ProcessInformation[11] = processInfor;
					//printf("  %7d",PMCounter.WorkingSetSize/1024);
				}			
		
			}
		//if ProcessEntry32 is not empty then continue
		}while(Process32Next(hProcessSnap,&ProcessEntry32));
	}
	//
	//get NtQuerySystemInformation API adress from NTDLL.dll
	//
    NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(
                                          GetModuleHandle("ntdll"),
                                         "NtQuerySystemInformation"
                                         );

    if (!NtQuerySystemInformation)
       cout<<"get NtQuerySystemInformation function failure from ntdll.dll"<<endl;

    // get number of processors in the system
    status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL);
    if (status != NO_ERROR)
        cout<<"get Information function failure from NtQuerySystemInformation"<<endl;
    
	
    
    
        // get new system time
	    status = NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),0);
        if (status!=NO_ERROR)
           cout<<"get Information function failure from NtQuerySystemInformation"<<endl;

        // get new CPU's idle time
        status = NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL);
        if (status != NO_ERROR)
            cout<<"get Information function failure from NtQuerySystemInformation"<<endl;

        // if it's a first call - skip it
       if (liOldIdleTime.QuadPart != 0)
       {
            // CurrentValue = NewValue - OldValue
            dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime);
            dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime);

            // CurrentCpuIdle = IdleTime / SystemTime
            dbIdleTime = dbIdleTime / dbSystemTime;

            // CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors
            dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;
			serverPerformance->Throughput = (UINT)dbIdleTime;
           // printf("\b\b\b\b%3d%%",(UINT)dbIdleTime);
       }

        // store new CPU's idle and system time
        liOldIdleTime = SysPerfInfo.liIdleTime;
        liOldSystemTime = SysTimeInfo.liKeSystemTime;
		
        // wait one second
        Sleep(1);
		
    
    //printf("\n");	


    return serverPerformance._retn();

#if defined(Linux)
	return 0;
#endif

#endif	

}
