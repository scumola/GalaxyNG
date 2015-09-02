#include "report.h"
#include "create.h"
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

char           *vreport =
    "$Id: report.c,v 1.20 2005/01/04 01:04:02 madcoder42 Exp $";

/****h* GalaxyNG/Report 
 * NAME
 *   Report -- functions to create the turn report.
 * PURPOSE
 *   A collection of functions to create the turn reports.
 *   There a many confusingly named functions, but the function
 *   that creates the actual turn report is report().
 *   The turn report can be formatted in two different formats
 *   (1) plain galaxy
 *   (2) Galaxy Plus (The russian variation of galaxy)
 *   In addition to this there are functions to create the turn
 *   report for the GM and functions to create the highscore list.
 *
 *   The turn report is a collection of tables plus an ASCII map
 *   of the galaxy.
 *   Each table has a heading. The tables consists of a number
 *   of rows. Each row consists of a number of fields.
 *   The formatting routines allow fields to be aligned across
 *   the rows.  This is done by a two pass process. In the first
 *   pass, nothing is printed but the maximum width of the field 
 *   in all rows is determined. In the second pass this information
 *   is used to properly print the field.    
 ********
 */


/****v* Report/formatBuffer
 * NAME
 *   formatBuffer -- buffer to format lines.
 * SOURCE
 */

char            formatBuffer[FORMATBUFFERSIZE];

/********/



/****f* Report/highScoreList
 * NAME
 *   highScoreList -- create high score list
 * SYNOPSIS
 *   void highScoreList(game *aGame)
 * FUNCTION
 *   Creates a file with the high score list for the current turn.
 *   This file is later included into the turn report.
 * RESULT
 *   High score list under the name notices/<game name>.score 
 * BUGS
 *   Should check for errors and write them to the log file.
 *******
 */

void
highScoreList(game *aGame)
{
  FILE           *reportFile;
  game           *prevTurn;
  char           *fileName;

  assert(aGame != NULL);

  pdebug(DFULL, "highScoreList\n");
  fileName =
      createString("%s/notices/%s.score", galaxynghome, aGame->name);
  if ((reportFile = GOS_fopen(fileName, "w"))) {
    if ((prevTurn = loadgame(aGame->name, aGame->turn - 1))) {
      loadNGConfig(prevTurn);
      score(prevTurn, aGame, 0, reportFile);
      freegame(prevTurn);
    }
    else {
      fprintf(stderr, "Can't open game \"%s\", turn %d\n", aGame->name,
              aGame->turn - 1);
    }
    fclose(reportFile);
  }
  else {
    fprintf(stderr, "Can't open %s\n", fileName);
  }
  free(fileName);
}



/****f* Report/mailGMReport
 * NAME
 *   mailGMReport -- mail the GM a status report.
 * SYNOPSIS
 *   void mailGMReport(game *aGame, char *gameName)
 * FUNCTION
 *   Email the GM a status report about a run of a turn.
 * SEE ALSO
 *   CMD_run()  
 * NOTES 
 *   Does log any errors.
 *   Does not return an error status.
 *****
 */

void
mailGMReport(game *aGame, char *gameName)
{
  char     *fileName;
  FILE     *gmreport;
  envelope *anEnvelope;

  assert(aGame != NULL);

  if (aGame->serverOptions.GMemail) {
    anEnvelope = createEnvelope();
    setHeader(anEnvelope, MAILHEADER_TO, "%s",
              aGame->serverOptions.GMemail);
    /*
    anEnvelope->from_name = strdup(aGame->serverOptions.GMname);
    anEnvelope->from_address = strdup(aGame->serverOptions.GMemail);
    */
    setHeader(anEnvelope, MAILHEADER_SUBJECT,
              "[GNG] %s turn %d text report for GM", gameName,
              aGame->turn);
    setHeader(anEnvelope, MAILHEADER_REPLYTO, aGame->serverOptions.ReplyTo);
    
    /* I'm changing these so the server emails to the GM, not the
     * GM to himself.
    anEnvelope->from_name = strdup(aGame->serverOptions.GMname);
    anEnvelope->from_address = strdup(aGame->serverOptions.GMemail);
     */

    anEnvelope->from_name = strdup(aGame->serverOptions.SERVERname);
    anEnvelope->from_address = strdup(aGame->serverOptions.SERVERemail);

    fileName = createString("%s/%s_GM", tempdir, gameName);
    if ((gmreport = fopen(fileName, "w"))) {
      createGMReport(aGame, gameName, gmreport);
      fclose(gmreport);
      eMail(aGame, anEnvelope, fileName);
      ssystem("rm %s", fileName);
    }
    else {
      fprintf(stderr, "Can't open \"%s\".\n", fileName);
    }
    destroyEnvelope(anEnvelope);
    free(fileName);
  }
  else {
    plog(LBRIEF,
         "No email address of GM set, so no GM report is mailed.\n");
  }
}


/****f* Report/createDummyGame
 * NAME
 *   createDummyGame -- create a dummy game.
 * SYNOPSIS
 *   game *createDummyGame(void)
 * FUNCTION
 *   Create a dummy game in case a real game could not be loaded.
 *   Then use it to read the .galaxyngrc file.
 *******
 */

game           *
createDummyGame(void)
{
  game           *aGame;

  aGame = allocStruct(game);

  setName(aGame, "UnknownGame");
  loadNGConfig(aGame);

  return aGame;
}


/****f* Report/createGMReport
 * NAME
 *   createGMReport -- create status report for GM
 * SYNOPSIS
 *   void createGMReport(game *aGame, char *gameName, FILE *gmreport)
 * FUNCTION
 *   Creates the status report for the GM. Currently consists of
 *   the log file and the high score list.
 ******
 */

void
createGMReport(game *aGame, char *gameName, FILE * gmreport)
{
  char           *fileName;
  struct fielddef fields;
  player         *aDummyPlayer;


  fileName = createString("%s/notices/%s.score", galaxynghome, gameName);
  appendToFile(fileName, gmreport);
  free(fileName);
  fileName = createString("%s/log/%s", galaxynghome, gameName);
  appendToFile(fileName, gmreport);
  free(fileName);

  fields.destination = gmreport;
  reportPlayers(aGame->players, &fields);
  reportRaceProduction(aGame->players, &fields);
  reportLastOrders(aGame->players, &fields);
  reportGlobalMessages(aGame->messages, &fields);
  reportGMBombings(aGame, &fields);
  aDummyPlayer = allocStruct(player);

  setName(aDummyPlayer, "DummyDummy");
  aDummyPlayer->msize = aGame->galaxysize;
  reportMap(aGame, aDummyPlayer, &fields);
}



/****f* Report/mailTurnReport
 * NAME 
 *   mailTurnReport -- create and mail a turn report
 * FUNCTION
 *   Create and mail a turn report to a player.
 *    0  -- all OK
 *   >0  -- something went wrong
 *******
 */

int
mailTurnReport(game *aGame, player *aPlayer, long kind)
{
  char           *fileName;
  char            ext[4];
  FILE           *turnreport;
  int             result;

  pdebug(DFULL, "mailTurnReport\n");

  result = 1;
  if ((aPlayer->flags & F_DEAD) == 0) {
    switch(kind) {
    case F_XMLREPORT:
      strcpy(ext, "xml");
      break;

    case F_MACHINEREPORT:
      strcpy(ext, "mch");
      break;

    default:
      strcpy(ext, "txt");
      break;
    }

    fileName = createString("%s/reports/%s/%s_%d.%s",
                            galaxynghome,
                            aGame->name, aPlayer->name, aGame->turn, ext);
    if ((turnreport = GOS_fopen(fileName, "w"))) {
      envelope       *anEnvelope;

      anEnvelope = createEnvelope();
      if (aPlayer->flags & F_COMPRESS) {
        anEnvelope->compress = TRUE;
      }
      setHeader(anEnvelope, MAILHEADER_TO, "%s", aPlayer->addr);
      anEnvelope->from_name = strdup(aGame->serverOptions.SERVERname);
      anEnvelope->from_address = strdup(aGame->serverOptions.SERVERemail);
      switch (kind) {
      case F_XMLREPORT:
        setHeader(anEnvelope, MAILHEADER_SUBJECT,
                  "[GNG] %s turn %d XML report for %s",
                  aGame->name, aGame->turn, aPlayer->name);
        break;
      case F_MACHINEREPORT:
        setHeader(anEnvelope, MAILHEADER_SUBJECT,
                  "[GNG] %s turn %d machine report for %s",
                  aGame->name, aGame->turn, aPlayer->name);
        break;
      default:
        setHeader(anEnvelope, MAILHEADER_SUBJECT,
                  "[GNG] %s turn %d text report for %s",
                  aGame->name, aGame->turn, aPlayer->name);
      }
      setHeader(anEnvelope, MAILHEADER_REPLYTO, aGame->serverOptions.ReplyTo);
      createTurnReport(aGame, aPlayer, turnreport, kind);
      fclose(turnreport);
      result = eMail(aGame, anEnvelope, fileName);
      destroyEnvelope(anEnvelope);
#if 0
#ifdef WIN32
      result |= ssystem("del %s", fileName);
#else
      result |= ssystem("rm %s", fileName);
#endif
#endif
    }
    else {
      fprintf(stderr, "Can't open %s\n", fileName);
    }
    free(fileName);
  }
  return result;
}


/****f* Report/saveTurnReport
 * NAME
 *   saveTurnReport --
 * FUNCTION
 *   Create and save a turn report in report/<game name>/
 *******
 */

