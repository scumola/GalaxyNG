#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "galaxy.h"
#include "list.h"
#include "phase.h"
#include "util.h"
#include "loadgame.h"
#include "savegame.h"

char           *vphase =
    "$Id: phase.c,v 1.9 2004/09/06 12:03:38 madcoder42 Exp $";

/****h* GalaxyNG/Phase
 * NAME
 *   Phase -- functions to compute the different phases in a turn.
 * BUGS
 *   At the moment the intercept phase is pretty buggy.
 *   If there are loops or paths, that is a group intercepts
 *   on a planet where there are groups that intercept an 
 *   another planet the outcome of the algorithm is pretty random.
 *****
 */

/****f* Phase/convertcap
 * NAME
 *   convertcap -- convert capital into industry.
 * SYNOPSIS
 *   void convertcap(planet *p)
 * SOURCE
 */

void
convertcap(planet *p)
{
  double          indinc;

  indinc = p->pop - p->ind;
  if (indinc > p->cap)
    indinc = p->cap;
  p->cap -= indinc;
  p->ind += indinc;
}

/********/


/****f* Phase/unloadcap
 * NAME
 *   unloadcap -- unload capital.
 * SYNOPSIS
 *   void unloadcap(planet *p, double x)
 * FUNCTION
 *   Unload capital unto a planet and if possible use it to
 *   increase the industry.
 * SOURCE
 */

void
unloadcap(planet *p, double x)
{
  p->cap += x;
  convertcap(p);
}

/*******/


/****f* Phase/unloadcol
 * NAME
 *   unloadcol -- unload colonists.
 * FUNCTION
 *   Unload colonists unto a planet defrost them and turn
 *   them into population.
 * SOURCE
 */

void
unloadcol(planet *p, double x, player *P)
{
  double          y;

  x *= POPPERCOL;
  y = p->size - p->pop;
  if (y > x)
    y = x;
  p->pop += y;
  x -= y;
  p->col += x / POPPERCOL;
  if (!p->owner)
    p->owner = P;
  convertcap(p);
}

/*******/


/****f* Phase/unloadgroup
 * NAME
 *   unloadgroup --
 * SYNOPSIS
 *   void unloadgroup(group *g, player *P, double amount)
 * FUNCTION
 * SOURCE
 */

void
unloadgroup(group *g, player *P, double amount)
{
  double          x;
  planet         *p;

  x = amount * g->ships;
  p = g->where;
  switch (g->loadtype) {
  case CG_CAP:
    unloadcap(p, x);
    break;
  case CG_MAT:
    p->mat += x;
    break;
  case CG_COL:
    unloadcol(p, x, P);
    break;
  }
  g->load -= amount;
  if (g->load < AMOUNTMIN) {
    g->loadtype = CG_EMPTY;
    g->load = 0.0;
  }
}

/****/



/****f* Phase/joinphase
 * NAME
 *   joinphase -- join similar groups into a single group.
 * SYNOPSIS
 *   void joinphase(game *aGame)
 *******
 */

void
joinphase(game *aGame)
{
  player         *P;
  group          *g, *g2, *g3;

  pdebug(DFULL, "Joinphase\n");
  for (P = aGame->players; P; P = P->next) {
    for (g = P->groups; g;) {
      if (g->ships) {
        for (g2 = P->groups; g2;) {
          g3 = g2->next;
          if (g != g2 &&
              g->thefleet == g2->thefleet &&
              g->type == g2->type &&
              g->drive == g2->drive &&
              g->weapons == g2->weapons &&
              g->shields == g2->shields &&
              g->cargo == g2->cargo &&
              g->loadtype == g2->loadtype &&
              g->load == g2->load &&
              g->from == g2->from &&
              g->where == g2->where &&
              g->flags == g2->flags && g->dist == g2->dist) {
            g->ships += g2->ships;
            remList(&P->groups, g2);
          }
          g2 = g3;
        }
        g = g->next;
      }
      else {
        g2 = g->next;
        remList(&P->groups, g);
        g = g2;
      }
    }
  }
}


/****f* Phase/kill
 * NAME
 *   kill --
 * NOTES
 *   Should be in battle.c I guess.
 *****
 */

