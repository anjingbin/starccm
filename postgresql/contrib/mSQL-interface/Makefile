#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/mSQL-interface/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $
#

subdir = contrib/mSQL-interface
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

NAME	:= mpgsql
SO_MAJOR_VERSION := 0
SO_MINOR_VERSION := 0
OBJS	:= mpgsql.o

override CPPFLAGS := -I$(libpq_srcdir) $(CPPFLAGS)

include $(top_srcdir)/src/Makefile.shlib

all: all-lib

install: all installdirs install-lib
	$(INSTALL_DATA) README.$(NAME) $(docdir)/contrib

installdirs:
	$(mkinstalldirs) $(libdir) $(docdir)/contrib

uninstall: uninstall-lib
	rm -f $(docdir)/contrib/README.$(NAME)

clean distclean maintainer-clean: clean-lib
	rm -f $(OBJS)