void
saveTurnReport(game *aGame, player *aPlayer, long kind)
{
  FILE           *turnreport;
  char           *fileName;

  switch (kind) {
  case F_XMLREPORT:
    fileName = createString("%s/reports/%s/%s_%d.xml",
                            galaxynghome, aGame->name, aPlayer->name,
                            aGame->turn);
    break;
  case F_MACHINEREPORT:
    fileName = createString("%s/reports/%s/%s_%d.m",
                            galaxynghome, aGame->name, aPlayer->name,
                            aGame->turn);
    break;
  default:
    fileName = createString("%s/reports/%s/%s_%d.txt",
                            galaxynghome, aGame->name, aPlayer->name,
                            aGame->turn);
  }
  if ((turnreport = GOS_fopen(fileName, "w"))) {
    createTurnReport(aGame, aPlayer, turnreport, kind);
    fclose(turnreport);
  }
  else {
    fprintf(stderr, "Can't open %s\n", fileName);
  }
  free(fileName);
}


/****f* Report/createTurnReport
 * NAME
 *   createTurnReport -- create turn report.
 * FUNCTION
 *   Creates the turn report, a concaterace of 
 *   bulletins and the actual turn report.
 ******
 */

void
createTurnReport(game *aGame, player *aPlayer, FILE * reportfile,
                 long kind)
{
  char           *fileName;

  fprintf(reportfile, "Bulletins for Galaxy Game %s Turn %d\n\n",
          aGame->name, aGame->turn);
  fileName =
      createString("%s/notices/%s.score", galaxynghome, aGame->name);
  appendToFile(fileName, reportfile);
  free(fileName);
  fileName = createString("%s/notices/global.bulletin", galaxynghome);
  appendToFile(fileName, reportfile);
  free(fileName);
  fileName = createString("%s/notices/%s.info", galaxynghome, aGame->name);
  appendToFile(fileName, reportfile);
  free(fileName);
  fileName = createString("%s/notices/%s.%d.notice",
                          galaxynghome, aGame->name, aGame->turn);
  appendToFile(fileName, reportfile);
  free(fileName);

  fprintf(reportfile, "\n\n\t\tProduction Status\n");
  fprintf(reportfile, "Sofar you have:\n");
  fprintf(reportfile, "  produced a total shipmass of %.2f\n",
          aPlayer->massproduced);
  fprintf(reportfile, "  lost a total shipmass of     %.2f\n",
          aPlayer->masslost);

  fprintf(reportfile, "\n\n\t\tReal Name\n");
  if (aPlayer->realName[0]) {
    fprintf(reportfile, "  Your real name is %s.\n", aPlayer->realName);
  }
  else {
    fprintf(reportfile, "  Your real name is unknown.\n"
            "  Use the '=' order to set it.\n");
  }
  fprintf(reportfile, "\n\nEnd of the Bulletins\n\n");



  switch (kind) {
  case F_XMLREPORT:
    report_xml(aGame, aPlayer, reportfile, Report);
    break;
  case F_MACHINEREPORT:
    report_m(aPlayer, aGame, reportfile);
    break;
  default:
    report(aGame, aPlayer, reportfile);
  }
}


/****f* Report/appendToFile
 * NAME
 *   appendToFile -- append contents of one file to another.
 * SYNOPSIS
 *   void appendToFile(char *fileName, FILE *report) 
 * FUNCTION
 *   Append the content of another file to a file.
 *   If the file does not exist, just skip it.
 ******
 */

void
appendToFile(char *fileName, FILE * report)
{
  FILE           *f;
  char           *isRead;

  f = GOS_fopen(fileName, "r");
  if (f) {
    for (isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, f);
         isRead; isRead = fgets(lineBuffer, LINE_BUFFER_SIZE, f)) {
      fputs(lineBuffer, report);
    }
    fclose(f);
  }
}


/****f* Report/score 
 * NAME
 *   score -- create high score list.
 * SYNOPSIS
 *   void score(game *g1, game *g2, int html, FILE *dest)
 * FUNCTION
 *   Compute the high score list based on the differences in
 *   the economic status of the races between two turns. 
 * INPUTS
 *   g1    -- previous turn
 *   g2    -- this turn
 *   html  -- do we want html output?
 *   dest  -- file to write the results to
 *******
 */

void
score(game *g1, game *g2, int html, FILE * dest)
{
  int             numberOfPlayers;
  int             number;
  time_t          ttp;
  char            timeBuffer[255];

  raceStatus(g1);
  raceStatus(g2);
  rateRaces(g2->players);
  rateRaces(g1->players);
  numberOfPlayers = numberOfElements(g2->players);

  if (html) {
    fprintf(dest, "<HTML>\n");
    fprintf(dest, "<BODY bgcolor=\"#000000\" text=\"#FFFFff\""
            " link=\"#00aaff\" vlink=\"#aaaaff\" alink=\"#33ff33\">");
    fprintf(dest, "<STRONG>%s Top %d of turn %d</STRONG>\n<PRE>\n",
            g2->name, numberOfPlayers, g2->turn);
  }
  else {
    fprintf(dest, "%s Top %d of turn %d which ran on ",
            g2->name, numberOfPlayers, g2->turn);
  }

  time(&ttp);
  strftime(timeBuffer, 255, "%H:%M:%S %a %b %d %Y\n", localtime(&ttp));
  fprintf(dest, "%s\n", timeBuffer);
  fprintf(dest, " # lt  Race          Pop   dlt    Ind   dlt   eInd"
          "   dlt  tech  dlt   #  dlt\n");

  for (number = 1; number <= numberOfPlayers; number++) {
    player         *curPlayer, *prevPlayer;

    for (curPlayer = g2->players, prevPlayer = g1->players;
         curPlayer && prevPlayer;
         curPlayer = curPlayer->next, prevPlayer = prevPlayer->next) {
      if (curPlayer->rating eq number) {
        double          deltaTech, sumTech;
        int             delta;

        sumTech = (curPlayer->drive + curPlayer->weapons +
                   curPlayer->shields + curPlayer->cargo);
        deltaTech = (curPlayer->drive + curPlayer->weapons +
                     curPlayer->shields + curPlayer->cargo) -
            (prevPlayer->drive + prevPlayer->weapons +
             prevPlayer->shields + prevPlayer->cargo);
        fprintf(dest, "%2d ", number);

        delta = prevPlayer->rating - number;

        fprintf(dest, "%3d ", delta);

        fprintf(dest, "%-10.10s", curPlayer->name);

        fprintf(dest, "%7.0f", curPlayer->totPop);
        fprintf(dest, "%6.0f", curPlayer->totPop - prevPlayer->totPop);
        fprintf(dest, "%7.0f", curPlayer->totInd);
        fprintf(dest, "%6.0f", curPlayer->totInd - prevPlayer->totInd);
        fprintf(dest, "%7.0f", effectiveIndustry(curPlayer->totPop,
                                                 curPlayer->totInd));
        fprintf(dest, "%6.0f", effectiveIndustry(curPlayer->totPop,
                                                 curPlayer->totInd) -
                effectiveIndustry(prevPlayer->totPop, prevPlayer->totInd));
        fprintf(dest, "%6.2f", sumTech);
        fprintf(dest, "%5.2f", deltaTech);
        fprintf(dest, "%4d", curPlayer->numberOfPlanets);
        fprintf(dest, "%4d\n", curPlayer->numberOfPlanets -
                prevPlayer->numberOfPlanets);
      }
    }
  }
  if (html) {
    fprintf(dest, "</PRE>\n");
    fprintf(dest, "<H3>Legend</H3><UL>\n");
    fprintf(dest, "<LI> # - position\n");
    fprintf(dest, "<LI> lt - change from previous turn\n");
    fprintf(dest, "<LI> Pop - population\n");
    fprintf(dest, "<LI> Ind - Industry\n");
    fprintf(dest, "<LI> eInd - Effective Industry\n");
    fprintf(dest, "<LI> tech - Sum of the technology levels\n");
    fprintf(dest, "<LI> # - number of planets\n");
    fprintf(dest, "<LI> dlt - change as compared to the previous turn\n");
    fprintf(dest, "</UL>\n</BODY>\n</HTML>\n");
  }
  else {
    fprintf(dest, "\n");
  }
}



/****f* Report/rateRaces
 * NAME
 *   rateRaces -- give races a ranking number.
 * SYNOPSIS
 *   void rateRaces(player *playerList)
 * NOTES
 *   If any race manages to get more that 1E20 effective
 *   industry this function will fail.
 *******
 */

void
rateRaces(player *playerList)
{
  player         *curPlayer;
  int             number;

  for (curPlayer = playerList; curPlayer; curPlayer = curPlayer->next) {
    curPlayer->flags &= ~F_SORTED;
  }

  for (number = numberOfElements(playerList); number; number--) {
    player         *lastPlayer;
    double          minEffInd;

    minEffInd = 1E20;
    lastPlayer = NULL;
    for (curPlayer = playerList; curPlayer; curPlayer = curPlayer->next) {
      if (!(curPlayer->flags & F_SORTED)) {
        double          effInd;

        effInd = effectiveIndustry(curPlayer->totPop, curPlayer->totInd);
        if (effInd <= minEffInd) {
          curPlayer->rating = number;
          curPlayer->flags |= F_SORTED;
          if (lastPlayer)
            lastPlayer->flags &= ~F_SORTED;
          lastPlayer = curPlayer;
          minEffInd = effInd;
        }
      }
    }
  }
}


/****f* Report/reportForecast
 * FUNCTION
 *   Create a forecast report.
 *
 ******
 */

