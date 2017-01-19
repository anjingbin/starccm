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
	@if not exist $(includedir)\pss \
	    @echo "Creating $(includedir)\pss..." & \
	    mkdir $(includedir)\pss

	@for %i in ( *.h ) do \
	    @echo Installing %i & \
	    copy %i $(includedir)\pss\%i

install_min:: install
