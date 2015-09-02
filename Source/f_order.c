#include "galaxyng.h"

/****f* Process/f_order
 * NAME
 *   f_order -- request Race email
 * SOURCE
 */

void
f_order( game *aGame, player *P, strlist **s )
{
    player *P2;                 /* player requested */

    DBUG_ENTER( "f_order" );

    P2 = findElement( player, aGame->players, getstr( 0 ) );

    if ( !P2 ) {
        mistake( P, ERROR, *s, "Race not recognized." );
        DBUG_VOID_RETURN;
    }

    if ( P2->addr[0] ) {
        if ( P2->flags & F_ANONYMOUS ) {
            sprintf( lineBuffer, "%s plays anonymously", P2->name );
        } else {
            sprintf( lineBuffer, "The address of %s is %s", P2->name,
                     P2->addr );
        }
    } else
        sprintf( lineBuffer, "There is currently no address for %s.\n"
                 "See http://galaxyng.sourceforge.net/manual.php#email for "
                 "details on relaying messages", P2->name );

    addList( &P->messages, makestrlist( lineBuffer ) );

    DBUG_VOID_RETURN;
}

/******/
