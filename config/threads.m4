dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl                          
dnl **********************************************************************

with_dce=no

WITH_DCE=no
AC_ARG_WITH(mtl-dce,
   --with-mtl-dce          compile MTL/C++ with support for DCE,
with_dce=yes
)

threads=no

if test $threads = no -a $with_dce = no
then
    AC_CACHE_CHECK(for FSU threads, ac_cv_fsu_threads,
    AC_TRY_COMPILE_GLOBAL(
    ,
    #include <pthread.h>
    void f()
    { pthread_queue_t q; }
    ,
    ac_cv_fsu_threads=yes, ac_cv_fsu_threads=no))

    if test $ac_cv_fsu_threads = yes
    then
	AC_DEFINE(HAVE_FSU_THREADS)
	AC_DEFINE(HAVE_POSIX_THREADS)
	threads=yes
    fi
else
    ac_cv_fsu_threads=no
fi

if test $threads = no -a $with_dce = no
then
    AC_CACHE_CHECK(for MIT threads, ac_cv_mit_threads,
    AC_TRY_COMPILE_GLOBAL(
    ,
    #include <pthread.h>
    #include <pthread/prio_queue.h>
    ,
    ac_cv_mit_threads=yes, ac_cv_mit_threads=no))

    if test $ac_cv_mit_threads = yes
    then
	AC_DEFINE(HAVE_MIT_THREADS)
	AC_DEFINE(HAVE_POSIX_THREADS)
	threads=yes
    fi
else
    ac_cv_mit_threads=no
fi

if test $threads = no -a $with_dce = no
then
    AC_CACHE_CHECK(for regular POSIX threads, ac_cv_posix_threads,
    AC_TRY_COMPILE_GLOBAL(
    ,
    #include <pthread.h>
    void f()
    { pthread_mutex_t m; pthread_mutex_init(&m, 0); }
    ,
    ac_cv_posix_threads=yes, ac_cv_posix_threads=no))

    if test $ac_cv_posix_threads = yes
    then
	AC_DEFINE(HAVE_POSIX_THREADS)
	threads=yes
    fi
else
    ac_cv_posix_threads=no
fi

if test $threads = no
then
    AC_CACHE_CHECK(for DCE threads, ac_cv_dce_threads,
    AC_TRY_COMPILE_GLOBAL(
    ,
    #include <pthread.h>
    void f()
    { pthread_mutex_t m;
      pthread_mutex_init(&m, pthread_mutexattr_default); }
    ,
    ac_cv_dce_threads=yes, ac_cv_dce_threads=no))

    if test $ac_cv_dce_threads = yes
    then
	AC_DEFINE(HAVE_DCE_THREADS)
	threads=yes
    fi
else
    ac_cv_dce_threads=no
fi

dnl ----------------------------------------------------------------------
dnl Check for header files
dnl ----------------------------------------------------------------------

AC_CHECK_HEADERS(sched.h)
AC_CHECK_HEADERS(sys/sched.h)

dnl ----------------------------------------------------------------------
dnl Check for thread libraries
dnl ----------------------------------------------------------------------

if test $threads = yes
then
    l_thread=
    if test "$ac_cv_fsu_threads" = "yes"
    then
	AC_CHECK_LIB(malloc, malloc)
	AC_CHECK_LIB(gthreads, pthread_self,l_thread=gthreads)
    else
	dnl
        dnl We check for __pthread_self, before pthread_self to work
        dnl around stupid DEC UNIX problems.
	dnl
	if test -z "$l_thread" -a "$ac_cv_dce_threads" = "no"; then
	    AC_CHECK_LIB(pthread, __pthread_self, l_thread=pthread)
	fi
	if test -z "$l_thread" -a "$ac_cv_dce_threads" = "no"; then
	    AC_CHECK_LIB(pthread, pthread_self, l_thread=pthread)
	fi
	if test -z "$l_thread" -a "$ac_cv_dce_threads" = "no"; then
	    AC_CHECK_LIB(pthreads, pthread_self, l_thread=pthreads)
	fi
	if test -z "$l_thread"; then
	    AC_CHECK_LIB(cma, pthread_self, l_thread=cma)
	fi
	if test -z "$l_thread"; then
	    AC_CHECK_LIB(c, pthread_self, l_thread=c)
	fi
	l_nanosleep=
	AC_CHECK_LIB(c, nanosleep, l_nanosleep=c)
	if test -z "$l_nanosleep"
	then
  	    AC_CHECK_LIB(posix4, nanosleep, l_nanosleep=posix4)
	fi
	if test -z "$l_nanosleep"
	then
	    AC_CHECK_LIB(rt, nanosleep, l_nanosleep=rt)
	fi
	if test ! -z "$l_nanosleep"
	then
	    if test "$l_nanosleep" != "c"
            then
		LIBS="$LIBS -l$l_nanosleep"
	    fi
	fi
    fi
else
    AC_MSG_ERROR(Operating system does not support threads!)
fi

if test $l_thread != 'c'; then
    LIBS="$LIBS -l$l_thread"
fi

dnl ----------------------------------------------------------------------
dnl Check for thread attributes
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(for pthread_attr_setstacksize, ac_cv_pthread_attr_setstacksize,
AC_TRY_LINK(#include <pthread.h>
,
pthread_attr_setstacksize(0, 0)
,
ac_cv_pthread_attr_setstacksize=yes, ac_cv_pthread_attr_setstacksize=no))
if test $ac_cv_pthread_attr_setstacksize = yes
then
    AC_DEFINE(HAVE_PTHREAD_ATTR_SETSTACKSIZE)
fi

AC_CACHE_CHECK(for pthread_delay_np, ac_cv_pthread_delay_np,
AC_TRY_LINK(#include <pthread.h>
,
pthread_delay_np(0)
,
ac_cv_pthread_delay_np=yes, ac_cv_pthread_delay_np=no))
if test $ac_cv_pthread_delay_np = yes
then
    AC_DEFINE(HAVE_PTHREAD_DELAY_NP)
fi

ac_cv_sched_yield=no
AC_CHECK_LIB($l_thread,sched_yield,ac_cv_sched_yield=yes)
if test $ac_cv_sched_yield = yes
then
    AC_DEFINE(HAVE_SCHED_YIELD)
else
    ac_cv_pthread_yield=no
    AC_CHECK_LIB($l_thread,pthread_yield,ac_cv_pthread_yield=yes)
    if test $ac_cv_pthread_yield = yes
    then
        AC_DEFINE(HAVE_PTHREAD_YIELD)
    fi
fi
