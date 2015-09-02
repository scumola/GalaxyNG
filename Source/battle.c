#include "battle.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "list.h"
#include "util.h"

/****h* GalaxyNG/Battle
 * FUNCTION
 *   This module contains the battle simulator.
 ******
 */

char           *vbattle =
    "$Id: battle.c,v 1.7 2004/09/06 12:03:37 madcoder42 Exp $";

/****f* Battle/fightphase
 * NAME
 *   fightphase
 * SYNOPSIS
 *   void fightphase(game *aGame, int phase) 
 * FUNCTION
 *   Itterates over all planets and checks where any battles
 *   need to take place. Allocates and initializes the necessary
 *   battle structures and calls the function to carry out the battle.
 *
 *   The code works on three basic structures:
 *     battle       keeps track of each battle.
 *     participant  keeps track of the participants in each battle, contains
 *                  the groups that take part in the battle.
 *     batstat      keeps track of the battle statistics during a battle.
 *     group        keeps track of which ship in the group fired, which
 *                  have been shot, and how many there are left that can fire.
 * INPUTS
 *   aGame --
 *   phase -- there are two fight phases, this tells which phase we are
 *            in. Is used for the battle protocol option.
 * SOURCE
 */

void
fightphase(game *aGame, int phase)
{
  planet         *p;

  pdebug(DFULL, "fightphase\n");
  plog(LFULL, "Checking for battles\n");
  for (p = aGame->planets; p; p = p->next) {
    battle         *b;

    if ((b = isBattle(aGame->players, p))) {
      batstat        *batstats;
      batstat        *aBatstat;
      participant    *part;

      plog(LFULL, "Battle at %s\n", p->name);
      pdebug(DFULL2, "Battle at %s\n", p->name);
      b->phase = phase;
      batstats = NULL;
      for (part = b->participants; part; part = part->next) {
        aBatstat = allocStruct(batstat);

        aBatstat->who = part->who;
        aBatstat->groups = part->groups;
        addList(&batstats, aBatstat);
      }

      allocEnemies(batstats);
      doBattle(batstats, b->protocol, phase);
      addList(&(aGame->battles), b);
      updateGroups(b);

      for (aBatstat = batstats; aBatstat; aBatstat = aBatstat->next) {
        free(aBatstat->enemies);
      }
      freelist(batstats);
    }
  }
}

/***** END fightphase ****/



/****f* Battle/doBattle
 * NAME
 *   doBattle --
 * FUNCTION
 *  Repeat the following until there is a draw or a win:
 *   (1) Repeat until all ships have fired.
 *   (1.1) Select randomly a ship from all ships that have not fired yet 
 *   (1.2) For each gun of the ship
 *       (1.2a) Select from the enemies of the owner of the ship, randomly
 *              a ship.
 *       (1.2b) attack this ship.
 *  
 * SOURCE
 */

void
doBattle(batstat *batstats, bprotocol *aProtocol, int phase)
{
  int             attackingShip;
  int             targetShip;
  group          *attackingGroup;
  group          *targetGroup;
  batstat        *attackingSide;
  batstat        *targetSide;
  int             gun;

  pdebug(DFULL, "doBattle\n");

  resetSides(batstats);
  while (!isDraw(batstats) && !isWon(batstats)) {
    while (attackersLeft(batstats)) {
      attackingGroup =
          selectAttackingGroup(batstats, &attackingSide, &attackingShip);
      assert(attackingGroup);
      attackingGroup->flags |= phase;

      for (gun = 0; gun < attackingGroup->type->attacks; gun++) {
        targetGroup =
            selectTargetGroup(attackingSide, &targetSide, &targetShip);
        if (targetGroup) {
          targetGroup->flags |= phase;
          attack(aProtocol,
                 attackingSide, attackingGroup, attackingShip,
                 targetSide, targetGroup, targetShip);
        }
      }
    }
    resetSides(batstats);
  }
}

/**** END doBattle ***/


