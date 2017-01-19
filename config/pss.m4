dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl
dnl Author: An Jingbin	anjb@sina.com	                         
dnl **********************************************************************

AC_SUBST(PSS_CPPFLAGS)
AC_SUBST(PSS_LDFLAGS)
AC_SUBST(PSS_IDLDIR)
AC_SUBST(PSS_LIBS)
AC_SUBST(PSS_LIBVER)
AC_SUBST(WITH_PSS)
AC_SUBST(PSS_IDLFLAGS)
AC_SUBST(PSDL)

PSS_CPPFLAGS=
PSS_LDFLAGS=
PSS_IDLDIR=
PSS_LIBS=
PSS_LIBVER=
WITH_PSS=no
PSS_IDLFLAGS=
PSDL=

AC_ARG_WITH(pss,
  --with-pss=DIR     make use of Persistent Service)
	
if test -z "$with_pss" -o "$with_pss" != "no"
then


pss_path=

if test -z "$pss_path" -a -n "$with_pss" -a "$with_pss" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-pss option
    dnl
    pss_path="${with_pss}/"
    if test ! -f ${pss_path}include/PSS.h
    then
        pss_path="${pss_path}pss/"
    fi
fi

if test -z "$pss_path" -a -n "$with_ob" -a "$with_ob" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ob option
    dnl
    pss_path="${with_ob}/"
    if test ! -f ${pss_path}include/PSS.h
    then
    	pss_path="${with_ob}/pss/"
	if test ! -f ${pss_path}include/PSS.h
    	then
	    pss_path=
    	fi
    fi
fi

if test -z "$pss_path" -a -n "$prefix"
then
    dnl
    dnl Search in the installation directory
    dnl
    pss_path="${prefix}/"
    if test ! -f ${pss_path}include/PSS.h
    then
	pss_path="${pss_path}pss/"
	if test ! -f ${pss_path}include/PSS.h
	then
	    pss_path=
	fi
    fi	
fi

if test -z "$pss_path"
then
    dnl
    dnl Search in the current directory or ../pss
    dnl	
    pss_path="./"
    if test ! -f ${pss_path}include/PSS.h
    then
	pss_path="../pss/"
	if test ! -f ${pss_path}include/PSS.h
	then
	    pss_path=
	fi
    fi
fi

if test ! -f ${pss_path}include/PSS.h
then
  AC_MSG_ERROR(Can't find Persistent Service! Use the --with-pss=DIR option.)
fi


dnl
dnl PersistentState Service shared library versioning
dnl
pss_ver=""
if test "$LIBEXT" != ".a"
then
    pss_ver=`grep '#define PSS_VERSION' \
 	< ${pss_path}include/PSS.h | sed -e 's/^.* \"//' -e 's/\"$//'`
    PSS_LIBVER=".${pss_ver}"
fi

dnl
dnl Add $(top_srcdir) to relative paths (current directory and ../ob)
dnl
if test "$pss_path" = "./"
then
    pss_path="\$(top_srcdir)/"
fi

if test "$pss_path" = "../pss/"
then
    pss_path="\$(top_srcdir)/$pss_path"
fi

PSS_CPPFLAGS="$PSS_CPPFLAGS -I${pss_path}include"
PSS_LDFLAGS="$PSS_LDFLAGS -L${pss_path}lib"
PSS_IDLDIR="${pss_path}idl"
PSS_IDLFLAGS="-I${PSS_IDLDIR}"
PSS_LIBS="$PSS_LIBS -lPSS"
AC_DEFINE(HAVE_PSS)
WITH_PSS=yes

if test -n "$ORB_NAME" -a "$ORB_NAME" = "OB"
then
PSDL="${pss_path}bin/psdl.${PORTNAME}"
elif test -n "$ORB_NAME" -a "$ORB_NAME" = "STARBUS"
then
PSDL="${pss_path}bin/star-psdl.${PORTNAME}"
fi

fi

