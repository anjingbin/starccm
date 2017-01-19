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

EXE_NAME	= $(top_srcdir)\bin\Assembly.exe
PDB_NAME	= $(top_srcdir)\bin\Assembly.pdb
PDB_FILES	= $(PDB_NAME) 
PDB_FLAGS	= /Fd$(PDB_NAME)
TARGETS		= $(EXE_NAME)

IDLDIR		= $(top_srcdir)\idl
                               
!include $(top_srcdir)\config\Make.rules.mak

!if "$(LITE_VERSION)" == "yes"
PSS_LIBS=
!endif

EXE_OBJS	= Assembly_impl.obj \
		AssemblyServer.obj \
		Manager_skel.obj \
		Manager.obj \
		Architecture.obj
		
			
                                   
ALL_CPPFLAGS	= /I. $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS)
ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
!if "$(ORB_NAME)" == "tao"
ALL_LIBS	= $(OB_LIBS) $(OB_NS_LIBS) $(CCM_LIBS) $(LIBS) $(XERCES_LIB) $(TAO_ROOT)\tao\DynamicAny\TAO_DynamicAny$(LIBSUFFIX).lib $(TAO_NS_LIBS)
!else
ALL_LIBS	= $(OB_LIBS) $(OB_NS_LIBS) $(CCM_LIBS) $(LIBS) $(PSS_LIBS) $(OTS_LIBS) $(XERCES_LIB)
!endif

Manager.cpp Manager.h Manager_skel.cpp: \
		$(IDLDIR)/Manager.idl
	if exist $(IDL) \
		del Manager.cpp & \
		del Manager.h & \
		del Manager_skel.cpp & \
		del Manager_skel.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\Manager.idl


$(EXE_NAME): $(EXE_OBJS) 
	if exist "$@" \
	del "$@"
	$(LINK) $(LINKFLAGS) /out:$@ \
	$(EXE_OBJS) $(ALL_LIBS)

install:: all
	copy $(EXE_NAME) $(bindir)
#	if exist $(PDB_NAME) \
#	copy $(PDB_NAME) $(bindir)

clean::

min:: all

install_min:: install


mkdep::


!include .windep
