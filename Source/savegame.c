#include <assert.h>
#include <string.h>

#include "savegame.h"

/****h* GalaxyNG/SaveGame
 * NAME
 *   SaveGame -- a number of functions to save a turn to file.
 * FUNCTION
 *   The whole game structure is written to file in a structured way.
 *   All data is written in ASCII format. Each value is written
 *   on a separate line. And empty line indicates a zero value.
 *   Pointers are converted to index number. This is possible because
 *   all pointers that are used in the game structure are pointers
 *   to elements in a list.
 *
 ****
 * ADDED
 *   The option to write out a .png file of the current map
 *****
 */

char           *vsavegame =
    "$Id: savegame.c,v 1.12 2005/01/18 01:32:36 madcoder42 Exp $";

/****i* SaveGame/writeString
 * NAME
 *   writeString -- write a string to file.
 *****
 */

void
writeString(char *s)
{
  fprintf(turnFile, "%s\n", s);
}

/****i* SaveGame/writeFloat
 * NAME
 *   writeFloat -- write a float to file.
 *****
 */

void
writeFloat(double d)
{
  if (d != 0)
    fprintf(turnFile, "%g\n", d);
  else
    fprintf(turnFile, "\n");
}

/****i* SaveGame/writeInt
 * NAME
 *   writeInt
 *****
 */

void
writeInt(int i)
{
  if (i)
    fprintf(turnFile, "%d\n", i);
  else
    fprintf(turnFile, "\n");
}

/****i* SaveGame/writeLong
 * NAME
 *   writeLong
 *****
 */

void
writeLong(long l)
{
  if (l)
    fprintf(turnFile, "%ld\n", l);
  else
    fprintf(turnFile, "\n");
}


/****i* SaveGame/savegame
 * NAME
 *   savegame -- save a turn to file.
 * RESULTS
 *   A file with the name <turn number>.new
 *   (optional) a file with the name <turn number>.png
 * NOTES
 *   The extension .new is there cause of historical reasons.
 * BUGS
 *   Uses Fopen().
 * SOURCE
 */

void
savegame(game *aGame)
{
  player* P;
  planet* p;

  sprintf(buf, "%s/data/%s/next_turn", galaxynghome, aGame->name);
  turnFile = fopen(buf, "w");
  assert(turnFile != NULL);
  fprintf(turnFile, "%d", aGame->turn + 1);
  fclose(turnFile);

  sprintf(buf, "%s/data/%s/%d.new", galaxynghome, aGame->name,
	  aGame->turn);
  turnFile = fopen(buf, "w");
  assert(turnFile != NULL);

  writeString(vcid);            /* Galaxy version */
  writeInt(aGame->turn);
  writeFloat(aGame->galaxysize);

  writeString("@GameOptions");  /* CB-20010407 */
  writeFloat(aGame->gameOptions.gameOptions);
  writeInt(aGame->gameOptions.galactic_peace*-1);
  writeFloat(aGame->gameOptions.initial_drive);
  writeFloat(aGame->gameOptions.initial_weapons);
  writeFloat(aGame->gameOptions.initial_shields);
  writeFloat(aGame->gameOptions.initial_cargo);
  writeString("@EGameOptions");
  
  writeString("@Players");
  
  for (P = aGame->players; P; P = P->next) {
    shiptype       *aShipType;
    fleetname      *aFleetName;
    
    writeString(P->name);
    writeString(P->addr);
    writeString(P->pswd);
    writeFloat(P->drive);
    writeFloat(P->weapons);
    writeFloat(P->shields);
    writeFloat(P->cargo);
    writeFloat(P->mx);
    writeFloat(P->my);
    writeFloat(P->msize);
    
    writeString(P->realName);
    writeInt(P->team);
    writeInt(P->unused3);
    writeInt(P->unused4);
    writeInt(P->unused5);
    writeFloat(P->masslost);
    writeFloat(P->massproduced);
    writeInt(P->lastorders);
    writeLong(P->flags);
    
    writeString("@ShipTypes");
    for (aShipType = P->shiptypes; aShipType; aShipType = aShipType->next) {
      writeString(aShipType->name);
      writeFloat(aShipType->drive);
      writeInt(aShipType->attacks);
      writeFloat(aShipType->weapons);
      writeFloat(aShipType->shields);
      writeFloat(aShipType->cargo);
    }
    writeString("@EShipTypes");
    
    writeString("@Fleets");
    for (aFleetName = P->fleetnames;
	 aFleetName; aFleetName = aFleetName->next) {
      writeString(aFleetName->name);
    }
    writeString("@EFleets");
  }
  writeString("@EPlayers");
  
  for (P = aGame->players; P; P = P->next) {
    alliance       *anAlliance;
    
    writeString("@Allies");
    for (anAlliance = P->allies; anAlliance; anAlliance = anAlliance->next) {
      writeInt(ptonum(aGame->players, anAlliance->who));
    }
    writeString("@EAllies");
  }
  
  writeString("@Planets");
  
  for (p = aGame->planets; p; p = p->next) {
    player         *owner;
    
    writeString(p->name);
    if ((owner = p->owner)) {
      writeInt(ptonum(aGame->players, owner));
    }
    else {
      writeInt(0);
    }
    writeFloat(p->x);
    writeFloat(p->y);
    writeFloat(p->size);
    writeFloat(p->resources);
    writeFloat(p->pop);
    writeFloat(p->ind);
    writeInt(p->producing);
    if (p->producingshiptype) {
      assert(owner != NULL);
      writeInt(ptonum(owner->shiptypes, p->producingshiptype));
    }
    else {
      writeInt(0);
    }
    writeFloat(p->cap);
    writeFloat(p->mat);
    writeFloat(p->col);
    writeFloat(p->inprogress);
    writeFloat(p->spent);       /* Should be removed */
  }
  writeString("@EPlanets");
  
  writeString("@Routes");
  for (p = aGame->planets; p; p = p->next) {
    int             routeIndex;
    
    for (routeIndex = 0; routeIndex < MAXCARGO; routeIndex++) {
      if (p->routes[routeIndex])
	writeInt(ptonum(aGame->planets, p->routes[routeIndex]));
      else
	writeInt(0);
    }
  }
  writeString("@ERoutes");
  
  for (P = aGame->players; P; P = P->next) {
    group          *aGroup;
    
    writeString("@PGroups");
    for (aGroup = P->groups; aGroup; aGroup = aGroup->next) {
      writeInt(ptonum(P->shiptypes, aGroup->type));
      writeInt(aGroup->number);
      writeFloat(aGroup->drive);
      writeFloat(aGroup->weapons);
      writeFloat(aGroup->shields);
      writeFloat(aGroup->cargo);
      writeInt(aGroup->loadtype);
      writeFloat(aGroup->load);
      if (aGroup->from)
	writeInt(ptonum(aGame->planets, aGroup->from));
      else
	writeInt(0);
      if (aGroup->where)
	writeInt(ptonum(aGame->planets, aGroup->where));
      else
	writeInt(0);
      writeFloat(aGroup->dist);
      writeInt(aGroup->ships);
      if (aGroup->thefleet)
	writeInt(ptonum(P->fleetnames, aGroup->thefleet));
      else
	writeInt(0);
    }
    writeString("@EPGroups");
  }
  
  saveRanTab(turnFile);
  fclose(turnFile);
}	
/****/
