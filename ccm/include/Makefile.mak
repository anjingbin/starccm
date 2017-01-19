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

install::
	@if not exist $(includedir)\ccm \
	    @echo "Creating $(includedir)\ccm..." & \
	    mkdir $(includedir)\ccm

	@for %i in ( *.h ) do \
	    @echo Installing %i & \
	    copy %i $(includedir)\ccm\%i

install_min:: install