int
kill(group *attack, group *target)
{
  double          defense, ratio;

  if (!attack->type->attacks)
    return 0;
  defense = calcdefense(target);
  if (!defense)
    return 1;
  ratio = attack->weapons * attack->type->weapons / defense;
  return ratio > pow(4.0, frand2());
}



/****f* Phase/bombphase
 * NAME
 *   bombphase -- bomb planets
 * SYNOPSIS
 *   void bombphase(game *aGame)
 * FUNCTION
 *   Examine which races have armed ships over a planet.
 *   If there are any, the planets are bombed and it is 
 *   determined who should own the planet.
 * SOURCE
 */

void
bombphase(game *aGame)
{
  player         *aPlayer;

  for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
    group          *attackGroup;

    for (attackGroup = aPlayer->groups;
         attackGroup; attackGroup = attackGroup->next) {
      if (mustBomb(aPlayer, attackGroup)) {
        planet         *targetPlanet;
        bombing        *aBombing;
        player         *viewingPlayer;

        targetPlanet = attackGroup->location;
        aBombing = allocStruct(bombing);

        setName(aBombing, targetPlanet->name);
        /* This should be a function */
        aBombing->where = targetPlanet;
        aBombing->owner = targetPlanet->owner;
        aBombing->pop = targetPlanet->pop;
        aBombing->ind = targetPlanet->ind;
        aBombing->producing = targetPlanet->producing;
        aBombing->producingshiptype = targetPlanet->producingshiptype;
        aBombing->cap = targetPlanet->cap;
        aBombing->mat = targetPlanet->mat;
        aBombing->col = targetPlanet->col;
        aBombing->viewers = NULL;

        for (viewingPlayer = aGame->players;
             viewingPlayer; viewingPlayer = viewingPlayer->next) {
          if (canseeplanet(viewingPlayer, targetPlanet)) {
            alliance       *a;
            a = allocStruct(alliance);

            a->who = viewingPlayer;
            addList(&aBombing->viewers, a);
          }
        }

        if (aGame->gameOptions.gameOptions & GAME_NONGBOMBING) {
          targetPlanet->owner = NULL;
        }
        else {
          targetPlanet->owner =
              determineOwnership(aGame, targetPlanet, aPlayer);
        }

        plog(LPART, "Race %s bombs planet %s\n",
             aPlayer->name, targetPlanet->name);

        if (targetPlanet->owner) {
          plog(LPART, "Ownership goes to %s\n", targetPlanet->owner->name);
          aBombing->who = targetPlanet->owner;
        }
        else {
          plog(LPART, "Ownership goes to nobody, planet neutral\n");
          aBombing->who = aPlayer;
        }
        addList(&(aGame->bombings), aBombing);

        bombPlanet(aGame, targetPlanet);

      }
    }
  }
}

/**********/


/****f* Phase/mustBomb
 * NAME
 *   mustBomb -- must a race bomb a planet?
 * SYNOPSIS
 *   int mustBomb(player *P, group *g)
 * FUNCTION
 *   Determine of a race is obliged to bomb a planet.
 * RESULT
 *   TRUE   -- yes the race should bomb.
 *   FALSE  -- no the race should not.
 * SOURCE
 */

int
mustBomb(player *P, group *g)
{
  int             must_bomb;

  must_bomb = g->location && g->location->owner && g->ships &&
      g->type->attacks && atwar(P, g->location->owner);
  return must_bomb;
}

/*****/


/****f* Phase/bombPlanet
 * NAME
 *   bombPlanet -- bomb a planet.
 * FUNCTION
 *   Depending on the preferences of the GM all live and industry
 *   on the planet is 
 *   (a) completely destroyed, cap and mat are wiped out too.
 *   (b) reduced to 25%,  cap and mat are untouched.
 * SOURCE
 */

void
bombPlanet(game *aGame, planet *p)
{
  p->col = 0;
  p->producing = PR_CAP;
  p->producingshiptype = 0;
  p->inprogress = 0;
  memset(p->routes, 0, sizeof(p->routes));

  if (aGame->gameOptions.gameOptions & GAME_NONGBOMBING) {
    p->pop = 0;
    p->ind = 0;
    p->cap = 0;
    p->mat = 0;
    p->spent = 0;
  }
  else {
    p->pop = p->pop * 0.25;
    p->ind = p->ind * 0.25;
    p->spent = 0;
    convertcap(p);
  }
}

