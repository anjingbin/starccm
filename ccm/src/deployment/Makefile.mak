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

!include $(top_srcdir)\config\Make.rules.mak

!if "$(LITE_VERSION)" == "yes"
PSS_LIBS=
!endif


DEPL_TOOLS = FileAccessorFactory Assembly AssemblyFactory ComponentInstallation Deploy DeployDomainMgr MainHostInstall ServerActivator DynDeploymentMgr HostInstall

$(EVERYTHING)::
	@for %i in ( $(DEPL_TOOLS) ) do \
	    @if exist %i.mak cmd /c $(MAKE) /NOLOGO /f %i.mak $@"
	    
mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log
