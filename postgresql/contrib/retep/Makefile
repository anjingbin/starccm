#-------------------------------------------------------------------------
#
# Makefile for contributed retep tools
#
# Copyright (c) 2001, PostgreSQL Global Development Group
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/retep/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = contrib/retep
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

all:
	$(ANT) -buildfile $(srcdir)/build.xml all

install: installdirs
	$(ANT) -buildfile $(srcdir)/build.xml install \
	  -Dinstall.directory=$(javadir)

installdirs:
	$(mkinstalldirs) $(javadir)

uninstall:
	$(ANT) -buildfile $(srcdir)/build.xml uninstall \
	  -Dinstall.directory=$(javadir)

clean distclean maintainer-clean:
	$(ANT) -buildfile $(srcdir)/build.xml clean
