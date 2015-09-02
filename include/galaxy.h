#ifndef GNG_GALAXY_H
#define GNG_GALAXY_H

/* 
 * 
 * GalaxyNG  -  UNIX Version 
 *
 * Created by
 * o Frans Slothouber (rfsber #at# xs4all.nl),
 * o Christophe  Barbier,
 * o Jacco van Weert,
 * o Tommy Lindqvist,
 * o Rogerio Fung.
 * 
 * This code contains parts of the the orginal Galaxy code which was
 * created by Russell Wallace (RWALLACE@vax1.tcd.ie), and updated by
 * the Galaxy PBeM Development Group which include: 
 * o Russell Wallace (RWALLACE@vax1.tcd.ie) 
 * o Tim Myers (tmyers@unlinfo.unl.edu), 
 * o Robert Stone (stone@athena.cs.uga.edu), 
 * o Mayan Moudgill (moudgill@cs.cornell.edu), 
 * o Graeme Griffiths (graeme@abekrd.co.uk), 
 * o K Pankhurst (k.pankhurst@ic.ac.uk).
 * 
 */

/****h* GalaxyNG/GameElements
 * FUNCTION
 *   This module has data types for the core elements of the game
 *   and functions to manipulate them.
 *****
 * Will move to another file later on
 */

/****h* GalaxyNG/Configuration
 * FUNCTION
 *   This module has data types for the game configuration.
 *   (As specified in the galaxyng.rc file).
 *****
 * Will move to another file later on
 */

#include "strutl.h"
#include "cookies.h"
#include "logging.h"

#define _GNU_SOURCE
#include <stdio.h>

#define   NAMESIZE     21       /* Maximum length of a name */
#define   MAXPOP       1000.0

/* How many people are turned into one colonist */
#define   POPPERCOL    8

/* Population increase per turn */
#define   POPINC       0.08

/* Number of industry points needed to produce one unit of ship */
#define   INDPERSHIP   10

/* Number of industry points needed to produce one unit of CAP */
#define   INDPERCAP    5

/* Minimum amount of cargo that has to be uploaded or downloaded */
#define   AMOUNTMIN    0.01

#define   BATTLEMAGIC  3.107232506

#define   DRIVEMAGIC   20.0

/* Turn at which the first phase ends. During the first phase there is a
 * strict policy for idle players. */
#define   ENDPHASE1TURN 12

/* the number of turns one is allowed to miss during the first phase */
#define   ORDERGAP1     2

/* the number of turns one is allowed to miss during phase 2 */
#define   ORDERGAP2     5

/* CB-19980922. At least TURNS_LEFT from arrival to allow recall a
 * group/fleet i */
#define   TURNS_LEFT    4
#define   MAPWIDTH      80
#define   MAPHEIGHT     40
#define   TRUE          1
#define   FALSE         0
#define   LINE_BUFFER_SIZE 1024
#define   eq           ==

#define   Report        0
#define   Forecast      1

/* May be this could help turns viewer */
#define   MACHINEREPORT_VERSION   0.1
#define   XMLREPORT_VERSION       1.5
#define   GNG_MAJOR               6
#define   GNG_MINOR               4
#define   GNG_RELEASE             8
#define   GNG_DATE                "January 2005"

#define   getLine(f) fgets(lineBuffer, LINE_BUFFER_SIZE, f)

void usage(void);


/****d* GameElements/ProductTypes
 * NAME
 *   ProductTypes -- things a planet can produce.
 * SOURCE
 */

enum {
  PR_CAP,
  PR_MAT,
  PR_SHIP,
  PR_DRIVE,
  PR_WEAPONS,
  PR_SHIELDS,
  PR_CARGO
};

/*********/

/****d* GameElements/CargoTypes
 * NAME
 *   CargoTypes
 * SOURCE
 */

enum {
  CG_CAP,
  CG_MAT,
  CG_COL,
  CG_EMPTY,
  MAXCARGO
};

/*****/

/****d* GalaxyNG/ErrorTypes
 * NAME
 *   ErrorTypes
 * SOURCE
 */

enum error_type {
  INFO,
  WARNING,
  ERROR
};

/*******/

typedef struct mapdimensions {
  double          x1, y1;
  double          x2, y2;
} mapdimensions;


struct player;
typedef struct player player;


/****s* GameElements/fleetname
 * NAME
 *   fleetname -- name of a fleet and its speed.
 * PURPOSE
 *   Structure to keep track of all fleet names of a race.
 *   Can be an element in a standard list.
 * NOTES
 *   fleetspeed is computed in the fleetphase(), it is not stored
 *   in the turn file. So fleetspeed is incorrect for most of the 
 *   time.
 * SOURCE
 */

