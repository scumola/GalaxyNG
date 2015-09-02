#include "galaxyng.h"

void cleanDeadPlayers( game *aGame );

int CMD_clean(int argc, char **argv) {
    int result;

	game *aGame;
	int turn;
	char *logName;

    result = EXIT_FAILURE;
	logName = createString( "%s/log/%s", galaxynghome, argv[2] );
	openLog( logName, "w" );
	free( logName );

	plogtime( LPART );
	plog( LPART, "Trying to clean Game \"%s\".\n", argv[2] );

	aGame = NULL;
	turn = LG_CURRENT_TURN;
	
	if ( ( aGame = loadgame( argv[2], turn ) ) ) {
		loadNGConfig( aGame );
		cleanDeadPlayers(aGame);
		aGame->turn--;  /* hack to force it to save to the current game file */
		savegame( aGame );
	}

	return EXIT_SUCCESS;
}
