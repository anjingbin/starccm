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

CCMLIB_NAME	= $(top_srcdir)\lib\ccm$(LIBSUFFIX).lib
CCMLIB_PDB_NAME	= $(top_srcdir)\lib\ccm$(LIBSUFFIX).pdb

PDB_FILES	= $(CCMLIB_PDB_NAME)

PDB_FLAGS	= /Fd$(CCMLIB_PDB_NAME)

TARGETS		= $(CCMLIB_NAME)

IDLDIR		= $(top_srcdir)\idl
HDIR		= $(top_srcdir)\include

GENERATED	= CCMCommon.cpp \
		$(HDIR)\CCMCommon.h \
		$(HDIR)\CCMCommon_skel.h \
		CCMCommon_skel.cpp \
		CCMHome.cpp \
		$(HDIR)\CCMHome.h \
		$(HDIR)\CCMHome_skel.h \
		CCMHome_skel.cpp \
		CCMObject.cpp \
		$(HDIR)\CCMObject.h \
		$(HDIR)\CCMObject_skel.h \
		CCMObject_skel.cpp \
		Components.cpp \
		$(HDIR)\Components.h \
		$(HDIR)\Components_skel.h \
		Components_skel.cpp \
		ComponentsClient.cpp \
		$(HDIR)\ComponentsClient.h \
		$(HDIR)\ComponentsClient_skel.h \
		ComponentsClient_skel.cpp \
		ComponentsPSS.cpp \
		$(HDIR)\ComponentsPSS.h \
		$(HDIR)\ComponentsPSS_skel.h \
		ComponentsPSS_skel.cpp \
		ComponentsPSS_pss.cpp \
		$(HDIR)\ComponentsPSS_pss.h \
		$(HDIR)\ComponentsPSS_psdl.h \
		ComponentsPSS_psdl.cpp \
		Configurator.cpp \
		$(HDIR)\Configurator.h \
		$(HDIR)\Configurator_skel.h \
		Configurator_skel.cpp \
		DeployDomainMgr.cpp \
		$(HDIR)\DeployDomainMgr.h \
		$(HDIR)\DeployDomainMgr_skel.h \
		DeployDomainMgr_skel.cpp \
		Deployment.cpp \
		$(HDIR)\Deployment.h \
		$(HDIR)\Deployment_skel.h \
		Deployment_skel.cpp \
		EJBean.cpp \
		$(HDIR)\EJBean.h \
		$(HDIR)\EJBean_skel.h \
		EJBean_skel.cpp \
		EnterpriseComponent.cpp \
		$(HDIR)\EnterpriseComponent.h \
		$(HDIR)\EnterpriseComponent_skel.h \
		EnterpriseComponent_skel.cpp \
		HomeFinder.cpp \
		$(HDIR)\HomeFinder.h \
		$(HDIR)\HomeFinder_skel.h \
		HomeFinder_skel.cpp \
		orb.cpp \
		$(HDIR)\orb.h \
		$(HDIR)\orb_skel.h \
		orb_skel.cpp \
		ResourcePool.cpp \
		$(HDIR)\ResourcePool.h \
		$(HDIR)\ResourcePool_skel.h \
		ResourcePool_skel.cpp \
		UserTransaction.cpp \
		$(HDIR)\UserTransaction.h \
		$(HDIR)\UserTransaction_skel.h \
		UserTransaction_skel.cpp \
		FileAccessor.cpp \
		$(HDIR)\FileAccessor.h \
		HomeRegistration.cpp \
		$(HDIR)\HomeRegistration.h \
		ArchObject.cpp \
		$(HDIR)\ArchObject.h \
		$(HDIR)\ArchObject_skel.h \
		ArchObject_skel.cpp
		
!include $(top_srcdir)\config\Make.rules.mak



