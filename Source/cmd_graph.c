#include "galaxyng.h"

/****f* CLI/CMD_graph
 * NAME
 *   CMD_graph -- create a data dump for a graph of a game.
 * NOTES 
 *   Experimental. See Tools/graphscore.tcl
 * SOURCE
 */

int CMD_graph( int argc, char **argv ) {
    game *aGame;
    int result;

    result = EXIT_FAILURE;
    if ( argc == 4 ) {
        if ( ( aGame = loadgame( argv[2], atoi( argv[3] ) ) ) ) {
            player *aPlayer;
            int number;

            raceStatus( aGame );
            for ( number = 0, aPlayer = aGame->players;
                  aPlayer; aPlayer = aPlayer->next, number++ ) {
                printf( "%d %d %s %f\n", aGame->turn, number, aPlayer->name,
                        effectiveIndustry( aPlayer->totPop,
                                           aPlayer->totInd ) );
            }
        }
		else {
            fprintf( stderr, "Could not load game \"%s\" turn %s\n",
                     argv[2], argv[3] );
        }
    }
	else {
        usage(  );
    }
	
    return result;
}
