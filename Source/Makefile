#
# 	$Id: Makefile,v 1.4 2005/01/22 15:34:24 madcoder42 Exp $	
#
# This are the setting for a Sun Sparc Station and Linux Box
#
# !! Warning !!
# DO NOT USE -O2 OR ANY OTHER FORM OF OPTIMIZATION
# IT WILL BREAK THE CODE
#
#
# NOTE
#
#   There will be many errors if you run 'make depend' -- this is
#   expected. I've added the option to not include the standard
#   directories. In practice the standard headers don't change, but
#   they are easily located in different places on different
#   systems. As it is written, the makedepend command will generate
#   all the local dependencies needed to ensure that all objects will
#   be updated if a header changes, and that's all we really need.
#
#   I would suggest that the 'make depend' command only be run by a
#   developer, as we can recognize the standard headers in the error
#   messages and ignore them, while still recognizing other errors
#
# This makefile is much bigger than necessary. With a few implicit
# rules it could be a fraction of the size. However not all versions
# of make support this. This makefile will hopefully work with even
# all versions of make.
#

#
# Influence mapping:
# if you want to have a .png map generated when the -influence 
# command is used, then you'll need to have libgd 
# (http://www.boutell.com/gd/) installed with png, jpeg, and 
# freetype support enabled and uncomment the following lines:
ADDTL_CFLAGS = -DDRAW_INFLUENCE_MAP
ADDTL_LIBS = -lgd -lpng -lz -ljpeg -lfreetype
ADDTL_SRCS = influence.c
ADDTL_OBJS = influence.o
#
# NOTE: that influence mapping is now turned on by default and you'll
#       have to comment out the above lines to get rid of it
#

CFLAGS = -Wall -pedantic -I../include -I$(HOME)/include
CFLAGS += $(ADDTL_CFLAGS)
#CFLAGS += -fdelete-null-pointer-checks -std=gnu99 -D__USE_GNU 
#CFLAGS += -pipe -Wcast-qual -Wconversion -Wfloat-equal -Wformat=2
#CFLAGS += -Wmissing-declarations -Wmissing-format-attribute -Wnested-externs
#CFLAGS += -Wredundant-decls -Wstrict-prototypes -Wunreachable-code

LIBS = -L$(HOME)/lib -L../lib -lgalaxyng -lm $(ADDTL_LIBS)


CC=gcc

GNG_SRCS=galaxyng.c loadgame.c savegame.c create.c phase.c battle.c \
	process.c report.c selftest.c report_xml.c cmd_template.c cmd_run.c \
	report_m.c avl.c usage.c cmd_immediate.c influence.c checkTime.c \
	cmd_check.c cmd_relay.c cmd_relaymessage.c cmd_create.c cmd_mail0.c \
	cmd_clean.c cmd_report.c cmd_score.c cmd_graph.c cmd_dump.c \
	cmd_test.c cmd_selftest.c cmd_battletest.c cmd_ordersdue.c \
	cmd_webcheck.c cmd_webreport.c $(ADDTL_SOURCES)

GNG_OBJS=$(GNG_SRCS:.c=.o)

WOPR_SRCS=wopr.c 
WOPR_OBJS=$(WOPR_SRCS:.c=.o)

GALAXYNG=galaxyng
WOPR=wopr

all: $(GALAXYNG)

loadgame.o: loadgame.c
	$(CC) $(CFLAGS) -c loadgame.c -o loadgame.o

cmd_template.o: cmd_template.c
	$(CC) $(CFLAGS) -c cmd_template.c -o cmd_template.o

cmd_relay.o: cmd_relay.c
	$(CC) $(CFLAGS) -c cmd_relay.c -o cmd_relay.o

cmd_relaymessage.o: cmd_relaymessage.c
	$(CC) $(CFLAGS) -c cmd_relaymessage.c -o cmd_relaymessage.o

cmd_create.o: cmd_create.c
	$(CC) $(CFLAGS) -c cmd_create.c -o cmd_create.o

cmd_mail0.o: cmd_mail0.c
	$(CC) $(CFLAGS) -c cmd_mail0.c -o cmd_mail0.o

cmd_immediate.o: cmd_immediate.c
	$(CC) $(CFLAGS) -c cmd_immediate.c -o cmd_immediate.o

cmd_run.o: cmd_run.c
	$(CC) $(CFLAGS) -c cmd_run.c -o cmd_run.o

cmd_clean.o: cmd_clean.c
	$(CC) $(CFLAGS) -c cmd_clean.c -o cmd_clean.o

cmd_report.o: cmd_report.c
	$(CC) $(CFLAGS) -c cmd_report.c -o cmd_report.o

