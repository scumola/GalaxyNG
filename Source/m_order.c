#include "galaxyng.h"

void
m_order( game *aGame, player *P, strlist **s )
{
    double x;
    double y;
    double z;

    DBUG_ENTER( "m_order" );

    x = atof( getstr( 0 ) );
    y = atof( getstr( 0 ) );
    z = atof( getstr( 0 ) );
    if ( z < 1 ) {
        mistake( P, ERROR, *s, "Size must be at least 1." );
        DBUG_VOID_RETURN;
    }
    P->mx = x;
    P->my = y;
    P->msize = z;
    DBUG_VOID_RETURN;
}
