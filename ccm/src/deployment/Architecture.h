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

#ifndef Architecture_H
#define Architecture_H

#include <CORBA.h>
#include <ArchObject.h>
#include <map>
#include <string>

using namespace std;

namespace STARCCM
{
	class Architecture
	{
	private:
		CORBA::ORB_var orb;
		CORBA::String_var assemblyUUID;
		CORBA::String_var rootDir;
		CORBA::String_var archFile;
		CORBA::String_var relationFile;

		struct ContainerInfo
		{
			CORBA::Object_var ref;
			UUIDs homeUUIDs;

			ContainerInfo() {}

			ContainerInfo(const ContainerInfo& c)
			{
			    ref = CORBA::Object::_duplicate(c.ref);
			    homeUUIDs = c.homeUUIDs;
			}

			ContainerInfo& operator=(const ContainerInfo& c)
			{
			    ref = CORBA::Object::_duplicate(c.ref);
			    homeUUIDs = c.homeUUIDs;
			    
			    return *this;
			}

			};		
		typedef map<string, ArchHome> MapArchHome;
		typedef map<string, ArchComponent> MapArchComponent;
		typedef map<string, ArchConnection> MapArchConnection;
		typedef map<string, ContainerInfo> MapContainer;
		
		MapArchHome aHomeMap;
		MapArchComponent aComponentMap;
		MapArchConnection aConnectionMap;
		MapContainer containerMap;

		
	public:
		Architecture(CORBA::ORB_ptr orb_, const char* id_, const char* rootDir);
		~Architecture();

		void addArchHome(ArchHome& ahome);
		void delArchHome(const char* homeuuid);
		ArchHomes* getAllArchHomes();
		ArchHome* getArchHome(const char* homeid);
		ArchHome* getArchHome_By_UUID(const char* uuid);
		
		void addArchComponent(ArchComponent& acom);
		void delArchComponent(const char* comuuid);
		ArchComponents* getAllArchComponents();
		ArchComponent* getArchComponent(const char* comid);
		ArchComponent* getArchComponent_By_UUID(const char* uuid);
		
		void addArchConnection(ArchConnection& aconn);
		void delArchConnection(const char* cookie);
		ArchConnections* getAllArchConnections();
		ArchConnection* getArchConnection(const char* connid);
		ArchConnection* getArchConnection_By_Cookie(const char* ck);
		
		void update();
		void clean();
		void printToFile();
		void initialize(const char* filename);
		void printToRelationFile();

		void setHomeId(const char* old_id, const char* new_id);
		void setComponentId(const char* old_id, const char* new_id);
		void setConnecitonId(const char* old_id, const char* new_id);
	};
};

#endif
