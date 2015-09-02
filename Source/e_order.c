#include "galaxyng.h"

/****f* Process/e_order
 * NAME
 *   e_order -- eliminate a ship or fleet
 * SOURCE
 */


void
e_order( game *aGame, player *P, strlist **s )
{
    shiptype *t;                /* type to be eliminated */
    fleetname *fl;              /* fleet to be eliminated */
    group *g;                   /* group containing the ship */
    planet *p;                  /* planet ship/fleet is orbiting */
    char *ns;                   /* retrieving parameters */

    DBUG_ENTER( "e_order" );

    /* see if this is a fleet or ship type */
    ns = getstr( 0 );
    t = findElement( shiptype, P->shiptypes, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    /* neither? must be a typo */
    if ( !t && !fl ) {
        mistake( P, ERROR, *s, "Ship or fleet type not recognized." );
        DBUG_VOID_RETURN;
    }

    /* if it's a fleet . . . */
    if ( fl ) {
        /* can't be in hyperspace */
        for ( g = P->groups; g; g = g->next ) {
            if ( ( g->thefleet == fl )
                 && ( epsilon( g->dist, 0.0, 0.0001 ) ) ) {
                mistake( P, ERROR, *s, "Group is in hyperspace." );
                DBUG_VOID_RETURN;
            }
        }

        /* find it, delete it */
        for ( g = P->groups; g; g = g->next ) {
            if ( g->thefleet == fl )
                g->thefleet = 0;
        }

        remList( &P->fleetnames, fl );
    } else {
        /* it must be a group */

        /* can't delete ship type if ships of that type still exist */
        for ( g = P->groups; g; g = g->next ) {
            if ( g->type == t ) {
                mistake( P, ERROR, *s, "Some of these ships still exist." );
                DBUG_VOID_RETURN;
            }
        }

        if ( g )
            DBUG_VOID_RETURN;

        /* can't delete ship type if you're currently building them */
        for ( p = aGame->planets; p; p = p->next ) {
            if ( p->producingshiptype == t ) {
                mistake( P, ERROR, *s,
                         "Some of these ships are still being produced." );
                DBUG_VOID_RETURN;
            }
        }

        if ( p )
            DBUG_VOID_RETURN;

        /* ok, it's really orphaned, delete it */
        remList( &P->shiptypes, t );
    }

    DBUG_VOID_RETURN;
}

/******/
