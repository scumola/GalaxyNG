#include "galaxyng.h"

/****f* CLI/CMD_score
 * NAME
 *   CMD_score -- Show highscore list.
 * FUNCTION
 *   Write a HTML-ized version of the highscore list to stdout.
 * SOURCE
 */

int CMD_score( int argc, char **argv ) {
    int result;
	int turn_nbr;
	
    game *aGameThisTurn;
    game *aGamePrevTurn;


    result = EXIT_FAILURE;

	switch(argc) {
		case 3:
			turn_nbr = LG_CURRENT_TURN;
			break;

		case 4:
			turn_nbr = atoi(argv[3]);
			break;

		default:
			usage();
			break;
	}
	
	if ( ( aGameThisTurn = loadgame( argv[2], turn_nbr ) ) ) {
		if ( ( aGamePrevTurn =
			   loadgame( aGameThisTurn->name,
						 aGameThisTurn->turn - 1 ) ) ) {
			score( aGamePrevTurn, aGameThisTurn, TRUE, stdout );
			result = EXIT_SUCCESS;
		}
		else {
			fprintf( stderr, "Could not load game \"%s\", turn %d\n",
					 argv[2], turn_nbr );
		}
	}
	else {
		fprintf( stderr, "Could not load game \"%s\" turn %d\n", argv[2],
				 turn_nbr );
	}
	
    return result;
}
