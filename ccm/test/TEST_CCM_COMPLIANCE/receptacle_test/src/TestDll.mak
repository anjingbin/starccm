# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: Zheng Di	sonofwind0109@sina.com
# **********************************************************************


top_srcdir	= ..\..\..\..

DLL_NAME	= ..\bin\TestDll.dll
PDB_NAME	= ..\bin\TestDll.pdb
IMP_LIB		= ..\bin\TestDll.lib
EXP_NAME	= ..\bin\TestDll.exp

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

OBJS	= CCM_test.obj \
	CCM_test_Context_impl.obj \
	test.obj \
	test_impl.obj \
	test_skel.obj \
	TestDll.obj

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