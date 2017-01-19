# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/seg/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $

subdir = contrib/seg
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

MODULE_big = seg
OBJS = seg.o segparse.o segscan.o buffer.o
DATA_built = seg.sql
DOCS = README.seg
REGRESS = seg


segparse.c: segparse.h ;

segparse.h: segparse.y
ifdef YACC
	$(YACC) -d $(YFLAGS) -p seg_yy $<
	mv -f y.tab.c segparse.c
	mv -f y.tab.h segparse.h
else
	@$(missing) bison $< $@
endif

segscan.c: segscan.l
ifdef FLEX
	$(FLEX) $(FLEXFLAGS) -Pseg_yy -o'$@' $<
else
	@$(missing) flex $< $@
endif

EXTRA_CLEAN = segparse.c segparse.h segscan.c y.tab.c y.tab.h


include $(top_srcdir)/contrib/contrib-global.mk
