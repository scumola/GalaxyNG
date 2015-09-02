#****h* GalaxyNG/Unittest
# FUNCTION
#   Unittest for GalaxyNG
#*****

RBT_DIR     = /home/galaxyng/robotester-0.0.3/Source
NG_DIR      = ..
ROBORUN     = $(RBT_DIR)/roborun.pl

CC     = gcc
LIBS   = -lm
CFLAGS = -g -Wall -pedantic -I$(RBT_DIR) -I$(NG_DIR) -DROBOTESTER -DFS_NEW_FORECAST

#****v* Unittest/RBT_SOURCES
# FUNCTION
#   These are the source files for the robotester C framework.
#   We always need these.
# SOURCE
#

RBT_SOURCES = $(RBT_DIR)/rbt_assert.c \
              $(RBT_DIR)/robotester.c \
              $(RBT_DIR)/testcase.c \
              $(RBT_DIR)/testsuite.c

RBT_OBJECTS = ${RBT_SOURCES:.c=.o}

#*****

#****v* Unittest/TEST_SOURCES
# FUNCTION
#   These are the source files of the unittest.
# SOURCE
#

TEST_SOURCES = create_suites.c test_ng.c
TEST_OBJECTS = ${TEST_SOURCES:.c=.o}

#******

#****v* Unittest/REAL_SOURCES
# FUNCTION
#   These are the common sources between wopr and galaxyng
# SOURCE
#

REAL_SOURCES = $(NG_DIR)/loadgame.c $(NG_DIR)/savegame.c \
               $(NG_DIR)/create.c $(NG_DIR)/phase.c \
               $(NG_DIR)/battle.c $(NG_DIR)/process.c \
               $(NG_DIR)/report.c $(NG_DIR)/util.c \
               $(NG_DIR)/list.c $(NG_DIR)/selftest.c \
               $(NG_DIR)/report_xml.c $(NG_DIR)/mail.c \
               $(NG_DIR)/report_m.c $(NG_DIR)/globals.c \
               $(NG_DIR)/avl.c $(NG_DIR)/usage.c \
               $(NG_DIR)/galaxyng.c
REAL_OBJECTS = ${REAL_SOURCES:.c=.o}

#******

#****v* Unittest/OBJECTS
# FUNCTION
#   All the objects needed for the tester application.
# SOURCE
#

OBJECTS = $(RBT_OBJECTS) $(TEST_OBJECTS) $(REAL_OBJECTS)

#******

#****e* Unittest/ngtester
# FUNCTION
#   This builds the test application that can run the individual
#   testcases in the unittest for GalaxyNG.
# SOURCE

ngtester : $(OBJECTS)
	$(CC) $(OBJECTS) -o ngtester $(LIBS)

#****

setup:
	rm -rf TempData
	mkdir  TempData
	mkdir  TempData/log

#****e* Example/test
# FUNCTION
#    This runs the testcases and collects all the results.
# SOURCE

test: ngtester
	perl $(ROBORUN)

#****

clean:
	-rm -f $(OBJECTS)
	-rm -f ngtester


depend:
	makedepend -Y"" $(REAL_SOURCES) $(NG_SOURCES) $(WOPR_SOURCES)

# DO NOT DELETE

../loadgame.o: ../loadgame.h ../galaxy.h ../list.h ../util.h
../savegame.o: ../savegame.h ../galaxy.h ../list.h ../util.h
../create.o: ../create.h ../galaxy.h ../avl.h ../list.h ../util.h ../report.h
../create.o: ../loadgame.h ../mail.h ../report_m.h ../report_xml.h
../create.o: ../savegame.h ../process.h ../phase.h ../battle.h ../selftest.h
../phase.o: ../galaxy.h ../list.h ../util.h ../phase.h ../loadgame.h
../phase.o: ../savegame.h
../battle.o: ../battle.h ../galaxy.h ../list.h ../util.h
../process.o: ../process.h ../galaxy.h ../list.h ../util.h ../savegame.h
../process.o: ../loadgame.h ../phase.h ../battle.h ../report.h ../mail.h
../process.o: ../report_m.h ../report_xml.h ../selftest.h
../report.o: ../report.h ../galaxy.h ../loadgame.h ../list.h ../util.h
../report.o: ../mail.h ../report_m.h ../report_xml.h ../create.h ../avl.h
../util.o: ../util.h ../galaxy.h ../list.h ../create.h ../avl.h ../loadgame.h
../list.o: ../list.h ../util.h ../galaxy.h
../selftest.o: ../selftest.h ../galaxy.h ../util.h ../list.h ../create.h
../selftest.o: ../avl.h ../process.h ../savegame.h ../loadgame.h ../phase.h
../selftest.o: ../battle.h ../report.h ../mail.h ../report_m.h
../selftest.o: ../report_xml.h
../report_xml.o: ../create.h ../galaxy.h ../avl.h ../report_xml.h ../report.h
../report_xml.o: ../loadgame.h ../list.h ../util.h ../mail.h ../report_m.h
../mail.o: ../mail.h ../galaxy.h ../util.h ../list.h
../report_m.o: ../create.h ../galaxy.h ../avl.h ../report_m.h ../list.h
../report_m.o: ../util.h ../report.h ../loadgame.h ../mail.h ../report_xml.h
../globals.o: ../galaxy.h
../avl.o: ../avl.h
../usage.o: ../galaxy.h
../galaxyng.o: ../galaxy.h ../util.h ../list.h ../process.h ../savegame.h
../galaxyng.o: ../loadgame.h ../phase.h ../battle.h ../report.h ../mail.h
../galaxyng.o: ../report_m.h ../report_xml.h ../selftest.h ../create.h
../galaxyng.o: ../avl.h
