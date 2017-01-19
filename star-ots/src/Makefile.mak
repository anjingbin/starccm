# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: Ren Yi    starots@sohu.com	
# **********************************************************************

top_srcdir	= ..

!include $(top_srcdir)\config\Make.rules.mak

SubMak = OTSlib OTS_lib RESlib OTSCurrent OTS RES

$(EVERYTHING)::
	@for %i in ( $(SubMak) ) do \
	    @if exist %i.mak cmd /c $(MAKE) /NOLOGO /f %i.mak $@"
	    
mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log
