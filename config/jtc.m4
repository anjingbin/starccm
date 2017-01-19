dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl                          
dnl **********************************************************************

dnl ----------------------------------------------------------------------
dnl JThreads/C++-specific configuration
dnl ----------------------------------------------------------------------

AC_SUBST(JTC_CPPFLAGS)
AC_SUBST(JTC_LDFLAGS)
AC_SUBST(JTC_LIBS)

JTC_CPPFLAGS=
JTC_LDFLAGS=
JTC_LIBS=

builtin(include,../config/threads.m4)

dnl
dnl Discover the JThreads/C++ path.
dnl
jtc_path=

if test -z "$jtc_path" -a -n "$with_jtc" -a "$with_jtc" != "yes"
then
    if test "$with_jtc" = "no"
    then
	AC_MSG_ERROR(JThreads/C++ required!)
    fi

    dnl
    dnl Search in the directory specified by the --with-ob option
    dnl
    jtc_path="${with_jtc}/"
    if test ! -f ${jtc_path}include/JTC/Thread.h
    then
	jtc_path="${jtc_path}jtc/"
	if test ! -f ${jtc_path}include/JTC/Thread.h
	then
	    jtc_path=
	fi
    fi
fi

if test -z "$jtc_path"
then
    dnl
    dnl Search in the current directory or ../jtc
    dnl	
    jtc_path="./"
    if test ! -f ${jtc_path}include/JTC/Thread.h
    then
	jtc_path="../jtc/"
	if test ! -f ${jtc_path}include/JTC/Thread.h
	then
	    jtc_path=
	fi
    fi
fi

if test -z "$jtc_path" -a -n "$prefix"
then
    dnl
    dnl Search in the installation directory
    dnl
    jtc_path="${prefix}/"
    if test ! -f ${jtc_path}include/JTC/Thread.h
    then
	jtc_path="${jtc_path}jtc/"
	if test ! -f ${jtc_path}include/JTC/Thread.h
	then
	    jtc_path=
	fi
    fi	
fi

dnl
dnl Error message if we didn't find the JThreads/C++ path.
dnl
if test ! -f ${jtc_path}include/JTC/Thread.h
then
    AC_MSG_ERROR(Can't find JThreads/C++! Use the --with-jtc=DIR option.)
fi

dnl
dnl Add $(top_srcdir) to relative paths (current directory and ../jtc)
dnl
if test "$jtc_path" = "../jtc/"
then
    jtc_path="\$(top_srcdir)/../jtc/"
fi

JTC_LDFLAGS="-L${jtc_path}lib"
JTC_CPPFLAGS="-I${jtc_path}include"
JTC_LIBS="-lJTC"
