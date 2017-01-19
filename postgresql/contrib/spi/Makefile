# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/spi/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $

subdir = contrib/spi
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

MODULES = autoinc insert_username moddatetime refint timetravel
DATA_built = $(addsuffix .sql, $(MODULES))
DOCS	= README.spi $(addsuffix .example, $(MODULES))

ifdef REFINT_VERBOSE
PG_CPPFLAGS = -DREFINT_VERBOSE
endif

include $(top_srcdir)/contrib/contrib-global.mk