/****f* Battle/attack
 * NAME
 *   attack -- let one ship fire on another ship
 * FUNCTION
 *   Let one ship fire on another ship and if the ship got destroyed update
 *   the battle statistics.
 * INPUTS
 *   aProtocol      -- battle protocol structure to store info about
 *                     who fired on who
 *   attackingSide  -- batstat of the side that own the attacking ship
 *   attackingGroup -- group the attacking ship belongs to
 *   attackingShip  -- number of the ship within the group
 *   targetSide     -- batstat of the side that own the target ship
 *   targetGroup    -- group the target ship belongs to
 *   targetShip     -- number of the ship within the group
 * SEE ALSO
 *   shoot()
 * SOURCE
 */

void
attack(bprotocol *aProtocol,
       batstat *attackingSide, group *attackingGroup, int attackingShip,
       batstat *targetSide, group *targetGroup, int targetShip)
{

  if (shoot(attackingGroup, targetGroup)) {
    targetGroup->left--;
    (targetGroup->alive)[targetShip] = FALSE;
    if (targetGroup->type->weapons) {
      if ((targetGroup->canshoot)[targetShip]) {
        (targetGroup->canshoot)[targetShip] = FALSE;
        targetGroup->numberOfAttackersLeft--;
        targetSide->numberOfAttackersLeft--;
      }
    }
    addShot(aProtocol, attackingSide->who, attackingGroup->type,
            targetSide->who, targetGroup->type, 1);
    targetSide->numberOfTargets--;
    targetSide->who->masslost += typemass(targetGroup->type);
  }
  else {
    addShot(aProtocol, attackingSide->who, attackingGroup->type,
            targetSide->who, targetGroup->type, 0);
  }
}

/**** END attack ****/



/****f* Battle/shoot
 * NAME
 *   shoot -- does a ship surive a shot? 
 * SYNOPSIS
 *   int shoot(group *attacker, group *defender)
 * FUNCTION
 *   Let one ship fire an other and determine whether the ship 
 *   got killed.
 * RESULT
 *   TRUE -- ship got killed
 *   FALSE -- ship survived
 * NOTES
 *   The name of this function is wrong.
 * SEE ALSO
 *   attack()
 * SOURCE
 */

int
shoot(group *attacker, group *defender)
{
  double          defense;

  assert(attacker->type->weapons > 0);
  defense = defender->defense;
  return ((defense > 0) ?
          ((attacker->attack / defense) > pow(4.0, frand2())) : 1);
}

/***** END of shoot *******/



/****f* Battle/isWon
 * NAME
 *   isWon -- is a battle is won?
 * FUNCTION
 *   Determine if the battle is won by any of the sides.
 *   A battle is won if the battle is not a draw and none of
 *   the sides in the battle has any targets left.
 * SOURCE
 */

int
isWon(batstat *aBatstat)
{
  int             iswon = TRUE;

  for (; aBatstat && iswon; aBatstat = aBatstat->next) {
    int             enemy;

    if (aBatstat->numberOfAttackers) {
      for (enemy = 0; enemy < aBatstat->noEnemies; enemy++) {
        if ((aBatstat->enemies)[enemy]->numberOfTargets) {
          iswon = FALSE;
          break;
        }
      }
    }
  }

  return iswon;
}

/**** END OF isWon *****/


/****f* Battle/attackersLeft
 * NAME
 *   attackersLeft -- are there any ships that haven't fired yet?
 * FUNCTION
 *   Scan's all batstats to find ships that haven't fired yet.
 *   If one is found return TRUE. 
 * INPUTS
 *   aBatstat -- pointer to the first batstat in list of batstats.
 * RESULT
 *   TRUE  -- There are ships left that can fire
 *   FALSE -- All ships have fired their guns.
 * SOURCE
 */

int
attackersLeft(batstat *aBatstat)
{
  for (; aBatstat; aBatstat = aBatstat->next) {
    if (aBatstat->numberOfAttackersLeft) {
      return TRUE;
    }
  }
  return FALSE;
}

/******/


/****f* Battle/isDraw
 * NAME
 *   isDraw -- is a battle a draw?
 * SYNOPSIS
 *   int isDraw(batstat *batstats)
 * FUNCTION
 *   Determines whether a battle is a draw or not.  This is the case
 *   for each of the participants their biggest gun is not strong
 *   enough the destroy the enemy ship with the smallest shield.
 * INPUTS
 *   batstats -- first batstat in the list of batstats.
 * RESULT
 *   TRUE  -- the battle is a draw
 *   FALSE -- otherwise
 * SOURCE
 */

