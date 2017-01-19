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

GENERATED 	= CCM_PetStore.cpp \
		CCM_PetStore.h \
		CCM_PetStore_Context_impl.cpp \
		CCM_PetStore_Context_impl.h \
		CCM_PetStore_skel.cpp \
		CCM_PetStore_skel.h \
		PetStore.cpp \
		PetStore.h \
		PetStoreDll.cpp \
		PetStore_impl.cpp \
		PetStore_impl.h \
		PetStore_psdl.cpp \
		PetStore_psdl.h \
		PetStore_pss.cpp \
		PetStore_pss.h \
		PetStore_skel.cpp \
		PetStore_skel.h \
		PetStore_ValueType_impl.cpp \
		PetStore_ValueType_impl.h \
		StdPetStoreDll.cpp \
		CCM_PetStore.idl \
		PetStore.idl


!include $(top_srcdir)\config\Make.rules.mak

DLLS = PetStore StdPetStore

CCMDIR=$(prefix)
ORBDIR=$(OB_HOME)
install all:: CCM_PetStore.idl

!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_Dinner.idl: ../PetStore.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) --std-dll-entry --dll-entry PetStore.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) PetStore.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) --with-pss --std-dll-entry PetStore.cidl
!endif

!if "$(ORB_NAME)" == "ob"
CCM_PetStore.idl: PetStore.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --std-dll-entry --dll-entry PetStore.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) PetStore.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --with-pss --std-dll-entry PetStore.cidl
!endif

!if "$(ORB_NAME)" == "star"
CCM_PetStore.idl: PetStore.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --std-dll-entry --dll-entry PetStore.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) PetStore.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --with-pss --std-dll-entry PetStore.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_PetStore.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_PetStore.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_PetStore.idl
!endif

!if "$(ORB_NAME)" == "tao"
!else
	$(PSDL) -I. $(PSS_IDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) -DPSDL PetStore.psdl
!endif

$(EVERYTHING)::
	@for %i in ( $(DLLS) ) do \
	    @if exist %i.mak cmd /c $(MAKE) /NOLOGO /f %i.mak $@"

install:: all
	if exist $(top_srcdir)\bin\PetStore.zip \
		$(top_srcdir)\bin\unzip.exe -o -d temp $(top_srcdir)\bin\PetStore.zip
	copy ..\bin\PetStore.dll temp\Cart\1408d24a-2886-11d7-8609-d971ee33e54a
	copy ..\bin\StdPetStore.dll temp\Cart\1408d24a-2886-11d7-8609-d971ee33e54a
	copy ..\bin\PetStore.dll temp\Manager\93e1e9a6-2880-11d7-9c64-f6a003291bf5
	copy ..\bin\StdPetStore.dll temp\Manager\93e1e9a6-2880-11d7-9c64-f6a003291bf5
	copy ..\bin\PetStore.dll temp\Order\744697e3-287f-11d7-9c64-f6a003291bf5
	copy ..\bin\StdPetStore.dll temp\Order\744697e3-287f-11d7-9c64-f6a003291bf5
	copy ..\bin\PetStore.dll temp\OrderItem\d31e39d4-287f-11d7-9c64-f6a003291bf5
	copy ..\bin\StdPetStore.dll temp\OrderItem\d31e39d4-287f-11d7-9c64-f6a003291bf5
	copy ..\bin\PetStore.dll temp\Pricer\d35275cc-286a-11d7-8c80-8fca68dc242e
	copy ..\bin\StdPetStore.dll temp\Pricer\d35275cc-286a-11d7-8c80-8fca68dc242e
	copy ..\bin\PetStore.dll temp\Product\fcda6382-287e-11d7-9c64-f6a003291bf5
	copy ..\bin\StdPetStore.dll temp\Product\fcda6382-287e-11d7-9c64-f6a003291bf5
	copy ..\bin\PetStore.dll temp\Sales\2aa2dbc5-2880-11d7-9c64-f6a003291bf5
	copy ..\bin\StdPetStore.dll temp\Sales\2aa2dbc5-2880-11d7-9c64-f6a003291bf5
	cd temp & \
	$(top_srcdir)\..\bin\zip.exe -r PetStore.zip * & \
	move PetStore.zip $(bindir)
	$(top_srcdir)\bin\rm -rf temp


mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log









