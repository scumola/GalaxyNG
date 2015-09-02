/****h* Galaxy/GalaxyNG
 *  NAME  
 *    galaxyng -- Server for the play-by-email game GalaxyNG
 *  SYNOPSIS
 *    galaxyng [command [options]]
 *  FUNCTION
 *    Checks incoming orders, runs the turn, and sends out the turn
 *    reports. 
 *
 *    The code is divided into a number of modules:
 *     List     -- (list.c list.h) functions for manipulating lists.
 *     Util     -- (util.c util.h) frequently used functions.
 *     Phase    -- (phase.c phase.h) code for the various phases in the game.
 *     Process  -- (process.c process.h ) code for order checking, 
 *                 order processing, and running a turn.
 *     Report   -- (report.c report.h) 
 *                 Code to generate the turn reports.
 *     Battle   -- (battle.c battle.h) code that performs the battles.
 *     Loadgame -- saves a turn to disk
 *     Savegame -- load a turn from disk
 *     GalaxyNG -- glues it all together.
 *  AUTHOR
 *    Created by:
 *    o Frans Slothouber
 *    o Christophe  Barbier
 *    o Jacco van Weert
 *    o Tommy Lindqvist
 *    o Rogerio Fung
 *    o Ken Weinert
 *
 *    This code contains parts of the the orginal Galaxy code which was
 *    created by Russell Wallace and updated by the
 *    Galaxy PBeM Development Group which include 
 *    o Russell Wallace
 *    o Tim Myers
 *    o Robert Stone
 *    o Mayan Moudgill
 *    o Graeme Griffiths
 *    o K Pankhurst
 *  CREATION DATE
 *    4-Jan-1997
 *  COPYRIGHT
 *    GPL  see ../COPYING
 *  NOTES
 *    This is not the most pretty code around. It is a product of many
 *    years and many people. The code hosts a lot of global variables
 *    and many not very descriptively named function. The code is
 *    pretty stable however.
 *  BUGS
 *    See the sourceforge page at
 *       http://sourceforge.net/projects/galaxyng/
 **********
 */

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef WIN32
    /* Empty */
#else
#include <unistd.h>
#endif
#include "galaxy.h"
#include "util.h"
#include "process.h"
#include "create.h"
#include "loadgame.h"
#include "savegame.h"
#include "report.h"
#include "selftest.h"
#include "mail.h"
#include "galaxyng.h"


char *galaxyng = "$Id: galaxyng.c,v 1.65 2005/01/22 15:34:24 madcoder42 Exp $";

char vcid[128];


/****h* GalaxyNG/CLI
 * FUNCTION
 *   Function for the command line interface of GalaxyNG.
 *****
 * This stuff will move to a seperate file later on.
 */

/****f* CLI/main
 * NAME
 *   main -- the start of it all.
 * RESULT
 *   Reports any errors that occur back to the environment.
 * SOURCE
 */

