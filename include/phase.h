#ifndef GNG_PHASE_H
#define GNG_PHASE_H

#include "galaxy.h"


/* $Id: phase.h,v 1.2 2003/07/31 18:39:06 madcoder42 Exp $ */

/* void giveposition(void); */
void            convertcap(planet *p);
void            unloadcap(planet *p, double x);
void            unloadcol(planet *p, double x, player *P);
void            unloadgroup(group *g, player *P, double amount);
int             kill(group *attack, group *target);

int             mustBomb(player *P, group *g);
player         *determineOwnership(game *aGame, planet *targetPlanet,
                                   player *aPlayer);
void            bombPlanet(game *aGame, planet *p);

void            bombphase(game *aGame);
void            joinphase(game *aGame);
void            loadphase(game *aGame);
void            interceptphase(game *aGame);
void            fleetphase(game *aGame);
void            movephase(game *aGame);
void            unloadphase(game *aGame);
void            producephase(game *aGame);
void            produceShip(planet *aPlanet, double industry);
void            produceMatCap(planet *aPlanet, double industry);
void            produceTechnology(planet *aPlanet, double industry);
void            producePopulation(planet *aPlanet);

void            sortphase(game *aGame);

double          ftrunc2(double x);
void            cktech(group *g);

int             nametocargotype(char *s);



#endif                          /* GNG_PHASE_H */
