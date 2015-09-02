#include <assert.h>
#include "loadgame.h"
#include <stdlib.h>
#include <string.h>

char           *vloadgame =
    "$Id: loadgame.c,v 1.25 2005/01/18 01:39:06 madcoder42 Exp $";

/****h* GalaxyNG/LoadGame
 * NAME
 *   LoadGame -- functions to read a game from file.
 *****
 */

/****i* LoadGame/readFloat
 * NAME
 *   readFloat
 *****
 */

double
readFloat(void)
{
  double          d;

  if (*lineBuffer == '\n')
    d = 0.0;
  else
    d = atof(lineBuffer);
  return d;
}

/****i* LoadGame/readString
 * NAME
 *   readString --
 ****
 */

char           *
readString(void)
{
  char           *c;

  for (c = lineBuffer; *c; c++)
    if (*c == '\n')
      *c = '\0';
  return lineBuffer;
}


/****i* LoadGame/readInt
 * NAME
 *   readInt --
 *****
 */

int
readInt(void)
{
  int             i;

  if (*lineBuffer == '\n')
    i = 0;
  else
    i = atoi(lineBuffer);
  return i;
}

/****i* LoadGame/readLong
 * NAME
 *   readLong --
 ******
 */

int
readLong(void)
{
  long            l;

  if (*lineBuffer == '\n')
    l = 0;
  else
    l = strtol(lineBuffer, (char **) NULL, 10);
  return l;
}

/****i* LoadGame/readPointer
 * NAME
 *   readPointer --
 *****
 */

list           *
readPointer(void *base)
{
  int             i;
  list           *p;

  if (*lineBuffer == '\n')
    i = 0;
  else
    i = atoi(lineBuffer);
  if (i) {
    int             nr;

    for (nr = 1, p = base; p && (nr != i); p = p->next, nr++);
  }
  else
    p = NULL;

  return p;
}

/****i* LoadGame/getReadFloat
 * NAME
 *   getReadFloat --
 *****
 */

double
getReadFloat(void)
{
  double          d;

  getLine(turnFile);
  if (*lineBuffer == '\n')
    d = 0.0;
  else
    d = atof(lineBuffer);
  return d;
}

/****i* LoadGame/getReadString
 * NAME
 *   getReadString
 *****
 */

char           *
getReadString(void)
{
  char           *c;

  getLine(turnFile);
  for (c = lineBuffer; *c; c++)
    if (*c == '\n')
      *c = '\0';
  return lineBuffer;
}

/****i* LoadGame/getReadInt
 * NAME
 *   getReadInt --
 *****
 */

int
getReadInt(void)
{
  int             i;

  getLine(turnFile);
  if (*lineBuffer == '\n')
    i = 0;
  else
    i = atoi(lineBuffer);
  return i;
}

/****i* LoadGame/getReadLong
 * NAME
 *   getReadLong --
 *****
 */

int
getReadLong(void)
{
  long            l;

  getLine(turnFile);
  if (*lineBuffer == '\n')
    l = 0;
  else
    l = strtol(lineBuffer, (char **) NULL, 10);
  return l;
}

/****i* LoadGame/getReadPointer
 * NAME
 *   getReadPointer
 *****
 */

list           *
getReadPointer(void *base)
{
  int             i;
  list           *p;

  getLine(turnFile);
  if (*lineBuffer == '\n')
    i = 0;
  else
    i = atoi(lineBuffer);
  if (i) {
    int             nr;

    for (nr = 1, p = base; p && (nr != i); p = p->next, nr++);
  }
  else
    p = NULL;

  return p;
}

/****i* LoadGame/getReadVersion
 * NAME
 *   getReadVersion -- read server version number.
 * BUGS
 *   Fails if version or revisions is larger than 9.
 *****
 */

void
getReadVersion(int *version, int *revision)
{
  char           *c;

  getLine(turnFile);
  c = strstr(lineBuffer, "release");
  c += 8;
  *version = atoi(c);
  c += 2;
  *revision = atoi(c);
}



/****f* LoadGame/loadgame
 * NAME
 *   loadgame -- load game from disk.
 * FUNCTION
 *   Reads the game structure from disk.
 * NOTES
 *   Horrible use of feof().
 *   Should be split up into a number of function.
 *   Should use createFileName().
 * BUGS
 *   The speed of fleets is not stored in the turn file. So fleet
 *   speeds are incorrect until the fleetphase() is executed.
 *****/

