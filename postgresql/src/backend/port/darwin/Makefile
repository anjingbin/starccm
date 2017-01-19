#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for port/darwin
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/darwin/Makefile,v 1.1 2003/12/30 00:06:08 AnJingBin Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/port/darwin
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = sem.o system.o

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
