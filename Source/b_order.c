#include "galaxyng.h"

/****f* Process/b_order
 * NAME
 *   b_order -- break off ships from a group or remove a group from a fleet
 * SOURCE
 */

void
b_order( game *aGame, player *P, strlist **s )
{
    group *g;                   /* pointer to named group */
    group *g2;                  /* pointer to new group */
    int i;                      /* int value for number of ships */
    char *ns;                   /* char value for number of ships */

    DBUG_ENTER( "b_order" );

    /* find the named group */
    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, INFO, *s, "Group not recognized." );
        DBUG_VOID_RETURN;
    }

    /* check to see if we're detaching from a fleet */
    ns = getstr( 0 );
    if ( noCaseStrncmp( ns, "fleet", 5 ) == 0 ) {
        if ( !epsilon( g->dist, 0.0, 0.0001 ) ) {
            mistake( P, WARNING, *s, "Fleet is in hyperspace." );
            DBUG_VOID_RETURN;
        }
        g->thefleet = 0;
        DBUG_VOID_RETURN;
    }

    /* are there enough ships to detach? */
    i = atoi( ns );
    if ( i > g->ships ) {       /* FS Dec 1998 */
        mistake( P, INFO, *s, "Not enough ships in group." );
        DBUG_VOID_RETURN;
    }

    /* this was an odd problem */
    if ( i < 0 ) {              /* KDW July 1999 */
        mistake( P, WARNING, *s, "Can't have negative number of ships." );
        DBUG_VOID_RETURN;
    }

    /* create a new group for what's being broken off and add it to
     * the list of groups the player owns
     */
    g2 = allocStruct( group );
    *g2 = *g;
    g2->ships = i;
    g->ships -= i;
    g2->thefleet = 0;
    g2->next = NULL;
    numberGroup( P, g2 );
    g2->name = ( char * ) malloc( 8 );
    sprintf( g2->name, "%d", g2->number );
    addList( &P->groups, g2 );

    DBUG_VOID_RETURN;
}

/******/
