#include "galaxyng.h"

void
y_order( game *aGame, player *P, strlist **s )
{
    char *ns;

    DBUG_ENTER( "y_order" );

    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "New password not provided." );
        DBUG_VOID_RETURN;
    }
    if ( P->pswd )
        free( P->pswd );
    P->pswd = strdup( ns );
    P->pswdstate = 1;
    DBUG_VOID_RETURN;
}