cmd_webreport.o: cmd_webreport.c
	$(CC) $(CFLAGS) -c cmd_webreport.c -o cmd_webreport.o

cmd_score.o: cmd_score.c
	$(CC) $(CFLAGS) -c cmd_score.c -o cmd_score.o

cmd_graph.o: cmd_graph.c
	$(CC) $(CFLAGS) -c cmd_graph.c -o cmd_graph.o

cmd_dump.o: cmd_dump.c
	$(CC) $(CFLAGS) -c cmd_dump.c -o cmd_dump.o

cmd_test.o: cmd_test.c
	$(CC) $(CFLAGS) -c cmd_test.c -o cmd_test.o

cmd_selftest.o: cmd_selftest.c
	$(CC) $(CFLAGS) -c cmd_selftest.c -o cmd_selftest.o

cmd_battletest.o: cmd_battletest.c
	$(CC) $(CFLAGS) -c cmd_battletest.c -o cmd_battletest.o

cmd_ordersdue.o: cmd_ordersdue.c
	$(CC) $(CFLAGS) -c cmd_ordersdue.c -o cmd_ordersdue.o

usage.o: usage.c
	$(CC) $(CFLAGS) -c usage.c -o usage.o

savegame.o: savegame.c
	$(CC) $(CFLAGS) -c savegame.c -o savegame.o

mail0.o: mail0.c
	$(CC) $(CFLAGS) -c mail0.c -o mail0.o

phase.o: phase.c
	$(CC) $(CFLAGS) -c phase.c -o phase.o

battle.o: battle.c
	$(CC) $(CFLAGS) -c battle.c -o battle.o

process.o: process.c
	$(CC) $(CFLAGS) -c process.c -o process.o

report.o: report.c
	$(CC) $(CFLAGS) -c report.c -o report.o

list.o: list.c
	$(CC) $(CFLAGS) -c list.c -o list.o

ordersdue.o: ordersdue.c
	$(CC) $(CFLAGS) -c ordersdue.c -o ordersdue.o

report_xml.o: report_xml.c
	$(CC) $(CFLAGS) -c report_xml.c -o report_xml.o

report_m.o: report_m.c
	$(CC) $(CFLAGS) -c report_m.c -o report_m.o

avl.o: avl.c
	$(CC) $(CFLAGS) -c avl.c -o avl.o

influence.o: influence.c
	$(CC) $(CFLAGS) -c influence.c -o influence.o

galaxyng.o: galaxyng.c
	$(CC) $(CFLAGS) -c galaxyng.c -o galaxyng.o

wopr.o: wopr.c
	$(CC) $(CFLAGS) -c wopr.c -o wopr.o


$(GALAXYNG): $(GNG_OBJS)
	$(CC) $(GNG_OBJS) -o $(GALAXYNG) $(LIBS)

$(WOPR): $(WOPR_OBJS)
	$(CC) $(WOPR_OBJS) -o $(WOPR) $(LIBS)

clean:
	rm -f $(GNG_OBJS) $(WOPR_OBJS)
	rm -f $(GALAXYNG)
	rm -f $(GALAXYNG).o
	rm -f $(WOPR)
	rm -f $(WOPR).o
	rm -f core
	rm -f *.*~
	rm -f *~
	rm -f tags
	rm -f $(DOCS)
	rm -f masterindex.html

#=========================================================
#                For development 
#=========================================================

test: galaxyng
	cp galaxyng $(HOME)/Games/galaxyng

count:
	egrep -v -G "^ \*" *.c *.h | egrep -v -G "/\*"  | wc

TAGS: $(SOURCES)
	etags $(SOURCES)

depend:
	-@makedepend -Y -- $(CFLAGS) -- $(GNG_SRCS) $(WOPR_SRCS) 

# DO NOT DELETE THIS LINE -- make depend depends on it.

