#
# $Id: Makefile,v 1.6 2004/06/25 11:45:26 madcoder42 Exp $
#

CFLAGS = -Wall -pedantic -I../include $(ADDTL_CFLAGS)
CC = gcc

SOURCES=strutl.c logging.c list.c globals.c mail.c util.c loadNGConfig.c \
		lockFile.c
OBJECTS=$(SOURCES:.c=.o)

all : libgalaxyng.a

libgalaxyng.a: $(OBJECTS)
	ar cr libgalaxyng.a $(OBJECTS)

clean:
	rm -f libgalaxyng.a $(OBJECTS) *~

depend:
	makedepend -- $(CFLAGS) -- $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

strutl.o: ../include/strutl.h ../include/cookies.h ../include/logging.h 
logging.o: ../include/logging.h 
list.o: ../include/list.h ../include/util.h ../include/galaxy.h
list.o: ../include/strutl.h ../include/cookies.h ../include/logging.h 
globals.o: ../include/galaxy.h ../include/strutl.h ../include/cookies.h
globals.o: ../include/logging.h
mail.o: ../include/mail.h ../include/galaxy.h ../include/strutl.h
mail.o: ../include/logging.h ../include/util.h ../include/list.h
util.o: ../include/util.h ../include/galaxy.h ../include/strutl.h
util.o: ../include/logging.h ../include/list.h
util.o: ../include/create.h ../include/avl.h ../include/loadgame.h
