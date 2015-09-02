#include "galaxy.h"
#include "list.h"
#include "report.h"

/* Needed for machine report */
/* void reports_m (char *gameName); */
void            report_m(player *P, game *aGame, FILE *reportFile);

void            rHE_M(game *aGame, player *P);
void rMR_M(void);
void rGZ_M(game *aGame);
void            rPW_M(game *aGame, player *P);
void            rMM_M(game *aGame, player *P, strlist *messages, char *c);
void            rGO_M(game *aGame);
void            rOP_M(game *aGame, player *P);
void            rOD_M(game *aGame, player *P);
void            rMK_M(game *aGame, player *P);
void            rST_M(game *aGame, player *P);
void            rSH_M(game *aGame, player *P);
void            rBT_M(game *aGame, player *P);
void            rBB_M(game *aGame, player *P);
void            rGP_M(group *g, int n, int mode);
void            rMP_M(game *aGame, player *P);
void            rIC_M(game *aGame, player *P);
void            rAP_M(game *aGame, player *P);
void            rPL_M(planet *p);
void            rRT_M(game *aGame, player *P);
void            rPT_M(game *aGame, player *P);
void            rGG_M(game *aGame, player *P);
void            rFL_M(game *aGame, player *P);
void            rGS_M(game *aGame, player *P);
void            rPD_M(planet *p);
void            rEN_M(void);
