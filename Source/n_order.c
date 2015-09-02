#include "galaxyng.h"

void
n_order( game *aGame, player *P, strlist **s )
{
    planet *p;
    char *ns;

    DBUG_ENTER( "n_order" );

    ns = getstr( 0 );
    p = findElement( planet, aGame->planets, ns );

    if ( !p ) {
        mistake( P, ERROR, *s, "Planet not recognized." );
        DBUG_VOID_RETURN;
    }
    if ( p->owner != P ) {
        mistake( P, ERROR, *s, "Planet not owned by you." );
        DBUG_VOID_RETURN;
    }
    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "New planet name not provided." );
        DBUG_VOID_RETURN;
    }
    if ( strlen( ns ) > NAMESIZE ) {
        mistake( P, ERROR, *s, "Name is too long\n." );
        DBUG_VOID_RETURN;
    }
    if ( findPlanet( aGame, ns ) != NULL ) {
        mistake( P, ERROR, *s, "Name already in use." );
        DBUG_VOID_RETURN;
    }
    setName( p, ns );
    ns = getstr( 0 );
    if ( *ns ) {
        mistake( P, ERROR, *s, "Too many parameters." );
        DBUG_VOID_RETURN;
    }

    DBUG_VOID_RETURN;
}
