#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "includes/types.h"

#include "includes/map.h"
#include "includes/player.h"
#include "includes/unit.h"
#include "includes/util.h"
#include "includes/cursor.h"
#include "includes/info_screen.h"
#include "includes/menu.h"
#include "includes/game.h"
#include "includes/terminal.h"
#include "includes/keys.h"
#include "includes/ai.h"
#include "includes/weapon.h"
#include "includes/vector.h"

#define UNIT_DELIMITER ':'
#define TEAM_DELIMITER ';'
#define TEAM_END '#'

void game_play_game(char *teams) {

	exit_on_null(teams,"Error in game_play_game, team is null");

	Map main_map = DEFAULT_MAP;
	Map copy_map = DEFAULT_MAP;

	Terminal term;
	terminal_init(&term);

	Cursor main_cursor = DEFAULT_CURSOR;
	Missile missile = DEFAULT_MISSILE;

	Menu_manager mmanager;
	menu_manager_init(&mmanager);

	Info_screen iscreen = DEFAULT_INFO_SCREEN;

	Game game;
	game_init_game(&game, COLOR_YELLOW, COLOR_BLUE);
	
	exit_on_error(map_load("data/map.data", &main_map), "Error in main_test during load map");
	exit_on_error(map_init(&copy_map,main_map.sizex, main_map.sizey), "Error in copy map initialization");

	info_screen_init(&iscreen);

	game_create_players(&game,teams,&main_map);
	
	Game_media media;
	game_media_set_media(&media, &term, &copy_map, &main_cursor, &iscreen, &mmanager, &game, &missile);

	terminal_welcome_message(&term);

	while(game_is_running(&game)) {
	
		exit_on_error(map_copy(&main_map, &copy_map), "Error in main_test, during map copying");
		
		game_remove_destroyed_units(&media,&main_map);
		if(game_check_end_conditions(&media)) {
			printf("\n");
			terminal_print(&term);		
			break;
		}

		game_post_units(&media,&copy_map);
		weapon_post_missile(&missile,&copy_map);
		//cursor_insert_to_map(&copy_map, &main_cursor);

		pre_print();
		game_print_header(&game);

		Cursor *c = NULL;
		if(!ai_is_ai_turn(&game.ai)) {
			c= &main_cursor;
		}

		map_print(&media, &copy_map, c, mmanager.actual_unit, &iscreen);
		printf("\n");
		util_separation_line();
		menu_print(mmanager.actual_menu);
		util_separation_line();

		printf("\n");
		terminal_print(&term);		
		system("setterm -cursor off");
		
		usleep(100000);

		if(ai_is_ai_turn(&game.ai)) {
			if(!ai_do_move(&game, &media)) {
				game_ai_finished_turn(&game,&term);
			}
			continue;				
		}else {
			
			if(mmanager.actual_unit!=NULL && unit_is_firing(mmanager.actual_unit)) {

				weapon_missile_count_fire(mmanager.actual_unit, media.mftunit, &media, &missile);
				continue;	

			}

			char c=getchar();
		
			Menu_option* mo;
			if((mo=menu_find_option_by_key(mmanager.actual_menu,c,BOOL_TRUE))!=NULL) {
				if(mo->next_menu != MENU_NO_TYPE) {
					mo->action(&mmanager, mo->next_menu, &media);
					continue;
				}
				if(menu_is_type(mmanager.actual_menu,MENU_UNIT)) {
					mo->action(mmanager.actual_unit, &media, c, &mmanager);
					continue;
				}
				if(menu_is_type(mmanager.actual_menu,MENU_GAME)) {
					mo->action(&game, &term);
					continue;
				}
				if(menu_is_type(mmanager.actual_menu,MENU_MAP)) {
					mo->action(&media, &mmanager,c);
					continue;
				}	
				if(menu_is_type(mmanager.actual_menu,MENU_FIRING)) {
					mo->action(mmanager.actual_unit, &media,c, &mmanager);
					continue;
				}
			}
		}

	}

	system("setterm -cursor on");	

	info_screen_free(&iscreen);
	game_free_game(&game);
	map_free(&main_map);
	map_free(&copy_map);

}

void game_init_game(Game *g, Color_type p, Color_type f) {

	exit_on_null(g,"Error in game_init_game, game is null");

	if(p==f) {
		exit_on_error(GAMEERR_SAME_COLORS,"Error in game_init_game, player and opponent colors are the same");
	}		

	g->player.units.data=NULL;
	g->opponent.units.data=NULL;

	player_init(&g->player, p);
	player_init(&g->opponent, f);

	g->round = 1;
	g->is_running = GAME_RUNNING;

	ai_init(&g->ai);

}

void game_create_players(Game *g, char *teams, Map *m) {

	exit_on_null(g,"Error in game_create_players, game is null");
	exit_on_null(teams, "Error in game_create_players, teams is null");
	exit_on_null(m,"Error in game_create_players, map is null");

	char team1[MAXLINE];
	char team2[MAXLINE];

	game_split_teams(teams,team1,team2);
	game_create_player(&(g->player),team1,m,TEAM_PLAYER);
	game_create_player(&(g->opponent),team2,m,TEAM_FOE);

}

