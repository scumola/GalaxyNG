/****h* GalaxyNG/Create [1.0] *
*  NAME
*    Create -- create a new galaxy
*  NOTES
*    This code was developed since we found that the old code
*    sometimes created galaxies that left some races in rather
*    unfair positions.  That is some races would have their home
*    planet located far away from other planets, while others races
*    home planets were located near a whole bunch of good planets.
*    The code tries to provide all player with an equal chance on
*    success, while still keeping the layout of the galaxy diverse
*    and interesting.
*
*    How is this accomplised:
*
*    (1) Each race will get the same number of planets within a
*        circle of a given radius.   The size of these planets differ
*        but the total sum of the sizes is the same for all races.
*        The resources of the planets are still chosen random.
*
*    (2) The home world can be spaced widely from each other, ensuring
*        that a race isn't kicked out in the first few turns.
*
*    (3) To improve the tactical possibilities a number of `stuff
*        planets' will be scattered across the galaxy.  These are
*        small useless planets that provide players with different
*        routes to attack their enemies.
*
*    (4) In addition it is possible to let the races start with
*        more than one home planet, this will speed up the game.
*
*    All these options can be controled with a set of variables
*    that you will be asked to supply when you run the galaxy
*    program to create a new galaxy.  A large number of
*    different galaxies is thus possible. Try to experiment a
*    little before you settle on a galaxy you like.
*
*    Notice that as opposed to the original code the names of
*    the races at turn zero will not be  Player_xx   but
*    race_XX.
*
*    Added the option to space out the empires along the edge of
*    a large circle, in order to make certain that no one got
*    a better placement geographically than any other player.
*    No attempt was made to ensure any seeding of players. If
*    you get stuck between Jacco and Frans, you are still toast. :)
*    Tommy 980629
*  AUTHOR
*    Frans, Rogerio Fung, Tommy
*  BUGS
*    You found bugs?
*    Catch them in a jar and send them to fslothouber@acm.org
*
**********
*/


char           *vcreate =
    "$Id: create.c,v 1.16 2005/01/23 05:37:18 madcoder42 Exp $";

#include "create.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "galaxy.h"
#include "list.h"
#include "util.h"
#include "report.h"
#include "savegame.h"
#include "process.h"

/****f* Create/readGameSpec
 * GAME
 *   readGameSpec -- read specs for a game from file
 * SYNOPSIS
 *   gamespecification *readGameSpec(FILE *)
 *   spec    =          readGameSpec(specfile)
 * FUNCTION
 *   Reads and interprets a file that specifies the layout of the 
 *   galaxy, and information about the players.
 *   
 * RESULT
 *   Pointer to a gamespecification structure. Any parameters
 *   not defined in the specification file are given some
 *   "sensible" default.
 * SEE ALSO
 *   See Doc/example.glx for information on the layout of such a file. 
 *   CMD_template()
 * SOURCE
 */

