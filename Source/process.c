#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "process.h"


/****h* GalaxyNG/Process
 * FUNCTION
 *   This module contains functions to process orders
 *   and run a turn.
 ******
 */

extern int nbrProducts;

void cleanDeadPlayers( game *aGame );

char *vprocess = "$Id: process.c,v 1.61 2005/01/18 01:32:25 madcoder42 Exp $";

void forecast_xml( game *aGame, player *aPlayer, FILE *forecast );

/****v* Process/phase1orders
 * NAME
 *   phase1orders -- map of all phase 1 orders.
 * FUNCTION
 *   A map that maps an order name to the corresponding function
 *   that executes the order.  
 * SOURCE
 */

orderinfo phase1orders[] = {
    {"@", &at_order},           /* send message */
    {"=", &eq_order},           /* FS 1999/12 set real name */
    {"a", &a_order},            /* alliance */
    {"b", &b_order},            /* break off ships */
    {"d", &d_order},            /* design ship */
    {"e", &e_order},            /* eliminate ship type */
    {"f", &f_order},            /* get Race's email address */
    {"h", &h_order},            /* CB-19980923, to recall (halt) a group */
    {"i", &i_order},            /* intercept */
    {"j", &j_order},            /* group join fleet */
    {"l", &l_order},            /* load cargo */
    {"m", &m_order},            /* change map area */
    {"o", &o_order},            /* set options */
    {"p", &p_order},            /* set production */
    {"q", &q_order},            /* quit */
    {"r", &r_order},            /* set route */
    {"s", &s_order},            /* send group/fleet to planet */
    {"u", &u_order},            /* unload cargo */
    {"v", &v_order},            /* claim victory */
    {"w", &w_order},            /* cancel alliance */
    {"x", &x_order},            /* scrap group */
    {"y", &y_order},            /* change password */
    {"z", &z_order},            /* change email */
    {NULL, NULL}
};

/**********/

/****v* Process/phase2orders
 * NAME
 *   phase2orders -- map of all phase 2 orders.
 * FUNCTION
 *   A map that maps an order name to the corresponding function
 *   that executes the order.  
 * SOURCE
 */

orderinfo phase2orders[] = {
    {"g", &g_order},            /* upgrade ships */
    {"n", &n_order},            /* rename planet */
    {"t", &t_order},            /* change ship/fleet name */
    {NULL, NULL}
};

/*********/

/****v* Process/phase3orders
 * NAME
 *   phase3orders -- map of all phase 3 orders.
 * FUNCTION
 *   A map that maps an order name to the corresponding function
 *   that executes the order.  
 * SOURCE
 */

orderinfo phase3orders[] = {
    {"c", &c_order},            /* change race name */
    {NULL, NULL}
};

/*********/


/* WIN32 */

/****f* Process/mistake
 * NAME
 *   mistake -- generate an error message about an order.
 * FUNCTION
 *   Dynamically generate an error message and add it to
 *   the list of mistakes of a player.
 * SOURCE
 */

void
mistake( player *P, enum error_type elevel, strlist *s, char *format, ... )
{
    int n;                      /* return value */
    va_list ap;                 /* argument list */
    char *lformat;              /* local copy of format */

    va_start( ap, format );

    lformat = ( char * ) malloc( strlen( format ) + 4 );

    switch ( elevel ) {
    case INFO:
        sprintf( lformat, "+I %s", format );
        break;

    case WARNING:
        sprintf( lformat, "+W %s", format );
        break;

    case ERROR:
        sprintf( lformat, "+E %s", format );
        break;
    }

#ifdef WIN32
    vsprintf( lineBuffer, format, ap );
#else
    n = vsnprintf( lineBuffer, LINE_BUFFER_SIZE, lformat, ap );
    assert( n != -1 );
#endif

    free( lformat );

    va_end( ap );

    insertList( &P->orders, s, makestrlist( lineBuffer ) );
}

/********/


/****f* Process/at_order
 * NAME
 *   at_order -- write message to a race
 * SOURCE
 */

void
at_order( game *aGame, player *P, strlist **s )
{
    char *ns;                   /* name string */
    alliance *a;                /* traversing the alliance list */
    alliance *plist;            /* list of people in the alliance */
    player *P2;                 /* for player searching */

    pdebug( DFULL, "at_order\n" );

    ns = getstr( 0 );           /* for whom is the message */
    if ( ns[0] ) {
        /* find named player */
        for ( plist = NULL; ns[0]; ns = getstr( 0 ) ) {
            if ( ( P2 = findElement( player, aGame->players, ns ) ) )
            {
                a = allocStruct( alliance );

                a->who = P2;
                addList( &plist, a );
            } else
                mistake( P, INFO, *s, "Race not recognized" );
        }

        /* create a list of players to send message to */
        for ( a = plist; a; a = a->next )
            addList( &a->who->messages, makestrlist( "-message starts-" ) );

        /* add the message to each player */
        for ( *s = ( *s )->next; ( *s ) && ( ( *s )->str[0] != '@' );
              *s = ( *s )->next ) {
            for ( a = plist; a; a = a->next )
                addList( &a->who->messages, makestrlist( ( *s )->str ) );
        }

        /* end the message */
        for ( a = plist; a; a = a->next )
            addList( &a->who->messages, makestrlist( "-message ends-" ) );

        freelist( plist );
    } else {                    /* Message is global */
        addList( &( aGame->messages ), makestrlist( "-message starts-" ) );

        for ( *s = ( *s )->next; ( *s ) && ( ( *s )->str[0] != '@' );
              *s = ( *s )->next ) {
            addList( &( aGame->messages ), makestrlist( ( *s )->str ) );
        }
        addList( &( aGame->messages ), makestrlist( "-message ends-" ) );
    }
}

/********/



/****f* Process/eq_order
 * NAME
 *   eq_order -- set real name
 * SOURCE
 */

void
eq_order( game *aGame, player *P, strlist **s )
{
    char *ns;                   /* name string */

    pdebug( DFULL, "eq_order\n" );

    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, INFO, *s, "No name provided." );
        return;
    }

    /* if they already have a name, free it up */
    if ( P->realName )
        free( P->realName );

    /* add the new name */
    P->realName = strdup( ns );
}

/******/


/****f* Process/a_order
 * NAME
 *   a_order -- add a player to an alliance
 * SOURCE
 */

void
a_order( game *aGame, player *P, strlist **s )
{
    player *P2;                 /* player to find */
    alliance *a;                /* existing alliance */

    pdebug( DFULL, "a_order\n" );

    /* find the named player */
    P2 = findElement( player, aGame->players, getstr( 0 ) );

    if ( !P2 ) {
        mistake( P, ERROR, *s, "Race not recognized" );
        return;
    }

    /* is the player already part of the alliance? */
    for ( a = P->allies; a; a = a->next ) {
        if ( a->who == P2 ) {
            return;
        }
    }

    /* if no alliance, add the player */
    if ( !a && P2 != P ) {
        a = allocStruct( alliance );

        a->who = P2;
        addList( &P->allies, a );
    }
}

/******/


/****f* Process/b_order
 * NAME
 *   b_order -- break off ships from a group or remove a group from a fleet
 * SOURCE
 */

void
b_order( game *aGame, player *P, strlist **s )
{
    group *g;                   /* pointer to named group */
    group *g2;                  /* pointer to new group */
    int i;                      /* int value for number of ships */
    char *ns;                   /* char value for number of ships */

    pdebug( DFULL, "b_order\n" );

    /* find the named group */
    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, INFO, *s, "Group not recognized." );
        return;
    }

    /* check to see if we're detaching from a fleet */
    ns = getstr( 0 );
    if ( noCaseStrncmp( ns, "fleet", 5 ) == 0 ) {
        if ( g->dist ) {
            mistake( P, WARNING, *s, "Fleet is in hyperspace." );
            return;
        }
        g->thefleet = 0;
        return;
    }

    /* are there enough ships to detach? */
    i = atoi( ns );
    if ( i > g->ships ) {       /* FS Dec 1998 */
        mistake( P, INFO, *s, "Not enough ships in group." );
        return;
    }

    /* this was an odd problem */
    if ( i < 0 ) {              /* KDW July 1999 */
        mistake( P, WARNING, *s, "Can't have negative number of ships." );
        return;
    }

    /* create a new group for what's being broken off and add it to
     * the list of groups the player owns
     */
    g2 = allocStruct( group );
    *g2 = *g;
    g2->ships = i;
    g->ships -= i;
    g2->thefleet = 0;
    g2->next = NULL;
    numberGroup( P, g2 );
    g2->name = ( char * ) malloc( 8 );
    sprintf( g2->name, "%d", g2->number );
    addList( &P->groups, g2 );
}

/******/


/****f* Process/c_order
 * NAME
 *   c_order -- change player's Race name
 * SOURCE
 */


void
c_order( game *aGame, player *P, strlist **s )
{
    char *ns;                   /* new name */
    char *c;                    /* loop variable */
    int i;                      /* loop variable */

    pdebug( DFULL, "c_order\n" );

    /* find name, chop off any non a-n chars */
    ns = getstr( 0 );
    i = strlen( ns );
    while ( i && !isalnum( ns[i - 1] ) )
        i--;
    ns[i] = 0;

    /* was a name given? */
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "No new race name given." );
        return;
    }

	if (noCaseStrcmp(ns, "GM") == 0) {
		mistake(P, ERROR, *s, "GM is a reserved name");
		return;
	}

	if (noCaseStrcmp(aGame->name, ns) == 0) {
		mistake(P, ERROR, *s, "You cannot use game name for a Race name");
		return;
	}
	
    /* someone else already using it? */
    if ( findElement( player, aGame->players, ns ) != NULL ) {
        mistake( P, ERROR, *s, "Race name already in use." );
        return;
    }

    /* is it too long? */
    if ( strlen( ns ) > NAMESIZE ) {
        mistake( P, ERROR, *s, "Name is too long\n." );
        return;
    }

    /* set the new name, convert non a-n chars to '_' */
    setName( P, ns );

    for ( c = P->name; *c; c++ ) {
        if ( !isalnum( *c ) )
            *c = '_';
    }
    pdebug( DFULL, "c_order end\n" );
}


