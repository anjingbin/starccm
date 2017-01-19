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

DLL_NAME	= ..\bin\StdWithdrawComp.dll
PDB_NAME	= ..\bin\StdWithdrawComp.pdb
IMP_LIB		= ..\bin\StdWithdrawComp.lib
EXP_NAME	= ..\bin\StdWithdrawComp.exp

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

OBJS	=   CCM_WithdrawComp.obj \
	CCM_WithdrawComp_impl.obj \
	CCM_WithdrawComp_skel.obj \
	WithdrawComp.obj \
	WithdrawComp_ValueType_impl.obj \
	StdWithdrawCompDll.obj \
	Account.obj \
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