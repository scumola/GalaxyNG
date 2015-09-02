/*  $Id: are.c,v 1.19 2005/01/21 23:40:29 christhecat Exp $ */


/****h* Galaxy/ARE
 * NAME
 *   ARE - automatic registration engine
 * SYNOPSIS
 *   are <game name> <player limit> <total planet size>
 *       <maximum planet size> <max. number of planets> 
 *            <  <email> 
 * FUNCTION
 *   Automate the registration process for a game.
 *   The program processes email send in by players wishing to join
 *   a particular game. 
 *
 *   In addition players can specify their home planet sizes
 *   by including the line: 
 *
 *    #PLANETS <size of planet 1> <size of planet 2> ... <size of planet n>
 *  
 *   in the body of their email.
 *
 * INPUTS
 *   game name              --  
 *   player limit           -- maximum number of players that can enroll.
 *   total planet size      -- limit on the sum of all planet sizes.
 *   maximum planet size    -- limit on the size of each planet.
 *   max. number of planets -- limit on the number of planet that can 
 *                             be specified.
 *   email                  -- that what formail generates.
 * RESULTS
 *   A confirmation message is send to the players.
 *   Three kind of messages can be generated:
 *   (1)  You are accepted.
 *   (2)  The game is full.
 *   (3)  You made an error in your #PLANETS line.
 *
 *   The email address and planet sizes are written to two files:
 *   (1) $GALAXYHOME/<game name>.players  for players that were accepted
 *   (2) $GALAXYHOME/<game name>.standby  for players that were rejected.
 *   The contents of these files can be pasted into your .glx file.
 *   $GALAXYHOME is either the value you gave it, or if it is undefined, 
 *   $HOME/Games.
 *
 *
 *   The programs does the following checks on the #PLANETS line
 *   (1) Whether the size of a single planet exceeds the limit set.
 *   (2) Whether too many planets are specified.
 *   (3) Whether the sum of all sizes exceeds the limit set.
 *   (4) Whether the sum of all sizes is too small. This will be most
 *       likely be because the player made an error. If this error then
 *       shows up in the turn 0 the player is most likely to drop out.
 *
 *   If any of these errors occur the player is notified and has
 *   to retransmit the enroll request.
 *
 * EXAMPLE
 *
 *   The entry in the .procmailrc file to accept 25 players for the
 *   game Jangi.  Players can specify a maximum of 5 planets, of which
 *   the total size should now exceed 2000.0. Each single planet
 *   should be no larger that 1000.0
 *
 *     GALAXYNGHOME=$HOME/Games
 *
 *     :0 rw :jangi.lock
 *     * ^Subject.*Join Jangi
 *     |/usr/bin/formail -rkbt -s    {continued on the next line}
 *          $GALAXYNGHOME/are Jangi 25 2000 1000 5 | /usr/sbin/sendmail -t 
 *
 *   This assumes the program is installed in $GALAXYNGHOME
 *
 *   Players can now enroll in the game sending you an email with 
 *   the subject:
 *
 *      "Join Jangi"
 *******
 */


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define eq ==

#define TRUE  1
#define FALSE 0

#define LINE_BUFFER_SIZE 1024
char lineBuffer[LINE_BUFFER_SIZE+2] ;


/****** ARE/UserDefines
 * NAME
 *   UserDefines -- 
 * FUNCTION
 *   Please change these to reflect your own system's settings.
 * SOURCE
 */

#define FROMHEADER "From: Registration_Engine <youraddress@yourdomain>\n"
#define SUBJECT    "Subject: Enrollment Reply\n"
#define REPLYTO    "Reply-To: yourhomeaddress@localhost\n"
#define CCHEADER   "Cc: someotheraddress@localhost\n"

/*******/


/****v* GalaxyNG/galaxynghome
 * NAME
 *   galaxynghome -- path to all data files
 * SOURCE
 */

char *galaxynghome = NULL;

/*****/


#define T_PLAYER  1
#define T_STANDBY 2



int countPlayersRegistered(char *gameName);
int registerPlayer(char *address, char *planets, char *gameName, int type);
void playerMessage(char *gameName, char *address, char *planets);
void standbyMessage(char *gameName);
int getPlanetSizes(FILE *orders, char **planets, double totalPlanetSize,
		   int maxNumberOfPlanets, double maxPlanetSize);
char *getReturnAddress(FILE *orders);
char           *getstr(char *s);
int noCaseStrncmp(char *s, char *t, int n);
void badPlanetMessage(char *planets);



