#include "util.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#ifndef WIN32
#include <unistd.h>
#endif
#ifdef WIN32
#include <direct.h>
#endif
#include "create.h"
#include "list.h"
#include "loadgame.h"

/* $Id: util.c,v 1.6 2004/10/22 17:07:07 madcoder42 Exp $ */

/****h* GalaxyNG/Util
 * NAME
 *   Util 
 * FUNCTION
 *   A collection of frequently used functions.
 *******
 */



/*********/


/****i* Util/Fopen
 * NAME
 *   Fopen -- try to open a file.
 * SYNOPSIS
 *   FILE *Fopen(char *filename, char *mode)
 * INPUTS
 *   filename   -- name of the file to open
 *   mode       -- open mode 
 * RESULT
 *   file handle of the opened file.
 *   Program is aborted if the file could not be opened.
 * BUGS
 *   This function should not be used!
 *******
 */

FILE           *
Fopen(char *filename, char *mode)
{
  FILE           *f;

  f = fopen(filename, mode);
  if (f == 0) {
    fprintf(stderr, "Can't open file %s in mode %s\n", filename, mode);
    exit(1);
  }
  return f;
}


/****f* Util/GOS_fopen
 * NAME
 *   GOS_fopen --
 * FUNCTION
 *   Hide OS dependencies.
 ******
 */

FILE           *
GOS_fopen(char *filename, char *mode)
{
  FILE           *f;

#ifdef WIN32
  char           *c;

  for (c = filename; *c; c++) {
    if (*c == '/')
      *c = '\\';
  }
#endif
  f = fopen(filename, mode);
  return f;
}

/****f* Util/GOS_mkdir
 * NAME
 *   GOS_mkdir -- create a directory
 * FUNCTION
 *   Hide OS dependencies.
 *****
 */

int
GOS_mkdir(char *pathname, int mode)
{
  int             res;

#ifdef WIN32
  res = mkdir(pathname);
#else
  res = mkdir(pathname, mode);
#endif                          /* WIN32 */
  return res;
}

/****f* Util/GOS_copy
 * NAME
 *
 *****
 */

int
GOS_copy(char *fromfile, char *tofile)
{
  int             res;

  char           *c;
#ifdef WIN32

  for (c = fromfile; *c; c++) {
    if (*c == '/')
      *c = '\\';
  }
  for (c = tofile; *c; c++) {
    if (*c == '/')
      *c = '\\';
  }
  res = ssystem("copy %s %s", fromfile, tofile);
#else
  for (c = tofile; *c; c++) {
    if (*c == ' ')
      *c = '_';
  }
  plog(LBRIEF, "cp %s %s\n", fromfile, tofile);
  res = ssystem("cp %s %s", fromfile, tofile);
#endif
  return res;
}

/****f* Util/GOS_delete
 * NAME
 *
 *****
 */

int
GOS_delete(char *filename)
{
  int             res;

#ifdef WIN32
  char           *c;

  for (c = filename; *c; c++) {
    if (*c == '/')
      *c = '\\';
  }
  res = ssystem("del %s", filename);
#else
  res = ssystem("rm %s", filename);
#endif
  return res;
}

/****f* Util/GOS_copy
 * NAME
 *
 *****
 */


/****f* Util/alloc
 * NAME
 *   alloc -- allocate memory
 * SYNOPSIS
 *   void *alloc(unsigned int requestedSize)
 * FUNCTION
 *   Try to allocate memory. If the allocation fails, abort
 *   the program.
 * RESULTS
 *   Pointer to the allocated memory.
 * SOURCE
 */

void           *
alloc(unsigned int n)
{
  void           *p;

  p = malloc(n);
  if (p == 0) {
    printf("Out of memory\n");
    exit(1);
  }
  return p;
}

/************/





/****f* Util/addplanet
 * NAME
 *   addplanet -- add a planet to planet list
 * SYNOPSIS
 *   planet *addplanet(void)
 * FUNCTION
 *   Creates a planet stucture and adds it to the list of planets. The
 *   name of the planet is its index number in the list.
 * SEE ALSO
 *   planets, planet, create.c, create() 
 * SOURCE
 */

planet         *
addplanet(game *aGame)
{
  planet         *p;
  int             i;
  char            name[20];

  for (p = aGame->planets, i = 1; p; p = p->next, i++);
  p = allocStruct(planet);

  addList(&(aGame->planets), p);
  sprintf(name, "%d", i);
  setName(p, name);
  return p;
}

/*************/


/****f* Util/atwar
 * NAME
 *   atwar -- are two players at war?
 * SYNOPSIS
 *   int atwar(player *P, player *P2)
 * RESULT
 *   TRUE  - players are at war with eachother
 *   FALSE - they are at peace. 
 * SOURCE
 */

int
atwar(player *P, player *P2)
{
  alliance       *a;

  if (P == P2)
    return 0;
  for (a = P->allies; a; a = a->next)
    if (a->who == P2)
      return 0;
  return 1;
}

