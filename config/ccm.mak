#  **********************************************************************
#
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: An Jingbin	anjb@sina.com
#          Wang Kebo    mep@263.net
# **********************************************************************


CCM_HOME 	= $(top_srcdir)\..\ccm
XER_HOME	= $(top_srcdir)\..\xercesc\xerces-c1_7_0-win32

CCM_INCLUDE	= /I$(CCM_HOME)\include

!if "$(ORB_NAME)" == "ob"
CCM_CPPFLAGS	= $(CCM_CPPFLAGS) /DORBacus $(CCM_INCLUDE) /I$(XER_HOME)\include
CCM_CIDLFLAGS	= -DORBacus
CCM_IDL3FLAGS	= -DORBacus
!endif

!if "$(ORB_NAME)" == "star"
CCM_CPPFLAGS	= $(CCM_CPPFLAGS) /DStarBus $(CCM_INCLUDE) /I$(XER_HOME)\include
CCM_CIDLFLAGS	= -DStarBus
CCM_IDL3FLAGS	= -DStarBus
!endif

!if "$(ORB_NAME)" == "tao"
CCM_CPPFLAGS	= $(CCM_CPPFLAGS) /DTAO $(CCM_INCLUDE) /I$(XER_HOME)\include
CCM_CIDLFLAGS	= -DTAO
CCM_IDL3FLAGS	= -DTAO
!endif

!if "$(LITE_VERSION)" == "yes"
CCM_CPPFLAGS	= $(CCM_CPPFLAGS) /DLITE_VERSION
CCM_IDLFLAGS	= -I$(CCM_HOME)\idl -DLITE_VERSION
!else
CCM_CPPFLAGS	= $(CCM_CPPFLAGS)
CCM_IDLFLAGS	= -I$(CCM_HOME)\idl
!endif

!if "$(STARCCM_MTL)" == "yes"
CCM_CPPFLAGS	= $(CCM_CPPFLAGS) /DSTARCCM_MTL
CCM_IDLFLAGS	= -I$(CCM_HOME)\idl -DSTARCCM_MTL
!else
CCM_CPPFLAGS	= $(CCM_CPPFLAGS)
CCM_IDLFLAGS	= -I$(CCM_HOME)\idl
!endif



CCM_CFLAGS	= $(CCM_INCLUDE)
CCM_IDLDIR	= $(CCM_HOME)\idl
CCM_LIBS	= $(CCM_HOME)\lib\ccm$(LIBSUFFIX).lib $(XER_HOME)\lib\xerces-c_1D.lib $(CCM_HOME)\lib\Psapi.Lib
IDL3		= $(CCM_HOME)\bin\idl3.exe
CIDL		= $(CCM_HOME)\bin\cidl.exe
MKDEP		= $(top_srcdir)\..\config\mkdep.exe

!if "$(DLL)" == "yes"
PATH		= $(CCM_HOME)\lib;$(PATH)
!endif