gamespecification *
readGameSpec(FILE * specfile)
{
  gamespecification *aGameSpec;
  char           *isRead;
  char           *key;
  char           *value;
  int             team = 0;

  srand((int) time(NULL));
  aGameSpec = allocStruct(gamespecification);

  setName(aGameSpec, "TestGame");
  aGameSpec->gameOptions.gameOptions = 0;
  aGameSpec->galaxySize = 200;
  aGameSpec->numberOfHomePlanets = 2;
  aGameSpec->numberOfEmptyPlanets = 6;
  aGameSpec->numberOfStuffPlanets = 5;
  aGameSpec->minDist = 25.0;
  aGameSpec->radiusEmptyPlanets = 30.0;
  aGameSpec->radiusSecondaryPlanets = 3.0;
  aGameSpec->players = NULL;
  aGameSpec->numberOfPlayers = 0;
  aGameSpec->coreSizes = malloc(2 * sizeof(double));
  aGameSpec->teamGame = FALSE;
  aGameSpec->coreSizes[0] = 1000.0;
  aGameSpec->coreSizes[1] = 400.0;
  aGameSpec->gameOptions.galactic_peace = 0;
  aGameSpec->gameOptions.initial_drive = 1.0;
  aGameSpec->gameOptions.initial_weapons = 1.0;
  aGameSpec->gameOptions.initial_shields = 1.0;
  aGameSpec->gameOptions.initial_cargo = 1.0;

  for (isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, specfile);
       isRead; isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, specfile)) {

    key = getstr(lineBuffer);

    if (key[0] != '\0') {
      if (noCaseStrcmp("start_player", key) == 0) {
	newplayer *aNewPlayer;
	char       raceName[20];
	int        nbrHomePlanetsAllocated = 10;
	
	aNewPlayer = allocStruct(newplayer);
	while (fgets(lineBuffer, LINE_BUFFER_SIZE, specfile)) {
	  key = getstr(lineBuffer);
	  if (noCaseStrcmp(key, "end_player") == 0) {
	    break;
	  }
	  else if (noCaseStrcmp(key, "email") == 0) {
	    aNewPlayer->addr = strdup(getstr(0));
	    aNewPlayer->team = team;
	    (aGameSpec->numberOfPlayers)++;
	    sprintf(raceName, "race_%d", aGameSpec->numberOfPlayers);
	    setName(aNewPlayer, raceName);
	    sprintf(raceName, "P%d", rand());
	    aNewPlayer->pswd = strdup(raceName);
	    addList(&(aGameSpec->players), aNewPlayer);
	    aNewPlayer->coreSizes = malloc(10 * sizeof(double));
	    memset(aNewPlayer->coreSizes, 0, 10*sizeof(double));
	    aNewPlayer->coreXOffset = malloc(10 * sizeof(double));
	    memset(aNewPlayer->coreXOffset, 0, 10*sizeof(double));
	    aNewPlayer->coreYOffset = malloc(10 * sizeof(double));
	    memset(aNewPlayer->coreYOffset, 0, 10*sizeof(double));
	  }
	  else if (noCaseStrcmp(key, "homeworld") == 0) {
	    int i = aNewPlayer->numberOfHomePlanets++;
	    if (aNewPlayer->numberOfHomePlanets == nbrHomePlanetsAllocated) {
	      int lastAllocSize = nbrHomePlanetsAllocated;
	      nbrHomePlanetsAllocated *= 2;
	      aNewPlayer->coreSizes =
		realloc(aNewPlayer->coreSizes,
			nbrHomePlanetsAllocated * sizeof(double));
	      aNewPlayer->coreXOffset =
		realloc(aNewPlayer->coreXOffset,
			nbrHomePlanetsAllocated * sizeof(double));
	      aNewPlayer->coreYOffset =
		realloc(aNewPlayer->coreYOffset,
			nbrHomePlanetsAllocated * sizeof(double));
	      
	      memset(&aNewPlayer->coreSizes[lastAllocSize], 0,
		     lastAllocSize*sizeof(double));
	      memset(&aNewPlayer->coreXOffset[lastAllocSize], 0,
		     lastAllocSize*sizeof(double));
	      memset(&aNewPlayer->coreYOffset[lastAllocSize], 0,
		     lastAllocSize*sizeof(double));
	    }
	    
	    value = getstr(0);
	    aNewPlayer->coreSizes[i] = atof(value);
	    if ((value = getstr(0)) != NULL)
	      aNewPlayer->coreXOffset[i] = atof(value);
	    if ((value = getstr(0)) != NULL)
	      aNewPlayer->coreYOffset[i] = atof(value);
#if 0
	    printf("%2d) %.2f %.2f %.2f\n", i, aNewPlayer->coreSizes[i],
		   aNewPlayer->coreXOffset[i], aNewPlayer->coreYOffset[i]);
#endif					  
	  }
	}
	
	if (aNewPlayer->numberOfHomePlanets == 0) {
	  int i;
	  
	  aNewPlayer->coreSizes =
	    malloc(aGameSpec->numberOfHomePlanets * sizeof(double));
	  
	  for (i = 0; i < aGameSpec->numberOfHomePlanets; i++) {
	    aNewPlayer->coreSizes[i] = aGameSpec->coreSizes[i];
	  }
	  aNewPlayer->numberOfHomePlanets = aGameSpec->numberOfHomePlanets;
	}
      }
      
      else if (noCaseStrcmp("peace", key) == 0) {
	value = getstr(0);
	aGameSpec->gameOptions.galactic_peace = atoi(value);
      }
      else if (noCaseStrcmp("size", key) == 0) {
	value = getstr(0);
	aGameSpec->galaxySize = atof(value);
      }
      else if (noCaseStrcmp("race_spacing", key) == 0) {
	value = getstr(0);
	aGameSpec->minDist = atof(value);
      }
      else if (noCaseStrcmp("empty_planets", key) == 0) {
	value = getstr(0);
	aGameSpec->numberOfEmptyPlanets = atoi(value);
      }
      else if (noCaseStrcmp("stuff_planets", key) == 0) {
	value = getstr(0);
	aGameSpec->numberOfStuffPlanets = atoi(value);
      }
      else if (noCaseStrcmp("rows", key) == 0) {
	value = getstr(0);
	aGameSpec->numberOfRows = atoi(value);
      }
      else if (noCaseStrcmp("columns", key) == 0) {
	value = getstr(0);
	aGameSpec->numberOfColumns = atoi(value);
      }
      else if (noCaseStrcmp("team", key) == 0) {
	value = getstr(0);
	team = atoi(value);
      }
      else if (noCaseStrcmp("empty_radius", key) == 0) {
	value = getstr(0);
	aGameSpec->radiusEmptyPlanets = atof(value);
      }
      else if (noCaseStrcmp("secondary_radius", key) == 0) {
	value = getstr(0);
	aGameSpec->radiusSecondaryPlanets = atof(value);
      }
      else if (noCaseStrcmp("name", key) == 0) {
	setName(aGameSpec, getstr(0));
      }
      else if (noCaseStrcmp("teamgame", key) == 0) {
	aGameSpec->teamGame = TRUE;
      }
      else if (noCaseStrcmp("core_sizes", key) == 0) {
	int number;
	
	for (value = getstr(0), aGameSpec->numberOfHomePlanets = 0;
	     value[0] != '\0';
	     value = getstr(0), aGameSpec->numberOfHomePlanets++);
	aGameSpec->coreSizes =
	  malloc(aGameSpec->numberOfHomePlanets * sizeof(double));
			  
	getstr(lineBuffer);
	for (value = getstr(0), number = 0;
	     value[0] != '\0'; value = getstr(0), number++) {
	  aGameSpec->coreSizes[number] = atof(value);
	}
      }
      else if (noCaseStrcmp("initialtechlevels", key) == 0) {   /* CB-20010407 
								 * ; no
								 * verification 
								 * at all */
	aGameSpec->gameOptions.initial_drive = atof(getstr(0)); /* seems
								 * strtod
								 * is not
								 * portable 
								 */
	aGameSpec->gameOptions.initial_weapons = atof(getstr(0));
	aGameSpec->gameOptions.initial_shields = atof(getstr(0));
	aGameSpec->gameOptions.initial_cargo = atof(getstr(0));
	if (aGameSpec->gameOptions.initial_drive < 1.0) {
	  aGameSpec->gameOptions.initial_drive = 1.0;
	}
	if (aGameSpec->gameOptions.initial_weapons < 1.0) {
	  aGameSpec->gameOptions.initial_weapons = 1.0;
	}
	if (aGameSpec->gameOptions.initial_shields < 1.0) {
	  aGameSpec->gameOptions.initial_shields = 1.0;
	}
	if (aGameSpec->gameOptions.initial_cargo < 1.0) {
	  aGameSpec->gameOptions.initial_cargo = 1.0;
	}
      }
      else if (noCaseStrcmp("fullbombing", key) == 0) {
	aGameSpec->gameOptions.gameOptions |= GAME_NONGBOMBING;
      }
      else if (noCaseStrcmp("keepproduction", key) == 0) {
	aGameSpec->gameOptions.gameOptions |= GAME_KEEPPRODUCTION;
      }
      else if (noCaseStrcmp("createcircle", key) == 0) {        /* Don't
								 * know if 
								 * still
								 * used
								 * ???? CB 
								 * 20010425 */
	aGameSpec->gameOptions.gameOptions |= GAME_CREATECIRCLE;
      }
      else if (noCaseStrcmp("dontdropdead", key) == 0) {
	aGameSpec->gameOptions.gameOptions |= GAME_NODROP;
      }
      else if (noCaseStrcmp("savereportcopy", key) == 0) {
	aGameSpec->gameOptions.gameOptions |= GAME_SAVECOPY;
      }
      else if (noCaseStrcmp("sphericalgalaxy", key) == 0) {     /* CB-20010401 
								 * ; see
								 * galaxy.h 
								 */
	/* printf ("*==> %ld\n", aGameSpec->gameOptions); */
	aGameSpec->gameOptions.gameOptions |= GAME_SPHERICALGALAXY;
	/* printf ("*==> %ld\n", aGameSpec->gameOptions); */
      }
      else {
	printf("Unknown key %s\n", key);
      }
    }
  }

  return aGameSpec;
}

/********/


/****f* Create/printGameSpecs
 * NAME
 *   printGameSpecs -- print the game specs.
 * SYNOPSIS
 *   void printGameSpecs(gamespecification *aGameSpec)
 * SOURCE
 */

