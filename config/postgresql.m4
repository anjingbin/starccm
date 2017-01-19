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

AC_SUBST(PSQL_CPPFLAGS)
AC_SUBST(PSQL_LDFLAGS)
AC_SUBST(PSQL_LIBS)
AC_SUBST(WITH_PSQL)

PSQL_CPPFLAGS=
PSQL_LDFLAGS=
PSQL_LIBS=
WITH_PSQL=no

AC_ARG_WITH(postgresql,
  --with-postgresql=DIR     make use of PostgreSQL)


if test -z "$with_postgresql" -o "$with_postgresql" != "no"
then

postgresql_path=

if test -z "$postgresql_path" -a -n "$with_postgresql" -a "$with_postgresql" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-postgresql option
    dnl
    postgresql_path="${with_postgresql}/"
    if test ! -f ${postgresql_path}include/postgres.h
    then
        postgresql_path="${postgresql_path}postgresql/"
    fi
fi

if test -z "$postgresql_path" -a -n "$with_ob" -a "$with_ob" != "yes"
then
    dnl
    dnl Search in the directory specified by the --with-ob option
    dnl
    postgresql_path="${with_ob}/"
    if test ! -f ${postgresql_path}src/include/postgres.h
    then
    	postgresql_path="${with_ob}/postgresql/"
	if test ! -f ${postgresql_path}src/include/postgres.h
    	then
	    postgresql_path=
    	fi
    fi
fi

if test -z "$postgresql_path" -a -n "$prefix"
then
    dnl
    dnl Search in the installation directory
    dnl
    postgresql_path="${prefix}/"
    if test ! -f ${postgresql_path}src/include/postgres.h
    then
	postgresql_path="${postgresql_path}postgresql/"
	if test ! -f ${postgresql_path}src/include/postgres.h
	then
	    postgresql_path=
	fi
    fi	
fi

if test -z "$postgresql_path"
then
    dnl
    dnl Search in the current directory and parent directory
    dnl
    postgresql_path="postgresql/"
    if test ! -f ${postgresql_path}src/include/postgres.h
    then
	postgresql_path="../postgresql/"
	if test ! -f ${postgresql_path}src/include/postgres.h
	then
	    postgresql_path=
	fi
    fi	
fi

if test ! -f ${postgresql_path}src/include/postgres.h
then
  AC_MSG_ERROR(Can't find PostgreSQL! Use the --with-postgresql=DIR option.)
fi

if test "$postgresql_path" = "../postgresql/"
then
    postgresql_path="\$(top_srcdir)/$postgresql_path"
fi

PSQL_CPPFLAGS="$PSQL_CPPFLAGS -I${postgresql_path}src/include -I${postgresql_path}src/interfaces/libpq"
PSQL_LDFLAGS="$PSQL_LDFLAGS -L${postgresql_path}lib"
PSQL_LIBS="${postgresql_path}src/interfaces/libpq/libpq.a"
AC_DEFINE(HAVE_PSQL)
WITH_PSQL=yes

fi
