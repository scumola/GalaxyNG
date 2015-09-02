#include "selftest.h"
#include <assert.h>
#include <math.h>
#include <string.h>
#include "util.h"
#include "create.h"
#include "process.h"


/****h* GalaxyNG/SelfTest
 * NAME
 *   SelfTest -- 
 * PURPOSE
 *   A number of funtions to test the engine for bugs.
 *   
 *   The main test is a series of functions that transferse the
 *   game structure and do various tests on the data.
 *
 *   There are also some functions that test some key functions
 *   of the engine.
 *****
 */


/****f* SelfTest/checkIntegrity
 * NAME
 *   checkIntegrity -- check integrity of game structure.
 * SYNOPSIS
 *   int      checkIntegrity(game *)
 *   result = checkIntegrity(aGame)
 * FUNCTION
 *   Check the integrity of the game structure. This is done
 *   going over the whole game structure and checking whether
 *   parameters have sensible values and pointers point to
 *   valid structures (using the COOKIE values).
 * DIAGNOSTICS
 *   All errors that are encountered are reported in the log file.
 *   Set logLevel to LFULL to get detailed output on the location
 *   of the error.
 * RESULT
 *   TRUE  -- All OK
 *   FALSE -- Something is terribly wrong.
 * SOURCE
 */

int checkIntegrity(game *aGame)
{
  planet         *aPlanet;
  player         *aPlayer;
  battle         *aBattle;
  int             check;

  plog(LFULL, "Checking Integrity\n");

  check = TRUE && checkStrlist(aGame->messages);

  for (aPlanet = aGame->planets; 
       aPlanet; 
       aPlanet = aPlanet->next) {
    check = check && checkPlanet(aPlanet);
  }

  for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
    if (checkPlayer(aPlayer)) {
      plog(LFULL, "\n** %s **\n", aPlayer->name);
      plog(LFULL, "GROUPS:   ");
      check = checkGroups(aPlayer->groups) && check;
      plog(LFULL, "FLEET:    ");
      check = checkFleetNames(aPlayer->fleetnames) && check;
      plog(LFULL, "FLEETS:   ");
      check = checkFleets(aPlayer) && check;
      plog(LFULL, "MESSAGES: ");
      check = checkStrlist(aPlayer->messages) && check;
      plog(LFULL, "MISTAKES: ");
      check = checkStrlist(aPlayer->mistakes) && check;
      plog(LFULL, "ORDERS:   ");
      check = checkStrlist(aPlayer->orders) && check;
    } else {
      check = FALSE;
    }
  }

  for (aBattle = aGame->battles; 
       aBattle; 
       aBattle = aBattle->next) {
    plog(LFULL, "o battle\n");
    check = checkBattle(aBattle) && check;
    plog(LFULL, "o participants\n");
    check = checkParticipants(aBattle->participants) && check;
  }
  
  if (!check) {
    plog(LBRIEF, "One of the integrity checks failed!\n");
  }
  return check;
}

/**** END checkIntegrity *****/



/****f* SelfTest/checkFleets
 * NAME
 *   checkFleets -- check the speeds of all fleets.
 * FUNCTION
 *   Checks if a fleet has groups with a speed that is slower
 *   that the given fleet speed.
 ******
 */

int
checkFleets(player *aPlayer)
{
  group          *aGroup;
  fleetname      *aFleet;

  for (aFleet = aPlayer->fleetnames;
       aFleet;
       aFleet = aFleet->next) {
    for (aGroup = aPlayer->groups;
	 aGroup;
	 aGroup = aGroup->next) {
      if (aGroup->thefleet eq aFleet) {
	if (groupSpeed(aGroup) < aFleet->fleetspeed) {
	  plog(LBRIEF, "  Fleet speed incorrect\n");
	  plog(LBRIEF, "  Fleet %s stated speed %f\n",
		 aFleet->name, aFleet->fleetspeed);
	  plog(LBRIEF, "  Has a ship with speed %f\n", groupSpeed(aGroup));
	  return FALSE;
	}
      }
    }
  }
  plog(LFULL, "\n");
  return TRUE;
}


/****f* SelfTest/checkStrlist
 * NAME
 *   checkStrlist -- check all strings in a string list.
 * FUNCTION
 *   Check if the list of strings is really a list of string.
 ******
 */

