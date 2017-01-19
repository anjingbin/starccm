// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang ShuFeng    icestar_wang@163.com 
//
// **********************************************************************

// Version: 1.0.0

#include <Architecture.h>
#include <Deployment.h>
#include <fstream>

#include <XMLParser.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/util/TranscodingException.hpp>

#include <xercesc/dom/DOM_DOMException.hpp>
#include <xercesc/dom/DOM.hpp>

using namespace STARCCM;
using namespace STARCCM::Deployment;
using namespace Components::Deployment;

Architecture::Architecture(CORBA::ORB_ptr orb_, const char* id_, const char* rootDir)
	:orb(CORBA::ORB::_duplicate(orb_))
{
	assemblyUUID = CORBA::string_dup(id_);

	this->rootDir = CORBA::string_dup(rootDir);

	archFile = this->rootDir;
	archFile += CORBA::string_dup(PATH_DELILIMITOR);
	archFile += assemblyUUID;
	archFile += CORBA::string_dup(PATH_DELILIMITOR);
	archFile += CORBA::string_dup("Architecture.xml");

	relationFile = this->rootDir;
	relationFile += CORBA::string_dup(PATH_DELILIMITOR);
	relationFile += assemblyUUID;
	relationFile += CORBA::string_dup(PATH_DELILIMITOR);
	relationFile += CORBA::string_dup("componentRelation.xml");
	
}

Architecture::~Architecture()
{
	aHomeMap.clear();
	aComponentMap.clear();
	aConnectionMap.clear();
	containerMap.clear();
}

void 
Architecture::addArchHome(ArchHome& ahome)
{
	MapArchHome::iterator home_iter = aHomeMap.find(string(ahome.uuid));
	if (home_iter != aHomeMap.end())
	{
		cout << "The ArchHome " << ahome.id << " has existed!" << endl;
		return;
	}
	
	aHomeMap.insert(MapArchHome::value_type(string(ahome.uuid),ahome));

	MapContainer::iterator container_iter = containerMap.find(string(ahome.containerid));
	if (container_iter == containerMap.end())
	{
		ContainerInfo cinfo;
		cinfo.ref = CORBA::Object::_duplicate(ahome.containerref);
		int len = cinfo.homeUUIDs.length();
		cinfo.homeUUIDs.length(len+1);
		cinfo.homeUUIDs[len] = ahome.uuid;
		containerMap.insert(MapContainer::value_type(string(ahome.containerid), cinfo));
	}
	else
	{
		int len = (*container_iter).second.homeUUIDs.length();
		(*container_iter).second.homeUUIDs.length(len+1);
		(*container_iter).second.homeUUIDs[len] = ahome.uuid; 
	}
}

void 
Architecture::delArchHome(const char* homeuuid)
{
	if (homeuuid == NULL)
	{
		cout  << "The home uuid is NULL!" << endl;
		return;
	}
		
	MapArchHome::iterator home_iter = aHomeMap.find(string(homeuuid));
	MapArchHome::iterator home_iter_end = aHomeMap.end();
		
	if(home_iter ==home_iter_end)
	{
		cout << "ArchHome " << homeuuid << " doesn't exist!" << endl;
		return;
	}

	int len = (*home_iter).second.coms.length();
	for (int i=0; i<len; i++)
	{
		MapArchComponent::iterator com_iter=aComponentMap.find(string((*home_iter).second.coms[i]));
		aComponentMap.erase(com_iter);
	}

	CORBA::String_var containerid = (*home_iter).second.containerid;
	aHomeMap.erase(home_iter);

	home_iter = aHomeMap.begin();
	while(home_iter !=home_iter_end)
	{
		if(strcmp((*home_iter).second.containerid, containerid) == 0)
			break;
		home_iter++;
	}
	if(home_iter ==home_iter_end)
	{
		MapContainer::iterator container_iter = containerMap.find(containerid.in());
		containerMap.erase(container_iter);
	}
}

ArchHomes* 
Architecture::getAllArchHomes()
{
	ArchHomes tmphomes;
	int len; 
	
	MapArchHome::iterator home_iter = aHomeMap.begin();
	MapArchHome::iterator home_iter_end = aHomeMap.end();
	while(home_iter !=home_iter_end)
	{
		len = tmphomes.length();
		tmphomes.length(len+1);
		tmphomes[len] = (*home_iter).second;
	}

	ArchHomes* ahomes = new ArchHomes(tmphomes);
	return ahomes;
}

ArchHome* 
Architecture::getArchHome(const char* homeid)
{
	MapArchHome::iterator home_iter = aHomeMap.begin();
	MapArchHome::iterator home_iter_end = aHomeMap.end();
	while(home_iter !=home_iter_end)
	{
		if(strcmp((*home_iter).second.id, homeid) == 0)
			break;
		home_iter++;
	}

	if(home_iter ==home_iter_end)
	{
		cout << "ArchHome " << homeid << " doesn't exist!" << endl;
		return NULL;
	}

	ArchHome* ahome = new ArchHome((*home_iter).second);

	return ahome;
}

ArchHome* 
Architecture::getArchHome_By_UUID(const char* uuid)
{
	MapArchHome::iterator home_iter = aHomeMap.find(string(uuid));
	if(home_iter ==aHomeMap.end())
	{
		cout << "ArchHome " << uuid << " doesn't exist!" << endl;
		return NULL;
	}
	
	ArchHome* ahome = new ArchHome((*home_iter).second);

	return ahome;
}
	
void 
Architecture::addArchComponent(ArchComponent& acom)
{
	MapArchComponent::iterator com_iter = aComponentMap.find(string(acom.uuid));
	if (com_iter != aComponentMap.end())
	{
		cout << "The ArchComponent " << acom.id << " has existed!" << endl;
		return;
	}
	
	aComponentMap.insert(MapArchComponent::value_type(string(acom.uuid), acom));

	MapArchHome::iterator home_iter = aHomeMap.find(string(acom.homeuuid));
	int len = (*home_iter).second.coms.length();
	(*home_iter).second.coms.length(len+1);
	(*home_iter).second.coms[len] = acom.uuid;
}

void 
Architecture::delArchComponent(const char* comuuid)
{
	if (comuuid == NULL)
	{
		cout << "The component uuid is NULL!" << endl;
		return;
	}

	MapArchComponent::iterator com_iter=aComponentMap.find(string(comuuid));	
	MapArchComponent::iterator com_iter_end=aComponentMap.end();

	if(com_iter == com_iter_end)
	{
		cout << "ArchComponent " << comuuid << " doesn't exist!" << endl;
		return;
	}

	MapArchHome::iterator home_iter = aHomeMap.find(string((*com_iter).second.homeuuid));
	int len = (*home_iter).second.coms.length();
	
	int i;
	for (i=0; i<len; i++)
	{
		if(strcmp((*home_iter).second.coms[i], (*com_iter).first.c_str()) == 0)
			break;
	}
	
	int j = i;
	for(j=i; j<len-1; j++)
	{
		(*home_iter).second.coms[j] = (*home_iter).second.coms[j+1];
	}
	(*home_iter).second.coms.length(len-1);

	aComponentMap.erase(com_iter);
}

ArchComponents* 
Architecture::getAllArchComponents()
{
	ArchComponents tmpcoms;
	int len; 
	
	MapArchComponent::iterator com_iter=aComponentMap.begin();	
	MapArchComponent::iterator com_iter_end=aComponentMap.end();
	while(com_iter!=com_iter_end)
	{
		len = tmpcoms.length();
		tmpcoms.length(len+1);
		tmpcoms[len] = (*com_iter).second;
	}

	ArchComponents* acoms = new ArchComponents(tmpcoms);
	return acoms;
}

ArchComponent* 
Architecture::getArchComponent(const char* comid)
{
	MapArchComponent::iterator com_iter=aComponentMap.begin();	
	MapArchComponent::iterator com_iter_end=aComponentMap.end();
	while(com_iter!=com_iter_end)
	{
		if (strcmp((*com_iter).second.id, comid) == 0)
			break;
		com_iter++;
	}

	if(com_iter == com_iter_end)
	{
		cout << "ArchComponent " << comid << " doesn't exist!" << endl;
		return NULL;
	}

	ArchComponent* acom = new ArchComponent((*com_iter).second);

	return acom;
}

