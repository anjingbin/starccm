# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/pg_controldata/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $

subdir = contrib/pg_controldata
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

PROGRAM = pg_controldata
OBJS	= pg_controldata.o pg_crc.o $(SNPRINTF)

pg_crc.c: $(top_srcdir)/src/backend/utils/hash/pg_crc.c
	rm -f $@ && $(LN_S) $< .

# this only gets done if configure finds system doesn't have snprintf()
snprintf.c: $(top_srcdir)/src/backend/port/snprintf.c
	rm -f $@ && $(LN_S) $< .

EXTRA_CLEAN = pg_crc.c snprintf.c

DOCS = README.pg_controldata

include $(top_srcdir)/contrib/contrib-global.mk
