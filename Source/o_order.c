#include "galaxyng.h"

void
o_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    int state;
    option *curOption;

    DBUG_ENTER( "o_order" );

    ns = getstr( 0 );
    state = TRUE;
    if ( !noCaseStrcmp( ns, "no" ) || !noCaseStrcmp( ns, "off" ) ) {
        state = FALSE;
        ns = getstr( 0 );
    }
    for ( curOption = options; curOption->optionName; curOption++ ) {
        if ( !noCaseStrcmp( ns, "xmlReport" ) )
            continue;
        if ( !noCaseStrcmp( ns, curOption->optionName ) ) {
            if ( state )
                P->flags |= curOption->optionMask;
            else
                P->flags &= ~( curOption->optionMask );
            break;
        }
    }
    if ( !curOption->optionName )
        mistake( P, ERROR, *s,
                 "Option not recognized or no longer supported." );

    DBUG_VOID_RETURN;

}
