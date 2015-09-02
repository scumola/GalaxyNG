#include "galaxyng.h"

/****f* Process/g_order
 * NAME
 *   g_order -- 
 * SOURCE
 */

void
g_order( game *aGame, player *P, strlist **s )
{
    planet *p;
    group *g;
    group *g2;
    shiptype *t;
    double x;
    double y;
    double z;
    char *ns;
    int i;

    DBUG_ENTER( "g_order" );

    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, ERROR, *s, "Group not recognized." );
        DBUG_VOID_RETURN;
    }

    if ( epsilon( g->dist, 0.0, 0.0001 ) ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        DBUG_VOID_RETURN;
    }

    p = g->where;
    t = g->type;
    plog( LFULL, "Upgrade: of %s on %s\n", t->name, p->name );
    if ( p->owner != P ) {
        mistake( P, ERROR, *s, "Planet is not owned by you." );
        DBUG_VOID_RETURN;
    }

    x = INDPERSHIP * ( ( 1 - g->drive / P->drive ) * t->drive +
                       ( 1 - g->weapons / P->weapons ) * weaponmass( t ) +
                       ( 1 - g->shields / P->shields ) * t->shields +
                       ( 1 - g->cargo / P->cargo ) * t->cargo );

    y = p->ind * .75 + p->pop * .25 - p->spent;
    if ( ( y <= 0.0 ) && ( x > 0.0 ) ) {
        mistake( P, ERROR, *s, "Planet has no remaining industry." );
        DBUG_VOID_RETURN;
    }

    if ( epsilon( x, 0.0, 0.0001 ) ) {
        mistake( P, WARNING, *s, "Group is already at current tech levels." );
        DBUG_VOID_RETURN;
    }

    ns = getstr( 0 );
    if ( ns[0] ) {
        i = atoi( ns );
        if ( i == 0 )
            i = g->ships;

        if ( i > g->ships ) {
            mistake( P, ERROR, *s, "Not enough ships, all available used." );
            i = g->ships;
        }

        if ( i < 0 ) {          /* KDW July 1999 */
            mistake( P, ERROR, *s,
                     "Can't use negative number of ships, all available used." );
            i = g->ships;
        }
    } else
        i = g->ships;

    if ( i != g->ships ) {
        g2 = allocStruct( group );

        *g2 = *g;
        numberGroup( P, g2 );
        g2->next = NULL;
        g2->name = ( char * ) malloc( 8 );
        sprintf( g2->name, "%d", g2->number );
        addList( &P->groups, g2 );
        g->ships -= i;
        g2->ships = i;
        g = g2;
    }
    plog( LFULL, "Old Levels:\n" );
    plog( LFULL, "%f %f %f %f\n", g->drive, g->weapons, g->shields,
          g->cargo );
    z = y / i;
    if ( z >= x ) {
        plog( LFULL, "Full Upgrade\n" );
        memcpy( &g->drive, &P->drive, 4 * sizeof( double ) );
    } else {
        plog( LFULL, "Partial Upgrade\n" );
        z /= x;
        x *= z;
        g->drive = g->drive + ( P->drive - g->drive ) * z;
        g->weapons = g->weapons + ( P->weapons - g->weapons ) * z;
        g->shields = g->shields + ( P->shields - g->shields ) * z;
        g->cargo = g->cargo + ( P->cargo - g->cargo ) * z;
    }
    cktech( g );
    plog( LFULL, "New Levels:\n" );
    plog( LFULL, "%f %f %f %f\n", g->drive, g->weapons, g->shields,
          g->cargo );
    p->spent += x * i;

    DBUG_VOID_RETURN;
}
