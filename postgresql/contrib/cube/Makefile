# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/cube/Makefile,v 1.2 2004/07/05 08:55:03 SuLiang Exp $

subdir = contrib/cube
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

MODULE_big = cube
OBJS= cube.o cubeparse.o cubescan.o buffer.o

DATA_built = cube.sql
DOCS = README.cube
REGRESS = cube


cubeparse.c: cubeparse.h ;

cubeparse.h: cubeparse.y
ifdef YACC
	$(YACC) -d $(YFLAGS) -p cube_yy $<
	mv -f y.tab.c cubeparse.c
	mv -f y.tab.h cubeparse.h
else
	@$(missing) bison $< $@
endif

cubescan.c: cubescan.l
ifdef FLEX
	$(FLEX) $(FLEXFLAGS) -Pcube_yy -o'$@' $<
else
	@$(missing) flex $< $@
endif

EXTRA_CLEAN = cubeparse.c cubeparse.h cubescan.c y.tab.c y.tab.h


include $(top_srcdir)/contrib/contrib-global.mk