/**********/


/****f* Util/weaponmass
 * NAME
 *   weaponmass -- compute weapon mass of a ship type
 * SYNOPSIS
 *   double weaponmass(shiptype *t)
 * FUNCTION
 *   Compute the mass of the weapons of a ship type, this is a function of
 *   the weapon and the number of attacks.
 * SOURCE
 */

double
weaponmass(shiptype *t)
{
  double          res;

  res = (t->attacks ? t->weapons : 0) +
      ((t->attacks > 1) ? ((t->attacks - 1) * 0.5 * t->weapons) : 0);
  return res;
}

/**********/


/****f* Util/typemass
 * NAME
 *   typemass -- compute mass of a ship type
 * SYNOPSIS
 *   double typemass(shiptype *t)
 * FUNCTION
 *   Compute mass of a ship type. This differs from the mass of a ship which
 *   includes the mass of the cargo carried by the ship.
 * SEE ALSO
 *   shipmass()
 * SOURCE
 */

double
typemass(shiptype *t)
{
  double          res;

  res = t->drive + weaponmass(t) + t->shields + t->cargo;
  return res;
}

/***********/


double
calcdefense(group *target)
{
  double          res;

  res = target->type->shields * target->shields * BATTLEMAGIC /
      pow(shipmass(target), .333333333333333);

  return res;
}


/****f*
 * NAME
 *   cankill --
 *
 ***/

int
cankill(group *attack, group *target)
{
  double          defense, ratio;

  if (!attack->type->attacks)
    return 0;
  defense = calcdefense(target);
  if (!defense)
    return 1;
  ratio = attack->weapons * attack->type->weapons / defense;
  return ratio > .251;
}


/***** Util/cargospace
 * NAME 
 *   cargospace -- compute cargo space of a ship in a group.
 * RESULT
 *   cargo space of one ship in a group at current
 *   tech levels.
 * SOURCE
 */

double
cargospace(group *g)
{
  double          size;
  double          res;

  size = g->type->cargo;
  res = g->cargo * size * (1 + size / 10.0);
  return res;
}

/********/



/****f* Util/typeSpeed
 * NAME
 *   typeSpeed -- basic speed of a ship of type t
 * SYNOPSIS
 *   double typeSpeed(shiptype *t)
 * FUNCTION
 *   Compute the speed of a ship of type t. This does not take
 *   in account any of the techlevels or the cargo carried by
 *   a ship. 
 * SOURCE
 */

double
typeSpeed(shiptype *t)
{
  double          res;

  res = t->drive * DRIVEMAGIC / typemass(t);
  return res;
}

/**********/


/****f* Util/typeDefense
 * NAME
 *   typeDefense -- basic shield strength of a ship of type t
 * SYNOPSIS
 *   double typeDefense(shiptype *t)
 * FUNCTION
 *   The strength of the shields of a ship of type t. 
 *   This does not take in account the shield tech level.
 *   Basically the shield strength of a ship without cargo
 *   with all tech levels set 1.0.
 * SEE ALSO
 *   groupDefense()
 * SOURCE
 */

double
typeDefense(shiptype *t)
{
  double          res;

  res = t->shields * BATTLEMAGIC / pow(typemass(t), .333333333333333);
  return res;
}

/********/


/****f* Util/groupDefense
 * NAME
 *   groupDefense -- shield strength of a ship in group g
 * SYNOPSIS
 *   double groupDefense(group *g)
 * FUNCTION
 *   Computes the shield strength of a ship in a group.
 *   Based on the mass of the ship (including cargo) and
 *   its current tech levels.
 * SEE ALSO
 *   typeDefense()
 * SOURCE
 */

double
groupDefense(group *g)
{
  double          res;

  res = g->type->shields * g->shields *
      BATTLEMAGIC / pow(shipmass(g), .333333333333333);
  return res;
}

/***********/


/****f* Util/groupAttack
 * NAME
 *   groupAttack
 * SYNOPSIS
 *   double groupAttack(group *g)
 * FUNCTION
 *   Compute the effective attack of a ship in a group. 
 *   This is the product of its weapon and weapon tech level.
 * SOURCE
 */

double
groupAttack(group *g)
{
  double          res;

  res = g->weapons * g->type->weapons;
  return res;
}

/**********/


/****f* Util/shipmass
 * NAME
 *   shipmass -- compute the mass of a ship in a group of ships
 * SYNOPSIS
 *   double shipmass(group *g)
 * FUNCTION
 *   Compute the mass of a ship in a group of ships. This includes
 *   the mass of the cargo carried by the ship.
 * SEE ALSO
 *   typemass()
 * SOURCE
 */

double
shipmass(group *g)
{
  double          l;
  double          res;

  l = g->load;
  if (g->cargo)
    l /= g->cargo;
  res = typemass(g->type) + l;
  return res;
}

/**********/


