#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for port/sparc
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/qnx4/Makefile,v 1.2 2004/07/05 09:13:23 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/port/qnx4
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = isnan.o rint.o sem.o shm.o

all: SUBSYS.o tstrint tstsem tstshm

SUBSYS.o: $(OBJS)
	$(LD) $(LDREL) $(LDOUT) SUBSYS.o $(OBJS)

tstrint: tstrint.o rint.o
	$(CC) -o tstrint rint.o tstrint.o

tstsem: tstsem.o sem.o
	$(CC) -o tstsem sem.o tstsem.o

tstshm: tstshm.o shm.o
	$(CC) -o tstshm shm.o tstshm.o

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

clean: 
	rm -f SUBSYS.o $(OBJS) tstrint tstrint.o tstsem tstsem.o tstshm tstshm.o

ifeq (depend,$(wildcard depend))
include depend
endif
