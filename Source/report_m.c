#include "create.h"
#include "report_m.h"
#include <string.h>
#include "list.h"
#include "report.h"

FILE           *Mreport;

/****h* GalaxyNG/MReport
 * NAME
 *   MReport -- machine report
 * PURPOSE
 *   Contains functions that create a turn report in a format
 *   that is more easy to understand for machines.
 * AUTHOR
 *   Christophe  Barbier
 * HISTORY
 *   199809: Creation of a machine report
 *   199903: 
 *     Corrected a bug in number of uninhabited planets Replaced
 *     semi-colon delimitor with space (native separator in tcl/tk)
 *     Section header : 
 *       Added a space before the "greater" sign, thus last record is
 *       no longer "polluted".  * Removed useless key words
 *       Try to get a standard form for header of each section :
 *       <section-name(only one word) name(if any) number-of-elements >
 *       Try to get a (poor) standard form in groups record : each
 *       alien group or group in a battle begin with a 0 (for his
 *       number)
 *       Section format (groups, battles...) is now :
 *       <"section name" "battle or location name" quantity >
 *       Messages section is now :
 *       <messages "global or personal" quantity >
 *   199904: 
 *     added report version, and galaxy size.  Groups record in battle
 *     now almost the same that in "normal situations"
 *   199911: 
 *       FS, updated for the use of game structure.
 *   200104: 
 *       CB, updated to add the game options section.
 * TODO  
 *   write a doc :).
 ******
 */


/****f* MReport/report_m
 * NAME
 *   report_m -- create machine report.
 * SYNOPSIS
 *   void report_m(player *P, game *aGame, FILE *reportFile)
 * SEE ALSO
 *   report()
 * SOURCE
 */

void
report_m(player *P, game *aGame, FILE * reportFile)
{
  Mreport = reportFile;

  rHE_M(aGame, P);              /* Writes header of report file */
  rMR_M();                      /* Machine report version */
  rGZ_M(aGame);                 /* Size of the galaxy */
  rMM_M(aGame, P, aGame->messages, "global");   /* Global messages */
  rMM_M(aGame, P, P->messages, "personnal");    /* Personnal messages */
  rGO_M(aGame);                 /* Game options, CB-20010401 ; see
                                 * galaxy.h */
  rOP_M(aGame, P);              /* Options */
  rOD_M(aGame, P);              /* Orders */
  rMK_M(aGame, P);              /* Mistakes in orders */
  rST_M(aGame, P);              /* Players status */
  rSH_M(aGame, P);              /* Ships types */
  rBT_M(aGame, P);              /* Battles */
  rBB_M(aGame, P);              /* Bombing */
  rMP_M(aGame, P);              /* Text map */
  rIC_M(aGame, P);              /* Incomming groups */
  rAP_M(aGame, P);              /* All planets */
  rPT_M(aGame, P);              /* Production table */
  rRT_M(aGame, P);              /* Routes */
  rGG_M(aGame, P);              /* Player's groups */
  rGS_M(aGame, P);              /* Others players groups */
  rFL_M(aGame, P);              /* Fleets */
  rEN_M();                      /* Writes last line of machine report file 
                                 *
                                 * * *  * *  * *  * *  */
}

/*****/


/****f* MReport/rHE_M
 * NAME
 *   rHE_M --
 * FUNCTION
 *   Write header of report file 
 ******
 */

void
rHE_M(game *aGame, player *P)
{
  fprintf(Mreport, "<report GAME %s TURN %i NATION %s",
          aGame->name, aGame->turn, P->name);
  if (P->pswdstate != 2) {
    fprintf(Mreport, " PASSWORD %s ", P->pswd);
  }
  fprintf(Mreport, ">\n");
}

/****f* GalaxyNG/rMR_M
 * NAME 
 *   rMR_M --
 * FUNCTION
 *   Write the machine report version May be it can help turns viewers
 *   creators, when changes in machine report break the compatibility
 *   with previous versions
 *****
 */

void
rMR_M(void)
{
  fprintf(Mreport, "<machinereport-version %5.2f >\n",
          MACHINEREPORT_VERSION);
  fprintf(Mreport, "</machinereport-version>\n");
}