#if FS_NEW_FORECAST
void reportForecast( game *aGame, char *raceName, FILE *forecast )
{
    struct fielddef fields;
    player *aPlayer;

    aPlayer = findElement( player, aGame->players, raceName );
    fields.destination = forecast;

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
#endif

/****f* Report/report
 * NAME
 *   report -- create a basic turn report.
 * SYNOPSIS
 *   report(game *aGame, player *P, FILE *report)
 * FUNCTION
 *   Extracts information from the game structure and formats
 *   it in human readable format. 
 *   Two kind of reports are supported, plain galaxy and
 *   galaxy plus.
 * SOURCE
 */

void
report(game *aGame, player *P, FILE * report)
{
  struct fielddef fields;

  pdebug(DFULL, "report\n");

  fields.destination = report;

  if (P->flags & F_GPLUS) {
    fprintf(fields.destination,
            "         %s Report for Galaxy PLUS %s Turn %d\n",
            P->name, aGame->name, aGame->turn);
    fprintf(fields.destination,
            "                Galaxy PLUS version 1.0-NG-GPLUS-Emulator\n\n");
    fprintf(fields.destination,
            "                Size: %.0f  Planets: %d  Players: %d\n\n",
            2 * aGame->galaxysize, numberOfElements(aGame->planets),
            numberOfElements(aGame->players));
  }
  else {
    fprintf(fields.destination, "\t%s\n\n", vcid);
    fprintf(fields.destination,
            "\t\tGalaxy Game %s Turn %d Report for %s\n\n",
            aGame->name, aGame->turn, P->name);
  }

  if (P->pswdstate eq 1) {
    fprintf(fields.destination,
            "\n\nPassword for player %s set to %s\n", P->name, P->pswd);
    fprintf(fields.destination,
            "First line of your orders should now be:\n");
    fprintf(fields.destination, "#galaxy %s %s %s TurnNumber [FinalOrders]\n", aGame->name, P->name,
            P->pswd);
  }
  reportGlobalMessages(aGame->messages, &fields);
  reportMessages(P, &fields);
  reportGameOptions(aGame, &fields);    /* CB-20010401 ; see galaxy.h */
  reportOptions(aGame, P, &fields);
  reportOrders(P, &fields);
  reportMistakes(P, &fields);
  raceStatus(aGame);
  reportStatus(aGame->players, P, &fields);
  reportYourShipTypes(P, &fields);
  reportShipTypes(aGame, P, &fields);
  reportBattles(aGame, P, &fields);
  reportBombings(aGame, P, &fields);
  reportMap(aGame, P, &fields);
  reportIncoming(aGame, P, &fields);
  reportYourPlanets(aGame->planets, P, &fields);
  reportProdTable(aGame->planets, P, &fields);
  reportRoutes(aGame->planets, P, &fields);
  reportPlanetsSeen(aGame, P, &fields);
  reportUnidentifiedPlanets(aGame->planets, P, &fields);
  reportUninhabitedPlanets(aGame->planets, P, &fields);
  reportYourGroups(aGame->planets, P, &fields);
  if (P->flags & F_GPLUS) {
    GreportFleets(aGame->planets, P, &fields);
  }
  else {
    reportFleets(P, &fields);
  }
  reportGroupsSeen(aGame, P, &fields);
}

/*****/


/****f* Report/reportGlobalMessages
 * NAME
 *   reportGlobalMessages --
 ******
 */

void
reportGlobalMessages(strlist *messages, fielddef *fields)
{
  if (messages) {
    strlist        *s;

    fprintf(fields->destination, "\nGLOBAL Messages\n\n");
    for (s = messages; s; s = s->next) {
      fprintf(fields->destination, "%s\n", s->str);
    }
  }
}

/****f* Report/reportMessages
 * NAME
 *   reportMessages --
 ******
 */


void
reportMessages(player *P, fielddef *fields)
{
  if (P->messages) {
    strlist        *s;

    fprintf(fields->destination, "\nPERSONAL Messages for %s\n\n",
            P->name);
    for (s = P->messages; s; s = s->next) {
      fprintf(fields->destination, "%s\n", s->str);
    }
  }
}


/****f* Report/reportOrders
 * NAME
 *   reportOrders --
 ******
 */

void
reportOrders(player *P, fielddef *fields)
{
    if (P->orders) {
        strlist        *s;

        fprintf(fields->destination, "\nORDERS RECEIVED\n\n");
        for (s = P->orders; s; s = s->next) {
            if (*(s->str) == '+') {
                switch(*((s->str)+1)) {
                    case 'I':
                        fprintf(fields->destination, ">   INFO: %s\n", (s->str)+3);
                        break;

                    case 'W':
                        fprintf(fields->destination, ">   WARNING: %s\n", (s->str)+3);
                        break;

                    case 'E':
                        fprintf(fields->destination, ">   ERROR: %s\n", (s->str)+3);
                        break;
                }
            }
            else {
                fprintf(fields->destination, "> %s\n", s->str);
            }
        }
    }
}


/****f* Report/reportMistakes
 * NAME
 *   reportMistakes --
 ******
 */

void
reportMistakes(player *P, fielddef *fields)
{
  if (P->mistakes) {
    strlist        *s;

    s = P->mistakes;
    fprintf(fields->destination, "\nMISTAKES\n\n");
    for (s = P->mistakes; s; s = s->next) {
      fprintf(fields->destination, "%s\n", s->str);
    }
  }
}


/****f* Report/reportStatus
 * NAME
 *   reportStatus --
 ******
 */

void
reportStatus(player *players, player *P, fielddef *fields)
{
  player         *P2;
  int             state;

  fprintf(fields->destination, "\n\t\tStatus of Players\n\n");

  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("N D W S C P I # R", "lcccccccc", fields);
    for (P2 = players; P2; P2 = P2->next) {
      formatString(P2->name, fields);
      formatFloat(P2->drive, fields);
      formatFloat(P2->weapons, fields);
      formatFloat(P2->shields, fields);
      formatFloat(P2->cargo, fields);
      formatFloat(P2->totPop, fields);
      formatFloat(P2->totInd, fields);
      formatInteger(P2->numberOfPlanets, fields);
      if (P2 != P)
        formatString((atwar(P, P2) ? "War" : "Peace"), fields);
      else
        formatString("-", fields);
      formatReturn(fields);
    }
    formatPrint(fields);
  }
}


/****f* Report/reportYourShipTypes
 * NAME
 *   reportYourShipTypes --
 ******
 */

void
reportYourShipTypes(player *P, fielddef *fields)
{
  int             state;

  if (P->shiptypes) {
    fprintf(fields->destination, "\n\t\tYour Ship Types\n\n");
    formatReset(fields);
    for (state = 0; state < 2; state++) {
      shiptype       *t;

      if (P->flags & F_GPLUS) {
        formatLabels("N D A W S C M", "lcccccc", fields);
      }
      else {
        formatLabels("N D A W S C Mass Speed Def", "lcccccccc", fields);
      }
      for (t = P->shiptypes; t; t = t->next) {
        if (P->flags & F_GPLUS) {
          GreportShipType(t, fields);
        }
        else {
          reportShipType(t, fields);
        }
      }
      formatPrint(fields);
    }
  }
}


/****f* Report/reportShipTypes
 * NAME
 *   reportShipTypes --
 ******
 */

void
reportShipTypes(game *aGame, player *P, fielddef *fields)
{
  int             state;
  player         *P2;

  for (P2 = aGame->players; P2; P2 = P2->next) {
    shiptype       *t;

    if (P2 != P) {
      if (visibleShipTypes(aGame, P2, P)) {
        fprintf(fields->destination, "\n\t\t%s Ship Types\n\n", P2->name);

        formatReset(fields);
        for (state = 0; state < 2; state++) {
          if (P->flags & F_GPLUS) {
            formatLabels("N D A W S C M", "lcccccc", fields);
          }
          else {
            formatLabels("N D A W S C Mass Speed Def",
                         "lcccccccc", fields);
          }
          for (t = P2->shiptypes; t; t = t->next) {
            if (t->flag) {
              if (P->flags & F_GPLUS) {
                GreportShipType(t, fields);
              }
              else {
                reportShipType(t, fields);
              }
            }
          }
          formatPrint(fields);
        }
      }
    }
  }
}


/****f* Report/reportShipType
 * NAME
 *   reportShipType --
 ******
 */

void
reportShipType(shiptype *t, fielddef *fields)
{
  formatString(t->name, fields);
  formatFloat(t->drive, fields);
  formatInteger(t->attacks, fields);
  formatFloat(t->weapons, fields);
  formatFloat(t->shields, fields);
  formatFloat(t->cargo, fields);
  formatFloat(typemass(t), fields);
  formatFloat(typeSpeed(t), fields);
  formatFloat(typeDefense(t), fields);
  formatReturn(fields);
}

/****f* Report/GreportShipType
 * NAME
 *   GreportShipType --
 ******
 */

void
GreportShipType(shiptype *t, fielddef *fields)
{
  formatString(t->name, fields);
  formatFloat(t->drive, fields);
  formatInteger(t->attacks, fields);
  formatFloat(t->weapons, fields);
  formatFloat(t->shields, fields);
  formatFloat(t->cargo, fields);
  formatFloat(typemass(t), fields);
  formatReturn(fields);
}

/****f* Report/reportBattles
 * NAME
 *   reportBattles --
 ******
 */

