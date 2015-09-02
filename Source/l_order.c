#include "galaxyng.h"

/****f* Process/l_order
 * NAME
 *   l_order -- load cargo onto a group of ships.
 * FUNCTION
 *
 * SOURCE
 */

void
l_order( game *aGame, player *P, strlist **s )
{
    group *g;
    group *g2;
    double x;
    double amount;
    double y;
    int amountFlag;
    planet *p;
    int numberOfShips;
    int typeOfCargo;
    char *ns;

    amountFlag = FALSE;

    DBUG_ENTER( "l_order" );

    g = findgroup( P, getstr( 0 ) );

    if ( !g ) {
        mistake( P, ERROR, *s, "Group not recognized." );
        DBUG_VOID_RETURN;
    }

    p = g->where;

    typeOfCargo = nametocargotype( getstr( 0 ) );

    if ( epsilon( g->type->cargo, 0.0, 0.0001 ) ) {
        mistake( P, ERROR, *s, "Group cannot carry cargo." );
        DBUG_VOID_RETURN;
    }

    if ( !epsilon( g->dist, 0.0, 0.0001 ) ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        DBUG_VOID_RETURN;
    }

    if ( cargospace( g ) <= g->load ) {
        /* '<=' is used cause we deal with doubles here */
        mistake( P, ERROR, *s, "Group is fully loaded." );
        DBUG_VOID_RETURN;
    }

    if ( typeOfCargo < 0 || typeOfCargo > 2 ) {
        mistake( P, ERROR, *s, "Cargo type not recognized." );
        DBUG_VOID_RETURN;
    }

    if ( !epsilon( g->load, 0.0, 0.0001 ) && g->loadtype != typeOfCargo ) {
        mistake( P, ERROR, *s,
                 "Group is already carrying a different load." );
        DBUG_VOID_RETURN;
    }

    /* Determine the number of ships to load the cargo on */

    numberOfShips = g->ships;   /* Start-off with using all ships */
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

    /* Determine the amount of cargo to be loaded per ship */
    if ( ns[0] != '\0' ) {
        if ( noCaseStrcmp( "amount", ns ) == 0 ) {
            ns = getstr( 0 );
            if ( ns[0] != '\0' ) {
                amount = atof( ns );
                if ( amount > ( cargospace( g ) - g->load ) ) {
                    mistake( P, ERROR, *s,
                             "Not enough cargo space available to carry this amount." );
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
        amountFlag = TRUE;
    } else {
        amount = cargospace( g ) - g->load;
    }

    if ( amount < AMOUNTMIN ) {
        mistake( P, ERROR, *s,
                 "You should load at least %f per ship,"
                 " you are trying to loading %f.", AMOUNTMIN, amount );
        DBUG_VOID_RETURN;
    }

    /* Check if the planet has enough of the goods requested. */

    switch ( typeOfCargo ) {
    case CG_CAP:
        y = p->cap;
        break;
    case CG_MAT:
        y = p->mat;
        break;
    case CG_COL:
        y = p->col;
        break;
    }

    /* Total amount that has to be uploaded */
    x = amount * numberOfShips;
    if ( epsilon( y, 0.0, 0.0001 ) ) {
        mistake( P, ERROR, *s, "No cargo of this type available on \"%s\".",
                 p->name );
        DBUG_VOID_RETURN;
    }

    if ( roundup2( y / numberOfShips ) < AMOUNTMIN ) {
        mistake( P, ERROR, *s, "Not enough cargo available on \"%s\" to"
                 " load at least %f per ship (%f per ship available, %f / %d).",
                 p->name, AMOUNTMIN, roundup2( y / numberOfShips ), y,
                 numberOfShips );
        DBUG_VOID_RETURN;
    }

    if ( y < x ) {
        if ( amountFlag ) {
            mistake( P, ERROR, *s,
                     "Not enough cargo available on \"%s\" to"
                     " load %.2f per ship,\nloading %.2f per ship.",
                     p->name, amount, y / numberOfShips );
        }
        amount = y / numberOfShips;
    }


    /* Everything is OK, break off a group, and load cargo */

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

    switch ( typeOfCargo ) {
    case CG_CAP:
        if ( y > x )
            y = x;
        g->load += y / g->ships;
        p->cap -= y;
        break;

    case CG_MAT:
        if ( y > x )
            y = x;
        g->load += y / g->ships;
        p->mat -= y;
        break;
    case CG_COL:
        if ( y > x )
            y = x;
        g->load += y / g->ships;
        p->col -= y;
        break;
    }
    g->loadtype = typeOfCargo;

    DBUG_VOID_RETURN;
}

/***** End l_order ******/
