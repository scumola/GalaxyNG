#include "galaxyng.h"

/****f* CLI/CMD_create
 * NAME
 *   CMD_create -- create a new galaxy and game.
 * SYNOPSIS
 *   galaxyng -create <game specification file>
 * FUNCTION
 *   Creates a new game based on the specification found in
 *   the specification file.
 * INPUTS
 *   specificationfile -- file with the dimensions of the galaxy
 *     and the addresses of all the players. (.glx file).
 * SEE 
 *   CMD_mail0(), CMD_template()  
 * SOURCE
 */

int
CMD_create( int argc, char **argv ) {
  gamespecification *gspec;
  game *aGame;
  int result;
  FILE *specfile;

  result = EXIT_FAILURE;
  if ( argc == 3 ) {
    if ( ( specfile = fopen( argv[2], "r" ) ) ) {
      gspec = readGameSpec( specfile );
      fclose( specfile );
      printGameSpecs( gspec );
      if ( ( aGame = creategame( gspec ) ) ) {
	struct fielddef fields;
	
	fields.destination = stdout;
	loadNGConfig( aGame );
	checkIntegrity( aGame );
	savegame( aGame );
	reportMap( aGame, aGame->players, &fields );
	printf( "Number of planets: %d\n",
		numberOfElements( aGame->planets ) );
	result = EXIT_SUCCESS;
      }
      else {
	fprintf( stderr, "Can't create the game\n" );
      }
    }
    else {
      fprintf( stderr, "Can't open specification file \"%s\"\n",
	       argv[2] );
    }
  }
  else {
    usage(  );
  }
  return result;
}
