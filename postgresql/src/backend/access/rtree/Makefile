#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for access/rtree
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/access/rtree/Makefile,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/access/rtree
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = rtget.o rtproc.o rtree.o rtscan.o rtstrat.o

all: SUBSYS.o

SUBSYS.o: $(OBJS)
	$(LD) $(LDREL) $(LDOUT) SUBSYS.o $(OBJS)

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

clean: 
	rm -f SUBSYS.o $(OBJS)

ifeq (depend,$(wildcard depend))
include depend
endif