!if "$(ORB_NAME)" == "tao"
CCMLIB_OBJS	= ccm.obj \
		CCMCommon.obj \
		CCMCommon_skel.obj \
		CCMCommonType_impl.obj \
		CCMHome.obj \
		CCMHome_impl.obj \
		CCMHome_skel.obj \
		CCMObject.obj \
		CCMObject_skel.obj \
		ComponentComposition.obj \
		Components.obj \
		Components_skel.obj \
		ComponentsClient.obj \
		ComponentsClient_skel.obj \
		ComponentServentBase.obj \
		CompositionManager.obj \
		Configurator.obj \
		Configurator_impl.obj \
		Configurator_skel.obj \
		DeployDomainMgr.obj \
		DeployDomainMgr_skel.obj \
		Deployment.obj \
		Deployment_skel.obj \
		EJBean.obj \
		EJBean_skel.obj \
		EnterpriseComponent.obj \
		EnterpriseComponent_impl.obj \
		EnterpriseComponent_skel.obj \
		ExecutorSegmentBase.obj \
		FileAccessor.obj \
		HomeComposition.obj \
		HomeExecutorBase.obj \
		HomeFinder.obj \
		HomeFinder_skel.obj \
		HomeRegistration.obj \
		md5c.obj \
		orb.obj \
		orb_skel.obj \
		ResourcePool.obj \
		ResourcePool_skel.obj \
		sysdep.obj \
		Trace.obj \
		UserTransaction.obj \
		UserTransaction_skel.obj \
		Util.obj \
		uuid.obj \
		UuidGenerator.obj \
		XMLParser.obj \
		CosTransactions.obj \
		CosTransactionsCurrent.obj \
		CosTransactionsOTS.obj \
		CommonFunc.obj
!else if "$(LITE_VERSION)" == "yes"
CCMLIB_OBJS	= ccm.obj \
		CCMCommon.obj \
		CCMCommon_skel.obj \
		CCMCommonType_impl.obj \
		CCMHome.obj \
		CCMHome_impl.obj \
		CCMHome_skel.obj \
		CCMObject.obj \
		CCMObject_skel.obj \
		ComponentComposition.obj \
		Components.obj \
		Components_skel.obj \
		ComponentsClient.obj \
		ComponentsClient_skel.obj \
		ComponentServentBase.obj \
		CompositionManager.obj \
		Configurator.obj \
		Configurator_impl.obj \
		Configurator_skel.obj \
		DeployDomainMgr.obj \
		DeployDomainMgr_skel.obj \
		Deployment.obj \
		Deployment_skel.obj \
		EJBean.obj \
		EJBean_skel.obj \
		EnterpriseComponent.obj \
		EnterpriseComponent_impl.obj \
		EnterpriseComponent_skel.obj \
		ExecutorSegmentBase.obj \
		FileAccessor.obj \
		HomeComposition.obj \
		HomeExecutorBase.obj \
		HomeFinder.obj \
		HomeFinder_skel.obj \
		HomeRegistration.obj \
		md5c.obj \
		orb.obj \
		orb_skel.obj \
		ResourcePool.obj \
		ResourcePool_skel.obj \
		sysdep.obj \
		Trace.obj \
		UserTransaction.obj \
		UserTransaction_skel.obj \
		Util.obj \
		uuid.obj \
		UuidGenerator.obj \
		XMLParser.obj \
		ArchObject.obj \
		ArchObject_skel.obj \
		CommonFunc.obj
!else
CCMLIB_OBJS	= ccm.obj \
		CCMCommon.obj \
		CCMCommon_skel.obj \
		CCMCommonType_impl.obj \
		CCMHome.obj \
		CCMHome_impl.obj \
		CCMHome_skel.obj \
		CCMObject.obj \
		CCMObject_skel.obj \
		ComponentComposition.obj \
		Components.obj \
		Components_skel.obj \
		ComponentsClient.obj \
		ComponentsClient_skel.obj \
		ComponentServentBase.obj \
		ComponentsPSS.obj \
		ComponentsPSS_psdl.obj \
		ComponentsPSS_pss.obj \
		ComponentsPSS_skel.obj \
		CompositionManager.obj \
		Configurator.obj \
		Configurator_impl.obj \
		Configurator_skel.obj \
		DeployDomainMgr.obj \
		DeployDomainMgr_skel.obj \
		Deployment.obj \
		Deployment_skel.obj \
		EJBean.obj \
		EJBean_skel.obj \
		EnterpriseComponent.obj \
		EnterpriseComponent_impl.obj \
		EnterpriseComponent_skel.obj \
		ExecutorSegmentBase.obj \
		FileAccessor.obj \
		HomeComposition.obj \
		HomeExecutorBase.obj \
		HomeFinder.obj \
		HomeFinder_skel.obj \
		HomeRegistration.obj \
		md5c.obj \
		orb.obj \
		orb_skel.obj \
		ResourcePool.obj \
		ResourcePool_skel.obj \
		sysdep.obj \
		Trace.obj \
		UserTransaction.obj \
		UserTransaction_skel.obj \
		Util.obj \
		uuid.obj \
		UuidGenerator.obj \
		XMLParser.obj \
		ArchObject.obj \
		ArchObject_skel.obj\
		CommonFunc.obj
