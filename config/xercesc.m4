dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl                          
dnl **********************************************************************


AC_SUBST(XERCESC_CPPFLAGS)
AC_SUBST(XERCESC_LDFLAGS)
AC_SUBST(XERCESC_LIBS)
AC_SUBST(WITH_XERCESC)

XERCESC_CPPFLAGS=
XERCESC_LDFLAGS=
XERCESC_LIBS=
WITH_XERCESC=no

AC_ARG_WITH(xercesc,
  --with-xercesc=DIR     make use of xercesc)


if test -z "$with_xercesc" -o "$with_xercesc" != "no"
then

xercesc_path=

if test -z "$xercesc_path" -a -n "$with_xercesc" -a "$with_xercesc" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-xercesc option
    dnl
    xercesc_path="${with_xercesc}/"
    if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
    then
        xercesc_path="${xercesc_path}xercesc/"
    fi
fi

if test -z "$xercesc_path" -a -n "$prefix"
then
    dnl
    dnl Search in the installation directory
    dnl
    xercesc_path="${prefix}/"
    if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
    then
	xercesc_path="${xercesc_path}xercesc/"
	if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
	then
	    xercesc_path=
	fi
    fi	
fi

if test -z "$xercesc_path"
then
    dnl
    dnl Search in the current directory and parent directory
    dnl
    if test -n ${PORTNAME} -a "${PORTNAME}" = "linux"
    then
    xercesc_path="xercesc/xerces-c1_7_0-linux7.2/"
    if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
    then
	xercesc_path="../xercesc/xerces-c1_7_0-linux7.2/"
	if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
	then
	    xercesc_path=
	fi
    fi
    
    elif test -n ${PORTNAME} -a "${PORTNAME}" = "solaris"
    then
    xercesc_path="xercesc/xerces-c1_7_0-SolForCC/"
    if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
    then
	xercesc_path="../xercesc/xerces-c1_7_0-SolForCC/"
	if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
	then
	    xercesc_path=
	fi
    fi
    fi
fi

if test -z "$xercesc_path"
then
    dnl
    dnl Search in the current directory and parent directory
    dnl
    xercesc_path="xercesc/"
    if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
    then
	xercesc_path="../xercesc/"
	if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
	then
	    xercesc_path=
	fi
    fi	
fi

if test ! -f ${xercesc_path}include/xercesc/internal/XMLScanner.hpp
then
  AC_MSG_ERROR(Can't find Xerces C++ ! Use the --with-xercesc=DIR option.)
fi


if test -n ${PORTNAME} -a "${PORTNAME}" = "solaris"
then
if test "$xercesc_path" = "../xercesc/" -o "$xercesc_path" = "../xercesc/xerces-c1_7_0-SolForCC/"
then
    xercesc_path="\$(top_srcdir)/$xercesc_path"
fi
fi

if test -n ${PORTNAME} -a "${PORTNAME}" = "linux"
then
if test "$xercesc_path" = "../xercesc/" -o "$xercesc_path" = "../xercesc/xerces-c1_7_0-linux7.2/"
then
    xercesc_path="\$(top_srcdir)/$xercesc_path"
fi
fi

XERCESC_CPPFLAGS="$XERCESC_CPPFLAGS -I${xercesc_path}include"
XERCESC_LDFLAGS="$XERCESC_LDFLAGS -L${xercesc_path}lib"
XERCESC_LIBS="$XERCESC_LIBS -lxerces-c1_7_0"
AC_DEFINE(HAVE_XERCESC)
WITH_XERCESC=yes

fi
