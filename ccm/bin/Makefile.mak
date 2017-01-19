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

BIN_FILES	= ApplicationServer.xml.xml \
		ccmc.cmd \
		cidl.exe \
		cidlc.cmd \
		cp.exe \
		cygwin1.dll \
		DebugConfig.ini \
		Demo.tcl \
		grep.exe \
		idl3.exe \
		idl3c.cmd \
		idlc.cmd \
		idlcpp.exe \
		kill.exe \
		msvcp60.dll \
		MSVCP60D.DLL \
		msvcrt.dll \
		MSVCRTD.DLL \
		mv.exe \
		ps.exe \
		psdlc.cmd \
		rm.exe \
		start_server.pl \
		stop_server.pl \
		starccm.properties \
		unzip.exe \
		xerces-c_1_7_0D.dll \
		zip.exe \
		nameserv.exe \
		initdb.expect \
		initdb.cmd \
		ConfigFile \
		test_ip.ini 
		 



install::
	@for %i in ( $(BIN_FILES) ) do \
	    @echo Installing %i & \
	    copy %i $(bindir)\%i

install_min:: install
