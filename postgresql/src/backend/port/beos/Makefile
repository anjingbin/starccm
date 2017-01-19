# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/port/beos/Makefile,v 1.1 2003/12/30 00:06:08 AnJingBin Exp $

subdir = src/backend/port/beos
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = sem.o shm.o support.o

all: SUBSYS.o 

SUBSYS.o: $(OBJS)
	$(LD) $(LDREL) $(LDOUT) $@ $^

depend dep:
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) *.c >depend

clean: 
	rm -f SUBSYS.o $(OBJS) 

ifeq (depend,$(wildcard depend))
include depend
endif
