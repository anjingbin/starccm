#-------------------------------------------------------------------------
#
# Makefile for src/interfaces
#
# Copyright (c) 1994, Regents of the University of California
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/interfaces/Makefile,v 1.2 2004/07/05 08:56:08 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/interfaces
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

DIRS := libpq ecpg libpgeasy

ALLDIRS := $(DIRS) odbc libpq++ libpgtcl perl5 python jdbc

ifeq ($(enable_odbc), yes)
DIRS += odbc
endif

ifeq ($(with_CXX), yes)
DIRS += libpq++
endif

ifeq ($(with_tcl), yes)
DIRS += libpgtcl
endif

ifeq ($(with_perl), yes)
DIRS += perl5
endif

ifeq ($(with_python), yes)
DIRS += python
endif

ifeq ($(with_java), yes)
DIRS += jdbc
endif


all install installdirs uninstall dep depend distprep:
	@for dir in $(DIRS); do $(MAKE) -C $$dir $@ || exit; done

clean:
	@for dir in $(DIRS); do $(MAKE) -C $$dir $@; done

distclean maintainer-clean:
	@for dir in $(ALLDIRS); do $(MAKE) -C $$dir $@; done
