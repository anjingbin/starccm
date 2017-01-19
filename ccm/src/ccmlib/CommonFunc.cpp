#include <CORBA.h>
#include <CommonFunc.h>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#ifdef WIN32
	#include <Winsock.h>
	#include <process.h>
	#include <Time.h>
	#include <direct.h>
	#include <Windows.h>
#else
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <fcntl.h>
	#include <fnmatch.h>	
	#include <netdb.h>
	#include <unistd.h>
	#include <dirent.h>	
	#include <arpa/inet.h>
	#include <errno.h>
#endif
using namespace std;

//deleted by xiao heping 2004/07/13
//char * HelpFun::getHostIP()
//{
//	char szHostName[128];
//	struct hostent *host=NULL;
//#ifdef WIN32
//	WSADATA wsaData;
//#endif
//	sockaddr_in sockAddr;
//
//	//socket initialized
//#ifdef WIN32
//	if(WSAStartup(MAKEWORD(2,0),&wsaData)!=0)
//		return NULL;
//#endif
//	if(gethostname( szHostName, sizeof(szHostName))!=0)
//		return NULL;
//
//	host = gethostbyname(szHostName);
//	if (host == NULL)
//		return NULL;
//	
//	memset(&sockAddr, 0, sizeof(sockAddr));
//#ifdef WIN32
//	CopyMemory(&sockAddr.sin_addr,host->h_addr_list[(CORBA::ULong)0],host->h_length);
//
//#else
//	memcpy(&sockAddr.sin_addr,host->h_addr_list[(CORBA::ULong)0],host->h_length);
//#endif
//	return CORBA::string_dup(inet_ntoa(sockAddr.sin_addr));
//}
//end deleted
	
bool HelpFun::ObjectExist(CORBA::ORB_ptr orb_,const char *corbaloc,int timeout)
{
	CORBA::Object_var obj;
	try
	{
		obj=orb_->string_to_object(corbaloc);
    }
    catch(const CORBA::BAD_PARAM&)
    {
        TRACE1(LEVEL1,"ObjectExist: Object corbaloc %s invalid \n ",corbaloc);        	
        return false;
    }
	
    if(CORBA::is_nil(obj))
    {		
		TRACE1(LEVEL1,"ObjectExist: Cannot get the reference of %s. \n ",corbaloc);        			
		return false;
   	}
 
	time_t ltime;
	time( &ltime );
	int timestart=ltime;
	int timenow=ltime;
	bool exist=false;
	TRACE1(LEVEL6,"ObjectExist: the start time is %d .\n",timenow);		
	
    while(((timenow-timestart)<timeout)&&(exist==false))
    {
    	try
    	{
    		if(!(obj->_non_existent()))
    		{
    			exist=true;
    		}
    	}
    	catch(const CORBA::TRANSIENT &)
    	{    			
    	}
    	timenow=time( &ltime );
    }
    	
    if(exist==false)
    {
    	TRACE1(LEVEL6,"ObjectExist: now it is time %d .\n",timenow);
    	TRACE0(LEVEL2,"ObjectExist: object dose not created correctly whithin timeout.\n");    		
    	return false;
    }
    return true;
}
//end modify

int HelpFun::deleteDir(const char *dir)
{
#ifdef WIN32
	string directory="";
	directory=directory+dir+"\\*.*";
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(directory.c_str(), &FindFileData);

	while(FindNextFile(hFind,&FindFileData)!=0)
	{
		TRACE1(LEVEL6,"deleteDir: the file name is %s. \n",FindFileData.cFileName);		
	
		string fileName=FindFileData.cFileName;

		if( (strcmp(fileName.c_str(),".") != 0) && (strcmp(fileName.c_str(),"..") != 0) )
		{
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				string subDir="";
				subDir=subDir+dir+"\\"+fileName;
				
				if(deleteDir(subDir.c_str())==-1)
				{					
					TRACE1(LEVEL1,"deleteDir %s failed. \n",FindFileData.cFileName);					
					FindClose(hFind);
					return -1;
				}
					
				if(_rmdir(subDir.c_str())==-1)
				{
					TRACE1(LEVEL1,"deleteDir %s failed. \n",FindFileData.cFileName);
					FindClose(hFind);
					return -1;
				}
			}
			else
			{
				string subFile="";
				subFile=subFile+dir+"\\"+fileName;
				if(DeleteFile(subFile.c_str())==0)
				{
					TRACE1(LEVEL1,"deleteDir: delete file %s failed.",subFile.c_str());
					
					FindClose(hFind);
					return -1;
				}
			}
		}//end of if to skip . and ..
	} //end of while
  
	 FindClose(hFind);

#else

	 DIR *dp;
	 struct dirent *d;
	 struct stat st;
	 
	 //open the directory
	 
	 if( (dp = opendir(dir)) == NULL )
	 {
		 TRACE1(LEVEL1,"deleteDir %s failed. \n",dir);
		 return -1;
	 }

	 //delet directory including each file in it
	 
	 while( ( d = readdir(dp) ) != NULL)
	 {
		 string fileName = string(d->d_name);
		 
		 if((strcmp(fileName.c_str(),"." ) != 0)&&(strcmp(fileName.c_str(),"..") != 0 ))
		 {
			string subDir = "";
			subDir = subDir + dir + PATH_DELILIMITOR + fileName;
			
			//get the state of the entry
			
			if( lstat(subDir.c_str(),&st) < 0 )
			{
				TRACE1(LEVEL1,"deleteDir %s failed. \n",dir);
				closedir(dp);
				return -1;
			}

			if(((st.st_mode) & S_IFMT) == S_IFDIR)
			{
			
				if(deleteDir(subDir.c_str()) == -1)
				{
					TRACE1(LEVEL1,"deleteDir %s failed. \n",dir);
					closedir(dp);
					return -1;
									
				}
			
				if(rmdir(subDir.c_str()) == -1)
				{
					TRACE1(LEVEL1,"deleteDir %s failed. \n",dir);
					closedir(dp);
					return -1;
				}
			}
			else
			{
				if(unlink(subDir.c_str()) == -1)
				{
					TRACE1(LEVEL1,"deleteDir %s failed. \n",dir);
					closedir(dp);
					return -1;
				}
			}
			 
		 }
			
	 }

	 closedir(dp);