void
reportBattles(game *aGame, player *P, fielddef *fields)
{
  int             state;

  if (aGame->battles) {
    battle         *b;

    for (b = aGame->battles; b; b = b->next) {
      participant    *r;

      for (r = b->participants; r; r = r->next)
        if (r->who eq P)
          break;
      if (r) {
        if (P->flags & F_GPLUS) {
          fprintf(fields->destination, "\n\t\tBattle at (#%d) %s\n",
                  ptonum(aGame->planets, b->where), b->where->name);
        }
        else {
          fprintf(fields->destination, "\n\t\tBattle at %s\n",
                  b->where->name);
        }
        for (r = b->participants; r; r = r->next) {
          player         *P2;

          P2 = r->who;
          if (P2 != P)
            fprintf(fields->destination, "\n\t\t%s Groups\n\n", P2->name);
          else
            fprintf(fields->destination, "\n\t\tYour Groups\n\n");
          formatReset(fields);
          for (state = 0; state < 2; state++) {
            group          *g;

            formatLabels("# T D W S C T Q L", "lcccccccc", fields);
            for (g = r->groups; g; g = g->next) {
              if (P->flags & F_GPLUS) {
                GreportGroup(aGame->planets, P, g, 0,
                             fields, G_MODE_BATTLE, b->phase);
              }
              else {
                reportGroup(g, 0, fields, G_MODE_BATTLE);
              }
            }
            formatPrint(fields);
          }
        }
        if ((P->flags & F_BATTLEPROTOCOL) || (P->flags & F_GPLUS)) {
          int             i;
          shot           *s;

          fprintf(fields->destination, "\n\t\tBattle Protocol\n\n");
          s = b->protocol->shots;
          for (i = 0; i < b->protocol->cur; i++) {
            fprintf(fields->destination, "%s %s fires on %s %s : ",
                    s[i].attacker->name, s[i].atype->name,
                    s[i].target->name, s[i].ttype->name);
            if (s[i].result) {
              fprintf(fields->destination, "Destroyed\n");
            }
            else {
              fprintf(fields->destination, "Shields\n");
            }
          }
        }
      }
    }
  }
}


/****f* Report/reportBombings
 * NAME
 *   reportBombings --
 ******
 */

void
reportBombings(game *aGame, player *P, fielddef *fields)
{
  int             state;

  if (canseeBombing(aGame, P)) {
    fprintf(fields->destination, "\n\t\tBombings\n\n");
    formatReset(fields);
    for (state = 0; state < 2; state++) {
      bombing        *B;
      alliance       *a;

      if (P->flags & F_GPLUS) {
        formatLabels("W O # N P I P $ M C A R", "lccccccccccc", fields);
      }
      else {
        formatLabels("W O N P I P $ M C", "lcccccccc", fields);
      }

      for (B = aGame->bombings; B; B = B->next) {
        for (a = B->viewers; a; a = a->next)
          if (a->who eq P)
            break;
        if (a) {
          formatString(B->who->name, fields);
          formatString(B->owner->name, fields);
          if (P->flags & F_GPLUS) {
            formatInteger(ptonum(aGame->planets, B->where), fields);
          }
          formatString(B->name, fields);
          formatFloat(B->pop, fields);
          formatFloat(B->ind, fields);
          if (B->producing eq PR_SHIP)
            formatString(B->producingshiptype->name, fields);
          else
            formatString(productname[B->producing], fields);
          formatFloat(B->cap, fields);
          formatFloat(B->mat, fields);
          formatFloat(B->col, fields);
          if (P->flags & F_GPLUS) {
            formatFloat(1.0, fields);
            formatString("Whiped", fields);
          }
          formatReturn(fields);
        }
      }
      formatPrint(fields);
    }
  }
}

/****f* Report/reportIncoming
 * NAME
 *   reportIncoming --
 ******
 */

void
reportIncoming(game *aGame, player *P, fielddef *fields)
{
  int             state;
  int             incomming;
  player         *P2;

  incomming = FALSE;
  for (P2 = aGame->players; P2 && !incomming; P2 = P2->next) {
    if (P2 != P) {
      group          *g;

      for (g = P2->groups; g && !incomming; g = g->next) {
        if (g->dist && g->where->owner eq P) {
          incomming = TRUE;
        }
      }
    }
  }
  if (incomming) {
    fprintf(fields->destination, "\n\t\tIncoming Groups\n\n");
    formatReset(fields);
    for (state = 0; state < 2; state++) {
      formatLabels("O D R S M", "lcccc", fields);
      for (P2 = aGame->players; P2; P2 = P2->next) {
        if (P2 != P) {
          group          *g;

          for (g = P2->groups; g; g = g->next) {
            if (g->dist && g->where->owner eq P) {
              if (P->flags & F_GPLUS) {
                GformatInteger(ptonum(aGame->planets, g->from), fields);
                formatString(g->where->name, fields);
                /* GformatInteger(ptonum(planets, g->where), fields); */
              }
              else {
                formatString(g->from->name, fields);
                formatString(g->where->name, fields);
              }
              formatFloat(g->dist, fields);
              if (g->thefleet)
                formatFloat(g->thefleet->fleetspeed, fields);
              else
                formatFloat(g->type->drive * g->drive *
                            DRIVEMAGIC / shipmass(g), fields);
              formatFloat(shipmass(g) * g->ships, fields);
              formatReturn(fields);
            }
          }
        }
      }
      formatPrint(fields);
    }
  }
}


/****f* Report/reportYourPlanets
 * NAME
 *   reportYourPlanets --
 ******
 */

void
reportYourPlanets(planet *planets, player *P, fielddef *fields)
{
  int             state;

  fprintf(fields->destination, "\n\t\tYour Planets\n\n");

  formatReset(fields);
  for (state = 0; state < 2; state++) {
    planet         *p;

    if (P->flags & F_GPLUS) {
      formatLabels("# X Y N S P I R P $ M C L", "lcccccccccccc", fields);
    }
    else {
      formatLabels("N X Y S P I R P $ M C L", "lccccccccccc", fields);
    }
    for (p = planets; p; p = p->next) {
      if (p->owner eq P) {
        if (P->flags & F_GPLUS) {
          GreportPlanet(planets, p, fields);
        }
        else {
          reportPlanet(p, fields);
        }
        formatReturn(fields);
      }
    }
    formatPrint(fields);
  }
}

/****f* Report/reportProdTable
 * NAME
 *   reportProdTable --
 ******
 */

void
reportProdTable(planet *planets, player *P, fielddef *fields)
{
  int             state;
  int             hasproduction;
  planet         *p;

  hasproduction = FALSE;
  for (p = planets; p; p = p->next) {
    if (p->owner eq P && p->producing eq PR_SHIP) {
      hasproduction = TRUE;
      break;
    }
  }

  if ((P->flags & F_PRODTABLE) && hasproduction) {
    fprintf(fields->destination, "\n\t\tShips In Production\n\n");
    formatReset(fields);
    for (state = 0; state < 2; state++) {
      if (P->flags & F_GPLUS) {
        formatLabels("# N S C P L", "lccccc", fields);
      }
      else {
        formatLabels("N P N U", "lccc", fields);
      }
      for (p = planets; p; p = p->next) {
        if (p->owner eq P) {
          if (P->flags & F_GPLUS) {
            GreportProduction(planets, p, fields);
          }
          else {
            reportProduction(p, fields);
          }
        }
      }
      formatPrint(fields);
    }
  }
}

/****f* Report/GreportProduction
 * NAME
 *   GreportProduction --
 ******
 */

void
GreportProduction(planet *planets, planet *p, fielddef *fields)
{
  double          mass;
  double          theshipmass;
  double          prog;

  if (p->producing eq PR_SHIP) {
    formatInteger(ptonum(planets, p), fields);
    formatString(p->name, fields);
    formatString(p->producingshiptype->name, fields);
    theshipmass = typemass(p->producingshiptype);
    mass = theshipmass * INDPERSHIP;
    prog = p->inprogress;
    if (theshipmass > p->mat)
      mass += (theshipmass - p->mat) / p->resources;
    formatFloat(mass, fields);
    formatFloat(prog, fields);
    formatFloat(effectiveIndustry(p->pop, p->ind), fields);
    formatReturn(fields);
  }
}


/****f* Report/reportProduction
 * NAME
 *   reportProduction --
 ******
 */

void
reportProduction(planet *p, fielddef *fields)
{
  double          mass;
  double          theshipmass;
  double          prog;

  if (p->producing eq PR_SHIP) {
    formatString(p->name, fields);
    formatString(p->producingshiptype->name, fields);
    theshipmass = typemass(p->producingshiptype);
    mass = theshipmass * INDPERSHIP;
    prog = p->inprogress;
    if (theshipmass > p->mat)
      mass += (theshipmass - p->mat) / p->resources;
    formatFloat(mass, fields);
    formatFloat(prog, fields);
    formatReturn(fields);
  }
}


/****f* Report/reportRoutes
 * NAME
 *   reportRoutes --
 ******
 */



void
reportRoutes(planet *planets, player *P, fielddef *fields)
{
  int             state;
  int             hasroutes;
  planet         *p;

  hasroutes = FALSE;
  for (p = planets; p; p = p->next) {
    if (p->owner eq P && (p->routes[0] || p->routes[1] ||
                          p->routes[2] || p->routes[3])) {
      hasroutes = TRUE;
      break;
    }
  }

  if (hasroutes) {
    fprintf(fields->destination, "\n\t\tYour Routes\n\n");
    formatReset(fields);
    for (state = 0; state < 2; state++) {

      formatLabels("N $ M C E", "lllll", fields);
      for (p = planets; p; p = p->next) {
        if (p->owner eq P && (p->routes[0] || p->routes[1] ||
                              p->routes[2] || p->routes[3])) {
          int             i;

          formatString(p->name, fields);
          for (i = 0; i != MAXCARGO; i++) {
            if (p->routes[i])
              formatString(p->routes[i]->name, fields);
            else
              formatString("-", fields);
          }
          formatReturn(fields);
        }
      }
      formatPrint(fields);
    }
  }
}

/****f* Report/reportPlanetsSeen
 * NAME
 *   reportPlanetsSeen --
 ******
 */

