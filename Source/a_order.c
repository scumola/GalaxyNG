#include "galaxyng.h"

/****f* Process/a_order
 * NAME
 *   a_order -- add a player to an alliance
 * SOURCE
 */

void
a_order( game *aGame, player *P, strlist **s )
{
    player *P2;                 /* player to find */
    alliance *a;                /* existing alliance */

    DBUG_ENTER( "a_order" );

    /* find the named player */
    P2 = findElement( player, aGame->players, getstr( 0 ) );

    if ( !P2 ) {
        mistake( P, ERROR, *s, "Race not recognized" );
        DBUG_VOID_RETURN;
    }

    /* is the player already part of the alliance? */
    for ( a = P->allies; a; a = a->next ) {
        if ( a->who == P2 ) {
            DBUG_VOID_RETURN;
        }
    }

    /* if no alliance, add the player */
    if ( !a && P2 != P ) {
        a = allocStruct( alliance );

        a->who = P2;
        addList( &P->allies, a );
    }

    DBUG_VOID_RETURN;
}

/******/