ArchComponent* 
Architecture::getArchComponent_By_UUID(const char* uuid)
{
	MapArchComponent::iterator com_iter = aComponentMap.find(string(uuid));
	if(com_iter ==aComponentMap.end())
	{
		cout << "ArchComponent " << uuid << " doesn't exist!" << endl;
		return NULL;
	}
	
	ArchComponent* acom = new ArchComponent((*com_iter).second);

	return acom;
}

void 
Architecture::addArchConnection(ArchConnection& aconn)
{
	MapArchConnection::iterator conn_iter = aConnectionMap.find(string(aconn.cookie));
	if (conn_iter != aConnectionMap.end())
	{
		cout << "The ArchConnection " << aconn.id << " has existed!" << endl;
		return;
	}
	
	MapArchComponent::iterator srccom_iter = aComponentMap.find(string(aconn.srccomuuid));
	aconn.srccomref = CORBA::Object::_duplicate((*srccom_iter).second.comref.in());
	
	int len = (*srccom_iter).second.links.length();
	(*srccom_iter).second.links.length(len+1);
	(*srccom_iter).second.links[len] = aconn.cookie;
	
	if (strcmp(aconn.targettype, "home") == 0)
	{
		MapArchHome::iterator home_iter = aHomeMap.begin();
		MapArchHome::iterator home_iter_end = aHomeMap.end();
		while (home_iter != home_iter_end)
		{
			if (aconn.targetref -> _is_equivalent((*home_iter).second.homeref.in()))
				break;
			home_iter++;
		}
		aconn.targetuuid = (*home_iter).second.uuid;
		
		len = (*home_iter).second.links.length();
		(*home_iter).second.links.length(len+1);
		(*home_iter).second.links[len] = aconn.cookie;
	}
	
	if (strcmp(aconn.targettype, "supports") == 0)
	{
		MapArchComponent::iterator com_iter=aComponentMap.begin();	
		MapArchComponent::iterator com_iter_end=aComponentMap.end();
		while(com_iter!=com_iter_end)
		{
			if (aconn.targetref -> _is_equivalent((*com_iter).second.comref.in()))
				break;
			com_iter++;
		}
		aconn.targetuuid = (*com_iter).second.uuid;
		aconn.targetport = CORBA::string_dup("supports");
		
		len = (*com_iter).second.links.length();
		(*com_iter).second.links.length(len+1);
		(*com_iter).second.links[len] = aconn.cookie;
	}

	if ((strcmp(aconn.targettype, "facet") == 0) || (strcmp(aconn.targettype, "consumer") == 0))
	{
		MapArchComponent::iterator com_iter=aComponentMap.begin();	
		MapArchComponent::iterator com_iter_end=aComponentMap.end();	
		while(com_iter!=com_iter_end)
		{
			int plen = (*com_iter).second.ports.length();
			
			int i;
			for (i=0; i<plen; i++)
			{
				if (aconn.targetref -> _is_equivalent((*com_iter).second.ports[i].reference))
				{
					aconn.targetport = (*com_iter).second.ports[i].name;
					break;
				}
			}
				
			if (i < plen)
				break;

			com_iter++;
		}
		aconn.targetuuid = (*com_iter).second.uuid;

		len = (*com_iter).second.links.length();
		(*com_iter).second.links.length(len+1);
		(*com_iter).second.links[len] = aconn.cookie;
	}

	aConnectionMap.insert(MapArchConnection::value_type(string(aconn.cookie), aconn));
}

void 
Architecture::delArchConnection(const char* cookie)
{
	if (cookie == NULL)
	{
		cout << "The connection's cookie is NULL!" << endl;
		return;
	}
	
	MapArchConnection::iterator conn_iter=aConnectionMap.find(string(cookie));	
	MapArchConnection::iterator conn_iter_end=aConnectionMap.end();

	if(conn_iter == conn_iter_end)
	{
		cout << "ArchConnection " << cookie << " doesn't exist!" << endl;
		return;
	}

	MapArchComponent::iterator com_iter = aComponentMap.find(string((*conn_iter).second.srccomuuid));
	int len = (*com_iter).second.links.length();
	
	int i;
	for (i=0; i<len; i++)
	{
		if(strcmp((*com_iter).second.links[i], (*conn_iter).first.c_str()) == 0)
			break;
	}
	
	int j;
	for(j=i; j<len-1; j++)
	{
		(*com_iter).second.links[j] = (*com_iter).second.links[j+1];
	}
	(*com_iter).second.links.length(len-1);

	if (strcmp((*conn_iter).second.targettype, "home") == 0)
	{
		MapArchHome::iterator home_iter = aHomeMap.find(string((*conn_iter).second.targetuuid));
		if (home_iter != aHomeMap.end())
		{
			len = (*home_iter).second.links.length();
			for (i=0; i<len; i++)
			{
				if(strcmp((*home_iter).second.links[i], (*com_iter).first.c_str()) == 0)
					break;
			}
			for(j=i; j<len-1; j++)
			{
				(*home_iter).second.links[j] = (*home_iter).second.links[j+1];
			}
			(*home_iter).second.links.length(len-1);
		}
	}
	else
	{
		com_iter = aComponentMap.find(string((*conn_iter).second.targetuuid));
		if (com_iter != aComponentMap.end())
		{
			len = (*com_iter).second.links.length();
			for (i=0; i<len; i++)
			{
				if(strcmp((*com_iter).second.links[i], (*conn_iter).first.c_str()) == 0)
					break;
			}
			for(j=i; j<len-1; j++)
			{
				(*com_iter).second.links[j] = (*com_iter).second.links[j+1];
			}
			(*com_iter).second.links.length(len-1);
		}
	}

	aConnectionMap.erase(conn_iter);
}

ArchConnections* 
Architecture::getAllArchConnections()
{
	ArchConnections tmpconns;
	int len; 
	
	MapArchConnection::iterator conn_iter = aConnectionMap.begin();	
	MapArchConnection::iterator conn_iter_end = aConnectionMap.end();
	while(conn_iter!=conn_iter_end)
	{
		len = tmpconns.length();
		tmpconns.length(len+1);
		tmpconns[len] = (*conn_iter).second;
	}

	ArchConnections* aconns = new ArchConnections(tmpconns);
	return aconns;
}

ArchConnection* 
Architecture::getArchConnection(const char* connid)
{
	MapArchConnection::iterator conn_iter = aConnectionMap.begin();	
	MapArchConnection::iterator conn_iter_end = aConnectionMap.end();
	while(conn_iter!=conn_iter_end)
	{
		if (strcmp((*conn_iter).second.id, connid) == 0)
			break;
		conn_iter++;
	}

	if(conn_iter == conn_iter_end)
	{
		cout << "ArchConnection " << connid << " doesn't exist!" << endl;
		return NULL;
	}

	ArchConnection* aconn = new ArchConnection((*conn_iter).second);

	return aconn;
}

ArchConnection* 
Architecture::getArchConnection_By_Cookie(const char* ck)
{
	MapArchConnection::iterator conn_iter = aConnectionMap.find(ck);
	if(conn_iter == aConnectionMap.end())
	{
		cout << "ArchConnection " << ck << " doesn't exist!" << endl;
		return NULL;
	}
	ArchConnection* aconn = new ArchConnection((*conn_iter).second);

	return aconn;
}
	
void 
Architecture::update()
{
	MapContainer::iterator container_iter = containerMap.begin();
	MapContainer::iterator container_iter_end = containerMap.end();
	while (container_iter != container_iter_end)
	{
		CORBA::Object_var obj = CORBA::Object::_duplicate((*container_iter).second.ref);
		Container_var container = Container::_narrow(obj);

		ArchComponents_var acoms;
		DeletedComs_var dcoms;
		ArchConnections_var aconns;
		ArchConnections_var dconns;

		container->updateArchitecture((*container_iter).second.homeUUIDs, acoms.out(), dcoms.out(), aconns.out(), dconns.out());

		int len;
		int i;
		//must delete archconnections info before add archconnections info
		//becuse in redirectconnections same cookies will turn out in dconns and aconns
		len = dconns->length();
		for (i=0; i<len; i++)
		{
			MapArchConnection::iterator conn_iter;
			if (dconns[i].cookie != NULL)
			{
				conn_iter = aConnectionMap.find(string(dconns[i].cookie));
			}
			else
			{
				conn_iter = aConnectionMap.begin();
				while (conn_iter != aConnectionMap.end())
				{
					if ((strcmp((*conn_iter).second.srccomuuid, dconns[i].srccomuuid) == 0)
						&& (strcmp((*conn_iter).second.srcport, dconns[i].srcport) == 0))
						break;
				}
			}

			if (conn_iter != aConnectionMap.end())
			{
				delArchConnection((*conn_iter).second.cookie);
			}
		}

		len = dcoms->length();
		for (i=0; i<len; i++)
		{
			MapArchComponent::iterator com_iter=aComponentMap.find(string(dcoms[i]));
			if (com_iter != aComponentMap.end())
			{
				delArchComponent((*com_iter).second.uuid);
			}
		}

		len = acoms->length();
		for (i=0; i<len; i++)
			addArchComponent(acoms[i]);

		len = aconns->length();
		for (i=0; i<len; i++)
			addArchConnection(aconns[i]);

		container_iter++;
	}

	printToFile();
	printToRelationFile();
	
}

