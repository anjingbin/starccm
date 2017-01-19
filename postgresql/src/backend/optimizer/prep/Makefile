#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for optimizer/prep
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/prep/Makefile,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/optimizer/prep
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = prepqual.o preptlist.o prepunion.o prepkeyset.o

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