int main(int argc, char **argv)
{
  int errorCode;
  char *value;

  if ((value = getenv("GALAXYNGHOME"))) {
    galaxynghome = strdup(value);
  } else if ((value = getenv("HOME"))) {
    sprintf(lineBuffer, "%s/Games", value);
    galaxynghome = strdup(lineBuffer);
  } else {
    galaxynghome = strdup("/please/set/your/HOME/or/GALAXYNGHOME/variable");
  }

  errorCode = EXIT_FAILURE;
  if (argc eq 6) {
    int maxNumberOfPlayers, curNumberOfPlayers, maxNumberOfPlanets;
    double totalPlanetSize, maxPlanetSize;
    int ret;
    char sys_string[2000];
    
    curNumberOfPlayers   = countPlayersRegistered(argv[1]);
    maxNumberOfPlayers   = atoi(argv[2]);
    totalPlanetSize      = atof(argv[3]);
    maxPlanetSize        = atof(argv[4]);
    maxNumberOfPlanets   = atoi(argv[5]);
    if (curNumberOfPlayers >= 0) {
      char *address; 
      if ((address = getReturnAddress(stdin))) {
	char *planets;

	printf("To: %s\n%s%s%s%s", address, FROMHEADER, CCHEADER, 
	       SUBJECT, REPLYTO);
	printf(
	  "\n\nMessage from A.R.E (Automatic Registration Engine)\n"
	  "\n"
	  "\nGreetings,\n");
	sprintf(sys_string,"/bin/grep %s %s/%s.players > /dev/null",address,galaxynghome,argv[1]);
	ret = system(sys_string);
	if (ret==0) {
	      printf("\nERROR:\n"
		     "You are already registered for this game\n");
	      exit(0);
	}
	if (getPlanetSizes(stdin, &planets, totalPlanetSize, maxNumberOfPlanets, maxPlanetSize)) {
	  errorCode =  registerPlayer(
	    address, planets, argv[1], 
	    (curNumberOfPlayers < maxNumberOfPlayers) ? T_PLAYER : T_STANDBY);
	  if (errorCode == EXIT_SUCCESS) { 
	    if (curNumberOfPlayers < maxNumberOfPlayers) {
	      playerMessage(argv[1], address, planets);
	    } else {
	      standbyMessage(argv[1]);
	    }
	  }
	} else { /* Player made a mistake in planet sizes */
	  badPlanetMessage(planets);
	}
	if (planets) free(planets);
	if (address) free(address);
      } else {
	fprintf(stderr, 
		"ARE: Can't determine return address.\n");
      }
    } else {
      fprintf(stderr, 
	      "ARE: Can't determine the number players that enrolled.\n");
    }
  } else {
    fprintf(stderr, 
	    "ARE: Incorrect number of parameters.\n"
	    "ARE: Correct usage: are <gamename> <limit>\n"
	    "     <total planet size> <maximum planet size>\n"
	    "     <maximum number of planets>\n"
      );
  }
  return errorCode;
}


int getPlanetSizes(FILE *orders, char **planets, double totalPlanetSize,
		   int maxNumberOfPlanets, double maxPlanetSize)
{
  char *isRead;
  char *c, *ns;
  double totalSize = 0.0;
  int numberOfPlanets = 0, result;
  double size;
  int tooBig;

  result = TRUE;   /* All is OK */

  for (isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, orders);
       isRead;
       isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, orders)) {
    if (noCaseStrncmp("#PLANETS", lineBuffer, 3) == 0) 
      break;
  }
  printf("\n");

  if (isRead != NULL) {
    getstr(lineBuffer);
    tooBig = FALSE;
    printf("\nProcessing your planet specifications.\n");
    for (ns = getstr(0);
	 ns[0] != '\0';
	 ns = getstr(0)) {
       size = atof(ns);
      totalSize += size;
      if (size > maxPlanetSize) { tooBig = TRUE; }
      numberOfPlanets++;
    }
    if (tooBig) {
      printf("\nERROR:\n"
	     "One or more of your planets are larger than the\n"
	     "limit of %.1f set by the game master.\n\n", maxPlanetSize);
      result = FALSE;
    }
    if (numberOfPlanets > maxNumberOfPlanets) {
      printf("\nERROR:\n"
	     "You specified %d planets, only %d planets are allowed.\n\n",
	     numberOfPlanets, maxNumberOfPlanets);
      result = FALSE;
    }
    if (totalSize > totalPlanetSize) {
      printf("\nERROR:\n"
	     "The sum of the sizes of all your planets is %9.2f.\n" 
             "This exceeds the limit of %9.2f set by your game master.\n"
	     "Please reduce the size of some of your planets.\n\n",
	     totalSize, totalPlanetSize);
      result = FALSE;
    }
    if (totalSize < totalPlanetSize - 25.0) {
      printf("\nWARNING:\n"
	     "The sum of the sizes of your planets is:  %9.2f\n" 
	     "The maximum limit set by your game master is: %9.2f\n"
	     "This means that you can increase the size of one (or more) of\n"
	     "your planets by %.2f!\n"
	     "My guess is that you want to edit and retransmit your\n"
	     "specification and make use of this additional space.\n\n",
	     totalSize, totalPlanetSize, totalPlanetSize - totalSize);
      result = FALSE;
    }
    *planets = strdup(lineBuffer + 8);
    if (result) {
      printf("All planets are OK.\n\n");
    }
  } else {
    *planets = NULL;  /* no planet sizes were specified */
  }
  return result;
}



