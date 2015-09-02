#ifndef GNG_PROCESS_H
#define GNG_PROCESS_H

#include "galaxy.h"
#include "list.h"
#include "util.h"
#include "savegame.h"
#include "loadgame.h"
#include "phase.h"
#include "battle.h"
#include "report.h"
#include "selftest.h"

#ifdef WIN32
#include <stdarg.h>
#include <stdio.h>
/* #include <varargs.h> */
#endif

/* $Id: process.h,v 1.13 2004/10/24 15:01:30 madcoder42 Exp $ */


/****s* GalaxyNG/orderinfo
 * NAME
 *   orderinfo
 * PURPOSE
 *   map a order name to its function
 * SOURCE
 */

typedef struct _orderinfo orderinfo;

struct _orderinfo {
  char           *name;
  void            (*func) (game *aGame, player *, strlist **);
};

/******/

/****d* GalaxyNG/RESCODES
 * NAME
 *   RESCODES -- Error Codes for the orders checker function 
 * SOURCE
 */

enum ResCodes {
	RES_OK,						/* no error */
	RES_NO_ORDERS,				/* missing orders */
	RES_ERR_GALAXY,				/* no #GALAXY line */
	RES_NO_GAME,				/* can't find the game name */
	RES_PASSWORD,				/* password mismatch */
	RES_PLAYER,					/* player name doesn't exist */
	RES_TURNRAN,				/* orders sent in for a turnt that ran */
	RES_DESTINATION,			/* can't find the race sending email to */
	RES_NODESTINATION,
	RES_NO_ORDERS_TURN_NBR,		/* missing turn number in orders */
	RES_NO_REPORT_TURN_NBR,		/* missing turn number in a report request */
	RES_DEAD_PLAYER				/* tried to relay to a dead player */
};

/**********/


#define ORDER_SIZE 1

/* order functions */

void            at_order(game *aGame, player *, strlist **);
void            eq_order(game *aGame, player *, strlist **);    /* FS
                                                                 * 
                                                                 * * * *
                                                                 * * * *
                                                                 * * * * * 
                                                                 * * * * * 
                                                                 * * * * * 
                                                                 * *
                                                                 * 12/1999 
                                                                 * * * * * 
                                                                 * * */
void            a_order(game *aGame, player *, strlist **);
void            b_order(game *aGame, player *, strlist **);
void            c_order(game *aGame, player *, strlist **);
void            d_order(game *aGame, player *, strlist **);
void            e_order(game *aGame, player *, strlist **);
void            f_order(game *aGame, player *, strlist **);
void            g_order(game *aGame, player *, strlist **);
void            h_order(game *aGame, player *, strlist **);     /* CB-1990923 
                                                                 * 
                                                                 * * * *
                                                                 * * * *
                                                                 * * * * * 
                                                                 * * * *
                                                                 * * */
void            i_order(game *aGame, player *, strlist **);
void            j_order(game *aGame, player *, strlist **);

/* free k */
void            l_order(game *aGame, player *, strlist **);
void            m_order(game *aGame, player *, strlist **);
void            n_order(game *aGame, player *, strlist **);
void            o_order(game *aGame, player *, strlist **);
void            p_order(game *aGame, player *, strlist **);
void            q_order(game *aGame, player *, strlist **);
void            r_order(game *aGame, player *, strlist **);
void            s_order(game *aGame, player *, strlist **);
void            t_order(game *aGame, player *, strlist **);
void            u_order(game *aGame, player *, strlist **);
void            v_order(game *aGame, player *, strlist **);
void            w_order(game *aGame, player *, strlist **);
void            x_order(game *aGame, player *, strlist **);
void            y_order(game *aGame, player *, strlist **);
void            z_order(game *aGame, player *, strlist **);
void            mistake(player *P, enum error_type, strlist *s, char *format, ...);

void
                copyOrders(game *aGame, FILE * orders,
                           char *raceName, char *password,
                           char* final_orders, int theTurnNumber);

int
                areValidOrders(FILE * ordersFile,
                               game **aGame,
                               char **raceName, char **password,
                               char** final_orders, int* theTurnNumber);

void
                preComputeGroupData(game *aGame);

void
                removeDeadPlayer(game *aGame);

int
                getTurnNumber(FILE * orders);

char           *getDestination(char* subject_line);

void
                doOrders(game *aGame,
                         player *aPlayer, orderinfo *orderInfo, int phase);

int
                runTurn(game *aGame, char *ordersFileName);

#if FS_NEW_FORECAST
void            checkOrders(game *aGame, char *raceName );
#else
void
                checkOrders(game *aGame,
                            char *raceName, FILE * forecast, int kind);
#endif

void
                generateErrorMessage(int resNumber,
                                     game *aGame,
                                     char *raceName,
                                     int theTurnNumber, FILE * forecast);

#endif                          /* GNG_PROCESS_H */