/*********/


/****f* Phase/determineOwnership
 * NAME
 *   determineOwnership -- determine ownership of planet after bombing.
 * FUNCTION
 *   Determines which race should get a planet after it is bombed.
 ******
 */

player         *
determineOwnership(game *aGame, planet *targetPlanet, player *aPlayer)
{
  player         *otherPlayer;
  int             noClaims;
  player         *claimingPlayer;
  group          *aGroup;
  int             standoff;

  /* Check if the owner of the planet has any ships left that can fire, *
   * * * * * * * * * in that case there is a standoff */
  for (standoff = FALSE, aGroup = targetPlanet->owner->groups;
       aGroup && !standoff; aGroup = aGroup->next) {
    if (aGroup->location eq targetPlanet && aGroup->type->attacks
        && aGroup->ships)
      standoff = TRUE;
  }

  claimingPlayer = (player *) NULL;
  noClaims = 0;

  if (!standoff) {
    claimingPlayer = aPlayer;
    for (otherPlayer = aGame->players, noClaims = 0;
         otherPlayer; otherPlayer = otherPlayer->next) {
      planet_claim   *curClaim;
      int             skipRest;

      for (skipRest = FALSE, curClaim = otherPlayer->claimed_planets;
           curClaim && !skipRest; curClaim = curClaim->next) {
        if (curClaim->planet_claimed eq targetPlanet) {
          plog(LPART, "%s claims planet\n", otherPlayer->name);
          for (aGroup = otherPlayer->groups;
               aGroup && !skipRest; aGroup = aGroup->next) {
            if (aGroup->location eq targetPlanet &&
                mustBomb(otherPlayer, aGroup)) {
              claimingPlayer = otherPlayer;
              plog(LPART, "%s claim is valid\n", otherPlayer->name);
              noClaims++;
              skipRest = TRUE;
            }
          }
        }
      }
    }
  }
  return (noClaims <= 1 && !standoff) ? claimingPlayer : NULL;
}




/****f* Phase/loadphase
 * NAME
 *   loadphase -- upload cargo from planet onto ships
 * FUNCTION
 *   Takes care of all cargo routes. 
 *******
 */

void
loadphase(game *aGame)
{
  player         *P;
  group          *g, *g2;
  int             i;
  planet         *p;
  double          qty, shipspace, availspace;

  pdebug(DFULL, "Load phase\n");

  for (p = aGame->planets; p; p = p->next)
    if ((P = p->owner) != 0) {
      if (p->routes[CG_COL])
        for (g = P->groups; g; g = g->next)
          if (g->where == p && g->type->cargo && g->dist == 0 &&
              g->type->drive > 0 && !g->thefleet) {
            if (g->loadtype == CG_EMPTY && p->col) {
              qty = p->col;
              shipspace = cargospace(g);
              availspace = g->ships * shipspace;
              if (qty > availspace)
                qty = availspace;
              p->col -= qty;
              availspace -= qty;
              i = availspace / shipspace;
              if (i) {
                g->ships -= i;
                g2 = allocStruct(group);

                *g2 = *g;
                numberGroup(P, g2);
		g2->name = (char*)malloc(8);
		sprintf(g2->name, "%d", g2->number);
		g2->next = NULL;
                addList(&P->groups, g2);
                g2->ships = i;
              }
              g->loadtype = CG_COL;
              g->load = qty / g->ships;
            }
            if (g->loadtype == CG_COL && g->type->drive)
              send(aGame, g, p->routes[CG_COL]);
          }
      if (p->routes[CG_CAP])
        for (g = P->groups; g; g = g->next)
          if (g->where == p && g->type->cargo && g->dist == 0 &&
              g->type->drive && !g->thefleet) {
            if (g->loadtype == CG_EMPTY && p->cap) {
              qty = p->cap;
              shipspace = cargospace(g);
              availspace = g->ships * shipspace;
              if (qty > availspace)
                qty = availspace;
              p->cap -= qty;
              availspace -= qty;
              i = availspace / shipspace;
              if (i) {
                g->ships -= i;
                g2 = allocStruct(group);

                *g2 = *g;
                numberGroup(P, g2);
		g2->name = (char*)malloc(8);
		sprintf(g2->name, "%d", g2->number);
		g2->next = NULL;
                addList(&P->groups, g2);
                g2->ships = i;
              }
              g->loadtype = CG_CAP;
              g->load = qty / g->ships;
            }
            if (g->loadtype == CG_CAP && g->type->drive)
              send(aGame, g, p->routes[CG_CAP]);
          }
      if (p->routes[CG_MAT])
        for (g = P->groups; g; g = g->next)
          if (g->where == p && g->type->cargo && g->dist == 0 &&
              g->type->drive && !g->thefleet) {
            if (g->loadtype == CG_EMPTY && p->mat) {
              qty = p->mat;
              shipspace = cargospace(g);
              availspace = g->ships * shipspace;
              if (qty > availspace)
                qty = availspace;
              p->mat -= qty;
              availspace -= qty;
              i = availspace / shipspace;
              if (i) {
                g->ships -= i;
                g2 = allocStruct(group);

                *g2 = *g;
                numberGroup(P, g2);
		g2->name = (char*)malloc(8);
		sprintf(g2->name, "%d", g2->number);
		g2->next = NULL;
		
                addList(&P->groups, g2);
                g2->ships = i;
              }
              g->loadtype = CG_MAT;
              g->load = qty / g->ships;
            }
            if (g->loadtype == CG_MAT && g->type->drive)
              send(aGame, g, p->routes[CG_MAT]);
          }
      if (p->routes[CG_EMPTY])
        for (g = P->groups; g; g = g->next)
          if (g->where == p && g->type->cargo && g->dist == 0 &&
              g->loadtype == CG_EMPTY && !g->thefleet && g->type->drive)
            send(aGame, g, p->routes[CG_EMPTY]);
    }
}



