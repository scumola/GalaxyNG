#include "galaxyng.h"

/****f* Process/v_order
 * NAME
 *   v_order -- claim a planet.
 * SOURCE
 */

void
v_order( game *aGame, player *P, strlist **s )
{
    planet *p;
    planet_claim *pclaim;
    /*303-452-3685 */
    DBUG_ENTER( "v_order" );

    p = findPlanet( aGame, getstr( 0 ) );
    if ( !p ) {
        mistake( P, ERROR, *s, "Planet not recognized." );
    } else {
        plog( LFULL, "Race %s claims planet %s\n", P->name, p->name );
        pclaim = allocStruct( planet_claim );

        pclaim->planet_claimed = p;
        pclaim->next = NULL;
        addList( &( P->claimed_planets ), pclaim );
    }
    DBUG_VOID_RETURN;
}

/***** END v_order *****/