/* Write the size of galaxy */
void
rGZ_M(game *aGame)
{
  fprintf(Mreport, "<galaxy-size %8.2f >\n", aGame->galaxysize);
  fprintf(Mreport, "</galaxy-size>\n");
}


/****f* MReport/rMM_M
 * NAME
 *   rMM_M --
 * FUNCTION
 *   report global or personnal messages.
 *****
 */

void
rMM_M(game *aGame, player *P, strlist *messages, char *c)
{
  strlist        *s;
  int             nb_mes = 0;

  s = messages;
  nb_mes = numberOfElements(s); /* Sorry, only the number of lines * * * *
                                 * * is * * * counted. */

  s = messages;
  if (messages) {
    fprintf(Mreport, "<messages %s %i >\n", c, nb_mes);
    while (s) {
      fprintf(Mreport, "%s\n", s->str);
      s = s->next;
    }
    fprintf(Mreport, "</messages>\n");
  }
}

/****f* MReport/rGO_M
 * NAME
 *   rGO_M --
 * FUNCTION
 *   game options
 *****
 */

void
rGO_M(game *aGame)
{
  int             nb_opt = 0;

  nb_opt = 2;                   /* OK, I know, it's ugly, but there is
                                 * currently only 2 options ;) */

  fprintf(Mreport, "<game-options %i >\n", nb_opt);     /* CB-20010401 ;
                                                         * see galaxy.h */
  if (aGame->gameOptions.gameOptions & GAME_NONGBOMBING) {
    fprintf(Mreport, "fullbombing %s\n", "ON");
  }
  else {
    fprintf(Mreport, "fullbombing %s\n", "OFF");
  }
  if (aGame->gameOptions.gameOptions & GAME_KEEPPRODUCTION) {
    fprintf(Mreport, "keepproduction %s\n", "ON");
  }
  else {
    fprintf(Mreport, "keepproduction %s\n", "OFF");
  }
  if (aGame->gameOptions.gameOptions & GAME_NODROP) {
    fprintf(Mreport, "dontdropdead %s\n", "OFF");
  }
  else {
    fprintf(Mreport, "dontdropdead %s\n", "ON");
  }
  if (aGame->gameOptions.gameOptions & GAME_SPHERICALGALAXY) {
    fprintf(Mreport, "sphericalsalaxy %s\n", "ON");
  }
  else {
    fprintf(Mreport, "sphericalsalaxy %s\n", "OFF");
  }
  fprintf(Mreport, "initialtechlevels %.2f %.2f %.2f %.2f\n",
          aGame->gameOptions.initial_drive,
          aGame->gameOptions.initial_weapons,
          aGame->gameOptions.initial_shields,
          aGame->gameOptions.initial_cargo);
  fprintf(Mreport, "</game-options>\n");
}

/****f* MReport/rOP_M
 * NAME
 *   rOP_M --
 * FUNCTION
 *   report options
 *****
 */

void
rOP_M(game *aGame, player *P)
{
  option         *curOption;

  int             nb_opt = 0;

  curOption = options;
  while (curOption->optionName) {
    nb_opt++;
    curOption++;
  }

  fprintf(Mreport, "<options %i >\n", nb_opt);

  for (curOption = options; curOption->optionName; curOption++) {
    fprintf(Mreport, "%s ", curOption->optionName);
    if (P->flags & curOption->optionMask)
      fprintf(Mreport, "%s\n", "ON");
    else
      fprintf(Mreport, "%s\n", "OFF");
  }

  fprintf(Mreport, "</options>\n");
}

/****f* MReport/rOD_M
 * NAME
 *   rOD_M --
 * FUNCTION
 *   report orders 
 *****
 */

void
rOD_M(game *aGame, player *P)
{
  strlist        *s;
  int             nb_ord = 0;

  s = P->orders;
  nb_ord = numberOfElements(s);

  fprintf(Mreport, "<orders %i >\n", nb_ord);   /* number of lines * * * *
                                                 * * in fact * * * * */
  if (P->orders) {

    for (s = P->orders; s; s = s->next) {
      fprintf(Mreport, "%s\n", s->str);
    }
  }
  fprintf(Mreport, "</orders>\n");
}


/****f* MReport/rMK_M
 * NAME
 *   rMK_M --
 * FUNCION
 *   report mistakes 
 *****
 */