/******/


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

    pdebug( DFULL, "d_order\n" );

    /* get design name, check for validity */
    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "No ship type name given." );
        return;
    }

    if ( strlen( ns ) > NAMESIZE ) {
        mistake( P, ERROR, *s, "Name is too long\n." );
        return;
    }

    if ( findElement( fleetname, P->fleetnames, ns ) )
    {
        mistake( P, ERROR, *s, "Name already in use for fleet." );
        return;
    }

    /* Occasionally someone designs a ship called "Cargo" or something, then 
     * doesn't understand why the planet didn't produce the ship when 
     * they do "P planet Cargo" :)  This will fix this problem.  - RJS
     */

    for ( i = 0; i < nbrProducts; i++ ) {
        if ( productname[i] && !noCaseStrcmp( productname[i], ns ) ) {
            mistake( P, ERROR, *s,
                     "Ship name can not be the same as a product name." );
            return;
        }
    }

    if ( findElement( shiptype, P->shiptypes, ns ) != NULL )
    {
        mistake( P, WARNING, *s, "Ship type name already in use." );
        return;
    }

    /* look for the fleet keyword, process it if this is new fleet
     * creation
     */
    if ( !noCaseStrncmp( ns, "fleet", 5 ) ) {
        /* get the fleet name, check for validity */
        ns = getstr( 0 );
        if ( !ns[0] ) {
            mistake( P, ERROR, *s, "No fleet name given." );
            return;
        }

        if ( isdigit( ns[0] ) ) {
            mistake( P, ERROR, *s,
                     "Fleet names cannot have an initial digit." );
            return;
        }

        if ( findElement( fleetname, P->fleetnames, ns ) )
        {
            mistake( P, ERROR, *s, "Fleet name already in use." );
            return;
        }

        if ( findElement( shiptype, P->shiptypes, ns ) )
        {
            mistake( P, ERROR, *s, "Name already in use for ship type." );
            return;
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
        if ( t->drive < 1 )
            t->drive = 0;

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

        if ( t->shields && t->shields < 1 ) {
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

        if ( t->cargo && t->cargo < 1 ) {
            t->cargo = 0;
            mistake( P, WARNING, *s,
                     "cargo size should be zero, or greater or equal to 1." );
        }

        if ( t->cargo && t->drive == 0.0 ) {    /* KDW 20040125 */
            mistake( P, WARNING, *s,
                     "unusual to have cargo ship with no drive." );
        }

        /* ensure the design can do something */
        if ( !t->drive && !t->attacks && !t->shields && !t->cargo ) {
            mistake( P, ERROR, *s,
                     "At least one component must be non-zero." );
            free( t );
            return;
        }

        if ( underDefined ) {
            mistake( P, WARNING, *s, "A ship design requires 5 parameters." );
        }

        addList( &P->shiptypes, t );
    }
}

/******/


/****f* Process/e_order
 * NAME
 *   e_order -- eliminate a ship or fleet
 * SOURCE
 */


void
e_order( game *aGame, player *P, strlist **s )
{
    shiptype *t;                /* type to be eliminated */
    fleetname *fl;              /* fleet to be eliminated */
    group *g;                   /* group containing the ship */
    planet *p;                  /* planet ship/fleet is orbiting */
    char *ns;                   /* retrieving parameters */

    pdebug( DFULL, "e_order\n" );

    /* see if this is a fleet or ship type */
    ns = getstr( 0 );
    t = findElement( shiptype, P->shiptypes, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    /* neither? must be a typo */
    if ( !t && !fl ) {
        mistake( P, ERROR, *s, "Ship or fleet type not recognized." );
        return;
    }

    /* if it's a fleet . . . */
    if ( fl ) {
        /* can't be in hyperspace */
        for ( g = P->groups; g; g = g->next ) {
            if ( ( g->thefleet == fl ) && ( g->dist ) ) {
                mistake( P, ERROR, *s, "Group is in hyperspace." );
                return;
            }
        }

        /* find it, delete it */
        for ( g = P->groups; g; g = g->next ) {
            if ( g->thefleet == fl )
                g->thefleet = 0;
        }

        remList( &P->fleetnames, fl );
    } else {
        /* it must be a group */

        /* can't delete ship type if ships of that type still exist */
        for ( g = P->groups; g; g = g->next ) {
            if ( g->type == t ) {
                mistake( P, ERROR, *s, "Some of these ships still exist." );
                return;
            }
        }

        if ( g )
            return;

        /* can't delete ship type if you're currently building them */
        for ( p = aGame->planets; p; p = p->next ) {
            if ( p->producingshiptype == t ) {
                mistake( P, ERROR, *s,
                         "Some of these ships are still being produced." );
                return;
            }
        }

        if ( p )
            return;

        /* ok, it's really orphaned, delete it */
        remList( &P->shiptypes, t );
    }
}

/******/


/****f* Process/f_order
 * NAME
 *   f_order -- request Race email
 * SOURCE
 */

void
f_order( game *aGame, player *P, strlist **s )
{
    player *P2;                 /* player requested */

    pdebug( DFULL, "f_order\n" );

    P2 = findElement( player, aGame->players, getstr( 0 ) );

    if ( !P2 ) {
        mistake( P, ERROR, *s, "Race not recognized." );
        return;
    }

    if ( P2->addr[0] ) {
        if ( P2->flags & F_ANONYMOUS ) {
            sprintf( lineBuffer, "%s plays anonymously", P2->name );
        } else {
            sprintf( lineBuffer, "The address of %s is %s", P2->name,
                     P2->addr );
        }
    } else
        sprintf( lineBuffer, "There is currently no address for %s.\n"
                 "See http://galaxyng.sourceforge.net/manual.php#email for "
                 "details on relaying messages", P2->name );

    addList( &P->messages, makestrlist( lineBuffer ) );
}

/******/


/****f* Process/g_order
 * NAME
 *   g_order -- 
 * SOURCE
 */

void
g_order( game *aGame, player *P, strlist **s )
{
    planet *p;
    group *g;
    group *g2;
    shiptype *t;
    double x;
    double y;
    double z;
    char *ns;
    int i;

    pdebug( DFULL, "g_order\n" );
    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, ERROR, *s, "Group not recognized." );
        return;
    }
    if ( g->dist ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        return;
    }
    p = g->where;
    t = g->type;
    plog( LFULL, "Upgrade: of %s on %s\n", t->name, p->name );
    if ( p->owner != P ) {
        mistake( P, ERROR, *s, "Planet is not owned by you." );
        return;
    }
    x = INDPERSHIP * ( ( 1 - g->drive / P->drive ) * t->drive +
                       ( 1 - g->weapons / P->weapons ) * weaponmass( t ) +
                       ( 1 - g->shields / P->shields ) * t->shields +
                       ( 1 - g->cargo / P->cargo ) * t->cargo );
    y = p->ind * .75 + p->pop * .25 - p->spent;
    if ( ( y <= 0.0 ) && ( x > 0.0 ) ) {
        mistake( P, ERROR, *s, "Planet has no remaining industry." );
        return;
    }
    if ( x == 0.0 ) {
        mistake( P, WARNING, *s, "Group is already at current tech levels." );
        return;
    }
    ns = getstr( 0 );
    if ( ns[0] ) {
        i = atoi( ns );
        if ( i == 0 )
            i = g->ships;
        if ( i > g->ships ) {
            mistake( P, ERROR, *s, "Not enough ships, all available used." );
            i = g->ships;
        }
        if ( i < 0 ) {          /* KDW July 1999 */
            mistake( P, ERROR, *s,
                     "Can't use negative number of ships, all available used." );
            i = g->ships;
        }
    } else
        i = g->ships;
    if ( i != g->ships ) {
        g2 = allocStruct( group );

        *g2 = *g;
        numberGroup( P, g2 );
        g2->next = NULL;
        g2->name = ( char * ) malloc( 8 );
        sprintf( g2->name, "%d", g2->number );
        addList( &P->groups, g2 );
        g->ships -= i;
        g2->ships = i;
        g = g2;
    }
    plog( LFULL, "Old Levels:\n" );
    plog( LFULL, "%f %f %f %f\n", g->drive, g->weapons, g->shields,
          g->cargo );
    z = y / i;
    if ( z >= x ) {
        plog( LFULL, "Full Upgrade\n" );
        memcpy( &g->drive, &P->drive, 4 * sizeof( double ) );
    } else {
        plog( LFULL, "Partial Upgrade\n" );
        z /= x;
        x *= z;
        g->drive = g->drive + ( P->drive - g->drive ) * z;
        g->weapons = g->weapons + ( P->weapons - g->weapons ) * z;
        g->shields = g->shields + ( P->shields - g->shields ) * z;
        g->cargo = g->cargo + ( P->cargo - g->cargo ) * z;
    }
    cktech( g );
    plog( LFULL, "New Levels:\n" );
    plog( LFULL, "%f %f %f %f\n", g->drive, g->weapons, g->shields,
          g->cargo );
    p->spent += x * i;
}



