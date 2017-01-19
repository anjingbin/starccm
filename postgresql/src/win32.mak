# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/win32.mak,v 1.2 2004/07/05 08:56:17 SuLiang Exp $

# Makefile for Microsoft Visual C++ 5.0 (or compat)
# Top-file makefile for Win32 parts of postgresql.
# Note that most parts are not ported to Win32!

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IFDEF	MULTIBYTE
MAKEMACRO = "MULTIBYTE=$(MULTIBYTE)"
!ENDIF

ALL: 
   cd include
   if not exist pg_config.h copy pg_config.h.win32 pg_config.h
   cd ..
   cd interfaces\libpq
   nmake /f win32.mak $(MAKEMACRO)
   cd ..\libpq++
   nmake /f win32.mak $(MAKEMACRO)
   cd ..\..\bin\psql
   nmake /f win32.mak $(MAKEMACRO)
   cd ..\..
   echo All Win32 parts have been built!

CLEAN:
   cd interfaces\libpq
   nmake /f win32.mak CLEAN
   cd ..\..\bin\psql
   nmake /f win32.mak CLEAN
   cd ..\..
   echo All Win32 parts have been cleaned!

DISTCLEAN: CLEAN
   cd include
   del config.h
   cd ..
