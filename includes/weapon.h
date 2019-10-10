#ifndef WEAPON_H
#define WEAPON_H

#include "types.h"

#define MISSILE_FIRE_SLEEP 100000
#define WEAPON_UNIT_SIZE 1

#define WEAPON_TYPE_ADJUST 301
#define WEAPONS_SIZE 4

int weapon_missile_get_posx(Missile *m);
int weapon_missile_get_posy(Missile *m);

int  weapon_range(Weapon_type wt);
const char *weapon_name(Weapon_type wt);
int weapon_get_consumed_action_points(Weapon_type type);
int weapon_get_rounds(Weapon_type type);
int weapon_get_consumed_average_hit_points(Weapon_type type);
int weapon_get_consume_rounds(Weapon_type type);
int weapon_get_fire_sleep(Weapon_type type);
int weapon_get_counter(Weapon_type type);

void    weapon_missile_count_fire(Unit *u, Unit *f, Game_media *gm, Missile *m);
Boolean weapon_missile_check_end(Unit *u, Missile *m);
void    weapon_missile_update_position(Missile *m, Unit *u);

void weapon_set_init_position_for_missile(Missile *m, Unit *u, Unit *foe);

void weapon_update_position_machinegun(Missile* m);
void weapon_update_position_rocket(Missile *m);
void weapon_update_position_laser(Missile *m);

int weapon_get_damages_for_weapon(Weapon_type wt);
int weapon_get_damages_for_machinegun();
int weapon_get_damages_for_laser();
int weapon_get_damages_for_rocket();
int weapon_get_damages_for_big_rocket();

Boolean weapon_check_end_machinegun(Weapon_type wt, Missile *m);
Boolean weapon_check_end_rocket(Weapon_type wt, Missile *m);
Boolean weapon_check_end_laser(Weapon_type wt, Missile *m);

void weapon_post_laser_in_map(Missile *m, Map *map);
Boolean weapon_is_weapon_type(int type);
Boolean weapon_is_no_weapon_type(int type);

void weapon_set_end_missile(Unit *u, Missile *m);

void weapon_post_missile(Missile *missile, Map *map);

void weapon_calc_damages(Unit *u, Unit *f, Game_media *gm, Missile *m);

int weapon_adjust_type(Weapon_type type);

#endif //WEAPON_H
