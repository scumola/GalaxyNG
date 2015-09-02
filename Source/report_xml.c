/*  xgamma -bgamma 0.1 -ggamma 0.1 -rgamma 0.3 */
#include <time.h>
#include "create.h"
#include "report_xml.h"

void   rXMLHeader(game* aGame, player* P);
static void   rGame(game* aGame, player* P, int level, int type);
static void   rFeatures(game* aGame, player* P, int level);
static void   rTurn(game* aGame, player* P, int level, int type);
static void   rRace(game* aGame, player* P, int level, int type);
static void   rReport(game* aGame, player* P, int level, int type);
static void   rOrders(game* aGame, player* P, int level);
static void   rStatus(game* aGame, player* P, int level);
static void   rAlienRaces(game* aGame, player* P, int level);
static void   rDesigns(game* aGame, player* P, int level);
static void   rBattles(game* aGame, player* P, int level);
static void   rBombings(game* aGame, player* P, int level);
static void   rMap(game* aGame, player* P, int level);
static void   rIncoming(game* aGame, player* P, int level);
static void   rRoutes(game* aGame, player* P, int level);
static void   rIdentifiedWorlds(game* aGame, player* P, int level, int flag);
static void   rUnidentifiedWorlds(game* aGame, player* P, int level);
static void   rUninhabitedWorlds(game* aGame, player* P, int level);
static void   rGroups(game* aGame, player* P, int level, int flag);
static void   rFleets(game* aGame, player* P, int level);
static void   rMessages(game* aGame, player* P, strlist* messages,
			char* type, int level, int nbr_msgs);
static void   rTrailer(game* aGame, player* P, int level);

static void   showTech(player* P, int level);

static char    *loadtypename[] =
{
  "CAP",
  "MAT",
  "COL",
  "EMP",
};

/* History :

199906 : copy of report_m, changed to output xml
199911 : update to match new game paradigm
199911 : FS, report_xml closed the XMLreport file. This is 
         the task of the function that calls report_xml...
         resulted in a coredump. 
200002 : [BJE] fixed bugs and XML spec violations
200003 : [BJE] fixed doubled groups bug
200104 : CB, updated to add the game options section.
200308 : KDW - complete rewrite to match the galaxy-1.5 dtd

TODO :  write a doc :).

*/

static FILE* fpR;

void
report_xml(game* aGame, player* P, FILE* XMLreport, int RorF)
{
  fpR = XMLreport;		/* local global */
  rXMLHeader(aGame, P);
  rGame(aGame, P, 2, RorF);
  rTrailer(aGame, P, 0);
}


char*
safename(char* name)
{
  /* since this is an xml report, we can't let some characters into
     the file. Mainly this consists of '&', '<', and '>'. These
     shouldn't be allowed in names to begin with, but evidently that's
     not being enforced.
  */
  static char buffer[8192];

  int i, j, namelen;

  namelen = strlen(name);
  j = 0;

  for(i = 0; i < namelen && j < 8192; i++) {
    switch(name[i]) {
    case '&':
      strncpy(&buffer[j], "&amp;", 5);
      j += 5;
      break;

    case '<':
      strncpy(&buffer[j], "&lt;", 4);
      j += 4;
      break;

    case '>':
      strncpy(&buffer[j], "&gt;", 4);
      j += 4;
      break;

    default:
      buffer[j++] = name[i];
      break;
    }
  }

  buffer[j] = '\0';

  return buffer;
}


/* Write header of report file */
void
rXMLHeader(game* aGame, player *P)
{
  time_t  now;
  char    tbuf[32];

  time(&now);
  strcpy(tbuf, asctime(localtime(&now)));

  fprintf(fpR, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
	  "<!DOCTYPE galaxy PUBLIC \"-//Galaxy//GalaxyNG//EN\" "
	  "\"http://galaxy.pbem.net/galaxyng-1.0.0.dtd\">\n\n");


  fprintf(fpR, "<!-- produced by galaxyng %d.%d.%d on %24.24s -->\n\n",
	  GNG_MAJOR, GNG_MINOR, GNG_RELEASE, tbuf);

  /* checked to galaxy-1.0.0.dtd */
  fprintf(fpR,
	  "<galaxy>\n"
	  "  <variant>GalaxyNG</variant>\n"
	  "  <version>%d.%d.%d</version>\n",
	  GNG_MAJOR, GNG_MINOR, GNG_RELEASE);
}

static void
rTrailer(game* aGame, player* P, int level)
{
  fprintf(fpR, "</galaxy>\n");
}

/* report orders */
void
rOrders(game* aGame, player *P, int level)
{
  strlist* s;
  char     indent[64];

  int      nb_ord = 0;
  int      i = 1;
  int      lastLineType = 0;

  sprintf(indent, "%*.*s", level, level, " ");

  /* if a line begins with a + sign then that is an indication that
     this is an error line and we need to do a little extra work */

  s = P->orders;
  nb_ord = numberOfElements(s);

  if (nb_ord) {
    fprintf(fpR, "%s<orders>\n", indent);

    if (P->orders) {
      for (s = P->orders; s; s = s->next) {
	if (*(s->str) == '+') {
	  lastLineType = 1;
	  fprintf(fpR, "\n");
	  switch(*((s->str)+1)) {
	  case 'I':
	    fprintf(fpR, "%s    <info>%s</info>\n",
		    indent, safename((s->str)+3));
	    break;

	  case 'W':
	    fprintf(fpR, "%s    <warning>%s</warning>\n",
		    indent, safename((s->str)+3));
	    break;

	  case 'E':
	    fprintf(fpR, "%s    <error>%s</error>\n",
		    indent, safename((s->str)+3));
	    break;
	  }
	}
	else {
	  if (i != 1) {
	    if (lastLineType == 0)
	      fprintf(fpR, "</line>\n");
	    else
	      fprintf(fpR, "%s  </line>\n", indent);
	  }
	  fprintf(fpR, "%s  <line num=\"%d\">%s",
		  indent, i++, safename(s->str));
	  lastLineType = 0;
	}
      }
    }
    if (lastLineType == 0)
      fprintf(fpR, "</line>\n");
    else
      fprintf(fpR, "%s  </line>\n", indent);
    fprintf(fpR, "%s</orders>\n", indent);
  }
}

