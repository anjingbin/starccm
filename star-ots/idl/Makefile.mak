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
	@if not exist $(idldir)\ots \
	    @echo "Creating $(idldir)\ots..." & \
	    mkdir $(idldir)\ots

	@for %i in ( *.idl ) do \
	    @echo Installing %i & \
	    copy %i $(idldir)\ots\%i

install_min:: install