void
Architecture::clean()
{
	MapContainer::iterator container_iter = containerMap.begin();
	MapContainer::iterator container_iter_end = containerMap.end();
	while (container_iter != container_iter_end)
	{
		CORBA::Object_var obj = CORBA::Object::_duplicate((*container_iter).second.ref);
		Container_var container = Container::_narrow(obj);

		container->cleanArchObjects((*container_iter).second.homeUUIDs);

		container_iter++;
	}
}

void
Architecture::printToFile()
{
	ofstream outfile(archFile.in());
	if(!outfile)
	{
		cout<<"Can not create the Architecture.xml ."<<endl;
		return;
	}

	//input the file header
	outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	//outfile << "<!DOCTYPE DynReconfig PUBLIC \"\" \"AssemblyArch.dtd\">" << endl;
	outfile << "<assembly idref=\"" << assemblyUUID << "\">" << endl;

	//for homes
	outfile << "	<homes>" << endl;

	MapArchHome::iterator home_iter = aHomeMap.begin();
	while (home_iter != aHomeMap.end())
	{
		outfile << "		<home id=\"" << (*home_iter).second.id << "\" >" << endl;
		outfile << "			<uuid>" << (*home_iter).second.uuid <<"</uuid>" << endl;
		
		CORBA::String_var s = orb->object_to_string((*home_iter).second.homeref);
		outfile << "			<homeref>" << s.in() << "</homeref>" << endl;
		outfile << "			<type>" << (*home_iter).second.type << "</type>" << endl;
		outfile << "			<homerepositoryid>" << (*home_iter).second.homerepid << "</homerepositoryid>" << endl;
		outfile << "			<componentrepositoryid>" << (*home_iter).second.componentrepid << "</componentrepositoryid>" << endl;

		s = orb->object_to_string((*home_iter).second.serveractivatorref);
		outfile << "			<serveractivator idref=\"" << (*home_iter).second.serveractivatorid << "\" ref=\"" <<
			s.in() << "\" />" << endl;

		s = orb->object_to_string((*home_iter).second.componentserverref);
		outfile << "			<componentserver idref=\"" << (*home_iter).second.componentserverid << "\" ref=\"" <<
			s.in() << "\" />" << endl;

		s = orb->object_to_string((*home_iter).second.containerref);
		outfile << "			<container idref=\"" << (*home_iter).second.containerid << "\" ref=\"" <<
			s.in() << "\" />" << endl;

		outfile << "			<componentinstantiations>" << endl;

		int len = (*home_iter).second.coms.length();				
		int i;
		for (i=0; i<len; i++)
		{
			CORBA::String_var cuuid = (*home_iter).second.coms[i];
			ArchComponent_var acom = getArchComponent_By_UUID(cuuid.in());
			s = orb->object_to_string(acom->comref);			
			outfile<< "				<componentinstantiation idref=\"" << acom->id << "\" ref=\"" <<
				s.in() << "\" />" << endl;
		}
		outfile << "			</componentinstantiations>" << endl;

		outfile << "			<links>" << endl;
		len = (*home_iter).second.links.length();
		for (i=0; i<len; i++)
		{
			CORBA::String_var ck = (*home_iter).second.links[i];
			ArchConnection_var aconn = getArchConnection_By_Cookie(ck.in());
			outfile << "				<astarget idref=\"" << aconn->id << "\" cookie=\"" << aconn->cookie <<
				"\" >" << endl;
			outfile << "					<connecttype>connectinterface</connecttype>" << endl;
			s = orb->object_to_string(aconn->srccomref);
			outfile << "					<srccomref>" << s.in() << "</srccomref>" << endl;
			outfile << "					<srcport>" << aconn->srcport << "</srcport>" << endl;
			outfile << "				</astarget>" << endl;
		}
		outfile <<"			</links>" << endl;

		outfile << "			<boundable set=\"" << (*home_iter).second.isboundable <<"\" />" << endl;

		outfile << "			<componentboundaryports>" << endl;
		len = (*home_iter).second.componentboundaryports.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<port name=\"" << (*home_iter).second.componentboundaryports[i] 
				<< "\" />" << endl;
		}
		outfile << "			</componentboundaryports>" << endl;
		
		outfile << "			<crypticconnections>" << endl;
		len = (*home_iter).second.crypticconnections.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<crypticconnection>" << endl;
			outfile << "					<port name=\"" << (*home_iter).second.crypticconnections[i].srcport
				<< "\" />"<< endl;
			outfile << "					<targetports>" << endl;
			int plen = (*home_iter).second.crypticconnections[i].targetports.length();
			for (int p=0; p<plen; p++)
			{
				outfile << "					<port name=\"" << (*home_iter).second.crypticconnections[p].targetports[p]
					<< "\" />" << endl;
			}
			outfile << "					</targetports>" << endl;	
			outfile << "				</crypticconnection>" << endl;
		}
		outfile << "			</crypticconnections>" << endl;

		outfile << "			<initialrequestports>" << endl;
		len = (*home_iter).second.initialreqports.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<port name=\"" << (*home_iter).second.initialreqports[i] << "\" />" << endl;
		}
		outfile << "			</initialrequestports>" << endl;

		outfile << "			<homeinitiative set=\"" << (*home_iter).second.homeinitiative << "\" />" << endl;

		outfile << "			<componentinitiative set=\"" << (*home_iter).second.componentinitiative << "\" />" << endl;

		outfile << "			<runcomplete set=\"" << (*home_iter).second.runcomplete << "\" >" << endl;
		outfile << "				<usesports>" << endl;
		len = (*home_iter).second.runcompleteusesports.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<port name=\"" << (*home_iter).second.runcompleteusesports[i] << "\" />" << endl;
		}
		outfile << "				</usesports>" << endl;
		outfile << "			</runcomplete>" << endl;

		outfile << "			<configurationcomplete set=\"" << (*home_iter).second.configurationcomplete << "\" >" << endl;
		outfile << "				<usesports>" << endl;
		len = (*home_iter).second.configurationcompleteusesports.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<port name=\"" << (*home_iter).second.configurationcompleteusesports[i] << "\" />" << endl;
		}
		outfile << "				</usesports>" << endl;
		outfile << "			</configurationcomplete>" << endl;
		
		s = orb->object_to_string((*home_iter).second.homefinder);
		outfile << "			<homefinder ref=\"" << s.in() << "\" />" << endl;

		outfile << "			<withhomefinder>" << endl;
		len = (*home_iter).second.withhomefinder.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<registername>" << (*home_iter).second.withhomefinder[i] 
				<< "</registername>" << endl;
		}
		outfile << "			</withhomefinder>" << endl;

		s = orb->object_to_string((*home_iter).second.nameservice);
		outfile << "			<nameservice ref=\"" << s.in() << "\" />" << endl;

		outfile << "			<withnaming>" << endl;
		len = (*home_iter).second.withnaming.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<registername>" << (*home_iter).second.withnaming[i] 
				<< "</registername>" << endl;
		}
		outfile << "			</withnaming>" << endl;	
		
		outfile << "		</home>" << endl;

		home_iter++;
	}

	outfile << "	</homes>" << endl;

	//for components
	outfile << "	<components>" << endl;

	MapArchComponent::iterator acom_iter = aComponentMap.begin();
	while (acom_iter != aComponentMap.end())
	{
		outfile << "		<component id=\"" << (*acom_iter).second.id << "\" >" << endl;
		outfile << "			<uuid>" << (*acom_iter).second.uuid << "</uuid>" << endl;
		CORBA::String_var s = orb->object_to_string((*acom_iter).second.comref);
		outfile << "			<comref>" << s.in() << "</comref>" << endl;
		ArchHome_var ahome = getArchHome_By_UUID((*acom_iter).second.homeuuid);
		s = orb->object_to_string(ahome->homeref);
		outfile << "			<homeplacement idref=\"" << ahome->id << "\" homeref=\"" << s.in() << "\" />" << endl;
		
		outfile << "			<ports>" << endl;
		int len = (*acom_iter).second.ports.length();
		PortSeq ports = (*acom_iter).second.ports;
		
		int i;
		for (i=0; i<len; i++)
		{
			if (strcmp(ports[i].type, "providesport") == 0)
			{
				s = orb->object_to_string(ports[i].reference);
				outfile << "				<providesport name=\"" << ports[i].name << "\" facetref=\"" <<
					s.in() << "\" />" << endl;
			}
			else
			{
				s = orb->object_to_string(ports[i].reference);
				outfile << "				<consumesport name=\"" << ports[i].name << "\" consumerref=\"" <<
					s.in() << "\" />" << endl;
			}
		}
		outfile << "			</ports>" << endl;
		
		outfile << "			<links>" << endl;
		len = (*acom_iter).second.links.length();
		for (i=0; i<len; i++)
		{
			CORBA::String_var ck = (*acom_iter).second.links[i];
			ArchConnection_var aconn = getArchConnection_By_Cookie(ck.in());
			if (strcmp(aconn->srccomuuid, (*acom_iter).second.uuid) ==0)
			{
				outfile << "				<assender idref=\"" << aconn->id << "\" cookie=\"" << aconn->cookie <<
					"\" >" << endl;
				if (strcmp(aconn->connectmode, "connect") == 0)
					outfile << "					<connecttype>connectinterface</connecttype>" << endl;
				else
					outfile << "					<connecttype>connectevent</connecttype>" << endl;
				s = orb->object_to_string(aconn->targetref);
				outfile << "					<targetref>" << s.in() << "</targetref>" << endl;
				outfile << "					<targetport>" << aconn->targetport << "</targetport>" << endl;
				outfile << "				</assender>" << endl;				
			}
			else
			{
				outfile << "				<astarget idref=\"" << aconn->id << "\" cookie=\"" << aconn->cookie <<
					"\" >" << endl;
				if (strcmp(aconn->connectmode, "connect") == 0)
					outfile << "					<connecttype>connectinterface</connecttype>" << endl;
				else
					outfile << "					<connecttype>connectevent</connecttype>" << endl;
				s = orb->object_to_string(aconn->srccomref);
				outfile << "					<srccomref>" << s.in() << "</srccomref>" << endl;
				outfile << "					<srcport>" << aconn->srcport << "</srcport>" << endl;
				outfile << "				</astarget>" << endl;
			}
		}
		outfile << "			</links>" << endl;

		outfile << "			<boundable set=\"" << (*acom_iter).second.isboundable << "\" />" << endl;

		outfile << "			<boundaryports>" << endl;
		len = (*acom_iter).second.boundaryports.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<port name=\"" << (*acom_iter).second.boundaryports[i] 
				<< "\" />" << endl;
		}
		outfile << "			</boundaryports>" << endl;
		
		s = orb->object_to_string((*acom_iter).second.nameservice);
		outfile << "			<nameservice ref=\"" << s.in() << "\" />" << endl;

		outfile << "			<withnaming>" << endl;
		len = (*acom_iter).second.withnaming.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<registername>" << (*acom_iter).second.withnaming[i] 
				<< "</registername>" << endl;
		}
		outfile << "			</withnaming>" << endl;

		outfile << "			<portwithnamings>" << endl;
		len = (*acom_iter).second.portwithnamings.length();
		for (i=0; i<len; i++)
		{
			outfile << "				<portwithnaming>" << endl;
			outfile << "					<port name=\"" << (*acom_iter).second.portwithnamings[i].portname
				<< "\" />" << endl;
			int wlen = (*acom_iter).second.portwithnamings[i].withnaming.length();
			for (int w=0; w<wlen; w++)
			{
				outfile << "					<registername>" << (*acom_iter).second.portwithnamings[i].withnaming[w]
					<< "</registername>" << endl;
			}
			outfile << "				</portwithnaming>" << endl;
		}
		outfile << "			</portwithnamings>" << endl;		
				
		outfile << "		</component>" << endl;

		acom_iter++;
	}

	outfile << "	</components>" << endl;

	//for connections
	outfile << "	<connections>" << endl;

	MapArchConnection::iterator aconn_iter = aConnectionMap.begin();
	while (aconn_iter != aConnectionMap.end())
	{
		outfile << "		<connection id=\"" << (*aconn_iter).second.id << "\" >" << endl;
		outfile << "			<cookie>" << (*aconn_iter).second.cookie << "</cookie>" << endl;
		if (strcmp((*aconn_iter).second.connectmode, "connect") == 0)
			outfile << "			<connecttype>connectinterface</connecttype>" << endl;
		else
			outfile << "			<connecttype>connectevent</connecttype>" << endl;
		ArchComponent_var acom = getArchComponent_By_UUID((*aconn_iter).second.srccomuuid);
		outfile << "			<srccomid>" << acom->id << "</srccomid>" << endl;
		CORBA::String_var s = orb->object_to_string((*aconn_iter).second.srccomref);
		outfile << "			<srccomref>" << s.in() << "</srccomref>" << endl;
		outfile << "			<srcport>" << (*aconn_iter).second.srcport << "</srcport>" << endl;
		if (strcmp((*aconn_iter).second.targettype, "home") == 0)
		{
			ArchHome_var ahome = getArchHome_By_UUID((*aconn_iter).second.targetuuid);
			outfile << "			<targetid>" << ahome->id << "</targetid>" << endl;
		}
		else
		{
			acom = getArchComponent_By_UUID((*aconn_iter).second.targetuuid);
			outfile << "			<targetid>" << acom->id << "</targetid>" << endl;
		}
		s = orb->object_to_string((*aconn_iter).second.targetref);
		outfile << "			<targetref>" << s.in() << "</targetref>" << endl;
		outfile << "			<targetport>" << (*aconn_iter).second.targetport << "</targetport>" << endl;
		outfile << "		</connection>" << endl;

		aconn_iter++;
	}

	outfile << "	</connections>" << endl;
	
	outfile << "</assembly>" << endl;
	outfile.close();
}