!endif
		



ALL_CPPFLAGS	= /I. $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(CPPFLAGS)

!if "$(DLL)" == "yes"

ALL_CPPFLAGS = $(ALL_CPPFLAGS) /UDLL_LIB

ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS) --dll-import CCM_IMPORT
!else
ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS)
!endif

ALL_PSDLFLAGS	= -I$(OB_IDLDIR) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)

ALL_CFLAGS	= /I. $(CCM_CFLAGS) $(CFLAGS)

DEF_NAME	= ccm$(LIBSUFFIX).def
DLL_NAME	= $(top_srcdir)\lib\ccm$(OB_LIBVER)$(LIBSUFFIX).dll
EXP_NAME	= $(top_srcdir)\lib\ccm$(LIBSUFFIX).exp



!if "$(DLL)" == "yes"

XERCES_LIB = ..\..\..\xercesc\xerces-c1_7_0-win32\lib\xerces-c_1D.lib

!if "$(ORB_NAME)" != "tao"
TAO_NS_LIBS=
!endif

!if "$(LITE_VERSION)" == "yes"
OTS_LIBS=
!endif

ALL_DLLLIBS	= $(OB_LIBS) $(XERCES_LIB)  $(TAO_NS_LIBS) $(OTS_LIBS)

$(CCMLIB_NAME): $(DLL_NAME)

$(DLL_NAME): $(CCMLIB_OBJS)
	perl ..\..\..\config\makedef.pl $(DEF_NAME) \
	ccm$(OB_LIBVER)$(LIBSUFFIX) $(CCMLIB_OBJS) 
	$(LINK) $(LINKDLLFLAGS) /BASE:0x64000000 /def:$(DEF_NAME) \
	/IMPLIB:$(CCMLIB_NAME) /out:$@ $(CCMLIB_OBJS) $(ALL_DLLLIBS)
	copy $(DLL_NAME)  $(top_srcdir)\bin\ccm$(OB_LIBVER)$(LIBSUFFIX).dll

!else

$(CCMLIB_NAME): $(CCMLIB_OBJS)
	if exist "$@" \
	del "$@"
	$(AR) $(ARFLAGS) /out:$@ $(CCMLIB_OBJS)
!endif

CCMCommon.cpp $(HDIR)\CCMCommon.h CCMCommon_skel: \
		$(IDLDIR)/CCMCommon.idl
	if exist $(IDL) \
		del CCMCommon.cpp & \
		del $(HDIR)\CCMCommon.h & \
		del $(HDIR)\CCMCommon_skel.h & \
		del CCMCommon_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\CCMCommon.idl & \
		move CCMCommon.h $(HDIR) & \
		move CCMCommon_skel.h $(HDIR)
		
CCMHome.cpp $(HDIR)\CCMHome.h CCMHome_skel.cpp: \
		$(IDLDIR)/CCMHome.idl
	if exist $(IDL) \
		del CCMHome.cpp & \
		del $(HDIR)\CCMHome.h & \
		del $(HDIR)\CCMHome_skel.h & \
		del CCMHome_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\CCMHome.idl & \
		move CCMHome.h $(HDIR) & \
		move CCMHome_skel.h $(HDIR)

