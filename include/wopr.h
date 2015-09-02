#ifndef GNG_WOPR_H
#define GNG_WOPR_H


#define READING_NONE   0
#define READING_TYPES  1
#define READING_GROUPS 2

game *WOPR_createGame(  );

shiptype *WOPR_createShiptype( char *name, double drive,
                               int attacks, double weapons, double shields,
                               double cargo );

void WOPR_allies( game *aGame, char *playerName1, char *playerName2 );

int WOPR_battle( int argc, char **argv );

player *WOPR_addPlayer( game *aGame, char *name );

int parse_group( game *aGame, player *aPlayer );

int WOPR_parse_type( game *aGame, player *aPlayer );

int WOPR_parse_scenario( FILE *scenario, game *aGame );

void WOPR_usage( void );

#endif /* GNG_WOPR_H */
