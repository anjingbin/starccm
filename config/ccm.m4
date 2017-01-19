dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl 
dnl Author An Jingbin	anjb@sina.com                         
dnl **********************************************************************

AC_SUBST(CCM_CPPFLAGS)
AC_SUBST(CCM_LDFLAGS)
AC_SUBST(CCM_IDLDIR)
AC_SUBST(CCM_LIBS)
AC_SUBST(CCM_LIBVER)
AC_SUBST(WITH_CCM)
AC_SUBST(IDL3)
AC_SUBST(CIDL)
AC_SUBST(CCM_IDLFLAGS)
AC_SUBST(CCM_IDL3FLAGS)
AC_SUBST(CCM_CIDLFLAGS)

CCM_CPPFLAGS=
CCM_LDFLAGS=
CCM_IDLDIR=
CCM_LIBS=
CCM_LIBVER=
WITH_CCM=no
IDL3=
CIDL=
CCM_IDLFLAGS=
CCM_IDL3FLAGS=
CCM_CIDLFLAGS=


AC_ARG_WITH(ccm,
  --with-ccm=DIR     make use of OB-CCM)
	
if test -z "$with_ccm" -o "$with_ccm" != "no"
then


ccm_path=

if test -z "$ccm_path" -a -n "$with_ccm" -a "$with_ccm" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ccm option
    dnl
    ccm_path="${with_ccm}/"
    if test ! -f ${ccm_path}include/CCM.h
    then
        ccm_path="${ccm_path}ccm/"
    fi
fi

if test -z "$ccm_path" -a -n "$with_ob" -a "$with_ob" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ob option
    dnl
    ccm_path="${with_ob}/"
    if test ! -f ${ccm_path}include/CCM.h
    then
    	ccm_path="${with_ob}/ccm/"
	if test ! -f ${ccm_path}include/CCM.h
    	then
	    ccm_path=
    	fi
    fi
fi

if test -z "$ccm_path" -a -n "$prefix"
then
    dnl
    dnl Search in the installation directory
    dnl
    ccm_path="${prefix}/"
    if test ! -f ${ccm_path}include/CCM.h
    then
	ccm_path="${ccm_path}ccm/"
	if test ! -f ${ccm_path}include/CCM.h
	then
	    ccm_path=
	fi
    fi	
fi

if test -z "$ccm_path"
then
    dnl
    dnl Search in the current directory or ../ccm
    dnl	
    ccm_path="./"
    if test ! -f ${ccm_path}include/CCM.h
    then
	ccm_path="../ccm/"
	if test ! -f ${ccm_path}include/CCM.h
	then
	    ccm_path=
	fi
    fi
fi

if test -z "$ccm_path"
then
    if test ! -f include/CCM.h
    then
	ccm_path="../ccm/"
    fi
fi

if test ! -f ${ccm_path}include/CCM.h
then
  AC_MSG_ERROR(Can't find OB_CCM! Use the --with-ccm=DIR option.)
fi

dnl
dnl CCM shared library versioning
dnl
ccm_ver=""
if test "$LIBEXT" != ".a"
then
    ccm_ver=`grep '#define CCM_VERSION' \
 	< ${ccm_path}include/Basic.h | sed -e 's/^.* \"//' -e 's/\"$//'`
    CCM_LIBVER=".${ccm_ver}"
fi

dnl
dnl Add $(top_srcdir) to relative paths (current directory and ../ccm)
dnl
if test "$ccm_path" = "./"
then
    ccm_path="\$(top_srcdir)/"
fi

if test "$ccm_path" = "../ccm/"
then
    ccm_path="\$(top_srcdir)/$ccm_path"
fi

AC_DEFINE(HAVE_CCM)
WITH_CCM=yes
CCM_LIBS="$CCM_LIBS -lCCM"

IDL3="${ccm_path}bin/idl3.${PORTNAME}"
CIDL="${ccm_path}bin/cidl.${PORTNAME}"

if test -n "$ORB_NAME" -a "$ORB_NAME" = "OB"
then

CCM_CPPFLAGS="-I${ccm_path}include -DORBacus"
CCM_LDFLAGS="-L${ccm_path}lib"
CCM_IDLDIR="${ccm_path}idl"
CCM_IDLFLAGS="-I${ccm_path}idl -DORBacus"

CCM_CIDLFLAGS="-DORBacus"
CCM_IDL3FLAGS="-DORBacus"

elif test -n "$ORB_NAME" -a "$ORB_NAME" = "STARBUS"
then

CCM_CPPFLAGS="-I${ccm_path}include -DStarBus  -DSTARCCM_MTL"
CCM_LDFLAGS="-L${ccm_path}lib"
CCM_IDLDIR="${ccm_path}idl"
CCM_IDLFLAGS="-I${ccm_path}idl -DStarBus"

CCM_CIDLFLAGS="-DStarBus"
CCM_IDL3FLAGS="-DStarBus"

fi

fi

