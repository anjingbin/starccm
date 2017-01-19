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

EXE_NAME	= $(top_srcdir)\bin\MainHostInstall.exe
PDB_NAME	= $(top_srcdir)\bin\MainHostInstall.pdb
PDB_FILES	= $(PDB_NAME) 
PDB_FLAGS	= /Fd$(PDB_NAME)		

TARGETS		= $(EXE_NAME)

                                
!include $(top_srcdir)\config\Make.rules.mak

!if "$(LITE_VERSION)" == "yes"
PSS_LIBS=
!endif

                                 
EXE_OBJS	= mainhostinstall.obj
                                   
ALL_CPPFLAGS	= /I. $(CCM_CPPFLAGS) $(OB_CPPFLAGS) $(CPPFLAGS)
ALL_IDLFLAGS	= $(OB_IDLFLAGS) $(CCM_IDLFLAGS)

ALL_CXXFLAGS	= $(CXXFLAGS) $(PDB_FLAGS)
!if "$(ORB_NAME)" == "tao"
ALL_LIBS	= $(OB_LIBS) $(CCM_LIBS) $(LIBS) $(XERCES_LIB) $(TAO_ROOT)\tao\DynamicAny\TAO_DynamicAny$(LIBSUFFIX).lib $(TAO_NS_LIBS)
!else
ALL_LIBS	= $(OB_LIBS) $(CCM_LIBS) $(LIBS) $(PSS_LIBS) $(OTS_LIBS) $(XERCES_LIB)
!endif


$(EXE_NAME): $(EXE_OBJS) 
	if exist "$@" \
	del "$@"
	$(LINK) $(LINKFLAGS) /out:$@ \
	$(EXE_OBJS) $(ALL_LIBS)

install:: all
	copy $(EXE_NAME) $(bindir)
#	if exist $(PDB_NAME) \
#	copy $(PDB_NAME) $(bindir)



clean::

min:: all

install_min:: install

mkdep::

!include .windep
