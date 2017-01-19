dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl                          
dnl **********************************************************************


dnl if test "$RUNCONFIG" != "yes"
dnl then
dnl     AC_MSG_ERROR(You must run ../runconfig before running configure!
dnl 		  See the file ../INSTALL for more information.)
dnl fi


SHLIB_FLAGS=$SHLIB_FLAGS
SHLIBEXT=$SHLIBEXT

AC_CANONICAL_HOST
AC_ISC_POSIX

AC_SUBST(host)
AC_SUBST(host_cpu)
AC_SUBST(host_os)
AC_SUBST(SHLIB_FLAGS)
AC_SUBST(SHLIBEXT)


case $host_os in
 aix*) template=aix ;;
 beos*) template=beos ;;
 bsdi*) template=bsdi ;;
 cygwin*) template=win ;;
 darwin*) template=darwin ;;
 dgux*) template=dgux ;;
 freebsd*) template=freebsd ;;
 hpux*) template=hpux ;;
 irix*) template=irix5 ;;
 linux*) template=linux ;;
 netbsd*) template=netbsd ;;
 nextstep*) template=nextstep ;;
 openbsd*) template=openbsd ;;
 osf*) template=osf ;;
 qnx*) template=qnx4 ;;
 sco*) template=sco ;;
 solaris*) template=solaris ;;
 sunos*) template=sunos4 ;;
 sysv4.2*)
 case $host_vendor in
     univel) template=univel ;;
 esac ;;
 sysv4*) template=svr4 ;;
 sysv5*) template=unixware ;;
 ultrix*) template=ultrix4 ;;
esac

AC_MSG_CHECKING([the current portname])

 if test x"$template" = x"" ; then
  AC_MSG_ERROR([[
*******************************************************************
StarCCM has apparently not been ported to your platform yet.
*******************************************************************
]])
 fi

AC_MSG_RESULT([$template])

PORTNAME=$template
AC_SUBST(PORTNAME)

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_YACC
AC_PROG_LEX
if test -z "$LEXLIB"
then
    AC_CHECK_LIB(l, yywrap, LEXLIB=-ll)
fi


AC_CHECK_HEADERS(stdlib.h stddef.h unistd.h)
AC_CHECK_HEADERS(strings.h bstring.h)
AC_CHECK_HEADERS(termio.h termios.h)
AC_CHECK_HEADERS(sys/time.h sys/types.h sys/select.h)
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
dnl
dnl COMPILERFIX: This does not work on AIX with VAC++ 5.0 so we have
dnl to do the tests manually below.
dnl
dnl AC_CHECK_HEADERS(iostream strstream fstream iomanip)
AC_CHECK_HEADERS(exception exception.h terminate.h unexpected.h)
AC_LANG_RESTORE

AC_CACHE_CHECK(for iostream...,
ac_cv_header_iostream,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
#include <iostream>
,
ac_cv_header_iostream=yes, ac_cv_header_iostream=no)
AC_LANG_RESTORE)

if test $ac_cv_header_iostream = yes
then
    AC_DEFINE(HAVE_IOSTREAM)
fi

AC_CACHE_CHECK(for strstream,
ac_cv_header_strstream,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
#include <strstream>
,
ac_cv_header_strstream=yes, ac_cv_header_strstream=no)
AC_LANG_RESTORE)

if test $ac_cv_header_strstream = yes
then
    AC_DEFINE(HAVE_STRSTREAM)
fi

AC_CACHE_CHECK(for fstream,
ac_cv_header_fstream,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
#include <fstream>
,
ac_cv_header_fstream=yes, ac_cv_header_fstream=no)
AC_LANG_RESTORE)

if test $ac_cv_header_fstream = yes
then
    AC_DEFINE(HAVE_FSTREAM)
fi

AC_CACHE_CHECK(for iomanip,
ac_cv_header_iomanip,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
#include <iomanip>
,
ac_cv_header_iomanip=yes, ac_cv_header_iomanip=no)
AC_LANG_RESTORE)

if test $ac_cv_header_iomanip = yes
then
    AC_DEFINE(HAVE_IOMANIP)
fi


AC_CHECK_FUNCS(strerror)
AC_CHECK_FUNCS(strcasecmp)
AC_CHECK_FUNCS(strncasecmp)


AC_CACHE_CHECK(whether there is a prototype for gethostname,
ac_cv_gethostname_has_prototype,
AC_EGREP_HEADER(gethostname, unistd.h,
		ac_cv_gethostname_has_prototype=yes,
		ac_cv_gethostname_has_prototype=no))

if test $ac_cv_gethostname_has_prototype = no
then
    AC_DEFINE(HAVE_NO_GETHOSTNAME_PROTOTYPE)
fi


AC_CACHE_CHECK(whether there is a declaration for h_errno,
ac_cv_h_errno_is_declared,
AC_EGREP_HEADER(h_errno, netdb.h,
		ac_cv_h_errno_is_declared=yes,
		ac_cv_h_errno_is_declared=no))

if test $ac_cv_h_errno_is_declared = no
then
    AC_DEFINE(HAVE_NO_H_ERRNO_DECLARATION)
fi


AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_CHECK_SIZEOF(char, 1)
AC_CHECK_SIZEOF(short, 2)
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long, 4)
AC_CHECK_SIZEOF(float, 4)
AC_CHECK_SIZEOF(double, 8)
AC_CHECK_SIZEOF(bool, 1)
AC_CHECK_SIZEOF(wchar_t, 2)
AC_CHECK_SIZEOF(long long, 8)
AC_CHECK_SIZEOF(long double, 16)
AC_C_BIGENDIAN
AC_LANG_RESTORE


AC_MSG_CHECKING(what the library suffix is)
AC_SUBST(LIBEXT)
if test -z "$LIBEXT"
then
    LIBEXT=".a"
fi
AC_MSG_RESULT($LIBEXT)



AC_MSG_CHECKING(how to build libraries)
AC_SUBST(AR)
AC_SUBST(ARFLAGS)
if test -z "$AR"
then
    AR="ar"
fi
if test -z "$ARFLAGS"
then
    ARFLAGS="cr"
fi
AC_MSG_RESULT(with $AR $ARFLAGS)


AC_MSG_CHECKING(how to install libraries)
AC_SUBST(INSTALL_LIBRARY)
if test -z "$INSTALL_LIBRARY"
then
    if test "$LIBEXT" = ".sl" # HP needs executable shared libs
    then
        INSTALL_LIBRARY="$INSTALL_PROGRAM"
    else
        INSTALL_LIBRARY="$INSTALL_DATA"
    fi
else
    INSTALL_LIBRARY="$INSTALL_DATA"
fi
AC_MSG_RESULT(with $INSTALL_LIBRARY)


AC_SUBST(SYMLNK_SHLIB)
AC_SUBST(RMLNK_SHLIB)
SYMLNK_SHLIB=":"
RMLNK_SHLIB=":"

if test "$LIBEXT" != ".a"
then
    if test "$LN_S" = "ln -s"
    then
        SYMLNK_SHLIB="ln -f -s"
    else
        SYMLNK_SHLIB="$LN_S -f"
    fi
    RMLNK_SHLIB="rm -f"
fi

SHLIB_FLAGS="$SHLIB_FLAGS"
SHLIBEXT="$SHLIBEXT"