void 
Architecture::initialize(const char* filename)
{
	aHomeMap.clear();
	aComponentMap.clear();
	aConnectionMap.clear();
	
	XMLParser* parser = new XMLParser();

  	try
  	{
  		parser->initialize(filename);
  	}
  	catch (const STARCCM::Deployment::XMLInitializeFailed &)
	{
		TRACE1(LEVEL1,"initialize: connot initialize Architeture Object by  \n",filename);	
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}	
		
		return;
	}    

	DOM_Node root = parser->getRootNode("assembly");
	if(root==NULL)
	{
		TRACE0(LEVEL1,"initialize: invalid xml file: miss assembly element. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		return;
	}

	//for homes
	DOM_Node homesNode = parser->findChild(root, "homes");
	if(homesNode==NULL)
	{
		TRACE0(LEVEL1,"initialize: invalid xml file: miss homes element. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		return;
	}

	DOM_NodeList homesList = homesNode.getChildNodes();
	DOM_Node childNode;
	DOMString childNodeName;

	int i;
	for (i=0; i<homesList.getLength(); i++)
	{
		childNode = homesList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("home"))
		{
			ArchHome_var ahome = new ArchHome;
			DOMString id = parser->getAttr(childNode, "id");
			ahome->id = CORBA::string_dup(id.transcode());
			
			DOM_Node uuidNode = parser->findChild(childNode, "uuid");
			if (uuidNode != NULL)
			{
				DOM_Node uuidText = uuidNode.getFirstChild();
				DOMString uuid = uuidText.getNodeValue();
				ahome->uuid = CORBA::string_dup(uuid.transcode());
			}

			DOM_Node homerefNode = parser->findChild(childNode, "homeref");
			if (homerefNode != NULL)
			{
				DOM_Node homerefText = homerefNode.getFirstChild();
				DOMString homeref = homerefText.getNodeValue();
				CORBA::Object_var obj = orb->string_to_object(homeref.transcode());
				ahome->homeref = CORBA::Object::_duplicate(obj);
			}

			DOM_Node typeNode = parser->findChild(childNode, "type");
			if (typeNode != NULL)
			{
				DOM_Node typeText = typeNode.getFirstChild();
				DOMString type = typeText.getNodeValue();
				ahome->type = CORBA::string_dup(type.transcode());
			}

			DOM_Node homerepositoryidNode = parser->findChild(childNode, "homerepositoryid");
			if (homerepositoryidNode != NULL)
			{
				DOM_Node homerepositoryidText = homerepositoryidNode.getFirstChild();
				DOMString homerepositoryid = homerepositoryidText.getNodeValue();
				ahome->homerepid = CORBA::string_dup(homerepositoryid.transcode());
			}

			DOM_Node componentrepositoryidNode = parser->findChild(childNode, "componentrepositoryid");
			if (componentrepositoryidNode != NULL)
			{
				DOM_Node componentrepositoryidText = componentrepositoryidNode.getFirstChild();
				DOMString componentrepositoryid = componentrepositoryidText.getNodeValue();
				ahome->componentrepid = CORBA::string_dup(componentrepositoryid.transcode());
			}

			DOM_Node serveractivatorNode = parser->findChild(childNode, "serveractivator");
			if (serveractivatorNode != NULL)
			{
				DOMString idref = parser->getAttr(serveractivatorNode, "idref");
				ahome->serveractivatorid = CORBA::string_dup(idref.transcode());
				DOMString ref = parser->getAttr(serveractivatorNode, "ref");
				CORBA::Object_var obj = orb->string_to_object(ref.transcode());
				ahome->serveractivatorref = CORBA::Object::_duplicate(obj);
			}

			DOM_Node componentserverNode = parser->findChild(childNode, "componentserver");
			if (componentserverNode != NULL)
			{
				DOMString idref = parser->getAttr(componentserverNode, "idref");
				ahome->componentserverid = CORBA::string_dup(idref.transcode());
				DOMString ref = parser->getAttr(componentserverNode, "ref");
				CORBA::Object_var obj = orb->string_to_object(ref.transcode());
				ahome->componentserverref = CORBA::Object::_duplicate(obj);
			}

			DOM_Node containerNode = parser->findChild(childNode, "container");
			if (containerNode != NULL)
			{
				DOMString idref = parser->getAttr(containerNode, "idref");
				ahome->containerid = CORBA::string_dup(idref.transcode());
				DOMString ref = parser->getAttr(containerNode, "ref");
				CORBA::Object_var obj = orb->string_to_object(ref.transcode());
				ahome->containerref = CORBA::Object::_duplicate(obj);
			}

			DOM_Node componentinstantiationsNode = parser->findChild(childNode, "componentinstantiations");
			if (componentinstantiationsNode != NULL)
			{
				DOM_NodeList comsList = componentinstantiationsNode.getChildNodes();
				DOM_Node comsChildNode;
				DOMString comsChildNodeName;
				
				int j;
				for (j=0; j<comsList.getLength(); j++)
				{
					comsChildNode = comsList.item(j);
					comsChildNodeName = comsChildNode.getNodeName();

					if (comsChildNodeName.equals("componentinstantiation"))
					{
						DOMString idref = parser->getAttr(comsChildNode, "idref");
						int len = ahome->coms.length();
						ahome->coms.length(len+1);
						ahome->coms[len] = CORBA::string_dup(idref.transcode());
						//the id will be turn to uuid next
					}
				}
			}
			
			DOM_Node linksNode = parser->findChild(childNode, "links");
			if (linksNode != NULL)
			{
				DOM_NodeList linksList = linksNode.getChildNodes();
				DOM_Node linksChildNode;
				DOMString linksChildNodeName;

				int j;
				for (j=0; j<linksList.getLength(); j++)
				{
					linksChildNode = linksList.item(j);
					linksChildNodeName = linksChildNode.getNodeName();

					if (linksChildNodeName.equals("astarget"))
					{
						DOMString ck = parser->getAttr(linksChildNode, "cookie");
						int len = ahome->links.length();
						ahome->links.length(len+1);
						ahome->links[len] = CORBA::string_dup(ck.transcode());
					}
				}
			}

			DOM_Node boundableNode = parser->findChild(childNode, "boundable");
			if (boundableNode != NULL)
			{
				DOMString set = parser->getAttr(boundableNode, "set");
				ahome->isboundable = CORBA::string_dup(set.transcode());
			}

			DOM_Node componentboundaryportsNode = parser->findChild(childNode, "componentboundaryports");
			if (componentboundaryportsNode != NULL)
			{
				DOM_NodeList componentboundaryportsList = componentboundaryportsNode.getChildNodes();
				DOM_Node componentboundaryportsChildNode;
				DOMString componentboundaryportsChildNodeName;

				int j;
				for (j=0; j<componentboundaryportsList.getLength(); j++)
				{
					componentboundaryportsChildNode = componentboundaryportsList.item(j);
					componentboundaryportsChildNodeName = componentboundaryportsChildNode.getNodeName();

					if (componentboundaryportsChildNodeName.equals("port"))
					{
						DOMString name = parser->getAttr(componentboundaryportsChildNode, "name");
						int len = ahome->componentboundaryports.length();
						ahome->componentboundaryports.length(len+1);
						ahome->componentboundaryports[len] = CORBA::string_dup(name.transcode());
					}
				}
			}

			DOM_Node crypticconnectionsNode = parser->findChild(childNode, "crypticconnections");
			if (crypticconnectionsNode != NULL)
			{
				DOM_NodeList crypticconnectionsList = crypticconnectionsNode.getChildNodes();
				DOM_Node crypticconnectionsChildNode;
				DOMString crypticconnectionsChildNodeName;

				int j;
				for (j=0; j<crypticconnectionsList.getLength(); j++)
				{
					crypticconnectionsChildNode = crypticconnectionsList.item(j);
					crypticconnectionsChildNodeName = crypticconnectionsChildNode.getNodeName();

					if (crypticconnectionsChildNodeName.equals("crypticconnection"))
					{
						DOM_Node portNode = parser->findChild(crypticconnectionsChildNode, "port");
						DOMString srcportname = parser->getAttr(portNode, "name");
						CrypticConnection_var cconn = new CrypticConnection;
						cconn->srcport = CORBA::string_dup(srcportname.transcode());

						DOM_Node targetportsNode = parser->findChild(crypticconnectionsChildNode, "targetports");

						DOM_NodeList tportsChildList = targetportsNode.getChildNodes();
						DOM_Node tportsChildNode;
						DOMString tportsChildNodeName;

						int t;
						for (t=0; t<tportsChildList.getLength(); t++)
						{
							tportsChildNode = tportsChildList.item(t);
							tportsChildNodeName = tportsChildNode.getNodeName();

							if (tportsChildNodeName.equals("port"))
							{
								DOMString targetportname = parser->getAttr(tportsChildNode, "name");
								int clen = cconn->targetports.length();
								cconn->targetports.length(clen+1);
								cconn->targetports[clen] = CORBA::string_dup(targetportname.transcode());
							}
						}

						int len = ahome->crypticconnections.length();
						ahome->crypticconnections.length(len+1);	
						ahome->crypticconnections[len] = cconn;
					}
				}
			}

			DOM_Node initialrequestportsNode = parser->findChild(childNode, "initialrequestports");
			if (initialrequestportsNode != NULL)
			{
				DOM_NodeList initialrequestportsList = initialrequestportsNode.getChildNodes();
				DOM_Node initialrequestportsChildNode;
				DOMString initialrequestportsChildNodeName;

				int j;
				for (j=0; j<initialrequestportsList.getLength(); j++)
				{
					initialrequestportsChildNode = initialrequestportsList.item(j);
					initialrequestportsChildNodeName = initialrequestportsChildNode.getNodeName();

					if (initialrequestportsChildNodeName.equals("port"))
					{
						DOMString name = parser->getAttr(initialrequestportsChildNode, "name");
						int len = ahome->initialreqports.length();
						ahome->initialreqports.length(len+1);
						ahome->initialreqports[len] = CORBA::string_dup(name.transcode());
					}
				}
			}

			DOM_Node homeinitiativeNode = parser->findChild(childNode, "homeinitiative");
			if (homeinitiativeNode != NULL)
			{
				DOMString set = parser->getAttr(homeinitiativeNode, "set");
				ahome->homeinitiative = CORBA::string_dup(set.transcode());
			}

			DOM_Node componentinitiativeNode = parser->findChild(childNode, "componentinitiative");
			if (componentinitiativeNode != NULL)
			{
				DOMString set = parser->getAttr(componentinitiativeNode, "set");
				ahome->componentinitiative = CORBA::string_dup(set.transcode());
			}

			DOM_Node runcompleteNode = parser->findChild(childNode, "runcomplete");
			if (runcompleteNode != NULL)
			{
				DOMString set = parser->getAttr(runcompleteNode, "set");
				ahome->runcomplete = CORBA::string_dup(set.transcode());

				DOM_Node usesportsNode = parser->findChild(childNode, "usesports");
				if (usesportsNode != NULL)
				{
					DOM_NodeList usesportsList = usesportsNode.getChildNodes();
					DOM_Node usesportsChildNode;
					DOMString usesportsChildNodeName;

					int j;
					for (j=0; j<usesportsList.getLength(); j++)
					{
						usesportsChildNode = usesportsList.item(j);
						usesportsChildNodeName = usesportsChildNode.getNodeName();

						if (usesportsChildNodeName.equals("port"))
						{
							DOMString name = parser->getAttr(usesportsChildNode, "name");
							int len = ahome->runcompleteusesports.length();
							ahome->runcompleteusesports.length(len+1);
							ahome->runcompleteusesports[len] = CORBA::string_dup(name.transcode());
						}
					}
				}
			}

			DOM_Node configurationcompleteNode = parser->findChild(childNode, "configurationcomplete");
			if (configurationcompleteNode != NULL)
			{
				DOMString set = parser->getAttr(configurationcompleteNode, "set");
				ahome->configurationcomplete = CORBA::string_dup(set.transcode());

				DOM_Node usesportsNode = parser->findChild(childNode, "usesports");
				if (usesportsNode != NULL)
				{
					DOM_NodeList usesportsList = usesportsNode.getChildNodes();
					DOM_Node usesportsChildNode;
					DOMString usesportsChildNodeName;

					int j;
					for (j=0; j<usesportsList.getLength(); j++)
					{
						usesportsChildNode = usesportsList.item(j);
						usesportsChildNodeName = usesportsChildNode.getNodeName();

						if (usesportsChildNodeName.equals("port"))
						{
							DOMString name = parser->getAttr(usesportsChildNode, "name");
							int len = ahome->configurationcompleteusesports.length();
							ahome->configurationcompleteusesports.length(len+1);
							ahome->configurationcompleteusesports[len] = CORBA::string_dup(name.transcode());
						}
					}
				}
			}

			DOM_Node homefinderNode = parser->findChild(childNode, "homefinder");
			if (homefinderNode != NULL)
			{
				DOMString ref = parser->getAttr(homefinderNode, "ref");
				CORBA::Object_var obj = orb->string_to_object(ref.transcode());
				ahome->homefinder = CORBA::Object::_duplicate(obj);
			}

			DOM_Node withhomefinderNode = parser->findChild(childNode, "withhomefinder");
			if (withhomefinderNode != NULL)
			{
				DOM_NodeList registernamesList = withhomefinderNode.getChildNodes();
				DOM_Node withhomefinderChildNode;
				DOMString withhomefinderChildNodeName;

				int j;
				for (j=0; j<registernamesList.getLength(); j++)
				{
					withhomefinderChildNode = registernamesList.item(j);
					withhomefinderChildNodeName = withhomefinderChildNode.getNodeName();

					if (withhomefinderChildNodeName.equals("registername"))
					{
						DOM_Node nameText = withhomefinderChildNode.getFirstChild();
						DOMString name = nameText.getNodeValue();
						int len = ahome->withhomefinder.length();
						ahome->withhomefinder.length(len+1);
						ahome->withhomefinder[len] = CORBA::string_dup(name.transcode());
					}
				}
			}

			DOM_Node nameserviceNode = parser->findChild(childNode, "nameservice");
			if (nameserviceNode != NULL)
			{
				DOMString ref = parser->getAttr(nameserviceNode, "ref");
				CORBA::Object_var obj = orb->string_to_object(ref.transcode());
				ahome->nameservice = CORBA::Object::_duplicate(obj);
			}

			DOM_Node withnamingNode = parser->findChild(childNode, "withnaming");
			if (withhomefinderNode != NULL)
			{
				DOM_NodeList registernamesList = withnamingNode.getChildNodes();
				DOM_Node withnamingChildNode;
				DOMString withnamingChildNodeName;

				int j;
				for (j=0; j<registernamesList.getLength(); j++)
				{
					withnamingChildNode = registernamesList.item(j);
					withnamingChildNodeName = withnamingChildNode.getNodeName();

					if (withnamingChildNodeName.equals("registername"))
					{
						DOM_Node nameText = withnamingChildNode.getFirstChild();
						DOMString name = nameText.getNodeValue();
						int len = ahome->withnaming.length();
						ahome->withnaming.length(len+1);
						ahome->withnaming[len] = CORBA::string_dup(name.transcode());
					}
				}
			}

			aHomeMap.insert(MapArchHome::value_type(string(ahome->uuid),ahome));
		}
	}

	//for components
	DOM_Node ComponentsNode = parser->findChild(root, "components");
	if(ComponentsNode==NULL)
	{
		TRACE0(LEVEL1,"initialize: invalid xml file: miss components element. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		return;
	}

	DOM_NodeList componentsList = ComponentsNode.getChildNodes();

	for (i=0; i<componentsList.getLength(); i++)
	{
		childNode = componentsList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("component"))
		{
			ArchComponent_var acom = new ArchComponent;
			DOMString id = parser->getAttr(childNode, "id");
			acom->id = CORBA::string_dup(id.transcode());

			DOM_Node uuidNode = parser->findChild(childNode, "uuid");
			if (uuidNode != NULL)
			{
				DOM_Node uuidText = uuidNode.getFirstChild();
				DOMString uuid = uuidText.getNodeValue();
				acom->uuid = CORBA::string_dup(uuid.transcode());
			}

			DOM_Node comrefNode = parser->findChild(childNode, "comref");
			if (comrefNode != NULL)
			{
				DOM_Node comrefText = comrefNode.getFirstChild();
				DOMString comref = comrefText.getNodeValue();
				CORBA::Object_var obj = orb->string_to_object(comref.transcode());
				acom->comref = CORBA::Object::_duplicate(obj);
			}

			DOM_Node homeNode = parser->findChild(childNode, "homeplacement");
			if (homeNode != NULL)
			{
				DOMString homeid = parser->getAttr(homeNode, "idref");
				ArchHome_var tmpahome = getArchHome(homeid.transcode());
				acom->homeuuid = tmpahome->uuid;
			}

			DOM_Node portsNode = parser->findChild(childNode, "ports");
			if (portsNode != NULL)
			{
				DOM_NodeList portsList = portsNode.getChildNodes();
				DOM_Node portsChildNode;
				DOMString portsChildNodeName;

				int j;
				for (j=0; j<portsList.getLength(); j++)
				{
					portsChildNode = portsList.item(j);
					portsChildNodeName = portsChildNode.getNodeName();

					if (portsChildNodeName.equals("providesport"))
					{
						Port_var port = new Port;
						port->type = CORBA::string_dup("providesport");
						DOMString name = parser->getAttr(portsChildNode, "name");
						port->name = CORBA::string_dup(name.transcode());
						DOMString facetref = parser->getAttr(portsChildNode, "facetref");
						CORBA::Object_var obj = orb->string_to_object(facetref.transcode());
						port->reference = CORBA::Object::_duplicate(obj);

						int len = acom->ports.length();
						acom->ports.length(len+1);
						acom->ports[len] = port;
					}

					if (portsChildNodeName.equals("consumesport"))
					{
						Port_var port = new Port;
						port->type = CORBA::string_dup("consumesport");
						DOMString name = parser->getAttr(portsChildNode, "name");
						port->name = CORBA::string_dup(name.transcode());
						DOMString consumerref = parser->getAttr(portsChildNode, "consumerref");
						CORBA::Object_var obj = orb->string_to_object(consumerref.transcode());
						port->reference = CORBA::Object::_duplicate(obj);

						int len = acom->ports.length();
						acom->ports.length(len+1);
						acom->ports[len] = port;
					}
				}
			}

			DOM_Node linksNode = parser->findChild(childNode, "links");
			if (linksNode != NULL)
			{
				DOM_NodeList linksList = linksNode.getChildNodes();
				DOM_Node linksChildNode;
				DOMString linksChildNodeName;

				int j;
				for (j=0; j<linksList.getLength(); j++)
				{
					linksChildNode = linksList.item(j);
					linksChildNodeName = linksChildNode.getNodeName();

					if (linksChildNodeName.equals("astarget")||linksChildNodeName.equals("assender"))
					{
						DOMString ck = parser->getAttr(linksChildNode, "cookie");
						int len = acom->links.length();
						acom->links.length(len+1);
						acom->links[len] = CORBA::string_dup(ck.transcode());
					}
				}
			}

			DOM_Node boundableNode = parser->findChild(childNode, "boundable");
			if (boundableNode != NULL)
			{
				DOMString set = parser->getAttr(boundableNode, "set");
				acom->isboundable = CORBA::string_dup(set.transcode());
			}

			DOM_Node boundaryportsNode = parser->findChild(childNode, "boundaryports");
			if (boundaryportsNode != NULL)
			{
				DOM_NodeList boundaryportsList = boundaryportsNode.getChildNodes();
				DOM_Node boundaryportsChildNode;
				DOMString boundaryportsChildNodeName;

				int j;
				for (j=0; j<boundaryportsList.getLength(); j++)
				{
					boundaryportsChildNode = boundaryportsList.item(j);
					boundaryportsChildNodeName = boundaryportsChildNode.getNodeName();

					if (boundaryportsChildNodeName.equals("port"))
					{
						DOMString name = parser->getAttr(boundaryportsChildNode, "name");
						int len = acom->boundaryports.length();
						acom->boundaryports.length(len+1);
						acom->boundaryports[len] = CORBA::string_dup(name.transcode());
					}
				}
			}

			DOM_Node nameserviceNode = parser->findChild(childNode, "nameservice");
			if (nameserviceNode != NULL)
			{
				DOMString ref = parser->getAttr(nameserviceNode, "ref");
				CORBA::Object_var obj = orb->string_to_object(ref.transcode());
				acom->nameservice = CORBA::Object::_duplicate(obj);
			}

			DOM_Node withnamingNode = parser->findChild(childNode, "withnaming");
			if (withnamingNode != NULL)
			{
				DOM_NodeList registernamesList = withnamingNode.getChildNodes();
				DOM_Node withnamingChildNode;
				DOMString withnamingChildNodeName;
				
				int j;
				for (j=0; j<registernamesList.getLength(); j++)
				{
					withnamingChildNode = registernamesList.item(j);
					withnamingChildNodeName = withnamingChildNode.getNodeName();

					if (withnamingChildNodeName.equals("registername"))
					{
						DOM_Node nameText = withnamingChildNode.getFirstChild();
						DOMString name = nameText.getNodeValue();
						int len = acom->withnaming.length();
						acom->withnaming.length(len+1);
						acom->withnaming[len] = CORBA::string_dup(name.transcode());
					}
				}
			}

			DOM_Node portwithnamingsNode = parser->findChild(childNode, "portwithnamings");
			if (portwithnamingsNode != NULL)
			{
				DOM_NodeList portwithnamingsList = portwithnamingsNode.getChildNodes();
				DOM_Node portwithnamingsChildNode;
				DOMString portwithnamingsChildNodeName;

				int j;
				for (j=0; j<portwithnamingsList.getLength(); j++)
				{
					portwithnamingsChildNode = portwithnamingsList.item(j);
					portwithnamingsChildNodeName = portwithnamingsChildNode.getNodeName();

					if (portwithnamingsChildNodeName.equals("portwithnaming"))
					{
						DOM_Node portNode = parser->findChild(portwithnamingsChildNode,"port");
						DOMString name = parser->getAttr(portNode,"name");
						PortWithNaming_var portwithnaming = new PortWithNaming;
						portwithnaming->portname = CORBA::string_dup(name.transcode());

						DOM_NodeList registernamesList = portwithnamingsChildNode.getChildNodes();
						DOM_Node portwithnamingChildNode;
						DOMString portwithnamingChildNodeName;

						int p;
						for (p=0; p<registernamesList.getLength(); p++)
						{
							portwithnamingChildNode = registernamesList.item(p);
							portwithnamingChildNodeName = portwithnamingChildNode.getNodeName();
							if (portwithnamingChildNodeName.equals("registername"))
							{
								DOM_Node registernameText = portwithnamingChildNode.getFirstChild();
								DOMString registername = registernameText.getNodeValue();
								int wlen = portwithnaming->withnaming.length();
								portwithnaming->withnaming.length(wlen+1);
								portwithnaming->withnaming[wlen] = CORBA::string_dup(registername.transcode());
							}
						}
						
						int len = acom->portwithnamings.length();
						acom->portwithnamings.length(len+1);
						acom->portwithnamings[len] = portwithnaming;
					}
				}
			}

			aComponentMap.insert(MapArchComponent::value_type(string(acom->uuid),acom));
		}
	}

	//fix the ahomes' component uuid from id
	MapArchHome::iterator iter = aHomeMap.begin();
	while (iter != aHomeMap.end())
	{
		for (i=0; i<(*iter).second.coms.length(); i++)
		{
			CORBA::String_var comid = (*iter).second.coms[i];
			ArchComponent_var acom = getArchComponent(comid.in());
			(*iter).second.coms[i] = acom->uuid;
		}
		
		iter++;
	}


	//for connections
	DOM_Node connectionsNode = parser->findChild(root, "connections");
	if(connectionsNode==NULL)
	{
		TRACE0(LEVEL1,"initialize: invalid xml file: miss connections element. \n");		
		
		if(parser!=NULL)
		{
			delete parser;
			parser=NULL;
		}
		
		return;
	}

	DOM_NodeList connectionsList = connectionsNode.getChildNodes();
	for (i=0; i<connectionsList.getLength(); i++)
	{
		childNode = connectionsList.item(i);
		childNodeName = childNode.getNodeName();

		if (childNodeName.equals("connection"))
		{
			ArchConnection_var aconn = new ArchConnection;
			DOMString aconnid = parser->getAttr(childNode, "id");
			aconn->id = CORBA::string_dup(aconnid.transcode());

			DOM_Node cookieNode = parser->findChild(childNode, "cookie");
			if (cookieNode != NULL)
			{
				DOM_Node ckText = cookieNode.getFirstChild();
				DOMString ck = ckText.getNodeValue();
				aconn->cookie = CORBA::string_dup(ck.transcode());
			}

			DOM_Node connecttypeNode = parser->findChild(childNode, "connecttype");
			if (connecttypeNode != NULL)
			{
				DOM_Node connecttypeText = connecttypeNode.getFirstChild();
				DOMString connecttype = connecttypeText.getNodeValue();
				if (strcmp(connecttype.transcode(), "connectinterface"))
					aconn->connectmode = CORBA::string_dup("connect");
				else
					aconn->connectmode = CORBA::string_dup("subscribe");
			}

			DOM_Node strcomidNode = parser->findChild(childNode, "strcomid");
			if (strcomidNode != NULL)
			{
				DOM_Node strcomidText = strcomidNode.getFirstChild();
				DOMString strcomid = strcomidText.getNodeValue();
				ArchComponent_var srccom = getArchComponent(strcomid.transcode());
				aconn->srccomuuid = srccom->uuid;
				aconn->srccomref = CORBA::Object::_duplicate(srccom->comref);
			}

			DOM_Node srcportNode = parser->findChild(childNode, "srcport");
			if (srcportNode != NULL)
			{
				DOM_Node srcportText = srcportNode.getFirstChild();
				DOMString srcport = srcportText.getNodeValue();
				aconn->srcport = CORBA::string_dup(srcport.transcode());
			}

			DOM_Node targetidNode = parser->findChild(childNode, "targetid");
			if (targetidNode != NULL)
			{
				DOM_Node targetidText = targetidNode.getFirstChild();
				DOMString targetid= targetidText.getNodeValue();
				ArchComponent* targetcom = getArchComponent(targetid.transcode());
				if (targetcom != NULL)
				{
					aconn->targetuuid = targetcom->uuid;

					DOM_Node targetrefNode = parser->findChild(childNode, "targetref");
					DOM_Node targetrefText = targetrefNode.getFirstChild();
					DOMString targetref= targetrefText.getNodeValue();
					CORBA::Object_var obj = orb->string_to_object(targetref.transcode());
					aconn->targetref = CORBA::Object::_duplicate(obj);

					DOM_Node targetportNode = parser->findChild(childNode, "targetport");
					DOM_Node targetportText = targetportNode.getFirstChild();
					DOMString targetport= targetportText.getNodeValue();					
					aconn->targetport = CORBA::string_dup(targetport.transcode());

					int len = targetcom->ports.length();
					int p;
					for (p=0; p<len; p++)
					{
						if (strcmp(targetcom->ports[p].name, targetport.transcode()) == 0)
							break;
					}
					
					if (p<len)
					{
						if (strcmp(targetcom->ports[p].type, "providesport") == 0)
						{
							aconn->targettype = CORBA::string_dup("facet");
						}
						else if (strcmp(targetcom->ports[p].type, "consumesport") == 0)
						{
							aconn->targettype = CORBA::string_dup("consumer");
						}
					}
					else
					{
						aconn->targettype = CORBA::string_dup("supports");
					}
									
					delete targetcom;
					targetcom = NULL;
				}
				else
				{
					ArchHome_var targethome = getArchHome(targetid.transcode());
					aconn->targetuuid = targethome->uuid;
					aconn->targettype = CORBA::string_dup("home");
					aconn->targetref = CORBA::Object::_duplicate(targethome->homeref);
				}
			}

			aConnectionMap.insert(MapArchConnection::value_type(string(aconn->cookie),aconn));
		}
	}

	if (parser != NULL)
	{
		delete parser;
		parser = NULL;
	}
}