/****f* Util/effectiveIndustry
 * NAME
 *   effectiveIndustry -- compute effective industry
 * SYNOPSIS
 *   double effectiveIndustry(double pop, double ind)
 * FUNCTION
 *   Compute the effective industry.
 * BUGS
 *   This should have the signature (Planet *p).  
 * SOURCE
 */

double
effectiveIndustry(double pop, double ind)
{
  double          res;

  res = (pop - ind) / 4.0 + ind;
  return res;
}

/*********/


/****f* Util/groupSpeed
 * NAME
 *   groupsSpeed -- compute the speed of a ship in a group of ships.
 * SYNOPSIS
 *   double groupSpeed(group *g)
 * FUNCTION
 *   Compute the speed of a ship in a group of ships, based on the
 *   mass of the ship and the drive technology.  
 * SOURCE
 */

double
groupSpeed(group *g)
{
  double          res;

  res = g->type->drive * g->drive * DRIVEMAGIC / shipmass(g);
  return res;
}

/*********/


/****f* Util/fleetSpeed
 * NAME
 *   fleetSpeed
 * SYNOPSIS
 *   double fleetSpeed(fleetname *fl, group *g)
 * FUNCTION
 *   Compute the speed of a fleet, this is the speed of the slowest
 *   ship in the fleet.  
 * INPUTS
 *   fl  -- name of the fleet.
 *   g   -- list with all groups of the race that owns the fleet.
 * RESULT
 *   Speed of the fleet.
 * NOTES
 *   This is a time expensive function since all groups of a race
 *   are scanned.
 * SOURCE
 */

double
fleetSpeed(fleetname *fl, group *g)
{                               /* CB-19980922 */
  double          res = 0, tmp;
  group          *g2;

  /* FS Mon Oct 12 14:39:11 MEST 1998 Sometimes people have fleets with *
   * * * * * * * * * * ships of speed 0.0 */
  for (g2 = g; g2; g2 = g2->next) {
    if (g2->thefleet == fl) {
      res = groupSpeed(g2);
      break;
    }
  }
  for (; g2; g2 = g2->next) {
    if (g2->thefleet == fl) {
      tmp = groupSpeed(g2);
      if (tmp <= res)
        res = tmp;
    }
  }
  return res;
}

/**********/



/****f* Util/numOfGroupsInFleet
 * NAME
 *   numOfGroupsInFleet -- number of groups in a fleet.
 * SYNOPSIS
 *   int numOfGroupsInFleet(fleetname *fl, group *g)
 * FUNCTION
 *   Count the number of groups in a fleet.
 * INPUTS
 *   fl  -- name of the fleet
 *   g   -- list with all groups of the race that owns the fleet
 * RESULT
 *   Number of groups in the fleet.
 * NOTES
 *   This is a time expensive function since all groups of a race
 *   are scanned.
 * SOURCE
 */

int
numOfGroupsInFleet(fleetname *fl, group *g)
{
  group          *g2;
  int             count;

  count = 0;
  for (g2 = g; g2; g2 = g2->next) {
    if (g2->thefleet == fl) {
      count++;
    }
  }

  return count;
}

/***********/


/****f* Util/dist
 * NAME
 *   dist -- distance between two planets
 * SYNOPSIS
 *   double dist(planet *p1, planet *p2)
 * SOURCE
 */

double
dist(game *aGame, planet *p1, planet *p2)
{
  double          dx, dy;
  double          res;
  double          x1, x2, y1, y2, s1, s2;

  x1 = p1->x;
  x2 = p2->x;
  y1 = p1->y;
  y2 = p2->y;

  dx = fabs(x1 - x2);
  dy = fabs(y1 - y2);

  if (aGame->gameOptions.gameOptions & GAME_SPHERICALGALAXY) {  /* CB-20010401 
                                                                 * ; see
                                                                 * galaxy.h 
                                                                 */
    s1 = aGame->galaxysize;
    s2 = aGame->galaxysize / 2.0;

    dx = (dx < s2) ? dx : s1 - dx;
    dy = (dy < s2) ? dy : s1 - dy;
  }

  res = sqrt(dx * dx + dy * dy);
  return res;
}

/********/



/****f* Util/putmap
 * NAME
 *   putmap --
 * FUNCTION
 *   Put an element on the map.
 * SOURCE
 */

void
putmap(mapdimensions *md, double x, double y, int c)
{
  int             ix, iy;

  ix = (x - md->x1) / (md->x2 - md->x1) * MAPWIDTH;
  iy = (y - md->y1) / (md->y2 - md->y1) * MAPHEIGHT;
  if (ix < 0 || ix >= MAPWIDTH || iy < 0 || iy >= MAPHEIGHT)
    return;
  map[ix][iy] = c;
}

/********/


/****f* Util/send
 * NAME
 *   send -- send a group to a planet.
 * SYNOPSIS
 *   void send(game *aGame, group *g, planet *p)
 * INPUTS
 *   g -- group to send
 *   p -- destination planet
 * SOURCE
 */

