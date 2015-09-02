#include "galaxyng.h"

/****f* CLI/CMD_report
 * NAME 
 *   CMD_report -- send a copy of a turn report.
 * SYNOPSIS
 *   ./galaxyng -check < file_with_email
 *   int CMD_report(int argc, char **argv) 
 * FUNCTION
 *   Recreate a turn report of a given turn. Send it to the player
 *   that requested it.
 * BUGS
 *   Does not send XML nor machine reports.
 * SOURCE
 */

int
CMD_report( int argc, char **argv ) {
  int result;
  char *logName;
  
  logName = createString( "%s/log/orders_processed.txt", galaxynghome );
  openLog( logName, "a" );
  free( logName );
  
  plogtime( LBRIEF );
  result = EXIT_FAILURE;
  if ( argc >= 2 ) {
    char* returnAddress;
    char* raceName;
    char* password;
    char* final_orders;
    int resNumber, theTurnNumber;
    game* aGame;
    FILE* report;
    char* reportName;
    envelope *anEnvelope;
    
    anEnvelope = createEnvelope(  );
    returnAddress = getReturnAddress( stdin );

    theTurnNumber = getTurnNumber( stdin );
    plog(LBRIEF, "Report request from %s for turn %d.\n",
		 returnAddress, theTurnNumber);
    raceName = NULL;
    password = NULL;
    final_orders = NULL;
    aGame = NULL;
    resNumber =
      areValidOrders( stdin, &aGame, &raceName, &password,
		      &final_orders, &theTurnNumber );
    
    reportName = createString("%s/temp_report_copy_%d_%s",
							  tempdir, theTurnNumber, returnAddress);
    {
      char *ptr = strrchr(reportName, '/')+1;
      while (*ptr) {
		  if (!isalnum(*ptr) || isspace(*ptr))
			  *ptr = '_';
		  ptr++;
      }
    }
	plog(LBRIEF, "Report is in \"%s\"\n", reportName);
    
    setHeader( anEnvelope, MAILHEADER_TO, "%s", returnAddress );
    setHeader(anEnvelope, MAILHEADER_REPLYTO,
	      aGame->serverOptions.ReplyTo);
    anEnvelope->from_name = strdup(aGame->serverOptions.SERVERname);
    anEnvelope->from_address = strdup(aGame->serverOptions.SERVERemail);
    
    report = fopen(reportName, "w");
    
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
			setHeader( anEnvelope, MAILHEADER_SUBJECT,
					   "[GNG] %s turn %d report copy for %s",
					   aGame->name, theTurnNumber, raceName );
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
			
			if (aPlayer->flags & F_COMPRESS)
				anEnvelope->compress = TRUE;
			
			if ( theTurnNumber == 0 )
				aPlayer->pswdstate = 1;
			highScoreList( aGame2 );
			createTurnReport( aGame2, aPlayer, report, 0 );
		}
		else {
			setHeader( anEnvelope, MAILHEADER_SUBJECT,
					   "[GNG] %s turn %d report copy for %s",
					   aGame->name, theTurnNumber, raceName  );
			fprintf( report,
					 "\n\nThe turn you requested is no longer available...\n" );
		}
    }
    else if ( resNumber == RES_OK ) {
		setHeader( anEnvelope, MAILHEADER_SUBJECT,
				   "[GNG] Major Trouble" );
		fprintf(report, "You can't request a %s report for turn %d.\n\n",
				aGame->name, theTurnNumber);
		fprintf(report, "Turn %d hasn't happened yet. While I'm a good "
				"(some say great) server, I can't predict the future for "
				"you.\n\n", theTurnNumber);
    }
    else {
		setHeader( anEnvelope, MAILHEADER_SUBJECT,
				   "[GNG] Major Trouble" );
		if (resNumber == RES_NO_ORDERS_TURN_NBR)
			resNumber = RES_NO_REPORT_TURN_NBR;
		
		generateErrorMessage( resNumber, aGame, raceName,
							  theTurnNumber, report );
		/*copyEmailBody();*/
    }
    
    fclose( report );
    result = eMail( aGame, anEnvelope, reportName );
    destroyEnvelope( anEnvelope );
    result |= ssystem( "rm %s", reportName );
    result = ( result ) ? EXIT_FAILURE : EXIT_SUCCESS;
    if ( raceName )
		free( raceName );
    if ( password )
		free( password );
  }
  
  closeLog(  );
  
  return result;
}