typedef struct fleetname {
  /* basic list structure */
  struct fleetname *next;
  long            cookie;
  char           *name;
  /* end basic list */
  double          fleetspeed;
} fleetname;

/*********/


/****s* GameElements/shiptype
 * NAME 
 *   shiptype -- design of a ship.
 * PURPOSE
 *   Structure to keep track of all shiptypes of a race.
 *   Can be an element in a standard list.
 * SOURCE
 */

typedef struct shiptype {
  /* basic list structure */
  struct shiptype *next;
  long            cookie;
  char           *name;
  /* end basic list */
  double          drive;
  int             attacks;
  double          weapons;
  double          shields;
  double          cargo;
  int             flag;
} shiptype;

/**************/


/****d* GameElements/PlanetFlags 
 * NAME
 *   PlanetFlags --
 * FUNCTION
 *   Planet was visable the previous turn,
 *   used by forecast functions to prevent people
 *   from looking into the future. That is examine
 *   planets they will visit next turn before the next
 *   turn runs. 
 * SOURCE
 */

#define PL_VISPREVTURN 1

/*******/


/****s* GameElements/planet
 * NAME
 *   planet --
 * PURPOSE
 *   Store information about planets.
 * ATTRIBUTES
 *   inprogress -- Production spent in the last turns, in case 
 *                 a ship takes several turns to produce.
 *   spent      -- production spend on upgrading ships.
 * SOURCE
 */

typedef struct planet {
  /* basic list structure */
  struct planet  *next;
  long            cookie;
  char           *name;
  /* end basic list */
  player         *owner;
  double          x, y;
  double          size;
  double          resources;
  double          pop;
  double          ind;
  int             producing;
  shiptype       *producingshiptype;
  double          cap;
  double          mat;
  double          col;
  double          inprogress;   /* Production spent in the last turns */
  double          spent;
  struct planet  *routes[MAXCARGO];
  long            flags;
} planet;

/*********/


/****d** GameElements/GroupFlags
 * NAME
 *   GroupFlags
 * SOURCE
 */

#define GF_INTERCEPT  1         /* group got intercept order */
#define GF_INBATTLE1  2         /* group took part in a battle in phase 1 */
#define GF_INBATTLE2  4         /* group took part in a battle in phase 2 */

/*******/

/****s* GameElements/group
 * NAME
 *   group -- group structure
 * FUNCTION
 *   Structure to keep track of groups.
 * ATTRIBUTES
 *   next
 *   cookie
 *   name      - unused.
 *   number    - group number, used as a name.
 *   type      - ship type of the ships in the group.
 *   drive, weapons, shields, cargo  - techlevels of the group.
 *   loadtype  - type of cargo loaded.
 *   load      - the total amount of cargo loaded.
 *   from      - planet of origin.
 *   where     - destination planet.
 *               To see how these are used see groupLocation()
 *   dist      - distance left to destination planet.
 *   ships     - quantity of ships (before a battle if any)
 *   left      - quantity of ships surviving a battle 
 *   flags     - see GroupFlags
 *   thefleet  - the fleet the group is part of, if any
 *   attack    - cached for speed
 *   defense   - cached for speed
 *   location  - cached for speed 
 *   canshoot  - array with one element per ship that indicates
 *               if the ship can fire or not.
 *   alive     - array with one element per ship that indicates
 *               if the ship is still alive of not.
 *   numberOfAttackers  --
 *   numberOfAttackersLeft  --
 * SOURCE
 */

typedef struct group {
  struct group   *next;
  unsigned long   cookie;
  char           *name;
  int             number;
  shiptype       *type;
  double          drive;
  double          weapons;
  double          shields;
  double          cargo;
  int             loadtype;
  double          load;
  planet         *from;
  planet         *where;
  double          dist;
  int             ships;
  int             left;
  long            flags;
  fleetname      *thefleet;
  double          attack;
  double          defense;
  int            *canshoot;
  int            *alive;
  int             numberOfAttackers;
  int             numberOfAttackersLeft;
  planet         *location;
} group;

/************/


/****s* GalaxyNG/alliance
 * NAME
 *   alliance
 * FUNCTION
 *   Structure to keep track who are part of an alliance.
 *   Can be an element in a standard list.
 * SOURCE
 */

typedef struct alliance {
  struct alliance *next;
  long            cookie;
  char           *name;
  player         *who;
} alliance;

/**************/