void
printGameSpecs(gamespecification *aGameSpec)
{
  newplayer      *curNewPlayer;

  /* aGameSpec->gameOptions = 0; CB-20010407 */
  printf("Galaxy:               %s\n", aGameSpec->name);
  if (aGameSpec->teamGame) {
    printf("TeamGame:                 Yes\n");
  }
  else {
    printf("TeamGame:                 No\n");
  }
  printf("Size:                     %.0f x %.0f\n",
         aGameSpec->galaxySize, aGameSpec->galaxySize);

  printf("Full bombing     :         %s\n",
         (aGameSpec->gameOptions.
          gameOptions & GAME_NONGBOMBING ? "ON" : "OFF"));
  printf("Keep production  :         %s\n",
         (aGameSpec->gameOptions.
          gameOptions & GAME_KEEPPRODUCTION ? "ON" : "OFF"));
  printf("Drop dead players:         %s\n",
         (aGameSpec->gameOptions.
          gameOptions & GAME_NODROP ? "ON" : "OFF"));
  printf("Save report copy :         %s\n",
         (aGameSpec->gameOptions.
          gameOptions & GAME_SAVECOPY ? "ON" : "OFF"));
  printf("Spherical Galaxy :         %s\n",
         (aGameSpec->gameOptions.
          gameOptions & GAME_SPHERICALGALAXY ? "ON" : "OFF"));

  printf("Initial tech levels:\n\
            * Drive:     %5.2f,\n\
            * Weapons:   %5.2f,\n\
            * Shields:   %5.2f,\n\
            * Cargo:     %5.2f.\n", aGameSpec->gameOptions.initial_drive,
		 aGameSpec->gameOptions.initial_weapons,
		 aGameSpec->gameOptions.initial_shields,
		 aGameSpec->gameOptions.initial_cargo);

  printf("Turns of Pax Galactica:   %d\n",
		 aGameSpec->gameOptions.galactic_peace*-1);
  printf("Races:                    %d\n", aGameSpec->numberOfPlayers);
  printf("Home planets/nation:      %d\n", aGameSpec->numberOfHomePlanets);
  printf("Spacing races:            %.1f\n", aGameSpec->minDist);
  printf("Secondary planet radius:  %.1f\n",
         aGameSpec->radiusSecondaryPlanets);
  printf("Empty planets/race:     %d\n",
         aGameSpec->numberOfEmptyPlanets);
  printf("Located in within a:      %.1f radius\n",
         aGameSpec->radiusEmptyPlanets);
  printf("Stuff planets/race:     %d\n",
         aGameSpec->numberOfStuffPlanets);

  for (curNewPlayer = aGameSpec->players;
       curNewPlayer; curNewPlayer = curNewPlayer->next) {
    printf("%s played by %s with password %s\n",
           curNewPlayer->name, curNewPlayer->addr, curNewPlayer->pswd);
  }
}

/**********/


/****f* Create/creategame
 * NAME
 *   creategame -- create a game based on a gamespecification.
 * SYNOPSIS
 *   game     *creategame(gamespecification *)
 *   newGame = creategame(aGameSpec)
 * FUNCTION
 *   Creates all races. For each race a number 
 *   of planets are created and intialized. The number
 *   of races and number of planets are defined in the 
 *   game specification.
 * RESULT   
 *   Pointer to an initialized game structure.
 * SOURCE
 */

game           *
creategame(gamespecification *aGameSpec)
{
  if (createGameDirectories(aGameSpec->name)) {
    int   planet_name;
    int   created_ok;
    game *aGame;
		
    srand((int) time(NULL));
    resetErnie(197162622 + (rand() & 0xFF));
    aGame = allocStruct(game);
    
    setName(aGame, aGameSpec->name);
    aGame->turn = 0;
    aGame->gameOptions.gameOptions = aGameSpec->gameOptions.gameOptions;
    aGame->gameOptions.initial_drive =
      aGameSpec->gameOptions.initial_drive;
    aGame->gameOptions.initial_weapons =
      aGameSpec->gameOptions.initial_weapons;
    aGame->gameOptions.initial_shields =
      aGameSpec->gameOptions.initial_shields;
    aGame->gameOptions.initial_cargo =
      aGameSpec->gameOptions.initial_cargo;
    aGame->gameOptions.galactic_peace =
      aGameSpec->gameOptions.galactic_peace;
    
    /* Do not change this! planets can be looked up by number and this
     * function expects the first planet to have number 1 */
    planet_name = 1;
    if (aGameSpec->teamGame) {
      created_ok = createCheckeredLayout(aGameSpec, aGame, planet_name);
      if (created_ok)
	setTeamAllies(aGame->players);
    }
    else {
      created_ok = createStandardLayout(aGameSpec, aGame, planet_name);
    }
    
    if (created_ok) {
      if (aGame->gameOptions.galactic_peace > 0) {
	player* aPlayer;
	player* aPlayer2;
	
	
	for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
	  for (aPlayer2 = aGame->players; aPlayer2; aPlayer2 = aPlayer2->next) {
	    alliance       *a;
	    
	    if (aPlayer == aPlayer2)
	      continue;
	    
	    a = allocStruct(alliance);
	    
	    a->who = aPlayer2;
	    addList(&aPlayer->allies, a);
	  }
	}
      }
      
      Randomize_Planet_Numbers(aGame);
      preComputeGroupData(aGame);
      raceStatus(aGame);
      return aGame;
    }
    else {
      freegame(aGame);
      return NULL;
    }
  }
  else {
    return NULL;
  }
}

/********/

int
createStandardLayout(gamespecification *aGameSpec,
                     game *aGame, int planet_name)
{
  newplayer      *aNewPlayer;

  aGame->galaxysize = aGameSpec->galaxySize;

  for (aNewPlayer = aGameSpec->players;
       aNewPlayer; aNewPlayer = aNewPlayer->next) {
    planet         *core_planet;
    player         *aPlayer;

    initUniqifyValues();        /* this is so that we can guarantee that
                                 * no two planets will inhabit the same
                                 * coordinates */

    aPlayer = createPlayer(aGameSpec, aGame, aNewPlayer);
    printf("o Adding player %s\n", aPlayer->name);
    addList(&(aGame->players), aPlayer);
    if ((core_planet =
         Add_Core_Home_Planet(aGame, aGameSpec->minDist,
                              &planet_name, aPlayer,
                              aNewPlayer->coreSizes[0],
							  aNewPlayer->coreXOffset[0],
							  aNewPlayer->coreYOffset[0]))) {
      double          x, y;

      x = core_planet->x;
      y = core_planet->y;
      if (aNewPlayer->numberOfHomePlanets > 1) {
        int             cur_extra_home;

	printf("o Adding secondary home planets.\n");
		
	for (cur_extra_home = 1;
             cur_extra_home < aNewPlayer->numberOfHomePlanets;
             cur_extra_home++) {
          Add_Extra_Home_Planets(aGame, x, y,
                                 aNewPlayer->coreSizes[cur_extra_home],
                                 aGameSpec->radiusSecondaryPlanets,
                                 &planet_name, aPlayer,
				 aNewPlayer->coreXOffset[cur_extra_home],
				 aNewPlayer->coreYOffset[cur_extra_home]);
        }
      }

      if (aGameSpec->numberOfEmptyPlanets) {
        Add_Empty_Planets(aGame, aGameSpec->numberOfEmptyPlanets,
                          aGameSpec->radiusEmptyPlanets, x, y,
                          &planet_name, 600.0, 700.0);
      }
      if (aGameSpec->numberOfStuffPlanets) {
        Add_Stuff_Planets(aGame,
                          aGameSpec->numberOfStuffPlanets, &planet_name);
      }
    }
    else {
      printf("Can't space homeworlds at least %f light years apart.\n",
             aGameSpec->minDist);
      printf("Please try again with a larger sized galaxy.\n");
      return FALSE;
    }
  }
  /* Add_Center_Planet(aGame, &planet_name); */
  return TRUE;
}


