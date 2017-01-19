// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Shufeng    icestar_wang@163.com
//
// **********************************************************************

// Version: 1.0.0

#include <CORBA.h>
#include <CCM.h>
#include <DynDeploymentMgr_impl.h>
#include <Architecture.h>
#include <HomeRegistration.h>
#include <CosNaming.h>
#include <malloc.h>

//added by xiao heping 
#include <CommonFunc.h>
//end add

#ifndef WIN32
#	include<unistd.h>
#endif

using namespace std;
using namespace Components;
using namespace Components::Deployment;
using namespace STARCCM::Deployment;
using namespace STARCCM::DynDeployment;
using namespace STARCCM;

//
// IDL:omg.org/STARCCM:1.0
//

//
// IDL:omg.org/STARCCM/DynDeployment:1.0
//

//
// IDL:omg.org/STARCCM/DynDeployment/DynDeploymentMgr:1.0
//
DynDeploymentMgr_impl::DynDeploymentMgr_impl(CORBA::ORB_ptr orb_, PortableServer::POA_ptr poa_, DeployDomainMgr_ptr manager, const char * dir, bool verbose_)
	:orb(CORBA::ORB::_duplicate(orb_)), poa(PortableServer::POA::_duplicate(poa_)),DomainMgr(DeployDomainMgr::_duplicate(manager)),verbose(verbose_)
{	
	rootDir=CORBA::string_dup(dir);
	arch = NULL;
	
	state=INACTIVE;
	
	if (verbose) {
		cout << "DynDeploymentMgr : DynDeploymentMgr"
		     <<" is running." 
		     << endl;
	};
}

DynDeploymentMgr_impl::~DynDeploymentMgr_impl()
{
	if (arch != NULL)
	{
		delete arch;
		arch = NULL;
	}
	
	if (verbose) {
		cout << "DynDeploymentMgr : DynDeploymentMg"
		     <<" is terminating." 
		     << endl;
	}
}

//
// IDL:omg.org/STARCCM/DynDeployment/DynDeploymentMgr/start:1.0
//
void
DynDeploymentMgr_impl::start()
    throw(CORBA::SystemException)
{
	if (state == INSERVICE)
	{
		cout << "DynDeploymentMgr: DynDeploymentMgr has been started." << endl;	
		return;
	}
	
    state=INSERVICE;
    if (verbose) {
		cout << "DynDeploymentMgr : DynDeploymentMgr"
		     <<" is inservice." 
		     << endl;
	};
}

//
// IDL:omg.org/STARCCM/DynDeployment/DynDeploymentMgr/stop:1.0
//
void
DynDeploymentMgr_impl::stop()
    throw(CORBA::SystemException)
{
	if (state == INACTIVE)
	{
		cout << "DynDeploymentMgr: DynDeploymentMgr has not been started yet." << endl;	
		return;
	}
	
    state=INACTIVE;
    if (verbose) {
		cout << "DynDeploymentMgr : DynDeploymentMgr"
		     <<" is inactive." 
		     << endl;
	};
}

//
// IDL:omg.org/STARCCM/DynDeployment/DynDeploymentMgr/dynDeploy:1.0
//

