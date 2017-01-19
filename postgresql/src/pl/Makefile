#-------------------------------------------------------------------------
#
# Makefile for src/pl (procedural languages)
#
# Copyright (c) 1994, Regents of the University of California
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/pl/Makefile,v 1.2 2004/07/05 08:56:18 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/pl
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

DIRS := plpgsql

ifeq ($(with_tcl), yes)
DIRS += tcl
endif

ifeq ($(with_perl), yes)
DIRS += plperl
endif

ifeq ($(with_python), yes)
DIRS += plpython
endif

all install installdirs uninstall depend distprep:
	@for dir in $(DIRS); do $(MAKE) -C $$dir $@ || exit; done

clean distclean maintainer-clean:
	@for dir in $(DIRS); do $(MAKE) -C $$dir $@; done