void
h_order( game *aGame, player *P, strlist **s )
{                               /* CB-1990923 */
    group *g;
    fleetname *fl;
    char *ns;

    pdebug( DFULL, "h_order\n" );
    ns = getstr( 0 );           /* get group number or fleet name */
    g = findgroup( P, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    if ( !g && !fl ) {
        mistake( P, ERROR, *s, "Group or fleet not recognized." );
        return;
    }
    if ( fl ) {
        double fleetspeed;

        /* Fixed the "can turn around fleets, late" bug */
        fleetspeed = fleetSpeed( fl, P->groups );

        for ( g = P->groups; g; g = g->next ) {
            if ( g->thefleet == fl )
                if ( !g->dist ) {
                    mistake( P, ERROR, *s, "Fleet is not in hyperspace." );
                    return;
                }
            if ( g->thefleet == fl )
                if ( g->dist < fleetspeed * TURNS_LEFT ) {
                    mistake( P, ERROR, *s,
                             "Fleet is too near from landing." );
                    return;
                }
        }
        for ( g = P->groups; g; g = g->next )
            if ( g->thefleet == fl )
                recall( aGame, g );
        return;
    }
    if ( !g->dist ) {
        mistake( P, ERROR, *s, "Group is not in hyperspace." );
        return;
    }
    if ( g->dist < g->type->drive * g->drive *
         DRIVEMAGIC / shipmass( g ) * TURNS_LEFT ) {
        mistake( P, ERROR, *s, "Group is too near from landing." );
        return;
    }
    recall( aGame, g );
}



void
i_order( game *aGame, player *P, strlist **s )
{
    group *inGroup;
    planet *inPlanet;
    fleetname *inFleet;
    char *ns;

    pdebug( DFULL, "i_order\n" );
    ns = getstr( 0 );
    inGroup = findgroup( P, ns );
    inFleet = findElement( fleetname, P->fleetnames, ns );

    if ( !inGroup && !inFleet ) {
        mistake( P, ERROR, *s, "Group or fleet not recognized." );
        return;
    } else {
        inPlanet = findPlanet( aGame, getstr( 0 ) );
        if ( !inPlanet ) {
            mistake( P, ERROR, *s, "Planet not recognized." );
            return;
        }
    }

    if ( inFleet ) {
        group *aGroup;

        for ( aGroup = P->groups; aGroup; aGroup = aGroup->next ) {
            if ( ( aGroup->thefleet == inFleet ) && ( aGroup->dist ) ) {
                mistake( P, ERROR, *s, "Fleet is in hyperspace." );
                return;
            }
            if ( ( aGroup->thefleet == inFleet ) && ( !aGroup->type->drive ) ) {
                mistake( P, ERROR, *s, "Fleet cannot move." );
                return;
            }
        }

        for ( aGroup = P->groups; aGroup; aGroup = aGroup->next ) {
            if ( aGroup->thefleet == inFleet ) {
                send( aGame, aGroup, inPlanet );
                aGroup->flags |= GF_INTERCEPT;
            }
        }
    } else {
        int noShips;

        if ( inGroup->type->drive == 0 ) {
            mistake( P, ERROR, *s, "Ships in this group can't move." );
            return;
        }
        if ( inGroup->dist ) {
            mistake( P, ERROR, *s, "Group is in hyperspace." );
            return;
        }
        ns = getstr( 0 );
        if ( *ns ) {
            noShips = atoi( ns );
            if ( noShips <= 0 ) {       /* KDW July 1999 - added return */
                mistake( P, WARNING, *s,
                         "You must specify more than 0 Ships." );
                return;
            }
        } else
            noShips = inGroup->ships;

        if ( noShips ) {
            group *newGroup;

            if ( noShips > inGroup->ships ) {
                mistake( P, WARNING, *s,
                         "Not enough ships, all available used." );
                noShips = inGroup->ships;
            }
            if ( noShips != inGroup->ships ) {
                newGroup = allocStruct( group );

                *newGroup = *inGroup;
                numberGroup( P, newGroup );
                newGroup->name = ( char * ) malloc( 8 );
                sprintf( newGroup->name, "%d", newGroup->number );
                newGroup->next = NULL;
                addList( &P->groups, newGroup );
                inGroup->ships -= noShips;
                newGroup->ships = noShips;
                send( aGame, newGroup, inPlanet );
                newGroup->flags |= GF_INTERCEPT;
                newGroup->thefleet = NULL;
            } else {
                send( aGame, inGroup, inPlanet );
                inGroup->flags |= GF_INTERCEPT;
                inGroup->thefleet = NULL;
            }
        } else {
            mistake( P, ERROR, *s, "No ships left in group." );
        }
    }
}



void
j_order( game *aGame, player *P, strlist **s )
{
    planet *p;
    planet *p2;
    char *ns;
    group *g;
    group *g2;
    double dist;
    double dist2;
    fleetname *fl;
    fleetname *fl2;
    int i;
    int j;

    pdebug( DFULL, "j_order\n" );

    ns = getstr( 0 );
    g = findgroup( P, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    if ( !g && !fl ) {
        mistake( P, ERROR, *s, "Source Group or fleet not recognized." );
        return;
    }
    if ( fl ) {
        fl2 = findElement( fleetname, P->fleetnames, getstr( 0 ) );

        if ( !fl2 ) {
            mistake( P, ERROR, *s, "Destination fleet not recognized." );
            return;
        }
        dist = 0.0;
        dist2 = 0.0;
        p = ( planet * ) NULL;
        p2 = ( planet * ) NULL;
        for ( g = P->groups; g; g = g->next ) {
            if ( g->thefleet == fl ) {
                p = g->where;
                dist = g->dist;
            }
            if ( g->thefleet == fl2 ) {
                p2 = g->where;
                dist2 = g->dist;
            }
        }
        if ( dist ) {
            mistake( P, ERROR, *s, "First fleet is in hyperspace." );
            return;
        }
        if ( dist2 ) {
            mistake( P, ERROR, *s, "Second fleet is in hyperspace." );
            return;
        }
        if ( ( p != p2 ) && ( p2 ) ) {
            mistake( P, ERROR, *s, "Fleets are not at the same planet." );
            return;
        }
        for ( g = P->groups; g; g = g->next )
            if ( g->thefleet == fl )
                g->thefleet = fl2;
        return;
    }
    if ( g->dist ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        return;
    }
    fl = findElement( fleetname, P->fleetnames, getstr( 0 ) );
    if ( !fl ) {
        mistake( P, ERROR, *s, "Fleet type not recognized." );
        return;
    }
    if ( ( j = atoi( getstr( 0 ) ) ) != 0 ) {
        i = j;
        if ( i > g->ships ) {
            mistake( P, ERROR, *s, "Not enough ships, all available used." );
            i = g->ships;
        }
        if ( i <= 0 ) {
            mistake( P, ERROR, *s,
                     "You must specify more than 0 ships, all available used." );
            i = g->ships;
        }
        if ( i != g->ships ) {
            g2 = allocStruct( group );

            *g2 = *g;
            g2->next = NULL;
            numberGroup( P, g2 );
            g2->name = ( char * ) malloc( 8 );
            sprintf( g2->name, "%d", g2->number );
            addList( &P->groups, g2 );
            g->ships -= i;
            g2->ships = i;
            g = g2;
        }
    }
    for ( g2 = P->groups; g2; g2 = g2->next ) {
        if ( ( g2->thefleet == fl ) && ( g2->dist ) ) {
            mistake( P, ERROR, *s, "Fleet is in hyperspace." );
            return;
        }
        if ( ( g2->thefleet == fl ) && ( !g2->dist )
             && ( g2->where != g->where ) ) {
            mistake( P, ERROR, *s, "Group is at the wrong planet." );
            return;
        }
    }
    g->thefleet = fl;
}


/****f* Process/l_order
 * NAME
 *   l_order -- load cargo onto a group of ships.
 * FUNCTION
 *
 * SOURCE
 */

void
l_order( game *aGame, player *P, strlist **s )
{
    group *g;
    group *g2;
    double x, amount;
    double y;
    int amountFlag;
    planet *p;
    int numberOfShips;
    int typeOfCargo;
    char *ns;

    amountFlag = FALSE;

    pdebug( DFULL, "l_order\n" );
    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, ERROR, *s, "Group not recognized." );
        return;
    }
    p = g->where;

    typeOfCargo = nametocargotype( getstr( 0 ) );
    if ( g->type->cargo == 0 ) {
        mistake( P, ERROR, *s, "Group cannot carry cargo." );
        return;
    }
    if ( g->dist ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        return;
    }
    if ( cargospace( g ) <= g->load ) {
        /* '<=' is used cause we deal with doubles here */
        mistake( P, ERROR, *s, "Group is fully loaded." );
        return;
    }
    if ( typeOfCargo < 0 || typeOfCargo > 2 ) {
        mistake( P, ERROR, *s, "Cargo type not recognized." );
        return;
    }
    if ( g->load && g->loadtype != typeOfCargo ) {
        mistake( P, ERROR, *s,
                 "Group is already carrying a different load." );
        return;
    }
    /* Determine the number of ships to load the cargo on */

    numberOfShips = g->ships;   /* Start-off with using all ships */
    ns = getstr( 0 );
    if ( ( ns[0] != '\0' ) &&
         ( noCaseStrcmp( "amount", ns ) != 0 ) && ( isdigit( ns[0] ) ) ) {
        numberOfShips = atoi( ns );
        if ( numberOfShips != 0 ) {
            if ( numberOfShips > g->ships ) {
                mistake( P, ERROR, *s,
                         "Not enough ships, all available used." );
                numberOfShips = g->ships;
            }
            if ( numberOfShips <= 0 ) { /* KDW July 1999 */
                mistake( P, ERROR, *s,
                         "You must specify > 0 ships, all available used." );
                numberOfShips = g->ships;
            }
        }
        ns = getstr( 0 );
    }
    /* Determine the amount of cargo to be loaded per ship */
    if ( ns[0] != '\0' ) {
        if ( noCaseStrcmp( "amount", ns ) == 0 ) {
            ns = getstr( 0 );
            if ( ns[0] != '\0' ) {
                amount = atof( ns );
                if ( amount > ( cargospace( g ) - g->load ) ) {
                    mistake( P, ERROR, *s,
                             "Not enough cargo space available to carry this amount." );
                    return;
                }
            } else {
                mistake( P, ERROR, *s,
                         "Keyword AMOUNT should be followed by a number." );
                return;
            }
        } else {
            mistake( P, ERROR, *s, "Expected the keyword AMOUNT." );
            return;
        }
        amountFlag = TRUE;
    } else {
        amount = cargospace( g ) - g->load;
    }

    if ( amount < AMOUNTMIN ) {
        mistake( P, ERROR, *s,
                 "You should load at least %f per ship,"
                 " you are trying to loading %f.", AMOUNTMIN, amount );
        return;
    }
    /* Check if the planet has enough of the goods requested. */

    switch ( typeOfCargo ) {
    case CG_CAP:
        y = p->cap;
        break;
    case CG_MAT:
        y = p->mat;
        break;
    case CG_COL:
        y = p->col;
        break;
    }

    /* Total amount that has to be uploaded */
    x = amount * numberOfShips;
    if ( y == 0.0 ) {
        mistake( P, ERROR, *s, "No cargo of this type available on \"%s\".",
                 p->name );
        return;
    }
    if ( roundup2(y / numberOfShips) < AMOUNTMIN ) {
        mistake( P, ERROR, *s,
                 "Not enough cargo available on \"%s\" to"
                 " load at least %f per ship (%f per ship available, %f / %d).",
				 p->name, AMOUNTMIN, roundup2(y / numberOfShips), y, numberOfShips );
        return;
    }
    if ( y < x ) {
        if ( amountFlag ) {
            mistake( P, ERROR, *s,
                     "Not enough cargo available on \"%s\" to"
                     " load %.2f per ship,\nloading %.2f per ship.",
                     p->name, amount, y / numberOfShips );
        }
        amount = y / numberOfShips;
    }
    /* Everything is OK, break off a group, and load cargo */

    if ( numberOfShips != g->ships ) {
        g2 = allocStruct( group );

        *g2 = *g;
        g2->next = NULL;
        numberGroup( P, g2 );
        g2->name = ( char * ) malloc( 8 );
        sprintf( g2->name, "%d", g2->number );
        addList( &P->groups, g2 );
        assert( numberOfShips < g->ships );
        g->ships -= numberOfShips;
        g2->ships = numberOfShips;
        g = g2;
    }
    switch ( typeOfCargo ) {
    case CG_CAP:
        if ( y > x )
            y = x;
        g->load += y / g->ships;
        p->cap -= y;
        break;
    case CG_MAT:
        if ( y > x )
            y = x;
        g->load += y / g->ships;
        p->mat -= y;
        break;
    case CG_COL:
        if ( y > x )
            y = x;
        g->load += y / g->ships;
        p->col -= y;
        break;
    }
    g->loadtype = typeOfCargo;
}

