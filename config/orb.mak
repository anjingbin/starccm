#  **********************************************************************
#
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: An Jingbin	anjb@sina.com
#          Wang Kebo    mep@263.net	
# **********************************************************************

!if "$(ORB_NAME)" == "ob"
OB_HOME 	= D:\Open_Micro_CCM\OB
THREAD_NAME  = jtc
OB_IDLFLAGS	= -DDLL_LIB -DORBacus -I$(OB_HOME)\idl
!endif

!if "$(ORB_NAME)" == "star"
OB_HOME		= D:\Work_CCM_NEW\StarBus
STAR_HOME	= $(OB_HOME)
OB_IDLFLAGS	= -DStarBus --no-localobject-ids -I$(OB_HOME)\idl
THREAD_NAME  = mtl

STAR_INCLUDE	= /I$(STAR_HOME)\include 
STAR_CPPFLAGS = $(STAR_INCLUDE)
STAR_IDLFLAGS	= -I$(STAR_HOME)\idl
STAR_LIBS	= $(STAR_HOME)\lib\star$(LIBSUFFIX).lib $(STAR_HOME)\lib\mtl$(LIBSUFFIX).lib
STAR_NS_LIBS	= $(STAR_HOME)\lib\CosNaming$(LIBSUFFIX).lib
STAR_IDLDIR	= $(STAR_HOME)\idl
!endif

CXXFLAGS        = $(CXXFLAGS) /GR

OB_INCLUDE	= /I$(OB_HOME)\include 
OB_CPPFLAGS = $(OB_INCLUDE)
OB_LIBS	= $(OB_HOME)\lib\$(ORB_NAME)$(LIBSUFFIX).lib $(OB_HOME)\lib\$(THREAD_NAME)$(LIBSUFFIX).lib
OB_NS_LIBS	= $(OB_HOME)\lib\CosNaming$(LIBSUFFIX).lib
IDL		= $(OB_HOME)\bin\idl.exe
OB_IDLDIR 	= $(OB_HOME)\idl

!if "$(DLL)" == "yes"
PATH		= $(OB_HOME)\lib;$(PATH)
!endif

!if "$(ORB_NAME)" == "tao"
TAO_ROOT = $(ACE_ROOT)\TAO
TAO_IDLFLAGS = -DTAO -o . -cs .cpp -hc .h -hs _skel.h -ss _skel.cpp -Sc -I$(CCM_HOME)\idl\tao -I$(TAO_ROOT) -Gv
TAO_IDLFLAGS_TIE = -DTAO -o . -cs .cpp -hc .h -hs _skel.h -ss _skel.cpp -sT _skel_tie.cpp -hT _skel_tie.h -I$(CCM_HOME)\idl -I$(TAO_ROOT) -Gv
TAO_INCLUDE = /I$(ACE_ROOT) /I$(TAO_ROOT) /I$(TAO_ROOT)\orbsvcs
TAO_CPPFLAGS = $(TAO_INCLUDE)
TAO_LIBS = $(ACE_ROOT)\ace\ace$(LIBSUFFIX).lib \
	$(TAO_ROOT)\tao\TAO$(LIBSUFFIX).lib \
	$(TAO_ROOT)\tao\PortableServer\TAO_PortableServer$(LIBSUFFIX).lib \
	$(TAO_ROOT)\tao\IFR_Client\TAO_IFR_Client$(LIBSUFFIX).lib \
	$(TAO_ROOT)\tao\DynamicInterface\TAO_DynamicInterface$(LIBSUFFIX).lib \
	$(TAO_ROOT)\tao\IORTable\TAO_IORTable$(LIBSUFFIX).lib \
	$(TAO_ROOT)\tao\TypeCodeFactory\TAO_TypeCodeFactory$(LIBSUFFIX).lib
TAO_NS_LIBS = $(TAO_ROOT)\orbsvcs\orbsvcs\TAO_CosNaming$(LIBSUFFIX).lib
IDL = $(ACE_ROOT)\bin\tao_idl.exe

OB_IDLFLAGS = $(TAO_IDLFLAGS)
OB_INCLUDE = $(TAO_INCLUDE)
OB_CPPFLAGS = $(OB_INCLUDE) /DHAVE_STD_IOSTREAM /DHAVE_FSTREAM
OB_LIBS	= $(TAO_LIBS)
OB_NS_LIBS = $(TAO_NS_LIBS)
!endif