void
Architecture::printToRelationFile()
{
	ofstream outfile(relationFile.in());
	if(!outfile)
	{
		cout<<"Can not create the Relation.xml ."<<endl;
		return;
	}

	//input the file header
	outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;

	outfile << "<domainconf name=\"ccmdomain\">" << endl;

	outfile << "	<connections>" << endl;

	MapArchConnection::iterator aconn_iter = aConnectionMap.begin();
	while (aconn_iter != aConnectionMap.end())
	{
		if (strcmp((*aconn_iter).second.connectmode, "connect") == 0)
			outfile << "		<connectinterface id=\"" << (*aconn_iter).second.id << "\">" << endl;
		else
			outfile << "		<connectevent id=\"" << (*aconn_iter).second.id << "\">" << endl;

		outfile << "			<connectsource>" << endl;
		MapArchComponent::iterator acom_iter = aComponentMap.find(string((*aconn_iter).second.srccomuuid));
		outfile << "				<componentinstantiationref idref=\"" << (*acom_iter).second.id << "\"/>" << endl;
		MapArchHome::iterator ahome_iter = aHomeMap.find(string((*acom_iter).second.homeuuid));
		CORBA::String_var comrepid = (*ahome_iter).second.componentrepid;
		char seps[] = ":/";
		char *token, *tmp, *type;
		token = strtok(comrepid, seps);
		type = token;
		token = strtok( NULL, seps );
		tmp = token;
		while (true)
		{
			token = strtok( NULL, seps );
			if (token == NULL)
				break;
			type = tmp;
			tmp = token;
		}
		outfile << "				<componenttype type=\"" << type << "\"/>" << endl;
		outfile << "				<componentkind name=\""  << (*ahome_iter).second.type << "\"/>" << endl;
		outfile << "				<destination IP=\"" << (*ahome_iter).second.serveractivatorid << "\"/>" << endl;
		CORBA::Object_var obj = CORBA::Object::_duplicate((*acom_iter).second.comref);
		if (obj->_non_existent())
			outfile << "				<Active active=\"INACTIVE\"/>" << endl;
		else
			outfile << "				<Active active=\"ACTIVE\"/>" << endl;
		outfile << "			</connectsource>" << endl;

		outfile << "			<connectdest>" << endl;
		if (strcmp((*aconn_iter).second.targettype, "home") == 0)
		{
			ahome_iter = aHomeMap.find(string((*aconn_iter).second.targetuuid));
			outfile << "				<homeinterfaceref idref=\"" << (*ahome_iter).second.id << "\"/>" << endl;
			CORBA::String_var homerepid = (*ahome_iter).second.homerepid;
			token = strtok(homerepid, seps);
			type = token;
			token = strtok( NULL, seps );
			tmp = token;
			while (true)
			{
				token = strtok( NULL, seps );
				if (token == NULL)
					break;
				type = tmp;
				tmp = token;
			}
			outfile << "				<hometype type=\"" << type << "\"/>" << endl;
			outfile << "				<homekind name=\""  << (*ahome_iter).second.type << "\"/>" << endl;
			obj = CORBA::Object::_duplicate((*ahome_iter).second.homeref);
		}
		else
		{
			acom_iter = aComponentMap.find(string((*aconn_iter).second.targetuuid));
			outfile << "				<componentinstantiationref idref=\"" << (*acom_iter).second.id << "\"/>" << endl;
			ahome_iter = aHomeMap.find(string((*acom_iter).second.homeuuid));	
			comrepid = (*ahome_iter).second.componentrepid;
			token = strtok(comrepid, seps);
			type = token;
			token = strtok( NULL, seps );
			tmp = token;
			while (true)
			{
				token = strtok( NULL, seps );
				if (token == NULL)
					break;
				type = tmp;
				tmp = token;
			}
			outfile << "				<componenttype type=\"" << type << "\"/>" << endl;
			outfile << "				<componentkind name=\""  << (*ahome_iter).second.type << "\"/>" << endl;
			obj = CORBA::Object::_duplicate((*acom_iter).second.comref);
		}

		outfile << "				<destination IP=\"" << (*ahome_iter).second.serveractivatorid << "\"/>" << endl;
		
		if (obj->_non_existent())
			outfile << "				<Active active=\"INACTIVE\"/>" << endl;
		else
			outfile << "				<Active active=\"ACTIVE\"/>" << endl;
		outfile << "			</connectdest>" << endl;
		
		if (strcmp((*aconn_iter).second.connectmode, "connect") == 0)
			outfile << "		</connectinterface>" << endl;
		else
			outfile << "		</connectevent>" << endl;

		aconn_iter++;
	}

	outfile << "	</connections>" << endl;

	outfile << "</domainconf>" << endl;
	
	outfile.close();
}