void
send(game *aGame, group *g, planet *p)
{
	g->from = g->where;
	g->where = p;
	g->dist = dist(aGame, p, g->from);
}

/*********/


/****f* Util/recall
 * NAME
 *   recall -- recall a group 
 * SYNOPSIS
 *   void recall(group *g, planet *p)
 * FUNCTION
 *   Make a group that is already underway to a planet return
 *   to the planet of origin.
 * INPUTS
 *   g -- group to be recalled
 * SOURCE
 */

void
recall(game *aGame, group *g)
{                               /* CB-19980923 */
  planet         *p2;

  g->dist = dist(aGame, g->from, g->where) - g->dist;
  p2 = g->from;
  g->from = g->where;
  g->where = p2;
}

/**********/


/****f* Util/setproduction
 * NAME
 *   setproduction -- set production of a planet
 * SYNOPSIS
 *   void setproduction(planet *p, int t)
 * FUNCTION
 *   Set the production type of a planet. If the game option
 *   GAME_KEEPPRODUCTION is set, the production points spend
 *   on the previous product are preserved, otherwise all points
 *   are lost.
 * SOURCE
 */

void
setproduction(game *aGame, planet *p, int t)
{
  double          perccomp;
  double          mass;
  double          theshipmass;
  double          prog;

  if (p->producing == PR_SHIP) {
    theshipmass = typemass(p->producingshiptype);
    mass = theshipmass * INDPERSHIP;
    prog = p->inprogress;
    if (theshipmass > p->mat)
      mass += (theshipmass - p->mat) / p->resources;
    perccomp = prog / mass;
    p->mat += typemass(p->producingshiptype) * perccomp;
  }
  if (!(aGame->gameOptions.gameOptions & GAME_KEEPPRODUCTION))
    p->inprogress = 0;
  p->producing = t;
  p->producingshiptype = 0;
}

/********/


/****f* Util/groupx
 * NAME
 *   groupx -- compute x coordinate of a group
 * SYNOPSIS
 *   double groupx(group *g)
 * FUNCTION
 *   Compute the x coordinate of a group. If a group is at a planet
 *   this coordinate is the x coordinate of that planet. If the group
 *   is traveling it is the x coordinate of the vector pointing to the
 *   destination planet and orginating from the planet of origin, and
 *   with a length equal to the destination traveled sofar.
 * SOURCE
 */

double
groupx(game *aGame, group *g)
{
  double          dx, ratio, dis;
  double          res;

  dis = dist(aGame, g->from, g->where);
  if (!dis)
    return g->where->x;
  dx = g->where->x - g->from->x;
  ratio = g->dist / dis;
  res = g->where->x - ratio * dx;
  return res;
}

/*********/


/****f* Util/groupy
 * NAME
 *   groupy -- compute y coordinate of a group
 * SYNOPSIS
 *   double groupy(group *g)
 * FUNCTION
 *   Compute the y coordinate of a group. If a group is at a planet
 *   this coordinate is the y coordinate of that planet. If the group
 *   is traveling it is the y coordinate of the vector pointing to the
 *   destination planet and orginating from the planet of origin, and
 *   with a length equal to the destination traveled sofar.
 * SOURCE
 */


double
groupy(game *aGame, group *g)
{
  double          dy, ratio, dis;
  double          res;

  dis = dist(aGame, g->from, g->where);
  if (!dis)
    return g->where->y;
  dy = g->where->y - g->from->y;
  ratio = g->dist / dis;
  res = g->where->y - ratio * dy;
  return res;
}

/*********/



/****i* Util/freebattles
 * NAME
 *   freebattles -- free all memory used by a list of battles.
 * NOTES
 *   Useless at the moment.
 ******
 */

void
freebattles(game *aGame)
{

  battle         *b, *b2;
  bombing        *B, *B2;
  participant    *r, *r2;

  pdebug(DFULL, "freebattles\n");

  b = aGame->battles;
  while (b) {
    validateStruct(battle, b);

    r = b->participants;
    while (r) {
      freelist(r->groups);
      r2 = r->next;
      free(r);
      r = r2;
    }
    if (b->protocol) {
      free(b->protocol->shots);
      free(b->protocol);
    }
    free(b->name);
    b2 = b->next;
    free(b);
    b = b2;
  }

  B = aGame->bombings;
  while (B) {
    validateStruct(bombing, B);

    freelist(B->viewers);
    B2 = B->next;
    free(B->name);
    free(B);
    B = B2;
  }

  aGame->battles = NULL;
  aGame->bombings = NULL;

}



/****i* Util/freegame
 * NAME
 *   freegame --
 * SYNOPSIS
 *   void freegame(void)
 * FUNCTION
 *   Free all memory used by the current game.
 * BUGS
 *   Does not work at all.
 *****
 */