/****s* Battle/participant
 * NAME
 *   participant -- participant in a battle.
 * PURPOSE
 *   Structure to keep track of who took part in a battle.
 *   Can be an element in a standard list.
 * SOURCE
 */

typedef struct participant {
  struct participant *next;
  long            cookie;
  char           *name;
  player         *who;
  group          *groups;
} participant;

/***********/


/****s* Battle/shot
 * NAME
 *   shot
 * PURPOSE
 *   Information about a single shot.
 * SOURCE
 */

typedef struct shot {
  player         *attacker;
  shiptype       *atype;
  player         *target;
  shiptype       *ttype;
  int             result;
} shot;

/*********/


/****s* Battle/bprotocol
 * NAME
 *   bprotocol -- 
 * PURPOSE
 *   Keep track of who fired on who (shot).
 * SOURCE
 */

typedef struct bprotocol {
  long            size;         /* maximum size */
  long            cur;          /* current size */
  shot           *shots;        /* Array of shots, grows when needed */
} bprotocol;

/*******/



/****s* Battle/battle
 * NAME
 *   battle -- battle structure
 * PURPOSE
 *   Structure to keep track of all battles that took place.
 *   Can be an element in a standard list. 
 * SOURCE
 */

typedef struct battle {
  struct battle  *next;
  long            cookie;
  char           *name;
  planet         *where;
  participant    *participants;
  bprotocol      *protocol;
  int             phase;        /* Either GF_INBATTLE1 or GF_INBATTLE2 */
} battle;

/**************/



/****s* Battle/bombing
 * NAME
 *   bombing -- bombing structure
 * PURPOSE
 *   Structure to keep track of which planets were bombed.
 *   Can be an element in a standard list.
 * SOURCE
 */

typedef struct bombing {
  struct bombing *next;
  long            cookie;
  char           *name;
  planet         *where;
  player         *owner;
  double          pop;
  double          ind;
  int             producing;
  shiptype       *producingshiptype;
  double          cap;
  double          mat;
  double          col;
  player         *who;
  alliance       *viewers;
} bombing;

/*************/



/****s* GameElements/planet_claim
 * NAME
 *   planet_claim
 * PURPOSE
 *   Structure to keep track which planets a race claims.
 * SOURCE
 */

typedef struct planet_claim {
  struct planet_claim *next;
  long            cookie;
  char           *name;
  struct planet  *planet_claimed;
} planet_claim;

/******/


/****s* GameElements/option
 * NAME
 *   option -- an option a player can switch on or off
 * PURPOSE
 *   optionName -- the name of the option as used in the O order
 *   optionMask -- 
 */

typedef struct option {
  char           *optionName;
  long            optionMask;
} option;

/******/


/****s* GameElements/player
 * NAME
 *   player --
 * PURPOSE
 *   Stores information about a race and the person that plays it.
 * BUGS
 *   This should be called race!
 * ATTRIBUTES
 *    unused3, unused4, unused5  
 *                   -- For future expansion.           
 *    masslost       -- Total amount of ship mass lost since turn 0.
 *    massproduced   -- Total amount of ship mass produces since turn 0.
 *    rating         -- Used for the high score list. 
 *    team           -- for which team does the player play.
 *    realName;      -- For in the Hall of Fame 
 * SOURCE
 */

struct player {
  player         *next;
  long            cookie;
  char           *name;
  char           *addr;
  char           *pswd;
  int             pswdstate;
  double          drive;
  double          weapons;
  double          shields;
  double          cargo;
  fleetname      *fleetnames;
  shiptype       *shiptypes;
  alliance       *allies;
  group          *groups;
  double          mx, my, msize;
  char           *realName;     /* For in the Hall of Fame */
  int             team;
  int             unused3;      /* For future expansion */
  int             unused4;      /* For future expansion */
  int             unused5;      /* For future expansion */
  double          masslost;
  double          massproduced;
  int             lastorders;
  long            flags;
  strlist        *orders;
  strlist        *messages;
  strlist        *mistakes;
  planet_claim   *claimed_planets;
  double          totPop;       /* Cache */
  double          totInd;       /* Cache */
  double          totCap;       /* Cache */
  double          totMat;       /* Cache */
  double          totCol;       /* Cache */
  int             numberOfPlanets;      /* Cache */
  int             rating;       /* Used for the high score list */
};

/********/



/****d* GameElements/PlayerFlags 
 * NAME
 *   PlayerFlags
 * SOURCE
 */

