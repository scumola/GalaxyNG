#ifndef GNG_MAIL_H
#define GNG_MAIL_H

#include "galaxy.h"
#include "util.h"

#ifdef WIN32
#include <stdio.h>
#include <stdarg.h>
#include <TCHAR.H> 
#endif

/****s* Mail/envelope
 * NAME
 *   envelope -- envelope for an email message.
 * PURPOSE
 *   Store the envelope of an email message.
 * SOURCE
 */

typedef struct envelope {
  int   compress;   /* Compress the body before sending */
  char* to;        /* destination */
  char* from;      /* Who sent it */
  char* subject;
  char* replyto;
  char* cc;
  char* bcc;
  char* contentType;
  char* contentEncoding;
  char* contentDescription;
  char* from_address;
  char* from_name;
} envelope;

/********/

/****d* Mail/MailHeaderTypes
 * NAME
 *   MailHeaderTypes -- things we can write on the envelope
 * SOURCE
 */

enum {MAILHEADER_TO = 1,
	  MAILHEADER_FROM,
	  MAILHEADER_SUBJECT,
	  MAILHEADER_REPLYTO,
	  MAILHEADER_CC,
	  MAILHEADER_BCC,
	  MAILHEADER_CONTENTTYPE,
	  MAILHEADER_CONTENTENCODING,
	  MAILHEADER_CONTENTDESCRIPTION
};

/******/

void createMailToAllHeader(game *aGame);
envelope* createEnvelope(void);
envelope* readEnvelope(FILE* fp);
void      destroyEnvelope(envelope *e);
void      setHeader(envelope *e, int headerType, char *format, ...);
int       eMail(game *aGame, envelope *e, char *fileName);
char*     getReturnAddress(FILE * orders);


void      addMimeHeader(FILE *mailFile);
void      addMimeText(FILE *mailFile);
void      addMimeZip(FILE *mailFile);
void      addMimeEnd(FILE *mailFile);
int       appendToMail(char *fileName, FILE *mailFile);
void      gmNotify(char* subject, char* filename, game* aGame);

#endif				/* GNG_MAIL_H */
