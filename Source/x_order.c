#include "galaxyng.h"

void
x_order( game *aGame, player *P, strlist **s )
{
    group *g;
    group *g2;
    int i;
    int j;

    DBUG_ENTER( "x_order" );

    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, ERROR, *s, "Group not recognized." );
        DBUG_VOID_RETURN;
    }
    if ( g->dist > 0.0 ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        DBUG_VOID_RETURN;
    }
    if ( g->loadtype == CG_COL && g->where->owner && g->where->owner != P ) {
        mistake( P, ERROR, *s,
                 "Can't unload colonists onto an alien planet." );
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
    unloadgroup( g, P, g->load );
    g->where->mat += shipmass( g ) * g->ships;
    g->ships = 0;
    DBUG_VOID_RETURN;
}