/****f* Phase/interceptphase
 * NAME
 *   interceptphase -- compute the destination of intercepting groups and
 *                     fleets.
 * SYNOPSIS
 *   void interceptphase(game *aGame)
 * FUNCTION
 *   Compute the destination of all groups and fleets that have been 
 *   given the intercept command.
 * NOTES
 *   This does not work perfectly, groups can end up unexpected locations.
 * SOURCE
 */

void
interceptphase(game *aGame)
{
	player* curPlayer;
	player* otherPlayer;
	double* massPerPlanet;
	int     noPlanets;

	pdebug(DFULL, "Intercept Phase\n");

	noPlanets = numberOfElements(aGame->planets);
	massPerPlanet = (double *) alloc((noPlanets + 1) * sizeof(double));

	for (curPlayer = aGame->players; curPlayer; curPlayer = curPlayer->next) {
		group* inGroup;

		for (inGroup = curPlayer->groups; inGroup; inGroup = inGroup->next) {
			if (inGroup->flags & GF_INTERCEPT) {
				double maxDist;
				double maxMass;
				planet* inPlanet;
				planet* targetPlanet;
				planet* curPlanet;
				int     planetIndex;

				if (inGroup->thefleet)
					maxDist = 2 * inGroup->thefleet->fleetspeed;
				else
					maxDist = 2 * groupSpeed(inGroup);

				inPlanet = inGroup->where;
				if (inGroup->thefleet) {
					pdebug(DFULL, "Fleet %s Intercept on Planet %s max Dist %f\n",
						   inGroup->thefleet->name, inPlanet->name, maxDist);
				}
				else {
					pdebug(DFULL, "Group %d Intercept on Planet %s max Dist %f\n",
						   inGroup->number, inPlanet->name, maxDist);
				}
				/* 
				 * Compute the total mass per destination planet, of all groups departing
				 * from the planet the intercept command was issued on.  Destination
				 * planet have to be with in two turn range. */
				memset(massPerPlanet, 0, (noPlanets + 1) * sizeof(double));
				
				for (otherPlayer = aGame->players;
					 otherPlayer; otherPlayer = otherPlayer->next) {
					if (otherPlayer != curPlayer) {
						group          *aGroup;
						
						for (aGroup = otherPlayer->groups;
							 aGroup; aGroup = aGroup->next) {
							if ((aGroup->dist) &&
								(aGroup->location eq inPlanet) &&
								(dist(aGame, aGroup->where, inGroup->location) <
								 maxDist)) {
								massPerPlanet[ptonum(aGame->planets, aGroup->where)] +=
									aGroup->ships * shipmass(aGroup);
							}
						}
					}
				}
				/* 
				 * Find the destination planet of the largest outgoing mass.  */
				targetPlanet = NULL;
				for (curPlanet = aGame->planets, planetIndex = 1, maxMass = 0;
					 curPlanet; planetIndex++, curPlanet = curPlanet->next) {
					assert(planetIndex < (noPlanets + 1));
					if (massPerPlanet[planetIndex] > maxMass) {
						targetPlanet = curPlanet;
						maxMass = massPerPlanet[planetIndex];
					}
				}
				
				if (targetPlanet) {
					pdebug(DFULL, "Result: Planet %s (%f ly away).\n",
						   targetPlanet->name, dist(aGame, inGroup->where,
													targetPlanet));
					inGroup->where = inGroup->from;
					send(aGame, inGroup, targetPlanet);
				}
                /* Remove the intercept flag. */ 
				inGroup->flags &= ~GF_INTERCEPT;        
			}
		}
	}
	free(massPerPlanet);
}

