#include "galaxy.h"
#include "influence.h"
#include "loadgame.h"

int  draw_maps(game* aGame, enum map_type type);

int
CMD_influence(int argc, char* argv[])
{
	game* aGame;
	char* type;
	int   result;
	int   turn_nbr;
	enum map_type map_type;
	
	result = EXIT_FAILURE;

	switch (argc) {
		case 1:
		case 2:
			usage();
			return result;

		case 3:
			type = "all";
			turn_nbr = -1;		/* load current turn */
			break;

		case 4:
			type = argv[3];
			turn_nbr = -1;		/* load current turn */
			break;

		case 5:
			type = argv[3];
			turn_nbr = atoi(argv[4]);
			break;

		default:
			usage();
			return result;
	}

	if (strcasecmp(type, "all") == 0) {
		map_type = AllMaps;
	}
	else if (strcasecmp(type, "industry") == 0) {
		map_type = EffIndMap;
	}
	else if (strcasecmp(type, "attack") == 0) {
		map_type = ShipAttMap;
	}
	else if (strcasecmp(type, "defend") == 0) {
		map_type = ShipDefMap;
	}
	else if (strcasecmp(type, "location") == 0) {
		map_type = LocationMap;
	}
	else if (strcasecmp(type, "public") == 0) {
		map_type = PublicMap;
	}
	else {
		usage();
	}

	if ((aGame = loadgame(argv[2], turn_nbr))) {
		loadNGConfig(aGame);
		if (map_type == AllMaps) {
			for (map_type = FirstMap; map_type < NbrMaps; map_type++)
				result += draw_maps(aGame, map_type);
		}
		else {
			result = draw_maps(aGame, map_type);
		}
	}
	else {
		fprintf(stderr, "Could not load game \"%s\".\n", argv[2]);
	}
	
	return result;
}

struct _map {
	float* influence;
	float* factor;
} pi_map[901][901];

