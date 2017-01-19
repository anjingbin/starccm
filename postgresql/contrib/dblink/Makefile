# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/dblink/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $

subdir = contrib/dblink
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

MODULE_big = dblink
PG_CPPFLAGS = -I$(libpq_srcdir)
OBJS	= dblink.o
SHLIB_LINK = $(libpq)

DATA_built = dblink.sql 
DOCS = README.dblink

include $(top_srcdir)/contrib/contrib-global.mk