void
freegame(game *aGame)
{
  player         *aPlayer, *P2;

  pdebug(DFULL, "freegame()\n");

  freelist(aGame->planets);
  aPlayer = aGame->players;
  while (aPlayer) {
    pdebug(DFULL, "freegame : freeing player %s\n", aPlayer->name);
    freelist(aPlayer->shiptypes);
    freelist(aPlayer->allies);
    freelist(aPlayer->groups);
    freelist(aPlayer->fleetnames);
    pdebug(DFULL, "free game : P->orders\n");
    freestrlist(aPlayer->orders);
    pdebug(DFULL, "free game : P->messages\n");
    freestrlist(aPlayer->messages);
    pdebug(DFULL, "free game : P->mistakes\n");
    freestrlist(aPlayer->mistakes);
    free(aPlayer->name);
    free(aPlayer->realName);
    free(aPlayer->addr);
    free(aPlayer->pswd);
    P2 = aPlayer->next;
    free(aPlayer);
    aPlayer = P2;
  }
  pdebug(DFULL, "free game : messages\n");
  freestrlist(aGame->messages);
  pdebug(DFULL, "free game : battles\n");
  freebattles(aGame);
  aGame->battles = NULL;
  aGame->planets = NULL;
  aGame->players = NULL;
  aGame->messages = NULL;
  freeDefaults(aGame);
  free(aGame->name);
  free(aGame);
}



/****i* Util/dumpGame
 * NAME
 *   dumpGame -- dump game data structure to stdout.
 * NOTES 
 *   For debug purpose. Does not do any thing at the moment.
 ******
 */

void
dumpGame(void)
{
  /* 
   * dumpPlanets(planets);  dumpPlayers(players); */
}

/****i* Util/dumpPlanets
 * NAME
 *   dumpPlanets -- dump all planet data to stdout.
 *****
 */

void
dumpPlanets(planet *aPlanet)
{
  /* 
   * for (; aPlanet; aPlanet = aPlanet->next) { printf("%s %.2f %.2f %.2f
   * %.2f %.2f %.2f ", aPlanet->name, aPlanet->x, aPlanet->y, aPlanet->size, 
   * aPlanet->resources, aPlanet->pop, aPlanet->ind); printf(" %.2f %.2f
   * %.2f %.2f ", aPlanet->cap, aPlanet->mat, aPlanet->col,
   * aPlanet->inprogress); if (aPlanet->owner) { printf("%d ",
   * ptonum(players, aPlanet->owner)); printf("%s\n", aPlanet->owner->name);
   * } else printf("0 none\n"); } */

}

/****i* Util/dumpPlayers
 * NAME
 *   dumpPlayers -- dump all data of the players to stdout
 *****
 */

void
dumpPlayers(player *aPlayer)
{
  /* 
   * for (; aPlayer; aPlayer = aPlayer->next) { printf("%s\n",
   * aPlayer->name); dumpStrList(aPlayer->messages);
   * dumpStrList(aPlayer->mistakes); } */
}


/****f* Util/ssystem
 * NAME
 *   ssystem -- run a dynamically created system command.
 * FUNCTION
 *   Construct a command using sprintf() and then run it using
 *   system().
 * NOTES
 *   Take care not to overflow the lineBuffer!
 * SOURCE
 */

int
ssystem(char *format, ...)
{
  int             res;
  va_list         ap;

  va_start(ap, format);
  vsprintf(lineBuffer, format, ap);
  res = system(lineBuffer);

  if (res != 0) {
    plog(LBRIEF, "%s returned %d\n", lineBuffer, res);
  }
  else {
    plog(LPART, "ran %s\n", lineBuffer);
  }
  va_end(ap);
  return res;
}

/**********/



/****f* Util/createString
 * NAME
 *   createString -- create a string
 * SYNOPSIS
 *   char *createString(char *,...)
 *   stringcopy = createString(format,...)
 * FUNCTION
 *   Creates a string using a printf() like syntax.
 * RESULT
 *   stringcopy, pointer to created string.  
 * SOURCE
 */

char           *
createString(char *format, ...)
{
  int             n;
  va_list         ap;

  va_start(ap, format);
#ifdef WIN32
  vsprintf(lineBuffer, format, ap);
#else
  n = vsnprintf(lineBuffer, LINE_BUFFER_SIZE, format, ap);
  assert(n != -1);
#endif
  va_end(ap);
  return strdup(lineBuffer);
}

/************/


/****f* Util/savefprintf
 * NAME
 *   savefprintf -- write to and and check if it was really written.
 * SYNOPSIS
 *   void savefprintf(FILE * f, char *format,...)
 * FUNCTION
 *   Writes data to file and checks if the write succeeded.
 *   If not the program is aborted.  This is primarily used
 *   to make sure the orders of players are really written to
 *   file.  There have been cases were a player got a reply 
 *   from the orders checker saying that their orders were valid,
 *   but when the turn ran there orders were no where to be found.    
 * NOTES
 *   The functions try to write to the log file... if a write to
 *   the orders file fails then a write to the log file is most
 *   likely to fail too.  Some other method of notifying the GM
 *   should be used.  
 ******
 */