int
createDistributedLayout(gamespecification *aGameSpec,
						game *aGame, int planet_name)
{
	newplayer* aNewPlayer;

	aGame->galaxysize = aGameSpec->galaxySize;

	for (aNewPlayer = aGameSpec->players;
		 aNewPlayer; aNewPlayer = aNewPlayer->next) {
		planet* core_planet;
		player* aPlayer;

		initUniqifyValues();        /* this is so that we can guarantee that
									 * no two planets will inhabit the same
									 * coordinates */

		aPlayer = createPlayer(aGameSpec, aGame, aNewPlayer);
		printf("o Adding player %s\n", aPlayer->name);
		addList(&(aGame->players), aPlayer);
		if ((core_planet =
			 Add_Core_Home_Planet(aGame, aGameSpec->minDist,
								  &planet_name, aPlayer,
								  aNewPlayer->coreSizes[0],
								  aNewPlayer->coreXOffset[0],
								  aNewPlayer->coreYOffset[0]))) {
			double x, y;
			
			x = core_planet->x;
			y = core_planet->y;
			if (aNewPlayer->numberOfHomePlanets > 1) {
				int             cur_extra_home;
				
				printf("o Adding secondary home planets.\n");
				for (cur_extra_home = 1;
					 cur_extra_home < aNewPlayer->numberOfHomePlanets;
					 cur_extra_home++) {
					Add_Extra_Home_Planets(aGame, x, y,
										   aNewPlayer->coreSizes[cur_extra_home],
										   aGameSpec->radiusSecondaryPlanets,
										   &planet_name, aPlayer,
										   aNewPlayer->coreXOffset[cur_extra_home],
										   aNewPlayer->coreYOffset[cur_extra_home]);
				}
			}
			if (aGameSpec->numberOfEmptyPlanets) {
				Add_Empty_Planets(aGame, aGameSpec->numberOfEmptyPlanets,
								  aGameSpec->radiusEmptyPlanets, x, y,
								  &planet_name, 600.0, 700.0);
			}
			if (aGameSpec->numberOfStuffPlanets) {
				Add_Stuff_Planets(aGame,
								  aGameSpec->numberOfStuffPlanets, &planet_name);
			}
		}
		else {
			printf("Can't space homeworlds at least %f light years apart.\n",
				   aGameSpec->minDist);
			printf("Please try again with a larger sized galaxy.\n");
			return FALSE;
		}
	}
	/* Add_Center_Planet(aGame, &planet_name); */
	return TRUE;
}

/****** Create/createCheckeredLayout
 *
 *  0  1  0  1 ... 0  1
 *  1  0  1  0 ... 1  0
 *  ...
 *  0  1  0  1 ... 0  1
 *
 *****/

int
createCheckeredLayout(gamespecification *aGameSpec,
                      game *aGame, int planet_name)
{
  int             row;
  int             column;
  int             team;
  player         *aPlayer;

  team = 0;

  aGame->galaxysize = 10;
  if (aGameSpec->numberOfRows > aGameSpec->numberOfColumns) {
    aGame->galaxysize += aGameSpec->numberOfRows * aGameSpec->minDist;
  }
  else {
    aGame->galaxysize += aGameSpec->numberOfColumns * aGameSpec->minDist;
  }

  Add_Center_Planet(aGame, &planet_name);

  for (row = 0; row < aGameSpec->numberOfRows; row++) {
    team = row & 1;
    for (column = 0; column < aGameSpec->numberOfColumns; column++) {
      newplayer      *aNewPlayer;

      printf("location (%d,%d)\n", row, column);
      if (aNewPlayer &&
          !(((row == 0) && (column == 0)) ||
            ((row == 0) && (column == 7)) ||
            ((row == 7) && (column == 0)) ||
            ((row == 7) && (column == 7)) ||
            ((row == 4) && (column == 3)) ||
            ((row == 4) && (column == 4)) ||
            ((row == 3) && (column == 3)) || ((row == 3) && (column == 4))
          )) {
        aNewPlayer = getNextTeamPlayer(aGameSpec->players, team);
        if (!aNewPlayer) {
          printf("No new player found\n");
        }
        if (aNewPlayer) {
          player         *aPlayer;
          int             cur_extra_home;
          double          x;
          double          y;

          x = 5 + (row + 1) * aGameSpec->minDist -
              0.5 * aGameSpec->minDist;
          y = 5 + (column + 1) * aGameSpec->minDist -
              0.5 * aGameSpec->minDist;
          aPlayer = createPlayer(aGameSpec, aGame, aNewPlayer);
          printf("o Adding player %s\n", aPlayer->name);
          addList(&(aGame->players), aPlayer);

          for (cur_extra_home = 0;
               cur_extra_home < aNewPlayer->numberOfHomePlanets;
               cur_extra_home++) {
            printf("o Adding secondary home planets.\n");
            Add_Extra_Home_Planets(aGame, x, y,
                                   aNewPlayer->coreSizes[cur_extra_home],
                                   aGameSpec->radiusSecondaryPlanets,
                                   &planet_name, aPlayer,
								   aNewPlayer->coreXOffset[cur_extra_home],
								   aNewPlayer->coreYOffset[cur_extra_home]);
          }
          if (aGameSpec->numberOfEmptyPlanets) {
            Add_Empty_Planets(aGame, aGameSpec->numberOfEmptyPlanets,
                              aGameSpec->radiusEmptyPlanets, x, y,
                              &planet_name, 600.0, 700.0);
          }
        }
      }
      if ((column > 0) && (row > 0)) {
        double          x;
        double          y;

        x = 5 + row * aGameSpec->minDist;
        y = 5 + column * aGameSpec->minDist;
        Add_Extra_Home_Planets(aGame, x, y, 4000, 3.3, &planet_name, NULL,
							   0.0, 0.0);
      }
      team++;
      if (team > 1) {
        team = 0;
      };
    }
  }


  for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
    if (aGameSpec->numberOfStuffPlanets) {
      int             i;

      for (i = 0; i < aGameSpec->numberOfStuffPlanets; i++) {
        Add_Team_Stuff_Planets(aGame, aGameSpec, &planet_name);
      }
    }
  }
  return TRUE;
}

