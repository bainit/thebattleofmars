#include <unistd.h>

//#include <time.h>

#include "includes/types.h"

#include "includes/ai.h"
#include "includes/util.h"
#include "includes/player.h"
#include "includes/unit.h"
#include "includes/vector.h"
#include "includes/game.h"
#include "includes/map.h"
#include "includes/weapon.h"

void ai_reset(AI *ai) {

	exit_on_null(ai, "Error in ai_reset, ai is null");

	ai->actual_unit = 0;
	ai->ai_turn = BOOL_FALSE;

	ai->ai_sleep=AI_MOVE_SLEEP;

}

void ai_init(AI *ai) {

	exit_on_null(ai,"Error in ai_init, ai is null");
	ai_reset(ai);

}

Boolean ai_is_ai_turn(AI *ai) {

	exit_on_null(ai,"Error in ai_is_ai_turn, ai is null");

	return ai->ai_turn;

}

Boolean ai_do_move(Game *g, Game_media *gm) {

	exit_on_null(g, "Error in ai_do_move, game is null");
	exit_on_null(gm, "Error in ai_do_move, game_media is null");

	AI *ai = &g->ai;
	Player *opponent = &g->opponent;

	int sz = opponent->units.size;

	if(ai->actual_unit >= sz) {
		ai_reset(ai);
		unit_clear_weapon_range(&(gm->mweapon_range));
		usleep(200000);
		return BOOL_FALSE;
	}

	Unit *u = NULL;
	u = (Unit*)vector_get_data(&opponent->units, ai->actual_unit);
	exit_on_null(u,"Error in ai_do_move, unit from player.units is null");
	
	while(BOOL_TRUE) {

		if(u->isfiring) {
                	weapon_missile_count_fire(u,gm->mftunit,gm, gm->mmissile);
                	break;
        	}else if(unit_has_enough_action_points(u,UNIT_SINGLE_MOVE)) {
			ai_think(u,g,gm);
		}else {
			ai->actual_unit++;
			//usleep(200000);
			break;
		}
		
		if(map_is_unit_in_map_view(u,gm->mmap)) {
			usleep(ai->ai_sleep);
			break;
		}

	}
	return BOOL_TRUE;
}

void ai_change_weapon_if_necessary(Unit *u, Unit *cu, Game_media *gm) {

	exit_on_null(u,"Error in ai_change_weapon_if_necessary,  unit is null");
	exit_on_null(cu,"Error in ai_change_weapon_if_necessary, enemy closest unit is null");
	exit_on_null(gm, "Error in ai_change_weapon_if_necessary, game_media is null");

	int dist = distance(u->posx, u->posy, cu->posx, cu->posy);	
	int swrange = weapon_range(u->sweapon.wtype);
	int swcap = weapon_get_consumed_action_points(u->sweapon.wtype);

	int rounds = weapon_get_consume_rounds(u->sweapon.wtype);

	if(dist<=swrange && unit_has_enough_action_points(u,swcap) && unit_has_enough_rounds(u,rounds)){
		unit_use_second_weapon(u,gm);
	}
}

void ai_think(Unit *f, Game *g, Game_media *gm) {
	
	exit_on_null(f,"Error in ai_think, foe unit is null");
	exit_on_null(g,"Error in ai_think, game is null");
	exit_on_null(gm, "Error in ai_think, game_media is null");

        Unit *cu = unit_find_closest_foe_unit(f,gm,&(gm->mgame->player));
        exit_on_null(cu,"Error in unit_do_random_move, cant get closest foe unit");

	ai_change_weapon_if_necessary(f,cu,gm);	

	int cap = weapon_get_consumed_action_points(unit_get_actual_weapon(f));

	unit_calc_weapon_range(gm,f);

        if(!unit_is_destroyed(cu) && unit_is_in_weapon_range(gm,f,cu) && unit_has_enough_action_points(f,cap)) {

                gm->mftunit = cu;
                unit_fire(f,gm);
                gm->mgame->ai.ai_sleep = MISSILE_FIRE_SLEEP;
		return;
        }

	unit_do_random_move(f,gm);

}