#ifndef ROBOTESTER
int
main( int argc, char **argv )
{
    char *value;
    int result;

    /* Some initializations */
    resetErnie( 197162622 );

    setLogLevel(LBRIEF);

    sprintf( vcid, "GalaxyNG release-%d-%d, %s.",
             GNG_MAJOR, GNG_MINOR, GNG_DATE );

    SetDirectoryVariables();

    if ( ( value = getenv( "GNG_LOG_LEVEL" ) ) ) {
        if ( strcasecmp( value, "full" ) == 0 )
            setLogLevel(LFULL);
        else if ( strcasecmp( value, "part" ) == 0 )
            setLogLevel(LPART);
        else if ( strcasecmp( value, "brief" ) == 0 )
            setLogLevel(LBRIEF);
        else if ( strcasecmp( value, "none" ) == 0 )
            setLogLevel(LNONE);
        else
            setLogLevel(LBRIEF);
    }

    if ( argc <= 1 ) {
        usage(  );
	} else if ( strstr( argv[1], "clean") ) {
		result = CMD_clean(argc, argv);
    } else if ( strstr( argv[1], "immediate") ) {
      result = CMD_immediate(argc, argv);
    } else if ( strstr( argv[1], "create" ) ) {
        result = CMD_create( argc, argv );
    } else if ( strstr( argv[1], "dummymail0" ) ) {
        result = CMD_mail0( argc, argv, CMD_CHECK_DUMMY );
    } else if ( strstr( argv[1], "mail0" ) ) {
        result = CMD_mail0( argc, argv, CMD_CHECK_REAL );
    }
#if 0
    else if ( strstr( argv[1], "filecheck" ) ) {
        result = CMD_checkFile( argc, argv, CMD_CHECK_DUMMY );
    }
#endif
    else if ( strstr( argv[1], "dummycheck" ) ) {
        result = CMD_check( argc, argv, CMD_CHECK_DUMMY );
    } else if ( strstr( argv[1], "webcheck" ) ) {
        result = CMD_webcheck( argc, argv, CMD_CHECK_REAL );
    } else if ( strstr( argv[1], "check" ) ) {
        result = CMD_check( argc, argv, CMD_CHECK_REAL );
    } else if ( strstr( argv[1], "dummyrun" ) ) {
        result = CMD_run( argc, argv, CMD_RUN_DUMMY );
    } else if ( strstr( argv[1], "run" ) ) {
        result = CMD_run( argc, argv, CMD_RUN_REAL );
    } else if ( strstr( argv[1], "selftest" ) ) {       /* experimental */
        result = CMD_selftest(  );
    } else if ( strstr( argv[1], "battletest" ) ) {     /* experimental */
        result = CMD_battletest( argc, argv );
    } else if ( strstr( argv[1], "test" ) ) {   /* experimental */
        result = CMD_test( argc, argv );
    } else if ( strstr( argv[1], "webreport" ) ) {
      result = CMD_webreport( argc, argv);
    } else if ( strstr( argv[1], "report" ) ) {
        result = CMD_report( argc, argv );
    } else if ( strstr( argv[1], "relay" ) ) {
        result = CMD_relay( argc, argv );
    } else if ( strstr( argv[1], "pscore" ) ) { /* experimental */
        result = CMD_dump( argc, argv, CMD_DUMP_PSCORE );
    } else if ( strstr( argv[1], "score" ) ) {
        result = CMD_score( argc, argv );
    } else if ( strstr( argv[1], "graph" ) ) {  /* experimental */
        result = CMD_graph( argc, argv );
    } else if ( strstr( argv[1], "template" ) ) {
        result = CMD_template( argc, argv );
    }
#if defined(DRAW_INFLUENCE_MAP)
    else if ( strstr( argv[1], "influence" ) ) {
        result = CMD_influence( argc, argv );
    }
#endif
	else if ( strstr( argv[1], "due" ) ) {
		result = CMD_ordersdue( argc, argv);
	}
    else if ( strstr( argv[1], "map" ) ) {
        result = CMD_dump( argc, argv, CMD_DUMP_MAP );
    } else if ( strstr( argv[1], "hall" ) ) {
        result = CMD_dump( argc, argv, CMD_DUMP_HALL );
    } else if ( strstr( argv[1], "lastorders" ) ) {
        result = CMD_dump( argc, argv, CMD_DUMP_LASTORDERS );
    } else if ( strstr( argv[1], "players" ) ) {
        result = CMD_dump( argc, argv, CMD_DUMP_PLAYERS );
    } else if ( strstr( argv[1], "toall" ) ) {
        result = CMD_dump( argc, argv, CMD_DUMP_MAILHEADER );
    } else if ( strstr( argv[1], "teaminfo" ) ) {
        result = CMD_dump( argc, argv, CMD_DUMP_TEAM_INFO );
    } else if ( strstr( argv[1], "teamdump" ) ) {
        result = CMD_dump( argc, argv, CMD_DUMP_TEAM_REPORT_NAMES );
    } else {
        usage(  );
    }
    return result;
}
#endif

/******/

/****f* CLI/SetDirectoryVariables
 * FUNCTION
 *   Set the values of the tempdir and galaxynghome.
 * SOURCE
 */
void
SetDirectoryVariables( void )
{
    char *value;
    if ( ( value = getenv( "GALAXYNGHOME" ) ) ) {
        galaxynghome = strdup( value );
    } else if ( ( value = getenv( "HOME" ) ) ) {
        sprintf( lineBuffer, "%s/Games", value );
        galaxynghome = strdup( lineBuffer );
    } else {
        galaxynghome =
            strdup( "/please/set/your/HOME/or/GALAXYNGHOME/variable" );
    }

    if ((value = getenv("GALAXYNGTMP"))) {
      tempdir = strdup(value);
    }
    else {
      tempdir = strdup( "/tmp" );
    }
}
