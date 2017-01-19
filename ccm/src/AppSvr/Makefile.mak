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

APPSVR_NAME	= $(top_srcdir)\bin\ComponentServer.exe
APPSVR_PDB_NAME	= $(top_srcdir)\bin\ComponentServer.pdb

PDB_FILES	= $(APPSVR_PDB_NAME)

PDB_FLAGS	= /Fd$(APPSVR_PDB_NAME)
TARGETS		= $(APPSVR_NAME)


IDLDIR		= $(top_srcdir)\idl
HDIR		= $(top_srcdir)\include


GENERATED	= RunTime.cpp \
		RunTime.h \
		RunTime_skel.cpp \
		RunTime_skel.h \
		RunTime_pss.cpp \
		RunTime_pss.h \
		RunTime_psdl.cpp \
		RunTime_psdl.h \
		Manager.cpp \
		Manager.h \
		Manager_skel.cpp \
		Manager_skel.h 
		
!include $(top_srcdir)\config\Make.rules.mak

!if "$(LITE_VERSION)" == "yes"
APPSVR_OBJS	= CCM2Context_impl.obj \
		CCMContext_impl.obj \
		CIFHook.obj \
		ComponentServer_impl.obj \
		ComponentServerServer.obj \
		ComponentsPool.obj \
		ConfigFile.obj \
		Container_impl.obj \
		ContainerBase.obj \
		ContainerRunTime.obj \
		Context2_impl.obj \
		Cookie.obj \
		Entity2Context_impl.obj \
		EntityContext_impl.obj \
		ExecutorInvoker.obj \
		ExecutorLoader.obj \
		HomeRegistration_impl.obj \
		Loader.obj \
		PassivationArbitor.obj \
		ResourcePool_impl.obj \
		ServantLoader.obj \
		Session2Context_impl.obj \
		SessionContext_impl.obj \
		SynchronizationManager.obj \
		TxnInterceptor.obj \
		UserTransaction_impl.obj\
		CustomThreadPool.obj\
		LinkedRequestQueue.obj \
		ComponentManager_impl.obj \
		TransactionManager_impl.obj \
		Manager_skel.obj \
		Manager.obj
		

!else
APPSVR_OBJS	= CCM2Context_impl.obj \
		CCMContext_impl.obj \
		CIFHook.obj \
		ComponentServer_impl.obj \
		ComponentServerServer.obj \
		ComponentsPool.obj \
		ConfigFile.obj \
		Container_impl.obj \
		ContainerBase.obj \
		ContainerRunTime.obj \
		Context2_impl.obj \
		Cookie.obj \
		Entity2Context_impl.obj \
		EntityContext_impl.obj \
		ExecutorInvoker.obj \
		ExecutorLoader.obj \
		HomeRegistration_impl.obj \
		Loader.obj \
		PassivationArbitor.obj \
		PSSInitializer.obj \
		ResourcePool_impl.obj \
		RunTime.obj \
		RunTime_psdl.obj \
		RunTime_pss.obj \
		RunTime_skel.obj \
		ServantLoader.obj \
		Session2Context_impl.obj \
		SessionContext_impl.obj \
		StorageObjectLoader.obj \
		SynchronizationManager.obj \
		TxnInterceptor.obj \
		UserTransaction_impl.obj\
		CustomThreadPool.obj\
		LinkedRequestQueue.obj \
		ComponentManager_impl.obj \
		TransactionManager_impl.obj \
		Manager_skel.obj\
		Manager.obj

!endif

!if "$(WITH_OTS)" == "yes"
ALL_CPPFLAGS	= /I. $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(CPPFLAGS) /DWITH_OTS
!else
ALL_CPPFLAGS	= /I. $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(PSS_CPPFLAGS) $(OTS_CPPFLAGS) $(CPPFLAGS)
!endif 

ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS)
ALL_PSDLFLAGS	= -I$(OB_IDLDIR) $(OTS_IDLFLAGS) $(PSS_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)

!if "$(LITE_VERSION)" == "yes"
PSS_LIBS=
!endif

!if "$(ORB_NAME)" == "tao"
ALL_LIBS	= $(OB_LIBS) $(CCM_LIBS) $(LIBS) $(XERCES_LIB) $(TAO_ROOT)\tao\DynamicAny\TAO_DynamicAny$(LIBSUFFIX).lib $(TAO_NS_LIBS)
!else
ALL_LIBS	= $(OB_LIBS) $(CCM_LIBS) $(LIBS) $(PSS_LIBS) $(OTS_LIBS) $(PSQL_LIBS) $(XERCES_LIB)
!endif


$(APPSVR_NAME): $(APPSVR_OBJS) 
	if exist "$@" \
	del "$@"
	$(LINK) $(LINKFLAGS) /out:$@ \
	$(APPSVR_OBJS) $(ALL_LIBS)
	
RunTime.cpp RunTime.h RunTime_skel.cpp: \
		$(IDLDIR)/RunTime.idl
	if exist $(IDL) \
		del RunTime.cpp & \
		del RunTime.h & \
		del RunTime_skel.cpp & \
		del RunTime_skel.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\RunTime.idl
		
RunTime_pss.cpp RunTime_pss.h RunTime_psdl.cpp RunTime_psdl.h: \
		$(IDLDIR)/RunTime.psdl
	if exist $(PSDL) \
		del RunTime_pss.h & \
		del RunTime_pss.cpp & \
		del RunTime_psdl.h & \
		del RunTime_psdl.cpp & \
		$(PSDL) $(ALL_PSDLFLAGS) $(IDLDIR)\RunTime.psdl
		
Manager.cpp Manager.h Manager_skel.cpp Manager_skel.h: \
		$(IDLDIR)/Manager.idl
	if exist $(IDL) \
		del Manager.cpp & \
		del Manager.h & \
		del Manager_skel.cpp & \
		del Manager_skel.h & \
		$(IDL) $(ALL_IDLFLAGS) $(IDLDIR)\Manager.idl
  
install:: all
	copy $(APPSVR_NAME) $(bindir)
#	if exist $(APPSVR_PDB_NAME) \
#	copy $(APPSVR_PDB_NAME) $(bindir)


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