#include "galaxy.h"
#include "report.h"
#include "list.h"

/* Needed for xml report */
void            report_xml(game* aGame, player* P, FILE* report, int type);

void            rMR_XML(void);
void            rGZ_XML(void);
void            rPW_XML(game* aGame, player *P);
void            rMM_XML(game* aGame, player *P, strlist *messages, char *c);
void            rGO_XML(game* aGame);
void            rOP_XML(game* aGame, player *P);
void            rOD_XML(game* aGame, player *P);
void            rMK_XML(game* aGame, player *P);
void            rST_XML(game* aGame, player *P);
void            rSH_XML(game* aGame, player *P);
void            rBT_XML(game* aGame, player *P);
void            rBB_XML(game* aGame, player *P);
void            rGP_XML(group *g, int n, int mode);
void            rMP_XML(game* aGame, player *P);
void            rIC_XML(game* aGame, player *P);
void            rAP_XML(game* aGame, player *P);
void            rPL_XML(planet *p, char *owner);
void            rRT_XML(game* aGame, player *P);
void            rPT_XML(game* aGame, player *P);
void            rGG_XML(game* aGame, player *P);
void            rFL_XML(game* aGame, player *P);
void            rGS_XML(game* aGame, player *P);
void            rPD_XML(planet *p);
void            rEN_XML(void);

