#include "galaxyng.h"

void
q_order( game *aGame, player *P, strlist **s )
{

    DBUG_ENTER( "q_order" );

    if ( findElement( player, aGame->players, getstr( 0 ) ) != P )
    {
        mistake( P, ERROR, *s, "Race identification not given." );
        DBUG_VOID_RETURN;
    }
    P->flags |= F_DEAD;
    DBUG_VOID_RETURN;
}