struct uniq_avl {
  struct avl      avl;
  char            key[17];      /* key is "%08d%08d", x*1000000000, y*100
                                 * -- this gives us room for a 10,000
                                 * square map */
};

int
cmpstr(void *a, void *b)
{
  /* printf("cmpstr: %p:%s %p:%s\n", a, ((struct uniq_avl*)a)->key, b,
   * ((struct uniq_avl*)b)->key); */
  return strcmp(((struct uniq_avl *) a)->key,
                ((struct uniq_avl *) b)->key);
}

static int
avliter(avl * val)
{
  /* this is only used for determining unique location for a planet, so if 
   * there is one here, return 1 since dups are not allowed */
  struct uniq_avl *node = (struct uniq_avl *) val;

  printf("%s is duplicate.\n", node->key);

  return 1;
}

struct avl_tree unique;
struct uniq_avl *unode;

void
initUniqifyValues()
{
  unique.compar = cmpstr;
  unique.root = 0;
}

void
Add_Team_Stuff_Planets(game *aGame,
                       gamespecification *aGameSpec, int *planet_name)
{
  planet         *aPlanet;
  double          min_dist;
  double          x, y;
  int             is_ok;
  int             count;

  min_dist = aGameSpec->radiusEmptyPlanets;
  for (is_ok = FALSE, count = 0; !is_ok && (count < 1000); count++) {
    is_ok = TRUE;
    x = round2(frand(aGame->galaxysize));
    y = round2(frand(aGame->galaxysize));
    if (x < 0 || x >= aGame->galaxysize)
      is_ok = FALSE;
    if (y < 0 || y >= aGame->galaxysize)
      is_ok = FALSE;
    for (aPlanet = aGame->planets; aPlanet; aPlanet = aPlanet->next) {
      if (aPlanet->owner) {
        double          dx, dy;

        dx = aPlanet->x - x;
        dy = aPlanet->y - y;
        if (dx * dx + dy * dy <= min_dist * min_dist) {
          is_ok = FALSE;
          break;
        }
      }
    }
  }
  if (count < 1000) {
    aPlanet = addplanet(aGame);
    (*planet_name)++;
    aPlanet->x = x;
    aPlanet->y = y;
    aPlanet->owner = NULL;
    aPlanet->producing = PR_DRIVE;
    aPlanet->size = 2.0 + frand(170.0);
    aPlanet->resources = round2(pow(2.0, frand2()));
    aPlanet->producing = PR_CAP;
    printf("stuff planet added\n");
  }
}


void
setTeamAllies(player *firstPlayer)
{
  player         *aPlayer;
  player         *ally;

  for (aPlayer = firstPlayer; aPlayer; aPlayer = aPlayer->next) {
    for (ally = firstPlayer; ally; ally = ally->next) {
      if ((ally != aPlayer) && (ally->team == aPlayer->team)) {
        alliance       *a;
        a = allocStruct(alliance);

        a->who = ally;
        addList(&aPlayer->allies, a);
      }
    }
  }
}


newplayer      *
getNextTeamPlayer(newplayer *aNewPlayer, int team)
{
  for (; aNewPlayer; aNewPlayer = aNewPlayer->next) {
    if (!(aNewPlayer->done) && (aNewPlayer->team == team)) {
      aNewPlayer->done = TRUE;
      break;
    }
  }
  return aNewPlayer;
}



/****f* Create/createGameDirectories
 * NAME
 *   createGameDirectories --
 * FUNCTION
 *   Creates a number of sub directories that are used to store
 *   files of a game. Each subdirectory has the same name as
 *   the game. They are created in the directories:
 *      reports/
 *      orders/
 *      statistics/
 * INPUTS
 *    name -- name of the game
 * SOURCE
 */

int
createGameDirectories(char *name)
{
  sprintf(lineBuffer, "%s/data/%s", galaxynghome, name);
  if (GOS_mkdir(lineBuffer, 0700)) {
#if 0
    fprintf(stderr, "Warning a game under that name already exists\n");
    fprintf(stderr, "Do you want to continue [y/n]?\n");
    scanf("%s", lineBuffer);
    if (*lineBuffer != 'y')
      return FALSE;
#endif
  }
  else {
    sprintf(lineBuffer, "%s/reports/%s", galaxynghome, name);
    GOS_mkdir(lineBuffer, 0700);
    sprintf(lineBuffer, "%s/orders/%s", galaxynghome, name);
    GOS_mkdir(lineBuffer, 0700);
    sprintf(lineBuffer, "%s/statistics/%s", galaxynghome, name);
    GOS_mkdir(lineBuffer, 0700);
    sprintf(lineBuffer, "%s/forecasts/%s", galaxynghome, name);
    GOS_mkdir(lineBuffer, 0700);
  }
  return TRUE;
}

/*******/


/****i* Create/createPlayer
 * NAME
 *   createPlayer -- allocate and initialize a player structure.
 * SYNOPSIS
 *    player *createPlayer(game *aGame, newplayer *aNewPlayer)
 * FUNCTION
 * RESULT
 *   Pointer to an initialized player structure. 
 * SOURCE
 */

player         *
createPlayer(gamespecification *aGameSpec, game *aGame,
             newplayer *aNewPlayer)
{
  player         *aPlayer;

  aPlayer = allocStruct(player);

  setName(aPlayer, aNewPlayer->name);
  aPlayer->pswd = strdup(aNewPlayer->pswd);
  aPlayer->addr = strdup(aNewPlayer->addr);
  aPlayer->team = aNewPlayer->team;
  aPlayer->realName = strdup("none");
  aPlayer->pswdstate = 1;
  aPlayer->drive = aGameSpec->gameOptions.initial_drive;
  aPlayer->weapons = aGameSpec->gameOptions.initial_weapons;
  aPlayer->shields = aGameSpec->gameOptions.initial_shields;
  aPlayer->cargo = aGameSpec->gameOptions.initial_cargo;
  aPlayer->msize = aGame->galaxysize;
  aPlayer->flags =
      F_ANONYMOUS | F_AUTOUNLOAD | F_PRODTABLE | F_SORTGROUPS |
      F_GROUPFORECAST | F_PLANETFORECAST | F_SHIPTYPEFORECAST |
      F_ROUTESFORECAST /*| F_XMLREPORT */| F_TXTREPORT ;
  /* F_MACHINEREPORT */
  return aPlayer;
}