CCMObject.cpp $(HDIR)\CCMObject.h CCMObject_skel.cpp: \
		$(IDLDIR)/CCMObject.idl
	if exist $(IDL) \
		del CCMObject.cpp & \
		del $(HDIR)\CCMObject.h & \
		del $(HDIR)\CCMObject_skel.h & \
		del CCMObject_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\CCMObject.idl & \
		move CCMObject.h $(HDIR) & \
		move CCMObject_skel.h $(HDIR)
			
ArchObject.cpp $(HDIR)\ArchObject.h ArchObject_skel.cpp: \
		$(IDLDIR)/ArchObject.idl
	if exist $(IDL) \
		del ArchObject.cpp & \
		del $(HDIR)\ArchObject.h & \
		del $(HDIR)\ArchObject_skel.h & \
		del ArchObject_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\ArchObject.idl & \
		move ArchObject.h $(HDIR) & \
		move ArchObject_skel.h $(HDIR)

Components.cpp $(HDIR)\Components.h Components_skel.cpp: \
		$(IDLDIR)/Components.idl
	if exist $(IDL) \
		del Components.cpp & \
		del $(HDIR)\Components.h & \
		del $(HDIR)\Components_skel.h & \
		del Components_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\Components.idl & \
		move Components.h $(HDIR) & \
		move Components_skel.h $(HDIR)		
		
ComponentsClient.cpp $(HDIR)\ComponentsClient.h ComponentsClient_skel.cpp: \
		$(IDLDIR)/ComponentsClient.idl
	if exist $(IDL) \
		del ComponentsClient.cpp & \
		del $(HDIR)\ComponentsClient.h & \
		del $(HDIR)\ComponentsClient_skel.h & \
		del ComponentsClient_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\ComponentsClient.idl & \
		move ComponentsClient.h $(HDIR) & \
		move ComponentsClient_skel.h $(HDIR)

ComponentsPSS.cpp $(HDIR)\ComponentsPSS.h ComponentsPSS_skel.cpp: \
		$(IDLDIR)/ComponentsPSS.idl
	if exist $(IDL) \
		del ComponentsPSS.cpp & \
		del $(HDIR)\ComponentsPSS.h & \
		del $(HDIR)\ComponentsPSS_skel.h & \
		del ComponentsPSS_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\ComponentsPSS.idl & \
		move ComponentsPSS.h $(HDIR) & \
		move ComponentsPSS_skel.h $(HDIR)

ComponentsPSS_psdl.cpp $(HDIR)\ComponentsPSS_psdl.h ComponentsPSS_pss.cpp: \
		$(IDLDIR)/ComponentsPSS.psdl
	if exist $(PSDL) \
		del ComponentsPSS_pss.cpp & \
		del $(HDIR)\ComponentsPSS_pss.h & \
		del $(HDIR)\ComponentsPSS_psdl.h & \
		del ComponentsPSS_psdl.cpp & \
		$(PSDL) $(ALL_PSDLFLAGS) $(IDLDIR)\ComponentsPSS.psdl & \
		move ComponentsPSS_pss.h $(HDIR) & \
		move ComponentsPSS_psdl.h $(HDIR)
		
Configurator.cpp $(HDIR)\Configurator.h Configurator_skel.cpp: \
		$(IDLDIR)/Configurator.idl
	if exist $(IDL) \
		del Configurator.cpp & \
		del $(HDIR)\Configurator.h & \
		del $(HDIR)\Configurator_skel.h & \
		del Configurator_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\Configurator.idl & \
		move Configurator.h $(HDIR) & \
		move Configurator_skel.h $(HDIR)

DeployDomainMgr.cpp $(HDIR)\DeployDomainMgr.h DeployDomainMgr_skel.cpp: \
		$(IDLDIR)/DeployDomainMgr.idl
	if exist $(IDL) \
		del DeployDomainMgr.cpp & \
		del $(HDIR)\DeployDomainMgr.h & \
		del $(HDIR)\DeployDomainMgr_skel.h & \
		del DeployDomainMgr_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\DeployDomainMgr.idl & \
		move DeployDomainMgr.h $(HDIR) & \
		move DeployDomainMgr_skel.h $(HDIR)	
		