int
checkStrlist(strlist *aStrlist)
{
  for (; aStrlist; aStrlist = aStrlist->next) {
    if (!isStruct(strlist, aStrlist)) {
      plog(LBRIEF, "!!not a strlist");
      return FALSE;
    }
  }
  plog(LFULL, "\n");
  return TRUE;
}


/****i* SelfTest/checkParticipants
 * NAME
 *   checkParticipants --
 ******
 */

int
checkParticipants(participant *aParticipant)
{
  if (!isStruct(participant, aParticipant)) {
    plog(LBRIEF, "  not a participant\n");
    checkPlayer(aParticipant->who);
    checkGroups(aParticipant->groups);
    return FALSE;
  }
  return TRUE;
}

/****i* SelfTest/checkPlanet
 * NAME
 *   checkPlanet --
 *******
 */

int
checkPlanet(planet *aPlanet)
{
  if (!isStruct(planet, aPlanet)) {
    plog(LBRIEF, "  not a planet\n");
    return FALSE;
  }
  if (aPlanet->owner) {
    if (!isStruct(player, aPlanet->owner)) {
      plog(LBRIEF, "  planet.owner of %s is not a player\n",
	     aPlanet->name);
      return FALSE;
    }
  }
  if (aPlanet->producingshiptype) {
    if (!isStruct(shiptype, aPlanet->producingshiptype)) {
      plog(LBRIEF,
	   "  planet.producingshiptype of %s is not a shiptype\n",
	   aPlanet->name);
      return FALSE;
    }
  }
  return TRUE;
}

/****i* SelfTest/checkPlayer
 * NAME
 *   checkPlayer --
 * SOURCE
 */

int
checkPlayer(player *aPlayer)
{
  if (!isStruct(player, aPlayer)) {
    plog(LBRIEF, "!!not a player");
    return FALSE;
  }
  return TRUE;
}

/*****/


/****i* SelfTest/checkFleetNames
 * NAME
 *   checkFleetNames --
 * SOURCE
 */

int
checkFleetNames(fleetname *aFleetName)
{
  int result = TRUE;
  for (; aFleetName; aFleetName = aFleetName->next) {
    plog(LFULL, "(");
    if (!isStruct(fleetname, aFleetName)) {
      plog(LBRIEF, "!!not a fleetname");
      result = FALSE;
    } else {
      plog(LFULL, "%s)", aFleetName->name);
    }
  }
  plog(LFULL, "\n");
  return TRUE;
}

/****/

/****i* SelfTest/checkGroups
 * NAME
 *   checkGroups --
 * SOURCE
 */

int
checkGroups(group *aGroup)
{
  int result = TRUE;

  for (; aGroup; aGroup = aGroup->next) {
    plog(LFULL, "(%d", aGroup->number);
    if (!isStruct(group, aGroup)) {
      plog(LBRIEF, "!!not a group\n");
      result = FALSE;
    }
    if (!isStruct(shiptype, aGroup->type)) {
      plog(LBRIEF, "!!group.shiptype is not a shiptype");
      result = FALSE;
    }
    if (aGroup->from) {
      if (!checkPlanet(aGroup->from)) {
	plog(LBRIEF, "!!group.from is not a planet");
	result = FALSE;
      }
    }
    if (aGroup->where) {
      if (!checkPlanet(aGroup->where)) {
	plog(LBRIEF, "!!group.where is not a planet");
	result = FALSE;
      }
    }
    if (aGroup->location) {
      if (!checkPlanet(aGroup->location)) {
	plog(LBRIEF, "!!group.location is not a planet");
	result = FALSE;
      }
    }
    if (aGroup->thefleet) {
      if (!isStruct(fleetname, aGroup->thefleet)) {
	plog(LBRIEF, "!!group.thefleet is not a fleetname");
	result = FALSE;
      }
    }
    if (aGroup->ships < 0) {
      plog(LBRIEF, "!!group.ships < 0");
      result = FALSE;
    }
    if (aGroup->left < 0) {
      plog(LBRIEF, "!!group.left < 0");
      result = FALSE;
    }
    if (aGroup->defense < 0.0) {
      plog(LBRIEF, "!!group.defense < 0");
      result = FALSE;
    }
    if (aGroup->attack < 0.0) {
      plog(LBRIEF, "!!group.attack < 0");
      result = FALSE;
    }
    plog(LFULL, ")", aGroup->number);
  }
  plog(LFULL, "\n");
  return result;
}

/*******/


/****i* SelfTest/checkBattle
 * NAME
 *   checkBattle --
 * SOURCE
 */

