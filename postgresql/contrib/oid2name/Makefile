# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/oid2name/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $

subdir = contrib/oid2name
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

PROGRAM = oid2name
OBJS	= oid2name.o

PG_CPPFLAGS = -I$(libpq_srcdir)
PG_LIBS = $(libpq)

DOCS = README.oid2name

include $(top_srcdir)/contrib/contrib-global.mk
