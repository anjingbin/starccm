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
	@if not exist $(idldir)\pss \
	    @echo "Creating $(idldir)\pss..." & \
	    mkdir $(idldir)\pss

	@for %i in ( *.idl ) do \
	    @echo Installing %i & \
	    copy %i $(idldir)\pss\%i

install_min:: install