void
savefprintf(FILE * f, char *format, ...)
{
  int             error;
  va_list         ap;

  va_start(ap, format);
  error = vfprintf(f, format, ap);
  if (error < 0) {
    fprintf(stderr, "Galaxyng: write to file failed!\n");
    plogtime(LNONE);
    plog(LFULL, "Galaxyng: write to file failed!\n");
    printf("Subject: Error\n");
    printf("\nThere was a serious error, your orders are lost.\n");
    printf("\nPlease Contact your game master\n");
    closeLog();
    abort();
  }
  va_end(ap);
}


/****f* Util/canseegroup
 * NAME
 *   canseegroup -- can a Race see a group?
 * SYNOPSIS
 *   int canseegroup(player *P, group *g)
 * RESULT
 *   TRUE if race P can see group g.
 * SOURCE
 */

int
canseegroup(player *P, group *g)
{
  group          *g2;

  if (g->dist)
    return 0;
  if (g->where->owner == P)
    return 1;
  for (g2 = P->groups; g2; g2 = g2->next)
    if (!g2->dist && g2->where == g->where)
      return 1;
  return 0;
}

/********/


/****f* Util/canseeplanet
 * NAME
 *   canseeplanet -- can a Race see a planet?
 * SYNOPSIS
 *   int canseeplanet(player *P, planet *p)
 * RESULT
 *   TRUE if race P can see planet p.
 * SOURCE
 */

int
canseeplanet(player *P, planet *p)
{
  group          *g;

  if (p->owner == P)
    return 1;
  for (g = P->groups; g; g = g->next)
    if (g->dist == 0 && g->where == p)
      return 1;
  return 0;
}

/******/


/****f* Util/isunidentified
 * NAME
 *   isunidentified -- is a planet unidentified?
 * SYNOPSIS
 *   int isunidentified(player *P, planet *p)
 * FUNCTION
 *   Is a planet considered unidentified for Race P?
 *   This is the case when a planet has an owner, and this
 *   owner is not P, and P can not see the planet (has no
 *   groups in orbit at the planet).
 * INPUTS
 *   P -- Race to check for
 *   p -- planet to check.
 * RESULT
 *   TRUE -- planet is unidentified
 * SOURCE
 */

int
isunidentified(player *P, planet *p)
{
  return (p->owner != NULL && p->owner != P && !canseeplanet(P, p));
}

/*********/


/****f* Util/isuninhabited
 * NAME
 *   isuninhabited -- is a planet uninhabited?
 * SYNOPSIS
 *   int isuninhabited(player *P, planet *p)
 * INPUTS
 *   p -- planet to check.
 * RESULT
 *   TRUE -- planet is uninhabited
 * BUGS
 *   P is not used!
 * SOURCE
 */

int
isuninhabited(player *P, planet *p)
{
  return (p->owner == NULL);
}

/********/



/****f* Util/groupLocation
 * NAME
 *   groupLocation -- compute location of a group
 * SYNOPSIS
 *   planet  *groupLocation(group *g)
 * FUNCTION
 *   Determine which planet a group is orbiting.
 * RESULT
 *   NULL if the group is in hyperspace, pointer to the 
 *   planet the group is orbitting otherwise.
 * SOURCE
 */

planet         *
groupLocation(game *aGame, group *g)
{
  planet         *p;

  p = NULL;
  if (g->dist eq 0)
    p = g->where;
  else if (g->dist eq dist(aGame, g->where, g->from))
    p = g->from;
  return p;
}

/**********/



/****f* Util/round2
 * NAME
 *   round2 -- round a number to 2 decimal places.
 * SYNOPSIS
 *   double round2(double f)
 * NOTES 
 *   Has most likely the same effect as ftrunc2(), so one of them
 *   can go.
 * SOURCE
 */

double
round2(double f)
{
  int             i;

  i = f * 100.0;
  f = i / 100.0;
  return f;
}

/********/


/****f* Util/roundup2
 * NAME
 *   roundup2 -- round a number to 2 decimal places, adding .005 first
 * SYNOPSIS
 *   double roundup2(double f)
 *
 * SOURCE
 */

double
roundup2(double f)
{
  int             i;

  i = (f + 0.005) * 100.0;
  f = i / 100.0;
  return f;
}

/********/



/****f* Util/numberGroup
 * NAME
 *   numberGroup -- gives a group a number
 * SYNOPSIS
 *   void numberGroup(player *aPlayer, group *aGroup)
 * FUNCTION
 *   Number the group with the next available number.
 * SOURCE 
 */

void
numberGroup(player *aPlayer, group *aGroup)
{
  group          *curGroup;
  int             groupNumber;

  groupNumber = 0;
  for (curGroup = aPlayer->groups; curGroup; curGroup = curGroup->next) {
    if (curGroup->number > groupNumber)
      groupNumber = curGroup->number;
  }
  groupNumber++;
  aGroup->number = groupNumber;
}

/********/



