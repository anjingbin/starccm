# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: An Jingbin	anjb@sina.com
# **********************************************************************



top_srcdir	= ..\..\..\..

GENERATED 	= HelloWorld.idl \
		HelloWorld_impl.h \
		HelloWorld_impl.cpp \
		HelloWorldDll.cpp \
		HelloWorld.h \
		HelloWorld.cpp \
		HelloWorld_skel.cpp \
		HelloWorld_skel.h \
		CCM_HelloWorld_Context_impl.h \
		CCM_HelloWorld_Context_impl.cpp \
		CCM_HelloWorld.idl \
		Person.ccd \
		CCM_HelloWorld.cpp \
		CCM_HelloWorld.h \
		CCM_DHelloWorld_skel.cpp \
		CCM_HelloWorld_skel.h \
		HelloWorld_psdl.cpp \
		HelloWorld_psdl.h \
		HelloWorld_pss.cpp \
		HelloWorld_pss.h \
		HelloWorld_ValueType_impl.h\
		HelloWorld_ValueType_impl.cpp \
		CCM_HelloWorld_skel.cpp \
		PersonHome.cpf \
		StdHelloWorldDll.cpp


!include $(top_srcdir)\config\Make.rules.mak




DLLS = HelloWorld StdHelloWorld SMT_Client

CCMDIR=$(prefix)
ORBDIR=$(OB_HOME)

install all:: CCM_HelloWorld.idl

!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_HelloWorld.idl: ..\idl\HelloWorld.idl3
	$(IDL3) --std-dll-entry -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) ..\idl\HelloWorld.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I..\idl HelloWorld.idl
	$(CIDL)  --std-dll-entry -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I..\idl ..\idl\HelloWorld.cidl
!else
CCM_HelloWorld.idl: ..\idl\HelloWorld.idl3
	$(IDL3) --std-dll-entry -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) ..\idl\HelloWorld.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) HelloWorld.idl
	$(CIDL) --std-dll-entry -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) -I..\idl ..\idl\HelloWorld.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_HelloWorld.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_HelloWorld.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_HelloWorld.idl
!endif

!if "$(ORB_NAME)" == "tao"
!else
	$(PSDL) -I. $(PSS_IDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) -DPSDL ..\idl\HelloWorld.psdl
!endif

$(EVERYTHING)::
	@for %i in ( $(DLLS) ) do \
	    @if exist %i.mak cmd /c $(MAKE) /NOLOGO /f %i.mak $@"


clean::		
	$(top_srcdir)\bin\rm -rf ..\bin
	
install:: all
	if exist $(top_srcdir)\bin\SMT_Package.zip \
		$(top_srcdir)\bin\unzip.exe -o -d temp $(top_srcdir)\bin\SMT_Package.zip
	copy ..\bin\HelloWorld.dll temp\Person\bb38d965-1e54-11d7-9b85-8a6194305b45
	copy ..\bin\StdHelloWorld.dll temp\Person\bb38d965-1e54-11d7-9b85-8a6194305b45
	cd temp & \
	$(top_srcdir)\..\bin\zip.exe -r SMT_Package.zip * & \
	move SMT_Package.zip $(bindir)
	$(top_srcdir)\bin\rm -rf temp
	
	copy ..\bin\SMT_Client.exe $(bindir)
	

mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log









