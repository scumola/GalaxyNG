#include "galaxyng.h"

/****f* Process/u_order
 * NAME
 *   u_order -- unload cargo
 * SOURCE
 */

void
u_order( game *aGame, player *P, strlist **s )
{
    group *g;
    group *g2;
    int numberOfShips;
    double amount;
    char *ns;

    DBUG_ENTER( "u_order" );

    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, ERROR, *s, "Group not recognized." );
        DBUG_VOID_RETURN;
    }
    if ( g->loadtype == CG_EMPTY ) {
        mistake( P, ERROR, *s, "No cargo on board." );
        DBUG_VOID_RETURN;
    }
    if ( g->dist > 0.0 ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        DBUG_VOID_RETURN;
    }
    /* Determine how many ships are unloaded */
    numberOfShips = g->ships;   /* Start-of with all ships */
    ns = getstr( 0 );
    if ( ( ns[0] != '\0' ) &&
         ( noCaseStrcmp( "amount", ns ) != 0 ) && ( isdigit( ns[0] ) ) ) {
        numberOfShips = atoi( ns );
        if ( numberOfShips != 0 ) {
            if ( numberOfShips > g->ships ) {
                mistake( P, ERROR, *s,
                         "Not enough ships, all available used." );
                numberOfShips = g->ships;
            }
            if ( numberOfShips <= 0 ) { /* KDW July 1999 */
                mistake( P, ERROR, *s,
                         "You must specify > 0 ships, all available used." );
                numberOfShips = g->ships;
            }
        }
        ns = getstr( 0 );
    }
    /* Determine the amount of cargo to be unloaded per ship */
    if ( ns[0] != '\0' ) {
        if ( noCaseStrcmp( "amount", ns ) == 0 ) {
            ns = getstr( 0 );
            if ( ns[0] != '\0' ) {
                amount = atof( ns );
                if ( amount > g->load ) {
                    amount = g->load;
                    mistake( P, ERROR, *s,
                             "Group does not carry that much cargo, using amount=%.2f",
                             amount );
                }
                if ( amount < AMOUNTMIN ) {
                    mistake( P, ERROR, *s, "Unload atleast %.2f per ship.",
                             AMOUNTMIN );
                    DBUG_VOID_RETURN;
                }
            } else {
                mistake( P, ERROR, *s,
                         "Keyword AMOUNT should be followed by a number." );
                DBUG_VOID_RETURN;
            }
        } else {
            mistake( P, ERROR, *s, "Expected the keyword AMOUNT." );
            DBUG_VOID_RETURN;
        }
    } else {
        amount = g->load;
    }

    if ( numberOfShips != g->ships ) {
        g2 = allocStruct( group );

        *g2 = *g;
        g2->next = NULL;
        numberGroup( P, g2 );
        g2->name = ( char * ) malloc( 8 );
        sprintf( g2->name, "%d", g2->number );
        addList( &P->groups, g2 );
        assert( numberOfShips < g->ships );
        g->ships -= numberOfShips;
        g2->ships = numberOfShips;
        g = g2;
    }
    unloadgroup( g, P, amount );
    DBUG_VOID_RETURN;
}

/***** END u_order ******/
