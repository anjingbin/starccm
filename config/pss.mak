#  **********************************************************************
#
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: An Jingbin	anjb@sina.com	
# **********************************************************************


PSS_HOME 	= $(top_srcdir)\..\pss

PSS_INCLUDE 	= /I$(PSS_HOME)\include

!if "$(ORB_NAME)" == "ob"
PSS_CPPFLAGS	= /DORBacus $(PSS_INCLUDE)
PSDL		= $(PSS_HOME)\bin\psdl.exe
PSS_IDLFLAGS	= -DORBacus -I$(PSS_HOME)\idl
!endif

!if "$(ORB_NAME)" == "star"
PSS_CPPFLAGS	= /DStarBus $(PSS_INCLUDE)
PSDL		= $(PSS_HOME)\bin\star-psdl.exe
PSS_IDLFLAGS	= -DStarBus -I$(PSS_HOME)\idl
!endif

PSS_CFLAGS	= $(PSS_INCLUDE)

PSS_LIBS	= $(PSS_HOME)\lib\pss$(LIBSUFFIX).lib
PSS_IDLDIR	= $(PSS_HOME)\idl
MKDEP		= $(top_srcdir)\..\config\mkdep.exe

!if "$(DLL)" == "yes"
PATH		= $(PSS_HOME)\lib;$(PATH)
!endif
