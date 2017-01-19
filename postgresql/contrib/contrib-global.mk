# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/contrib-global.mk,v 1.2 2004/07/05 08:55:02 SuLiang Exp $

# This file contains generic rules to build many kinds of simple
# contrib modules.  You only need to set a few variables and include
# this file, the rest will be done here.
#
# Use the following layout for your Makefile:
#
#   subdir = contrib/xxx
#   top_builddir = ../..
#   include $(top_builddir)/src/Makefile.global
#
#   [variable assignments, see below]
#   [custom rules, rarely necessary]
#
#   include $(top_srcdir)/contrib/contrib-global.mk
#
# The following variables can be set:
#
#   MODULES -- list of shared objects to be build from source file with
#     same stem (do not include suffix in this list)
#   DATA -- random files to install into $PREFIX/share/contrib
#   DATA_built -- random files to install into $PREFIX/share/contrib,
#     which need to be built first
#   DOCS -- random files to install under $PREFIX/doc/contrib
#   SCRIPTS -- script files (not binaries) to install into $PREFIX/bin
#   REGRESS -- list of regression test cases (without suffix)
#
# or at most one of these two:
#
#   PROGRAM -- a binary program to build (list objects files in OBJS)
#   MODULE_big -- a shared object to build (list object files in OBJS)
#
# The following can also be set:
#
#   EXTRA_CLEAN -- extra files to remove in 'make clean'
#   PG_CPPFLAGS -- will be added to CPPFLAGS
#   PG_LIBS -- will be added to PROGRAM link line
#   SHLIB_LINK -- will be added to MODULE_big link line
#
# Better look at some of the existing uses for examples...


override CPPFLAGS := -I$(srcdir) $(CPPFLAGS)

ifdef MODULES
override CFLAGS += $(CFLAGS_SL)
override DLLLIBS := $(BE_DLLLIBS) $(DLLLIBS)
endif

ifdef PG_CPPFLAGS
override CPPFLAGS := $(PG_CPPFLAGS) $(CPPFLAGS)
endif

all: $(PROGRAM) $(DATA_built) $(addsuffix $(DLSUFFIX), $(MODULES))

ifdef MODULE_big
# shared library parameters
NAME = $(MODULE_big)
SO_MAJOR_VERSION= 0
SO_MINOR_VERSION= 0
rpath =

override DLLLIBS := $(BE_DLLLIBS) $(DLLLIBS)

include $(top_srcdir)/src/Makefile.shlib

all: all-lib
endif # MODULE_big


install: all installdirs
ifneq (,$(DATA)$(DATA_built))
	@for file in $(addprefix $(srcdir)/, $(DATA)) $(DATA_built); do \
	  echo "$(INSTALL_DATA) $$file $(DESTDIR)$(datadir)/contrib"; \
	  $(INSTALL_DATA) $$file $(DESTDIR)$(datadir)/contrib; \
	done
endif # DATA
ifdef MODULES
	@for file in $(addsuffix $(DLSUFFIX), $(MODULES)); do \
	  echo "$(INSTALL_SHLIB) $$file $(DESTDIR)$(pkglibdir)"; \
	  $(INSTALL_SHLIB) $$file $(DESTDIR)$(pkglibdir); \
	done
endif # MODULES
ifdef DOCS
	@for file in $(addprefix $(srcdir)/, $(DOCS)); do \
	  echo "$(INSTALL_DATA) $$file $(DESTDIR)$(docdir)/contrib"; \
	  $(INSTALL_DATA) $$file $(DESTDIR)$(docdir)/contrib; \
	done
endif # DOCS
ifdef PROGRAM
	$(INSTALL_PROGRAM) $(PROGRAM)$(X) $(DESTDIR)$(bindir)
endif # PROGRAM
ifdef MODULE_big
	$(INSTALL_SHLIB) $(shlib) $(DESTDIR)$(pkglibdir)/$(MODULE_big)$(DLSUFFIX)
endif # MODULE_big
ifdef SCRIPTS
	@for file in $(addprefix $(srcdir)/, $(SCRIPTS)); do \
	  echo "$(INSTALL_SCRIPT) $$file $(DESTDIR)$(bindir)"; \
	  $(INSTALL_SCRIPT) $$file $(DESTDIR)$(bindir); \
	done
endif # SCRIPTS


installdirs:
ifneq (,$(DATA)$(DATA_built))
	$(mkinstalldirs) $(DESTDIR)$(datadir)/contrib
endif
ifneq (,$(MODULES)$(MODULE_big))
	$(mkinstalldirs) $(DESTDIR)$(pkglibdir)
endif
ifdef DOCS
	$(mkinstalldirs) $(DESTDIR)$(docdir)/contrib
endif
ifneq (,$(PROGRAM)$(SCRIPTS))
	$(mkinstalldirs) $(DESTDIR)$(bindir)
endif


uninstall:
ifneq (,$(DATA)$(DATA_built))
	rm -f $(addprefix $(DESTDIR)$(datadir)/contrib/, $(DATA) $(DATA_built))
endif
ifdef MODULES
	rm -f $(addprefix $(DESTDIR)$(pkglibdir)/, $(addsuffix $(DLSUFFIX), $(MODULES)))
endif
ifdef DOCS
	rm -f $(addprefix $(DESTDIR)$(docdir)/contrib/, $(DOCS))
endif
ifdef PROGRAM
	rm -f $(DESTDIR)$(bindir)/$(PROGRAM)$(X)
endif
ifdef MODULE_big
	rm -f $(DESTDIR)$(pkglibdir)/$(MODULE_big)$(DLSUFFIX)
endif
ifdef SCRIPTS
	rm -f $(addprefix $(DESTDIR)$(bindir)/, $(SCRIPTS))
endif


clean:
ifdef MODULES
	rm -f $(addsuffix $(DLSUFFIX), $(MODULES)) $(addsuffix .o, $(MODULES))
endif
ifdef DATA_built
	rm -f $(DATA_built)
endif
ifdef PROGRAM
	rm -f $(PROGRAM)$(X)
endif
ifdef OBJS
	rm -f $(OBJS)
endif
ifdef EXTRA_CLEAN
	rm -f $(EXTRA_CLEAN)
endif
ifdef REGRESS
# things created by various check targets
	rm -rf results tmp_check log
	rm -f regression.diffs regression.out regress.out run_check.out
ifeq ($(PORTNAME), win)
	rm -f regress.def
endif
endif # REGRESS

ifdef MODULE_big
clean: clean-lib
endif

distclean maintainer-clean: clean


ifdef REGRESS
.PHONY: submake
submake:
	$(MAKE) -C $(top_builddir)/src/test/regress pg_regress

# against installed postmaster
installcheck: submake
	$(top_builddir)/src/test/regress/pg_regress $(REGRESS)

# in-tree test doesn't work yet (no way to install my shared library)
#check: all submake
#	$(top_builddir)/src/test/regress/pg_regress --temp-install \
#	  --top-builddir=$(top_builddir) $(REGRESS)
check:
	@echo "'make check' is not supported."
	@echo "Do 'make install', then 'make installcheck' instead."
endif # REGRESS


# STANDARD RULES

ifneq (,$(MODULES)$(MODULE_big))
%.sql: %.sql.in
	sed 's,MODULE_PATHNAME,$$libdir/$*,g' $< >$@
endif

ifdef PROGRAM
$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(PG_LIBS) $(LDFLAGS) $(LIBS) -o $@
endif