int
isDraw(batstat *batstats)
{
  int             draw;
  batstat        *aBatstat;

  pdebug(DFULL, "isDraw\n");


  for (aBatstat = batstats; aBatstat; aBatstat = aBatstat->next) {
    double          maxAttack, curAttack, minDefense, curDefense;
    group          *aGroup;

    maxAttack = 0.0;
    minDefense = -1.0;
    for (aGroup = aBatstat->groups; aGroup; aGroup = aGroup->next) {
      if (aGroup->left) {
        curAttack = aGroup->attack;
        if (curAttack > maxAttack)
          maxAttack = curAttack;
        curDefense = aGroup->defense;
        if (minDefense < 0)
          minDefense = curDefense;
        else if (curDefense < minDefense)
          minDefense = curDefense;
      }
    }
    aBatstat->maxAttack = maxAttack;
    aBatstat->minDefense = minDefense;
  }

  draw = TRUE;
  for (aBatstat = batstats; aBatstat; aBatstat = aBatstat->next) {
    int             enemy;

    for (enemy = 0; enemy < aBatstat->noEnemies; enemy++) {
      batstat        *targetSide;

      targetSide = aBatstat->enemies[enemy];
      if (targetSide->minDefense >= 0) {
        if (targetSide->minDefense / 4.0 < aBatstat->maxAttack) {
          draw = FALSE;
        }
      }
    }
  }
  return draw;
}

/*** END OF isDraw ***/


/****f* Battle/resetSides
 * NAME
 *   resetSides -- reset all groups for another round of fighting
 * FUNCTION
 *   Reset the counters in all batstats so that all ships can fire their
 *   guns ones more.  Also computes the number of targets.
 * INPUTS
 *   batstats -- first batstat in the list of batstats.
 * SOURCE
 */

void
resetSides(batstat *aBatstat)
{
  for (; aBatstat; aBatstat = aBatstat->next) {
    group          *aGroup;

    aBatstat->numberOfAttackers = 0;
    aBatstat->numberOfTargets = 0;
    for (aGroup = aBatstat->groups; aGroup; aGroup = aGroup->next) {
      int             ship;

      if (aGroup->type->attacks) {
        for (ship = 0; ship < aGroup->left; ship++) {
          (aGroup->canshoot)[ship] = TRUE;
        }
        aGroup->numberOfAttackers = aGroup->left;
        aGroup->numberOfAttackersLeft = aGroup->left;
        aBatstat->numberOfAttackers += aGroup->left;
      }
      else {
        aGroup->numberOfAttackers = 0;
        aGroup->numberOfAttackersLeft = 0;
      }
      for (ship = 0; ship < aGroup->left; ship++) {
        (aGroup->alive)[ship] = TRUE;
      }
      for (; ship < aGroup->ships; ship++) {
        (aGroup->alive)[ship] = FALSE;
      }
      aBatstat->numberOfTargets += aGroup->left;
    }
    aBatstat->numberOfAttackersLeft = aBatstat->numberOfAttackers;
  }
}

/*****/


/****f* Battle/updateGroups
 * NAME
 *   updateGroups -- count the losses.
 * FUNCTION
 *   Updates all groups that took part in a battle
 * SOURCE
 */

void
updateGroups(battle *b)
{
  participant    *part;

  pdebug(DFULL, "updateGroups\n");
  for (part = b->participants; part; part = part->next) {
    group          *g, *g2;

    for (g = part->who->groups, g2 = part->groups; g; g = g->next) {
      if (g->location == b->where) {
        assert(g2 != NULL);
        assert(g->where == g2->where);
        assert(g->type == g2->type);
        g->ships = g2->left;
        g2 = g2->next;
      }
    }
  }
}

/****** END updateGroups *****/




/****f* Battle/selectAttackingGroup
 * NAME
 *   selectAttackingGroup -- select a group and a ship in this group
 * FUNCTION
 *   Randomly select the next ship that is going to attack.
 *   This ship is selected from all the ships that have not fired yet.
 * SOURCE
 */

