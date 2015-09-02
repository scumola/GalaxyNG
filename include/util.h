#ifndef GNG_UTIL_H
#define GNG_UTIL_H

#include "galaxy.h"
#include <limits.h>
#include "list.h"

#ifdef WIN32
#include <stdarg.h>
#include <stdio.h>
#endif

/* $Id: util.h,v 1.10 2004/10/22 17:06:37 madcoder42 Exp $ */

/* this function is meant to copy the body of the message into another
 * file. Note that it is "destructive" in that it won't remember the last
 * location in the file. Its' initial purpose is to put the body of the
 * sent mail in the reply to the user upon some sort of failure (request
 * for a turn in the future, relay to someone that doesn't exist, etc.
*/
void            copyEmailBody(FILE *inMail, FILE *outMail);
void            loadRanTab(FILE * f);
void            saveRanTab(FILE * f);
unsigned int    krandom(void);
void            resetErnie(unsigned long seed);
int             frand3(int r);

void            ftrunc3(void);
FILE           *Fopen(char *filename, char *mode);
FILE           *GOS_fopen(char *filename, char *mode);
int             GOS_mkdir(char *filename, int mode);
int             GOS_delete(char *filename);
int             GOS_copy(char *fromfile, char *tofile);
void           *alloc(unsigned int n);
double          frand(double x);
double          frand2(void);
planet         *addplanet(game *aGame);
int             atwar(player *P, player *P2);
double          weaponmass(shiptype *t);
double          typemass(shiptype *t);
double          shipmass(group *g);
double          groupSpeed(group *g);
double          fleetSpeed(fleetname *fl, group *g);    /* CB-19980922 */
double          cargospace(group *g);
double          calcdefense(group *target);
int             cankill(group *attack, group *target);

int             numOfGroupsInFleet(fleetname *fl, group *g);
void            putmap(mapdimensions *md, double x, double y, int c);
double          dist(game *aGame, planet *p1, planet *p2);      /* CB-20010401 */
void            send(game *aGame, group *g, planet *p); /* CB-20010401 */
void            recall(game *aGame, group *g);  /* CB-19980923 */
void            setproduction(game *aGame, planet *p, int t);
double          groupx(game *aGame, group *g);
double          groupy(game *aGame, group *g);
void            freebattles(game *aGame);
void            freemessages(void);
void            freegame(game *aGame);
double          effectiveIndustry(double pop, double ind);

void            dumpGame(void);
void            dumpPlanets(planet *aPlanet);
void            dumpPlayers(player *aPlayer);

char           *createString(char *format, ...);
int             ssystem(char *format, ...);

void            savefprintf(FILE * f, char *format, ...);

int             canseegroup(player *P, group *g);
int             canseeplanet(player *P, planet *p);

int             isunidentified(player *P, planet *p);
int             isuninhabited(player *P, planet *p);



double          typeSpeed(shiptype *t);
double          typeDefense(shiptype *t);
double          groupDefense(group *g);
double          groupAttack(group *g);

void           *allocStructF(unsigned int n, long cookie);

planet         *findPlanet(game *aGame, char *name);

planet         *groupLocation(game *aGame, group *g);

double          round2(double);
double          roundup2(double);

void            numberGroup(player *aPlayer, group *aGroup);
group          *findgroup(player *P, char *s);
void            raceStatus(game *aGame);
double          effectiveIndustry(double pop, double ind);
double          addShipAtt(game* aGame, planet* p);
char*           rightNow(void);

#endif                          /* GNG_UTIL_H */