/* Player want to stay anonymous */
#define F_ANONYMOUS          1
#define F_AUTOUNLOAD         2
#define F_PRODTABLE          4
#define F_SORTGROUPS         8
#define F_GROUPFORECAST     16
#define F_PLANETFORECAST    32
#define F_SHIPTYPEFORECAST  64
#define F_ROUTESFORECAST   128
/* used by the rateRace function */
#define F_SORTED           256
/* Compress turn reports before sending then */
#define F_COMPRESS         512
#define F_GPLUS           1024
/* Players ask for a "machine" turn report 19980620 */
#define F_MACHINEREPORT   2048
/* Include a battle protocol */
#define F_BATTLEPROTOCOL  4096
/* Make every /n a cr lf (not used, can be reused) */
#define F_CRLF            8192
/* Players ask for an xml turn report 19990611 */
#define F_XMLREPORT      16384
/* player is no longer active */
#define F_DEAD           32768
/* is a text report wanted? */
#define F_TXTREPORT      65536

/****************/


/****s* Configuration/server [CB 20010425]
 * NAME
 *   server -- NG server options structure
 * PURPOSE
 *   All server options in one place.
 *   Usually feed with .galaxyngrc file
 * SOURCE
 */

typedef struct server {
  char* sendmail;		/* the command to email a file */
  char* GMemail;		/* GM reports go to this address */
  char* GMname;			/* name the email should appear to be from */
  char* GMpassword;		/* GM password, for the relay command */
  char* SERVERemail;		/* Server email */
  char* SERVERname;		/* name the email should appear to be from */
  char* ReplyTo;		/* Server replyto */
  char* compress;		/* the command to compress the body of
				   an email */ 
  char* encode;			/* the command to encode the compressed body */
  char* fontpath;		/* the directory where the fonts are located */
  char* due;			/* this is used for the -due command */
  char* tick_interval;		/* this is used for the -immediate command */
} server;

/*******/

/****s* Configuration/gameOpt [CB 20010425]
 * NAME
 *   gameOpt -- NG game options structure
 * PURPOSE
 *   All game options in one place.
 *   Usualy feed with *.glx file
 * SOURCE
 */

typedef struct gameOpt {
	 long   gameOptions;
	 int	galactic_peace;
	 double initial_drive;
	 double initial_weapons;
	 double initial_shields;
	 double initial_cargo;
} gameOpt;

/*******/

/****s* GameElements/game
 * NAME
 *   game -- game structure
 * PURPOSE
 *   Entry point to all data for one game.
 * SOURCE
 */

typedef struct game {
  player         *next;
  long            cookie;
  char           *name;
  server          serverOptions;
  gameOpt         gameOptions;
  /* the time at which a turn is started, used for sanity check. */
  char           *starttime;
  int             turn;
  double          galaxysize;   /* CB-20010408 */
  player         *players;      /* list with races */
  planet         *planets;      /* list with planets */
  battle         *battles;      /* list with battles */
  bombing        *bombings;     /* list with bombings */
  strlist        *messages;     /* list with messages */
} game;

/*******/

typedef struct emailList {
  /* basic list structure */
  struct emailList* next;
  long              cookie;
  char*             name;
  /* end basic list */
  char*             addr;
  char*             pswd;
} emailList;


/****d* GameElements/GameFlags
 * NAME
 *   GameFlags
 * SOURCE
 */

/* Game Options Flags */
#define GAME_NONGBOMBING    1
/* planet is bombed completely and does not change owners */
#define GAME_KEEPPRODUCTION 2
/* production already spent is not lost after a switch of production type */
#define GAME_CREATECIRCLE   4
/* Create systems along the edge of a circle. */
#define GAME_NODROP         8
/* Do not drop idle players */
#define GAME_SAVECOPY      16
/* Save a copy of the turn report in /report when running a turn */
#define GAME_SPHERICALGALAXY 32
/* Galaxy is mapped on a sphere CB-20010401 */

/**********/



extern char  map[MAPWIDTH][MAPHEIGHT];
extern char  buf[LINE_BUFFER_SIZE];
extern char  lineBuffer[2 * LINE_BUFFER_SIZE];
extern char* galaxynghome;
extern char* tempdir;
extern char* productname[];
extern char* string_mail_to;
extern char* string_mail_subject;
extern char* string_mail_from;

/* used in loadgame.c, should be a parameter */
extern FILE    *turnFile;
extern struct option options[];

extern char    *vcreate;
extern char    vcid[];
extern char    *vprocess;
extern char    *vphase;
extern char    *vreport;
extern char    *vbattle;
extern char    *vsavegame;
extern char    *vloadgame;
extern int      nbrProducts;

void loadNGConfig(game* aGame);
int spcLockFile(const char* lfpath);

#endif                          /* GNG_GALAXY_H */
