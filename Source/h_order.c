#include "galaxyng.h"

/*
 * halt order
 *
 * reverse course of a fleet/group in space
 *
 * Groups in hyperspace that are more than four turns from their
 * destination planet may be ordered to reverse course and return to
 * their planet of origin. This is useful for retrieving groups
 * accidentally sent to the wrong planet.
 */

void
h_order( game *aGame, player *P, strlist **s )
{                               /* CB-1990923 */
    group *g;                   /* for finding the group */
    fleetname *fl;              /* or fleet */
    char *ns;                   /* general pointer */

    DBUG_ENTER( "h_order" );

    ns = getstr( 0 );           /* get group number or fleet name */
    g = findgroup( P, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    /* if the fleet or group can't be found, error out */
    if ( !g && !fl ) {
        mistake( P, ERROR, *s, "Group or fleet not recognized." );
        DBUG_VOID_RETURN;
    }

    /* we don't have a list of fleets - the only way to find a fleet is
       to look through the groups for which fleet they belong to.
     */
    /* if it's a fleet, see if it can be turned back */
    if ( fl ) {
        double fleetspeed;

        /* Fixed the "can turn around fleets, late" bug */
        fleetspeed = fleetSpeed( fl, P->groups );

        for ( g = P->groups; g; g = g->next ) {

            if ( g->thefleet == fl )
                if ( !epsilon( g->dist, 0.0, 0.0001 ) ) {
                    mistake( P, ERROR, *s, "Fleet is not in hyperspace." );
                    DBUG_VOID_RETURN;
                }

            if ( g->thefleet == fl )
                if ( g->dist < fleetspeed * TURNS_LEFT ) {
                    mistake( P, ERROR, *s,
                             "Fleet is too near from landing." );
                    DBUG_VOID_RETURN;
                }
        }

        for ( g = P->groups; g; g = g->next )
            if ( g->thefleet == fl )
                recall( aGame, g );
        DBUG_VOID_RETURN;
    }

    if ( !epsilon( g->dist, 0.0, 0.0001 ) ) {
        mistake( P, ERROR, *s, "Group is not in hyperspace." );
        DBUG_VOID_RETURN;
    }

    if ( g->dist < g->type->drive * g->drive *
         DRIVEMAGIC / shipmass( g ) * TURNS_LEFT ) {
        mistake( P, ERROR, *s, "Group is too near from landing." );
        DBUG_VOID_RETURN;
    }
    recall( aGame, g );

    DBUG_VOID_RETURN;
}
