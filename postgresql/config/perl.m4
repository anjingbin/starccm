# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/config/perl.m4,v 1.2 2004/07/05 08:55:02 SuLiang Exp $


# PGAC_PATH_PERL
# --------------
AC_DEFUN([PGAC_PATH_PERL],
[AC_PATH_PROG(PERL, perl)])


# PGAC_CHECK_PERL_DIRS
# ---------------------
AC_DEFUN([PGAC_CHECK_PERL_DIRS],
[
AC_REQUIRE([PGAC_PATH_PERL])
AC_MSG_CHECKING([Perl installation directories])

# These are the ones we currently need.  Others can be added easily.
perl_installsitearch=`$PERL -MConfig -e 'print $Config{installsitearch}'`
perl_installsitelib=`$PERL -MConfig -e 'print $Config{installsitelib}'`
perl_installman3dir=`$PERL -MConfig -e 'print $Config{installman3dir}'`

AC_SUBST(perl_installsitearch)[]dnl
AC_SUBST(perl_installsitelib)[]dnl
AC_SUBST(perl_installman3dir)[]dnl

AC_MSG_RESULT(done)
])


