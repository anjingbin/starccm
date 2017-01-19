# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/vacuumlo/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $

subdir = contrib/vacuumlo
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

PROGRAM = vacuumlo
OBJS	= vacuumlo.o

PG_CPPFLAGS = -I$(libpq_srcdir)
PG_LIBS = $(libpq)

DOCS = README.vacuumlo

include $(top_srcdir)/contrib/contrib-global.mk