/***** End l_order ******/






void
m_order( game *aGame, player *P, strlist **s )
{
    double x;
    double y;
    double z;

    pdebug( DFULL, "m_order\n" );

    x = atof( getstr( 0 ) );
    y = atof( getstr( 0 ) );
    z = atof( getstr( 0 ) );
    if ( z < 1 ) {
        mistake( P, ERROR, *s, "Size must be at least 1." );
        return;
    }
    P->mx = x;
    P->my = y;
    P->msize = z;
}



void
n_order( game *aGame, player *P, strlist **s )
{
    planet *p;
    char *ns;

    pdebug( DFULL, "n_order %s\n", ( *s )->str );

    ns = getstr( 0 );
    p = findElement( planet, aGame->planets, ns );

    if ( !p ) {
        mistake( P, ERROR, *s, "Planet not recognized." );
        return;
    }
    if ( p->owner != P ) {
        mistake( P, ERROR, *s, "Planet not owned by you." );
        return;
    }
    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "New planet name not provided." );
        return;
    }
    if ( strlen( ns ) > NAMESIZE ) {
        mistake( P, ERROR, *s, "Name is too long\n." );
        return;
    }
    if ( findPlanet( aGame, ns ) != NULL ) {
        mistake( P, ERROR, *s, "Name already in use." );
        return;
    }
    setName( p, ns );
    ns = getstr( 0 );
    if ( *ns ) {
        mistake( P, ERROR, *s, "Too many parameters." );
        return;
    }
}



void
o_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    int state;
    option *curOption;

    pdebug( DFULL, "o_order\n" );

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
}


void
p_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    planet *p;
    shiptype *t;

    pdebug( DFULL, "p_order\n" );

    p = findPlanet( aGame, getstr( 0 ) );
    if ( !p ) {
        mistake( P, ERROR, *s, "Planet not recognized." );
        return;
    }
    if ( p->owner != P ) {
        mistake( P, ERROR, *s, "Planet not owned by you." );
        return;
    }
    ns = getstr( 0 );
    if ( !noCaseStrcmp( ns, "cap" ) ) {
        setproduction( aGame, p, PR_CAP );
        return;
    }
    if ( !noCaseStrcmp( ns, "mat" ) ) {
        setproduction( aGame, p, PR_MAT );
        return;
    }
    if ( !noCaseStrcmp( ns, "drive" ) ) {
        setproduction( aGame, p, PR_DRIVE );
        return;
    }
    if ( !noCaseStrcmp( ns, "weapons" ) ) {
        setproduction( aGame, p, PR_WEAPONS );
        return;
    }
    if ( !noCaseStrcmp( ns, "shields" ) ) {
        setproduction( aGame, p, PR_SHIELDS );
        return;
    }
    if ( !noCaseStrcmp( ns, "cargo" ) ) {
        setproduction( aGame, p, PR_CARGO );
        return;
    }
    t = findElement( shiptype, P->shiptypes, ns );
    if ( !t ) {
        mistake( P, ERROR, *s, "Production type \"%s\" not recognized.", ns );
        return;
    }
    if ( p->producing != PR_SHIP || p->producingshiptype != t ) {
        setproduction( aGame, p, PR_SHIP );
        p->producingshiptype = t;
    }
}


void
q_order( game *aGame, player *P, strlist **s )
{

    pdebug( DFULL, "q_order\n" );

    if ( findElement( player, aGame->players, getstr( 0 ) ) != P )
    {
        mistake( P, ERROR, *s, "Race identification not given." );
        return;
    }
    P->flags |= F_DEAD;
}


void
r_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    planet *p;
    planet *p2;
    int i;

    pdebug( DFULL, "r_order\n" );

    ns = getstr( 0 );
    p = findPlanet( aGame, ns );
    if ( !p ) {
        mistake( P, ERROR, *s, "Source planet \"%s\" not recognized.", ns );
        return;
    }
    if ( p->owner != P ) {
        mistake( P, ERROR, *s, "You do not own planet \"%s\".", p->name );
        return;
    }
    ns = getstr( 0 );
    i = nametocargotype( ns );
    if ( i < 0 ) {
        mistake( P, ERROR, *s, "Cargo type \"%s\" not recognized.", ns );
        return;
    }
    p2 = 0;
    ns = getstr( 0 );
    if ( ns[0] ) {
        p2 = findPlanet( aGame, ns );
        if ( !p2 ) {
            mistake( P, ERROR, *s,
                     "Destination planet \"%s\" not recognized.", ns );
            return;
        }
    }
    p->routes[i] = p2;
}


void
s_order( game *aGame, player *P, strlist **s )
{
    group *g;
    group *g2;
    planet *p;
    int i;
    int j;
    fleetname *fl;
    char *ns;

    pdebug( DFULL, "s_order\n" );
    plog( LFULL, "s_order\n" );

    ns = getstr( 0 );
    plog( LFULL, "ns: %s\n", ns );

    g = findgroup( P, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    plog( LFULL, "g: %s   fl: %s\n", g ? g->name : "NULL", fl );
    if ( !g && !fl ) {
        mistake( P, ERROR, *s, "Group or fleet not recognized." );
        return;
    }
    if ( fl ) {
        p = findPlanet( aGame, getstr( 0 ) );
        if ( !p ) {
            mistake( P, ERROR, *s, "Planet not recognized." );
            return;
        }
        for ( g = P->groups; g; g = g->next ) {
            if ( ( g->thefleet == fl ) && ( g->dist ) ) {
                mistake( P, ERROR, *s, "Fleet is in hyperspace." );
                return;
            }
            if ( ( g->thefleet == fl ) && ( !g->type->drive ) ) {
                mistake( P, ERROR, *s, "Fleet cannot move." );
                return;
            }
        }
        for ( g = P->groups; g; g = g->next )
            if ( g->thefleet == fl )
                send( aGame, g, p );
        return;
    }
    if ( g->type->drive == 0 ) {
        mistake( P, ERROR, *s, "Ships in this group can't move." );
        return;
    }
    if ( g->dist ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        return;
    }
    p = findPlanet( aGame, getstr( 0 ) );
    if ( !p ) {
        mistake( P, ERROR, *s, "Planet not recognized." );
        return;
    }
    i = g->ships;
    if ( ( j = atoi( getstr( 0 ) ) ) != 0 ) {
        i = j;
        if ( i > g->ships ) {
            mistake( P, ERROR, *s, "Not enough ships, all available used." );
            i = g->ships;
        }
        if ( i <= 0 ) {         /* KDW July 1999 */
            mistake( P, ERROR, *s,
                     "You must specify more than 0 ships, all available used." );
            i = g->ships;
        }
        if ( i != g->ships ) {
            g2 = allocStruct( group );

            *g2 = *g;
            g2->next = NULL;
            numberGroup( P, g2 );
            g2->name = ( char * ) malloc( 8 );
            sprintf( g2->name, "%d", g2->number );
            addList( &P->groups, g2 );
            g->ships -= i;
            g2->ships = i;
            g = g2;
        }
    }
    g->thefleet = 0;
    send( aGame, g, p );
}


void
t_order( game *aGame, player *P, strlist **s )
{
    char *ns;
    fleetname *fl;
    shiptype *t;

    pdebug( DFULL, "t_order\n" );

    ns = getstr( 0 );
    t = findElement( shiptype, P->shiptypes, ns );
    fl = findElement( fleetname, P->fleetnames, ns );

    if ( !t && !fl ) {
        mistake( P, ERROR, *s, "Ship or fleet type not recognized." );
        return;
    }
    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "New name not provided" );
        return;
    }
    if ( findElement( fleetname, P->fleetnames, ns ) )
    {
        mistake( P, ERROR, *s, "Name already in use for fleet type." );
        return;
    }
    if ( findElement( shiptype, P->shiptypes, ns ) != NULL )
    {
        mistake( P, ERROR, *s, "Name already in use for ship type." );
        return;
    }
    if ( fl ) {
        setName( fl, ns );
    } else {
        setName( t, ns );
    }
}

