dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl
dnl Author:	An JingBin  anjb@sina.com
dnl
dnl **********************************************************************

ORB_NAME=$ORB_NAME

AC_SUBST(ORB_NAME)

 
if test -n "$ORB_NAME" -a "$ORB_NAME" = "OB"
then 


dnl ----------------------------------------------------------------------
dnl ORBacus-specific configuration
dnl ----------------------------------------------------------------------

AC_SUBST(OB_CPPFLAGS)
AC_SUBST(OB_LDFLAGS)
AC_SUBST(OB_IDLFLAGS)
AC_SUBST(OB_LIBS)
AC_SUBST(OB_LIBVER)
AC_SUBST(IDL)
AC_SUBST(OB_IDLDIR)

OB_CPPFLAGS=
OB_LDFLAGS=
OB_IDLFLAGS=
OB_LIBS=
OB_LIBVER=
IDL=
OB_IDLDIR=


dnl
dnl Discover the ORBacus path.
dnl
ob_path=

if test -z "$ob_path" -a -n "$with_orb" -a "$with_orb" != "yes"
then
    if test "$with_orb" = "no"
    then
	AC_MSG_ERROR(ORBacus required!)
    fi

    dnl
    dnl Search in the directory specified by the --with-orb option
    dnl
    ob_path="${with_orb}/"
    if test ! -f ${ob_path}include/OB/CORBA.h
    then
	ob_path="${ob_path}ob/"
	if test ! -f ${ob_path}include/OB/CORBA.h
	then
	    ob_path=
	fi
    fi
fi