void
reportPlanetsSeen(game *aGame, player *P, fielddef *fields)
{
  int             state;
  player         *P2;

  for (P2 = aGame->players; P2; P2 = P2->next) {
    if (P2 != P) {
      planet         *p;
      int             cansee;

      cansee = FALSE;
      for (p = aGame->planets; p; p = p->next) {
        if (p->owner eq P2 && canseeplanet(P, p)) {
          cansee = TRUE;
          break;
        }
      }
      if (cansee) {
        fprintf(fields->destination, "\n\t\t%s Planets\n\n", P2->name);
        formatReset(fields);
        for (state = 0; state < 2; state++) {
          planet         *p;

          if (P->flags & F_GPLUS) {
            formatLabels("# X Y N S P I R P $ M C L",
                         "lcccccccccccc", fields);
          }
          else {
            formatLabels("N X Y S P I R P $ M C L",
                         "lccccccccccc", fields);
          }
          for (p = aGame->planets; p; p = p->next) {
            if (p->owner eq P2 && canseeplanet(P, p)) {
              if (P->flags & F_GPLUS) {
                GreportPlanet(aGame->planets, p, fields);
              }
              else {
                reportPlanet(p, fields);
              }
              formatReturn(fields);
            }
          }
          formatPrint(fields);
        }
      }
    }
  }
}

/****f* Report/reportUnidentifiedPlanets
 * NAME
 *   reportUnidentifiedPlanets --
 ******
 */


void
reportUnidentifiedPlanets(planet *planets, player *P, fielddef *fields)
{
  int             state;

  fprintf(fields->destination, "\n\t\tUnidentified Planets\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    planet         *p;

    if (P->flags & F_GPLUS) {
      formatLabels("# X Y", "lcc", fields);
    }
    else {
      formatLabels("N X Y", "lcc", fields);
    }
    for (p = planets; p; p = p->next) {
      if (isunidentified(P, p)) {
        if (P->flags & F_GPLUS) {
          formatInteger(ptonum(planets, p), fields);
        }
        else {
          formatString(p->name, fields);
        }
        formatFloat(p->x, fields);
        formatFloat(p->y, fields);
        formatReturn(fields);
      }
    }
    formatPrint(fields);
  }
}

/****f* Report/reportUninhabitedPlanets
 * NAME
 *   reportUninhabitedPlanets --
 ******
 */

void
reportUninhabitedPlanets(planet *planets, player *P, fielddef *fields)
{
  int             state;

  fprintf(fields->destination, "\n\t\tUninhabited Planets\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    planet         *p;

    if (P->flags & F_GPLUS) {
      formatLabels("# X Y N S R $ M", "lccccccc", fields);
    }
    else {
      formatLabels("N X Y S R $ M", "lcccccc", fields);
    }

    for (p = planets; p; p = p->next) {
      if (isuninhabited(P, p)) {
        if (P->flags & F_GPLUS) {
          formatInteger(ptonum(planets, p), fields);
        }
        else {
          formatString(p->name, fields);
        }
        formatFloat(p->x, fields);
        formatFloat(p->y, fields);
        if (P->flags & F_GPLUS) {
          formatString(p->name, fields);
        }
        if (canseeplanet(P, p)) {
          formatFloat(p->size, fields);
          formatFloat(p->resources, fields);
          formatFloat(p->cap, fields);
          formatFloat(p->mat, fields);
        }
        else {
          if (P->flags & F_GPLUS) {
            formatString("0", fields);
            formatString("0", fields);
            formatString("0", fields);
            formatString("0", fields);
          }
          else {
            formatString("", fields);
            formatString("", fields);
            formatString("", fields);
            formatString("", fields);
          }
        }
        formatReturn(fields);
      }
    }
    formatPrint(fields);
  }
}


/****f* Report/reportYourGroups
 * NAME
 *   reportYourGroups --
 ******
 */

void
reportYourGroups(planet *planets, player *P, fielddef *fields)
{
  int             state;

  if (P->groups) {
    fprintf(fields->destination, "\n\t\tYour Groups\n\n");
    formatReset(fields);
    for (state = 0; state < 2; state++) {
      group          *g;
      planet         *p;

      if (P->flags & F_GPLUS) {
        formatLabels("G # T D W S C T Q D F R P M L S",
                     "lccccccccccccccc", fields);
      }
      else {
        formatLabels("G # T D W S C T Q D R O", "lccccccccccc", fields);
      }
      for (p = planets; p; p = p->next) {
        if (p->owner eq P) {
          for (g = P->groups; g; g = g->next) {
            if (!g->thefleet || (P->flags & F_GPLUS)) {
              if (g->location eq p) {
                if (P->flags & F_GPLUS) {
                  GreportGroup(planets, P, g, g->number,
                               fields, G_MODE_OWN, 0);
                }
                else {
                  reportGroup(g, g->number, fields, G_MODE_OWN);
                }
              }
            }
          }
          for (g = P->groups; g; g = g->next) {
            if (!g->thefleet || (P->flags & F_GPLUS)) {
              if (!g->location && g->where eq p) {
                if (P->flags & F_GPLUS) {
                  GreportGroup(planets, P, g, g->number,
                               fields, G_MODE_OWN, 0);
                }
                else {
                  reportGroup(g, g->number, fields, G_MODE_OWN);
                }
              }
            }
          }
        }
      }
      for (p = planets; p; p = p->next) {
        if (p->owner && p->owner != P) {
          for (g = P->groups; g; g = g->next) {
            if (!g->thefleet || (P->flags & F_GPLUS)) {
              if (g->location eq p) {
                if (P->flags & F_GPLUS) {
                  GreportGroup(planets, P, g, g->number,
                               fields, G_MODE_OWN, 0);
                }
                else {
                  reportGroup(g, g->number, fields, G_MODE_OWN);
                }
              }
            }
          }
          for (g = P->groups; g; g = g->next) {
            if (!g->thefleet || (P->flags & F_GPLUS)) {
              if (!g->location && g->where eq p) {
                if (P->flags & F_GPLUS) {
                  GreportGroup(planets, P, g, g->number,
                               fields, G_MODE_OWN, 0);
                }
                else {
                  reportGroup(g, g->number, fields, G_MODE_OWN);
                }
              }
            }
          }
        }
      }
      for (p = planets; p; p = p->next) {
        if (!p->owner) {
          for (g = P->groups; g; g = g->next) {
            if (!g->thefleet || (P->flags & F_GPLUS)) {
              if (g->location eq p) {
                if (P->flags & F_GPLUS) {
                  GreportGroup(planets, P, g, g->number,
                               fields, G_MODE_OWN, 0);
                }
                else {
                  reportGroup(g, g->number, fields, G_MODE_OWN);
                }
              }
            }
          }
          for (g = P->groups; g; g = g->next) {
            if (!g->thefleet || (P->flags & F_GPLUS)) {
              if (!g->location && g->where eq p) {
                if (P->flags & F_GPLUS) {
                  GreportGroup(planets, P, g, g->number,
                               fields, G_MODE_OWN, 0);
                }
                else {
                  reportGroup(g, g->number, fields, G_MODE_OWN);
                }
              }
            }
          }
        }
      }
      formatPrint(fields);
    }
  }
}

/****f* Report/GreportFleets
 * NAME
 *   GreportFleets --
 ******
 */

void
GreportFleets(planet *planets, player *P, fielddef *fields)
{
  int             state;

  if (P->fleetnames) {
    fleetname      *fl;

    fprintf(fields->destination, "\n\t\tYour Fleets\n\n");
    formatReset(fields);
    for (state = 0; state < 2; state++) {
      int             i;

      formatLabels("# N G D F R P", "lccccccc", fields);
      for (i = 1, fl = P->fleetnames; fl; fl = fl->next, i++) {
        group          *g;

        for (g = P->groups; g; g = g->next) {
          if (g->thefleet eq fl)
            break;
        };
        formatInteger(i, fields);
        formatString(fl->name, fields);
        formatInteger(numOfGroupsInFleet(fl, P->groups), fields);
        if (g) {
          if (canseeplanet(P, g->where)) {
            formatString(g->where->name, fields);
          }
          else {
            GformatInteger(ptonum(planets, g->where), fields);
          }
          if (g->dist) {
            formatString(g->from->name, fields);
            formatFloat(g->dist, fields);
          }
          else {
            formatString("-", fields);
            formatString("-", fields);
          }
        }
        else {
          formatString("BUG!", fields);
          formatString("BUG!", fields);
          formatString("BUG!", fields);
        }
        formatFloat(fl->fleetspeed, fields);
        formatReturn(fields);
      }
      formatPrint(fields);
    }
  }
}

/****f* Report/reportFleets
 * NAME
 *   reportFleets --
 ******
 */

void
reportFleets(player *P, fielddef *fields)
{
  int             state;

  if (P->fleetnames) {
    fleetname      *fl;

    for (fl = P->fleetnames; fl; fl = fl->next) {
      fprintf(fields->destination,
              "\n\t\tFleet %s (speed %.2f)\n\n", fl->name, fl->fleetspeed);
      formatReset(fields);
      for (state = 0; state < 2; state++) {
        group          *g;

        formatLabels("G # T D W S C T Q D R O", "lccccccccccc", fields);
        for (g = P->groups; g; g = g->next) {
          if (g->thefleet eq fl)
            reportGroup(g, g->number, fields, G_MODE_OWN);
        }
        formatPrint(fields);
      }
    }
  }
}

/****f* Report/reportGroupsSeen
 * NAME
 *   reportGroupsSeen --
 ******
 */