/****f* Process/u_order
 * NAME
 *   u_order -- unload cargo
 * SOURCE
 */

void
u_order( game *aGame, player *P, strlist **s )
{
    group *g;
    group *g2;
    int numberOfShips;
    double amount;
    char *ns;

    pdebug( DFULL, "u_order\n" );

    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, ERROR, *s, "Group not recognized." );
        return;
    }
    if ( g->loadtype == CG_EMPTY ) {
        mistake( P, ERROR, *s, "No cargo on board." );
        return;
    }
    if ( g->dist ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        return;
    }
    /* Determine how many ships are unloaded */
    numberOfShips = g->ships;   /* Start-of with all ships */
    ns = getstr( 0 );
    if ( ( ns[0] != '\0' ) &&
         ( noCaseStrcmp( "amount", ns ) != 0 ) && ( isdigit( ns[0] ) ) ) {
        numberOfShips = atoi( ns );
        if ( numberOfShips != 0 ) {
            if ( numberOfShips > g->ships ) {
                mistake( P, ERROR, *s,
                         "Not enough ships, all available used." );
                numberOfShips = g->ships;
            }
            if ( numberOfShips <= 0 ) { /* KDW July 1999 */
                mistake( P, ERROR, *s,
                         "You must specify > 0 ships, all available used." );
                numberOfShips = g->ships;
            }
        }
        ns = getstr( 0 );
    }
    /* Determine the amount of cargo to be unloaded per ship */
    if ( ns[0] != '\0' ) {
        if ( noCaseStrcmp( "amount", ns ) == 0 ) {
            ns = getstr( 0 );
            if ( ns[0] != '\0' ) {
                amount = atof( ns );
                if ( amount > g->load ) {
                    amount = g->load;
                    mistake( P, ERROR, *s,
                             "Group does not carry that much cargo, using amount=%.2f",
                             amount );
                }
                if ( amount < AMOUNTMIN ) {
                    mistake( P, ERROR, *s, "Unload atleast %.2f per ship.",
                             AMOUNTMIN );
                    return;
                }
            } else {
                mistake( P, ERROR, *s,
                         "Keyword AMOUNT should be followed by a number." );
                return;
            }
        } else {
            mistake( P, ERROR, *s, "Expected the keyword AMOUNT." );
            return;
        }
    } else {
        amount = g->load;
    }

    if ( numberOfShips != g->ships ) {
        g2 = allocStruct( group );

        *g2 = *g;
        g2->next = NULL;
        numberGroup( P, g2 );
        g2->name = ( char * ) malloc( 8 );
        sprintf( g2->name, "%d", g2->number );
        addList( &P->groups, g2 );
        assert( numberOfShips < g->ships );
        g->ships -= numberOfShips;
        g2->ships = numberOfShips;
        g = g2;
    }
    unloadgroup( g, P, amount );
}

/***** END u_order ******/


/****f* Process/v_order
 * NAME
 *   v_order -- claim a planet.
 * SOURCE
 */

void
v_order( game *aGame, player *P, strlist **s )
{
    planet *p;
    planet_claim *pclaim;

    pdebug( DFULL, "v_order\n" );

    p = findPlanet( aGame, getstr( 0 ) );
    if ( !p ) {
        mistake( P, ERROR, *s, "Planet not recognized." );
    } else {
        plog( LFULL, "Race %s claims planet %s\n", P->name, p->name );
        pclaim = allocStruct( planet_claim );

        pclaim->planet_claimed = p;
        pclaim->next = NULL;
        addList( &( P->claimed_planets ), pclaim );
    }
}

/***** END v_order *****/


/****f* Process/w_order
 * NAME
 *   w_order -- declare war on another race.
 * SOURCE
 */

void
w_order( game *aGame, player *P, strlist **s )
{
    player *P2;
    alliance *a;

    pdebug( DFULL, "w_order\n" );

	if (aGame->turn < aGame->gameOptions.galactic_peace) {
		mistake(P, ERROR, *s, "Galactic Peace enforced until turn %d.",
				aGame->gameOptions.galactic_peace);
		return;
	}
	
    P2 = findElement( player, aGame->players, getstr( 0 ) );

    if ( !P2 ) {
        mistake( P, ERROR, *s, "Race not recognized." );
        return;
    }

    for ( a = P->allies; a; a = a->next ) {
        if ( a->who == P2 ) {
            remList( &P->allies, a );
            return;
        }
    }
}

/***** END w_order *****/


void
x_order( game *aGame, player *P, strlist **s )
{
    group *g;
    group *g2;
    int i;
    int j;

    pdebug( DFULL, "x_order\n" );

    g = findgroup( P, getstr( 0 ) );
    if ( !g ) {
        mistake( P, ERROR, *s, "Group not recognized." );
        return;
    }
    if ( g->dist ) {
        mistake( P, ERROR, *s, "Group is in hyperspace." );
        return;
    }
    if ( g->loadtype == CG_COL && g->where->owner && g->where->owner != P ) {
        mistake( P, ERROR, *s,
                 "Can't unload colonists onto an alien planet." );
        return;
    }
    i = g->ships;
    if ( ( j = atoi( getstr( 0 ) ) ) != 0 ) {
        i = j;
        if ( i > g->ships ) {
            mistake( P, ERROR, *s, "Not enough ships, all available used." );
            i = g->ships;
        }
        if ( i <= 0 ) {         /* KDW July 1999 */
            mistake( P, ERROR, *s,
                     "You must specify more than 0 ships, all available used." );
            i = g->ships;
        }
        if ( i != g->ships ) {
            g2 = allocStruct( group );

            *g2 = *g;
            g2->next = NULL;
            numberGroup( P, g2 );
            g2->name = ( char * ) malloc( 8 );
            sprintf( g2->name, "%d", g2->number );
            addList( &P->groups, g2 );
            g->ships -= i;
            g2->ships = i;
            g = g2;
        }
    }
    unloadgroup( g, P, g->load );
    g->where->mat += shipmass( g ) * g->ships;
    g->ships = 0;
}


void
y_order( game *aGame, player *P, strlist **s )
{
    char *ns;

    pdebug( DFULL, "y_order\n" );

    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "New password not provided." );
        return;
    }
    if ( P->pswd )
        free( P->pswd );
    P->pswd = strdup( ns );
    P->pswdstate = 1;
}


void
z_order( game *aGame, player *P, strlist **s )
{
    char *ns;

    pdebug( DFULL, "z_order\n" );

    ns = getstr( 0 );
    if ( !ns[0] ) {
        mistake( P, ERROR, *s, "No new address given." );
        return;
    }
    if ( P->addr )
        free( P->addr );
    P->addr = strdup( ns );
}




/****f* Process/runTurn
 * NAME
 *   runTurn -- run a turn.
 * SYNOPSIS
 *   int runTurn(game *, char *)
 *   result = runTurn(aGame, ordersFileName)
 * FUNCTION
 *   Move one turn forward in time. All orders sent in by the races
 *   are executed and the different phases are executed.  
 *   Races that did not sent in orders for a number of consecutive
 *   turns are removed from the game.
 *   Although we can assume all the orders are correct, since they
 *   have been processed by checkorders(), we stil do some sanity
 *   checks.
 * INPUTS
 *   aGame --
 *   ordersFileName -- name of the file that contains the orders
 *                     of _all_ the races.
 * RESULTS
 *   aGame->turn is increased by one.
 *   aGame       contains the new situation, including battles,
 *               bombings and messages.
 *   result 
 *     FALSE -- Error occured during turn processing
 *     TRUE  -- All OK
 * NOTE
 *   The turn is not saved and no reports are generated, this is handled
 *   by seperate functions.  This function should be split in parts.
 * SOURCE
 */

