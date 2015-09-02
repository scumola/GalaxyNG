#ifndef GNG_BATTLE_H
#define GNG_BATTLE_H

#include "galaxy.h"

/****s* Battle/batstat
 * NAME
 *   batstat
 * PURPOSE
 *   Structure to track all groups of one player involved in a battle.
 * ATTRIBUTES
 *   targetArray -- An array with pointers to all groups.
 *                  First all armed groups, then all unarmed groups
 *   canshoot  --   array with one element per ship that indicates
 *                  if the ship can fire and has fired.
 *   noEnemies --   number of enemies.
 *   enemies   --   
 * SOURCE
 */

typedef struct batstat {
  struct batstat *next;
  long            cookie;
  char           *name;
  player         *who;
  group          *groups;
  double          maxAttack;
  double          minDefense;
  int             numberOfTargets;
  int             numberOfAttackers;
  int             numberOfAttackersLeft;
  int             noEnemies;
  struct batstat **enemies;
} batstat;

/***** END batstat ******/

int             isWon(batstat *aBatstat);
int             isDraw(batstat *aBatstat);
void            fightphase(game *aGame, int phase);
void            attack(bprotocol *aProtocol,
                       batstat *attackingSide, group *attackingGroup,
                       int attackingShip, batstat *targetSide,
                       group *targetGroup, int targetShip);
battle         *isBattle(player *players, planet *p);
int             mustBattle(participant *participants);
int             attackersLeft(batstat *aBatstat);
void            resetSides(batstat *aBatstat);
int             canShoot(group *g);
void            dumpBattle(battle *b);
int             shoot(group *attacker, group *defender);
void            allocEnemies(batstat *batstats);
void            doBattle(batstat *batstats, bprotocol *p, int phase);

group          *selectTargetGroup(batstat *attackingSide,
                                  batstat **targetSide, int *ship);
group          *selectAttackingGroup(batstat *batstats,
                                     batstat **attackingSide, int *ship);
void            updateGroups(battle *b);
bprotocol      *allocProtocol(void);
void            addShot(bprotocol *p, player *attacker, shiptype *t1,
                        player *target, shiptype *t2, int result);

#endif                          /* GNG_BATTLE_H */