group          *
selectAttackingGroup(batstat *batstats, batstat **attackingSide, int *ship)
{
  int             totalNumberOfAttackersLeft;
  batstat        *aBatstat;

  /* Compute the number of ships that haven't fired yet. */
  totalNumberOfAttackersLeft = 0;
  for (aBatstat = batstats; aBatstat; aBatstat = aBatstat->next) {
    totalNumberOfAttackersLeft += aBatstat->numberOfAttackersLeft;
  }

  if (totalNumberOfAttackersLeft > 0) {
    int             lowerLim;
    int             upperLim;
    int             n;

    lowerLim = 0;
    /* Randomly select one of these ships */
    n = frand3(totalNumberOfAttackersLeft);

    /* Find the the owner and the group the ship belongs to. */
    for (aBatstat = batstats; aBatstat; aBatstat = aBatstat->next) {
      upperLim = lowerLim + aBatstat->numberOfAttackersLeft;
      if ((n >= lowerLim) && (n < upperLim)) {
        group          *aGroup;

        /* Found the owner */
        *attackingSide = aBatstat;
        n -= lowerLim;
        lowerLim = 0;
        /* Now find the group the ship belongs to */
        for (aGroup = (*attackingSide)->groups;
             aGroup; aGroup = aGroup->next) {
          upperLim = lowerLim + aGroup->numberOfAttackersLeft;
          if ((n >= lowerLim) && (n < upperLim)) {
            int             curShip;

            n -= lowerLim;
            /* Now find the number of the ship within the group */
            for (*ship = 0, curShip = 0;
                 *ship < aGroup->numberOfAttackers; (*ship)++) {
              /* Did the ship fire yet? */
              if ((aGroup->canshoot)[*ship]) {
                if (n == curShip) {
                  aGroup->numberOfAttackersLeft--;
                  (*attackingSide)->numberOfAttackersLeft--;
                  (aGroup->canshoot)[*ship] = FALSE;
                  return aGroup;
                }
                else {
                  curShip++;
                }
              }
            }
          }
          lowerLim = upperLim;
        }
      }
      lowerLim = upperLim;
    }
  }
  return NULL;
}

/**** END OF selectAttackingGroup ****/



/****f* Battle/selectTargetGroup
 * NAME
 *   selectTargetGroup -- randomly select a group
 * SYNOPSIS
 *
 * FUNCTION
 *
 * INPUTS
 *   attackingSide -- batstat of the player that is about to fire  
 * RESULT
 *   targetSide -- the owner of the selected group 
 *   aGroup     -- the selected group
 *   ship       -- the selected ship in the group
 * SOURCE
 */

group          *
selectTargetGroup(batstat *attackingSide, batstat **targetSide, int *ship)
{
  /* group *targetGroup; */
  int             enemy;
  int             totalNumberOfTargets;

  pdebug(DFULL, "selectTargetGroup\n");

  /* targetGroup = NULL;*/
  *ship = 0;

  /* Compute the total number of targets */
  totalNumberOfTargets = 0;
  for (enemy = 0; enemy < attackingSide->noEnemies; enemy++) {
    totalNumberOfTargets +=
        (attackingSide->enemies[enemy])->numberOfTargets;
  }

  if (totalNumberOfTargets > 0) {
    int             lowerLim;
    int             upperLim;
    int             n;

    lowerLim = 0;

    /* Randomly select one of the targets */
    n = frand3(totalNumberOfTargets);

    /* Find the owner of the target this ship */
    for (enemy = 0; enemy < attackingSide->noEnemies; enemy++) {
      upperLim =
          lowerLim + (attackingSide->enemies[enemy])->numberOfTargets;
      if ((n >= lowerLim) && (n < upperLim)) {
        group          *aGroup;

        *targetSide = attackingSide->enemies[enemy];
        n -= lowerLim;
        lowerLim = 0;

        /* Find the group the ship belongs to, and the number of the ship
         * * within * the group */
        for (aGroup = (*targetSide)->groups; aGroup; aGroup = aGroup->next) {
          upperLim = lowerLim + aGroup->left;
          if ((n >= lowerLim) && (n < upperLim)) {
            int             curShip;

            n -= lowerLim;
            /* targetGroup = aGroup; */
            for (*ship = 0, curShip = 0; *ship < aGroup->ships; (*ship)++) {
              /* Is the ship alive */
              if ((aGroup->alive)[*ship]) {
                if (n == curShip) {
                  return aGroup;
                }
                else {
                  curShip++;
                }
              }
            }
            assert(0);
          }
          lowerLim = upperLim;
        }
      }
      lowerLim = upperLim;
    }
  }
  return NULL;
}

