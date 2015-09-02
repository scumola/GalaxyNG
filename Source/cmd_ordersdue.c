#include "galaxyng.h"

int CMD_ordersdue(int argc, char** argv) {
  FILE* gmnote;
  FILE* mof_fp;
  
  char* gmbody;
  
  game* aGame;
  player* aplayer;
  envelope* env;
  char* missing_orders_file = NULL;
  char* orders_dir;
  char* orders_file;
  int result;
  int msg_count = 0;
  
  result = EXIT_FAILURE;
  
  if (argc < 3) {
    usage();
  }
  else if ((aGame = loadgame(argv[2], LG_CURRENT_TURN)) != NULL) {
    loadNGConfig( aGame );
    gmbody = createString("%s/orders_due_%s", tempdir, aGame->name);
    gmnote = fopen(gmbody, "w");
    
    orders_dir = createString("%s/orders/%s/", galaxynghome, aGame->name);
    for (aplayer = aGame->players; aplayer; aplayer = aplayer->next) {
      if (aplayer->flags & F_DEAD)
	continue;
      
      orders_file = createString("%s/%s.%d", orders_dir,
				 aplayer->name, aGame->turn+1);
      if (access(orders_file, R_OK) == -1) {
	free(orders_file);
	orders_file = createString("%s/%s_final.%d", orders_dir,
				   aplayer->name, aGame->turn+1);
	if (access(orders_file, R_OK) == -1) {
	  env = createEnvelope();
	  env->to = strdup(aplayer->addr);
	  env->from = strdup(aGame->serverOptions.SERVERemail);
	  env->subject = createString("Turn %d of %s is about to run",
				      aGame->turn+1, argv[2]);
	  setHeader(env, MAILHEADER_REPLYTO,
		    aGame->serverOptions.ReplyTo);
	  env->from_name = strdup(aGame->serverOptions.SERVERname);
	  env->from_address = strdup(aGame->serverOptions.SERVERemail);

	  if (msg_count == 0) {
	    fprintf(gmnote, "The following players have not yet "
		    "submitted orders for turn %d of %s\n",
		    aGame->turn+1, aGame->name);
	    
	    missing_orders_file =
	      createString("%s/data/%s/missing_orders.%d",
			   galaxynghome, aGame->name,
			   aGame->turn+1);
	    mof_fp = fopen(missing_orders_file, "w");
	    fprintf(mof_fp,
		    "Your orders for turn %d for %s have not been "
		    "received.\nOrders are due in %s hours. "
		    "Please send them now.\n"
		    "If you don't plan on sending in orders, please submit a\n"
		    "Quit order (\"Q <racename>\") as it will stop these\n"
		    "annoying emails to you and could help speed up the game\n"
		    "for the other players. Thank you.\n",
		    aGame->turn+1, aGame->name,
		    aGame->serverOptions.due);
	    fclose(mof_fp);
	  }
	  fprintf(gmnote, "%s has not turned in orders.\n",
		  aplayer->name);
	  result |= eMail(aGame, env, missing_orders_file);
	  destroyEnvelope(env);
	  msg_count++;
	}
	free(orders_file);
      }
    }
    free(orders_dir);
  }
  else {
    fprintf(stderr, "Cannot open game %s\n", argv[2]);
  }
  
  if (missing_orders_file) {
    unlink(missing_orders_file);
    free(missing_orders_file);
  }
  
  if (msg_count) {
    fclose(gmnote);
    env = createEnvelope();
    env->to = strdup(aGame->serverOptions.GMemail);
    env->from = strdup(aGame->serverOptions.SERVERemail);
    env->subject = createString("Turn %d of %s is about to run",
				aGame->turn+1, aGame->name);
    setHeader(env, MAILHEADER_REPLYTO, aGame->serverOptions.ReplyTo);
    env->from_name = strdup(aGame->serverOptions.SERVERname);
    env->from_address = strdup(aGame->serverOptions.SERVERemail);

    result |= eMail(aGame, env, gmbody);
    
    destroyEnvelope(env);
  }
  
  return result;
}