void
rMK_M(game *aGame, player *P)
{
  strlist        *s;
  int             nb_mis = 0;

  s = P->mistakes;
  nb_mis = numberOfElements(s); /* number of lines in fact */

  fprintf(Mreport, "<mistakes %i >\n", nb_mis);
  for (s = P->mistakes; s; s = s->next) {
    fprintf(Mreport, "%s\n", s->str);
  }
  fprintf(Mreport, "</mistakes>\n");

}

/****f* MReport/rST_M
 * NAME
 *   rST_M --
 * FUNCTION
 *   report status of players 
 *****
 */

void
rST_M(game *aGame, player *P)
{
  player         *P2;
  int             nb_pla = 0;

  P2 = aGame->players;
  nb_pla = numberOfElements(P2);

  fprintf(Mreport, "<players %i >\n", nb_pla);
  for (P2 = aGame->players; P2; P2 = P2->next) {
    fprintf(Mreport,
            "%s %.2f %.2f %.2f %.2f %.2f %.2f %i",
            P2->name, P2->drive, P2->weapons, P2->shields, P2->cargo,
            P2->totPop, P2->totInd, P2->numberOfPlanets);

    if (P2 != P)
      fprintf(Mreport, " %s", (atwar(P, P2) ? "War" : "Peace"));
    else
      fprintf(Mreport, " -");

    if (P2->addr[0]) {
      if (P2->flags & F_ANONYMOUS) {
        fprintf(Mreport, " anonymous\n");
      }
      else {
        fprintf(Mreport, " %s\n", P2->addr);
      }
    }
    else
      fprintf(Mreport, " no-address\n");
  }
  fprintf(Mreport, "</players>\n");

}


/****f* MReport/rSH_M
 * NAME
 *   rSH_M --
 * FUNCTION
 *   Report ship types. 
 *****
 */

void
rSH_M(game *aGame, player *P)
{
  player         *P2;
  shiptype       *t;

  int             nbr_typ = 0;

  for (P2 = aGame->players; P2; P2 = P2->next) {
    if ((P2 eq P) || visibleShipTypes(aGame, P2, P)) {
      for (t = P2->shiptypes; t; t = t->next) {
        if ((P2 eq P) || (t->flag))
          nbr_typ++;
      }
    }
  }

  fprintf(Mreport, "<ship-types %i >\n", nbr_typ);

  for (P2 = aGame->players; P2; P2 = P2->next) {
    if ((P2 eq P) || visibleShipTypes(aGame, P2, P)) {
      for (t = P2->shiptypes; t; t = t->next) {
        if ((t->flag) || (P eq P2))
          fprintf(Mreport,
                  "%s %s %.2f %i %.2f %.2f %.2f\n",
                  t->name, P2->name, t->drive, t->attacks, t->weapons,
                  t->shields, t->cargo);
      }
    }
  }
  fprintf(Mreport, "</ship-types>\n");

}

/****f* MReport/rBT_M
 * NAME
 *   rBT_M --
 * FUNCTION
 *   report battles 
 ****
 */

void
rBT_M(game *aGame, player *P)
{
  battle         *b;
  participant    *r;
  int             nbr_bat = 0;

  for (b = aGame->battles; b; b = b->next)
    for (r = b->participants; r; r = r->next)   /* Count battles * * in *
                                                 * * * which * P is * * *
                                                 * * * * involved. */
      if (r->who eq P)
        nbr_bat++;

  if (nbr_bat) {
    fprintf(Mreport, "<battles %i >\n", nbr_bat);
    for (b = aGame->battles; b; b = b->next) {
      for (r = b->participants; r; r = r->next)
        if (r->who eq P)
          break;
      if (r) {                  /* A battle with P... */
        int             nbr_par = 0;

        for (r = b->participants; r; r = r->next)
          nbr_par++;
        fprintf(Mreport, "<battle %s %i >\n", b->where->name, nbr_par);
        for (r = b->participants; r; r = r->next) {
          group          *g;
          int             nbr_grp = 0;

          for (g = r->groups; g; g = g->next)
            nbr_grp++;
          fprintf(Mreport, "<groups %s %i >\n", r->who->name, nbr_grp);
          for (g = r->groups; g; g = g->next) {
            rGP_M(g, 0, G_MODE_BATTLE);
          }
          fprintf(Mreport, "</groups>\n");

        }
        fprintf(Mreport, "</battle>\n");
      }
    }
    fprintf(Mreport, "</battles>\n");
  }
}

