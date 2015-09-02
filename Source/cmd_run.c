#include "galaxyng.h"

/****f* CLI/CMD_run
 * NAME
 *   CMD_run -- run turn and send turn reports
 * SYNOPSIS
 *   ./galaxyng -run <game name> <file with all orders>
 * FUNCTION
 *   Run an turn, Compute the highscore list, and send the turn reports 
 *   to all players. The GM is send a status report. If the option
 *   SaveReportCopy is specified in the .galaxyngrc file, a copy of each
 *   turn report is also saved in reports/
 *
 *   This function is also run for: 
 *     ./galaxyng -dummyrun <game name> <file with all orders>
 *   In this case the run is a dummy run, and the turn reports are stored 
 *   in reports/. Nothing is mailed. This is used to debug the server code.
 * 
 * OUTPUT
 *   Turn reports are send out to the players and the turn is saved
 *   to disk. The GM is sent a status report. A log can be found in
 *   log/<game name>.
 * DIAGNOSTICS
 *   Message to stderr in case of an error.
 *   (Game does not exist, not the right time to run the game,
 *    game structure is corrupted).
 * RESULT
 *   EXIT_FAILURE  or  EXIT_SUCCESS
 * SOURCE
 */

int CMD_run( int argc, char **argv, int kind ) {
    int result;

    result = EXIT_FAILURE;
    if ( argc >= 4 ) {
        game* aGame;
        int   turn;
        char* logName;
		
        logName = createString( "%s/log/%s", galaxynghome, argv[2] );
        openLog( logName, "w" );
        free( logName );

        plogtime( LPART );
        plog( LPART, "Trying to run Game \"%s\".\n", argv[2] );

        aGame = NULL;
        turn = ( argc == 4 ) ? LG_CURRENT_TURN : atoi( argv[4] ) - 1;
        if ( ( aGame = loadgame( argv[2], turn ) ) ) {
            player* aPlayer;

            loadNGConfig( aGame );

            if ( checkTime( aGame ) || ( kind == CMD_RUN_DUMMY ) ) {
                checkIntegrity( aGame );

                if ( runTurn( aGame, argv[3] ) ) {
                    highScoreList( aGame );
                    result = 0;
					
                    for ( aPlayer = aGame->players; aPlayer;
                          aPlayer = aPlayer->next ) {
                        if ( aPlayer->flags & F_TXTREPORT ) {
                            if ( kind == CMD_RUN_REAL ) {
                                result |=
                                    mailTurnReport( aGame, aPlayer,
                                                    F_TXTREPORT );
                                if ( aGame->gameOptions.
                                     gameOptions & GAME_SAVECOPY ) {
                                    saveTurnReport( aGame, aPlayer,
                                                    F_TXTREPORT );
                                }
                            } else {
                                saveTurnReport( aGame, aPlayer, F_TXTREPORT );
                            }
                        }

                        if ( aPlayer->flags & F_XMLREPORT ) {
                            if ( kind == CMD_RUN_REAL ) {
                                result |=
                                    mailTurnReport( aGame, aPlayer,
                                                    F_XMLREPORT );
                                if ( aGame->gameOptions.
                                     gameOptions & GAME_SAVECOPY ) {
                                    saveTurnReport( aGame, aPlayer,
                                                    F_XMLREPORT );
                                }
                            } else {
								saveTurnReport( aGame, aPlayer,
												F_XMLREPORT );
							}
                        }

                        if ( aPlayer->flags & F_MACHINEREPORT ) {
                            if ( kind == CMD_RUN_REAL ) {
                                result |=
                                    mailTurnReport( aGame, aPlayer,
                                                    F_MACHINEREPORT );
                                if ( aGame->gameOptions.
                                     gameOptions & GAME_SAVECOPY ) {
                                    saveTurnReport( aGame, aPlayer,
                                                    F_MACHINEREPORT );
                                }
							} else {
								saveTurnReport( aGame, aPlayer,
												F_MACHINEREPORT );
							}
                        }
                    }
                    savegame( aGame );
                } else {
                    fprintf( stderr,
                             "The server has detected an error in the game data structure. The run\n"
                             "of the turn has been aborted. No turn reports have been sent. Please\n"
                             "contact Ken Weinert at mc@quarter-flash.com for a solution to this\n"
                             "problem.\n" );
                    result = 1;
                }
                plogtime( LPART );
                plog( LPART, "Run is completed.\n" );
                result = ( result ) ? EXIT_FAILURE : EXIT_SUCCESS;
            } else {
                plog( LBRIEF, "Error, attempt to run the game \"%s\" at the"
                      " wrong time.\n"
                      "You specified a start time of %s in your .galaxyngrc file.\n",
                      argv[2], aGame->starttime );
                fprintf( stderr,
                         "Error, attempt to run the game \"%s\" at the wrong time.\n"
                         "You specified a start time of %s in your .galaxyngrc file.\n",
                         argv[2], aGame->starttime );
            }
            closeLog(  );
            if ( kind == CMD_RUN_REAL ) {
                mailGMReport( aGame, argv[2] );
            }
            freegame( aGame );
        }

        else {
            plog( LBRIEF, "Game \"%s\" does not exist.\n", argv[2] );
            fprintf( stderr, "Game \"%s\" does not exist.\n", argv[2] );
        }
    } else {
        usage(  );
    }
    closeLog(  );
    return result;
}