int
checkBattle(battle *aBattle)
{
  if (!isStruct(battle, aBattle)) {
    plog(LBRIEF, "  not a battle\n");
    return FALSE;
  }
  return TRUE;
}

/****/


/****** SelfTest/getstrTest
 * NAME
 *   getstrTest -- Test getstr() function.
 * FUNCTION
 *   The getstr() function is one of the most used function of the
 *   server. Most of the input passes through this function. This
 *   performs some sanity checks on this function.
 * RESULT
 *   The program aborts if an error was found.
 * SOURCE
 */

char *tststr1 = "word1  word2,word3  word4\n";
char *tststr2 = "  \"Word Word 1 <<;; <\" \"; a remark ;\n";

void getstrTest(void)
{
  char *word;
  int result;
  
  result = TRUE;
  printf("---- Testing: getstr() ----\n");
  printf("  with string:   %s", tststr1);
  word = getstr(tststr1);
  assert (strcmp(word, "word1") == 0);
  word = getstr(NULL);
  assert (strcmp(word, "word2") == 0);
  word = getstr(NULL);
  assert (strcmp(word, "word3") == 0);
  word = getstr(NULL);
  assert (strcmp(word, "word4") == 0); 
  printf("  string is now: %s", tststr1);
  printf("  with string:   %s", tststr2);
  word = getstr(tststr2);
  assert (strcmp(word, "Word_Word_1__") == 0);
  word = getstr(NULL);
  assert (strcmp(word, "_a_remark__") == 0);
  printf("  string is now: %s", tststr2);
  printf("---- completed ----\n");
}

/**********/

#define TEST_RANGE1 20000

/****f* SelfTest/frandTest
 * NAME
 *   frandTest
 ******
 */

int 
frandTest () 
{
  int i;
  double f, sum, min, max;
  printf("---- Testing: frand() ----\n");
  max = -3141.0;
  min = 3141.0;
  for (sum = 0.0, i = 0; i < TEST_RANGE1; i++) {
    f = frand(1000.0);
    if (f < min) min = f;
    if (f > max) max = f;
    sum += f;
  }
  printf("Test result: [%f  %f  %f]\n", min, sum/TEST_RANGE1, max);
  printf("Should be:   [0.027665  500.382084  999.933610]\n");
  printf("---- completed ----\n");
  return 0;
}

#define TEST_RANGE2 100000

int frand2Test ()
{
  int i;
  double f, sum, min, max;
  printf("---- Testing: frand2() ----\n");
  max = -100; 
  min = 100.0;
  for (sum = 0.0, i = 0; i < TEST_RANGE2; i++) {
    f = frand2();
    if (f < min) min = f;
    if (f > max) max = f;
    sum += f;
  }
  printf("Test result: [%f %f  %f] \n", min, sum / TEST_RANGE2, max);
  printf("Should be:   [-0.999991 0.001166  0.999976]\n");
  printf("---- completed ----\n");
  return 0;
}


#define TEST_RANGE3 100

int frand3Test ()
{
  int i;
  int count[TEST_RANGE3+2];
  printf("---- Testing: frand3() ----\n");

  for (i = 0; i < TEST_RANGE3; i++) {
    count[i] = 0;
  }
  for (i = 0; i < 100000; i++) {
    int ind;
    
    ind = frand3(TEST_RANGE3);
    assert(ind >= 0);
    assert(ind < TEST_RANGE3);
    count[ind]++;
  }
  for (i = 0; i < TEST_RANGE3; i++) {
    printf("%4d ", count[i]);
    if (!((i+1) % 10)) printf("\n");
  }
  printf("\n");
  printf("---- completed ----\n");
  return 0;
}


/* === Tests for the battle engine === */

