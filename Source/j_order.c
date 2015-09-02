#include "galaxyng.h"

void
j_order( game *aGame, player *P, strlist **s )
{
    planet *p;
    planet *p2;
    char *ns;
    group *g;
    group *g2;
    double dist;
    double dist2;
    fleetname *fl;
    fleetname *fl2;
    int i;
    int j;

    DBUG_ENTER( "j_order" );

    ns = getstr( 0 );
    g = findgroup( P, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    if ( !g && !fl ) {
        mistake( P, ERROR, *s, "Source Group or fleet not recognized." );
        DBUG_VOID_RETURN;
    }

    if ( fl ) {
        fl2 = findElement( fleetname, P->fleetnames, getstr( 0 ) );

        if ( !fl2 ) {
            mistake( P, ERROR, *s, "Destination fleet not recognized." );
            DBUG_VOID_RETURN;
        }

        dist = 0.0;
        dist2 = 0.0;
        p = ( planet * ) NULL;
        p2 = ( planet * ) NULL;

        for ( g = P->groups; g; g = g->next ) {
            if ( g->thefleet == fl ) {
                p = g->where;
                dist = g->dist;
            }

            if ( g->thefleet == fl2 ) {
                p2 = g->where;
                dist2 = g->dist;
            }
        }

        if ( epsilon( dist, 0.0, 0.0001 ) ) {
            mistake( P, ERROR, *s, "First fleet is in hyperspace." );
            DBUG_VOID_RETURN;
        }

        if ( epsilon( dist2, 0.0, 0.0001 ) ) {
            mistake( P, ERROR, *s, "Second fleet is in hyperspace." );
            DBUG_VOID_RETURN;
        }

        if ( ( p != p2 ) && ( p2 ) ) {
            mistake( P, ERROR, *s, "Fleets are not at the same planet." );
            DBUG_VOID_RETURN;
        }

        for ( g = P->groups; g; g = g->next )
            if ( g->thefleet == fl )
                g->thefleet = fl2;
        DBUG_VOID_RETURN;
    }

    if ( epsilon( g->dist, 0.0, 0.0001 ) ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        DBUG_VOID_RETURN;
    }

    fl = findElement( fleetname, P->fleetnames, getstr( 0 ) );

    if ( !fl ) {
        mistake( P, ERROR, *s, "Fleet type not recognized." );
        DBUG_VOID_RETURN;
    }

    if ( ( j = atoi( getstr( 0 ) ) ) != 0 ) {
        i = j;
        if ( i > g->ships ) {
            mistake( P, ERROR, *s, "Not enough ships, all available used." );
            i = g->ships;
        }

        if ( i <= 0 ) {
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

    for ( g2 = P->groups; g2; g2 = g2->next ) {
        if ( ( g2->thefleet == fl ) && epsilon( g2->dist, 0.0, 0.0001 ) ) {
            mistake( P, ERROR, *s, "Fleet is in hyperspace." );
            DBUG_VOID_RETURN;
        }

        if ( ( g2->thefleet == fl ) && ( !epsilon( g2->dist, 0.0, 0.0001 ) )
             && ( g2->where != g->where ) ) {
            mistake( P, ERROR, *s, "Group is at the wrong planet." );
            DBUG_VOID_RETURN;
        }
    }

    g->thefleet = fl;

    DBUG_VOID_RETURN;
}
