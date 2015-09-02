#include "mail.h"
#include "util.h"
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


/****h* GalaxyNG/Mail
 * FUNCTION
 *   This module contains functions to create,
 *   fill and send email messages.
 *****
 */


void createMailToAllHeader(game *aGame) {
  player* aPlayer;
  int state;

  if (aGame->serverOptions.SERVERemail) {
    if (aGame->serverOptions.SERVERname) { 
      printf("From: %s <%s>\n", aGame->serverOptions.SERVERname, aGame->serverOptions.SERVERemail);
    } else {
      printf("From: %s\n", aGame->serverOptions.SERVERemail);
    }
  }
  
  printf("To: %s\nBCC: ", aGame->serverOptions.GMemail); 
  for (aPlayer = aGame->players, state = 0;
       aPlayer;
       aPlayer = aPlayer->next) {
    if (!(aPlayer->flags & F_DEAD)) { 
      if (state == 0) {
	printf("  %s", aPlayer->addr);
	state = 1;
      } else {
	printf(",\n  %s", aPlayer->addr);
      }
    }
  }
  printf("\nSubject:\n");
}

/****f* Mail/createEnvelope
 * NAME
 *   createEnvelope -- create an envelope.
 * FUNCTION
 *   Creates and initializes an envelope.
 ******
 */

envelope* createEnvelope() {
  envelope *e;
  e = malloc(sizeof(envelope));
  assert(e != NULL);
  e->to = NULL;
  e->from = NULL;
  e->replyto = NULL;
  e->subject = NULL;
  e->from_address = NULL;
  e->from_name = NULL;
  e->bcc = NULL;
  e->cc = NULL;
  e->compress = FALSE;
  e->contentType = NULL;
  e->contentEncoding = NULL;
  e->contentDescription = NULL;
  return e; 
}


/****f* Mail/readEnvelope
 * NAME
 *   readEnvelope -- create an envelope, filling in values from mail header.
 * FUNCTION
 *   Creates and initializes an envelope from a file.
 ******
 */

envelope* readEnvelope(FILE* fp) {
  envelope *e;
  char      buffer[4096];
  char*     ptr;
  
  e = createEnvelope();
  
  while (fgets(buffer, 4096, fp) != NULL) {
    *(strchr(buffer, '\n')) = '\0';
    
    if (buffer[0] == '\0')
      break;				/* end of headers */
    
    if ((ptr = strchr(buffer, ':')) == NULL) 
      continue;			/* skip header, no : in it */
    
    *ptr = '\0';
    
    if (noCaseStrcmp(buffer, "from") == 0) {
      e->from = strdup(ptr+2);
    }
    else if (noCaseStrcmp(buffer, "to") == 0) {
      e->to = strdup(ptr+2);
    }
    else if (noCaseStrcmp(buffer, "subject") == 0) {
      e->subject = strdup(ptr+2);
    }
    else if (noCaseStrcmp(buffer, "cc") == 0) {
      e->cc = strdup(ptr+2);
    }
    else if (noCaseStrcmp(buffer, "content-type") == 0) {
      e->contentType = strdup(ptr+2);
    }
    else if (noCaseStrcmp(buffer, "content-transfer-encoding") == 0) {
      e->contentEncoding = strdup(ptr+2);
    }
    else if (noCaseStrcmp(buffer, "content-description") == 0) {
      e->contentDescription = strdup(ptr+2);
    }
    else
      continue;
  }

  return e; 
}


/****f* Mail/setHeader
 * NAME
 *   setHeader -- write something on the envelope.
 * SYNOPSIS
 *   void setHeader(envelope *e, int headerType, char *format, ...)
 * FUNCTION
 *   Dynamically create a mail header. 
 *******
 */

void setHeader(envelope *e, int headerType, char *format, ...) {
  int     n;
  
  va_list ap;
  
  assert(e != NULL);
  
  va_start(ap, format);
#ifdef WIN32
  vsprintf(lineBuffer, format, ap);
#else
  n = vsnprintf(lineBuffer, LINE_BUFFER_SIZE, format, ap);
  assert(n != -1);
#endif
  va_end(ap);
  
  switch(headerType) {
  case MAILHEADER_TO:
    if (e->to)
      free(e->to);
    e->to = strdup(lineBuffer);
    break;
    
  case MAILHEADER_FROM:
    if (e->from)
      free(e->from);
    e->from = strdup(lineBuffer);
    break;
    
  case MAILHEADER_REPLYTO:
    if (e->replyto)
      free(e->replyto);
    e->replyto = strdup(lineBuffer);
    break;
    
  case MAILHEADER_SUBJECT:
    if (e->subject)
      free(e->subject);
    e->subject = strdup(lineBuffer);
    break;
    
  case MAILHEADER_BCC:
    if (e->bcc)
      free(e->bcc);
    e->bcc = strdup(lineBuffer);
    break;
    
  case MAILHEADER_CC:
    if (e->cc)
      free(e->cc);
    e->cc = strdup(lineBuffer);
    break;
    
    
  case MAILHEADER_CONTENTTYPE:
    if (e->contentType)
      free(e->contentType);
    e->contentType = strdup(lineBuffer);
    break;
    
  case MAILHEADER_CONTENTENCODING:
    if (e->contentEncoding)
      free(e->contentEncoding);
    e->contentEncoding = strdup(lineBuffer);
    break;
    
  case MAILHEADER_CONTENTDESCRIPTION:
    if (e->contentDescription)
      free(e->contentDescription);
    e->contentDescription = strdup(lineBuffer);
    
  default:
    assert(0);
  }
}

