#
# $Id: makefile,v 1.6 2004/03/15 03:10:32 madcoder42 Exp $
#

all : 
	make -C lib
	make -C Source galaxyng
	make -C Util
	make -C ARE

install: Source/galaxyng
	./install.sh

clean:
	make -C lib clean
	make -C Source clean
	make -C Util clean
	make -C ARE clean
	rm -f *~ *.bak

depend:
	make -C lib depend
	make -C Source depend
	make -C Util depend
	make -C ARE depend
