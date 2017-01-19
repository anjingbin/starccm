# **********************************************************************
#
# Copyright (c) 2003
# Intervision Software Co., Ltd.
# Haidian District, Beijing.
#
# All Rights Reserved
#
# **********************************************************************

top_srcdir	= .

!include Make.rules.mak
!include ..\..\..\..\config\Make.rules.mak

all::classes
	$(JIDL) --no-skel --package bank --output-dir $(prefix_)\src --all \
	 $(includePath) $(prefix_)\idl\$(ManagerIDL).idl
	 
	 $(JIDL) --no-skel --package bank --output-dir $(prefix_)\src --all \
	 $(includePath) $(prefix_)\idl\$(ATMIDL).idl
	set CLASSPATH=.;$(prefix_)\lib\STAR.jar;$(prefix_)\lib\jbcl.jar;$(JDK)\lib\tools.jar;$(prefix_)\classes;$(RT_JAR)$(CLASSPATH)
	
	$(JAVAC) -sourcepath $(SOURCEPATH) -d $(prefix_)\classes $(prefix_)\src\bank\ui\*.java
	$(JAVAC) -sourcepath $(SOURCEPATH) -d $(prefix_)\classes $(prefix_)\src\bank\Bank\*.java
	$(JAVAC) -sourcepath $(SOURCEPATH) -d $(prefix_)\classes $(prefix_)\src\bank\Components\*.java
	$(JAVAC) -sourcepath $(SOURCEPATH) -d $(prefix_)\classes $(prefix_)\src\bank\CORBA\*.java
	$(JAVAC) -d $(prefix_)\classes $(prefix_)\src\bank\CORBA\ContainedPackage\*.java
	$(JAVAC) -d $(prefix_)\classes $(prefix_)\src\bank\CORBA\ContainerPackage\*.java
	$(JAVAC) -d $(prefix_)\classes $(prefix_)\src\bank\CORBA\InterfaceDefPackage\*.java
	$(JAVAC) -d $(prefix_)\classes $(prefix_)\src\bank\CORBA\PolicyManagerPackage\*.java
	$(JAVAC) -d $(prefix_)\classes $(prefix_)\src\bank\CORBA\ValueDefPackage\*.java
	
	
	@copy $(prefix_)\resource\*.jpg $(prefix_)\classes\bank\ui	
	
BANKClass = $(prefix)\classes
install::all
   if not exist $(BANKClass) \
	 @echo "Creating $(BANKClass)..." & \
	  mkdir $(BANKClass)    
   copy $(top_srcdir)\bin\ATMclient.pl $(bindir)
   copy $(top_srcdir)\bin\Managerclient.pl $(bindir)
   xcopy /e $(top_srcdir)\classes $(prefix)\classes
   copy $(top_srcdir)\lib\*.* $(libdir)
   @copy $(prefix_)\bin\account.txt $(bindir)
    	
	