int
runTurn( game *aGame, char *ordersFileName )
{
    player *P;
    char *oGameName;
    char *raceName;
    char *password;
    FILE *ordersFile;

	char* rm_notify;

	rm_notify = createString("/bin/rm -f %s/orders/%s/*.notify",
							 galaxynghome, aGame->name);
	/*printf("executing \"%s\"\n", rm_notify);*/
	ssystem(rm_notify);
	free(rm_notify);
			
    plog( LPART, "Reading orders from file %s\n", ordersFileName );

    ordersFile = Fopen( ordersFileName, "r" );

    getLine( ordersFile );
    for ( ; !feof( ordersFile ); ) {
      char* ptr;
      ptr = lineBuffer + strspn(lineBuffer, " \t");
      if (ptr && *ptr == ';')
	continue;		/* skip comment lines */
      if ((ptr = strchr(lineBuffer, '#')) == NULL)
	ptr = lineBuffer;
        if ( noCaseStrncmp( "#GALAXY", ptr, 7 ) == 0 ) {
            player *aPlayer;

            getstr( ptr );
            oGameName = strdup( getstr( NULL ) );
            raceName = strdup( getstr( NULL ) );
            password = strdup( getstr( NULL ) );
            if ( noCaseStrcmp( oGameName, aGame->name ) == 0 ) {
                aPlayer = findElement( player, aGame->players, raceName );

                if ( aPlayer ) {
                    aPlayer->lastorders = aGame->turn + 1;
                    if ( noCaseStrcmp( aPlayer->pswd, password ) == 0 ) {
                        aPlayer->orders = NULL;
                        getLine( ordersFile );
			if ((ptr = strchr(lineBuffer, '#')) == NULL)
			  ptr = lineBuffer;
                        for ( ; !feof( ordersFile ) &&
                              noCaseStrncmp( "#GALAXY", ptr, 7 ) &&
                              noCaseStrncmp( "#END", ptr, 4 ); ) {
                            strlist *s;

                            if ( ( s = makestrlist( lineBuffer ) ) != NULL )
                                addList( &( aPlayer->orders ), s );
                            getLine( ordersFile );
			    if ((ptr = strchr(lineBuffer, '#')) == NULL)
			      ptr = lineBuffer;

                        }
                    } else {
                        plog( LPART, "Password Incorrect.\n" );
                    }
                } else {
                    plog( LPART, "Unrecognized player %s.\n", raceName );
                }
            } else {
                plog( LPART, "Orders are not for game %s.\n", aGame->name );
            }
            free( oGameName );
            free( raceName );
            free( password );
        }
        getLine( ordersFile );
    }
    fclose( ordersFile );

    ( aGame->turn )++;

    if ( !checkIntegrity( aGame ) )
        return FALSE;

    plog( LPART, "Orders read, processing...\n" );
    plog( LFULL, "# Phase 1 Orders\n" );
    for ( P = aGame->players; P; P = P->next ) {
        doOrders( aGame, P, phase1orders, 1 );
    }

    if ( !checkIntegrity( aGame ) )
        return FALSE;

    plog( LFULL, "# Phase 2 Orders\n" );
    for ( P = aGame->players; P; P = P->next ) {
        doOrders( aGame, P, phase2orders, 2 );
    }

    if ( !checkIntegrity( aGame ) )
        return FALSE;

    plog( LFULL, "# Phase 3 Orders\n" );
    for ( P = aGame->players; P; P = P->next ) {
        doOrders( aGame, P, phase3orders, 3 );
    }

    if ( !checkIntegrity( aGame ) )
        return FALSE;

    plog( LFULL, "# joinphase I\n" );
    joinphase( aGame );
    preComputeGroupData( aGame );
    plog( LFULL, "# fightphase I\n" );
    fightphase( aGame, GF_INBATTLE1 );
    plog( LFULL, "# bombphase I\n" );
    bombphase( aGame );
    plog( LFULL, "# loadphase\n" );
    loadphase( aGame );
    plog( LFULL, "# fleetphase I \n" );
    fleetphase( aGame );
    if ( !checkIntegrity( aGame ) )
        return FALSE;
    plog( LFULL, "# interceptphase\n" );
    interceptphase( aGame );
    plog( LFULL, "# movephase\n" );
    movephase( aGame );
    plog( LFULL, "# joinphase II\n" );
    joinphase( aGame );
    preComputeGroupData( aGame );
    plog( LFULL, "# fightphase II\n" );
    fightphase( aGame, GF_INBATTLE2 );
    plog( LFULL, "# bombphase II\n" );
    bombphase( aGame );
    plog( LFULL, "# producephase\n" );
    producephase( aGame );
    plog( LFULL, "# unloadphase\n" );
    unloadphase( aGame );
    plog( LFULL, "# joinphase III\n" );
    joinphase( aGame );
    plog( LFULL, "# fleetphase II\n" );
    fleetphase( aGame );
    if ( !checkIntegrity( aGame ) )
        return FALSE;
    preComputeGroupData( aGame );
    sortphase( aGame );

    if ( !( aGame->gameOptions.gameOptions & GAME_NODROP ) )
        removeDeadPlayer( aGame );
    raceStatus( aGame );

    return TRUE;
}

/****************/


/****f* Process/checkOrders
 * NAME
 *   checkOrders --  check orders
 * SYNOPSIS
 *   int checkOrders(char *ordersFileName)
 *   resNumber = checkOrders(ordersFileName)
 * FUNCTION
 *   Checks a file with orders and prints a report with a forecast and 
 *   any errors found in the orders to stdout.
 *   It is checked that the orders contain a valid game name, race
 *   name and password.
 *   A copy of the orders is stored in the directory orders/<gameName>. 
 *
 *   If the orders start with #REPORT instead of #GALAXY a copy
 *   of the previous turn report is send to stdout.
 * INPUTS
 *   ordersFileName - name of the file with the orders.
 * RESULT
 *   resNumber -- return code:
 *      RES_NO_ORDERS - no line containing "#GALAXY" or #REPORT was found.
 *      RES_PASSWORD  - password was incorrect.
 *      RES_PLAYER    - no such player (Race) exists.
 *      RES_NO_GAME   - no such game exists.
 *      RES_TURNRAN   - orders are for a turn that already ran.
 *      RES_OK        - everything was OK.
 * BUGS
 *   Does not handle machine reports or xml reports.
 * SEE ALSO
 *   areValidOrders(), copyOrders() 
 * SOURCE
 */

#if FS_NEW_FORECAST
void
checkOrders( game *aGame, char *raceName )
{
    player *aPlayer;

    pdebug( DFULL, "check orders\n" );
    aPlayer = findElement( player, aGame->players, raceName );

    tagVisiblePlanets( aGame->planets, aPlayer );

    checkIntegrity( aGame );

    doOrders( aGame, aPlayer, phase1orders, 1 );
    doOrders( aGame, aPlayer, phase2orders, 2 );

    joinphase( aGame );
    loadphase( aGame );
    fleetphase( aGame );
    checkIntegrity( aGame );
    interceptphase( aGame );
    movephase( aGame );
    joinphase( aGame );
    producephase( aGame );
    unloadphase( aGame );
    joinphase( aGame );
    fleetphase( aGame );

    preComputeGroupData( aGame );
    sortphase( aGame );
    checkIntegrity( aGame );
}
#else
void
checkOrders( game *aGame, char *raceName, FILE *forecast, int kind )
{
    player *aPlayer;
    struct fielddef fields;

    /* blatant attempt to avoid doing orders more than once if more than 
       one report type is being generated */
    static int orders_done = 0;

    pdebug( DFULL, "check orders\n" );
    aPlayer = findElement( player, aGame->players, raceName );

    fields.destination = forecast;
    tagVisiblePlanets( aGame->planets, aPlayer );

    checkIntegrity( aGame );

    if ( orders_done == 0 ) {
        doOrders( aGame, aPlayer, phase1orders, 1 );
        doOrders( aGame, aPlayer, phase2orders, 2 );

        joinphase( aGame );
        loadphase( aGame );
        fleetphase( aGame );
        checkIntegrity( aGame );
        interceptphase( aGame );
        movephase( aGame );
        joinphase( aGame );
        producephase( aGame );
        unloadphase( aGame );
        joinphase( aGame );
        fleetphase( aGame );

        preComputeGroupData( aGame );
        sortphase( aGame );
        checkIntegrity( aGame );
    }

    ( aGame->turn )++;
    if ( kind == F_XMLREPORT ) {
        report_xml( aGame, aPlayer, forecast, Forecast );
    } else {
        raceStatus( aGame );
        reportGlobalMessages( aGame->messages, &fields );
        reportMessages( aPlayer, &fields );
        reportOrders( aPlayer, &fields );
        reportMistakes( aPlayer, &fields );
        yourStatusForecast( aGame->planets, aPlayer, &fields );
        if ( aPlayer->flags & F_SHIPTYPEFORECAST ) {
            reportYourShipTypes( aPlayer, &fields );
        }
        if ( aPlayer->flags & F_PLANETFORECAST ) {
            yourPlanetsForecast( aGame->planets, aPlayer, &fields );
            reportProdTable( aGame->planets, aPlayer, &fields );
        }
        if ( aPlayer->flags & F_ROUTESFORECAST ) {
            reportRoutes( aGame->planets, aPlayer, &fields );
        }
        if ( aPlayer->flags & F_GROUPFORECAST ) {
            reportYourGroups( aGame->planets, aPlayer, &fields );
            reportFleets( aPlayer, &fields );
        }
    }

    if ( orders_done == 0 ) {
        orders_done = 1;
        doOrders( aGame, aPlayer, phase3orders, 3 );
    }

    ( aGame->turn )--;
}
#endif

/*************/


/****f* Process/copyOrders
 * NAME
 *   copyOrders -- copy incoming orders to file.
 * SYNOPSIS
 * void copyOrders(game *aGame, FILE *orders, char *raceName, 
 *                 char *password, int theTurnNumber)
 * FUNCTION
 *   Copy the orders to a file called 
 *     GAMEHOME/orders/<game name>/<race name>.<turn number>
 *   If and #END line is missing one is generated.
 * INPUTS
 *   orderFile      - should point to the line after the #GALAXY line.
 *   aGame 
 *   raceName
 *   password
 *   theTurnNumber
 * SOURCE
 */

