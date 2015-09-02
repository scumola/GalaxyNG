#include "galaxyng.h"

/****f* CLI/CMD_test
 * NAME
 *   CMD_test -- check the integrity of a game.
 * FUNCTION
 *   Check if a GAME is OK by running checkIntegrity() on it.
 ******
 */

int CMD_test( int argc, char **argv ) {
    game *aGame;
    int result;
    int turn;

    turn = LG_CURRENT_TURN;

    if ( ( aGame = loadgame( argv[2], LG_CURRENT_TURN ) ) ) {
        checkIntegrity( aGame );
        dumpPlanets( aGame->planets );
        dumpPlayers( aGame->players );
        result = EXIT_SUCCESS;
    }
	else {
        result = EXIT_FAILURE;
    }
	
    return result;
}
