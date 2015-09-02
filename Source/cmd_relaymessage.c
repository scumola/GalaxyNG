#include "galaxyng.h"

/****i* CLI/relayMessage
 * NAME
 *   relayMessage --
 ******
 */

int
relayMessage(game *aGame, envelope *anEnvelope, char *raceName,
	     player* from, emailList* to )
{
  char *messageName;
  char *isRead;
  FILE *message;
  
  int result;
  static int message_read = 0;
  static strlist* msg;
  strlist* s;
  
  result = 1;
  
  messageName = createString( "%s/NGmessage_%s_%s", tempdir, aGame->name, from->name );
  
  if (!message_read) {
    char  lineBuffer[2 * LINE_BUFFER_SIZE];
    message_read = 1;

    msg = makestrlist("\n-*- Message follows -*-\n\n" );
    
    for ( isRead = fgets( lineBuffer, LINE_BUFFER_SIZE, stdin );
	  isRead;
	  isRead = fgets( lineBuffer, LINE_BUFFER_SIZE, stdin ) ) {
      char* ptr;
      
      if ((ptr = strstr(lineBuffer, from->pswd)) != NULL)
	memset(ptr, '*', strlen(from->pswd));
      
      if (noCaseStrncmp("#end", lineBuffer, 4) == 0)
	break;
      addList(&msg, makestrlist(lineBuffer));
    }
    
  }
  
  if ( to->addr ) {
    if ( ( message = fopen( messageName, "w" ) ) ) {
      
      setHeader( anEnvelope, MAILHEADER_TO, "%s", to->addr );
      setHeader(anEnvelope, MAILHEADER_REPLYTO, aGame->serverOptions.ReplyTo);
      anEnvelope->from_name = strdup(aGame->serverOptions.SERVERname);
      anEnvelope->from_address = strdup(aGame->serverOptions.SERVERemail);
      
      if (strstr(raceName, "@") != NULL) {
	setHeader(anEnvelope, MAILHEADER_SUBJECT,
		  "[GNG] %s message relay GM", aGame->name);
      }
      else {
	setHeader( anEnvelope, MAILHEADER_SUBJECT,
		   "[GNG] %s message relay %s", aGame->name, raceName );
      }
      
      fprintf( message, "#galaxy %s %s %s\n",
	       aGame->name, to->name, to->pswd );
      
      for (s = msg; s; s = s->next)
	fprintf(message, "%s\n", s->str);
      
      fprintf(message, "\n\n");
      fclose( message );
      result = eMail( aGame, anEnvelope, messageName );
      result |= ssystem( "rm %s", messageName );
      free( messageName );
    }
    else {
      fprintf( stderr, "Can't open \"%s\".\n", messageName );
    }
  }
  
  return result;
}
