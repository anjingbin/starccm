#  **********************************************************************
#
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
# **********************************************************************

top_srcdir	= ..\..

!include $(top_srcdir)\config\Make.rules.mak

install::
	@if not exist $(idldir)\ccm \
	    @echo "Creating $(idldir)\ccm..." & \
	    mkdir $(idldir)\ccm

	@for %i in ( *.idl ) do \
	    @echo Installing %i & \
	    copy %i $(idldir)\ccm\%i

install_min:: install
