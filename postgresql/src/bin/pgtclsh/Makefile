#-------------------------------------------------------------------------
#
# Makefile for src/bin/pgtclsh
# (a tclsh workalike with pgtcl commands installed)
#
# Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
# Portions Copyright (c) 1994, Regents of the University of California
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/pgtclsh/Makefile,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/bin/pgtclsh
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

#
# Include definitions from the tclConfig.sh file
#
-include Makefile.tcldefs
ifeq ($(with_tk), yes)
-include Makefile.tkdefs
endif

libpgtcl_srcdir = $(top_srcdir)/src/interfaces/libpgtcl
libpgtcl_builddir = $(top_builddir)/src/interfaces/libpgtcl
libpgtcl = -L$(libpgtcl_builddir) -lpgtcl

override CPPFLAGS := -I$(libpgtcl_srcdir) $(CPPFLAGS) $(TK_XINCLUDES)


# If we are here then Tcl is available
PROGRAMS = pgtclsh

# Add Tk targets if Tk is available
ifeq ($(with_tk), yes)
PROGRAMS += pgtksh
endif

all: submake $(PROGRAMS)

pgtclsh: pgtclAppInit.o
	$(CC) $(CFLAGS) $^ $(libpgtcl) $(libpq) $(TCL_LIB_SPEC) $(TCL_LIBS) $(LDFLAGS) $(LIBS) -o $@

pgtksh: pgtkAppInit.o
	$(CC) $(CFLAGS) $^ $(libpgtcl) $(libpq) $(TK_LIB_SPEC) $(TK_LIBS) $(TCL_LIB_SPEC) $(LDFLAGS) $(LIBS) -o $@

.PHONY: submake
submake:
	$(MAKE) -C $(libpgtcl_builddir) all

install: all installdirs
	$(INSTALL_PROGRAM) pgtclsh $(DESTDIR)$(bindir)/pgtclsh
ifeq ($(with_tk), yes)
	$(INSTALL_PROGRAM) pgtksh $(DESTDIR)$(bindir)/pgtksh
endif

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

uninstall:
	rm -f $(DESTDIR)$(bindir)/pgtclsh $(DESTDIR)$(bindir)/pgtksh

Makefile.tcldefs: mkMakefile.tcldefs.sh
	$(SHELL) $< '$(TCL_CONFIG_SH)' '$@'

Makefile.tkdefs: mkMakefile.tkdefs.sh
	$(SHELL) $< '$(TK_CONFIG_SH)' '$@'

clean distclean maintainer-clean:
	rm -f pgtclAppInit.o pgtkAppInit.o Makefile.tcldefs Makefile.tkdefs pgtclsh pgtksh

dep depend:
	$(CC) -MM $(CFLAGS) *.c > depend
