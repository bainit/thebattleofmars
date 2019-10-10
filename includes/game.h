#ifndef GAME_H
#define GAME_H

#include "types.h"

#define GAME_OK 0

#define GAME_RUNNING 1
#define GAME_ENDED 0

#define GAMEERR_NULL_GAME -801
#define GAMEERR_SAME_COLORS -802

void game_play_game(char *teams);

void game_init_game(Game *g, Color_type p, Color_type f);
Boolean game_is_running(Game *g);
void game_create_players(Game *g, char *teams, Map *m);
void game_create_player(Player *p, char *team, Map *m, char t);
void game_split_teams(char *teams, char *t1, char *t2);
void game_end_game(Game *g, Terminal *t);
void game_free_game(Game *g);
void game_next_round(Game *g, Terminal *t);
void game_print_header(Game *g);
void game_post_units(Game_media *gm, Map *m);

void game_media_set_media(Game_media *media, Terminal *t, Map *m, Cursor *c, Info_screen *is, Menu_manager *mma,  Game *g, Missile *mm);

void game_ai_finished_turn(Game *g, Terminal *t);

Boolean game_is_unit_friend(Game *g, Unit *u);
Boolean game_is_unit_foe(Game *g, Unit *u);

Boolean game_check_end_conditions(Game_media *g);
void game_remove_destroyed_units(Game_media *gm, Map *m);

#endif //GAME_H