/****f* MReport/rBB_M
 * NAME
 *   rBB_M --
 * FUNCTION
 *   report bombings 
 ****
 */

void
rBB_M(game *aGame, player *P)
{

  if (canseeBombing(aGame, P)) {
    bombing        *B;
    alliance       *a;
    int             nbr_bom = 0;

    for (B = aGame->bombings; B; B = B->next)
      for (a = B->viewers; a; a = a->next)
        if (a->who eq P)
          nbr_bom++;
    if (nbr_bom) {
      fprintf(Mreport, "<bombing %i >\n", nbr_bom);
      for (B = aGame->bombings; B; B = B->next) {
        for (a = B->viewers; a; a = a->next)
          if (a->who eq P)
            break;
        if (a) {
          fprintf(Mreport, "%s %s %s %.2f %.2f",
                  B->who->name, B->owner->name, B->name, B->pop, B->ind);
          if (B->producing eq PR_SHIP)
            fprintf(Mreport, " %s", B->producingshiptype->name);
          else
            fprintf(Mreport, " %s", productname[B->producing]);

          fprintf(Mreport, " %.2f %.2f %.2f\n", B->cap, B->mat, B->col);

        }
      }
      fprintf(Mreport, "</bombing>\n");
    }
  }
}

/****f* MReport/rMP_M
 * NAME
 *   rMP_M --
 * FUNCTION
 *   report text based map 
 *****
 */

void
rMP_M(game *aGame, player *P)
{
  player         *P2;
  int             i, j;
  group          *g;
  planet         *p;
  mapdimensions   mapDim;

  mapDim.x1 = P->mx;
  mapDim.x2 = P->mx + P->msize;
  mapDim.y1 = P->my;
  mapDim.y2 = P->my + P->msize;

  fprintf(Mreport, "<text-map %i >\n", MAPHEIGHT + 4);

  fprintf(Mreport, "%.2f %.2f %.2f %.2f\n",
          mapDim.x1, mapDim.y1, mapDim.x2, mapDim.y1);
  fprintf(Mreport, "%.2f %.2f %.2f %.2f\n",
          mapDim.x1, mapDim.y2, mapDim.x2, mapDim.y2);

  for (i = 0; i != MAPWIDTH; i++)
    fprintf(Mreport, "-");
  fprintf(Mreport, "\n");

  memset(map, ' ', sizeof map);

  for (P2 = aGame->players; P2; P2 = P2->next)
    if (P2 != P)
      for (g = P2->groups; g; g = g->next)
        putmap(&mapDim, groupx(aGame, g), groupy(aGame, g), '-');

  for (g = P->groups; g; g = g->next)
    putmap(&mapDim, groupx(aGame, g), groupy(aGame, g), '.');

  for (p = aGame->planets; p; p = p->next)
    if (!p->owner)
      putmap(&mapDim, p->x, p->y, 'o');

  for (p = aGame->planets; p; p = p->next)
    if (p->owner && p->owner != P)
      putmap(&mapDim, p->x, p->y, '+');

  for (p = aGame->planets; p; p = p->next)
    if (p->owner eq P)
      putmap(&mapDim, p->x, p->y, '*');

  for (i = 0; i != MAPHEIGHT; i++) {
    for (j = 0; j != MAPWIDTH; j++) {
      fprintf(Mreport, "%c", map[j][i]);
    }
    fprintf(Mreport, "\n");
  }
  for (i = 0; i != MAPWIDTH; i++)
    fprintf(Mreport, "-");
  fprintf(Mreport, "\n");

  fprintf(Mreport, "</text-map>\n");

}

/****f* MReport/rIC_M
 * NAME
 *   rIC_M
 * FUNCTION
 *   report incomming groups 
 ****
 */