/****f* Mail/destroyEnvelope
 * NAME
 *   destroyEnvelope -- free memory used by an envelope.
 * SYNOPSIS
 *   destroyEnvelope(envelope *e)
 ********
 */

void destroyEnvelope(envelope *e) {
  assert(e != NULL);
  if (e->to)
    free(e->to);
  if (e->from)
    free(e->from);
  if (e->from_address)
    free(e->from_address);
  if (e->from_name)
    free(e->from_name);
  if (e->replyto)
    free(e->replyto);
  if (e->subject)
    free(e->subject);
  if (e->bcc)
    free(e->bcc);
  if (e->cc)
    free(e->cc);
  if (e->contentType)
    free(e->contentType);
  if (e->contentEncoding)
    free(e->contentEncoding);
  if (e->contentDescription)
    free(e->contentDescription);
  
  free(e);
}


/****f* Mail/eMail 
 * NAME
 *   eMail -- mail the contents of a file to someone.
 * FUNCTION
 *   Mail a file to someone. 
 * INPUTS
 *   e        -- envelope
 *   aGame    -- game structure, contains settings for mailer etc.
 *   fileName -- name of the file to send.
 * RESULT
 *   status  0, all OK.
 *          >0, something went wrong
 * BUGS
 *   Does not write anything to the log file.
 ******
 */

int eMail(game *aGame, envelope *e, char *fileName) {
  FILE*  mailFile;
  char   template[128];
  int    result;
  char   command[4096];

  pdebug(DFULL, "eMail\n");
 
  sprintf(template, "%s/galaxyXXXXXX", tempdir);
 
  assert(fileName != NULL);
  assert(aGame != NULL);
  
  mailFile = fdopen(mkstemp(template),  "w");
  result = 1;
  
  assert(e->to);
  assert(e->subject);

  if (e->from_address && e->from_name) { 
    fprintf(mailFile, "From: %s <%s>\n", e->from_name, e->from_address);
  } else if (e->from) {
    fprintf(mailFile, "From: %s\n", e->from);
  }
  
  fprintf(mailFile, "To: %s\n", e->to);
  fprintf(mailFile, "Subject: %s\n", e->subject);
  
  if (e->replyto)
    fprintf(mailFile, "Reply-To: %s\n", e->replyto);
  
  if (e->bcc) 
    fprintf(mailFile, "BCC: %s\n", e->bcc);
  if (e->cc)
    fprintf(mailFile, "CC: %s\n", e->cc);
  if (e->contentType)
    fprintf(mailFile, "Content-Type: %s\n", e->contentType);
  if (e->contentEncoding)
    fprintf(mailFile, "Content-tranfer-encoding: %s\n",
	    e->contentEncoding);
  if (e->contentDescription)
    fprintf(mailFile, "Content-description: %s\n", e->contentDescription);
  
#ifndef WIN32
  if (e->compress &&
      aGame->serverOptions.compress &&
      aGame->serverOptions.encode) {
    addMimeHeader(mailFile);
  }
  fprintf(mailFile, "\n\n");
  if (e->compress &&
      aGame->serverOptions.compress &&
      aGame->serverOptions.encode) {
    char *relative_path; 
    char *ptr;
    char zipped_name[4096];
    char encoded_name[4096];

    addMimeText(mailFile);
    fprintf(mailFile, "Turn report is attached as .zip file.\n\n");
    relative_path = strstr(fileName, "reports");
    if (relative_path == NULL) {
      fprintf(stderr, 
	      "Reports are not in their standards position\n");
      relative_path = fileName;
    }
    strcpy(zipped_name, fileName);
    if ((ptr = strrchr(zipped_name, '.')) != NULL)
	*ptr = '_';
    strcat(zipped_name, ".zip");
    strcpy(encoded_name, fileName);
    if ((ptr = strrchr(encoded_name, '.')) != NULL)
	*ptr = '_';
    result = ssystem("%s %s %s > /dev/null 2>&1",
		     aGame->serverOptions.compress,
		     zipped_name,
		     relative_path);
    result |= ssystem("%s < %s > %s 2> /dev/null", 
		      aGame->serverOptions.encode, 
		      zipped_name, 
		      encoded_name);
    addMimeZip(mailFile);
    result |= appendToMail(encoded_name, mailFile);
    addMimeEnd(mailFile);
    result |= ssystem("rm %s %s", zipped_name, encoded_name);
  }
  else {
    result = appendToMail(fileName, mailFile);
  }
#endif
  fclose(mailFile);
#ifndef WIN32
  sprintf(command, "%s", aGame->serverOptions.sendmail);
  if (e->from_address) {
    char tmpBuf[4096];
    sprintf(tmpBuf, " -f \"%s\"", e->from_address);
    strcat(command, tmpBuf);
  }
  if (e->from_name) {
    char tmpBuf[4096];
    sprintf(tmpBuf, " -F \"%s\"", e->from_name);
    strcat(command, tmpBuf);
  }
  result |= ssystem("%s < %s", command, template);
  result |= ssystem("rm %s", template);
#endif
  return result;
}



