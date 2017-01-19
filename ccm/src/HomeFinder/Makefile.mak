# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: An Jingbin	anjb@sina.com	
# **********************************************************************


top_srcdir	= ..\..

HF_NAME	= $(top_srcdir)\bin\HomeFinder.exe
HF_PDB_NAME	= $(top_srcdir)\bin\HomeFinder.pdb

PDB_FILES	= $(HF_PDB_NAME)

PDB_FLAGS	= /Fd$(HF_PDB_NAME)
TARGETS		= $(HF_NAME)

IDLDIR		= $(top_srcdir)\idl
HDIR		= $(top_srcdir)\include

GENERATED 	= HomeFinder_skel.cpp \
		HomeFinder_skel.h \
		HomeFinder_skel_tie.h \
		HomeRegistration_skel.cpp \
		HomeRegistration_skel.h \
		HomeRegistration_skel_tie.h \
		HomeFinder.cpp \
		HomeFinder.h \
		HomeRegistration.cpp \
		HomeRegistration.h
		
!include $(top_srcdir)\config\Make.rules.mak




HF_OBJS	= HomeFinder_impl_tie.obj \
	HomeFinder_skel.obj \
	HomeRegistration_skel.obj \
	Server.obj


ALL_CPPFLAGS	= /I. $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(CPPFLAGS)
ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)

!if "$(ORB_NAME)" == "tao"
ALL_LIBS	= $(OB_LIBS) $(CCM_LIBS) $(LIBS) $(OB_NS_LIBS) $(TAO_ROOT)\tao\DynamicAny\TAO_DynamicAny$(LIBSUFFIX).lib
!else
ALL_LIBS	= $(OB_LIBS) $(CCM_LIBS) $(LIBS)
!endif


$(HF_NAME): $(HF_OBJS)
	if exist "$@" \
	del "$@"
	$(LINK) $(LINKFLAGS) /out:$@ \
	$(HF_OBJS) $(ALL_LIBS)
	

!if "$(ORB_NAME)" == "tao"
HomeFinder_skel.cpp HomeFinder_skel.h HomeFinder_skel_tie.h: \
		$(IDLDIR)/HomeFinder.idl
	if exist $(IDL) \
		del HomeFinder_skel.cpp & \
		del HomeFinder_skel.h & \
		del HomeFinder_skel_tie.h & \
		$(IDL) $(TAO_IDLFLAGS_TIE) $(IDLDIR)\HomeFinder.idl


HomeRegistration_skel.cpp HomeRegistration_skel.h HomeRegistration_skel_tie.h: \
		$(IDLDIR)/HomeFinder.idl
	if exist $(IDL) \
		del HomeRegistration_skel.cpp & \
		del HomeRegistration_skel.h & \
		del HomeRegistration_skel_tie.h & \
		$(IDL) $(TAO_IDLFLAGS_TIE) $(IDLDIR)\HomeRegistration.idl
!else
HomeFinder_skel.cpp HomeFinder_skel.h HomeFinder_skel_tie.h: \
		$(IDLDIR)/HomeFinder.idl
	if exist $(IDL) \
		del HomeFinder_skel.cpp & \
		del HomeFinder_skel.h & \
		del HomeFinder_skel_tie.h & \
		$(IDL) $(ALL_IDLFLAGS) --tie $(IDLDIR)\HomeFinder.idl


HomeRegistration_skel.cpp HomeRegistration_skel.h HomeRegistration_skel_tie.h: \
		$(IDLDIR)/HomeFinder.idl
	if exist $(IDL) \
		del HomeRegistration_skel.cpp & \
		del HomeRegistration_skel.h & \
		del HomeRegistration_skel_tie.h & \
		$(IDL) $(ALL_IDLFLAGS) --tie $(IDLDIR)\HomeRegistration.idl
!endif

install:: all
	copy $(HF_NAME) $(bindir)
#	if exist $(HF_PDB_NAME) \
#	copy $(HF_PDB_NAME) $(bindir)



clean::

min:: all

install_min:: install

mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log
		
!if exist (".windep")
!include .windep
!endif