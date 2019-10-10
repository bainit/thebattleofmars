#ifndef UNIT_H
#define UNIT_H

#include "types.h"

#define UNIT_OK 0

#define UERR_TYPE_OUT_OF_RANGE -20
#define UERR_POS_OUT_OF_RANGE -21
#define UERR_LINE_OUT_OF_RANGE -22
#define UERR_HEALTH_REDUCE_OUT_OF_RANGE -23

#define UNITS_SIZE 4
#define UNIT_MAX_LINE_SIZE 250
#define UNIT_MAX_HEALTH_REDUCE 100
#define UNIT_SINGLE_MOVE 1

#define UNIT_TYPE_ADJUST 201

int  unit_set_values(Unit *u, Unit_type t, int px, int py, Player *owner);
void unit_set_weapon(Weapon_type wt, Weapon_slot *slot);
int  unit_get_weapon_range(Unit *u);

Weapon_type unit_get_weapon_type_for_unit_type(Unit_type ut, int wt);

int unit_get_weapon_rounds(Unit *u);
void unit_reduce_rounds(Unit *u);

const char *unit_get_type_name(Unit *u);
const char *unit_get_weapon_name(Unit *u); 

void unit_use_first_weapon(Unit *u, Game_media *m);
void unit_use_second_weapon(Unit *u, Game_media *m);

Weapon_type unit_get_actual_weapon(Unit *u);
Weapon_type unit_get_not_actual_weapon(Unit *u);
void unit_reduce_action_points(Unit *u, int val);

void unit_move_in_direction(Unit *u, Game_media *gm, char direction);

void unit_to_string(Unit *u, char *line, int lsize, Game_media *gm);
void unit_friend_to_string(Unit *u, char *line, int lsize);
void unit_foe_to_string(Unit *u, char *line, int lsize);

void unit_msg_to_infoscreen(Unit *u, Game_media *gm); 
void unit_aimed_unit_to_infoscreen(Unit *f, Game_media *gm);
void unit_basic_msg_to_infoscreen(Unit *u, Game_media *gm);
void unit_weapon_msg_to_infoscreen(Unit *u, Game_media *gm);
void unit_next_round(Unit *u);

int  unit_may_movex(Unit *u, Map *m, Terminal *t);
int  unit_may_movey(Unit *u, Map *m, Terminal *t);

void unit_move_right(Unit *u, Map *m, Cursor *c, Terminal *t);
void unit_move_left(Unit *u, Map *m, Cursor *c, Terminal *t);
void unit_move_up(Unit *u,Map *m, Cursor *c, Terminal *t);
void unit_move_down(Unit *u, Map *m, Cursor *c, Terminal *t);

void unit_check_posx(Unit *u, Map *m);
void unit_check_posy(Unit *u,Map *m);

Color_type unit_get_fcolor(Unit *u);
Color_type unit_get_bcolor(Unit *u);

int  unit_adjust_type(Unit_type type);
int  unit_weapon_adjust(Weapon_type);

Boolean unit_roughly_range(Unit *u, int x1, int y1, int x2, int y2, int range);
Boolean unit_range(Unit *u, int dist, int range);

Boolean  unit_cast_ray(Unit *u, int px, int py, Vector *obs);

void 	unit_fire(Unit *u, Game_media *gm);
Boolean unit_is_firing(Unit *u);

Boolean unit_has_enough_action_points(Unit *u, int ap);
Boolean unit_has_enough_rounds(Unit *u, int r);
Boolean unit_is_no_unit_type(int type);
Boolean unit_is_using_first_weapon(Unit *u);
Boolean unit_is_using_second_weapon(Unit *u);

void unit_do_random_move(Unit *u, Game_media *gm);
void unit_clear_weapon_range(Unit_weapon_range *uwr);
void unit_calc_weapon_range(Game_media *gm, Unit *u);
Boolean unit_is_in_weapon_range(Game_media *mg, Unit *u, Unit *eu);
char unit_get_weapon_range_char(Unit_weapon_range *wr, int x, int y, int cx, int cy);

Unit* unit_find_closest_foe_unit(Unit *u, Game_media *gm, Player *foe);
void unit_reduce_health(Unit *u, int rh);

Color_type unit_get_color_for_health(double val);
void unit_get_health_bar(double val, char *msg);

void unit_get_team_string(Unit *u, char *team, char t);
void unit_get_position_string(Unit *u, char *pos);
void unit_get_health_string(Unit *u, char *health);
void unit_get_foe_health_string(Unit *u, char *health);
void unit_get_weapon_string(Unit *u, int w, char *weapon);

Boolean unit_has_two_weapons(Unit *u);

Boolean unit_is_destroyed(Unit *u);

#endif //UNIT_H
