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

AC_SUBST(OTS_CPPFLAGS)
AC_SUBST(OTS_LDFLAGS)
AC_SUBST(OTS_IDLDIR)
AC_SUBST(OTS_LIBS)
AC_SUBST(OTS_LIBVER)
AC_SUBST(WITH_OTS)
AC_SUBST(OTS_IDLFLAGS)

OTS_CPPFLAGS=
OTS_LDFLAGS=
OTS_IDLDIR=
OTS_LIBS=
OTS_LIBVER=
WITH_OTS=no
OTS_IDLFLAGS=

AC_ARG_WITH(ots,
  --with-ots=DIR     make use of Transaction Service)
	
if test -z "$with_ots" -o "$with_ots" != "no"
then


if test -n "$ORB_NAME" -a "$ORB_NAME" = "OB"
then


ots_path=

if test -z "$ots_path" -a -n "$with_ots" -a "$with_ots" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ots option
    dnl
    ots_path="${with_ots}/"
    if test ! -f ${ots_path}include/OTS.h
    then
        ots_path="${ots_path}ots/"
    fi
fi

if test -z "$ots_path" -a -n "$with_ob" -a "$with_ob" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ob option
    dnl
    ots_path="${with_ob}/"
    if test ! -f ${ots_path}include/OTSHeader.h
    then
    	ots_path="${with_ob}/ots/"
	if test ! -f ${ots_path}include/OTSHeader.h
    	then
	    ots_path=
    	fi
    fi
fi

if test -z "$ots_path" -a -n "$prefix"
then
    dnl
    dnl Search in the installation directory
    dnl
    ots_path="${prefix}/"
    if test ! -f ${ots_path}include/OTSHeader.h
    then
	ots_path="${ots_path}ots/"
	if test ! -f ${ots_path}include/OTSHeader.h
	then
	    ots_path=
	fi
    fi	
fi

if test -z "$ots_path"
then
    dnl
    dnl Search in the current directory or ../ots
    dnl	
    ots_path="./"
    if test ! -f ${ots_path}include/OTSHeader.h
    then
	ots_path="../ots/"
	if test ! -f ${ots_path}include/OTSHeader.h
	then
	    ots_path=
	fi
    fi
fi

if test -z "$ots_path"
then
    if test ! -f include/OTSHeader.h
    then
	ots_path="../ots/"
    fi
fi

if test ! -f ${ots_path}include/OTSHeader.h
then
  AC_MSG_ERROR(Can't find Transaction Service! Use the --with-ots=DIR option.)
fi


dnl
dnl OTS shared library versioning
dnl
ots_ver=""
if test "$LIBEXT" != ".a"
then
    ots_ver=`grep '#define OTS_VERSION' \
 	< ${ots_path}include/OTSHeader.h | sed -e 's/^.* \"//' -e 's/\"$//'`
    OTS_LIBVER=".${ots_ver}"
fi

dnl
dnl Add $(top_srcdir) to relative paths (current directory and ../ots)
dnl
if test "$ots_path" = "./"
then
    ots_path="\$(top_srcdir)/"
fi

if test "$ots_path" = "../ots/"
then
    ots_path="\$(top_srcdir)/$ots_path"
fi

fi

if test -n "$ORB_NAME" -a "$ORB_NAME" = "STARBUS"
then


ots_path=

if test -z "$ots_path" -a -n "$with_ots" -a "$with_ots" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ots option
    dnl
    ots_path="${with_ots}/"
    if test ! -f ${ots_path}include/OTS.h
    then
        ots_path="${ots_path}star-ots/"
    fi
fi

if test -z "$ots_path" -a -n "$with_ob" -a "$with_ob" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ob option
    dnl
    ots_path="${with_ob}/"
    if test ! -f ${ots_path}include/OTSHeader.h
    then
    	ots_path="${with_ob}/star-ots/"
	if test ! -f ${ots_path}include/OTSHeader.h
    	then
	    ots_path=
    	fi
    fi
fi

if test -z "$ots_path" -a -n "$prefix"
then
    dnl
    dnl Search in the installation directory
    dnl
    ots_path="${prefix}/"
    if test ! -f ${ots_path}include/OTSHeader.h
    then
	ots_path="${ots_path}ots/"
	if test ! -f ${ots_path}include/OTSHeader.h
	then
	    ots_path=
	fi
    fi	
fi

if test -z "$ots_path"
then
    dnl
    dnl Search in the current directory or ../ots
    dnl	
    ots_path="./"
    if test ! -f ${ots_path}include/OTSHeader.h
    then
	ots_path="../star-ots/"
	if test ! -f ${ots_path}include/OTSHeader.h
	then
	    ots_path=
	fi
    fi
fi

if test -z "$ots_path"
then
    if test ! -f include/OTSHeader.h
    then
	ots_path="../star-ots/"
    fi
fi

if test ! -f ${ots_path}include/OTSHeader.h
then
  AC_MSG_ERROR(Can't find Transaction Service! Use the --with-ots=DIR option.)
fi


dnl
dnl OTS shared library versioning
dnl
ots_ver=""
if test "$LIBEXT" != ".a"
then
    ots_ver=`grep '#define OTS_VERSION' \
 	< ${ots_path}include/OTSHeader.h | sed -e 's/^.* \"//' -e 's/\"$//'`
    OTS_LIBVER=".${ots_ver}"
fi

dnl
dnl Add $(top_srcdir) to relative paths (current directory and ../ots)
dnl
if test "$ots_path" = "./"
then
    ots_path="\$(top_srcdir)/"
fi

if test "$ots_path" = "../star-ots/"
then
    ots_path="\$(top_srcdir)/$ots_path"
fi


fi


OTS_CPPFLAGS="$OTS_CPPFLAGS -I${ots_path}include"
OTS_LDFLAGS="$OTS_LDFLAGS -L${ots_path}lib"
OTS_IDLDIR="${ots_path}idl"
OTS_IDLFLAGS="-I${OTS_IDLDIR}"
OTS_LIBS="$OTS_LIBS -lrt -lresolv -lOTS -lRES -lOTSCurrent"
AC_DEFINE(HAVE_OTS)
WITH_OTS=yes

fi