/*********/



/****f* Phase/fleetphase 
 * NAME
 *   fleetphase -- update fleet data
 * FUNCTION
 *   Remove fleets that have no groups left.  Recompute the speed of
 *   each fleet. The speed might have changed due to upgrades or loss
 *   of slow ships in battles.
 * SEE ALSO
 *   fleetSpeed()
 * SOURCE
 */

void
fleetphase(game *aGame)
{
  player         *P;
  fleetname      *fl, *fl2;
  group          *g;

  /* double shipspeed; */
  int             hasships;

  pdebug(DFULL, "Fleet Phase\n");

  for (P = aGame->players; P; P = P->next) {
    for (fl = P->fleetnames; fl; fl = fl2) {
      fl2 = fl->next;
      hasships = FALSE;
      for (g = P->groups; g; g = g->next) {
        if (g->thefleet eq fl) {
          fl->fleetspeed = groupSpeed(g);       /* This can be * * removed 
                                                 *
                                                 * * I * * * think! FS */
          hasships = TRUE;
          break;
        }
      }
      fl->fleetspeed = fleetSpeed(fl, P->groups);       /* CB-19980922 * */
      pdebug(DFULL, "Fleet %s  speed %f\n", fl->name, fl->fleetspeed);
      if (!hasships) {
        pdebug(DFULL, "Removing Fleet %s\n", fl->name);
        remList(&P->fleetnames, fl);
      }
    }
  }
}

/***********/


/****f* Phase/movephase
 * NAME
 *   movephase -- ships move through hyperspace
 * FUNCTION
 *   Advance all ships (groups and fleets) through hyperspace. 
 *   The distance they travel depend on the drive, drive tech,
 *   and total mass of the ship (including cargo).
 * NOTES
 *   Should use groupSpeed() and fleetSpeed()
 * SEE ALSO
 *   shipmass() 
 ******
 */

void
movephase(game *aGame)
{
  player         *P;
  group          *g;

  pdebug(DFULL, "Move Phase\n");

  for (P = aGame->players; P; P = P->next) {
    for (g = P->groups; g; g = g->next) {
      if (g->thefleet)
        g->dist -= g->thefleet->fleetspeed;
      else
        g->dist -= g->type->drive * g->drive * DRIVEMAGIC / shipmass(g);
      if (g->dist < 0.01) {
        g->dist = 0;
        g->from = g->where;
      }
    }
  }
}


/****f* Phase/unloadphase
 * NAME
 *   unloadphase -- unload cargo unto planet
 * SOURCE
 */

