#ifndef TransactionManager_IMPL_H
#define TransactionManager_IMPL_H

#include <Manager_skel.h>
#include <map>
#include <string>
#include <Manager.h>
#include <ContainerRunTime_fwd.h>
#include <ContainerRunTime.h>
#include <list>

#include <Deployment_skel.h>
#include <DeployDomainMgr.h>
#include <ContainerBase.h>

//
// Module declare ::ManagerDomain
//


using namespace std;
using namespace Components;
//using namespace Components::Deployment;
using namespace STARCCM::Deployment;
// Module declare ::ManagerDomain
//
namespace ManagerDomain
{

//
// Interface declare ::ManagerDomain::TransactionManager
//
class TransactionManager_impl : virtual public POA_ManagerDomain::TransactionManager,
                                virtual public PortableServer::RefCountServantBase
{
    //TransactionManager_impl(const TransactionManager_impl&);
    //void operator=(const TransactionManager_impl&);

    //PortableServer::POA_var poa_;

public:

    //TransactionManager_impl(PortableServer::POA_ptr);
     TransactionManager_impl();
    ~TransactionManager_impl();

    //virtual PortableServer::POA_ptr _default_POA();

    //
    // Operation declare GetTransactionInfor
    //
    virtual ManagerDomain::TransactionInfor GetTransactionInfor()
        throw(ManagerDomain::GetTransactionError,
              CORBA::SystemException);
};

} // End of namespace ManagerDomain

#endif