galaxyng.o: ../include/galaxy.h ../include/strutl.h ../include/cookies.h
galaxyng.o: ../include/logging.h ../include/util.h ../include/list.h
galaxyng.o: ../include/process.h ../include/savegame.h ../include/loadgame.h
galaxyng.o: ../include/phase.h ../include/battle.h ../include/report.h
galaxyng.o: ../include/mail.h ../include/report_m.h ../include/report_xml.h
galaxyng.o: ../include/selftest.h ../include/create.h ../include/avl.h
galaxyng.o: ../include/galaxyng.h
loadgame.o: ../include/loadgame.h ../include/galaxy.h ../include/strutl.h
loadgame.o: ../include/cookies.h ../include/logging.h ../include/list.h
loadgame.o: ../include/util.h
savegame.o: ../include/savegame.h ../include/galaxy.h ../include/strutl.h
savegame.o: ../include/cookies.h ../include/logging.h ../include/list.h
savegame.o: ../include/util.h
create.o: ../include/create.h ../include/galaxy.h ../include/strutl.h
create.o: ../include/cookies.h ../include/logging.h ../include/avl.h
create.o: ../include/list.h ../include/util.h ../include/report.h
create.o: ../include/loadgame.h ../include/mail.h ../include/report_m.h
create.o: ../include/report_xml.h ../include/savegame.h ../include/process.h
create.o: ../include/phase.h ../include/battle.h ../include/selftest.h
phase.o: ../include/galaxy.h ../include/strutl.h ../include/cookies.h
phase.o: ../include/logging.h ../include/list.h ../include/util.h
phase.o: ../include/phase.h ../include/loadgame.h ../include/savegame.h
battle.o: ../include/battle.h ../include/galaxy.h ../include/strutl.h
battle.o: ../include/cookies.h ../include/logging.h ../include/list.h
battle.o: ../include/util.h
process.o: ../include/process.h ../include/galaxy.h ../include/strutl.h
process.o: ../include/cookies.h ../include/logging.h ../include/list.h
process.o: ../include/util.h ../include/savegame.h ../include/loadgame.h
process.o: ../include/phase.h ../include/battle.h ../include/report.h
process.o: ../include/mail.h ../include/report_m.h ../include/report_xml.h
process.o: ../include/selftest.h
report.o: ../include/report.h ../include/galaxy.h ../include/strutl.h
report.o: ../include/cookies.h ../include/logging.h ../include/loadgame.h
report.o: ../include/list.h ../include/util.h ../include/mail.h
report.o: ../include/report_m.h ../include/report_xml.h ../include/create.h
report.o: ../include/avl.h
selftest.o: ../include/selftest.h ../include/galaxy.h ../include/strutl.h
selftest.o: ../include/cookies.h ../include/logging.h ../include/util.h
selftest.o: ../include/list.h ../include/create.h ../include/avl.h
selftest.o: ../include/process.h ../include/savegame.h ../include/loadgame.h
selftest.o: ../include/phase.h ../include/battle.h ../include/report.h
selftest.o: ../include/mail.h ../include/report_m.h ../include/report_xml.h
report_xml.o: ../include/create.h ../include/galaxy.h ../include/strutl.h
report_xml.o: ../include/cookies.h ../include/logging.h ../include/avl.h
report_xml.o: ../include/report_xml.h ../include/report.h
report_xml.o: ../include/loadgame.h ../include/list.h ../include/util.h
report_xml.o: ../include/mail.h ../include/report_m.h
cmd_template.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_template.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_template.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_template.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_template.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_template.o: ../include/report_xml.h ../include/selftest.h
cmd_template.o: ../include/create.h ../include/avl.h
cmd_run.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_run.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_run.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_run.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_run.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_run.o: ../include/report_xml.h ../include/selftest.h ../include/create.h
cmd_run.o: ../include/avl.h
report_m.o: ../include/create.h ../include/galaxy.h ../include/strutl.h
report_m.o: ../include/cookies.h ../include/logging.h ../include/avl.h
report_m.o: ../include/report_m.h ../include/list.h ../include/util.h
report_m.o: ../include/report.h ../include/loadgame.h ../include/mail.h
report_m.o: ../include/report_xml.h
avl.o: ../include/avl.h
usage.o: ../include/galaxy.h ../include/strutl.h ../include/cookies.h
usage.o: ../include/logging.h
cmd_immediate.o: ../include/galaxyng.h ../include/galaxy.h
cmd_immediate.o: ../include/strutl.h ../include/cookies.h
cmd_immediate.o: ../include/logging.h ../include/util.h ../include/list.h
cmd_immediate.o: ../include/loadgame.h ../include/process.h
cmd_immediate.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_immediate.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_immediate.o: ../include/report_xml.h ../include/selftest.h
cmd_immediate.o: ../include/create.h ../include/avl.h
influence.o: ../include/galaxy.h ../include/strutl.h ../include/cookies.h
influence.o: ../include/logging.h ../include/influence.h ../include/list.h
influence.o: ../include/util.h ../include/avl.h ../include/loadgame.h
checkTime.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
checkTime.o: ../include/cookies.h ../include/logging.h ../include/util.h
checkTime.o: ../include/list.h ../include/loadgame.h ../include/process.h
checkTime.o: ../include/savegame.h ../include/phase.h ../include/battle.h
checkTime.o: ../include/report.h ../include/mail.h ../include/report_m.h
checkTime.o: ../include/report_xml.h ../include/selftest.h
checkTime.o: ../include/create.h ../include/avl.h
cmd_check.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_check.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_check.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_check.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_check.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_check.o: ../include/report_xml.h ../include/selftest.h
cmd_check.o: ../include/create.h ../include/avl.h
cmd_relay.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_relay.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_relay.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_relay.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_relay.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_relay.o: ../include/report_xml.h ../include/selftest.h
cmd_relay.o: ../include/create.h ../include/avl.h
cmd_relaymessage.o: ../include/galaxyng.h ../include/galaxy.h
cmd_relaymessage.o: ../include/strutl.h ../include/cookies.h
cmd_relaymessage.o: ../include/logging.h ../include/util.h ../include/list.h
cmd_relaymessage.o: ../include/loadgame.h ../include/process.h
cmd_relaymessage.o: ../include/savegame.h ../include/phase.h
cmd_relaymessage.o: ../include/battle.h ../include/report.h ../include/mail.h
cmd_relaymessage.o: ../include/report_m.h ../include/report_xml.h
cmd_relaymessage.o: ../include/selftest.h ../include/create.h
cmd_relaymessage.o: ../include/avl.h
cmd_create.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_create.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_create.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_create.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_create.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_create.o: ../include/report_xml.h ../include/selftest.h
cmd_create.o: ../include/create.h ../include/avl.h
cmd_mail0.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_mail0.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_mail0.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_mail0.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_mail0.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_mail0.o: ../include/report_xml.h ../include/selftest.h
cmd_mail0.o: ../include/create.h ../include/avl.h
cmd_clean.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_clean.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_clean.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_clean.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_clean.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_clean.o: ../include/report_xml.h ../include/selftest.h
cmd_clean.o: ../include/create.h ../include/avl.h
cmd_report.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_report.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_report.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_report.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_report.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_report.o: ../include/report_xml.h ../include/selftest.h
cmd_report.o: ../include/create.h ../include/avl.h
cmd_score.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_score.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_score.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_score.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_score.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_score.o: ../include/report_xml.h ../include/selftest.h
cmd_score.o: ../include/create.h ../include/avl.h
cmd_graph.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_graph.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_graph.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_graph.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_graph.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_graph.o: ../include/report_xml.h ../include/selftest.h
cmd_graph.o: ../include/create.h ../include/avl.h
cmd_dump.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_dump.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_dump.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_dump.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_dump.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_dump.o: ../include/report_xml.h ../include/selftest.h ../include/create.h
cmd_dump.o: ../include/avl.h
cmd_test.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_test.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_test.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_test.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_test.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_test.o: ../include/report_xml.h ../include/selftest.h ../include/create.h
cmd_test.o: ../include/avl.h
cmd_selftest.o: ../include/galaxyng.h ../include/galaxy.h ../include/strutl.h
cmd_selftest.o: ../include/cookies.h ../include/logging.h ../include/util.h
cmd_selftest.o: ../include/list.h ../include/loadgame.h ../include/process.h
cmd_selftest.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_selftest.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_selftest.o: ../include/report_xml.h ../include/selftest.h
cmd_selftest.o: ../include/create.h ../include/avl.h
cmd_battletest.o: ../include/galaxyng.h ../include/galaxy.h
cmd_battletest.o: ../include/strutl.h ../include/cookies.h
cmd_battletest.o: ../include/logging.h ../include/util.h ../include/list.h
cmd_battletest.o: ../include/loadgame.h ../include/process.h
cmd_battletest.o: ../include/savegame.h ../include/phase.h
cmd_battletest.o: ../include/battle.h ../include/report.h ../include/mail.h
cmd_battletest.o: ../include/report_m.h ../include/report_xml.h
cmd_battletest.o: ../include/selftest.h ../include/create.h ../include/avl.h
cmd_ordersdue.o: ../include/galaxyng.h ../include/galaxy.h
cmd_ordersdue.o: ../include/strutl.h ../include/cookies.h
cmd_ordersdue.o: ../include/logging.h ../include/util.h ../include/list.h
cmd_ordersdue.o: ../include/loadgame.h ../include/process.h
cmd_ordersdue.o: ../include/savegame.h ../include/phase.h ../include/battle.h
cmd_ordersdue.o: ../include/report.h ../include/mail.h ../include/report_m.h
cmd_ordersdue.o: ../include/report_xml.h ../include/selftest.h
cmd_ordersdue.o: ../include/create.h ../include/avl.h
wopr.o: ../include/galaxy.h ../include/strutl.h ../include/cookies.h
wopr.o: ../include/logging.h ../include/util.h ../include/list.h
wopr.o: ../include/process.h ../include/savegame.h ../include/loadgame.h
wopr.o: ../include/phase.h ../include/battle.h ../include/report.h
wopr.o: ../include/mail.h ../include/report_m.h ../include/report_xml.h
wopr.o: ../include/selftest.h ../include/create.h ../include/avl.h
wopr.o: ../include/wopr.h