dnl
dnl Error message if we didn't find the ORBacus path.
dnl
if test ! -f ${ob_path}include/OB/CORBA.h
then
    AC_MSG_ERROR(Can't find ORBacus! Use the --with-orb=DIR option.)
fi

dnl
dnl ORBacus shared library versioning
dnl
ob_ver=""
if test "$LIBEXT" != ".a"
then
    ob_ver=`grep '#define OB_VERSION' \
 	< ${ob_path}include/OB/Basic.h | sed -e 's/^.* \"//' -e 's/\"$//'`
    OB_LIBVER=".${ob_ver}"
fi

dnl
dnl Add $(top_srcdir) to relative paths (current directory and ../ob)
dnl
if test "$ob_path" = "./"
then
    ob_path="\$(top_srcdir)/"
fi

if test "$ob_path" = "../ob/"
then
    ob_path="\$(top_srcdir)/../ob/"
fi

dnl --------------------------------
dnl modified by baixiaobo 2004.03.17
dnl --------------------------------
dnl if test "$ORB_NAME" = "OB"
dnl then

dnl fi

jtc_path="$ob_path"

OB_LDFLAGS="$OB_LDFLAGS  -L${ob_path}lib"
OB_CPPFLAGS="$OB_CPPFLAGS -I${ob_path}include"
OB_IDLFLAGS="$OB_IDLFLAGS -I${ob_path}idl -DORBacus"
OB_LIBS="$OB_LIBS -lOB"
IDL="${ob_path}bin/idl"
OB_IDLDIR="${ob_path}idl"
JTC_LDFLAGS="-L${jtc_path}lib"
JTC_CPPFLAGS="-I${jtc_path}include"
JTC_LIBS="-lJTC"

OB_LIBS="$OB_LIBS $JTC_LIBS"


elif test -n "$ORB_NAME" -a "$ORB_NAME" = "STARBUS"
then 


dnl ----------------------------------------------------------------------
dnl StarBus-specific configuration
dnl ----------------------------------------------------------------------

AC_SUBST(OB_CPPFLAGS)
AC_SUBST(OB_LDFLAGS)
AC_SUBST(OB_IDLFLAGS)
AC_SUBST(OB_LIBS)
AC_SUBST(OB_LIBVER)
AC_SUBST(IDL)
AC_SUBST(OB_IDLDIR)

OB_CPPFLAGS=
OB_LDFLAGS=
OB_IDLFLAGS=
OB_LIBS=
OB_LIBVER=
IDL=
OB_IDLDIR=

dnl
dnl Discover the StarBus path.
dnl
star_path=

if test -z "$star_path" -a -n "$with_orb" -a "$with_orb" != "yes"
then
    if test "$with_orb" = "no"
    then
	AC_MSG_ERROR(StarBus required!)
    fi

    dnl
    dnl Search in the directory specified by the --with-orb option
    dnl
    star_path="${with_orb}/"
    if test ! -f ${star_path}include/STAR/CORBA.h
    then
	star_path="${star_path}star/"
	if test ! -f ${star_path}include/STAR/CORBA.h
	then
	    star_path=
	fi
    fi
fi

dnl
dnl Error message if we didn't find the StarBus path.
dnl
if test ! -f ${star_path}include/STAR/CORBA.h
then
    AC_MSG_ERROR(Can't find StarBus! Use the --with-orb=DIR option.)
fi

dnl
dnl StarBus shared library versioning
dnl
star_ver=""
if test "$LIBEXT" != ".a"
then
    star_ver=`grep '#define STAR_VERSION' \
 	< ${star_path}include/STAR/Basic.h | sed -e 's/^.* \"//' -e 's/\"$//'`
    STAR_LIBVER=".${star_ver}"
fi


mtl_path="$star_path"

OB_LDFLAGS="$OB_LDFLAGS  -L${star_path}lib"
OB_CPPFLAGS="$OB_CPPFLAGS -I${star_path}include"
OB_IDLFLAGS="$OB_IDLFLAGS -I${star_path}idl -DStarBus"
OB_LIBS="$OB_LIBS -lSTAR"
IDL="${star_path}bin/idl"
OB_IDLDIR="${star_path}idl"
MTL_LDFLAGS="-L${mtl_path}lib"
MTL_CPPFLAGS="-I${mtl_path}include"
MTL_LIBS="-lMTL"

OB_LIBS="$OB_LIBS $MTL_LIBS"




elif test -n "$ORB_NAME" -a "$ORB_NAME" = "TAO"
then 
	AC_MSG_ERROR(TAO is not supported yet,check and modify the file ./config/orb.m4.)

dnl ----------------------------------------------------------------------
dnl TAO-specific configuration
dnl ----------------------------------------------------------------------


AC_SUBST(OB_CPPFLAGS)
AC_SUBST(OB_LDFLAGS)
AC_SUBST(OB_IDLFLAGS)
AC_SUBST(OB_LIBS)
AC_SUBST(OB_LIBVER)
AC_SUBST(IDL)

OB_CPPFLAGS=
OB_LDFLAGS=
OB_IDLFLAGS=
OB_LIBS=
OB_LIBVER=
IDL=


dnl
dnl Discover the ORBacus path.
dnl
ob_path=

if test -z "$ob_path" -a -n "$with_orb" -a "$with_orb" != "yes"
then
    if test "$with_orb" = "no"
    then
	AC_MSG_ERROR(ORBacus required!)
    fi

    dnl
    dnl Search in the directory specified by the --with-orb option
    dnl
    ob_path="${with_orb}/"
    if test ! -f ${ob_path}include/OB/CORBA.h
    then
	ob_path="${ob_path}ob/"
	if test ! -f ${ob_path}include/OB/CORBA.h
	then
	    ob_path=
	fi
    fi
fi

dnl
dnl Error message if we didn't find the ORBacus path.
dnl
if test ! -f ${ob_path}include/OB/CORBA.h
then
    AC_MSG_ERROR(Can't find ORBacus! Use the --with-orb=DIR option.)
fi

dnl
dnl ORBacus shared library versioning
dnl
ob_ver=""
if test "$LIBEXT" != ".a"
then
    ob_ver=`grep '#define OB_VERSION' \
 	< ${ob_path}include/OB/Basic.h | sed -e 's/^.* \"//' -e 's/\"$//'`
    OB_LIBVER=".${ob_ver}"
fi

dnl
dnl Add $(top_srcdir) to relative paths (current directory and ../ob)
dnl
if test "$ob_path" = "./"
then
    ob_path="\$(top_srcdir)/"
fi

if test "$ob_path" = "../ob/"
then
    ob_path="\$(top_srcdir)/../ob/"
fi

dnl --------------------------------
dnl modified by baixiaobo 2004.03.17
dnl --------------------------------
dnl if test "$ORB_NAME" = "OB"
dnl then

dnl fi

jtc_path="$ob_path"

OB_LDFLAGS="$OB_LDFLAGS  -L${ob_path}lib"
OB_CPPFLAGS="$OB_CPPFLAGS -I${ob_path}include"
OB_IDLFLAGS="$OB_IDLFLAGS -I${ob_path}idl -DORBacus"
OB_LIBS="$OB_LIBS -lOB"
IDL="${ob_path}bin/idl"

JTC_LDFLAGS="-L${jtc_path}lib"
JTC_CPPFLAGS="-I${jtc_path}include"
JTC_LIBS="-lJTC"

OB_LIBS="$OB_LIBS $JTC_LIBS"


fi



