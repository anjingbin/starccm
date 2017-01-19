# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
#  Author: An Jingbin	anjb@sina.com	
# **********************************************************************


top_srcdir	= ..\..\..

LIB_NAME	= .\debug\libpq.lib
DLL_NAME	= .\debug\libpq.dll
PDB_NAME	= .\debug\libpq.pdb
IMP_NAME	= .\debug\libpqdll.pdb
EXP_NAME	= .\debug\libpqdll.exp
PDB_FILES	= $(PDB_NAME)
PDB_FLAGS	= /Fd$(PDB_NAME)

#TARGETS		= $(LIB_NAME) $(DLL_NAME)

!include $(top_srcdir)\config\Make.rules.mak

.c{$(CPP_OBJS)}.obj::
	$(CPP) $(CPP_PROJ) $<
	
.cpp{$(CPP_OBJS)}.obj::
	$(CPP) $(CPP_PROJ) $<

.cxx{$(CPP_OBJS)}.obj::
	$(CPP) $(CPP_PROJ) $<

.c{$(CPP_SBRS)}.sbr::
	$(CPP) $(CPP_PROJ) $<

.cpp{$(CPP_SBRS)}.sbr::
	$(CPP) $(CPP_PROJ) $<

.cxx{$(CPP_SBRS)}.sbr::
	$(CPP) $(CPP_PROJ) $<


LIB32_OBJS	= dllist.obj \
		md5.obj \
		fe-auth.obj \
		fe-connect.obj \
		fe-exec.obj \
		fe-lobj.obj \
		fe-misc.obj \
		fe-print.obj \
		pqexpbuffer.obj


LINK32_OBJS	= libpqdll.obj \
		libpq.lib \
		libpq.res


CPP=cl.exe
RSC=rc.exe

"debug" :
	if not exist "debug" mkdir "debug"

CPP_PROJ=/nologo /MDd /Fd$(PDB_NAME) /W3 /GX /O2 /I "..\..\include" /D "FRONTEND" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS"\
 /c  /D "HAVE_VSNPRINTF" /D "HAVE_STRDUP"


CPP_OBJS=.
CPP_SBRS=.

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(LIB_NAME)"


RSC_PROJ=/l 0x409 /fo"libpq.res"

LINK32=link.exe

LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib wsock32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(PDB_NAME)" /machine:I386 /out:"$(DLL_NAME)"\
 /implib:"$(IMP_NAME)"  /def:libpqdll.def
 
 
LINK32_OBJS= \
	"libpqdll.obj" \
	"$(LIB_NAME)" \
	"libpq.res"


"$(LIB_NAME)" : "debug" $(LIB32_OBJS)
	$(LIB32) $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)

"libpq.res" : libpq.rc
    $(RSC) $(RSC_PROJ) libpq.rc


"$(DLL_NAME)" : "debug" "libpqdll.obj" "libpq.res"
	$(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)

"dllist.obj" : ..\..\backend\lib\dllist.c
	$(CPP) $(CPP_PROJ) ..\..\backend\lib\dllist.c

"md5.obj" : ..\..\backend\libpq\md5.c
	$(CPP) $(CPP_PROJ) ..\..\backend\libpq\md5.c


clean::
	-del $(LIB_OBJS) 2> NUL
	-del libpqdll.obj 2> NUL
	-del libpq.res 2> NUL


install:: all
	copy $(LIB_NAME) $(libdir)
#	if exist $(PDB_NAME) \
#	copy $(PDB_NAME) $(libdir)