// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Dou Lei    fancydou@sina.com 
//
// **********************************************************************

// Version: 1.0.0

#include <CORBA.h>
 
#include <fileaccessor_impl.h>
#include <Trace.h>

#include <string>
#include <stdio.h>
#include <malloc.h>

#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <errno.h>



using namespace std;
using namespace STARCCM::Deployment;

FileAccessor_impl::FileAccessor_impl()	
{
	TRACE0(LEVEL5,"FileAccessor: FileAccessor started.\n");
}

void 
FileAccessor_impl::locate_file(const char* filepath)
        throw(STARCCM::Deployment::FileSystemError,
              CORBA::SystemException)
{	
	
	TRACE1(LEVEL5,"FileAccessor: locate file %s.\n",filepath);
	
	char *directoryname;
	FILE *fileReadStream;

	//get the directory and the fileName from the filepath
	int fullpathlen=strlen(filepath);	
	
	if(strrchr(filepath,'.')==NULL)
	{
		TRACE1(LEVEL1,"FileAccessor: locate file %s failed: File name or extended name is missed.\n",filepath);		
		throw FileSystemError(ENTRY_PATH_ERROR ,"File name or extended name is missed .");
	}
	
	if(strrchr(filepath,PATH_DELILIMITOR_CHAR)==NULL)
	{
		TRACE1(LEVEL1,"FileAccessor: locate file %s failed: Path is invalid.\n",filepath);		
		throw FileSystemError(ENTRY_PATH_ERROR ,"Path Invalid.");
	}
	else
		fileName=CORBA::String_var(CORBA::string_dup(strrchr(filepath,PATH_DELILIMITOR_CHAR)+1));
		
	int fileNamelen=strlen(fileName.in());
	int directorylen=fullpathlen-fileNamelen-1;

	directoryname=(char *)malloc(directorylen+1);
	if(directoryname==NULL)
		throw CORBA::NO_MEMORY();
		
	strncpy(directoryname,filepath,directorylen);
	directoryname[directorylen]=NULL;
	
	
	//assert whether the directory is valid;
	
#ifdef WIN32
	if( _chdir(directoryname))
#else
	if(chdir(directoryname) == -1)
#endif
	{
		TRACE2(LEVEL1,"FileAccessor: locate file %s failed: Unable to locate the directory %s.\n",filepath,directoryname);
	   	free(directoryname);	   	
	   	throw FileSystemError(ENTRY_PATH_ERROR ,"Directory not exist.");
     
   	}  

	if( (fileReadStream  = fopen( filepath, "rb" )) == NULL )
	{
		
		TRACE2(LEVEL1,"FileAccessor: locate file %s failed: Unable to locate the file %s.\n",filepath,fileName.in());				
		free(directoryname);	
		throw FileSystemError(ENTRY_PATH_ERROR ,"File not exist.");
	}		
	
	directory=CORBA::String_var(CORBA::string_dup(filepath));
	free(directoryname);
	fclose(fileReadStream);
}
            

STARCCM::Deployment::FileOctetSeq* 
FileAccessor_impl::get_octet_seq(STARCCM::Deployment::FileOffset from_octet,
                                 STARCCM::Deployment::FileCount max_octets)
        throw(STARCCM::Deployment::FileSystemError,
              CORBA::SystemException)
{
	TRACE1(LEVEL5,"FileAccessor: get_octect_seq of file %s.\n",fileName.in());
	
	FILE *fileReadStream;
	

	//try to open the file under the directory

	if( (fileReadStream  = fopen( directory, "rb" )) == NULL )
	{
		TRACE1(LEVEL1,"FileAccessor: get_octet_seq of %s failed: Unable to locate the file.\n",fileName.in());
		throw FileSystemError(ENTRY_PATH_ERROR ,"File not exist.");
	}	
	
	//read the file of max_octets from from_octet
	
	FileOctetSeq_var readBuffer=new FileOctetSeq(max_octets);
	readBuffer->length(max_octets);	   
		
	if(fseek( fileReadStream, from_octet , SEEK_SET)!=0)
	{
		
		TRACE1(LEVEL1,"FileAccessor: get_octet_seq of %s failed: File for read seek error.\n",fileName.in());		
		throw FileSystemError(ENTRY_IO_ERROR ,"File for read seek error.");
	}
	
	int readSize=fread( readBuffer->get_buffer(), 1, max_octets, fileReadStream );			
	if(ferror(fileReadStream))
	{
		
		TRACE1(LEVEL1,"FileAccessor: get_octet_seq of %s failed: File read error.\n",fileName.in());		
		throw FileSystemError(ENTRY_IO_ERROR ,"File read error.");  
	}
	
	readBuffer->length(readSize);
			 
	fclose(fileReadStream);		
	
	return readBuffer._retn(); 
	
}	             


FileAccessorFactory_impl::FileAccessorFactory_impl() 	
{
	TRACE0(LEVEL5,"FileAccessorFactory: FileAccessorfactory started.\n");
	cout<<"FileAccessorFactory: FileAccessorfactory started."<<endl;
}

STARCCM::Deployment::FileAccessor_ptr 
FileAccessorFactory_impl::createFileAccessor()
   	throw(STARCCM::Deployment::CreateFailure,
        	CORBA::SystemException)
{

	TRACE0(LEVEL5,"FileAccessorFactory: FileAccessorfactory create FileAccessor.\n");
	   		
    	FileAccessor_impl* fileaccessorImpl=new FileAccessor_impl();
   	PortableServer::ServantBase_var servant = fileaccessorImpl;	
   	FileAccessor_var fileaccessor = fileaccessorImpl -> _this();
   	
   	return fileaccessor._retn();        
        
}


