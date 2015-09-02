#include "galaxyng.h"

/****f* CLI/CMD_check
 * NAME
 *   CMD_check -- check incoming orders.
 * FUNCTION
 *   Check incoming orders and create a forecast of the
 *   situation at the next turn.
 * INPUTS
 *   Orders come in via stdin. The forecast is mailed directy to the player.
 *   Orders are assumed to have a proper mailheader, that is start with:
 *      To: <player>@theaddress
 *      Subject:  orders [turn number]
 *   This header can be produced with formail (see .procmailrc file).
 * RESULTS
 *   Orders are stored in
 *   $GALAXYNGHOME/orders/<game name>/<race name>.<turn number>
 *   Forecast is mailed to the player.
 *   A log is kept of all order processing in log/orders_processed.txt
 * SOURCE
 */


#if FS_NEW_FORECAST

enum EReportFormat { REP_TXT, REP_XML };

FILE* openForecast( char* forecastName ) {
  FILE* forecast;

  if ( ( forecast = GOS_fopen( forecastName, "w" ) ) == NULL ) {
    plog( LBRIEF, "Could not open %s for forecasting\n", forecastName );
    fprintf( stderr, "Could not open %s for forecasting\n", forecastName );
    return 0;
  }
  return forecast;
}

int mailForecast(char* forecastName,
		 char* tag,
		 envelope* anEnvelope,
		 game* aGame,
		 int kind ) {
  int result = FALSE;
  if ( kind == CMD_CHECK_REAL ) {
    plog( LBRIEF, "mailing %s report %s to %s\n",
	  tag, forecastName, anEnvelope->to );
    fprintf( stderr, "mailing %s report %s to %s\n",
	     tag, forecastName, anEnvelope->to );
    result |= eMail( aGame, anEnvelope, forecastName );
  } else {
    /* TODO Create a file copy */
  }
  return result;
}


int mail_AdvanceReport(game* aGame,
		       player *aPlayer,
		       envelope *anEnvelope,
		       char* raceName,
		       int kind,
		       enum EReportFormat report_format ) {
  int result = FALSE;
  /* TODO */
  return result;
}

int mail_Forecast(game* aGame,
		  player *aPlayer,
		  envelope *anEnvelope,
		  char* raceName,
		  int kind,
		  enum EReportFormat report_format ) {
  char* tag = 0;
  int result = FALSE;
  FILE *forecast;
  char* forecastName;
  
  switch ( report_format ) {
  case REP_TXT :
    tag = "text";
    break;
  case REP_XML :
    tag = "XML";
    break;
  default:
    assert( 0 );
  }
  
  forecastName = createString( "%s/NG_%s_%d_forecast",
			       tempdir, tag, getpid(  ) );
  forecast = openForecast( forecastName );
  if ( forecast ) {
    /* OK */
  }
  else {
    return EXIT_FAILURE;
  }
  
  setHeader( anEnvelope, MAILHEADER_SUBJECT,
	     "[GNG] %s turn %d %s forecast for %s",
	     aGame->name, ( aGame->turn ) + 1, tag, raceName );
  
  /* Create the report */
  ( aGame->turn )++;
  fprintf( stderr, "Creating %s report, %s:%d\n", tag, raceName, kind );
  switch ( report_format ) {
  case REP_TXT :
    reportForecast( aGame, raceName, forecast );
    break;
  case REP_XML :
    report_xml( aGame, aPlayer, forecast, Forecast );
    break;
  default:
    assert( 0 );
  }
  ( aGame->turn )--;
  
  /* Mail it */
  result |= mailForecast( forecastName, tag, anEnvelope, aGame, kind );
  result |= GOS_delete( forecastName );
  free( forecastName );
  fclose( forecast );
  return result;
}

int mail_TXT_Error(game* aGame,
		   envelope *anEnvelope,
		   char* raceName,
		   int kind,
		   int resNumber,
		   int theTurnNumber ) {
  int result = FALSE;
  FILE* forecast;
  
  char *forecastName = createString( "%s/NG_TXT_%d_errors",
				     tempdir, getpid(  ) );
  forecast = openForecast( forecastName );
  if ( forecast ) {
    /* OK */
  }
  else {
    return TRUE;
  }
  
  setHeader( anEnvelope, MAILHEADER_SUBJECT, "[GNG] Major Trouble" );
  plog( LBRIEF, "Major Trouble %d\n", resNumber );
  
  generateErrorMessage( resNumber, aGame, raceName, theTurnNumber, forecast );
  fclose( forecast );
  
  result |= mailForecast( forecastName, "TXT", anEnvelope, aGame, kind );
  result |= GOS_delete( forecastName );
  free( forecastName );
  fclose( forecast );
  return result;
}

