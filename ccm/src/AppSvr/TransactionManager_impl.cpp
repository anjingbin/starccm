#include <CORBA.h>
#include <TransactionManager_impl.h>

#include <Deployment_skel.h>
#include <DeployDomainMgr.h>
//
#include <ContainerBase_fwd.h>
#include <ContainerBase.h>

#ifdef WIN32
#include <Windows.h>
#endif

#include <list>
#include <ContainerRunTime_fwd.h>
#include <ContainerRunTime.h>

#include <CustomThreadPool.h>
#include <ResourcePool.h>
#include <TxnInterceptor.h>

// Module declare ::ManagerDomain
//
using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;
using namespace ManagerDomain;

//
// Operation declare GetTransactionInfor
//
ManagerDomain::TransactionManager_impl::TransactionManager_impl()
    
{
}

ManagerDomain::TransactionManager_impl::~TransactionManager_impl()
{
}
ManagerDomain::TransactionInfor
ManagerDomain::TransactionManager_impl::GetTransactionInfor()
    throw(ManagerDomain::GetTransactionError,
          CORBA::SystemException)
{
    // TODO: Implementation
    ManagerDomain::TransactionInfor _r ;//= new ManagerDomain::TransactionInfor;
    _r.TotalCommitted = TxnInterceptor::getCommitCount();
    _r.TotalRolledBack = TxnInterceptor::getTotalRolledBack();
    _r.TransactionPolicy = REQUIRED;
    _r.TotalTransactions = _r.TotalCommitted+_r.TotalRolledBack;
    return _r;
}

