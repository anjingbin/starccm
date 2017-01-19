dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl
dnl **********************************************************************

l_gethostbyname=
if test -z "$l_socket"
then
    AC_CHECK_LIB(c, gethostbyname, l_gethostbyname=c)
fi
if test -z "$l_gethostbyname"
then
    AC_CHECK_LIB(nsl, gethostbyname, l_gethostbyname=nsl)
fi
if test -z "$l_gethostbyname"
then
    AC_CHECK_LIB(socket, gethostbyname, l_gethostbyname=socket)
fi
if test -z "$l_gethostbyname"
then
    AC_CHECK_LIB(resolv, gethostbyname, l_gethostbyname=resolv)
fi
if test -z "$l_gethostbyname"
then
    AC_CHECK_LIB(net, gethostbyname, l_gethostbyname=net)
fi

l_socket=
if test -z "$l_socket"
then
    if test -n "$l_gethostbyname"
    then
        AC_CHECK_LIB($l_gethostbyname, socket, l_socket=$l_gethostbyname)
    fi
fi
if test -z "$l_socket"
then
    AC_CHECK_LIB(c, socket, l_socket=c)
fi
if test -z "$l_socket"
then
    AC_CHECK_LIB(socket, socket, l_socket=socket)
fi
if test -z "$l_socket"
then
    AC_CHECK_LIB(nsl, socket, l_socket=nsl)
fi
if test -z "$l_socket"
then
    AC_CHECK_LIB(net, socket, l_socket=net)
fi

if test -z "$l_socket"
then
    AC_MSG_ERROR(couldn't find socket() call in any library!)
else
    if test "$l_socket" != "c"
    then
    	LIBS="$LIBS -l$l_socket"
    fi
fi

if test -z "$l_gethostbyname"
then
    AC_MSG_ERROR(couldn't find gethostbyname() call in any library!)
else
    if test "$l_gethostbyname" != "c"
    then
	if test "$l_socket" != "$l_gethostbyname"
        then
  	    LIBS="$LIBS -l$l_gethostbyname"
	fi
    fi
fi


AC_CACHE_CHECK(whether socklen_t may be used for socket operations,
ac_cv_have_socklen_t,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE(
#include <sys/types.h>
#include <sys/socket.h>
,
socklen_t len;
accept((int)0, (struct sockaddr*)0, &len);
,
ac_cv_have_socklen_t=yes, ac_cv_have_socklen_t=no)
AC_LANG_RESTORE)

if test $ac_cv_have_socklen_t = yes
then
    AC_DEFINE(HAVE_SOCKLEN_T)
fi


AC_CACHE_CHECK(for size_t as socket operations length parameter type,
ac_cv_sockets_with_size_t,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE(
#include <sys/types.h>
#include <sys/socket.h>
,
size_t len;
accept((int)0, (struct sockaddr*)0, &len);
,
ac_cv_sockets_with_size_t=yes, ac_cv_sockets_with_size_t=no)
AC_LANG_RESTORE)

if test $ac_cv_sockets_with_size_t = yes
then
    AC_DEFINE(HAVE_SOCKETS_WITH_SIZE_T)
fi


AC_CACHE_CHECK(whether h_errno has bad declaration,
ac_cv_herrno_bad_decl,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE(
#include <netdb.h>
,
int a;
if(h_errno == NETDB_INTERNAL) a = 0;
,
ac_cv_herrno_bad_decl=no, ac_cv_herrno_bad_decl=yes)
AC_LANG_RESTORE)

if test $ac_cv_herrno_bad_decl = yes
then
    AC_DEFINE(HAVE_BROKEN_H_ERRNO_DECL)
fi
