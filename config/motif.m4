dnl **********************************************************************
dnl
dnl Copyright (c) 2003
dnl StarMiddleware.net	 
dnl www.StarMiddleware.net
dnl                       
dnl All Rights Reserved   
dnl                          
dnl **********************************************************************

AC_PATH_XTRA


AC_SUBST(WITH_MOTIF)

WITH_MOTIF=no

if test -z "$no_x"
then
    AC_CACHE_CHECK(for Motif include directory, ac_cv_motif_includes,
    for dir in "$x_includes" "${prefix}/include" \
            /usr/include /usr/local/include \
            /usr/include/Motif2.0 /usr/include/Motif1.2 /usr/include/Motif1.1 \
            /usr/include/X11R6 /usr/include/X11R5 /usr/include/X11R4 \
	    /usr/X11R6/include \
            /usr/dt/include /usr/openwin/include \
            /usr/dt/*/include /opt/*/include /usr/include/Motif* \
            "${prefix}"/*/include /usr/*/include /usr/local/*/include \
            "${prefix}"/include/* /usr/include/* /usr/local/include/*
    do
        if test -f "$dir/Xm/Xm.h"
        then
            ac_cv_motif_includes=$dir
            break
        fi
    done)

    if test -n "$ac_cv_motif_includes"
    then
        AC_CACHE_CHECK(for Motif library directory, ac_cv_motif_libraries,
        for dir in "$x_libraries" "${prefix}/lib" /usr/lib /usr/local/lib \
                /usr/lib/Motif2.0 /usr/lib/Motif1.2 /usr/lib/Motif1.1 \
                /usr/lib/X11R6 /usr/lib/X11R5 /usr/lib/X11R4 \
		/usr/X11R6/lib \
                /usr/dt/lib /usr/openwin/lib \
                /usr/dt/*/lib /opt/*/lib /usr/lib/Motif* \
                "${prefix}"/*/lib /usr/*/lib /usr/local/*/lib \
                "${prefix}"/lib/* /usr/lib/* /usr/local/lib/*
        do
            if test -d "$dir" && test "`ls $dir/libXm.* 2> /dev/null`" != ""
            then
                ac_cv_motif_libraries=$dir
                break
            fi
        done)

	if test -n "$ac_cv_motif_libraries"
        then
	    WITH_MOTIF="yes"

	    if test "$ac_cv_motif_includes" != "/usr/include"
	    then
	        XM_CFLAGS="-I$ac_cv_motif_includes"
	    fi

	    if test "$ac_cv_motif_libraries" != "/usr/lib"
	    then
		XM_LIBS="-L$ac_cv_motif_libraries"
	    fi
        fi
    fi
fi

AC_SUBST(OB_XM_CPPFLAGS)
AC_SUBST(OB_XM_LIBS)

OB_XM_CPPFLAGS=
OB_XM_LIBS=

if test "$WITH_MOTIF" = yes
then
  OB_XM_CPPFLAGS="$XM_CFLAGS"
  OB_XM_LIBS="$XM_LIBS -lXm"
fi
