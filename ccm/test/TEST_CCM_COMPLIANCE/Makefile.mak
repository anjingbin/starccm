# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: Zheng Di	sonofwind0109@sina.com	
# **********************************************************************

top_srcdir	= ..\..

!include $(top_srcdir)\config\Make.rules.mak

SUBDIRS		= facet_test session_test support_test homefinder_test home_test event_test receptacle_test

$(EVERYTHING)::
	@for %i in ( $(SUBDIRS) ) do \
	    @if exist %i cmd /c "cd %i & cd &$(MAKE) /NOLOGO /f Makefile.mak $@"

