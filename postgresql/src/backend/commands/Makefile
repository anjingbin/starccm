#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for commands
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/commands/Makefile,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/commands
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS = async.o creatinh.o command.o comment.o copy.o indexcmds.o define.o \
       remove.o rename.o vacuum.o vacuumlazy.o analyze.o view.o cluster.o \
       explain.o sequence.o trigger.o user.o proclang.o \
       dbcommands.o variable.o

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
