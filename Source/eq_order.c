#include "galaxyng.h"

/****f* Process/eq_order
 * NAME
 *  eq_order  (= is the char used in orders)
 * Set the player's real name for use in the Hall of Fame.
 *
 * SOURCE
 */

void
eq_order( game *aGame, player *P, strlist **s )
{
    char *ns;                   /* name string */

    DBUG_ENTER( "eq_order" );

    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, INFO, *s, "No name provided." );
        return;
    }

    /* if they already have a name, free it up */
    if ( P->realName )
        free( P->realName );

    /* add the new name */
    P->realName = strdup( ns );

    DBUG_VOID_RETURN;
}

/******/
