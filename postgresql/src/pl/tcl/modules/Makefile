# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/pl/tcl/modules/Makefile,v 1.2 2004/07/05 08:56:18 SuLiang Exp $

subdir = src/pl/tcl/modules
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

MODULES = pltcl_loadmod pltcl_delmod pltcl_listmod

all: $(MODULES)

$(MODULES): %: %.in $(top_builddir)/src/Makefile.global
	sed 's,@TCLSH@,$(TCLSH),g' $< >$@
	chmod a+x $@

install: all installdirs
	$(INSTALL_SCRIPT) pltcl_loadmod $(DESTDIR)$(bindir)/pltcl_loadmod
	$(INSTALL_SCRIPT) pltcl_delmod 	$(DESTDIR)$(bindir)/pltcl_delmod
	$(INSTALL_SCRIPT) pltcl_listmod $(DESTDIR)$(bindir)/pltcl_listmod
	$(INSTALL_DATA) $(srcdir)/unknown.pltcl $(DESTDIR)$(datadir)/unknown.pltcl

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir) $(DESTDIR)$(datadir)

uninstall:
	rm -f $(DESTDIR)$(bindir)/pltcl_loadmod $(DESTDIR)$(bindir)/pltcl_delmod $(DESTDIR)$(bindir)/pltcl_listmod $(DESTDIR)$(datadir)/unknown.pltcl

clean distclean maintainer-clean:
	rm -f $(MODULES)