void
copyOrders( game *aGame, FILE *orders, char *raceName, char *password,
	    char* final_orders, int theTurnNumber )
{
    strlist *s;
    char *copyFileName;
    FILE *copyFile;
    player *aPlayer;

    aPlayer = findElement( player, aGame->players, raceName );

    aPlayer->orders = NULL;
    copyFileName = alloc( strlen( aGame->name ) + strlen( aPlayer->name ) +
                          strlen( galaxynghome ) + strlen( "/orders//" ) +
                          20 );
    if (final_orders) {
      sprintf( copyFileName, "%s/orders/%s/%s.%d", galaxynghome,
	       aGame->name, aPlayer->name, theTurnNumber );
      unlink(copyFileName);
      sprintf( copyFileName, "%s/orders/%s/%s_final.%d", galaxynghome,
	       aGame->name, aPlayer->name, theTurnNumber );
    }
    else {
      sprintf( copyFileName, "%s/orders/%s/%s_final.%d", galaxynghome,
	       aGame->name, aPlayer->name, theTurnNumber );
      unlink(copyFileName);
      sprintf( copyFileName, "%s/orders/%s/%s.%d", galaxynghome, aGame->name,
	       aPlayer->name, theTurnNumber );
	}
	
    copyFile = Fopen( copyFileName, "w" );
    savefprintf( copyFile, "#GALAXY %s %s %s",
                 aGame->name, raceName, password );
	if (final_orders)
		savefprintf(copyFile, " FinalOrders");
	savefprintf(copyFile, "\n");
	
    getLine( orders );
    for ( ; !feof( orders ) && noCaseStrncmp( "#END", lineBuffer, 4 ); ) {
        savefprintf( copyFile, "%s", lineBuffer );
        s = makestrlist( lineBuffer );
        addList( &( aPlayer->orders ), s );
        getLine( orders );
    }
    if ( feof( orders ) )
        savefprintf( copyFile, "#END\n" );
    else
        savefprintf( copyFile, "%s\n", lineBuffer );

    free( copyFileName );
}

/*****************/


/****f* Process/areValidOrders
 * NAME
 *   areValidOrders -- check if orders are valid and load game.
 * SYNOPSIS
 *   int areValidOrders(FILE *ordersFile, 
 *                      char **command, game **game, 
 *                      char **raceName, char** final_orders,
 *                      char **password)
 * FUNCTION
 *   Scans through a file with orders until a line that starts
 *   with "#" is found.  The the rest of the line is then
 *   used to determine the name of the game, the race the orders
 *   are for, and the password for the race.
 *   The given game is loaded.  It is checked that the given
 *   player exists in this game, and that the given password
 *   is equal to the stored password.
 * INPUTS
 *   ordersFile     - pointer to a opened file with orders.
 *   theTurnNumber  - turn number the orders are supposed to be for.
 * RESULT
 *   result      - return code:
 *      RES_NO_ORDERS - no line containing "#GALAXY" was found.
 *      RES_PASSWORD  - password was incorrect.
 *      RES_PLAYER    - no such player (Race) exists.
 *      RES_NO_GAME   - no such game exists.
 *      RES_TURNRAN   - orders are for a turn that already ran.
 *      RES_OK        - everything was OK.
 *   aGame       - game Structure loaded from disk
 *   raceName  - name of the race the orders are for.
 *   password    - given password of the race.
 *
 *   ordersFile  - points to the line after the #GALAXY line.
 *   
 *   If the result is RES_OK then the game and its configuration
 *   file are loaded from disk.
 * SEE ALSO
 *   CMD_report(), CMD_check(), CMD_relay()
 * SOURCE
 */

int
areValidOrders( FILE *ordersFile, game **aGame, char **raceName,
		char **password, char** final_orders, int* theTurnNumber )
{
  int   resNumber;
  int   foundOrders;
  char* gameName;
  char* isRead;
  
  gameName = NULL;
  
  foundOrders = FALSE;
  for ( isRead = fgets( lineBuffer, LINE_BUFFER_SIZE, ordersFile );
	isRead;
	isRead = fgets( lineBuffer, LINE_BUFFER_SIZE, ordersFile ) ) {
    if ( noCaseStrncmp( "#GALAXY", lineBuffer, 7 ) == 0 ) {
      foundOrders = TRUE;
      break;
    }
  }

  *theTurnNumber = LG_CURRENT_TURN;
	
  if ( foundOrders ) {
    char* ptr;
    getstr( lineBuffer );
    gameName = strdup( getstr( NULL ) );
    *raceName = strdup( getstr( NULL ) );
    *password = strdup( getstr( NULL ) );
    if ((ptr = getstr(NULL)) != NULL) {
		*theTurnNumber = atoi(ptr);
		if (!isdigit(*ptr)) {
			*theTurnNumber = LG_CURRENT_TURN;
			if ((*aGame = loadgame(gameName, LG_CURRENT_TURN)) != NULL)
				loadNGConfig(*aGame);
			else {
				*aGame = allocStruct( game );
				
				setName( *aGame, "UnknownGame" );
				loadNGConfig( *aGame );
				if ( gameName )
					setName( *aGame, gameName );
				return RES_NO_GAME;
			}
			return RES_NO_REPORT_TURN_NBR;
		}
    }
    
    if ((ptr = getstr(NULL)) != NULL) {
		if (noCaseStrcmp(ptr, "FinalOrders") == 0)
			*final_orders = strdup(ptr);
    }
    
    if ( ( *aGame = loadgame( gameName, LG_CURRENT_TURN ) ) ) {
		player *aPlayer;
		
		loadNGConfig( *aGame );
		
		if (noCaseStrcmp("GM", *raceName) == 0) {
			if (strcmp((*aGame)->serverOptions.GMpassword, *password) == 0) {
				resNumber = RES_OK;
			}
		}
		else {
			aPlayer = findElement( player, ( *aGame )->players,
								   *raceName );
	
	if ( aPlayer ) {
	  if ( noCaseStrcmp( aPlayer->pswd, *password ) eq 0 ) {
	    if ( ( *theTurnNumber >= ( *aGame )->turn + 1 ) ||
		 ( *theTurnNumber == LG_CURRENT_TURN ) ) {
	      resNumber = RES_OK;
	    } else {
	      resNumber = RES_TURNRAN;
	    }
	  } else {
	    resNumber = RES_PASSWORD;
	  }
	} else {
	  resNumber = RES_PLAYER;
	}
      }
    } else {
      resNumber = RES_NO_GAME;
    }
  } else {
    resNumber = RES_NO_ORDERS;
  }
  
  if ( ( resNumber == RES_NO_GAME ) || ( resNumber == RES_NO_ORDERS ) ) {
    *aGame = allocStruct( game );
    
    setName( *aGame, "UnknownGame" );
    loadNGConfig( *aGame );
    if ( gameName )
      setName( *aGame, gameName );
  }
  
  return resNumber;
}

/*********/


/****f* Process/getTurnNumber
 * NAME
 *   getTurnNumber -- get the turn number
 * FUNCTION
 *   Scans an incomming email for the From: line.
 *   If it is found it looks for the word "order" or "report"
 *   It then looks if there is a number after this word.
 *   If there is this number is returned.
 *   If no From: line was found the program aborts.
 * RESULTS
 *   >= 0             -- the turn number
 *   LG_CURRENT_TURN  -- no turn number was specified
 ****/

int
getTurnNumber( FILE *orders )
{
    int theTurnNumber;
    char *isRead;

    theTurnNumber = LG_CURRENT_TURN;
    for ( isRead = fgets( lineBuffer, LINE_BUFFER_SIZE, orders );
          isRead; isRead = fgets( lineBuffer, LINE_BUFFER_SIZE, orders ) ) {
        /* WIN32 */
        if ( noCaseStrncmp( string_mail_subject, lineBuffer, 8 ) == 0 ) {
            int temp, nrRead;
            char *c;

            for ( c = lineBuffer; *c; c++ )
                *c = ( char ) tolower( *c );
            c = strstr( lineBuffer, "order" );
            if ( c == NULL ) {
                c = strstr( lineBuffer, "report" );
            }
            if ( c == NULL ) {
                c = strstr( lineBuffer, "shutdown" );
            }
            if ( c != NULL ) {
                nrRead = sscanf( c, "%*s%d", &temp );
                if ( nrRead == 1 )
                    theTurnNumber = temp;
            } else {
                assert( 0 );
            }
            break;
        }
    }
    assert( isRead != NULL );

    return theTurnNumber;
}


/****f* Process/getDestination
 * NAME
 *   getDestination -- get Destination for the message
 * FUNCTION
 *   Extract the destination of a relay message from a players
 *   email.
 * NOTE
 *   The message should contain the line
 *      Subject:   relay <race name>
 ******
 */

char *
getDestination( char *subject )
{
    int theTurnNumber;
    char* destination;
    char *c;

    theTurnNumber = LG_CURRENT_TURN;

    /*plog(LBRIEF, ">getDestination(%s)\n", subject);*/

    c = strlwr(strdup(subject));
      
    if ((destination = strstr(c, "relay")) == NULL) {
      plog(LBRIEF, "  subject does not have \"relay\" in it\n");
      return NULL;		/* can't be a relay subject */
    }
    else
      destination += 5;

    while (isspace(*destination))
      destination++;

    destination = strdup(destination);
    free (c);

    /*plog(LBRIEF, "<getDestination(%s)\n", destination);*/

    return destination;
}





/****f* Process/doOrders
 * NAME
 *   doOrders -- carry out all orders.
 * SYNOPSIS
 *   void doOrders(game *aGame, player *aPlayer, 
 *                 orderinfo *orderInfo, int phase)
 * FUNCTION
 *   Carry out all orders for one player. 
 * SOURCE
 */

void
doOrders( game *aGame, player *aPlayer, orderinfo *orderInfo, int phase )
{
    strlist *s;
    orderinfo *op;

    plog( LFULL, "doOrders: Phase %d Race %s\n", phase, aPlayer->name );

    pdebug( DFULL, "doOrders\n" );
    pdebug( DFULL2, "  Phase %d Race %s\n", phase, aPlayer->name );
    for ( s = aPlayer->orders; s; ) {
        char *order;

        pdebug( DFULL2, "  Order %s\n", s->str );
        for ( order = getstr( s->str );
              ( s ) && ( phase != 1 ) && ( *order eq '@' ); ) {
            plog( LFULL, "order: %s phase:%d\n", order, phase );
            for ( s = s->next; s; s = s->next ) {
                order = getstr( s->str );
                if ( *order eq '@' )
                    break;
            }
            if ( s ) {
                s = s->next;
                if ( s )
                    order = getstr( s->str );
            }
        }

        if ( s ) {
            order = getstr( s->str );
            for ( op = orderInfo; op->name != 0; op++ ) {
                if ( noCaseStrncmp( op->name, order, ORDER_SIZE ) == 0 ) {
                    ( *( op->func ) ) ( aGame, aPlayer, &s );
                    break;
                }
            }
        }
        if ( s )
            s = s->next;
    }
}