void
rIC_M(game *aGame, player *P)
{
  int             nbr_inc = 0;
  player         *P2;

  nbr_inc = 0;
  for (P2 = aGame->players; P2; P2 = P2->next) {
    if (P2 != P) {
      group          *g;

      for (g = P2->groups; g; g = g->next) {
        if (g->dist && g->where->owner eq P) {
          nbr_inc++;
        }
      }
    }
  }

  if (nbr_inc) {
    fprintf(Mreport, "<incomming %i >\n", nbr_inc);
    for (P2 = aGame->players; P2; P2 = P2->next) {
      if (P2 != P) {
        group          *g;

        for (g = P2->groups; g; g = g->next) {
          if (g->dist && g->where->owner eq P) {

            fprintf(Mreport, "%s %s %.2f",
                    g->from->name, g->where->name, g->dist);
            if (g->thefleet)
              fprintf(Mreport, " %.2f", g->thefleet->fleetspeed);
            else
              fprintf(Mreport, " %.2f",
                      g->type->drive * g->drive * DRIVEMAGIC /
                      shipmass(g));
            fprintf(Mreport, " %.2f\n", shipmass(g) * g->ships);
          }
        }
      }
    }
    fprintf(Mreport, "</incomming>\n");
  }
}

/****f* MReport/rPT_M
 * NAME
 *   rPT_M --
 * FUNCTION
 *   report production tables 
 *****
 */

void
rPT_M(game *aGame, player *P)
{
  planet         *p;
  int             nbr_pro = 0;

  if (P->flags & F_PRODTABLE) {
    for (p = aGame->planets; p; p = p->next) {
      if (p->owner eq P && p->producing eq PR_SHIP) {
        nbr_pro++;
      }
    }
    if (nbr_pro) {
      fprintf(Mreport, "<production-tables %i >\n", nbr_pro);
      for (p = aGame->planets; p; p = p->next) {
        if (p->owner eq P && p->producing eq PR_SHIP) {
          rPD_M(p);
        }
      }
      fprintf(Mreport, "</production-tables>\n");
    }
  }
}


/****f* MReport/rPD_M
 * NAME
 *   rPD_M --
 * FUNCTION
 *   report production (it's a sub routine) 
 *****
 */

void
rPD_M(planet *p)
{
  double          mass;
  double          theshipmass;
  double          prog;

  fprintf(Mreport, "%s %s", p->name, p->producingshiptype->name);

  theshipmass = typemass(p->producingshiptype);
  mass = theshipmass * INDPERSHIP;
  prog = p->inprogress;
  if (theshipmass > p->mat)
    mass += (theshipmass - p->mat) / p->resources;

  fprintf(Mreport, " %.2f %.2f\n", mass, prog);
}


/****f* MReport/rRT_M
 * NAME
 *   rRT_M --
 * FUNCTION
 *   report routes
 *****
 */

void
rRT_M(game *aGame, player *P)
{
  int             nbr_rou = 0;
  planet         *p;

  for (p = aGame->planets; p; p = p->next) {
    if (p->owner eq P && (p->routes[0] || p->routes[1] ||
                          p->routes[2] || p->routes[3])) {
      nbr_rou++;
    }
  }

  if (nbr_rou) {
    fprintf(Mreport, "<routes %i >\n", nbr_rou);
    for (p = aGame->planets; p; p = p->next) {
      if (p->owner eq P && (p->routes[0] || p->routes[1] ||
                            p->routes[2] || p->routes[3])) {
        int             i;

        fprintf(Mreport, "%s", p->name);
        for (i = 0; i != MAXCARGO; i++) {
          if (p->routes[i])
            fprintf(Mreport, " %s", p->routes[i]->name);
          else
            fprintf(Mreport, " -");
        }
        fprintf(Mreport, "\n");
      }
    }                           /* for */
    fprintf(Mreport, "</routes>\n");
  }
}


/****f* MReport/rAP_M
 * NAME
 *   rAP_M --
 * FUNCTION
 *   report (all) planets 
 *****
 */

