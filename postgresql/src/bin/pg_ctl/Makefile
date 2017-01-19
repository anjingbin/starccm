#-------------------------------------------------------------------------
#
# Makefile for src/bin/pg_ctl
#
# Copyright (c) 1999, PostgreSQL Global Development Group
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/pg_ctl/Makefile,v 1.2 2004/07/05 08:55:50 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/bin/pg_ctl
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

all: pg_ctl

pg_ctl: pg_ctl.sh
	sed -e 's/@VERSION@/$(VERSION)/g' \
	    -e 's,@bindir@,$(bindir),g' \
	  $< >$@
	chmod a+x $@

install: all installdirs
	$(INSTALL_SCRIPT) pg_ctl $(DESTDIR)$(bindir)/pg_ctl

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

uninstall:
	rm -f $(DESTDIR)$(bindir)/pg_ctl

clean distclean maintainer-clean:
	rm -f pg_ctl