/* report mistakes */
void
rMK_XML(game* aGame, player *P)
{
  strlist        *s;
  int             nb_mis = 0;
  int             i = 1;

  s = P->mistakes;
  nb_mis = numberOfElements(s);	/* number of lines in fact */

  if (nb_mis == 0) {
    fprintf(fpR, "    <mistakes/>\n");
  }
  else {
    fprintf(fpR, "    <mistakes>\n");
    for (s = P->mistakes; s; s = s->next) {
      fprintf(fpR, "      <line num=\"%d\">%s</line>\n",
	      i++, safename(s->str));
    }
    fprintf(fpR, "    </mistakes>\n");
  }
}

/* report status of players */
void
rST_XML(game* aGame, player *P)
{
  player         *P2;
  int             nb_pla = 0;

  P2 = aGame->players;
  nb_pla = numberOfElements(P2);

  fprintf(fpR, "    <playerlist>\n");
  for (P2 = aGame->players; P2; P2 = P2->next) {
    fprintf(fpR,
	    "      <player name=\"%s\">\n"
	    "        <drive>%.2f</drive>\n"
	    "        <weapons>%.2f</weapons>\n"
	    "        <shields>%.2f</shields>\n"
	    "        <cargo>%.2f</cargo>\n"
	    "        <totpop>%.2f</totpop>\n"
	    "        <totind>%.2f</totind>\n"
	    "        <nbrOfPlanets>%d</nbrOfPlanets>\n",
	    safename(P2->name), P2->drive, P2->weapons, P2->shields, P2->cargo,
	    P2->totPop, P2->totInd, P2->numberOfPlanets);

    if (P2 != P) {
      fprintf(fpR, "        <posture>%s</posture>\n",
	      (atwar(P, P2) ? "War" : "Peace"));
    }
    else {
      fprintf(fpR, "        <posture>-</posture>\n");
    }

    if (P2->addr[0]) {
      if (P2->flags & F_ANONYMOUS) {
	fprintf(fpR, "        <address>anonymous</address>\n");
      }
      else {
	fprintf(fpR, "        <address>%s</address>\n",
		safename(P2->addr));
      }
    }
    else {
      fprintf(fpR, "        <address>none</address>\n");
    }
    fprintf(fpR, "      </player>\n");
  }
  fprintf(fpR, "    </playerlist>\n");
}

/* report ship types */
void
rSH_XML(game* aGame, player *P)
{
  player         *P2;
  shiptype       *t;

  int             nbr_typ = 0;

  for (P2 = aGame->players; P2; P2 = P2->next) {
    if ((P2 eq P) || visibleShipTypes(aGame, P2, P)) {
      for (t = P2->shiptypes; t; t = t->next) {
	if ((P2 eq P) || (t->flag)) {
	  nbr_typ++;
	}
      }
    }
  }

  if (nbr_typ == 0) {
    fprintf(fpR, "    <shiptypes/>\n");
  }
  else {
    fprintf(fpR, "    <shiptypes>\n");
      
    for (P2 = aGame->players; P2; P2 = P2->next) {
      if ((P2 eq P) || visibleShipTypes(aGame, P2, P)) {
	for (t = P2->shiptypes; t; t = t->next) {
	  if ((t->flag) || (P eq P2)) {
	    /* the following printf is split in 2 because
	       safename uses a static buffer */
	    fprintf( fpR,
		    "      <type owner=\"%s\"", safename(P2->name));
	    fprintf(fpR,
		    " name=\"%s\">\n"
		    "        <drive>%.2f</drive>\n"
		    "        <attacks>%d</attacks>\n"
		    "        <weapons>%.2f</weapons>\n"
		    "        <shields>%.2f</shields>\n"
		    "        <cargo>%.2f</cargo>\n"
		    "      </type>\n",
		    safename(t->name), t->drive, t->attacks,
		    t->weapons, t->shields, t->cargo);
	  }
	}
      }
    }
    fprintf(fpR, "    </shiptypes>\n");
  }
}



/* output game section */
void
rGame(game* aGame, player* P, int level, int RorF)
{
  fprintf(fpR, "%*.*s<game name=\"%s\">\n",
	  level, level, " ", aGame->name);

  rFeatures(aGame, P, level+2);		/* features */
  rTurn(aGame, P, level+2, RorF);

  fprintf(fpR, "%*.*s</game>\n", level, level, " ");

  return;
}


static void
rFeatures(game* aGame, player* P, int level)
{
  char indent[64];
  sprintf(indent, "%*.*s", level, level, " ");
  fprintf(fpR, "%s<features>\n      <size>%.2f</size>\n",
	  indent, aGame->galaxysize);
  fprintf(fpR,
	  "%s  <numWorlds>%d</numWorlds>\n",
	  indent, numberOfElements(aGame->planets));
  fprintf(fpR,
	  "%s  <numRaces>%d</numRaces>\n",
	  indent, numberOfElements(aGame->players));


  fprintf(fpR, "%s  <fullBombing status=\"%s\"/>\n", indent,
	  aGame->gameOptions.gameOptions & GAME_NONGBOMBING ? "ON" : "OFF");


  fprintf(fpR, "%s  <keepProduction status=\"%s\"/>\n", indent,
	  aGame->gameOptions.gameOptions & GAME_KEEPPRODUCTION ? "ON" : "OFF");

  
  fprintf(fpR, "%s  <dontDropDead status=\"%s\"/>\n", indent,
	  aGame->gameOptions.gameOptions & GAME_NODROP ? "ON" : "OFF");

  
  fprintf(fpR, "%s  <sphericalGalaxy status=\"%s\"/>\n", indent,
	  aGame->gameOptions.gameOptions & GAME_SPHERICALGALAXY ? "ON" : "OFF");

  fprintf(fpR,
	  "%s</features>\n", indent);
}

