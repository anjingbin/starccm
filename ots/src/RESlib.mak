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

LIB_NAME	= $(top_srcdir)\lib\res$(LIBSUFFIX).lib
PDB_NAME	= $(top_srcdir)\lib\res$(LIBSUFFIX).pdb
PDB_FILES	= $(PDB_NAME)
PDB_FLAGS	= /Fd$(PDB_NAME)
TARGETS		= $(LIB_NAME)

!include $(top_srcdir)\config\Make.rules.mak

LIB_OBJS	= CosTransactionsOTS.obj \
		CosTransactionsUser.obj \
		PostgreSQLResource.obj \
		PostgreSQLResource_impl.obj \
		PostgreSQLResource_skel.obj \
		ReourceFactoryServer.obj \
		Resource.obj \
		Resource_impl.obj \
		Resource_skel.obj

ALL_CPPFLAGS	= /I. $(OTS_CPPFLAGS) $(OB_CPPFLAGS) $(CPPFLAGS) $(PSQL_INCLUDE)
ALL_IDLFLAGS	= -I$(OB_IDLDIR) $(OTS_IDLFLAGS)
ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)

$(LIB_NAME): $(LIB_OBJS)
	if exist "$@" \
	del "$@"
	$(AR) $(ARFLAGS) /out:$@ $(LIB_OBJS)

install:: all
	copy $(LIB_NAME) $(libdir)
#	if exist $(LIB_PDB_NAME) \
#	copy $(LIB_PDB_NAME) $(libdir)


clean::

min:: all

install_min:: install


mkdep::


!include .windep
