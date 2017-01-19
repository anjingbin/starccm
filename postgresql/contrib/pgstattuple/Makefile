#-------------------------------------------------------------------------
#
# pgstattuple Makefile
#
# $Id: Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = contrib/pgstattuple
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

MODULE_big	:= pgstattuple
SRCS		+= pgstattuple.c
OBJS		:= $(SRCS:.c=.o)
DOCS		:= README.pgstattuple README.pgstattuple.euc_jp
DATA_built	:= pgstattuple.sql

PG_CPPFLAGS	:=
SHLIB_LINK 	:=

include $(top_srcdir)/contrib/contrib-global.mk