int
draw_maps(game* aGame, enum map_type map_type)
{
	player*     P;
	planet*     p;
	group*      gp;
	
	FILE*      mapfile;      /* output for map */
	char*      err;
	char       map_title[64];
	
	gdImagePtr map_png;	/* image */
	int        white;        /* colour indices */
	int        ship;
	
	float     scale;
	
	int        brect[8];
	int        i, j, k;
	int        iscale;
	
	char       font[1024];
	
	int        idx, x, y, player_nbr;
	int        x0, y0, df, d_e, d_se;
	int        r, g, b;
int map_height;
int name_rows;
	float     weight;
	float     influence;
	float     factor, limit;
	
	int        map_colors[][4] = {
		{0, 0, 0, 0},               /* black */
		{255, 255, 255, 0},         /* white */
		{255,   0,   0, 0},		/* ship color */
		{0, 255, 127, 0},           /* spring green */
		{205, 92, 92, 0},           /* indian red */
		{132, 112, 255, 0},         /* light slate blue */
		{193, 255, 193, 0},         /* DarkSeaGreen1 */
		{238, 154, 73, 0},          /* tan2 */
		{178, 34, 34, 0},           /* firebrick */
		{85, 107, 47, 0},           /* dark olive green */
		{255, 250, 205, 0},         /* lemon chiffon */
		{240, 255, 255, 0},         /* azure */
		{230, 230, 250, 0},         /* lavender */
		{255, 228, 225, 0},         /* misty rose */
		{235, 245, 230, 0},         /* mint cream */
		{135, 206, 250, 0},         /* light sky blue */
		/**/ {176, 224, 230, 0},   /* powder blue */
		{0, 206, 209, 0},           /* dark turquoise */
		{0, 255, 255, 0},           /* cyan */
		{102, 205, 170, 0},         /* medium aquamarine */
		{46, 139, 87, 0},           /* sea green */
		{152, 251, 152, 0},         /* pale green */
		{173, 255, 47, 0},          /* green yellow */
		{107, 142, 35, 0},          /* olive drab */
		/**/ {238, 232, 170, 0},   /* pale goldenrod */
		{255, 215, 0, 0},           /* gold */
		{222, 184, 135, 0},         /* burlywood */
		{255, 160, 122, 0},         /* light salmon */
		{255, 140, 0, 0},           /* dark orange */
		{240, 128, 128, 0},         /* light coral */
		{100, 149, 237, 0},         /* cornflower blue */
		{255, 99, 71, 0},           /* tomato */
		{255, 105, 180, 0},         /* hot pink */
		/**/ {219, 112, 147, 0},   /* pale violet red */
		{176, 48, 96, 0},           /* maroon */
		{186, 85, 211, 0},          /* medium orchid */
		{148, 0, 211, 0},           /* dark violet */
		{255, 250, 250, 0},         /* snow1 */
		{255, 246, 143, 0},         /* khaki1 */
		{238, 220, 130, 0},         /* LightGoldenrod2 */
		{205, 155, 155, 0},         /* RosyBrown3 */
		{139, 95, 101, 0},          /* LightPink4 */
		{205, 150, 205, 0},         /* plum3 */
	};
	
	int  nbr_of_players, nbr_of_colors;
	int  x1, y1;  /* temporary coordinates */
	
	/*long total_mem = 0;*/
	nbr_of_colors = sizeof(map_colors) / (sizeof(int)*4);
	nbr_of_players = numberOfElements(aGame->players);
	scale = 900.0 / aGame->galaxysize;
	iscale = (int)scale;

	if (aGame->serverOptions.fontpath)
		sprintf(font, "%s/influence.ttf", aGame->serverOptions.fontpath);
	else
		strcpy(font, "./influence.ttf");

	name_rows = nbr_of_players / 4 + ((nbr_of_players % 4) != 0);
	switch(map_type) {
		case EffIndMap:
			sprintf(buf, "%s/data/%s/effind_%d.png",
					galaxynghome, aGame->name, aGame->turn);
			strcpy(map_title, "Effective Industry");
			break;
			
#if 0
		case ShipAttMap:
			sprintf(buf, "%s/data/%s/shpatt_%d.png",
					galaxynghome, aGame->name, aGame->turn);
			strcpy(map_title, "Ship Attack Potential");
			break;
			
		case ShipDefMap:
			sprintf(buf, "%s/data/%s/shpdef_%d.png",
					galaxynghome, aGame->name, aGame->turn);
			strcpy(map_title, "Ship Defense Potential");
			break;
#endif
		case LocationMap:
			sprintf(buf, "%s/data/%s/location_%d.png",
					galaxynghome, aGame->name, aGame->turn);
			strcpy(map_title, "Locations Only");
			break;

		case PublicMap:
			sprintf(buf, "%s/data/%s/public_%d.png",
					galaxynghome, aGame->name, aGame->turn);
			strcpy(map_title, "Overview");
			name_rows = 0;
			break;

		default:
			break;
	}
	
	mapfile = GOS_fopen(buf, "wb");
	
	map_height = 902 + (name_rows+1)*17+2;
	map_png = gdImageCreateTrueColor(902, map_height);
	
	for (i = 0; i < nbr_of_colors; i++) {
		map_colors[i][3] = gdImageColorAllocate(map_png, map_colors[i][0],
												map_colors[i][1],
												map_colors[i][2]);
	}
	
	white = map_colors[1][3];
	ship = map_colors[2][3];
	
	gdImageRectangle(map_png, 0, 0, 901, map_height-1, white);
	gdImageRectangle(map_png, 0, 901, 902, 902, white);
	gdImageLine(map_png, 0, map_height-1, map_height-1, map_height-1, white);
	gdImageLine(map_png, 0, 919, map_height-1, 919, white);

	sprintf(buf, "Map Size: %d", (int)aGame->galaxysize);
	err = gdImageStringFT((gdImagePtr) NULL, &brect[0], white, font,
						  12., 0., 0, 0, buf);
	if (err)
		fprintf(stderr, "%s\n", err);
	gdImageStringFT(map_png, NULL, white, font, 12., 0., 3, 916, buf);


	gdImageLine(map_png, 5 + brect[4], 914, 5+(10*scale) + brect[4], 914, white);

	for (i = 0; i <= 1; i++) {
		gdImageLine(map_png, 5+(10*i*scale) + brect[4],
					914, 5+(10*i*scale) + brect[4], 908, white);
	}

	strcpy(buf, "(10 LY)");
	gdImageStringFT(map_png, NULL, white, font, 12.0, 0.,
					7+brect[4]+10*scale, 916, buf);
	{int dist_used = 9 + brect[4] + 10*scale;
	err = gdImageStringFT((gdImagePtr) NULL, &brect[0], white, font,
						  12., 0., 0, 0, buf);
	if (err)
		fprintf(stderr, "%s\n", err);

	gdImageLine(map_png, dist_used + brect[4], 919, dist_used+brect[4], 902, white);
	gdImageLine(map_png, 902 - (dist_used + brect[4]), 919, 902 - (dist_used+brect[4]), 902, white);
	sprintf(buf, "Turn: %d", aGame->turn);
	gdImageStringFT(map_png, NULL, white, font, 12.0, 0., 904 - (dist_used+brect[4]), 916, buf); 
	}
	
	sprintf(buf, "%s: %s", aGame->name, map_title);
	err = gdImageStringFT((gdImagePtr) NULL, &brect[0], white, font,
						  12., 0., 0, 0, buf);
	if (err)
		fprintf(stderr, "%s\n", err);
	
	gdImageStringFT(map_png, NULL, white, font, 12., 0.,
					451 - (brect[4] / 2), 916, buf);

	/* set  up variables to hold the influence, one float for each
	 * player. We'll combine at the end
	 */
	for (j = 0; j < 900; j++) {
		for (k = 0; k < 900; k++) {
			if (pi_map[j][k].influence != NULL) {
				memset((void*)pi_map[j][k].influence, 0,
					   sizeof(float)*nbr_of_players);
				memset((void*)pi_map[j][k].factor, 0,
					   sizeof(float)*nbr_of_players);
			}
		}
	}

	if (name_rows) {
		int row;
int name_len;
		/* draw the player names in the key */
		i = 3;                        /* nations off by 3 due to
									   * black/white/ship color */
		j = 0;
		row = 0;
		for (P = aGame->players; P; P = P->next) {
			if (i == nbr_of_colors) {
				i = 3;
			}

strcpy(buf, P->name);
name_len = strlen(buf);
while (1) {
err = gdImageStringFT((gdImagePtr) NULL, &brect[0], white, font,
						  12., 0., 0, 0, buf);
if (brect[4] < 220)
break;
buf[--name_len] = '\0';
}

			err = gdImageStringFT(map_png, NULL, map_colors[i][3], font, 12.,
								  0., 3 + (j*225), 933 + (row * 16), buf);
			if (err)
				fprintf(stderr, "%s\n", err);
			j++;
			if (j == 4) {
				j = 0;
				row++;
			}
			i++;
		}      
	}

	switch(map_type) {
		case EffIndMap:
		case ShipAttMap:
		case ShipDefMap:
			/* now add up the influence for each player */
			for (p = aGame->planets; p; p = p->next) {
				player* owner;
				
				owner = p->owner;
				
				player_nbr = ptonum(aGame->players, owner) - 1;
				
				if (player_nbr < 0 && map_type == EffIndMap)
					continue;
				
				switch(map_type) {
					case EffIndMap:
						weight = effectiveIndustry(p->pop, p->ind);
						influence = factor = 0.9;
						limit = 0.5;
						break;
#if 0
					case ShipAttMap:
						weight=addShipAtt(aGame, p);
						influence = factor = 0.925;
						limit = 0.1;
						break;
						
					case ShipDefMap:
						weight=addShipDef(aGame, P);
						influence = factor = 0.925;
						limit = 0.1;
						break;
#endif
					default:
						break;
				}	
				
				x = (int) ((scale * p->x) + 1);
				y = (int) ((scale * p->y) + 1);
				
				idx = 1;
				while (weight > limit) {
					weight *= factor;
					influence *= factor;
					
					x0 = 0;
					y0 = idx;
					
					df = 1 - idx;
					d_e = 3;
					d_se = -2 * idx + 5;
					
					do {
						if (x+x0 < 900 && y + y0 < 900) {
							x1 = x+x0;
							y1 = y+y0;
							if (pi_map[x1][y1].influence == NULL) {
								/*total_mem += sizeof(float)*nbr_of_players*2;*/
								pi_map[x1][y1].influence =
									(float*)calloc(nbr_of_players, sizeof(float));
								pi_map[x1][y1].factor =
									(float*)calloc(nbr_of_players, sizeof(float));
							}
							pi_map[x1][y1].influence[player_nbr] += weight;
							pi_map[x1][y1].factor[player_nbr] += influence;
						}
						
						if (x0 && (x-x0 > 1 && y+y0<900)) {
							x1 = x - x0;
							y1 = y + y0;
							if (pi_map[x1][y1].influence == NULL) {
								/*total_mem += sizeof(float)*nbr_of_players*2;*/
								pi_map[x1][y1].influence =
									(float*)calloc(nbr_of_players, sizeof(float));
								pi_map[x1][y1].factor =
									(float*)calloc(nbr_of_players, sizeof(float));
							}
							pi_map[x1][y1].influence[player_nbr] += weight;
							pi_map[x1][y1].factor[player_nbr] += influence;
						}
						
						if (y0 && (x+x0 < 900 && y-y0 > 1)) {
							x1 = x+x0;
							y1 = y-y0;
							if (pi_map[x1][y1].influence == NULL) {
								/*total_mem += sizeof(float)*nbr_of_players*2;*/
								pi_map[x1][y1].influence =
									(float*)calloc(nbr_of_players, sizeof(float));
								pi_map[x1][y1].factor =
									(float*)calloc(nbr_of_players, sizeof(float));
							}
							
							pi_map[x1][y1].influence[player_nbr] += weight;
							pi_map[x1][y1].factor[player_nbr] += influence;
						}
						
						if (x0 && y0 && (x-x0>1 && y-y0>1)) {
							x1 = x - x0;
							y1 = y - y0;
							if (pi_map[x1][y1].influence == NULL) {
								/*total_mem += sizeof(float)*nbr_of_players*2;*/
								pi_map[x1][y1].influence =
									(float*)calloc(nbr_of_players, sizeof(float));
								pi_map[x1][y1].factor =
									(float*)calloc(nbr_of_players, sizeof(float));
							}
							pi_map[x1][y1].influence[player_nbr] += weight;
							pi_map[x1][y1].factor[player_nbr] += influence;
						}
						
						if (x0 != y0) {
							if (x+y0<900 && y+x0<900) {
								x1 = x+y0;
								y1 = y+x0;
								if (pi_map[x1][y1].influence == NULL) {
									/*total_mem += sizeof(float)*nbr_of_players*2;*/
									pi_map[x1][y1].influence =
										(float*)calloc(nbr_of_players, sizeof(float));
									pi_map[x1][y1].factor =
										(float*)calloc(nbr_of_players, sizeof(float));
								}
								
								pi_map[x1][y1].influence[player_nbr] += weight;
								pi_map[x1][y1].factor[player_nbr] += influence;
							}
							
							if (x0 && (x+y0<900 && y-x0>1)) {
								x1 = x+y0;
								y1 = y-x0;
								if (pi_map[x1][y1].influence == NULL) {
									/*total_mem += sizeof(float)*nbr_of_players*2;*/
									pi_map[x1][y1].influence =
										(float*)calloc(nbr_of_players, sizeof(float));
									pi_map[x1][y1].factor =
										(float*)calloc(nbr_of_players, sizeof(float));
								}
								
								pi_map[x1][y1].influence[player_nbr] += weight;
								pi_map[x1][y1].factor[player_nbr] += influence;
							}
							
							if (y0 && (x-y0>1 && y+x0<900)) {
								x1 = x-y0;
								y1 = y+x0;
								if (pi_map[x1][y1].influence == NULL) {
									/*total_mem += sizeof(float)*nbr_of_players*2;*/
									pi_map[x1][y1].influence =
										(float*)calloc(nbr_of_players, sizeof(float));
									pi_map[x1][y1].factor =
										(float*)calloc(nbr_of_players, sizeof(float));
								}
								pi_map[x1][y1].influence[player_nbr] += weight;
								pi_map[x1][y1].factor[player_nbr] += influence;
							}
							
							if (x0 && y0 && (x-y0>1 && y-x0>1)) {
								x1 = x-y0;
								y1 = y-x0;
								if (pi_map[x1][y1].influence == NULL) {
									/*total_mem += sizeof(float)*nbr_of_players*2;*/
									pi_map[x1][y1].influence =
										(float*)calloc(nbr_of_players, sizeof(float));
									pi_map[x1][y1].factor =
										(float*)calloc(nbr_of_players, sizeof(float));
								}
								pi_map[x1][y1].influence[player_nbr] += weight;
								pi_map[x1][y1].factor[player_nbr] += influence;
							}
						}
						
						if (df < 0) {
							df += d_e;
							d_e += 2;
							d_se += 2;
						}
						else {
							df += d_se;
							d_e += 2;
							d_se += 4;
							y0--;
						}
						
						x0++;
					} while (x0 < y0);
					idx++;
				}
			}
			
			/*fprintf(stderr, "total mem used: %ld\n", total_mem);*/
			
			for (i = 1; i < 900; i++) {
				for (j = 1; j < 900; j++) {
					int winning_player;
					float max_weight;
					
					max_weight = 0.0;
					winning_player = -1;
					for (k = 0; k < nbr_of_players; k++) {
						if (pi_map[i][j].influence == NULL)
							continue;
						
						if (max_weight < pi_map[i][j].influence[k]) {
							max_weight = pi_map[i][j].influence[k];
							winning_player = k;
							
							if (pi_map[i][j].factor[k] > 1.0)
								pi_map[i][j].factor[k] = 1.0;
						}
					}
					
					if (winning_player == -1)
						continue;
					
					r = map_colors[(winning_player%40)+3][0] * pi_map[i][j].factor[winning_player];
					g = map_colors[(winning_player%40)+3][1] * pi_map[i][j].factor[winning_player];
					b = map_colors[(winning_player%40)+3][2] * pi_map[i][j].factor[winning_player];
					
					gdImageSetPixel(map_png, i, j,
									gdImageColorResolve(map_png, r, g, b));
				}
			}
			break;

		case LocationMap:
			for (p = aGame->planets; p; p = p->next) {
				player* owner;
				
				owner = p->owner;
				
				player_nbr = ptonum(aGame->players, owner) - 1;
				
				if (player_nbr < 0)
					continue;
				
				x = (int) ((scale * p->x) + 1);
				y = (int) ((scale * p->y) + 1);
				
				r = map_colors[(player_nbr%40)+3][0];
				g = map_colors[(player_nbr%40)+3][1];
				b = map_colors[(player_nbr%40)+3][2];
				
				
				gdImageArc(map_png, x, y, 8, 8, 0, 360,
						   gdImageColorResolve(map_png, r, g, b));
			}
			break;

		case PublicMap:
			for (p = aGame->planets; p; p = p->next) {
				player* owner;
				
				owner = p->owner;
				
				player_nbr = ptonum(aGame->players, owner) - 1;
				
				if (player_nbr < 0)
					continue;
				
				x = (int) ((scale * p->x) + 1);
				y = (int) ((scale * p->y) + 1);
				
				gdImageArc(map_png, x, y, 9, 9, 0, 360,
						   gdImageColorResolve(map_png, 0, 255, 0));
			}
			break;

		case AllMaps:
		case NbrMaps:
			/* Trouble in River City - these should never happen */
			break;
	}

	/* put in markers for all planets */
	for (p = aGame->planets; p; p = p->next) {
		x = (int) ((scale * p->x) + 1);
		y = (int) ((scale * p->y) + 1);
		gdImageArc(map_png, x, y, 3, 3, 0, 360, white);
	}

	/* put in markers for ships in transit */
	for (P = aGame->players; P; P = P->next) {
		for (gp = P->groups; gp; gp = gp->next) {
			if (groupLocation(aGame, gp) == NULL) {
				x = (int) ((scale * groupx(aGame, gp)) + 1);
				y = (int) ((scale * groupy(aGame, gp)) + 1);
				gdImageArc(map_png, x, y, 2, 2, 0, 360, ship);
			}
		}
	}
	
	gdImagePng(map_png, mapfile);
	fclose(mapfile);
	gdImageDestroy(map_png);

	return 1;
}

