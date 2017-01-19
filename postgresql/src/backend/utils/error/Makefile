#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for utils/error
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/error/Makefile,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/utils/error
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = assert.o elog.o exc.o excabort.o excid.o format.o

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
