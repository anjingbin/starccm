#-------------------------------------------------------------------------
#
# Makefile for src/bin/ipcclean
#
# Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
# Portions Copyright (c) 1994, Regents of the University of California
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/ipcclean/Makefile,v 1.2 2004/07/05 08:55:50 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/bin/ipcclean
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

all: ipcclean

ipcclean: ipcclean.sh
	cp $< $@
	chmod a+x $@

install: all installdirs
	$(INSTALL_SCRIPT) ipcclean $(DESTDIR)$(bindir)/ipcclean

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

uninstall:
	rm -f $(DESTDIR)$(bindir)/ipcclean

clean distclean maintainer-clean:
	rm -f ipcclean
