#include "galaxyng.h"

/****f* Process/w_order
 * NAME
 *   w_order -- declare war on another race.
 * SOURCE
 */

void
w_order( game *aGame, player *P, strlist **s )
{
    player *P2;
    alliance *a;

    DBUG_ENTER( "w_order" );

    if ( aGame->turn < aGame->gameOptions.galactic_peace ) {
        mistake( P, ERROR, *s, "Galactic Peace enforced until turn %d.",
                 aGame->gameOptions.galactic_peace );
        DBUG_VOID_RETURN;
    }

    P2 = findElement( player, aGame->players, getstr( 0 ) );

    if ( !P2 ) {
        mistake( P, ERROR, *s, "Race not recognized." );
        DBUG_VOID_RETURN;
    }

    for ( a = P->allies; a; a = a->next ) {
        if ( a->who == P2 ) {
            remList( &P->allies, a );
            DBUG_VOID_RETURN;
        }
    }
    DBUG_VOID_RETURN;
}

/***** END w_order *****/
