# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: Zheng Di	sonofwind0109@sina.com
# **********************************************************************



top_srcdir	= ..\..\..\..

GENERATED 	= Philosopher.cpf \
                PhilosopherHome.cpf \
                Observer.cpf \
                ObserverHome.cpf \
                test.idl \
                test_impl.cpp \
                test_impl.h \
                test_ValueType_impl.cpp \
                test_ValueType_impl.h \
                testDll.cpp \
                test.cpp \
                test.h \
                test_skel.cpp \
                test_skel.h \
                CCM_test.idl \
                CCM_test_Context_impl.cpp \
                CCM_test_Context_impl.h \
                CCM_test_skel.cpp \
                CCM_test_skel.h \
                Philosopher.ccd \
                StdtestDll.cpp \
                Observer.ccd \
                CCM_test.cpp \
                CCM_test.h \
                test_psdl.cpp \
                test_psdl.h \
                test_pss.cpp \
                test_pss.h 

!include $(top_srcdir)\config\Make.rules.mak

DLLS = StdTestDll TestDll event_client

CCMDIR=$(prefix)
ORBDIR=$(OB_HOME)

install all:: CCM_test.idl

!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_test.idl: ..\idl\test.idl3
	$(IDL3) --std-dll-entry -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) ..\idl\test.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I..\idl test.idl
	$(CIDL)  --std-dll-entry -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I..\idl ..\idl\test.cidl
!else
CCM_test.idl: ..\idl\test.idl3
	$(IDL3) --std-dll-entry -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) ..\idl\test.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) test.idl
	$(CIDL) --std-dll-entry -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) -I..\idl ..\idl\test.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_test.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_test.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_test.idl
!endif

!if "$(ORB_NAME)" == "tao"
!else
	$(PSDL) -I. $(PSS_IDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) -DPSDL ..\idl\test.psdl
!endif

$(EVERYTHING)::
	@for %i in ( $(DLLS) ) do \
	    @if exist %i.mak cmd /c $(MAKE) /NOLOGO /f %i.mak $@"


clean::		
	$(top_srcdir)\bin\rm -rf ..\bin
	
install:: all
	if exist $(top_srcdir)\bin\event.zip \
		$(top_srcdir)\bin\unzip.exe -o -d temp $(top_srcdir)\bin\event.zip
	copy ..\bin\TestDll.dll temp\Philosopher\8f42239b-1e67-11d7-b81f-dc8107a7889d
	copy ..\bin\StdTestDll.dll temp\Philosopher\8f42239b-1e67-11d7-b81f-dc8107a7889d
	copy ..\bin\TestDll.dll temp\Observer\1185822a-1e53-11d7-bcdb-c44ce23073d6
	copy ..\bin\StdTestDll.dll temp\Observer\1185822a-1e53-11d7-bcdb-c44ce23073d6
	cd temp & \
	$(top_srcdir)\..\bin\zip.exe -r event.zip * & \
	move event.zip $(bindir)
	$(top_srcdir)\bin\rm -rf temp
	
	copy ..\bin\event_client.exe $(bindir)


mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log









