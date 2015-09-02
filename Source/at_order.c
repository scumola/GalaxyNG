#include "galaxyng.h"

/****f* Process/at_order
 * NAME
 *   at_order  (@ is the single character used for this)
 *
 * Send a message to one or more players within their turn reports. All
 * lines between the order and the next line with an @ symbol as its
 * first non-blank character are treated as message text. Semicolons
 * between the @ symbols are treated as part of the message, and not as
 * comments. The server does not indicate the sender, so unsigned
 * messages are anonymous and messages can be forged. Note that races
 * usually send each other email if they want to verify the
 * authenticity of their messages. The alien race name parameter should
 * be a list of race names separated by spaces. If no race name is
 * provided, or if a race name is provided with no space between it and
 * the @ symbol, the message will be sent to all races.
 *
 * SOURCE
 */

void
at_order( game *aGame, player *p, strlist **s )
{
    char *ns;                   /* name string */
    alliance *a;                /* traversing the alliance list */
    alliance *plist;            /* list of people in the alliance */
    player *p2;                 /* for player searching */

    DBUG_ENTER( "at_order" );

    ns = getstr( NULL );        /* for whom is the message */

    if ( ns[0] ) {
        /* find named player */
        for ( plist = NULL; ns[0]; ns = getstr( 0 ) ) {
            if ( ( p2 = findElement( player, aGame->players, ns ) ) )
            {
                a = allocStruct( alliance );

                a->who = p2;
                addList( &plist, a );
            } else {
                mistake( p, INFO, *s, "Race not recognized" );
            }
        }

        /* create a list of players to send message to */
        for ( a = plist; a; a = a->next ) {
            addList( &a->who->messages, makestrlist( "-message starts-" ) );

            /* add the message to each player */
            for ( *s = ( *s )->next; ( *s ) && ( ( *s )->str[0] != '@' );
                  *s = ( *s )->next ) {
                for ( a = plist; a; a = a->next )
                    addList( &a->who->messages, makestrlist( ( *s )->str ) );
            }

            /* end the message */
            for ( a = plist; a; a = a->next )
                addList( &a->who->messages, makestrlist( "-message ends-" ) );

            freelist( plist );
        }
    } else {                    /* Message is global */
        addList( &( aGame->messages ), makestrlist( "-message starts-" ) );

        for ( *s = ( *s )->next; ( *s ) && ( ( *s )->str[0] != '@' );
              *s = ( *s )->next ) {
            addList( &( aGame->messages ), makestrlist( ( *s )->str ) );
        }
        addList( &( aGame->messages ), makestrlist( "-message ends-" ) );
    }

    DBUG_VOID_RETURN;
}

/********/
