#include "galaxy.h"

char            map[MAPWIDTH][MAPHEIGHT];
char            buf[LINE_BUFFER_SIZE];

/****v* GalaxyNG/lineBuffer
 * NAME
 *   lineBuffer -- global line buffer.
 * NOTES
 *   2 is there to prevent buffer overflows.
 * SOURCE
 */

char            lineBuffer[2 * LINE_BUFFER_SIZE];

/******/


/****v* GalaxyNG/productname
 * NAME
 *   productname -- things a planet can produce.
 * SOURCE
 */

char           *productname[] = {
  "CAP", "MAT", 0, "Drive", "Weapons", "Shields", "Cargo",
};

/******/

int             nbrProducts = sizeof(productname) / sizeof(char *);

FILE           *turnFile = NULL;


/******/


/****v* GalaxyNG/galaxynghome
 * NAME
 *   galaxynghome -- path to all data files
 * SOURCE
 */

char           *galaxynghome = NULL;

/*****/

/****v* GalaxyNG/tempdir
 * NAME
 *   tempdir -- path to the directory used to store temporary files.
 * SOURCE
 */

char           *tempdir = NULL;

/*****/


/****v* GalaxyNG/options
 * NAME 
 *   options -- options available to players. 
 * FUNCTION
 *   Associative array to look-up option flag by option names. 
 * SOURCE
 */

struct option   options[] = {
  {"anonymous", F_ANONYMOUS},
  {"autoUnload", F_AUTOUNLOAD},
  {"prodTable", F_PRODTABLE},
  {"sortGroups", F_SORTGROUPS},
  {"groupForeCast", F_GROUPFORECAST},
  {"planetForeCast", F_PLANETFORECAST},
  {"shipTypeForecast", F_SHIPTYPEFORECAST},
  {"routesForecast", F_ROUTESFORECAST},
  {"compress", F_COMPRESS},
  {"gplus", F_GPLUS},
  {"machineReport", F_MACHINEREPORT},   /* CB 1998 - Experimental */
  {"battleProtocol", F_BATTLEPROTOCOL},
  {"xmlReport", F_XMLREPORT},   /* KDW 2003 */
  {"txtReport", F_TXTREPORT},
  {NULL, 0}
};


char* string_mail_subject = "subject:"; /* Dutch: onderwerp: */
char* string_mail_to = "to:";   /* Dutch: aan: */
char* string_mail_from = "from:";       /* Dutch: van: */

/**********/
