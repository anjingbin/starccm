// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Kebo    mep@263.net
//
// **********************************************************************

top_srcdir	= ..\..\..\..\..

!include $(top_srcdir)\config\Make.rules.mak

# Compiling & packaging & Deploying the bean

JAVAC = javac.exe
BUILD_DIR = build
JAVA_FLAGS = -d $(BUILD_DIR)
CLASSPATH = D:\bea\weblogic81\server\lib\weblogic.jar;.
JAR = jar.exe
JAR_FLAGS = cvf
WEBLOGIC_DIR = D:\bea\weblogic81\samples\domains\examples\applications

.SUFFIXES:	.java .class

.java.class::
	$(JAVAC) $(JAVA_FLAGS) -classpath $(CLASSPATH) $<
	copy ejb-jar.xml $(BUILD_DIR)\META-INF
	copy weblogic-ejb-jar.xml $(BUILD_DIR)\META-INF

Dinner-Observing.ear: Dinner-Observing.jar
	

Dinner-Observing.jar: Observing.class \
					ObservingBean.class \
					ObservingHome.class \
					ejb-jar.xml \
					weblogic-ejb-jar.xml
	cd $(BUILD_DIR) &\
	$(JAR) $(JAR_FLAGS) ..\Dinner-Observing.jar * &\
	cd ..

install all:: Dinner-Observing.jar

install:: all
	copy Dinner-Observing.jar $(WEBLOGIC_DIR)