/****** ARE/registerPlayer
 * NAME
 *   registerPlayer --
 * FUNCTION
 *   
 * RESULT
 *   errorCode
 *      EXIT_SUCCESS  All is OK
 *      EXIT_FAILURE  Something went wrong
 *   Address of the player that registered is appended to
 *   either <gameName>.players or <gameName>.standby
 ******
 */

int registerPlayer(char *address, char *planets, char *gameName, int type)    
{
  FILE *registerFile;
  char *readit;
  int errorCode;

  errorCode = EXIT_SUCCESS;

  if (type eq T_PLAYER) {
    sprintf(lineBuffer, "%s/%s.players", galaxynghome, gameName);
  } else {
    sprintf(lineBuffer, "%s/%s.standby", galaxynghome, gameName);
  }
  
  if ((registerFile = fopen(lineBuffer, "a"))) {
    fprintf(registerFile, "start_player\n");
    fprintf(registerFile, "email %s\n", address);
    fprintf(registerFile, "end_player"); 
    if (planets) {
      fprintf(registerFile, " %s", planets);
    } else {
      fprintf(registerFile, "\n");
    }
    fclose(registerFile);
  } else {
    fprintf(stderr, "ARE: can't open %s\n", lineBuffer);
    errorCode = EXIT_FAILURE;
  }
  return errorCode;
}


/****** ARE/countPlayersRegistered
 * NAME
 *   countPlayersRegistered
 * SYNOPSIS
 *   int     countPlayersRegistered(char *) 
 *   count = countPlayersRegistered(gameName)
 * FUNCTION
 *   Count the number of players that registered sofar for a game.
 *   This is done by counting the number of lines in the 
 *   <gamename>.players file.  
 * INPUTS
 *   gameName -- Name of the game the player is enrolling in.
 * RESULT
 *   count
 *      < 0  -- something went wrong, registration file could not be opened.
 *     >= 0  -- number of players that registered sofar 
 * SEE ALSO
 *   GAMEPATH
 ********
 */

int countPlayersRegistered(char *gameName) 
{
  FILE *registerFile;
  char *fileName;
  int count;

  sprintf(lineBuffer, "%s/%s.players", galaxynghome, gameName);
  fileName = strdup(lineBuffer);

  if ((registerFile = fopen(fileName, "r"))) {
    char *readIt;
    count = 0;
    for(readIt = fgets (lineBuffer, LINE_BUFFER_SIZE, registerFile);
	readIt;
	readIt = fgets (lineBuffer, LINE_BUFFER_SIZE, registerFile)) {
      count++;
    }
    fclose(registerFile);
    count /= 3; /* count one player per 3 lines */
  } else { /* Maybe the file does not exist yet */
    if (registerFile = fopen(fileName, "w")) {
      count = 0;
      fclose(registerFile);
    } else {
      fprintf(stderr, "ARE: can't open %s\n", fileName); 
      count = -1;
    }
  }
  free(fileName);
  return count;
}



void badPlanetMessage(char *planets)
{
  printf(
"\nYou tried to enroll using the following #PLANETS specification\n"
	 "  %s\n", planets);
  printf(
"This line contains one or more errors/warnings as described above.\n"
"You are therefore not yet enrolled in the game.\n"
"Please correct the mistake and retransmit the enroll request.\n");
}