static void
rTurn(game* aGame, player* P, int level, int RorF)
{
  char indent[64];
  sprintf(indent, "%*.*s", level, level, " ");
  fprintf(fpR, "%s<turn num=\"%d\">\n", indent, aGame->turn);
  rRace(aGame, P, level+2, RorF);
  fprintf(fpR, "%s</turn>\n", indent);
}

static void
rRace(game* aGame, player* P, int level, int RorF)
{
  option*  curOption;
  strlist* s;
  char     indent[64];

  int      nb_opt = 0;
  int      nbr_msgs[2];

  sprintf(indent, "%*.*s", level, level, " ");
  fprintf(fpR, "%s<race name=\"%s\" id=\"%d\"",
		  indent, safename(P->name), ptonum(aGame->players, P));
  fprintf(fpR, " password=\"%s\">\n", safename(P->pswd));
  fprintf(fpR, "%s  <options>\n", indent);

  curOption = options;
  while (curOption->optionName) {
    nb_opt++;
    curOption++;
  }

  for (curOption = options; curOption->optionName; curOption++) {
    fprintf(fpR, "%s    <%s status=\"%s\"/>\n", indent, curOption->optionName,
	    P->flags&curOption->optionMask ? "ON" : "OFF");
  }

  fprintf(fpR, "%s  </options>\n", indent);
  s = aGame->messages;
  nbr_msgs[0] = numberOfElements(s);
  s = P->messages;
  nbr_msgs[1] = numberOfElements(s);
  if (nbr_msgs[0] || nbr_msgs[1]) {
    fprintf(fpR, "%s  <messages>\n", indent);
    if (nbr_msgs[0])
      rMessages(aGame, P, aGame->messages, "global", level+4, nbr_msgs[0]);
    if (nbr_msgs[1])
      rMessages(aGame, P, P->messages, "personal", level+4, nbr_msgs[1]);
    fprintf(fpR, "%s  </messages>\n", indent);
  }

  rReport(aGame, P, level+2, RorF);
  fprintf(fpR, "%s</race>\n", indent);
}


/* report global or personal messages */
static void
rMessages(game* aGame, player *P, strlist *messages, char *c,
	  int level, int nb_mes)
{
  strlist* s;
  char     indent[64];
  
  int      seq = 1;

  s = messages;
  sprintf(indent, "%*.*s", level, level, " ");

  fprintf(fpR, "%s<message type=\"%s\" lines=\"%d\" >\n",
	  indent, c, nb_mes);
  while (s) {
    fprintf(fpR, "%s  <line seq=\"%d\">%s</line>\n",
	    indent, seq, safename(s->str));
    s = s->next;
    seq++;
  }
  fprintf(fpR, "%s</message>\n", indent);
}


static void
rReport(game* aGame, player* P, int level, int RorF)
{
  char indent[64];

  sprintf(indent, "%*.*s", level, level, " ");

  switch (RorF) {
  case Report:
    fprintf(fpR, "%s<report>\n", indent);
    break;

  case Forecast:
    fprintf(fpR, "%s<forecast>\n", indent);
    break;
  }

  rOrders(aGame, P, level+2);
  rStatus(aGame, P, level+2);

  if (RorF == Report)
    rAlienRaces(aGame, P, level+2);

  if (RorF == Report || P->flags & F_SHIPTYPEFORECAST)
    rDesigns(aGame, P, level+2);
  
  if (RorF == Report) {
    rBattles(aGame, P, level+2);
    rBombings(aGame, P, level+2);
    rMap(aGame, P, level+2);
  }

  if (RorF == Report) {
    rIncoming(aGame, P, level+2);
  }

  if (RorF == Report || P->flags & F_ROUTESFORECAST)
    rRoutes(aGame, P, level+2);

  if (RorF == Report || P->flags & F_PLANETFORECAST)
    rIdentifiedWorlds(aGame, P, level+2, RorF);

  if (RorF == Report) {
    rUnidentifiedWorlds(aGame, P, level+2);
    rUninhabitedWorlds(aGame, P, level+2);
  }

  if (RorF == Report || P->flags & F_GROUPFORECAST) {
    rGroups(aGame, P, level+2, RorF);
    rFleets(aGame, P, level+2);
  }

  switch (RorF) {
  case Report:
    fprintf(fpR, "%s</report>\n", indent);
    break;

  case Forecast:
    fprintf(fpR, "%s</forecast>\n", indent);
    break;
  }


}

static void
rStatus(game* aGame, player* P, int level)
{
  char indent[64];

  sprintf(indent, "%*.*s", level, level, " ");

  fprintf(fpR, "%s<status>\n", indent);
  showTech(P, level+2);
  fprintf(fpR, "%s  <numPlanets>%d</numPlanets>\n",
	  indent, P->numberOfPlanets);
  fprintf(fpR, "%s  <population>%d</population>\n", indent, (int)P->totPop);
  fprintf(fpR, "%s  <industry>%.2f</industry>\n", indent, P->totInd);
  fprintf(fpR, "%s  <stockpiles>\n", indent);
  fprintf(fpR, "%s    <capital>%.2f</capital>\n", indent, P->totCap);
  fprintf(fpR, "%s    <material>%.2f</material>\n", indent, P->totMat);
  fprintf(fpR, "%s    <colonists>%.2f</colonists>\n", indent, P->totCol);
  fprintf(fpR, "%s  </stockpiles>\n", indent);
  fprintf(fpR, "%s</status>\n", indent);
}

