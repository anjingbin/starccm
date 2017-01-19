#  **********************************************************************
# 
#  Copyright (c) 2001-2003 StarMiddleware.net
# 
#  www.StarMiddleware.net
#  All Rights Reserved
# 
# **********************************************************************

# Version: 1.0.0

top_srcdir	= ..\..

!include $(top_srcdir)\config\Make.rules.mak

SUBDIRS	= libpq 

$(EVERYTHING)::
	@for %i in ( $(SUBDIRS) ) do \
	    @cmd /c "cd %i & cd & $(MAKE) /NOLOGO  /f Makefile.mak $@"
