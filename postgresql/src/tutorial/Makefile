#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for tutorial
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/tutorial/Makefile,v 1.2 2004/07/05 08:56:18 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/tutorial
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

override CFLAGS+= $(CFLAGS_SL)
override DLLLIBS := $(BE_DLLLIBS) $(DLLLIBS)

#
# DLOBJS are the dynamically-loaded object files.  The "funcs" queries
# include CREATE FUNCTIONs that load routines from these files.
#
DLOBJS= complex$(DLSUFFIX) funcs$(DLSUFFIX)

QUERIES= advanced.sql basics.sql complex.sql funcs.sql syscat.sql

all: $(DLOBJS) $(QUERIES)

%.sql: %.source
	rm -f $@; \
	C=`pwd`; \
	sed -e "s:_OBJWD_:$$C:g" < $< > $@

clean:
	rm -f $(DLOBJS) $(QUERIES)
