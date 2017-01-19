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

EXE_NAME	= $(top_srcdir)\bin\FileAccessorFactory.exe
PDB_NAME	= $(top_srcdir)\bin\FileAccessorFactory.pdb
PDB_FILES	= $(PDB_NAME) 
PDB_FLAGS	= /Fd$(PDB_NAME)		

TARGETS		= $(EXE_NAME)

IDLDIR		= $(top_srcdir)\idl
HDIR		= $(top_srcdir)\include

GENERATED	= FileAccessor.cpp \
		FileAccessor.h \
		FileAccessor_skel.cpp \
		FileAccessor_skel.h 
		                                
!include $(top_srcdir)\config\Make.rules.mak

!if "$(LITE_VERSION)" == "yes"
PSS_LIBS=
!endif


EXE_OBJS	= FileAccessor_impl.obj \
		FileAccessor_skel.obj \
		FileAccessorServer.obj  

ALL_CPPFLAGS	= /I. $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(CPPFLAGS)
ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
!if "$(ORB_NAME)" == "tao"
ALL_LIBS	= $(OB_LIBS) $(CCM_LIBS) $(LIBS) $(XERCES_LIB) $(TAO_ROOT)\tao\DynamicAny\TAO_DynamicAny$(LIBSUFFIX).lib $(TAO_NS_LIBS)
!else
ALL_LIBS	= $(OB_LIBS) $(CCM_LIBS) $(LIBS) $(XERCES_LIB)
!endif
# $(PSS_LIBS) $(OTS_LIBS)

$(EXE_NAME): $(EXE_OBJS) 
	if exist "$@" \
	del "$@"
	$(LINK) $(LINKFLAGS) /out:$@ \
	$(EXE_OBJS) $(ALL_LIBS)
	
FileAccessor.cpp FileAccessor.h FileAccessor_skel.cpp FileAccessor_skel.h: \
		$(IDLDIR)/FileAccessor.idl
	if exist $(IDL) \
		del FileAccessor.cpp & \
		del FileAccessor.h & \
		del FileAccessor_skel.cpp & \
		del FileAccessor_skel.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\FileAccessor.idl

install:: all
	copy $(EXE_NAME) $(bindir)
#	if exist $(PDB_NAME) \
#	copy $(PDB_NAME) $(bindir)



clean::

min:: all

install_min:: install

mkdep::

!include .windep