/***** END selectTargetGroup *****/



/****f* Battle/isBattle
 * NAME
 *   isBattle -- should a battle take place on a planet?
 * SYNOPSIS
 *   battle  *isBattle(player *players, planet *p)
 *   aBattle = isBattle(players, aPlanet)
 * FUNCTION
 *   Checks if a battle should take place on a planet based
 *   on the groups that orbit a planet.
 * RESULT
 *   aBattle  -- initialized battle structure.
 *               NULL, no battle needs to take place.
 * NOTE 
 *   This function does much more than just determine of a battle should
 *   take place.  It also initializes and allocates various data structures
 *   that are needed for the battle simulation. (Should be split in parts)
 * SOURCE
 */

battle         *
isBattle(player *players, planet *p)
{
  player         *side;
  participant    *participants;
  battle         *aBattle;

  pdebug(DFULL, "isBattle\n");

  participants = NULL;
  aBattle = NULL;

  for (side = players; side; side = side->next) {
    group          *aGroup;

    for (aGroup = side->groups; aGroup; aGroup = aGroup->next) {
      if (aGroup->location == p) {
        participant    *aParticipant;
        aParticipant = allocStruct(participant);

        assert(aParticipant != NULL);
        aParticipant->who = side;
        aParticipant->groups = NULL;
        addList(&participants, aParticipant);
        break;
      }
    }
  }

  if (numberOfElements(participants) >= 2) {
    participant    *aParticipant;

    for (aParticipant = participants;
         aParticipant; aParticipant = aParticipant->next) {
      group          *aGroup;

      for (aGroup = aParticipant->who->groups;
           aGroup; aGroup = aGroup->next) {
        if (aGroup->location == p) {
          group          *newGroup;
          newGroup = allocStruct(group);

          assert(newGroup != NULL);
          *newGroup = *aGroup;
          assert(newGroup->ships == aGroup->ships);
          newGroup->left = aGroup->ships;
	  newGroup->next = NULL;
	  newGroup->name = (char*)malloc(8);
	  sprintf(newGroup->name, "%d", newGroup->number);
          addList(&(aParticipant->groups), newGroup);
        }
      }
    }

    if (mustBattle(participants)) {
      participant    *aParticipant;

      aBattle = allocStruct(battle);

      assert(aBattle != NULL);
      aBattle->participants = participants;
      aBattle->where = p;
      aBattle->protocol = allocProtocol();
      for (aParticipant = participants;
           aParticipant; aParticipant = aParticipant->next) {
        group          *aGroup;

        for (aGroup = aParticipant->groups; aGroup; aGroup = aGroup->next) {
          aGroup->canshoot = alloc(sizeof(int) * (aGroup->ships));
          aGroup->alive = alloc(sizeof(int) * (aGroup->ships));
          assert(aGroup->canshoot);
          assert(aGroup->alive);
        }
      }
    }
  }
  if (aBattle == NULL) {
    participant    *r, *r2;

    pdebug(DFULL2, "isBattle : freeing  participants.\n");
    r = participants;
    while (r) {
      freelist(r->groups);
      r2 = r->next;
      free(r);
      r = r2;
    }
  }
  return aBattle;
}

/**********/



/****f* Battle/mustBattle
 * NAME
 *   mustBattle -- check whether participants are gonnna battle
 * SYNOPSIS
 *   int mustBattle(participant *participants)
 * FUNCTION
 *   Checks if the participants in a battle are actually at war
 *   with each other and have the necessary means to wage war.
 * SEE ALSO
 *   isBattle()
 * SOURCE
 */