void 
Architecture::setHomeId(const char* old_id, const char* new_id)
{
	MapArchHome::iterator home_iter = aHomeMap.begin();
	MapArchHome::iterator home_iter_end = aHomeMap.end();
	while(home_iter !=home_iter_end)
	{
		if(strcmp((*home_iter).second.id, old_id) == 0)
			break;
		home_iter++;
	}

	if(home_iter ==home_iter_end)
	{
		cout << "ArchHome " << old_id<< " doesn't exist!" << endl;
		return;
	}

	(*home_iter).second.id = CORBA::string_dup(new_id);
}

void 
Architecture::setComponentId(const char* old_id, const char* new_id)
{
	MapArchComponent::iterator com_iter=aComponentMap.begin();	
	MapArchComponent::iterator com_iter_end=aComponentMap.end();
	while(com_iter!=com_iter_end)
	{
		if (strcmp((*com_iter).second.id, old_id) == 0)
			break;
		com_iter++;
	}

	if(com_iter == com_iter_end)
	{
		cout << "ArchComponent " << old_id<< " doesn't exist!" << endl;
		return;
	}

	(*com_iter).second.id = CORBA::string_dup(new_id);

}

void 
Architecture::setConnecitonId(const char* old_id, const char* new_id)
{
	MapArchConnection::iterator conn_iter=aConnectionMap.begin();	
	MapArchConnection::iterator conn_iter_end=aConnectionMap.end();
	while(conn_iter!=conn_iter_end)
	{
		if (strcmp((*conn_iter).second.id, old_id) == 0)
			break;
		conn_iter++;
	}

	if(conn_iter == conn_iter)
	{
		cout << "ArchConnection " << old_id<< " doesn't exist!" << endl;
		return;
	}

	(*conn_iter).second.id = CORBA::string_dup(new_id);
}
