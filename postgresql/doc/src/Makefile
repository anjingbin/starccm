# Postgres documentation makefile
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/doc/src/Makefile,v 1.2 2004/07/05 08:55:11 SuLiang Exp $

subdir = doc/src
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

ZIPSUFFIX= gz

BOOKS = admin developer reference programmer tutorial user  postgres

TARGETS= $(BOOKS:%=%.tar.$(ZIPSUFFIX))

.PRECIOUS:
.PHONY: install all sources clean distclean

install:
	$(MAKE) all
	(mv -f *.$(ZIPSUFFIX) ..)

clean:
	$(MAKE) -C sgml clean

distclean:
	$(MAKE) -C sgml distclean

all: $(TARGETS)

sources:
	$(MAKE) sources.tar.$(ZIPSUFFIX)

sources.tar:
	$(TAR) -cf $@ sgml graphics

# Build twice to resolve index entries
$(addsuffix .tar, $(BOOKS)):
	$(MAKE) -C sgml clean
	$(MAKE) -C sgml $(basename $@).html
	$(MAKE) -C sgml $(basename $@).html JADEFLAGS='-V html-manifest'
	cd sgml && $(TAR) -cf ../$@ `cat HTML.manifest` `echo *.gif | grep -v '\*'` *.css

man.tar:
	$(MAKE) -C sgml man
	$(TAR) -cf $@ -C sgml man1 man$(sqlmansect_dummy)