void
addMimeHeader(FILE *mailFile) 
{
  fprintf(mailFile, "Mime-Version: 1.0\n");
  fprintf(mailFile, 
	  "Content-Type: multipart/mixed; boundary=\"9jxsPFA5p3P2qPhR\"\n");
  fprintf(mailFile, "Content-Disposition: inline\n");
}



void
addMimeText(FILE *mailFile)
{
  fprintf(mailFile, "--9jxsPFA5p3P2qPhR\n");
  fprintf(mailFile, "Content-Type: text/plain; charset=us-ascii\n");
  fprintf(mailFile, "Content-Disposition: inline\n");
  fprintf(mailFile, "\n");
}



void
addMimeUUE(FILE *mailFile)
{
  fprintf(mailFile, "--9jxsPFA5p3P2qPhR\n");
  fprintf(mailFile, "Content-Type: application/zip\n");
  fprintf(mailFile, "Content-Disposition: attachment; filename=\"turn.zip\"\n");
  fprintf(mailFile, "Content-Transfer-Encoding: base64\n");
  fprintf(mailFile, "\n");
}

void
addMimeZip(FILE *mailFile)
{
  fprintf(mailFile, "--9jxsPFA5p3P2qPhR\n");
  fprintf(mailFile, "Content-Type: application/zip\n");
  fprintf(mailFile, "Content-Disposition: attachment; filename=\"turn.zip\"\n");
  fprintf(mailFile, "Content-Transfer-Encoding: base64\n");
  fprintf(mailFile, "\n");
}



void
addMimeEnd(FILE *mailFile)
{
  fprintf(mailFile, "\n--9jxsPFA5p3P2qPhR--\n");
}



int
appendToMail(char *fileName, FILE *mailFile)
{
  FILE *f;
  char *isRead;
  f = fopen(fileName, "r");
  if (f) {
    for (isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, f);
	 isRead;
	 isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, f)) {
      fputs(lineBuffer, mailFile);
    }
    fclose(f);
    return FALSE;
  } else {
    return TRUE;
  }
}


/****f* Process/getReturnAddress
 * NAME
 *   getReturnAddress
 * FUNCTION
 *   Extract the return address from a players email.
 ******
 */

char *
getReturnAddress( FILE *orders )
{
  char *isRead;
  char *c;
  
  for ( isRead = fgets( lineBuffer, LINE_BUFFER_SIZE, orders );
	isRead; isRead = fgets( lineBuffer, LINE_BUFFER_SIZE, orders ) ) {
    /* WIN32 */
    if ( noCaseStrncmp( string_mail_to, lineBuffer, 3 ) == 0 )
      break;
  }
  assert( isRead != NULL );
  for ( c = lineBuffer; *c; c++ ) {
    if ( *c == '\n' )
      *c = '\0';
  }
  
  return strdup( lineBuffer + 3 );
}


void gmNotify(char* subject, char* filename, game* aGame) {
	envelope* env;

	env = createEnvelope();

	if (aGame == NULL) {
		char  buffer[4096];
		FILE* tmpFP;

		aGame = (game*)malloc(sizeof(game));
		
		loadNGConfig(aGame);
		if (aGame->serverOptions.GMemail == NULL) {
			fprintf(stderr, "**ERROR** Could not load game or config in "
					"gmNotify!!!\n");
			fprintf(stderr, "This is the message that was trying to be "
					"delivered:\n");
			fprintf(stderr, "Subject: %s\n", subject);
			tmpFP = fopen(filename, "r");
			while (fgets(buffer, 4096, tmpFP) != NULL)
				fprintf(stderr, "%s", buffer);
			fclose(tmpFP);
			return;
		}
	}
	
	env->to = strdup(aGame->serverOptions.GMemail);
	env->from = strdup(aGame->serverOptions.SERVERemail);
	env->replyto = strdup(aGame->serverOptions.ReplyTo);
	env->subject = strdup(subject);

	eMail(aGame, env, filename);

	destroyEnvelope(env);
	
	return;
}
