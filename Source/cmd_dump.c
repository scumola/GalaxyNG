#include "galaxyng.h"

/****f* CLI/CMD_dump
 *  NAME
 *    CMD_dump -- dump game data.
 *  SYNOPSIS
 *     ./galaxyng -players     <game> [turn]
 *     ./galaxyng -lastorders  <game> [turn]
 *     ./galaxyng -map         <game> [turn]
 *
 *     CMD_dump(int argc, char **argv, int kind) 
 *  FUNCTION
 *    Dump game information.
 *  SWITCHES
 *    kind:
 *      CMD_DUMP_LASTORDERS
 *         show turn when players last send in orders.
 *      CMD_DUMP_MAP
 *         show a map of the galaxy
 *      CMD_DUMP_PLAYERS
 *         show password and address of players
 *      CMD_DUMP_HALL
 *         show information for use in the hall of fame.
 *      CMD_DUMP_TEAM_INFO
 *         show info on all members of a team
 *         (used in Tiger games).
 * SOURCE
 */

int CMD_dump( int argc, char **argv, int kind ) {
    game *aGame;
    int result;
    int turn;
    int team;

    result = EXIT_FAILURE;
    if ( argc >= 3 ) {
        if ( argc == 3 ) {
            turn = LG_CURRENT_TURN;
        }
		else {
            turn = atoi( argv[3] );
        }
		
        if ( ( aGame = loadgame( argv[2], turn ) ) ) {
            player *aDummyPlayer;
            struct fielddef fields;

            fields.destination = stdout;

            loadNGConfig( aGame );
            switch ( kind ) {
				case CMD_DUMP_MAP: {

					aDummyPlayer = allocStruct( player );
					
					setName( aDummyPlayer, "DummyDummy" );
					aDummyPlayer->msize = aGame->galaxysize;
					reportMap( aGame, aDummyPlayer, &fields );
				}
					break;
					
				case CMD_DUMP_MAP_GNUPLOT:{
                    aDummyPlayer = allocStruct( player );
					
                    setName( aDummyPlayer, "DummyDummy" );
                    aDummyPlayer->msize = aGame->galaxysize;
                    reportMap_gnuplot( aGame, aDummyPlayer, &fields );
                }
                    break;
					
				case CMD_DUMP_LASTORDERS:{
                    reportLastOrders( aGame->players, &fields );
                }
                    break;

				case CMD_DUMP_PLAYERS:{
                    reportPlayers( aGame->players, &fields );
                }
                    break;

				case CMD_DUMP_PSCORE:{
                    scorePercent( aGame, &fields );
                }
                    break;

				case CMD_DUMP_HALL:{
                    reportHall( aGame, &fields );
                }
                    break;
					
				case CMD_DUMP_MAILHEADER:{
                    createMailToAllHeader( aGame );
                }
                    break;

				case CMD_DUMP_TEAM_INFO:{
                    if ( argc == 5 ) {
                        team = atoi( argv[4] );
                        reportTeam( aGame, &fields, team );
                    }
					else {
                        fprintf( stderr,
                                 "You have to specify a team number.\n" );
                    }
                }
                    break;

				case CMD_DUMP_TEAM_REPORT_NAMES:{
                    if ( argc == 5 ) {
                        player *aPlayer;

                        team = atoi( argv[4] );
                        for ( aPlayer = aGame->players;
                              aPlayer; aPlayer = aPlayer->next ) {
                            if ( aPlayer->team == team ) {
                                printf( "reports/%s/%s_%d.txt\n", aGame->name,
                                        aPlayer->name, aGame->turn );
                            }
                        }
                    }
					else {
                        fprintf( stderr,
                                 "You have to specify a team number.\n" );
                    }
                }
                    break;
            }
			
            result = EXIT_SUCCESS;
            freegame( aGame );
        }
		else {
            fprintf( stderr, "Could not load game \"%s\".\n", argv[2] );
        }
    }
	else {
        usage(  );
    }

    return result;
}