/****f* Util/findPlanet
 * NAME
 *   findPlanet -- find a planet
 * SYNOPSIS
 *   planet     *findPlanet(game *, char *)
 *   thePlanet = findPlanet(aGame, name)
 * FUNCTION
 *   Find a planet in the list of planets using its name.
 *   The name can be specified in two ways:
 *   #<planet number at turn 0>
 *   <current planet name as listed in the turn report>
 * INPUTS
 *   aGame  -- game structure.
 *   name   -- planet's name
 * SOURCE
 */

planet         *
findPlanet(game *aGame, char *name)
{
  if (name[0] eq '#') {
    char           *c;

    for (c = name + 1; *c && isdigit(*c); c++);
    if (!(*c)) {
      return (planet *) numtop(aGame->planets, atoi(name + 1));
    }
    else {
      return NULL;
    }
  }
  else {
    return findElement(planet, aGame->planets, name);
  }
}

/*************/



/****f* Util/findgroup
 * NAME
 *   findgroup -- find a group
 * SYNOPSIS
 *   group     *findgroup(player *, char *)
 *   theGroup = findgroup(aPlayer, name)
 * FUNCTION
 *   Find a group in the list of groups using its name.
 *   The name is either the ASCII representation of the group
 *   number of the keyword "MAX" in which case the last group
 *   in the list is returned.
 * INPUTS
 *   aPlayer -- player who's groups will be examined.
 *   name    -- group name.
 * SOURCE
 */

group          *
findgroup(player *P, char *s)
{
  group          *g;

  g = NULL;
  if (P->groups) {
    if (!noCaseStrcmp(s, "max")) {
      for (g = P->groups; g->next; g = g->next);
    }
    else {
      int             groupNumber;

      groupNumber = atoi(s);
      for (g = P->groups; g && (g->number != groupNumber); g = g->next);
    }
  }
  return g;
}

/***********/



/****f* Util/raceStatus
 * NAME
 *   raceStatus -- compute status of all races
 * FUNCTION
 *   Compute population, industry, and number of planets of all races.
 * SOURCE
 */

void
raceStatus(game *aGame)
{
  planet         *aPlanet;
  player         *aPlayer;

  for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
    aPlayer->totPop = 0;
    aPlayer->totInd = 0;
    aPlayer->numberOfPlanets = 0;
    aPlayer->totCap = 0;
    aPlayer->totMat = 0;
    aPlayer->totCol = 0;
    for (aPlanet = aGame->planets; aPlanet; aPlanet = aPlanet->next) {
      if (aPlanet->owner eq aPlayer) {
        aPlayer->totPop += aPlanet->pop;
        aPlayer->totInd += aPlanet->ind;
        aPlayer->totCap += aPlanet->cap;
        aPlayer->totMat += aPlanet->mat;
        aPlayer->totCol += aPlanet->col;
        aPlayer->numberOfPlanets++;
      }
    }
  }
}

/*************/





/****f* Util/krandom
 * NAME
 *   krandom -- portable random number generator
 * SYNOPSIS
 *   unsigned int krandom(void) 
 * FUNCTION
 *   Random number generator based on an algorithm described
 *   in Donald Knuth's "The art of computer programming".
 * SEE ALSO
 *   frand(), frand2(), frand3()
 * SOURCE
 */

#define RTAB_LENGTH 55          /* Don't ever change this!!! */
unsigned int    Ran_tab[RTAB_LENGTH];
int             Ran_k;
int             Ran_j;

unsigned int
krandom(void)
{
  Ran_tab[Ran_k] += Ran_tab[Ran_j];
  Ran_k--;
  Ran_j--;
  if (Ran_k < 0)
    Ran_k = 54;
  if (Ran_j < 0)
    Ran_j = 54;
  return (Ran_tab[Ran_k]);
}

/***********/


/****f* Util/frand
 * NAME
 *   frand -- generate a random float, range [0,x]
 * SYNOPSIS
 *   double frand(double x)
 * FUNCTION
 *   Generate a random float in the range [0,x] 
 * BUGS
 *   Has a non descriptive name.
 * SEE ALSO
 *   frand2(), frand3(), krandom()
 * SOURCE
 */

double
frand(double x)
{
  double          fr;

  fr = (double) krandom() / (double) (UINT_MAX);        /* From limits.h */
  return x * fr;
}

/********/


/****f* Util/frand2
 * NAME
 *   frand2 -- generate a random float, range [-1,1]
 * SYNOPSIS
 *   double frand2(void)
 * FUNCTION
 *   Generate a random float in the range [-1,1] 
 * BUGS
 *   Has a non descriptive name.
 * SEE ALSO
 *   frand(), frand3(), krandom()
 * SOURCE
 */

double
frand2(void)
{
  return ((double) krandom() / (double) (UINT_MAX)) * 2.0 - 1.0;
}

/************/