void
reportGroupsSeen(game *aGame, player *P, fielddef *fields)
{
  int             state;
  player         *P2;

  for (P2 = aGame->players; P2; P2 = P2->next) {
    if (P2 != P) {
      int             cansee;
      group          *g;

      cansee = FALSE;
      for (g = P2->groups; g; g = g->next) {
        if (canseegroup(P, g)) {
          cansee = TRUE;
          break;
        }
      }
      if (cansee) {
        fprintf(fields->destination, "\n\t\t%s Groups\n\n", P2->name);
        formatReset(fields);
        for (state = 0; state < 2; state++) {
          group          *g;

          if (P->flags & F_GPLUS) {
            formatLabels("# T D W S C T Q D P M", "lcccccccccc", fields);
          }
          else {
            formatLabels("# T D W S C T Q D", "lcccccccc", fields);
          }
          for (g = P2->groups; g; g = g->next) {
            if (canseegroup(P, g)) {
              if (P->flags & F_GPLUS) {
                GreportGroup(aGame->planets, P, g, 0,
                             fields, G_MODE_ALIEN, 0);
              }
              else {
                reportGroup(g, 0, fields, G_MODE_ALIEN);
              }
            }
          }
          formatPrint(fields);
        }
      }
    }
  }
}

/****f* Report/reportGroup
 * NAME
 *   reportGroup --
 ******
 */

void
reportGroup(group *g, int n, fielddef *fields, int mode)
{
  static char    *loadtypename[] = {
    "CAP",
    "MAT",
    "COL",
    "-",
  };

  if (mode eq G_MODE_OWN)
    formatInteger(n, fields);
  formatInteger(g->ships, fields);
  formatString(g->type->name, fields);
  formatFloat(g->drive, fields);
  formatFloat(g->weapons, fields);
  formatFloat(g->shields, fields);
  formatFloat(g->cargo, fields);
  formatString(loadtypename[g->loadtype], fields);
  formatFloat(g->load, fields);
  if (mode != G_MODE_BATTLE)
    formatString(g->where->name, fields);
  if (mode eq G_MODE_OWN) {
    if (g->dist) {
      formatFloat(g->dist, fields);
      formatString(g->from->name, fields);
    }
    else {
      formatString("", fields);
      formatString("", fields);
    }
  }
  if (mode eq G_MODE_BATTLE)
    formatInteger(g->left, fields);

  formatReturn(fields);
}


/****f* Report/GreportGroup
 * NAME
 *   GreportGroup --
 ******
 */

void
GreportGroup(planet *planets, player *P, group *g, int n,
             fielddef *fields, int mode, int phase)
{
  static char    *loadtypename[] = {
    "CAP", "MAT", "COL", "-"
  };

  if (mode eq G_MODE_OWN)
    formatInteger(n, fields);
  formatInteger(g->ships, fields);
  formatString(g->type->name, fields);
  formatFloat(g->drive, fields);
  formatFloat(g->weapons, fields);
  formatFloat(g->shields, fields);
  formatFloat(g->cargo, fields);
  formatString(loadtypename[g->loadtype], fields);
  formatFloat(g->load, fields);
  if (mode != G_MODE_BATTLE) {
    if (canseeplanet(P, g->where)) {
      formatString(g->where->name, fields);
    }
    else {
      GformatInteger(ptonum(planets, g->where), fields);
    }
  }
  if (mode eq G_MODE_OWN) {
    if (g->dist) {
      formatString(g->from->name, fields);
      formatFloat(g->dist, fields);
    }
    else {
      formatString("-", fields);
      formatString("-", fields);
    }
    formatFloat(groupSpeed(g), fields);
    formatFloat(shipmass(g), fields);
    if (!g->thefleet) {
      formatString("-", fields);
    }
    else {
      formatString(g->thefleet->name, fields);
    }
    if (g->dist) {
      formatString("In_Space", fields);
    }
    else {
      formatString("In_Orbit", fields);
    }
  }
  if (mode eq G_MODE_ALIEN) {
    formatFloat(groupSpeed(g), fields);
    formatFloat(shipmass(g), fields);
  }
  if (mode eq G_MODE_BATTLE) {
    formatInteger(g->left, fields);
    if (g->flags & phase) {
      formatString("In_Battle", fields);
    }
    else {
      formatString("Out_Battle", fields);
    }
  }
  formatReturn(fields);
}





/****f* Report/reportPlanet
 * NAME
 *   reportPlanet --
 ******
 */

void
reportPlanet(planet *p, fielddef *fields)
{

  formatString(p->name, fields);
  formatFloat(p->x, fields);
  formatFloat(p->y, fields);
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
  formatFloat(effectiveIndustry(p->pop, p->ind), fields);
}


/****f* Report/GreportPlanet
 * NAME
 *   GreportPlanet --
 ******
 */

void
GreportPlanet(planet *planets, planet *p, fielddef *fields)
{
  formatInteger(ptonum(planets, p), fields);
  formatFloat(p->x, fields);
  formatFloat(p->y, fields);
  formatString(p->name, fields);
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
  formatFloat(effectiveIndustry(p->pop, p->ind), fields);
}


/****f* Report/canseeBombing
 * NAME
 *   canseeBombing --
 ******
 */

int
canseeBombing(game *aGame, player *P)
{
  int             cansee;
  bombing        *B;
  alliance       *a;

  cansee = FALSE;

  for (B = aGame->bombings; B; B = B->next) {
    for (a = B->viewers; a; a = a->next)
      if (a->who eq P) {
        cansee = TRUE;
        break;
      }
  }
  return cansee;
}

/****f* Report/visibleShipTypes
 * NAME
 *   visibleShipTypes --
 ******
 */


int
visibleShipTypes(game *aGame, player *P2, player *P)
{
  planet         *p;
  shiptype       *t;
  battle         *b;
  participant    *r;
  bombing        *B;
  group          *g;
  alliance       *a;
  int             status;

  for (t = P2->shiptypes; t; t = t->next)
    t->flag = 0;
  for (b = aGame->battles; b; b = b->next) {
    for (r = b->participants; r; r = r->next) {
      if (r->who eq P)
        break;
    }
    if (r) {
      for (r = b->participants; r; r = r->next) {
        if (r->who eq P2)
          break;
      }
      if (r) {
        for (g = r->groups; g; g = g->next) {
          g->type->flag = 1;
        }
      }
    }
  }
  for (g = P2->groups; g; g = g->next) {
    if (canseegroup(P, g))
      g->type->flag = 1;
  }
  for (p = aGame->planets; p; p = p->next) {
    if (p->owner eq P2 && p->producing eq PR_SHIP && canseeplanet(P, p)) {
      p->producingshiptype->flag = 1;
    }
  }
  for (B = aGame->bombings; B; B = B->next) {
    for (a = B->viewers; a; a = a->next) {
      if (a->who eq P)
        break;
    }
    if (a && B->producing eq PR_SHIP) {
      B->producingshiptype->flag = 1;
    }
  }
  status = FALSE;
  for (t = P2->shiptypes; t; t = t->next) {
    if (t->flag) {
      status = TRUE;
      break;
    }
  }
  return status;
}

/****f* Report/formatLabels
 * NAME
 *   formatLabels --
 ******
 */

void
formatLabels(char *labels, char *align, fielddef *fields)
{
  char           *ns;

  for (ns = getstr(labels); *ns && *align; ns = getstr(0), align++) {
    switch (*align) {
    case 'l':
      formatStringMode(ns, fields, M_ALIGN_LEFT);
      break;
    case 'r':
      formatStringMode(ns, fields, M_ALIGN_RIGHT);
      break;
    case 'c':
      formatStringMode(ns, fields, M_ALIGN_CENTER);
      break;
    }
  }
  formatReturn(fields);
}


/****f* Report/formatPrint
 * NAME
 *   formatPrint --
 ******
 */

void
formatPrint(fielddef *fields)
{
  fields->format = FALSE;
}

/****f* Report/formatReset
 * NAME
 *   formatReset --
 ******
 */

void
formatReset(fielddef *fields)
{
  int             fieldNumber;

  fields->format = TRUE;
  fields->fieldNumber = 0;
  for (fieldNumber = 0; fieldNumber < MAXNOFIELDS; fieldNumber++)
    fields->fieldSizes[fieldNumber] = 0;
}

/****f* Report/storeLength
 * NAME
 *   storeLength --
 ******
 */

void
storeLength(fielddef *fields, int length)
{
  if (length > fields->fieldSizes[fields->fieldNumber]) {
    fields->fieldSizes[fields->fieldNumber] = length;
  }
  fields->fieldNumber++;
  assert(fields->fieldNumber < MAXNOFIELDS);
}

/****f* Report/formatChar
 * NAME
 *   formatChar --
 ******
 */

void
formatChar(char c, fielddef *fields, int mode)
{
  sprintf(formatBuffer, "%c", c);
  if (fields->format) {
    storeLength(fields, strlen(formatBuffer));
  }
  else {
    dumpItem(fields, mode);
  }
}

/****f* Report/formatStringMode
 * NAME
 *   formatStringMode --
 ******
 */

void
formatStringMode(char *s, fielddef *fields, int mode)
{
  sprintf(formatBuffer, "%s", s);
  if (fields->format) {
    storeLength(fields, strlen(formatBuffer));
  }
  else {
    dumpItem(fields, mode);
  }
}

/****f* Report/formatString
 * NAME
 *   formatString
 ******
 */

void
formatString(char *s, fielddef *fields)
{
  sprintf(formatBuffer, "%s", s);
  if (fields->format) {
    storeLength(fields, strlen(formatBuffer));
  }
  else {
    dumpItem(fields, M_ALIGN_LEFT);
  }
}

/****f* Report/formatStringCenter
 * NAME
 *   formatStringCenter --
 ******
 */


