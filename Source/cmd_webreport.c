#include "galaxyng.h"

/****f* CLI/CMD_webreport
 * NAME 
 *   CMD_webreport -- output a text copy of a turn report.
 * SYNOPSIS
 *   ./galaxyng -webreport < file_with_email
 *   int CMD_report(int argc, char **argv) 
 * FUNCTION
 *   Recreate a turn report of a given turn. Output it for the player
 *   that requested it.
 * SOURCE
 */

int
CMD_webreport( int argc, char **argv ) {
  int result;
  char *logName;
  
  logName = createString( "%s/log/orders_processed.txt", galaxynghome );
  openLog( logName, "a" );
  free( logName );
  
  plogtime( LBRIEF );
  result = EXIT_FAILURE;
  if ( argc >= 2 ) {
    char* raceName;
    char* password;
    char* final_orders;
    int resNumber, theTurnNumber;
    game* aGame;
    
    raceName = NULL;
    password = NULL;
    final_orders = NULL;
    aGame = NULL;
    resNumber =
      areValidOrders( stdin, &aGame, &raceName, &password,
		      &final_orders, &theTurnNumber );
    
    plog(LBRIEF, "Report request from %s for turn %d.\n",
	 raceName, theTurnNumber);

    if ( ( resNumber == RES_TURNRAN ) ||
	 ( ( resNumber == RES_OK ) &&
	   ( theTurnNumber == LG_CURRENT_TURN ) ) ) {
      game *aGame2;
      
      if ( theTurnNumber > 0 ) {
	aGame2 = loadgame( aGame->name, theTurnNumber - 1 );
      }
      else if ( theTurnNumber == LG_CURRENT_TURN ) {
	theTurnNumber = aGame->turn;
	aGame2 = loadgame( aGame->name, theTurnNumber - 1 );
      }
      else {
	aGame2 = loadgame( aGame->name, 0 );
      }
      
      if ( aGame2 ) {
	player *aPlayer;
	int index;
			
	loadNGConfig( aGame2 );

	if ( theTurnNumber > 0 ) {  /* Rerun the turn */
	  char *ordersName;
				
	  ordersName =
	    createString( "%s/orders/%s/%d.all",
			  galaxynghome, aGame2->name,
			  theTurnNumber );
	  runTurn( aGame2, ordersName );
	  free( ordersName );
	}
	/* Translate the current race name into the name used
	   during the turn that is requested
	*/
	aPlayer =
	  findElement( player, aGame->players, raceName );
			
	index = ptonum( aGame->players, aPlayer );
	aPlayer = numtop( aGame2->players, index );
			
	if ( theTurnNumber == 0 )
	  aPlayer->pswdstate = 1;
	highScoreList( aGame2 );
	createTurnReport( aGame2, aPlayer, stdout, 0 );
      }
      else {
	printf("\n\nThe turn you requested is no longer available...\n" );
      }
    }
    else if ( resNumber == RES_OK ) {
      printf("MAJOR TROUBLE\n");
      printf("You can't request a %s report for turn %d.\n\n",
	     aGame->name, theTurnNumber);
      printf("Turn %d hasn't happened yet. While I'm a good "
	     "(some say great) server, I can't predict the future for "
	     "you.\n\n", theTurnNumber);
    }
    else {
      if (resNumber == RES_NO_ORDERS_TURN_NBR)
	resNumber = RES_NO_REPORT_TURN_NBR;
		
      generateErrorMessage( resNumber, aGame, raceName,
			    theTurnNumber, stdout );
    }
    
    result = ( result ) ? EXIT_FAILURE : EXIT_SUCCESS;
    if ( raceName )
      free( raceName );
    if ( password )
      free( password );
  }
  
  closeLog(  );
  
  return result;
}
