# $Header: /home/hjcvs/OB-CCM-1.0/postgresql/contrib/Makefile,v 1.2 2004/07/05 08:55:02 SuLiang Exp $

subdir = contrib
top_builddir = ..
include $(top_builddir)/src/Makefile.global

WANTED_DIRS = \
		array		\
		btree_gist	\
		chkpass		\
		cube		\
		dbase		\
		dblink		\
		earthdistance	\
		findoidjoins	\
		fulltextindex	\
		fuzzystrmatch	\
		intarray	\
		isbn_issn	\
		lo		\
		miscutil	\
		noupdate	\
		oid2name	\
		pg_controldata	\
		pg_dumplo	\
		pg_logger	\
		pg_resetxlog	\
		pgbench		\
		pgcrypto	\
		pgstattuple	\
		rserv		\
		rtree_gist	\
		seg		\
		spi		\
		string		\
		tips		\
		tsearch		\
		userlock	\
		vacuumlo

ifeq ($(with_java),yes)
WANTED_DIRS += retep
endif

# Missing:
#		ipc_check	\ (does not have a makefile)
#		mSQL-interface	\ (requires msql installed)
#		mac		\ (does not have a makefile)
#		mysql		\ (does not have a makefile)
#		oracle		\ (does not have a makefile)
#		start-scripts	\ (does not have a makefile)
#		tools		\ (does not have a makefile)
#		xml		\ (non-standard makefile)


all install installdirs uninstall clean distclean maintainer-clean check installcheck:
	@for dir in $(WANTED_DIRS); do \
		$(MAKE) -C $$dir $@ || exit; \
	done