Deployment.cpp $(HDIR)\Deployment.h Deployment_skel.cpp: \
		$(IDLDIR)/Deployment.idl
	if exist $(IDL) \
		del Deployment.cpp & \
		del $(HDIR)\Deployment.h & \
		del $(HDIR)\Deployment_skel.h & \
		del Deployment_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\Deployment.idl & \
		move Deployment.h $(HDIR) & \
		move Deployment_skel.h $(HDIR)		

EJBean.cpp $(HDIR)\EJBean.h EJBean_skel.cpp: \
		$(IDLDIR)/EJBean.idl
	if exist $(IDL) \
		del EJBean.cpp & \
		del $(HDIR)\EJBean.h & \
		del $(HDIR)\EJBean_skel.h & \
		del EJBean_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\EJBean.idl & \
		move EJBean.h $(HDIR) & \
		move EJBean_skel.h $(HDIR)


EnterpriseComponent.cpp $(HDIR)\EnterpriseComponent.h EnterpriseComponent_skel.cpp: \
		$(IDLDIR)/EnterpriseComponent.idl
	if exist $(IDL) \
		del EnterpriseComponent.cpp & \
		del $(HDIR)\EnterpriseComponent.h & \
		del $(HDIR)\EnterpriseComponent_skel.h & \
		del EnterpriseComponent_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\EnterpriseComponent.idl & \
		move EnterpriseComponent.h $(HDIR) & \
		move EnterpriseComponent_skel.h $(HDIR)
		
HomeFinder.cpp $(HDIR)\HomeFinder.h HomeFinder_skel.cpp: \
		$(IDLDIR)/HomeFinder.idl
	if exist $(IDL) \
		del HomeFinder.cpp & \
		del $(HDIR)\HomeFinder.h & \
		del $(HDIR)\HomeFinder_skel.h & \
		del HomeFinder_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\HomeFinder.idl & \
		move HomeFinder.h $(HDIR) & \
		move HomeFinder_skel.h $(HDIR)

orb.cpp $(HDIR)\orb.h orb_skel.cpp: \
		$(IDLDIR)/orb.idl
	if exist $(IDL) \
		del orb.cpp & \
		del $(HDIR)\orb.h & \
		del $(HDIR)\orb_skel.h & \
		del orb_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\orb.idl & \
		move orb.h $(HDIR) & \
		move orb_skel.h $(HDIR)

ResourcePool.cpp $(HDIR)\ResourcePool.h ResourcePool_skel.cpp: \
		$(IDLDIR)/ResourcePool.idl
	if exist $(IDL) \
		del ResourcePool.cpp & \
		del $(HDIR)\ResourcePool.h & \
		del $(HDIR)\ResourcePool_skel.h & \
		del ResourcePool_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\ResourcePool.idl & \
		move ResourcePool.h $(HDIR) & \
		move ResourcePool_skel.h $(HDIR)

UserTransaction.cpp $(HDIR)\UserTransaction.h UserTransaction_skel.cpp: \
		$(IDLDIR)/UserTransaction.idl
	if exist $(IDL) \
		del UserTransaction.cpp & \
		del $(HDIR)\UserTransaction.h & \
		del $(HDIR)\UserTransaction_skel.h & \
		del UserTransaction_skel.cpp & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\UserTransaction.idl & \
		move UserTransaction.h $(HDIR) & \
		move UserTransaction_skel.h $(HDIR)

!if "$(ORB_NAME)" == "ob"
FileAccessor.cpp $(HDIR)\FileAccessor.h: \
		$(IDLDIR)/FileAccessor.idl
	if exist $(IDL) \
		del FileAccessor.cpp & \
		del $(HDIR)\FileAccessor.h & \
		$(IDL) $(ALL_IDLFLAGS) --no-skeletons $(IDLDIR)\FileAccessor.idl & \
		move FileAccessor.h $(HDIR) & 

