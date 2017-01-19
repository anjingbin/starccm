# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: An Jingbin	anjb@sina.com
# **********************************************************************


top_srcdir	= ..\..\..\..

DLL_NAME	= ..\bin\HelloWorld.dll
PDB_NAME	= ..\bin\HelloWorld.pdb
IMP_LIB		= ..\bin\HelloWorld.lib
EXP_NAME	= ..\bin\HelloWorld.exp

PDB_FILES	= $(PDB_NAME)

PDB_FLAGS	= /Fd$(PDB_NAME)
TARGETS		= $(DLL_NAME) $(PDB_NAME) $(IMP_LIB) $(EXP_NAME)


IDLDIR		= $(top_srcdir)\idl
HDIR		= $(top_srcdir)\include


GENERATED	=

!include $(top_srcdir)\config\Make.rules.mak

!if "$(LITE_VERSION)" == "yes"
PSS_LIBS=
!endif

OBJS	= CCM_HelloWorld.obj \
	CCM_HelloWorld_Context_impl.obj \
	HelloWorld.obj \
	HelloWorld_impl.obj \
	HelloWorld_skel.obj \
	HelloWorldDll.obj

ALL_CPPFLAGS	= /I. /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(CPPFLAGS)
ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
ALL_LIBS	= $(OB_LIBS) $(OTS_LIBS) $(PSS_LIBS) $(CCM_LIBS) $(LIBS)


$(DLL_NAME): $(OBJS)
	if exist "$@" \
	del "$@"
	$(LINK) /dll /incremental:no /pdb:"$(PDB_NAME)" /implib:"$(IMP_LIB)" $(LINKFLAGS) /out:$@ $(OBJS) $(ALL_LIBS)


clean::

min:: all

install:: all

install_min:: install

!if exist (".windep")
!include .windep
!endif