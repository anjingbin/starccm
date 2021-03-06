# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: Ren Yi    starots@sohu.com	
# **********************************************************************

top_srcdir	= ..

EXE_NAME	= $(top_srcdir)\bin\res.exe
PDB_NAME	= $(top_srcdir)\bin\res.pdb
PDB_FILES	= $(PDB_NAME)
PDB_FLAGS	= /Fd$(PDB_NAME)

TARGETS		= $(EXE_NAME)

!include $(top_srcdir)\config\Make.rules.mak

EXE_OBJS	= CosTransactionsOTS.obj \
		CosTransactionsUser.obj \
		PostgreSQLResource.obj \
		PostgreSQLResource_impl.obj \
		PostgreSQLResource_skel.obj \
		ResourceFactoryServer.obj \
		Resource.obj \
		Resource_impl.obj \
		Resource_skel.obj



ALL_CPPFLAGS	= /I. $(OTS_CPPFLAGS) $(STAR_CPPFLAGS) $(CPPFLAGS) $(PSQL_INCLUDE)
ALL_IDLFLAGS	= -I$(STAR_IDLDIR) $(OTS_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
ALL_LIBS	= $(STAR_LIBS) $(LIBS) $(PSQL_LIBS) $(OTS_LIBS)

$(EXE_NAME): $(EXE_OBJS)
	if exist "$@" \
	del "$@"
	$(LINK) $(LINKFLAGS) /out:$@ \
	$(EXE_OBJS) $(ALL_LIBS)

install:: all
	copy $(EXE_NAME) $(bindir)
#	if exist $(LIB_PDB_NAME) \
#	copy $(LIB_PDB_NAME) $(libdir)


clean::

min:: all

install_min:: install


mkdep::


!include .windep
