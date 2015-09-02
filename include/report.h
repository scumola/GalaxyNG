#ifndef GNG_REPORT_H
#define GNG_REPORT_H

/* $Id: report.h,v 1.7 2004/09/06 12:03:39 madcoder42 Exp $ */

#include "galaxy.h"
#include "loadgame.h"
#include "list.h"
#include "util.h"
#include "mail.h"
#include "report_m.h"
#include "report_xml.h"

#ifdef WIN32
/* #include <varargs.h> */
#include <stdarg.h>
#include <stdio.h>
#endif

#define FORMATBUFFERSIZE 512

#define MAXNOFIELDS     100
#define M_ALIGN_LEFT    0
#define M_ALIGN_RIGHT   1
#define M_ALIGN_CENTER  2

/****s* GalaxyNG/fielddef
 * NAME
 *   fielddef -- Printing field definitions.
 * PURPOSE
 *   Stores information about the fields in a row of a table.
 *   The turn report consists of a number of tables.  Each table
 *   consists of a number of rows of fields.  The fields in  
 *   a table are aligned. To get them aligned the maximum fields
 *   length needs to be known. This length is stored in this 
 *   structure.  Tables are printed in a two step process.
 *   In the first step nothing is printed and the maximum field
 *   length is computed. In the second step the maximum field length
 *   is used to align and print the fields.
 * SOURCE
 */

typedef struct fielddef {
  int             fieldSizes[MAXNOFIELDS];
  int             fieldNumber;
  int             format;
  FILE           *destination;
} fielddef;

/********/


#define G_MODE_OWN     0
#define G_MODE_BATTLE  1
#define G_MODE_ALIEN   2

game           *createDummyGame(void);

void            highScoreList(game *aGame);

void            mailGMReport(game *aGame, char *gameName);

void            createGMReport(game *aGame, char *gameName,
                               FILE * gmreport);

int             mailTurnReport(game *aGame, player *aPlayer, long kind);

void            saveTurnReport(game *aGame, player *aPlayer, long kind);

void            createTurnReport(game *aGame, player *aPlayer,
                                 FILE * report, long kind);

void            createHeader(game *aGame, player *aPlayer,
                             FILE * reportfile);

void            appendToFile(char *fileName, FILE * report);

void            report(game *aGame, player *P, FILE * report);

void            score(game *g1, game *g2, int html, FILE * dest);

void            rateRaces(player *playerList);

void            storeLength(fielddef *fields, int length);

void            formatReset(fielddef *fields);

void            formatString(char *s, fielddef *fields);

void            formatStringCenter(char *s, fielddef *fields);

void            formatStringMode(char *s, fielddef *fields, int mode);

void            formatFloat(double g, fielddef *fields);

void            formatInteger(int i, fielddef *fields);

void            GformatInteger(int i, fielddef *fields);

void            BformatInteger(int i, fielddef *fields, char *form);

void            formatReturn(fielddef *fields);

void            formatPrint(fielddef *fields);

void            formatLabels(char *labels, char *align, fielddef *fields);

void            dumpItem(fielddef *fields, int mode);

int             visibleShipTypes(game *aGame, player *P2, player *P);

int             canseeBombing(game *aGame, player *P);

void            formatChar(char c, fielddef *fields, int mode);

void            reportPlanet(planet *p, fielddef *fields);

void            GreportPlanet(planet *planets, planet *p,
                              fielddef *fields);

void            reportProduction(planet *p, fielddef *fields);

void            GreportProduction(planet *planets, planet *p,
                                  fielddef *fields);

void            reportGroup(group *g, int n, fielddef *fields, int mode);

void            GreportGroup(planet *planets, player *P, group *g, int n,
                             fielddef *fields, int mode, int phase);

void            reportMap(game *aGame, player *P, fielddef *fields);

void            reportMap_gnuplot(game *aGame, player *P,
                                  fielddef *fields);

void            reportGlobalMessages(strlist *messages, fielddef *fields);

void            reportMessages(player *P, fielddef *fields);

void            reportOrders(player *P, fielddef *fields);

void            reportMistakes(player *P, fielddef *fields);

void            reportLastOrders(player *players, fielddef *fields);

void            reportStatus(player *players, player *P, fielddef *fields);

void            reportYourShipTypes(player *P, fielddef *fields);

void            reportShipTypes(game *aGame, player *P, fielddef *fields);

void            reportShipType(shiptype *t, fielddef *fields);

void            GreportShipType(shiptype *t, fielddef *fields);

void            reportBattles(game *aGame, player *P, fielddef *fields);

void            reportBombings(game *aGame, player *P, fielddef *fields);

void            reportIncoming(game *aGame, player *P, fielddef *fields);

void            reportYourPlanets(planet *planets, player *P,
                                  fielddef *fields);

void            reportProdTable(planet *planets, player *P,
                                fielddef *fields);

void            reportRoutes(planet *planets, player *P, fielddef *fields);

void            reportPlanetsSeen(game *aGame, player *P,
                                  fielddef *fields);

void            reportUnidentifiedPlanets(planet *planets, player *P,
                                          fielddef *fields);

void            reportUninhabitedPlanets(planet *planets, player *P,
                                         fielddef *fields);

void            reportYourGroups(planet *planets, player *P,
                                 fielddef *fields);

void            reportFleets(player *P, fielddef *fields);

void            GreportFleets(planet *planets, player *P,
                              fielddef *fields);

void            reportGroupsSeen(game *aGame, player *P, fielddef *fields);

void            reportGameOptions(game *aGame, fielddef *fields);

void            reportOptions(game *aGame, player *P, fielddef *fields);

void            tagVisiblePlanets(planet *planets, player *P);

void            yourPlanetsForecast(planet *planets, player *P,
                                    fielddef *fields);

void            yourStatusForecast(planet *planets, player *P,
                                   fielddef *fields);

void            reportHall(game *aGame, fielddef *fields);

void            reportTeam(game *aGame, fielddef *fields, int team);

void            reportPlayers(player *players, fielddef *fields);

void            reportRaceProduction(player *players, fielddef *fields);

void            reportGMBombings(game *aGame, fielddef *fields);

void            scorePercent(game *g1, fielddef *fields);


#if FS_NEW_FORECAST
void reportForecast( game *aGame, char *raceName, FILE *forecast );
#endif

#endif                          /* GNG_REPORT_H */
