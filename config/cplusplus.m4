dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl                          
dnl **********************************************************************

AC_CACHE_CHECK(whether c++ compiler supports RTTI,
ac_cv_rtti,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
struct X { virtual ~X() { } };
struct Y : virtual public X { virtual ~Y() { } };
void f() { Y y; X* xp = &y; Y* yp = dynamic_cast<Y*>(xp); }
,
ac_cv_rtti=yes, ac_cv_rtti=no)
AC_LANG_RESTORE)

if test $ac_cv_rtti = no
then
    AC_MSG_ERROR(Compiler does not support RTTI!)
fi

dnl ----------------------------------------------------------------------
dnl Check whether c++ compiler supports exception handling
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler supports exception handling,
ac_cv_exception_handling,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
void f() { throw "abc"; }
void g() { try { f(); } catch(char*) { } }
,
ac_cv_exception_handling=yes, ac_cv_exception_handling=no)
AC_LANG_RESTORE)

if test $ac_cv_exception_handling = no
then
    AC_MSG_ERROR(Compiler does not support exception handling!)
fi

dnl ----------------------------------------------------------------------
dnl Check whether environment supports set unexpected
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler supports set_unexpected,
ac_cv_set_unexpected,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
#include <exception.h>
void test() { set_unexpected(0); }
,
ac_cv_set_unexpected=yes, ac_cv_set_unexpected=no)
AC_LANG_RESTORE)

if test $ac_cv_set_unexpected = no
then
    AC_DEFINE(HAVE_NO_SET_UNEXPECTED)
fi

dnl ----------------------------------------------------------------------
dnl Check whether c++ compiler supports namespaces
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler supports namespaces,
ac_cv_namespace,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
namespace N { typedef int I; }; N::I i;
,
ac_cv_namespace=yes, ac_cv_namespace=no)
AC_LANG_RESTORE)

if test $ac_cv_namespace = no
then
    AC_MSG_ERROR(Compiler does not support namespaces!)
fi

dnl ----------------------------------------------------------------------
dnl Check whether c++ compiler supports iostream in namespace std
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler supports iostream in namespace std,
ac_cv_std_iostream,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
if test $ac_cv_header_iostream = yes
then
AC_TRY_COMPILE_GLOBAL(
,
#include <iostream>
void f(std::ostream s) { s << "xxx\n"; }
,
ac_cv_std_iostream=yes, ac_cv_std_iostream=no)
else
AC_TRY_COMPILE_GLOBAL(
,
#include <iostream.h>
void f(std::ostream s) { s << "xxx\n"; }
,
ac_cv_std_iostream=yes, ac_cv_std_iostream=no)
fi
AC_LANG_RESTORE)

if test $ac_cv_std_iostream = yes
then
    AC_DEFINE(HAVE_STD_IOSTREAM)
fi

dnl ----------------------------------------------------------------------
dnl Check whether c++ compiler supports STL in namespace std
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler supports STL in namespace std,
ac_cv_std_stl,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
#include <vector>
typedef std::vector<int> IntVector;
,
ac_cv_std_stl=yes, ac_cv_std_stl=no)
AC_LANG_RESTORE)

if test $ac_cv_std_stl = yes
then
    AC_DEFINE(HAVE_STD_STL)
fi

dnl ----------------------------------------------------------------------
dnl Check whether c++ compiler can explicitly instantiate templates
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler can explicitly instantiate templates,
ac_cv_explicit_templates,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
template<class T> class X { T t; }; template class X<int>; X<int> x;
,
ac_cv_explicit_templates=yes, ac_cv_explicit_templates=no)
AC_LANG_RESTORE)

if test $ac_cv_explicit_templates = no
then
    AC_DEFINE(HAVE_NO_EXPLICIT_TEMPLATES)
fi

dnl ----------------------------------------------------------------------
dnl Check whether c++ compiler supports typename
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler supports typename,
ac_cv_typename,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
template<class T> class X { public: typedef typename T::Y Y; };
class Z { public: typedef int Y; };
X<Z> x;
,
ac_cv_typename=yes, ac_cv_typename=no)
AC_LANG_RESTORE)

if test $ac_cv_typename = no
then
    AC_DEFINE(HAVE_NO_TYPENAME)
fi

dnl ----------------------------------------------------------------------
dnl Check whether c++ compiler knows mutable
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler knows mutable,
ac_cv_mutable,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
class X { mutable int i; }; X x;
,
ac_cv_mutable=yes, ac_cv_mutable=no)
AC_LANG_RESTORE)

if test $ac_cv_mutable = no
then
    AC_DEFINE(HAVE_NO_MUTABLE)
fi

dnl ----------------------------------------------------------------------
dnl Check whether c++ compiler knows new casts
dnl ----------------------------------------------------------------------

AC_CACHE_CHECK(whether c++ compiler knows new-style casts,
ac_cv_new_casts,
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE_GLOBAL(
,
void f() { double * d = 0; char * x = reinterpret_cast<char *>(d); }
,
ac_cv_new_casts=yes, ac_cv_new_casts=no)
AC_LANG_RESTORE)

if test $ac_cv_new_casts = yes
then
    AC_DEFINE(HAVE_NEW_CASTS)
fi
