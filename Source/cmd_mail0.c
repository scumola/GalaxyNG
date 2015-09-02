#include "galaxyng.h"

/****f* CLI/CMD_mail0
 * NAME
 *   CMD_mail0 -- mail the turn 0 reports.
 * SYNOPSIS
 *   ./galaxyng -mail0 <Game Name>
 *   int CMD_mail0(int argc, char **argv) 
 * FUNCTION
 *   Mail the turn 0 reports to the players. 
 *   To be run after a new game is created.
 * SOURCE
 */

int CMD_mail0( int argc, char **argv, int kind ) {
    game *aGame;
    int result;

    result = EXIT_FAILURE;
    if ( argc == 3 ) {
        if ( ( aGame = loadgame( argv[2], LG_CURRENT_TURN ) ) ) {
            player *aPlayer;

            loadNGConfig( aGame );
            checkIntegrity( aGame );
            for ( aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next ) {
                aPlayer->pswdstate = 1;

                if ( ( aGame->gameOptions.gameOptions & GAME_SAVECOPY ) |
                     ( kind == CMD_CHECK_DUMMY ) ) {
                    if ( aPlayer->flags & F_TXTREPORT )
                        saveTurnReport( aGame, aPlayer, F_TXTREPORT );
                    if ( aPlayer->flags & F_XMLREPORT )
                        saveTurnReport( aGame, aPlayer, F_XMLREPORT );
                }

                if ( aPlayer->flags & F_TXTREPORT ) {
                    if ( kind == CMD_CHECK_DUMMY ) {
                        saveTurnReport( aGame, aPlayer, F_TXTREPORT );
                    } else {
                        mailTurnReport( aGame, aPlayer, F_TXTREPORT );
                    }

                }

                if ( aPlayer->flags & F_XMLREPORT ) {
                    if ( kind == CMD_CHECK_DUMMY ) {
                        saveTurnReport( aGame, aPlayer, F_XMLREPORT );
                    } else {
                        mailTurnReport( aGame, aPlayer, F_XMLREPORT );
                    }
                }

                if ( aPlayer->flags & F_MACHINEREPORT ) {
                    if ( kind == CMD_CHECK_DUMMY ) {
                        saveTurnReport( aGame, aPlayer, F_MACHINEREPORT );
                    } else {
                        mailTurnReport( aGame, aPlayer, F_MACHINEREPORT );
                    }
                }
            }
            result = EXIT_SUCCESS;
        }
		else {
            fprintf( stderr, "Could not load game \"%s\"\n", argv[2] );
        }
    }
	else {
        usage(  );
    }
    return result;
}
