#include "galaxyng.h"

/****f* Process/c_order
 * NAME
 *   c_order -- change player's Race name
 * SOURCE
 */


void
c_order( game *aGame, player *P, strlist **s )
{
    char *ns;                   /* new name */
    char *c;                    /* loop variable */
    int i;                      /* loop variable */

    DBUG_ENTER( "c_order" );

    /* find name, chop off any non a-n chars */
    ns = getstr( 0 );
    i = strlen( ns );
    while ( i && !isalnum( ns[i - 1] ) )
        i--;
    ns[i] = 0;

    /* was a name given? */
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "No new race name given." );
        DBUG_VOID_RETURN;
    }

    if ( noCaseStrcmp( ns, "GM" ) == 0 ) {
        mistake( P, ERROR, *s, "GM is a reserved name" );
        DBUG_VOID_RETURN;
    }

    if ( noCaseStrcmp( aGame->name, ns ) == 0 ) {
        mistake( P, ERROR, *s, "You cannot use game name for a Race name" );
        DBUG_VOID_RETURN;
    }

    /* someone else already using it? */
    if ( findElement( player, aGame->players, ns ) != NULL )
    {
        mistake( P, ERROR, *s, "Race name already in use." );
        DBUG_VOID_RETURN;
    }

    /* is it too long? */
    if ( strlen( ns ) > NAMESIZE ) {
        mistake( P, ERROR, *s, "Name is too long\n." );
        DBUG_VOID_RETURN;
    }

    /* set the new name, convert non a-n chars to '_' */
    setName( P, ns );

    for ( c = P->name; *c; c++ ) {
        if ( !isalnum( *c ) )
            *c = '_';
    }

    DBUG_VOID_RETURN;
}


/******/
