#ifndef GNG_GALAXYNG_H
#define GNG_GALAXYNG_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "galaxy.h"
#include "util.h"
#include "loadgame.h"
#include "process.h"
#include "create.h"

#define CMD_RUN_REAL    1       /* -run will mail turn reports */
#define CMD_RUN_DUMMY   2       /* -run will save turn reports to disk */
#define CMD_CHECK_REAL  1
#define CMD_CHECK_DUMMY 2


enum
{
    CMD_DUMP_MAP = 1, CMD_DUMP_LASTORDERS, CMD_DUMP_PLAYERS,
    CMD_DUMP_PSCORE, CMD_DUMP_HALL, CMD_DUMP_MAILHEADER,
    CMD_DUMP_TEAM_INFO, CMD_DUMP_TEAM_REPORT_NAMES, CMD_DUMP_MAP_GNUPLOT
};

void SetDirectoryVariables( void );
int CMD_immediate(int argc, char** argv);
int CMD_create( int argc, char **argv );
int CMD_mail0( int argc, char **argv, int kind );
int CMD_run( int argc, char **argv, int kind );
int CMD_webcheck( int argc, char **argv, int kind );
int CMD_check( int argc, char **argv, int kind );
#if 0
int CMD_checkFile( int argc, char **argv, int kind );
#endif
int CMD_report( int argc, char **argv );
int CMD_webreport( int argc, char **argv );
int CMD_score( int argc, char **argv );
int CMD_graph( int argc, char **argv );
int CMD_dump( int argc, char **argv, int kind );
int CMD_test( int argc, char **argv );
int CMD_relay( int argc, char **argv );
int CMD_template( int argc, char **argv );
int CMD_selftest(  );
int CMD_battletest( int argc, char **argv );
#if defined(DRAW_INFLUENCE_MAP)
int CMD_influence( int argc, char **argv );
#endif
int checkTime( game *aGame );
int relayMessage(game *aGame, envelope *anEnvelope, char *nationName,
		 player *from, emailList *to );
int CMD_ordersdue(int argc, char** argv);
int CMD_clean(int argc, char** argv);

#endif                          /* GNG_GALAXYNG_H */
