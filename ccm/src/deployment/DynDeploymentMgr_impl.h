#ifndef ___DynDeploymentMgr_impl_h__
#define ___DynDeploymentMgr_impl_h__

#include <DynDeploymentMgr_skel.h>
#include <DeployDomainMgr.h>
#include <FileAccessor.h>
#include <XMLParser.h>
#include <CCMObject.h>
#include <Deployment.h>
#include <Architecture.h>

#include <map>
#include <string>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>

#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>

#ifdef WIN32
#	include <process.h>
#	include <direct.h>
#else
#	include <unistd.h>
#endif

using namespace std;
using namespace Components;
using namespace STARCCM::Deployment;

//
// IDL:omg.org/STARCCM:1.0
//
namespace STARCCM
{

//
// IDL:omg.org/STARCCM/DynDeployment:1.0
//
namespace DynDeployment
{
	
	class DynCreateFailure
	{
	public:
	    DynCreateFailure(){};		    
	};	

	class DynRemoveFailure
	{
	public:
	    DynRemoveFailure(){};		    
	};	

	class DynReplacementFailure
	{
	public:
	    DynReplacementFailure(){};		    
	};	

	class DynMigrateFailure
	{
	public:
	    DynMigrateFailure(){};		    
	};
	
	class DynLinkFailure
	{
	public:
	    DynLinkFailure(){};		    
	};	

	class DynUnLinkFailure
	{
	public:
	    DynUnLinkFailure(){};		    
	};	

	class ChangePropertyFailure
	{
	public:
	    ChangePropertyFailure(){};		    
	};	
	
	class FileDownloadFailure
	{
	public:
		FileDownloadFailure() {};
	};

	class ConfigFailure
	{
	public:
		ConfigFailure() {};
	};

//
// IDL:omg.org/STARCCM/DynDeployment/DynDeploymentMgr:1.0
//
class DynDeploymentMgr_impl : virtual public POA_STARCCM::DynDeployment::DynDeploymentMgr,
                              virtual public PortableServer::RefCountServantBase
{
public:

    DynDeploymentMgr_impl(CORBA::ORB_ptr orb_, PortableServer::POA_ptr poa_, DeployDomainMgr_ptr manager, const char * dir, bool verbose_);
    ~DynDeploymentMgr_impl();

    //
    // IDL:omg.org/STARCCM/DynDeployment/DynDeploymentMgr/start:1.0
    //
    virtual void start()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/STARCCM/DynDeployment/DynDeploymentMgr/stop:1.0
    //
    virtual void stop()
        throw(CORBA::SystemException);

    //
    // IDL:omg.org/STARCCM/DynDeployment/DynDeploymentMgr/dynDeploy:1.0
    //
    virtual void dynDeploy(const char* drdfile)
        throw(STARCCM::DynDeployment::InvalidDrdFile,
              STARCCM::DynDeployment::InvalidAddFile,
              STARCCM::DynDeployment::DynDeployFailure,
              CORBA::SystemException);
              
private:
	enum dynDeploymentMgrState
	{
		INACTIVE,
		INSERVICE
	};

	bool verbose;	
		
	CORBA::ORB_var orb;
			
	PortableServer::POA_var poa;
			
	DeployDomainMgr_var DomainMgr;

	CORBA::String_var rootDir;

	CORBA::String_var initialDir;

	CORBA::String_var installDir;
	
	dynDeploymentMgrState state;

	Components::Deployment::Assembly_var assembly;

	STARCCM::Architecture* arch;
	
	CORBA::String_var archFile;

	typedef map<string, CORBA::StringSeq_var> MapOidSeq;		

	typedef map<string, ::Components::ObjectSeq_var> MapObjSeq;

	typedef map<string, ::Components::Deployment::PassivateClientTypeSeq_var> MapPassivateType;	
	
	void create(const char* dcd_loc)
	 throw(DynCreateFailure,
	 	STARCCM::DynDeployment::InvalidAddFile,
		CORBA::SystemException);

	void remove(XMLParser *parser, DOM_Node rmnode)
		 throw(DynRemoveFailure,
		 	STARCCM::DynDeployment::InvalidDrdFile,
		 	CORBA::SystemException);

	void add(XMLParser *parser, DOM_Node addnode)
	 throw(DynCreateFailure,
	 	STARCCM::DynDeployment::InvalidDrdFile,
	 	STARCCM::DynDeployment::InvalidAddFile,
		CORBA::SystemException);

	void replacement(XMLParser *parser, DOM_Node rpnode)
	 throw(DynReplacementFailure,
	 	STARCCM::DynDeployment::InvalidDrdFile,
		CORBA::SystemException);

	void migrate(XMLParser *parser, DOM_Node mgnode)
	 throw(DynMigrateFailure,
	 	STARCCM::DynDeployment::InvalidDrdFile,
		CORBA::SystemException);

	void link(XMLParser *parser,DOM_Node linknode)
		 throw(DynLinkFailure,
		 	STARCCM::DynDeployment::InvalidDrdFile,
		 	CORBA::SystemException);

	void unlink(XMLParser *parser,DOM_Node unlinknode)
		 throw(DynUnLinkFailure,
		 	STARCCM::DynDeployment::InvalidDrdFile,
		 	CORBA::SystemException);

	void changeproperty(XMLParser *parser,DOM_Node cpnode)
		 throw(ChangePropertyFailure,
		 	STARCCM::DynDeployment::InvalidDrdFile,
		 	CORBA::SystemException);  

	void download(const char* fileloc)
		 throw(STARCCM::DynDeployment::FileDownloadFailure,
		FileSystemError,
		CORBA::SystemException);

	int copyFile(const char *source, const char *target);
			
//deleted by xiao heping 2004/05/27
//	    int copyFile(const char *source, const char *target);
//end delete

	void passivatecomponents(XMLParser *parser, DOM_Node passivateNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);

	void runcomplete(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);

	void unregister(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);

	void deleteCom(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);

	void setComproperties(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::ConfigFailure,
			STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);

	void activatecomponents(XMLParser *parser, DOM_Node activateNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);

	void statetransfer(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);

	void configurationcomplete(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);

	void redirectrequest(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException);
};

} // End of namespace DynDeployment

} // End of namespace STARCCM

#endif