void
formatStringCenter(char *s, fielddef *fields)
{
  sprintf(formatBuffer, "%s", s);
  if (fields->format) {
    storeLength(fields, strlen(formatBuffer));
  }
  else {
    dumpItem(fields, M_ALIGN_CENTER);
  }
}

/****f* Report/formatFloat
 * NAME
 *   formatFloat --
 ******
 */


void
formatFloat(double g, fielddef *fields)
{
  sprintf(formatBuffer, "%.2f", g);
  if (fields->format) {
    storeLength(fields, strlen(formatBuffer));
  }
  else {
    dumpItem(fields, M_ALIGN_RIGHT);
  }
}


/****f* Report/formatInteger
 * NAME
 *   formatInteger --
 ******
 */

void
formatInteger(int i, fielddef *fields)
{
  BformatInteger(i, fields, "%d");
}


/****f* Report/GformatInteger
 * NAME
 *   GformatInteger --
 ******
 */

void
GformatInteger(int i, fielddef *fields)
{
  BformatInteger(i, fields, "#%d");
}


/****f* Report/BformatInteger
 * NAME
 *   BformatInteger --
 ******
 */

void
BformatInteger(int i, fielddef *fields, char *form)
{
  sprintf(formatBuffer, form, i);
  if (fields->format) {
    storeLength(fields, strlen(formatBuffer));
  }
  else {
    dumpItem(fields, M_ALIGN_RIGHT);
  }
}

/****f* Report/formatReturn
 * NAME
 *   formatReturn --
 ******
 */

void
formatReturn(fielddef *fields)
{
  fields->fieldNumber = 0;
  if (!fields->format) {
    fprintf(fields->destination, "\n");
  }
}


/****f* Report/dumpItem
 * NAME
 *   dumpItem --
 ******
 */

void
dumpItem(fielddef *fields, int mode)
{
  int             length;
  int             diff;

  length = strlen(formatBuffer);
  assert(length <= fields->fieldSizes[fields->fieldNumber]);
  diff = fields->fieldSizes[fields->fieldNumber] - length;
  switch (mode) {
  case M_ALIGN_LEFT:
    {
      fprintf(fields->destination, "%s", formatBuffer);
      for (; diff > 0; diff--)
        fprintf(fields->destination, " ");
      fprintf(fields->destination, " ");
      break;
    }
  case M_ALIGN_RIGHT:
    {
      for (; diff > 0; diff--)
        fprintf(fields->destination, " ");
      fprintf(fields->destination, "%s", formatBuffer);
      fprintf(fields->destination, " ");
      break;

    }
  case M_ALIGN_CENTER:
    {
      int             diff1;

      diff1 = diff / 2;
      diff = diff - diff / 2;
      for (; diff > 0; diff--)
        fprintf(fields->destination, " ");
      fprintf(fields->destination, "%s", formatBuffer);
      for (; diff1 > 0; diff1--)
        fprintf(fields->destination, " ");
      fprintf(fields->destination, " ");
      break;
    }
  }
  fields->fieldNumber++;
  assert(fields->fieldNumber < MAXNOFIELDS);
}

/****f* Report/reportGameOptions
 * NAME
 *   reportGameOptions --
 ******
 */

void
reportGameOptions(game *aGame, fielddef *fields)
{                               /* CB-20010401 ; see galaxy.h */
  int             state;

  fprintf(fields->destination, "\n\t\tGame Options\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("N S", "lc", fields);

    formatString("Full bombing", fields);
    if (aGame->gameOptions.gameOptions & GAME_NONGBOMBING) {
      formatString("ON", fields);
    }
    else {
      formatString("OFF", fields);
    }
    formatReturn(fields);

    formatString("Keep production", fields);
    if (aGame->gameOptions.gameOptions & GAME_KEEPPRODUCTION) {
      formatString("ON", fields);
    }
    else {
      formatString("OFF", fields);
    }
    formatReturn(fields);

    formatString("Drop idle players", fields);
    if (aGame->gameOptions.gameOptions & GAME_NODROP) {
      formatString("OFF", fields);
    }
    else {
      formatString("ON", fields);
    }
    formatReturn(fields);

    formatString("Spherical Galaxy", fields);
    if (aGame->gameOptions.gameOptions & GAME_SPHERICALGALAXY) {
      formatString("ON", fields);
    }
    else {
      formatString("OFF", fields);
    }
    formatReturn(fields);

    formatPrint(fields);

  }

  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatString("InitialTechLevels", fields);
    formatFloat(aGame->gameOptions.initial_drive, fields);
    formatFloat(aGame->gameOptions.initial_weapons, fields);
    formatFloat(aGame->gameOptions.initial_shields, fields);
    formatFloat(aGame->gameOptions.initial_cargo, fields);
    formatReturn(fields);
    formatPrint(fields);
  }
}

/****f* Report/reportOptions
 * NAME
 *   reportOptions --
 ******
 */

void
reportOptions(game *aGame, player *P, fielddef *fields)
{
  int             state;
  option         *curOption;

  fprintf(fields->destination, "\n\t\tYour Options\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("N S", "lc", fields);
    {
      for (curOption = options; curOption->optionName; curOption++) {
        formatString(curOption->optionName, fields);
        if (P->flags & curOption->optionMask)
          formatString("ON", fields);
        else
          formatString("OFF", fields);
        formatReturn(fields);
      }
    }
    formatPrint(fields);
  }
}


/****f* Report/reportMap_gnuplot
 * NAME
 *   reportMap_gnuplot --
 * AUTHOR: Steven Webb (steve@badcheese.com)
 ******
 */

void
reportMap_gnuplot(game *aGame, player *P, fielddef *fields)
{
  player         *P2;
  group          *g;
  planet         *p;
  mapdimensions   mapDim;

  mapDim.x1 = P->mx;
  mapDim.x2 = P->mx + P->msize;
  mapDim.y1 = P->my;
  mapDim.y2 = P->my + P->msize;

  /* png */

  fprintf(fields->destination, "set output 'plot.png'\n");
  fprintf(fields->destination, "set term png color small\n");
  fprintf(fields->destination, "set nokey\n");
  fprintf(fields->destination, "set grid x y\n");
  fprintf(fields->destination, "set size 1.7,1.7\n");
  fprintf(fields->destination, "set xrange [%.2f:%.2f]\n", mapDim.x1,
          mapDim.x2);
  fprintf(fields->destination, "set yrange [%.2f:%.2f]\n", mapDim.y2,
          mapDim.y1);

  memset(map, ' ', sizeof map);

  /* Not needed for now - just debugging stuff. for (p = aGame->planets;
   * p; p = p->next) fprintf(fields->destination, "set label \" %s\" at
   * %.2f, %.2f\n",p->name,p->x,p->y); */

  fprintf(fields->destination, "plot '-','-' index 0:1\n");
  for (p = aGame->planets; p; p = p->next)
    fprintf(fields->destination, "%.2f %.2f\n", p->x, p->y);
  fprintf(fields->destination, "e\n");

  for (P2 = aGame->players; P2; P2 = P2->next)
    if (P2 != P)
      for (g = P2->groups; g; g = g->next)
        if (groupLocation(aGame, g) == NULL)
          fprintf(fields->destination, "%.2f %.2f\n", groupx(aGame, g),
                  groupy(aGame, g));
  fprintf(fields->destination, "e\n");
}


/****f* Report/reportMap
 * NAME
 *   reportMap --
 ******
 */

void
reportMap(game *aGame, player *P, fielddef *fields)
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
	
	
	fprintf(fields->destination, "\n%.2f,%.2f  ", mapDim.x1, mapDim.y1);
	fprintf(fields->destination, "%.2f,%.2f\n", mapDim.x2, mapDim.y1);
	
	for (i = 0; i != MAPWIDTH; i++)
		fprintf(fields->destination, "-");
	fprintf(fields->destination, "\n");
	
	memset(map, ' ', sizeof map);
	
	for (p = aGame->planets; p; p = p->next)
		if (!p->owner)
			putmap(&mapDim, p->x, p->y, 'o');
	
	for (p = aGame->planets; p; p = p->next)
		if (p->owner && p->owner != P)
			putmap(&mapDim, p->x, p->y, '+');
	
	for (p = aGame->planets; p; p = p->next)
		if (p->owner eq P)
			putmap(&mapDim, p->x, p->y, '*');
	
	for (g = P->groups; g; g = g->next)
		if (groupLocation(aGame, g) == NULL)
			putmap(&mapDim, groupx(aGame, g), groupy(aGame, g), '.');
	
	for (P2 = aGame->players; P2; P2 = P2->next)
		if (P2 != P)
			for (g = P2->groups; g; g = g->next)
				if (groupLocation(aGame, g) == NULL)
					putmap(&mapDim, groupx(aGame, g), groupy(aGame, g), '-');
	
	for (i = 0; i != MAPHEIGHT; i++) {
		for (j = 0; j != MAPWIDTH; j++) {
			fprintf(fields->destination, "%c", map[j][i]);
		}
		fprintf(fields->destination, "\n");
	}
	
	for (i = 0; i != MAPWIDTH; i++)
		fprintf(fields->destination, "-");
	fprintf(fields->destination, "\n");
	
	
	fprintf(fields->destination, "%.2f,%.2f  ", mapDim.x1, mapDim.y2);
	fprintf(fields->destination, "%.2f,%.2f\n", mapDim.x2, mapDim.y2);
}


/****f* Report/tagVisiblePlanets
 * NAME
 *   tagVisiblePlanets
 ******
 */


