# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: Huang Jie	huangjie@email.com
# **********************************************************************


top_srcdir	= ..\..\..

DLL_NAME	= ..\bin\StdManager.dll
PDB_NAME	= ..\bin\StdManager.pdb
IMP_LIB		= ..\bin\StdManager.lib
EXP_NAME	= ..\bin\StdManager.exp

PDB_FILES	= $(PDB_NAME)

PDB_FLAGS	= /Fd$(PDB_NAME)

#modified by xiao heping 2004/06/29

TARGETS		= $(DLL_NAME) 

IDLDIR		= $(top_srcdir)\idl
HDIR		= $(top_srcdir)\include


GENERATED	=

!include $(top_srcdir)\config\Make.rules.mak

!if "$(DEBUG)" == "yes"
ALL_CPPFLAGS	= /I. /D "STDDINNER_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(CPPFLAGS)
all:: $(PDB_NAME) $(IMP_LIB) $(EXP_NAME)
!else
ALL_CPPFLAGS	= /I. /D "STDDINNER_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(CPPFLAGS)
!endif
#end modify 2004/06/29

!if "$(LITE_VERSION)" == "yes"
PSS_LIBS=
!endif

OBJS	=   CCM_Manager.obj \
	CCM_Manager_impl.obj \
	CCM_Manager_skel.obj \
	Manager.obj \
	Manager_ValueType_impl.obj \
	StdManagerDll.obj \
	OpenAccountComp.obj \
	DelAccountComp.obj \
	ATM.obj \
	Account.obj \
	Authentication.obj \
	DepositComp.obj \
	WithdrawComp.obj \
	QueryComp.obj \
	InterestComputer.obj

ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
ALL_LIBS	= $(OB_LIBS) $(OTS_LIBS) $(PSS_LIBS) $(CCM_LIBS) $(LIBS)

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