/****f* Util/frand3
 * NAME
 *   frand3 -- generate a random integer
 * SYNOPSIS
 *   int frand3(int r)
 * FUNCTION
 *   Generate a random integer in the range [0,r-1] 
 * BUGS
 *   This function has a stupid name.
 * SEE ALSO
 *   frand2(), krandom()
 * SOURCE
 */

int
frand3(int r)
{
  return (int) floor(((double) krandom() / (double) (UINT_MAX)) * r);
}

/***********/


/****f* Util/saveRanTab
 * NAME
 *   saveRanTab -- save seed values to turn file
 * FUNCTION
 *   Save the table with seed values for the random number generator
 *   to the turn file.
 * SEE ALSO
 *   loadRanTab()
 * SOURCE
 */

void
saveRanTab(FILE * f)
{
  int             i;

  for (i = 0; i < RTAB_LENGTH; i++) {
    fprintf(f, "%u\n", Ran_tab[i]);
  }
}

/***********/


/****f* Util/loadRanTab
 * NAME
 *   loadRanTab -- load seed values from turn file
 * FUNCTION
 *   Load the table with seed values for the random number generator
 *   from the turn file.
 * SEE ALSO
 *   saveRanTab()
 * SOURCE
 */

void
loadRanTab(FILE * f)
{
  int             i, loaded;

  loaded = TRUE;
  for (i = 0; i < RTAB_LENGTH; i++) {
    unsigned int    s;

    if (fscanf(f, "%u", &s) != 1) {
      loaded = FALSE;
      break;
    }
    else {
      Ran_tab[i] = s;
    }
  }
  if (loaded) {
    Ran_k = 23;
    Ran_j = 54;
  }
  else {
    plog(LPART, "Failed to load the random table, creating one.\n");
    resetErnie(197162622);
  }
}

/**********/


/****f* Util/resetErnie
 * NAME
 *   resetErnie -- reset random number generator
 * FUNCTION
 *   Reset the random number generator.
 *   Based on a seed value, a second random number generator is
 *   used to create the seed values for the actual random number
 *   generator.
 * SEE ALSO
 *   krandom()
 * SOURCE
 */

void
resetErnie(unsigned long seed)
{
  unsigned long   rval;
  int             i, bit;

  pdebug(DFULL, "resetErnie\n");
  rval = seed;
  for (i = 0; i < 55; i++) {
    unsigned long   temp;

    for (bit = 0, temp = 0; bit < 32; bit++) {
      temp = temp << 1;
      rval = ((((rval >> 31) ^ (rval >> 6) ^ (rval >> 4) ^ (rval >> 2)
                ^ (rval >> 1) ^ rval) & 0x1) << 31) | (rval >> 1);
      temp |= rval & 0x00000001;
    }
    Ran_tab[i] = temp;
  }
  Ran_k = 23;
  Ran_j = 54;
  for (i = 0; i < 18000; i++)
    krandom();
}

/*********/

/****f* Util/addShipAtt
 * NAME
 *   addShipAtt - determine the attack strength of groups at this planet
 * SYNOPSIS
 *   double addShipAtt(Game*, planet*)
 * FUNCTION
 *   determine the attack strength of all ships located 
 *   at the specified planet on a per player basis
 * SOURCE
 */

double
addShipAtt(game* aGame, planet* p)
{
	double total = 0.;
	player* P = aGame->players;
	group*  g;

	while (P) {
		printf("checking player %s\n", P->name);
		for (g = P->groups; g; g = g->next) {
			if (g->where == p && g->dist == 0.) {
				printf("found a group, value: %f\n", g->weapons*g->type->weapons);
				total += g->weapons * g->type->weapons;
			}
		}
		P = P->next;
	}
	printf("%s: %f\n", p->name, total);
	return total;
}

/*******/

void
freeDefaults(game *aGame)
{
  free(aGame->serverOptions.sendmail);
  free(aGame->serverOptions.encode);
  free(aGame->serverOptions.compress);
  free(aGame->serverOptions.GMemail);
  free(aGame->serverOptions.GMname);
  free(aGame->serverOptions.GMpassword);
  free(aGame->serverOptions.SERVERemail);
  free(aGame->serverOptions.SERVERname);
  free(aGame->serverOptions.fontpath);
  free(aGame->serverOptions.due);
}


char*
rightNow() {
  static char timestamp[32];

  time_t now = time(NULL);
	
  strcpy(timestamp, asctime(localtime(&now)));
  
  return timestamp;
}


void
copyEmailBody(FILE *inMail, FILE *outMail)
{
  char buffer[4096];

  rewind(inMail);

  /* read all the header stuff which only goes until the first blank line */
  while (fgets(buffer, sizeof(char)*4096, inMail)) {
    if (buffer[0] == '\n')
      break;
  }

  /* now copy everything up to the (optional) #end */
  while (fgets(buffer, sizeof(char)*4096, inMail)) {
    if (noCaseStrncmp(buffer, "#end", 4) == 0) {
      fprintf(outMail, "%s", buffer);
      break;
    }
    fprintf(outMail, "%s", buffer);
  }

  return;
}
