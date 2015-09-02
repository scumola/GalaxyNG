#include "galaxyng.h"

/****f* CLI/checkTime
 * NAME
 *   checkTime -- check if it is really time to run a turn.
 * SYNOPSIS
 *   int checkTime(game *aGame)
 * FUNCTION
 *   Does a sanity check to see if a turn really has to be run.  On
 *   some systems, after a reboot, crontab goes bezerk and executes a
 *   whole bunch of entries in your crontab file for no good reason.
 *   This will cause turns to be run prematurely.
 *
 *   This function checks if the current time is equal to the time a
 *   game is supposed to be run.  The GM has to put this time in a
 *   game specific .galaxyngrc file, using the starttime key.
 *
 * EXAMPLE
 *   You want to make sure a game runs at 13:00. Add the entry
 *
 *     starttime 13:00 
 *
 *   to the .galaxyngrc file. The a turn will then only run, if and
 *   only if, it is started by between 13:00 and 13:09.  Add starttime
 *   13:10, and it will only run if started between 13:10 and 13:19.
 * RESULT
 *   TRUE  -- the game is allowed to run.
 *   FALSE -- the game is not allowed to run.
 * SOURCE
 */

int checkTime( game *aGame ) {
    int runGame;

    assert( aGame != NULL );

    runGame = FALSE;
    if ( aGame->starttime ) {
        char timeBuffer[255];
        time_t ttp;

        time( &ttp );
        strftime( timeBuffer, 255, "%H:%M", localtime( &ttp ) );
        if ( strncmp( timeBuffer, aGame->starttime, 4 ) == 0 ) {
            runGame = TRUE;
        }
    }
	else {
        runGame = TRUE;
    }
    return runGame;
}