static void
rAlienRaces(game* aGame, player* P, int level)
{
  char indent[64];
  player* P2;

  sprintf(indent, "%*.*s", level, level, " ");

  fprintf(fpR, "%s<alienRaces>\n", indent);
  for (P2 = aGame->players; P2; P2=P2->next) {
    if (P2 == P)
      continue;
    fprintf(fpR, "%s  <alienRace name=\"%s\" id=\"%d\">\n",
			indent, safename(P2->name), ptonum(aGame->players, P2));

    if (!(P->flags && F_ANONYMOUS)) {
      fprintf(fpR, "%s    <address>%s</address>\n", indent, P2->addr);
    }
    showTech(P2, level+4);
    fprintf(fpR, "%s    <state>%s</state>\n",
	    indent, (atwar(P, P2) ? "War" : "Peace"));
    fprintf(fpR, "%s  </alienRace>\n", indent);
  }
  fprintf(fpR, "%s</alienRaces>\n", indent);
}


static void 
showTech(player* P, int level)
{
  char indent[64];
  sprintf(indent, "%*.*s", level, level, " ");
  fprintf(fpR, "%s<tech>\n", indent);
  fprintf(fpR, "%s  <drive>%.2f</drive>\n", indent, P->drive);
  fprintf(fpR, "%s  <weapons>%.2f</weapons>\n", indent, P->weapons);
  fprintf(fpR, "%s  <shields>%.2f</shields>\n", indent, P->shields);
  fprintf(fpR, "%s  <cargo>%.2f</cargo>\n", indent, P->cargo);
  fprintf(fpR, "%s</tech>\n", indent);
}

static void
rDesigns(game* aGame, player* P, int level)
{
  shiptype* st;
  char      indent[64];

  sprintf(indent, "%*.*s", level, level, " ");

  if (numberOfElements(P->shiptypes) == 0) {
    fprintf(fpR, "%s<designs/>\n", indent);
    return;
  }

  fprintf(fpR, "%s<shiptypes>\n", indent);
  for (st = P->shiptypes; st; st=st->next) {
    fprintf(fpR, "%s  <shiptype name=\"%s\">\n", indent, safename(st->name));
    fprintf(fpR, "%s    <drive>%.2f</drive>\n", indent, st->drive);
    fprintf(fpR, "%s    <numAttacks>%d</numAttacks>\n", indent, st->attacks);
    fprintf(fpR, "%s    <weapons>%.2f</weapons>\n", indent, st->weapons);
    fprintf(fpR, "%s    <shields>%.2f</shields>\n", indent, st->shields);
    fprintf(fpR, "%s    <cargo>%.2f</cargo>\n", indent, st->cargo);
    fprintf(fpR, "%s  </shiptype>\n", indent);
  }
  fprintf(fpR, "%s</shiptypes>\n", indent);
}

static void
rBattles(game* aGame, player* P, int level)
{
  char indent[64];

  battle         *b;
  participant    *r;
  int             header_output = 0;

  sprintf(indent, "%*.*s", level, level, " ");

  for (b = aGame->battles; b; b = b->next) {
    for (r = b->participants; r; r = r->next) {
      if (r->who eq P) {
	int             nbr_par = 0;
	participant	  *plr;
	  
	if (header_output == 0) {
	  fprintf(fpR, "%s<battles>\n", indent);
	  header_output = 1;
	}

	for (plr = b->participants; plr; plr = plr->next) {
	  nbr_par++;
	}
	  
	fprintf(fpR, "%s  <battle>\n", indent);
	fprintf(fpR, "%s    <location>%s</location>\n",
		indent, safename(b->where->name));

	for (plr = b->participants; plr; plr = plr->next) {
	  group          *g;
	  int             nbr_grp = 0;
	  
	  for (g = plr->groups; g; g = g->next) {
	    nbr_grp++;
	  }
	  
	  fprintf(fpR, "%s    <battleGroups>\n", indent);
	  fprintf(fpR, "%s      <owner>%s</owner>\n",
		  indent, safename(plr->who->name));
	  
	  for (g = plr->groups; g; g = g->next) {
	    fprintf(fpR, "%s      <battleGroup>\n", indent);
	    fprintf(fpR, "%s        <numBefore>%d</numBefore>\n",
		    indent, g->ships);
	    fprintf(fpR, "%s        <sighting>\n", indent);
	    fprintf(fpR, "%s          <model>%s</model>\n",
		    indent, safename(g->type->name));
	    fprintf(fpR, "%s          <velocity>%.2f</velocity>\n",
		    indent, groupSpeed(g));
	    fprintf(fpR, "%s          <numAttacks>%.2f</numAttacks>\n",
		    indent, g->attack);
	    fprintf(fpR, "%s          <weapon>%.2f</weapon>\n",
		    indent, g->weapons);
	    fprintf(fpR, "%s          <shield>%.2f</shield>\n",
		    indent, g->shields);
	    fprintf(fpR, "%s          <mass>%.2f</mass>\n",
		    indent, shipmass(g)*g->ships);
	    fprintf(fpR, "%s        </sighting>\n", indent);
	    fprintf(fpR, "%s        <numAfter>%d</numAfter>\n",
		    indent, g->left);
	    fprintf(fpR, "%s      </battleGroup>\n", indent);
	  }
	  fprintf(fpR, "%s    </battleGroups>\n", indent);
	}
        if (P->flags & F_BATTLEPROTOCOL) {
          int             i;
          shot           *s;

	  fprintf(fpR, "%s    <battleProtocol>\n", indent);
          s = b->protocol->shots;
          for (i = 0; i < b->protocol->cur; i++) {
	    fprintf(fpR, "%s    <shot>\n", indent);
	    fprintf(fpR, "%s      <attacker>\n", indent);
	    fprintf(fpR, "%s        <name>%s</name>\n",
		    indent, safename(s[i].attacker->name));
	    fprintf(fpR, "%s        <type>%s</type>\n",
		    indent, safename(s[i].atype->name));
	    fprintf(fpR, "%s      </attacker>\n", indent);

	    fprintf(fpR, "%s      <defender>\n", indent);
	    fprintf(fpR, "%s        <name>%s</name>\n",
		    indent, safename(s[i].target->name));
	    fprintf(fpR, "%s        <type>%s</type>\n",
		    indent, safename(s[i].ttype->name));
	    fprintf(fpR, "%s      </defender>\n", indent);

            if (s[i].result) {
              fprintf(fpR, "%s      <result>destroyed</result>\n", indent);
            }
            else {
              fprintf(fpR, "%s      <result>shields</result>\n", indent);
            }
	    fprintf(fpR, "%s    </shot>\n", indent);
          }
        }
	fprintf(fpR, "%s  </battle>\n", indent);
      }
    }
  }
  if (header_output)
    fprintf(fpR, "%s</battles>\n", indent);
}