game*
loadgame(char *gameName, int whichTurn)
{
  FILE           *nextTurnFile;
  player         *P;
  int             turn;
  game           *aGame;
  int             turnversion;
  int             turnrevision;

  pdebug(DFULL, "Load Game\n");

  aGame = allocStruct(game);

  setName(aGame, gameName);

  if (whichTurn == LG_CURRENT_TURN) {
    sprintf(lineBuffer, "%s/data/%s/next_turn", galaxynghome, aGame->name);
    nextTurnFile = GOS_fopen(lineBuffer, "r");
    if (nextTurnFile eq NULL)
      return FALSE;
    fscanf(nextTurnFile, "%d", &turn);
    turn--;
    fclose(nextTurnFile);
  }
  else {
    turn = whichTurn;
  }
  
  sprintf(lineBuffer, "%s/data/%s/%d.new", galaxynghome, aGame->name,
	  turn);
  turnFile = fopen(lineBuffer, "r");
  aGame->turn = turn;
  
  if (turnFile == NULL)
    return FALSE;
  
  getReadVersion(&turnversion, &turnrevision);
  
  getReadInt();
  aGame->galaxysize = getReadFloat();
  
  pdebug(DFULL, "Load Game : Game Options\n");  /* CB-20010407 */
  for (getLine(turnFile);
       !feof(turnFile) && !strstr(lineBuffer, "@EGameOptions");
       getLine(turnFile)) {
    aGame->gameOptions.gameOptions = getReadInt();
    aGame->gameOptions.initial_drive = getReadFloat();
    if (aGame->gameOptions.initial_drive <= 0) {
      aGame->gameOptions.galactic_peace =
	(int)(aGame->gameOptions.initial_drive * -1);
      aGame->gameOptions.initial_drive = getReadFloat();
    }
    aGame->gameOptions.initial_weapons = getReadFloat();
    aGame->gameOptions.initial_shields = getReadFloat();
    aGame->gameOptions.initial_cargo = getReadFloat();
  }
  
  pdebug(DFULL, "Load Game : Players\n");
  
  getLine(turnFile);
  for (getLine(turnFile);
       !feof(turnFile) && !strstr(lineBuffer, "@EPlayers");
       getLine(turnFile)) {
    P = allocStruct(player);
    
    assert(P != NULL);
    P->fleetnames = NULL;
    P->shiptypes = NULL;
    P->mistakes = NULL;
    P->messages = NULL;
    P->orders = NULL;
    P->allies = NULL;
    P->groups = NULL;
    P->claimed_planets = NULL;
    
    setName(P, readString());
    P->addr = strdup(getReadString());
    P->pswd = strdup(getReadString());
    
    P->drive = getReadFloat();
    P->weapons = getReadFloat();
    P->shields = getReadFloat();
    P->cargo = getReadFloat();
    P->mx = getReadFloat();
    P->my = getReadFloat();
    P->msize = getReadFloat();
    P->realName = strdup(getReadString());
    P->team = getReadInt();
    P->unused3 = getReadInt();
    P->unused4 = getReadInt();
    P->unused5 = getReadInt();
    P->masslost = getReadFloat();
    P->massproduced = getReadFloat();
    P->lastorders = getReadInt();
    P->flags = getReadLong();
    
    /* temporarily turn off all xml reports */
    P->flags &= ~F_XMLREPORT;
    
    pdebug(DFULL, "Load Game : Ship Types\n");
    
    getLine(turnFile);
    for (getLine(turnFile);
	 !feof(turnFile) && !strstr(lineBuffer, "@EShipTypes");
	 getLine(turnFile)) {
      shiptype       *aShipType;
      
      aShipType = allocStruct(shiptype);
      
      setName(aShipType, readString());
      aShipType->drive = getReadFloat();
      aShipType->attacks = getReadInt();
      aShipType->weapons = getReadFloat();
      aShipType->shields = getReadFloat();
      aShipType->cargo = getReadFloat();
      addList(&P->shiptypes, aShipType);
    }
    
    getLine(turnFile);
    for (getLine(turnFile);
	 !feof(turnFile) && !strstr(lineBuffer, "@EFleets");
	 getLine(turnFile)) {
      fleetname      *aFleetName;
      
      aFleetName = allocStruct(fleetname);
      
      setName(aFleetName, readString());
      aFleetName->fleetspeed = 0.0;     /* Cause it is not stored */
      addList(&P->fleetnames, aFleetName);
    }
    addList(&(aGame->players), P);
  }
  
  pdebug(DFULL, "Load Game : Allies\n");
  
  for (P = aGame->players; P; P = P->next) {
    getLine(turnFile);
    for (getLine(turnFile);
	 !feof(turnFile) && !strstr(lineBuffer, "@EAllies");
	 getLine(turnFile)) {
      alliance       *anAlliance;
      
      anAlliance = allocStruct(alliance);
      
      anAlliance->who = (player *) readPointer((list *) aGame->players);
      assert(anAlliance->who != NULL);
      addList(&P->allies, anAlliance);
    }
  }
  
  pdebug(DFULL, "Load Game : Planets\n");
  
  getLine(turnFile);
  for (getLine(turnFile);
       !feof(turnFile) && !strstr(lineBuffer, "@EPlanets");
       getLine(turnFile)) {
    planet         *aPlanet;
    int             temp;
    
    aPlanet = allocStruct(planet);
    
    setName(aPlanet, readString());
    aPlanet->owner = (player *) getReadPointer((list *) aGame->players);
    aPlanet->x = getReadFloat();
    aPlanet->y = getReadFloat();
    aPlanet->size = getReadFloat();
    aPlanet->resources = getReadFloat();
    aPlanet->pop = getReadFloat();
    aPlanet->ind = getReadFloat();
    aPlanet->producing = getReadInt();
    temp = getReadInt();
    if (temp) {
      assert(aPlanet->owner != NULL);
      aPlanet->producingshiptype =
	(shiptype *) readPointer((list *) aPlanet->owner->shiptypes);
    }
    else
      aPlanet->producingshiptype = NULL;
    aPlanet->cap = getReadFloat();
    aPlanet->mat = getReadFloat();
    aPlanet->col = getReadFloat();
    aPlanet->inprogress = getReadFloat();
    aPlanet->spent = getReadFloat();
    aPlanet->flags = 0;
    addList(&(aGame->planets), aPlanet);
  }
  
  pdebug(DFULL, "Load Game : Routes\n");
  
  getLine(turnFile);
  for (getLine(turnFile);
       !feof(turnFile) && !strstr(lineBuffer, "@ERoutes");) {
    planet         *aPlanet;
    
    for (aPlanet = aGame->planets; aPlanet; aPlanet = aPlanet->next) {
      int             routeIndex;
      
      for (routeIndex = 0; routeIndex < MAXCARGO; routeIndex++) {
	aPlanet->routes[routeIndex] =
	  (planet *) readPointer((list *) aGame->planets);
	getLine(turnFile);
      }
    }
  }
  
  pdebug(DFULL, "Load Game : Groups\n");
  
  for (P = aGame->players; P; P = P->next) {
    group          *aGroup;
    
    getLine(turnFile);
    for (getLine(turnFile);
	 !feof(turnFile) && !strstr(lineBuffer, "@EPGroups");
	 getLine(turnFile)) {
      aGroup = allocStruct(group);
      aGroup->next = NULL;
      aGroup->type = (shiptype *) readPointer((list *) P->shiptypes);
      assert(aGroup->type != NULL);
      aGroup->number = getReadInt();
      aGroup->name = (char*)malloc(8);
      sprintf(aGroup->name, "%d", aGroup->number);
      aGroup->drive = getReadFloat();
      aGroup->weapons = getReadFloat();
      aGroup->shields = getReadFloat();
      aGroup->cargo = getReadFloat();
      aGroup->loadtype = getReadInt();
      aGroup->load = getReadFloat();
      aGroup->from = (planet *) getReadPointer((list *) aGame->planets);
      aGroup->where = (planet *) getReadPointer((list *) aGame->planets);
      aGroup->dist = getReadFloat();
      aGroup->ships = getReadInt();
      aGroup->thefleet =
	(fleetname *) getReadPointer((list *) (P->fleetnames));
      addList(&P->groups, aGroup);
      
      aGroup->left = aGroup->ships;
      aGroup->attack = 0.0;
      aGroup->defense = 0.0;
      aGroup->location = NULL;
    }
  }
  loadRanTab(turnFile);
  fclose(turnFile);
  return aGame;
}