/***************/

/****f* Process/removeDeadPlayer 
 * NAME
 *   removeDeadPlayer -- Removes idle players 
 * FUNCTION
 *   Check if a player last sent-in orders, and if this was too
 *   long ago the player is removed from the game. 
 * SOURCE
 */

void
removeDeadPlayer( game *aGame )
{
  player *P;
  player *P3;
  int allowedOrderGap;
  int nbrPlanets;
	
  pdebug( DFULL, "removeDeadPlayer\n" );
  allowedOrderGap = ( aGame->turn < ENDPHASE1TURN ) ? ORDERGAP1 : ORDERGAP2;
  for ( P = aGame->players; P; P = P3 ) {
    P3 = P->next;
    if ( P->addr[0] ) {
      int idleTurns;
      
      idleTurns = ( P->lastorders ) ? aGame->turn - P->lastorders :
	allowedOrderGap + 1;
      plog( LFULL, "Player %s idle turns %d\n", P->name, idleTurns );
      if ( idleTurns != 0 ) {
	if ( idleTurns < allowedOrderGap ) {
	  int gap = allowedOrderGap - idleTurns;
	  sprintf( lineBuffer, "\n\
*** NOTE: You didn't send orders this turn.  You have %d more turn%s to\n\
*** remain idle before you forfeit your position.", gap, &"s"[gap == 1] );
	  
	  addList( &P->messages, makestrlist( lineBuffer ) );
	} else if ( idleTurns == allowedOrderGap ) {
	  addList( &P->messages, makestrlist( "\n\
*** WARNING: If you do not send orders for this next turn then you will\n \
*** forfeit your position in the game!  Please send orders next turn if you\n \
*** wish to continue playing." ) );
	} else if ( idleTurns > allowedOrderGap ) {
	  planet *p;

	  P->flags |= F_DEAD;
	  
	  /* fix for bug 991269 */
	  freelist(P->groups);
	  P->groups = NULL;

	  nbrPlanets = 0;
	  for ( p = aGame->planets; p; p = p->next ) {
	    if ( p->owner == P ) {
	      nbrPlanets++;
	      plog( LPART, "Resetting planet %s\n",
		    p->name );
	      p->col = 0;
	      p->producing = PR_CAP;
	      p->producingshiptype = 0;
	      p->inprogress = 0;
	      memset( p->routes, 0, sizeof( p->routes ) );
	      p->pop = 0;
	      p->ind = 0;
	      p->cap = 0;
	      p->mat = 0;
	      p->owner = NULL;
	    }
	  }
	  if (nbrPlanets) {
	    plog( LPART, "Discontinuing reports for %s\n",
		  P->name );
	    sprintf( lineBuffer,
		     "\n-*-*-*-\n%s had an unfortunate accident and was "
		     "obliterated.\n-*-*-*-\n", P->name );
	    addList( &( aGame->messages ),
		     makestrlist( lineBuffer ) );
	  }
	  /*                    }*/
	}
      }
    }
  }
}


void
cleanDeadPlayers( game *aGame )
{
    player *P;
    player *P3;
    int allowedOrderGap;
	
    pdebug( DFULL, "removeDeadPlayer\n" );
    allowedOrderGap = ( aGame->turn < ENDPHASE1TURN ) ? ORDERGAP1 : ORDERGAP2;
    for ( P = aGame->players; P; P = P3 ) {
        P3 = P->next;
        if ( P->addr[0] ) {
            int idleTurns;
			
            idleTurns = ( P->lastorders ) ? aGame->turn - P->lastorders :
                allowedOrderGap + 1;
            plog( LFULL, "Player %s idle turns %d\n", P->name, idleTurns );
            if ( idleTurns != 0 ) {
                if ( idleTurns < allowedOrderGap ) {
                    int gap = allowedOrderGap - idleTurns;
                    sprintf( lineBuffer, "\n\
*** NOTE: You didn't send orders this turn.  You have %d more turn%s to\n\
*** remain idle before you forfeit your position.", gap, &"s"[gap == 1] );

                    addList( &P->messages, makestrlist( lineBuffer ) );
                } else if ( idleTurns == allowedOrderGap ) {
                    addList( &P->messages, makestrlist( "\n\
*** WARNING: If you do not send orders for this next turn then you will\n\
*** forfeit your position in the game!  Please send orders next turn if you\n\
*** wish to continue playing." ) );
                } else if (idleTurns > allowedOrderGap) {
                    planet *p;

                    plog( LPART, "Discontinuing reports for %s\n", P->name );
                    P->flags |= F_DEAD;

/*                    if ( aGame->turn < ENDPHASE1TURN ) {*/
                        P->groups = NULL;
                        sprintf( lineBuffer,
                                 "\n-*-*-*-\n%s had an unfortunate accident and was "
                                 "obliterated.\n-*-*-*-\n", P->name );
                        addList( &( aGame->messages ),
                                 makestrlist( lineBuffer ) );

                        for ( p = aGame->planets; p; p = p->next ) {
                            if ( p->owner eq P ) {
                                plog( LPART, "Resetting planet %s\n",
                                      p->name );
                                p->col = 0;
                                p->producing = PR_CAP;
                                p->producingshiptype = 0;
                                p->inprogress = 0;
                                memset( p->routes, 0, sizeof( p->routes ) );
                                p->pop = 0;
                                p->ind = 0;
                                p->cap = 0;
                                p->mat = 0;
                                p->owner = NULL;
                            }
                        }
/*                    }*/
                }
            }
        }
    }
}

/***********/


/****f* Process/preComputeGroupData
 * NAME
 *   preComputeGroupData -- precompute some frequently used values.
 * FUNCTION
 *   This is a very nasty function that precomputes some properties of
 *   groups.  This speeds up turn processing but also creates some
 *   serious problems when the function is not called at the right
 *   time.  A very nice case of premature optimization.
 * SOURCE
 */

void
preComputeGroupData( game *aGame )
{
    player *aPlayer;
    group *aGroup;

    for ( aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next ) {
        for ( aGroup = aPlayer->groups; aGroup; aGroup = aGroup->next ) {
            aGroup->attack = groupAttack( aGroup );
            aGroup->defense = groupDefense( aGroup );
            aGroup->location = groupLocation( aGame, aGroup );
        }
    }
}

/**********/


/****f* Process/generateErrorMessage 
 * NAME
 *   generateErrorMessage -- create error message for faulty orders.
 * FUNCTION
 *   
 * INPUTS
 *    resNumber -- The kind of error.
 *    forecast  -- file to write the message to.
 * SOURCE
 */

void generateErrorMessage( int resNumber, game *aGame,
			   char *raceName, int theTurnNumber,
			   FILE *forecast ) {
  switch ( resNumber ) {
  case RES_NO_ORDERS:
    fprintf(forecast, "O Wise Leader, your mail did not contain any "
	    "orders.\nRemember orders start with\n\n"
	    "#GALAXY GameName RaceName Password TurnNumber "
	    "[FinalOrders]\n\nand end with\n\n#END\n" );
    break;
    
  case RES_ERR_GALAXY:
    fprintf(forecast, "O Wise Leader, you must supply your race name "
	    "and galaxy name.\n Remember orders start with,\n\n"
	    "#GALAXY GameName RaceName Password TurnNumber "
	    "[FinalOrders]\n\nand end with\n\n#END\n");
    break;
    
  case RES_NO_GAME:
    fprintf(forecast, "O Wise Leader, there is no galaxy called %s.\n"
	    "This probably means that you mispelled the galaxy name "
	    "in your orders\n", aGame->name );
    break;
    
  case RES_PASSWORD:
    fprintf(forecast, "O Wise Leader, the password you gave is "
	    "incorrect.\n" );
    break;
    
  case RES_PLAYER:
    fprintf(forecast, "O Wise Leader there is no race called %s.\n"
	    "This probably means that you mispelled your "
	    "race name.\n", raceName );
    break;
    
  case RES_TURNRAN:
    fprintf(forecast, "O Wise Leader, you sent in orders for turn %d "
	    "but that turn already ran.\nThe next turn is %d.",
	    theTurnNumber, aGame->turn+1);
    break;
    
  case RES_DESTINATION:
    fprintf(forecast, "O Wise Leader, the recipient of the message "
	    "you sent does not exist.\n" );
    break;
    
  case RES_NODESTINATION:
    fprintf(forecast, "O Wise Leader, you failed to give a "
	    "destination for your message.\n" );
    break;
    
  case RES_NO_ORDERS_TURN_NBR:
    fprintf(forecast, "O Wise Leader, you didn't specify a turn "
	    "number.\nRemember that orders start with\n\n"
	    "#GALAXY GameName RaceName Password TurnNumber "
	    "[FinalOrders]\n\n"
	    "and end with\n\n#END\n");
    break;

    case RES_NO_REPORT_TURN_NBR:
    fprintf(forecast, "O Wise Leader, you didn't specify a turn "
	    "number.\nRemember that report requests start with\n\n"
	    "#GALAXY GameName RaceName Password TurnNumber\n\n"
	    "and end with\n\n#END\n");
    break;
  }
  fprintf( forecast,
	   "\nYour orders have been discarded!\n"
	   "Please correct the mistake and retransmit your orders.\n" );
}

/***********/