game *bat_createGame ()
{
    if (createGameDirectories("bat_test")) {
        game           *aGame;
        player         *playerA, *playerB, *playerC, *playerD, *playerE;
        int            planet_name = 0;
        aGame = allocStruct(game);
        setName(aGame, "bat_test");
        aGame->turn = 0;
        aGame->galaxysize = 30.0;

        playerA = bat_createPlayer(aGame, "A");
        playerB = bat_createPlayer(aGame, "B");
        playerC = bat_createPlayer(aGame, "C");
        playerD = bat_createPlayer(aGame, "D");
        playerE = bat_createPlayer(aGame, "E");

        addList(&(aGame->players), playerA);
        addList(&(aGame->players), playerB);
        addList(&(aGame->players), playerC);
        addList(&(aGame->players), playerD);
        addList(&(aGame->players), playerE);

        bat_addPlanet(aGame, playerA, &planet_name, 10.0, 10.0);
        bat_addPlanet(aGame, playerB, &planet_name, 20.0, 10.0);
        bat_addPlanet(aGame, playerC, &planet_name, 10.0, 20.0);
        bat_addPlanet(aGame, playerD, &planet_name, 20.0, 20.0);
        bat_addPlanet(aGame, playerE, &planet_name, 15.0, 15.0);

        bat_addShiptypes(playerA);
        bat_addShiptypes(playerB);
        bat_addShiptypes(playerC);
        bat_addShiptypes(playerD);
        bat_addShiptypes(playerE);

        preComputeGroupData(aGame);
        raceStatus(aGame);
        return aGame; 
    } else {
        return NULL;
    }   
}


void bat_addPlanet(game *aGame, player *owner, int *planet_name, double x, double y) 
{
    planet *p;
    p = addplanet(aGame);
    (*planet_name)++;
    p->owner = owner;
    p->x = x;
    p->y = y;
    p->pop = 1000.0;
    p->ind = 1000.0;
    p->size = 1000;
    p->resources = 10;
    p->producing = PR_CAP;   
}


player *bat_createPlayer (game *aGame, char *name) {
    player         *aPlayer;

    aPlayer = allocStruct(player);

    setName(aPlayer, name);
    aPlayer->pswd = strdup("none");
    aPlayer->addr = strdup("none");
    aPlayer->realName = strdup("none");
    aPlayer->pswdstate = 1;
    aPlayer->drive = 3;
    aPlayer->weapons = 1;
    aPlayer->shields = 1;
    aPlayer->cargo = 1;
    aPlayer->msize = aGame->galaxysize;
    aPlayer->flags =
        F_ANONYMOUS | F_AUTOUNLOAD | F_PRODTABLE | F_SORTGROUPS |
        F_GROUPFORECAST | F_PLANETFORECAST | F_SHIPTYPEFORECAST |
        F_ROUTESFORECAST | F_BATTLEPROTOCOL;
    return aPlayer;
}


void bat_addShiptypes(player *aPlayer) 
{
   shiptype *t;
   t = bat_createShiptype("drone", 1, 0, 0, 0, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("armed_drone", 1, 1, 1, 0, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Big_Gun", 33, 1, 33, 33, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Big_Shield", 50, 1, 1, 350, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Multi_Gun", 33, 5, 10, 36, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Multi_Gun2", 33, 4, 13.2, 33, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Anti_Flak", 34.5, 98, 1, 15, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Anti_Flak2", 33.0, 93, 1, 15, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Mini_Anti_Flak", 16.6, 37, 1, 11.9, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Giant_Anti_Flak", 69.5, 219, 1, 19.5, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Tiny_Anti_Flak", 8.6, 12, 1, 9.6, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("SmFlak", 33.25, 95, 1, 17.62, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("Main_Gun", 34, 7, 10.14, 23.82, 0);
   addList(&aPlayer->shiptypes, t);
   t = bat_createShiptype("AntiFlak3", 33.2, 118, 1, 6.1, 0);
   addList(&aPlayer->shiptypes, t);
}



shiptype *bat_createShiptype(char *name, double drive, int attacks, double weapons, double shields, double cargo)
{
  shiptype *t;
  t = allocStruct(shiptype);
  setName(t, name);
  t->drive = drive;
  t->attacks = attacks;
  t->weapons = weapons;
  t->shields = shields;
  t->cargo   = cargo;

  return t;
}


void bat_addGroups(game *aGame, char *playerName, char *planetName, char *typeName, int numberOfShips)
{
   group    *aGroup;
   player   *aPlayer;
   planet   *aPlanet;
   shiptype *aShiptype;

   aPlayer = findElement(player, aGame->players, playerName);
   aShiptype = findElement(shiptype, aPlayer->shiptypes, typeName);
   aPlanet = findElement(planet, aGame->planets, planetName);
   assert(aPlayer);
   assert(aShiptype);
   assert(aPlanet);

   aGroup = allocStruct(group);

   aGroup->ships = numberOfShips;
   aGroup->type = aShiptype;
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
   addList(&aPlayer->groups, aGroup);
}


void bat_allies(game *aGame, char *playerName1, char *playerName2)
{
   player *aPlayer1;
   player *aPlayer2;
   alliance       *a;

   aPlayer1 = findElement(player, aGame->players, playerName1);
   aPlayer2 = findElement(player, aGame->players, playerName2);

   a = allocStruct(alliance);
   a->who = aPlayer1;
   addList(&aPlayer2->allies, a);

   a = allocStruct(alliance);
   a->who = aPlayer2;
   addList(&aPlayer1->allies, a);
}


void bat_scenario_1 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Big_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "B", "1", "drone", 200);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 1);
}

