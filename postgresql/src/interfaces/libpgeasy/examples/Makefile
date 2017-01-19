#-------------------------------------------------------------------------
#
# Makefile
#    Makefile for pgeasy examples
#
# IDENTIFICATION
#    $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/interfaces/libpgeasy/examples/Makefile,v 1.2 2004/07/05 08:56:13 SuLiang Exp $
#
#-------------------------------------------------------------------------

CFLAGS=-I/usr/local/pgsql/include
TARGET = pginsert pgwordcount pgnulltest
LDFLAGS = -L/usr/local/pgsql/lib -lpgeasy

all : $(TARGET)

pginsert:
	gcc -o $@ $(CFLAGS) $@.c $(PGEASY) $(LDFLAGS)

pgwordcount:
	gcc -o $@ $(CFLAGS) $@.c $(PGEASY) $(LDFLAGS)

pgnulltest:
	gcc -o $@ $(CFLAGS) $@.c $(PGEASY) $(LDFLAGS)

clean:
	rm -f *.o $(TARGET) log core

