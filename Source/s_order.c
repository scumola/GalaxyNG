#include "galaxyng.h"

void
s_order( game *aGame, player *P, strlist **s )
{
    group *g;
    group *g2;
    planet *p;
    int i;
    int j;
    fleetname *fl;
    char *ns;

    DBUG_ENTER( "s_order" );

    ns = getstr( 0 );
    plog( LFULL, "ns: %s\n", ns );

    g = findgroup( P, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    plog( LFULL, "g: %s   fl: %s\n", g ? g->name : "NULL", fl );
    if ( !g && !fl ) {
        mistake( P, ERROR, *s, "Group or fleet not recognized." );
        DBUG_VOID_RETURN;
    }

    if ( fl ) {
        p = findPlanet( aGame, getstr( 0 ) );
        if ( !p ) {
            mistake( P, ERROR, *s, "Planet not recognized." );
            DBUG_VOID_RETURN;
        }

        for ( g = P->groups; g; g = g->next ) {
            if ( ( g->thefleet == fl ) && ( g->dist > 0.0 ) ) {
                mistake( P, ERROR, *s, "Fleet is in hyperspace." );
                DBUG_VOID_RETURN;
            }

            if ( ( g->thefleet == fl ) && ( g->type->drive > 0.0 ) ) {
                mistake( P, ERROR, *s, "Fleet cannot move." );
                DBUG_VOID_RETURN;
            }
        }

        for ( g = P->groups; g; g = g->next )
            if ( g->thefleet == fl )
                send( aGame, g, p );
        DBUG_VOID_RETURN;
    }

    if ( epsilon( g->type->drive, 0.0, 0.0001 ) ) {
        mistake( P, ERROR, *s, "Ships in this group can't move." );
        DBUG_VOID_RETURN;
    }

    if ( g->dist > 0.0 ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        DBUG_VOID_RETURN;
    }

    p = findPlanet( aGame, getstr( 0 ) );
    if ( !p ) {
        mistake( P, ERROR, *s, "Planet not recognized." );
        DBUG_VOID_RETURN;
    }

    i = g->ships;
    if ( ( j = atoi( getstr( 0 ) ) ) != 0 ) {
        i = j;
        if ( i > g->ships ) {
            mistake( P, ERROR, *s, "Not enough ships, all available used." );
            i = g->ships;
        }

        if ( i <= 0 ) {         /* KDW July 1999 */
            mistake( P, ERROR, *s,
                     "You must specify more than 0 ships, all available used." );
            i = g->ships;
        }

        if ( i != g->ships ) {
            g2 = allocStruct( group );

            *g2 = *g;
            g2->next = NULL;
            numberGroup( P, g2 );
            g2->name = ( char * ) malloc( 8 );
            sprintf( g2->name, "%d", g2->number );
            addList( &P->groups, g2 );
            g->ships -= i;
            g2->ships = i;
            g = g2;
        }
    }

    g->thefleet = 0;
    send( aGame, g, p );
    DBUG_VOID_RETURN;
}