void bat_scenario_2 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Big_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "B", "1", "armed_drone", 100);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 1);
}


void bat_scenario_3 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Big_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "B", "1", "armed_drone", 75);
   bat_addGroups(aGame, "B", "1", "drone", 50);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 1);
}


void bat_scenario_4 (game *aGame)
{
   bat_allies(aGame, "A", "C");
   bat_addGroups(aGame, "A", "1", "Anti_Flak", 1);
   bat_addGroups(aGame, "C", "1", "Anti_Flak", 1);
   bat_addGroups(aGame, "A", "1", "drone", 100);
   bat_addGroups(aGame, "C", "1", "drone", 100);
   bat_addGroups(aGame, "C", "1", "Big_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "B", "1", "drone", 200);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 1);
}


void bat_scenario_5 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Anti_Flak", 1);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Big_Gun", 2);

   bat_addGroups(aGame, "B", "1", "Anti_Flak", 1);
   bat_addGroups(aGame, "B", "1", "armed_drone", 100);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 2);
}



void bat_scenario_6 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Giant_Anti_Flak", 1);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Big_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "B", "1", "drone", 200);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 1);
}

void bat_scenario_7 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Giant_Anti_Flak", 1);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Big_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Mini_Anti_Flak", 4);
   bat_addGroups(aGame, "B", "1", "drone", 200);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 1);
}

void bat_scenario_8 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Giant_Anti_Flak", 1);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Big_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Tiny_Anti_Flak", 8);
   bat_addGroups(aGame, "B", "1", "drone", 200);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 1);
}


void bat_scenario_9 (game *aGame)
{

   bat_allies(aGame, "A", "C");
   bat_addGroups(aGame, "A", "1", "Big_Shield", 1);
   bat_addGroups(aGame, "A", "1", "drone", 1);
   bat_addGroups(aGame, "B", "1", "Big_Gun", 1);
}


void bat_scenario_10 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Giant_Anti_Flak", 1);
   bat_addGroups(aGame, "A", "1", "drone", 400);
   bat_addGroups(aGame, "A", "1", "Multi_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Anti_Flak", 2);
   bat_addGroups(aGame, "B", "1", "drone", 400);
   bat_addGroups(aGame, "B", "1", "Multi_Gun", 1);
}

void bat_scenario_11 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Giant_Anti_Flak", 1);
   bat_addGroups(aGame, "A", "1", "drone", 400);
   bat_addGroups(aGame, "A", "1", "Multi_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Mini_Anti_Flak", 4);
   bat_addGroups(aGame, "B", "1", "drone", 400);
   bat_addGroups(aGame, "B", "1", "Multi_Gun", 1);
}

void bat_scenario_12 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Giant_Anti_Flak", 1);
   bat_addGroups(aGame, "A", "1", "drone", 400);
   bat_addGroups(aGame, "A", "1", "Multi_Gun", 1);

   bat_addGroups(aGame, "B", "1", "Tiny_Anti_Flak", 8);
   bat_addGroups(aGame, "B", "1", "drone", 400);
   bat_addGroups(aGame, "B", "1", "Multi_Gun", 1);
}


void bat_scenario_13 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "Anti_Flak2", 2);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Multi_Gun2", 1);

   bat_addGroups(aGame, "B", "1", "Mini_Anti_Flak", 4);
   bat_addGroups(aGame, "B", "1", "drone", 200);
   bat_addGroups(aGame, "B", "1", "Multi_Gun2", 1);
}

void bat_scenario_14 (game *aGame)
{
   bat_allies(aGame, "A", "C");

   bat_addGroups(aGame, "A", "1", "AntiFlak3", 4);
   bat_addGroups(aGame, "A", "1", "drone", 200);
   bat_addGroups(aGame, "A", "1", "Main_Gun", 1);

   bat_addGroups(aGame, "B", "1", "SmFlak", 1);
   bat_addGroups(aGame, "B", "1", "drone", 200);
   bat_addGroups(aGame, "B", "1", "Main_Gun", 1);
}