static void
rBombings(game* aGame, player* P, int level)
{
  char indent[64];
  bombing        *B;
  alliance       *a;
  int             nbr_bom = 0;

  sprintf(indent, "%*.*s", level, level, " ");

  if (!canseeBombing(aGame, P)) {
    fprintf(fpR, "%s<bombings/>\n", indent);
    return;
  }
    
  for (B = aGame->bombings; B; B = B->next)	{
    for (a = B->viewers; a; a = a->next) {
      if (a->who == P) {
	nbr_bom++;
	break;
      }
    }
  }
    
  if (nbr_bom == 0) {
    fprintf(fpR, "%s<bombings/>\n", indent);
    return;
  }

  fprintf(fpR, "%s<bombings>\n", indent);
  for (B = aGame->bombings; B; B = B->next) {
    for (a = B->viewers; a; a = a->next) {
      if (a->who == P) {
	break;
      }
    }
	      
    if (a) {
      fprintf(fpR,
	      "%s  <bombing>\n", indent);
      fprintf(fpR,
	      "%s    <owner>%s</owner>\n",
	      indent, safename(B->who->name));
      /*fprintf(fpR, " defender=\"%s\">\n", safename(B->owner->name));*/
      fprintf(fpR,
	      "%s    <location>%s</location>\n"
	      "%s    <population>%.2f</population>\n"
	      "%s    <industry>%.2f</industry>\n",
	      indent, B->name, indent, B->pop, indent, B->ind);

      fprintf(fpR,
	      "%s    <stockpiles>\n", indent);
      fprintf(fpR,
	      "%s      <capital>%.2f</capital>\n"
	      "%s      <material>%.2f</material>\n"
	      "%s      <colonists>%.2f</colonists>\n",
	      indent, B->cap, indent, B->mat, indent, B->col);
      fprintf(fpR,
	      "%s    </stockpiles>\n", indent);
      if (B->producing eq PR_SHIP) {
	fprintf(fpR,
		"%s    <production>%s</production>\n",
		indent, safename(B->producingshiptype->name));
      }
      else {
	fprintf(fpR,
		"%s    <production>%s</production>\n",
		indent, productname[B->producing]);
      }
      fprintf(fpR,
	      "%s  </bombing>\n", indent);
    }
  }

  fprintf(fpR, "%s</bombings>\n", indent);

}

static void 
rMap(game* aGame, player* P, int level)
{
  char     indent[64];
  player*  P2;
  group*   g;
  int      height;
  int      width;

  sprintf(indent, "%*.*s", level, level, " ");

  height = (int) (P->msize * MAPHEIGHT);
  width  = (int) (P->msize * MAPWIDTH);

  fprintf(fpR, "%s<map>\n", indent);
  fprintf(fpR, "%s  <ulx>%.2f</ulx>\n", indent, P->mx);
  fprintf(fpR, "%s  <uly>%.2f</uly>\n", indent, P->my);
  fprintf(fpR, "%s  <lrx>%.2f</lrx>\n", indent, P->mx+P->msize);
  fprintf(fpR, "%s  <lry>%.2f</lry>\n", indent, P->my+P->msize);

  for (P2 = aGame->players; P2; P2 = P2->next) {
    if (P2 != P) {
      for (g = P2->groups; g; g = g->next) {
        if (groupLocation(aGame, g) == NULL) {
	  fprintf(fpR, "%s  <group>\n%s    <position>\n", indent, indent);
	  fprintf(fpR, "%s      <x>%.2f</x>\n",
		  indent, (groupx(aGame, g) - P->mx) / width);
	  fprintf(fpR, "%s      <y>%.2f</y>\n",
		  indent, (groupy(aGame, g) - P->my) / height);
	  fprintf(fpR, "%s    </position\n%s  </group>\n", indent, indent);
	}
      }
    }
  }

  fprintf(fpR, "%s</map>\n", indent);
}

