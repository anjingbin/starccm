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

LIB_NAME	= $(top_srcdir)\lib\otscurrent$(LIBSUFFIX).lib
PDB_NAME	= $(top_srcdir)\lib\otscurrent$(LIBSUFFIX).pdb
PDB_FILES	= $(PDB_NAME)
PDB_FLAGS	= /Fd$(PDB_NAME)
TARGETS		= $(LIB_NAME)

!include $(top_srcdir)\config\Make.rules.mak

LIB_OBJS	= CosTransactionsCurrent.obj \
		CosTransactionsCurrent_impl.obj \
		CosTransactionsPolicy.obj \
		CosTransactionsPolicyType.obj \
		CosTSInteroperation.obj \
		ImplicitOTSInterceptor.obj \
		OTSClientInterceptor_impl.obj \
		OTSIORInterceptor_impl.obj \
		OTSPolicyFactory_impl.obj \
		OTSServerInterceptor_impl.obj



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
