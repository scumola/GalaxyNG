#include "galaxyng.h"

void
p_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    planet *p;
    shiptype *t;

    DBUG_ENTER( "p_order" );

    p = findPlanet( aGame, getstr( 0 ) );
    if ( !p ) {
        mistake( P, ERROR, *s, "Planet not recognized." );
        DBUG_VOID_RETURN;
    }
    if ( p->owner != P ) {
        mistake( P, ERROR, *s, "Planet not owned by you." );
        DBUG_VOID_RETURN;
    }
    ns = getstr( 0 );
    if ( !noCaseStrcmp( ns, "cap" ) ) {
        setproduction( aGame, p, PR_CAP );
        DBUG_VOID_RETURN;
    }
    if ( !noCaseStrcmp( ns, "mat" ) ) {
        setproduction( aGame, p, PR_MAT );
        DBUG_VOID_RETURN;
    }
    if ( !noCaseStrcmp( ns, "drive" ) ) {
        setproduction( aGame, p, PR_DRIVE );
        DBUG_VOID_RETURN;
    }
    if ( !noCaseStrcmp( ns, "weapons" ) ) {
        setproduction( aGame, p, PR_WEAPONS );
        DBUG_VOID_RETURN;
    }
    if ( !noCaseStrcmp( ns, "shields" ) ) {
        setproduction( aGame, p, PR_SHIELDS );
        DBUG_VOID_RETURN;
    }
    if ( !noCaseStrcmp( ns, "cargo" ) ) {
        setproduction( aGame, p, PR_CARGO );
        DBUG_VOID_RETURN;
    }
    t = findElement( shiptype, P->shiptypes, ns );
    if ( !t ) {
        mistake( P, ERROR, *s, "Production type \"%s\" not recognized.", ns );
        DBUG_VOID_RETURN;
    }
    if ( p->producing != PR_SHIP || p->producingshiptype != t ) {
        setproduction( aGame, p, PR_SHIP );
        p->producingshiptype = t;
    }

    DBUG_VOID_RETURN;
}