//*******************************************************************************************
//1.download the dynamic reconfiguration descriptor file(.drd) to the rootdir\temp
//
void
DynDeploymentMgr_impl::dynDeploy(const char* drdfile)
    throw(STARCCM::DynDeployment::InvalidDrdFile,
          STARCCM::DynDeployment::InvalidAddFile,
          STARCCM::DynDeployment::DynDeployFailure,
          CORBA::SystemException)
{

	if(verbose)
	{
		cout<<"dynDeploy: dynDeployment begins." << endl;
		cout<<"dynDeploy: download the drd file "<<drdfile<<endl;
	}

	if (state==INACTIVE)
	{
		cout << "dynDeploy: fail in not starting DynDeploymentMgr." << endl;
		return;
	}
	
	//
	//download the drd file from the console to the host 
	//and rename it as oldfilename(.drd)
	//rootdir\temp\filename(.drd)
	//	
    	try
    	{
    		download(drdfile);
    	}
    	catch(const CORBA::SystemException &)
    	{
    		throw;
    	}
    	catch(...)
    	{
    		TRACE1(LEVEL1,"dynDeploy: create drd file failed in download the file from %s.\n ",drdfile);
    		throw STARCCM::DynDeployment::InvalidDrdFile();
    	}

	//To find out the drd file name
#ifdef SOLARIS
	const char* tmp = strrchr(drdfile,PATH_DELILIMITOR_CHAR)+1;
#else
	char* tmp = strrchr(drdfile,PATH_DELILIMITOR_CHAR)+1;
#endif
	CORBA::String_var drdFileName = rootDir;
	drdFileName += CORBA::string_dup(PATH_DELILIMITOR);
	drdFileName += CORBA::string_dup("temp");
	drdFileName += CORBA::string_dup(PATH_DELILIMITOR);
	drdFileName	+= tmp;

	//To get the initail Dir
	int initialDirLen = strlen(drdfile)-strlen(tmp)-1;
	char* str = (char*)malloc((initialDirLen+1)*sizeof(char));
	strncpy(str, drdfile, initialDirLen);
	str[initialDirLen] = '\0';
	initialDir = CORBA::string_dup(str);
	
	//
	//XMLParser initialilze
	//
  	XMLParser *parser=new XMLParser();
  	
  	try
  	{
  		parser ->initialize(drdFileName.in());
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE0(LEVEL1,"dynDeploy: XMLParser initialized failed. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		throw STARCCM::DynDeployment::DynDeployFailure();
	}    

    	DOM_Node root = parser ->getRootNode("DynReconfig");

	if(root==NULL)
	{
		TRACE0(LEVEL1,"dynDeploy: Invalid dynmic reconfiguration descriptor file: have no DynReconfig element. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw STARCCM::DynDeployment::DynDeployFailure();
	}

	//To get Assembly UUID, then get Assembly corbaloc
	DOM_Node assemblynode = parser ->findChild(root,"componentassembly");
	if(assemblynode==NULL)
	{
		TRACE0(LEVEL1,"dynDeploy: Invalid dynmic reconfiguration descriptor file: have no componentassembly element. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		throw STARCCM::DynDeployment::DynDeployFailure();
	}

	DOMString assemblyUUID = parser ->getAttr(assemblynode, "idref");
	if(assemblyUUID==NULL)
	{
		TRACE0(LEVEL2,"dynDeploy: invalid cad file \n");
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		throw STARCCM::DynDeployment::DynDeployFailure();
	}

	CORBA::String_var assemblyloc;
	try
	{
		assemblyloc= DomainMgr ->getObject(ASSEMBLY,assemblyUUID.transcode());
	}
	catch(STARCCM::Deployment::ObjectNotExist)
	{
		cout << "dynDeploy: the assembly is not exist.\n";
		//newAssembly();

		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		throw STARCCM::DynDeployment::DynDeployFailure();
	}

	//To get Assembly object referrence
	CORBA::Object_var obj;
	try
	{
	       obj = orb->string_to_object(assemblyloc);
        }
        catch(const CORBA::BAD_PARAM&)
        {
        	cout << "dynDeploy: invalid Assembly corbaloc.\n";  
        	if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
        	throw STARCCM::DynDeployment::DynDeployFailure();
        }
	
    	if(CORBA::is_nil(obj))
    	{
        	cout << "dynDeploy: cannot get the reference of Assembly.\n";    
        	if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		throw STARCCM::DynDeployment::DynDeployFailure();
   	}

    	assembly = Assembly::_narrow(obj);
    		assert(!CORBA::is_nil(assembly));	

	//to update the assembly architecture
	assembly->updateArchitecture();


    	//To copy the drd file into rootDir/assemblyUUID
    	
	//
	//make target install directory
	//
	installDir=rootDir;
	installDir+=CORBA::string_dup(PATH_DELILIMITOR);
	installDir+=CORBA::string_dup(assemblyUUID.transcode());

	//To initial the arch object with targetDir\Architecture.xml
	archFile = installDir;
	archFile +=CORBA::string_dup(PATH_DELILIMITOR);
	archFile += CORBA::string_dup("Architecture.xml");
	arch = new ::STARCCM::Architecture(orb, assemblyUUID.transcode(), rootDir.in());
	arch->initialize(archFile.in());
	
	CORBA::String_var targetFile=installDir;
	targetFile+=CORBA::string_dup(PATH_DELILIMITOR);
	targetFile+=tmp;	
	
	if(HelpFun::copyFile(drdFileName,targetFile.in())==-1)
	{
		throw STARCCM::DynDeployment::DynDeployFailure();
	}    	

	//To do one of the dynmic actions
	DOM_Node nextnode = assemblynode.getNextSibling();
	while (nextnode != NULL)
	{
		//get node name
		DOMString op = nextnode.getNodeName();
		if (strcmp(op.transcode(),"remove")==0)
		{
			try
			{
				remove(parser, nextnode);
			}
			catch(DynRemoveFailure &)
			{
				cout << "DynRemove fails!" << endl;
				throw STARCCM::DynDeployment::DynDeployFailure();
			}
			catch(...)
			{
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				throw;
			}
			
			nextnode = nextnode.getNextSibling();
			continue;
		}

		if (strcmp(op.transcode(),"add")==0)
		{
			try
			{
				add(parser, nextnode);
			}
			catch(DynCreateFailure &)
			{
				cout << "DynCreate fails!" << endl;
				throw STARCCM::DynDeployment::DynDeployFailure();
			}
			catch(...)
			{
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				throw;
			}

			nextnode = nextnode.getNextSibling();
			continue;
		}

		if (strcmp(op.transcode(),"replacement")==0)
		{
			try
			{
				replacement(parser, nextnode);
			}
			catch(DynReplacementFailure &)
			{
				cout << "DynReplacement fails in dynremove!" << endl;
				throw STARCCM::DynDeployment::DynDeployFailure();
			}
			catch(...)
			{
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				throw;
			}

			nextnode = nextnode.getNextSibling();
			continue;
		}

		if (strcmp(op.transcode(),"migrate")==0)
		{
			try
			{
				replacement(parser, nextnode);
			}
			catch(DynMigrateFailure &)
			{
				throw STARCCM::DynDeployment::DynDeployFailure();
			}
			catch(const CORBA::SystemException &)
			{
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				throw;
			}
			
			nextnode = nextnode.getNextSibling();
			continue;
		}
		
		if(strcmp(op.transcode(),"link")==0)
		{
			try
			{
				link(parser, nextnode);
			}
			catch(DynLinkFailure &)
			{
				throw STARCCM::DynDeployment::DynDeployFailure();
			}
			catch(const CORBA::SystemException &)
			{
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				throw;
			}

			nextnode = nextnode.getNextSibling();
			continue;
		}

		if(strcmp(op.transcode(),"unlink")==0)
		{
			try
			{
				unlink(parser, nextnode);
			}
			catch(DynUnLinkFailure &)
			{
				throw STARCCM::DynDeployment::DynDeployFailure();
			}
			catch(const CORBA::SystemException &)
			{
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				throw;
			}

			nextnode = nextnode.getNextSibling();
			continue;
		}

		if (strcmp(op.transcode(),"changeproperty")==0)
		{
			try
			{
				changeproperty(parser, nextnode);
			}
			catch(DynRemoveFailure &)
			{
				throw STARCCM::DynDeployment::DynDeployFailure();
			}
			catch(const CORBA::SystemException &)
			{
				if(parser!=NULL)
				{
					delete parser;
					parser=NULL;
				}
				throw;
			}

			nextnode = nextnode.getNextSibling();
			continue;
		}	

		nextnode = nextnode.getNextSibling();
	}

	//to update the assembly architecture
	assembly->updateArchitecture();

	//delete the drd file in rootDir/temp/*.drd
#ifdef WIN32
		if(!DeleteFile(drdFileName))
#else
		if(::unlink(drdFileName.in()) == -1)
#endif
		{
			TRACE0(LEVEL1,"XMLParser: delete the temp dtd file failed.");
			
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}
			
			throw STARCCM::DynDeployment::DynCreateFailure();		
		}

	//delete the drd file in rootDir/assemblyUUID/*.drd
#ifdef WIN32
		if(!DeleteFile(targetFile.in()))
#else
		if(::unlink(targetFile.in()) == -1)
#endif
		{
			TRACE0(LEVEL1,"XMLParser: delete the temp dtd file failed.");
			
			if(parser!=NULL)
			{
				delete parser;
				parser=NULL;
			}
			
			throw STARCCM::DynDeployment::DynCreateFailure();		
		}

	//
	//free XMLParser resource
	//
	
	if(parser!=NULL)
	{
		delete parser;
		parser=NULL;
	}	

	//
	//free arch object
	//
	
	if(arch!=NULL)
	{
		delete arch;
		arch = NULL;
	}
	
	//to stop the dyndeployment
	stop();
	
    	if(verbose)
	{
		cout<<"dynDeploy: dynDeployment is finished successfully!"<<endl;
	}
	
}

void
DynDeploymentMgr_impl::create(const char* dcd_loc)
	 throw(DynCreateFailure,
	 	STARCCM::DynDeployment::InvalidAddFile,
		CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"create: begin to create new components." << endl;
	}

	//To get assemblyfactory corbaloc
	CORBA::String_var assemblyfactoryloc;
	try
	{
		assemblyfactoryloc = DomainMgr ->getObject(ASSEMBLYFACTORY,"");
	}
	catch(STARCCM::Deployment::ObjectNotExist)
	{
		cout << "create: the assemblyfacotry is not exist.\n";
		throw STARCCM::DynDeployment::DynCreateFailure();
	}

	//To get assemblyfactory object referrence
	CORBA::Object_var obj = orb -> string_to_object(assemblyfactoryloc.in());
	try
	{
	       obj = orb->string_to_object(assemblyfactoryloc);
        }
        catch(const CORBA::BAD_PARAM&)
        {
        	cout << "create: invalid AssemblyFactory corbaloc.\n";  
        	throw STARCCM::DynDeployment::DynCreateFailure();
        }
	
    	if(CORBA::is_nil(obj))
    	{
        	cout << "create: cannot get the reference of AssemblyFactory.\n";    
		throw STARCCM::DynDeployment::DynCreateFailure();
   	}

    	AssemblyFactory_var assemblyfactoryobj = AssemblyFactory::_narrow(obj);
    		assert(!CORBA::is_nil(assemblyfactoryobj));

	Cookie_var cookie;
    	try
    	{
    		cout<<"dynamicly create assembly"<<endl;
    		cookie = assemblyfactoryobj ->dynCreate(dcd_loc);
    		
//    		cout<<"look up assembly"<<endl;
//    		Assembly_var assembly=assemblyfactoryobj->lookup(cookie.in());
 		
    		cout<<"get the assembly state now."<<endl;
    		try
    		{
    			if(assembly->get_state()==INSERVICE)
    			{
    				cout<<"assembly state is inservice"<<endl;
    			}
    			else if(assembly->get_state()==INACTIVE)
    			{
				cout<<"assembly state is inActive"<<endl;				
    			}
    			else 
    			{
    				cout<<"Assembly is wrong."<<endl;
    				throw STARCCM::DynDeployment::DynCreateFailure();
    			}
    		}
    		catch(const CORBA::SystemException &)
    		{
    			throw;
    		}
    		catch(...)
    		{
    			cout<<"assembly is wrong ,get state failed"<<endl;
    			throw STARCCM::DynDeployment::DynCreateFailure();
    		}	

    		cout<<"dynamicly build the assembly"<<endl;
    		try
    		{
    			assembly->dynBuild();
    		}
    		catch(const CORBA::SystemException &)
    		{
    			TRACE0(LEVEL1,"create: build assembly failed in CORBA System Exception.\n");
    			throw;
    		}
    		catch(const Components::CreateFailure &)
    		{
    			TRACE0(LEVEL1,"create: build assembly failed in CreateFailure.\n");
    			throw STARCCM::DynDeployment::DynCreateFailure();		
    		}
    		

    		cout<<"get the assembly state now."<<endl;
    		try
    		{
    			if(assembly->get_state()==INSERVICE)
    			{
    				cout<<"assembly state is inservice"<<endl;
    			}
    			else if(assembly->get_state()==INACTIVE)
    			{
				cout<<"assembly state is inActive"<<endl;				
    			}
    			else 
    			{
    				cout<<"Assembly is wrong."<<endl;
    				throw STARCCM::DynDeployment::DynCreateFailure();
    			}
    		}
    		catch(const CORBA::SystemException &)
    		{
    			throw;
    		}
    		catch(...)
    		{
    			cout<<"assembly is wrong ,get state failed"<<endl;
    			throw STARCCM::DynDeployment::DynCreateFailure();
    		}
    		
      	}
    	catch(const CORBA::SystemException &)
    	{
    		throw;
    	}	
    	catch(const Components::Deployment::InvalidLocation &)			              
    	{
    		cout<<"dynamicly create assembly failed in invalid location."<<endl;
    		throw STARCCM::DynDeployment::DynCreateFailure();
    	}
    	catch(const Components::CreateFailure &) 
    	{
    		cout<<"dynamicly create assembly failed in Components::CreateFailure."<<endl;
    		throw STARCCM::DynDeployment::DynCreateFailure();	
    	}
    	catch(const Components::Deployment::InvalidAssembly &)
    	{
    		cout<<"lookup or destroy assembly failed in InvalidAssembly."<<endl;
    		throw STARCCM::DynDeployment::DynCreateFailure();	    		
    	}    	
	catch(const Components::RemoveFailure &)
	{
    		cout<<"destroy assembly failed in remove failure."<<endl;
    		throw STARCCM::DynDeployment::DynCreateFailure();			
	}	
	
	arch->initialize(archFile.in());
	
	if(verbose)
	{
		cout<<"create: create new components successfully!." << endl;
	}
}

void
DynDeploymentMgr_impl::remove(XMLParser *parser, DOM_Node rmnode)
    	 throw(DynRemoveFailure,
    	 	STARCCM::DynDeployment::InvalidDrdFile,
    	 	CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"remove: begin to remove." << endl;
	}

	//find out the passivatecomponents node
	DOM_Node passivateNode = parser -> findChild(rmnode, "passivatecomponents");
	if (passivateNode == NULL)
	{
		TRACE0(LEVEL1,"dynremove: Invalid dynmic reconfiguration descriptor file: have no passivatecomponents element. \n");		
		throw DynRemoveFailure();
	}

	try
	{
		//STEP 1 passivate components
		passivatecomponents(parser, passivateNode);

		//STEP 2 rumcomplete
		runcomplete(parser, rmnode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynRemoveFailure();
	}

	//STEP 3 disconnections
	DOM_Node disconnectionsNode = parser -> findChild(rmnode, "disconnections");
	if (disconnectionsNode != NULL)
	{
		try
		{
			assembly->dynUnLink("remove");
		}
		catch(...)
		{
			cout << "dynremove: fails in disconnections" << endl;
			throw DynRemoveFailure();
		}
	}

	try
	{
		//STEP 4 unregister
		unregister(parser, rmnode);

		//STEP 5 delete components
		deleteCom(parser, rmnode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynRemoveFailure();
	}

	//STEP 6 set components' properties
	DOM_Node componentspropertiesNode = parser->findChild(rmnode, "componentsproperties");
	if (componentspropertiesNode != NULL)
	{
		try
		{
			setComproperties(parser, componentspropertiesNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynRemoveFailure();
		}
	}

	//find out the activatecomponents node
	DOM_Node activateNode = parser -> findChild(rmnode, "activatecomponents");
	if (activateNode != NULL)
	{
		try
		{
			//STEP 7 activate components
			activatecomponents(parser, activateNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynRemoveFailure();
		}
	}
	
	if(verbose)
	{
		cout<<"remove: remove successfully!" << endl;
	}
	
}

void 
DynDeploymentMgr_impl::add(XMLParser *parser, DOM_Node addnode)
	 throw(DynCreateFailure,
	 	STARCCM::DynDeployment::InvalidDrdFile,
	 	STARCCM::DynDeployment::InvalidAddFile,
		CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"add: begin to add." << endl;
	}

	//find out the passivatecomponents node
	DOM_Node passivateNode = parser -> findChild(addnode, "passivatecomponents");
	if (passivateNode != NULL)
	{
		try
		{
			//STEP 1 passivate components
			passivatecomponents(parser, passivateNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynCreateFailure();
		}
	}

	DOM_Node createNode  = parser -> findChild(addnode, "create");
	if (createNode == NULL)
	{
		cout << "add: fails in no create node!" << endl;
		throw STARCCM::DynDeployment::InvalidAddFile();
	}

	DOMString filename = parser->getAttr(createNode, "filename");

	CORBA::String_var archiveFile = initialDir;
	archiveFile += CORBA::string_dup(PATH_DELILIMITOR);
	archiveFile += CORBA::string_dup(filename.transcode());

	try
	{
		//STEP 2 create new components
		create(archiveFile.in());

		//STEP 3 statetransfer
		statetransfer(parser, addnode);
	}
	catch(STARCCM::DynDeployment::InvalidAddFile &)
	{
		throw;
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynCreateFailure();
	}

	//STEP 4 set components' properties
	DOM_Node componentspropertiesNode = parser->findChild(addnode, "componentsproperties");
	if (componentspropertiesNode != NULL)
	{
		try
		{
			setComproperties(parser, componentspropertiesNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynCreateFailure();
		}
	}	

	//STEP 5 connections
	DOM_Node connectionsNode = parser -> findChild(addnode, "connections");
	if (connectionsNode != NULL)
	{
		try
		{
			assembly->dynLink("add");
		}
		catch(...)
		{
			cout << "dynadd: fails in connections" << endl;
			throw DynCreateFailure();
		}
	}

	try
	{
		//STEP 6 configurationcomplete
		configurationcomplete(parser, addnode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynCreateFailure();
	}

	//find out the activatecomponents node
	DOM_Node activateNode = parser -> findChild(addnode, "activatecomponents");
	if (activateNode != NULL)
	{
		try
		{
			//STEP 7 activate components
			activatecomponents(parser, activateNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynCreateFailure();
		}
	}
	
	if(verbose)
	{
		cout<<"add: add successful!" << endl;
	}
}

void 
DynDeploymentMgr_impl::replacement(XMLParser *parser, DOM_Node rpnode)
	 throw(DynReplacementFailure,
	 	STARCCM::DynDeployment::InvalidDrdFile,
		CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"replacement: begin to replacement." << endl;
	}

	//find out the passivatecomponents node
	DOM_Node passivateNode = parser -> findChild(rpnode, "passivatecomponents");
	if (passivateNode == NULL)
	{
		TRACE0(LEVEL1,"replacement: Invalid dynmic reconfiguration descriptor file: have no passivatecomponents element. \n");		
		throw DynReplacementFailure();
	}

	try
	{
		//STEP 1 passivate components
		passivatecomponents(parser, passivateNode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynReplacementFailure();
	}
	
	DOM_Node createNode  = parser -> findChild(rpnode, "create");
	if (createNode == NULL)
	{
		cout << "replacement: fails in no create node!" << endl;
		throw STARCCM::DynDeployment::InvalidAddFile();
	}

	DOMString filename = parser->getAttr(createNode, "filename");

	CORBA::String_var archiveFile = initialDir;
	archiveFile += CORBA::string_dup(PATH_DELILIMITOR);
	archiveFile += CORBA::string_dup(filename.transcode());

	try
	{
		//STEP 2 create new components
		create(archiveFile.in());

		//STEP 3 statetransfer
		statetransfer(parser, rpnode);

		//STEP 4 rumcomplete
		runcomplete(parser, rpnode);
	}
	catch(STARCCM::DynDeployment::InvalidAddFile &)
	{
		throw;
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynReplacementFailure();
	}

	//STEP 5 disconnections
	DOM_Node disconnectionsNode = parser -> findChild(rpnode, "disconnections");
	if (disconnectionsNode != NULL)
	{
		try
		{
			assembly->dynUnLink("replacement");
		}
		catch(...)
		{
			cout << "replacement: fails in disconnections" << endl;
			throw DynReplacementFailure();
		}
	}

	try
	{
		//STEP 6 unregister
		unregister(parser, rpnode);

		//STEP 7 delete components
		deleteCom(parser, rpnode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynReplacementFailure();
	}

	//STEP 8 connections
	DOM_Node connectionsNode = parser -> findChild(rpnode, "connections");
	if (connectionsNode != NULL)
	{
		try
		{
			assembly->dynLink("replacement");
		}
		catch(...)
		{
			cout << "replacement: fails in connections" << endl;
			throw DynReplacementFailure();
		}
	}

	//STEP 9 set components' properties
	DOM_Node componentspropertiesNode = parser->findChild(rpnode, "componentsproperties");
	if (componentspropertiesNode != NULL)
	{
		try
		{
			setComproperties(parser, componentspropertiesNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynReplacementFailure();
		}
	}

	try
	{
		//STEP 10 redirect request
		redirectrequest(parser, rpnode);
			
		//STEP 11 configurationcomplete
		configurationcomplete(parser, rpnode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynReplacementFailure();
	}

	//find out the activatecomponents node
	DOM_Node activateNode = parser -> findChild(rpnode, "activatecomponents");
	if (activateNode != NULL)
	{
		try
		{
			//STEP 12 activate components
			activatecomponents(parser, activateNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynReplacementFailure();
		}
	}
	
	if(verbose)
	{
		cout<<"replacement: replacement successfully!" << endl;
	}
}

void
DynDeploymentMgr_impl::link(XMLParser *parser, DOM_Node linknode)
    	 throw(DynLinkFailure,
    	 	STARCCM::DynDeployment::InvalidDrdFile,
    	 	CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"dynlink: begin to link." << endl;
	}

	//find out the passivatecomponents node
	DOM_Node passivateNode = parser -> findChild(linknode, "passivatecomponents");
	if (passivateNode == NULL)
	{
		TRACE0(LEVEL1,"dynlink: Invalid dynmic reconfiguration descriptor file: have no passivatecomponents element. \n");		
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}

	try
	{
		//STEP 1 passivate components
		passivatecomponents(parser, passivateNode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynLinkFailure();
	}

	DOM_Node connectionsNode = parser -> findChild(linknode, "connections");
	if (connectionsNode == NULL)
	{
		TRACE0(LEVEL1,"dynlink: Invalid dynmic reconfiguration descriptor file: have no connections element. \n");		
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}

	try
	{
		//STEP 2 connections
		assembly->dynLink("link");
	}
	catch(...)
	{
		cout << "dynlink: fails in connections" << endl;
		throw DynLinkFailure();
	}

	//STEP 3 set components' properties
	DOM_Node componentspropertiesNode = parser->findChild(linknode, "componentsproperties");
	if (componentspropertiesNode != NULL)
	{
		try
		{
			setComproperties(parser, componentspropertiesNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynLinkFailure();
		}
	}	

	DOM_Node activateNode = parser -> findChild(linknode, "activatecomponents");
	if (activateNode == NULL)
	{
		TRACE0(LEVEL1,"dynlink: Invalid dynmic reconfiguration descriptor file: have no activatecomponents element. \n");		
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}
	
	try
	{
		//STEP 4 activate components
		activatecomponents(parser, activateNode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynLinkFailure();
	}
	
	if(verbose)
	{
		cout<<"dynlink: link successfully!" << endl;
	}
}

void
DynDeploymentMgr_impl::unlink(XMLParser *parser, DOM_Node unlinknode)
    	 throw(DynUnLinkFailure,
    	 	STARCCM::DynDeployment::InvalidDrdFile,
    	 	CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"dynunlink: begin to unlink." << endl;
	}

	//find out the passivatecomponents node
	DOM_Node passivateNode = parser -> findChild(unlinknode, "passivatecomponents");
	if (passivateNode == NULL)
	{
		TRACE0(LEVEL1,"dynunlink: Invalid dynmic reconfiguration descriptor file: have no passivatecomponents element. \n");		
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}

	try
	{
		//STEP 1 passivate components
		passivatecomponents(parser, passivateNode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynUnLinkFailure();
	}

	DOM_Node connectionsNode = parser -> findChild(unlinknode, "connections");
	if (connectionsNode == NULL)
	{
		TRACE0(LEVEL1,"dynunlink: Invalid dynmic reconfiguration descriptor file: have no connections element. \n");		
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}

	try
	{
		//STEP 2 connections
		assembly->dynLink("unlink");
	}
	catch(...)
	{
		cout << "dynunlink: fails in connections" << endl;
		throw DynUnLinkFailure();
	}

	//STEP 3 set components' properties
	DOM_Node componentspropertiesNode = parser->findChild(unlinknode, "componentsproperties");
	if (componentspropertiesNode != NULL)
	{
		try
		{
			setComproperties(parser, componentspropertiesNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw DynUnLinkFailure();
		}
	}	

	DOM_Node activateNode = parser -> findChild(unlinknode, "activatecomponents");
	if (activateNode == NULL)
	{
		TRACE0(LEVEL1,"dynunlink: Invalid dynmic reconfiguration descriptor file: have no activatecomponents element. \n");		
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}
	
	try
	{
		//STEP 4 activate components
		activatecomponents(parser, activateNode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw DynUnLinkFailure();
	}
	
	if(verbose)
	{
		cout<<"dynunlink: unlink successfully!" << endl;
	}
}

void
DynDeploymentMgr_impl::changeproperty(XMLParser *parser, DOM_Node cplnode)
    	 throw(ChangePropertyFailure,
    	 	STARCCM::DynDeployment::InvalidDrdFile,
    	 	CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"changeproperty: begin to change property." << endl;
	}

	//find out the passivatecomponents node
	DOM_Node passivateNode = parser -> findChild(cplnode, "passivatecomponents");
	if (passivateNode == NULL)
	{
		TRACE0(LEVEL1,"changeproperty: Invalid dynmic reconfiguration descriptor file: have no passivatecomponents element. \n");		
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}

	try
	{
		//STEP 1 passivate components
		passivatecomponents(parser, passivateNode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw ChangePropertyFailure();
	}

	//STEP 2 set components' properties
	DOM_Node componentspropertiesNode = parser->findChild(cplnode, "componentsproperties");
	if (componentspropertiesNode != NULL)
	{
		try
		{
			setComproperties(parser, componentspropertiesNode);
		}
		catch(STARCCM::DynDeployment::InvalidDrdFile &)
		{
			throw;
		}
		catch(CORBA::SystemException &)
		{
			throw;
		}
		catch(...)
		{
			throw ChangePropertyFailure();
		}
	}	

	DOM_Node activateNode = parser -> findChild(cplnode, "activatecomponents");
	if (activateNode == NULL)
	{
		TRACE0(LEVEL1,"changeproperty: Invalid dynmic reconfiguration descriptor file: have no activatecomponents element. \n");		
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}
	
	try
	{
		//STEP 3 activate components
		activatecomponents(parser, activateNode);
	}
	catch(STARCCM::DynDeployment::InvalidDrdFile &)
	{
		throw;
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
	catch(...)
	{
		throw ChangePropertyFailure();
	}
	
	if(verbose)
	{
		cout<<"changeproperty: change property successfully!" << endl;
	}
}

//*******************************************************
//download file from the console
//if fileaccessor method wrong ,throw FileSystemError
//other error ,throw FileDownloadFailure
//*******************************************************
void
DynDeploymentMgr_impl::download(const char* fileloc)
throw(STARCCM::DynDeployment::FileDownloadFailure, 
	  FileSystemError,
		CORBA::SystemException)
{

	//
	//first
	//get the reference of the fileaccessorfactory object on console from the DomainMgr
	//    
	CORBA::String_var objloc;
 	try
	{
 		objloc = DomainMgr -> getObject(FILEACCESSORFACTORY,"");
	}
	catch(STARCCM::Deployment::ObjectNotExist &)
	{
		cout << "DynDeploymentMgr: can not get fileacessorfactory " << endl;
		throw STARCCM::DynDeployment::FileDownloadFailure();
	}
	catch(CORBA::SystemException &)
	{
		throw;
	}
 	TRACE1(LEVEL6,"DynDeploymentMgr: the object corbaloc is %s .\n",objloc);
 	
	CORBA::Object_var obj;
	try
	{
		obj = orb -> string_to_object(objloc.in());
	}
	catch(const CORBA::BAD_PARAM&)
    {
    	TRACE1(LEVEL1,"DynDeploymentMgr: invalid corbaloc %s.\n",objloc);        	
		return;
    }
	if(obj==NULL)
	{
		TRACE0(LEVEL1,"DynDeploymentMgr: can not get the fileaccesorfactory object of the console.\n");		
		throw FileDownloadFailure();
	}
	
	FileAccessorFactory_var ConsoleFileAccessorFactoryObj = FileAccessorFactory::_narrow(obj);
    	assert(!CORBA::is_nil(ConsoleFileAccessorFactoryObj)); 
    	
	//
	//create the fileaccessor object 
	//
	FileAccessor_var ConsoleFileAccessorObj=ConsoleFileAccessorFactoryObj->createFileAccessor();
	assert(!CORBA::is_nil(ConsoleFileAccessorObj)); 
	
	
	//
	//download the file from the console to the target directory
	//the target dir is rootDir\temp
	//
    	
    	CORBA::String_var targetDir=rootDir;
    	targetDir+=CORBA::string_dup(PATH_DELILIMITOR);
	targetDir+=CORBA::string_dup("temp");
    	
    	FILE *fileWriteStream;    	
    	CORBA::String_var path=fileloc;	
    	
    	try
    	{
    		TRACE0(LEVEL5,"DynDeploymentMgr: Locate file.\n");    		
    		ConsoleFileAccessorObj -> locate_file(path.in()); 

		TRACE0(LEVEL5,"DynDeploymentMgr: Transfer the file from remote host.\n");    				
#ifdef WIN32
		if( _chdir(targetDir.in()))
#else
		if(chdir(targetDir.in()) == -1)
#endif
		{
			TRACE1(LEVEL1,"DynDeploymentMgr: Unable to locate the directory %s.",targetDir.in());			
	   		throw FileDownloadFailure();
     
	   	}   
		
		//To find out the file name
#ifdef SOLARIS
		const char* tmp = strrchr(fileloc,PATH_DELILIMITOR_CHAR);
#else
		char* tmp = strrchr(fileloc,PATH_DELILIMITOR_CHAR);
#endif
		CORBA::String_var targetFilename = CORBA::string_dup(tmp+1);
    	
		if( (fileWriteStream = fopen( targetFilename.in(), "wb" )) == NULL )
		{
			TRACE0(LEVEL1,"DynDeploymentMgr: Unable to create the file.\n");			
		  	throw FileDownloadFailure();
		}    	
	  
    		int from_octet=0;
    		int max_octets=1024000;
    		int writeSize;	  
		FileOctetSeq_var writeBuffer;    	
		
    		do
    		{
    		  			  
    			writeBuffer=ConsoleFileAccessorObj->get_octet_seq(from_octet,max_octets);
    			writeSize=writeBuffer->length();
    		
    			if(fseek( fileWriteStream, from_octet , SEEK_SET)!=0)
    			{
    				TRACE0(LEVEL1,"DynDeploymentMgr: File seek error. \n");    				
    				throw FileDownloadFailure();
    			}
    		
    			fwrite(writeBuffer->get_buffer(), 1, writeSize, fileWriteStream);
    		
    			from_octet+=writeSize;
    		
    		}
    		while(writeSize==max_octets);
    	}
    	catch(const FileSystemError &e)
    	{
    		TRACE1(LEVEL1,"DynDeploymentMgr: file transer error : %s .\n",e.desc);    		
    		throw;
    	}
    	catch(const CORBA::SystemException &)
    	{    		
    		throw;
    	}
	catch(...)
	{
	   	TRACE0(LEVEL1,"DynDeploymentMgr: file transfer happen unknown exception \n");    		
	   	throw CORBA::UNKNOWN();	   		    	
	};    
	    
	fclose(fileWriteStream);	
	
}

//deleted by xiao heping 2004/05/27
//****************************************************
//copyFile copy the source file specified by source 
//to the target file specified by target
//if any error occured return -1, else return 0
//****************************************************
//int 
//DynDeploymentMgr_impl::copyFile(const char *source, const char *target)
//{
//
//	TRACE1(LEVEL6,"dynCreate: copy file the source file is %s \n",source);
//	TRACE1(LEVEL6,"dynCreate: copy file the target file is %s \n",target);	
//
//#ifdef WIN32	
//
//	if(CopyFile(source,target,false)==0)
//	{
//		TRACE0(LEVEL1,"dynCreate: Copy file into the directory specified failed.\n");		
//		return -1;
//	}
//#endif
//	return 0;
//}
//end delete

void 
DynDeploymentMgr_impl::passivatecomponents(XMLParser *parser, DOM_Node passivateNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"passivatecomponents: begin to passivate components." << endl;
	}
	
	DOM_NodeList passivateChildrenNodeList = passivateNode.getChildNodes();
	DOM_Node passivateChild;
	DOMString passivateChildName;

	MapOidSeq PassivateClientMap;
	MapPassivateType PassivateTypeMap;

	MapOidSeq::iterator iter;
	MapPassivateType::iterator titer;

	for (int i=0; i<passivateChildrenNodeList.getLength(); i++)
	{
		passivateChild = passivateChildrenNodeList.item(i);
		passivateChildName = passivateChild.getNodeName();

		if (passivateChildName.equals("passivatecomponent"))
		{
			DOMString comid = parser->getAttr(passivateChild, "idref");
			ArchComponent_var acom = arch->getArchComponent(comid.transcode());//xiaoxin
			CCMObject_var ccmobj = Components::CCMObject::_narrow(acom->comref);
			if (!CORBA::is_nil(ccmobj))
			{
				try
				{
					ccmobj->passivate_component();
				}
				catch(...)
				{
					cout << "passivate Components fails!" << endl;
					throw;
				}
			}
			continue;
		}

		if (passivateChildName.equals("passivateclient"))
		{
			DOMString comid = parser->getAttr(passivateChild, "idref");
			DOMString passivatetype = parser->getAttr(passivateChild, "passivatetype");
			ArchComponent_var acom = arch->getArchComponent(comid.transcode());//xiaoxin
			iter = PassivateClientMap.find(string(acom->homeuuid));
			if (iter != PassivateClientMap.end())
			{
				int len = (*iter).second->length();
				(*iter).second->length(len+1);
				(*iter).second[len] = CORBA::string_dup(acom->id);
				
				titer = PassivateTypeMap.find(string(acom->homeuuid));
				if (strcmp(passivatetype.transcode(), "block") == 0)
				{
					len = (*titer).second->length();
					(*titer).second->length(len+1);
					(*titer).second[len] = HOLDREQUEST;
				}
				else if (strcmp(passivatetype.transcode(), "reject") == 0)
					{
						len = (*titer).second->length();
						(*titer).second->length(len+1);
						(*titer).second[len] = REJECTREQUEST;
					}
					else
					{
						cout << "passivate clients fails in invalid drd file!" << endl;
						throw STARCCM::DynDeployment::InvalidDrdFile();
					}
			}
			else
			{
				CORBA::StringSeq_var seq = new CORBA::StringSeq;
				seq->length(1);
				seq[0] = CORBA::string_dup(acom->id);
				PassivateClientMap.insert(MapOidSeq::value_type(string(acom->homeuuid), seq));

				::Components::Deployment::PassivateClientTypeSeq_var typeseq = new ::Components::Deployment::PassivateClientTypeSeq;
				typeseq->length(1);
				if (strcmp(passivatetype.transcode(), "block") == 0)
					typeseq[0] = HOLDREQUEST;
				else if (strcmp(passivatetype.transcode(), "reject") == 0)
						typeseq[0] = REJECTREQUEST;
					else
					{
						cout << "passivate clients fails in invalid drd file!" << endl;
						throw STARCCM::DynDeployment::InvalidDrdFile();
					}
					
				PassivateTypeMap.insert(MapPassivateType::value_type(string(acom->homeuuid), typeseq));
			}

		}
	}

	iter = PassivateClientMap.begin();
	while (iter != PassivateClientMap.end())
	{
		ArchHome_var ahome = arch->getArchHome_By_UUID((*iter).first.c_str());
		Container_var container = ::Components::Deployment::Container::_narrow(ahome->containerref);
		if (!CORBA::is_nil(container))
		{
			titer = PassivateTypeMap.find((*iter).first.c_str());
			try
			{
				container->passivateClient(ahome->uuid, (*iter).second, (*titer).second);
			}
			catch(...)
			{
				cout << "passivatecomponents: fails in passivate clients!" << endl;
				throw;
			}
		}
		iter++;
	}
	
	if(verbose)
	{
		cout<<"passivatecomponents: passivate components successfully!" << endl;
	}
}

void 
DynDeploymentMgr_impl::runcomplete(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	DOM_Node runcompleteNode = parser->findChild(fatherNode, "runcomplete");
	if (runcompleteNode == NULL)
		return;

	if(verbose)
	{
		cout<<"runcomplete: begin to invocate components' runcomplete operation." << endl;
	}
	
	DOM_NodeList childrenList = fatherNode.getChildNodes();
	DOM_Node childNode;
	DOMString childNodeName;

	for (int i=0; i<childrenList.getLength(); i++)
	{
		childNode = childrenList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("runcomplete"))
		{
			DOMString comid = parser->getAttr(childNode, "idref");
			ArchComponent_var acom = arch->getArchComponent(comid.transcode());
			::Components::CCMObject_var ccmobj = ::Components::CCMObject::_narrow(acom->comref);
			if (!CORBA::is_nil(ccmobj))
			{
				try
				{
					ccmobj->run_complete();
				}
				catch(...)
					{
					cout << "rumcomplete fails in component: "  << acom->id << endl;
					throw;
				}				
			}
		}
	}

	if(verbose)
	{
		cout<<"runcomplete: invocate components' runcomplete operation successfully!" << endl;
	}
}

void 
DynDeploymentMgr_impl::unregister(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	DOM_Node unregisterNode = parser->findChild(fatherNode, "unregister");
	if (unregisterNode == NULL)
		return;

	if(verbose)
	{
		cout<<"unregister: begin to unregister components." << endl;
	}
	
	DOM_Node rootNode = parser->getRootNode("DynReconfig");
	DOM_Node componentAssemblyNode = parser->findChild(rootNode, "componentassembly");
	DOM_Node namingserviceNode = parser->findChild(componentAssemblyNode, "namingservice");
	if (namingserviceNode == NULL)
	{
		cout << "unregister: invalid drd file no namingservice node!" << endl;
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}
	DOMString namingserviceCorbaloc = parser->getAttr(namingserviceNode, "corbaloc");
	if (namingserviceCorbaloc == NULL)
	{
		cout << "unregister: invalid drd file no namingservice corbaloc!" << endl;
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}

	CORBA::Object_var obj;
	try
	{
		obj=orb->string_to_object(namingserviceCorbaloc.transcode());		
	}
	catch(const CORBA::BAD_PARAM&)
	{
		TRACE1(LEVEL2,"unregister: can not get the nameServiceContext object from the corbaloc %s.\n",namingserviceCorbaloc.transcode());
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}	
	
	if(CORBA::is_nil(obj))
	{
		TRACE1(LEVEL2,"Assembly: can not get the nameServiceContext object from the corbaloc %s.\n",namingserviceCorbaloc.transcode());
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}
 	
	CosNaming::NamingContext_var nameServiceContextObj = CosNaming::NamingContext::_narrow(obj);
	assert(!CORBA::is_nil(nameServiceContextObj));  
	
	DOM_NodeList childrenList = fatherNode.getChildNodes();
	DOM_Node childNode;
	DOMString childNodeName;

	for (int i=0; i<childrenList.getLength(); i++)
	{
		childNode = childrenList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("unregister"))
		{
			DOMString name = parser->getAttr(childNode, "name");
			
			//unregister the component from the namingservice
			try
			{	
				CosNaming::Name aName;						
				aName.length(1);
				aName[0].id = CORBA::string_dup(name.transcode());
			    	aName[0].kind = CORBA::string_dup("");									    
			    	TRACE1(LEVEL6,"dynDeploy: unregister Component from naming service with the name is %s. \n",aName[0].id);
				nameServiceContextObj -> unbind(aName);
			}
			catch(...)
			{
				cout << "unregister the component from the namingservice fails." << endl;
				throw;	
			}
		}
	}

	if(verbose)
	{
		cout<<"unregister: unregister components successfully!" << endl;
	}
}

void 
DynDeploymentMgr_impl::deleteCom(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"deleteCom: begin to delete components." << endl;
	}
	
	DOM_Node deleteNode = parser->findChild(fatherNode, "delete");
	if (deleteNode == NULL)
	{
		cout << "deleteCom: fails in no delete node!" << endl;
		throw STARCCM::DynDeployment::InvalidDrdFile();
	}
	
	DOM_NodeList childrenList = fatherNode.getChildNodes();
	DOM_Node childNode;
	DOMString childNodeName;

	for (int i=0; i<childrenList.getLength(); i++)
	{
		childNode = childrenList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("delete"))
		{
			DOMString comid = parser->getAttr(childNode, "idref");
			ArchComponent_var acom = arch->getArchComponent(comid.transcode());//xiaoxin
			::Components::CCMObject_var ccmobj = ::Components::CCMObject::_narrow(acom->comref);
			ArchHome_var ahome = arch->getArchHome_By_UUID(acom->homeuuid);
			::Components::CCMHome_var ccmhome = ::Components::CCMHome::_narrow(ahome->homeref);
			if (!CORBA::is_nil(ccmhome))
			{
				try
				{
					ccmhome->remove_component(ccmobj.in());
				}
				catch(...)
					{
					cout << "remove componet fails home's remove_component invokation: " << ahome->id << " remove " <<
						acom->id  << endl;
					throw;
				}				
			}
		}
	}

	if(verbose)
	{
		cout<<"deleteCom: delete components successfully!" << endl;
	}
}

void 
DynDeploymentMgr_impl::setComproperties(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::ConfigFailure,
			STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"setComproperties: begin to set components properties." << endl;
	}
	
	DOM_NodeList childrenList = fatherNode.getChildNodes();
	DOM_Node childNode;
	DOMString childNodeName;

	for (int i=0; i<childrenList.getLength(); i++)
	{
		childNode = childrenList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("componentproperties"))
		{
			DOMString comid = parser->getAttr(childNode, "idref");
			if (comid == NULL)
			{
				cout << "setComproperties: fails in invalid drd file componentproperties node has no idref attribute." << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			
			DOMString filename = parser->getAttr(childNode, "filename");

			CORBA::String_var cpfFileName = initialDir;
			cpfFileName += CORBA::string_dup(PATH_DELILIMITOR);
			cpfFileName += CORBA::string_dup(filename.transcode());
			//
			//download the cpf file from the console to the host 
			//and rename it as oldfilename(.cpf)
			//rootdir\temp\filename(.cpf)
			//	
		    	try
		    	{
		    		download(cpfFileName.in());
		    	}
		    	catch(const CORBA::SystemException &)
		    	{
		    		cout << "setComproperties: fails in downloading the cpf file" << endl;
		    		throw;
		    	}
		    	catch(...)
		    	{
		    		TRACE1(LEVEL1,"setComproperties: failed in download the cpf file from %s.\n ",filename.transcode());
		    		throw STARCCM::DynDeployment::InvalidDrdFile();
		    	}

			//To copy the cpf file into the install Dir
			CORBA::String_var cpfName = rootDir;
			cpfName += CORBA::string_dup(PATH_DELILIMITOR);
			cpfName += "temp";
			cpfName += CORBA::string_dup(PATH_DELILIMITOR);
			cpfName	+= CORBA::string_dup(filename.transcode());	
			
			CORBA::String_var targetFile=installDir;
			targetFile+=CORBA::string_dup(PATH_DELILIMITOR);
			targetFile+=CORBA::string_dup(filename.transcode());	
			
			if(HelpFun::copyFile(cpfName,targetFile.in())==-1)
			{
				cout << "setComproperties: fails in copy the cpf file to install Dir!" << endl;
				throw STARCCM::DynDeployment::ConfigFailure();
			}  

			try
			{
				assembly->setComponentPropertis(comid.transcode(), targetFile.in());
			}
			catch(...)
			{
				cout << "setComproperties: fails in assemby's setComponentPropertis invocation!" << endl;
				throw STARCCM::DynDeployment::ConfigFailure();
			}
		}
	}

	if(verbose)
	{
		cout<<"setComproperties: set components properties successfully!" << endl;
	}
}

void 
DynDeploymentMgr_impl::activatecomponents(XMLParser *parser, DOM_Node activateNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	if(verbose)
	{
		cout<<"activatecomponents: begin to activate components." << endl;
	}
	
	DOM_NodeList activateChildrenNodeList = activateNode.getChildNodes();
	DOM_Node activateChild;
	DOMString activateChildName;

	MapOidSeq ActivateClientMap;

	MapOidSeq::iterator iter;

	for (int i=0; i<activateChildrenNodeList.getLength(); i++)
	{
		activateChild = activateChildrenNodeList.item(i);
		activateChildName = activateChild.getNodeName();

		if (activateChildName.equals("activatecomponent"))
		{
			DOMString comid = parser->getAttr(activateChild, "idref");
			ArchComponent_var acom = arch->getArchComponent(comid.transcode());
			CCMObject_var ccmobj = Components::CCMObject::_narrow(acom->comref);
			if (!CORBA::is_nil(ccmobj))
			{
				try
				{
					ccmobj->activate_component();
				}
				catch(...)
				{
					cout << "activate Components fails!" << endl;
					throw;
				}
			}
			continue;
		}

		if (activateChildName.equals("activateclient"))
		{
			DOMString comid = parser->getAttr(activateChild, "idref");
			ArchComponent_var acom = arch->getArchComponent(comid.transcode());
			iter = ActivateClientMap.find(string(acom->homeuuid));
			if (iter != ActivateClientMap.end())
			{
				int len = (*iter).second->length();
				(*iter).second->length(len+1);
				(*iter).second[len] = CORBA::string_dup(acom->id);
			}
			else
			{
				CORBA::StringSeq_var seq = new CORBA::StringSeq;
				seq->length(1);
				seq[0] = CORBA::string_dup(acom->id);
				ActivateClientMap.insert(MapOidSeq::value_type(string(acom->homeuuid), seq));				
			}
		}
	}

	iter = ActivateClientMap.begin();
	while (iter != ActivateClientMap.end())
	{
		ArchHome_var ahome = arch->getArchHome_By_UUID((*iter).first.c_str());
		Container_var container = ::Components::Deployment::Container::_narrow(ahome->containerref);
		if (!CORBA::is_nil(container))
		{
			try
			{
				container->activateClient(ahome->uuid, (*iter).second);
			}
			catch(...)
			{
				cout << "activatecomponents: fails in acivate clients!" << endl;
				throw;
			}
		}
		iter++;
	}
	
	if(verbose)
	{
		cout<<"activatecomponents: activate components successfully!" << endl;
	}
}

void 
DynDeploymentMgr_impl::statetransfer(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	DOM_Node statetransferNode = parser->findChild(fatherNode, "statetransfer");
	if (statetransferNode == NULL)
		return;

	if(verbose)
	{
		cout<<"statetransfer: begin to transfer components' state." << endl;
	}
	
	DOM_NodeList childrenList = fatherNode.getChildNodes();
	DOM_Node childNode;
	DOMString childNodeName;

	for (int i=0; i<childrenList.getLength(); i++)
	{
		childNode = childrenList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("statetransfer"))
		{
			DOM_Node sourcecomponentNode = parser->findChild(childNode, "sourcecomponent");
			if (sourcecomponentNode == NULL)
			{
				cout << "statetransfer: fails in no sourcecomponent node" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			DOMString srccomid = parser->getAttr(sourcecomponentNode, "idref");
			if (srccomid == NULL)
			{
				cout << "statetransfer: fails in sourcecomponent node no idref attribute!" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			
			DOM_Node destinationcomponentNode = parser->findChild(childNode, "destinationcomponent");
			if (destinationcomponentNode == NULL)
			{
				cout << "statetransfer: fails in no destinationcomponent node" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			DOMString descomid = parser->getAttr(destinationcomponentNode, "idref");
			if (descomid == NULL)
			{
				cout << "statetransfer: fails in destinationcomponent node no idref attribute!" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			
			ArchComponent* acom = arch->getArchComponent(srccomid.transcode());
			if (acom == NULL)
			{
				cout << "statetransfer: fails in invalid sourcecomponent node idref attribute!" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			::Components::CCMObject_var srcobj = ::Components::CCMObject::_narrow(acom->comref);
			delete acom;
			acom = NULL;
			
			acom = arch->getArchComponent(descomid.transcode());
			if (acom == NULL)
			{
				cout << "statetransfer: fails in invalid destinationcomponent node idref attribute!" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			::Components::CCMObject_var desobj = ::Components::CCMObject::_narrow(acom->comref);
			delete acom;
			acom = NULL;
			
			try
			{
				Components::ConfigValues_var cfvalue = srcobj->externalize();
				desobj->initialize(cfvalue);
			}
			catch(...)
				{
				cout << "statetransfer: fails in components'statetransfer invocation! "  << endl;
				throw;
			}							
		}
	}
	
	if(verbose)
	{
		cout<<"statetransfer: transfer components' state successfully!" << endl;
	}
}

void 
DynDeploymentMgr_impl::configurationcomplete(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	DOM_Node configurationcompleteteNode = parser->findChild(fatherNode, "configurationcomplete");
	if (configurationcompleteteNode == NULL)
		return;

	if(verbose)
	{
		cout<<"configurationcomplete: begin to invocate components' configurationcomplete operation." << endl;
	}
	
	DOM_NodeList childrenList = fatherNode.getChildNodes();
	DOM_Node childNode;
	DOMString childNodeName;

	for (int i=0; i<childrenList.getLength(); i++)
	{
		childNode = childrenList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("configurationcomplete"))
		{
			DOMString comid = parser->getAttr(childNode, "idref");
			ArchComponent_var acom = arch->getArchComponent(comid.transcode());
			::Components::CCMObject_var ccmobj = ::Components::CCMObject::_narrow(acom->comref);
			if (!CORBA::is_nil(ccmobj))
			{
				try
				{
					ccmobj->configuration_complete();
				}
				catch(...)
					{
					cout << "configurationcomplete fails in component: "  << acom->id << endl;
					throw;
				}				
			}
		}
	}

	if(verbose)
	{
		cout<<"configurationcomplete: invocate components' configurationcomplete operation successfully!" << endl;
	}
}

void 
DynDeploymentMgr_impl::redirectrequest(XMLParser *parser, DOM_Node fatherNode)
		throw(STARCCM::DynDeployment::InvalidDrdFile,
	    		CORBA::SystemException)
{
	DOM_Node redirectrequestNode = parser->findChild(fatherNode, "redirectrequest");
	if (redirectrequestNode == NULL)
		return;

	if(verbose)
	{
		cout<<"redirectrequest: begin to redirect request." << endl;
	}

	MapOidSeq RedirectReqOldObjMap;
	MapObjSeq RedirectReqNewObjMap;

	MapOidSeq::iterator iter;
	MapObjSeq::iterator niter;
	
	DOM_NodeList childrenList = fatherNode.getChildNodes();
	DOM_Node childNode;
	DOMString childNodeName;

	for (int i=0; i<childrenList.getLength(); i++)
	{
		childNode = childrenList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("redirectrequest"))
		{
			DOM_Node sourcecomponentNode = parser->findChild(childNode, "sourcecomponent");
			if (sourcecomponentNode == NULL)
			{
				cout << "redirectrequest: fails in no sourcecomponent node" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			DOMString srccomid = parser->getAttr(sourcecomponentNode, "idref");
			if (srccomid == NULL)
			{
				cout << "redirectrequest: fails in sourcecomponent node no idref attribute!" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			
			DOM_Node destinationcomponentNode = parser->findChild(childNode, "destinationcomponent");
			if (destinationcomponentNode == NULL)
			{
				cout << "redirectrequest: fails in no destinationcomponent node" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			DOMString descomid = parser->getAttr(destinationcomponentNode, "idref");
			if (descomid == NULL)
			{
				cout << "redirectrequest: fails in destinationcomponent node no idref attribute!" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			
			ArchComponent* acom = arch->getArchComponent(srccomid.transcode());
			ArchHome_var ahome = arch->getArchHome_By_UUID(acom->homeuuid);
			if (acom == NULL)
			{
				cout << "redirectrequest: fails in invalid sourcecomponent node idref attribute!" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}
			delete acom;
			acom = NULL;			
			
			acom = arch->getArchComponent(descomid.transcode());
			if (acom == NULL)
			{
				cout << "redirectrequest: fails in invalid destinationcomponent node idref attribute!" << endl;
				throw STARCCM::DynDeployment::InvalidDrdFile();
			}

			iter = RedirectReqOldObjMap.find(string(ahome->uuid));
			if (iter != RedirectReqOldObjMap.end())
			{
				int len = (*iter).second->length();
				(*iter).second->length(len+1);
				(*iter).second[len] = CORBA::string_dup(srccomid.transcode());

				niter = RedirectReqNewObjMap.find(string(ahome->uuid));
				len = (*niter).second->length();
				(*niter).second->length(len+1);
				(*niter).second[len] = CORBA::Object::_duplicate(acom->comref);
			}
			else
			{
				CORBA::StringSeq_var oseq = new CORBA::StringSeq;
				oseq->length(1);
				oseq[0] = CORBA::string_dup(srccomid.transcode());
				RedirectReqOldObjMap.insert(MapOidSeq::value_type(string(ahome->uuid), oseq));

				::Components::ObjectSeq_var nseq = new ::Components::ObjectSeq;
				nseq->length(1);
				nseq[0] = CORBA::Object::_duplicate(acom->comref);
				RedirectReqNewObjMap.insert(MapObjSeq::value_type(string(ahome->uuid), nseq));
			}
			
			delete acom;
			acom = NULL;
		}
	}

	iter = RedirectReqOldObjMap.begin();
	while (iter != RedirectReqOldObjMap.end())
	{
		ArchHome_var ahome = arch->getArchHome_By_UUID((*iter).first.c_str());
		Container_var container = ::Components::Deployment::Container::_narrow(ahome->containerref);
		if (!CORBA::is_nil(container))
		{
			niter = RedirectReqNewObjMap.find((*iter).first.c_str());
			try
			{
				container->setRedirectReqInfo(ahome->uuid, (*iter).second, (*niter).second, 0);
			}
			catch(...)
			{
				cout << "redirectrequest: fails in setRedirectReqInfo!" << endl;
				throw;
			}
		}
		iter++;
	}
	
	if(verbose)
	{
		cout<<"redirectrequest: redirect request successfully!" << endl;
	}
}
