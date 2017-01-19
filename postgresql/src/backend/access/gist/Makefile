#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for access/gist
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/access/gist/Makefile,v 1.1 2003/12/30 00:05:09 AnJingBin Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/access/gist
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = gist.o gistget.o gistscan.o giststrat.o

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
