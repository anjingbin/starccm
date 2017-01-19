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
dnl Check for X11 libraries
dnl ----------------------------------------------------------------------

AC_PATH_XTRA

AC_SUBST(WITH_X11)
AC_SUBST(OB_X11_CPPFLAGS)
AC_SUBST(OB_X11_PRE_LIBS)
AC_SUBST(OB_X11_LIBS)

if test -z "$no_x"
then
    WITH_X11=yes
    AC_CHECK_HEADERS(X11/Intrinsic.h)
else
    WITH_X11=no
fi

OB_X11_CPPFLAGS=
OB_X11_PRE_LIBS=
OB_X11_LIBS=

if test "$WITH_X11" = yes
then
  OB_X11_CPPFLAGS="$X_CFLAGS"
  OB_X11_PRE_LIBS="-lOBX11"
  OB_X11_LIBS="$X_LIBS $X_PRE_LIBS -lXt -lX11 $X_EXTRA_LIBS"
fi
