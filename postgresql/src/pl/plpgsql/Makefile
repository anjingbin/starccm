#-------------------------------------------------------------------------
#
# Makefile for src/pl/plpgsql (PostgreSQL's SQL procedural language)
#
# Copyright (c) 1994, Regents of the University of California
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/pl/plpgsql/Makefile,v 1.2 2004/07/05 08:56:18 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/pl/plpgsql
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

all install installdirs uninstall distprep:
	$(MAKE) -C src $@

clean distclean maintainer-clean:
	-$(MAKE) -C src $@
