#
# Makefile for the access methods module
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/access/Makefile,v 1.1 2003/12/30 00:05:09 AnJingBin Exp $
#

subdir = src/backend/access
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

SUBDIRS	    := common gist hash heap index nbtree rtree transam
SUBDIROBJS  := $(SUBDIRS:%=%/SUBSYS.o)

all: SUBSYS.o

SUBSYS.o: $(SUBDIROBJS)
	$(LD) $(LDREL) $(LDOUT) $@ $^

$(SUBDIROBJS): $(SUBDIRS:%=%-recursive)

.PHONY: $(SUBDIRS:%=%-recursive)
$(SUBDIRS:%=%-recursive):
	$(MAKE) -C $(subst -recursive,,$@) SUBSYS.o

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@ || exit; done
	rm -f SUBSYS.o

dep depend:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@ || exit; done
