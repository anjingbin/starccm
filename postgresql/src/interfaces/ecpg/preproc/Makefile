# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/interfaces/ecpg/preproc/Makefile,v 1.2 2004/07/05 08:56:08 SuLiang Exp $

subdir = src/interfaces/ecpg/preproc
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

MAJOR_VERSION=2
MINOR_VERSION=9
PATCHLEVEL=0

override CPPFLAGS := -I$(srcdir)/../include -I$(srcdir) $(CPPFLAGS) \
	-DMAJOR_VERSION=$(MAJOR_VERSION) \
	-DMINOR_VERSION=$(MINOR_VERSION) -DPATCHLEVEL=$(PATCHLEVEL) \
	-DINCLUDE_PATH=\"$(includedir)\" 

OBJS=preproc.o pgc.o type.o ecpg.o ecpg_keywords.o output.o\
    keywords.o c_keywords.o ../lib/typename.o descriptor.o variable.o

all: ecpg

ifdef SNPRINTF
OBJS+=$(top_builddir)/src/backend/port/snprintf.o

$(top_builddir)/src/backend/port/snprintf.o:
	$(MAKE) -C $(top_builddir)/src/backend/port snprintf.o
endif

ifdef STRDUP
OBJS+=$(top_builddir)/src/utils/strdup.o

$(top_builddir)/src/utils/strdup.o:
	$(MAKE) -C $(top_builddir)/src/utils strdup.o
endif

ecpg: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

$(srcdir)/preproc.c: $(srcdir)/preproc.h ;

$(srcdir)/preproc.h: preproc.y
ifdef YACC
	$(YACC) -d $(YFLAGS) $<
	mv -f y.tab.c $(srcdir)/preproc.c
	mv -f y.tab.h $(srcdir)/preproc.h
else
	@$(missing) bison $< $@
endif

$(srcdir)/pgc.c: pgc.l
ifdef FLEX
	$(FLEX) $(FLEXFLAGS) -o'$@' $<
else
	@$(missing) flex $< $@
endif

c_keywords.o ecpg_keywords.o keywords.o pgc.o: preproc.h


distprep: $(srcdir)/preproc.c $(srcdir)/preproc.h $(srcdir)/pgc.c

install: all installdirs
	$(INSTALL_PROGRAM) ecpg$(X) $(DESTDIR)$(bindir)

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

uninstall:
	rm -f $(DESTDIR)$(bindir)/ecpg$(X)

clean distclean:
	rm -f *.o ecpg$(X)
# garbage from partial builds
	@rm -f y.tab.c y.tab.h
# garbage from development
	@rm -f core a.out *~ *.output *.tab.c

# `make clean' does not remove preproc.c, preproc.h, or pgc.c since we
# want to ship those files in the distribution for people with
# inadequate tools.
maintainer-clean: distclean
	rm -f $(srcdir)/preproc.c $(srcdir)/preproc.h $(srcdir)/pgc.c


depend dep: preproc.c pgc.c
	$(CC) -MM $(CFLAGS) *.c >depend

ifeq (depend,$(wildcard depend))
include depend
endif
