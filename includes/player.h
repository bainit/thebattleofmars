#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

void player_init(Player *p, Color_type pc);
void player_add_unit(Player *p, Unit_type t, int px, int py);
void player_post_units(Player *p, Map *m);
void player_next_round(Player *p);
Boolean player_is_same(Player *p1, Player *p2);
void player_remove_destroyed_units(Player *p, Map *m);
Point player_rand_position(Map *m, char t);

void  player_free(Player *p);

#endif //PLAYER_H
