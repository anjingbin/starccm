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

DLL_NAME	= ..\bin\StdPlusDll.dll
PDB_NAME	= ..\bin\StdPlusDll.pdb
IMP_LIB		= ..\bin\StdPlusDll.lib
EXP_NAME	= ..\bin\StdPlusDll.exp

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

OBJS	=   CCM_plus.obj \
	CCM_plus_impl.obj \
	CCM_plus_skel.obj \
	plus.obj \
	plus_ValueType_impl.obj \
	StdPlusDll.obj \

ALL_CPPFLAGS	= /I. /D "STDDINNER_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(CPPFLAGS)
ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
ALL_LIBS	= $(OB_LIBS) $(OTS_LIBS) $(PSS_LIBS) $(CCM_LIBS) $(LIBS) GDI32.lib USER32.lib

$(DLL_NAME): $(OBJS)
	if exist "$@" \
	del "$@"
	$(LINK) /dll /incremental:no /pdb:"$(PDB_NAME)" /implib:"$(IMP_LIB)" $(LINKFLAGS) /out:$@ $(OBJS) $(ALL_LIBS)

install:: all

clean::

min:: all

install_min:: install

!if exist (".windep")
!include .windep
!endif