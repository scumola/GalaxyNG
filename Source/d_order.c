#include "galaxyng.h"

/****f* Process/d_order
 * NAME
 *   d_order -- design a new ship or create a new fleet
 * SOURCE
 */


void
d_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    char *par;
    fleetname *fl;
    shiptype *t;
    int i;
    int underDefined;

    DBUG_ENTER( "d_order" );

    /* get design name, check for validity */
    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "No ship type name given." );
        DBUG_VOID_RETURN;
    }

    if ( strlen( ns ) > NAMESIZE ) {
        mistake( P, ERROR, *s, "Name is too long\n." );
        DBUG_VOID_RETURN;
    }

    if ( findElement( fleetname, P->fleetnames, ns ) )
    {
        mistake( P, ERROR, *s, "Name already in use for fleet." );
        DBUG_VOID_RETURN;
    }

    /* Occasionally someone designs a ship called "Cargo" or something, then 
     * doesn't understand why the planet didn't produce the ship when 
     * they do "P planet Cargo" :)  This will fix this problem.  - RJS
     */

    for ( i = 0; i < nbrProducts; i++ ) {
        if ( productname[i] && !noCaseStrcmp( productname[i], ns ) ) {
            mistake( P, ERROR, *s,
                     "Ship name can not be the same as a product name." );
            DBUG_VOID_RETURN;
        }
    }

    if ( findElement( shiptype, P->shiptypes, ns ) != NULL )
    {
        mistake( P, WARNING, *s, "Ship type name already in use." );
        DBUG_VOID_RETURN;
    }

    /* look for the fleet keyword, process it if this is new fleet
     * creation
     */
    if ( !noCaseStrncmp( ns, "fleet", 5 ) ) {
        /* get the fleet name, check for validity */
        ns = getstr( 0 );
        if ( !ns[0] ) {
            mistake( P, ERROR, *s, "No fleet name given." );
            DBUG_VOID_RETURN;
        }

        if ( isdigit( ns[0] ) ) {
            mistake( P, ERROR, *s,
                     "Fleet names cannot have an initial digit." );
            DBUG_VOID_RETURN;
        }

        if ( findElement( fleetname, P->fleetnames, ns ) )
        {
            mistake( P, ERROR, *s, "Fleet name already in use." );
            DBUG_VOID_RETURN;
        }

        if ( findElement( shiptype, P->shiptypes, ns ) )
        {
            mistake( P, ERROR, *s, "Name already in use for ship type." );
            DBUG_VOID_RETURN;
        }

        /* valid fleet name, create it */
        fl = allocStruct( fleetname );
        setName( fl, ns );
        fl->fleetspeed = 0.0;
        addList( &P->fleetnames, fl );
    } else {
        /* must be a ship design */
        t = allocStruct( shiptype );
        setName( t, ns );

        /* set drive */
        t->drive = atof( getstr( 0 ) );
        if ( t->drive < 1.0 )
            t->drive = 0.0;

        underDefined = FALSE;

        /* set attacks */
        par = getstr( 0 );
        if ( *par ) {
            t->attacks = atoi( par );
        } else {
            underDefined = TRUE;
            t->attacks = 0;
        }

        /* set weapons */
        par = getstr( 0 );
        if ( *par ) {
            t->weapons = atof( par );
        } else {
            underDefined = TRUE;
            t->weapons = 0.0;
        }

        if ( t->attacks && t->weapons < 1 ) {
            mistake( P, WARNING, *s,
                     "Warning: gun size should be greater or equal to 1." );
            t->attacks = 0;
        }
        if ( !t->attacks )
            t->weapons = 0;

        /* set shields */
        par = getstr( 0 );
        if ( *par ) {
            t->shields = atof( par );
        } else {
            underDefined = TRUE;
            t->shields = 0.0;
        }

        if ( epsilon( t->shields, 0.0, 0.0001 ) && t->shields < 1.0 ) {
            t->shields = 0;
            mistake( P, WARNING, *s,
                     "shield size should be zero, or greater or equal to 1." );
        }

        /* set cargo */
        par = getstr( 0 );
        if ( *par ) {
            t->cargo = atof( par );
        } else {
            underDefined = TRUE;
            t->cargo = 0.0;
        }

        if ( epsilon( t->cargo, 0.0, 0.0001 ) && t->cargo < 1.0 ) {
            t->cargo = 0.0;
            mistake( P, WARNING, *s,
                     "cargo size should be zero, or greater or equal to 1." );
        }

        if ( epsilon( t->cargo, 0.0, 0.0001 ) && epsilon( t->drive, 0.0, 0.0001 ) ) {   /* KDW 20040125 */
            mistake( P, WARNING, *s,
                     "unusual to have cargo ship with no drive." );
        }

        /* ensure the design can do something */
        if ( !epsilon( t->drive, 0.0, 0.0001 ) &&
             !t->attacks &&
             !epsilon( t->shields, 0.0, 0.0001 ) &&
             !epsilon( t->cargo, 0.0, 0.0001 ) ) {
            mistake( P, ERROR, *s,
                     "At least one component must be non-zero." );
            free( t );
            DBUG_VOID_RETURN;
        }

        if ( underDefined ) {
            mistake( P, WARNING, *s, "A ship design requires 5 parameters." );
        }

        addList( &P->shiptypes, t );
    }

    DBUG_VOID_RETURN;
}

/******/
