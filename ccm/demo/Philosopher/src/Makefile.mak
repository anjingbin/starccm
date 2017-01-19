# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: An Jingbin	anjb@sina.com
# **********************************************************************



top_srcdir	= ..\..\..

GENERATED 	= Dinner.idl \
		Dinner_impl.h \
		Dinner_impl.cpp \
		Dinner_ValueType_impl.h \
		Dinner_ValueType_impl.cpp \
		DinnerDll.cpp \
		Philosopher.cpf \
		Cutlery.cpf \
		Observer.cpf \
		PhilosopherHome.cpf \
		CutleryHome.cpf \
		ObserverHome.cpf \
		Dinner.h \
		Dinner.cpp \
		Dinner_skel.cpp \
		Dinner_skel.h \
		CCM_Dinner_Context_impl.h \
		CCM_Dinner_Context_impl.cpp \
		CCM_Dinner.idl \
		Philosopher.ccd \
		Cutlery.ccd \
		Observer.ccd \
		CCM_Dinner.cpp \
		CCM_Dinner.h \
		CCM_Dinner_skel.cpp \
		CCM_Dinner_skel.h \
		Dinner_psdl.cpp \
		Dinner_psdl.h \
		Dinner_pss.cpp \
		Dinner_pss.h


!include $(top_srcdir)\config\Make.rules.mak




DLLS = Dinner StdDinner

CCMDIR=$(prefix)
ORBDIR=$(OB_HOME)

install all:: CCM_Dinner.idl

!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_Dinner.idl: Dinner.idl3
	$(IDL3) --impltxt -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Dinner.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Dinner.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Dinner.cidl
!else
CCM_Dinner.idl: Dinner.idl3
	$(IDL3) --impltxt -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Dinner.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Dinner.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Dinner.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_Dinner.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_Dinner.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_Dinner.idl
!endif

!if "$(ORB_NAME)" == "tao"
!else
	$(PSDL) -I. $(PSS_IDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) -DPSDL Dinner.psdl
!endif

$(EVERYTHING)::
	@for %i in ( $(DLLS) ) do \
	    @if exist %i.mak cmd /c $(MAKE) /NOLOGO /f %i.mak $@"

install:: all
	if exist $(top_srcdir)\bin\philosopher.zip \
		$(top_srcdir)\bin\unzip.exe -o -d temp $(top_srcdir)\bin\philosopher.zip
	copy ..\bin\Dinner.dll temp\Cutlery\bb38d965-1e54-11d7-9b85-8a6194305b45
	copy ..\bin\StdDinner.dll temp\Cutlery\bb38d965-1e54-11d7-9b85-8a6194305b45
	copy ..\bin\Dinner.dll temp\Observer\1185822a-1e53-11d7-bcdb-c44ce23073d6
	copy ..\bin\StdDinner.dll temp\Observer\1185822a-1e53-11d7-bcdb-c44ce23073d6
	copy ..\bin\Dinner.dll temp\Philosopher\8f42239b-1e67-11d7-b81f-dc8107a7889d
	copy ..\bin\StdDinner.dll temp\Philosopher\8f42239b-1e67-11d7-b81f-dc8107a7889d
	cd temp & \
	$(top_srcdir)\..\bin\zip.exe -r philosopher.zip * & \
	move philosopher.zip $(bindir)
	$(top_srcdir)\bin\rm -rf temp


mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log