/****/


/****i* Create/printPlanetStats
 * NAME
 *   printPlanetStats
 * SOURCE
 */

void
printPlanetStats(planet *p)
{
  printf("  Planet %s %.2f %.2f %.2f %.2f %.2f %.2f\n",
         p->name, p->x, p->y, p->size, p->pop, p->ind, p->resources);
}

/*********/


/****f* Create/Add_Stuff_Planets
 * NAME
 *   Add_Stuff_Planets -- add a number of stuff planets.
 * SYNOPSIS
 *  void Add_Stuff_Planets(game *, int, int *)
 *       Add_Stuff_Planets(aGame, stuff_planets, planet_name)
 * FUNCTION
 *   Stuff planets are small useless planets that are randomly
 *   scattered accross the galaxy to fill up the empty space
 *   between the empires.
 * INPUTS
 *   aGame         -- game to add the planets to.
 *   stuff_planets -- the number of stuff planets to be added.
 *   planet_name   -- the name (number) of the previous planet.
 * RESULT
 *   A number of planet are added to the game's planet list.
 *   planet_name is modified, will have the name (number) of
 *   the last planet that was added. 
 * SOURCE
 */

void
Add_Stuff_Planets(game *aGame, int stuff_planets, int *planet_name)
{
  int             cur_planet_nr;
  planet         *p;

  double          new_x, new_y;
  int             is_ok;

  for (cur_planet_nr = 0; cur_planet_nr < stuff_planets; cur_planet_nr++) {

    unode = (struct uniq_avl *) malloc(sizeof(struct uniq_avl));

    for (is_ok = FALSE; !is_ok;) {
      is_ok = TRUE;
      new_x = round2(frand(aGame->galaxysize));
      new_y = round2(frand(aGame->galaxysize));
      if (new_x < 0 || new_x >= aGame->galaxysize) {
        is_ok = FALSE;
        continue;
      }
      if (new_y < 0 || new_y >= aGame->galaxysize) {
        is_ok = FALSE;
        continue;
      }
      sprintf(unode->key, "%08d%08d", (int) (new_x * 100),
              (int) (new_y * 100));
      if (avl_search(&unique, (avl *) unode, avliter) == 0) {
        avl_insert(&unique, (avl *) unode);
      }
      else {
        printf("x, y: %.2f,%.2f  ;; key: %s\n", new_x, new_y, unode->key);
        is_ok = FALSE;
        continue;
      }
    }

    p = addplanet(aGame);
    (*planet_name)++;
    p->x = new_x;
    p->y = new_y;
    p->size = 2.0 + frand(170.0);
    p->resources = round2(pow(2.0, frand2()));
    p->producing = PR_CAP;
  }
}

/**********/


/****i* Create/Add_Circle_Stuff_Planets
 * NAME
 *   Add_Circle_Stuff_Planets --
 *****
 */

void
Add_Circle_Stuff_Planets(game *aGame,
                         int stuff_planets, int *planet_name,
                         double sphere)
{
  int             cur_planet_nr;
  planet         *p;
  double          x, y, r;
  double          beta, radius;

  for (cur_planet_nr = 0; cur_planet_nr < stuff_planets; cur_planet_nr++) {
    p = addplanet(aGame);
    (*planet_name)++;
    r = (aGame->galaxysize - 4.0) / 2.0;
    beta = frand(360.0) * (2 * 3.141592654 / 360.0);
    radius = 0.3 * r + r * frand(0.7);
    x = radius * cos(beta) + aGame->galaxysize / 2.0;
    y = radius * sin(beta) + aGame->galaxysize / 2.0;
    assert((x < aGame->galaxysize) && (x > 0));
    assert((y < aGame->galaxysize) && (y > 0));
    p->x = x;
    p->y = y;
    p->size = 2.0 + frand(170.0);
    p->resources = round2(pow(2.0, frand2()));
    p->producing = PR_CAP;
  }
}


/****f* Create/Randomize_Planet_Numbers
 * NAME
 *   Randomize_Planet_Numbers -- shuffle planet names.
 * FUNCTION
 *   Randomly shuffle the planet names.  The method used is randomly
 *   selecting two planets and swapping all their data. This process
 *   is repeated 2000 times.  The names are randomized to prevent
 *   players from guessing which planets are the good planets and bad
 *   planets around the races core home planets.
 * NOTES
 *   This is not a particular good or speedy way of shuffling.  
 *   Someone suggested a much faster method, but that got misplaced.
 * SOURCE
 */

void
Randomize_Planet_Numbers(game *aGame)
{
  int             total_nr_of_planets;
  planet         *p;
  int             randomizor;

  for (p = aGame->planets, total_nr_of_planets = 0; p; p = p->next) {
    total_nr_of_planets++;
  }

  for (randomizor = 0; randomizor < 2000; randomizor++) {
    int             pln1, pln2, nm;
    planet         *p1, *p2;

    pln1 = rand() % total_nr_of_planets;
    pln2 = rand() % total_nr_of_planets;
    for (p1 = aGame->planets, nm = 0; nm < pln1; p1 = p1->next, nm++);
    for (p2 = aGame->planets, nm = 0; nm < pln2; p2 = p2->next, nm++);

    if (!p1->owner && !p2->owner) {
      player         *temp;

      temp = p2->owner;
      p2->owner = p1->owner;
      p1->owner = temp;
      swap_data(&p1->x, &p2->x);
      swap_data(&p1->y, &p2->y);
      swap_data(&p1->size, &p2->size);
      swap_data(&p1->pop, &p2->pop);
      swap_data(&p1->ind, &p2->ind);
      swap_data(&p1->resources, &p2->resources);
    }
  }
}

/***********/


/****f* Create/Add_Core_Home_Planet
 * NAME
 *   Add_Core_Home_Planet -- add a core home planet for a race.
 * FUNCTION
 *   Each race has at least one home planet. This is it's core home
 *   planet. The other home planets are located around it.  This
 *   function allocates a core home planet. It makes sure that there
 *   is a minimum distance between all core home planets.  The core
 *   home planets are all located on a big disc that fits inside the
 *   square galaxy.  The disc is used to prevent races ending up in
 *   one of the corners.
 * INPUTS
 *   aGame        -- game to add the planet to.
 *   min_dist     -- minimum distance between the core planets.
 *   planet_name  -- name of the previously added planet.
 *   aPlayer      -- race the core planet will belong to.
 * RESULTS
 *   A core plane is added to the game's planet list.  planet_name is
 *   modified, will have the name (number) of this planet.
 *   NULL if something went wrong.
 *****
 */

