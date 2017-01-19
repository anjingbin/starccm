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

!if "$(ORB_NAME)" == "ob"
OTS_HOME 	= $(top_srcdir)\..\ots
!endif

!if "$(ORB_NAME)" == "star"
OTS_HOME 	= $(top_srcdir)\..\star-ots
!endif

OTS_INCLUDE	= /I$(OTS_HOME)\include 
OTS_CPPFLAGS	= $(OTS_INCLUDE)
OTS_IDLFLAGS	= -I$(OTS_HOME)\idl
OTS_IDLDIR	= $(OTS_HOME)\idl
OTS_LIBS	= $(OTS_HOME)\lib\ots_lib$(LIBSUFFIX).lib
MKDEP		= $(top_srcdir)\..\config\mkdep.exe

!if "$(DLL)" == "yes"
PATH		= $(OTS_HOME)\lib;$(PATH)
!endif