void
unloadphase(game *aGame)
{
  planet         *p, *p2;
  player         *cur_player;
  group          *g;
  int             i;

  pdebug(DFULL, "Unload Phase\n");

  /* Auto Unload */

  for (p = aGame->planets; p; p = p->next) {
    for (cur_player = aGame->players;
         cur_player; cur_player = cur_player->next) {
      if (cur_player->flags & F_AUTOUNLOAD) {
        for (g = cur_player->groups; g; g = g->next) {
          if (g->where == p && g->dist == 0 && g->ships) {
            if ((p->owner && p->owner == cur_player) || !p->owner) {
              unloadgroup(g, cur_player, g->load);
            }
          }
        }
      }
    }
  }

  /* Routes */

  for (p = aGame->planets; p; p = p->next) {
    if (p->owner) {
      for (i = 0; i != CG_EMPTY; i++) {
        p2 = p->routes[i];
        for (g = p->owner->groups; g; g = g->next) {
          if (g->where == p2 &&
              g->dist == 0 && g->loadtype == i && g->ships) {
            unloadgroup(g, p->owner, g->load);
          }
        }
      }
    }
  }
}

/*********/


/****f* Phase/ftrunc2
 * NAME
 *   ftrunc2 -- clip a number to two decimal points. 
 * SYNOPSIS
 *   double ftrunc2(double x)
 * NOTES
 *   Should use lineBuffer.
 * SOURCE
 */

double
ftrunc2(double x)
{
  char            buf[32];

  sprintf(buf, "%0.2f", x);
  return atof(buf);
}

/******/


/****f* Phase/cktech
 * NAME
 *   cktech -- check tech levels of a group.
 * FUNCION
 *   Clip all tech levels of a group to two decimal points.
 *   Set tech levels of zero components to zero.
 * NOTES
 *   This should be in util.c
 * SOURCE
 */

void
cktech(group *g)
{
  shiptype       *t;

  t = g->type;
  g->drive = ftrunc2(g->drive);
  g->weapons = ftrunc2(g->weapons);
  g->shields = ftrunc2(g->shields);
  g->cargo = ftrunc2(g->cargo);
  if (!t->drive)
    g->drive = 0;
  if (!t->attacks)
    g->weapons = 0;
  if (!t->shields)
    g->shields = 0;
  if (!t->cargo)
    g->cargo = 0;
}

/*********/


/****f* Phase/producephase
 * NAME
 *   producephase -- planets produce their products.
 * FUNCTION
 *   Iterate over all planets and let each planet produce its thing.
 *   Also takes care of the population and colonist increase.
 * SOURCE
 */

void
producephase(game *aGame)
{
  planet         *aPlanet;

  pdebug(DFULL, "Produce Phase\n");

  for (aPlanet = aGame->planets; aPlanet; aPlanet = aPlanet->next) {
    if (aPlanet->owner) {
      double          industry;

      switch (aPlanet->producing) {
      case PR_SHIP:
        industry =
            aPlanet->ind * .75 + aPlanet->pop * .25 - aPlanet->spent +
            aPlanet->inprogress;
        aPlanet->spent = 0.0;
        produceShip(aPlanet, industry);
        break;
      case PR_MAT:
      case PR_CAP:
        industry =
            aPlanet->ind * .75 + aPlanet->pop * .25 - aPlanet->spent +
            aPlanet->inprogress;
        aPlanet->spent = 0.0;
        produceMatCap(aPlanet, industry);
        break;
      }
    }
  }

  for (aPlanet = aGame->planets; aPlanet; aPlanet = aPlanet->next) {
    if (aPlanet->owner) {
      double          industry;

      switch (aPlanet->producing) {
      case PR_DRIVE:
      case PR_WEAPONS:
      case PR_SHIELDS:
      case PR_CARGO:
        industry =
            aPlanet->ind * .75 + aPlanet->pop * .25 - aPlanet->spent +
            aPlanet->inprogress;
        aPlanet->spent = 0.0;
        produceTechnology(aPlanet, industry);
        break;
      }
      producePopulation(aPlanet);
    }
  }
}

/********/


/****f* Phase/produceShip
 * NAME
 *   produceShip -- try to produce some ships.
 * FUNCTION
 *   Computes based on the amount of industry (points) how many ships
 *   can be build. If this is 1 or more the ships are produces and a
 *   new group with these ships is created. If it is less than 1 the
 *   production is delayed to the next turn.
 * NOTE
 *   Because of MAT the expression to compute the number of ships 
 *   that can be produced is a tricky one.
 * SOURCE
 */