HomeRegistration.cpp $(HDIR)\HomeRegistration.h: \
		$(IDLDIR)/HomeRegistration.idl
	if exist $(IDL) \
		del HomeRegistration.cpp & \
		del $(HDIR)\HomeRegistration.h & \
		$(IDL) $(ALL_IDLFLAGS) --no-skeletons $(IDLDIR)\HomeRegistration.idl & \
		move HomeRegistration.h $(HDIR) &
!endif

!if "$(ORB_NAME)" == "star"
FileAccessor.cpp $(HDIR)\FileAccessor.h: \
		$(IDLDIR)/FileAccessor.idl
	if exist $(IDL) \
		del FileAccessor.cpp & \
		del $(HDIR)\FileAccessor.h & \
		$(IDL) $(ALL_IDLFLAGS) --no-skel $(IDLDIR)\FileAccessor.idl & \
		move FileAccessor.h $(HDIR) & 

HomeRegistration.cpp $(HDIR)\HomeRegistration.h: \
		$(IDLDIR)/HomeRegistration.idl
	if exist $(IDL) \
		del HomeRegistration.cpp & \
		del $(HDIR)\HomeRegistration.h & \
		$(IDL) $(ALL_IDLFLAGS) --no-skel $(IDLDIR)\HomeRegistration.idl & \
		move HomeRegistration.h $(HDIR) &
!endif

!if "$(ORB_NAME)" == "tao"
FileAccessor.cpp $(HDIR)\FileAccessor.h: \
		$(IDLDIR)/FileAccessor.idl
	if exist $(IDL) \
		del FileAccessor.cpp & \
		del $(HDIR)\FileAccessor.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\FileAccessor.idl & \
		del /Q FileAccessor_skel.* &\
		move FileAccessor.h $(HDIR) & 

HomeRegistration.cpp $(HDIR)\HomeRegistration.h: \
		$(IDLDIR)/HomeRegistration.idl
	if exist $(IDL) \
		del HomeRegistration.cpp & \
		del $(HDIR)\HomeRegistration.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\HomeRegistration.idl & \
		del /Q HomeRegistration_skel.* &\
		move HomeRegistration.h $(HDIR) &
CosTransactions.cpp $(HDIR)\CosTransactions.h: \
		$(IDLDIR)/tao/CosTransactions.idl
	if exist $(IDL) \
		del CosTransactions.cpp & \
		del $(HDIR)\CosTransactions.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\tao\CosTransactions.idl & \
		del /Q CosTransactions_skel.cpp & \
		move CosTransactions.h $(HDIR) & \
		move /Y *.i $(HDIR)
CosTransactionsOTS.cpp $(HDIR)\CosTransactionsOTS.h: \
		$(IDLDIR)/tao/CosTransactionsOTS.idl
	if exist $(IDL) \
		del CosTransactionsOTS.cpp & \
		del $(HDIR)\CosTransactionsOTS.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\tao\CosTransactionsOTS.idl & \
		del /Q CosTransactionsOTS_skel.cpp &\
		move CosTransactionsOTS.h $(HDIR) & \
		move /Y *.i $(HDIR)
CosTransactionsCurrent.cpp $(HDIR)\CosTransactionsCurrent.h: \
		$(IDLDIR)/tao/CosTransactionsCurrent.idl
	if exist $(IDL) \
		del CosTransactionsCurrent.cpp & \
		del $(HDIR)\CosTransactionsCurrent.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\tao\CosTransactionsCurrent.idl & \
		del /Q CosTransactionsCurrent_skel.cpp &\
		move CosTransactionsCurrent.h $(HDIR) & \
		move /Y *.i $(HDIR)
!endif

	
install:: all
	copy $(CCMLIB_NAME) $(libdir)
#	copy $(DLL_NAME) $(bindir)
#	if exist $(CCMLIB_PDB_NAME) \
#	copy $(CCMLIB_PDB_NAME) $(libdir)


clean::

min:: all

install_min:: install

mkdep::
	$(MKDEP) /V /Q /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp *.c
	@if exist MkDep.log \
		@del MkDep.log

!if exist (".windep")
!include .windep
!endif