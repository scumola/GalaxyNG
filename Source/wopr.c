/****h* Galaxy/WOPR
*  NAME
*    wopr --
*  SYNOPSIS
*    wopr 
*  FUNCTION
*  AUTHOR
*    Created by:  
*    o Frans Slothouber 
*    o Steve Webb
*  CREATION DATE
*    Feb-2001
*  COPYRIGHT
*    GPL
*  NOTES
*  BUGS
*
**********
*/

#include "galaxy.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "util.h"
#include "process.h"
#include "create.h"
#include "wopr.h"

char vcid[128];

#ifndef ROBOTESTER
int
main( int argc, char **argv )
{
    /* Some initializations */
    resetErnie( 197162622 );

    strcpy( vcid, "WOPR release-0-1, February 2001." );
    if ( argc == 2 ) {
        printf( "%s\n", argv[1] );
        return ( WOPR_battle( argc, argv ) ) ? EXIT_SUCCESS : EXIT_FAILURE;
    } else {
        WOPR_usage(  );
        return EXIT_FAILURE;
    }
}
#endif


void
WOPR_usage( void )
{
    printf( "%s\n", vcid );
    /* Usage message */
}



int
WOPR_battle( int argc, char **argv )
{
    game *aGame;
    FILE *scenario;

    aGame = WOPR_createGame(  );
    if ( !aGame ) {
        fprintf( stderr, "Can't allocate a game structure\n" );
        return FALSE;
    }
    scenario = fopen( argv[1], "r" );
    if ( !scenario ) {
        fprintf( stderr, "Can't open the scenario file %s\n", argv[1] );
        return FALSE;
    }

    if ( WOPR_parse_scenario( scenario, aGame ) ) {
        player *aPlayer;

        /* All data is read, precompute some date */
        raceStatus( aGame );
        preComputeGroupData( aGame );

        /* and do battle */
        fightphase( aGame, GF_INBATTLE2 );

        /* remove empty groups */
        joinphase( aGame );

        /* Dump some of the result data */
        for ( aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next ) {
            aPlayer->pswdstate = 1;
            printf( "%s %d  \n", aPlayer->name,
                    numberOfElements( aPlayer->groups ) );

            /* Only works if you have the correct directories setup in $HOME/Games */
            /* galaxynghome = strdup("/home/chris"); */
            saveTurnReport( aGame, aPlayer, 0 );
        }

        fclose( scenario );
    } else {
        fprintf( stderr, "Parse error in file %s\n", argv[1] );
    }
    return TRUE;
}


/* FUNCTION
 *   Parse the scenario file.
 *   Add all information found in the file to the game structure.
 */

int
WOPR_parse_scenario( FILE *scenario, game *aGame )
{
    char *read;
    int reading;
    player *aPlayer = NULL;

    for ( read = getLine( scenario );
          read != NULL; read = getLine( scenario ) ) {
        char *key;


        key = getstr( lineBuffer );
        if ( noCaseStrcmp( key, "PLAYER" ) == 0 ) {
            char *name;
            name = getstr( NULL );
            if ( name[0] != '\0' ) {
                aPlayer = WOPR_addPlayer( aGame, name );
                reading = READING_NONE;
            } else {
                fprintf( stderr, "No name specified\n" );
                return FALSE;
            }
        } else if ( noCaseStrcmp( key, "GROUPS" ) == 0 ) {
            reading = READING_GROUPS;
        } else if ( noCaseStrcmp( key, "TYPES" ) == 0 ) {
            reading = READING_TYPES;
        } else if ( key[0] == '\0' ) {
            /* It was a remark, skip it */
        } else {
            int result;
            switch ( reading ) {
            case READING_GROUPS:
                assert( aPlayer );
                result = parse_group( aGame, aPlayer );
                break;
            case READING_TYPES:
                assert( aPlayer );
                result = WOPR_parse_type( aGame, aPlayer );
                break;
            }
            if ( !result ) {
                return FALSE;
            }
        }
    }
    return TRUE;
}


/* FUNCTION
 *   Parse single line containing shiptype information
 */


int
WOPR_parse_type( game *aGame, player *aPlayer )
{
    char *typeName;
    char *par;
    int underDefined;
    shiptype *aShiptype;

    typeName = getstr( lineBuffer );

    if ( !typeName[0] ) {
        fprintf( stderr, "No ship type name given.\n" );
        return FALSE;
    }
    if ( findElement( shiptype, aPlayer->shiptypes, typeName ) != NULL )
    {
        fprintf( stderr, "Ship type name already in use.\n" );
        return FALSE;
    }

    aShiptype = allocStruct( shiptype );
    setName( aShiptype, typeName );

    aShiptype->drive = atof( getstr( 0 ) );
    if ( aShiptype->drive < 1 )
        aShiptype->drive = 0;

    underDefined = FALSE;

    par = getstr( 0 );
    if ( *par ) {
        aShiptype->attacks = atoi( par );
    } else {
        underDefined = TRUE;
        aShiptype->attacks = 0;
    }

    par = getstr( 0 );
    if ( *par ) {
        aShiptype->weapons = atof( par );
    } else {
        underDefined = TRUE;
        aShiptype->weapons = 0.0;
    }

    if ( aShiptype->attacks && aShiptype->weapons < 1 ) {
        fprintf( stderr,
                 "Warning: gun size should be greater or equal to 1.\n" );
        aShiptype->attacks = 0;
    }
    if ( !aShiptype->attacks )
        aShiptype->weapons = 0;

    par = getstr( 0 );
    if ( *par ) {
        aShiptype->shields = atof( par );
    } else {
        underDefined = TRUE;
        aShiptype->shields = 0.0;
    }

    if ( aShiptype->shields && aShiptype->shields < 1 ) {
        aShiptype->shields = 0;
        fprintf( stderr,
                 "Warning: shield size should be zero, or greater or equal to 1.\n" );
    }
    par = getstr( 0 );
    if ( *par ) {
        aShiptype->cargo = atof( par );
    } else {
        underDefined = TRUE;
        aShiptype->cargo = 0.0;
    }

    if ( aShiptype->cargo && aShiptype->cargo < 1 ) {
        aShiptype->cargo = 0;
        fprintf( stderr,
                 "Warning: cargo size should be zero, or greater or equal to 1.\n" );
    }
    if ( !aShiptype->drive && !aShiptype->attacks &&
         !aShiptype->shields && !aShiptype->cargo ) {
        fprintf( stderr, "At least one component must be non-zero.\n" );
        return FALSE;
    }
    if ( underDefined ) {
        fprintf( stderr, "Warning: A ship design requires 5 parameters.\n" );
    }
    addList( &aPlayer->shiptypes, aShiptype );
    return TRUE;
}



