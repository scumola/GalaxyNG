#ifndef GNG_LOADGAME_H
#define GNG_LOADGAME_H

#include "galaxy.h"
#include "list.h"
#include "util.h"

#define LG_CURRENT_TURN -1      /* Load current turn flag, if you supply * 
                                 *
                                 * * *  * *  * * this as turn number, *
                                 * loadgame * will * load  *  * * the *
                                 * current turn */

void            loadNGConfig(game *aGame);
void            readDefaults(game *aGame, FILE * f);
void            freeDefaults(game *aGame);

void            getReadVersion(int *version, int *revision);
game           *loadgame(char *gameName, int whichTurn);
double          readFloat(void);
char           *readString(void);
int             readInt(void);
int             readLong(void);
list           *readPointer(void *base);

double          getReadFloat(void);
char           *getReadString(void);
int             getReadInt(void);
int             getReadLong(void);
list           *getReadPointer(void *base);

#endif                          /* GNG_LOADGAME_H */
