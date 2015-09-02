#include "galaxyng.h"

void
t_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    fleetname *fl;
    shiptype *t;

    DBUG_ENTER( "t_order" );

    ns = getstr( 0 );
    t = findElement( shiptype, P->shiptypes, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    if ( !t && !fl ) {
        mistake( P, ERROR, *s, "Ship or fleet type not recognized." );
        DBUG_VOID_RETURN;
    }
    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "New name not provided" );
        DBUG_VOID_RETURN;
    }
    if ( findElement( fleetname, P->fleetnames, ns ) )
    {
        mistake( P, ERROR, *s, "Name already in use for fleet type." );
        DBUG_VOID_RETURN;
    }
    if ( findElement( shiptype, P->shiptypes, ns ) != NULL )
    {
        mistake( P, ERROR, *s, "Name already in use for ship type." );
        DBUG_VOID_RETURN;
    }
    if ( fl ) {
        setName( fl, ns );
    } else {
        setName( t, ns );
    }
    DBUG_VOID_RETURN;
}
