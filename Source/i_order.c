#include "galaxyng.h"

void
i_order( game *aGame, player *P, strlist **s )
{
    group *inGroup;
    planet *inPlanet;
    fleetname *inFleet;
    char *ns;

    DBUG_ENTER( "i_order" );

    ns = getstr( 0 );

    inGroup = findgroup( P, ns );
    inFleet = findElement( fleetname, P->fleetnames, ns );

    if ( !inGroup && !inFleet ) {
        mistake( P, ERROR, *s, "Group or fleet not recognized." );
        DBUG_VOID_RETURN;
    } else {
        inPlanet = findPlanet( aGame, getstr( 0 ) );
        if ( !inPlanet ) {
            mistake( P, ERROR, *s, "Planet not recognized." );
            DBUG_VOID_RETURN;
        }
    }

    if ( inFleet ) {
        group *aGroup;

        for ( aGroup = P->groups; aGroup; aGroup = aGroup->next ) {
            if ( ( aGroup->thefleet == inFleet ) &&
                 epsilon( aGroup->dist, 0.0, 0.0001 ) ) {
                mistake( P, ERROR, *s, "Fleet is in hyperspace." );
                DBUG_VOID_RETURN;
            }

            if ( ( aGroup->thefleet == inFleet ) &&
                 !epsilon( aGroup->type->drive, 0.0, 0.0001 ) ) {
                mistake( P, ERROR, *s, "Fleet cannot move." );
                DBUG_VOID_RETURN;
            }
        }

        for ( aGroup = P->groups; aGroup; aGroup = aGroup->next ) {
            if ( aGroup->thefleet == inFleet ) {
                send( aGame, aGroup, inPlanet );
                aGroup->flags |= GF_INTERCEPT;
            }
        }
    } else {
        int noShips;

        if ( epsilon( inGroup->type->drive, 0.0, 0.0001 ) ) {
            mistake( P, ERROR, *s, "Ships in this group can't move." );
            DBUG_VOID_RETURN;
        }

        if ( !epsilon( inGroup->dist, 0.0, 0.0001 ) ) {
            mistake( P, ERROR, *s, "Group is in hyperspace." );
            DBUG_VOID_RETURN;
        }

        ns = getstr( 0 );
        if ( *ns ) {
            noShips = atoi( ns );
            if ( noShips <= 0 ) {       /* KDW July 1999 - added return */
                mistake( P, WARNING, *s,
                         "You must specify more than 0 Ships." );
                DBUG_VOID_RETURN;
            }
        } else {
            noShips = inGroup->ships;
        }

        if ( noShips ) {
            group *newGroup;

            if ( noShips > inGroup->ships ) {
                mistake( P, WARNING, *s,
                         "Not enough ships, all available used." );
                noShips = inGroup->ships;
            }

            if ( noShips != inGroup->ships ) {
                newGroup = allocStruct( group );

                *newGroup = *inGroup;
                numberGroup( P, newGroup );
                newGroup->name = ( char * ) malloc( 8 );
                sprintf( newGroup->name, "%d", newGroup->number );
                newGroup->next = NULL;
                addList( &P->groups, newGroup );
                inGroup->ships -= noShips;
                newGroup->ships = noShips;
                send( aGame, newGroup, inPlanet );
                newGroup->flags |= GF_INTERCEPT;
                newGroup->thefleet = NULL;
            } else {
                send( aGame, inGroup, inPlanet );
                inGroup->flags |= GF_INTERCEPT;
                inGroup->thefleet = NULL;
            }
        } else {
            mistake( P, ERROR, *s, "No ships left in group." );
        }
    }
    DBUG_VOID_RETURN;
}
