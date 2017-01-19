#  **********************************************************************
#
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: An Jingbin	anjb@sina.com	
# **********************************************************************

top_srcdir	= ..

LIB_NAME 	= $(top_srcdir)\lib\pss$(LIBSUFFIX).lib
PDB_NAME	= $(top_srcdir)\lib\pss$(LIBSUFFIX).pdb
DLL_NAME 	= $(top_srcdir)\lib\pss$(LIBSUFFIX).dll
PDB_FILES	= $(PDB_NAME)
PDB_FLAGS       = /Fd$(PDB_NAME)

TARGETS		= $(LIB_NAME)


IDLDIR		= $(top_srcdir)\idl
HDIR		= $(top_srcdir)\include


GENERATED	= CosPersistentState.cpp \
		CosPersistentState_skel.cpp \
		$(HDIR)\CosPersistentState.h \
		$(HDIR)\CosPersistentState_skel.h

		
!include $(top_srcdir)\config\Make.rules.mak


OBJS		= ConnectorRegistry_impl.obj \
		StorageObject.obj \
		CosPersistentState.obj \
		ConnectorBase.obj \
		DatabaseCatalog.obj \
		DatabaseConnector.obj \
		DatabaseSession.obj \
		PersistentHome.obj \
		PersistentObject.obj \
		PostgreSQLConnector.obj \
		PSSInit.obj \
		TransactionalDatabaseSession.obj \
		PostgreSQLSession.obj \
		PID.obj \
		Utility.obj \
		PSS_Error.obj \
		md5c.obj \
		sysdep.obj \
		uuid.obj


DEF_NAME = pss.def

	
!if "$(DLL)" == "yes"

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
ALL_CPPFLAGS	= /DOB_BUILD_DLL /I. $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(PSQL_INCLUDE) $(CPPFLAGS)
ALL_DLLLIBS	= $(OB_DLLLIBS) $(DLLLIBS)
ALL_CFLAGS	= /I. $(PSS_CFLAGS) $(CFLAGS) $(PDB_FLAGS)

$(NAME): $(DLL_NAME)
$(DEF_NAME):
$(DLL_NAME) : $(OBJS) $(DEF_NAME)
	$(LINK) $(LINKDLLFLAGS) /def:$(DEF_NAME) \
	/IMPLIB:$(NAME) /out:$@ $(LIB_OBJS) $(ALL_DLLLIBS)

!else

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
ALL_CPPFLAGS	= /I. /DWININC $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(PSQL_INCLUDE) $(CPPFLAGS)
ALL_CFLAGS	= /I. /DWININC $(PSS_CFLAGS) $(CFLAGS) $(PDB_FLAGS)
$(LIB_NAME): $(OBJS)
	$(AR) $(ARFLAGS) /out:$@ $(OBJS)

!endif

ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS)

CosPersistentState.cpp CosPersistentState_skel.cpp $(HDIR)\CosPersistentState.h $(HDIR)\CosPersistentState_skel.h: \
		$(IDLDIR)/CosPersistentState.idl
	if exist $(IDL) \
		del CosPersistentState.cpp & \
		del CosPersistentState_skel.cpp & \
		del $(HDIR)\CosPersistentState.h & \
		del $(HDIR)\CosPersistentState_skel.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\CosPersistentState.idl & \
		move CosPersistentState.h $(HDIR)\CosPersistentState.h & \
		move CosPersistentState_skel.h $(HDIR)\CosPersistentState_skel.h & \

mkdep::
	$(MKDEP) /V /Q /W /E /O.windep /I. $(PSS_CPPFLAGS) *.cpp *.c
	@if exist MkDep.log \
		@del MkDep.log

install:: all
	copy $(LIB_NAME) $(libdir)
#	if exist $(PDB_NAME) \
#	copy $(PDB_NAME) $(libdir)

!if exist (".windep")
!include .windep
!endif