int CMD_check( int argc, char **argv, int kind ) {
  char *logName;
  int result = FALSE;
  
  logName = createString( "%s/log/orders_processed.txt", galaxynghome );
  openLog( logName, "a" );
  free( logName );
  
  plogtime( LBRIEF );
  if ( argc < 2 ) {
    result = TRUE;
  }
  else {
    envelope *anEnvelope = createEnvelope(  );
    char* returnAddress = getReturnAddress( stdin );
    int   theTurnNumber = getTurnNumber( stdin );
    char* raceName = NULL;
    char* password = NULL;
    char* final_orders = NULL;
    game *aGame = NULL;
    int resNumber = areValidOrders( stdin, &aGame, &raceName,
				    &password, &final_orders, &theTurnNumber );
    plog( LBRIEF, "game %s\n", aGame->name );

    setHeader(anEnvelope, MAILHEADER_FROM, "%s", aGame->serverOptions.SERVERemail);
    
    setHeader( anEnvelope, MAILHEADER_TO, "%s", returnAddress );
    setHeader(anEnvelope, MAILHEADER_REPLYTO, aGame->serverOptions.ReplyTo);
    anEnvelope->from_address = strdup(aGame->serverOptions.SERVERemail);
    anEnvelope->from_name = strdup(aGame->serverOptions.SERVERname);

    if ( resNumber == RES_OK ) {
      player *aPlayer = findElement( player, aGame->players, raceName );
      assert( aPlayer);
      aPlayer->orders = NULL;
      plog( LBRIEF, "Orders from %s\n", returnAddress );
      
      if ( ( theTurnNumber == LG_CURRENT_TURN ) ||
	   ( theTurnNumber == ( aGame->turn ) + 1 ) ) {
	/* They are orders for the coming turn, copy them. */
	copyOrders( aGame, stdin, raceName, password, aGame->turn + 1 );
	/* Check them */
	checkOrders( aGame, raceName );
	/* Now mail the result */
	if ( aPlayer->flags & F_XMLREPORT ) {
	  result = mail_Forecast( aGame, aPlayer, anEnvelope, raceName,
				  kind, REP_XML );
	}
	if ( aPlayer->flags & F_TXTREPORT ) {
	  result = mail_Forecast( aGame, aPlayer, anEnvelope, raceName,
				  kind, REP_TXT );
	}
      } else if ( theTurnNumber > ( aGame->turn ) + 1 ) {
	/* They are advance orders */
	copyOrders( aGame, stdin, raceName, password, theTurnNumber );
	setHeader( anEnvelope, MAILHEADER_SUBJECT,
		   "[GNG] %s turn %d advance orders received for %s.",
		   aGame->name, theTurnNumber, raceName );
	plog( LBRIEF, "%s turn %d advance orders received for %s.\n",
	      aGame->name, theTurnNumber, raceName );
	if ( aPlayer->flags & F_XMLREPORT ) {
	  result = mail_AdvanceReport( aGame, aPlayer, anEnvelope, raceName,
				       kind, REP_XML );
	}
	if ( aPlayer->flags & F_TXTREPORT ) {
	  result = mail_AdvanceReport( aGame, aPlayer, anEnvelope, raceName,
				       kind, REP_TXT );
	}
      }
      else {
	/* Orders for a turn that already ran. 
	 * Should be handled by areValidOrders() 
	 */
	assert( 0 );
      }
    }
    else {
      /* Some major error */
      result |= mail_TXT_Error( aGame, anEnvelope, 
				raceName, kind, resNumber, theTurnNumber );
    }
  }
  
  result = ( result ) ? EXIT_FAILURE : EXIT_SUCCESS;
  return result;
}
#else
int CMD_check( int argc, char **argv, int kind ) {
  int result;
  char *logName;
  envelope *anEnvelope;
  char* forecastName;
  char* returnAddress;
  char* raceName;
  char* password;
  char* final_orders;
  game* aGame;
  FILE* forecast;
  player* aPlayer;
  
  int resNumber, theTurnNumber;
  
  result = FALSE;
  
  logName = createString( "%s/log/orders_processed.txt", galaxynghome );
  openLog( logName, "a" );
  free( logName );
  
  plogtime( LBRIEF );
  if ( argc >= 2 ) {
    anEnvelope = createEnvelope(  );
    returnAddress = getReturnAddress( stdin );
    raceName = NULL;
    password = NULL;
    final_orders = NULL;
    aGame = NULL;
    resNumber = areValidOrders( stdin, &aGame, &raceName,
				&password, &final_orders, &theTurnNumber );
    plog( LBRIEF, "game %s\n", aGame->name );
    
    setHeader(anEnvelope, MAILHEADER_FROM, "%s", aGame->serverOptions.SERVERemail);
    
    setHeader( anEnvelope, MAILHEADER_TO, "%s", returnAddress );
    setHeader(anEnvelope, MAILHEADER_REPLYTO, aGame->serverOptions.ReplyTo);
    anEnvelope->from_name = strdup(aGame->serverOptions.SERVERname);
    anEnvelope->from_address = strdup(aGame->serverOptions.SERVERemail);

    if ( resNumber == RES_OK) {
      aPlayer = findElement( player, aGame->players, raceName );
      aPlayer->orders = NULL;
      
      plog( LBRIEF, "Orders from %s\n", returnAddress );
      
      /* produce an XML forecast */
      if ( aPlayer->flags & F_XMLREPORT ) {
	if ( ( theTurnNumber == LG_CURRENT_TURN ) ||
	     ( theTurnNumber == ( aGame->turn ) + 1 ) ) {
	  forecastName = createString( "%s/NG_XML_%d_forecast",
				       tempdir, getpid(  ) );
	  copyOrders( aGame, stdin, raceName, password, final_orders,
		      aGame->turn + 1 );
	  if ( ( forecast =
		 GOS_fopen( forecastName, "w" ) ) == NULL ) {
	    plog( LBRIEF, "Could not open %s for forecasting\n",
		  forecastName );
	    fprintf( stderr,
		     "Could not open %s for forecasting\n",
		     forecastName );
	    return EXIT_FAILURE;
	  }
	  
	  if (final_orders)
	    setHeader( anEnvelope, MAILHEADER_SUBJECT,
		       "[GNG] %s turn %d XML finalorders forecast for %s",
		       aGame->name, ( aGame->turn ) + 1, raceName );
	  else
	    setHeader( anEnvelope, MAILHEADER_SUBJECT,
		       "[GNG] %s turn %d XML forecast for %s",
		       aGame->name, ( aGame->turn ) + 1, raceName );
	  
	  checkOrders( aGame, raceName, forecast, F_XMLREPORT );
	  
	  fclose( forecast );
	  if ( kind == CMD_CHECK_REAL ) {
	    plog( LBRIEF, "mailing XML report %s to %s\n",
		  forecastName, anEnvelope->to );
	    fprintf( stderr, "mailing XML report %s to %s\n",
		     forecastName, anEnvelope->to );
	    result |= eMail( aGame, anEnvelope, forecastName );
	  } else {
	    char *forecastFile;
	    forecastFile =
	      createString( "%s/forecasts/%s/%s_XML",
			    galaxynghome, argv[2],
			    returnAddress );
	    GOS_copy( forecastName, forecastFile );
	  }
	  result |= GOS_delete( forecastName );
	  free( forecastName );
	}
      }
      
      /* produce a text forecast */
      if ( aPlayer->flags & F_TXTREPORT ) {
	if ( ( theTurnNumber == LG_CURRENT_TURN ) ||
	     ( theTurnNumber == ( aGame->turn ) + 1 ) ) {
	  forecastName = createString( "%s/NG_TXT_%d_forecast",
				       tempdir, getpid(  ) );
	  if ( ( forecast =
		 GOS_fopen( forecastName, "w" ) ) == NULL ) {
	    plog( LBRIEF, "Could not open %s for forecasting\n",
		  forecastName );
	    return EXIT_FAILURE;
	  }
	  
	  if ( aPlayer->orders == NULL )
	    copyOrders( aGame, stdin, raceName, password, final_orders,
			aGame->turn + 1 );
	  
	  if (final_orders)
	    setHeader( anEnvelope, MAILHEADER_SUBJECT,
		       "[GNG] %s turn %d text finalorders forecast for %s",
		       aGame->name, ( aGame->turn ) + 1, raceName );
	  else 
	    setHeader( anEnvelope, MAILHEADER_SUBJECT,
		       "[GNG] %s turn %d text forecast for %s",
		       aGame->name, ( aGame->turn ) + 1, raceName );
	  
	  checkOrders( aGame, raceName, forecast, F_TXTREPORT );
	  
	  fclose( forecast );
	  
	  if ( kind == CMD_CHECK_REAL ) {
	    plog( LBRIEF, "mailing TXT report %s to %s\n",
		  forecastName, anEnvelope->to );
	    fprintf( stderr, "mailing TXT report %s to %s\n",
		     forecastName, anEnvelope->to );
	    
	    result |= eMail( aGame, anEnvelope, forecastName );
	  } else {
	    char *forecastFile;
	    forecastFile =
	      createString( "%s/forecasts/%s/%s_TXT",
			    galaxynghome, argv[2],
			    returnAddress );
	    GOS_copy( forecastName, forecastFile );
	  }
	  result |= GOS_delete( forecastName );
	  free( forecastName );
	}
      }
      
    } else {
      forecastName = createString( "%s/NG_TXT_%d_errors",
				   tempdir, getpid(  ) );
      forecast = GOS_fopen( forecastName, "w" );
      setHeader( anEnvelope, MAILHEADER_SUBJECT,
		 "[GNG] Major Trouble" );
      plog( LBRIEF, "Major Trouble %d\n", resNumber );
      
      generateErrorMessage( resNumber, aGame, raceName,
			    theTurnNumber, forecast );
      fclose( forecast );
      
      if ( kind == CMD_CHECK_REAL ) {
	plog( LBRIEF, "mailing error report %s to %s\n", forecastName,
	      anEnvelope->to );
	
	result |= eMail( aGame, anEnvelope, forecastName );
      } else {
	char *forecastFile;
	forecastFile =
	  createString( "%s/forecasts/%s/%s_ERR",
			galaxynghome, argv[2], returnAddress );
	GOS_copy( forecastName, forecastFile );
			}
      result |= GOS_delete( forecastName );
      free( forecastName );
    }
    
    /* code here for advanced orders, we need to see how to determine this */
    if ( theTurnNumber > (aGame->turn+1)) {
      
      if ( aPlayer->orders == NULL )
	copyOrders( aGame, stdin, raceName, password, final_orders,
		    theTurnNumber );
      
      if (final_orders) {
	setHeader( anEnvelope, MAILHEADER_SUBJECT,
		   "[GNG] %s turn %d advance finalorders received for %s.",
		   aGame->name, theTurnNumber, raceName );
	plog( LBRIEF, "%s turn %d advance finalorders received for %s.\n",
	      aGame->name, theTurnNumber, raceName );
      }
      else {
	setHeader( anEnvelope, MAILHEADER_SUBJECT,
		   "[GNG] %s turn %d advance orders received for %s.",
		   aGame->name, theTurnNumber, raceName );
	plog( LBRIEF, "%s turn %d advance orders received for %s.\n",
	      aGame->name, theTurnNumber, raceName );
      }
      
      if ( aPlayer->flags & F_XMLREPORT ) {
	forecastName = createString( "%s/NG_XML_forecast", tempdir );
	forecast = GOS_fopen( forecastName, "w" );
	
	fprintf( forecast,
		 "<galaxy>\n  <variant>GalaxyNG</variant>\n" );
	fprintf( forecast, "  <version>%d.%d.%d</version>\n",
		 GNG_MAJOR, GNG_MINOR, GNG_RELEASE );
	fprintf( forecast, "  <game name=\"%s\">\n", aGame->name );
	fprintf( forecast, "    <turn num=\"%d\">\n", theTurnNumber );
	fprintf( forecast, "      <race name=\"%s\">\n", raceName );
	fprintf( forecast, "        <message>\n" );
	fprintf( forecast, "          <line num=\"1\">"
		 "O wise leader, your orders for turn %d</line>",
		 theTurnNumber );
	fprintf( forecast, "          <line num=\"2\">"
		 "have been received and stored.</line>" );
	fprintf( forecast, "        </message>\n" );
	fprintf( forecast, "      </race>\n" );
	fprintf( forecast, "    </turn>\n" );
	fprintf( forecast, "  </game>\n" );
	fprintf( forecast, "</galaxy>\n" );
	fclose( forecast );
	if ( kind == CMD_CHECK_REAL ) {
	  result |= eMail( aGame, anEnvelope, forecastName );
	} else {
	  char *forecastFile;
	  
	  forecastFile =
	    createString( "%s/forecasts/%s/%s_XML",
			  galaxynghome, argv[2], returnAddress );
	  GOS_copy( forecastName, forecastFile );
	}
	result |= GOS_delete( forecastName );
	free( forecastName );
      }
      
      if ( aPlayer->flags & F_TXTREPORT ) {
	if ( aPlayer->orders == NULL )
	  copyOrders( aGame, stdin, raceName, password, final_orders,
		      theTurnNumber );
	forecastName = createString( "%s/NG_TXT_forecast", tempdir );
	forecast = GOS_fopen( forecastName, "w" );
	fprintf( forecast, "O wise leader your orders for turn %d "
		 "have been received and stored.\n", theTurnNumber );
	fclose( forecast );
	if ( kind == CMD_CHECK_REAL ) {
	  result |= eMail( aGame, anEnvelope, forecastName );
	} else {
	  char *forecastFile;
	  
	  forecastFile =
	    createString( "%s/forecasts/%s/%s_TXT",
			  galaxynghome, argv[2], returnAddress );
	  GOS_copy( forecastName, forecastFile );
	}
	
	result |= GOS_delete( forecastName );
	free( forecastName );
      }
    }
  }
  
  if ( raceName )
    free( raceName );
  if ( password )
    free( password );
  destroyEnvelope( anEnvelope );
  result = ( result ) ? EXIT_FAILURE : EXIT_SUCCESS;
  
  return result;
}
#endif
