#include "galaxyng.h"

/****f* CLI/CMD_template
 * NAME
 *   CMD_template -- Create a template .glx file that the GM can edit.
 * SYNOPSIS
 *   ./galaxyng <name> <number of players>
 * FUNCTION
 *   Creates a template .glx file.
 *   All parameters are given some sensible default values.
 *
 *   The number of players is used to determine the size of the galaxy,
 *   using the following formule
 *      42 * ceil (sqrt(number of players))
 *   For games with very few players this size is sometimes too small.
 * SOURCE
 */

/*
 * NOTE: this function will be changing once the ARE modifications are
 * complete as we'll need to read from the .arerc to set the game
 * parameters.
 */

int CMD_template( int argc, char **argv ) {
  FILE *glxfile;
  char *glxname;
  int numberOfPlayers;
  int result = EXIT_FAILURE;
  
  if (argc != 4) {
    usage();
    return result;
  }
  
  glxname = createString("%s.glx", argv[2] );
  numberOfPlayers = atoi(argv[3]);
  
  if ((glxfile = GOS_fopen( glxname, "w" ))) {
    int i;
    int gsize;
    
    fprintf( glxfile, "name %s\n\n", argv[2] );
    
    /* ensure the galaxy size is a multiple of 10 */
    gsize = (int) (42.0 * ceil(sqrt((double)numberOfPlayers)));
    gsize /= 10;
    gsize *= 10;
    
    fprintf(glxfile,
	    "; The following size is only approximately right.\n"
	    "; You probably want to experiment with different sizes to get\n"
	    "; a galaxy that looks right. It should not be too crowded nor\n"
	    "; too sparse. The size should be a multiple of ten.\n\n"
	    "size %d\n\n", gsize);
    
    fprintf(glxfile,
	    "; The engine will make sure that distance between the\n"
	    "; primary homeworlds is at least 30 light years.\n\n"
	    "race_spacing 30\n\n");
		
    fprintf(glxfile,
	    "; The sizes of the homeworlds for each race.\n"
	    "; The following would give each race 3 homeworlds of sizes\n"
	    "; 1000 250 350. The first one is the primary homeworld.\n"
	    "; You have to define these before any of the player definitions.\n\n"
	    "core_sizes 1000 250 350\n\n" );
    
    fprintf(glxfile,
	    "; Within a radius [2,r] from the primary homeworld the engine\n"
	    "; allocates a number of empty planets, size 200 - 1,000 for the\n"
	    "; race to colonize. The following two parameters define how\n"
	    "; many there are per race and within what radius. A number\n"
	    "; between 4 and 10 and a radius of  race_spacing/2.0 is a\n"
	    "; good guess.\n\n"
	    "empty_planets 6\nempty_radius 15\n\n");
    
    fprintf(glxfile,
	    "; It is possible to add a number of 'stuff' planets. These are\n"
	    "; useless planets, all of size 200 or less, that are use to\n"
	    "; fill up the empty space between the home worlds. They make it\n"
	    "; possible for a players to approach (attack) other players by\n"
	    "; different routes. The following parameter specifies how many\n"
	    "; there are per race.\n\nstuff_planets 8\n\n");

    fprintf(glxfile,
	    "; Initial tech levels:\n"
	    "; In order : Drive Weapons Shields Cargo. They can't be lower\n"
	    "; than 1. In the case below, drive will be forced to 1 by the\n"
	    "; server. Uncomment the next line if you want this option.\n\n"
	    ";InitialTechLevels 0.42 1 3.21 1.24\n\n");
    
    fprintf(glxfile,
	    "; Full bombing:\n"
	    "; Bombed planets are completely bombed and all population,\n"
	    "; industry, capital, colonists, and materials are gone.\n"
	    "; Normally the population and industry is reduced to 25%% of\n"
	    "; its original value. Uncomment the next line if you want\n"
	    "; this option.\n\n"
	    ";FullBombing\n\n");
		
    fprintf(glxfile,
	    "; Pax Galactica:\n"
	    "; You can enforce global peace just by putting in the number\n"
	    "; of turns you want to disallow players to declare war\n"
	    "; on each other. For example, Peace 20 means players cannot\n"
	    "; declare war until turn 21. (Note that players can load cargo\n"
	    "; at other players planets. If you do not wish to allow this\n"
	    "; you must make it a house rule. It cannot be prevented by the\n"
	    "; GalaxyNG server program.) Uncomment the next line if you\n"
	    "; want this option.\n\n"
	    ";Peace 20\n\n");
		
    fprintf(glxfile,
	    "; Keep Production:\n"
	    "; If keepproduction is set, the production points spent\n"
	    "; on the previous product are preserved, otherwise all points\n"
	    "; are lost. Uncomment the next line if you want this option.\n\n"
	    ";KeepProduction\n\n"); \
		
    fprintf(glxfile,
	    "; Don't kill off players:\n"
	    "; Don't remove idle races from a game. Normally if players do\n"
	    "; not send in orders for a couple of turns their race self\n"
	    "; destructs. Uncomment the next line if you want this option.\n\n"
	    ";DontDropDead\n\n");
		
#if 0
    fprintf(glxfile,
	    "; Keep report copies:\n"
	    "; Sometimes, if you have enough disk space, it is nice to get a\n"
	    "; copy of turn reports that are send out to the players.\n"
	    "; If you uncomment the following parameter, a copy of each turn\n"
	    "; report that is send out is stored in reports/<game name>/\n"
	    "; Uncomment the next line if you want this option.\n\n"
	    ";SaveReportCopy\n\n");
#endif
		
    fprintf(glxfile,
	    "; Make the galaxy a sphere:\n"
	    "; The galaxy can be (roughly) mapped on a sphere\n"
	    "; This way, the gap between x (or y) coordinates of two\n"
	    "; planets is computed with border lines crossing and\n"
	    "; reappearing on the other side. Uncomment the next line\n"
	    "; if you want this option.\n\n"
	    ";sphericalgalaxy\n\n");
	    		
    fprintf(glxfile,
	    "; Here is the list of players.  You must set the email\n"
	    "; address of each player that enrolled in your game.\n\n");
    
    fprintf(glxfile,
	    "; You can also set the size of the homeworlds for each player.\n"
	    "; For instance the following player will get one homeworld\n"
	    "; of size 1600.0\n\n"
	    ";start_player\n"
	    ";email race_1@their.address.tld\n"
	    ";homeworld 1600\n"
	    ";end_player\n\n");
    
    fprintf(glxfile,
	    "; You can also set the coordinates of each homeworld. The\n"
	    "; following player gets 3 home planets of sizes 500.0, 100.0,\n"
	    "; and 1000.0 grouped around coordinate 50,50.\n\n"
	    ";start_player\n"
	    ";email race_1@their.address.tld\n");
    
    fprintf(glxfile,
	    ";homeworld 500 50 50\n"
	    ";homeworld 100 45 55\n"
	    ";homeworld 1000 55 45\n"
	    ";end_player\n\n");
    
    fprintf(glxfile,
	    "; Note that the server assumes that the first homeworld listed\n"
	    "; is the main world for determining development planet radius\n"
	    "; and distance between races.\n\n");

    for (i = 1; i <= numberOfPlayers; i++) {
      fprintf(glxfile, "start_player\nemail race_%d@their.address.tld\n",
	      i);
      fprintf(glxfile, "end_player\n\n");
    }
    fprintf(glxfile, "\n");
		
    fclose(glxfile);
    printf( "Created the file \"%s\".\n", glxname );
  } else {
    fprintf( stderr, "Can't open \"%s\".\n", glxname );
  }
	
  free( glxname );
	
  return result;
}