void
rAP_M(game *aGame, player *P)
{
  planet         *p;
  player         *P2;
  int             nbr_pla;

  /* First : the player is the owner, or he can see the planet, owned by * 
   *
   * * * * *  *  * * another player */
  for (P2 = aGame->players; P2; P2 = P2->next) {
    nbr_pla = 0;
    for (p = aGame->planets; p; p = p->next) {  /* count planets */
      if (((P2 eq P) && (p->owner eq P)) ||
          ((P2 != P) && (p->owner eq P2) && canseeplanet(P, p))
          )
        nbr_pla++;
    }
    if (nbr_pla) {
      fprintf(Mreport, "<planets %s %i >\n", P2->name, nbr_pla);
      for (p = aGame->planets; p; p = p->next) {        /* examines all *
                                                         * * * * the * *
                                                         * * * planets * */
        if (((P2 eq P) && (p->owner eq P)) ||
            ((P2 != P) && (p->owner eq P2) && canseeplanet(P, p))
            ) {
          /* if (p->owner eq P || (isunidentified(P, p) && * * * * * * * *
           * canseeplanet(P,p))) */
          rPL_M(p);
        }
      }
      fprintf(Mreport, "</planets>\n");
    }
  }

  /* Second : planets owned by others players, but not visible */
  for (p = aGame->planets; p; p = p->next) {    /* count planets */
    if (p->owner != P && isunidentified(P, p) && !canseeplanet(P, p))
      nbr_pla++;
  }
  if (nbr_pla) {
    fprintf(Mreport, "<planets unidentified %i >\n", nbr_pla);
    for (p = aGame->planets; p; p = p->next) {  /* examines all * * the *
                                                 * * * * * * planets */
      if (p->owner != P && isunidentified(P, p) && !canseeplanet(P, p)) {
        fprintf(Mreport, "%s %.2f %.2f\n", p->name, p->x, p->y);
      }
    }
    fprintf(Mreport, "</planets>\n");
  }
  /* Third : uninhabited planets */
  nbr_pla = 0;
  for (p = aGame->planets; p; p = p->next) {    /* count planets */
    if (isuninhabited(P, p))
      nbr_pla++;
  }
  if (nbr_pla) {
    fprintf(Mreport, "<planets uninhabited %i >\n", nbr_pla);
    for (p = aGame->planets; p; p = p->next) {  /* examines all * * the *
                                                 * * * * * * planets */
      if (isuninhabited(P, p)) {
        fprintf(Mreport, "%s %.2f %.2f", p->name, p->x, p->y);
        if (canseeplanet(P, p)) {
          fprintf(Mreport, " %.2f %.2f", p->size, p->resources);
        }
        fprintf(Mreport, "\n");
      }
    }
    fprintf(Mreport, "</planets>\n");
  }
}


/****f* MReport/rGG_M
 * NAME
 *   rGG_M --
 * FUNCTION
 *   report groups
 *****
 */

void
rGG_M(game *aGame, player *P)
{

  int             nbr_grp = 0;
  group          *g;

  for (g = P->groups; g; g = g->next)
    if (!g->thefleet)
      nbr_grp++;

  if (nbr_grp) {
    planet         *p;

    fprintf(Mreport, "<groups %s %i >\n", P->name, nbr_grp);
    for (p = aGame->planets; p; p = p->next) {  /* First : * * player's *
                                                 * * * * * * planets */
      if (p->owner eq P) {
        for (g = P->groups; g; g = g->next) {
          if (!g->thefleet) {   /* It's not a fleet */
            if ((g->location eq p) || (!g->location && g->where eq p)) {
              rGP_M(g, g->number, G_MODE_OWN);
            }
          }
        }
      }
    }

    for (p = aGame->planets; p; p = p->next) {  /* Next : planets * * *
                                                 * owned by * * * * * * * 
                                                 * * * others * * players */
      if (p->owner && p->owner != P) {
        for (g = P->groups; g; g = g->next) {
          if (!g->thefleet) {
            if ((g->location eq p) || (!g->location && g->where eq p)) {
              rGP_M(g, g->number, G_MODE_OWN);
            }
          }
        }
      }
    }

    for (p = aGame->planets; p; p = p->next)    /* Next : * * uninhabited
                                                 * * * * * * planets */
      if (!p->owner) {
        for (g = P->groups; g; g = g->next) {
          if (!g->thefleet) {
            if (g->location eq p) {
              rGP_M(g, g->number, G_MODE_OWN);
            }
          }
        }

        for (g = P->groups; g; g = g->next) {
          if (!g->thefleet) {
            if (!g->location && g->where eq p) {
              rGP_M(g, g->number, G_MODE_OWN);
            }
          }
        }

      }
    fprintf(Mreport, "</groups>\n");

  }
}


/****f* MReport/rGS_M
 * NAME
 *   rGS_M --
 * FUNCTION
 *   Report groups seen 
 *****
 */


