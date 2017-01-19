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

#ifndef FILEACCESSOR_IMPL_H
#define FILEACCESSOR_IMPL_H

#include <CORBA.h>
#include <FileAccessor_skel.h>

namespace STARCCM
{
	namespace Deployment
	{ 
		class FileAccessor_impl : public POA_STARCCM::Deployment::FileAccessor,
				   public PortableServer::RefCountServantBase
		{
		    
		public:
		
		    FileAccessor_impl();
		
		    virtual void locate_file(const char* filepath)
		        throw(STARCCM::Deployment::FileSystemError,
		              CORBA::SystemException);
		
		    virtual STARCCM::Deployment::FileOctetSeq* get_octet_seq(STARCCM::Deployment::FileOffset from_octet,
		                                                      STARCCM::Deployment::FileCount max_octets)
		        throw(STARCCM::Deployment::FileSystemError,
		              CORBA::SystemException);
		
		private:	
			
			CORBA::String_var fileName;	
			CORBA::String_var directory;
			bool verbose;
		};
		
		
		class FileAccessorFactory_impl : public POA_STARCCM::Deployment::FileAccessorFactory,
				   public PortableServer::RefCountServantBase
		{
		public:
		
		    FileAccessorFactory_impl();
		    			
		    virtual STARCCM::Deployment::FileAccessor_ptr createFileAccessor()
        		throw(STARCCM::Deployment::CreateFailure,
              			CORBA::SystemException) ;                 			
              	
        	};
	}

}

#endif