static void 
rIdentifiedWorlds(game* aGame, player* P, int level, int flag)
{
  char    indent[64];
  planet* pt;
  player* p2;
  
  int     header_output;

  sprintf(indent, "%*.*s", level, level, " ");

  
  for (p2 = aGame->players; p2; p2 = p2->next) {
    if (flag == Forecast && p2 != P)
	continue;
    header_output = 0;
    for (pt = aGame->planets; pt; pt=pt->next) {
      if (pt->owner == p2 && canseeplanet(P, pt)) {
	if (!header_output) {
	  fprintf(fpR, "%s<identifiedWorlds owner=\"%s\">\n",
		  indent, p2->name);
	  header_output = 1;
	}
	fprintf(fpR, "%s  <world name=\"%s\">\n", indent, pt->name);
	fprintf(fpR, "%s    <position>\n%s      <x>%.2f</x>\n"
		"%s      <y>%.2f</y>\n%s    </position>\n",
		indent, indent, pt->x, indent, pt->y, indent);
	fprintf(fpR, "%s    <resources>%.2f</resources>\n",
		indent, pt->resources);
	fprintf(fpR, "%s    <size>%.2f</size>\n", indent, pt->size);
	fprintf(fpR, "%s    <population>%.2f</population>\n",
		indent, pt->pop);
	fprintf(fpR, "%s    <industry>%.2f</industry>\n", indent, pt->ind);
	fprintf(fpR, "%s    <stockpiles>\n", indent);
	fprintf(fpR,
		"%s      <capital>%.2f</capital>\n"
		"%s      <material>%.2f</material>\n"
		"%s      <colonists>%.2f</colonists>\n",
		indent, pt->cap, indent, pt->mat, indent, pt->col);
	fprintf(fpR, "%s    </stockpiles>\n", indent);
	if (pt->producing == PR_SHIP) {
	  fprintf(fpR,
		  "%s    <production>%s</production>\n",
		  indent, safename(pt->producingshiptype->name));
	}
	else {
	  fprintf(fpR,
		  "%s    <production>%s</production>\n",
		  indent, productname[pt->producing]);
	}
	fprintf(fpR, "%s  </world>\n", indent);
      }
    }
    if (header_output)
      fprintf(fpR, "%s</identifiedWorlds>\n", indent);
  }
}

static void
rUnidentifiedWorlds(game* aGame, player* P, int level)
{
  planet* pt;
  char    indent[64];

  sprintf(indent, "%*.*s", level, level, " ");

  fprintf(fpR, "%s<unidentifiedWorlds>\n", indent);
  for (pt = aGame->planets; pt; pt = pt->next) {
    if (isunidentified(P, pt)) {
      fprintf(fpR, "%s  <world name=\"%s\">\n", indent, safename(pt->name));
      fprintf(fpR, "%s    <position>\n%s      <x>%.2f</x>\n"
	      "%s      <y>%.2f</y>\n%s    </position>\n",
	      indent, indent, pt->x, indent, pt->y, indent);
      fprintf(fpR, "%s  </world>\n", indent);
    }
  }
  fprintf(fpR, "%s</unidentifiedWorlds>\n", indent);
}

static void
rUninhabitedWorlds(game* aGame, player* P, int level)
{
  planet* pt;
  char    indent[64];
  int     header_output = 0;
  
  sprintf(indent, "%*.*s", level, level, " ");

  for (pt = aGame->planets; pt; pt = pt->next) {
    if (isuninhabited(P, pt)) {
      if (header_output == 0) {
	fprintf(fpR, "%s<uninhabitedWorlds>\n", indent);
	header_output = 1;
      }

      fprintf(fpR, "%s  <world name=\"%s\">\n", indent, pt->name);
      fprintf(fpR, "%s    <position>\n%s      <x>%.2f</x>\n"
	      "%s      <y>%.2f</y>\n",
	      indent, indent, pt->x, indent, pt->y);
      fprintf(fpR, "%s    </position>\n", indent);
      if (canseeplanet(P, pt)) {
	fprintf(fpR, "%s    <size>%.2f</size>\n", indent, pt->size);
	fprintf(fpR, "%s    <resources>%.2f</resources>\n", indent,
		pt->resources);
	fprintf(fpR, "%s    <stockpiles>\n", indent);
	fprintf(fpR,
		"%s      <capital>%.2f</capital>\n"
		"%s      <material>%.2f</material>\n"
		"%s      <colonists>%.2f</colonists>\n",
		indent, pt->cap, indent, pt->mat, indent, pt->col);
	fprintf(fpR, "%s    </stockpiles>\n", indent);
      }
      fprintf(fpR, "%s  </world>\n", indent);
    }
  }

  if (header_output)
    fprintf(fpR, "%s</uninhabitedWorlds>\n", indent);
}


static void
rIncoming(game* aGame, player* P, int level)
{
  char    indent[64];
  int     header_output = 0;
  player* P2;

  sprintf(indent, "%*.*s", level, level, " ");

  for (P2 = aGame->players; P2; P2=P2->next) {
    if (P2 != P) {
      group* g;

      for (g = P2->groups; g; g = g->next) {
	if (g->dist && g->where->owner == P) {
	  if (header_output == 0) {
	    fprintf(fpR, "%s<incomingGroups>\n", indent);
	    header_output = 1;
	  }

	  fprintf(fpR, "%s  <group>\n", indent);
	  fprintf(fpR, "%s    <from>%s</from>\n",
		  indent, safename(g->from->name));
	  fprintf(fpR, "%s    <destination>%s</destination>\n",
		  indent, safename(g->where->name));
	  fprintf(fpR, "%s    <remaining>%.2f</remaining>\n",
		  indent, g->dist);
	  if (g->thefleet)
	    fprintf(fpR, "%s    <speed>%.2f</speed>\n",
		    indent, g->thefleet->fleetspeed);
	  else
	    fprintf(fpR, "%s    <speed>%.2f</speed>\n",
		    indent, g->type->drive*g->drive*DRIVEMAGIC/shipmass(g));
	  fprintf(fpR, "%s    <mass>%.2f</mass>\n",
		  indent, g->ships*shipmass(g));
	  fprintf(fpR, "%s  </group>\n", indent);
	}
      }
    }
  }
  if (header_output)
    fprintf(fpR, "%s</incomingGroups>\n", indent);
}

