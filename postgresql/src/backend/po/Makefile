# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/po/Makefile,v 1.1 2003/12/30 00:06:08 AnJingBin Exp $

subdir = src/backend/po
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global


gettext-files:
	find $(srcdir)/.. -name '*.c' -print >$@

maintainer-clean:
	rm -f gettext-files
