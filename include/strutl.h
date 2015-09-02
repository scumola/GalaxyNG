#if !defined(STRUTL_H_)
#define STRUTL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <assert.h>

#include "cookies.h"
#include "logging.h"

/****s* GalaxyNG/strlist
 * NAME
 *   strlist -- 
 * PURPOSE
 *   Structure to store strings. Used to store orders, mistakes, and
 *   messages.
 * NOTES
 *   Has a wrong name. Should be called String or so, since it is
 *   an element in a list.
 * SOURCE
 */

typedef struct strlist {
  struct strlist *next;
  long            cookie;
  char           *str;
} strlist;

/********/

char*    getstr(char* s);
strlist* makestrlist(char* ns);
void     freestrlist(strlist* s);
void     dumpstrlist(strlist* aStrlist);
int      noCaseStrncmp(const char *s, const char *t, int n);
int      noCaseStrcmp(const char *s, const char *t);
char*    strlwr(char* str);

#endif