#endif
		 
	 return 0;
}


int HelpFun::copyFile(const char *source, const char *target)
{
	TRACE1(LEVEL6,"copyFile: the source file is %s .\n",source);
	TRACE1(LEVEL6,"copyFile: the target file is %s .\n",target);
#ifdef WIN32	
	if(CopyFile(source,target,false)==0)
	{
		TRACE0(LEVEL1,"copyFile: Copy file into the directory specified failed..\n");
		return -1;
	}
#else

	char block[1024000];
	int in, out, nread;
	
	in = open(source, O_RDONLY);
	out = open(target, O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	
	if(in == -1)
	{
		TRACE1(LEVEL1,"copyFile: Copy file failed for open source file failure: %s.\n",strerror(errno));
		return -1;
	}

	if(out == -1)
	{
		TRACE1(LEVEL1,"copyFile: Copy file failed for open target file failure: %s.\n",strerror(errno));
		return -1;
	}
	
	while( (nread = read(in,block,sizeof(block)) ) > 0 )
	{
		write(out,block,nread);
	}

	if( (close(in) == -1) || (close(out) == -1) )
	{
		TRACE1(LEVEL1,"copyFile: Copy file failed for close file failure:  %s.\n",strerror(errno));
		return -1;
	}
	
	if(nread == -1)
	{
		TRACE0(LEVEL1,"copyFile: Copy file failed for read failure.\n");
		return -1;
	}
#endif
	return 0;
}

//*********************************************************
//locateFile locate the file specified by the fileName,
//if any error occured return NULL string, 
//else return the correct fileName not the full path
//*********************************************************

char *
HelpFun::locateFile(const char *fileName)
{
	CORBA::String_var findFileName;
#ifdef WIN32
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
		
	hFind = FindFirstFile(fileName, &FindFileData);
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
    		TRACE1(LEVEL2,"locateFile: can not find the file %s .\n",fileName);     		
    		return NULL;
  	} 
  	findFileName=CORBA::string_dup(FindFileData.cFileName);  	
    	FindClose(hFind);
    	TRACE1(LEVEL6,"locateFile: the file found is %s .\n",findFileName);

	return findFileName._retn();
	
#else

	//get the directory  from the argument which actually represents filepath
	findFileName = CORBA::string_dup(strrchr(fileName,PATH_DELILIMITOR_CHAR)+1);

	int findFileNameLen = strlen(findFileName.in());
	int fullPathLen = strlen(fileName);
	int directoryLen = fullPathLen - findFileNameLen - 1;

	char * directoryName = (char *)malloc(directoryLen + 1);
	strncpy(directoryName, fileName, directoryLen);
	directoryName[directoryLen] = NULL;
	CORBA::String_var directory = CORBA::string_dup(directoryName);
	free(directoryName);
	
	//search in the directory
	DIR *dp;
	struct dirent *d;
	
	if((dp = opendir(directory.in())) == NULL)
	{
		TRACE1(LEVEL2,"locateFile: can not find the file %s .\n",fileName);
		return NULL;
	}

	while((d = readdir(dp)) != NULL)
	{
		CORBA::String_var currentFileName = CORBA::string_dup(d->d_name);

		if(fnmatch(findFileName.in(),currentFileName.in(),0) == 0 )
		{
			return currentFileName._retn();
		}
	}

	return NULL;
	
#endif	
}

char* HelpFun::GetCCMProfileString(const char* strSectionName,
								const char* keyName, 
								const char* defautValue,
								const char* fileName)
{
	const char* homeDir = getenv("CCM_HOME");
	string filepath = homeDir;
	filepath += PATH_DELILIMITOR;
	filepath += fileName;

	FILE* iniFile = fopen(filepath.c_str(),"r");
	if(iniFile == NULL)
		return NULL;

	char *break_set = " \t\n=";
	char dataBuf[128];
	string section = "[";
	section += strSectionName;
	section += "]";
	
	CORBA::String_var strRtn = defautValue;

	while(!feof(iniFile))
	{
		char *tokp,*sp;
		sp = dataBuf;
		fgets(dataBuf,128,iniFile);
		tokp = strtok(sp,break_set);
		if(tokp == NULL)
			continue;

		if(strcmp(tokp,section.c_str()) == 0)
		{
			while(!feof(iniFile))
			{
				fgets(dataBuf,128,iniFile);
				sp = dataBuf;
				tokp = strtok(sp,break_set);
				if(tokp == NULL)
					continue;
				if(tokp[0] == '[')
					goto EXIT;
				if(strcmp(tokp,keyName)==0)
				{
					sp = NULL;
					tokp = strtok(sp,break_set);
					if(tokp != NULL) 
					{
						strRtn = CORBA::string_dup(tokp);
						goto EXIT;
					}
				}//for statement if(strcmp(tokp,keyName)==0)
			}//for statement while(!feof(iniFile))
		}//for statement if(strcmp(tokp,section.c_str()) == 0)
	}//for statement while(!feof(iniFile))
EXIT:
	fclose(iniFile);
	return strRtn._retn();
}