void
rGS_M(game *aGame, player *P)
{
  player         *P2;

  for (P2 = aGame->players; P2; P2 = P2->next) {
    if (P2 != P) {
      int             nbr_grp = 0;
      group          *g;

      for (g = P2->groups; g; g = g->next) {
        if (canseegroup(P, g))
          nbr_grp++;
      }
      if (nbr_grp) {
        group          *g;

        fprintf(Mreport, "<groups %s %i >\n", P2->name, nbr_grp);
        for (g = P2->groups; g; g = g->next) {
          if (canseegroup(P, g))
            rGP_M(g, 0, G_MODE_ALIEN);
        }
        fprintf(Mreport, "</groups>\n");
      }
    }
  }
}


/****f* MReport/rFL_M
 * NAME
 *   rFL_M --
 * FUNCTION
 *   Report fleets 
 *****
 */

void
rFL_M(game *aGame, player *P)
{
  if (P->fleetnames) {
    fleetname      *fl;
    int             nbr_flt;

    nbr_flt = numberOfElements(P->fleetnames);
    fprintf(Mreport, "<fleets %i >\n", nbr_flt);
    for (fl = P->fleetnames; fl; fl = fl->next) {
      group          *g;
      int             nbr_grp = 0;

      for (g = P->groups; g; g = g->next)
        if (g->thefleet eq fl)
          nbr_grp++;

      fprintf(Mreport, "<fleet %s %.2f %i >\n", fl->name, fl->fleetspeed,
              nbr_grp);
      for (g = P->groups; g; g = g->next)
        if (g->thefleet eq fl)
          rGP_M(g, g->number, G_MODE_OWN);
      fprintf(Mreport, "</fleet>\n");
    }
    fprintf(Mreport, "</fleets>\n");
  }
}


/****f* MReport/rEN_M
 * NAME
 *   rEN_M --
 * FUNCTION
 *   Write last line of file report 
 *****
 */


void
rEN_M()
{
  fprintf(Mreport, "</report>");
}


/****f* MReport/rGP_M
 * NAME
 *   rGP_M --
 * FUNCTION
 *   report groups (it's a sub routine) 
 *****
 */

void
rGP_M(group *g, int n, int mode)
{
  static char    *loadtypename[] = {
    "CAP",
    "MAT",
    "COL",
    "-",
  };

  if (mode eq G_MODE_OWN)
    fprintf(Mreport, "%i ", n); /* n is the group number */
  else
    fprintf(Mreport, "0 ");     /* 19990313 - CB ; To give a standard * *
                                 * * * record format. */

  /* 19990313 - CB ; To give a standard record format. */
  if (mode eq G_MODE_BATTLE)
    fprintf(Mreport, "%i ", g->left);   /* Quantity of surviving * * *
                                         * ships */
  else
    fprintf(Mreport, "%i ", g->ships);

  fprintf(Mreport,
          "%s %.2f %.2f %.2f %.2f %s %.2f",
          g->type->name, g->drive, g->weapons, g->shields,
          g->cargo, loadtypename[g->loadtype], g->load);

  /* if (mode != G_MODE_BATTLE) */
  fprintf(Mreport, " %s", g->where->name);
  if (mode eq G_MODE_OWN) {
    if (g->dist) {
      fprintf(Mreport, " %.2f %s", g->dist, g->from->name);
    }
    /* else { fprintf(Mreport, " %s %s", "", ""); * * } */
  }
  if (mode eq G_MODE_BATTLE)
    fprintf(Mreport, " %i", g->ships);  /* Quantity of ships * * BEFORE a
                                         * * * battle */

  fprintf(Mreport, "\n");

}

/****f* MReport/rPL_M
 * NAME
 *   rPL_M --
 * FUNCTION
 *   report planets (it's a sub routine) 
 *****
 */


void
rPL_M(planet *p)
{
  fprintf(Mreport, "%s %.2f %.2f %.2f %.2f %.2f %.2f",
          p->name, p->x, p->y, p->size, p->pop, p->ind, p->resources);
  if (p->producing eq PR_SHIP)
    fprintf(Mreport, " %s", p->producingshiptype->name);
  else
    fprintf(Mreport, " %s", productname[p->producing]);
  fprintf(Mreport, " %.2f %.2f %.2f\n", p->cap, p->mat, p->col);
}
