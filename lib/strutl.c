#include "strutl.h"

/****f* strutl/getstr 
 *
 * NAME 
 *   getstr -- extract a word from a longer string 
 * SYNOPSIS 
 *   char *getstr(char *s)
 *   word = getstr(someString)
 * FUNCTION
 *   Extract a word from a string of words.  A word is any things that
 *   is seperated by white spaces or a comma, any string that is
 *   delimited by quotes ("), or or any string delimited by { }.
 *
 *   The function is intended to parse a string word by word. It
 *   should be first called as:
 *
 *     firstword = getstr(myStringOfWords);
 *
 *   This gets the first word from the string pointed to by
 *   myStringOfWords.  Then consecutive calls of
 *
 *     nextword = getstr(NULL);
 *
 *   you get the remaining words.
 *
 *   White spaces inside a quote delimited word are turned into 
 *   underscores.  
 *
 *   In a string a ';' signifies the start of a comment. Any words
 *   after a ';' are not parsed.
 * 
 *   In the non comment part ';' and '<' are removed before the words
 *   are returned.
 *  
 * RESULTS
 *   word = a pointer to a nul terminated string.
 *
 *   Program is aborted if the length of the word is longer than 256,
 *   since then some is most likely trying to crack your system.
 * 
 *   When there are no more words"\0" is returned.
 * NOTES
 *   This function can only work on one string at the time since it
 *   works with a statically allocated buffer.  This function is used
 *   almost everywhere in the program.
 * SOURCE
 */

char *
getstr( char *s )
{
    static char *s1;

    static char buf[256];
    int i;
    int j;

    if ( s )
        s1 = s;

    /* first invocation of this function, for an order line.  Each next
     * times, for the same order line, s1 will "progress" by a word to
     * the right
     */
    assert( s1 != NULL );

    i = 0;
    for ( ; *s1 && isspace( *s1 ); s1++ )       /* skips spaces */
        ;

    if ( *s1 == '"' ) {
        /* Players can enclose name (ie : including spaces) with double quotes */
        for ( s1++; *s1 && *s1 != '"'; ) {
            buf[i] = isspace( *s1 ) ? '_' : *s1;
            s1++;
            i++;
            assert( i < 256 );  /* abort execution if s1 too long */
        }
    } else if ( *s1 == '{' ) {
        for ( s1++; *s1 && *s1 != '}'; ) {
            buf[i] = *s1;
            s1++;
            i++;
            assert( i < 256 );  /* abort execution if s1 too long */
        }
    } else {
        if ( *s1 != ';' ) {     /* otherwise, it's a comment */
            for ( ; *s1 && !isspace( *s1 ) && *s1 != ','; ) {
                /* space or ',' can be used as separators, */
                buf[i] = *s1;
                s1++;
                i++;
                assert( i < 256 );      /* abort execution if s1 too long */
            }
        }
    }

    buf[i] = '\0';
    if ( *s1 )
        s1++;                   /* Skip ',' or space */
    /* CB, 19980922. Remove ";" and "<" from names (planets, ships...), * *
     * * * to * protect machine report from corruption. Don't break messages 
     * * * * * and * comments. */
    i = 0;
    j = 0;
    while ( buf[j] && j < 256 ) {
        if ( buf[j] != ';' && buf[j] != '<' ) {
            buf[i] = buf[j];
            i++;
        }
        j++;
    }
    if ( i )
        buf[i] = '\0';

    return buf;
}

/***********/



/****f* strutl/makestrlist 
 * NAME
 *   makestrlist -- create an element for a string list
 * FUNCTION
 *   
 * RESULT
 *   Pointer to an initialized strlist structure.
 * BUGS
 *   This function has a wrong name.
 * SOURCE
 */

strlist* makestrlist(char* ns) {
    strlist* s;
    strlist* cur;

	char* tmp;
    char* sPtr;
	char* ePtr;
	char* sep;
	
    cur = NULL;

	sPtr = tmp = strdup(ns);
    sep = ePtr = strpbrk(sPtr, "\n\r");

	if (ePtr == NULL) {
		cur = (strlist*)allocStruct(strlist);
		cur->str = tmp;
		return cur;
	}
	
    do {
        if ( cur ) {
            cur->next = ( strlist * ) allocStruct( strlist );
            cur = cur->next;
        } else
            s = cur = ( strlist * ) allocStruct( strlist );

		*ePtr = '\0';

        cur->str = strdup( sPtr );

		if (*sep == '\n' && *(ePtr+1) == '\r')
			sPtr = ePtr + 2;
		else
			sPtr = ePtr + 1;
    } while ((sep = ePtr = strpbrk(sPtr, "\n\r")) != NULL);

	if (*sPtr != '\0') {
		cur->next = ( strlist * ) allocStruct( strlist );
		cur = cur->next;
		cur->str = strdup(sPtr);
	}
		
    free( tmp );

    return s;
}

/*********/


/****f* strutl/freestrlist
 * NAME
 *   freestrlist -- free memory used by a string list.
 * SYNOPSIS
 *   void freestrlist(strlist *s)
 * FUNCTION
 *   Free all memory used by a string list. Before
 *   trying to free the memory used by an element check
 *   if the element is valid.
 * SOURCE
 */

void
freestrlist( strlist *s )
{
    strlist *s2;

    pdebug( DFULL, "free strlist\n" );
    while ( s ) {
        validateStruct( strlist, s );

        s2 = s->next;
        free( s->str );
        free( s );
        s = s2;
    }
}

void
dumpstrlist(strlist *aStrlist)
{
    for (; aStrlist; aStrlist = aStrlist->next) { 
	printf("%s\n", aStrlist->str); 
    }
}

/****f* strutl/noCaseStrcmp
 * NAME
 *   noCaseStrcmp --
 * SYNOPSIS
 *   int noCaseStrcmp(char *s, char *t)
 * FUNCTION
 *   Compare two strings without paying no attention to the case of
 *   the letters.
 * RESULT
 *    0  s == t
 *   -1  s < t
 *    1  s > t
 * SOURCE
 */

int
noCaseStrcmp(const char *s, const char *t)
{
  for (; tolower(*s) == tolower(*t); s++, t++)
    if (*s == '\0')
      return 0;
  return (int) (tolower(*s) - tolower(*t));
}

/*********/

/****f* strutl/noCaseStrncmp
 * NAME
 *   noCaseStrncmp --
 * SYNOPSIS
 *   int noCaseStrncmp(char *, char *, int)
 *   result = noCaseStrncmp(s, t, n)
 * FUNCTION
 *   Compare two strings without paying no attention to the case of
 *   the letters, but compare no more than n characters.
 * RESULT
 *    0  s == t
 *   -1  s < t
 *    1  s > t
 * SOURCE
 */

int
noCaseStrncmp(const char *s, const char *t, int n)
{
  for (n--; (tolower(*s) == tolower(*t)) && (n > 0); s++, t++, n--)
    if (*s == '\0')
      return 0;
  return (int) (tolower(*s) - tolower(*t));
}

/***********/



char* strlwr(char* str) {
  char* ptr = str;

  while (*ptr) {
    if (isupper(*ptr))
      *ptr = tolower(*ptr);
    ptr++;
  }

  return str;
}
