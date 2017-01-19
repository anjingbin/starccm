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

SUBDIRS		= src

all min install install_min:: bin

$(EVERYTHING)::
	@for %i in ( $(SUBDIRS) ) do \
	    @if exist %i cmd /c "cd %i & cd &$(MAKE) /NOLOGO /f Makefile.mak $@"

