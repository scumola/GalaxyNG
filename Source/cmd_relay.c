#include "galaxyng.h"

/****i* CLI/CMD_relay
 * NAME 
 *   CMD_relay -- relay a message from one race to another.
 * FUNCTION
 ******
 */

#define SINGLE_PLAYER 0
#define ALL_PLAYERS   1

int
CMD_relay( int argc, char **argv ) 
{
  int   result;
  char* logName;
  char* confirmName;
  int   resNumber;
  game* aGame;
  FILE* confirm;
  emailList* toPlayers = NULL;
  emailList* listPlayer;
  
  player* itPlayer;
  player* fromPlayer;
  char* destination;
  char* raceName;
  char* password;
  char* final_orders;
  int theTurnNumber = LG_CURRENT_TURN;
  envelope* anEnvelope;
  
  result = EXIT_FAILURE;
  
  if (argc < 2) {
    usage();
    return result;
  }
  
  logName = createString("%s/log/orders_processed.txt", galaxynghome);
  openLog(logName, "a");
  free(logName);
  plogtime(LBRIEF);
  
  anEnvelope = readEnvelope(stdin);
  
  destination = getDestination( anEnvelope->subject );
  
  raceName = NULL;
  password = NULL;
  final_orders = NULL;
  aGame = NULL;
  resNumber = areValidOrders( stdin, &aGame, &raceName, &password,
			      &final_orders, &theTurnNumber );

  plog(LBRIEF, "CMD_relay(\"%s\", \"%s\")\n", raceName, destination);

  confirmName = createString("%s/relay_%s_confirm", tempdir, raceName);
  if ((confirm = fopen(confirmName, "w")) == NULL) {
    plog(LBRIEF, "Can't open \"%s\".\n", confirmName);
    fprintf(stderr, "Can't open \"%s\".\n", confirmName);
    free(confirmName);
    return result;
  }
  
  setHeader(anEnvelope, MAILHEADER_FROM, "%s",
	    aGame->serverOptions.SERVERemail);
  setHeader(anEnvelope, MAILHEADER_REPLYTO, aGame->serverOptions.ReplyTo);
  anEnvelope->from_name = strdup(aGame->serverOptions.SERVERname);
  anEnvelope->from_address = strdup(aGame->serverOptions.SERVERemail);

  fprintf(confirm, "Relay for game \"%s\"\n", aGame->name);
  
  /* we need to make a list of recipients */
  if (noCaseStrcmp(raceName, "GM") == 0) {
    fromPlayer = allocStruct(player);
    fromPlayer->name = strdup("GM");
    fromPlayer->addr = strdup(aGame->serverOptions.GMemail);
    fromPlayer->pswd = strdup(aGame->serverOptions.GMpassword);
  }
  else 
    fromPlayer = findElement(player, aGame->players, raceName);
  
  if (fromPlayer == NULL) {
    setHeader(anEnvelope, MAILHEADER_SUBJECT, "[GNG] Major Trouble");
    generateErrorMessage(RES_PLAYER, aGame, raceName,
			 theTurnNumber, confirm);
    fprintf(confirm, "\n\n%s\n", vcid);
    fclose(confirm);
    result |= eMail(aGame, anEnvelope, confirmName);
    plog(LBRIEF, "cmd_relay: can't tell who mail is from.\n");
    return result;
  }
  
  if ( destination == NULL ) {
    setHeader(anEnvelope, MAILHEADER_SUBJECT, "[GNG] Major Trouble");
    generateErrorMessage(RES_NODESTINATION, aGame, raceName,
			 theTurnNumber, confirm);
    fprintf(confirm, "\n\n%s\n", vcid);
    fclose(confirm);
    result |= eMail(aGame, anEnvelope, confirmName);
    plog(LBRIEF, "cmd_relay: can't tell who mail is to.\n");
    return result;
  }
  
  if (noCaseStrcmp(destination, aGame->name) == 0) {
    /* since we are relaying to the game, then the relay goes to
     * all players and the GM
     */

    for (itPlayer = aGame->players; itPlayer; itPlayer = itPlayer->next) {
      /* skip dead players, they dislike getting email about the game :)
       */
      if (itPlayer->flags & F_DEAD) {
	continue;
      }
      
      /* don't relay to yourself -- note: this is a bogus check,
       * but I'm leaving it in to remind myself to fix it. This
       * is a C ==, not a class ==
       * */
      if (itPlayer == fromPlayer)
		  continue;
      
      listPlayer = allocStruct(emailList);
      listPlayer->name = strdup(itPlayer->name);
      listPlayer->addr = strdup(itPlayer->addr);
      listPlayer->pswd = strdup(itPlayer->pswd);
      
      addList(&toPlayers, listPlayer);
    }
    
    listPlayer = allocStruct(emailList);
    listPlayer->name = strdup("GM");
    listPlayer->addr = strdup(aGame->serverOptions.GMemail);
    listPlayer->pswd = strdup(aGame->serverOptions.GMpassword);

    addList(&toPlayers, listPlayer);
  }
  else {
    /* if we are only sending to a single player, there are two
     * possibilities: I've named the GM or I've named a player
     */
    if (noCaseStrcmp(destination, "GM") == 0) {
      
      listPlayer = allocStruct(emailList);
      listPlayer->name = strdup("GM");
      listPlayer->addr = strdup(aGame->serverOptions.GMemail);
      listPlayer->pswd = strdup(aGame->serverOptions.GMpassword);

      addList(&toPlayers, listPlayer);
    }
    else {
      if ((itPlayer =
	   findElement(player, aGame->players, destination)) == NULL) {
		  setHeader(anEnvelope, MAILHEADER_SUBJECT,
					"[GNG] Major Trouble");
		  generateErrorMessage(RES_DESTINATION, aGame, raceName,
							   theTurnNumber, confirm);
		  fprintf(confirm, "\n\n%s\n", vcid);
		  fclose(confirm);
		  result |= eMail(aGame, anEnvelope, confirmName);
		  plog(LBRIEF, "can't find player mail is directed to.\n");
		  return result;
      }
      
      /* is the relay to a dead player? */
      if (itPlayer->flags & F_DEAD) {
		  setHeader(anEnvelope, MAILHEADER_SUBJECT,
					"[GNG] Major Trouble");
		  generateErrorMessage(RES_DEAD_PLAYER, aGame, raceName,
							   theTurnNumber, confirm);
		  fprintf(confirm, "\n\n%s\n", vcid);
		  fclose(confirm);
		  result |= eMail(aGame, anEnvelope, confirmName);
		  plog(LBRIEF, "cmd_relay: relay is to a dead player.\n");
		  return result;
      }
      
      
      listPlayer = allocStruct(emailList);
      listPlayer->name = strdup(itPlayer->name);
      listPlayer->addr = strdup(itPlayer->addr);
      listPlayer->pswd = strdup(itPlayer->pswd);

      addList(&toPlayers, listPlayer);
    }
  }
 
  
  for (listPlayer = toPlayers; listPlayer; listPlayer = listPlayer->next) {
    
    result = relayMessage(aGame, anEnvelope, raceName, fromPlayer, listPlayer);
    
    if (result == 0) {
      setHeader(anEnvelope, MAILHEADER_SUBJECT, "[GNG] %s message sent to %s", aGame->name, listPlayer->name);
      fprintf(confirm, "Message has been sent to %s.\n", listPlayer->name);
    }
    else {
      setHeader(anEnvelope, MAILHEADER_SUBJECT,
		"[GNG] %s message not sent to %s", aGame->name, listPlayer->name);
      fprintf(confirm, "Due to a server error the message was not "
	      "sent to %s!\nPlease contact your Game Master.\n",
	      listPlayer->name);
    }
  }
  
  
  fprintf(confirm, "\n\n%s\n", vcid);
  fclose(confirm);
  setHeader(anEnvelope, MAILHEADER_TO, fromPlayer->addr);
  result |= eMail(aGame, anEnvelope, confirmName);
  if (destination)
    free(destination);
  if (raceName)
    free(raceName);
  if (password)
    free(password);
  destroyEnvelope(anEnvelope);
  
  result |= ssystem("rm %s", confirmName);
  result = result ? EXIT_FAILURE : EXIT_SUCCESS;
  
  return result;
}
