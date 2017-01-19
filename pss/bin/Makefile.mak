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

!if "$(ORB_NAME)" == "ob"
BIN_FILE	= psdl.exe
!endif

!if "$(ORB_NAME)" == "star"
BIN_FILE	= star-psdl.exe
!endif


install::
	@echo Installing $(BIN_FILE) & \
	    copy $(BIN_FILE) $(bindir)\psdl.exe

install_min:: install
