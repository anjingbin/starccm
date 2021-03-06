#-------------------------------------------------------------------------
#
# Makefile for parser
#
# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/parser/Makefile,v 1.1 2003/12/30 00:06:07 AnJingBin Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/parser
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS= analyze.o gram.o keywords.o parser.o parse_agg.o parse_clause.o \
      parse_expr.o parse_func.o parse_node.o parse_oper.o parse_relation.o \
      parse_type.o parse_coerce.o parse_target.o scan.o scansup.o


all: SUBSYS.o

SUBSYS.o: $(OBJS)
	$(LD) $(LDREL) $(LDOUT) $@ $^


# There is no correct way to write a rule that generates two files.
# Rules with two targets don't have that meaning, they are merely
# shorthand for two otherwise separate rules.  To be safe for parallel
# make, we must chain the dependencies like this.  The semicolon is
# important, otherwise make will choose the built-in rule for
# gram.y=>gram.c.

$(srcdir)/gram.c: $(srcdir)/parse.h ;

$(srcdir)/parse.h: gram.y
ifdef YACC
	$(YACC) -d $(YFLAGS) $<
	mv -f y.tab.c $(srcdir)/gram.c
	mv -f y.tab.h $(srcdir)/parse.h
else
	@$(missing) bison $< $@
endif


$(srcdir)/scan.c: scan.l
ifdef FLEX
	$(FLEX) $(FLEXFLAGS) -Pbase_yy -o'$@' $<
else
	@$(missing) flex $< $@
endif


# Force these dependencies to be known even without dependency info built:

analyze.o keywords.o parse_clause.o parse_expr.o parser.o scan.o: $(srcdir)/parse.h


# gram.c, parse.h, and scan.c are in the distribution tarball, so they
# are not cleaned here.
clean: 
	rm -f SUBSYS.o $(OBJS)
# And the garbage that might have been left behind by partial build:
	@rm -f y.tab.c y.tab.h lex.yy.c


# This is unusual:  We actually have to build some of the parts before
# we know what the header file dependencies are.  
dep depend: gram.c scan.c
	$(CC) -MM $(CFLAGS) *.c >depend

ifeq (depend,$(wildcard depend))
include depend
endif
