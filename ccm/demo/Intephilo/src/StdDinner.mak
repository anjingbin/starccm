# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: An Jingbin	anjb@sina.com
# **********************************************************************


top_srcdir	= ..\..\..

DLL_NAME	= ..\bin\StdDinner.dll
PDB_NAME	= ..\bin\StdDinner.pdb
IMP_LIB		= ..\bin\StdDinner.lib
EXP_NAME	= ..\bin\StdDinner.exp

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

OBJS	=   CCM_Dinner.obj \
	CCM_Dinner_impl.obj \
	CCM_Dinner_skel.obj \
	Dinner.obj \
	Dinner_ValueType_impl.obj \
	StdDinnerDll.obj \
	Observing.obj \
	Ph.res

RSC=rc.exe
RSC_PROJ=/l 0x804 /foPh.res /d "NDEBUG" 

ALL_CPPFLAGS	= /I. /D "STDDINNER_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(CPPFLAGS)
ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
ALL_LIBS	= $(OB_LIBS) $(OTS_LIBS) $(PSS_LIBS) $(CCM_LIBS) $(LIBS) GDI32.lib USER32.lib $(OB_NS_LIBS)

$(DLL_NAME): $(OBJS)
	if exist "$@" \
	del "$@"
	$(LINK) /dll /incremental:no /pdb:"$(PDB_NAME)" /implib:"$(IMP_LIB)" $(LINKFLAGS) /out:$@ $(OBJS) $(ALL_LIBS)

Ph.res: Ph.rc
	$(RSC) $(RSC_PROJ) Ph.rc
	
install:: all

clean::
	if exist Ph.res \
	del Ph.res

min:: all

install_min:: install

!if exist (".windep")
!include .windep
!endif