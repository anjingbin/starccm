#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for tioga
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/tioga/Makefile,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/tioga
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS = tgRecipe.o Varray.o
override CPPFLAGS := -I$(srcdir) $(CPPFLAGS)

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
