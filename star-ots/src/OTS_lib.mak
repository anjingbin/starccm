#  **********************************************************************
#
#  Copyright (c) 2001-2003 CCM Group
#
#  School of Computer Science,NUDT
#  Changsha,Hunan,China
#
#  All Rights Reserved
#
# **********************************************************************

# Version: 1.0.0

top_srcdir	= ..

LIB_NAME	= $(top_srcdir)\lib\ots_lib$(LIBSUFFIX).lib
PDB_NAME	= $(top_srcdir)\lib\ots_lib$(LIBSUFFIX).pdb
PDB_FILES	= $(PDB_NAME)
PDB_FLAGS	= /Fd$(PDB_NAME)
TARGETS		= $(LIB_NAME)

!include $(top_srcdir)\config\Make.rules.mak

LIB_OBJS	= CosTransactions.obj \
		CosTransactions_skel.obj \
		CosTransactionsCurrent.obj \
		CosTransactionsCurrent_impl.obj \
		CosTransactionsOTS.obj \
		CosTransactionsOTS_impl.obj \
		CosTransactionsOTS_skel.obj \
		CosTransactionsPolicy.obj \
		CosTransactionsPolicyType.obj \
		CosTransactionsUser.obj \
		CosTransactionsUser_skel.obj \
		CosTSInteroperation.obj \
		ImplicitOTSInterceptor.obj \
		OTSClientInterceptor_impl.obj \
		OTSIORInterceptor_impl.obj \
		OTSPolicyFactory_impl.obj \
		OTSServerInterceptor_impl.obj \
		PostgreSQLResource.obj \
		PostgreSQLResource_impl.obj \
		PostgreSQLResource_skel.obj \
		Resource.obj \
		Resource_impl.obj \
		Resource_skel.obj



ALL_CPPFLAGS	= /I. $(OTS_CPPFLAGS) $(STAR_CPPFLAGS) $(CPPFLAGS) $(PSQL_INCLUDE)
ALL_IDLFLAGS	= -I$(STAR_IDLDIR) $(OTS_IDLFLAGS)
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
