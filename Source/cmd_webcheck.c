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


int
CMD_webcheck( int argc, char **argv, int kind) {
  int result;
  char *logName;
  char *forecastName;
  char *raceName;
  char *password;
  char *final_orders;
  game *aGame;
  player *aPlayer;
  
  int resNumber, theTurnNumber;
  
  result = FALSE;
  
  logName = createString("%s/log/orders_processed.txt", galaxynghome);
  openLog(logName, "a");
  free(logName);
  
  plogtime(LBRIEF);
  if (argc >= 2) {
    raceName = NULL;
    password = NULL;
    final_orders = NULL;
    aGame = NULL;
    resNumber = areValidOrders(stdin, &aGame, &raceName,
			       &password, &final_orders, &theTurnNumber);
    plog(LBRIEF, "game %s\n", aGame->name);
    
    if (resNumber == RES_OK) {
      aPlayer = findElement(player, aGame->players, raceName);
      aPlayer->orders = NULL;
      
      plog(LBRIEF, "Orders from %s\n", raceName);
      
      /* produce an XML forecast */
      if (aPlayer->flags & F_XMLREPORT) {
	if ((theTurnNumber == LG_CURRENT_TURN) ||
	     (theTurnNumber == (aGame->turn) + 1)) {

	  copyOrders(aGame, stdin, raceName, password, final_orders,
		      aGame->turn + 1);
	  
	  checkOrders(aGame, raceName, stdout, F_XMLREPORT);
	  
	  if (kind != CMD_CHECK_REAL) {
	    char *forecastFile;
	    forecastFile =
	      createString("%s/forecasts/%s/%s_XML",
			    galaxynghome, argv[2],
			    raceName);
	    GOS_copy(forecastName, forecastFile);
	  }
	  result |= GOS_delete(forecastName);
	  free(forecastName);
	}
      }
      
      /* produce a text forecast */
      if (aPlayer->flags & F_TXTREPORT) {
	if ((theTurnNumber == LG_CURRENT_TURN) ||
	     (theTurnNumber == (aGame->turn) + 1)) {
	  
	  if (aPlayer->orders == NULL)
	    copyOrders(aGame, stdin, raceName, password, final_orders,
			aGame->turn + 1);
	  
	  checkOrders(aGame, raceName, stdout, F_TXTREPORT);
	  
	}
      }
      
    }
    else {
      plog(LBRIEF, "Major Trouble %d\n", resNumber);
      
      generateErrorMessage(resNumber, aGame, raceName,
			    theTurnNumber, stdout);
      
    }
    
    /* code here for advanced orders, we need to see how to determine this */
    if (theTurnNumber > (aGame->turn+1)) {
      
      if (aPlayer->orders == NULL)
	copyOrders(aGame, stdin, raceName, password, final_orders,
		    theTurnNumber);
      
      if (final_orders) {
	plog(LBRIEF, "%s turn %d advance finalorders received for %s.\n",
	      aGame->name, theTurnNumber, raceName);
      }
      else {
	plog(LBRIEF, "%s turn %d advance orders received for %s.\n",
	      aGame->name, theTurnNumber, raceName);
      }
      
      if (aPlayer->flags & F_XMLREPORT) {
	
	fprintf(stdout,
		 "<galaxy>\n  <variant>GalaxyNG</variant>\n");
	fprintf(stdout, "  <version>%d.%d.%d</version>\n",
		 GNG_MAJOR, GNG_MINOR, GNG_RELEASE);
	fprintf(stdout, "  <game name=\"%s\">\n", aGame->name);
	fprintf(stdout, "    <turn num=\"%d\">\n", theTurnNumber);
	fprintf(stdout, "      <race name=\"%s\">\n", raceName);
	fprintf(stdout, "        <message>\n");
	fprintf(stdout, "          <line num=\"1\">"
		 "O wise leader, your orders for turn %d</line>",
		 theTurnNumber);
	fprintf(stdout, "          <line num=\"2\">"
		 "have been received and stored.</line>");
	fprintf(stdout, "        </message>\n");
	fprintf(stdout, "      </race>\n");
	fprintf(stdout, "    </turn>\n");
	fprintf(stdout, "  </game>\n");
	fprintf(stdout, "</galaxy>\n");
     }
 
      if (aPlayer->flags & F_TXTREPORT) {
	if (aPlayer->orders == NULL)
	  copyOrders(aGame, stdin, raceName, password, final_orders,
		      theTurnNumber);
	fprintf(stdout, "O wise leader your orders for turn %d "
		 "have been received and stored.\n", theTurnNumber);
	
      }
    }
  }
  
  if (raceName)
    free(raceName);
  if (password)
    free(password);

  result = (result) ? EXIT_FAILURE : EXIT_SUCCESS;
  
  return result;
}
