#include "galaxy.h"

/****f* CLI/usage
 * NAME
 *   usage -- print usage info.
 *******
 */

void
usage()
{
  printf("%s\n", vcid);
  printf("Usage: galaxyng <command> <parameters> <options>\n"
	 "\nCommands Available:\n"
	 " General Commands\n"
	 "   -template   <game name> <number of players>\n"
	 "   -create     <specifcation file>\n"
	 "   -mail0      <game name>\n"
	 "   -check\n"
	 "   -run        <game name> <file with all orders> [turn]\n"
	 "   -score      <game name> [turn]\n"
	 "   -toall      <game name>\n"
	 "   -report\n"
	 "   -relay\n"
	 "   -hall       <game name>\n");
  printf("   -lastorders <game name> [turn]\n"
	 "   -players    <game name> [turn]\n"
	 "   -due        <game name>\n"
	 "   -immediate  <game name>\n"
	 "   -teaminfo   <game name> <turn> <team number>\n");
  
  printf(" Debug commands\n"
	 "   -dummyrun   <game name> <file with all orders> [turn]\n"
	 "   -dummymail0 <game name>\n"
	 "   -dummycheck <game name> <file with all orders> [turn]\n"
	 "   -test       <game name>\n"
	 " Experimental Commands\n"
	 "   -teaminfo   <game name> <turn> <team number>\n"
	 "   -graph      <game name> [turn]\n"
	 "   -map        <game name> [turn]\n"
#if defined(DRAW_INFLUENCE_MAP)
	 "   -influence  <game name> <type> [turn]\n"
	 "               (<type> is all, industry, attack, defend, location, public)\n"
#endif		   
	 "   -gnuplot    <game name> [turn]\n");
  printf("\nExplanation:\n"
	 " -template   - create a template .glx file for use with -create.\n"
	 " -create     - create a new game.\n"
	 " -mail0      - mail the turn 0 reports.\n"
	 " -check      - check incoming orders, orders are read from stdin.\n");
  printf(" -run        - run a game and mail the turn reports.\n"
	 " -score      - creates highscore list in HTML format.\n"
	 " -report     - creates and mails a report, turn and race are"
	 " read from stdin.\n"
	 " -relay      - relay email between players,"
	 " orders are read from stdin.\n"
	 " -hall       - create information for the Hall of Fame.\n"
	 " -dummyrun   - run a game but do not mail the turn reports.\n");
  printf(" -dummycheck - check orders, but do not mail the forecast.\n"
	 "               orders are read from stdin.\n"
	 " -dummymail0 - create the turn 0 reports, but do not email them.\n"
	 " -test       - test the integrity of a game file.\n"
	 " -teaminfo   - create an info report for a team captain\n"
	 " -graph      - dump game data for graph creation.\n");
  
  printf(" -map        - dump ASCII game map.\n"
#if defined(DRAW_INFLUENCE_MAP)
	 " -influence  - create influence map(s). The available maps are\n"
	 "               Effective Industry, Attack Strength, Defense "
	 "Strength, Planet Locater, Public\n"
#endif		   
	 " -lastorders - list the turn when players last sent in orders.\n"
	 " -due        - send email to players that haven't submitted orders.\n");
  printf(" -immediate  - run the turn right now if either condition is met:\n"
	 "               a) all players have submitted final orders\n"
	 "               b) the GM specified amount of time has expired\n"
	 " -players    - list address and password of all players.\n");
}