void
produceShip(planet *aPlanet, double industry)
{
  int             n, numberOfShips;
  shiptype       *typeProduced;
  double          typeMass, indForProduction, indForMaterials;

  typeProduced = aPlanet->producingshiptype;
  typeMass = typemass(typeProduced);
  if (typeMass > 49.5)
    typeMass = typeMass - 0.01;   /* Fudge Factor, cause people keep on
								   * building those 99.01 ships and then
								   * complain they get not built */

  for (n = (int) (industry / (typeMass * INDPERSHIP)) + 1, numberOfShips =
       0; n >= 0; n--) {
    indForProduction = n * typeMass * INDPERSHIP;
    indForMaterials = (n * typeMass - aPlanet->mat) / aPlanet->resources;
    if (indForMaterials < 0)
      indForMaterials = 0;
    if (indForProduction + indForMaterials <= industry) {
      numberOfShips = n;
      break;
    }
  }

  if (numberOfShips == 0) {
    /* Delay Construction to next turn */
    aPlanet->inprogress = industry;
  }
  else {
    group          *aGroup;
    player         *aPlayer;

    aPlayer = aPlanet->owner;
    indForProduction = numberOfShips * typeMass * INDPERSHIP;
    indForMaterials = (numberOfShips * typeMass - aPlanet->mat)
        / aPlanet->resources;
    if (indForMaterials < 0) {
      indForMaterials = 0;
      aPlanet->mat -= numberOfShips * typeMass;
    }
    else {
      aPlanet->mat = 0;
    }
    industry -= indForProduction + indForMaterials;
    aPlanet->inprogress = industry;
    /* This Should be a function */
    aGroup = allocStruct(group);

    aGroup->ships = numberOfShips;
    aGroup->type = typeProduced;
    aGroup->drive = aPlayer->drive;
    aGroup->weapons = aPlayer->weapons;
    aGroup->shields = aPlayer->shields;
    aGroup->cargo = aPlayer->cargo;
    cktech(aGroup);
    aGroup->load = 0;
    aGroup->loadtype = CG_EMPTY;
    aGroup->from = aGroup->where = aPlanet;
    aGroup->dist = 0;
    aGroup->thefleet = NULL;
    aGroup->defense = 0.0;
    aGroup->attack = 0.0;
    aGroup->flags = 0;
    aGroup->location = aPlanet;
    numberGroup(aPlayer, aGroup);
    aGroup->name = (char*)malloc(8);
    sprintf(aGroup->name, "%d", aGroup->number);
    aGroup->next = NULL;
    addList(&aPlayer->groups, aGroup);
    aPlayer->massproduced += numberOfShips * typemass(typeProduced);
  }
}

/*******/


/****f* Phase/produceMatCap
 * NAME
 *   produceMatCap -- Produce CAP or MAT.
 * NOTE
 *   The function to compute the CAP increase is a tricky one.
 * SOURCE
 */

void
produceMatCap(planet *aPlanet, double industry)
{
  double          materialDemand;

  switch (aPlanet->producing) {
  case PR_CAP:
    materialDemand = industry / INDPERCAP;      /* 1 unit of MAT per 1 CAP 
                                                 *
                                                 * * *  * *  */
    if (materialDemand > aPlanet->mat)
      materialDemand = aPlanet->mat;
    industry -= materialDemand * INDPERCAP;
    aPlanet->cap += materialDemand;
    aPlanet->mat -= materialDemand;
    aPlanet->cap += industry / (INDPERCAP + 1 / aPlanet->resources);
    break;
  case PR_MAT:
    aPlanet->mat += industry * aPlanet->resources;
    break;
  default:
    assert(0);
  }
  aPlanet->inprogress = 0.0;
}

/*******/



/****f* Phase/produceTechnology
 * NAME
 *   produceTechnology -- produce different kind of technologies.
 * SOURCE
 */

