# **********************************************************************
#
# Copyright (c) 2003
# Intervision Software Co., Ltd.
# Haidian District, Beijing.
#
# All Rights Reserved
#
# **********************************************************************

prefix_	= .
RT_JAR          = $(JAVA_HOME)/jre/lib/rt.jar

JAVA		= java

IdlFilePath     = $(prefix_)\idl
includePath     = -I. -I$(prefix_)\bin\idl\include -I$(prefix_)\idl
ATMIDL     = ATM
ManagerIDL    = Manager

JDK             = C:\j2sdk1.4.2

JAVAC		= javac


JAVACDEBUG	= #-g
JAVACFLAGS	= $(JAVACDEBUG) $(BOOTCLASSPATH) 

JAR		= jar
JARFLAGS	= c0f

JIDL		= $(prefix_)\bin\jidl
SOURCEPATH      = $(prefix_)\src

.SUFFIXES:
.SUFFIXES:	.java .class

.java.class:
	$(JAVAC) $(JAVACFLAGS) $<

all::

clean::
	-del $(prefix_)\classes\bank\ui\*.*
	-rmdir /s /q $(prefix_)\classes\bank\Bank
	-rmdir /s /q $(prefix_)\classes\bank\Components
	-rmdir /s /q $(prefix_)\classes\bank\CORBA
	
	

install::

test::

classes:
	mkdir classes

generated:
	mkdir generated

$(top_srcdir)\lib:
	mkdir $(top_srcdir)\lib