void
tagVisiblePlanets(planet *planets, player *P)
{
  planet         *aPlanet;

  for (aPlanet = planets; aPlanet; aPlanet = aPlanet->next) {
    if ((aPlanet->owner eq P) || canseeplanet(P, aPlanet))
      aPlanet->flags |= PL_VISPREVTURN;
    else
      aPlanet->flags &= ~PL_VISPREVTURN;
  }
}


/****f* Report/yourStatusForecast
 * NAME
 *   yourStatusForecast
 ******
 */

void
yourStatusForecast(planet *planets, player *P, fielddef *fields)
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
yourPlanetsForecast(planet *planets, player *P, fielddef *fields)
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


/****f* GalaxyNG/reportHall
 * NAME
 *   reportHall -- report hall of fame information.
 *****
 */

void
reportHall(game *aGame, fielddef *fields)
{
  int             state;
  player         *aPlayer;

  raceStatus(aGame);

  fprintf(fields->destination, "\n\t\tHall Of Fame Info %s\n\n",
          aGame->name);
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("Race Real_Name Address EffInd", "llll", fields);
    for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
      if (!(aPlayer->flags & F_DEAD)) {
        formatString(aPlayer->name, fields);
        formatString(aPlayer->realName, fields);
        formatString(aPlayer->addr, fields);
        formatFloat(effectiveIndustry(aPlayer->totPop, aPlayer->totInd),
                    fields);
        formatReturn(fields);
      }
    }
    formatPrint(fields);
  }
}


/****f* GalaxyNG/reportTeam
 * NAME
 *   reportTeam -- report statistics about team players to team leader
 *
 ****/

void
reportTeam(game *aGame, fielddef *fields, int team)
{
  int             state;
  player         *aPlayer;
  double          tot_eff_ind = 0.0;
  double          tot_massproduced = 0.0;
  double          tot_masslost = 0.0;
  double          tot_delta = 0.0;

  raceStatus(aGame);

  fprintf(fields->destination,
          "\n\t\tTeam Leader Status Report for Game %s\n\n", aGame->name);

  fprintf(fields->destination, "\n\t\tTeam Address Info\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("Race Address Password Last_Orders", "lllll", fields);
    for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
      if (!(aPlayer->flags & F_DEAD) && (aPlayer->team == team)) {
        formatString(aPlayer->name, fields);
        formatString(aPlayer->addr, fields);
        formatString(aPlayer->pswd, fields);
        formatInteger(aPlayer->lastorders, fields);
        formatReturn(fields);
      }
    }
    formatPrint(fields);
  }

  fprintf(fields->destination, "\n\t\tTeam Ship Production Info\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("Race Eff_Ind Produced Lost Delta", "lrrrr", fields);
    tot_eff_ind = 0.0;
    tot_massproduced = 0.0;
    tot_masslost = 0.0;
    tot_delta = 0.0;
    for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
      if (!(aPlayer->flags & F_DEAD) && (aPlayer->team == team)) {
        formatString(aPlayer->name, fields);
        formatFloat(effectiveIndustry(aPlayer->totPop, aPlayer->totInd),
                    fields);
        formatFloat(aPlayer->massproduced, fields);
        formatFloat(aPlayer->masslost, fields);
        formatFloat(aPlayer->massproduced - aPlayer->masslost, fields);
        formatReturn(fields);
        tot_eff_ind += effectiveIndustry(aPlayer->totPop, aPlayer->totInd);
        tot_massproduced += aPlayer->massproduced;
        tot_masslost += aPlayer->masslost;
        tot_delta += aPlayer->massproduced - aPlayer->masslost;
      }
    }
    formatString("Total", fields);
    formatFloat(tot_eff_ind, fields);
    formatFloat(tot_massproduced, fields);
    formatFloat(tot_masslost, fields);
    formatFloat(tot_delta, fields);
    formatReturn(fields);
    formatPrint(fields);
  }


  fprintf(fields->destination, "\n\t\tTeam Technology Info\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("Race Drive Weapons Shields Cargo", "lrrrr", fields);
    for (aPlayer = aGame->players; aPlayer; aPlayer = aPlayer->next) {
      if (!(aPlayer->flags & F_DEAD) && (aPlayer->team == team)) {
        formatString(aPlayer->name, fields);
        formatFloat(aPlayer->drive, fields);
        formatFloat(aPlayer->weapons, fields);
        formatFloat(aPlayer->shields, fields);
        formatFloat(aPlayer->cargo, fields);
        formatReturn(fields);
      }
    }
    formatPrint(fields);
  }
}


void
reportPlayers(player *players, fielddef *fields)
{
  int             state;
  player         *aPlayer;

  fprintf(fields->destination, "\n\t\tPlayer Info\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("Race Password Address Status", "llll", fields);
    for (aPlayer = players; aPlayer; aPlayer = aPlayer->next) {
      formatString(aPlayer->name, fields);
      formatString(aPlayer->pswd, fields);
      formatString(aPlayer->addr, fields);
      if (aPlayer->flags & F_DEAD) {
        formatString("inactive", fields);
      }
      else {
        formatString("active", fields);
      }
      formatReturn(fields);
    }
    formatPrint(fields);
  }
}

void
reportRaceProduction(player *players, fielddef *fields)
{
  int             state;
  player         *aPlayer;

  fprintf(fields->destination, "\n\t\tRace Production Info\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("Race Produced Lost Delta", "llll", fields);
    for (aPlayer = players; aPlayer; aPlayer = aPlayer->next) {
      formatString(aPlayer->name, fields);
      formatFloat(aPlayer->massproduced, fields);
      formatFloat(aPlayer->masslost, fields);
      formatFloat(aPlayer->massproduced - aPlayer->masslost, fields);
      formatReturn(fields);
    }
    formatPrint(fields);
  }
}


void
reportLastOrders(player *players, fielddef *fields)
{
  int             state;
  player         *aPlayer;

  fprintf(fields->destination, "\n\t\tLast Orders\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("Race Turn", "lc", fields);
    for (aPlayer = players; aPlayer; aPlayer = aPlayer->next) {
      formatString(aPlayer->name, fields);
      formatInteger(aPlayer->lastorders, fields);
      formatReturn(fields);
    }
    formatPrint(fields);
  }
}


/****f* GalaxyNG/reportGMBombings
 * NAME
 *   reportGMBombings --
 * SYNOPSIS
 *   void reportGMBombings(game *aGame, fielddef *fields)
 * FUNCTION
 *   Report all bombings that took place during the turn.
 *****
 */

void
reportGMBombings(game *aGame, fielddef *fields)
{
  int             state;

  fprintf(fields->destination, "\n\t\tBombings\n\n");
  formatReset(fields);
  for (state = 0; state < 2; state++) {
    bombing        *B;

    formatLabels("W O N P I P $ M C", "lcccccccc", fields);
    for (B = aGame->bombings; B; B = B->next) {
      formatString(B->who->name, fields);
      formatString(B->owner->name, fields);
      formatString(B->name, fields);
      formatFloat(B->pop, fields);
      formatFloat(B->ind, fields);
      if (B->producing eq PR_SHIP)
        formatString(B->producingshiptype->name, fields);
      else
        formatString(productname[B->producing], fields);
      formatFloat(B->cap, fields);
      formatFloat(B->mat, fields);
      formatFloat(B->col, fields);
      formatReturn(fields);
    }
    formatPrint(fields);
  }
}





double
percent(double tot, double f)
{
  return (f / tot) * 100.0;
}


void
scorePercent(game *g1, fielddef *fields)
{
  int             numberOfPlayers;
  int             number;
  int             state;
  player         *aPlayer;
  int             totNumberOfPlanets;
  double          totPop, totInd, totEInd;

  totNumberOfPlanets = numberOfElements(g1->planets);
  raceStatus(g1);
  rateRaces(g1->players);
  numberOfPlayers = numberOfElements(g1->players);
  fprintf(fields->destination, "\n\t\tRace Status with PercentsXXX\n\n");

  totPop = 0.0;
  totInd = 0.0;
  totEInd = 0.0;
  for (aPlayer = g1->players; aPlayer; aPlayer = aPlayer->next) {
    totPop += aPlayer->totPop;
    totInd += aPlayer->totInd;
    totEInd += effectiveIndustry(aPlayer->totPop, aPlayer->totInd);
  }

  formatReset(fields);
  for (state = 0; state < 2; state++) {
    formatLabels("Race Pop pcnt Ind pcnt EInd pcnt Planets pcnt",
                 "lcccccccc", fields);
    for (number = 1; number <= numberOfPlayers; number++) {
      for (aPlayer = g1->players; aPlayer; aPlayer = aPlayer->next) {
        if (aPlayer->rating eq number) {
          formatString(aPlayer->name, fields);
          formatFloat(aPlayer->totPop, fields);
          formatFloat(percent(totPop, aPlayer->totPop), fields);
          formatFloat(aPlayer->totInd, fields);
          formatFloat(percent(totInd, aPlayer->totInd), fields);
          formatFloat(effectiveIndustry(aPlayer->totPop, aPlayer->totInd),
                      fields);
          formatFloat(percent(totEInd, effectiveIndustry(aPlayer->totPop,
                                                         aPlayer->totInd)),
                      fields);
          formatInteger(aPlayer->numberOfPlanets, fields);
          formatFloat(percent((double) totNumberOfPlanets,
                              (double) aPlayer->numberOfPlanets), fields);
          formatReturn(fields);
        }
      }
    }
    formatString("Total", fields);
    formatFloat(totPop, fields);
    formatFloat(100.0, fields);
    formatFloat(totInd, fields);
    formatFloat(100.0, fields);
    formatFloat(totEInd, fields);
    formatFloat(100.0, fields);
    formatInteger(totNumberOfPlanets, fields);
    formatFloat(0.0, fields);
    formatReturn(fields);
    formatPrint(fields);
  }
}





