#-------------------------------------------------------------------------
#
# Makefile for src/bin/pg_dump
#
# Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
# Portions Copyright (c) 1994, Regents of the University of California
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/pg_dump/Makefile,v 1.2 2004/07/05 08:55:50 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/bin/pg_dump
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS= pg_backup_archiver.o pg_backup_db.o pg_backup_custom.o pg_backup_files.o \
       pg_backup_null.o pg_backup_tar.o

ifdef STRDUP
OBJS+=$(top_builddir)/src/utils/strdup.o

$(top_builddir)/src/utils/strdup.o:
	$(MAKE) -C $(top_builddir)/src/utils strdup.o
endif

ifdef STRTOUL
OBJS+=$(top_builddir)/src/backend/port/strtoul.o

$(top_builddir)/src/backend/port/strtoul.o:
	$(MAKE) -C $(top_builddir)/src/backend/port strtoul.o
endif

override CPPFLAGS := -I$(libpq_srcdir) $(CPPFLAGS)

all: submake pg_dump pg_restore pg_dumpall

pg_dump: pg_dump.o common.o $(OBJS) $(libpq_builddir)/libpq.a 
	$(CC) $(CFLAGS) pg_dump.o common.o $(OBJS) $(libpq) $(LDFLAGS) $(LIBS) -o $@

pg_restore: pg_restore.o $(OBJS) $(libpq_builddir)/libpq.a
	$(CC) $(CFLAGS) pg_restore.o $(OBJS) $(libpq) $(LDFLAGS) $(LIBS) -o $@

pg_dumpall: pg_dumpall.sh
	sed -e 's,@VERSION@,$(VERSION),g' \
	    -e 's,@MULTIBYTE@,$(MULTIBYTE),g' \
	    -e 's,@bindir@,$(bindir),g' \
	  $< >$@
	chmod a+x $@

.PHONY: submake
submake:
	$(MAKE) -C $(libpq_builddir) all

install: all installdirs
	$(INSTALL_PROGRAM) pg_dump$(X) $(DESTDIR)$(bindir)/pg_dump$(X)
	$(INSTALL_PROGRAM) pg_restore$(X) $(DESTDIR)$(bindir)/pg_restore$(X)
	$(INSTALL_SCRIPT) pg_dumpall $(DESTDIR)$(bindir)/pg_dumpall

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

uninstall:
	rm -f $(addprefix $(DESTDIR)$(bindir)/, pg_dump$(X) pg_restore$(X) pg_dumpall)

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

clean distclean maintainer-clean:
	rm -f pg_dump$(X) pg_restore$(X) $(OBJS) pg_dump.o common.o pg_restore.o pg_dumpall

ifeq (depend,$(wildcard depend))
include depend
endif
