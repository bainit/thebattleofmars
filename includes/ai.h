#ifndef AI_H
#define AI_H

#include "types.h"

#define AI_OK 0

#define AI_MOVE_SLEEP 300000

void 		ai_init(AI *ai);
Boolean		ai_next_move(AI *ai);
Boolean		ai_is_ai_turn(AI *ai);
Boolean		ai_do_move(Game *g, Game_media *gm);
void 		ai_change_weapon_if_necessary(Unit *u, Unit *cu, Game_media *gm);

void		ai_think(Unit *u, Game* g, Game_media *gm);

#endif //AI_H