static void
rRoutes(game* aGame, player* P, int level)
{
  planet* pt;
  char    indent[64];
  int     header_output = 0;

  sprintf(indent, "%*.*s", level, level, " ");

  for (pt = aGame->planets; pt; pt = pt->next) {
    if (pt->owner == P &&
	(pt->routes[0] ||
	 pt->routes[1] ||
	 pt->routes[2] ||
	 pt->routes[3])) {
      int i;

      if (header_output == 0) {
	fprintf(fpR, "%s<routes>\n", indent);
	header_output = 1;
      }

      fprintf(fpR, "%s  <route>\n", indent);
      fprintf(fpR, "%s    <world>%s</world>\n",
	      indent, safename(pt->name));
      for (i = 0; i < MAXCARGO; i++) {
	if (pt->routes[i]) 
	  fprintf(fpR, "%s    <%s>%s</%s>\n", indent, loadtypename[i],
		  safename(pt->routes[i]->name), loadtypename[i]);
      }
      fprintf(fpR, "%s  </route>\n", indent);
    }
  }
  if (header_output)
    fprintf(fpR, "%s</routes>\n", indent);
	  
}

static void
rGroups(game* aGame, player* P, int level, int flag)
{
  char    indent[64];
  int     header_output = 0;
  planet* pt;
  player* P2;
  group*  g;

  sprintf(indent, "%*.*s", level, level, " ");

  for (P2 = aGame->players; P2; P2 = P2->next) {
    if (P2 != P && flag == Forecast)
      continue;
    if (!P2->groups)
      continue;
    header_output = 0;

    for (pt = aGame->planets; pt; pt = pt->next) {
      if (pt->owner == P) {
	for (g = P2->groups; g; g = g->next) {
	  if (!g->thefleet) { /* It's not a fleet */
	    if ((g->location == pt) ||
		(!g->location && g->where == pt)) {

	      if (header_output == 0) {
		fprintf(fpR, "%s<groups owner=\"%s\">\n",
			indent, safename(P2->name));
		header_output = 1;
	      }

	      fprintf(fpR, "%s  <group>\n", indent);
	      if (P2 == P)
		fprintf(fpR, "%s    <id>%d</id>\n", indent, g->number);
	      fprintf(fpR, "%s    <type>%s</type>\n",
		      indent, safename(g->type->name));
	      fprintf(fpR, "%s    <tech>\n", indent);
	      fprintf(fpR, "%s      <drive>%.2f</drive>\n", indent, g->drive);
	      fprintf(fpR, "%s      <weapons>%.2f</weapons>\n",
		      indent, g->weapons);
	      fprintf(fpR, "%s      <shields>%.2f</shields>\n",
		      indent, g->shields);
	      fprintf(fpR, "%s      <cargo>%.2f</cargo>\n", indent, g->cargo);
	      fprintf(fpR, "%s    </tech>\n", indent);
	      if (g->loadtype && g->loadtype != 3) {
		fprintf(fpR, "%s    <loadtype>%s</loadtype>\n",
			indent, loadtypename[g->loadtype]);
		fprintf(fpR, "%s    <load>%.2f</load>\n", indent, g->load);
	      }

	      if (canseeplanet(P, g->where))
		fprintf(fpR, "%s    <destination>%s</destination>\n",
			indent, safename(g->where->name));

	      if (P == P2) {
		if (g->dist) {
		  fprintf(fpR, "%s    <source>%s</source>\n",
			  indent, safename(g->from->name));
		  fprintf(fpR, "%s    <remaining>%.2f</remaining>\n",
			  indent, g->dist);
		}

		if (g->thefleet) {
		  fprintf(fpR, "%s    <speed>%.2f</speed>\n",
			  indent, g->thefleet->fleetspeed);
		  fprintf(fpR, "%s    <fleet>%s</fleet>\n",
			  indent, safename(g->thefleet->name));
		}
		else {
		  fprintf(fpR, "%s    <speed>%.2f</speed>\n",
			  indent,
			  g->type->drive*g->drive*DRIVEMAGIC/shipmass(g));
		}
	      }
	      fprintf(fpR, "%s  </group>\n", indent);
	    }
	  }
	}
      }
    }
    if (header_output)
      fprintf(fpR, "%s</groups>\n", indent);
  }
    
}

static void
rFleets(game* aGame, player* P, int level)
{
  char indent[64];

  sprintf(indent, "%*.*s", level, level, " ");

  if (P->fleetnames) {
    fleetname* fn;
    int        i;
    
    fprintf(fpR, "%s<fleets>\n", indent);
    for (i = 1, fn = P->fleetnames; fn; fn = fn->next, i++) {
      group* g;

      for (g = P->groups; g; g=g->next) {
	if (g->thefleet == fn)
	  break;
      }

      fprintf(fpR, "%s  <fleet>\n", indent);
      fprintf(fpR, "%s    <id>%s</id>\n", indent, fn->name);
      fprintf(fpR, "%s    <size>%d</size>\n",
	      indent, numOfGroupsInFleet(fn, P->groups));
      if (g) {
	if (canseeplanet(P, g->where)) {
	  fprintf(fpR, "%s    <destination>%s</destination>\n",
		  indent, safename(g->where->name));
	}
	if (g->dist) {
	  fprintf(fpR, "%s    <distance>%.2f</distance>\n",
		  indent, g->dist);
	  fprintf(fpR, "%s    <from>%s</from>\n",
		  indent, safename(g->from->name));
	}
	fprintf(fpR, "%s    <speed>%.2f</speed>\n",
		indent, fn->fleetspeed);
      }
      fprintf(fpR, "%s  </fleet>\n", indent);
    }
    fprintf(fpR, "%s</fleets>\n", indent);
  }
}


