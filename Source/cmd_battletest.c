#include "galaxyng.h"

int CMD_battletest( int argc, char **argv ) {
    game *aGame;

    aGame = bat_createGame(  );
    if ( ( aGame ) ) {
        player *aPlayer;

        /* srand((int) time(NULL)); */
        /* printf("%d", atoi(argv[2])); */
        resetErnie( atoi( argv[2] ) );
        switch ( atoi( argv[3] ) ) {
        case 1:
            bat_scenario_1( aGame );
            break;
        case 2:
            bat_scenario_2( aGame );
            break;
        case 3:
            bat_scenario_3( aGame );
            break;
        case 4:
            bat_scenario_4( aGame );
            break;
        case 5:
            bat_scenario_5( aGame );
            break;
        case 6:
            bat_scenario_6( aGame );
            break;
        case 7:
            bat_scenario_7( aGame );
            break;
        case 8:
            bat_scenario_8( aGame );
            break;
        case 9:
            bat_scenario_9( aGame );
            break;
        case 10:
            bat_scenario_10( aGame );
            break;
        case 11:
            bat_scenario_11( aGame );
            break;
        case 12:
            bat_scenario_12( aGame );
            break;
        case 13:
            bat_scenario_13( aGame );
            break;
        case 14:
            bat_scenario_14( aGame );
            break;
        }
        preComputeGroupData( aGame );
        fightphase( aGame, GF_INBATTLE2 );
        joinphase( aGame );

        for ( aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next ) {
            aPlayer->pswdstate = 1;
            printf( "%s %d  ", aPlayer->name,
                    numberOfElements( aPlayer->groups ) );
            /* saveTurnReport(aGame, aPlayer, 0); */
        }

        printf( "\n" );
    }
	
    return 0;
}