planet *
Add_Core_Home_Planet(game *aGame, double min_dist, int *planet_name,
                     player *aPlayer, double size, double px, double py)
{
	int             try;
	int             found_location;
	planet         *p;
	double          x, y, beta, radius;
	double  half_galaxy = aGame->galaxysize / 2.0;
	
	printf("o Adding main home planet.\n");

	unode = (struct uniq_avl *) malloc(sizeof(struct uniq_avl));
	
	/* if x and y are passed, use them */
	if (px > 0.0 && py > 0.0) {
		x = px;
		y = py;
		sprintf(unode->key, "%08d%08d", (int) (x * 100), (int) (y * 100));
		if (avl_search(&unique, (avl *) unode, avliter) == 0) {
			avl_insert(&unique, (avl *) unode);
		}
	}
	else {
		for (found_location = FALSE, try = 0; !found_location; try++) {
			double dx, dy;
		
			found_location = TRUE;
			beta = frand(360.0) * 0.01745329; /* (2 * 3.141592654 / 360.0) */
			radius = frand(half_galaxy - 4.0);
			x = round2(radius * cos(beta) + half_galaxy);
			y = round2(radius * sin(beta) + half_galaxy);
			assert((x < aGame->galaxysize) && (x > 0));
			assert((y < aGame->galaxysize) && (y > 0));
			sprintf(unode->key, "%08d%08d", (int) (x * 100), (int) (y * 100));
			if (avl_search(&unique, (avl *) unode, avliter) == 0) {
				avl_insert(&unique, (avl *) unode);
			}
			else {
				printf("x, y: %.2f,%.2f  ;; key: %s\n", x, y, unode->key);
				found_location = FALSE;
				if (try == 4000)
					return NULL;
				continue;
			}
			
			for (p = aGame->planets; p; p = p->next) {
				if (p->owner) {
					dx = p->x - x;
					dy = p->y - y;
					if (dx * dx + dy * dy <= min_dist * min_dist) {
						found_location = FALSE;
						if (unique.root)
							avl_remove(&unique, (avl *) unode);
						
						if (try == 4000) {
							return NULL;
						}
						
					}
				}
			}
		}
	}
	
	p = addplanet(aGame);
	(*planet_name)++;
	p->x = x;
	p->y = y;
	p->owner = aPlayer;
	p->size = size;
	p->resources = 10.0;
	p->pop = size;
	p->ind = size;
	p->producing = PR_DRIVE;
	printPlanetStats(p);
	return p;
}


/****i* Create/Add_Circle_Home_Planet
 * NAME
 *   Add_Circle_Home_Planet
 * NOTES
 *   Currently not used since the code to create a circle game
 *   has not yet been added to creategame().
 *****
 */

planet         *
Add_Circle_Home_Planet(game *aGame,
                       int pnum, double min_dist, double sphere,
                       int *planet_name, player *aPlayer, int num_players)
{

  int             found_location;
  planet         *p;
  double          x, y;
  double          alpha, beta, radius;

#ifdef WIN32
  float           M_PI = 3.14159265358979;
#endif

  found_location = TRUE;
  if (num_players < 3)
    return Add_Core_Home_Planet(aGame, min_dist, planet_name, aPlayer,
                                1000.0, 0.0, 0.0);

  radius = min_dist / (2.0 * sin(M_PI / num_players));
  alpha = M_PI * (num_players - 2) / num_players;

  beta = (pnum - 1) * (M_PI - alpha);
  x = radius * cos(beta) + (aGame->galaxysize - sphere) / 2.0;
  y = radius * sin(beta) + (aGame->galaxysize - sphere) / 2.0;

  p = addplanet(aGame);
  (*planet_name)++;
  p->x = round2(x);
  p->y = round2(y);
  p->owner = aPlayer;
  p->size = MAXPOP;
  p->resources = 10.0;
  p->pop = MAXPOP;
  p->ind = MAXPOP;
  p->producing = PR_DRIVE;

  return p;
}


/****f* Create/Add_Extra_Home_Planets
 * NAME
 *   Add_Extra_Home_Planets
 * FUNCTION
 *   In addition to its core home planet a race can have
 *   a number of additional home planets. This functions adds these.
 *   They are added randomly on a donut shape disk centered around
 *   the core home planet. The donut hole has a radius of 1, and the
 *   donut disk has a radius of max_radius.
 * SOURCE
 */

void
Add_Extra_Home_Planets(game *aGame,
                       double x, double y,
                       double size,
                       double max_radius,
                       int *planet_name, player *aPlayer,
					   double px, double py)
{
	planet         *p;

	double          radius, angle;
	double          new_x, new_y;
	int             is_ok;
	
	unode = (struct uniq_avl *) malloc(sizeof(struct uniq_avl));
	
	if (px > 0.0 && py > 0.0) {
		new_x = px;
		new_y = py;
		sprintf(unode->key, "%08d%08d", (int) (new_x * 100),
				(int) (new_y * 100));
		if (avl_search(&unique, (avl *) unode, avliter) == 0) {
			avl_insert(&unique, (avl *) unode);
		}
	}
	else {
		for (is_ok = FALSE; !is_ok;) {
			is_ok = TRUE;
			radius = 1.0 + frand(max_radius - 1.0);
			angle = frand(360.0) * (2 * 3.141592654 / 360.0);
			new_x = round2(x + sin(angle) * radius);
			new_y = round2(y + cos(angle) * radius);
			if (new_x < 0 || new_x >= aGame->galaxysize) {
				is_ok = FALSE;
				continue;
			}
			if (new_y < 0 || new_y >= aGame->galaxysize) {
				is_ok = FALSE;
				continue;
			}
			sprintf(unode->key, "%08d%08d", (int) (new_x * 100),
					(int) (new_y * 100));
			if (avl_search(&unique, (avl *) unode, avliter) == 0) {
				avl_insert(&unique, (avl *) unode);
			}
			else {
				printf("x, y: %.2f,%.2f  ;; key: %s\n", new_x, new_y, unode->key);
				is_ok = FALSE;
				continue;
			}
		}
	}
	p = addplanet(aGame);
	(*planet_name)++;
	p->x = round2(new_x);
	p->y = round2(new_y);
	p->owner = aPlayer;
	p->size = size;
	p->producing = PR_DRIVE;
	p->resources = 10;
	p->pop = size;
	p->ind = size;
	printPlanetStats(p);
}

/*******/


/****i* Create/Add_Extra_Circle_Stuff_Planets
 * NAME
 *   Add_Extra_Circle_Stuff_Planets
 ****
 */