int
mustBattle(participant *participants)
{
  int             war;
  participant    *part1, *part2;

  pdebug(DFULL, "mustBattle\n");
  war = FALSE;
  for (part1 = participants; part1 && !war; part1 = part1->next) {
    for (part2 = participants; part2; part2 = part2->next) {
      if (part1 != part2) {
        if (atwar(part1->who, part2->who) && canShoot(part1->groups)) {
          war = TRUE;
          break;
        }
      }
    }
  }
  return war;
}

/**** END OF mustBattle ****/


/****f* Battle/canShoot
 * NAME 
 *   canShoot -- are there armed groups in a list of groups
 * FUNCTION
 *
 * SYNOPSIS
 *   int canShoot(group *g)
 * INPUTS
 *   g -- pointer to the first group in the list of groups of a player.
 * RESULT
 *   TRUE -- player has ships with a gun.
 * SOURCE
 */

int
canShoot(group *g)
{
  int             can;

  for (can = FALSE; g; g = g->next) {
    if (g->type->attacks) {
      can = TRUE;
      break;
    };
  }
  return can;
}

/**** END of canShoot *****/


void
dumpBattle(battle *b)
{
  participant    *part;

  printf("Battle at Planet %s\n", b->where->name);
  printf("Participants\n");
  for (part = b->participants; part; part = part->next) {
    group          *g;

    printf("Player %s\n", part->who->name);
    printf("  Groups\n");
    for (g = part->groups; g; g = g->next) {
      printf("   %d %s\n", g->ships, g->type->name);
    }
  }
}



/****i* Battle/allocEnemies
 * NAME
 *   allocEnemies --
 * FUNCTION
 *
 *****
 */

void
allocEnemies(batstat *batstats)
{
  batstat        *aBatstat, *aBatstat2;

  for (aBatstat = batstats; aBatstat; aBatstat = aBatstat->next) {
    for (aBatstat->noEnemies = 0, aBatstat2 = batstats;
         aBatstat2; aBatstat2 = aBatstat2->next) {
      if (aBatstat != aBatstat2) {
        if (atwar(aBatstat->who, aBatstat2->who) ||
            (atwar(aBatstat2->who, aBatstat->who) &&
             canShoot(aBatstat2->groups))) {
          aBatstat->noEnemies++;
        }
      }
    }

    if (aBatstat->noEnemies) {
      batstat       **enemy;

      aBatstat->enemies = alloc(sizeof(batstat *) * aBatstat->noEnemies);

      assert(aBatstat->enemies != NULL);
      for (enemy = aBatstat->enemies, aBatstat2 = batstats;
           aBatstat2; aBatstat2 = aBatstat2->next) {
        if (aBatstat != aBatstat2) {
          if (atwar(aBatstat->who, aBatstat2->who) ||
              (atwar(aBatstat2->who, aBatstat->who) &&
               canShoot(aBatstat2->groups))) {
            *enemy = aBatstat2;
            enemy++;
          }
        }
      }
    }
  }
}


/****f* Battle/allocProtocol
 * NAME
 *   allocProtocol -- allocate a battle protocol structure
 * FUNCTION
 *
 ******
 */

bprotocol      *
allocProtocol(void)
{
  bprotocol      *p;
  p = (bprotocol *) alloc(sizeof(bprotocol));
  p->shots = (shot *) alloc(10 * sizeof(shot));

  p->size = 10;
  p->cur = 0;
  return p;
}


/****i* Battle/addShot
 * NAME
 *  addShot --
 *****
 */

void
addShot(bprotocol *p, player *attacker, shiptype *t1,
        player *target, shiptype *t2, int result)
{
  shot           *s;

  s = p->shots;
  (s[p->cur]).attacker = attacker;
  (s[p->cur]).atype = t1;
  (s[p->cur]).target = target;
  (s[p->cur]).ttype = t2;
  (s[p->cur]).result = result;
  p->cur++;
  if (p->cur >= p->size) {
    p->size *= 2;
    p->shots = realloc(p->shots, p->size * sizeof(shot));
  }
}
