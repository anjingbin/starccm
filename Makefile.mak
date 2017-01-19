#  **********************************************************************
# 
#  Copyright (c) 2001-2003 StarMiddleware.net
# 
#  www.StarMiddleware.net
#  All Rights Reserved
# 
# **********************************************************************

# Version: 1.0.0

top_srcdir	= .

!include $(top_srcdir)\config\Make.rules.mak

!if "$(ORB_NAME)" == "ob"
SUBDIRS		= ots pss ccm postgresql
!endif

!if "$(ORB_NAME)" == "star"
SUBDIRS		= star-ots pss ccm postgresql
!endif

install install_min::
	@if not exist $(bindir) \
	    @echo "Creating $(bindir)..." & \
	    mkdir $(bindir)

	@if not exist $(libdir) \
	    @echo "Creating $(libdir)..." & \
	    mkdir $(libdir)

	@if not exist $(includedir) \
	    @echo "Creating $(includedir)..." & \
	    mkdir $(includedir)

	@if not exist $(idldir) \
	    @echo "Creating $(idldir)..." & \
	    mkdir $(idldir)
	    
$(EVERYTHING)::
	@for %i in ( $(SUBDIRS) ) do \
	    @cmd /c "cd %i & cd & $(MAKE) /NOLOGO  /f Makefile.mak $@"
