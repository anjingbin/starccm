/***********************************************************************
 *
 *  Copyright (c) 2003
 *  StarMiddleware.net	 
 *  www.StarMiddleware.net
 *                        
 *  All Rights Reserved   
 *                           
 * **********************************************************************/


@TOP@

/* Define if your c++ compiler supports iostream in namespace std. */
#undef HAVE_STD_IOSTREAM

/* Define if your c++ compiler supports STL in namespace std. */
#undef HAVE_STD_STL

/* Define if your c++ compiler supports set_terminate and set_unexpected
   in namespace std. */
#undef HAVE_STD_SET_X

/* Define if your c++ compiler can explicitly instanciate templates. */
#undef HAVE_NO_EXPLICIT_TEMPLATES

/* Define if your c++ compiler doesn't have typename. */
#undef HAVE_NO_TYPENAME

/* Define if your c++ compiler supports new-style casts. */
#undef HAVE_NEW_CASTS

/* Define if you don't have a prototype for gethostname(). */
#undef HAVE_NO_GETHOSTNAME_PROTOTYPE

/* Define if socklen_t type is known. */
#undef HAVE_SOCKLEN_T

/* Define if there is no declaration for h_errno. */
#undef HAVE_NO_H_ERRNO_DECLARATION

/* Define if socket operations require address length of type size_t. */
#undef HAVE_SOCKETS_WITH_SIZE_T

/* Define if h_errno has a broken declaration. */
/* (HPUX 11.x defines h_errno without an extern "C" block.) */
#undef HAVE_BROKEN_H_ERRNO_DECL

/* Define if there is no gethostbyname(). */
#undef HAVE_NO_GETHOSTBYNAME

/* Define if OS supports MIT threads. */
#undef HAVE_MIT_THREADS

/* Define if OS supports FSU threads. */
#undef HAVE_FSU_THREADS

/* Define if OS supports posix threads. */
#undef HAVE_POSIX_THREADS

/* Define if OS supports DCE threads. */
#undef HAVE_DCE_THREADS

/* Define if OS supports VxWorks threads. */
#undef HAVE_VXWORKS_THREADS

/* Define if OS supports pthread_attr_setstacksize. */
#undef HAVE_PTHREAD_ATTR_SETSTACKSIZE

/* Define if OS supports pthread_delay_np. */
#undef HAVE_PTHREAD_DELAY_NP

/* Define if OS supports pthread_sched_yield. */
#undef HAVE_SCHED_YIELD

/* Define if OS supports pthread_yield. */
#undef HAVE_PTHREAD_YIELD

/* Define if JThreads/C++ has no support for iostreams. */
#undef HAVE_JTC_NO_IOSTREAM

/* Define if ORBacus/E has no support for tracing. */
#undef HAVE_OBE_NO_TRACE

/* Define if ORBacus/E has no support for collocated servers. */
#undef HAVE_OBE_NO_COLLOC

/* Define if the Event Service is available. */
#undef HAVE_OBEVENT

/* Define if the Notification is available. */
#undef HAVE_OBNOTIFY

/* Define if you have the <new> header file.  */
#undef HAVE_NEW

/* Define if you have the <iostream> header file.  */
#undef HAVE_IOSTREAM

/* Define if you have the <sstream> header file.  */
#undef HAVE_SSTREAM

/* Define if you have the deprecated (!) <strstream> header file.  */
#undef HAVE_STRSTREAM

/* Define if you have the <fstream> header file.  */
#undef HAVE_FSTREAM

/* Define if you have the <iomanip> header file.  */
#undef HAVE_IOMANIP

/* Define if you shared libraries are used */
#undef HAVE_SHARED

/* Define if shl_load() is available instead of dlopen() */
/* (HPUX 10.20 uses shl_load() instead of dlopen()) */
#undef HAVE_SHL_LOAD

/* Define if rand_r is broken. HP-UX 10.20 has a rand_r
   function that takes two arguments */
#undef HAVE_BROKEN_RAND_R
