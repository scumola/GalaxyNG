#include "galaxyng.h"

void
z_order( game *aGame, player *P, strlist **s )
{
    char *ns;

    DBUG_ENTER( "z_order" );

    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "No new address given." );
        DBUG_VOID_RETURN;
    }
    if ( P->addr )
        free( P->addr );
    P->addr = strdup( ns );
    DBUG_VOID_RETURN;
}
