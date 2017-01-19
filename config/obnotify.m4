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
dnl ORBacus-specific configuration
dnl ----------------------------------------------------------------------

AC_SUBST(OBNOTIFY_CPPFLAGS)
AC_SUBST(OBNOTIFY_LDFLAGS)
AC_SUBST(OBNOTIFY_IDLDIR)
AC_SUBST(OBNOTIFY_LIBS)
AC_SUBST(WITH_OBNOTIFY)

OBNOTIFY_CPPFLAGS=
OBNOTIFY_LDFLAGS=
OBNOTIFY_IDLDIR=
OBNOTIFY_LIBS=
WITH_OBNOTIFY=no

AC_ARG_WITH(obnotify,
  --with-obnotify=DIR     make use of ORBacus Notify,

if test -z "$with_obnotify" -o "$with_obnotify" != "no"
then

dnl
dnl Discover the ORBacus Notify path
dnl

obnotify_path=

if test -z "$obnotify_path" -a -n "$with_obnotify" -a "$with_obnotify" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-obnotify option
    dnl
    obnotify_path="${with_obnotify}/"
    if test ! -f ${obnotify_path}include/OB/NotifyService.h
    then
        obnotify_path="${obnotify_path}notify/"
    fi
fi

if test -z "$obnotify_path" -a -n "$with_ob" -a "$with_ob" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ob option
    dnl
    obnotify_path="${with_ob}/"
    if test ! -f ${obnotify_path}include/OB/NotifyService.h
    then
    	obnotify_path="${with_ob}/notify/"
	if test ! -f ${obnotify_path}include/OB/NotifyService.h
    	then
	    obnotify_path=
    	fi
    fi
fi

if test -z "$obnotify_path" -a -n "$prefix"
then
    dnl
    dnl Search in the installation directory
    dnl
    obnotify_path="${prefix}/"
    if test ! -f ${obnotify_path}include/OB/NotifyService.h
    then
	obnotify_path="${obnotify_path}notify/"
	if test ! -f ${obnotify_path}include/OB/NotifyService.h
	then
	    obnotify_path=
	fi
    fi	
fi

if test -z "$obnotify_path"
then
    if test ! -f include/OB/NotifyService.h
    then
	obnotify_path="../notify/"
    fi
fi

if test ! -f ${obnotify_path}include/OB/NotifyService.h
then
  AC_MSG_ERROR(Can't find ORBacus Notify! Use the --with-obnotify=DIR option.)
fi

if test "$obnotify_path" = "../notify/"
then
    obnotify_path="\$(top_srcdir)/$obnotify_path"
fi

OBNOTIFY_CPPFLAGS="$OBNOTIFY_CPPFLAGS -I${obnotify_path}include"
OBNOTIFY_LDFLAGS="$OBNOTIFY_LDFLAGS -L${obnotify_path}lib"
OBNOTIFY_IDLDIR="${obnotify_path}idl"
OBNOTIFY_LIBS="$OBNOTIFY_LIBS -lOBNotifyService  -lOBNotifyFilter"
OBNOTIFY_LIBS="$OBNOTIFY_LIBS -lOBNotify"
AC_DEFINE(HAVE_OBNOTIFY)
WITH_OBNOTIFY=yes

fi
)