void
Add_Extra_Circle_Stuff_Planets(game *aGame,
                               int circle_stuff_planets,
                               double x,
                               double y, int *planet_name, int min_dist)
{
  int             pno;
  planet         *p;

  unode = (struct uniq_avl *) malloc(sizeof(struct uniq_avl));

  for (pno = 0; pno < circle_stuff_planets; pno++) {
    double          radius, angle;
    double          new_x, new_y;
    int             is_ok;

    for (is_ok = FALSE; !is_ok;) {
      is_ok = TRUE;
      radius = 1.0 + frand(min_dist);
      angle = frand(360.0) * (2 * 3.141592654 / 360.0);
      new_x = round2(x + sin(angle) * radius);
      new_y = round2(y + cos(angle) * radius);
      if (new_x < 0 || new_x >= aGame->galaxysize) {
        is_ok = FALSE;
        continue;
      }
      if (new_y < 0 || new_y >= aGame->galaxysize) {
        is_ok = FALSE;
        continue;
      }
      sprintf(unode->key, "%08d%08d", (int) (new_x * 100),
              (int) (new_y * 100));
      if (avl_search(&unique, (avl *) unode, avliter) == 0) {
        avl_insert(&unique, (avl *) unode);
      }
      else {
        printf("x, y: %.2f,%.2f  ;; key: %s\n", new_x, new_y, unode->key);
        is_ok = FALSE;
        continue;
      }

    }
    p = addplanet(aGame);
    (*planet_name)++;
    p->x = new_x;
    p->y = new_y;
    p->size = 2.0 + frand(170.0);
    p->resources = round2(pow(2.0, frand2()));
    p->producing = PR_DRIVE;
  }
}

/****f* Create/Add_Center_Planet
 * NAME
 *   Add_Center_Planet -- add a planet in the exact centre of the galaxy.
 * SOURCE
 */

void
Add_Center_Planet(game *aGame, int *planet_name)
{
  planet         *p;

  p = addplanet(aGame);
  (*planet_name)++;
  p->x = round2(aGame->galaxysize / 2.0);
  p->y = round2(aGame->galaxysize / 2.0);
  p->size = 5000.0;
  p->producing = PR_DRIVE;
  p->resources = round2(pow(2.0, frand2()));
}

/********/


/****f* Create/Add_Empty_Planets
 * NAME
 *   Add_Empty_Planets -- add some good size empty planets.
 * FUNCTION
 *   This adds a number of good (relatively big sized) planets
 *   around the core home planet of a race.
 *   The sum of the sizes of all these planets is about the
 *   same for each race.
 * SOURCE
 */

void
Add_Empty_Planets(game *aGame,
                  int nplanets, double empty_dist,
                  double x, double y,
                  int *planet_name, double low_range, double up_range)
{
  double          tot_mass;
  double         *planet_sizes;
  int             cur_planet_nr;
  double          cur_total_size;
  planet         *p;

  printf("o Adding empty planets.\n");
  planet_sizes = alloc(nplanets * sizeof(double));

  for (cur_total_size = 0.0, cur_planet_nr = 0; cur_planet_nr < nplanets; cur_planet_nr++) {
    planet_sizes[cur_planet_nr] = frand(MAXPOP - 80) + 80.0;
    cur_total_size += planet_sizes[cur_planet_nr];
  }

  /* Adjust the Planet sizes in order to make sure that each players get
   * about the same total amount */

  for (;
       (cur_total_size < nplanets * low_range) ||
       (cur_total_size > nplanets * up_range);) {
    int             pl_sel;
    float           adj_factor;

    adj_factor = 7.5 + frand(5);
    pl_sel = frand3(nplanets);
    if (cur_total_size > nplanets * up_range) {
      (planet_sizes[pl_sel]) -= adj_factor;
      cur_total_size -= adj_factor;
    }
    else {
      (planet_sizes[pl_sel]) += adj_factor;
      cur_total_size += adj_factor;
    }

    if (planet_sizes[pl_sel] < 20.0) {
      planet_sizes[pl_sel] += adj_factor;
      cur_total_size += adj_factor;
    }

    if (planet_sizes[pl_sel] > 980.0) {
      planet_sizes[pl_sel] -= adj_factor;
      cur_total_size -= adj_factor;
    }

  }

  for (cur_planet_nr = 0, tot_mass = 0.0;
       cur_planet_nr < nplanets; cur_planet_nr++) {
    double          radius, angle;
    double          new_x, new_y;
    int             is_ok;

    unode = (struct uniq_avl *) malloc(sizeof(struct uniq_avl));

    for (is_ok = FALSE; !is_ok;) {
      is_ok = TRUE;
      radius = 0.2 + frand(empty_dist - 0.2);
      angle = frand(360.0) * (2 * 3.141592654 / 360.0);
      new_x = round2(x + sin(angle) * radius);
      new_y = round2(y + cos(angle) * radius);
      if (new_x < 0 || new_x >= aGame->galaxysize) {
        is_ok = FALSE;
        continue;
      }
      if (new_y < 0 || new_y >= aGame->galaxysize) {
        is_ok = FALSE;
        continue;
      }
      sprintf(unode->key, "%08d%08d", (int) (new_x * 100),
              (int) (new_y * 100));
      if (avl_search(&unique, (avl *) unode, avliter) == 0) {
        avl_insert(&unique, (avl *) unode);
      }
      else {
        printf("x, y: %.2f,%.2f  ;; key: %s\n", new_x, new_y, unode->key);
        is_ok = FALSE;
        continue;
      }

    }
    p = addplanet(aGame);
    (*planet_name)++;
    p->x = new_x;
    p->y = new_y;
    p->size = planet_sizes[cur_planet_nr];
    p->resources = round2(0.3 + pow(8.0, frand2()));
    p->producing = PR_CAP;
    tot_mass += (p->size) / (10.0 + 1.0 / p->resources);
    printPlanetStats(p);
  }
  printf("  Total production mass: %.2f\n", tot_mass);
  free(planet_sizes);
}

/*****/


/****f* Create/swap_data
 * NAME
 *   swap_data -- swap two doubles
 * SYNOPSIS
 *   void swap_data(double *v1, double *v2)
 ******
 */

void
swap_data(double *v1, double *v2)
{
  double          temp;

  temp = *v1;
  *v1 = *v2;
  *v2 = temp;
}



/****i* Create/CalcCirclegalaxysize
 * NAME
 *   CalcCirclegalaxysize
 *****
 */

double
CalcCirclegalaxysize(int num, double dist, double sphere)
{

  /* Will not actually create a circle, rather a regular polygon with as * 
   *
   * * * * *  *  * * many */
  /* sides as we have players. Then we calculate diameter and add a few */
  /* lightyears depending on the radius within which we want our empty */
  /* systems. TL 980629 */
  double          result;

#ifdef WIN32
  float           M_PI = 3.14159265358979;

#endif
  result = dist / sin(M_PI / num);

  /* now we add the fuzzines factor to the radius we have gotten. */
  result += 4.0 * sphere;

  return result;
}
