#ifndef GNG_SELF_H
#define GNG_SELF_H

#include "galaxy.h"

int checkIntegrity(game *aGame);
int checkParticipants(participant *aParticipant);
int checkPlanet(planet *aPlanet);
int checkPlayer(player *aPlayer);
int checkGroups(group *aGroup);
int checkBattle(battle *aBattle);
int checkFleetNames(fleetname *aFleetName);
int checkStrlist(strlist *aStrlist);
int checkFleets(player *aPlayer);
void getstrTest (void);
int frandTest (void); 
int frand2Test (void);
int frand3Test (void);

player *bat_createPlayer (game *aGame, char *name);
void bat_addPlanet(game *aGame, player *owner, int *planet_name, double x, double y);
game *bat_createGame();
void bat_addShiptypes(player *aPlayer);
shiptype *bat_createShiptype(char *name, double drive, int attacks, double weapons, double shields, double cargo);
void bat_addGroups(game *aGame, char *playerName, char *planetName, char *typeName, int numberOfShips);
void bat_scenario_1 (game *aGame);
void bat_scenario_2 (game *aGame);
void bat_scenario_3 (game *aGame);
void bat_scenario_4 (game *aGame);
void bat_scenario_5 (game *aGame);
void bat_scenario_6 (game *aGame);
void bat_scenario_7 (game *aGame);
void bat_scenario_8 (game *aGame);
void bat_scenario_9 (game *aGame);
void bat_scenario_10 (game *aGame);
void bat_scenario_11 (game *aGame);
void bat_scenario_12 (game *aGame);
void bat_scenario_13 (game *aGame);
void bat_scenario_14 (game *aGame);
void bat_allies(game *aGame, char *playerName1, char *playerName2);

#endif				/* GNG_SELF_H */
