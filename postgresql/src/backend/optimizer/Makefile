#
# Makefile for optimizer
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/Makefile,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
#

subdir = src/backend/optimizer
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

SUBDIRS     := geqo path plan prep util
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
