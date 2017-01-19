#  **********************************************************************
#
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: An Jingbin	anjb@sina.com
#          Wang Kebo    mep@263.net
# **********************************************************************


# ----------------------------------------------------------------------
# Use the following lines to set your installation directories
# ----------------------------------------------------------------------


#ORB_NAME = ob
ORB_NAME = star
#ORB_NAME = tao
WITH_OTS = yes
#DLL=yes

#LITE_VERSION=yes
STARCCM_MTL = yes

prefix		= C:\StarCCM2.0
bindir		= $(prefix)\bin
libdir		= $(prefix)\lib
includedir	= $(prefix)\include
idldir		= $(prefix)\idl


# ----------------------------------------------------------------------
# Either define OPTIMIZE_SPEED as "yes" to build with speed
# optimization turned on, or define OPTIMIZE_SIZE as "yes" to optimize
# for size, or define DEBUG as "yes" to build with debug
# information. Note that all these settings are mutual exclusive.
#
# NOTE: If you change these options later, you need to completely
# rebuild this distribution. (Using "nmake /f Makefile.mak clean", and
# "nmake /f Makefile.mak" after that.)
# ----------------------------------------------------------------------

#OPTIMIZE_SPEED=yes
#OPTIMIZE_SIZE=yes
DEBUG=yes

# ----------------------------------------------------------------------
# Define OLD_IOSTREAM as "yes" if you want to use the old iostream
# library, i.e., if you want to use '#include <iostream.h>" instead of
# "#include <iostream>". 
#
# NOTE: If you want to check your application with "purify", please
# use the old iostream library instead of the new one. If the new one
# is used, purify will detect lots of non-existent memory problems.
# ----------------------------------------------------------------------

#OLD_IOSTREAM=yes

# ----------------------------------------------------------------------
# If you want to make use of a Dynamic Link Library (DLL) instead of a
# static library in the packages where this is supported, uncomment
# the following line to define DLL as "yes". Note that in this case
# you need "perl" for the compilation process.
#
# NOTE: If you want to compile ORBacus as a DLL you must also compile
# JThreads/C++ as a DLL.  When using the DLL version of ORBacus or
# JThreads/C++, you must not specify a statically linked runtime
# library with the /MT, /MTd, /ML or /MLd compiler flags.
# ----------------------------------------------------------------------

#DLL=yes

# ----------------------------------------------------------------------
# Experts only: You can modify the lines below, for example to change
# the settings for code optimizations.
# ----------------------------------------------------------------------

#
# These options are for building with optimizations turned on.	See
# Microsoft's compiler documentation for more information on the /O
# flag. Please note that debug information is automatically disabled
# by the Microsoft compiler when optimization is enabled.
#

!if "$(OPTIMIZE_SPEED)" == "yes"

OPT_CXXFLAGS	= /Ox 
OPT_LINKFLAGS	=
OPT_CPPFLAGS	= /DNDEBUG

!endif

!if "$(OPTIMIZE_SIZE)" == "yes"

OPT_CXXFLAGS	= /O1 
OPT_LINKFLAGS	=
OPT_CPPFLAGS	= /DNDEBUG

!endif

!if "$(DEBUG)" == "yes"

#
# These options are for building with debug information, and no
# optimization.
#

OPT_CXXFLAGS	= /Zi $(OPT_CXXFLAGS)
OPT_LINKFLAGS	= /debug $(OPT_LINKFLAGS)
OPT_CPPFLAGS	= $(OPT_CPPFLAGS) /D_DEBUG
LIBSUFFIX	= $(LIBSUFFIX)d
CXX_RTL		= /MDd
!else

CXX_RTL		= /MD

!endif

CXX		= cl.exe
CXXFLAGS	= /nologo $(CXX_RTL) /W3 /GX /Gy $(OPT_CXXFLAGS)
CXXCPP		= cl.exe /nologo /E
CPPFLAGS	= /DWIN32 $(OPT_CPPFLAGS) # /DOB_CLEAR_MEM
CFLAGS		= $(CXXFLAGS) $(CPPFLAGS)
LIBS		= 
GUILIBS		= 
DLLLIBS		=
LINK	= link.exe
LINKFLAGS	= /nologo $(OPT_LINKFLAGS) /subsystem:console /incremental:no
LINKGUIFLAGS	= /nologo $(OPT_LINKFLAGS) /subsystem:windows /incremental:no
LINKDLLFLAGS	= /nologo $(OPT_LINKFLAGS) /dll /incremental:no
AR		= lib.exe
ARFLAGS		= /nologo

SHELL		= bash # Only needed for "nmake /f Makefile.mak test"

# ----------------------------------------------------------------------
# Don't change anything below this line
# ----------------------------------------------------------------------

!ifndef INCLUDE
!error The INCLUDE environment variable needs to be set.
!endif
!ifndef LIB
!error The LIB environment variable needs to be set.
!endif

EVERYTHING	= all install min install_min clean clean-all mkdep

.SUFFIXES:
.SUFFIXES:	.c .cc .cpp .obj

!ifdef NMAKE_NO_BATCH_MODE
.cpp.obj:
	$(CXX) /c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $<

.cc.obj:
	$(CXX) /c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) /TP $<

.c.obj:
	$(CC) /c $(ALL_CFLAGS) $<
!else
.cpp.obj::
	$(CXX) /c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $<

.cc.obj::
	$(CXX) /c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) /TP $<

.c.obj::
	$(CC) /c $(ALL_CFLAGS) $<
!endif

all:: $(TARGETS)

install::

min::

install_min::

test::

clean::
#	@echo "Deleting intermediate files and output files: *.obj *.bak *.flc *.pdb *.pch..."
	-@del *.obj *.bak *.flc *.pdb *.pch 2> NUL

!if "$(TARGETS)" != ""
clean::
	@for %i in ( $(TARGETS) ) do \
	    @if exist %i del %i
!endif

!if "$(PDB_FILES)" != ""
clean::
	@for %i in ( $(PDB_FILES) ) do \
	    @if exist %i del %i
!endif

clean-all:: clean

!if "$(GENERATED)" != ""
clean-all:: clean
	@for %i in ( $(GENERATED) ) do \
	    @if exist %i del %i
!endif
	
distclean:: clean
!if "$(TARGETS)" != ""
	-@del $(TARGETS)
!endif

install::

test::

lib:
	mkdir $@

bin:
	mkdir $@
