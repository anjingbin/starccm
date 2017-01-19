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

GENERATED 	= Plus.cpf \
                PlusHome.cpf \
                plus.idl \
                plus_impl.cpp \
                plus_impl.h \
                plus_ValueType_impl.cpp \
                plus_ValueType_impl.h \
                PlusDll.cpp \
                plus.cpp \
                plus.h \
                plus_skel.cpp \
                plus_skel.h \
                CCM_plus.idl \
                CCM_plus_Context_impl.cpp \
                CCM_plus_Context_impl.h \
                CCM_plus_skel.cpp \
                CCM_plus_skel.h \
                Plus.ccd \
                StdPlusDll.cpp \
                CCM_plus.cpp \
                CCM_plus.h \
                plus_psdl.cpp \
                plus_psdl.h \
                plus_pss.cpp \
                plus_pss.h 
                

!include $(top_srcdir)\config\Make.rules.mak

DLLS = StdPlusDll PlusDll homefinder_client

CCMDIR=$(prefix)
ORBDIR=$(OB_HOME)

install all:: CCM_plus.idl

!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_plus.idl: ..\idl\plus.idl3
	$(IDL3) --std-dll-entry -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) ..\idl\plus.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I..\idl plus.idl
	$(CIDL)  --std-dll-entry -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I..\idl ..\idl\plus.cidl
!else
CCM_plus.idl: ..\idl\plus.idl3
	$(IDL3) --std-dll-entry -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) ..\idl\plus.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) plus.idl
	$(CIDL) --std-dll-entry -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) -I..\idl ..\idl\plus.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_plus.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_plus.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_plus.idl
!endif

!if "$(ORB_NAME)" == "tao"
!else
	$(PSDL) -I. $(PSS_IDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) -DPSDL ..\idl\plus.psdl
!endif

$(EVERYTHING)::
	@for %i in ( $(DLLS) ) do \
	    @if exist %i.mak cmd /c $(MAKE) /NOLOGO /f %i.mak $@"


clean::		
	$(top_srcdir)\bin\rm -rf ..\bin
	
install:: all
	if exist $(top_srcdir)\bin\homefinder.zip \
		$(top_srcdir)\bin\unzip.exe -o -d temp $(top_srcdir)\bin\homefinder.zip
	copy ..\bin\PlusDll.dll temp\plus\aea9ce4f-9b4d-11d7-8bcb-ada9fed77468
	copy ..\bin\StdPlusDll.dll temp\plus\aea9ce4f-9b4d-11d7-8bcb-ada9fed77468
	cd temp & \
	$(top_srcdir)\..\bin\zip.exe -r homefinder.zip * & \
	move homefinder.zip $(bindir)
	$(top_srcdir)\bin\rm -rf temp
	
	copy ..\bin\homefinder_client.exe $(bindir)
	

mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log