void game_split_teams(char *teams, char *team1, char *team2) {

	exit_on_null(teams,"Error in game_parse_teams, teams is null");
	exit_on_null(team1, "Error in game_parse_teams, team1 is null");
	exit_on_null(team2, "Error in game_parse_teams, team2 is null");

	char *tt = teams;
	char *t1 = team1;
	char *t2 = team2;

	int i;
	for(i=0;i<MAXLINE;i++) {
		if(*teams==TEAM_DELIMITER)
			break;
		if(*teams==UNIT_DELIMITER) {
			*team1++=' ';
			*teams++;
		}else {
			*team1++=*teams++;
		}
	}
	*team1='\0';
	teams++;
	if(i>=MAXLINE)
		exit_on_error(-1,"Error in game_parse_teams, error in parsing team1");
	for(;i<MAXLINE;i++) {
		if(*teams==TEAM_END)
			break;
		if(*teams==UNIT_DELIMITER) {
			*team2++=' ';
			*teams++;
		}else {
			*team2++=*teams++;
		}
	}
	*team2='\0';
}

void game_create_player(Player* p, char *team, Map *m, char t) {

	exit_on_null(p, "Error in game_create_player, player is null");
	exit_on_null(team, "Error in game_create_player, team is null");
	exit_on_null(m, "Error in game_create_player, map is null");

	int type;

	char buf[10];
        int al = 0;
        int l = strlen(team);

	printf("team: %s\n", team);
	Point pos;

        while(al<l) {

                sscanf(team+al,"%s ",buf);
                type = atoi(buf);
		pos = player_rand_position(m,t);
		player_add_unit(p, type, pos.x, pos.y);
                al += strlen(buf)+1;
        }
}

Boolean game_is_running(Game *g) {

	exit_on_null(g,"Error in game_is_running, game is null");

	if(g->is_running == GAME_RUNNING)
		return BOOL_TRUE;

	return BOOL_FALSE;

}

void game_end_game(Game *g, Terminal *t) {

	terminal_add_msg(t,"The game is ended", TERMINAL_MSG_NORMAL);
	g->is_running = GAME_ENDED;

}

void game_next_round(Game *g, Terminal *t) {

	exit_on_null(g,"Error in game_helper_next_round, game is null");
	player_next_round(&g->player);
	player_next_round(&g->opponent);

	char msg[TERMINAL_LINE_LENGTH];
	sprintf(msg,"AI turn");	

	terminal_add_msg(t,msg,TERMINAL_MSG_NORMAL);

	g->ai.ai_turn = BOOL_TRUE;

}

void game_print_header(Game *g) {

	exit_on_null(g,"Error in game_helper_print_header, game is null");
	printf("Round: %d\n",g->round);

}

void game_free_game(Game *g) {
	
	exit_on_null(g,"Error in game_helper_free_game, game is nulll");
	player_free(&g->player);
	player_free(&g->opponent);

}

void game_post_units(Game_media *gm, Map *m) {

	exit_on_null(gm,"Error in game_post_unist, game media is null");
	exit_on_null(m,"Error in game_post_units, map is null");

	player_post_units(&(gm->mgame->player),m);
        player_post_units(&(gm->mgame->opponent),m);

}

void game_media_set_media(Game_media *media, Terminal *t, Map *m, Cursor *c, Info_screen *is, Menu_manager *mma, Game *g, Missile *gm) {

	media->mterm = t;
	media->mmap = m;
	media->mcursor = c;
	media->minfo = is;
	media->mmanager = mma;
	media->mmissile = gm;
	media->mgame = g;
	media->mftunit = NULL;

	unit_clear_weapon_range(&(media->mweapon_range));

}

void game_ai_finished_turn(Game *g, Terminal *t) {

	g->round++;

        char msg[TERMINAL_LINE_LENGTH];

        sprintf(msg,"Round %d begun, your turn",g->round);

	terminal_add_msg(t,msg,TERMINAL_MSG_NORMAL);
}

Boolean game_is_unit_friend(Game *g, Unit *u) {

        exit_on_null(u, "Error in game_is_unit_friend, unit is null");
	exit_on_null(g, "Error in game_is_unit_fiend, game is null");

        if(u->owner->pcolor == g->player.pcolor)
                return BOOL_TRUE;

        return BOOL_FALSE;

}

Boolean game_is_unit_foe(Game *g, Unit *u) {

        exit_on_null(u, "Error in game_is_unit_foe, unit is null");
	exit_on_null(g, "Error in game_isUnit_foe, game is null");
        if(u->owner->pcolor == g->opponent.pcolor)
                return BOOL_TRUE;

        return BOOL_FALSE;

}

Boolean game_check_end_conditions(Game_media *gm) {

	exit_on_null(gm,"Error in game_check_end_conditions, game media is null");

	Game *g = gm->mgame;

	int pus = vector_get_size(&(g->player.units));
	if(pus <= 0) {
		terminal_add_msg(gm->mterm,"You lost asshole!",TERMINAL_MSG_NORMAL);
		return BOOL_TRUE;
	}

	int ous = vector_get_size(&(g->opponent.units));
	if(ous <= 0) {
		terminal_add_msg(gm->mterm,"You win, congratulations!", TERMINAL_MSG_NORMAL);
		return  BOOL_TRUE;
	}
		
	return BOOL_FALSE;

}

void game_remove_destroyed_units(Game_media *gm, Map *m) {

	exit_on_null(gm,"Error in game_remove_destroyed_units, game media is null");

	player_remove_destroyed_units(&(gm->mgame->player), m);
	player_remove_destroyed_units(&(gm->mgame->opponent), m);

}
