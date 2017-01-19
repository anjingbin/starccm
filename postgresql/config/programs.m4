# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/config/programs.m4,v 1.2 2004/07/05 08:55:02 SuLiang Exp $


# PGAC_PATH_FLEX
# --------------
# Look for Flex, set the output variable FLEX to its path if found.
# Avoid the buggy version 2.5.3. Also find Flex if its installed
# under `lex', but do not accept other Lex programs.

AC_DEFUN([PGAC_PATH_FLEX],
[AC_CACHE_CHECK([for flex], pgac_cv_path_flex,
[# Let the user override the test
if test -n "$FLEX"; then
  pgac_cv_path_flex=$FLEX
else
  pgac_save_IFS=$IFS
  IFS=:
  for pgac_dir in $PATH; do
    if test -z "$pgac_dir" || test x"$pgac_dir" = x"."; then
      pgac_dir=`pwd`
    fi
    for pgac_prog in flex lex; do
      pgac_candidate="$pgac_dir/$pgac_prog"
      if test -f "$pgac_candidate" \
        && $pgac_candidate --version </dev/null >/dev/null 2>&1
      then
        echo '%%'  > conftest.l
        if $pgac_candidate -t conftest.l 2>/dev/null | grep FLEX_SCANNER >/dev/null 2>&1; then
          if $pgac_candidate --version | grep '2\.5\.3' >/dev/null 2>&1; then
            pgac_broken_flex=$pgac_candidate
            continue
          fi

          pgac_cv_path_flex=$pgac_candidate
          break 2
        fi
      fi
    done
  done
  IFS=$pgac_save_IFS
  rm -f conftest.l
  : ${pgac_cv_path_flex=no}
fi
])[]dnl AC_CACHE_CHECK

if test x"$pgac_cv_path_flex" = x"no"; then
  if test -n "$pgac_broken_flex"; then
    AC_MSG_WARN([
*** The Flex version 2.5.3 you have at $pgac_broken_flex contains a bug. You
*** should get version 2.5.4 or later.])
  fi

  AC_MSG_WARN([
*** Without Flex you will not be able to build PostgreSQL from CVS or
*** change any of the scanner definition files.  You can obtain Flex from
*** a GNU mirror site.  (If you are using the official distribution of
*** PostgreSQL then you do not need to worry about this because the Flex
*** output is pre-generated.)])
fi

if test x"$pgac_cv_path_flex" = x"no"; then
  FLEX=
else
  FLEX=$pgac_cv_path_flex
fi

AC_SUBST(FLEX)
AC_SUBST(FLEXFLAGS)
])# PGAC_PATH_FLEX



# PGAC_CHECK_READLINE
# -------------------
# Check for the readline library and dependent libraries, either
# termcap or curses.  Also try libedit, since NetBSD's is compatible.
# Add the required flags to LIBS, define HAVE_LIBREADLINE.

AC_DEFUN([PGAC_CHECK_READLINE],
[AC_REQUIRE([AC_CANONICAL_HOST])
AC_MSG_CHECKING([for readline])

AC_CACHE_VAL([pgac_cv_check_readline],
[pgac_cv_check_readline=no
for pgac_lib in "" " -ltermcap" " -lncurses" " -lcurses" ; do
  for pgac_rllib in -lreadline -ledit ; do
    pgac_save_LIBS=$LIBS
    LIBS="${pgac_rllib}${pgac_lib} $LIBS"
    AC_TRY_LINK_FUNC([readline], [[
      # NetBSD and OpenBSD have a broken linker that does not
      # recognize dependent libraries
      case $host_os in netbsd* | openbsd* )
        case $pgac_lib in
          *curses*) ;;
          *) pgac_lib=" -lcurses" ;;
        esac
      esac

      pgac_cv_check_readline="${pgac_rllib}${pgac_lib}"
      break 2
    ]])
    LIBS=$pgac_save_LIBS
  done
done
LIBS=$pgac_save_LIBS
])[]dnl AC_CACHE_VAL

if test "$pgac_cv_check_readline" != no ; then
  AC_DEFINE(HAVE_LIBREADLINE)
  LIBS="$pgac_cv_check_readline $LIBS"
  AC_MSG_RESULT([yes ($pgac_cv_check_readline)])
else
  AC_MSG_RESULT(no)
fi])# PGAC_CHECK_READLINE



# PGAC_CHECK_GETTEXT
# ------------------

AC_DEFUN([PGAC_CHECK_GETTEXT],
[
  AC_SEARCH_LIBS(gettext, intl, [],
                 [AC_MSG_ERROR([a gettext implementation is required for NLS])])
  AC_CHECK_HEADER([libintl.h], [],
                  [AC_MSG_ERROR([header file <libintl.h> is required for NLS])])
  AC_CHECK_PROGS(MSGFMT, msgfmt)
  if test -z "$MSGFMT"; then
    AC_MSG_ERROR([msgfmt is required for NLS])
  fi
  AC_CHECK_PROGS(MSGMERGE, msgmerge)
dnl FIXME: We should probably check for version >=0.10.36.
  AC_CHECK_PROGS(XGETTEXT, xgettext)

  # Note: share/locale is always the default, independent of $datadir
  localedir='${prefix}/share/locale'
  if test x"$prefix" = x"NONE"; then
    exp_localedir="$ac_default_prefix/share/locale"
  else
    exp_localedir="$prefix/share/locale"
  fi

  AC_SUBST(localedir)
  AC_DEFINE_UNQUOTED(LOCALEDIR, ["$exp_localedir"],
                     [location of locale files])
])# PGAC_CHECK_GETTEXT