void
XMLPlanetsForecast(game* aGame, player *P, FILE* XMLreport)
{
  planet* p;
  option* curOption;
  char    indent[64];

  fpR = XMLreport;

  rXMLHeader(aGame, P);

  fprintf(fpR, "  <game name=\"%s\">\n", aGame->name);
  rFeatures(aGame, P, 4);
  fprintf(fpR, "    <turn num=\"%d\">\n", aGame->turn+1);
  fprintf(fpR, "      <race name=\"%s\">\n", safename(P->name));
  strcpy(indent, "        ");
  fprintf(fpR, "%s<options>\n", indent);
  for (curOption = options; curOption->optionName; curOption++) {
    if (P->flags&curOption->optionMask) {
      if (curOption->optionMask == F_ANONYMOUS)
	continue;
      fprintf(fpR, "%s  <%s/>\n", indent, curOption->optionName);
    }
  }
  fprintf(fpR, "%s</options>\n", indent);
  fprintf(fpR, "%s<forecast>\n", indent);

  rOrders(aGame, P, 10);

  fprintf(fpR, "%s  <worlds>\n", indent);

  for (p = aGame->planets; p; p = p->next) {
    if (p->owner == P) {
      fprintf(fpR, "%s    <world name=\"%s\">\n", indent, safename(p->name));
      fprintf(fpR, "%s      <position>\n", indent);
      fprintf(fpR, "%s        <x>%.2f</x>\n", indent, p->x);
      fprintf(fpR, "%s        <y>%.2f</y>\n", indent, p->y);
      fprintf(fpR, "%s      </position>\n", indent);

      if (p->flags & PL_VISPREVTURN) {
	fprintf(fpR, "%s      <resources>%.2f</resources>\n",
		indent, p->resources);
	fprintf(fpR, "%s      <size>%.2f</size>\n", indent, p->size);
	fprintf(fpR, "%s        <population>%.2f</population>\n",
		indent, p->pop);
	fprintf(fpR, "%s        <industry>%.2f</industry>\n",
		indent, p->ind);
	fprintf(fpR, "%s      <stockpiles>\n", indent);
	fprintf(fpR, "%s        <capital>%.2f</capital>\n",
		indent, p->cap);
	fprintf(fpR, "%s        <material>%.2f</material>\n",
		indent, p->mat);
	fprintf(fpR, "%s        <colonists>%.2f</colonists>\n",
		indent, p->col);

	fprintf(fpR, "%s      </stockpiles>\n", indent);

	if (p->producing eq PR_SHIP)
	  fprintf(fpR, "%s      <producing>%s</producing\n",
		  indent, safename(p->producingshiptype->name));
	else
	  fprintf(fpR, "%s      <producing>%s</producing\n",
		  indent, productname[p->producing]);
      }
      fprintf(fpR, "%s    </world>\n", indent);
    }
  }
  fprintf(fpR, "%s  </worlds>\n", indent);
  fprintf(fpR, "%s</forecast>\n", indent);

  fprintf(fpR, "      </race>\n");
  fprintf(fpR, "    </turn>\n");
  fprintf(fpR, "  </game>\n");
  fprintf(fpR, "</galaxy>\n");
}


/****f* Report/yourStatusForecast
 * NAME
 *   yourStatusForecast
 ******
 */

void
yourXMLStatusForecast(planet *planets, player *P, fielddef *fields)
{
  int             state;
  planet         *p;

  for (p = planets; p; p = p->next) {
    if (p->owner eq P) {
      if (!(p->flags & PL_VISPREVTURN)) {
        P->totInd -= p->ind;
        P->totPop -= p->pop;
      }
    }
  }

  fprintf(fields->destination, "\n\t\tYour Status\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {

    formatLabels("N D W S C P I # R", "lcccccccc", fields);
    formatString(P->name, fields);
    formatFloat(P->drive, fields);
    formatFloat(P->weapons, fields);
    formatFloat(P->shields, fields);
    formatFloat(P->cargo, fields);
    formatFloat(P->totPop, fields);
    formatFloat(P->totInd, fields);
    formatInteger(P->numberOfPlanets, fields);
    formatString((atwar(P, P) ? "War" : "Peace"), fields);
    formatReturn(fields);
    formatPrint(fields);
  }
}

/****f* GalaxyNG/yourPlanetsForecast
 * NAME
 *   yourPlanetsForecast -- planet forecast for next turn.
 * FUNCTION
 *   This is used by the forecaster (orders checker) 
 *   to predict the situation
 *   of the planets the next turn.
 *****
 */

void
yourXMLPlanetsForecast(planet *planets, player *P, fielddef *fields)
{
  int             state;

  fprintf(fields->destination, "\n\t\tYour Planets\n\n");

  formatReset(fields);
  for (state = 0; state < 2; state++) {
    planet         *p;

    formatLabels("N X Y S P I R P $ M C", "lcccccccccc", fields);
    for (p = planets; p; p = p->next) {
      if (p->owner eq P) {
        formatString(p->name, fields);
        formatFloat(p->x, fields);
        formatFloat(p->y, fields);
        if (p->flags & PL_VISPREVTURN) {
          formatFloat(p->size, fields);
          formatFloat(p->pop, fields);
          formatFloat(p->ind, fields);
          formatFloat(p->resources, fields);
          if (p->producing eq PR_SHIP)
            formatString(p->producingshiptype->name, fields);
          else
            formatString(productname[p->producing], fields);
          formatFloat(p->cap, fields);
          formatFloat(p->mat, fields);
          formatFloat(p->col, fields);
        }
        else {
          formatStringCenter("?", fields);
          formatStringCenter("?", fields);
          formatStringCenter("?", fields);
          formatStringCenter("?", fields);
          formatString(productname[PR_CAP], fields);
          formatStringCenter("?", fields);
          formatStringCenter("?", fields);
          formatStringCenter("?", fields);
        }
        formatReturn(fields);
      }
    }
    formatPrint(fields);
  }
}


