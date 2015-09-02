#ifndef GNG_CREATEGAME_H
#define GNG_CREATEGAME_H

/* $Id: create.h,v 1.6 2005/01/16 02:14:19 madcoder42 Exp $ */

#include "galaxy.h"
#include "avl.h"
#include <fcntl.h>

#ifdef WIN32
/* #include <varargs.h> */
#include <stdarg.h>
#include <stdio.h>
#endif

/****s* Create/newplayer
 * NAME
 *   newplayer --
 * SOURCE
 */

typedef struct newplayer {
	struct newplayer *next;
	long            cookie;
	char           *name;
	char           *addr;
	char           *pswd;
	int             numberOfHomePlanets;
	double         *coreSizes;    /* array of doubles; */
	double         *coreXOffset;
	double		   *coreYOffset;
	int             team;
	int             done;         /* has been selected in a team game */
} newplayer;

/**********/


/****s* Create/gamespecification
 * NAME
 *   gamespecification --
 * FUNCTIONS
 *   Defines the layout of a game, and other things too.
 * SOURCE
 */

typedef struct gamespecification {
  player         *next;
  long            cookie;
  char           *name;
  newplayer      *players;
  double          galaxySize;
  double          minDist;
  double          radiusEmptyPlanets;
  double          radiusSecondaryPlanets;
  int             numberOfHomePlanets;
  double         *coreSizes;    /* Default size of each core home planet */
  int             numberOfEmptyPlanets;
  int             numberOfStuffPlanets;
  int             numberOfPlayers;
  int             numberOfRows;
  int             numberOfColumns;
  int             teamGame;
  gameOpt         gameOptions;  /* CB 20010425 */
} gamespecification;

/*******/


int             createGameDirectories(char *name);

player         *createPlayer(gamespecification *aGameSpec, game *aGame,
                             newplayer *aNewPlayer);

gamespecification *readGameSpec(FILE * specfile);

void            printGameSpecs(gamespecification *gspec);

void            swap_data(double *v1, double *v2);

void            Add_Empty_Planets(game *aGame, int nplanets,
                                  double empty_dist, double x, double y,
                                  int *planet_name, double low_range,
                                  double up_range);

planet         *Add_Core_Home_Planet(game *aGame, double min_dist,
                                     int *planet_name, player *Player,
                                     double size, double px, double py);

planet         *Add_Circle_Home_Planet(game *aGame, int pnum,
                                       double min_dist, double sphere,
                                       int *planet_name, player *aPlayer,
                                       int num_players);

void            Randomize_Planet_Numbers(game *aGame);

void            Add_Stuff_Planets(game *aGame, int stuff_planets,
                                  int *planet_name);

int             createStandardLayout(gamespecification *aGameSpec,
                                     game *aGame, int planet_name);

int             createCheckeredLayout(gamespecification *aGameSpec,
                                      game *aGame, int planet_name);

double          CalcCirclegalaxysize(int num, double dist, double sphere);

game           *creategame(gamespecification *gspec);

void            Add_Circle_Stuff_Planets(game *aGame, int stuff_planets,
                                         int *planet_name, double sphere);

void            Add_Extra_Circle_Stuff_Planets(game *aGame,
                                               int circle_stuff_planets,
                                               double x, double y,
                                               int *planet_name,
                                               int min_dist);

void            Add_Center_Planet(game *aGame, int *planet_name);

newplayer      *getNextTeamPlayer(newplayer *aNewPlayer, int team);

void
                Add_Extra_Home_Planets(game *aGame,
                                       double x, double y,
                                       double size,
                                       double max_radius,
                                       int *planet_name, player *aPlayer,
									   double px, double py);

void            setTeamAllies(player *firstPlayer);


void            Add_Team_Stuff_Planets(game *aGame,
                                       gamespecification *aGameSpec,
                                       int *planet_name);

void            initUniqifyValues(void);

#endif                          /* GNG_CREATEGAME_H */