/****** ARE/playerMessage
 * NAME
 *   playerMessage -- write the "accepted" message
 * FUNCTION
 *   Prints the "You Are Accepted" message to stdout. 
 ******
 */

void playerMessage(char *gameName, char *address, char *planets)
{
  printf(
    "I am happy to report that you have been accepted for the game %s.\n"
    "Your address:\n"
    "   %s\n"
    "has been added to the list.\n"  
    " \n",
    gameName, address);
  if (planets) {
    printf("Your planet sizes will be:\n  %s\n\n", planets);
  }
  printf(
  "Please note that I am a program. My only function is to register\n"
  "your name. I am not smart enough to answer any questions that you\n"
  "might have put in your mail.\n"
  "\n"
  "For more information about GalaxyNG, have a look at\n"
  "  http://galaxyng.sourceforge.net/\n"
  "Now is a good time to read the manual:\n"
  "  http://galaxyng.sourceforge.net/manual.php\n"
  "and the FAQ: \n"
"   http://galaxyng.sourceforge.net/faq.php\n\n"
  "If you then still have questions, join the Galaxy mailing list:\n"
  "  \n"
  "\n"
  "Happy Hunting in Galaxy %s!\n\n",
  gameName);
}


void standbyMessage(char *gameName)
{
printf(
"\nIt's my sad duty to report that the game %s is already full.\n"
"\n\n"
"Please note that I am a program, and that my only function is to\n"
"register your name.  I am not smart enough to answer any\n"
"questions that you might have put in your email, so please do\n"
"not get angry because they are not answered.\n"
"\n"
"Have Fun!\n\n",
gameName);
}



/****f* ARE/getstr 
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
 *   almost every where in the program.
 * SOURCE
 */

char           *
getstr(char *s)
{
  static char    *s1;
  static char     buf[256];
  int             i, j;

  if (s) s1 = s; 
  /* first invocation of this function, for an order line. 
     Each next times, for the same order line, s1 will "progress" 
     by a word to the right */
  assert (s1 != NULL);

  i = 0;
  for (; *s1 && isspace(*s1); s1++);	/* skips spaces */

  if (*s1 eq '"') {
/* Players can enclose name (ie : including spaces) with double quotes */
    for (s1++; *s1 && *s1 != '"';) {	
      buf[i] = isspace(*s1) ? '_' : *s1;
      s1++;
      i++;
      assert(i < 256);	/* abort execution if s1 too long */
    }
  } else if (*s1 eq '{') {		
    for (s1++; *s1 && *s1 != '}';) {
      buf[i] = *s1;
      s1++;
      i++;
      assert(i < 256);	/* abort execution if s1 too long */
    }
  } else {
    if (*s1 != ';') {		/* otherwise, it's a comment */
      for (; *s1 && !isspace(*s1) && *s1 != ',';) {
	/* space or ',' can be  used as separators, */
	buf[i] = *s1;
	s1++;
	i++;
	assert(i < 256);  /* abort execution if s1 too long */
      }
    }
  }

  buf[i] = '\0';
  if (*s1) s1++;  /* Skip ',' or space */
  /* CB, 19980922. Remove ";" and "<" from names (planets,  ships...), 
     to protect machine report from corruption. Don't break messages 
     and comments. */
  i = 0;
  j = 0;	
  while (buf[j] && j < 256) {
    if (buf[j] != ';' && buf[j] != '<') {
      buf[i] = buf[j];
      i++;
    }
    j++;
  }
  if (i) buf[i] = '\0';	

  return buf;
}

/***********/



/****f* ARE/getReturnAddress
 * NAME
 *   getReturnAddress
 * FUNCTION
 *   Extract the return address from a players email.
 ******
 */

char *
getReturnAddress(FILE *orders)
{
  char *isRead;
  char *c;

  for (isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, orders);
       isRead;
       isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, orders)) {
    if (noCaseStrncmp("To:", lineBuffer, 3) == 0) 
      break;
  }
  assert(isRead != NULL);
  for (c = lineBuffer; *c; c++) {
    if (*c == '\n') *c = '\0';
  }
  return strdup(lineBuffer + 3) ;
}


/****f* ARE/noCaseStrncmp
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
noCaseStrncmp(char *s, char *t, int n)
{
  for (n--; (tolower(*s) == tolower(*t)) && (n > 0); s++, t++, n--)
    if (*s == '\0')
      return 0;
  return (int) (tolower(*s) - tolower(*t));
}

/***********/
