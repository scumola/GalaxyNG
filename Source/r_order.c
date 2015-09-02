#include "galaxyng.h"

void
r_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    planet *p;
    planet *p2;
    int i;

    DBUG_ENTER( "r_order" );

    ns = getstr( 0 );
    p = findPlanet( aGame, ns );
    if ( !p ) {
        mistake( P, ERROR, *s, "Source planet \"%s\" not recognized.", ns );
        DBUG_VOID_RETURN;
    }
    if ( p->owner != P ) {
        mistake( P, ERROR, *s, "You do not own planet \"%s\".", p->name );
        DBUG_VOID_RETURN;
    }
    ns = getstr( 0 );
    i = nametocargotype( ns );
    if ( i < 0 ) {
        mistake( P, ERROR, *s, "Cargo type \"%s\" not recognized.", ns );
        DBUG_VOID_RETURN;
    }
    p2 = 0;
    ns = getstr( 0 );
    if ( ns[0] ) {
        p2 = findPlanet( aGame, ns );
        if ( !p2 ) {
            mistake( P, ERROR, *s,
                     "Destination planet \"%s\" not recognized.", ns );
            DBUG_VOID_RETURN;
        }
    }
    p->routes[i] = p2;

    DBUG_VOID_RETURN;
}