/* FUNCTION
 *   Parse a single line containing group information.
 */

int
parse_group( game *aGame, player *aPlayer )
{
    group *aGroup;
    char *typeName;
    char *par;
    shiptype *aShiptype;

    aGroup = allocStruct( group );

    par = getstr( lineBuffer );
    if ( *par ) {
        aGroup->ships = atoi( par );
    } else {
        fprintf( stderr, "Number of ships not specified\n" );
    }

    typeName = getstr( NULL );

    if ( !typeName[0] ) {
        fprintf( stderr, "No ship type name given.\n" );
        return FALSE;
    }
    if ( ( aShiptype = findElement( shiptype, aPlayer->shiptypes, typeName ) )
         == NULL )
    {
        fprintf( stderr, "Ship type name not found\n" );
        return FALSE;
    }
    aGroup->type = aShiptype;

    par = getstr( 0 );
    if ( *par ) {
        aGroup->drive = atof( par );
    } else {
        fprintf( stderr, "No drive tech specified\n" );
    }

    par = getstr( 0 );
    if ( *par ) {
        aGroup->weapons = atof( par );
    } else {
        fprintf( stderr, "No drive tech specified\n" );
    }

    par = getstr( 0 );
    if ( *par ) {
        aGroup->shields = atof( par );
    } else {
        fprintf( stderr, "No shield tech specified\n" );
    }

    par = getstr( 0 );
    if ( *par ) {
        aGroup->cargo = atof( par );
    } else {
        fprintf( stderr, "No shield tech specified\n" );
    }

    cktech( aGroup );
    aGroup->loadtype = CG_EMPTY;
    aGroup->from = aGroup->where = findPlanet( aGame, "BattleGround" );
    aGroup->dist = 0;
    aGroup->thefleet = NULL;
    aGroup->defense = 0.0;
    aGroup->attack = 0.0;
    aGroup->flags = 0;
    aGroup->location = findPlanet( aGame, "BattleGround" );
    numberGroup( aPlayer, aGroup );
    aGroup->name = ( char * ) malloc( 8 );
    sprintf( aGroup->name, "%d", aGroup->number );
    addList( &aPlayer->groups, aGroup );
    return TRUE;
}





game *
WOPR_createGame(  )
{
    game *aGame;
    planet *aPlanet;

    aGame = allocStruct( game );
    if ( aGame != NULL ) {
        setName( aGame, "wopr" );
        aGame->turn = 0;
        aGame->galaxysize = 30.0;
        aPlanet = addplanet( aGame );
        setName( aPlanet, "BattleGround" );
    } else {
        return NULL;
    }
    return aGame;
}




void
wopr_allies( game *aGame, char *playerName1, char *playerName2 )
{
    player *aPlayer1;
    player *aPlayer2;
    alliance *a;

    aPlayer1 = findElement( player, aGame->players, playerName1 );
    aPlayer2 = findElement( player, aGame->players, playerName2 );

    a = allocStruct( alliance );
    a->who = aPlayer1;
    addList( &aPlayer2->allies, a );

    a = allocStruct( alliance );
    a->who = aPlayer2;
    addList( &aPlayer1->allies, a );
}



/* FUNCTION
 *   Create and initialize a player and add it to
 *   a game.
 */

player *
WOPR_addPlayer( game *aGame, char *name )
{
    player *aPlayer;

    aPlayer = allocStruct( player );

    setName( aPlayer, name );
    aPlayer->pswd = strdup( "none" );
    aPlayer->addr = strdup( "none" );
    aPlayer->realName = strdup( "none" );
    aPlayer->pswdstate = 1;
    aPlayer->drive = 1;
    aPlayer->weapons = 1;
    aPlayer->shields = 1;
    aPlayer->cargo = 1;
    aPlayer->msize = aGame->galaxysize;
    aPlayer->flags =
        F_ANONYMOUS | F_AUTOUNLOAD | F_PRODTABLE | F_SORTGROUPS |
        F_GROUPFORECAST | F_PLANETFORECAST | F_SHIPTYPEFORECAST |
        F_ROUTESFORECAST | F_BATTLEPROTOCOL;
    /* Should check if the player exists already... */
    addList( &( aGame->players ), aPlayer );
    return aPlayer;
}