void
produceTechnology(planet *aPlanet, double industry)
{
  player         *aPlayer;

  aPlayer = aPlanet->owner;
  switch (aPlanet->producing) {
  case PR_DRIVE:
    aPlayer->drive += industry / 5000.0;
    plog(LFULL, "Producing DRIVE %lf\n", industry / 5000.0);
    break;
  case PR_WEAPONS:
    aPlayer->weapons += industry / 5000.0;
    plog(LFULL, "Producing WEAPONS %lf\n", industry / 5000.0);
    break;
  case PR_SHIELDS:
    aPlayer->shields += industry / 5000.0;
    plog(LFULL, "Producing SHIELDS %lf\n", industry / 5000.0);
    break;
  case PR_CARGO:
    aPlayer->cargo += industry / 2500.0;
    plog(LFULL, "Producing CARGO %lf\n", industry / 2500.0);
    break;
  default:
    assert(0);
  }
  aPlanet->inprogress = 0.0;
}

/********/


/****f* Phase/producePopulation
 * NAME
 *   producePopulation -- growth of population and colonists.
 * FUNCTION
 *   Lets the population of a planet grow. Excess population
 *   (growth beyond the size of the planet) is turned into colonists
 *   by condensing a number of persons into one colonist.
 * SOURCE
 */

void
producePopulation(planet *aPlanet)
{
  double          popinc, poproom;

  if (aPlanet->owner) {
    popinc = aPlanet->pop * POPINC;
    poproom = aPlanet->size - aPlanet->pop;
    if (poproom > popinc)
      poproom = popinc;
    aPlanet->pop += poproom;
    aPlanet->col += (popinc - poproom) / POPPERCOL;
    convertcap(aPlanet);
  }
}

/*************/


/****i* Phase/nametocargotype
 * NAME
 *   nametocargotype --
 * SYNOPSIS
 *   int nametocargotype(char *s) 
 * NOTES
 *   Should be in Util
 *****
 */

int
nametocargotype(char *s)
{
  int             i;
  static char    *cargonames[] = {
    "cap", "mat", "col", "emp",
  };

  for (i = 0; i != MAXCARGO; i++)
    if (!noCaseStrcmp(cargonames[i], s))
      return i;
  return -1;
}


/*****f* Phase/sortphase
 * NAME
 *   sortphase -- renumber the groups
 * SYNOPSIS
 *   void sortphase(game *aGame)
 * FUNCTION
 *   Renumber the groups such that they are sorted according
 *   to planet. In particular they are number in the following
 *   order:
 *     (1) groups at planets owned by the race,
 *     (2) groups at planets owned by other races,
 *     (3) groups at all other planets, 
 *     (4) groups that are part of a fleet.
 * SOURCE
 */

void
sortphase(game *aGame)
{
  player         *P;
  group          *g;
  planet         *p;
  fleetname      *fl;
  int             n;

  pdebug(DFULL, "Sort Phase\n");

  for (P = aGame->players; P; P = P->next) {
    n = 1;
    if (P->flags & F_SORTGROUPS) {
      /* Groups at planets owned by the race */
      for (p = aGame->planets; p; p = p->next) {
        if (p->owner == P) {
          for (g = P->groups; g; g = g->next) {
            if (g->location == p && !g->thefleet) {
              g->number = n;
              n++;
            }
          }
          for (g = P->groups; g; g = g->next) {
            if (!g->location && g->where == p && !g->thefleet) {
              g->number = n;
              n++;
            }
          }
        }
      }
      /* Groups at planets owned by other races */
      for (p = aGame->planets; p; p = p->next) {
        if (p->owner && p->owner != P) {
          for (g = P->groups; g; g = g->next) {
            if (g->location == p && !g->thefleet) {
              g->number = n;
              n++;
            }
          }
          for (g = P->groups; g; g = g->next) {
            if (!g->location && g->where == p && !g->thefleet) {
              g->number = n;
              n++;
            }
          }
        }
      }
      /* Groups at all other planets */
      for (p = aGame->planets; p; p = p->next) {
        if (!p->owner) {
          for (g = P->groups; g; g = g->next) {
            if (g->location == p && !g->thefleet) {
              g->number = n;
              n++;
            }
          }
          for (g = P->groups; g; g = g->next) {
            if (!g->location && g->where == p && !g->thefleet) {
              g->number = n;
              n++;
            }
          }
        }
      }

      /* Groups that are part of fleets */
      for (fl = P->fleetnames; fl; fl = fl->next) {
        for (g = P->groups; g; g = g->next) {
          if (g->thefleet eq fl) {
            g->number = n;
            n++;
          }
        }
      }
    }
  }
}

/******/
